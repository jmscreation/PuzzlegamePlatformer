#include <cstring>
#include "arbgrid.h"

template<class T> ArbGrid<T>::ArbGrid(int sz): size(sz) {
    size = size<1 ? 1 : size>8192 ? 8192 : size; // limit size; technically, true working max is 46340, but 8192 should ever be more than sufficient

    int sq = size*size;

    grid = new Cell*[sq];
    memset(grid,0,sq * sizeof(Cell*));
}

template<class T> ArbGrid<T>::~ArbGrid() {
    clear();

    delete[] grid;
}

template<class T> void ArbGrid<T>::cellAdd(int x,int y,T value) {
    Cell *c = getCell(x,y);

    Item* item = c->start;
    Item** ptr = &c->start; // pointer to be updated (final NULL pointer in linkedlist)

    while(item != NULL) {
        if(item->item == value) return; // item is already in this cell; exit
        ptr = &item->next;
        item = item->next;
    }
    // add to list
    *ptr = item = new Item;
    item->item = value;
    item->next = NULL;
}

template<class T> void ArbGrid<T>::cellRemove(int x,int y,T value) {
    Cell **cptr;
    Cell *c = getExistingCell(x,y,&cptr);

    if(c == NULL) return; // there is no cell here

    Item* item = c->start;
    Item** ptr = &c->start;
    while(item != NULL) {
        if(item->item == value) {
            if(iterator == item)
                iterator = item->next;
            *ptr = item->next; // join previous item to skip this item
            delete item;
            break;
        }
        ptr = &item->next;
        item = item->next;
    }

    if(c->start == NULL) // deleted the last item in this cell; drop the cell
        removeCell(c,cptr);
}

template<class T> void ArbGrid<T>::cellClear(int x,int y) {
    Cell **cptr;
    Cell *c = getExistingCell(x,y,&cptr);

    if(c == NULL) return; // there is no cell here

    Item* item = c->start, *pitem;

    while(item != NULL) {
        if(iterator == item)
            iterator = NULL; // cell will be empty; NULL out iterator
        pitem = item;
        item = item->next;
        delete pitem;
    }

    removeCell(c,cptr);
}

template<class T> void ArbGrid<T>::clear() {
    iterator = NULL;

    for(int i=0,sq=size*size;i<sq;i++) { // destroy each cell-set
        Cell *curcell, *pcell;
        Item *curitem, *pitem;

        curcell = grid[i];
        grid[i] = NULL;

        while(curcell != NULL) { // destroy each cell in a cell-set
            curitem = curcell->start;
            while(curitem != NULL) { // destroy each item reference in a cell
                pitem = curitem;
                curitem = curitem->next;
                delete pitem;
            }

            pcell = curcell;
            curcell = curcell->next;
            delete pcell;
        }
    }
}

template<class T> bool ArbGrid<T>::inCell(int x,int y,T value) {
    if(!hasCell(x,y)) return false;
    Cell *c = getCell(x,y);

    Item* item = c->ctart;
    while(item != NULL) {
        if(item->item == value) return true;
        item = item->next;
    }

    return false;
}

template<class T> bool ArbGrid<T>::hasCell(int x,int y) {
    return getExistingCell(x,y) == NULL;
}

template<class T> void ArbGrid<T>::getBox(int *left,int *right,int *top,int *bottom) {
    int l=0,r=0,t=0,b=0;

    Cell* curcell;
    for(int i=0,sq=size*size;i<sq;i++) {
        curcell = grid[i];
        while(curcell != NULL) {
            if(curcell->x < l) l = curcell->x;
            if(curcell->x > r) r = curcell->x;
            if(curcell->y < t) t = curcell->y;
            if(curcell->y > b) b = curcell->y;

            curcell = curcell->next;
        }
    }

    if(left != NULL) *left = l;
    if(right != NULL) *right = r;
    if(top != NULL) *top = t;
    if(bottom != NULL) *bottom = b;
}

template<class T> bool ArbGrid<T>::iterateCell(int x,int y,ArbGrid<T>::func_celliterator iter,void* data) {
    Cell *c = getExistingCell(x,y);

    Item *item = c->start;

    while(item != NULL) {
        if(!iter(item->item,data))
            return false;
        item = item->next;
    }

    return true;
}

template<class T> bool ArbGrid<T>::iterateAll(ArbGrid<T>::func_celliterator iter,void* data) {
    Cell *curcell;
    Item *curitem;
    for(int i=0,sq=size*size;i<sq;i++) { // iterate each cell-set on the grid
        curcell = grid[i];
        while(curcell != NULL) { // iterate each cell in a cell-set
            curitem = curcell->start;
            while(curitem != NULL) { // iterate each item in a cell
                if(!iter(curitem->item,data)) return false;
                curitem = curitem->next;
            }
            curcell = curcell->next;
        }
    }
    return true;
}

template<class T> void ArbGrid<T>::iterateCellBegin(int x,int y) {
    Cell* c = getExistingCell(x,y);
    iterator = c == NULL ? NULL : c->start;
}

template<class T> T ArbGrid<T>::iterateCellNext() {
    if(iterator == NULL)
        return T();
    T& cur = iterator->item;
    iterator = iterator->next;
    return cur;
}

template<class T> typename ArbGrid<T>::Cell* ArbGrid<T>::getCell(int x,int y,Cell*** ptr_out) {
    unsigned int ind = ((unsigned int)x)%size + ((unsigned int)y)%size * size;

    Cell* c = grid[ind];
    Cell** ptr = grid+ind; // pointer to be updated (if cell doesn't exist)

    while(c != NULL) { // search cells
        if(c->x == x && c->y == y) { // found cell
            if(ptr_out != NULL)
                *ptr_out = ptr;
            return c;
        }
        ptr = &c->next;
        c = c->next;
    }

    // didn't find cell; create a new one, and join last node to new cell (via ptr)
    *ptr = c = new Cell;
    if(ptr_out != NULL)
        *ptr_out = ptr;
    c->x = x; c->y = y;
    c->start = NULL;
    c->next = NULL;

    return c;
}

template<class T> typename ArbGrid<T>::Cell* ArbGrid<T>::getExistingCell(int x,int y,Cell*** ptr_out) {
    unsigned int ind = ((unsigned int)x)%size + ((unsigned int)y)%size * size;

    Cell* c = grid[ind];
    Cell** ptr = grid+ind;

    while(c != NULL) { // search cells
        if(c->x == x && c->y == y) {
            if(ptr_out != NULL)
                *ptr_out = ptr;
            return c;
        }
        ptr = &c->next;
        c = c->next;
    }

    return NULL;
}

template<class T> void ArbGrid<T>::removeCell(Cell* c,Cell** ptr) {
    *ptr = c->next;
    delete c;
}

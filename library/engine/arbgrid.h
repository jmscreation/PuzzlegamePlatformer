#ifndef __ARBGRID_H__
#define __ARBGRID_H__

/*
    ArbGrid<class>:
    Arbitrary Grid: A 2-dimensional grid of containers; maps any int x,y to an array of pointers to any choice type

    -- Designed for mapping objects to arbitrary cells in a world for fast local drawing and local collision testing --
*/

template<class T> class ArbGrid {
public:
    typedef bool (*func_celliterator)(T itm,void* args);

    ArbGrid(int sz=32);
    virtual ~ArbGrid();

    void cellAdd(int x,int y,T itm);
    void cellRemove(int x,int y,T itm);
    void cellClear(int x,int y);
    void clear();
    bool inCell(int x,int y,T itm);
    bool hasCell(int x,int y);
    void getBox(int *left=NULL,int *right=NULL,int *top=NULL,int *bottom=NULL);

    bool iterateCell(int x,int y,func_celliterator iter,void* data=NULL);
    bool iterateAll(func_celliterator iter,void* data=NULL);

    void iterateCellBegin(int x,int y);
    T iterateCellNext();

private:
    int size;

    struct Item {
        T item;
        ArbGrid::Item* next;
    } *iterator;
    struct Cell {
        int x, y;
        ArbGrid::Item* start;
        ArbGrid::Cell* next;
    } **grid;

    Cell *getCell(int x,int y,Cell*** ptr_out=NULL);
    Cell *getExistingCell(int x,int y,Cell*** ptr_out=NULL);
    void removeCell(Cell* c,Cell** ptr);
};

#include "arbgrid.tpp"

#endif // __ARBGRID_H__

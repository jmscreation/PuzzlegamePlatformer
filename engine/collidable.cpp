#include "engine.h"
using namespace std;

namespace Engine {

    ArbGrid<BaseCollidable*> BaseCollidable::grid(ARB_GRID_CELLS);
    int BaseCollidable::iterCell = -1;
    int BaseCollidable::iterX = 0;
    int BaseCollidable::iterY = 0;

    static int cellOffsets[] = {0,0, -1,-1, 0,-1, 1,-1, -1,0, 1,0, -1,1, 0,1, 1,1};

    BaseCollidable::BaseCollidable(float x,float y): mask(NULL) {
        int cx = int(floor(x / COLLIDABLE_REGION_SIZE)),
            cy = int(floor(y / COLLIDABLE_REGION_SIZE));

        grid.cellAdd(cx,cy,this);
    }

    BaseCollidable::~BaseCollidable() {
        if(mask != NULL)
            delete mask;
    }

    void BaseCollidable::beginPosition(float x,float y) {
        iterX = int(floor(x / COLLIDABLE_REGION_SIZE));
        iterY = int(floor(y / COLLIDABLE_REGION_SIZE));
        iterCell = 0;
        grid.iterateCellBegin(iterX+cellOffsets[0], iterY+cellOffsets[1]);
    }

    BaseCollidable* BaseCollidable::iteratePosition() {
        if(iterCell == -1) return NULL;
        BaseCollidable* c;
        do {
            c = grid.iterateCellNext();
            if(c == NULL) {
                if(++iterCell==9) {
                    iterCell = -1;
                    return NULL;
                }
                grid.iterateCellBegin(iterX+cellOffsets[iterCell*2], iterY+cellOffsets[iterCell*2+1]);
            }
        } while(c == NULL);
        return c;
    }

    void BaseCollidable::beginCollision() {
        beginPosition(x(),y());
    }

    BaseCollidable* BaseCollidable::iterateCollidable() {
        if(iterCell == -1) return NULL;
        BaseCollidable* c;
        do {
            c = grid.iterateCellNext();
            if(c == NULL) {
                if(++iterCell==9) {
                    iterCell = -1;
                    return NULL;
                }
                grid.iterateCellBegin(iterX+cellOffsets[iterCell*2], iterY+cellOffsets[iterCell*2+1]);
            }
        } while(c == NULL || c == this);
        return c;
    }

    void BaseCollidable::move(float ox,float oy,float nx,float ny) {
        int ocx = int(floor(ox / COLLIDABLE_REGION_SIZE)),
            ocy = int(floor(oy / COLLIDABLE_REGION_SIZE)),
            ncx = int(floor(nx / COLLIDABLE_REGION_SIZE)),
            ncy = int(floor(ny / COLLIDABLE_REGION_SIZE));
        if(mask != NULL) {
            mask->x = nx;
            mask->y = ny;
        }
        if(ocx != ncx || ocy != ncy) {
            grid.cellRemove(ocx,ocy,this);
            grid.cellAdd(ncx,ncy,this);
        }
    }

    void BaseCollidable::remove(float x,float y) {
        int cx = int(floor(x / COLLIDABLE_REGION_SIZE)),
            cy = int(floor(y / COLLIDABLE_REGION_SIZE));

        grid.cellRemove(cx,cy,this);
    }

    Collidable::Collidable(float x,float y):
        BaseCollidable(x,y) {}

    Collidable::~Collidable() {}

    void Collidable::move(float dx,float dy) {
        BaseCollidable::move(_x,_y,_x+dx,_y+dy);
        _x += dx; _y += dy;
    }

    void Collidable::x(float x) {
        BaseCollidable::move(_x,_y,x,_y);
        _x = x;
    }

    void Collidable::y(float y) {
        BaseCollidable::move(_x,_y,_x,y);
        _y = y;
    }

    void Collidable::setPosition(float x,float y) {
        BaseCollidable::move(_x,_y,x,y);
        _x = x; _y = y;
    }

}

#include "engine.h"

namespace Engine {

    ArbGrid<BaseDrawable*> BaseDrawable::grid(ARB_GRID_CELLS);
    BaseDrawable::BaseDrawable(Animation* ani,float x,float y,float d): _depth(d), sprite(ani) {
        int cx = int(floor(x / DRAWABLE_REGION_SIZE)),
            cy = int(floor(y / DRAWABLE_REGION_SIZE));

        grid.cellAdd(cx,cy,this);

        // App::current().viewState.needsUpdate = true;
    }

    BaseDrawable::~BaseDrawable() {}

    void BaseDrawable::depth(float d) {
        if(_depth != d) {
            _depth = d;
            // App::current().viewState.needsUpdate = true;
        }
    }

    void BaseDrawable::draw(sf::RenderWindow& win,sf::Time& delta) {
        win.draw(sprite);
    }

    void BaseDrawable::move(float ox,float oy,float nx,float ny) {
        int ocx = int(floor(ox / DRAWABLE_REGION_SIZE)),
            ocy = int(floor(oy / DRAWABLE_REGION_SIZE)),
            ncx = int(floor(nx / DRAWABLE_REGION_SIZE)),
            ncy = int(floor(ny / DRAWABLE_REGION_SIZE));
        if(ocx != ncx || ocy != ncy) {
            grid.cellRemove(ocx,ocy,this);
            grid.cellAdd(ncx,ncy,this);

            // App::current().viewState.needsUpdate = true;
        }
    }

    void BaseDrawable::remove(float x,float y) {
        int cx = int(floor(x / DRAWABLE_REGION_SIZE)),
            cy = int(floor(y / DRAWABLE_REGION_SIZE));

        grid.cellRemove(cx,cy,this);

        // App::current().viewState.needsUpdate = true;
    }

    Drawable::Drawable(Animation* ani,float x,float y,float depth):
        BaseDrawable(ani,x,y,depth), _x(x), _y(y) {
        sprite.setPosition(_x,_y);
    }

    Drawable::~Drawable() {
        BaseDrawable::remove(_x,_y);
    }

    void Drawable::move(float dx,float dy) {
        BaseDrawable::move(_x,_y,_x+dx,_y+dy);
        _x += dx; _y += dy;
        sprite.setPosition(_x,_y);
    }

    void Drawable::setPosition(float x,float y) {
        BaseDrawable::move(_x,_y,x,y);
        _x = x; _y = y;
        sprite.setPosition(_x,_y);
    }

    void Drawable::x(float x) {
        BaseDrawable::move(_x,_y,x,_y);
        _x = x;
        sprite.setPosition(_x,_y);
    }

    void Drawable::y(float y) {
        BaseDrawable::move(_x,_y,_x,y);
        _y = y;
        sprite.setPosition(_x,_y);
    }

    SortList<WorldDrawable*> WorldDrawable::list;

    WorldDrawable::WorldDrawable(float d): _depth(d) {
        list.add(_depth,this);
    }
    WorldDrawable::~WorldDrawable() {
        auto node = list.find(this);
        list.rem(node);
    }

    void WorldDrawable::depth(float d) {
        if(d != _depth) {
            _depth = d;
            // pop out, update t, and re-insert node
            auto node = list.find(this);
            list.pop(node);
            node->t = _depth;
            list.add(node);
        }
    }

    SortList<ScreenDrawable*> ScreenDrawable::list;

    ScreenDrawable::ScreenDrawable(View* vw,float d) {
        if(vw == NULL)
            _view = new View();
        else
            _view = vw;
        _depth = d;
        list.add(_depth,this);
    }
    ScreenDrawable::~ScreenDrawable() {
        auto node = list.find(this);
        list.rem(node);
    }

    void ScreenDrawable::depth(float d) {
        if(d != _depth) {
            _depth = d;
            // pop out, update t, and re-insert node
            auto node = list.find(this);
            list.pop(node);
            node->t = _depth;
            list.add(node);
        }
    }

}

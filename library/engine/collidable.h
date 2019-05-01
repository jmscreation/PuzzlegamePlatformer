#ifndef __ENGINE_COLLIDABLE_H__
#define __ENGINE_COLLIDABLE_H__

#ifndef COLLIDABLE_REGION_SIZE
#define COLLIDABLE_REGION_SIZE 512
#endif

namespace Engine {

    class Mask;

    class BaseCollidable {
        static ArbGrid<BaseCollidable*> grid;
        static int iterCell, iterX, iterY;
    public:
        BaseCollidable(float x,float y);
        virtual ~BaseCollidable();

        static void beginPosition(float x,float y);
        static BaseCollidable* iteratePosition();
        static void beginAll();
        static BaseCollidable* iterateAll();

        virtual int type() = 0;
        virtual void move(float dx,float dy) = 0;
        virtual void setPosition(float x,float y) = 0;
        virtual void x(float x) = 0;
        virtual void y(float y) = 0;
        virtual float x() = 0;
        virtual float y() = 0;
        void beginCollision();
        BaseCollidable* iterateCollidable();

        Mask* mask;

    private:
        void move(float ox,float oy,float nx,float ny);
        void remove(float x,float y);

        friend class App;
        friend class Collidable;
        friend class GameObject;
    };

    class Collidable: public BaseCollidable {
    public:
        Collidable(float x=0,float y=0);
        virtual ~Collidable();

        void move(float dx,float dy);
        void setPosition(float x,float y);
        void x(float x);
        void y(float y);
        float x() { return _x; }
        float y() { return _y; }
        void beginCollision();
        BaseCollidable* iterateCollidable();

        virtual int type() = 0;

    private:
        float _x,_y;
    };

}

#endif // __ENGINE_COLLIDABLE_H__
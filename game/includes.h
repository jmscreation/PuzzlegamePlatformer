#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#include "resource.h"

#define PATH_SND_ERROR OGG_ERROR
#ifdef LOAD_FROM_FILE
    #define PATH_PLAYER "images/player.png"
    #define PATH_ITEM "images/item.png"
    #define PATH_WALL "images/wall.jpg"
    #define PATH_BOX "images/box.png"
    #define PATH_BACKGROUND "images/background.png"
    #define PATH_SND_MUSIC "audio/music.ogg"
    #define PATH_SND_ITEM "audio/item.ogg"
    #define PATH_SND_JUMP "audio/jump.ogg"
    #define PATH_SND_SWOOSH "audio/swoosh.ogg"
#else
    #define PATH_PLAYER PNG_PLAYER
    #define PATH_ITEM PNG_ITEM
    #define PATH_WALL PNG_WALL
    #define PATH_BOX PNG_BOX
    #define PATH_BACKGROUND PNG_BACKGROUND
    #define PATH_SND_MUSIC OGG_MUSIC
    #define PATH_SND_ITEM OGG_ITEM
    #define PATH_SND_JUMP OGG_JUMP
    #define PATH_SND_SWOOSH OGG_SWOOSH
#endif

#include <fstream>
#include "../engine/engine.h"

#include "puzzlegame.h"

#include "background.h"
#include "playerobj.h"
#include "wallobj.h"
#include "itemobj.h"
#include "boxobj.h"

#endif // __GAME_INCLUDES_H__

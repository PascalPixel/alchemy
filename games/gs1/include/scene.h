#ifndef ALCHEMY_SCENE_H
#define ALCHEMY_SCENE_H

#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct Ent {
    s32 f00;
    u8 f04;
    u8 f05;
    u16 f06:9;
    u16 f07:7;
    s32 f08;
};

struct Cam {
    u8 pad[228];
    s32 x;
    s32 y;
};

#endif

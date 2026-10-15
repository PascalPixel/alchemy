#ifndef ALCHEMY_ALTIN_H
#define ALCHEMY_ALTIN_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct AngledSlot
{
    u8  head[6];
    u16 angle;
};

struct SceneMotion
{
    u8  unk00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 ground;
    s32 scaleX;
    s32 scaleY;
    u8  unk20[8];
    s32 velocity;
    u8  unk2c[47];
    u8  state;
    u8  unk5c[8];
    s16 timer;
    s16 delay;
    s32 active;
    void (*callback)(struct SceneMotion *);
};

struct Actor
{
    s32 f00;
    s32 f04;
    s32 f08;
};

struct SpanActor
{
    u8  head[18];
    u8  gap18[6];
    s32 span;
    s32 reach;
    u8  gap32[48];
    u8 *attached;
    u8  gap84;
    u8  state;
};

#endif

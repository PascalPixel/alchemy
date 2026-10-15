#include "edition.h"
#ifndef TOLBI_H
#define TOLBI_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
extern u8 gWindowWork[];

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

#define SCENE_OBJECT_ID (*(s32 *)((u8 *)gEventWork + 0x1f4))

struct TolbiSceneWork
{
#if EDITION_INTERNATIONAL
    u8 unk00[0xea4];
#else
    u8 unk00[0xf34];
#endif
    u8 widenProximity;
};

struct SceneSprite
{
    u8 unk00[0x15];
    u8 unk15Low : 2;
    u8 priority15 : 2;
    u8 unk15High : 4;
};

struct Actor
{
    u8 pad00[0x23];
    u8 field23;
    u8 pad24[0x31];
    u8 field55;
};

#include "types.h"

struct SceneActor
{
    u8  unk00[6];
    u16 facing;
    s32 x, y, z;
    u8  unk14[71];
    u8  active;
    u8  unk5c[8];
    u16 proximityFlags;
};

#endif

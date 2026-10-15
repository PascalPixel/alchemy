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

struct TorebiSceneWork
{
#if EDITION_INTERNATIONAL
	u8 unknown_00[0xea4];
#else
	u8 unknown_00[0xf34];
#endif
	u8 widen_proximity;
};

struct SceneSprite
{
	u8 unknown_00[0x15];
	u8 unknown_15_low : 2;
	u8 priority_15 : 2;
	u8 unknown_15_high : 4;
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
	u8	unk_00[6];
	u16 facing;
	s32 x, y, z;
	u8	unk_14[71];
	u8	active;
	u8	unk_5c[8];
	u16 proximity_flags;
};

#endif

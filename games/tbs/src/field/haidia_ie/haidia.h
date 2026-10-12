#include "resource.h"
#ifndef HAIDIA_H
#define HAIDIA_H

#include "types.h"
#include "vram_block.h"
#include "callback_scheduler.h"

#define NULL ((void *)0)

#include "field_event.h"
#include "field_scene.h"
#include "facing_object.h"
#include "field_effect.h"

#include "battle_effect_runtime.h"

struct Obj
{
	u8	pad00[6];
	u16 f06;
	u8	pad08[0x30];
	s32 f38;
	s32 f3c;
	s32 f40;
};

struct VerticalEffectAnchor
{
	u8	pad00[8];
	s32 x;
	u8	pad0c[4];
	s32 z;
};

struct SceneVerticalEffect
{
	u8							 pad00[8];
	s32							 x;
	s32							 y;
	s32							 z;
	u8							 pad14[4];
	s32							 amplitude_x;
	s32							 amplitude_y;
	u8							 pad20[0x44];
	u16							 frame;
	u8							 pad66[2];
	struct VerticalEffectAnchor *anchor;
};

struct PairDetail
{
	u8 unknown_00[22];
	u8 field_16;
};

struct PairSprite
{
	struct FieldSprite sprite;
	struct PairDetail *detail;
};

union PairObject
{
	union FieldObject object;
	s32				  words[28];
	struct
	{
		u8				  unknown_00[0x68];
		union PairObject *parent;
	} link;
};

struct SceneVerticalEffect;

struct PairWork
{
	u8	unknown_00[70];
	u16 vram_block;
};

struct WorldMapOam
{
	u8	unknown_00[4];
	u16 attr0;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
};

extern struct PairWork *gEffectWork;

s32	 Math_RemainderUnsigned();
void Engine_ActorSetSpriteFlags();

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);

LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);

#endif

#ifndef ALCHEMY_ANIMSPR_H
#define ALCHEMY_ANIMSPR_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

struct AnimationEntry
{
	s16	   anim_id;
	u16	   timer;
	u8	   kind;
	u8	   param;
	u8	   priority;
	u8	   mode;
	void **frames;
	u8	 **scripts;
	u8	  *script;
	u8	   pos;
	u8	   step;
	u8	   frame;
	u8	   frame_base;
};

struct ResourceSlot
{
	s32	  tag;
	void *buffer;
};

struct ObjectSystemWork
{
	u16					used;
	u8					unknown_02[2];
	s16					suspended;
	u8					edge;
	u8					fill;
	u8					unknown_08[0x10];
	s32					count;
	struct ResourceSlot slots[8];
};

LAYOUT_OFFSET_GUARD(ObjectSystemWork_Suspended, struct ObjectSystemWork, suspended, 4);
LAYOUT_OFFSET_GUARD(ObjectSystemWork_Count, struct ObjectSystemWork, count, 0x18);
LAYOUT_OFFSET_GUARD(ObjectSystemWork_Slots, struct ObjectSystemWork, slots, 0x1c);
LAYOUT_SIZE_GUARD(ObjectSystemWork_Size, struct ObjectSystemWork, 0x5c);

struct AnimationPosition
{
	s32 x;
	s32 y;
	s32 z;
	s32 ground;
};

LAYOUT_SIZE_GUARD(AnimationPosition_Size, struct AnimationPosition, 0x10);
LAYOUT_OFFSET_GUARD(AnimationPosition_Ground, struct AnimationPosition, ground, 0x0c);

#if GAME_TLA
void Map_BuildProbeRing(s32 x, s32 z, struct AnimationPosition *points);
#endif

struct AnimationSpritePart
{
	u8	unknown_00[4];
	u16 y : 8;
	u16 affine : 2;
	u16 object_mode : 2;
	u16 mosaic : 1;
	u16 full_color : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
	u16 tile : 10;
	u16 priority : 2;
	u16 palette : 4;
	u8	unknown_0a[2];
};

struct AnimationObject
{
	struct AnimationSpritePart part[2];
	s32						   scale;
	u8						   slot;
	u8						   display_flags;
	u16						   rotation;
	u8						   width;
	u8						   height;
	s8						   offset_x;
	s8						   offset_y;
	u8						   last_no;
	u8						   dirty;
	u8						   flags;
	u8						   count;
	struct AnimationEntry	  *entries[4];
};

LAYOUT_SIZE_GUARD(AnimationEntry_Size, struct AnimationEntry, 0x18);
LAYOUT_SIZE_GUARD(AnimationSpritePart_Size, struct AnimationSpritePart, 0x0c);
LAYOUT_OFFSET_GUARD(AnimationObject_Scale, struct AnimationObject, scale, 0x18);
LAYOUT_OFFSET_GUARD(AnimationObject_Rotation, struct AnimationObject, rotation, 0x1e);
LAYOUT_OFFSET_GUARD(AnimationObject_Width, struct AnimationObject, width, 0x20);
LAYOUT_SIZE_GUARD(AnimationObject_Size, struct AnimationObject, 0x38);
LAYOUT_OFFSET_GUARD(AnimationObject_Entries, struct AnimationObject, entries, 0x28);

#endif

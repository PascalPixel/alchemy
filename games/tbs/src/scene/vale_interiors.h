#ifndef GUARD_VALE_INTERIORS_H
#define GUARD_VALE_INTERIORS_H

#include "types.h"

#define NULL ((void *)0)

#include "facing_object.h"
#include "field_event.h"
#include "field_scene.h"

enum ValeHouseFlag
{
	FLAG_GOT_FAREWELL_HERB = 0x81b
};

enum
{
	ITEM_HERB = 180
};

typedef struct
{
	u8	pad_to_position[8];
	s32 x;
	s32 y;
	u8	pad_to_scale[8];
	s32 scale_x;
	s32 scale_y;
	u8	pad_to_velocity[0x44];
	s16 velocity_x;
	s16 velocity_y;
} ScaledMotion;

union SceneActor
{
	s32 words[26];
	u16 halfwords[52];
};

union SceneField
{
	u16 value;
	u8	bytes[2];
} __attribute__((packed, aligned(2)));

extern struct SceneEvent gValeHouseEvents[];
extern struct SceneEvent gValeHouseEventsAfterLeaving[];
extern struct SceneEvent gValeHouseReturnEvents[];

#endif

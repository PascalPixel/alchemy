#ifndef NIWA_H
#define NIWA_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"

#define NULL ((void *)0)

#include "facing_object.h"

enum
{
	ITEM_NUT = 181
};

struct SceneHandle_0200090c
{
	u8	unknown_00[30];
	s16 field1e;
};

struct SceneEntity_0200090c
{
	u8							 unknown_00[8];
	s32							 x;
	s32							 y;
	u8							 unknown_10[0x20];
	s32							 phase;
	u8							 unknown_34[4];
	s32							 origin_x;
	s32							 origin_y;
	u8							 unknown_40[0x10];
	struct SceneHandle_0200090c *handle;
};

typedef struct OrbitingSceneObjectSprite
{
	u8 padding_00[5];
	u8 flags_05_low : 5;
	u8 flags_05_bit_5 : 1;
	u8 flags_05_high : 2;
	u8 padding_06[3];
	u8 flags_09_low : 2;
	u8 flags_09_mode : 2;
	u8 flags_09_high : 4;
	u8 padding_0a[18];
	u8 palette;
	u8 padding_1d[10];
	u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject
{
	u8						   padding_00[8];
	s32						   x;
	s32						   y;
	u8						   padding_10[19];
	u8						   flags_23;
	u8						   padding_24[12];
	s32						   orbit_angle;
	u8						   padding_34[4];
	s32						   orbit_center_x;
	s32						   orbit_center_y;
	u8						   padding_40[16];
	OrbitingSceneObjectSprite *sprite;
	u8						   padding_54;
	u8						   mode;
	u8						   state;
	u8						   padding_57[5];
	u8						   active;
	u8						   padding_5d[4];
	u8						   visible;
	u8						   padding_62[10];
	u32						   callback;
} OrbitingSceneObject;

#endif

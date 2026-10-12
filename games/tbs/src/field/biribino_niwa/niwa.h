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

#endif

#ifndef IMIRU_FUCHIN_H
#define IMIRU_FUCHIN_H

#include "types.h"
#include "keystate.h"
#include "field_event.h"
#include "field_scene.h"

enum
{

	ITEM_DRAGONS_EYE = 230
};

#define NULL ((void *)0)

extern void *gWorkSlot[];

struct TrackingWork
{
	u8				   unknown_00[24];
	struct FieldActor *actor;
};

extern s16 gImiruFuchinKeyHeadings[];
extern s16 gImiruFuchinHeadings[];
extern s32 gImiruFuchinDragonsEye;
extern u32 gImiruFuchinExits[];

#endif

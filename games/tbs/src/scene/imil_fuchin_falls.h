#ifndef IMIL_FUCHIN_FALLS_H
#define IMIL_FUCHIN_FALLS_H

#include "types.h"
#include "keystate.h"
#include "field_event.h"
#include "field_scene.h"

enum
{

	ITEM_DRAGONS_EYE = 230
};

extern void *gWorkSlot[];

struct TrackingWork
{
	u8				   unknown_00[24];
	struct FieldActor *actor;
};

#endif

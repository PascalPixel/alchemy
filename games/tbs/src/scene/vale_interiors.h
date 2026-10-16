#ifndef GUARD_VALE_INTERIORS_H
#define GUARD_VALE_INTERIORS_H

#include "types.h"

#include "facing_object.h"
#include "field_event.h"
#include "field_scene.h"

enum ValeHouseFlag { FLAG_GOT_FAREWELL_HERB = 0x81b };

enum { ITEM_HERB = 180 };

extern struct SceneEvent gValeHouseEvents[];
extern struct SceneEvent gValeHouseEventsAfterLeaving[];
extern struct SceneEvent gValeHouseReturnEvents[];

#endif

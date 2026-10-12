#ifndef GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H
#define GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H

#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "types.h"

#define NULL ((void *)0)

struct FieldActor;

struct FieldActor *Engine_ObjectCreate(s32, s32, s32, s32);
void			   Engine_ActorSetSpriteFlags(struct FieldActor *, s32);

#endif

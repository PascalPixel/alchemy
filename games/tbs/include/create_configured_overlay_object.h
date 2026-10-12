#ifndef GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H
#define GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H

#include "unit/battle/effect/object.h"
#include "unit/battle/effect/sparkle.h"
#include "unit/battle/effect/evactor.h"
#include "unit/battle/effect/evcamera.h"
#include "unit/battle/effect/djinnev.h"
#include "unit/battle/effect/psydisp.h"
#include "unit/battle/effect/psyfx.h"
#include "types.h"

#define NULL ((void *)0)

struct FieldActor;

struct FieldActor *Engine_ObjectCreate(s32, s32, s32, s32);
void			   Engine_ActorSetSpriteFlags(struct FieldActor *, s32);

#endif

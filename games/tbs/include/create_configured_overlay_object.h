#ifndef GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H
#define GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H

#include "unit/object/object.h"
#include "unit/object/psyfx.h"
#include "types.h"

#define NULL ((void *)0)

struct FieldActor;

struct FieldActor *Engine_ObjectCreate(s32, s32, s32, s32);
void			   Engine_ActorSetSpriteFlags(struct FieldActor *, s32);

#endif

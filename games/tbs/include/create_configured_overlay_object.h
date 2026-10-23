#ifndef GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H
#define GUARD_CREATE_CONFIGURED_OVERLAY_OBJECT_H

/// @file
/// The engine calls a scene uses to create an overlay object and set its sprite
/// flags.

#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "types.h"

#define NULL ((void *)0)

struct ObjectEvent;

struct ObjectEvent *Engine_ObjectCreate(s32, s32, s32, s32);
void Engine_ObjectEventSetSpriteFlags(struct ObjectEvent *, s32);

#endif

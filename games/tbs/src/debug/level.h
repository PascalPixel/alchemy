#ifndef ALCHEMY_DEBUG_LEVEL_H
#define ALCHEMY_DEBUG_LEVEL_H

#include "types.h"
#include "unit.h"

#include "window.h"

#include "field_work.h"
#include "heap_slots.h"

void Engine_ObjectEventSetAnimation();
void Engine_ObjectEventSetPosition(u32 localId, s32 x, s32 z);
void Engine_ScriptRequestWarp();

#endif

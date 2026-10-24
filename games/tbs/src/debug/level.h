#ifndef ALCHEMY_DEBUG_LEVEL_H
#define ALCHEMY_DEBUG_LEVEL_H

#include "types.h"
#include "unit.h"

#include "window.h"

#include "field_work.h"
#include "heap_slots.h"

void Import_ObjectEventSetAnimation();
void Import_ObjectEventSetPosition(u32 localId, s32 x, s32 z);
void Import_ScriptRequestWarp();

#endif

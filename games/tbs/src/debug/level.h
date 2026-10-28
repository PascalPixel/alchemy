#ifndef ALCHEMY_DEBUG_LEVEL_H
#define ALCHEMY_DEBUG_LEVEL_H

#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "playback_queue.h"
#include "battle_action_run.h"

#include "window.h"

#include "field_work.h"
#include "heap_slots.h"

void Import_ActorSetAnimation();
void Import_ActorSetPosition(u32 actorId, s32 x, s32 z);
void Import_EventSetExitRequest();

#endif

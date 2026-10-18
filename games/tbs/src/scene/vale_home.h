#include "resource.h"
#ifndef VALE_HOME_H
#define VALE_HOME_H

#include "types.h"
#include "animspr.h"
#include "vram_block.h"
#include "callback_scheduler.h"

#define NULL ((void *)0)

#include "field_event.h"
#include "field_scene.h"
#include "facing_object.h"
#include "field_effect.h"

#include "battle_effect_runtime.h"

struct PairWork {
  u8 unk00[70];
  u16 vramBlock;
};

#include "global_cells.h"

s32 Math_RemainderUnsigned();
void Engine_ActorSetSpriteFlags();

#endif

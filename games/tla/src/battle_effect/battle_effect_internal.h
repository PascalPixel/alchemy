#ifndef ALCHEMY_BATTLE_EFFECT_INTERNAL_H
#define ALCHEMY_BATTLE_EFFECT_INTERNAL_H

/// @file
/// What every part of the battle_effect object shares: its includes. battle_effect.c
/// includes it before the parts, and each part includes it too.

#include "types.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "system.h"
#include "ram_buffer.h"
#include "bg_scroll.h"
#include "dma.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "battle_display.h"
#include "task.h"
#include "motobj.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "graphics.h"
#include "transform.h"
#include "resource.h"
#include "heap_release.h"
#include "input.h"
#include "battle_work.h"
#include "irq_lock.h"
#include "render_model.h"
#include "battle.h"
#include "vram_tab.h"
#include "item_icon.h"
#include "battle_fx_work.h"
#include "lib.h"
#include "sound.h"
#include "battle_effect_argument.h"
#include "actor.h"
#include "macros.h"

#endif // ALCHEMY_BATTLE_EFFECT_INTERNAL_H

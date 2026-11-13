#ifndef ALCHEMY_SUMMON_EFFECTS_INTERNAL_H
#define ALCHEMY_SUMMON_EFFECTS_INTERNAL_H

/// @file
/// What every part under summon_effects/ shares: its includes.
/// battle_effect.c includes it before those parts, and each part includes it
/// too.

#include "types.h"
#include "battle/battle_effect.h"
#include "battle_effect_work.h"
#include "field/effect_step.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/heap_release.h"
#include "system/ram_buffer.h"
#include "battle/render_model.h"
#include "bg_scroll.h"
#include "system/input.h"
#include "system/io_write_queue.h"
#include "system/task.h"
#include "graphics/graphics.h"
#include "graphics/resource.h"
#include "battle/motobj.h"
#include "battle_display.h"
#include "battle_work.h"
#include "battle/battle.h"
#include "actor.h"
#include "system/vector3.h"
#include "system/dma.h"
#include "system/ram_code.h"
#include "macros.h"

#endif // ALCHEMY_SUMMON_EFFECTS_INTERNAL_H

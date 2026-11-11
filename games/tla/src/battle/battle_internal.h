#ifndef ALCHEMY_BATTLE_INTERNAL_H
#define ALCHEMY_BATTLE_INTERNAL_H

/// @file
/// What every part of the battle object shares: its includes, in their
/// original order, and the names two or more parts use. battle.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves. It places no bytes.

#include "types.h"
#include "owner_state.h"
#include "input.h"
#include "main_routine_work.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "battle_display.h"
#include "battle_work.h"
#include "ram_buffer.h"
#include "serial.h"
#include "text/msg_ids.h"
#include "battle_side.h"
#include "dma.h"
#include "io_reg.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "battle_system.h"
#include "battle_type.h"
#include "battle_summon.h"
#include "battle_unit.h"
#include "heap_release.h"
#include "tla_edition.h"
#include "edition.h"
#include "battle_action.h"
#include "motobj.h"
#include "macros.h"
#include "fixed_math.h"
#include "version.h"
#include "character_ids.h"

#endif

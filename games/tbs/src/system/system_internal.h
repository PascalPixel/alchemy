#ifndef ALCHEMY_SYSTEM_INTERNAL_H
#define ALCHEMY_SYSTEM_INTERNAL_H

/// @file
/// What every part of the System object shares: its includes and the library
/// call two parts make. system.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "resource_ids.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "game/game.h"
#include "gamepak/rom_header.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/affine.h"
#include "system/io_write.h"
#include "system/io_write_queue.h"
#include "system/iwram_call.h"
#include "system/math_table.h"
#include "system/number_text.h"
#include "system/projection.h"
#include "system/ram_buffer.h"
#include "system/ram_code.h"
#include "system/save.h"
#include "system/savedata.h"
#include "system/serial.h"
#include "system/transform.h"
#include "system/vram_cache.h"

/// The compiler library's call through r3, which system_oam.c uses to run code
/// copied to RAM.
s32 _call_via_r3(s32, s32, s32, s32);

#endif

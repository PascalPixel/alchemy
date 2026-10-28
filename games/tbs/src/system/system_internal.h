#ifndef ALCHEMY_SYSTEM_INTERNAL_H
#define ALCHEMY_SYSTEM_INTERNAL_H

/// @file
/// What every part of the System object shares: its includes and the library
/// call two parts make. system.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "heap_release.h"
#include "dma.h"
#include "ram_code.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "task.h"
#include "irq.h"
#include "types.h"
#include "battle_sprite_id.h"
#include "resource_ids.h"
#include "heap_slots.h"
#include "rom_header.h"
#include "ram_buffer.h"
#include "serial.h"
#include "frame.h"
#include "input.h"
#include "oam.h"
#include "map_grid_scroll.h"
#include "heap.h"
#include "irq_lock.h"
#include "io_write.h"
#include "lib.h"
#include "fixed_math.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_cache.h"
#include "string.h"
#include "static_assert.h"
#include "edition.h"
#include "transform.h"
#include "projection.h"
#include "save.h"
#include "runtime_interfaces.h"
#include "flash.h"
#include "sound_driver_symbols.h"
#include "module/system/math_table.h"
#include "module/system/number_text.h"
#include "savedata.h"
#include "sounds.h"

/// The compiler library's call through r3, which oam.c and save.c use to
/// run code copied to RAM with four arguments.
s32 _call_via_r3(s32, s32, s32, s32);

#endif

#ifndef ALCHEMY_SYSTEM_INTERNAL_H
#define ALCHEMY_SYSTEM_INTERNAL_H

/// @file
/// What every part of the System object shares: its includes and the library
/// call two parts make. system.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "ram_code.h"
#include "io_write_queue.h"
#include "resource_ids.h"
#include "rom_header.h"
#include "ram_buffer.h"
#include "serial.h"
#include "map_grid_scroll.h"
#include "io_write.h"
#include "affine.h"
#include "iwram_call.h"
#include "vram_cache.h"
#include "transform.h"
#include "projection.h"
#include "save.h"
#include "sound_driver_symbols.h"
#include "module/system/math_table.h"
#include "module/system/number_text.h"
#include "savedata.h"

/// The compiler library's call through r3, which oam.c uses to run code
/// copied to RAM.
s32 _call_via_r3(s32, s32, s32, s32);

#endif

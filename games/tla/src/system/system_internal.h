#ifndef ALCHEMY_SYSTEM_INTERNAL_H
#define ALCHEMY_SYSTEM_INTERNAL_H

/// @file
/// What every part of the System object shares: its includes and the one
/// variable two parts declare. system.c includes it before the parts, and each
/// part includes it too, so that a part read on its own still resolves.

#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "macros.h"
#include "affine.h"
#include "bg_scroll.h"
#include "dma.h"
#include "enemy_definition.h"
#include "fixed_math.h"
#include "flash.h"
#include "frame.h"
#include "heap_release.h"
#include "input.h"
#include "io_reg.h"
#include "io_write.h"
#include "io_write_queue.h"
#include "irq.h"
#include "irq_lock.h"
#include "iwram_call.h"
#include "oam.h"
#include "ram_buffer.h"
#include "resource.h"
#include "rom_header.h"
#include "save.h"
#include "savedata.h"
#include "scene.h"
#include "serial.h"
#include "sound_driver.h"
#include "string.h"
#include "task.h"
#include "transform.h"
#include "vram_tab.h"

/// Cleared at power-up (start_up.c) and read by the debug text writers
/// (debug_text.c), which write only while it is set.
extern u8 gData_03001110;

#endif

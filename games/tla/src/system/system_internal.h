#ifndef ALCHEMY_SYSTEM_INTERNAL_H
#define ALCHEMY_SYSTEM_INTERNAL_H

/// @file
/// What every part of the System object shares: its includes and the one
/// variable two parts declare. system.c includes it before the parts, and each
/// part includes it too, so that a part read on its own still resolves.

#include "module/system/system.h"
#include "battle/battle.h"
#include "debug.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "minigame.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "edition.h"
#include "macros.h"
#include "system/affine.h"
#include "bg_scroll.h"
#include "system/dma.h"
#include "game/enemy_definition.h"
#include "fixed_math.h"
#include "gamepak/flash.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/io_write_queue.h"
#include "system/irq.h"
#include "system/irq_lock.h"
#include "system/iwram_call.h"
#include "graphics/oam.h"
#include "system/ram_buffer.h"
#include "graphics/resource.h"
#include "gamepak/rom_header.h"
#include "system/save.h"
#include "system/savedata.h"
#include "system/scene.h"
#include "system/serial.h"
#include "sound_driver.h"
#include "string.h"
#include "system/task.h"
#include "system/transform.h"
#include "graphics/vram_tab.h"

extern u8 gDebugTextEnabled;

#endif

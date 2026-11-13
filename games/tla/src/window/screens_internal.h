#ifndef ALCHEMY_SCREENS_INTERNAL_H
#define ALCHEMY_SCREENS_INTERNAL_H

/// @file
/// What every part of the screens object shares: its includes and the
/// declarations the parts disagreed on. screens.c includes it before the
/// parts, and each part includes it too, so that a part read on its own still
/// resolves.

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "render_input.h"
#include "types.h"
#include "output.h"
#include "ram_buffer.h"
#include "resource.h"
#include "vram_tab.h"
#include "window.h"
#include "stddef.h"
#include "heap_release.h"
#include "dma.h"
#include "ram_code.h"
#include "uiwindow.h"
#include "io_reg.h"
#include "battle_work.h"
#include "main_routine_work.h"
#include "uiflow.h"
#include "edition.h"
#include "irq_lock.h"
#include "static_assert.h"
#include "battle_side.h"
#include "b5_context.h"
#include "save.h"
#include "battle_unit.h"
#include "owner_state.h"
#include "frame.h"
#include "scene.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "task.h"
#include "field_sprite.h"
#include "text_render.h"
#include "minigame.h"
#include "text/msg_ids.h"
#include "input.h"
#include "text_font.h"
#include "battle_summon.h"
#include "system/affine.h"
#include "glyph.h"
#include "irq.h"
#include "system/io_write_queue.h"
#include "battle_system.h"
#include "event.h"
#include "flag_call.h"
#include "module/system/system.h"
#include "module/window/screens.h"
#include "module/window/window.h"

// The screens object's functions its parts call before their definitions.
struct BobPairObject;

s32 Message_OpenWindow(s32 message, s32 x, s32 y, s32 style);

#endif

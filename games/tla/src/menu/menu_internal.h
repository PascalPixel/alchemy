#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

/// @file
/// What every part of the menu object shares: its includes and the declarations
/// its parts disagreed on. menu.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "tla_edition.h"
#include "version.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "frame.h"
#include "input.h"
#include "fixed_math.h"
#include "resource.h"
#include "task.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "item.h"
#include "battle_unit.h"
#include "window.h"
#include "uiwindow.h"
#include "render_input.h"
#include "render_placement.h"
#include "sprite.h"
#include "m7_interfaces.h"
#include "text/msg_ids.h"

// The menu object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
void StatusPanel_DrawHpPp(struct BattleUnit *unit, s32 window);
void Text_DrawStatComparison(const struct BattleUnit *alt,
                             const struct BattleUnit *base, s32 work);
void Text_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                   s32 x, s32 y);

#endif

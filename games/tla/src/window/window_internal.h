#ifndef ALCHEMY_WINDOW_INTERNAL_H
#define ALCHEMY_WINDOW_INTERNAL_H

/// @file
/// What every part of the window object shares: its includes. window.c
/// includes it before the parts, and each part includes it too, so that a part
/// read on its own still resolves.

// The window object defines these with its own types.
#define OWN_UiTextResource_Initialize
#define OWN_UiTextResource_SetPosition

#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "edition.h"
#include "system/heap_release.h"
#include "window.h"
#include "graphics/uiwindow.h"
#include "system/task.h"
#include "system/dma.h"
#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "graphics/uiflow.h"
#include "graphics/render_input.h"
#include "graphics/output.h"
#include "graphics/resource.h"
#include "graphics/glyph.h"
#include "system/input.h"
#include "system/main_routine_work.h"
#include "system/frame.h"
#include "version.h"
#include "tla_edition.h"
#include "graphics/text_font.h"
#include "graphics/text_render.h"
#include "fixed_math.h"
#include "system/ram_code.h"
#include "graphics/text_reader.h"
#include "battle/battle_unit.h"
#include "game/item.h"
#include "system/affine.h"
#include "system/scene.h"
#include "minigame.h"
#include "menu/select.h"
#include "text/msg_ids.h"
#include "menu/node_chain.h"
#include "static_assert.h"
#include "macros.h"
#include "graphics/oam.h"
#include "flag_call.h"

// The window object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height);
void Window_MapTextCanvasTiles(s32 x, s32 y, u32 width, u32 height, s32 whole);
void Text_ResetStyle();
struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags);
void Window_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                        u16 *spacing, u32 flags);
s32 Text_GetMessageSize(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
s32 Glyph_Draw(struct Window *window, s32 character, s32 x, s32 y, s32 mode);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y);

#endif

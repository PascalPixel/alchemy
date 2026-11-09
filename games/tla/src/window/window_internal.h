#ifndef ALCHEMY_WINDOW_INTERNAL_H
#define ALCHEMY_WINDOW_INTERNAL_H

/// @file
/// What every part of the window object shares: its includes. window.c
/// includes it before the parts, and each part includes it too, so that a part
/// read on its own still resolves.

// The window object defines these with its own types.
#define OWN_UiTextResource_Initialize
#define OWN_UiTextResource_SetPosition

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "heap_release.h"
#include "window.h"
#include "uiwindow.h"
#include "task.h"
#include "dma.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "uiflow.h"
#include "render_input.h"
#include "output.h"
#include "resource.h"
#include "glyph.h"
#include "input.h"
#include "main_routine_work.h"
#include "frame.h"
#include "version.h"
#include "tla_edition.h"
#include "text_font.h"
#include "text_render.h"
#include "fixed_math.h"
#include "ram_code.h"
#include "text_reader.h"
#include "battle_unit.h"
#include "item.h"
#include "affine.h"
#include "scene.h"
#include "minigame.h"
#include "select.h"
#include "text/msg_ids.h"
#include "node_chain.h"
#include "static_assert.h"
#include "macros.h"
#include "oam.h"
#include "flag_call.h"

// The window object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
void Window_DrawBorder(s32 x, s32 y, u32 width, u32 height);
void Func_0803a2b0(s32 x, s32 y, u32 width, u32 height, s32 whole);
void Text_ResetStyle();
struct UiChannelSlot *Message_Start(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags);
void Window_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                        u16 *spacing, u32 flags);
s32 Text_GetMessageSize(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
s32 Func_0803bde4(struct Window *window, s32 character, s32 x, s32 y,
                  s32 mode);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y);

#endif

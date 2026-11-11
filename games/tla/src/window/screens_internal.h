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

// The window object's functions, declared once as it defines them.
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_DrawMessage(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Text_DrawStringInWindow(u8 *text, s32 arg1, u32 x, u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y);
void WindowSprite_RedrawSavedRect(struct Window *window);
void WindowSprite_PrepareForRedraw(struct Window *work);
void MessageArgument_Push(u32 value, u32 kind);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);

// The screens object's functions its parts call before their definitions.
struct BobPairObject;
void Func_08043b70(void);
s32 Func_08043cd8(s32 selectedFile, s32 menuMode);
void MenuTitle_Bob(struct BobPairObject *object);
struct WindowSprite *RenderResource_CreatePair(s32 group,
                                               struct RenderInput *input, s32 x,
                                               s32 y);

#endif

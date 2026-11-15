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
#include "graphics/vram_tab.h"
#include "stddef.h"
#include "battle_work.h"
#include "system/irq_lock.h"
#include "battle/battle_side.h"
#include "battle/b5_context.h"
#include "system/save.h"
#include "owner_state.h"
#include "field_sprite.h"
#include "battle/battle_summon.h"
#include "system/irq.h"
#include "system/io_write_queue.h"
#include "battle/battle_system.h"
#include "field/event.h"
#include "module/system/system.h"
#include "module/window/screens.h"
#include "module/window/window.h"
#include "actor.h"
#include "battle/battle_action.h"
#include "menu/resource_menu.h"
#include "graphics/display_transition.h"
#include "scene_call.h"
#include "field/sprite.h"

/// The window animation a message can start (its window-state code): the
/// window's frame flashes as it swells, for the message's countdown.
#define WINDOW_ANIMATION_SWELL 4

// Window animation lengths.
#define WINDOW_OPEN_FRAMES 8
#define WINDOW_CLOSE_FRAMES 4

/// Writes code at position in the ring of decoded text and steps position on,
/// wrapping at the ring's end.
#define TEXT_APPEND(entries, position, code)                                   \
  ((entries)[position] = (code),                                               \
   (position) = ((position) + 1) & TEXT_BUFFER_MASK)

#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
    LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
#define TEXT_ARGUMENT_SPACE ' '
#else
#define TEXT_ARGUMENT_SPACE 143
#endif

#define MESSAGE_ARGUMENT_COUNT 8 ///< the message argument table's places
/// A name the decoder writes out into a message, decoded first into a buffer
/// of this many halfwords.
#define MESSAGE_NAME_HALFWORDS 24
#define TEXT_NUMBER_BUFFER_SIZE 16 ///< String_FormatNumber writes 14 of them

/// The control codes of decoded message text (WindowWork.textBuffer). Codes
/// from TEXT_SPACE up are characters.
enum {
  TEXT_END = 0,  ///< the text ends; the window stays open
  TEXT_PAGE = 1, ///< wait for a button, then clear the window for a new page
  TEXT_WAIT_KEY = 2, ///< wait for a button, then close the window
  TEXT_NEWLINE = 3,
  TEXT_PAUSE_LONG = 4,    ///< a 60-frame pause
  TEXT_PAUSE_SHORT = 5,   ///< a 20-frame pause a button can cut short
  TEXT_PAUSE_LONGEST = 6, ///< a 120-frame pause a button can cut short
  TEXT_COLORS_RESET = 7,  ///< the colour, style and line spacing set at start
  TEXT_COLOR = 8,         ///< the next entry is the text colour
  TEXT_OUTLINE = 9,       ///< the next entry is the text style
  /// The next entry is the line spacing; a glyph sprite sits half of it below
  /// its line's top.
  TEXT_LINE_SPACING = 10,
  TEXT_WINDOW_STATE = 15, ///< the next entry is the window animation to run
  TEXT_END_ALT = 30,      ///< ends the text as TEXT_END does
  /// The message runs the codes below it as control codes and draws the rest.
  TEXT_COMMANDS = 31,
  TEXT_SPACE = 32,
  // Characters the decoder tests by code, as the fonts draw them: the Japanese
  // one by charmap_ja.tsv, the others by Latin-1 and its CP1252 extension.
  TEXT_OPENING_QUOTE = 142,             ///< “, outside Japanese
  TEXT_INVERTED_EXCLAMATION_MARK = 161, ///< ¡, outside Japanese
  TEXT_JAPANESE_FULL_STOP = 161,        ///< 。
  TEXT_JAPANESE_COMMA = 164,            ///< 、
  TEXT_JAPANESE_MIDDLE_DOT = 165,       ///< ・
  TEXT_INVERTED_QUESTION_MARK = 191     ///< ¿, outside Japanese
};

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
s16 DebugSceneSelect_Run(void);
s32 DebugSceneSelect_HandleRowInput(s32 arg, s16 value, s16 *sub, s16 *mode);

#endif

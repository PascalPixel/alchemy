#ifndef ALCHEMY_WINDOW_INTERNAL_H
#define ALCHEMY_WINDOW_INTERNAL_H

/// @file
/// What every part of the window object shares: its includes, edition values,
/// macros and the codes of decoded text. window.c includes it before the
/// parts, and each part includes it too, so that a part read on its own still
/// resolves.

#include "unit/window/scales.h"
#include "unit/window/articles.h"
#include "unit/window/message_speed.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/window/glyph_arm.h"
#include "unit/runtime/runtime.h"
#include "types.h"
#include "edition.h"
#include "window.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "task.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "heap_slots.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "save_block.h"
#include "battle_work.h"
#include "heap.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "battle_party.h"
#include "message_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "item.h"
#include "vram_cache.h"
#include "select.h"
#include "text/msg_ids.h"
#include "songs.h"
#include "static_assert.h"
#include "event_layer.h"
#include "battle_type.h"
#include "uiflow.h"
#include "unit.h"
#include "option_menu.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save.h"
#include "irq.h"
#include "status_screen.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "io_write_queue.h"
#include "map_grid_scroll.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "particle.h"
#include "icon_menu.h"
#include "battle_summon.h"
#include "serial_runtime.h"
#include "battle_anim_work.h"
#include "shop.h"
#include "display_transition.h"
#include "frame.h"
#include "macros.h"
#include "flags.h"
#include "ability_ids.h"

// Edition-specific text and window layout values. The possessives
// (gLocalizedWord_ItemPossessives) are by number in Spanish, singular and
// plural, and by item gender in German and Italian; MESSAGE_EM_DASH starts with
// TEXT_ARGUMENT_SPACE, which only the international decoder writes; a
// space in a window of sprite glyphs advances WINDOW_SPRITE_SPACE_WIDTH pixels,
// as Text_MeasureDecoded counts it.
#if (LANGUAGE == LANGUAGE_SPANISH)
#define ITEM_POSSESSIVE_TEXT_COUNT 2
#else
#define ITEM_POSSESSIVE_TEXT_COUNT 4
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TEXT_ARGUMENT_SPACE ' '
#else
#define TEXT_ARGUMENT_SPACE 143
#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define WINDOW_SPRITE_SPACE_WIDTH 7
#else
#define WINDOW_SPRITE_SPACE_WIDTH 5
#endif

#ifdef MODERN
// In the matching build these calls precede their declarations. Prototypes
// reorder three argument moves for ClearTileAttributesInRect and two for
// PutGlyph in the English image.
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode);
void Window_ClearTextTilesInRect(s32 x, s32 y, u32 width, u32 height);
#endif

/// A glyph resource id meaning no glyph resource is loaded.
#define NO_GLYPH_RESOURCE 99

// WindowWork owns three text printers, each with four line gaps.
#define TEXT_PRINTER_COUNT 3
#define TEXT_PRINTER_LINE_COUNT 4 ///< lines a page; spacing has one gap each
#define TEXT_DEFAULT_COLOR                                                     \
  TEXT_COLOR_WHITE                   ///< a text printer's text colour at start
#define TEXT_DEFAULT_LINE_SPACING 10 ///< a text printer's line spacing at start

// Window animation lengths.
#define WINDOW_OPEN_FRAMES 8
#define WINDOW_CLOSE_FRAMES 4

// Scratch capacities are in bytes unless the name says halfwords.
#define TEXT_NUMBER_BUFFER_SIZE 16 ///< String_FormatNumber writes 14 of them
/// A byte string widened to codes: 255 bytes and the terminator.
#define TEXT_WIDE_STRING_BUFFER_SIZE 0x200
/// A name the decoder writes out into a message, decoded first into a buffer
/// of this many halfwords.
#define MESSAGE_NAME_HALFWORDS 24
#define MESSAGE_ARGUMENT_COUNT 8      ///< the message argument table's places
#define SELECTION_MENU_ENTRY_LIMIT 16 ///< entries a selection row can list
#define AREA_NAME_WINDOW_FRAMES 90    ///< how long the area's name shows

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
  TEXT_COLORS_RESET =
      7, ///< TEXT_DEFAULT_COLOR, style 0 and TEXT_DEFAULT_LINE_SPACING again
  TEXT_COLOR = 8, ///< the next entry is the text colour
  /// The next entry is the text style (enum TextStyle).
  TEXT_OUTLINE = 9,
  /// The next entry is the line spacing; a glyph sprite sits half of it below
  /// its line's top.
  TEXT_LINE_SPACING = 10,
  TEXT_WINDOW_STATE = 15, ///< the next entry is the window effect to run
  TEXT_END_ALT = 30,      ///< ends the text as TEXT_END does
  /// The printer runs the codes below it as control codes and draws the rest.
  TEXT_COMMANDS = 31,
  TEXT_SPACE = 32,
  TEXT_VOICED_MARK = 222,    ///< dakuten
  TEXT_SEMIVOICED_MARK = 223 ///< handakuten
};

// The kana mark drawn with a character, in the top two bits of its code (the
// index into gFont_MarkGlyphCodes).
#define TEXT_CODE_MARK_SHIFT 14
#define TEXT_CODE_VOICED (1 << TEXT_CODE_MARK_SHIFT)
#define TEXT_CODE_SEMIVOICED (2 << TEXT_CODE_MARK_SHIFT)

/// Which selection icon records SelectionMenu_FindFreeIcon looks in.
enum SelectionIconGroup {
  SELECTION_ICON_ROW = 0,   ///< the seven icons of the row
  SELECTION_ICON_CHOSEN = 1 ///< the five icons already chosen; any nonzero
};

/// The style of sprite text (WindowWork.outline, set by TEXT_OUTLINE): its
/// glyphs outlined, or a motion of the glyph sprites (Glyph_Draw).
enum TextStyle {
  TEXT_STYLE_PLAIN = 0,
  TEXT_STYLE_OUTLINED = 1, ///< drawn a pixel wider (Text_DrawGlyphPair)
  TEXT_STYLE_WOBBLE = 2,   ///< WINDOW_SPRITE_MOTION_WOBBLE
  TEXT_STYLE_SHAKE = 3,    ///< WINDOW_SPRITE_MOTION_SHAKE
  TEXT_STYLE_POP = 4,      ///< WINDOW_SPRITE_MOTION_POP
  TEXT_STYLE_SPIN = 5      ///< WINDOW_SPRITE_MOTION_SPIN, a pixel wider too
};

/// The window effect a message can start (TEXT_WINDOW_STATE): the window's
/// frame grows by a tile and its text sprites pulse, for the printer's
/// countdown.
#define WINDOW_ANIMATION_SWELL 4

/// The heap block the message decoder routine is copied into to run, for the
/// length of one message's decoding (Message_Decode), or of one
/// Message_DecodeRaw call when the block is empty.
#define TEXT_WORK_BLOCK 50

/// HEAP_SLOT_GLYPH_TRANSFER's pointer, kept in IWRAM (system/iwram.s, after
/// gWindowWork and gPartyStatusWork): the glyph transfer work icons are built
/// in. Each icon builder allocates the slot for its build and frees it after.
extern GlyphTransfer *gGlyphWork;

/// Each message context bank's Huffman models and their offsets, which the
/// message catalog built from games/tbs/text defines.
extern const void *const Text_MessageContexts[][2];

#endif

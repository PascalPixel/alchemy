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
#include "debug/debug.h"
#include "djinn_ids.h"
#include "enemy_ids.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "edition.h"
#include "system/heap_release.h"
#include "window/window.h"
#include "graphics/uiwindow.h"
#include "system/task.h"
#include "system/dma.h"
#include "system/iwram_call.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "graphics/uiflow.h"
#include "graphics/render_input.h"
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
#include "menu/select.h"
#include "text/msg_ids.h"
#include "menu/node_chain.h"
#include "static_assert.h"
#include "macros.h"
#include "graphics/oam.h"
#include "field/flag_call.h"
#include "graphics/vram_tab.h"
#include "stddef.h"
#include "battle/battle_work.h"
#include "system/irq_lock.h"
#include "battle/battle_side.h"
#include "system/save.h"
#include "game/owner_state.h"
#include "field/field_sprite.h"
#include "battle/battle_summon.h"
#include "system/irq.h"
#include "system/io_write_queue.h"
#include "battle/battle_system.h"
#include "field/event.h"
#include "window/screens.h"
#include "window/window.h"
#include "field/actor.h"
#include "battle/battle_action.h"
#include "window/icon_menu.h"
#include "graphics/display_transition.h"
#include "field/scene_call.h"
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
/// A byte string widened to codes: 255 bytes and the terminator.
#define TEXT_WIDE_STRING_BUFFER_SIZE 512
#define SELECTION_MENU_ENTRY_LIMIT 16 ///< entries a selection row can list
#define AREA_NAME_WINDOW_FRAMES 90    ///< how long the area's name shows

// WindowWork owns three messages, each with four line gaps.
#define MESSAGE_COUNT 3
#define MESSAGE_LINE_COUNT 4         ///< lines a page; spacing has one gap each
#define TEXT_DEFAULT_LINE_SPACING 10 ///< a message's line spacing at start
/// The pixels a line of text takes in a window fitted to it.
#define TEXT_LINE_HEIGHT 15

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
  /// MESSAGE_ACTOR_NAME as Message_DecodeRaw keeps it, unexpanded: the next
  /// entry names the unit, and drawing and measuring skip both.
  TEXT_ACTOR_NAME = 17,
  /// MESSAGE_ARTICLE_CLASS as Message_DecodeRaw keeps it outside Japanese, at
  /// the head of an item or class name: drawing skips it and its class.
  TEXT_ARTICLE_CLASS = 29,
  TEXT_END_ALT = 30, ///< ends the text as TEXT_END does
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
void Text_ResetStyle();
void Window_FitOnScreen(s32 no, s32 *px, s32 *py, u32 *pw, u32 *ph,
                        u16 *spacing, u32 flags);
s32 Text_GetMessageSize(s32 no, s32 *x, s32 *y, u32 *width, u32 *height);
s32 Glyph_Draw(struct Window *window, s32 character, s32 x, s32 y, s32 mode);
void Window_DrawDividerLine(struct Window *win, u32 x1, u32 y1, u32 x2, u32 y2);
void Text_DrawMessage(s32 message, s32 window, s32 x, s32 y);
void Text_DrawStringInWindow(u8 *text, s32 window, u32 x, u32 y);
void Text_DrawNumberInWindow(s32 value, s32 width, s32 output, s32 x, s32 y);
void Text_DrawNumber(s32 value, s32 width, s32 window, s32 x, s32 y);
void Text_DrawWideStringAtOffset(u16 *text, struct Window *window, s32 x,
                                 s32 y);
s16 DebugSceneSelect_Run(void);
s32 DebugSceneSelect_HandleRowInput(s32 arg, s16 value, s16 *sub, s16 *mode);

/// The first of the 20 digit tiles Graphics_ExpandVramTilesByColorTable
/// builds for the battle lists' page numbers: the UI digits '0' to '9' with
/// colour 1 drawn as 10, then the same digits in their own colours.
#define LIST_PAGE_DIGITS_TILE 768

/// Tiles the battle lists draw over their window's top edge: the page numbers
/// and arrows.
enum {
  /// Page n's number: + n gives digit n + 1, from LIST_PAGE_DIGITS_TILE + 1,
  /// the UI digits with colour 1 drawn as 10.
  LIST_TILE_PAGE_NUMBER = WINDOW_TILE(LIST_PAGE_DIGITS_TILE + 1),
  /// The current page's number, the same digits in their own colours.
  LIST_TILE_PAGE_NUMBER_LIT = WINDOW_TILE(LIST_PAGE_DIGITS_TILE + 10 + 1),
  /// Left arrow, tile 820, which Link_DrawPageArrows slides.
  LIST_TILE_PAGES_LEFT = WINDOW_TILE(820),
  LIST_TILE_PAGES_RIGHT = WINDOW_TILE(821), ///< Right arrow, tile 821
  /// The Status screen's own two arrows, tiles 822 and 823, which
  /// BattleStatus_DrawPageArrows slides.
  STATUS_TILE_PAGES_LEFT = WINDOW_TILE(822),
  STATUS_TILE_PAGES_RIGHT = WINDOW_TILE(823)
};

/// The battle lists' two page arrow tiles in VRAM, which Link_DrawPageArrows
/// draws each frame (LIST_TILE_PAGES_LEFT and LIST_TILE_PAGES_RIGHT).
#define LIST_PAGE_ARROW_VRAM                                                   \
  ((s32)BG_CHAR_BLOCK(0) +                                                     \
   (LIST_TILE_PAGES_LEFT & TILE_NUMBER_MASK) * TILE_SIZE_4BPP)
/// The Status screen's arrow tiles in VRAM.
#define STATUS_PAGE_ARROW_VRAM                                                 \
  ((s32)BG_CHAR_BLOCK(0) +                                                     \
   (STATUS_TILE_PAGES_LEFT & TILE_NUMBER_MASK) * TILE_SIZE_4BPP)

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
  TEXT_STYLE_WOBBLE = 2,
  TEXT_STYLE_SHAKE = 3,
  TEXT_STYLE_POP = 4,
  TEXT_STYLE_SPIN = 5 ///< a pixel wider too
};

/// The two battle command rows BattleCommandRow_Run runs.
enum CommandRowMode {
  COMMAND_ROW_PARTY = 0, ///< the start of a turn: Fight, Flee and Status
  COMMAND_ROW_MEMBER =
      1 ///< a party member's commands; any mode but 0 is this row
};

/// What BattleTarget_RunSelection shows on a party member for the action
/// being aimed (Battle_ClassifyEntryKind), and which party member it starts on.
enum BattleTargetHint {
  TARGET_HINT_NONE = 0,
  TARGET_HINT_HP = 1, ///< heals HP: the party member's HP
  TARGET_HINT_PP = 2, ///< restores PP: the party member's PP
  /// Cures poison (ABILITY_EFFECT_CURE_POISON): starts on a poisoned party
  /// member.
  TARGET_HINT_CURE_POISON = 3,
  /// Cures what Restore cures (ABILITY_EFFECT_RESTORE): starts on a party
  /// member who has one of those statuses, and lists them.
  TARGET_HINT_RESTORE = 4,
  /// Revives: starts on a downed party member.
  TARGET_HINT_REVIVE = 5,
  /// Cures Restore's statuses, poison and the evil spirit, as Tonic does
  /// (ABILITY_EFFECT_CURE_ALL).
  TARGET_HINT_CURE_ALL = 6,
  TARGET_HINT_SET_DJINNI = 7 ///< setting a Djinni: "Set Djinni"
};

/// What Save_ScanFileSummaries adds to its count of files when every file is
/// cleared (its sendFlag set by saving at the game's end).
#define SAVE_SCAN_ALL_CLEARED 100

/// Places a battle menu's pointing hand at its list window's selected row:
/// `position` takes the hand's spot, `margin` pixels left of the window and
/// 20 below the row's top, and `sprite` becomes a plain 16x16 sprite, its
/// position still to come. The Djinn, item, summon and Psynergy battle menus
/// each place their cursor so; the name is ours, as The Broken Seal's.
#define MENU_PLACE_CURSOR(position, window, row, margin, sprite)               \
  do {                                                                         \
    (position).x = (s16)(window)->x * 8 - (margin);                            \
    (position).y = ((row) * 2 + (s16)(window)->y) * 8 + 20;                    \
    ((u32 *)(sprite))[1] = OBJ_SIZE_16X16;                                     \
    ((u32 *)(sprite))[2] = 0;                                                  \
  } while (0)

#endif

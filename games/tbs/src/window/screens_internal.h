#ifndef ALCHEMY_SCREENS_INTERNAL_H
#define ALCHEMY_SCREENS_INTERNAL_H

/// @file
/// What every part of the menu object shares: its includes, edition values,
/// macros and the link round state. screens.c includes it before the
/// parts, and each part includes it too, so that a part read on its own still
/// resolves.

#include "render_placement.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/overworld/overworld.h"
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
#include "game_state.h"
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
#include "field_offset.h"
#include "message_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "item.h"
#include "vram_cache.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "songs.h"
#include "flags.h"
#include "static_assert.h"
#include "script_layer.h"
#include "b5_context.h"
#include "battle_type.h"
#include "uiflow.h"
#include "unit.h"
#include "option_menu.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save.h"
#include "irq.h"
#include "summary_screen.h"
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
#include "unit/overworld/field_services.h"
#include "unit/window/scales.h"
#include "ability_ids.h"
#include "djinn_ids.h"

#ifdef MODERN
// The menu object calls these window functions with no prototype in scope, as
// its bytes need (unit/window/window.h says why for each); a current GCC gets
// them here.
void WindowSprite_AppendToList(struct WindowSpriteList *list,
                               struct WindowSprite *node);
struct TextPrinter *TextPrinter_Add(struct Window *window, s32 entry, s32 x,
                                    s32 y, u16 *spacing, s32 flags);
void Text_DrawWideStringInWindow(s16 *text, struct Window *window, s32 x,
                                 s32 y);
void Window_PutGlyph(struct Window *win, u32 tile, u32 x, u32 y, s32 mode);
void Text_DrawPrefixedNumberAtOffset(s32 value, const struct Window *work,
                                     s32 offsetX, s32 offsetY, s32 variant);
#endif

// Edition-specific text and window layout values: places and widths in
// tiles, except where a note says pixels.
//
// The Djinn preview's columns of old values (HP and PP, then Attack, Defense
// and Agility), and how many leading characters of the 11-character number
// text the stats skip: 7 leaves four digits, 8 three. The help line under
// them ("R: Psynergy Gained") is placed in pixels.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DJINN_PREVIEW_VITALS_X 4
#else
#define DJINN_PREVIEW_VITALS_X 5
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 7
#define DJINN_PREVIEW_STAT_X 4
#else
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 8
#define DJINN_PREVIEW_STAT_X 6
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define DJINN_PREVIEW_HELP_X 40
#else
#define DJINN_PREVIEW_HELP_X 24
#endif

// The second of the two UI tiles before a Psynergy's PP cost in the lists
// (the first is tile 0x1f).
#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define PSYNERGY_COST_SECOND_TILE 0xf005
#else
#define PSYNERGY_COST_SECOND_TILE 0xf01e
#endif

// The summary screen's level label, after the member's name, in pixels.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define OWNER_STATUS_LEVEL_X 51
#else
#define OWNER_STATUS_LEVEL_X 56
#endif

// The summon list's window.
#if LANGUAGE != LANGUAGE_JAPANESE
#define SUMMON_MENU_WINDOW_X 13
#define SUMMON_MENU_WINDOW_WIDTH 17
#else
#define SUMMON_MENU_WINDOW_X 16
#define SUMMON_MENU_WINDOW_WIDTH 14
#endif

// While a member's commands are chosen (Battle_CollectPartyCommands): the
// screen x of the member's portrait sprite, in pixels.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_CLASS_ICON_X 0
#else
#define BATTLE_CLASS_ICON_X 4
#endif

// Where "Attack" is written in its window once chosen, in pixels.
#if (LANGUAGE == LANGUAGE_FRENCH)
#define BATTLE_COMMAND_NAME_X 14
#else
#define BATTLE_COMMAND_NAME_X 16
#endif

// The screen x of the chosen command's icon, in pixels, while the summon
// list is open.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_SUMMON_ICON_X 80
#else
#define BATTLE_SUMMON_ICON_X 104
#endif

// The chosen summon's window, and the columns of its first Djinn cost: the
// element icon, then the count.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_SUMMON_WINDOW_WIDTH 17
#else
#define BATTLE_SUMMON_WINDOW_WIDTH 14
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_SUMMON_ELEMENT_COLUMN 13
#define BATTLE_SUMMON_COST_COLUMN 14
#else
#define BATTLE_SUMMON_ELEMENT_COLUMN 10
#define BATTLE_SUMMON_COST_COLUMN 11
#endif

// The chosen item's window.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define BATTLE_ITEM_WINDOW_WIDTH 16
#else
#define BATTLE_ITEM_WINDOW_WIDTH 15
#endif

// The password level menu (IconMenu_ChooseSendOption,
// RESOURCE_LAYOUT_PASSWORD_LEVEL): its heading window ("Password Selection")
// and, outside the Japanese edition, the explanation window under it.
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define RESOURCE_MODE0_WINDOW_X 7
#define RESOURCE_MODE0_WINDOW_WIDTH 17
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define RESOURCE_MODE0_WINDOW_X 5
#define RESOURCE_MODE0_WINDOW_WIDTH 21
#elif (LANGUAGE == LANGUAGE_FRENCH)
#define RESOURCE_MODE0_WINDOW_X 5
#define RESOURCE_MODE0_WINDOW_WIDTH 22
#else
#define RESOURCE_MODE0_WINDOW_X 9
#define RESOURCE_MODE0_WINDOW_WIDTH 13
#endif

#if (LANGUAGE == LANGUAGE_GERMAN)
#define RESOURCE_MODE1_WINDOW_X 1
#define RESOURCE_MODE1_WINDOW_WIDTH 28
#else
#define RESOURCE_MODE1_WINDOW_X 3
#define RESOURCE_MODE1_WINDOW_WIDTH 25
#endif

// The transfer method menu (IconMenu_ChooseSendOption,
// RESOURCE_LAYOUT_TRANSFER_METHOD): its heading window ("Transfer Method"), the
// heading's x in it in pixels, and the help window.
#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_GERMAN)
#define TRANSFER_METHOD_WINDOW_X 6
#define TRANSFER_METHOD_WINDOW_WIDTH 18
#elif (LANGUAGE == LANGUAGE_SPANISH)
#define TRANSFER_METHOD_WINDOW_X 4
#define TRANSFER_METHOD_WINDOW_WIDTH 21
#elif (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TRANSFER_METHOD_WINDOW_X 5
#define TRANSFER_METHOD_WINDOW_WIDTH 20
#else
#define TRANSFER_METHOD_WINDOW_X 9
#define TRANSFER_METHOD_WINDOW_WIDTH 13
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
#define TRANSFER_METHOD_LABEL_X 2
#else
#define TRANSFER_METHOD_LABEL_X 0
#endif

#if (LANGUAGE == LANGUAGE_ENGLISH) || (LANGUAGE == LANGUAGE_SPANISH) ||        \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define TRANSFER_HELP_WINDOW_X 1
#define TRANSFER_HELP_WINDOW_WIDTH 28
#elif (LANGUAGE == LANGUAGE_GERMAN)
#define TRANSFER_HELP_WINDOW_X 0
#define TRANSFER_HELP_WINDOW_WIDTH 30
#else
#define TRANSFER_HELP_WINDOW_X 5
#define TRANSFER_HELP_WINDOW_WIDTH 21
#endif

// The width inside the frame of the name window of the shop's commands
// (icons 0x11 to 0x14) and of the Sanctum's services (icons 0x19 to 0x1c).
#if (LANGUAGE == LANGUAGE_GERMAN)
#define ENTRY11_COMMAND_LABEL_WIDTH 8
#elif LANGUAGE != LANGUAGE_JAPANESE
#define ENTRY11_COMMAND_LABEL_WIDTH 7
#else
#define ENTRY11_COMMAND_LABEL_WIDTH 6
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
#define ENTRY19_COMMAND_LABEL_WIDTH 10
#else
#define ENTRY19_COMMAND_LABEL_WIDTH 8
#endif

/// OAM attribute bits a window sprite is created with: the size field of
/// attribute 1 (bits 30-31 of the first word). A square sprite of size 1 is
/// 16x16 pixels, of size 2 32x32.
#define OUTPUT_SIZE_16X16 0x40000000
#define OUTPUT_SIZE_32X32 0x80000000

// The two portrait slots of the UI work (WindowWork.portraitPictures and
// portraitSprites), and their palette.
#define PORTRAIT_SLOT_COUNT 2
#define PORTRAIT_PALETTE 14 ///< OBJ palette of slot 0; slot 1 uses 15
/// portraitPictures value of a free slot, as Portrait_ClearSlots writes it.
#define PORTRAIT_SLOT_EMPTY 999

// The battle menus' message buffers, in halfwords, and the copier that fills
// them: the Japanese edition decodes the message (Message_DecodeRaw), the
// others copy its text (Message_CopyExpanded). The callers' buffers hold
// 64 halfwords either way.
#if LANGUAGE != LANGUAGE_JAPANESE
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY Message_CopyExpanded
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY Message_DecodeRaw
#endif

/// Tiles the battle lists draw over their window's top edge: the page numbers
/// and arrows, and the summon list's element icon. The digit and arrow tiles
/// are built when the party's commands open (Battle_CollectPartyCommands).
enum {
  /// Element icon: + enum Element, in BG palette 5.
  LIST_TILE_SUMMON_ELEMENT = 0x5001,
  /// Page n's number: + n gives digit n + 1, tile 0x301 on, the UI digits
  /// with colour 1 drawn as 10 (Graphics_ExpandVramTilesByColorTable).
  LIST_TILE_PAGE_NUMBER = 0xf301,
  /// The current page's number, the same digits in their own colours.
  LIST_TILE_PAGE_NUMBER_LIT = 0xf30b,
  /// Left arrow, tile 0x334, which Link_DrawShiftedTilePair slides.
  LIST_TILE_PAGES_LEFT = 0xf334,
  LIST_TILE_PAGES_RIGHT = 0xf335 ///< Right arrow, tile 0x335
};

/// The two battle command rows BattleCommandRow_Run runs.
enum CommandRowMode {
  COMMAND_ROW_PARTY = 0, ///< the start of a round: Fight, Flee and Status
  COMMAND_ROW_MEMBER = 1 ///< a member's commands; any mode but 0 is this row
};

/// What BattleTarget_RunSelection shows on a party member for the action
/// being aimed (Battle_ClassifyEntryKind), and which member it starts on.
enum BattleTargetHint {
  TARGET_HINT_NONE = 0,
  TARGET_HINT_HP = 1, ///< heals HP: the member's HP
  TARGET_HINT_PP = 2, ///< restores PP: the member's PP
  /// Cures poison (ABILITY_EFFECT_CURE_POISON): starts on a poisoned member.
  TARGET_HINT_CURE_POISON = 3,
  /// Cures what Restore cures (ABILITY_EFFECT_RESTORE): starts on a member
  /// who has one of those statuses, and lists them.
  TARGET_HINT_RESTORE = 4,
  /// Revives (Battle_IsReviveEffectFar): starts on a downed member.
  TARGET_HINT_REVIVE = 5,
  /// Cures Restore's statuses, poison and the evil spirit, as Tonic does
  /// (ABILITY_EFFECT_CURE_ALL).
  TARGET_HINT_CURE_ALL = 6,
  TARGET_HINT_SET_DJINNI = 7 ///< setting a Djinni: "Set Djinni"
};

#define BATTLE_OK 0x80 ///< AbilityDefinition.flags: usable in battle

/// One icon of the command row (struct SelectionWork): its sprite, where it
/// stands and the command it stands for. pictures.c loads it, command_row.c
/// runs the row.
struct SelectionEntry {
  /// The icon as a 32x32 sprite, drawn only while it is selected (pulsing);
  /// the others show as tiles of BG1.
  struct MenuSprite sprite;
  /// resource: the VRAM cache entry its sprite's tiles go into, one 1 KB
  /// entry that every icon of the row shares, reloaded with the selected
  /// icon. unused is never read.
  u16 resource, unused;
  /// Screen pixels: the sprite's top left before the pulse moves it.
  s32 positionX, positionY;
  s32 kind; ///< its command: MsgCommandName + kind names it
};

/// The command row's work block: the icons' shared palette, up to six icons,
/// the label window, the pulsing icon's transform and the selection.
/// BattleCommandRow_Run takes it from the EWRAM pool and frees it on
/// return.
struct SelectionWork {
  /// BG palette slots for the icons' 256-colour pixels, filled as each icon
  /// is loaded (Text_LoadRemappedGlyph).
  struct GlyphPalette palette;
  struct SelectionEntry entries[6];
  struct Window *window; ///< shows the selected command's name
  s32 iconCount;
  /// The selected icon's scale, 8.8 fixed point: it pulses between 304 and
  /// 336 (1.19 and 1.31 times) every 16 frames.
  struct AffineTransform affine;
  /// affineIndex: the OAM matrix built from affine this frame. selectedIndex:
  /// the icon under the cursor. drawnIndex: the icon whose name the window
  /// shows, -1 before the first is drawn.
  s32 affineIndex, selectedIndex, drawnIndex;
  /// The commands in row order, ended by -1 when fewer than six (see
  /// BattleCommandRow_Run for the kinds).
  s32 commandKinds[6];
};

// The Djinn menu's blinking tile, and where each element's Djinn count sits in
// the party status window, in tiles, to blink while the Djinni under the
// cursor is of that element, on standby and ready to summon (windata.s).
/// The arrow tile BattleDjinnMenu_Run copies to BG tiles 0x328 and 0x329 each
/// frame: 0x328 blinks (four frames shown, four blank), 0x329 stays. The
/// Djinn preview draws one of them, flipped, between the old and new class:
/// the blinking one when the class changes.
extern const u8 gDjinnMenu_BlinkTile[32];
extern const u8 gDjinnMenu_ElementXOffsets[4];
extern const u8 gDjinnMenu_ElementYOffsets[4];

#endif

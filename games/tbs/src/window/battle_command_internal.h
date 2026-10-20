#ifndef ALCHEMY_BATTLE_COMMAND_INTERNAL_H
#define ALCHEMY_BATTLE_COMMAND_INTERNAL_H

/// @file
/// What every part of the menu object shares: its includes, edition values,
/// macros and the link round state. battle_command.c includes it before the
/// parts, and each part includes it too, so that a part read on its own still
/// resolves.

#include "render_placement.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/object/object.h"
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
#include "callback_scheduler.h"
#include "dma.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "global_cells.h"
#include "menu_list.h"
#include "render_input.h"
#include "resource.h"
#include "output.h"
#include "glyph.h"
#include "game_state.h"
#include "battle_work.h"
#include "heap_state.h"
#include "text_font.h"
#include "text_render_runtime.h"
#include "fixed_math.h"
#include "runtime_interfaces.h"
#include "resource_ids.h"
#include "ramcode.h"
#include "affine.h"
#include "battle_party.h"
#include "scene.h"
#include "text_reader.h"
#include "lib.h"
#include "battle_unit.h"
#include "battle_system.h"
#include "item.h"
#include "vram_block.h"
#include "select.h"
#include "node_chain.h"
#include "text/msg_ids.h"
#include "songs.h"
#include "flags.h"
#include "static_assert.h"
#include "field_event.h"
#include "b5_context.h"
#include "battle_type.h"
#include "uiflow.h"
#include "owner_state.h"
#include "workspace_options.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "save_state.h"
#include "irq.h"
#include "character_menu.h"
#include "animspr.h"
#include "four_object_motion.h"
#include "uiwindow.h"
#include "io_write_queue.h"
#include "map_scroll.h"
#include "vram_tab.h"
#include "djinn_preview.h"
#include "ui.h"
#include "motobj.h"
#include "effect_step.h"
#include "resource_menu.h"
#include "battle_summon.h"
#include "serial_runtime.h"
#include "battle_effect_work.h"
#include "shop.h"
#include "display_transition.h"
#include "frame.h"
#include "unit/object/psynergy_effects.h"
#include "unit/window/scales.h"

#ifdef MODERN
// The menu object calls these window functions with no prototype in scope, as
// its bytes need (unit/window/window.h says why for each); a current GCC gets
// them here.
void RenderOutput_AppendToList(struct RenderOutputList *list,
                               struct RenderOutput *node);
struct UiChannelSlot *UiText_QueueRenderEntries(struct UiWindow *window,
                                                s32 entry, s32 x, s32 y,
                                                u16 *spacing, s32 flags);
void UiText_RenderWideStringInWindow(s16 *text, struct UiWindow *window, s32 x,
                                     s32 y);
void UiWindow_PutGlyph(struct UiWindow *win, u32 tile, u32 x, u32 y, s32 mode);
void UiText_DrawPrefixedNumberAtOffset(s32 value, const struct UiWindow *work,
                                       s32 offsetX, s32 offsetY, s32 variant);
#endif

// Edition-specific text and window layout values.
#if !TBS_PATCH_1
#define DJINN_PREVIEW_VITALS_X 4
#else
#define DJINN_PREVIEW_VITALS_X 5
#endif

#if !TBS_PATCH_1
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 7
#define DJINN_PREVIEW_STAT_X 4
#else
#define DJINN_PREVIEW_STAT_DIGIT_OFFSET 8
#define DJINN_PREVIEW_STAT_X 6
#endif

#if !TBS_PATCH_1
#define DJINN_PREVIEW_HELP_X 40
#else
#define DJINN_PREVIEW_HELP_X 24
#endif

#if (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||         \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define PSYNERGY_COST_SECOND_TILE 0xf005
#else
#define PSYNERGY_COST_SECOND_TILE 0xf01e
#endif

#if (LANGUAGE == LANGUAGE_FRENCH)
#define OWNER_STATUS_LEVEL_X 51
#else
#define OWNER_STATUS_LEVEL_X 56
#endif

#if TBS_PATCH_1
#define SUMMON_MENU_WINDOW_X 13
#define SUMMON_MENU_WINDOW_WIDTH 17
#else
#define SUMMON_MENU_WINDOW_X 16
#define SUMMON_MENU_WINDOW_WIDTH 14
#endif

#if TBS_PATCH_1
#define BATTLE_CLASS_ICON_X 0
#else
#define BATTLE_CLASS_ICON_X 4
#endif

#if (LANGUAGE == LANGUAGE_FRENCH)
#define BATTLE_COMMAND_NAME_X 14
#else
#define BATTLE_COMMAND_NAME_X 16
#endif

#if TBS_PATCH_1
#define BATTLE_SUMMON_ICON_X 80
#else
#define BATTLE_SUMMON_ICON_X 104
#endif

#if TBS_PATCH_1
#define BATTLE_SUMMON_WINDOW_WIDTH 17
#else
#define BATTLE_SUMMON_WINDOW_WIDTH 14
#endif

#if TBS_PATCH_1
#define BATTLE_SUMMON_ELEMENT_COLUMN 13
#define BATTLE_SUMMON_COST_COLUMN 14
#else
#define BATTLE_SUMMON_ELEMENT_COLUMN 10
#define BATTLE_SUMMON_COST_COLUMN 11
#endif

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define BATTLE_ITEM_WINDOW_WIDTH 16
#else
#define BATTLE_ITEM_WINDOW_WIDTH 15
#endif

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

#if TBS_PATCH_1
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

#if (LANGUAGE == LANGUAGE_GERMAN)
#define ENTRY11_COMMAND_LABEL_WIDTH 8
#elif TBS_PATCH_1
#define ENTRY11_COMMAND_LABEL_WIDTH 7
#else
#define ENTRY11_COMMAND_LABEL_WIDTH 6
#endif

#if TBS_PATCH_1
#define ENTRY19_COMMAND_LABEL_WIDTH 10
#else
#define ENTRY19_COMMAND_LABEL_WIDTH 8
#endif

/// OAM attribute bits a sprite output is created with: the size field of
/// attribute 1 (bits 30-31 of the first word). A square sprite of size 1 is
/// 16x16 pixels, of size 2 32x32.
#define OUTPUT_SIZE_16X16 0x40000000
#define OUTPUT_SIZE_32X32 0x80000000

// The two portrait slots of the UI work, and the palette the first one uses
// (the second uses the next).
#define PORTRAIT_SLOT_COUNT 2
#define PORTRAIT_PALETTE 14
#define PORTRAIT_SLOT_EMPTY 999

// The battle menus' message buffers, in halfwords, and the copier that fills
// them.
#if TBS_PATCH_1
#define BATTLE_MENU_MESSAGE_CAPACITY 52
#define BATTLE_MENU_MESSAGE_COPY UiText_CopyMessageString
#else
#define BATTLE_MENU_MESSAGE_CAPACITY 32
#define BATTLE_MENU_MESSAGE_COPY UiText_DecodeMessage
#endif

/// Tiles the battle lists draw over their window's top edge: the page numbers
/// and arrows, and the summon list's element icon.
enum {
  SUMMON_ELEMENT_TILE = 0x5001,       ///< Element icon
  LIST_TILE_PAGE_NUMBER = 0xf301,     ///< Page number
  LIST_TILE_PAGE_NUMBER_LIT = 0xf30b, ///< Current page number
  LIST_TILE_PAGES_LEFT = 0xf334,      ///< Left arrow
  LIST_TILE_PAGES_RIGHT = 0xf335      ///< Right arrow
};

#define BATTLE_OK 0x80 ///< flags: usable in battle

/// The command icon row's work block and one of its icons: pictures.c loads
/// the icons, command_row.c runs the row.
struct SelectionEntry {
  struct MenuSprite sprite;
  u16 resource, unused;
  s32 posX, posY;
  s32 kind;
};

/// The command icon row's work block: the icons' shared palette, up to six
/// icons, the label window, the pulsing icon's transform and the selection.
struct SelectionWork {
  struct GlyphPalette palette;
  struct SelectionEntry entries[6];
  struct UiWindow *window;
  s32 iconCount;
  struct AffineTransform affine;
  s32 affineIndex, selectedIndex, drawnIndex;
  s32 commandKinds[6];
};

// The Djinn menu's blinking tile, and where each element's Djinn count sits in
// the party status window, in tiles, to blink while it has Djinn on standby
// (windata.s).
extern const u8 gDjinnMenu_BlinkTile[32];
extern const u8 gDjinnMenu_ElementXOffsets[4];
extern const u8 gDjinnMenu_ElementYOffsets[4];

#endif

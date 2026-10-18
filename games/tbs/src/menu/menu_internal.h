#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

// What every part of the menu object shares: its includes, and the macros
// and types more than one part uses. menu.c includes it before the parts,
// and each part includes it too, so that a part read on its own resolves. It
// places no bytes.

#include "render_placement.h"
#include "unit/menu/menu.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"
#include "uiwindow.h"
#include "mem.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "battle_system.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "fixed_math.h"
#include "lib.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"
#include "keystate.h"
#include "window.h"
#include "menu_result.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "songs.h"
#include "iwram_call.h"
#include "heap_state.h"
#include "menu_list.h"
#include "irq.h"
#include "object_factory.h"
#include "far_runtime.h"
#include "equipment_menu.h"
#include "owner_state.h"
#include "inventory.h"
#include "static_assert.h"
#include "battle_type.h"
#include "battle_calc.h"
#include "event_work.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "battle_effect.h"
#include "four_object_motion.h"
#include "item_ids.h"
#include "macros.h"
#include "flags.h"

extern u8 IwramCopyWords[];

s32 _call_via_r3(void *, void *, s32, void *);

// BattleAct.flags: where an ability or item can be used, as the Psynergy
// list says ("Usable in the field", "Usable in battle").
#define ACTION_USABLE_IN_FIELD 0x40
#define ACTION_USABLE_IN_BATTLE 0x80

// Game flags of the menus. Start was pressed: every menu screen unwinds and
// the whole menu closes.
#define FLAG_MENU_START_PRESSED 0x150
// The info window holds a message the next selection must not draw over.
#define FLAG_MENU_INFO_KEPT 0x151

// The target id of a Psynergy or item used on the whole party.
#define MENU_TARGET_WHOLE_PARTY 9

// Inventory icons occupy five columns in the fifteen-slot list.
#define MENU_ITEM_GRID_COLUMNS 5

// What Menu_DrawOwnerStatusPanel shows below the name and ailments: the low
// byte of its mode.
enum StatusPanelKind {
  STATUS_PANEL_EXPERIENCE = 0, // class, HP, PP and experience
  STATUS_PANEL_EQUIP = 2,      // stats if the item were equipped
  STATUS_PANEL_LEARN = 4,      // the Psynergy a Psynergy item teaches
  STATUS_PANEL_CLASS = 6,      // class, HP and PP
  STATUS_PANEL_STATS = 8,      // Attack, Defense, Agility and Luck
};
#define STATUS_PANEL_KIND_MASK 0xff
// Keeps the open window, the name and the ailments, and redraws only the
// part below them.
#define STATUS_PANEL_LOWER_ONLY 0x100

// The revised information pane restores its tiles and draws directly into them.
#if TBS_PATCH_1
#define MENU_RESET_INFO_WINDOW RenderOutput_RedrawSavedRectFar
#define MENU_DRAW_INFO_TEXT UiText_DrawCharacterAtOffsetFar
#else
#define MENU_RESET_INFO_WINDOW RenderOutput_ClearListFar
#define MENU_DRAW_INFO_TEXT UiText_DrawResourceFar
#endif

// Status labels use the Latin font's pixel positions or the Japanese tile grid.
#if LANGUAGE == LANGUAGE_JAPANESE
#define MENU_DRAW_STATUS_LABEL UiText_DrawStringInWindowFar
#else
#define MENU_DRAW_STATUS_LABEL UiText_DrawStringAtOffsetFar
#endif

// Text colours of the status panel's numbers.
#define TEXT_COLOR_LOW_HP 4 // HP under a quarter of the maximum
#define TEXT_COLOR_NO_HP 2  // HP 0
#define TEXT_COLOR_NORMAL 15

// RenderOutput.active as the menu sets it: shown, or hidden but kept.
#define ICON_SHOWN 1
#define ICON_HIDDEN 13

// RenderOutput_CreateFar's sprite attributes for the menu's 16x16 icons.
#define ICON_SPRITE_16X16 0x40000000

// The preview loop's four operations, as its confirmation text describes them.
enum DjinnCompareOperation {
  DJINN_COMPARE_TRADE = 0,
  DJINN_COMPARE_GIVE = 1,
  DJINN_COMPARE_STANDBY = 2,
  DJINN_COMPARE_SET = 3
};

#endif

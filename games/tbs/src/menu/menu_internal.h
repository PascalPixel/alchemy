#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

/// @file
/// What every part of the menu object shares: its includes, and the macros
/// and types more than one part uses. menu.c includes it before the parts,
/// and each part includes it too, so that a part read on its own resolves. It
/// places no bytes.

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

/// BattleAct.flags: where an ability or item can be used, as the Psynergy
/// list says ("Usable in the field", "Usable in battle"). The same values as
/// enum BattleActionUse (battle_unit.h), which these macros shadow here.
#define ACTION_USABLE_IN_FIELD 0x40
#define ACTION_USABLE_IN_BATTLE 0x80

/// Game flags of the menus. Start was pressed: every menu screen unwinds and
/// the whole menu closes. Menu_CancelSoundReset clears it and
/// Menu_CancelSoundTick sets it.
#define FLAG_MENU_START_PRESSED 0x150
/// The info window holds a message the next selection must not draw over.
/// InventoryMenu_ShowModalMessage sets it; the next chooser that would draw
/// into the info window clears it instead (see the BUG in
/// PsynergyMenu_DrawDetailPage).
#define FLAG_MENU_INFO_KEPT 0x151

/// The target id the Psynergy screen passes for an action whose range is
/// ABILITY_RANGE_ALL. It is no unit's id: BattleEffect_ApplyToTargets starts
/// from unit 0 in its place, and visits every member because of the range, not
/// this id. The Item screen never passes it: a whole-party item is used with
/// the work's paneOwner[1] as it stands (ItemMenu_Use), and its range alone
/// reaches every member.
#define MENU_TARGET_WHOLE_PARTY 9

/// The result of using an item or Psynergy from the menu: its message,
/// counted from MsgItemUseResult (BattleEffect_ApplyToTargets).
enum ItemUseResult {
  ITEM_USE_RESULT_HP_FULLY_RECOVERED = 0, ///< "You recovered all HP!"
  ITEM_USE_RESULT_HP_RECOVERED = 1,       ///< "You recovered HP!"
  ITEM_USE_RESULT_HP_NOT_RECOVERED = 2,   ///< "You couldn't recover HP."
  ITEM_USE_RESULT_PARTY_HP_RECOVERED = 3, ///< "Everyone recovered HP!"
  ITEM_USE_RESULT_HP_FULL = 4,            ///< "Your HP is maxed out!"
  ITEM_USE_RESULT_PP_FULLY_RECOVERED = 5, ///< "You recovered all PP!"
  ITEM_USE_RESULT_PP_RECOVERED = 6,       ///< "You recovered some PP!"
  ITEM_USE_RESULT_PP_FULL = 7,            ///< "Your PP is maxed out!"
  ITEM_USE_RESULT_PARTY_PP_RECOVERED = 8, ///< "Everyone recovered PP!"
  /// A second "Your PP is maxed out!", which no use chooses.
  ITEM_USE_RESULT_PP_FULL_UNUSED = 9,
  ITEM_USE_RESULT_POISON_PURGED = 10, ///< "The poison was purged!"
  ITEM_USE_RESULT_NOT_POISONED = 11,  ///< "You're not poisoned."
  ITEM_USE_RESULT_REVIVED = 12,       ///< "You were revived!"
  ITEM_USE_RESULT_NOT_DOWN = 13,      ///< "You're not down."
  /// "The curse was removed." and "You are not cursed.", which no use chooses.
  ITEM_USE_RESULT_CURSE_REMOVED = 14,
  ITEM_USE_RESULT_NOT_CURSED = 15,
  ITEM_USE_RESULT_MAX_HP_UP = 16,  ///< "Your maximum HP increased!"
  ITEM_USE_RESULT_MAX_PP_UP = 17,  ///< "Your maximum PP increased!"
  ITEM_USE_RESULT_AGILITY_UP = 18, ///< "Your Agility increased!"
  ITEM_USE_RESULT_LUCK_UP = 19,    ///< "Your Luck increased!"
  ITEM_USE_RESULT_ATTACK_UP = 20,  ///< "Your Attack increased!"
  ITEM_USE_RESULT_DEFENSE_UP = 21  ///< "Your Defense increased!"
};

/// The bytes of the menu work, heap block HEAP_SLOT_MENU. Each screen's run
/// function takes the block as it opens and releases it as it closes.
#define MENU_WORK_SIZE 0xa70

/// Inventory icons occupy five columns in the fifteen-slot list.
#define MENU_ITEM_GRID_COLUMNS 5

/// What Menu_DrawOwnerStatusPanel shows below the name and ailments: the low
/// byte of its mode.
enum StatusPanelKind {
  STATUS_PANEL_EXPERIENCE = 0, ///< class, HP, PP and experience
  /// Stats if the item were equipped, or removed when equipPreview is set.
  /// Kind 3 draws the same.
  STATUS_PANEL_EQUIP = 2,
  STATUS_PANEL_LEARN = 4, ///< the Psynergy a Psynergy item teaches
  STATUS_PANEL_CLASS = 6, ///< class, HP and PP
  STATUS_PANEL_STATS = 8, ///< Attack, Defense, Agility and Luck
};
/// The panel kind in Menu_DrawOwnerStatusPanel's mode; the bits above it are
/// STATUS_PANEL_LOWER_ONLY.
#define STATUS_PANEL_KIND_MASK 0xff
/// Keeps the open window, the name and the ailments, and redraws only the
/// part below them.
#define STATUS_PANEL_LOWER_ONLY 0x100

// The information pane's text: outside Japanese it is drawn character by
// character into the window's tiles, so clearing it redraws the empty frame;
// the Japanese edition decodes it into the text canvas (UiText_DrawResource),
// so clearing it releases the canvas's render outputs. Both follow the text
// system, so the test is on the language.
#if LANGUAGE != LANGUAGE_JAPANESE
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
#define TEXT_COLOR_LOW_HP 4  ///< HP under a quarter of the maximum
#define TEXT_COLOR_NO_HP 2   ///< HP 0
#define TEXT_COLOR_NORMAL 15 ///< the window text's default colour

/// RenderOutput.active as the menu sets it: shown, or hidden but kept.
#define ICON_SHOWN 1   ///< UI_SPRITE_STILL
#define ICON_HIDDEN 13 ///< UI_SPRITE_HIDDEN

/// RenderOutput_CreateFar's sprite attributes for the menu's 16x16 icons:
/// OAM attribute 0 in the low half (shape square) and attribute 1 in the high
/// half (size 1).
#define ICON_SPRITE_16X16 0x40000000

/// The preview loop's four operations, as its confirmation text describes them
/// (OwnerAction_RunCompareLoop's mode). Trade and Give, the first two, have
/// two members and the paged previews; Standby and Set one member.
enum DjinnCompareOperation {
  DJINN_COMPARE_TRADE = 0,
  DJINN_COMPARE_GIVE = 1,
  DJINN_COMPARE_STANDBY = 2,
  DJINN_COMPARE_SET = 3
};

#endif

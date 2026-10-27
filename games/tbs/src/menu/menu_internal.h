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
#include "task.h"
#include "dma.h"
#include "item_menu.h"
#include "io_reg.h"
#include "window.h"
#include "mem.h"
#include "status_screen.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "battle_system.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_cache.h"
#include "render_input.h"
#include "fixed_math.h"
#include "lib.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "m7_interfaces.h"
#include "heap_slots.h"
#include "keystate.h"
#include "paged_list.h"
#include "tbs_edition.h"
#include "save_block.h"
#include "songs.h"
#include "iwram_call.h"
#include "heap.h"
#include "menu_list.h"
#include "irq.h"
#include "actor_create.h"
#include "equipment_menu.h"
#include "unit.h"
#include "inventory.h"
#include "static_assert.h"
#include "battle_type.h"
#include "battle_calc.h"
#include "field_work.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "blitter.h"
#include "djinn_figures.h"
#include "item_ids.h"
#include "macros.h"
#include "flags.h"
#include "unit/window/message_speed.h"
#include "game/character_ids.h"
#include "ability_ids.h"

extern u8 IwramCopyWords[];

s32 _call_via_r3(void *, void *, s32, void *);

/// AbilityDefinition.flags: where an ability or item can be used, as the
/// Psynergy list says ("Usable in the field", "Usable in battle"). The same
/// values as enum AbilityUse (battle_unit.h), which these macros shadow here.
#define ABILITY_USABLE_IN_FIELD 0x40
#define ABILITY_USABLE_IN_BATTLE 0x80

/// Game flags of the menus. Start was pressed: every menu screen unwinds and
/// the whole menu closes. Menu_CancelSoundReset clears it and
/// Menu_CancelSoundTick sets it.
#define FLAG_MENU_START_PRESSED 0x150
/// The info window holds a message the next selection must not draw over.
/// ItemMenu_ShowMessage sets it; the next chooser that would draw
/// into the info window clears it instead (see the BUG in
/// PsynergyMenu_DrawDetailPage).
#define FLAG_MENU_INFO_KEPT 0x151

/// The target id the Psynergy screen passes for an action whose range is
/// ABILITY_RANGE_ALL. It is no unit's id: Ability_ApplyToTargets starts
/// from unit 0 in its place, and visits every party member because of the
/// range, not this id. The Item screen never passes it: a whole-party item is
/// used with the work's sideMember[1] as it stands (ItemMenu_Use), and its
/// range alone reaches every party member.
#define MENU_TARGET_WHOLE_PARTY 9

/// The result of using an item or Psynergy from the menu: its message,
/// counted from MsgItemUseResult (Ability_ApplyToTargets).
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

/// StatusScreenWork.page: which screen the party member row belongs to.
enum StatusScreenPage {
  STATUS_SCREEN_PAGE_STATUS = 0, ///< the Status menu (StatusScreen_Run)
  /// The party member chooser (MemberChooser_Open).
  STATUS_SCREEN_PAGE_CHOOSER = 3
};

/// PsynergyMenuWork.mode: what choosing a Psynergy does.
enum PsynergyMenuMode {
  PSYNERGY_MODE_USE = 0,   ///< use it
  PSYNERGY_MODE_SET_L = 1, ///< make it the L shortcut
  PSYNERGY_MODE_SET_R = 2  ///< make it the R shortcut
};

/// The Psynergy shortcuts (PsynergyMenu_SetShortcut).
enum PsynergyShortcut { PSYNERGY_SHORTCUT_L = 0, PSYNERGY_SHORTCUT_R = 1 };

/// Which Psynergy PsynergyMenu_CollectActions lists.
enum PsynergyListMode {
  /// Those with a field effect or usable in the field first, then the rest.
  PSYNERGY_LIST_ALL = 0,
  PSYNERGY_LIST_SHORTCUTS = 1, ///< only those that work on the field
  /// Those with a field effect or usable in the field.
  PSYNERGY_LIST_FIELD = 2
};

/// How the item menu can use an item (Item_ClassifyUseMode).
enum ItemUseMode {
  /// It cannot be used here: it has no action, it is equipment the party member
  /// cannot wear or that teaches Psynergy, or its action is usable only in
  /// battle.
  ITEM_USE_MODE_UNUSABLE = -1,
  /// It works on the field (Scene_HasPsynergyTriggerFar), or its action is
  /// usable
  /// neither in the field nor in battle, and is used once the menu closes.
  ITEM_USE_MODE_FROM_FIELD = 0,
  ITEM_USE_MODE_CHOOSE_MEMBER = 1, ///< choose a party member to use it on
  ITEM_USE_MODE_WHOLE_PARTY = 2 ///< ABILITY_RANGE_ALL: the whole party at once
};

/// Whom ItemMenu_SelectTarget chooses.
enum ItemTargetMode {
  ITEM_TARGET_USE = 0, ///< the party member to use the item on
  ITEM_TARGET_GIVE = 1 ///< the party member to give it to
};

/// The bytes of the menu work, heap block HEAP_SLOT_MENU. Each screen's run
/// function takes the block as it opens and releases it as it closes.
#define MENU_WORK_SIZE 0xa70

/// Inventory icons occupy five columns in the fifteen-slot list.
#define MENU_ITEM_GRID_COLUMNS 5

/// What StatusPanel_Draw shows below the name and statuses: the low
/// byte of its mode. The kinds go up in twos; no call passes an odd one, and of
/// the odd values only STATUS_PANEL_EQUIP + 1 draws anything.
enum StatusPanelKind {
  STATUS_PANEL_EXPERIENCE = 0, ///< class, HP, PP and experience
  /// Stats if the item were equipped, or removed when equipPreview is set.
  /// STATUS_PANEL_EQUIP + 1 draws the same.
  STATUS_PANEL_EQUIP = 2,
  STATUS_PANEL_LEARN = 4, ///< the Psynergy a Psynergy item teaches
  STATUS_PANEL_CLASS = 6, ///< class, HP and PP; no call passes it
  /// Attack, Defense, Agility and Luck, while R is held on the Item screen.
  STATUS_PANEL_STATS = 8,
};
/// The panel kind in StatusPanel_Draw's mode; the bits above it are
/// STATUS_PANEL_LOWER_ONLY.
#define STATUS_PANEL_KIND_MASK 0xff
/// Keeps the open window, the name and the statuses, and redraws only the
/// part below them.
#define STATUS_PANEL_LOWER_ONLY 0x100

// The information side's text: outside Japanese it is drawn character by
// character into the window's tiles, so clearing it redraws the empty frame;
// the Japanese edition decodes it into the text canvas (Text_DrawMessage),
// so clearing it releases the canvas's window sprites. Both follow the text
// system, so the test is on the language.
#if LANGUAGE != LANGUAGE_JAPANESE
#define MENU_RESET_DESCRIPTION_WINDOW WindowSprite_RedrawSavedRectFar
#define MENU_DRAW_DESCRIPTION Text_DrawMessageAsTilesFar
#else
#define MENU_RESET_DESCRIPTION_WINDOW WindowSprite_ClearListFar
#define MENU_DRAW_DESCRIPTION Text_DrawMessageFar
#endif

// Status labels use the Latin font's pixel positions or the Japanese tile grid.
#if LANGUAGE == LANGUAGE_JAPANESE
#define MENU_DRAW_STATUS_LABEL Text_DrawStringInWindowFar
#else
#define MENU_DRAW_STATUS_LABEL Text_DrawStringAtOffsetFar
#endif

// Text colours of the status panel's numbers.
#define TEXT_COLOR_LOW_HP                                                      \
  TEXT_COLOR_YELLOW                     ///< HP under a quarter of the maximum
#define TEXT_COLOR_NO_HP TEXT_COLOR_RED ///< HP 0
#define TEXT_COLOR_NORMAL TEXT_COLOR_WHITE ///< the window text's default colour

/// WindowSprite.motion as the menu sets it: shown, or hidden but kept.
#define ICON_SHOWN 1   ///< WINDOW_SPRITE_MOTION_STILL
#define ICON_HIDDEN 13 ///< WINDOW_SPRITE_MOTION_HIDDEN

/// The Djinn change DjinnMenu_DrawStatPreview previews, with the Djinn in the
/// work's sides removePairSlot and addPairSlot.
enum DjinnPreviewMode {
  DJINN_PREVIEW_ADD_AND_SET = 0, ///< the other's Djinni is added and set
  DJINN_PREVIEW_UNSET = 1,       ///< the chosen Djinni is unset
  /// A trade: the own Djinni is unset if set, the other's is added, and set
  /// if it was.
  DJINN_PREVIEW_TRADE = 2,
  DJINN_PREVIEW_NO_CHANGE = 3, ///< nothing changes; maximum HP and PP shown
  DJINN_PREVIEW_ADD = 4        ///< the other's Djinni is added, set if it was
};

/// PsynergyList_RunCompareLoop's pages for a give or trade, which R cycles; the
/// first two also index its per-member page counts.
enum DjinnComparePage {
  DJINN_COMPARE_PAGE_GIVER = 0,    ///< the giver's stats and Psynergy
  DJINN_COMPARE_PAGE_RECEIVER = 1, ///< the receiver's stats and Psynergy
  DJINN_COMPARE_PAGE_BOTH = 2      ///< both party members' stats, shown first
};

/// What the Djinn menu's choosers return. DjinnMenu_SelectDjinn's Djinni
/// chooser returns DJINN_RESULT_CHOSEN, DJINN_RESULT_STATUS or
/// DJINN_RESULT_HELP; its action chooser one of the commands. Every Djinn menu
/// screen returns DJINN_RESULT_BACK for B and DJINN_RESULT_START for Start.
enum DjinnMenuResult {
  DJINN_RESULT_START = -2, ///< Start: the Djinn menu closes
  DJINN_RESULT_BACK = -1,
  DJINN_RESULT_CHOSEN = 1,    ///< A on a Djinni
  DJINN_RESULT_CONFIRMED = 1, ///< A on the preview's "Is this OK?"
  DJINN_RESULT_SET = 1,
  DJINN_RESULT_STANDBY = 2,
  DJINN_RESULT_GIVE = 3,
  DJINN_RESULT_TRADE = 4,
  /// The four commands applied without their preview, which no chooser
  /// returns.
  DJINN_RESULT_SET_NOW = 5,
  DJINN_RESULT_STANDBY_NOW = 6,
  DJINN_RESULT_STATUS = 7, ///< L: the party member's status
  DJINN_RESULT_GIVE_NOW = 8,
  DJINN_RESULT_TRADE_NOW = 9,
  DJINN_RESULT_HELP = 10 ///< Select: the help
};

/// DjinnMenuBackdrop.tutorialStep: where the Djinn tutorial is. Each lesson's
/// steps follow its first in order, each waiting, showing a message or
/// pressing a key for the player (DjinnMenu_SelectDjinn,
/// PsynergyList_RunCompareLoop); the flags that pick the lesson are
/// FLAG_DJINN_LESSON_PART_A and _PART_B (DjinnMenu_Run).
enum DjinnTutorialStep {
  DJINN_TUTORIAL_OFF = 0,
  /// Giving a Djinni and setting it: "To give your Djinni to someone else..."
  DJINN_TUTORIAL_GIVE = 1,
  /// The Djinn's powers: "Each Djinni has different powers."
  DJINN_TUTORIAL_POWERS = 14,
  /// Recovery after a summon: "After a Djinni has been used to summon..."
  DJINN_TUTORIAL_RECOVERY = 27,
  /// Waits for the player's A, then presses B.
  DJINN_TUTORIAL_CLOSE = 28
};

/// DjinnMenu_SelectDjinn's two choosers.
enum DjinnSelectMode {
  DJINN_SELECT_DJINNI = 0, ///< a Djinni in any party member's column
  DJINN_SELECT_ACTION = 1  ///< what to do with the Djinni chosen
};

/// The window map entry of the first element icon, in palette 5; the four
/// elements follow it in order.
#define DJINN_ELEMENT_TILE 0x5001

/// A Djinn list entry's party member, element and index (DJINN_ENTRY_MEMBER,
/// _ELEMENT and _INDEX), shifted down.
#define DJINN_MEMBER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)

/// The preview loop's four operations, as its confirmation text describes them
/// (PsynergyList_RunCompareLoop's mode). Trade and Give, the first two, have
/// two party members and the paged previews; Standby and Set one party member.
enum DjinnCompareOperation {
  DJINN_COMPARE_TRADE = 0,
  DJINN_COMPARE_GIVE = 1,
  DJINN_COMPARE_STANDBY = 2,
  DJINN_COMPARE_SET = 3
};

#endif

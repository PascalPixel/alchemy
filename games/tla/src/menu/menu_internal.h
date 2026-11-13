#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

/// @file
/// What every part of the menu object shares: its includes and the declarations
/// its parts disagreed on. menu.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "tla_edition.h"
#include "version.h"
#include "dma.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "frame.h"
#include "input.h"
#include "fixed_math.h"
#include "resource.h"
#include "task.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "item.h"
#include "battle_unit.h"
#include "window.h"
#include "uiwindow.h"
#include "render_input.h"
#include "render_placement.h"
#include "sprite.h"
#include "m7_interfaces.h"
#include "static_assert.h"
#include "text/msg_ids.h"
#include "irq.h"
#include "owner_state.h"
#include "inventory.h"
#include "heap_release.h"
#include "menu_result.h"
#include "battle_type.h"
#include "battle_system.h"
#include "display_transition.h"
#include "a9_motion.h"
#include "battle_effect.h"
#include "djinn_menu.h"
#include "macros.h"
#include "vram_tab.h"

/// The item menu's work, Ram_HeapSlots->menuRuntime: The Broken Seal's
/// ItemMenuWork, whose names its fields take. From itemWindow to the entry
/// icons it lies 4 bytes further on than The Broken Seal's, and from
/// questionWindow on 28 bytes closer.
struct ItemMenuWork {
  u8 unused[8];
  s32 selectedMember; ///< the party member chosen
  u8 unk00c[4];
  struct RenderInput *mainWindow;
  struct WindowSprite *sideIcons[2];
  s8 sideIndex[2];
  s8 sideCount[2];
  u8 unk020[4];
  struct RenderInput *itemWindow;
  struct RenderInput *statusWindow;
  struct RenderInput *buttonHelpWindow;
  struct RenderInput *infoWindow;
  u8 unk034[4];
  struct RenderInput *listWindow;
  u8 unk03c[4];
  struct RenderInput *modalWindow;
  u8 unk044[8];
  struct WindowSprite *entryIcons[32];
  u8 unk0cc[0x24];
  struct RenderInput *questionWindow;
  u8 selectedColumn;
  u8 selectedRow;
  u8 columnCount;
  u8 rowCount;
  u8 unk0f8[0x30];
  s16 memberY[8]; ///< screen pixels
  u8 unk138[0x30];
  u16 selectedSlots[2]; ///< each side's inventory slot chosen
  u16 selectedItems[2]; ///< each side's inventory entry chosen
  struct WindowSprite *cursor;
  u8 unk174[4];
  struct WindowSprite *ownerIcons[2];
  u8 unk180[0x44];
  u16 items[32];    ///< the inventory entries listed
  s16 memberIds[8]; ///< the party in order (Party_ListMembersFar)
  u8 itemCount;
  u8 partyCount;
  u8 sideMember[2]; ///< each side's party member
  struct WindowSprite *selectedItemIcon;
  u8 unk21c[2];
  s16 completionFlag; ///< set to 1 as item commands end
  u8 unk220[0x3e];
  s16 messageOffset; ///< the item use's result, after MsgItemUseResult
  s8 equipPreview;   ///< 1 while the stats preview a removal, not an equip
  u8 listMode;       ///< the command chosen last; 0xff for none
};

STATIC_ASSERT(offsetof(struct ItemMenuWork, itemWindow) == 0x24);
STATIC_ASSERT(offsetof(struct ItemMenuWork, entryIcons) == 0x4c);
STATIC_ASSERT(offsetof(struct ItemMenuWork, questionWindow) == 0xf0);
STATIC_ASSERT(offsetof(struct ItemMenuWork, memberY) == 0x128);
STATIC_ASSERT(offsetof(struct ItemMenuWork, selectedSlots) == 0x168);
STATIC_ASSERT(offsetof(struct ItemMenuWork, items) == 0x1c4);
STATIC_ASSERT(offsetof(struct ItemMenuWork, sideMember) == 0x216);
STATIC_ASSERT(offsetof(struct ItemMenuWork, completionFlag) == 0x21e);
STATIC_ASSERT(offsetof(struct ItemMenuWork, messageOffset) == 0x25e);
STATIC_ASSERT(offsetof(struct ItemMenuWork, listMode) == 0x261);

struct SpriteAttr {
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

struct MenuIcon {
  struct MenuIcon *next; ///< the window sprite's list link
  s8 kind;
  u8 active;
  u16 x;
  u16 y;
  u8 unk00a[2];
  u16 animStep;
  u8 unk0e;
  u8 sentinel;
  u8 unk010[4];
  struct SpriteAttr oam;
};

struct MenuWindow {
  u8 unk000[0xc];
  u16 x;
  u16 y;
};

struct MenuArrow {
  u8 unk00[5];
  u8 mode;
  u8 unk06[6];
  u16 timer;
};

struct MenuPageWork {
  u8 unk000[0x4c];
  struct MenuPageIcon *icons[32];
  u8 unk0cc[0x5c];
  u16 colours[4];
  u8 unk130[0x48];
  struct MenuArrow *up;
  struct MenuArrow *down;
  u8 unk180[0x94];
  u8 entryCount;
  u8 unk215[3];
};

struct OwnerMenuState {
  u8 unk000[0x10];
  struct RenderInput *auxiliaryWindow;
  struct WindowSprite *sideIcon[2];
  s8 tabIndex[2];
  u8 unk01e[0xa];
  void *statusWindow;
  u8 unk02c[0xc4];
  void *selectorWindow;
  u8 unk0f4[4];
  struct AnimationObject *tabObjects[8];
  s16 tabX[8];
  s16 rowPositions[8];
  s32 ownerScale[8];
  u8 unk158[0x20];
  struct WindowSprite *arrows[2];
  u8 unk180[0x84];
  s16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
  u8 unk216[6];
  u16 page;
  u16 skipSlide;
};

struct InventoryMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[0x10];
  s8 paneIndex;
  u8 unk01d[7];
  s32 itemWindow;
  s32 statusWindow;
  u8 unk02c[4];
  void *infoWindow;
  s32 equipWindow;
  u8 unk038[0xb8];
  s32 messageWindow;
  u8 unk0f4[0x74];
  u16 selectedSlots[2];
  u16 selectedItems[2];
  u8 unk170[0x54];
  u16 items[32];
  s16 ownerIds[8];
  u8 itemCount;
  u8 partyCount;
  u8 paneOwner[2];
  struct WindowSprite *selectedItemIcon;
  u8 unk21c[0x42];
  u16 messageOffset;
  u8 unk260[4];
  s8 selectedIndexByOwner[8];
  u8 unk26c[0x282];
  s16 resourceSlots[2];
  u8 unk4f2[2];
};

/// The Psynergy menu's work (the menu heap slot) as these functions read it.
struct PsynergyMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  struct MenuWindow *rowWindow;
  struct MenuIcon *sideIcons[2];
  s8 tabIndex[2];
  s8 tabCount;
  u8 unk01f[5];
  s32 psynergyWindow;
  void *statusWindow;
  s32 shortcutWindow;
  void *infoWindow;
  u8 unk034[4];
  s32 listWindow;
  u8 unk03c[0xc];
  struct MenuIcon *entryGridCursor;
  struct MenuIcon *entryIcons[32];
  u8 unk0cc[0x24];
  s32 messageWindow;
  u8 unk0f4[4];
  void *tabObjects[8];
  u8 unk118[0x50];
  u16 paneRow[2];    ///< each pane's Psynergy row chosen
  u16 paneAction[2]; ///< each pane's Psynergy chosen, as a member action entry
  u8 unk170[8];
  struct MenuIcon *paneIcon[2];
  u8 unk180[0x44];
  u16 psynergies[32];
  u16 ownerIds[8];
  u8 psynergyCount; ///< the chosen member's Psynergy listed
  u8 memberCount;
  /// The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy on
  /// the whole party.
  u8 memberIds[2];
  struct MenuIcon *cursorIcon;
  /// bits: 0 set after a use or a cancelled target
  u16 flags;
  u16 skipSlide; ///< 1 skips the cursor's next slide once
  u8 unk220[0x3e];
  s16 useResult; ///< the use's result message, after MsgItemUseResult
  u8 unk260[4];
  s8 selectedIndexByOwner[8];
  u8 mode; ///< 0 to use Psynergy; 1 or 2 to set the L or R shortcut
  u8 unk26d[3];
};

struct DjinnUnit {
  u8 unk000[0xf8];
  u32 djinnHeld[4];
  u32 djinnSet[4];
};

// The menu object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
void StatusPanel_DrawHpPp(struct BattleUnit *unit, s32 window);
void Text_DrawStatComparison(const struct BattleUnit *alt,
                             const struct BattleUnit *base, s32 work);
void Text_DrawMessageAsTilesFar(s32 message, struct RenderInput *window, s32 x,
                                s32 y);

// Tags the parts name in prototypes before the part that defines them.
struct MenuEntryWindow;
struct MenuPanel;
struct OwnerList;

// What several parts call, declared once here for all of them.
s32 GameFlag_Test(s32);
void GameFlag_ClearBit(s32 flag_no);
s32 Inventory_EquipFar(s32 owner, s32 slot);
void Window_DrawBorderFar(s32 x, s32 y, s32 width, s32 height);
void Window_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Text_UseExtraTilesFar(s32 flag);
void Window_MarkVisibleTextTilesFar(void);
void Menu_CreateOwnerEntryObjects(s32 first);
void Menu_HighlightPageEntry(s32 unused, s32 index, s32 count);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);
void Resource_LoadByModeIntoSlotFar(s32 mode, s32 id, s32 slot, s32 flags);
extern u8 MsgShortcutHelp[];
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
void Link_DrawPageArrowsFar(s32 addr);
void Menu_UpdateEntryObjectTransforms(void);
void Input_ResetKeyRepeat(void);
s32 StatusScreen_BuildAvailability(u8 *ailments, s32 requested, s32 owner);
void ItemMenu_DrawCategory(s32 window, s32 ownerId, s32 mode);
s32 Ability_ApplyToTargets(s32 entry, s32 member, s32 target, s32 mode);
s32 Ability_PlayUseAnimation(s32 action);
void DjinnFigures_InitializeTopRow(void);
void DjinnFigures_ReleaseTopRow(void);
void Menu_ResetTwoResourceEntries(void);
extern u8 gMenu_BackdropBorderTile[];
void WindowSprite_PrepareForRedrawFar(s32 window);
s32 Message_AllTypedFar(void);
void DjinnFigures_SetSlotPhase(s32 index, s32 scale);
void MenuIcon_SetPosition(s32 slot, s32 x, s32 y);
void Func_08020018(struct AnimationObject *object,
                   struct SpritePlacement *position, struct SpriteScale *scale,
                   s32 depth, s32 mode);

#endif

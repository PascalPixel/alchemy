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
  u8 unk174[0x50];
  u16 items[32];    ///< the inventory entries listed
  s16 memberIds[8]; ///< the party in order (Party_ListActiveOwnersFar)
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

// The menu object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
void StatusPanel_DrawHpPp(struct BattleUnit *unit, s32 window);
void Text_DrawStatComparison(const struct BattleUnit *alt,
                             const struct BattleUnit *base, s32 work);
void Text_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                   s32 x, s32 y);

#endif

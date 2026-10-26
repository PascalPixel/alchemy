#include "types.h"
#include "edition.h"
#include "ram_buffer.h"
#include "input.h"

void Menu_ReleaseEntryObjects(void);
void ItemMenu_HideAllIcons(void);
void WaitFrames(s32 frames);
void UiWindow_CloseIfOpen(void *window, s32 mode);

void ItemMenu_Close(void) {
  u8 *menu;
  u8 *cursor;

  menu = Ram_HeapSlots->menuRuntime;
  Menu_ReleaseEntryObjects();
  ItemMenu_HideAllIcons();
  WaitFrames(1);
  cursor = *(u8 **)(menu + 0x170);
  cursor[5] = 13;
  UiWindow_CloseIfOpen(menu + 0x10, 1);
  UiWindow_CloseIfOpen(menu + 0x24, 1);
  UiWindow_CloseIfOpen(menu + 0xf0, 1);
  UiWindow_CloseIfOpen(menu + 0x28, 1);
  UiWindow_CloseIfOpen(menu + 0x2c, 1);
  UiWindow_CloseIfOpen(menu + 0x30, 1);
  UiWindow_CloseIfOpen(menu + 0x34, 1);
  UiWindow_CloseIfOpen(menu + 0x38, 1);
  UiWindow_CloseIfOpen(menu + 0x3c, 1);
  UiWindow_CloseIfOpen(menu + 0x40, 1);
  UiWindow_CloseIfOpen(menu + 0x44, 1);
}

struct ItemMenuIcon {
  u8 unk00[5];
  u8 state;
};

struct ItemMenuWork {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  void *pageWindow;
  struct ItemMenuIcon *paneIcons[2];
  s8 paneIndex[2];
  s8 paneCount[2];
  u8 unk020[4];
  void *itemWindow;
  void *statusWindow;
  void *helpWindow;
  void *infoWindow;
  u8 unk034[0x144];
  struct ItemMenuIcon *ownerIcons[2];
  u8 unk180[0x44];
  u16 items[32];
  u16 ownerIds[8];
  u8 itemCount;
  u8 partyCount;
  u8 paneOwner[2];
  struct ItemMenuIcon *selectedItemIcon;
};

struct ItemMenuOwner {
  u8 unk000[0xd8];
  u16 inventory[15];
};

void RenderOutput_ClearListFar(void *window);
void Func_080f8ab4(s32 x, s32 y);
void *Unit_Get(s32 unitId);
s32 ItemMenu_CollectItems(void *owner, u16 *items, s32 mode);
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items);
void UiIcon_PrepareObject(void *icon);

s32 ItemMenu_SelectMember(s32 pane) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s32 result = 0;
  s32 index;
  void *owner;

  index = menu->paneIndex[pane];
  RenderOutput_ClearListFar(menu->infoWindow);
  menu->paneCount[pane] = menu->partyCount;
  if (index == -1) {
    menu->paneIndex[pane] = result;
    index = 0;
  } else {
    Func_080f8ab4(index * 24 - 10, 16);
  }
  owner = Unit_Get(menu->ownerIds[index]);
  menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
  result = ItemMenu_RunOwnerSelection(menu->ownerIds, menu->items);
  UiIcon_PrepareObject(menu->paneIcons[pane]);
  WaitFrames(1);
  return result;
}

/// The item menu's help, followed by the equipment help. "L+A: Arrange
/// items"; the next message is "R: View equipment".
extern u8 MsgArrangeItemsHelp[];
#if LANGUAGE == LANGUAGE_JAPANESE
/// The key names Japanese draws before the help: "L+A:" and "R:".
extern const u8 gItemMenu_ArrangeKeysString[];
extern const u8 gItemMenu_EquipmentKeyString[];
#endif

s32 UiWindow_UpdateOrCreate(void **window, s32 x, s32 y, s32 width, s32 height,
                            s32 style);
void Menu_SpawnIconEntries(struct ItemMenuWork *menu, void *window);
struct ItemMenuIcon *RenderOutput_CreateFromResourceFar(s32 kind, s32 index,
                                                        void *window, s32 x,
                                                        s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void UiText_DrawStringInWindowFar(const u8 *text, void *window, s32 x, s32 y);
s32 GameFlag_Test(s32 flag);
void Func_080fa84c(s32 owner);
void Func_080f8170(void *window, s32 owner, s32 slot, s32 style);
void ItemMenu_RefreshOwner(s32 owner, s32 mode);
void Func_08104ef8(s32 first);
void Menu_HighlightPageEntry(void *window, s32 selection, s32 count);
void Func_080f8a44(s32 x, s32 y);
void InventoryMenu_SortByListOrder(u16 *inventory, s32 order);
void Audio_PlayCue(s32 cue);
s32 ItemMenu_CountItems(s32 owner);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);

/// The party member row of the item menu: moving shows each member's items and
/// status, holding R their equipment; L + A sorts the member's items, a new
/// order each press; A chooses a member who has items and B backs out. The
/// members are paged four at a time by Menu_MoveGridCursor, which moves the
/// selection and says what to redraw (2 for a new page). Returns the chosen
/// member's id, or -1.
// As The Broken Seal's ItemMenu_RunOwnerSelection.
s32 ItemMenu_RunOwnerSelection(u16 *ownerIds, u16 *items) {
  struct ItemMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s32 selection = menu->paneIndex[0];
  /* FAKEMATCH(temporary): the first owner's byte offset, taken before the
   * count; as Unit_Get(ownerIds[selection]) below, 2 of 307 instructions
   * differ in tla-en. */
  s32 firstOffset = selection * 2;
  s32 count;
  s32 redraw;
  s32 result;
  u8 sortMode;
  s32 showEquipment;
  struct ItemMenuOwner *owner;
  void *window;

  /* FAKEMATCH(barrier): an empty asm keeps the offset's shift ahead of the
   * count load; as count's initializer in its declaration, 2 of 307
   * instructions differ in tla-en. */
  asm("");
  count = menu->paneCount[0];
  redraw = 2;
  result = 0;
  sortMode = 0;
  showEquipment = 0;
  owner = Unit_Get(*(u16 *)(firstOffset + (s32)ownerIds));

  if (UiWindow_UpdateOrCreate(&menu->itemWindow, 13, 3, 17, 10, redraw))
    Menu_SpawnIconEntries(menu, menu->itemWindow);
  if (UiWindow_UpdateOrCreate(&menu->helpWindow, 13, 13, 17, 4, redraw)) {
    menu->selectedItemIcon =
        RenderOutput_CreateFromResourceFar(2, 0, menu->helpWindow, 0, result);
    menu->selectedItemIcon->state = 13;
  }
#if LANGUAGE == LANGUAGE_JAPANESE
  // Japanese draws the key names as their own strings, the help beside them.
  UiText_DrawStringInWindowFar(gItemMenu_ArrangeKeysString, menu->helpWindow, 0,
                               0);
  UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, menu->helpWindow,
                                  32, 0);
  UiText_DrawStringInWindowFar(gItemMenu_EquipmentKeyString, menu->helpWindow,
                               16, 8);
  UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1,
                                  menu->helpWindow, 32, 8);
#else
  UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp, menu->helpWindow, 0,
                                  0);
  UiText_DrawCharacterAtOffsetFar((s32)MsgArrangeItemsHelp + 1,
                                  menu->helpWindow, 0, 8);
#endif
  menu->paneIcons[0]->state = 1;
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      sortMode = 0;
      window = menu->statusWindow;
      owner = Unit_Get(ownerIds[selection]);
      if (showEquipment) {
        menu->itemCount = ItemMenu_CollectItems(Unit_Get(ownerIds[selection]),
                                                menu->items, 0);
        Func_080fa84c(ownerIds[selection]);
        Func_080f8170(window, ownerIds[selection], 0, 8);
      } else {
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        Func_080f8170(window, ownerIds[selection], 0, 0);
      }
      if (redraw == 2) {
        Func_08104ef8(selection / 4 * 4);
        WaitFrames(1);
      }
      Menu_HighlightPageEntry(menu->pageWindow, selection, count);
    }
    Func_080f8a44(selection % 4 * 24 - 10, 16);
    WaitFrames(1);
    if (gInput.pressed & 1) {
      if (gInput.held & 0x200) {
        sortMode = (sortMode + 4) % 4;
        InventoryMenu_SortByListOrder(owner->inventory, sortMode);
        sortMode++;
        ItemMenu_RefreshOwner(ownerIds[selection], 0);
        Audio_PlayCue(112);
      } else if (ItemMenu_CountItems(ownerIds[selection])) {
        Audio_PlayCue(112);
        result = ownerIds[selection];
        break;
      } else {
        Audio_PlayCue(114);
      }
    }
    if (gInput.pressed & 2) {
      Audio_PlayCue(113);
      result = -1;
      break;
    }
    if (gInput.pressed & 0x100) {
      showEquipment = 1;
      redraw = 1;
    } else if (!(gInput.held & 0x100) && showEquipment == 1) {
      showEquipment = 0;
      redraw = 1;
    } else {
      redraw = Menu_MoveGridCursor(&selection, count, 4);
    }
  }
  menu->paneIndex[0] = selection;
  menu->selectedOwner = ownerIds[selection];
  menu->paneOwner[0] = ownerIds[selection];
  menu->ownerIcons[0]->state = 13;
  menu->ownerIcons[1]->state = 13;
  return result;
}

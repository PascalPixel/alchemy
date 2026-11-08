#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "menu_result.h"
#include "edition.h"

// The Japanese list puts its help line at another column.
#if LANGUAGE == LANGUAGE_JAPANESE
#define CHANGE_HELP_X 16
#else
#define CHANGE_HELP_X 0
#endif

struct ListIcon {
  u8 unk00[5];
  u8 state;
  u8 unk06[6];
  u16 animStep;
  u8 unk0e;
  u8 sentinel;
};

struct ListMenu {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  void *pageWindow;
  u8 unk014[8];
  s8 paneIndex[2];
  u8 unk01e[0x1a];
  void *listWindow;
  u8 unk03c[0xc];
  struct ListIcon *gridCursor;
  struct ListIcon *entryIcons[32];
  u8 unk0cc[0x2c];
  void *ownerObjects[8];
  u8 unk118[0x40];
  s16 partySlots[4];
  u8 unk160[8];
  u16 selectedSlots[2];
  u16 selectedItems[2];
  u8 unk170[8];
  struct ListIcon *ownerIcons[2];
  u8 unk180[0x44];
  u16 items[32];
  u16 ownerIds[8];
  u8 itemCount;
  u8 partyCount;
  u8 paneOwner[2];
  struct ListIcon *selectedItemIcon;
  u8 unk21c[0x44];
  u8 equipPreview;
  u8 unk261[3];
  u8 selectedIndexByOwner[8];
};

struct ListOwner {
  u8 unk000[0xd8];
  u16 inventory[15];
};

// "L/R: Change character", under the list in pane 0.
extern u8 MsgChangeCharacterHelp[];
extern u32 gFrameCount;

s32 Window_UpdateOrCreate(void **window, s32 x, s32 y, s32 width, s32 height,
                          s32 style);
void *Unit_Get(s32 owner);
s32 ItemMenu_CollectItems(void *owner, u16 *items, s32 mode);
void ItemMenu_DrawIcons(u16 *items, s32 style);
s32 ItemMenu_PageResult(struct MenuResult *state, s32 pane);
void Func_080f8a44(s32 x, s32 y);
void Icon_PrepareActor(struct ListIcon *icon);
void wait(s32 frames);
void Func_080fc608(void *window, s32 mode, struct MenuResult *state);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Func_080fc558(void *window, s32 *work, struct MenuResult *state);
void Func_080fae8c(s32 owner, s32 slot, s32 mode, s32 target);
void Animation_ApplyChildArgumentFar(void *object, s32 argument);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
s32 Func_080f8f9c(s32 horizontal, s32 count, s32 perPage, s32 *row, s32 *page);
void Sound_PlayFar(s32 cue);
s32 GameFlag_Test(s32 flag);
void Menu_CreateOwnerEntryObjects(s32 first);
void Menu_HighlightPageEntry(void *window, s32 selection, s32 count);
void Window_ClearInteriorTilesFar(void *window, s32 x, s32 y, s32 width,
                                  s32 height);

s32 Func_080fc6bc(s32 pane) {
  struct ListMenu *menu;
  void *window;
  s32 changed;
  s32 moved;
  s32 member;
  s32 count;
  u8 i;
  s32 result;
  s32 redraw;
  s32 done;
  struct ListOwner *owner;
  s32 previous;
  struct ListIcon *icon;
  s32 work[5];
  struct MenuResult state;

  menu = Ram_HeapSlots->menuRuntime;
  result = 0;
  previous = 0;
  menu->equipPreview = result;
  Window_UpdateOrCreate(&menu->listWindow, 13, 3, 17, 14, 2);
  window = menu->listWindow;
  done = 0;

  while (done == 0 && GameFlag_Test(0x150) == 0) {
    owner = Unit_Get(menu->paneOwner[pane]);
    menu->itemCount = ItemMenu_CollectItems(owner, menu->items, 0);
    ItemMenu_DrawIcons(menu->items, 0);
    menu->selectedItemIcon->state = 13;
    ItemMenu_PageResult(&state, pane);
    Func_080f8a44(98, state.selectedIndex * 16 + 36);
    changed = 1;
    redraw = 1;

    while (GameFlag_Test(0x150) == 0) {
      Func_080f8a44(98, state.row * 16 + 36);

      if (changed != 0) {
        changed = 0;
        if (owner->inventory[previous] != 0) {
          icon = menu->entryIcons[previous];
          Icon_PrepareActor(icon);
        }
        if (redraw != 0) {
          wait(1);
          Func_080fc608(window, 0, &state);
          if (pane == 0)
            Text_DrawCharacterAtOffsetFar((s32)MsgChangeCharacterHelp, window,
                                          CHANGE_HELP_X, 88);
          redraw = 0;
        }
        Func_080fc558(window, work, &state);
        menu->selectedItems[pane] = menu->items[state.selectedIndex];
        Func_080fae8c(menu->paneOwner[pane], state.selectedIndex, 0,
                      menu->paneOwner[pane]);
        if (menu->items[state.selectedIndex] != 0) {
          icon = menu->entryIcons[state.selectedIndex];
          icon->state = 9;
          icon->animStep = 0;
          icon->sentinel = 250;
        }
        for (i = 0; i < menu->partyCount; i++)
          Animation_ApplyChildArgumentFar(menu->ownerObjects[i], 1);
      }

      if ((gFrameCount & 31) == 0) {
        for (i = 0; i <= 3; i++) {
          s32 slot = menu->partySlots[i];
          if (slot != -1 &&
              Item_CanOwnerEquip(slot,
                                 0x1ff & menu->items[state.selectedIndex]) != 0)
            Animation_ApplyChildArgumentFar(menu->ownerObjects[i], 3);
        }
      }

      wait(1);
      previous = state.selectedIndex;
      moved = Func_080f8f9c(0, state.entryCount, 5, &state.row, &state.page);
      if (moved == 1) {
        redraw = 1;
        changed = 1;
      }
      if (moved == 0)
        changed = 1;
      if (moved == -1)
        changed = 0;

      if ((gInput.pressed & 1) != 0 && menu->items[state.selectedIndex] != 0) {
        Sound_PlayFar(173);
        result = menu->items[state.selectedIndex];
        done = 1;
        break;
      }
      if ((gInput.pressed & 2) != 0) {
        Sound_PlayFar(113);
        result = -1;
        done = 1;
        break;
      }
      if ((gInput.repeat & 0x100) != 0 || (gInput.repeat & 0x200) != 0) {
        if (pane == 1) {
          Sound_PlayFar(114);
          wait(1);
        } else {
          Sound_PlayFar(111);
          menu->selectedIndexByOwner[menu->paneOwner[pane]] =
              state.selectedIndex;
          member = menu->paneIndex[pane];
          count = menu->partyCount;
          do {
            if ((gInput.repeat & 0x100) != 0)
              member++;
            else
              member--;
            member = (member + count) % count;
            menu->selectedOwner = menu->ownerIds[member];
            menu->paneOwner[pane] = menu->ownerIds[member];
            menu->paneIndex[pane] = member;
            menu->itemCount = ItemMenu_CollectItems(
                Unit_Get(menu->paneOwner[pane]), menu->items, 0);
          } while (menu->itemCount == 0);
          Menu_CreateOwnerEntryObjects(member / 4 * 4);
          Menu_HighlightPageEntry(menu->pageWindow, member, count);
          menu->ownerIcons[0]->state = 13;
          menu->ownerIcons[1]->state = 13;
          wait(1);
          break;
        }
      }
    }
  }

  Window_ClearInteriorTilesFar(window, 0, 88, 120, 96);
  Icon_PrepareActor(menu->gridCursor);
  menu->selectedSlots[pane] = state.selectedIndex;
  menu->selectedIndexByOwner[menu->paneOwner[pane]] = state.selectedIndex;
  menu->selectedItems[pane] = result;
  if (GameFlag_Test(0x150) != 0)
    result = -1;
  wait(1);
  return result;
}

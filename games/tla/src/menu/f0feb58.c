#include "types.h"
#include "ram_buffer.h"
#include "input.h"

struct OwnerPageState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  void *ownerWindow;
  u8 unk014[8];
  s8 ownerIndex;
  u8 unk01d;
  s8 ownerCount;
  u8 unk01f[0xd1];
  void *messageWindow;
  u8 unk0f4[0xd0];
  u16 psynergies[32];
  u16 ownerIds[8];
  u8 psynergyCount;
  u8 unk215;
  u8 selectedIds[6];
  u16 page;
};

extern u8 MsgChooseCharacter[];

void *Unit_Get(s32 owner);
void WindowSprite_RedrawSavedRectFar(void *window);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);
void Menu_CreateWindowAndEntryObjects(s32 owner, s32 page);
void PsynergyMenu_CallIconRoutineWithValue(void *menu, s32 owner);
s32 Func_080fd6f0(void *unit, u16 *list, s32 mode);
void PsynergyMenu_DrawPsynergyIcons(u16 *list);
void Func_081005e4(s32 x, s32 y, s32 columns);
void Menu_HideEmptyEntryIcons(u16 *list);
void Menu_CreateOwnerEntryObjects(s32 first);
void Menu_HighlightPageEntry(void *window, s32 selection, s32 count);
void Func_080f8a44(s32 x, s32 y);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);

s32 Func_080feb58(void) {
  struct OwnerPageState *menu =
      (struct OwnerPageState *)Ram_HeapSlots->menuRuntime;
  s32 selection = menu->ownerIndex;
  s32 count = menu->ownerCount;
  s32 pending = 2;
  s32 page = menu->page;
  s32 result;

  Unit_Get(menu->ownerIds[selection]);
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Text_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter, menu->messageWindow, 0,
                                0);
  Text_DrawCharacterAtOffsetFar((s32)MsgChooseCharacter + 1,
                                menu->messageWindow, 0, 16);
  while (1) {
    // PERM
    if (pending) {
      selection = (selection + count) % count;
      Unit_Get(menu->ownerIds[selection]);
      page = (page + 3) % 3;
      Menu_CreateWindowAndEntryObjects(menu->ownerIds[selection], page);
      PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[selection]);
      menu->psynergyCount = Func_080fd6f0(Unit_Get(menu->ownerIds[selection]),
                                          menu->psynergies, 0);
      PsynergyMenu_DrawPsynergyIcons(menu->psynergies);
      Func_081005e4(96, 96, 8);
      Menu_HideEmptyEntryIcons(menu->psynergies);
      if (pending == 2) {
        Menu_CreateOwnerEntryObjects(selection / 4 * 4);
        wait(1);
      }
      Menu_HighlightPageEntry(menu->ownerWindow, selection, count);
    }
    Func_080f8a44((selection % 4) * 24 - 10, 16);
    wait(1);
    if (((struct InputState *)&gInput)->pressed & 1) {
      Sound_PlayFar(112);
      result = 1;
      break;
    }
    if (((struct InputState *)&gInput)->pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    pending = Menu_MoveGridCursor(&selection, count, 4);
  }
  menu->ownerIndex = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->selectedIds[0] = menu->ownerIds[selection];
  return result;
}

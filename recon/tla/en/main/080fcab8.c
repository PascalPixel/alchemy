// First draft of Func_080fcab8: a second item picker an event script opens,
// built as Func_080f94a4 (recon/tla/en/main/080f94a4.c, ☀️'s
// ItemMenu_Run) without the window tile save: opens the item menu
// over the field, runs the command loop Func_080fcbd8, and when it returns 1
// looks up the chosen item's action (BattleActGet, result unused) and records
// owner << 10 | item in the event work at 0x166. Returns the loop's result.
// No ☀️ twin. Differ 50 of 117, register choice only: the native builds the
// heap slots base in r1 and keeps that pseudo in r8, and takes r1 for the
// offsets 0x16c and 0x166 and r2 for the base reloads, where this draft takes
// r2 and r3 (item | owner << 10 in this operand order is measured: the
// other order gives differ 58).
#include "types.h"
#include "mem.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[0x28];
  void *statusWindow;
  u8 unk02c[0xc4];
  s32 messageWindow;
  u8 unk0f4[0x78];
  u16 selectedItems[2];
  u8 unk170[0x94];
  s16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
};

struct ObjectSystemWork {
  u32 unk00;
  u16 suspended;
};

void UiWindow_DrawFrameFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_InitializeWork(s32 mode);
s32 Party_ListActiveOwnersFar(s16 *owners);
void Func_080fa368(s32 a, s32 b, s32 c, s32 d);
void Palette_LightenBankHighlight(s32 palette);
void Link_DrawShiftedTilePairFar(void *tiles);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
s32 Func_080fcbd8(s32 *ownerOut, s32 *targetOut, s32 *itemOut);
void *BattleActGet(s32 action);
void RenderOutput_ClearListFar(void *window);
void ItemMenu_Close(void);
void Event_ClearInvalidPackedValuesFar(void);
void WaitFrames(s32 frames);

s32 Func_080fcab8(void) {
  struct InventoryMenuState *screen;
  s32 item;
  s32 target;
  s32 owner;
  s32 result;

  screen = Runtime_AllocateHeapBlock(220, 0xcec);
  Ram_HeapSlots->objectSystem->suspended = 1;
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  screen->partyCount = Party_ListActiveOwnersFar(screen->ownerIds);
  Func_080fa368(0, 3, 0, 7);
  screen->messageWindow = UiWindow_CreateFar(13, 0, 17, 3, 2);
  Palette_LightenBankHighlight(14);
  Link_DrawShiftedTilePairFar((void *)0x06002500);

  Menu_CancelSoundReset();
  result = Func_080fcbd8(&owner, &target, &item);
  Menu_EnsureCancelSound();
  if (result == 1) {
    u8 *event = Ram_HeapSlots->eventWork;

    BattleActGet(screen->selectedItems[0] & 0x3fff);
    *(u16 *)(event + 0x166) = item | (owner << 10);
  }

  RenderOutput_ClearListFar(screen->statusWindow);
  Ram_HeapSlots->windowTiles[6] = 1;
  ItemMenu_Close();
  UiWindow_DrawFrameFar(0, 0, 30, 20);
  Runtime_ReleaseHeapBlock(220);
  Ram_HeapSlots->objectSystem->suspended = 0;
  WaitFrames(1);
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  Ram_HeapSlots->windowTiles[6] = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

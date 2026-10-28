#include "types.h"
#include "heap_release.h"
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

void Window_DrawBorderFar(s32 x, s32 y, s32 width, s32 height);
void Window_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Window_InitializeWork(s32 mode);
s32 Party_ListActiveOwnersFar(s16 *owners);
void Func_080fa368(s32 a, s32 b, s32 c, s32 d);
void Palette_LightenHighlight(s32 palette);
void Link_DrawShiftedTilePairFar(void *tiles);
s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
s32 PsynergyMenu_RunSteps(s32 *ownerOut, s32 *targetOut, s32 *itemOut);
void *BattleAction_Get(s32 action);
void WindowSprite_ClearListFar(void *window);
void ItemMenu_Close(void);
void Event_ClearInvalidPackedValuesFar(void);
void wait(s32 frames);

/// A second item picker an event script opens, built as Func_080f94a4: opens
/// the item menu over the field and runs its command loop; when that returns 1,
/// looks up the chosen item's action (unused) and records owner << 10 | item in
/// the event work at 0x166. Returns the loop's result.
s32 Func_080fcab8(void) {
  // German, Spanish, French and Italian also keep the tiles at 0x06004000
  // aside, filled with a pattern, while the picker is open, and stay on their
  // scaffolds: written so, the tile address is kept in a register the native
  // reloads from the pool at each call.
  struct InventoryMenuState *screen;
  s32 item;
  s32 target;
  s32 owner;
  s32 result;

  screen = HeapSlot_Alloc(220, 0xcec);
  Ram_HeapSlots->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);
  screen->partyCount = Party_ListActiveOwnersFar(screen->ownerIds);
  Func_080fa368(0, 3, 0, 7);
  screen->messageWindow = Window_CreateFar(13, 0, 17, 3, 2);
  Palette_LightenHighlight(14);
  Link_DrawShiftedTilePairFar((void *)0x06002500);

  Menu_CancelSoundReset();
  result = PsynergyMenu_RunSteps(&owner, &target, &item);
  Menu_EnsureCancelSound();
  if (result == 1) {
    u8 *event = Ram_HeapSlots->eventWork;
    u16 request;

    BattleAction_Get(0x3fff & screen->selectedItems[0]);
    request = item | (owner << 10);
    *(u16 *)(event + 0x166) = request;
  }

  WindowSprite_ClearListFar(screen->statusWindow);
  Ram_HeapSlots->windowTiles[6] = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  HeapSlot_Release(220);
  Ram_HeapSlots->objectSystem->suspended = 0;
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Ram_HeapSlots->windowTiles[6] = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

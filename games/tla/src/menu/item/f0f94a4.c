#include "types.h"
#include "version.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[0x28];
  void *statusWindow;
  u8 unk02c[0xc4];
  s32 messageWindow;
  u8 unk0f4[0x74];
  u16 selectedSlots[2];
  u8 unk16c[0x98];
  s16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
};

struct AssetSelectionResult {
  s32 unk000[0x5a];
  u16 chosenItem;
  u8 unk16a[0x18];
  u16 chosenSlot;
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
void Resource_LoadPairedBlocks(void);
void Palette_LightenHighlight(s32 palette);
void Link_DrawShiftedTilePairFar(void *tiles);
s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 Menu_SuspendFieldTasks(void);
s32 Menu_ResumeFieldTasks(void);
void Text_UseExtraTilesFar(s32 flag);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
s32 Func_080f9644(s32 *ownerOut, s32 *targetOut, s32 *itemOut);
void WindowSprite_ClearListFar(void *window);
void ItemMenu_Close(void);
void Menu_ResetTwoResourceEntries(void);
void Func_08038290(void);
void Event_ClearInvalidPackedValuesFar(void);
void wait(s32 frames);

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants; as direct calls, 92 of 162 instructions differ in tla-en. */
  return Iwram_CopyWords(destination, source, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants; as a direct call, 47 of 162 instructions differ in tla-en. */
  return Iwram_FillWords(destination, size, value);
}

s32 Func_080f94a4(void) {
#if TLA_PATCH_1
  void *backup;
#endif
  struct InventoryMenuState *screen;
  s32 item;
  s32 target;
  s32 owner;
  s32 result;
#if TLA_PATCH_1
  s32 size;
#endif

  screen = HeapSlot_Alloc(220, 0xcec);
#if TLA_PATCH_1
  size = 0x2000;
  backup = Heap_AllocEwram(size);
#endif
  Ram_HeapSlots->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);
  screen->partyCount = Party_ListActiveOwnersFar(screen->ownerIds);
  Func_080fa368(0, 3, 0, 7);
  Resource_LoadPairedBlocks();
  Palette_LightenHighlight(14);
  Link_DrawShiftedTilePairFar((void *)0x06002500);
  screen->messageWindow = Window_CreateFar(13, 0, 17, 3, 2);
#if TLA_PATCH_1
  Menu_SuspendFieldTasks();
  CopyWords_(backup, (void *)0x06004000, size);
  FillWords_((void *)0x06004000, size, 0x33333333);
  Text_UseExtraTilesFar(1);
#endif

  Menu_CancelSoundReset();
  result = Func_080f9644(&owner, &target, &item);
  Menu_EnsureCancelSound();
  if (result == 1) {
    struct AssetSelectionResult *selection = Ram_HeapSlots->eventWork;

    selection->chosenItem = (owner << 10) | (item & 0x1ff);
    selection->chosenSlot = screen->selectedSlots[0];
  }

  WindowSprite_ClearListFar(screen->statusWindow);
  Ram_HeapSlots->windowTiles[6] = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  Menu_ResetTwoResourceEntries();
  HeapSlot_Release(220);
  Ram_HeapSlots->objectSystem->suspended = 0;
#if TLA_PATCH_1
  Func_08038290();
  Text_UseExtraTilesFar(0);
  CopyWords_((void *)0x06004000, backup, size);
  Ram_HeapSlots->windowTiles[6] = 0;
  Heap_Release(backup);
  wait(1);
  Menu_ResumeFieldTasks();
#endif
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Ram_HeapSlots->windowTiles[6] = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

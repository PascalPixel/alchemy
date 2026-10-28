// First draft of Func_080f94a4: the item picker an event script opens, the
// twin of ☀️'s ItemMenu_Run (games/tbs/src/menu/menu.c) in its
// TBS_PATCH_1 form: opens the item menu over the field (saving the window
// tiles at 0x06004000), runs ItemMenu_RunCommands (Func_080f9644), records the
// choice in the event work and returns 1 if one was made. Differ 96 of 162
// (draft 166), register choice more than order: the native keeps the heap
// slots base 0x03000000 in r7, the size in r10 and the result in r8, reloads
// 0x06004000 from the pool at each use, and loads the window flag's 0 once
// from the pool into r11 for all three stores (here each is a movs); this
// draft keeps the base in r8, VRAM in r7 and the size in r9.
#include "types.h"
#include "mem.h"
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
s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut);
void WindowSprite_ClearListFar(void *window);
void ItemMenu_Close(void);
void Menu_ResetTwoResourceEntries(void);
void Func_08038290(void);
void Event_ClearInvalidPackedValuesFar(void);
void WaitFrames(s32 frames);

s32 Func_080f94a4(void) {
  void *backup;
  struct InventoryMenuState *screen;
  s32 item;
  s32 target;
  s32 owner;
  s32 result;
  s32 size;

  screen = Runtime_AllocateHeapBlock(220, 0xcec);
  size = 0x2000;
  backup = Runtime_BumpAllocateAlternatePool(size);
  Ram_HeapSlots->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  WaitFrames(1);
  Window_InitializeWork(0);
  screen->partyCount = Party_ListActiveOwnersFar(screen->ownerIds);
  Func_080fa368(0, 3, 0, 7);
  Resource_LoadPairedBlocks();
  Palette_LightenHighlight(14);
  Link_DrawShiftedTilePairFar((void *)0x06002500);
  screen->messageWindow = Window_CreateFar(13, 0, 17, 3, 2);
  Menu_SuspendFieldTasks();
  Iwram_CopyWords(backup, (void *)0x06004000, size);
  Iwram_FillWords((void *)0x06004000, size, 0x33333333);
  Text_UseExtraTilesFar(1);

  Menu_CancelSoundReset();
  result = ItemMenu_RunCommands(&owner, &target, &item);
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
  Runtime_ReleaseHeapBlock(220);
  Ram_HeapSlots->objectSystem->suspended = 0;
  Func_08038290();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)0x06004000, backup, size);
  Ram_HeapSlots->windowTiles[6] = 0;
  MemFree(backup);
  WaitFrames(1);
  Menu_ResumeFieldTasks();
  WaitFrames(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Ram_HeapSlots->windowTiles[6] = 0;
  Event_ClearInvalidPackedValuesFar();
  return result;
}

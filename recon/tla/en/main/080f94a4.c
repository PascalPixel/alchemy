// Differ 22 of 162 (tla-en, 2026-11-08, -mtune=gba), with three tagged devices (two pins,
// one barrier). Without the r4 pin 96, pin r6 96, no empty asm 30 (an empty
// once-only block in its place 28, around the allocation 30, around the suspend
// store 78), no r1 hoist 24, no devices at all 92. Other editions not re-measured.
// Round 7 (2026-11-08): devices re-measured, the same figures; the size hoist as a
// plain block-local copy instead of the r1 pin 24; pin r5 110. Not adoptable as it
// stands: r4 is call-clobbered (-fcall-used-r4), so the pinned backup is read back
// from r4 after calls that may change it; the native keeps it in r6.
// Tried 2026-11-08: with no device, a set-once tiles local for 0x06004000 set after
// the allocation 51 (49 with the r1 hoist; set first 59; block-local or the copy
// back as a literal 51): global.c still ranks it above the pool's 0 (4 refs over
// 202 insns against 3 over 234), so VRAM keeps a register. Once-only blocks around
// the copies 108. A three-minute permute of the device-free form found only invalid
// candidates (24, copying the backup onto itself).
// 2026-11-06: the same 22 in de, es, fr and it;
// 0 of 109 in tla-ja (2026-11-07, its whole extent; the listing runs on).
// Near miss with an r4 pin, empty asm and r1 hoist found by search, now tagged. The rest is allocation: the native keeps the
// backup in r6 and reloads 0x06004000 from the pool, here VRAM takes r6. Tried
// 2026-11-06: no pin (96), pin r6 (96), no hoist (24), ☀️ twin's cache/process
// pointers (89).
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
// Editions: German, Spanish, French and Italian compile the English form (their
// listings differ from English only in call and pool addresses). Japanese is
// the older, shorter form (TLA_PATCH_1 off): no 0x2000-byte backup of the
// window tiles at 0x06004000, so no Heap_AllocEwram, Menu_SuspendFieldTasks,
// copy and fill, Text_UseExtraTilesFar, Func_08038290, copy back, first
// windowTiles[6] clear, Heap_Release, Menu_ResumeFieldTasks or second wait(1).
// Their listings' labels cover the whole block through 0x080fa2dc (English
// addresses): ItemMenu_RunCommands (080f9644.c), then ItemMenu_Use, Func_080fa368
// and the item interface helpers, already C in games/tla/src/menu/item.
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
s32 ItemMenu_RunCommands(s32 *ownerOut, s32 *targetOut, s32 *itemOut);
void WindowSprite_ClearListFar(void *window);
void ItemMenu_Close(void);
void Menu_ResetTwoResourceEntries(void);
void Func_08038290(void);
void Event_ClearInvalidPackedValuesFar(void);
void wait(s32 frames);

s32 Func_080f94a4(void) {
#if TLA_PATCH_1
  /* FAKEMATCH(pin): the backup held in r4; as a plain local, 96 of 162
   * instructions differ in tla-en. */
  register void *backup asm("r4");
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
  /* FAKEMATCH(barrier): an empty asm after the allocation; without it, 30 of
   * 162 instructions differ in tla-en. */
  asm("");
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
  Iwram_CopyWords(backup, (void *)0x06004000, size);
  {
    /* FAKEMATCH(pin): the size held in r1 for the fill; as size passed
     * directly, 24 of 162 instructions differ in tla-en. */
    register s32 hoisted asm("r1") = size;
    Iwram_FillWords((void *)0x06004000, hoisted, 0x33333333);
  }
  Text_UseExtraTilesFar(1);
#endif

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
  HeapSlot_Release(220);
  Ram_HeapSlots->objectSystem->suspended = 0;
#if TLA_PATCH_1
  Func_08038290();
  Text_UseExtraTilesFar(0);
  Iwram_CopyWords((void *)0x06004000, backup, size);
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

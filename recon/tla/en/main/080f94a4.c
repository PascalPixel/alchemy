// Matches: differ 0 (tla-en, 2026-11-08)
// With two tagged wrapper devices and no other: the two IWRAM calls go through
// draft-local inlines (CopyWords_, FillWords_), as ☀️'s twin ItemMenu_Run does
// through its iwram_call.h layer. Integration substitutes the constant
// arguments straight into r0 to r2, so 0x06004000 never becomes one shared
// pseudo: the native reloads it from the pool at each call, keeps the backup in
// r6 and gives r11 to the window flag's 0 (the pseudo the suspend store's
// read-modify-write leaves). Called directly, CSE shares 0x06004000 across all
// three calls, it outranks the 0 in global.c and takes r7. Both wrappers are
// needed: copy direct 92, fill direct 47, both direct 92. The earlier near miss
// (22, an r4 pin on the backup, an r1 pin on the size and an empty asm) is
// replaced; it was not adoptable, as r4 is call-clobbered (-fcall-used-r4).
// ⚓️'s iwram_call.h says the game calls the fixed entries directly, so the
// inlines are tagged as f101638.c's are; if ⚓️ turns out to share ☀️'s layer,
// they become ordinary C there.
// Editions (2026-11-08, each at its own address): de, es, fr and it match all
// 162 instructions of the function, ja (TLA_PATCH_1 off) all 109; their
// listings run on past the final pop into ItemMenu_RunCommands, because the
// scaffold's labels cover the whole block, so the tool prints differ 1358 (de,
// es, it), 1363 (fr) and 1362 (ja) for the rest of the block. de, es and fr
// need the address: 081034a4, 080ff498, 080fb4a4.
// First draft of Func_080f94a4: the item picker an event script opens, the
// twin of ☀️'s ItemMenu_Run (games/tbs/src/menu/menu/item.c) in its
// TBS_PATCH_1 form: opens the item menu over the field (saving the window
// tiles at 0x06004000), runs ItemMenu_RunCommands (Func_080f9644), records the
// choice in the event work and returns 1 if one was made.
// Japanese is the older, shorter form (TLA_PATCH_1 off): no 0x2000-byte backup
// of the window tiles at 0x06004000, so no Heap_AllocEwram,
// Menu_SuspendFieldTasks, copy and fill, Text_UseExtraTilesFar, Func_08038290,
// copy back, first windowTiles[6] clear, Heap_Release, Menu_ResumeFieldTasks or
// second wait(1). Its listing's labels cover the whole block through 0x080fa2dc
// (English addresses): ItemMenu_RunCommands (080f9644.c), then ItemMenu_Use,
// Func_080fa368 and the item interface helpers, already C in
// games/tla/src/menu/item.
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

// Func_08100e7c runs the Djinn menu, keeping the field's tiles and the party's
// event mode; the twin of The Broken Seal's DjinnMenu_Run
// (games/tbs/src/menu/menu/djinn.c), rewritten with this game's heap block
// (220, 0xcec), menu work and calls (Func_0810106c is the screen loop).
#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "iwram_call.h"

struct ObjectSystemWork {
  u8 unk00[4];
  u16 suspended;
};

struct DjinnMenuBackdrop {
  u8 list[0xa8];
  u8 tiles[0x2000];
  u8 palette[0x80];
  s32 tutorialTimer;
  s32 tutorialStep;
};

struct DjinnMenuWork {
  u8 unk000[0x1c];
  s8 selectedColumn[2];
  u8 unk01e[0xd2];
  s32 helpWindow;
  u8 unk0f4[0x74];
  u16 selection[2];
  u16 pairEntry;
  u8 unk16e[0x12];
  struct DjinnMenuBackdrop *backdrop;
  u8 unk184[0x80];
  u16 owners[8];
  u8 unk214;
  u8 ownerCount;
};

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void MemFree(void *block);
s32 GameFlag_Test(s32 flag);
void UiWindow_DrawBorderFar(s32 x, s32 y, s32 width, s32 height);
void WaitFrames(s32 frames);
void UiWindow_InitializeWork(s32 mode);
void Scheduler_EnableOverlayCallbacksWithFlags(void);
void Scheduler_DisableOverlayCallbacksWithFlags(void);
void UiWork_SetAltFlagAndClearTableFar(s32 flag);
void Link_DrawShiftedTilePairFar(void *tiles);
s32 Party_ListActiveOwnersFar(u16 *owners);
void Resource_LoadPairedBlocksIfAvailable(void);
void Func_080fa368(s32 x, s32 y, s32 mode, s32 columns);
void Menu_SetFirstObjectRowCoordinates(s32 offset);
void Palette_LightenHighlight(s32 paletteNumber);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 frame);
void Func_081051a8(void);
s32 Func_0810106c(void);
void MenuIcon_ReleaseObjects(void);
void Menu_ResetTwoResourceEntries(void);
void ItemMenu_Close(void);
void Func_08038290(void);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Event_ClearInvalidPackedValuesFar(void);

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants, as the scene layer does; as plain calls, 14 of 201
   * instructions differ in tla-en. */
  Iwram_CopyWords(destination, source, size);
}

s32 Func_08100e7c(void) {
  struct DjinnMenuWork *menu;
  u32 savedEventMode;
  s32 enabled;
  struct DjinnMenuBackdrop *backdrop;
  s32 zero;

  menu = Runtime_AllocateHeapBlock(220, 0xcec);
  savedEventMode = gAdventure.eventMode;
  gAdventure.eventMode = 2;
  enabled = Ram_HeapSlots->objectSystem->suspended = 1;
  UiWindow_DrawBorderFar(0, 0, 30, 20);
  WaitFrames(1);
  UiWindow_InitializeWork(0);
  backdrop = Runtime_BumpAllocateAlternatePool(0x2130);
  menu->backdrop = backdrop;
  backdrop->tutorialTimer = 0;
  backdrop->tutorialStep = 0;
  if (GameFlag_Test(0x16e)) {
    if (!GameFlag_Test(0x16f)) {
      if (!GameFlag_Test(0x171))
        backdrop->tutorialStep = enabled;
      else
        backdrop->tutorialStep = 14;
    } else {
      if (!GameFlag_Test(0x171))
        backdrop->tutorialStep = 27;
      else
        backdrop->tutorialStep = 28;
    }
  }
  Scheduler_EnableOverlayCallbacksWithFlags();
  UiWork_SetAltFlagAndClearTableFar(1);
  Link_DrawShiftedTilePairFar((void *)0x06002500);
  menu->ownerCount = Party_ListActiveOwnersFar(menu->owners);
  Resource_LoadPairedBlocksIfAvailable();
  Func_080fa368(0, 3, 0, 7);
  Menu_SetFirstObjectRowCoordinates(0);
  Palette_LightenHighlight(14);
  menu->helpWindow = UiWindow_CreateFar(13, 0, 17, 5, 2);
  menu->pairEntry = 255;
  zero = 0;
  menu->selectedColumn[0] = zero;
  menu->selectedColumn[1] = zero;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  Func_081051a8();
  Func_0810106c();
  MenuIcon_ReleaseObjects();
  Menu_ResetTwoResourceEntries();
  WaitFrames(1);
  ItemMenu_Close();
  UiWindow_DrawBorderFar(0, 0, 30, 20);
  Ram_HeapSlots->objectSystem->suspended = 0;
  Func_08038290();
  UiWork_SetAltFlagAndClearTableFar(0);
  CopyWords_((void *)0x06004000, backdrop->tiles, 0x2000);
  CopyWords_((void *)0x05000080, backdrop->palette, 128);
  WaitFrames(1);
  Scheduler_DisableOverlayCallbacksWithFlags();
  UiWindow_EraseBorderRectFar(0, 0, 30, 20);
  MemFree(menu->backdrop);
  Runtime_ReleaseHeapBlock(220);
  Event_ClearInvalidPackedValuesFar();
  gAdventure.eventMode = savedEventMode;
  return 1;
}

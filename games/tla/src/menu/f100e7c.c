// Func_08100e7c runs the Djinn menu, keeping the field's tiles and the party's
// event mode; the twin of The Broken Seal's DjinnMenu_Run
// (games/tbs/src/menu/menu/djinn.c), rewritten with this game's heap block
// (220, 0xcec), menu work and calls (Func_0810106c is the screen loop).
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "djinn_menu.h"

struct ObjectSystemWork {
  u8 unk00[4];
  u16 suspended;
};

void *HeapSlot_Alloc(s32 slot, s32 size);
void *Heap_AllocEwram(s32 size);
void HeapSlot_Release(s32 slot);
void Heap_Release(void *block);
s32 GameFlag_Test(s32 flag);
void Window_DrawBorderFar(s32 x, s32 y, s32 width, s32 height);
void wait(s32 frames);
void Window_InitializeWork(s32 mode);
void Menu_SuspendFieldTasks(void);
void Menu_ResumeFieldTasks(void);
void Text_UseExtraTilesFar(s32 flag);
void Link_DrawShiftedTilePairFar(void *tiles);
s32 Party_ListActiveOwnersFar(u16 *owners);
void Resource_LoadPairedBlocksIfAvailable(void);
void ItemMenu_Initialize(s32 x, s32 y, s32 mode, s32 columns);
void Menu_SetFirstObjectRowCoordinates(s32 offset);
void Palette_LightenHighlight(s32 paletteNumber);
s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 frame);
void Func_081051a8(void);
s32 Func_0810106c(void);
void MenuIcon_ReleaseObjects(void);
void Menu_ResetTwoResourceEntries(void);
void ItemMenu_Close(void);
void Func_08038290(void);
void Window_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
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

  menu = HeapSlot_Alloc(220, 0xcec);
  savedEventMode = gMainRoutineWork.eventMode;
  gMainRoutineWork.eventMode = 2;
  enabled = Ram_HeapSlots->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Window_InitializeWork(0);
  backdrop = Heap_AllocEwram(0x2130);
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
  Menu_SuspendFieldTasks();
  Text_UseExtraTilesFar(1);
  Link_DrawShiftedTilePairFar((void *)0x06002500);
  menu->memberCount = Party_ListActiveOwnersFar(menu->owners);
  Resource_LoadPairedBlocksIfAvailable();
  ItemMenu_Initialize(0, 3, 0, 7);
  Menu_SetFirstObjectRowCoordinates(0);
  Palette_LightenHighlight(14);
  menu->helpWindow = Window_CreateFar(13, 0, 17, 5, 2);
  menu->pairEntries[0] = 255;
  zero = 0;
  menu->selectedColumn[0] = zero;
  menu->selectedColumn[1] = zero;
  menu->selection[0] = 0;
  menu->selection[1] = 0;
  Func_081051a8();
  Func_0810106c();
  MenuIcon_ReleaseObjects();
  Menu_ResetTwoResourceEntries();
  wait(1);
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  Ram_HeapSlots->objectSystem->suspended = 0;
  Func_08038290();
  Text_UseExtraTilesFar(0);
  CopyWords_((void *)0x06004000, backdrop->tiles, 0x2000);
  CopyWords_((void *)&BG_PLTT_COLOR(4, 0), backdrop->palette, 128);
  wait(1);
  Menu_ResumeFieldTasks();
  Window_EraseBorderRectFar(0, 0, 30, 20);
  Heap_Release(menu->backdrop);
  HeapSlot_Release(220);
  Event_ClearInvalidPackedValuesFar();
  gMainRoutineWork.eventMode = savedEventMode;
  return 1;
}

#include "types.h"
#include "ram_buffer.h"
#include "mem.h"
#include "animspr.h"
#include "window.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "display_transition.h"

struct OwnerMenuState {
  u8 unk000[0x28];
  void *statusWindow;
  u8 unk02c[0xc4];
  void *selectorWindow;
  u8 unk0f4[0x34];
  u16 ownerY[4];
  u8 unk130[0xd4];
  s16 ownerIds[8];
  u8 unk214;
  u8 partyCount;
  u8 unk216[6];
  u16 page;
};

void Window_DrawBorderFar(s32 x, s32 y, s32 width, s32 height);
void Window_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void wait(s32 frames);
void Window_InitializeWork(s32 unused);
s32 Party_ListActiveOwnersFar(s16 *owners);
void Func_080fee04(s32 a, s32 b, s32 c, s32 d);
void *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 flags);
s32 Func_080fe49c(void);
void WindowSprite_ClearListFar(void *window);
void ItemMenu_Close(void);
void *Heap_AllocEwram(s32 size);
void Heap_Release(void *block);
void Menu_SuspendFieldTasks(void);
void Menu_ResumeFieldTasks(void);
void Palette_LightenHighlight(s32 paletteNumber);
void Text_UseExtraTilesFar(s32 flag);
s32 Party_SumDjinnCountsFar(s32 owner);
void Func_081051a8(void);
void Func_081053a8(void);
void Link_DrawShiftedTilePairFar(s32 address);
void Menu_CancelSoundReset(void);
void Menu_EnsureCancelSound(void);
void MenuIcon_ReleaseSlots(void);
void MenuIcon_ReleaseObjects(void);
void Func_08038290(void);

static __inline__ void CopyWords_(void *destination, const void *source,
                                  s32 size) {
  /* FAKEMATCH(wrapper): forwarding through the inline keeps each call's own
   * constants; as direct Iwram_CopyWords calls, CSE keeps 0x05000000 in a
   * saved register and 48 of 216 instructions differ in tla-en. */
  Iwram_CopyWords(destination, source, size);
}

s32 Func_080fe274(void) {
  struct OwnerMenuState *state =
      (struct OwnerMenuState *)HeapSlot_Alloc(220, 0x0cec);
  void *palette = Heap_AllocEwram(64);
  void *tiles = Heap_AllocEwram(0x2800);
  s32 result = -1;
  struct HeapSlots *heap;
  struct ScreenTransitionState *transition;
  u32 saved;
  u16 *page;

  heap = Ram_HeapSlots;
  transition = heap->screenTransition;
  heap->objectSystem->suspended = 1;
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  saved = *(u16 *)&transition->active;
  transition->active = 17;
  Menu_SuspendFieldTasks();
  Window_InitializeWork(0);
  page = &state->page;
  *page = 0;
  state->partyCount = Party_ListActiveOwnersFar(state->ownerIds);
  Func_080fee04(0, 3, 0, 7);
  CopyWords_(palette, (void *)0x05000000, 64);
  CopyWords_(tiles, (void *)0x06004000, 0x2800);
  Palette_LightenHighlight(14);
  Dma_Set((void *)0x05000200, (void *)0x05000000, 0x80000010, REG_DMA3);
  Dma_Set((void *)0x050001c8, (void *)0x0500001c, 0x80000001, REG_DMA3);
  Dma_Set((void *)0x05000200, (void *)0x05000020, 0x80000010, REG_DMA3);
  Dma_Set((void *)0x050001e8, (void *)0x0500003c, 0x80000001, REG_DMA3);
  Text_UseExtraTilesFar(1);
  state->selectorWindow = Window_CreateFar(13, 0, 17, 5, 2);
  if (Party_SumDjinnCountsFar(result) != 0)
    Func_081051a8();
  Link_DrawShiftedTilePairFar(0x06002500);
  Func_081053a8();
  Menu_CancelSoundReset();
  *page = 0;

  result = Func_080fe49c();

  Menu_EnsureCancelSound();
  MenuIcon_ReleaseSlots();
  WindowSprite_ClearListFar(state->statusWindow);
  MenuIcon_ReleaseObjects();
  Menu_ResumeFieldTasks();
  Window_DrawBorderFar(0, 0, 30, 20);
  wait(1);
  Func_08038290();
  Text_UseExtraTilesFar(0);
  CopyWords_((void *)0x05000000, palette, 64);
  CopyWords_((void *)0x06004000, tiles, 0x2800);
  Heap_Release(tiles);
  Heap_Release(palette);
  ((struct WindowWork *)heap->windowTiles)->menuBusy = 1;
  ItemMenu_Close();
  Window_DrawBorderFar(0, 0, 30, 20);
  HeapSlot_Release(220);
  heap->objectSystem->suspended = 0;
  transition->active = saved;
  wait(1);
  Window_EraseBorderRectFar(0, 0, 30, 20);
  ((struct WindowWork *)heap->windowTiles)->menuBusy = 0;
  return result;
}

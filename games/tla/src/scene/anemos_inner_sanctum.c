/// @file
/// The scene code of overlay 0x6b0, in ROM order.

#include "shared/actors.h"
#include "shared/effects.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "types.h"
#include "task.h"
#include "ram_buffer.h"
#include "actor.h"

#include "main_routine_work.h"
#include "event_work.h"
#include "module/scene/anemos_inner_sanctum.h"

/// Writes one background palette entry from its red, green and blue.
// A statement macro, written do { } while (0): this store matches through that
// shape (see the header).
#define BG_PLTT_SET_RGB(index, r, g, b)                                        \
  do {                                                                         \
    int value = RGB(r, g, b);                                                  \
                                                                               \
    BG_PLTT[index] = value;                                                    \
  } while (0)

void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gPaletteSlots = slots;
  gPaletteColors = colours;
  gPaletteCycleColors = a;
  gPaletteCycleSlots = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int index = *slots++;
      int red = *colours++;
      int green = *colours++;
      int blue = *colours++;

      BG_PLTT_SET_RGB(index, red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)0x05000000, saved, 0x84000070, REG_DMA3);
  Dma_Set((const void *)0x05000200, saved + 448, 0x84000070, REG_DMA3);
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_02000328();
}

DEFINE_PALETTE_CYCLE_START(Func_020000f4, gPaletteCycleIndex,
                           gPaletteCycleLength, List_CountEntries,
                           gPaletteSlots, gPaletteCycleDelay, 15,
                           gPaletteCycleTimer, 16, gPaletteCycleBright,
                           PaletteCycle_Fade)

void Func_02000158(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  FieldTask_Remove((s32)PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START(Func_02000178, gPaletteCycleIndex,
                           gPaletteCycleLength, List_CountEntries,
                           gPaletteSlots, gPaletteCycleDelay, 2,
                           gPaletteCycleTimer, 2, gPaletteCycleBright,
                           PaletteCycle_Fade)

DEFINE_LIST_COUNT_ENTRIES(List_CountEntries)

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gPaletteCycleTimer,
                          gPaletteCycleDelay, gPaletteCycleColors,
                          gPaletteCycleLength, gPaletteCycleSlots,
                          gPaletteCycleIndex, gPaletteSlots,
                          gPaletteCycleBright, gPaletteColors)

void Func_020017b4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02003458_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_02000328(void) {
  struct AnemosInnerSanctumEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  Func_02003458_(0x10000, 2);
  ((struct AnemosInnerSanctumEventWork *)Ram_HeapSlots->eventWork)->value1ac =
      0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000178();
  Teleport_PlayWhirl();
  System_Wait(40);
  Func_02000158();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

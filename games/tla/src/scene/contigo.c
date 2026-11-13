/// @file
/// The scene code of overlay 0x694, in ROM order.

#include "shared/effects.h"
#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "contigo.h"
#include "task.h"
#include "actor.h"
#include "module/scene/contigo.h"
#include "module/scene/scene.h"

extern u16 *gPaletteColors;
extern u16 *gPaletteCycleSlots;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS(Palette_SetColors, gPaletteSlots, gPaletteColors,
                          gPaletteCycleColors, gPaletteCycleSlots,
                          Func_02000328)

DEFINE_PALETTE_CYCLE_START_VBLANK(Contigo_StartSlowPaletteCycle,
                                  gPaletteCycleIndex, gPaletteCycleLength,
                                  List_CountEntries, gPaletteSlots,
                                  gPaletteCycleDelay, 15, gPaletteCycleTimer,
                                  16, gPaletteCycleBright, PaletteCycle_Fade)

void Contigo_StopPaletteCycle(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 256;
  Task_Remove(PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START_VBLANK(Contigo_StartFastPaletteCycle,
                                  gPaletteCycleIndex, gPaletteCycleLength,
                                  List_CountEntries, gPaletteSlots,
                                  gPaletteCycleDelay, 2, gPaletteCycleTimer, 2,
                                  gPaletteCycleBright, PaletteCycle_Fade)

s32 List_CountEntries(u16 *list) {
  s32 count = 0;

  while (list[count] != 0xffff)
    count++;
  count--;
  return count;
}

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gPaletteCycleTimer,
                          gPaletteCycleDelay, gPaletteCycleColors,
                          gPaletteCycleLength, gPaletteCycleSlots,
                          gPaletteCycleIndex, gPaletteSlots,
                          gPaletteCycleBright, gPaletteColors)

void Teleport_PlayWhirl(void);

void Func_02000328(void) {
  struct ContigoEventWork *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  BattleEffect_ApplyColorToTargetBuffer(0x202108, 1);
  PaletteFade_ApplyTarget(0x10000, 2);
  ((struct ContigoEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Contigo_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  Contigo_StopPaletteCycle();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "../field/object/set_sprite_priority.c"

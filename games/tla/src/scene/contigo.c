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

extern u16 *gData_02006014;
extern u16 *gData_02006004;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_PALETTE_SET_COLORS(Palette_SetColors, gData_02006010, gData_02006014,
    Data_02006018, gData_02006004, Func_02000328)

DEFINE_PALETTE_CYCLE_START_VBLANK(Contigo_StartSlowPaletteCycle, gData_02006020, gData_0200601c,
    Func_020001d4, gData_02006010, gData_0200600c, 15, gData_02006008, 16,
    gData_02006000, PaletteCycle_Fade)

void Contigo_StopPaletteCycle(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 256;
  Task_RemoveFar(PaletteCycle_Fade);
}

DEFINE_PALETTE_CYCLE_START_VBLANK(Contigo_StartFastPaletteCycle, gData_02006020, gData_0200601c,
    Func_020001d4, gData_02006010, gData_0200600c, 2, gData_02006008, 2,
    gData_02006000, PaletteCycle_Fade)

s32 Func_020001d4(u16 *list) {
  s32 count = 0;

  while (list[count] != 0xffff)
    count++;
  count--;
  return count;
}

DEFINE_PALETTE_CYCLE_FADE(PaletteCycle_Fade, gData_02006008, gData_0200600c,
    Data_02006018, gData_0200601c, gData_02006004, gData_02006020,
    gData_02006010, gData_02006000, gData_02006014)

void Func_02004284(void);

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
  Func_02004284();
  wait(40);
  Contigo_StopPaletteCycle();
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
  BattleEffect_StartBufferInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

#include "../field/object/set_sprite_priority.c"

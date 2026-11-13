/// @file
/// The scene code of overlay 0x694, in ROM order.

#include "shared/routines.h"
#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "shared/contigo.h"
#include "task.h"
#include "actor.h"
#include "module/scene/contigo.h"
#include "module/scene/scene.h"

extern u16 *gData_02006014;
extern u16 *gData_02006004;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
DEFINE_FUNC_02000038_4(Func_02000038, gData_02006010, gData_02006014,
    Data_02006018, gData_02006004, Func_02000328)

DEFINE_FUNC_020000F4_2(Func_020000f4, gData_02006020, gData_0200601c,
    Func_020001d4, gData_02006010, gData_0200600c, 15, gData_02006008, 16,
    gData_02006000, Func_020001f8)

void Func_02000158(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 256;
  Task_RemoveFar(Func_020001f8);
}

DEFINE_FUNC_020000F4_2(Func_02000178, gData_02006020, gData_0200601c,
    Func_020001d4, gData_02006010, gData_0200600c, 2, gData_02006008, 2,
    gData_02006000, Func_020001f8)

s32 Func_020001d4(u16 *list) {
  s32 count = 0;

  while (list[count] != 0xffff)
    count++;
  count--;
  return count;
}

void Func_020001f8(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02006008 != 0) {
    if (--gData_02006008 == 0) {
      step = 1;
      gData_02006008 = gData_0200600c;
      if (gData_0200600c > 2)
        gData_0200600c--;
    }
  }
  if (step == 0)
    return;
  src = Data_02006018;
  for (i = 0; i < gData_0200601c; i++) {
    dst = &BG_PLTT[gData_02006004[gData_02006020 * gData_0200601c + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gData_02006010[gData_0200601c]];
  if (gData_02006000 != 0)
    src = gData_02006014 + gData_0200601c * 3;
  else
    src = Data_02006018 + gData_0200601c * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gData_02006000 ^= 1;
  if (++gData_02006020 == gData_0200601c)
    gData_02006020 = 0;
}

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
  Func_02000178();
  Func_02004284();
  wait(40);
  Func_02000158();
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

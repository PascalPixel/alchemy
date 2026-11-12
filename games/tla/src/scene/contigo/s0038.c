#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "contigo.h"
#include "task.h"
#include "actor.h"

extern u16 *gData_02006010;
extern u16 *gData_02006014;
extern u16 *Data_02006018;
extern u16 *gData_02006004;

void Func_02000328(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Func_02000038(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02006010 = slots;
  gData_02006014 = colours;
  Data_02006018 = a;
  gData_02006004 = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(dead): the colour's build and store in a once-only
       * block, so both lists are stepped right after their loads and the
       * next entry's marker loads before 0xffff is built; as plain
       * statements, 22 of 80 instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)BG_PLTT)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_02000328();
}

extern u16 gData_02006000;
extern u16 gData_02006008;
extern u16 gData_0200600c;
extern u16 gData_0200601c;
extern u16 gData_02006020;

s32 Func_020001d4(u16 *list);
void Func_020001f8(void);

void Func_020000f4(void) {
  gData_02006020 = 0;
  gData_0200601c = Func_020001d4(gData_02006010);
  gData_0200600c = 15;
  gData_02006008 = 16;
  gData_02006000 = 1;
  Task_Add(Func_020001f8, TASK_PASS_VBLANK);
}

void Func_02000158(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 256;
  Task_RemoveFar(Func_020001f8);
}

void Func_02000178(void) {
  gData_02006020 = 0;
  gData_0200601c = Func_020001d4(gData_02006010);
  gData_0200600c = 2;
  gData_02006008 = 2;
  gData_02006000 = 1;
  Task_Add(Func_020001f8, TASK_PASS_VBLANK);
}

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
  for (i = 0; i < gData_0200601c;) {
    dst = &BG_PLTT[gData_02006004[gData_02006020 * gData_0200601c + i]];
    red = *src++;
    green = *src++;
    /* FAKEMATCH(dead): the statement in a once-only block, whose end holds
     * the order around it; as the plain statement, 4 of 120 instructions differ
     * in tla-en. */
    do {
      blue = *src++;
    } while (0);
    *dst = (blue << 10) | (green << 5) | red;
    /* FAKEMATCH(dead): the count's step in a once-only block, whose end
     * holds it before the exit test's load; as the for loop's own step, 2 of
     * 120 instructions differ in tla-en. */
    do {
      i++;
    } while (0);
  }
  dst = &BG_PLTT[gData_02006010[gData_0200601c]];
  if (gData_02006000 != 0)
    src = gData_02006014 + gData_0200601c * 3;
  else
    src = Data_02006018 + gData_0200601c * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(dead): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02006000 ^= 1;
  if (++gData_02006020 == gData_0200601c)
    gData_02006020 = 0;
}

struct ContigoEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

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

/// @file
/// Overlay 0x6b0's code from its entry table up to field/object/set_sprite_priority, in ROM order.

#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "types.h"
#include "task.h"
#include "ram_buffer.h"
#include "actor.h"

#include "main_routine_work.h"

extern u16 *gData_02004444;
extern u16 *Data_02004448;
extern u16 *Data_0200444c;
extern u16 *Data_02004438;

void Func_02000328(void);

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

  gData_02004444 = slots;
  Data_02004448 = colours;
  Data_0200444c = a;
  Data_02004438 = b;
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

extern u16 gData_02004434;
extern u16 gData_0200443c;
extern u16 gData_02004440;
extern u16 gData_02004450;
extern u16 gData_02004454;

s32 Func_020001d4(u16 *list);
void Func_020001f8(void);

void Func_020000f4(void) {
  gData_02004454 = 0;
  gData_02004450 = Func_020001d4(gData_02004444);
  gData_02004440 = 15;
  gData_0200443c = 16;
  gData_02004434 = 1;
  Task_AddFrame(Func_020001f8, TASK_PASS_VBLANK);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

void Func_02000158(void) {
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x100;
  FieldTask_Remove((s32)Func_020001f8);
}

void Func_02000178(void) {
  gData_02004454 = 0;
  gData_02004450 = Func_020001d4(gData_02004444);
  gData_02004440 = 2;
  gData_0200443c = 2;
  gData_02004434 = 1;
  Task_AddFrame(Func_020001f8, TASK_PASS_VBLANK);
}

s32 Func_020001d4(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
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
  if (gData_0200443c != 0) {
    if (--gData_0200443c == 0) {
      step = 1;
      gData_0200443c = gData_02004440;
      if (gData_02004440 > 2)
        gData_02004440--;
    }
  }
  if (step == 0)
    return;
  src = Data_0200444c;
  for (i = 0; i < gData_02004450; i++) {
    dst = &BG_PLTT[Data_02004438[gData_02004454 * gData_02004450 + i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    /* FAKEMATCH(dead): the store in a once-only block, whose end holds the
     * order around it; as the plain statement, 2 of 120 instructions differ in
     * tla-en. */
    do {
      *dst = (blue << 10) | (green << 5) | red;
    } while (0);
  }
  dst = &BG_PLTT[gData_02004444[gData_02004450]];
  if (gData_02004434 != 0)
    src = Data_02004448 + gData_02004450 * 3;
  else
    src = Data_0200444c + gData_02004450 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(dead): the last read in a once-only block, whose end holds the
   * order around it; as the plain statement, 2 of 120 instructions differ in
   * tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02004434 ^= 1;
  if (++gData_02004454 == gData_02004450)
    gData_02004454 = 0;
}

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_020017b4(void);
void Func_020034a8(void);

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
  Func_020034a8();
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

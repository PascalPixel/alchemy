#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"
#include "event_inline.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[12];
  void *handle;
};

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Func_02003130(void *handle);
void Func_020024e4(void);
s32 Func_02002708(void);
void Func_02002e6c(struct Actor *self);
#include "main_routine_work.h"

void Func_02003118(struct Actor *object, s32 value);
void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_02001608(void);
void Func_020032a8(void);
void Func_0200166c(void);
void Func_0200055c(void);
void Func_0200170c(void);

void Func_0200166c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(Func_0200170c);
}

#include "mars_lighthouse_room_m.h"

void Func_0200168c(void) {
  gData_02003938 = 0;
  gData_02003934 = Func_020016e8(gData_02003928);
  gData_02003924 = 2;
  gData_02003920 = 2;
  gData_02003918 = 1;
  Task_AddFrame(Func_0200170c, 0x480);
}

s32 Func_020016e8(u16 *list) {
  s32 count = 0;

  while (*list != 0xffff) {
    list++;
    count++;
  }
  count--;
  return count;
}

#include "io_reg.h"

extern u16 *Data_0200391c;
extern u16 *Data_0200392c;
extern u16 *Data_02003930;

void Func_0200170c(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gData_02003920 != 0) {
    if (--gData_02003920 == 0) {
      step = 1;
      gData_02003920 = gData_02003924;
      if (gData_02003924 > 2)
        gData_02003924--;
    }
  }
  if (step == 0)
    return;
  src = Data_02003930;
  for (i = 0; i < gData_02003934; i++) {
    dst = &BG_PLTT[Data_0200391c[gData_02003938 * gData_02003934 + i]];
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
  dst = &BG_PLTT[gData_02003928[gData_02003934]];
  if (gData_02003918 != 0)
    src = Data_0200392c + gData_02003934 * 3;
  else
    src = Data_02003930 + gData_02003934 * 3;
  red = *src++;
  green = *src++;
  /* FAKEMATCH(dead): the last read in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 120 instructions differ
   * in tla-en. */
  do {
    blue = *src++;
  } while (0);
  *dst = (blue << 10) | (green << 5) | red;
  gData_02003918 ^= 1;
  if (++gData_02003938 == gData_02003934)
    gData_02003938 = 0;
}

struct MarsLighthouseRoomMEventWork1ac {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Func_020017b4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02003248_(s32 a, s32 b) {
  BattleEffect_ApplyColorToTargetBuffer(a, b);
}

void Func_0200183c(void) {
  struct MarsLighthouseRoomMEventWork1ac *work = Ram_HeapSlots->eventWork;
  s32 saved;

  saved = work->value1ac;
  PaletteFade_ApplyTarget(0x202108, 1);
  Func_02003248_(0x10000, 2);
  ((struct MarsLighthouseRoomMEventWork1ac *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_0200168c();
  Func_020032a0();
  System_Wait(40);
  Func_0200166c();
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->unk2c2 = 0;
  }
  work->value1ac = saved;
}

s32 Func_020018c4(struct Actor *a, struct Actor *b) {
  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0x80000 &&
      (a->y - b->y >= 0 ? a->y - b->y : b->y - a->y) < 0x80000 &&
      (a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
    return 1;
  return 0;
}

s32 Func_02001928(struct Actor *a, struct Actor *b) {
  s32 height;

  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000) {
    height = a->y - b->y;
    if (height > -0x80000 && height < 0x180000) {
      if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0xc0000)
        return 1;
    }
  }
  return 0;
}

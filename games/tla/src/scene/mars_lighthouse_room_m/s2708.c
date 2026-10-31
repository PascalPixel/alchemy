#include "../../../../games/tla/src/scene/mars_lighthouse_room_m/mars_lighthouse_room_m.h"
#include "flag_call.h"

s32 Func_02002368(s32 a, s32 b);
void Func_02003218(s32 a, s32 b);
void Func_02003240(s32 a, s32 b);
s32 Func_02003168(s32 layer, s32 x, s32 z);
void Func_020024bc(struct Actor *object);
void Animation_ApplyChildValues(struct Actor *object, s32 value);

struct RideEvent {
  u8 unk000[0x15a];
  u16 field15a;
  u16 field15c;
  u16 field15e;
  u8 unk160[4];
  u16 field164;
  u16 field166;
  u8 unk168[0x44];
  s32 field1ac;
  u8 unk1b0[2];
  u16 started;
  s32 field1b4;
};

struct RideCar {
  u8 unk00[0x64];
  s16 stopped;
  u8 unk66[0xa];
  s32 stepX;
  s32 unk74;
  s32 stepZ;
};

struct RideState {
  s32 counter;
  s16 active;
  u8 unk06[0x66];
  struct RideCar *car;
};

s32 Func_02002708(void) {
  /* FAKEMATCH(signature): typed to return s32, which it never sets; as void,
   * 2 of 138 instructions differ in tla-en. */
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  if (event->started == 0) {
    Func_02003240(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Flag_Clear(0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  /* FAKEMATCH(signature): Func_02002368 declared returning s32, which it
   * never sets; as void, 2 of 138 instructions differ in tla-en. */
  Func_02002368(-10, 0);
  Func_02003218(0x70000, 0xe000);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gData_020023c4 + 0x188);
  Flag_Set(0x106);
  Func_020031e0();
  Func_02003280(0);
  Flag_Clear(0x106);
  leader->update = (void (*)(void))Func_020024bc;
  leader->angle = 0x4000;
  Func_02003118(leader, 49);
  // A goto loop: as a while, do-while or for loop, loop.c hoists the
  // 0xfff00000 mask out of the loop into r5; 17 of 138 instructions differ in
  // tla-en.
  if (ride->car->stopped != 0)
    goto done;
step:
  {
    s32 kind = Func_02003168(0, leader->x, leader->z);
    struct RideCar *car = ride->car;

    if (kind != 7) {
      leader->x += car->stepX;
      leader->z += car->stepZ;
    } else {
      if (car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      {
        /* FAKEMATCH(temporary): the car reread through a pointer to its slot,
         * in a block of its own, into r3 for this test as in the native; as
         * ride->car->stepZ, 9 of 138 instructions differ in tla-en. */
        struct RideCar **slot = &ride->car;

        if ((*slot)->stepZ != 0)
          leader->z = (leader->z & 0xfff00000) + 0x80000;
      }
    }
    ride->active = 0;
    wait(1);
  }
  if (ride->car->stopped == 0)
    goto step;
done:
  FieldEvent_Wait(30);
  leader->update = 0;
  Animation_ApplyChildValues(leader, 0);
  ride->active = 0;
}

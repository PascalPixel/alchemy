#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "flag_call.h"

void Func_02001fcc(void);
void Func_020043c4(s32 a, s32 b);
void Func_020043fc(s32 a, s32 b);
s32 Func_0200422c(s32 layer, s32 x, s32 z);
void Func_02002120(struct Actor *object);
void Func_020042dc(void);
void Func_0200444c(s32 skip);
extern u8 gData_020023c4[];
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

// XXX: is the car an actor (its stopped halfword sits where an actor's action
// is, its steps in unk70)? The code that stores the car's pointer at the ride
// state's 0x6c would settle it.
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

s32 Func_0200236c(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  s32 frame[3]; // Unused: gives the stack frame its 12 bytes.

  /* FAKEMATCH(signature): typed returning s32, though it returns nothing; as
   * void, 2 of 138 instructions differ in tla-en. */

  if (event->started == 0) {
    Func_020043fc(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Flag_Clear(0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
#ifdef UBFIX
  Func_02001fcc();
#else
  /* FAKEMATCH(wrapper): Func_02001fcc called through a cast to
   * s32 (*)(s32, s32) with -10 and 0; as Func_02001fcc(), 5 of 138
   * instructions differ in tla-en. */
  ((s32 (*)(s32, s32))Func_02001fcc)(-10, 0);
#endif
  Func_020043c4(0x70000, 0xe000);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gData_020023c4 + 0x188);
  Flag_Set(0x106);
  Func_020042dc();
  Func_0200444c(0);
  Flag_Clear(0x106);
  leader->update = (void (*)(void))Func_02002120;
  leader->angle = 0x4000;
  Func_020041cc(leader, 49);
  // As while (ride->car->stopped == 0), or a do-while under an if, 17 of 138
  // instructions differ in tla-en.
  if (ride->car->stopped != 0)
    goto done;
step:
  {
    s32 kind = Func_0200422c(0, leader->x, leader->z);
    struct RideCar *car = ride->car;

    if (kind != 7) {
      leader->x += car->stepX;
      leader->z += car->stepZ;
    } else {
      if (car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      {
        /* FAKEMATCH(temporary): the car reloaded through a pointer to its
         * slot; as ride->car->stepZ, which keeps the car from the first test,
         * 9 of 138 instructions differ in tla-en. */
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

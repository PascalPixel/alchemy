// Matches: differ 0 of 138 (tla-en, 2026-10-30).
// Draft of overlay 6ab's Func_0200236c, rewritten from its listing as C over
// the ride's state (the four Mars Lighthouse overlays keep one copy each),
// through three tagged devices. The function is typed returning s32 though it
// returns nothing: the use of r0 at its end gives the last argument move one
// more dependent, so sched2 sets r0 for Animation_ApplyChildValues before it
// clears the update hook, as the native does (typed void, the two tie on
// dependents and the store goes first by RTL order; the permuter found only
// return-type changes). The -10 call is declared returning s32, which it does
// not, so its arguments are set in the native's order. Ride_CarSteps reloads
// the car for the second step test into r3 as the native does; the plain
// reread, an array of steps and &stepX indexing all reload it into r1.
#include "../../../../games/tla/src/scene/mars_lighthouse_star_room/mars_lighthouse_star_room.h"
#include "call.h"

s32 Func_02001fcc(s32 a, s32 b);
void Func_020043c4(s32 a, s32 b);
void Func_020043fc(s32 a, s32 b);
s32 Func_0200422c(s32 layer, s32 x, s32 z);
void Func_02002120(void);
void Func_020042dc(void);
void Func_0200444c(s32 value);
void Func_020041cc(struct Actor *object, s32 value);
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
  s32 car;
};

static __inline__ u8 *Ride_CarSteps(u8 *slot) {
  return ((struct Actor *)*(s32 *)slot)->unk70;
}

s32 Func_0200236c(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  s32 frame[3];

  /* FAKEMATCH(signature): typed returning s32, though it returns nothing; as
   * void, 2 of 138 instructions differ in tla-en. */

  if (event->started == 0) {
    Func_020043fc(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  /* FAKEMATCH(signature): Func_02001fcc declared returning s32, though it
   * returns nothing; as void, 2 of 138 instructions differ in tla-en. */
  Func_02001fcc(-10, 0);
  Func_020043c4(0x70000, 0xe000);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gData_020023c4 + 0x188);
  Call1((void (*)())GameFlag_SetBit, 0x106);
  Func_020042dc();
  Func_0200444c(0);
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  leader->update = Func_02002120;
  leader->angle = 0x4000;
  Func_020041cc(leader, 49);
  if (((struct RideCar *)ride->car)->stopped != 0)
    goto done;
step:
  {
    s32 kind = Func_0200422c(0, leader->x, leader->z);
    struct RideCar *car = (struct RideCar *)ride->car;

    if (kind != 7) {
      leader->x += car->stepX;
      leader->z += car->stepZ;
    } else {
      if (car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      /* FAKEMATCH(wrapper): the z step read through Ride_CarSteps; as
       * ((struct RideCar *)ride->car)->stepZ, 6 of 138 instructions differ in
       * tla-en. */
      if (((s32 *)Ride_CarSteps((u8 *)ride + 0x6c))[2] != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    wait(1);
  }
  if (((struct RideCar *)ride->car)->stopped == 0)
    goto step;
done:
  FieldEvent_Wait(30);
  leader->update = 0;
  Animation_ApplyChildValues(leader, 0);
  ride->active = 0;
}

// Draft of overlay 6aa's Func_02002860, rewritten from its listing as C over
// the ride's state (the four Mars Lighthouse overlays keep one copy each):
// differ 4 of 138 in tla-en. Left: the native loads the leader's z before the
// car's z step, and sets r0 for the last Animation_ApplyChildValues before it
// clears the update hook (one row each). The -10 call is declared returning
// s32, which it does not, so its arguments are set in the native's order
// (declared void: 6). Ride_CarSteps reloads the car for the second step test
// into r3 as the native does (as ride->car: 10).
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"
#include "call.h"

s32 Func_020024c0(s32 a, s32 b);
void Func_02004d10(s32 a, s32 b);
void Func_02004d50(s32 a, s32 b);
s32 Func_02004c18(s32 layer, s32 x, s32 z);
void Func_02002614(void);
void Func_02004ca8(void);
void Func_02004da0(s32 value);
void Func_02004bc0(struct Actor *object, s32 value);
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

void Func_02002860(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  s32 frame[3];

  if (event->started == 0) {
    Func_02004d50(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  Func_020024c0(-10, 0);
  Func_02004d10(0x70000, 0xe000);
  leader = Actor_GetById(gAdventure.currentOwner);
  ride = (struct RideState *)(gData_020023c4 + 0x188);
  Call1((void (*)())GameFlag_SetBit, 0x106);
  Func_02004ca8();
  Func_02004da0(0);
  Call1((void (*)())GameFlag_ClearBit, 0x106);
  leader->update = Func_02002614;
  leader->angle = 0x4000;
  Func_02004bc0(leader, 49);
  if (((struct RideCar *)ride->car)->stopped != 0)
    goto done;
step:
  {
    s32 kind = Func_02004c18(0, leader->x, leader->z);
    struct RideCar *car = (struct RideCar *)ride->car;

    if (kind != 7) {
      leader->x += car->stepX;
      leader->z += car->stepZ;
    } else {
      if (car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (((s32 *)Ride_CarSteps((u8 *)ride + 0x6c))[2] != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    WaitFrames(1);
  }
  if (((struct RideCar *)ride->car)->stopped == 0)
    goto step;
done:
  FieldEvent_WaitFrames(30);
  leader->update = 0;
  Animation_ApplyChildValues(leader, 0);
  ride->active = 0;
}

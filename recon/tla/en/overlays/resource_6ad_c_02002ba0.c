// Matches: differ 0 of 138 (tla-en, 2026-10-30).
// Draft of overlay 6ad's Func_02002ba0, rewritten from its listing as C over
// the ride's state (the four Mars Lighthouse overlays keep one copy each).
// Declared returning s32 with no return, the leader's r0 for the last
// Animation_ApplyChildValues is set before the update hook is cleared, as in
// the native (as void: 2). The -10 call is declared returning s32, which it
// does not, so its arguments are set in the native's order (declared void:
// 2). Ride_CarSteps reloads the car for the second step test into r3 as the
// native does (as ride->car: 6). The flag calls go through flag_call.h's
// Flag_Set and Flag_Clear, as the room's matched code does.
#include "../../../../games/tla/src/scene/mars_lighthouse_room_f/room.h"
#include "flag_call.h"

s32 Func_02002800(s32 a, s32 b);
void Func_020055d0(s32 a, s32 b);
void Func_02005610(s32 a, s32 b);
s32 Func_020054d8(s32 layer, s32 x, s32 z);
void Func_02002954(void);
void Func_02005560(void);
void Func_02005648(s32 value);
void Func_02005480(struct Actor *object, s32 value);
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

s32 Func_02002ba0(void) {
  /* FAKEMATCH(signature): declared returning s32 with no return; as void, 2 of 138 instructions differ in tla-en. */
  /* FAKEMATCH(signature): Func_02002800 declared returning s32; as void, 2 of 138 instructions differ in tla-en. */
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  s32 frame[3];

  if (event->started == 0) {
    Func_02005610(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Flag_Clear(0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  Func_02002800(-10, 0);
  Func_020055d0(0x70000, 0xe000);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gData_020023c4 + 0x188);
  Flag_Set(0x106);
  Func_02005560();
  Func_02005648(0);
  Flag_Clear(0x106);
  leader->update = Func_02002954;
  leader->angle = 0x4000;
  Func_02005480(leader, 49);
  if (((struct RideCar *)ride->car)->stopped != 0)
    goto done;
step:
  {
    s32 kind = Func_020054d8(0, leader->x, leader->z);
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

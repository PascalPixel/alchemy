#include "../../../../games/tla/src/scene/mars_lighthouse_room_f/room.h"
#include "event_work.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
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
  u8 unk64[8];
  struct Actor *unk6c;
  void *handle;
};

extern u8 gData_020023c4[];
void Func_02002800(void);
void Func_02002954(struct Actor *object);
void Func_02005610(s32 mode, s32 frames);
s32 Func_020054d8(s32 layer, s32 x, s32 z);

s32 Func_02002ba0(void) {
  /* FAKEMATCH(signature): declared returning s32 with no return; as void, 2
   * of 138 instructions differ in tla-en. */
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct RideWork *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  // Read unsigned: as event->screenStatus, 18 of 138 instructions differ in
  // tla-en.
  if (*(u16 *)&event->screenStatus == 0) {
    Func_02005610(event->screenEffect, event->delay);
    event->screenStatus = 1;
  }
  Flag_Clear(0x106);
  event->unk166 = 0;
  event->unk15a = 0;
  *(u16 *)&event->unk15c[2] = 0;
  *(u16 *)&event->unk15c[0] = 0;
  event->encounterEnemyGroup = 0;
  // UB: Func_02002800 takes no arguments and returns nothing; the bytes call
  // it with two through an int-returning type.
#ifdef UBFIX
  Func_02002800();
#else
  /* FAKEMATCH(wrapper): called through a cast to the int-returning
   * two-argument type, which sets r1 before negating r0; as Func_02002800(),
   * 5 of 138 instructions differ in tla-en. */
  ((s32 (*)(s32, s32))Func_02002800)(-10, 0);
#endif
  Func_020055d0(0x70000, 0xe000);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  ride = (struct RideWork *)(gData_020023c4 + 0x188);
  Flag_Set(0x106);
  Func_02005560();
  Func_02005648(0);
  Flag_Clear(0x106);
  leader->update = (void (*)(void))Func_02002954;
  leader->angle = 0x4000;
  Func_02005480(leader, 49);
  // A goto loop: as a while loop, 17 of 138 instructions differ in tla-en.
  if (ride->unk6c->action != 0)
    goto done;
step:
  {
    s32 kind = Func_020054d8(0, leader->x, leader->z);
    struct Actor *actor = ride->unk6c;

    if (kind != 7) {
      leader->x += ((s32 *)actor->unk70)[0];
      leader->z += ((s32 *)actor->unk70)[2];
    } else {
      if (((s32 *)actor->unk70)[0] != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      {
        /* FAKEMATCH(temporary): the actor at unk6c reread through a pointer
         * to its slot, which keeps the load where the two tests join; as
         * ride->unk6c, 9 of 138 instructions differ in tla-en. */
        struct Actor **slot = &ride->unk6c;

        if (((s32 *)(*slot)->unk70)[2] != 0)
          leader->z = (leader->z & 0xfff00000) + 0x80000;
      }
    }
    ride->active = 0;
    wait(1);
  }
  if (ride->unk6c->action == 0)
    goto step;
done:
  FieldEvent_Wait(30);
  leader->update = 0;
  Animation_ApplyChildValues(leader, 0);
  ride->active = 0;
}

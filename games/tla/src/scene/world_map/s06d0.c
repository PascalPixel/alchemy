#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"
#include "event_inline.h"

extern u8 gMapBlocks[];

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 GetWorldMapCollision(s32 *position);
s32 Func_02004b88(struct Actor *object, s32 *position);
void Func_02004c20(void);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b48(struct Actor *object);
void Func_02004dc8(void);
void Func_02004dc0(void);
void Func_02004c28(void);

void Func_020006d0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ship = Actor_GetById(8);
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 cell = GetWorldMapCollision(&ship->x);
  s32 position[3];
  s32 shore = 0;
  s32 blocked;
  u8 *block;
  u8 *blocks = gMapBlocks;
  u8 flags;

  position[0] = ship->x;
  position[1] = ship->y;
  position[2] = ship->z;
  Vector_AddPolarOffsetFar(0x180000, ship->angle, position);
  blocked = 0;
  block = blocks + ((((position[0] / 0x200000) & 31) +
                     ((position[2] / 0x200000) & 31) * 32) *
                    4);
  if (cell > 12) {
    blocked = 1;
  } else {
    if ((u32)(cell - 1) <= 2)
      shore = 1;
    if ((block[3] & 64) == 0 && shore != 0) {
      blocked = 1;
    } else {
      position[0] += 0x60000;
      blocked |= Func_02004b88(ship, position);
      position[0] += -0xc0000;
      blocked |= Func_02004b88(ship, position);
      position[0] += 0x60000;
      position[1] += 0x60000;
      blocked |= Func_02004b88(ship, position);
      position[1] += -0xc0000;
      blocked |= Func_02004b88(ship, position);
      position[1] += 0x60000;
      blocked |= Func_02004b88(ship, position);
    }
  }
  if (blocked == 0) {
    Event_PlaySound(78);
    Func_02004c20();
    FieldEvent_ResolvePendingActions(0);
    FieldEffect_ScheduleRatioTransition(0x10000, 10);
    gMainRoutineWork.renderMode = 1;
    gMainRoutineWork.ownerOverride = 0;
    Event_ClearFlag(0x176);
    gMainRoutineWork.savedX = ship->x;
    gMainRoutineWork.savedZ = ship->z;
    gMainRoutineWork.unk280 = ship->angle;
    flags = leader->motionFlags;
    leader->x = ship->x;
    leader->y = 0x40000;
    leader->z = ship->z;
    leader->motionFlags = 0;
    Actor_SetSingleChildFlags(leader, 0);
    Func_02004b18(leader, 6);
    System_Wait(6);
    Event_PlaySound(152);
    Func_02004b18(leader, 7);
    leader->speedLimit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocityY = 0x40000;
    leader->motionFlags = flags;
    Func_02004b48(leader);
    FieldActor_SetMoveTarget(leader, position[0], 0, position[2]);
    Actor_WaitForMove(leader);
    Func_02004b18(leader, 6);
    System_Wait(6);
    Func_02004c28();
    Func_02004dc8();
    Func_02004dc0();
    *(s16 *)work->unk15c = 0;
  }
}

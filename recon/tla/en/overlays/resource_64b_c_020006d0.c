// Draft, differ 180 of 251: leaves the ship (actor 8): checks the cell
// ahead of its bow and the cells around it, then the leader jumps ashore.
// Left: the native sets blocked through one shared tail (and places a pool
// there), and its register choice differs (the party leader in r10).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "event_work.h"

extern u8 gMapBlocks[];

void GameFlag_ClearBit(s32 flag);
void WaitTicks(s32 ticks);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 GetWorldMapCollision(s32 *position);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
s32 Func_02004b88(struct ObjectRuntime *object, s32 *position);
void Func_02004c20(void);
void Func_02004db8(s32 value);
void Func_02004e30(s32 cue);
void Func_02004b18(struct ObjectRuntime *object, s32 mode);
void Func_02004b48(struct ObjectRuntime *object);
void Func_02004b50(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004b58(struct ObjectRuntime *object);
void Func_02004dc8(void);
void Func_02004dc0(void);
void Func_02004c28(void);
void Func_02004df8(s32 a, s32 b);

void Func_020006d0(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *ship = Object_GetById(8);
  struct EventWork *work = Ram_HeapSlots->eventWork;
  s32 cell = GetWorldMapCollision(&ship->x);
  s32 position[3];
  s32 shore = 0;
  s32 blocked;
  u8 *block;
  u8 flags;

  position[0] = ship->x;
  position[1] = ship->y;
  position[2] = ship->z;
  Vector_AddPolarOffsetFar(0x180000, ship->angle, position);
  blocked = 0;
  block = &gMapBlocks[(((position[0] / 0x200000) & 31) +
                       ((position[2] / 0x200000) & 31) * 32) *
                      4];
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
  if (blocked != 0)
    return;
  Func_02004e30(78);
  Func_02004c20();
  Func_02004db8(0);
  Func_02004df8(0x10000, 10);
  gPartyState.renderMode = 1;
  gPartyState.ownerOverride = blocked;
  GameFlag_ClearBit(0x176);
  gPartyState.savedX = ship->x;
  gPartyState.savedZ = ship->z;
  gPartyState.unk280 = ship->angle;
  leader->x = ship->x;
  flags = leader->motionFlags;
  leader->y = 0x40000;
  leader->z = ship->z;
  leader->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags(leader, 0);
  Func_02004b18(leader, 6);
  WaitTicks(6);
  Func_02004e30(152);
  Func_02004b18(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x40000;
  leader->motionFlags = flags;
  Func_02004b48(leader);
  Func_02004b50(leader, position[0], 0, position[2]);
  Func_02004b58(leader);
  Func_02004b18(leader, 6);
  WaitTicks(6);
  Func_02004c28();
  Func_02004dc8();
  Func_02004dc0();
  *(s16 *)work->unk15c = blocked;
}

// Draft, differ 168 of 251, skeleton gap 68 (tla-en, 2026-11-01): leaves the
// ship (actor 8): checks the cell ahead of its bow and the cells around it,
// then the leader jumps ashore.
// The stores are the native's plain form, nearly all that is left is one
// cause. gcse's PRE hoists position's address (sfp - 12) into one pseudo that
// lives to Func_02004b50, so it holds r7 through the jump. The native rebuilds
// it there (add r3, sp, #4) and gives r7 to the motion flags' address. That
// one register shifts the rest: native leader r10, blocked r9, shore r11, here
// r9, r11, r10. CSE already shares ownerOverride's HImode zero with
// motionFlags (reg 143 in the cse dump), the native keeps that zero in r11, and
// a constant into a high register comes from the pool. Here all seven high
// registers are taken, it loses its register and is rebuilt with movs. That
// pool's pool_range of 32 is what splits the native's two pools and adds the
// far branch; "movs r3, #1" sharing "mov r9, r3" with the last |= is cross-jump
// on reload's rotating scratch. No shape here has to change for those.
// Tried, no gain: a pointer for position (PRE then takes the bb11 load too), a
// struct position, a forwarding inline for the jump, early return, flags in an
// inner block. A pointer reused for position and motion leaves the rebuild but
// spills shore (frame 20) and is a temporary device. Native's bb11 sharing fits
// CSE following the jump with no PRE of the address: possibly a compiler
// question, not a C one (compare resource_680_c_02000528.c).
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_work.h"

extern u8 gMapBlocks[];

void GameFlag_ClearBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 GetWorldMapCollision(s32 *position);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
s32 Func_02004b88(struct Actor *object, s32 *position);
void Func_02004c20(void);
void Func_02004db8(s32 value);
void Func_02004e30(s32 cue);
void Func_02004b18(struct Actor *object, s32 mode);
void Func_02004b48(struct Actor *object);
void Func_02004b50(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02004b58(struct Actor *object);
void Func_02004dc8(void);
void Func_02004dc0(void);
void Func_02004c28(void);
void Func_02004df8(s32 a, s32 b);

void Func_020006d0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *ship = Actor_GetById(8);
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
  if (blocked == 0) {
    Func_02004e30(78);
    Func_02004c20();
    Func_02004db8(0);
    Func_02004df8(0x10000, 10);
    gMainRoutineWork.renderMode = 1;
    gMainRoutineWork.ownerOverride = 0;
    GameFlag_ClearBit(0x176);
    gMainRoutineWork.savedX = ship->x;
    gMainRoutineWork.savedZ = ship->z;
    gMainRoutineWork.unk280 = ship->angle;
    leader->x = ship->x;
    flags = leader->motionFlags;
    leader->y = 0x40000;
    leader->z = ship->z;
    leader->motionFlags = 0;
    Actor_SetSingleChildFlags(leader, 0);
    Func_02004b18(leader, 6);
    wait(6);
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
    wait(6);
    Func_02004c28();
    Func_02004dc8();
    Func_02004dc0();
    *(s16 *)work->unk15c = 0;
  }
}

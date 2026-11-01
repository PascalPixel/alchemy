// Draft, differ 128 of 251, skeleton gap 32 (tla-en, 2026-11-01): leaves the
// ship (actor 8): checks the cell ahead of its bow and the cells around it,
// then the leader jumps ashore.
// This is the plain form. The same draft with two devices reaches differ 9
// (skeleton gap 7), and those differ only at two sites. The devices are out
// because they do not reach 0. To put them back, change only this:
//   s32 *p = position;              at the top, used nowhere else there
//   p = (s32 *)&leader->motionFlags; flags = *(u8 *)p;   before leader->x
//   *(u8 *)p = 0;  *(u8 *)p = flags;   for the two motionFlags stores
//   p = 0;                          last statement of the if
// Why it works: gcse's PRE copies position's address (sfp - 12) into a new
// pseudo R for its use at Func_02004b50. In the plain form R stays the one
// long-lived pointer, so it holds r7 through the jump. Reusing p (temporary)
// stops CPROP folding R back into p. The dead store (dead) puts p's last
// reference after R's, so cse.c's make_regs_eqv keeps p canonical. R then gets
// no register, its REG_EQUAL becomes REG_EQUIV and reload rematerializes it.
// That gives the native's allocation: leader r10, blocked r9, shore r11, r7
// free for the motion flags, the pooled zero in r11, frame 16, two pools. The
// array spellings matter: p[n] in the first region lets PRE take the bb11 load.
// Left at differ 9:
// 1. The native has add r3, sp, #4; ldr r1, [r3]; ldr r3, [r3, #8]; the draft
//    has ldr r1, [sp, #4]; ldr r3, [sp, #12]. That needs a real Z = sfp - 12.
//    A local defaulted to NULL and set just here keeps Z (first use outside
//    the block makes it canonical, cse.c) but comes out add r0, sp, #4; adds
//    r3, r0, #0, and that reload shifts the last three scratch registers
//    (differ 19). The native's scratch order shows it has no reload there, so
//    Z is set directly. local-alloc only does that when R has two refs and its
//    set's source is the plus (reg_equiv_replace); here R's source is p.
//    gcse hashes every plus into a pseudo, so a second literal plus is
//    redundant. No C form found that gets past this; the likely lever is a
//    first-region occurrence hash_scan_set does not record.
// 2. ldrh r3, [r6, #6] goes one insn early, before movs r1, #160. Casts, a
//    local, a pointer to unk280 and a value temporary did not move it. It
//    looks like the load-after-store scheduling seen elsewhere, unproven.
// Plain-form findings: blocks, a one-constant local, puts gMapBlocks in its
// own pseudo (165 to 128 here, 128 to 42 with the devices). flags read first
// beats reading it after leader->x (136 here).
// Tried, no gain: a static inline for the jump, struct position, early return,
// flags in an inner block, s32/u32/s8 flags or a copy of it, p reset in the
// check block, gMapBlocks as u32 or struct array.
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
    flags = leader->motionFlags;
    leader->x = ship->x;
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

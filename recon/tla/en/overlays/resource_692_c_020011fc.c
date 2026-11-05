// Draft, differ 15 of 148, skeleton gap 4 (tla-en, 2026-11-01)
// The leader rides an object on the cell ahead: if one is there
// (Actor_FindInCell, two cells ahead of the leader's cell centre at its facing
// rounded to a quarter turn), the leader crouches (mode 6), jumps with sound 152
// (mode 7), is carried with it over 12 frames and lands on top of it, falling
// and solid again.
// Left: two things, and all the other differences are pool and branch distances
// from the first. (1) The entry `b` to the bottom test: the listing falls from
// the set of i into the loop head. (2) i = 0 stands before the divisions; the
// listing sets it after the second.
// Why the goto: as a for or do-while entered at the head, gcse's PRE keeps
// leader->x from the stepX division in a register and reloads it on the back
// edge (differ 71; the native rereads it in the body). Entering at the bottom
// test stops that, because the exit edge does not want the load. cse folds the
// i test only by following a jump to a label after a barrier, and this one
// follows i++, so the jump stays. Folding it sooner brings PRE back.
// Why i = 0 sits early: set after the divisions, i crosses one call, passes the
// caller-save test and lands in r1 with a stack save (frame 16, differ 45), and
// r8 and the frame of 12 come back only when it crosses two or more.
// Tried: goto into a do-while (same 15, or 45 with i = 0 late); i = 0 between
// the divisions (same 15); for (;;) with a break (71).
// To close it, find a form with no PRE hoist and no entry block. A store or a
// real call between the load and the loop would do it, but the listing has none.
// Devices: none in the draft.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"

void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *vector);
struct Actor *Actor_FindInCell(s32 *position,
                                        struct Actor *self);
void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004f30(s32 sound);

void Func_020011fc(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *target;
  s32 position[3];
  s32 stepX;
  s32 stepZ;
  s32 i;
  s32 angle = (leader->angle + 0x2000) & 0xc000;

  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y - 0x100000;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  Vector_AddPolarOffsetFar(0x200000, angle, position);
  target = Actor_FindInCell(position, leader);
  if (target == 0)
    return;
  Func_02004d40();
  Func_02004e90(0);
  Func_02004c98(leader, 6);
  wait(6);
  Func_02004f30(152);
  Func_02004c98(leader, 7);
  leader->speedLimit = 0x30000;
  leader->acceleration = 0x20000;
  leader->velocityY = 0x4cccc;
  leader->motionFlags &= 0x7e;
  Actor_SetSingleChildFlags(leader, 0);
  stepX = (position[0] - leader->x) / 12;
  stepZ = (position[2] - leader->z) / 12;
  i = 0;
  do {
    leader->x += target->x - position[0] + stepX;
    leader->z += target->z - position[2] + stepZ;
    position[0] = target->x;
    position[2] = target->z;
    wait(1);
    i++;
  } while (i != 12);
  leader->x = target->x;
  leader->y = leader->terrainHeight = target->y + 0x100000;
  leader->z = target->z;
  Func_02004c98(leader, 6);
  Actor_SetSingleChildFlags(leader, 1);
  leader->motionFlags |= 3;
  Func_02004d48();
}

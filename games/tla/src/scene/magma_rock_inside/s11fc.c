#include "local.h"

void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *vector);
struct Actor *Actor_FindInCell(s32 *position, struct Actor *self);
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

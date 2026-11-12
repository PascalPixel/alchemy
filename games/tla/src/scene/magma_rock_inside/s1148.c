#include "local.h"
#include "macros.h"

struct MagmaRockInsideWork {
  s32 unk00;
  s32 x;
  s32 y;
  s32 z;
  s32 unk10;
  struct Actor *target;
};

void Func_02000ed0(struct Actor *object);
void Func_02004a5c(s32 x, s32 y, s32 z, s32 kind);
void Func_02004b18(void);
void Func_02004bd4(void);
void Func_02004d40(void);
void Func_02004d48(void);

void Func_02001148(void) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;
  struct Actor *object;

  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(10);
  object = work->target;
  object->update = NULL;
  Actor_SetSpriteColor(object, 7);
  FieldEvent_Wait(3);
  Actor_SetSpriteColor(work->target, 0);
  FieldEvent_Wait(1);
  Func_02000ed0(work->target);
  Func_02004d48();
}

void Func_02001190(s32 mode, s32 cue) {
  struct MagmaRockInsideWork *work = Ram_HeapSlots->effectWork;

  if (cue == 0)
    Func_02004b18();
  if (mode == 2) {
    struct Actor *object = work->target;

    Func_02004a5c(object->x, object->y + 0xf0000, object->z, 1);
  }
  if (mode == 3) {
    struct Actor *object = work->target;
    struct Actor *target;

    Func_02004a5c(object->x, object->y + 0xf0000, object->z, 8);
    target = work->target;
    SET_POSITION(target, 0, 0, 0);
  }
  if (cue == 0x3e7)
    Func_02004bd4();
  if (cue == 178)
    Func_02004c98(work->target, 3);
}

struct Actor *Actor_FindInCell(s32 *position, struct Actor *self);

void Func_020011fc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *target;
  s32 position[3];
  s32 stepX;
  s32 stepZ;
  s32 i;
  s32 angle = (leader->angle + 0x2000) & 0xc000;

  position[0] = (leader->x & 0xfff00000) + 0x80000;
  position[1] = leader->y - 0x100000;
  position[2] = (leader->z & 0xfff00000) + 0x80000;
  FieldVector_AddPolarOffset(0x200000, angle, position);
  target = Actor_FindInCell(position, leader);
  if (target == 0)
    return;
  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  Func_02004c98(leader, 6);
  System_Wait(6);
  Event_PlaySound(152);
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
    System_Wait(1);
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

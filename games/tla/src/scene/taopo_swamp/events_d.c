#include "local.h"

void Func_0200065c(void) {
  struct Actor *object = Actor_Get(8);
  s32 speed;
  s32 i;

  Func_02002898();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 8, 0);
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner, 256, 30);
  FieldCamera_MoveTo(PIXELS(408), -1, PIXELS(312), 1);
  Func_02002928();
  Func_02002910_(8, 257);
  speed = 0x8000;
  i = 2;
  do {
    object->velocityY = 0x20000;
    Event_PlaySound(152);
    object->speedLimit = speed;
    object->acceleration = speed / 2;
    Actor_WalkByAndWait(8, 0, 8);
    speed += 0x4000;
  } while (--i >= 0);
  Event_PlaySound(310);
  object->velocityY = 0x40000;
  Actor_WalkByAndWait(8, 0, 8);
  FieldEvent_Wait(60);
  Event_SetFlag(0x310);
  Func_020028a0();
}

void Func_02000710(s32 id) {
  s32 n = 60;
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 3;
  while (n != 0) {
    System_Wait(1);
    if (object->velocityY == 0)
      break;
    n--;
  }
  System_Wait(10);
  object->motionFlags = 0;
}

void Func_02000744(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 9 && object->x >> 20 == 16) {
    Func_02000710(9);
    MapGrid_CopyCellAttributes(15, 26, 1, 1, 16, 26);
    Event_SetFlag(0xa6f);
  }
}

void Func_02000784(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_SetAnimation(id, 1);
  Event_SetFlag(0x220);
  Event_SetFlag(0x222);
}

void Func_020007bc(s32 a, s32 id) {
  Func_02002840_(28, 8, 1, 1, 26, 8);
  FieldActor_SetSpritePriority(id, 2);
  Actor_Get(id)->priorityFlags &= 0xfd;
  Func_02002778(0, id);
  Event_SetFlag(0x221);
  Event_ClearFlag(0x222);
}

void Func_02000810(s32 id) {
  struct Actor *leader;
  struct Actor *object;
  s32 zero = 0;

  Actor_SetAnimation(id, 8);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = (void (*)(void))zero;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  Event_ClearFlag(0x221);
  Event_ClearFlag(0x222);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
    Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  Func_02002840_(22, 8, 1, 1, 26, 8);
}

void Func_020008d8(s32 id) {
  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = 0;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  Event_ClearFlag(0x222);
}

void Func_02000920(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_SetAnimation(id, 1);
  if (a == 5 || FieldGameFlag_GetByte(0x230) > 3) {
    Event_SetFlag(0x220);
    Event_SetFlag(0x222);
  }
}

void Func_0200096c(s32 a, s32 id) {
  Func_02002840_(23, 27, 1, 1, 25, 27);
  FieldActor_SetSpritePriority(id, 2);
  Actor_Get(id)->priorityFlags &= 0xfd;
  Func_02002778(0, id);
  Event_SetFlag(0x221);
  Event_ClearFlag(0x222);
}

void Func_020009c0(s32 id) {
  struct Actor *leader;
  struct Actor *object;

  Actor_SetAnimation(id, 8);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = 0;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  Event_ClearFlag(0x221);
  Event_ClearFlag(0x222);
  GameFlag_SetByte(0x230, 0);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
    Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  Func_02002840_(1, 1, 1, 1, 25, 27);
}

void Func_02000a94(s32 id) {
  s32 zero = 0;

  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = (void (*)(void))zero;
  Func_02002778(0, 0);
  Event_ClearFlag(0x220);
  GameFlag_SetByte(0x230, 0);
}

void Func_02000adc(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x331);
  Event_ClearFlag(0x332);
}

void Func_02000afc(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 7) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    /* FAKEMATCH: the native stores the collision flags before addressing the
     * priority byte. */
    __asm__ volatile("");
    object->priorityFlags |= 2;
    Event_SetFlag(0x332);
    Event_SetFlag(0x333);
    MapGrid_CopyCellAttributes(7, 17, 1, 1, 7, 15);
  }
}

void Func_02000b58(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(0x334);
  Event_ClearFlag(0x335);
  Func_02002840_(27, 14, 1, 1, 27, 13);
}

void Func_02000b90(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->z >> 20 == 16) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    /* FAKEMATCH: the native stores the collision flags before addressing the
     * priority byte. */
    __asm__ volatile("");
    object->priorityFlags |= 2;
    Event_SetFlag(0x335);
    Event_SetFlag(0x336);
    Actor_SetSpeedParameters_(id, 0x8000, 0x4000);
    Actor_SetDestinationOffset(id, -32, 0);
    FieldActor_WaitForMoveById(id);
    Actor_SetAnimation(id, 3);
    MapGrid_CopyCellAttributes(25, 18, 1, 1, 25, 16);
  }
}

void Func_02000c14(struct Actor *object) {
  *(s16 *)((u8 *)object->animation + 18) -= 0x800;
}

void Func_02000c24(void) {
  s32 a;
  s32 x;
  s32 z;
  struct Actor *object;

  gData_02003458--;
  a = Random16Far() * 32;
  x = Random16Far() * 16 + a + 0x12c0000;
  z = Random16Far() * 16 + a + 0x1040000;
  object = FieldActor_Create(0x11d, x, 0, z);
  Func_02002800(object, gData_020029c8);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  if (gData_02003458 == 0)
    FieldTask_Remove(Func_02000c24);
}

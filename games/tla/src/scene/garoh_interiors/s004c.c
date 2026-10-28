#include "garoh_interiors.h"

const u8 *Func_0200004c(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov660Room1)
    return gData_02000964;
  if (scene == (s32)&SceneId_Ov660Room2)
    return gData_02000ab4;
  if (scene == (s32)&SceneId_Ov660Room3)
    return gData_02000b2c;
  return gData_0200094c;
}

void Func_0200009c(s32 actor) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as
   * plain statements the constant is built first (differ 2) */
  do {
    u16 facing = (leader->angle + 0x2000) & ~0x3fff;

    if (facing == 0xc000 && GameFlag_Test(0x91b)) {
      Func_0200089c(10, actor);
    } else {
      if (GameFlag_Test(0x91b))
        Func_02000844((s32)MsgGarohInteriors52);
      else
        Func_02000844((s32)MsgGarohInteriors17);
      Func_0200085c(actor, 0);
    }
  } while (0);
}

void Func_02000114(s32 actor) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as
   * plain statements the constant is built first (differ 2) */
  do {
    u16 facing = (leader->angle + 0x2000) & ~0x3fff;

    if (facing == 0xc000 && GameFlag_Test(0x91b)) {
      Func_0200089c(11, actor);
    } else {
      if (GameFlag_Test(0x91b))
        Func_02000844((s32)MsgGarohInteriors54);
      else
        Func_02000844((s32)MsgGarohInteriors19);
      Func_0200085c(actor, 0);
    }
  } while (0);
}

void Func_0200018c(s32 actor) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as
   * plain statements the constant is built first (differ 2) */
  do {
    u16 facing = (leader->angle + 0x2000) & ~0x3fff;

    if (facing == 0xc000) {
      Func_0200089c(12, actor);
    } else if (GameFlag_Test(0x91b)) {
      Func_02000844((s32)MsgGarohInteriors56);
      Func_0200085c(actor, 0);
    } else {
      s32 message = (s32)MsgGarohInteriors21;

      Func_02000844(message);
      FieldMessage_ShowAtActor(actor, 0);
      if (Event_AskYesNoWithActorAnimation(Func_0200088c(), 0) == 0) {
        FieldEvent_Wait(10);
        Func_02000844(message + 1);
      } else {
        FieldEvent_Wait(20);
        Func_02000844(message + 2);
      }
      Func_0200085c(actor, 0);
    }
  } while (0);
}

void Func_02000230(s32 actor) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  /* FAKEMATCH: the one-pass block keeps the angle load before the constant; as
   * plain statements the constant is built first (differ 2) */
  do {
    u16 facing = (leader->angle + 0x2000) & ~0x3fff;

    if (facing == 0xc000) {
      Func_020008ac(3, actor);
    } else {
      if (GameFlag_Test(0x91b))
        Func_02000844((s32)MsgGarohInteriors58);
      else
        Func_02000844((s32)MsgGarohInteriors23);
      Func_0200085c(actor, 0);
    }
  } while (0);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, frames);
}

void Func_02000298(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  s16 facing;

  u16 angle;

  /* FAKEMATCH: the one-pass block loads the angle before the quarter turn is
   * built. */
  do {
    angle = leader->angle;
  } while (0);
  facing = (angle + 0x2000) & ~0x3fff;
  Actor_SetAngleToward(8, gSequenceParentWork.currentOwner, 0);
  if ((u16)facing == 0xc000) {
    Func_020008a4(8);
  } else {
    if (GameFlag_Test(0x91b))
      Func_02000844((s32)MsgGarohInteriors66);
    else
      Func_02000844((s32)MsgGarohInteriors31);
    Func_0200085c(8, 0);
  }
  Actor_Turn(8, 0x4000, 0);
}

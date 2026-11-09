#include "garoh_interiors.h"
#include "event_inline.h"
#include "scene_tables.h"

const u8 *Func_0200004c(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov660Room1)
    return gData_02000964;
  if (scene == (s32)&SceneId_Ov660Room2)
    return gData_02000ab4;
  if (scene == (s32)&SceneId_Ov660Room3)
    return gData_02000b2c;
  return gData_0200094c;
}

void Func_0200009c(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

  if (facing == 0xc000 && GameFlag_Test(0x91b)) {
    Func_0200089c(10, actor);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors52);
    else
      Event_SetMessageId((s32)MsgGarohInteriors17);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000114(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

  if (facing == 0xc000 && GameFlag_Test(0x91b)) {
    Func_0200089c(11, actor);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors54);
    else
      Event_SetMessageId((s32)MsgGarohInteriors19);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_0200018c(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

  if (facing == 0xc000) {
    Func_0200089c(12, actor);
  } else if (GameFlag_Test(0x91b)) {
    Event_SetMessageId((s32)MsgGarohInteriors56);
    Event_ShowMessageAndWait(actor, 0);
  } else {
    s32 message = (s32)MsgGarohInteriors21;

    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(actor, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(message + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000230(s32 actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  u16 facing = (leader->angle + 0x2000) & ~0x3fff;

  if (facing == 0xc000) {
    Func_020008ac(3, actor);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors58);
    else
      Event_SetMessageId((s32)MsgGarohInteriors23);
    Event_ShowMessageAndWait(actor, 0);
  }
}

void Func_02000298(void) {
  s16 facing;

  facing = ((Actor_GetById(gMainRoutineWork.currentOwner)->angle) + 0x2000) &
           ~0x3fff;
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  if ((u16)facing == 0xc000) {
    Func_020008a4(8);
  } else {
    if (GameFlag_Test(0x91b))
      Event_SetMessageId((s32)MsgGarohInteriors66);
    else
      Event_SetMessageId((s32)MsgGarohInteriors31);
    Event_ShowMessageAndWait(8, 0);
  }
  Actor_FaceDirection(8, FACING_SOUTH, 0);
}

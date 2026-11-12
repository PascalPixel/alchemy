/// @file
/// The scene code of overlay 0x65e that its scene tables belong to, in ROM order; the rest of the overlay is in mikasalla_interiors/.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern const u8 gData_02000840[];
extern const u8 gData_02000960[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2)
    return (const struct ActorPlacement *)gData_02000960;
  return (const struct ActorPlacement *)gData_02000840;
}

void Func_020005c4();
void Func_020005cc();

s32 Func_02000078(void) {
  Func_020005c4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors18);
  Event_ShowMessage(8, 0);
  Func_020005cc();
  return 0;
}

s32 Func_020000a0(void) {
  Func_020005c4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors32);
  Event_ShowMessage(9, 0);
  Func_020005cc();
  return 0;
}

extern const u8 gData_02000a20[];
extern const u8 gData_02000bac[];

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2)
    return (const struct Trigger *)gData_02000bac;
  return (const struct Trigger *)gData_02000a20;
}

void Func_0200063c();
void Func_02000644();
void Func_0200064c();

void Func_020000f4(void) {
  s32 result0;
  s32 result1;

  Func_020005c4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors03);
  Event_OpenMessage(10, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto L_0200812c;
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId(((s32)MsgMikasallaInteriors03 + 1));
  goto L_02008138;
L_0200812c:
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(((s32)MsgMikasallaInteriors03 + 2));
L_02008138:
  Event_ShowMessage(10, 0);
  Func_020005cc();
  return;
}

void Func_0200014c(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_0200063c(13, actor);
  } else {
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors11);
    Event_ShowMessage(actor, 0);
  }
}

void Func_0200019c(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_0200064c(4, actor);
  } else {
    Func_020005c4();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors14);
    Event_ShowMessage(actor, 0);
    Func_020005cc();
  }
}

void Func_020001fc(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Func_020005c4();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17);
    Event_OpenMessage(actor, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(
            FieldParty_GetLeaderActor(), 0) == 0) {
      FieldEvent_Wait(10);
      FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17 + 1);
    } else {
      FieldEvent_Wait(20);
      FieldEvent_SetMessageId((s32)MsgMikasallaInteriors17 + 2);
    }
    Event_ShowMessage(actor, 0);
    Func_020005cc();
  }
}

void Func_02000288(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Func_020005c4();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors29);
    Event_ShowMessage(actor, 0);
    Func_020005cc();
  }
}

void Func_020002e4(s32 actor) {
  u16 facing;

  facing =
      (Actor_Get(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (facing == 0xc000) {
    Func_02000644(actor);
  } else {
    Func_020005c4();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_SetMessageId((s32)MsgMikasallaInteriors27);
    Event_ShowMessage(actor, 0);
    Func_020005cc();
  }
}

void Func_02000454(void);

s32 Func_02000340(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom1) {
    if (gMainRoutineWork.entrance == 5)
      Func_02000454();
    else if (!Event_GetFlag(0x821) ||
             (Event_GetFlag(0xa39) && !Event_GetFlag(0x341)) ||
             Event_GetFlag(34))
      Actor_SetPosition(18, 0, 0);
    else
      Actor_SetSpriteColor(Actor_Get(18), 4);
    return 0;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65ERoom2) {
    s32 entrance = gMainRoutineWork.entrance;

    if (entrance == 1) {
      Actor_Get(8)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(8, 2);
    } else if (entrance == 5) {
      Event_ClearFlag(0x12f);
      Actor_Get(9)->collisionFlags |= 4;
      FieldActor_SetSpritePriority(9, 2);
    } else if (entrance == 4) {
      Event_ClearFlag(0x12f);
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000454(void) {
  *(s32 *)((u8 *)((s32)Ram_HeapSlots->eventWork + 0x1ac) + 0) = 0x209;
  Event_ClearFlag(0x12f);
  return;
}

void Func_020004dc(void);

void Func_02000470(void) {
  if (Event_GetFlag(0xa39)) {
    FieldEvent_SetMessageId(Event_GetFlag(0x340) ? (s32)MsgMikasallaInteriors20
                                            : (s32)MsgMikasallaInteriors21);
    Event_ShowMessage(18, 0);
  } else {
    Func_020004dc();
  }
}

void Func_020004b0(void) {
  s32 result0;

  result0 = Event_GetFlag(0xa39);
  if (result0 != 0)
    goto L_020084c6;
  Func_020004dc();
  goto L_020084d4;
L_020084c6:
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors22);
  Event_ShowMessage(18, 0);
L_020084d4:
  return;
}

void Func_020004dc(void) {
  s32 result0;

  Func_020005c4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMikasallaInteriors19);
  Actor_ShowEmote(18, 0x100, 60);
  Actor_FaceActor(18, gMainRoutineWork.currentOwner, 0);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 60);
  Actor_SetAnimation(18, 4);
  FieldEvent_Wait(15);
  Event_ShowMessage(18, 0);
  Actor_ShowEmote(18, 0x108, 60);
  Event_OpenMessage(18, 0);
  result0 =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  if (result0 != 0)
    goto L_02008568;
  Event_ShowMessage(18, 0);
  Event_SetFlag(0x340);
  goto L_02008582;
L_02008568:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessage(18, 0);
L_02008582:
  Event_SetFlag(0xa39);
  Event_SetFlag(0x341);
  Func_020005cc();
  return;
}

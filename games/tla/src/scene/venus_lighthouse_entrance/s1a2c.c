#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"
s32 Func_020028b4();
s32 Func_020029a4();
void Func_020028bc(void);
extern u8 MsgWhyDoI[];
extern char SceneId_VenusLighthouseEntrance;
extern u8 gMainRoutineWork[];

void Func_02001a2c(void) {
  Func_020028b4();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhyDoI);
  Actor_WalkToAndWait(5, 72, 266);
  Actor_FaceDirection(5, 0x8000, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  FieldActor_SetAnimation(22, 1);
  FieldEvent_Wait(10);
  Actor_ShowEmote(22, 0x101, 50);
  FieldEvent_Wait(10);
  Actor_FaceDirection(22, 0, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 35);
  Actor_FaceDirection(22, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 0xc);
  FieldEvent_ShowMessageThenWait(22, 0, 0);
  Actor_SetAnimation(22, 1);
  Actor_ShowEmote(22, 0x100, 50);
  Actor_FaceDirection(22, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  FieldActor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(22, 0x10000, 0x8000);
  Actor_WalkByAndWait(22, 0x10, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 0xa);
  Actor_SetSpeed(22, 0x20000, 0x10000);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 25);
  FieldActor_Jump(22, 6, 0);
  Actor_WalkBy(22, 0x1a, 0);
  FieldEvent_Wait(8);
  Actor_SetAnimation(22, 1);
  Event_SetAfterBattleDestination((s32)&SceneId_VenusLighthouseEntrance, 0x63);
  Event_SetDefeatDestination((s32)&SceneId_VenusLighthouseEntrance, 0x62);
  FIELD_AT((u8 *)gMainRoutineWork, u8, 587) = 2;
  Call2(Func_020029a4, 9, 0);
  Func_020028bc();
}

#include "types.h"
#include "field_event.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void SceneState_RunActor13AtColumn42Setup(void) {
  struct FieldActor *obj;
  s32 val;
  s32 a;
  s32 b;

  obj = Actor_Get(10);
  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  if (obj->x >> 20 == 42) {
    FieldEvent_Wait(30);
    Event_PlaySound(188);
    obj->motionFlags = 0;
    val = 0xfffe0000;
    obj->terrainHeight = val;
    obj->y = val;
    Event_SetFlag(0x200);
    a = 3;
    b = 5;
    MapGrid_CopyCellsTo(44, 117, 41, 117, a, b);
  }
  Import_EventEnd();
}

void VenusLighthouse_ReadRelief(void) {
  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), 1);
  Message_ShowCentered((s32)MsgVenusLighthouseReliefWords, 1);
  Import_EventEnd();
}

void VenusLighthouse_ReadTrueHeart(void) {
  Import_EventStart();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(FieldParty_GetLeaderActor(), 1);
  Message_ShowCentered((s32)MsgVenusLighthouseTrueHeart, 1);
  Import_EventEnd();
}

#include "types.h"
#include "field_event.h"
#include "text/msg_ids.h"

void SceneState_RunActor13AtColumn42Setup(void) {
  struct FieldActor *obj;
  s32 val;
  s32 a;
  s32 b;

  obj = Import_ActorGet(10);
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  if (obj->x >> 20 == 42) {
    Import_EventWaitFrames(30);
    Import_SoundPlay(188);
    obj->motionFlags = 0;
    val = 0xfffe0000;
    obj->terrainHeight = val;
    obj->y = val;
    Import_GameFlagSet(0x200);
    a = 3;
    b = 5;
    Import_MapCopyCellPictureIndicesRect(44, 117, 41, 117, a, b);
  }
  Import_EventEnd();
}

void VenusLighthouse_ReadRelief(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_ActorSetAnimation(Import_PartyGetLeaderActor(), 1);
  Import_FieldMessageShow((s32)MsgVenusLighthouseReliefWords, 1);
  Import_EventEnd();
}

void VenusLighthouse_ReadTrueHeart(void) {
  Import_EventStart();
  Import_EventResolvePendingActions(0);
  Import_ActorSetAnimation(Import_PartyGetLeaderActor(), 1);
  Import_FieldMessageShow((s32)MsgVenusLighthouseTrueHeart, 1);
  Import_EventEnd();
}

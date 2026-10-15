#include "types.h"
#include "field_obj.h"
#include "field_event.h"

static void SceneActor_RunActorZeroHandledMotion(s32 a) {
  struct FieldActor *v = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Engine_AudioPlayCue(0xe4);
  v->update = (void (*)(union FieldObject *))Mercury_TrailSparks;
  v->speed = 0x3333;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  v->update = 0;
  Engine_EventWait(30);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(a);
  Engine_EventEnd();
}

static void Mercury_DropLeaderToColumn(s32 a0) {
  s32 record;

  Engine_EventBegin();
  Engine_AudioPlayCue(228);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  record = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_EventWait(8);
  Actor_SetPosition(ACTOR_PARTY_LEADER, ((a0 << 19) + 0x80000), 0);
  Engine_EventWait(30);
}

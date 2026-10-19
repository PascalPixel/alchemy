// Mercury Lighthouse and its entrance: leaving a floor through one of its
// sparkling passages.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "songs.h"

// Touch a passage: the leader moves north with the trail of sparks, vanishes,
// and after half a second the screen fades and the party takes exit.
static void MercuryLighthouse_LeaveThroughPassage(s32 exit) {
  struct FieldActor *v = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Engine_AudioPlayCue(SE_228);
#ifdef UBFIX
  v->update = MercuryLighthouse_TrailSparks;
#else
  v->update = (void (*)(union FieldObject *))MercuryLighthouse_TrailSparks;
#endif
  v->speed = 0x3333;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -6);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  v->update = 0;
  Engine_EventWait(30);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(exit);
  Engine_EventEnd();
}

// Unused: the leader moves north with the passages' sound and vanishes, then
// is put at pixel (8 * a0 + 8, 0).
static void UNUSED MercuryLighthouse_DropLeaderToColumn(s32 a0) {
  struct FieldActor *record;

  Engine_EventBegin();
  Engine_AudioPlayCue(SE_228);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  record = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_EventWait(8);
  Actor_SetPosition(ACTOR_PARTY_LEADER, ((a0 << 19) + 0x80000), 0);
  Engine_EventWait(30);
}

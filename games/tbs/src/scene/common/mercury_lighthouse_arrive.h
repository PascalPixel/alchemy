/// @file
/// Mercury Lighthouse and its entrance: arriving on a floor through one of its
/// sparkling passages.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"
#include "songs.h"

/// Unless the party returns to where it was, the leader appears 16 pixels north
/// of the entrance, hidden, as the screen opens, then comes south with the
/// trail of sparks and is drawn again on the way.
static void MercuryLighthouse_ArriveWithSparks(void) {
  struct FieldActor *actor;
  s32 flag;
  struct FieldActor *record;
  struct AnimationObject *sprite;

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  flag = GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0) {
    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    actor->motionFlags = 0;
    Engine_ActorSetPosition(0, actor->x.part.pixel << 16,
                            (actor->z.part.pixel << 16) + -0x100000);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_AudioPlayCue(SE_228);
#ifdef UBFIX
    actor->update = MercuryLighthouse_TrailSparks;
#else
    actor->update =
        (void (*)(union FieldObject *))MercuryLighthouse_TrailSparks;
#endif
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    Actor_SetChildValue(ACTOR_PARTY_LEADER, 0);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Engine_ActorSetSpriteFlags(record, SPRITE_FLAG_SHADOW);
    sprite = actor->sprite;
    sprite->image.priority = 1;
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
    actor->motionFlags = 3;
    actor->update = NULL;
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
  }
}

/// @file
/// Mercury Lighthouse and its entrance: arriving on a floor through one of its
/// sparkling passages.
#include "types.h"
#include "actor.h"
#include "event.h"
#include "flags.h"
#include "songs.h"

/// Unless the party returns to where it was, the leader appears 16 pixels north
/// of the entrance, hidden, as the screen opens, then comes south with the
/// trail of sparks and is drawn again on the way.
static void MercuryLighthouse_ArriveWithSparks(void) {
  struct Actor *actor;
  s32 flag;
  struct Actor *record;
  struct Sprite *sprite;

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  flag = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0) {
    Event_Start();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    actor->motionFlags = 0;
    Import_ActorSetPosition(0, actor->x.part.pixel << 16,
                            (actor->z.part.pixel << 16) + -0x100000);
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Sound_PlaySong(SE_SPARKLING_PASSAGE);
#ifdef UBFIX
    actor->update = MercuryLighthouse_TrailSparks;
#else
    actor->update = (void (*)(struct Actor *))MercuryLighthouse_TrailSparks;
#endif
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x6666, 0x3333);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
    record = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetSpriteFlags(record, SPRITE_FLAG_SHADOW);
    sprite = actor->sprite;
    sprite->image.priority = 1;
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
    actor->motionFlags = 3;
    actor->update = NULL;
    Scene_PlayQueuedMusic();
    Event_End();
  }
}

/// @file
/// Mercury Lighthouse and its entrance: arriving on a floor through one of its
/// sparkling passages.
#include "types.h"
#include "actor.h"
#include "event.h"
#include "flags.h"
#include "sounds.h"

/// Unless the party returns to where it was, the leader appears 16 pixels north
/// of the entrance, hidden, as the screen opens, then comes south with the
/// trail of sparks and is drawn again on the way.
static void MercuryLighthouse_ArriveWithSparks(void) {
  struct Actor *actor;
  struct Sprite *sprite;

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Event_Start();
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    actor->motionFlags = 0;
    Actor_SetPositionFixed(0, px(actor->x.part.pixel),
                           px(actor->z.part.pixel) + -px(16));
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Sound_Play(SE_SPARKLING_PASSAGE);
    actor->update = (void (*)(struct Actor *))MercuryLighthouse_TrailSparks;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.4);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 8);
    Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
    sprite = actor->sprite;
    sprite->image.priority = 1;
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 10);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->update = NULL;
    Scene_PlayQueuedMusic();
    Event_End();
  }
}

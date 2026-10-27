/// @file
/// Mercury Lighthouse and its entrance: leaving a floor through one of its
/// sparkling passages.
#include "types.h"
#include "actor.h"
#include "script_layer.h"
#include "songs.h"

/// Stepping into a passage: the leader moves north with the trail of sparks,
/// vanishes, and after half a second the screen fades and the party takes that
/// exit.
static void MercuryLighthouse_LeaveThroughPassage(s32 exit) {
  struct Actor *v = Actor_GetById(ACTOR_PARTY_LEADER);
  FieldScript_Start();
  Sound_PlaySong(SE_SPARKLING_PASSAGE);
#ifdef UBFIX
  v->update = MercuryLighthouse_TrailSparks;
#else
  v->update = (void (*)(union ActorOrEffect *))MercuryLighthouse_TrailSparks;
#endif
  v->speedLimit = PIXELS(1) / 5;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  ObjectMotion_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 0, -6);
  ObjectMotion_CommitCurrentPositionAndActivate(ACTOR_PARTY_LEADER);
  Actor_ConfigureSpriteColor(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PARTY_LEADER), 0);
  v->update = 0;
  FieldScript_WaitFrames(30);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestExit(exit);
  FieldScript_End();
}

// XXX (no evidence): whether this is an early form of
// MercuryLighthouse_LeaveThroughPassage, which plays the same sound and steps
// the same way; nothing calls it, so nothing shows what would have followed.
/// Unused: the leader steps 8 pixels north with the passages' sound and
/// vanishes, then is put at x 8 * column + 8, z 0, with no exit and no end to
/// the script; nothing in either overlay calls it.
static void UNUSED MercuryLighthouse_DropLeaderToColumn(s32 column) {
  struct Actor *record;

  FieldScript_Start();
  Sound_PlaySong(SE_SPARKLING_PASSAGE);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectMotion_SetActionVariant(ACTOR_PARTY_LEADER, 2);
  ObjectMotion_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 0, -8);
  record = Actor_Get(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(record, 0);
  FieldScript_WaitFrames(8);
  Actor_SetPosition(ACTOR_PARTY_LEADER, ((column << 19) + PIXELS(8)), 0);
  FieldScript_WaitFrames(30);
}

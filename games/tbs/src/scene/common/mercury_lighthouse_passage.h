/// @file
/// Mercury Lighthouse and its entrance: leaving a floor through one of its
/// sparkling passages.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"
#include "songs.h"

/// Stepping into a passage: the leader moves north with the trail of sparks,
/// vanishes, and after half a second the screen fades and the party takes that
/// exit.
static void MercuryLighthouse_LeaveThroughPassage(s32 exit) {
  struct ObjectEvent *v = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  FieldScript_Start();
  Sound_PlaySong(SE_SPARKLING_PASSAGE);
#ifdef UBFIX
  v->update = MercuryLighthouse_TrailSparks;
#else
  v->update =
      (void (*)(union ObjectEventOrEffect *))MercuryLighthouse_TrailSparks;
#endif
  v->speed = PIXELS(1) / 5;
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, 0, -6);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_ConfigureSpriteColor(LOCALID_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
  ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
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
  struct ObjectEvent *record;

  FieldScript_Start();
  Sound_PlaySong(SE_SPARKLING_PASSAGE);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, 0, -8);
  record = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(record, 0);
  FieldScript_WaitFrames(8);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, ((column << 19) + PIXELS(8)),
                          0);
  FieldScript_WaitFrames(30);
}

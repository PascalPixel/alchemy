/// @file
/// Mercury Lighthouse and its entrance: leaving a floor through one of its
/// sparkling passages.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"
#include "songs.h"

/// Stepping into a passage: the leader moves north with the trail of sparks,
/// vanishes, and after half a second the screen fades and the party takes that
/// warp.
static void MercuryLighthouse_LeaveThroughPassage(s32 warp) {
  struct ObjectEvent *v = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ScriptStart();
  Engine_AudioPlayCue(SE_228);
#ifdef UBFIX
  v->update = MercuryLighthouse_TrailSparks;
#else
  v->update = (void (*)(union FieldObject *))MercuryLighthouse_TrailSparks;
#endif
  v->speed = PIXELS(1) / 5;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_WALK);
  Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 0, -6);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetChildValue(LOCALID_PARTY_LEADER, 15);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER),
                                   0);
  v->update = 0;
  Engine_ScriptWait(30);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptRequestWarp(warp);
  Engine_ScriptEnd();
}

/// Unused: the leader steps 8 pixels north with the passages' sound and
/// vanishes, then is put at x 8 * column + 8, z 0, with no warp and no end to
/// the script; nothing in either overlay calls it. XXX: likely an early form of
/// LeaveThroughPassage, because it plays the same sound and steps the same way.
static void UNUSED MercuryLighthouse_DropLeaderToColumn(s32 column) {
  struct ObjectEvent *record;

  Engine_ScriptStart();
  Engine_AudioPlayCue(SE_228);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventSetDestinationOffset(LOCALID_PARTY_LEADER, 0, -8);
  record = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetSpriteFlags(record, 0);
  Engine_ScriptWait(8);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, ((column << 19) + PIXELS(8)),
                          0);
  Engine_ScriptWait(30);
}

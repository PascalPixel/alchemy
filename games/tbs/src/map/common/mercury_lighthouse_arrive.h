/// @file
/// Mercury Lighthouse and its entrance: arriving on a floor through one of its
/// sparkling passages.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"
#include "flags.h"
#include "songs.h"

/// Unless the party returns to where it was, the leader appears 16 pixels north
/// of the entrance, hidden, as the screen opens, then comes south with the
/// trail of sparks and is drawn again on the way.
static void MercuryLighthouse_ArriveWithSparks(void) {
  struct ObjectEvent *objectEvent;
  s32 flag;
  struct ObjectEvent *record;
  struct AnimationObject *sprite;

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  flag = GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0) {
    Engine_ScriptStart();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    objectEvent->motionFlags = 0;
    Engine_ObjectEventSetPosition(0, objectEvent->x.part.pixel << 16,
                                  (objectEvent->z.part.pixel << 16) +
                                      -0x100000);
    ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 15);
    record = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    Engine_ObjectEventSetSpriteFlags(record, 0);
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_AudioPlayCue(SE_228);
#ifdef UBFIX
    objectEvent->update = MercuryLighthouse_TrailSparks;
#else
    objectEvent->update =
        (void (*)(union FieldObject *))MercuryLighthouse_TrailSparks;
#endif
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x6666, 0x3333);
    ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, 8);
    ObjectEvent_SetChildValue(LOCALID_PARTY_LEADER, 0);
    record = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    Engine_ObjectEventSetSpriteFlags(record, SPRITE_FLAG_SHADOW);
    sprite = objectEvent->sprite;
    sprite->image.priority = 1;
    ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, 10);
    objectEvent->motionFlags = 3;
    objectEvent->update = NULL;
    Map_PlayQueuedMusic();
    Engine_ScriptEnd();
  }
}

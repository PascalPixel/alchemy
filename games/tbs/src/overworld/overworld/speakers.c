/// @file
/// Speaker definitions: voices and idle lines by sprite.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Returns the speaker definition of sprite id from
/// gSpeakerDefinitions, or its last entry, the default, when there is
/// none. The search stops after 0x82 entries, all but the table's last of its
/// 131.
struct SpeakerDefinition *Speaker_FindDefinition(u32 id) {
  struct SpeakerDefinition *entry = gSpeakerDefinitions;
  u32 definitionIndex = 0;

  if (entry->id != id) {
    do {
      definitionIndex += 1;
      entry++;
      if (definitionIndex > 0x81)
        break;
    } while (entry->id != id);
  }
  return entry;
}

/// Returns the sprite id stored by a live object event's animated sprite, or 0
/// when that object event has no animated sprite or its sprite-id pointer is
/// absent. The object event must exist: a NULL from ObjectEvent_GetByLocalId is
/// read through. Called by Speaker_GetIdleFlags.
s32 FieldEffect_GetAnimationValue(s32 id) {
  struct SpeakerObject *object = ObjectEvent_GetByLocalId(id);

  if (object->kind != 1 || object->animation == NULL ||
      object->animation->value28 == NULL) {
    return 0;
  }
  return *object->animation->value28;
}

/// Returns the voice of sprite id's messages: the first of its four blip
/// sounds, 0x100 plus its definition's value, or 0 for none (value 0xff) or
/// with the Speech option off (gSaveBlock.speech). Message_OpenWindowFar
/// takes it in the high half of its last argument and
/// Message_SetVoiceFromObjectEvent as the window's voiceSound
/// (field_services/messages.c); the shops ask it for their keepers.
s32 Speaker_GetVoice(u32 id) {
  u8 portraitId;

  if (gSaveBlock.speech == 0 ||
      (portraitId = Speaker_FindDefinition(id)->value) == 0xFF) {
    return 0;
  }
  return portraitId + 0x100;
}

/// Returns the definition flags of object event id's sprite, which choose its
/// line when it has nothing to say: Map_TalkToObjectEvent picks a Halted object
/// event's pair of lines with them, and Halt cast with no map event
/// (FieldEffect_Run) stops nothing when the nearest object event's flags are
/// 0xff (FieldEffect_FilterObjectIdByFlags).
s32 Speaker_GetIdleFlags(s32 id) {
  return Speaker_FindDefinition(FieldEffect_GetAnimationValue(id))->flags;
}

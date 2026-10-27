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

/// Returns the sprite id stored by a live actor's animated sprite, or 0
/// when that actor has no animated sprite or its sprite-id pointer is
/// absent. The actor must exist: a NULL from Actor_Lookup is
/// read through. Called by Speaker_GetIdleFlags.
s32 FieldEffect_GetAnimationValue(s32 id) {
  struct Actor *object = Actor_Lookup(id);
  struct SpeakerSprite *sprite;

  if (object->spriteStorageKind != ACTOR_SPRITE_ONE ||
      (sprite = (struct SpeakerSprite *)object->sprite) == NULL ||
      sprite->firstEntry == NULL) {
    return 0;
  }
  return *sprite->firstEntry;
}

/// Returns the voice of sprite id's messages: the first of its four blip
/// sounds, 0x100 plus its definition's value, or 0 for none (value 0xff) or
/// with the Speech option off (gSaveBlock.speech). Message_OpenWindowFar
/// takes it in the high half of its last argument and
/// Message_SetVoiceFromActor as the window's voiceSound
/// (field_services/messages.c); the shops ask it for their keepers.
s32 Speaker_GetVoice(u32 id) {
  u8 portraitId;

  if (gSaveBlock.speech == 0 ||
      (portraitId = Speaker_FindDefinition(id)->value) == 0xFF) {
    return 0;
  }
  return portraitId + 0x100;
}

/// Returns the definition flags of actor id's sprite, which choose its
/// line when it has nothing to say: Scene_TalkToActor picks a Halted
/// actor's pair of lines with them, and Halt cast with no trigger
/// (FieldEffect_Run) stops nothing when the nearest actor's flags are
/// 0xff (FieldEffect_FilterActorIdByFlags).
s32 Speaker_GetIdleFlags(s32 id) {
  return Speaker_FindDefinition(FieldEffect_GetAnimationValue(id))->flags;
}

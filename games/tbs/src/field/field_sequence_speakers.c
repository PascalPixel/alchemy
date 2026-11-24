/// @file
/// Speaker definitions: voices and Halted lines by sprite.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// The rows of gSpeakerDefinitions Speaker_FindDefinition searches; the row
/// after them is the default.
#define SPEAKER_DEFINITION_COUNT 130

/// Returns the speaker definition of sprite id from gSpeakerDefinitions, or its
/// last entry, the default, when there is none.
const struct SpeakerDefinition *Speaker_FindDefinition(u32 id) {
  const struct SpeakerDefinition *entry = gSpeakerDefinitions;
  u32 definitionIndex = 0;

  if (entry->id != id) {
    do {
      definitionIndex += 1;
      entry++;
      if (definitionIndex >= SPEAKER_DEFINITION_COUNT)
        break;
    } while (entry->id != id);
  }
  return entry;
}

/// Returns the sprite id actor id shows, its one sprite's first animation's,
/// or 0 when it shows no single sprite or that animation is absent.
s32 Speaker_GetActorSprite(s32 id) {
  struct Actor *actor = Actor_Lookup(id);

  if (actor->spriteStorageKind != ACTOR_SPRITE_ONE || actor->sprite == NULL ||
      actor->sprite->entries[0] == NULL) {
    return 0;
  }
  return actor->sprite->entries[0]->animationId;
}

/// Returns the voice of sprite spriteId's messages: the first of its four blip
/// sounds, SE_VOICE_FIRST plus its definition's value, or 0 for none
/// (SPEAKER_VOICE_NONE) or with the Speech option off
/// (gMainRoutineState.speech).
s32 Speaker_GetVoice(u32 spriteId) {
  u8 voice;

  if (gMainRoutineState.speech == 0 ||
      (voice = Speaker_FindDefinition(spriteId)->value) == SPEAKER_VOICE_NONE) {
    return 0;
  }
  return voice + SE_VOICE_FIRST;
}

/// Returns SpeakerDefinition.haltedLines of actor id's sprite: the pair of
/// lines Scene_TalkToActor gives it while Halted, or
/// SPEAKER_HALTED_LINES_NONE when Halt passes it over
/// (FieldPsynergy_FilterHaltTarget).
s32 Speaker_GetHaltedLines(s32 id) {
  return Speaker_FindDefinition(Speaker_GetActorSprite(id))->haltedLines;
}

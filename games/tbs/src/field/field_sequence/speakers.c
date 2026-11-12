/// @file
/// Speaker definitions: voices and Halted lines by sprite.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"

/// Returns the speaker definition of sprite id from
/// gSpeakerDefinitions, or its last entry, the default, when there is
/// none. The search stops after 130 entries, all but the table's last of its
/// 131.
struct SpeakerDefinition *Speaker_FindDefinition(u32 id) {
  struct SpeakerDefinition *entry = gSpeakerDefinitions;
  u32 definitionIndex = 0;

  if (entry->id != id) {
    do {
      definitionIndex += 1;
      entry++;
      if (definitionIndex > 129)
        break;
    } while (entry->id != id);
  }
  return entry;
}

/// Returns the sprite id stored by a live actor's animated sprite, or 0
/// when that actor has no animated sprite or its sprite-id pointer is
/// absent. The actor must exist: a NULL from Actor_Lookup is
/// read through. Called by Speaker_GetHaltedLines.
s32 Speaker_GetActorSprite(s32 id) {
  struct Actor *actor = Actor_Lookup(id);
  struct SpeakerSprite *sprite;

  if (actor->spriteStorageKind != ACTOR_SPRITE_ONE ||
      (sprite = (struct SpeakerSprite *)actor->sprite) == NULL ||
      sprite->firstEntry == NULL) {
    return 0;
  }
  return *sprite->firstEntry;
}

/// Returns the voice of sprite id's messages: the first of its four blip
/// sounds, SE_VOICE_FIRST plus its definition's value, or 0 for none
/// (SPEAKER_VOICE_NONE) or with the Speech option off
/// (gMainRoutineWork.speech).
/// Message_OpenWindowFar takes it in the high half of its last argument and
/// Message_SetVoiceFromActor as the window's voiceSound
/// (event_commands/messages.c); the shops ask it for their keepers.
s32 Speaker_GetVoice(u32 id) {
  u8 voice;

  if (gMainRoutineWork.speech == 0 ||
      (voice = Speaker_FindDefinition(id)->value) == SPEAKER_VOICE_NONE) {
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

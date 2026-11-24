/// @file
/// Who speaks: each sprite's speaker definition, its voice and its halted
/// lines, and the pair table.
// Part of field_sequence.c.

/// The rows of gSpeakerDefinitions Speaker_FindDefinition searches; the row
/// after them is the default.
#define SPEAKER_DEFINITION_COUNT 243

/// Returns the speaker definition of sprite id from gSpeakerDefinitions, or its
/// last entry, the default, when there is none.
struct SpeakerDefinition *Speaker_FindDefinition(u32 id) {
  struct SpeakerDefinition *entry = gSpeakerDefinitions;
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

/// Returns the sprite actor actorId shows, or 0 when it shows no animated
/// sprite.
s32 Speaker_GetActorSprite(u32 actorId) {
  struct Actor *actor = Actor_Lookup(actorId);

  if (actor->spriteStorageKind != ACTOR_SPRITE_ONE || actor->sprite == NULL ||
      ((struct Sprite *)actor->sprite)->entries[0] == NULL) {
    return 0;
  }
  return ((struct Sprite *)actor->sprite)->entries[0]->animationId;
}

/// Returns the voice of sprite id's messages: the first of its four blip
/// sounds, SE_VOICE_FIRST plus its definition's value, or 0 for none
/// (SPEAKER_VOICE_NONE) or with the Speech option off
/// (gMainRoutineState.speech). Message_OpenWindowFar takes it in the high half
/// of its last argument.
s32 Speaker_GetVoice(s32 id) {
  u8 voice;

  if (gMainRoutineState.speech == 0 ||
      (voice = Speaker_FindDefinition(id)->value) == SPEAKER_VOICE_NONE)
    return 0;
  return voice + SE_VOICE_FIRST;
}

/// Returns SpeakerDefinition.haltedLines of actor id's sprite: the pair of
/// lines Scene_TalkToActor gives it while Halted, or
/// SPEAKER_HALTED_LINES_NONE when Halt passes it over
/// (FieldPsynergy_FilterHaltTarget).
s32 Speaker_GetHaltedLines(s32 id) {
  return Speaker_FindDefinition(Speaker_GetActorSprite(id))->haltedLines;
}

extern const s32 gPairTable[];

/// Unused: nothing calls PairTable_FindFar. Looks key up in gPairTable's
/// {key, value} rows and returns its value, or -1; the table holds only its
/// end, so every key gets -1.
s32 PairTable_Find(s32 key) {
  const s32 *entry = gPairTable;
  s32 value;

  while (1) {
    value = entry[0];
    if (value == -1)
      return value;
    if (value == key)
      return entry[1];
    entry += 2;
  }
}

/// Unused: nothing calls PairTable_FindAndIgnoreFar. Looks key up as
/// PairTable_Find does and returns 0.
s32 PairTable_FindAndIgnore(s32 key) {
  PairTable_Find(key);
  return 0;
}

/// Unused: nothing calls PairTable_FindCopyFar. PairTable_Find again.
s32 PairTable_FindCopy(s32 key) { return PairTable_Find(key); }

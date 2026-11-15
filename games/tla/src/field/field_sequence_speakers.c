/// @file
/// Who speaks: each sprite's speaker definition, its voice and its halted
/// lines, and the pair table.
// Part of field_sequence.c.

/// Returns the speaker definition of sprite id from gSpeakerDefinitions, or its
/// last entry, the default, when there is none.
struct SpeakerDefinition *Speaker_FindDefinition(u32 id) {
  struct SpeakerDefinition *entry = gSpeakerDefinitions;
  u32 definitionIndex = 0;

  if (entry->id != id) {
    do {
      definitionIndex += 1;
      entry++;
      if (definitionIndex > 242)
        break;
    } while (entry->id != id);
  }
  return entry;
}

/// Returns the sprite id stored by a live actor's animated sprite, or 0 when
/// that actor has no animated sprite or its sprite-id pointer is absent.
s32 Speaker_GetActorSprite(u32 objectId) {
  struct Actor *object = Actor_Lookup(objectId);

  if (object->spriteStorageKind != ACTOR_SPRITE_ONE || object->sprite == NULL ||
      ((struct Sprite *)object->sprite)->entries[0] == NULL) {
    return 0;
  }
  return ((struct Sprite *)object->sprite)->entries[0]->animationId;
}

/// Returns the voice of sprite id's messages: the first of its four blip
/// sounds, SE_VOICE_FIRST plus its definition's value, or 0 for none
/// (SPEAKER_VOICE_NONE) or with the Speech option off
/// (gMainRoutineWork.speech). Message_OpenWindowFar takes it in the high half
/// of its last argument.
s32 Speaker_GetVoice(s32 id) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  u8 value;

  if (gMainRoutineWork.speech == 0 ||
      (value = Speaker_FindDefinition(id)->value) == SPEAKER_VOICE_NONE)
    return 0;
  return value + SE_VOICE_FIRST;
}

/// Returns the halted lines of actor id's sprite: the pair of lines
/// Scene_TalkToActor gives it while Halted, or 0xff when Halt passes it over
/// (FieldPsynergy_FilterHaltTarget).
s32 Speaker_GetHaltedLines(s32 id) {
  return Speaker_FindDefinition(Speaker_GetActorSprite(id))->haltedLines;
}

extern const s32 gPairTable[];

/// Looks key up in gPairTable's pairs and returns its value, or -1.
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

/// Looks key up as PairTable_Find does and returns 0.
s32 PairTable_FindAndIgnore(s32 key) {
  PairTable_Find(key);
  return 0;
}

/// PairTable_Find again.
s32 PairTable_FindCopy(s32 key) { return PairTable_Find(key); }

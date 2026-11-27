// Who speaks: each sprite's speaker definition, its voice and its halted lines, and the pair table.
// Part of field_mode.c.

// The rows of SpeakerDefinitions fdSpeakerFindDefinition searches; the row after them is the
// default.
#define SPEAKER_DEFINITION_COUNT 243

// Returns the speaker definition of sprite id from SpeakerDefinitions, or its last entry, the
// default, when there is none.
SPEAKER_DEFINITION *fdSpeakerFindDefinition(Uint32 id)
{
    SPEAKER_DEFINITION *entry           = SpeakerDefinitions;
    Uint32              definitionIndex = 0;

    if (entry->id != id)
    {
        do
        {
            definitionIndex += 1;
            entry++;
            if (definitionIndex >= SPEAKER_DEFINITION_COUNT)
                break;
        } while (entry->id != id);
    }
    return entry;
}

// Returns the sprite actor actorId shows, or 0 when it shows no animated sprite.
Sint32 fdSpeakerGetActorSprite(Uint32 actorId)
{
    ACTOR *actor = fdActorLookup(actorId);

    if (actor->spriteStorageKind != ACTOR_SPRITE_ONE || actor->sprite == NULL
        || ((SPR *)actor->sprite)->entries[0] == NULL)
    {
        return 0;
    }
    return ((SPR *)actor->sprite)->entries[0]->animationId;
}

// Returns the voice of sprite id's messages: the first of its four blip sounds, SE_VOICE_FIRST plus
// its definition's value, or 0 for none (SPEAKER_VOICE_NONE) or with the Speech option off
// (MainRoutineStateWork.speech). mnMessageOpenWindowStub takes it in the high half of its last
// argument.
Sint32 fdSpeakerGetVoice(Sint32 id)
{
    Uint8 voice;

    if (MainRoutineStateWork.speech == 0
        || (voice = fdSpeakerFindDefinition(id)->value) == SPEAKER_VOICE_NONE)
        return 0;
    return voice + SE_VOICE_FIRST;
}

// Returns SPEAKER_DEFINITION.haltedLines of actor id's sprite: the pair of lines fdTalkToActor
// gives it while Halted, or SPEAKER_HALTED_LINES_NONE when Halt passes it over
// (fdPsynergyFilterHaltTarget).
Sint32 fdSpeakerGetHaltedLines(Sint32 id)
{
    return fdSpeakerFindDefinition(fdSpeakerGetActorSprite(id))->haltedLines;
}

extern const Sint32 PairTable[];

// Unused: nothing calls fdPairTableFindStub. Looks key up in PairTable's {key, value} rows and
// returns its value, or -1; the table holds only its end, so every key gets -1.
Sint32              fdPairTableFind(Sint32 key)
{
    const Sint32 *entry = PairTable;
    Sint32        value;

    while (1)
    {
        value = entry[0];
        if (value == -1)
            return value;
        if (value == key)
            return entry[1];
        entry += 2;
    }
}

// Unused: nothing calls fdPairTableFindAndIgnoreStub. Looks key up as fdPairTableFind does and
// returns 0.
Sint32 fdPairTableFindAndIgnore(Sint32 key)
{
    fdPairTableFind(key);
    return 0;
}

// Unused: nothing calls fdPairTableFindCopyStub. fdPairTableFind again.
Sint32 fdPairTableFindCopy(Sint32 key)
{
    return fdPairTableFind(key);
}

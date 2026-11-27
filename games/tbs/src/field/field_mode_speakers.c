// Speaker definitions: voices and Halted lines by sprite. Part of field_mode.c.

#include "field_mode_internal.h"

// The rows of SpeakerDefinitions fdSpeakerFindDefinition searches; the row after them is the
// default.
#define SPEAKER_DEFINITION_COUNT 130

// Returns the speaker definition of sprite id from SpeakerDefinitions, or its last entry, the
// default, when there is none.
const SPEAKER_DEFINITION *fdSpeakerFindDefinition(Uint32 id)
{
	const SPEAKER_DEFINITION *entry           = SpeakerDefinitions;
	Uint32                    definitionIndex = 0;

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

// Returns the sprite id actor id shows, its one sprite's first animation's, or 0 when it shows no
// single sprite or that animation is absent.
Sint32 fdSpeakerGetActorSprite(Sint32 id)
{
	ACTOR *actor = fdActorLookup(id);

	if (actor->spriteStorageKind != ACTOR_SPRITE_ONE || actor->sprite == NULL
	    || actor->sprite->entries[0] == NULL)
	{
		return 0;
	}
	return actor->sprite->entries[0]->animationId;
}

// Returns the voice of sprite spriteId's messages: the first of its four blip sounds,
// SE_VOICE_FIRST plus its definition's value, or 0 for none (SPEAKER_VOICE_NONE) or with the Speech
// option off (MainRoutineStateWork.speech).
Sint32 fdSpeakerGetVoice(Uint32 spriteId)
{
	Uint8 voice;

	if (MainRoutineStateWork.speech == 0
	    || (voice = fdSpeakerFindDefinition(spriteId)->value) == SPEAKER_VOICE_NONE)
	{
		return 0;
	}
	return voice + SE_VOICE_FIRST;
}

// Returns SPEAKER_DEFINITION.haltedLines of actor id's sprite: the pair of lines fdTalkToActor
// gives it while Halted, or SPEAKER_HALTED_LINES_NONE when Halt passes it over
// (fdPsynergyFilterHaltTarget).
Sint32 fdSpeakerGetHaltedLines(Sint32 id)
{
	return fdSpeakerFindDefinition(fdSpeakerGetActorSprite(id))->haltedLines;
}

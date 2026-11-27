/* Speaker definitions: voices and Halted lines by sprite. Part of
   field_mode.c. */

#include "field_mode_internal.h"

/* The rows of SpeakerDefinitions fdSpeakerFindDefinition searches; the row
   after them is the default. */
#define SPEAKER_DEFINITION_COUNT 130

/* Returns the speaker definition of sprite id from SpeakerDefinitions, or its
   last entry, the default, when there is none. */
const SPEAKER_DEFINITION *fdSpeakerFindDefinition(Uint32 id)
{
	const SPEAKER_DEFINITION *entry = SpeakerDefinitions;
	Uint32					  definitionIdx = 0;

	if(entry->id != id) {
		do {
			definitionIdx += 1;
			entry++;
			if(definitionIdx >= SPEAKER_DEFINITION_COUNT)
				break;
		} while(entry->id != id);
	}
	return entry;
}

/* Returns the sprite id actor id shows, its one sprite's first animation's, or
   0 when it shows no single sprite or that animation is absent. */
Sint32 fdSpeakerGetActorSpr(Sint32 id)
{
	ACTOR *actor = fdActorLookup(id);

	if(actor->sprStorageKind != ACTOR_SPRITE_ONE || actor->spr == NULL ||
		actor->spr->entries[0] == NULL) {
		return 0;
	}
	return actor->spr->entries[0]->anmId;
}

/* Returns the voice of sprite sprId's messages: the first of its four blip
   sounds, SE_VOICE_FIRST plus its definition's value, or 0 for none
   (SPEAKER_VOICE_NONE) or with the Speech option off
   (MainRoutineStateWork.speech). */
Sint32 fdSpeakerGetVoice(Uint32 sprId)
{
	Uint8 voice;

	if(MainRoutineStateWork.speech == 0 ||
		(voice = fdSpeakerFindDefinition(sprId)->value) == SPEAKER_VOICE_NONE) {
		return 0;
	}
	return voice + SE_VOICE_FIRST;
}

#include "field/speaker_get_halted_lines.c"

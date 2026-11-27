/* fdSpeakerGetHaltedLines, the same in both games. Both games'
   field/field_mode_speakers.c include it where the function stands. */

/* Returns SPEAKER_DEFINITION.haltedLines of actor id's sprite: the pair of
   lines fdTalkToActor gives it while Halted, or SPEAKER_HALTED_LINES_NONE when
   Halt passes it over (fdPsynergyFilterHaltTarget). */
Sint32 fdSpeakerGetHaltedLines(Sint32 id)
{
	return fdSpeakerFindDefinition(fdSpeakerGetActorSpr(id))->haltedLines;
}

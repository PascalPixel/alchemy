/* sySndStopAllMusicPlayers and sySndResumeAllMusicPlayers, the same in both
   games. The Broken Seal's sound/sound.c and The Lost Age's
   sound/sound_play.c include it where the functions stand. */

/* Stops every music player where it is, for sySndResumeAllMusicPlayers to go on
   from. In The Broken Seal only a bare stub, which no code names, leads
   here. */
void sySndStopAllMusicPlayers(void)
{
	m4aMPlayAllStop();
}

/* Resumes every music player from where sySndStopAllMusicPlayers stopped it. In
   The Broken Seal only a bare stub, which no code names, leads here. */
void sySndResumeAllMusicPlayers(void)
{
	m4aMPlayAllContinue();
}

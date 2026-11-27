/* sySndWaitForBGMRestore, the same in both games. The Broken Seal's
   sound/sound.c and The Lost Age's sound/sound_play.c include it where the
   function stands. */

/* Waits until a jingle has ended and the music has begun to come back, giving
   up after BGM_WAIT_TIMEOUT_TICKS. */
void sySndWaitForBGMRestore(void)
{
	Sint32 ticksWaited = 0;

	do {
		if(BgmRestoreDelay == 0)
			break;
		sySynch(1);
		ticksWaited++;
	} while(ticksWaited < BGM_WAIT_TIMEOUT_TICKS);
}

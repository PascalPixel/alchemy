/* mnSndWaitForJingle, the same in both games. Both games' shop/shop_run.c
   include it where the function stands. */

/* Waits until the jingle playing has ended and the music it interrupted is
   back: the inn's night, the sanctum's healing and scripts that wait for a
   jingle. */
void mnSndWaitForJingle(void)
{
	while(sySndGetBGMRestoreDelayStub() != 0) {
		sySynch(1);
	}
}

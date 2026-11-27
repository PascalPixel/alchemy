/* sySetScreenDist, the same in both games. Both games'
   system/system_matrix.c include it where the function stands. */

/* Sets ProjectionWork's focal length and depth range directly. */
void sySetScreenDist(Uint32 focal, Uint32 nearDepth, Uint32 farDepth)
{
	PROJECTION *projection = &ProjectionWork;

	projection->focal = focal;
	projection->near = nearDepth;
	projection->far = farDepth;
}

/* syLookAtLoad, the same in both games. Both games' system/system_matrix.c
   include it where the function stands. */

/* Makes the camera at `eye` looking at `tgt` the whole of CurrentMatrix. */
void syLookAtLoad(POINT eye, POINT tgt)
{
	syMatrixBuildLookAt(eye, tgt, CurrentMatrix);
}

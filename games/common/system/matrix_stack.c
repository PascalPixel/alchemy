/* syPushMatrix to syUnitMatrixCurrent, the same in both games. Both games'
   system/system_matrix.c include it where the functions stand. */

/* Saves CurrentMatrix into the save area (one level deep; a second save is
   ignored). */
void syPushMatrix(void)
{
	if(MatrixStackDepth <= 0) {
		syDMAXCopy(CurrentMatrix, MatrixStackTop, MATRIX_DMA, REG_DMA3);
		MatrixStackDepth++;
		MatrixStackTop += sizeof(CurrentMatrix);
	}
}

/* Copies CurrentMatrix to `dst`. */
void syGetMatrix(MATRIX dst)
{
	syDMAXCopy(CurrentMatrix, dst, MATRIX_DMA, REG_DMA3);
}

/* Copies the matrix at `src` into CurrentMatrix. */
void syLoadMatrix(const MATRIX src)
{
	syDMAXCopy(src, CurrentMatrix, MATRIX_DMA, REG_DMA3);
}

/* Restores CurrentMatrix from the save area, if syPushMatrix saved it. */
void syPopMatrix(void)
{
	if(MatrixStackDepth > 0) {
		--MatrixStackDepth;
		MatrixStackTop -= sizeof(CurrentMatrix);
		syDMAXCopy(MatrixStackTop, CurrentMatrix, MATRIX_DMA, REG_DMA3);
	}
}

/* Resets CurrentMatrix to the unit matrix. Reached through the stub call
   table. */
void syUnitMatrixCurrent(void)
{
	syUnitMatrixCurrentInline();
}

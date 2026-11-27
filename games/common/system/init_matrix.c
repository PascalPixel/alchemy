/* syInitMatrix, the same in both games. Both games' system/system_matrix.c
   include it where the function stands. */

/* Allocates the one-level save area (heap slot HEAP_SLOT_MATRIX_STACK), clears
   MatrixStackDepth and resets CurrentMatrix to the unit matrix. */
void syInitMatrix(void)
{
	MatrixStackTop =
		syHeapSlotAllocEWRAM(HEAP_SLOT_MATRIX_STACK, sizeof(CurrentMatrix));
	MatrixStackDepth = 0;
	syUnitMatrixCurrentInline();
}

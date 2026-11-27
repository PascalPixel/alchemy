/* mnTextSetCanvasPixel, the same in both games. Both games'
   window/glyph_arm.c include it where the function stands. */

/* Sets one pixel in the column-major text canvas. winHeight includes the two
   border rows; doubledPixelRow is twice the row within a tile. repeatedCol
   holds the colour index in all four nibbles. */
static __inline__ void mnTextSetCanvasPixel(Uint32 winHeight, Uint32 x,
	Uint32 tileY, Uint32 doubledPixelRow, Uint8 *canvas, Uint32 repeatedCol)
{
	Uint16 *pixelHalfword;
	Uint32	pixelMask;

	/* The tile is column x / 8's, row tileY down it; within the tile a pixel
	   row is two halfwords of four pixels, the leftmost in the low nibble. */
	pixelHalfword =
		(Uint16 *)(((Uint32)canvas +
					   (((x >> 3) * (winHeight - 2) + tileY) << 5)) |
			((doubledPixelRow | ((x & 7) >> 2)) << 1));
	pixelMask = 15 << ((x & 3) << 2);
	*pixelHalfword = (*pixelHalfword & ~pixelMask) | (pixelMask & repeatedCol);
}

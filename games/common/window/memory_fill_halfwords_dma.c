/* mnMemoryFillHalfwordsDMA, the same in both games. Both games'
   window/window_borders.c include it where the function stands. */

/* Fills count halfwords at dst with value by DMA, and returns the position
   after them. value holds the halfword in both its halves. */
Uint16 *mnMemoryFillHalfwordsDMA(Uint16 *dst, Sint32 value, Sint32 cnt)
{
	if(cnt > 0) {
		DMA_FILL16(value, dst, cnt * 2);
		dst += cnt;
	}
	return dst;
}

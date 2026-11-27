/* fdSprColRemapPixels, the same in both games. Both games'
   field/field_map_grid.c include it where the function stands. */

/* Remaps byteCnt bytes in place through SpriteColorGreyTable. */
void fdSprColRemapPixels(Uint8 *bytes, Sint32 byteCnt)
{
	Uint8 *remapTbl = (Uint8 *)SpriteColorGreyTable;

	while(byteCnt-- != 0) {
		*bytes = remapTbl[*bytes];
		bytes++;
	}
}

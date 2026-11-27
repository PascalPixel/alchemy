/* btTransitionRunMosaicFadeOut and btTransitionBuildAffineTilemap, the same
   in both games. Both games' battle/battle_transition.c include it where the
   functions stand. */

/* Fades the screen to black over sixteen ticks as every background's mosaic
   blocks grow wider, then shows no layer. Sets REG_BLDCNT to darken the
   backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT in mode 1 with
   every layer off; the four BGxCNT are put back as they were. Returns 0. */
Sint32 btTransitionRunMosaicFadeOut(void)
{
	Uint16			 bg0cnt;
	Uint16			 bg1cnt;
	Uint16			 bg2cnt;
	Uint16			 bg3cnt;
	Sint32			 mosaicSize;
	volatile Uint16 *registerCursor;

	syIORead16(&REG_BG0CNT, &bg0cnt);
	REG_BG0CNT = bg0cnt | BGCNT_MOSAIC;
	registerCursor = &REG_BG0CNT;
	syIORead16(++registerCursor, &bg1cnt);
	*registerCursor = bg1cnt | BGCNT_MOSAIC;
	syIORead16(++registerCursor, &bg2cnt);
	*registerCursor = bg2cnt | BGCNT_MOSAIC;
	syIORead16(++registerCursor, &bg3cnt);
	*registerCursor = bg3cnt | BGCNT_MOSAIC;
	registerCursor += &REG_BLDCNT - &REG_BG3CNT;
	*registerCursor = BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BG3 |
		BLDCNT_TGT1_BD | BLDCNT_EFFECT_DARKEN | BLDCNT_TGT2_BG1 |
		BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3 | BLDCNT_TGT2_OBJ | BLDCNT_TGT2_BD;

	syScreenFadeToBlack(16);
	for(mosaicSize = 0; mosaicSize <= MOSAIC_SIZE_MAX; mosaicSize++) {
		/* Four random numbers a tick, drawn only to move the generator on. */
		syRandom();
		syRandom();
		syRandom();
		syRandom();
		/* The backgrounds' and sprites' blocks grow wider, staying a line
		   tall. */
		REG_MOSAIC = (mosaicSize << 8) | mosaicSize;
		sySynch(1);
	}

	REG_DISPCNT = DISPCNT_MODE_1;
	sySynch(4);
	REG_BG0CNT = bg0cnt;
	REG_BG1CNT = bg1cnt;
	REG_BG2CNT = bg2cnt;
	REG_BG3CNT = bg3cnt;
	return 0;
}

/* Builds the zoomed background's 32 by 32 affine map, in which the background
   picture's two halves come from character blocks 2 and 3: the upper half's
   tiles numbered from 0, the lower half's from 0 again, then tile 255 for the
   rest. */
void btTransitionBuildAffineTilemap(void *tilemap)
{
	Uint32 *words = tilemap;
	Uint32	tiles;
	Uint32	i;

	tiles = AFFINE_MAP_ENTRIES(0, 1, 2, 3);
	for(i = 0; i < BACKGROUND_UPPER_TILES / 4; i++) {
		*words++ = tiles;
		tiles += AFFINE_MAP_ENTRIES(4, 4, 4, 4);
	}
	tiles = AFFINE_MAP_ENTRIES(0, 1, 2, 3);
	for(i = 0; i < BACKGROUND_LOWER_TILES / 4; i++) {
		*words++ = tiles;
		tiles += AFFINE_MAP_ENTRIES(4, 4, 4, 4);
	}
	syWordsFill(words, BACKGROUND_AFFINE_MAP_BYTES - BACKGROUND_TILES, -1);
}

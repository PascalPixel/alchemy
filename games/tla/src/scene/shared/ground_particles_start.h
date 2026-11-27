/* Starts the rain about a doused stone: its tiles decoded into VRAM and every
 */
/* drop at the map's origin with a staggered timer, drawn semi-transparent */
/* (fdGroundParticlesDraw); the overlay's copy of */
/* fdEffectInitSparkles. */
void fdGroundParticlesStart(void) {
	GROUND_PARTICLE_STATE *state =
		syHeapSlotAllocEwram(HEAP_SLOT_WEATHER, sizeof(GROUND_PARTICLE_STATE));
	WEATHER_PARTICLE *p = state->particles;
	u8 *buf;
	u32 i;
	u32 size;
	volatile u32 zero;

	size = 0;
	zero = size;
	syDMAXCopy((const void *)&zero, state,
			   DMA_FILL_WORDS(sizeof(GROUND_PARTICLE_STATE) / 4), REG_DMA3);
	buf = syHeapSlotAllocEwram(HEAP_SLOT_TILE_SCRATCH, 1024);
	syDecompressByteLz(Field_GroundTiles, buf);
	state->entry = syVramCacheFindFreeEntry();
	state->tileBase = mnVramLoad(state->entry, 768, buf);
	syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
	state->leader = (s32 *)size;
	i = 0;
	do {
		size = (u32)p;
		*(u32 *)size = 0;
		size += 4;
		*(u32 *)size = OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT);
		size += 4;
		*(u32 *)size = OBJ_PRIORITY(1) | OBJ_PALETTE(11);
		p->posX = 0;
		p->posZ = 0;
		p->posY = toFIXED(fdMapGetTerrainHeight(0, 0, 0));
		p->tmr = (i & (RAIN_SPLASH_FRAMES - 1)) + 1;
		i++;
		p++;
	} while(i < ARRAY_COUNT(state->particles));
	syTaskAddFrame(fdGroundParticlesDraw, TASK_PASS_FRAME);
}

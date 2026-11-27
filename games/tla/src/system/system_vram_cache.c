/* @file */
/* The sprite VRAM cache: blocks, entries and loading pictures, then the */
/* affine matrix builder that follows it. */
/* Part of system.c. */

/* Sprite tile VRAM (OBJ_VRAM0) is VRAM_BLOCK_COUNT blocks of VRAM_BLOCK_BYTES,
 */
/* and VramCacheBlockOwners records each block's owner, its cache entry. An */
/* entry whose offset is VRAM_CACHE_OFFSET_FREE is free, and one whose size is
 */
/* VRAM_CACHE_SIZE_NO_BLOCKS or less owns no VRAM. */

/* Finds the first run of free blocks that holds `size` bytes and gives it to */
/* entry `id`, writing `id` into VramCacheBlockOwners for each block. Returns */
/* the run's byte offset, or -1 when no run fits. */
s32 syVramCacheAllocateBlocks(u32 id, u32 size) {
	u32 blocks;
	s32 first;
	s32 block;
	u32 i;

	blocks = size / VRAM_BLOCK_BYTES;
	if(id >= VRAM_CACHE_ENTRY_COUNT) {
		return -1;
	}
	block = 0;
	for(;;) {
		if(block >= VRAM_BLOCK_COUNT) {
			return -1;
		}
		if(VramCacheBlockOwners[block] == VRAM_BLOCK_OWNER_FREE) {
			if(block + blocks > VRAM_BLOCK_COUNT) {
				return -1;
			}
			first = block;
			for(; block < first + blocks; block++) {
				if(VramCacheBlockOwners[block] != VRAM_BLOCK_OWNER_FREE) {
					goto occupied;
				}
			}
			for(i = 0; i < blocks; i++) {
				VramCacheBlockOwners[first + i] = id;
			}
			break;
		}
	occupied:
		/* Skip an owned block by its owner's whole size. */
		block += (u32)VramCacheEntries[VramCacheBlockOwners[block]].size /
				 VRAM_BLOCK_BYTES;
	}
	first *= VRAM_BLOCK_BYTES;
	return first;
}

/* Unused: returns the length, in blocks, of the longest run of free blocks. */
UNUSED s32 syVramCacheGetLongestFreeRun(void) {
	s32 run = 0;
	s32 longestRun = 0;
	s32 i;

	for(i = 0; i < VRAM_BLOCK_COUNT; i++) {
		if(VramCacheBlockOwners[i] != VRAM_BLOCK_OWNER_FREE) {
			run = 0;
		}
		else {
			run++;
			if(longestRun < run)
				longestRun = run;
		}
	}
	return longestRun;
}

/* Counts the free blocks (owner VRAM_BLOCK_OWNER_FREE) of the sprite VRAM */
/* cache's VRAM_BLOCK_COUNT. */
s32 syVramCacheCountFreeBlocks(void) {
	s32 freeCount = 0;
	s32 i;

	for(i = 0; i < VRAM_BLOCK_COUNT; i++) {
		if(VramCacheBlockOwners[i] == VRAM_BLOCK_OWNER_FREE)
			freeCount++;
	}
	return freeCount;
}

/* Frees every block entry `id` owns in VramCacheBlockOwners. Returns -1 when */
/* it freed any (or `id` is out of range) and 0 when the entry owned none. */
s32 syVramCacheFreeBlocks(s32 id) {
	s32 freed = 0;
	u8 *owner;
	s32 block;

	if((u32)id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	owner = VramCacheBlockOwners;
	for(block = 0; block < VRAM_BLOCK_COUNT; block++, owner++) {
		if(*owner == id) {
			*owner = VRAM_BLOCK_OWNER_FREE;
			freed++;
		}
	}
	if(freed != 0)
		return -1;
	return 0;
}

/* Frees cache entry `id` and the VRAM it holds; returns -1 for an entry out of
 */
/* range. */
s32 syVramCacheFreeEntry(u32 id) {
	VRAM_CACHE_ENTRY *entry = &VramCacheEntries[id];

	if(id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if(entry->ofs != VRAM_CACHE_OFFSET_FREE) {
		syVramCacheFreeBlocks(id);
		entry->ofs |= VRAM_CACHE_OFFSET_FREE;
		entry->size = 0;
	}
	return 0;
}

/* Gives back entry `id`'s blocks but keeps the entry taken, with size 1, so a
 */
/* later syVramCacheLoad can refill it. */
s32 syVramCacheReleaseVram(u32 id) {
	VRAM_CACHE_ENTRY *entry = &VramCacheEntries[id];

	if(id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if(entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
		syVramCacheFreeBlocks(id);
		entry->size = 1;
	}
	return 0;
}

/* Loads `size` bytes from `src` into the sprite VRAM of cache entry `id`, */
/* reusing its blocks when the size is unchanged (`src` 0 loads nothing, -1 */
/* clears); returns the first sprite tile number, or 0 when it does not fit. */
s32 syVramCacheLoad(u32 id, u32 size, const void *src) {
	VRAM_CACHE_ENTRY *entry;
	s32 offset;
	void *dst;

	entry = &VramCacheEntries[id];
	if(id >= VRAM_CACHE_ENTRY_COUNT)
		return 0;
	if(size > VRAM_CACHE_LOAD_MAX)
		return 0;
	/* A same-size upload keeps its existing VRAM run. */
	if(entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
		if(entry->size != size) {
			syVramCacheFreeEntry(id);
			offset = syVramCacheAllocateBlocks(id, size);
		}
		else {
			offset = entry->ofs;
		}
	}
	else {
		offset = syVramCacheAllocateBlocks(id, size);
	}

	if(offset != -1) {
		/* Commit the allocation before copying, clearing or leaving its pixels.
		 */
		dst = (void *)(OBJ_VRAM0 + offset);
		entry->size = size;
		entry->ofs = offset;
		if(src != 0) {
			if(src == (const void *)-1)
				syWordsClearEntry(dst, size);
			else
				syDMAXCopy(src, dst, DMA_COPY_WORDS(size >> 2), REG_DMA3);
		}
		return (u32)offset / TILE_SIZE_4BPP;
	}
	return 0;
}

/* Frees every VRAM block and every cache entry. */
void syVramCacheReset(void) {
	VRAM_CACHE_ENTRY *entry;
	u8 *owner = VramCacheBlockOwners;
	u32 count;

	for(count = 0; count < VRAM_BLOCK_COUNT; count++)
		*owner++ = VRAM_BLOCK_OWNER_FREE;
	entry = VramCacheEntries;
	for(count = 0; count < VRAM_CACHE_ENTRY_COUNT; count++, entry++) {
		entry->ofs |= VRAM_CACHE_OFFSET_FREE;
		entry->size = 0;
	}
}

/* Returns the first free cache entry, or VRAM_CACHE_ENTRY_COUNT when all are */
/* in use. */
s32 syVramCacheFindFreeEntry(void) {
	s32 found = VRAM_CACHE_ENTRY_COUNT;
	s32 id;
	VRAM_CACHE_ENTRY *entry = VramCacheEntries;

	for(id = 0; id < VRAM_CACHE_ENTRY_COUNT; id++, entry++) {
		if(entry->ofs == VRAM_CACHE_OFFSET_FREE) {
			found = id;
			break;
		}
	}
	return found;
}

/* Takes a free cache entry, gives it `size` bytes of sprite VRAM and returns */
/* its number. */
s32 syVramCacheAllocEntry(s32 size) {
	s32 id;

	id = syVramCacheFindFreeEntry();
	syVramCacheLoad(id, size, 0);
	return id;
}

/* Copies `src` again into entry `id`'s VRAM, keeping its size. Returns the */
/* first sprite tile number. */
s32 syVramCacheReload(s32 id, s32 src) {
	return syVramCacheLoad(id, VramCacheEntries[id].size, (const void *)src);
}

/* Takes the next of the OBJ_AFFINE_MATRIX_COUNT sprite affine matrices and */
/* fills it with a rotation by angle and the scales; returns its index, or the
 */
/* last matrix's when all are taken. The body is */
/* syOamMatrixAllocFromTransform's. */
s32 syAffineMatrixBuild(s32 angle, s32 xScale, s32 yScale) {
	AFFINE_MATRIX *matrix;
	s16 *coefficient;
	u8 index;

	index = OamMatrixCount;
	if(index >= OBJ_AFFINE_MATRIX_COUNT)
		return OBJ_AFFINE_MATRIX_COUNT - 1;

	matrix = &OamMatrices[index];
	coefficient = matrix->coefficients;
	OamMatrixCount = index + 1;
	if((xScale == yScale || xScale == -yScale) && angle == 0) {
		/* Unrotated and evenly scaled (or mirrored): pa and pd are the scale's
		 */
		/* reciprocals (FIXED_ONE / scale, the 8.8 inverse of an 8.8 scale), pb
		 * and */
		/* pc 0, two halfwords to a row. */
		s32 reciprocal;
		s32 xReciprocal;

		reciprocal = syIntegerDivide(FIXED_ONE, yScale);
		xReciprocal = reciprocal;
		if(xScale == -yScale)
			xReciprocal = -reciprocal;

		matrix->rows[0] = (u16)xReciprocal;
		matrix->rows[1] = (u32)reciprocal << 16;
	}
	else {
		/* The inverse of rotate-then-scale: a 16.16 sine over an 8.8 scale is
		 * an */
		/* 8.8 coefficient. */
		FIXED sine;
		FIXED cosine;

		sine = sySin(angle);
		cosine = syCos(angle);
		*coefficient = cosine / xScale;
		coefficient++;
		*coefficient = sine / xScale;
		coefficient++;
		*coefficient = (-sine) / yScale;
		coefficient++;
		*coefficient = cosine / yScale;
	}

	return index;
}

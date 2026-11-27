// The sprite VRAM cache: blocks, entries and loading pictures, then the affine matrix builder that
// follows it. Part of system.c.

// Sprite tile VRAM (OBJ_VRAM0) is VRAM_BLOCK_COUNT blocks of VRAM_BLOCK_BYTES, and
// VramCacheBlockOwners records each block's owner, its cache entry. An entry whose offset is
// VRAM_CACHE_OFFSET_FREE is free, and one whose size is VRAM_CACHE_SIZE_NO_BLOCKS or less owns no
// VRAM.

// Finds the first run of free blocks that holds `size` bytes and gives it to entry `id`, writing
// `id` into VramCacheBlockOwners for each block. Returns the run's byte offset, or -1 when no run
// fits.
Sint32 syVRAMCacheAllocateBlocks(Uint32 id, Uint32 size)
{
	Uint32 blocks;
	Sint32 first;
	Sint32 block;
	Uint32 i;

	blocks = size / VRAM_BLOCK_BYTES;
	if (id >= VRAM_CACHE_ENTRY_COUNT)
	{
		return -1;
	}
	block = 0;
	for (;;)
	{
		if (block >= VRAM_BLOCK_COUNT)
		{
			return -1;
		}
		if (VramCacheBlockOwners[block] == VRAM_BLOCK_OWNER_FREE)
		{
			if (block + blocks > VRAM_BLOCK_COUNT)
			{
				return -1;
			}
			first = block;
			for (; block < first + blocks; block++)
			{
				if (VramCacheBlockOwners[block] != VRAM_BLOCK_OWNER_FREE)
				{
					goto occupied;
				}
			}
			for (i = 0; i < blocks; i++)
			{
				VramCacheBlockOwners[first + i] = id;
			}
			break;
		}
	occupied:
		// Skip an owned block by its owner's whole size.
		block += (Uint32)VramCacheEntries[VramCacheBlockOwners[block]].size / VRAM_BLOCK_BYTES;
	}
	first *= VRAM_BLOCK_BYTES;
	return first;
}

// Unused: returns the length, in blocks, of the longest run of free blocks.
UNUSED Sint32 syVRAMCacheGetLongestFreeRun(void)
{
	Sint32 run        = 0;
	Sint32 longestRun = 0;
	Sint32 i;

	for (i = 0; i < VRAM_BLOCK_COUNT; i++)
	{
		if (VramCacheBlockOwners[i] != VRAM_BLOCK_OWNER_FREE)
		{
			run = 0;
		}
		else
		{
			run++;
			if (longestRun < run)
				longestRun = run;
		}
	}
	return longestRun;
}

// Counts the free blocks (owner VRAM_BLOCK_OWNER_FREE) of the sprite VRAM cache's VRAM_BLOCK_COUNT.
Sint32 syVRAMCacheCountFreeBlocks(void)
{
	Sint32 freeCount = 0;
	Sint32 i;

	for (i = 0; i < VRAM_BLOCK_COUNT; i++)
	{
		if (VramCacheBlockOwners[i] == VRAM_BLOCK_OWNER_FREE)
			freeCount++;
	}
	return freeCount;
}

// Frees every block entry `id` owns in VramCacheBlockOwners. Returns -1 when it freed any (or `id`
// is out of range) and 0 when the entry owned none.
Sint32 syVRAMCacheFreeBlocks(Sint32 id)
{
	Sint32 freed = 0;
	Uint8 *owner;
	Sint32 block;

	if ((Uint32)id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	owner = VramCacheBlockOwners;
	for (block = 0; block < VRAM_BLOCK_COUNT; block++, owner++)
	{
		if (*owner == id)
		{
			*owner = VRAM_BLOCK_OWNER_FREE;
			freed++;
		}
	}
	if (freed != 0)
		return -1;
	return 0;
}

// Frees cache entry `id` and the VRAM it holds; returns -1 for an entry out of range.
Sint32 syVRAMCacheFreeEntry(Uint32 id)
{
	VRAM_CACHE_ENTRY *entry = &VramCacheEntries[id];

	if (id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if (entry->offset != VRAM_CACHE_OFFSET_FREE)
	{
		syVRAMCacheFreeBlocks(id);
		entry->offset |= VRAM_CACHE_OFFSET_FREE;
		entry->size    = 0;
	}
	return 0;
}

// Gives back entry `id`'s blocks but keeps the entry taken, with size 1, so a later syVRAMCacheLoad
// can refill it.
Sint32 syVRAMCacheReleaseVRAM(Uint32 id)
{
	VRAM_CACHE_ENTRY *entry = &VramCacheEntries[id];

	if (id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if (entry->size > VRAM_CACHE_SIZE_NO_BLOCKS)
	{
		syVRAMCacheFreeBlocks(id);
		entry->size = 1;
	}
	return 0;
}

// Loads `size` bytes from `src` into the sprite VRAM of cache entry `id`, reusing its blocks when
// the size is unchanged (`src` 0 loads nothing, -1 clears); returns the first sprite tile number,
// or 0 when it does not fit.
Sint32 syVRAMCacheLoad(Uint32 id, Uint32 size, const void *src)
{
	VRAM_CACHE_ENTRY *entry;
	Sint32            offset;
	void             *dest;

	entry = &VramCacheEntries[id];
	if (id >= VRAM_CACHE_ENTRY_COUNT)
		return 0;
	if (size > VRAM_CACHE_LOAD_MAX)
		return 0;
	// A same-size upload keeps its existing VRAM run.
	if (entry->size > VRAM_CACHE_SIZE_NO_BLOCKS)
	{
		if (entry->size != size)
		{
			syVRAMCacheFreeEntry(id);
			offset = syVRAMCacheAllocateBlocks(id, size);
		}
		else
		{
			offset = entry->offset;
		}
	}
	else
	{
		offset = syVRAMCacheAllocateBlocks(id, size);
	}

	if (offset != -1)
	{
		// Commit the allocation before copying, clearing or leaving its pixels.
		dest          = (void *)(OBJ_VRAM0 + offset);
		entry->size   = size;
		entry->offset = offset;
		if (src != 0)
		{
			if (src == (const void *)-1)
				syWordsClearEntry(dest, size);
			else
				syDMAXCopy(src, dest, DMA_COPY_WORDS(size >> 2), REG_DMA3);
		}
		return (Uint32)offset / TILE_SIZE_4BPP;
	}
	return 0;
}

// Frees every VRAM block and every cache entry.
void syVRAMCacheReset(void)
{
	VRAM_CACHE_ENTRY *entry;
	Uint8            *owner = VramCacheBlockOwners;
	Uint32            count;

	for (count = 0; count < VRAM_BLOCK_COUNT; count++)
		*owner++ = VRAM_BLOCK_OWNER_FREE;
	entry = VramCacheEntries;
	for (count = 0; count < VRAM_CACHE_ENTRY_COUNT; count++, entry++)
	{
		entry->offset |= VRAM_CACHE_OFFSET_FREE;
		entry->size    = 0;
	}
}

// Returns the first free cache entry, or VRAM_CACHE_ENTRY_COUNT when all are in use.
Sint32 syVRAMCacheFindFreeEntry(void)
{
	Sint32            found = VRAM_CACHE_ENTRY_COUNT;
	Sint32            id;
	VRAM_CACHE_ENTRY *entry = VramCacheEntries;

	for (id = 0; id < VRAM_CACHE_ENTRY_COUNT; id++, entry++)
	{
		if (entry->offset == VRAM_CACHE_OFFSET_FREE)
		{
			found = id;
			break;
		}
	}
	return found;
}

// Takes a free cache entry, gives it `size` bytes of sprite VRAM and returns its number.
Sint32 syVRAMCacheAllocateEntry(Sint32 size)
{
	Sint32 id;

	id = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(id, size, 0);
	return id;
}

// Copies `src` again into entry `id`'s VRAM, keeping its size. Returns the first sprite tile
// number.
Sint32 syVRAMCacheReload(Sint32 id, Sint32 src)
{
	return syVRAMCacheLoad(id, VramCacheEntries[id].size, (const void *)src);
}

// Takes the next of the OBJ_AFFINE_MATRIX_COUNT sprite affine matrices and fills it with a rotation
// by angle and the scales; returns its index, or the last matrix's when all are taken. The body is
// syOAMMatrixAllocateFromTransform's.
Sint32 syAffineMatrixBuild(Sint32 angle, Sint32 xScale, Sint32 yScale)
{
	AFFINE_MATRIX *matrix;
	Sint16        *coefficient;
	Uint8          index;

	index = OamMatrixCount;
	if (index >= OBJ_AFFINE_MATRIX_COUNT)
		return OBJ_AFFINE_MATRIX_COUNT - 1;

	matrix         = &OamMatrices[index];
	coefficient    = matrix->coefficients;
	OamMatrixCount = index + 1;
	if ((xScale == yScale || xScale == -yScale) && angle == 0)
	{
		// Unrotated and evenly scaled (or mirrored): pa and pd are the scale's reciprocals
		// (toFIXED(1) / scale, the 8.8 inverse of an 8.8 scale), pb and pc 0, two halfwords to a
		// row.
		Sint32 reciprocal;
		Sint32 xReciprocal;

		reciprocal  = syIntegerDivide(toFIXED(1), yScale);
		xReciprocal = reciprocal;
		if (xScale == -yScale)
			xReciprocal = -reciprocal;

		matrix->rows[0] = (Uint16)xReciprocal;
		matrix->rows[1] = (Uint32)reciprocal << 16;
	}
	else
	{
		// The inverse of rotate-then-scale: a 16.16 sine over an 8.8 scale is an 8.8 coefficient.
		FIXED sine;
		FIXED cosine;

		sine         = sySin(angle);
		cosine       = syCos(angle);
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

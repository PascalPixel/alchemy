/* The sprite VRAM cache: blocks, entries and loading pictures. Part of
   system.c. */

/* Sprite tile VRAM (OBJ_VRAM0) is VRAM_BLOCK_COUNT blocks of VRAM_BLOCK_BYTES,
   and VramCacheBlockOwners records each block's owner, its cache entry. An
   entry whose offset is VRAM_CACHE_OFFSET_FREE is free, and one whose size is
   VRAM_CACHE_SIZE_NO_BLOCKS or less owns no VRAM. */

/* Finds the first run of free blocks that holds `size` bytes and gives it to
   entry `id`, writing `id` into VramCacheBlockOwners for each block. */
Sint32 syVRAMCacheAllocateBlocks(Uint32 id, Uint32 size)
{
	Uint32 blocks;
	Sint32 ret;
	Sint32 pos;
	Uint32 end;
	Uint32 i;

	blocks = size / VRAM_BLOCK_BYTES;
	if(id >= VRAM_CACHE_ENTRY_COUNT) {
		return -1;
	}
	pos = 0;
	for(;;) {
		ret = -1;
		if(pos >= VRAM_BLOCK_COUNT) {
			return ret;
		}
		if(VramCacheBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
			ret = pos;
			end = blocks + ret;
			for(; pos < end; pos++) {
				if(VramCacheBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
					goto occupied;
				}
			}
			for(i = 0; i < blocks; i++) {
				VramCacheBlockOwners[ret + i] = id;
			}
			break;
		}
	occupied:
		/* Skip an owned block by its owner's whole size. */
		pos +=
			VramCacheEntries[VramCacheBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
	}
	ret *= VRAM_BLOCK_BYTES;
	return ret;
}

/* Unused: returns the length, in blocks, of the longest run of free blocks. */
Sint32 syVRAMCacheGetLongestFreeRun(void)
{
	Sint32 run = 0;
	Sint32 longestRun = 0;
	Sint32 i;

	for(i = 0; i < VRAM_BLOCK_COUNT; i++) {
		if(VramCacheBlockOwners[i] != VRAM_BLOCK_OWNER_FREE) {
			run = 0;
		} else {
			run++;
			if(longestRun < run)
				longestRun = run;
		}
	}
	return longestRun;
}

/* Frees every block entry `id` owns in VramCacheBlockOwners. Returns -1 when it
   freed any (or `id` is out of range) and 0 when the entry owned none. */
Sint32 syVRAMCacheFreeBlocks(Sint32 id)
{
	Sint32 freed = 0;
	Uint8 *owner;
	Sint32 block;

	if((Uint32)id >= VRAM_CACHE_ENTRY_COUNT)
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

/* Frees cache entry `id`: its blocks go back and the entry is marked free
   (offset VRAM_CACHE_OFFSET_FREE). */
Sint32 syVRAMCacheFreeEntry(Uint32 id)
{
	VRAM_CACHE_ENTRY *entry = &VramCacheEntries[id];

	if(id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if(entry->offset != VRAM_CACHE_OFFSET_FREE) {
		syVRAMCacheFreeBlocks(id);
		entry->offset |= VRAM_CACHE_OFFSET_FREE;
		entry->size = 0;
	}
	return 0;
}

/* Gives back entry `id`'s blocks but keeps the entry taken, with size 1, so a
   later syVRAMCacheLoad can refill it. */
Sint32 syVRAMCacheReleaseVRAM(Uint32 id)
{
	VRAM_CACHE_ENTRY *entry = &VramCacheEntries[id];

	if(id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if(entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
		syVRAMCacheFreeBlocks(id);
		entry->size = 1;
	}
	return 0;
}

/* Gives cache entry `id` `size` bytes of sprite VRAM and copies `src` there, or
   clears them for VRAM_CACHE_CLEAR, or leaves them for a null `src`. An entry
   that already holds VRAM of the same size keeps it. Returns the first sprite
   tile number of its VRAM, or 0 when it cannot. */
Sint32 syVRAMCacheLoad(Uint32 id, Uint32 size, const void *src)
{
	VRAM_CACHE_ENTRY *entry;
	Sint32			  offset;
	void			 *dest;

	entry = &VramCacheEntries[id];
	if(id >= VRAM_CACHE_ENTRY_COUNT)
		return 0;
	if(size > VRAM_CACHE_LOAD_MAX)
		return 0;
	/* A same-size upload keeps its existing VRAM run. */
	if(entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
		if(entry->size != size) {
			syVRAMCacheFreeEntry(id);
			offset = syVRAMCacheAllocateBlocks(id, size);
		} else {
			offset = entry->offset;
		}
	} else {
		offset = syVRAMCacheAllocateBlocks(id, size);
	}

	if(offset != -1) {
		/* Commit the allocation before copying, clearing or leaving its
		   pixels. */
		dest = (void *)(OBJ_VRAM0 + offset);
		entry->size = size;
		entry->offset = offset;
		if(src != 0) {
			if(src == VRAM_CACHE_CLEAR) {
				syWordsClear(dest, size);
			} else {
				syDMAXCopy(src, dest, DMA_COPY_WORDS(size >> 2), REG_DMA3);
			}
		}
		return (Uint32)offset / TILE_SIZE_4BPP;
	}
	return 0;
}

/* Frees every block and every cache entry. */
void syVRAMCacheReset(void)
{
	Uint32 limit = VRAM_BLOCK_COUNT - 1;
	Uint8 *p = VramCacheBlockOwners;
	Uint32 n;

	for(n = 0; n <= limit; n++) {
		*p++ = VRAM_BLOCK_OWNER_FREE;
	}

	{
		VRAM_CACHE_ENTRY *entry = VramCacheEntries;

		for(n = 0; n < VRAM_CACHE_ENTRY_COUNT; n++) {
			entry->offset |= VRAM_CACHE_OFFSET_FREE;
			entry->size = 0;
			entry++;
		}
	}
}

/* Returns the number of the first free cache entry, or VRAM_CACHE_ENTRY_COUNT
   when all are taken. */
Sint32 syVRAMCacheFindFreeEntry(void)
{
	Sint32			  ret = VRAM_CACHE_ENTRY_COUNT;
	Sint32			  id;
	VRAM_CACHE_ENTRY *entry = VramCacheEntries;

	for(id = 0; id < VRAM_CACHE_ENTRY_COUNT; id++, entry++) {
		if(entry->offset == VRAM_CACHE_OFFSET_FREE) {
			ret = id;
			break;
		}
	}
	return ret;
}

/* Takes a free cache entry, gives it `size` bytes of sprite VRAM and returns
   its number. */
Sint32 syVRAMCacheAllocateEntry(Sint32 size)
{
	Sint32 id;

	id = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(id, size, 0);
	return id;
}

/* Copies `src` again into entry `id`'s VRAM, keeping its size. Returns the
   first sprite tile number. */
Sint32 syVRAMCacheReload(Sint32 id, Sint32 src)
{
	return syVRAMCacheLoad(id, VramCacheEntries[id].size, (const void *)src);
}

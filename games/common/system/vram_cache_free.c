/* syVRAMCacheFreeBlocks, syVRAMCacheFreeEntry and syVRAMCacheReleaseVRAM,
   the same in both games. Both games' system/system_vram_cache.c include it
   where the functions stand. */

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

/* Frees cache entry `id` and the VRAM it holds: its blocks go back and the
   entry is marked free (offset VRAM_CACHE_OFFSET_FREE). Returns -1 for an entry
   out of range. */
Sint32 syVRAMCacheFreeEntry(Uint32 id)
{
	VRAM_CACHE_ENTRY *entry = &VramCacheEntries[id];

	if(id >= VRAM_CACHE_ENTRY_COUNT)
		return -1;
	if(entry->ofs != VRAM_CACHE_OFFSET_FREE) {
		syVRAMCacheFreeBlocks(id);
		entry->ofs |= VRAM_CACHE_OFFSET_FREE;
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

/* syVRAMCacheAllocEntry and syVRAMCacheReload, the same in both games. Both
   games' system/system_vram_cache.c include it where the functions stand. */

/* Takes a free cache entry, gives it `size` bytes of sprite VRAM and returns
   its number. */
Sint32 syVRAMCacheAllocEntry(Sint32 size)
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

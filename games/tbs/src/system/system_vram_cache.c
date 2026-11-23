/// @file
/// The sprite VRAM cache: blocks, entries and loading pictures.
// Part of system.c.

#include "system_internal.h"

// Sprite tile VRAM (OBJ_VRAM0) is VRAM_BLOCK_COUNT blocks of VRAM_BLOCK_BYTES,
// and gVramCacheBlockOwners records each block's owner, its cache entry. An
// entry whose offset is VRAM_CACHE_OFFSET_FREE is free, and one whose size is
// VRAM_CACHE_SIZE_NO_BLOCKS or less owns no VRAM.

/// Finds the first run of free blocks that holds `size` bytes and gives it to
/// entry `id`, writing `id` into gVramCacheBlockOwners for each block.
s32 VramCache_AllocateBlocks(u32 id, u32 size) {
  u32 blocks;
  s32 ret;
  s32 pos;
  u32 end;
  u32 i;

  blocks = size / VRAM_BLOCK_BYTES;
  if (id >= VRAM_CACHE_ENTRY_COUNT) {
    return -1;
  }
  pos = 0;
  for (;;) {
    ret = -1;
    if (pos >= VRAM_BLOCK_COUNT) {
      return ret;
    }
    if (gVramCacheBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
      ret = pos;
      end = blocks + ret;
      for (; pos < end; pos++) {
        if (gVramCacheBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
      }
      for (i = 0; i < blocks; i++) {
        gVramCacheBlockOwners[ret + i] = id;
      }
      break;
    }
  occupied:
    // Skip an owned block by its owner's whole size.
    pos +=
        gVramCacheEntries[gVramCacheBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
  }
  ret *= VRAM_BLOCK_BYTES;
  return ret;
}

/// Unused: returns the length, in blocks, of the longest run of free blocks.
s32 VramCache_GetLongestFreeRun(void) {
  s32 run = 0;
  s32 longestRun = 0;
  s32 i;

  for (i = 0; i < VRAM_BLOCK_COUNT; i++) {
    if (gVramCacheBlockOwners[i] != VRAM_BLOCK_OWNER_FREE) {
      run = 0;
    } else {
      run++;
      if (longestRun < run)
        longestRun = run;
    }
  }
  return longestRun;
}

/// Frees every block entry `id` owns in gVramCacheBlockOwners. Returns -1 when
/// it freed any (or `id` is out of range) and 0 when the entry owned none.
s32 VramCache_FreeBlocks(s32 id) {
  s32 freed = 0;
  u8 *owner;
  s32 block;

  if ((u32)id >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  owner = gVramCacheBlockOwners;
  for (block = 0; block < VRAM_BLOCK_COUNT; block++, owner++) {
    if (*owner == id) {
      *owner = VRAM_BLOCK_OWNER_FREE;
      freed++;
    }
  }
  if (freed != 0)
    return -1;
  return 0;
}

/// Frees cache entry `id`: its blocks go back and the entry is marked free
/// (offset VRAM_CACHE_OFFSET_FREE).
s32 VramCache_FreeEntry(u32 id) {
  struct VramCacheEntry *entry = &gVramCacheEntries[id];

  if (id >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
    VramCache_FreeBlocks(id);
    entry->offset |= VRAM_CACHE_OFFSET_FREE;
    entry->size = 0;
  }
  return 0;
}

/// Gives back entry `id`'s blocks but keeps the entry taken, with size 1, so a
/// later VramCache_Load can refill it.
s32 VramCache_ReleaseVram(u32 id) {
  struct VramCacheEntry *entry = &gVramCacheEntries[id];

  if (id >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
    VramCache_FreeBlocks(id);
    entry->size = 1;
  }
  return 0;
}

/// Gives cache entry `id` `size` bytes of sprite VRAM and copies `src` there,
/// or clears them for VRAM_CACHE_CLEAR, or leaves them for a null `src`. An
/// entry that already holds VRAM of the same size keeps it. Returns the first
/// sprite tile number of its VRAM, or 0 when it cannot.
s32 VramCache_Load(u32 id, u32 size, const void *src) {
  struct VramCacheEntry *entry;
  s32 offset;
  void *dst;

  entry = &gVramCacheEntries[id];
  if (id >= VRAM_CACHE_ENTRY_COUNT)
    return 0;
  if (size > VRAM_CACHE_LOAD_MAX)
    return 0;
  // A same-size upload keeps its existing VRAM run.
  if (entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
    if (entry->size != size) {
      VramCache_FreeEntry(id);
      offset = VramCache_AllocateBlocks(id, size);
    } else {
      offset = entry->offset;
    }
  } else {
    offset = VramCache_AllocateBlocks(id, size);
  }

  if (offset != -1) {
    // Commit the allocation before copying, clearing or leaving its pixels.
    dst = (void *)(OBJ_VRAM0 + offset);
    entry->size = size;
    entry->offset = offset;
    if (src != 0) {
      if (src == VRAM_CACHE_CLEAR) {
        Iwram_ClearWords(dst, size);
      } else {
        Dma_Set(src, dst, DMA_COPY_WORDS(size >> 2), REG_DMA3);
      }
    }
    return (u32)offset / TILE_SIZE_4BPP;
  }
  return 0;
}

/// Frees every block and every cache entry.
void VramCache_Reset(void) {
  u32 limit = VRAM_BLOCK_COUNT - 1;
  u8 *p = gVramCacheBlockOwners;
  u32 n;

  for (n = 0; n <= limit; n++) {
    *p++ = VRAM_BLOCK_OWNER_FREE;
  }

  {
    struct VramCacheEntry *entry = gVramCacheEntries;

    for (n = 0; n < VRAM_CACHE_ENTRY_COUNT; n++) {
      entry->offset |= VRAM_CACHE_OFFSET_FREE;
      entry->size = 0;
      entry++;
    }
  }
}

/// Returns the number of the first free cache entry, or VRAM_CACHE_ENTRY_COUNT
/// when all are taken.
s32 VramCache_FindFreeEntry(void) {
  s32 ret = VRAM_CACHE_ENTRY_COUNT;
  s32 id;
  struct VramCacheEntry *entry = gVramCacheEntries;

  for (id = 0; id < VRAM_CACHE_ENTRY_COUNT; id++, entry++) {
    if (entry->offset == VRAM_CACHE_OFFSET_FREE) {
      ret = id;
      break;
    }
  }
  return ret;
}

/// Takes a free cache entry, gives it `size` bytes of sprite VRAM and returns
/// its number.
s32 VramCache_AllocEntry(s32 size) {
  s32 id;

  id = VramCache_FindFreeEntry();
  VramCache_Load(id, size, 0);
  return id;
}

/// Copies `src` again into entry `id`'s VRAM, keeping its size. Returns the
/// first sprite tile number.
s32 VramCache_Reload(s32 id, s32 src) {
  return VramCache_Load(id, gVramCacheEntries[id].size, (const void *)src);
}

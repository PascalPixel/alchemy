/// @file
/// The sprite VRAM cache: blocks, entries and loading pictures.
// Part of system.c.

#include "../system_internal.h"

// The sprite VRAM cache. Sprite tile memory (from 0x06010000) is split into
// 512 blocks of 64 bytes, and gVramCacheBlockOwners records which of the 96
// cache entries owns each block. An entry whose offset is
// VRAM_CACHE_OFFSET_FREE is free; one whose size is 16 or less holds no VRAM.

/// Finds the first run of free blocks that holds `size` bytes and gives it to
/// entry `id`, writing `id` into gVramCacheBlockOwners for each block. Called
/// by VramCache_Load. Returns its byte offset in sprite VRAM, or -1 when no run
/// is long enough. The run is `size` / 64 blocks, rounded down, so a size
/// that is not a multiple of 64 gets less than it asked for.
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
      while (pos < end) {
        if (gVramCacheBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
        pos++;
      }
      for (i = 0; i < blocks; i++) {
        gVramCacheBlockOwners[ret + i] = id;
      }
      break;
    }
  occupied:
    // Skip an owned block by its owner's whole size. This assumes the owner's
    // run starts here, which holds while runs are only ever taken whole.
    pos +=
        gVramCacheEntries[gVramCacheBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
  }
  ret *= VRAM_BLOCK_BYTES;
  return ret;
}

/// Returns the length, in blocks, of the longest run of free blocks. Nothing
/// in ☀️ calls it.
s32 VramCache_GetLongestFreeRun(void) {
  s32 run = 0;
  u8 *owner = gVramCacheBlockOwners;
  s32 longestRun = 0;
  s32 remaining = VRAM_BLOCK_COUNT;

  do {
    if (*owner++ != VRAM_BLOCK_OWNER_FREE) {
      run = 0;
    } else {
      run++;
      if (longestRun < run)
        longestRun = run;
    }
    remaining--;
  } while (remaining != 0);
  return longestRun;
}

/// Frees every block entry `id` owns in gVramCacheBlockOwners. Called by
/// VramCache_FreeEntry and VramCache_ReleaseVram. Returns -1 when it freed any
/// (or `id` is out of range) and 0 when the entry owned none.
s32 VramCache_FreeBlocks(s32 id) {
  s32 cleared = 0;
  s32 remaining;
  u8 *owner;
  u8 empty;

  if ((u32)id >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  owner = gVramCacheBlockOwners;
  empty = VRAM_BLOCK_OWNER_FREE;
  remaining = VRAM_BLOCK_COUNT;
  do {
    if (*owner == id) {
      *owner = empty;
      cleared++;
    }
    remaining--;
    owner++;
  } while (remaining != 0);
  if (cleared != 0)
    return -1;
  return 0;
}

/// Frees cache entry `no`: its blocks go back and the entry is marked free
/// (offset VRAM_CACHE_OFFSET_FREE). Called throughout the game when a sprite's
/// picture is no longer shown.
s32 VramCache_FreeEntry(u32 no) {
  struct VramCacheEntry *entry = &gVramCacheEntries[no];

  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
    VramCache_FreeBlocks(no);
    entry->offset |= VRAM_CACHE_OFFSET_FREE;
    entry->size = 0;
  }
  return 0;
}

/// Gives back entry `no`'s blocks but keeps the entry taken, with size 1, so
/// a later VramCache_Load can refill it. Called by the field sprites and
/// cameras.
s32 VramCache_ReleaseVram(u32 no) {
  struct VramCacheEntry *entry = &gVramCacheEntries[no];

  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->size > 16) {
    s32 one;

    VramCache_FreeBlocks(no);
    one = 1;
    entry->size = one;
  }
  return 0;
}

/// Gives cache entry `no` `size` bytes of sprite VRAM and copies `src` there.
/// An entry that already holds VRAM of the same size keeps it. A null `src`
/// copies nothing and (void *)-1 clears the memory. Writes the entry's offset
/// and size into gVramCacheEntries. Called throughout the game to load sprite
/// pictures. Returns the first sprite tile number (32-byte units), or 0 when
/// there was no room; a run at offset 0 also returns 0, so a caller cannot
/// tell the two apart from the result. The entry keeps its VRAM until
/// VramCache_FreeEntry, VramCache_ReleaseVram or VramCache_Reset.
s32 VramCache_Load(u32 no, u32 size, const void *src) {
  struct VramCacheEntry *entry;
  s32 offset;
  void *dst;

  entry = &gVramCacheEntries[no];
  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return 0;
  // 8 KB, 128 blocks, is a quarter of sprite VRAM and the most one entry holds
  if (size > 8192)
    return 0;
  // A same-size upload keeps its existing VRAM run. Sizes of 16 or less mark
  // an entry that holds no VRAM (VramCache_ReleaseVram leaves 1).
  if (entry->size > 16) {
    if (entry->size != size) {
      VramCache_FreeEntry(no);
      offset = VramCache_AllocateBlocks(no, size);
    } else {
      offset = entry->offset;
    }
  } else {
    offset = VramCache_AllocateBlocks(no, size);
  }

  if (offset != -1) {
    // Commit the allocation before copying, clearing or leaving its pixels.
    dst = (void *)(OBJ_VRAM0 + offset);
    entry->size = size;
    entry->offset = offset;
    if (src != 0) {
      if (src == (const void *)-1) {
        Iwram_ClearWords(dst, size);
      } else {
        Dma_Set(src, dst, DMA_COPY_WORDS(size >> 2), REG_DMA3);
      }
    }
    return (u32)offset >> 5;
  }
  return 0;
}

/// Frees every block and every cache entry. Called by System_Initialize,
/// MainRoutine_Run and the debug modes that start over.
void VramCache_Reset(void) {
  u32 limit = VRAM_BLOCK_COUNT - 1;
  u8 *p = gVramCacheBlockOwners;
  u32 n = 0;
  u32 empty = VRAM_BLOCK_OWNER_FREE;

  do {
    *p++ = empty;
    n++;
  } while (n <= limit);

  {
    struct VramCacheEntry *entry = gVramCacheEntries;

    n = 0;
    do {
      entry->offset |= VRAM_CACHE_OFFSET_FREE;
      entry->size = 0;
      entry++;
      n++;
    } while (n < VRAM_CACHE_ENTRY_COUNT);
  }
}

/// Returns the number of the first free cache entry, or VRAM_CACHE_ENTRY_COUNT
/// when all are taken. Called before VramCache_Load by code that keeps the
/// entry number.
s32 VramCache_FindFreeEntry(void) {
  s32 ret = VRAM_CACHE_ENTRY_COUNT;
  s32 no;
  struct VramCacheEntry *entry = gVramCacheEntries;

  for (no = 0; no < VRAM_CACHE_ENTRY_COUNT; no++, entry++) {
    if (entry->offset == VRAM_CACHE_OFFSET_FREE) {
      ret = no;
      break;
    }
  }
  return ret;
}

/// Takes a free cache entry, gives it `size` bytes of sprite VRAM and returns
/// its number. Called by the battle prompts and the menus' command rows.
/// When every entry is taken it returns VRAM_CACHE_ENTRY_COUNT, and when no
/// run of blocks is long enough the entry it returns stays free.
s32 VramCache_AllocEntry(s32 size) {
  s32 no;

  no = VramCache_FindFreeEntry();
  VramCache_Load(no, size, 0);
  return no;
}

/// Copies `src` again into entry `no`'s VRAM, keeping its size. Called by
/// code that redraws a sprite picture in place, such as the battle prompts.
/// Returns the first sprite tile number.
s32 VramCache_Reload(s32 no, s32 src) {
  return VramCache_Load(no, gVramCacheEntries[no].size, (const void *)src);
}

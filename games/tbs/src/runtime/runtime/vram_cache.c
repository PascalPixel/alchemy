/// @file
/// The sprite VRAM cache: blocks, entries and loading pictures.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

// The sprite VRAM cache. Sprite tile memory (from 0x06010000) is split into
// 512 blocks of 64 bytes, and gResourceBlockOwners records which of the 96
// cache entries owns each block. An entry whose offset is 0xffff is free; one
// whose size is 16 or less holds no VRAM.

/// Finds the first run of free blocks that holds `size` bytes and gives it to
/// entry `id`, writing `id` into gResourceBlockOwners for each block. Called by
/// VramCache_Load. Returns its byte offset in sprite VRAM, or -1 when no run
/// is long enough.
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
    if (gResourceBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
      ret = pos;
      end = blocks + ret;
      while (pos < end) {
        if (gResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
        pos++;
      }
      for (i = 0; i < blocks; i++) {
        gResourceBlockOwners[ret + i] = id;
      }
      break;
    }
  occupied:
    // Skip an owned block by its owner's whole size
    pos += gVramBlockCache[gResourceBlockOwners[pos]].size / VRAM_BLOCK_BYTES;
  }
  ret *= VRAM_BLOCK_BYTES;
  return ret;
}

/// Returns the length, in blocks, of the longest run of free blocks. Nothing
/// in ☀️ calls it.
s32 VramCache_GetLongestFreeRun(void) {
  s32 run = 0;
  u8 *owner = gResourceBlockOwners;
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

/// Frees every block entry `id` owns in gResourceBlockOwners. Called by
/// Resource_ResetEntry and VramCache_ReleaseVram. Returns -1 when it freed any
/// (or `id` is out of range) and 0 when the entry owned none.
s32 VramCache_FreeBlocks(s32 id) {
  s32 cleared = 0;
  s32 remaining;
  u8 *owner;
  u8 empty;

  if ((u32)id >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  owner = gResourceBlockOwners;
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
/// (offset 0xffff). Called throughout the game when a sprite's picture is no
/// longer shown.
s32 Resource_ResetEntry(u32 no) {
  struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

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
  struct VramBlockCacheEntry *entry = &gVramBlockCache[no];

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
/// and size into gVramBlockCache. Called throughout the game to load sprite
/// pictures. Returns the first sprite tile number (32-byte units), or 0 when
/// there was no room.
s32 VramCache_Load(u32 no, u32 size, const void *src) {
  struct VramBlockCacheEntry *entry;
  s32 offset;
  void *dst;

  entry = &gVramBlockCache[no];
  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return 0;
  if (size > 8192)
    return 0;
  // A same-size upload keeps its existing VRAM run.
  if (entry->size > 16) {
    if (entry->size != size) {
      Resource_ResetEntry(no);
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
/// Game_ResetForNewGame and the debug modes that start over.
void VramCache_Reset(void) {
  u32 limit = VRAM_BLOCK_COUNT - 1;
  u8 *p = gResourceBlockOwners;
  u32 n = 0;
  u32 empty = VRAM_BLOCK_OWNER_FREE;

  do {
    *p++ = empty;
    n++;
  } while (n <= limit);

  {
    struct VramBlockCacheEntry *entry = gVramBlockCache;

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
s32 Resource_FindFreeEntry(void) {
  s32 ret = VRAM_CACHE_ENTRY_COUNT;
  s32 no;
  struct VramBlockCacheEntry *entry = gVramBlockCache;

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
s32 VramCache_AllocEntry(s32 size) {
  s32 no;

  no = Resource_FindFreeEntry();
  VramCache_Load(no, size, 0);
  return no;
}

/// Copies `src` again into entry `no`'s VRAM, keeping its size. Called by
/// code that redraws a sprite picture in place, such as the battle prompts.
/// Returns the first sprite tile number.
s32 VramCache_Reload(s32 no, s32 src) {
  return VramCache_Load(no, gVramBlockCache[no].size, (const void *)src);
}

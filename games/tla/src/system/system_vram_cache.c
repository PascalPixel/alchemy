/// @file
/// The sprite VRAM cache: blocks, entries and loading pictures, then the
/// affine matrix builder that follows it.
// Part of system.c.

#include "system_internal.h"
#include "fixed_math.h"

// Divide OBJ character memory (from 0x06010000) into 512 blocks of 64 bytes;
// gVramCacheBlockOwners records each block's owner (cache index).
// There are 96 cache entries; byte offset 0xffff marks a free entry.
// Entries with size <= VRAM_CACHE_SIZE_NO_BLOCKS do not own VRAM.

/// Finds the first run of free blocks that holds `size` bytes and gives it to
/// entry `id`, writing `id` into gVramCacheBlockOwners for each block.
s32 VramCache_AllocateBlocks(u32 slot, u32 size) {
  u32 blocks;
  s32 ret;
  s32 pos;
  u32 i;

  blocks = size / VRAM_BLOCK_BYTES;
  if (slot >= VRAM_CACHE_ENTRY_COUNT) {
    return -1;
  }
  pos = 0;
  for (;;) {
    if (pos >= VRAM_BLOCK_COUNT) {
      return -1;
    }
    if (gVramCacheBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
      if (pos + blocks > VRAM_BLOCK_COUNT) {
        return -1;
      }
      ret = pos;
      while (pos < ret + blocks) {
        if (gVramCacheBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
        pos++;
      }
      for (i = 0; i < blocks; i++) {
        gVramCacheBlockOwners[ret + i] = slot;
      }
      break;
    }
  occupied:
    pos += (u32)gVramCacheEntries[gVramCacheBlockOwners[pos]].size /
           VRAM_BLOCK_BYTES;
  }
  ret *= VRAM_BLOCK_BYTES;
  return ret;
}

/// Returns the length, in blocks, of the longest run of free blocks. Nothing
/// in the game calls it.
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

/// Counts the free blocks (owner 0xff) of the sprite VRAM cache's 512
/// blocks, as The Broken Seal's.
s32 VramCache_CountFreeBlocks(void) {
  u8 *marker = gVramCacheBlockOwners;
  s32 freeCount;
  s32 remaining;

  freeCount = 0;
  remaining = VRAM_BLOCK_COUNT;
  do {
    if (*marker++ == VRAM_BLOCK_OWNER_FREE) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
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

/// Frees a cache entry and the VRAM it holds; returns -1 for an index out of
/// range.
s32 VramCache_FreeEntry(u32 resourceIndex) {
  struct VramCacheEntry *entry = &gVramCacheEntries[resourceIndex];

  if (resourceIndex >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
    VramCache_FreeBlocks(resourceIndex);
    entry->offset |= VRAM_CACHE_OFFSET_FREE;
    entry->size = 0;
  }
  return 0;
}

/// Gives back entry `no`'s blocks but keeps the entry taken, with size 1, so a
/// later VramCache_Load can refill it.
s32 VramCache_ReleaseVram(u32 resourceIndex) {
  struct VramCacheEntry *entry = &gVramCacheEntries[resourceIndex];

  if (resourceIndex >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
    s32 value;

    VramCache_FreeBlocks(resourceIndex);
    value = 1;
    entry->size = value;
  }
  return 0;
}

/// Loads size bytes from src into the object VRAM of cache entry no, reusing
/// its blocks when the size is unchanged (src 0 loads nothing, -1 clears);
/// returns the tile number, or 0 when it does not fit.
s32 VramCache_Load(u32 no, u32 size, const void *src) {
  struct VramCacheEntry *entry;
  s32 offset;
  void *dst;

  entry = &gVramCacheEntries[no];
  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return 0;
  if (size > VRAM_CACHE_LOAD_MAX)
    return 0;
  if (entry->size > VRAM_CACHE_SIZE_NO_BLOCKS) {
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
    dst = (void *)(OBJ_VRAM0 + offset);
    entry->size = size;
    entry->offset = offset;
    if (src != 0) {
      if (src == (const void *)-1)
        Iwram_ClearWordsEntry(dst, size);
      else
        Dma_Set(src, dst, DMA_COPY_WORDS(size >> 2), REG_DMA3);
    }
    return (u32)offset >> 5;
  }
  return 0;
}

/// Frees every VRAM block and every cache entry.
void VramCache_Reset(void) {
  u32 count;
  u32 limit;
  u8 *marker = gVramCacheBlockOwners;

  count = 0;
  limit = VRAM_BLOCK_COUNT - 1;
  for (; count <= limit; count++)
    *marker++ = VRAM_BLOCK_OWNER_FREE;
  {
    struct VramCacheEntry *entry = gVramCacheEntries;

    count = 0;
    do {
      entry->offset |= VRAM_CACHE_OFFSET_FREE;
      entry->size = 0;
      entry++;
      count++;
    } while (count < VRAM_CACHE_ENTRY_COUNT);
  }
}

/// Returns the first free cache entry, or VRAM_CACHE_ENTRY_COUNT when all are
/// in use.
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
/// its number.
s32 VramCache_AllocEntry(s32 size) {
  s32 no;

  no = VramCache_FindFreeEntry();
  VramCache_Load(no, size, 0);
  return no;
}

/// Copies `src` again into entry `no`'s VRAM, keeping its size. Returns the
/// first sprite tile number.
s32 VramCache_Reload(s32 no, s32 src) {
  return VramCache_Load(no, gVramCacheEntries[no].size, (const void *)src);
}

/// Takes the next of the 32 sprite affine matrices and fills it with a
/// rotation by angle and the scales; returns its index, or 31 when all are
/// taken. The body is OamMatrix_AllocFromTransform's.
s32 AffineMatrix_Build(s32 angle, s32 xScale, s32 yScale) {
  union AffineMatrix *matrix;
  s16 *coefficient;
  u8 index;

  index = gOamMatrixCount;
  if (index >= OBJ_AFFINE_MATRIX_COUNT)
    return OBJ_AFFINE_MATRIX_COUNT - 1;

  matrix = &gOamMatrices[index];
  coefficient = matrix->coefficients;
  gOamMatrixCount = index + 1;
  if ((xScale == yScale || xScale == -yScale) && angle == 0) {
    s32 reciprocal;
    s32 xReciprocal;

    reciprocal = Iwram_SignedDivide(FIXED_ONE, yScale);
    xReciprocal = reciprocal;
    if (xScale == -yScale)
      xReciprocal = -reciprocal;

    matrix->rows[0] = (u16)xReciprocal;
    matrix->rows[1] = reciprocal << 16;
  } else {
    FIXED sine;
    FIXED cosine;

    sine = Trig_Sin(angle);
    cosine = Trig_Cos(angle);
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

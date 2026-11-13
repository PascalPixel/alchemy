/// @file
/// The sprite VRAM cache: blocks, entries and loading pictures, then the
/// affine matrix builder that follows it.
// Part of system.c.

#include "../system_internal.h"

// Divide OBJ character memory (from 0x06010000) into 512 blocks of 64 bytes;
// gResourceBlockOwners records each block's owner (cache index).
// There are 96 cache entries; byte offset 0xffff marks a free entry.
// Entries with size <= 16 do not own VRAM.

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
    if (gResourceBlockOwners[pos] == VRAM_BLOCK_OWNER_FREE) {
      if (pos + blocks > VRAM_BLOCK_COUNT) {
        return -1;
      }
      ret = pos;
      while (pos < ret + blocks) {
        if (gResourceBlockOwners[pos] != VRAM_BLOCK_OWNER_FREE) {
          goto occupied;
        }
        pos++;
      }
      for (i = 0; i < blocks; i++) {
        gResourceBlockOwners[ret + i] = slot;
      }
      break;
    }
  occupied:
    pos += (u32)gResourceTableEntries[gResourceBlockOwners[pos]].size /
           VRAM_BLOCK_BYTES;
  }
  ret *= VRAM_BLOCK_BYTES;
  return ret;
}

s32 VramCache_GetLongestFreeRun(void) {
  s32 run = 0;
  u8 *marker = gResourceBlockOwners;
  s32 longest = 0;
  s32 remaining = VRAM_BLOCK_COUNT;

  do {
    if (*marker++ != VRAM_BLOCK_OWNER_FREE) {
      run = 0;
    } else {
      run++;
      if (longest < run)
        longest = run;
    }
    remaining--;
  } while (remaining != 0);
  return longest;
}

/// Counts the free blocks (owner 0xff) of the sprite VRAM cache's 512
/// blocks, as The Broken Seal's.
s32 VramCache_CountFreeBlocks(void) {
  u8 *marker = gResourceBlockOwners;
  s32 freeCount;
  s32 remaining;

  freeCount = 0;
  remaining = 0x200;
  do {
    if (*marker++ == 0xff) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
}

s32 VramCache_FreeBlocks(s32 resourceId) {
  s32 clearedCount = 0;
  s32 remaining;
  u8 *marker;
  u8 emptyMarker;

  if ((u32)resourceId >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  marker = gResourceBlockOwners;
  emptyMarker = VRAM_BLOCK_OWNER_FREE;
  remaining = VRAM_BLOCK_COUNT;
  do {
    if (*marker == resourceId) {
      *marker = emptyMarker;
      clearedCount++;
    }
    remaining--;
    marker++;
  } while (remaining != 0);
  if (clearedCount != 0)
    return -1;
  return 0;
}

/// Frees a cache entry and the VRAM it holds; returns -1 for an index out of
/// range.
s32 VramCache_FreeEntry(u32 resourceIndex) {
  struct VramBlockCacheEntry *entry = &gResourceTableEntries[resourceIndex];

  if (resourceIndex >= VRAM_CACHE_ENTRY_COUNT)
    return -1;
  if (entry->offset != VRAM_CACHE_OFFSET_FREE) {
    VramCache_FreeBlocks(resourceIndex);
    entry->offset |= VRAM_CACHE_OFFSET_FREE;
    entry->size = 0;
  }
  return 0;
}

s32 VramCache_ReleaseVram(u32 resourceIndex) {
  u16 *entry = (u16 *)&gResourceTableEntries[resourceIndex];

  if (resourceIndex > 95)
    return -1;
  if (*entry > 16) {
    s32 value;

    VramCache_FreeBlocks(resourceIndex);
    value = 1;
    *entry = value;
  }
  return 0;
}

/// Loads size bytes from src into the object VRAM of cache entry no, reusing
/// its blocks when the size is unchanged (src 0 loads nothing, -1 clears);
/// returns the tile number, or 0 when it does not fit.
s32 VramCache_Load(u32 no, u32 size, const void *src) {
  struct VramBlockCacheEntry *entry;
  s32 offset;
  void *dst;

  entry = &gResourceTableEntries[no];
  if (no >= VRAM_CACHE_ENTRY_COUNT)
    return 0;
  if (size > 8192)
    return 0;
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
    dst = (void *)(0x06010000 + offset);
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
  u32 limit = 128;
  u8 *marker = gResourceBlockOwners;

  count = 0;
  limit = (limit << 1) + 255;
  for (; count <= limit; count++)
    *marker++ = VRAM_BLOCK_OWNER_FREE;
  {
    struct VramBlockCacheEntry *entry = gResourceTableEntries;

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
  struct VramBlockCacheEntry *entry = gResourceTableEntries;

  for (no = 0; no < VRAM_CACHE_ENTRY_COUNT; no++, entry++) {
    if (entry->offset == VRAM_CACHE_OFFSET_FREE) {
      ret = no;
      break;
    }
  }
  return ret;
}

s32 VramCache_AllocEntry(s32 size) {
  s32 slot;

  slot = VramCache_FindFreeEntry();
  VramCache_Load(slot, size, 0);
  return slot;
}

/// Copies src into cache no again, keeping its current size. Returns the OBJ
/// tile index.
s32 VramCache_Reload(s32 no, s32 src) {
  return VramCache_Load(no, gResourceTableEntries[no].size, (const void *)src);
}

/// Takes the next of the 32 sprite affine matrices and fills it with a
/// rotation by angle and the scales; returns its index, or 31 when all are
/// taken. The body is AffineMatrix_BuildForEffect's.
s32 AffineMatrix_Build(s32 angle, s32 x_scale, s32 y_scale) {
  union AffineMatrix *matrix;
  s16 *coefficient;
  u8 index;

  index = gObjAffineCount;
  if (index > 31)
    return 31;

  matrix = &gObjAffineMatrices[index];
  coefficient = matrix->coefficients;
  gObjAffineCount = index + 1;
  if ((x_scale == y_scale || x_scale == -y_scale) && angle == 0) {
    s32 reciprocal;
    s32 x_reciprocal;

    reciprocal = Iwram_SignedDivide(0x10000, y_scale);
    x_reciprocal = reciprocal;
    if (x_scale == -y_scale)
      x_reciprocal = -reciprocal;

    matrix->rows[0] = (u16)x_reciprocal;
    matrix->rows[1] = reciprocal << 16;
  } else {
    s32 sine;
    s32 cosine;

    sine = Trig_Sin(angle);
    cosine = Trig_Cos(angle);
    *coefficient = cosine / x_scale;
    coefficient++;
    *coefficient = sine / x_scale;
    coefficient++;
    *coefficient = (-sine) / y_scale;
    coefficient++;
    *coefficient = cosine / y_scale;
  }

  return index;
}

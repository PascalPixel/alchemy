/// @file
/// Resources: decompress a resource with a palette.

#include "resource.h"
#include "iwram_call.h"
#include "io_reg.h"

/// Decompress resource id into dst. If the first 128 bytes are a palette,
/// copy_pal copies it to the start of BG palettes; skip_pal skips it on
/// decoding.
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal) {
  u8 *res = Resource_GetTableEntry(id);

  if (copy_pal != 0) {
    /* FAKEMATCH: called through a local pointer to its fixed entry; as a plain
     * call through the header, all six ⚓️ editions differ. */
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)BG_PLTT, res, 128);
  }
  if (skip_pal != 0)
    res += 128;
  Resource_DecodeType01(res, dst);
}

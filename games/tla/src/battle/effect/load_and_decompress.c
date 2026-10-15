//--------------------------------------------------------------------------
// load_and_decompress.c
// リソース：パレットつきのリソースを展開する
//--------------------------------------------------------------------------

#include "resource.h"
#include "iwram_call.h"

//--------------------------------------------------------------------------
// リソース id を dst に展開する。先頭 128 バイトがパレットのとき、
// copy_pal なら BG パレットの先頭に写し、skip_pal ならとばして展開する
//--------------------------------------------------------------------------
void Resource_LoadAndDecompress(s32 id, void *dst, s32 skip_pal, s32 copy_pal) {
  u8 *res = Resource_GetTableEntry(id);

  if (copy_pal != 0) {
    /* FAKEMATCH: the routine is called through a local pointer to its fixed
     * entry; a plain call matches The Broken Seal through its IWRAM call layer
     * but not The Lost Age, whose header calls the entry directly (all six of
     * its editions differ). */
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy((void *)0x05000000, res, 128);
  }
  if (skip_pal != 0)
    res += 128;
  Resource_DecodeType01(res, dst);
}

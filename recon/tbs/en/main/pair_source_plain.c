/* 2026-10-01: alias-free plain-C attempt at RenderResource_LoadTableEntry.
   With the approved agscc flags, every case having the same table address
   folds the whole switch. The complete function is 24 bytes rather than
   68: it loses the dispatch branches and three literal-pool words.
   TILE.C retains the matching switch with tagged source assembly, using
   one table name and four local literal slots. */
#include "TYPES.H"

extern u8 RenderResource_PairSourceTable;
void VramBlock_LoadCached(void *, s32, void *);

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination)
{
    void *source;
    switch (value) {
    case 1:
        source = &RenderResource_PairSourceTable;
        break;
    case 2:
        source = &RenderResource_PairSourceTable;
        break;
    case 3:
        source = &RenderResource_PairSourceTable;
        break;
    case 0:
    default:
        source = &RenderResource_PairSourceTable;
        break;
    }
    VramBlock_LoadCached(destination, 32, source);
    return 1;
}

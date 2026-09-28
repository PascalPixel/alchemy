#include "TYPES.H"
#include "SCENE.H"

void VramBlock_LoadCached(void *, s32, void *);
/* FAKEMATCH: the reference keeps one literal-pool entry per case for the one
   pair source table, so each case names it through its own alias (MAIN.LD). */
extern u8 RenderResource_PairSourceTable;
extern u8 RenderResource_PairSourceTable2;
extern u8 RenderResource_PairSourceTable3;
extern u8 RenderResource_PairSourceTable4;

s32 RenderResource_LoadTableEntry(u32 value, s32 unused, void *destination)
{
    void *source;
    switch (value) {
    case 1:
        source = &RenderResource_PairSourceTable;
        break;
    case 2:
        source = &RenderResource_PairSourceTable2;
        break;
    case 3:
        source = &RenderResource_PairSourceTable3;
        break;
    case 0:
    default:
        source = &RenderResource_PairSourceTable4;
        break;
    }
    VramBlock_LoadCached(destination, 32, source);
    return 1;
}

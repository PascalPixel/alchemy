#include "types.h"

/*
 * resource_380 owner at 0x02004248, 24 bytes including the trailing
 * alignment halfword: a two-call convenience wrapper.  Both Func_ names are
 * the overlay's import-veneer entries for main-ROM code, not runtime
 * addresses.  The callees' roles beyond call shape remain open.
 */

extern void Func_02008bea(s32 arg0);
extern void Func_02008cb4(s32 arg0, s32 arg1);

void FieldScene_RunSplitPairSteps(s32 a, s32 b)
{
    Func_02008cb4(a, 0);
    Func_02008bea(b);
}

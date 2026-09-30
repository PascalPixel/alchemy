#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 IwramClearWords[];
extern u8 gObjAffineCount[];
extern u8 Data_03001400[];

/*
 * _call_via_r3 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */
s32 _call_via_r3(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
    *(s8 *)((u32)&gObjAffineCount) = 0;
    _call_via_r3(((u32)&Data_03001400), 0x400, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}

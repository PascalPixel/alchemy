#include "TYPES.H"

extern u8 IwramClearWords[];
extern u8 gObjAffineCount[];
extern u8 Data_03001400[];

s32 _call_via_r3(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
    *(s8 *)((u32)&gObjAffineCount) = 0;
    _call_via_r3(((u32)&Data_03001400), 0x400, ((u32)&gObjAffineCount), (u32)IwramClearWords);
}

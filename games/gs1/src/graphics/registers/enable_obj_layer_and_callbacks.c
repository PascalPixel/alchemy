#include "types.h"
#include "scene.h"

s32 WaitFrames(s32);

void Graphics_EnableObjLayerAndCallbacks(void)
{
    Sys_Check((u32)Sys_Run);
    Sys_Check((u32)Sys_unk2_4);
    Sys_Apply(0x10000, 1);
    Sys_unk2(1);
    WaitFrames(1);
    *(u16 *)0x04000000 = (0xF1FF & *(u16 *)0x04000000) | 0x1000;
}

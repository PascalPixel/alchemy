#include "resource_386_state.h"

#define Resource386_CheckWindow Func_02000030

void Resource386_CheckWindow(void) {
    s32 temp_r3;
    s32 temp_r5;

    temp_r5 = ((struct Resource386FirstView *)Resource386_GetFirstView(0))->sample_08;
    temp_r3 = (s32) ((struct Resource386SecondView *)Resource386_GetSecondView(0))->sample_10 >> 0x14;
    if (((u32) ((temp_r5 >> 0x14) - 0x22) <= 1U) && (temp_r3 > 0x28) && (temp_r3 <= 0x2A)) {
        Resource386_OnWindowMatch(0x250);
        return;
    }
    Resource386_OnWindowMiss(0x250);
}

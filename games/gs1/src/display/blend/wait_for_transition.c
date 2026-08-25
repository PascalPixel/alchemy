#include "types.h"
#include "global_cells.h"

s32 WaitFrames(s32);

#define Blend_WaitForTransition Func_08003ce0

void Blend_WaitForTransition(void) {
    if (*(u8 *)ADDR_03001C98 != 0) {
        do {
            WaitFrames(1);
        } while (*(u8 *)ADDR_03001C98 != 0);
    }
}

#include "types.h"
#include "global_cells.h"

s32 Func_080030f8(s32);

void Func_08003ce0(void) {
    if (*(u8 *)ADDR_03001C98 != 0) {
        do {
            Func_080030f8(1);
        } while (*(u8 *)ADDR_03001C98 != 0);
    }
}

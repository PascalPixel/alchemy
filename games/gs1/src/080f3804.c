#include "types.h"
#include "global_cells.h"

s32 Func_080f3078(s32, void *, void *, s32);

void Func_080f3804(s32 arg0, s32 arg1) {
    void *target = *(void **)ADDR_03001ED0;
    if (target != NULL) {
        Func_080f3078(arg0, target, (u8 *)target + 0x1000, arg1);
    }
}

#include "types.h"
#include "global_cells.h"

#define Shop_AdvanceWorkCursor Func_080b00f4

s32 Func_080b08b8(s32);

void Shop_AdvanceWorkCursor(void) {
    Func_080b08b8(*(s32 *)ADDR_03001F2C + 0x380);
}

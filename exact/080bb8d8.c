#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080bb8d8(void) {
    M2C_FIELD(*(void **)ADDR_03001EE4, s32 *, 8) = 1;
}

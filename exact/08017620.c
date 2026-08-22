#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08017620(s32 flags) {
    void *state;

    state = *(void **)ADDR_03001E8C;
    if (state != NULL) {
        if (flags & 1) {
            M2C_FIELD(state, s8 *, RENDER_BUSY_OFS + 1) = 1;
        }
        if (2 & flags) {
            M2C_FIELD(state, s8 *, RENDER_BUSY_OFS + 2) = 1;
        }
    }
}

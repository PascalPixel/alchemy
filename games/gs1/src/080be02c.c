#include "types.h"
#include "global_cells.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void WaitFrames(u32);
void Func_08004278(void *);
s32 Func_080bdfec(void);
void Func_080bd898(void);

s32 Func_080be02c(void) {
    s32 state;
    void *runtime;

    runtime = *(void **)ADDR_03001E74;
    state = M2C_FIELD(runtime, s32 *, 0x800);
    if (state == 0) {
        M2C_FIELD(runtime, s32 *, 0x800) = 1;
        state = 1;
    }
    if (state != 4) {
        do {
            WaitFrames(1U);
        } while (M2C_FIELD(runtime, s32 *, 0x800) != 4);
    }
    Func_08004278((void *)Func_080bd898);
    return Func_080bdfec();
}

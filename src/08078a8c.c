#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_08078414();
u8 *Func_08078b9c(u16);

u8 Func_08078a8c(void) {
    return *Func_08078b9c(M2C_FIELD(Func_08078414(), u16 *, 0x28));
}

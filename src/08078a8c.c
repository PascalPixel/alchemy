#include "types.h"
#include "item.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

u8 *Func_08078b9c(u16);

u8 Func_08078a8c(s32 item) {
    return *Func_08078b9c(
        M2C_FIELD(Func_08078414(item), u16 *, 0x28));
}

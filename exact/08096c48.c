#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08003f3c(u32 index);

void *Func_08096c48(void *arg0, void *arg1) {
    void *result;
    void *object;

    object = arg0;
    result = NULL;
    if (object != NULL) {
        if (arg1 == NULL) {
            M2C_FIELD(object, u8 *, 0x1D) = (u8) (M2C_FIELD(object, u8 *, 0x1D) | 1);
        } else {
            Func_08003f3c(M2C_FIELD(object, u8 *, 0x1C));
            M2C_FIELD(object, u8 *, 0x1C) = (u8) M2C_FIELD(arg1, u8 *, 0x1C);
            M2C_FIELD(object, u8 *, 0x1D) = (u8) (M2C_FIELD(object, u8 *, 0x1D) | 1);
            object = arg1;
        }
        result = object;
    }
    return result;
}

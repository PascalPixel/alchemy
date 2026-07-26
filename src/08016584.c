#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_08016584(void *arg0, s8 *arg1) {
    if (arg0 != NULL) {
        *(volatile s32 *)M2C_FIELD(arg0, s32 **, 4) = (s32) arg1;
        *(s32 * volatile *)((u8 *)arg0 + 4) = (s32 *)arg1;
    }
}

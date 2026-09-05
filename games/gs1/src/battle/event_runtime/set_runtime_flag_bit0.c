#include "types.h"

#define Battle_SetRuntimeFlagBit0 Func_080bb928

struct Runtime_080bb928 {
    u8 padding_000[0x16c];
    s32 flags;
};

void Battle_SetRuntimeFlagBit0(struct Runtime_080bb928 *runtime) {
    runtime->flags |= 1;
}

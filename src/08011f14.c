#include "types.h"

extern u8 Data_080133fc[];

s32 Func_08011f14(s8 *arg0, u32 arg1, u32 arg2) {
    return arg0[Data_080133fc[arg1 + (arg2 << 4)]] << 19;
}

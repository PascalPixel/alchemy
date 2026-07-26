#include "types.h"

extern s32 Data_08088db8[];

s32 Func_080797ec(s32 arg0, s32 arg1) {
    return Data_08088db8[(arg0 * 4) + arg1];
}

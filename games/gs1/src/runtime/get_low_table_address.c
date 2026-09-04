#include "types.h"

#define Runtime_GetLowTableAddress Func_080045e8

extern const u8 Data_0800779c[];

s32 Runtime_GetLowTableAddress(void) {
    return (s32)Data_0800779c;
}

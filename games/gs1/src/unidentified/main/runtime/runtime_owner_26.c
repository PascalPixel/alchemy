#include "types.h"

extern u32 Data_03001ae8;
extern u16 Data_0809f0f8[];

u16 Func_08097b54(void) {
    return Data_0809f0f8[(Data_03001ae8 >> 4) & 15];
}

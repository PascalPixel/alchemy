#include "types.h"

extern u8 Data_08185024[];

u8 *Func_08185008(u32 arg0) {
    return &Data_08185024[(arg0 & 0xfff) * 20];
}

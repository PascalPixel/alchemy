#include "types.h"

s32 Resource_CopyData(s32, s32, s32);
extern const u8 Data_080317e4[];

void Resource_CopyFixedBlockA(s32 arg0) {
    Resource_CopyData(arg0, 0x80, (s32)Data_080317e4);
}

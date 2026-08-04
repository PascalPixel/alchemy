#include "types.h"

extern s32 Data_0200d144;
extern u8 Data_02009f15[];

extern void Func_02005b14(s32, s32, s32);
extern void Func_02005b1e(s32, s32, s32);
extern s32 Func_02005a18(s32, s32);

void Func_02001ed8(void) {
    s32 z = 0;
    Data_0200d144 = z;
    Func_02005b14(20, 0x19999, 0xcccc);
    Func_02005b1e(21, 0x19999, 0xcccc);
    Func_02005a18((s32)Data_02009f15, 3200);
}

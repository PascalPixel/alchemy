#include "types.h"
#define NULL ((void *)0)

extern s16 Data_02000240[];
extern u8 Data_0200b380[];
extern u8 Data_0200b560[];
extern u8 Data_0200b7d0[];
extern u8 Data_0200b170[];
s32 Func_02002732(s32);
s32 Func_02002754(s32);

void *Func_020000b8(void) {
    if (Func_02002732(0x834) != 0) {
        return Data_0200b380;
    }
    if (Data_02000240[225] == 12) {
        return Data_0200b560;
    }
    if (Func_02002754(0x87a) != 0) {
        return Data_0200b7d0;
    }
    return Data_0200b170;
}

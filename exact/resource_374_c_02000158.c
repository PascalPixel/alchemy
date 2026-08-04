#include "types.h"
#define NULL ((void *)0)

extern s16 Data_02000240[];
extern u8 Data_0200bcec[];
extern u8 Data_0200bb3c[];
extern u8 Data_0200bb30[];
extern u8 Data_0200ba64[];
extern u8 Data_0200b938[];
s32 Func_020027d2(s32);
s32 Func_020027e0(s32);
s32 Func_02002802(s32);

void *Func_02000158(void) {
    if (Func_020027d2(0x87a) != 0) {
        return Data_0200bcec;
    }
    if (Func_020027e0(0x815) != 0) {
        return Data_0200bb3c;
    }
    if (Data_02000240[225] == 12) {
        return Data_0200bb30;
    }
    if (Func_02002802(0x834) != 0) {
        return Data_0200ba64;
    }
    return Data_0200b938;
}

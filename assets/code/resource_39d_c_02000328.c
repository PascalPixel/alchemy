#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_0200b9d4[];

void Func_020039f6(s32);

void *Func_02000328(void) {
    if (Data_02000240[225] != 1) {
        Func_020039f6(0x253);
    }
    return Data_0200b9d4;
}

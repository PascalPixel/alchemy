#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_02008d4c[];
extern u8 Data_02008a28[];

s32 Func_02000570(void) {
    if (Data_02000240[225] == 8) {
        return (s32)Data_02008d4c;
    }
    return (s32)Data_02008a28;
}

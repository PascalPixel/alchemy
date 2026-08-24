#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 Data_0200bc0c[];
extern u8 Data_0200bef4[];

s32 Func_02002370(void) {
    if (Data_02000240[224] == (s32)&Value_000000ac) {
        return (s32)Data_0200bc0c;
    }
    return (s32)Data_0200bef4;
}

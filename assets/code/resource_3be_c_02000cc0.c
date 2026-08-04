#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000098;
extern u8 Value_0000009d;
extern u8 Value_0000009e;
extern u8 Data_020097b4[];
extern u8 Data_020097fc[];
extern u8 Data_02009874[];
extern u8 Data_02009784[];

s32 Func_02000cc0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000098) {
        return (s32)Data_020097b4;
    }
    if (v == (s32)&Value_0000009d) {
        return (s32)Data_020097fc;
    }
    if (v == (s32)&Value_0000009e) {
        return (s32)Data_02009874;
    }
    return (s32)Data_02009784;
}

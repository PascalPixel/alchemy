#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000075;
extern u8 Value_00000076;
extern u8 Value_00000078;
extern u8 Data_0200a898[];
extern u8 Data_0200a8e0[];
extern u8 Data_0200a928[];
extern u8 Data_0200a868[];

s32 Func_020009f0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000075) {
        return (s32)Data_0200a898;
    }
    if (v == (s32)&Value_00000076) {
        return (s32)Data_0200a8e0;
    }
    if (v == (s32)&Value_00000078) {
        return (s32)Data_0200a928;
    }
    return (s32)Data_0200a868;
}

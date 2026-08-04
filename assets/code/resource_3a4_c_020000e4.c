#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000004d;
extern u8 Value_0000004e;
extern u8 Value_0000004f;
extern u8 Value_00000050;
extern u8 Value_00000051;
extern u8 Value_00000052;
extern u8 Value_00000053;
extern u8 Value_00000054;
extern u8 Value_00000055;
extern u8 Value_00000056;
extern u8 Value_00000057;
extern u8 Data_0200c194[];
extern u8 Data_0200c20c[];
extern u8 Data_0200c26c[];
extern u8 Data_0200c314[];
extern u8 Data_0200c3ec[];
extern u8 Data_0200c464[];
extern u8 Data_0200c524[];
extern u8 Data_0200c59c[];
extern u8 Data_0200c644[];
extern u8 Data_0200c704[];
extern u8 Data_0200c77c[];
extern u8 Data_0200c164[];

s32 Func_020000e4(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c194;
    }
    if (v == (s32)&Value_0000004e) {
        return (s32)Data_0200c20c;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c26c;
    }
    if (v == (s32)&Value_00000050) {
        return (s32)Data_0200c314;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200c3ec;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200c464;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200c524;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200c59c;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200c644;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200c704;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200c77c;
    }
    return (s32)Data_0200c164;
}

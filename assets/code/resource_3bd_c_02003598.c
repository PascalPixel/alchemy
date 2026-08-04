#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000093;
extern u8 Value_00000094;
extern u8 Value_00000095;
extern u8 Value_00000096;
extern u8 Value_00000097;
extern u8 Data_0200c688[];
extern u8 Data_0200c724[];
extern u8 Data_0200c76c[];
extern u8 Data_0200c808[];
extern u8 Data_0200c850[];
extern u8 Data_0200c5e0[];

s32 Func_02003598(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000093) {
        return (s32)Data_0200c688;
    }
    if (v == (s32)&Value_00000094) {
        return (s32)Data_0200c724;
    }
    if (v == (s32)&Value_00000095) {
        return (s32)Data_0200c76c;
    }
    if (v == (s32)&Value_00000096) {
        return (s32)Data_0200c808;
    }
    if (v == (s32)&Value_00000097) {
        return (s32)Data_0200c850;
    }
    return (s32)Data_0200c5e0;
}

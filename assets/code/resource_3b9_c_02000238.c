#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000008d;
extern u8 Value_0000008c;
extern u8 Value_0000008e;
extern u8 Data_0200be70[];
extern u8 Data_0200c110[];
extern u8 Data_0200be94[];
extern u8 Data_0200bf60[];
extern u8 Data_0200be64[];

s32 Func_02000238(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000008d) {
        return (s32)Data_0200be70;
    }
    if (v == (s32)&Value_0000008c) {
        if (Data_02000240[225] == 12) {
            return (s32)Data_0200c110;
        }
        return (s32)Data_0200be94;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200bf60;
    }
    return (s32)Data_0200be64;
}

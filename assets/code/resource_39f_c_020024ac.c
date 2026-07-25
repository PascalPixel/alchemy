typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000044;
extern u8 Value_00000045;
extern u8 Value_00000046;
extern u8 Data_0200ba48[];
extern u8 Data_0200bb20[];
extern u8 Data_0200bc1c[];
extern u8 Data_0200bd54[];

s32 Func_020024ac(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000044) {
        return (s32)Data_0200ba48;
    }
    if (v == (s32)&Value_00000045) {
        return (s32)Data_0200bb20;
    }
    if (v == (s32)&Value_00000046) {
        return (s32)Data_0200bc1c;
    }
    return (s32)Data_0200bd54;
}

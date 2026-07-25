typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000059;
extern u8 Value_0000005a;
extern u8 Value_0000005b;
extern u8 Value_0000005c;
extern u8 Data_0200a174[];
extern u8 Data_0200a1d4[];
extern u8 Data_0200a234[];
extern u8 Data_0200a2dc[];
extern u8 Data_0200a12c[];

s32 Func_020003ac(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000059) {
        return (s32)Data_0200a174;
    }
    if (v == (s32)&Value_0000005a) {
        return (s32)Data_0200a1d4;
    }
    if (v == (s32)&Value_0000005b) {
        return (s32)Data_0200a234;
    }
    if (v == (s32)&Value_0000005c) {
        return (s32)Data_0200a2dc;
    }
    return (s32)Data_0200a12c;
}

typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Value_00000039;
extern u8 Data_0200e700[];
extern u8 Data_0200e7a8[];
extern u8 Data_0200e838[];
extern u8 Data_0200e988[];
extern u8 Data_0200e6e8[];

s32 Func_02000e80(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000036) {
        return (s32)Data_0200e700;
    }
    if (v == (s32)&Value_00000037) {
        return (s32)Data_0200e7a8;
    }
    if (v == (s32)&Value_00000038) {
        return (s32)Data_0200e838;
    }
    if (v == (s32)&Value_00000039) {
        return (s32)Data_0200e988;
    }
    return (s32)Data_0200e6e8;
}

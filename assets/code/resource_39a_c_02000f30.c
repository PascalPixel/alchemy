typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000034;
extern u8 Value_0000003e;
extern u8 Value_0000003f;
extern u8 Value_00000040;
extern u8 Value_00000041;
extern u8 Value_00000042;
extern u8 Value_00000043;
extern u8 Data_0200abd8[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ad1c[];
extern u8 Data_0200ae24[];
extern u8 Data_0200b058[];
extern u8 Data_0200b130[];
extern u8 Data_0200b184[];
extern u8 Data_0200abcc[];

s32 Func_02000f30(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000034) {
        return (s32)Data_0200abd8;
    }
    if (v == (s32)&Value_0000003e) {
        return (s32)Data_0200ac08;
    }
    if (v == (s32)&Value_0000003f) {
        return (s32)Data_0200ad1c;
    }
    if (v == (s32)&Value_00000040) {
        return (s32)Data_0200ae24;
    }
    if (v == (s32)&Value_00000041) {
        return (s32)Data_0200b058;
    }
    if (v == (s32)&Value_00000042) {
        return (s32)Data_0200b130;
    }
    if (v == (s32)&Value_00000043) {
        return (s32)Data_0200b184;
    }
    return (s32)Data_0200abcc;
}

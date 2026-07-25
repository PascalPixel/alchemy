typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000031;
extern u8 Value_00000030;
extern u8 Value_0000002f;
extern u8 Data_020089ec[];
extern u8 Data_02008a64[];
extern u8 Data_02008b24[];
extern u8 Data_020089bc[];

s32 Func_02000040(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000031) {
        return (s32)Data_020089ec;
    }
    if (v == (s32)&Value_00000030) {
        return (s32)Data_02008a64;
    }
    if (v == (s32)&Value_0000002f) {
        return (s32)Data_02008b24;
    }
    return (s32)Data_020089bc;
}

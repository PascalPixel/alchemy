typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_0000007b;
extern u8 Value_0000007c;
extern u8 Value_0000007d;
extern u8 Data_0200b310[];
extern u8 Data_0200b358[];
extern u8 Data_0200b3a0[];
extern u8 Data_0200b400[];
extern u8 Data_0200b448[];
extern u8 Data_0200b478[];

s32 Func_02000cc4(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b310;
    }
    if (v == (s32)&Value_00000072) {
        return (s32)Data_0200b358;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b3a0;
    }
    if (v == (s32)&Value_0000007c) {
        return (s32)Data_0200b400;
    }
    if (v == (s32)&Value_0000007d) {
        return (s32)Data_0200b448;
    }
    return (s32)Data_0200b478;
}

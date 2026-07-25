typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_0000007b;
extern u8 Value_0000007c;
extern u8 Value_0000007d;
extern u8 Data_0200b904[];
extern u8 Data_0200b8e0[];
extern u8 Data_0200b9f4[];
extern u8 Data_0200bd48[];
extern u8 Data_0200bd6c[];
extern u8 Data_0200b880[];

s32 Func_02000ec4(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b904;
    }
    if (v == (s32)&Value_00000072) {
        return (s32)Data_0200b8e0;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b9f4;
    }
    if (v == (s32)&Value_0000007c) {
        return (s32)Data_0200bd48;
    }
    if (v == (s32)&Value_0000007d) {
        return (s32)Data_0200bd6c;
    }
    return (s32)Data_0200b880;
}

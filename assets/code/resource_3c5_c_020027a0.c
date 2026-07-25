typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000b0;
extern u8 Value_000000af;
extern u8 Value_000000ae;
extern u8 Data_0200b91c[];
extern u8 Data_0200b9e8[];
extern u8 Data_0200bac0[];
extern u8 Data_0200bc28[];

s32 Func_020027a0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000b0) {
        return (s32)Data_0200b91c;
    }
    if (v == (s32)&Value_000000af) {
        return (s32)Data_0200b9e8;
    }
    if (v == (s32)&Value_000000ae) {
        return (s32)Data_0200bac0;
    }
    return (s32)Data_0200bc28;
}

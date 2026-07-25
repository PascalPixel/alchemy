typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000006a;
extern u8 Value_000000a2;
extern u8 Value_000000a1;
extern u8 Value_000000a0;
extern u8 Value_000000a3;
extern u8 Data_0200e9d0[];
extern u8 Data_0200ee08[];
extern u8 Data_0200ec28[];
extern u8 Data_0200eac0[];
extern u8 Data_0200ee98[];
extern u8 Data_0200e9b8[];

s32 Func_02000a80(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000006a) {
        return (s32)Data_0200e9d0;
    }
    if (v == (s32)&Value_000000a2) {
        return (s32)Data_0200ee08;
    }
    if (v == (s32)&Value_000000a1) {
        return (s32)Data_0200ec28;
    }
    if (v == (s32)&Value_000000a0) {
        return (s32)Data_0200eac0;
    }
    if (v == (s32)&Value_000000a3) {
        return (s32)Data_0200ee98;
    }
    return (s32)Data_0200e9b8;
}

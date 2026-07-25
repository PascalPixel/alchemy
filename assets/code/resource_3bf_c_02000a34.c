typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Value_000000a3;
extern u8 Data_0200e910[];
extern u8 Data_0200e97c[];
extern u8 Data_0200e8a4[];

s32 Func_02000a34(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000a1) {
        return (s32)Data_0200e910;
    }
    if (v == (s32)&Value_000000a2 || v == (s32)&Value_000000a3) {
        return (s32)Data_0200e97c;
    }
    return (s32)Data_0200e8a4;
}

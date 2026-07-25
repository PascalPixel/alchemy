typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000b5;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 Data_0200dd68[];
extern u8 Data_0200e020[];
extern u8 Data_0200e230[];
extern u8 Data_0200e350[];
extern u8 Data_0200e548[];
extern u8 Data_0200ddc8[];

s32 Func_02000e04(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000b5) {
        return (s32)Data_0200dd68;
    }
    if (v == (s32)&Value_000000b7) {
        return (s32)Data_0200e020;
    }
    if (v == (s32)&Value_000000b8) {
        return (s32)Data_0200e230;
    }
    if (v == (s32)&Value_000000b9) {
        return (s32)Data_0200e350;
    }
    if (v == (s32)&Value_000000ba) {
        return (s32)Data_0200e548;
    }
    return (s32)Data_0200ddc8;
}

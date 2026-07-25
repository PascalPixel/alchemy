typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Data_0200b474[];
extern u8 Data_0200b654[];
extern u8 Data_0200b42c[];

s32 Func_02001270(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000ac) {
        return (s32)Data_0200b474;
    }
    if (v == (s32)&Value_000000ad) {
        return (s32)Data_0200b654;
    }
    return (s32)Data_0200b42c;
}

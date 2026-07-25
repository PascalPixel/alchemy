typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000006f;
extern u8 Data_0200e984[];
extern u8 Data_0200e96c[];

s32 Func_02000340(void) {
    if (Data_02000240[224] == (s32)&Value_0000006f) {
        return (s32)Data_0200e984;
    }
    return (s32)Data_0200e96c;
}

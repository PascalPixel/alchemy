typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;
extern u8 Data_02009f9c[];
extern u8 Data_0200a014[];
extern u8 Data_0200a134[];
extern u8 Data_02009f6c[];

s32 Func_02000d20(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000005d) {
        return (s32)Data_02009f9c;
    }
    if (v == (s32)&Value_0000005e) {
        return (s32)Data_0200a014;
    }
    if (v == (s32)&Value_0000005f) {
        return (s32)Data_0200a134;
    }
    return (s32)Data_02009f6c;
}

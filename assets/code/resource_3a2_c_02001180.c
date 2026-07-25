typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000004a;
extern u8 Data_02009c9c[];
extern u8 Data_02009b10[];

s32 Func_02001180(void) {
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009c9c;
    }
    return (s32)Data_02009b10;
}

typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000067;
extern u8 Data_02009f38[];
extern u8 Data_02009f2c[];

s32 Func_020000a4(void) {
    if (Data_02000240[224] == (s32)&Value_00000067) {
        return (s32)Data_02009f38;
    }
    return (s32)Data_02009f2c;
}

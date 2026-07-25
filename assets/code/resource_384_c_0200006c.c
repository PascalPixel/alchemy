typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Data_020084a0[];
extern u8 Data_020083ec[];

s32 Func_0200006c(void) {
    if (Data_02000240[225] == 10) {
        return (s32)Data_020084a0;
    }
    return (s32)Data_020083ec;
}

typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Data_02009c04[];
extern u8 Data_020099f4[];

s32 Func_02000060(void) {
    s16 v = Data_02000240[225];

    if (v == 11 || v == 9) {
        return (s32)Data_02009c04;
    }
    return (s32)Data_020099f4;
}

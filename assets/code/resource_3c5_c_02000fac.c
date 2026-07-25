typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000b0;
extern u8 Data_0200b5b8[];

s32 Func_02000fac(void) {
    if (Data_02000240[224] == (s32)&Value_000000b0) {
        return (s32)Data_0200b5b8;
    }
    return 0;
}

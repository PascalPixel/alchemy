typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000ad;
extern u8 Data_0200b81c[];

s32 Func_020012b0(void) {
    if (Data_02000240[224] == (s32)&Value_000000ad) {
        return (s32)Data_0200b81c;
    }
    return 0;
}

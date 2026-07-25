typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_000000b3;
extern u8 Data_02009690[];
extern u8 Data_020096b0[];

s32 Func_0200004c(void) {
    if (Data_02000240[224] == (s32)&Value_000000b3) {
        return (s32)Data_02009690;
    }
    return (s32)Data_020096b0;
}

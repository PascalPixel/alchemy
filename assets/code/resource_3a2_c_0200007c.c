typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000004a;
extern u8 Data_02009844[];
extern u8 Data_020097b4[];

s32 Func_0200007c(void) {
    if (Data_02000240[224] == (s32)&Value_0000004a) {
        return (s32)Data_02009844;
    }
    return (s32)Data_020097b4;
}

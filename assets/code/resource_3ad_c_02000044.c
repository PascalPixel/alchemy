typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000006a;
extern u8 Data_02009cd8[];
extern u8 Data_02009cc0[];

s32 Func_02000044(void) {
    if (Data_02000240[224] == (s32)&Value_0000006a) {
        return (s32)Data_02009cd8;
    }
    return (s32)Data_02009cc0;
}

typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000075;
extern u8 Value_00000076;
extern u8 Value_00000078;
extern u8 Data_0200abb4[];
extern u8 Data_0200acb0[];
extern u8 Data_0200adac[];
extern u8 Data_0200aba8[];

s32 Func_02000ee0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000075) {
        return (s32)Data_0200abb4;
    }
    if (v == (s32)&Value_00000076) {
        return (s32)Data_0200acb0;
    }
    if (v == (s32)&Value_00000078) {
        return (s32)Data_0200adac;
    }
    return (s32)Data_0200aba8;
}

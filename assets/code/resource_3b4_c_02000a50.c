typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000075;
extern u8 Value_00000076;
extern u8 Value_00000078;
extern u8 Data_0200a9b0[];
extern u8 Data_0200aa40[];
extern u8 Data_0200aad0[];
extern u8 Data_0200a998[];

s32 Func_02000a50(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000075) {
        return (s32)Data_0200a9b0;
    }
    if (v == (s32)&Value_00000076) {
        return (s32)Data_0200aa40;
    }
    if (v == (s32)&Value_00000078) {
        return (s32)Data_0200aad0;
    }
    return (s32)Data_0200a998;
}

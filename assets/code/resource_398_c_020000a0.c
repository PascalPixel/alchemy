typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000031;
extern u8 Value_00000030;
extern u8 Value_0000002f;
extern u8 Data_02008c2c[];
extern u8 Data_02008c5c[];
extern u8 Data_02008cbc[];
extern u8 Data_02008c14[];

s32 Func_020000a0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000031) {
        return (s32)Data_02008c2c;
    }
    if (v == (s32)&Value_00000030) {
        return (s32)Data_02008c5c;
    }
    if (v == (s32)&Value_0000002f) {
        return (s32)Data_02008cbc;
    }
    return (s32)Data_02008c14;
}

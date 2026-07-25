typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000060;
extern u8 Value_00000061;
extern u8 Value_00000062;
extern u8 Data_02009f98[];
extern u8 Data_0200a064[];
extern u8 Data_0200a190[];
extern u8 Data_02009f8c[];

s32 Func_020002e0(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000060) {
        return (s32)Data_02009f98;
    }
    if (v == (s32)&Value_00000061) {
        return (s32)Data_0200a064;
    }
    if (v == (s32)&Value_00000062) {
        return (s32)Data_0200a190;
    }
    return (s32)Data_02009f8c;
}

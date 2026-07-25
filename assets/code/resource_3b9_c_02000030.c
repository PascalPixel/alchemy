typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000008c;
extern u8 Value_0000008e;
extern u8 Data_0200b094[];
extern u8 Data_0200b274[];
extern u8 Data_0200b034[];

s32 Func_02000030(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000008c) {
        return (s32)Data_0200b094;
    }
    if (v == (s32)&Value_0000008e) {
        return (s32)Data_0200b274;
    }
    return (s32)Data_0200b034;
}

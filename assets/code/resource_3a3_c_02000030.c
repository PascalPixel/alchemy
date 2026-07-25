typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000004b;
extern u8 Value_0000004c;
extern u8 Data_02009120[];
extern u8 Data_02009288[];
extern u8 Data_020090f0[];

s32 Func_02000030(void) {
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_0000004b) {
        return (s32)Data_02009120;
    }
    if (v == (s32)&Value_0000004c) {
        return (s32)Data_02009288;
    }
    return (s32)Data_020090f0;
}

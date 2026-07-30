typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200dad8[];
extern u8 Data_0200da48[];
extern u8 Data_0200d9e8[];
extern u8 Data_0200d688[];
extern u8 Data_0200d394[];
extern u8 Data_0200d004[];

extern s32 Func_020083ee(s32);
extern s32 Func_020083fc(s32);
extern s32 Func_02008410(s32);
extern s32 Func_0200841e(s32);

s32 Func_02004034(void) {
    if (Data_02000240[224] == (s32)&Value_0000008b) {
        if (Func_020083ee(0x950) != 0) {
            return (s32)Data_0200dad8;
        }
        if (Func_020083fc(0x962) != 0) {
            return (s32)Data_0200da48;
        }
        return (s32)Data_0200d9e8;
    }
    if (Func_02008410(0x950) != 0) {
        return (s32)Data_0200d688;
    }
    if (Func_0200841e(0x962) != 0) {
        return (s32)Data_0200d394;
    }
    return (s32)Data_0200d004;
}

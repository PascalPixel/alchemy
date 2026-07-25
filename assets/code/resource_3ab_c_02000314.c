typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

extern s16 Data_02000240[];
extern u8 Value_00000068;
extern u8 Value_0000009f;
extern u8 Data_02009d3c[];
extern u8 Data_02009bec[];

s32 Func_02000314(void) {
    s16 v = Data_02000240[224];

    if (v != (s32)&Value_00000068 && v == (s32)&Value_0000009f) {
        return (s32)Data_02009d3c;
    }
    return (s32)Data_02009bec;
}

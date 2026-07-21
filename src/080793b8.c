typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern u8 Data_02000040[];

u8 Func_080793b8(s32 arg0) {
    s32 temp_r3;

    temp_r3 = arg0 << 20;
    arg0 = (u32)temp_r3 >> 23;
    return Data_02000040[arg0];
}

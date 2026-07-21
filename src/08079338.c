typedef signed int s32;
typedef unsigned int u32;
typedef unsigned char u8;
s32 Func_08079338(s32 arg0) {
    s32 mask;
    u8 *base;
    s32 temp_r3;

    mask = 1 << (7 & arg0);
    arg0 = (u32) (arg0 << 0x14) >> 0x17;
    base = (u8 *) 0x02000040;
    temp_r3 = base[arg0] & mask;
    return (s32) ((u32) ((0 - temp_r3) | temp_r3) >> 0x1F);
}

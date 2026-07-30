typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

void Func_020047f4(void);
void Func_020048ba(s32);
s32 Func_020047e2(s32);
s32 Func_020048e6(s32, s32);
s32 Func_0200483e(s32, s32);
void Func_0200481a(s32);
void Func_0200491c(s32, s32);
void Func_02004858(void);

void Func_02000414(void)
{
    Func_020047f4();
    Func_020048ba(0x178A);
    if (Func_020047e2(0x890) != 0) {
        u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
        q[0] = q[0] + 4;
    }
    Func_020048e6(8, 0);
    if (Func_0200483e(0, 0) == 0) {
        Func_0200481a(0x890);
    } else {
        u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
        q[0] = q[0] + 1;
    }
    Func_0200491c(8, 0);
    Func_02004858();
}

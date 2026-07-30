typedef signed int s32;
typedef unsigned short u16;
typedef unsigned char u8;

extern u8 *Data_03001ebc;

void Func_02004864(void);
s32 Func_0200484a(s32);
void Func_02004934(s32);
s32 Func_0200495c(s32, s32);
void Func_02004888(void);
void Func_02004948(s32);
s32 Func_02004958(s32, s32);
s32 Func_020048b0(s32, s32);
s32 Func_0200497c(s32, s32);
s32 Func_020048d4(s32, s32);
void Func_020049a6(s32, s32);
void Func_020048e2(void);

void Func_02000484(void)
{
    Func_02004864();
    if (Func_0200484a(0x88F) != 0) {
        Func_02004934(0x17D6);
        Func_0200495c(12, 0);
        Func_02004888();
    } else {
        Func_02004948(0x1794);
        Func_02004958(12, 0);
        if (Func_020048b0(0, 0) == 1) {
            u16 *q = (u16 *)(Data_03001ebc + 0x1D8);
            q[0] = q[0] + 1;
            Func_0200497c(12, 0);
            if (Func_020048d4(0, 0) == 1) {
                u16 *r = (u16 *)(Data_03001ebc + 0x1D8);
                r[0] = r[0] + 1;
            }
        }
        Func_020049a6(12, 0);
        Func_020048e2();
    }
}

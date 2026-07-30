typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_0200b99e(s32);
extern s32 Func_0200b9b4(s32);
extern s32 Func_0200b9ce(s32);
extern s32 Func_0200b9c4(void);
extern s32 Func_0200b9ca(void);

s32 Func_02005ae0(s32 *p)
{
    s16 *q = (s16 *)p[20];
    s32 a, b;
    s32 d = Func_0200b99e(p[12]) * 2;
    if (d > 0)
        d = -d;
    p[2] = p[14] + Func_0200b9b4(p[12]) * 2;
    p[3] = p[15] + d;
    q[15] = Func_0200b9ce(p[12] + 0x8000) / 8;
    a = Func_0200b9c4();
    b = Func_0200b9ca();
    p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
    return 0;
}

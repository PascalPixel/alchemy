typedef signed int s32;
typedef unsigned char u8;

extern u8 *Func_02004bfa(s32);
extern void Func_02004b68(s32, s32, s32 *);

void Func_02000f10(s32 a, s32 b, s32 c)
{
    s32 k1 = 0x1f80000;
    s32 k2 = 0x180000;
    s32 k3 = 0x900000;
    u8 *obj = Func_02004bfa(a);
    s32 buf[3];
    s32 *bp = buf;

    bp[0] = k1;
    bp[2] = k2;
    Func_02004b68(b, c, bp);
    *(s32 *)(obj + 8) = bp[0];
    *(s32 *)(obj + 12) = bp[2];
    *(s32 *)(obj + 16) = k3;
}

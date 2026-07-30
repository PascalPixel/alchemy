typedef signed int s32;
typedef unsigned int u32;

extern s32 Func_02008950(s32, s32);
extern void Func_020089f0(s32, s32);
extern s32 Func_02008966(s32, s32);
extern void Func_02008758(s32);

void Func_02004304(s32 o)
{
    volatile s32 *p = (volatile s32 *)0x03001e40;
    s32 t;
    s32 v;

    if ((*p & 1) != 0) {
        t = (s32)((u32)*p >> 1);
        Func_020089f0(o, Func_02008950(t, 6));
    }
    v = (*(s32 *)0x0200d7fc << 3) + 16;
    if (Func_02008966(*p, v) == 0) {
        Func_02008758(o);
    }
}

typedef signed int s32;
typedef signed char s8;
typedef unsigned char u8;

extern u8 *Func_02004a64(s32);
extern void Func_02004a06(s32, s32, s32, s32, s32, s32);
extern void Func_02004a66(s32);

void Func_02000d90(void)
{
    u8 *obj = *(u8 **)0x03001f30;
    u8 *p = Func_02004a64(11);
    s32 t;

    obj += 0x35;
    t = *obj;
    t = (s8)t;
    if (t == 0) {
        s32 a = 0x49;
        s32 b = 0x11;
        Func_02004a06(0x4c, 0x10, 1, 1, a, b);
        if (p != 0) {
            s32 c = 2;
            p[0x55] = c;
            p[0x23] = t;
        }
        Func_02004a66(0x211);
    }
}

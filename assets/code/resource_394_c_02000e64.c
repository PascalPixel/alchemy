typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;

extern void Func_02001d9e(void);
extern s32 Func_02001d64(s32, s32);
extern void Func_02001e22(void);
extern void Func_02001e06(void);
extern void Func_02001fc0(s32, s32);

void Func_02000e64(s32 a)
{
    u32 x;

    Func_02001d9e();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = Func_02001d64(*pal, a);
        }
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    Func_02001e22();
    Func_02001e06();
    Func_02001fc0(0x10000, 0);
}

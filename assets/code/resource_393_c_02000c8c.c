typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;

extern void Func_020019ee(void);
extern s32 Func_020019b4(s32, s32);
extern void Func_02001a72(void);
extern void Func_02001a56(void);
extern void Func_02001b98(s32, s32);

void Func_02000c8c(s32 a)
{
    u32 x;

    Func_020019ee();
    x = 0;
    do {
        u32 idx = x >> 16;
        if (x + 0xffef0000 > 0x60000 && (idx + 0xff3f) << 16 > 0x70000) {
            u16 *pal = (u16 *)(0x5000000 + idx * 2);
            *pal = Func_020019b4(*pal, a);
        }
        {
            u32 nx = x + 0x10000;
            x = nx;
            if (nx > 0xdf0000) {
                break;
            }
        }
    } while (1);
    Func_02001a72();
    Func_02001a56();
    Func_02001b98(0x10000, 0);
}

typedef signed char s8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern void *Func_08096c80(s32, s32, s32, s32);
extern void Func_08096bec(void *, s32, s32);
extern void Func_08009080(void *, s32);
extern void Func_08009098(void *, void *);
extern s32 Func_08004458(void);
extern void Func_080f9010(s32);

void *Func_08098070(void *arg0)
{
    s32 angle;
    s32 y;
    s32 x;
    s32 index;
    u32 random;
    u32 random2;
    s32 zero;
    void *parent;
    void *child;

    angle = (*(u16 *)((s8 *)arg0 + 6) + 0x2000) & 0xc000;
    parent = Func_08096c80(0xd7, *(s32 *)((s8 *)arg0 + 8),
                           *(s32 *)((s8 *)arg0 + 12) + 0x100000,
                           *(s32 *)((s8 *)arg0 + 16));
    if (parent == 0)
        return 0;
    *(s32 *)((s8 *)parent + 0x1c) = 0x4000;
    *(s32 *)((s8 *)parent + 0x18) = 0x4000;
    *(s32 *)((s8 *)parent + 0x6c) = 0x08097b71;
    *(s32 *)((s8 *)parent + 0x30) = 0x20000;
    *(s32 *)((s8 *)parent + 0x34) = 0x20000;
    zero = 0;
    *(s8 *)((s8 *)parent + 0x55) = zero;
    Func_08009080(parent, 3);
    Func_08096bec(parent, 0x100000, angle);

    index = 7;
    do {
        child = Func_08096c80(0x11d, *(s32 *)((s8 *)arg0 + 8),
                              *(s32 *)((s8 *)arg0 + 12) + 0x100000,
                              *(s32 *)((s8 *)arg0 + 16));
        if (child != 0) {
            Func_08009098(child, (void *)0x0809f0d4);
            x = Func_08004458() + 0x10000;
            *(s32 *)((s8 *)child + 0x34) = 0x10000;
            *(s32 *)((s8 *)child + 0x30) = x;
            *(s8 *)((s8 *)child + 0x55) = 2;
            *(s32 *)((s8 *)child + 0x48) = 0x51e;
            random = Func_08004458();
            *(s32 *)((s8 *)child + 0x28) = random - Func_08004458();
            y = Func_08004458() * 0x18 + 0x80000;
            random2 = Func_08004458();
            Func_08096bec(child, y,
                          ((random2 - Func_08004458()) >> 3) +
                          *(u16 *)((s8 *)arg0 + 6));
        }
        index--;
    } while (index >= 0);
    Func_080f9010(0x8a);
    return parent;
}

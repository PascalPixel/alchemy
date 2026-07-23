typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

void Func_08006864(s32 *, u8 *, void *);

void Func_080fa928(void)
{
    s32 *state;
    s32 value;
    s32 zero;

    value = *(state = *(s32 **)0x03007ff0);
    if ((u32)(value + 0x978c92ad) <= 1) {
        *state = value + 10;
        if (*(u32 *)0x040000c4 & 0x02000000)
            *(u32 *)0x040000c4 = 0x84400004;
        if (*(u32 *)0x040000d0 & 0x02000000)
            *(u32 *)0x040000d0 = 0x84400004;
        {
            volatile u16 *io = (volatile u16 *)0x040000c6;
            *io = 0x400;
            io += 6;
            *io = 0x400;
        }
        zero = 0;
        Func_08006864(&zero, (u8 *)state + 848, (void *)0x05000318);
    }
}

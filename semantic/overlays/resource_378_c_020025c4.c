typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/* Resource 378 object finalisation at 0x020025c4 (156 bytes including pool). */

extern u8 *Func_02005bda();
extern void Func_02005c30();
extern void Func_02005c40();
extern u8 *Func_02005bf6();
extern u8 *Func_02005bfe();
extern u8 *Func_02005c08();
extern u8 *Func_02005c12();
extern u8 *Func_02005c4a();

void Func_020025c4(void)
{
    u8 *candidate = Func_02005bda(8);
    u8 *target;
    u8 *source;
    u8 *state;
    u8 *object;
    u8 *sprite;
    u8 *entry;
    u8 *flags;
    u8 i;
    u8 count;

    if (candidate != 0) {
        Func_02005c30(14, *(s32 *)(candidate + 8), *(s32 *)(candidate + 16));
    }
    Func_02005c40(14, 0);

    target = Func_02005bf6(14);
    source = Func_02005bfe(8);
    *(u16 *)(target + 6) = *(u16 *)(source + 6);

    state = Func_02005c08(14);
    *(s32 *)(state + 108) = (s32)0x0200a67d;

    object = Func_02005c12(14);
    sprite = *(u8 **)(object + 80);
    count = sprite[39];
    for (i = 0; i < count; i++) {
        entry = *(u8 **)(sprite + 40 + i * 4);
        if (entry != 0 && *(s32 *)(entry + 16) != 0) {
            entry[5] = 10;
        }
    }
    sprite[37] = 1;

    flags = Func_02005c4a(14) + 35;
    *flags &= (u8)~1;
    sprite[9] = (u8)((sprite[9] & 0xf3) | 8);
}

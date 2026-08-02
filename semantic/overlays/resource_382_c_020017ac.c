typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

extern u8 *Func_0808a080(s32); extern u32 Func_080000f8(void);
extern u8 *Func_080090c8(s32,s32,s32,s32); extern void Func_08009080(u8 *,s32);
extern void Func_08009098(u8 *,const void *); extern s32 Func_030003e0(s32,s32);

void Func_020017ac(s32 options)
{
    u8 *source = Func_0808a080(19);
    u8 *object;
    u8 *sprite;
    s32 x;
    s32 z;
    s32 direction;

    if (source == 0)
        return;

    x = *(s32 *)(source + 8) + ((s32)((Func_080000f8() << 3) >> 16) - 4) * 0x10000;
    z = *(s32 *)(source + 16) + ((s32)((Func_080000f8() << 3) >> 16) - 4) * 0x10000;
    object = Func_080090c8(172, x, *(s32 *)(source + 12), z);
    if (object == 0)
        return;

    sprite = *(u8 **)(object + 80);
    if ((Func_080000f8() & 1) != 0) {
        Func_08009080(object, 3);
        Func_08009098(object, (const void *)0x0200a8c4);
    } else {
        Func_08009080(object, 2);
        Func_08009098(object, (const void *)0x0200a8dc);
    }
    object[85] = 0;

    if ((options & 2) != 0) {
        s32 side = options & 1;
        s32 spread = Func_030003e0((s32)Func_080000f8(), 10) + 5 + ((side ^ 1) * 4);
        direction = 0x3332 * side - 0x1999;
        *(s32 *)(object + 52) = direction * spread;
        *(s32 *)(object + 48) = 0x1999 * (Func_030003e0((s32)Func_080000f8(), 15) - 7);
        *(s16 *)(object + 100) = 0;
    } else {
        direction = 0x3332 * options - 0x1999;
        *(s32 *)(object + 48) = direction * (Func_030003e0((s32)Func_080000f8(), 10) + 8);
        *(s32 *)(object + 52) = 0x1999 * (Func_030003e0((s32)Func_080000f8(), 14) + 1);
        *(s16 *)(object + 100) = 1;
    }

    *(s32 *)(object + 108) = 0x02009755;
    sprite[38] = 0;
    sprite[9] = (u8)((sprite[9] & (u8)~12) | (*(u8 **)(source + 80))[9] & 12);
}

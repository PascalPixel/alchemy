typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/* Resource 378 object finalisation at 0x020025c4 (156 bytes including pool). */

extern u8 *Func_0808a080();
extern void Func_0808a0f0();
extern void Func_0808a100();

void Func_020025c4(void)
{
    u8 *candidate = Func_0808a080(8);
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
        Func_0808a0f0(14, *(s32 *)(candidate + 8), *(s32 *)(candidate + 16));
    }
    Func_0808a100(14, 0);

    target = Func_0808a080(14);
    source = Func_0808a080(8);
    *(u16 *)(target + 6) = *(u16 *)(source + 6);

    state = Func_0808a080(14);
    *(s32 *)(state + 108) = (s32)0x0200a67d;

    object = Func_0808a080(14);
    sprite = *(u8 **)(object + 80);
    count = sprite[39];
    for (i = 0; i < count; i++) {
        entry = *(u8 **)(sprite + 40 + i * 4);
        if (entry != 0 && *(s32 *)(entry + 16) != 0) {
            entry[5] = 10;
        }
    }
    sprite[37] = 1;

    flags = Func_0808a080(14) + 35;
    *flags &= (u8)~1;
    sprite[9] = (u8)((sprite[9] & 0xf3) | 8);
}

typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u8 *Func_08077008(s32);

s32 Func_080b78e4(s32 id, u8 *output)
{
    u8 *state = Func_08077008(id);
    s8 mode = *(s8 *)(state + 0x131);
    u32 flags = 0;

    if (mode == 1)
        flags = 1;
    if (mode == 2)
        flags |= mode;
    if (state[0x138] != 0)
        flags |= 0x20;
    if (state[0x13B] != 0) {
        s32 kind;
        flags |= 4;
        kind = state[0x128];
        if (kind == 0x79 || kind == 0x94)
            flags &= ~4;
    }
    if (state[0x13D] != 0)
        flags |= 8;
    if (state[0x140] != 0)
        flags |= 0x40;
    if (state[0x13C] != 0)
        flags |= 0x10;
    if (state[0x141] != 0)
        flags |= 1 << (state[0x141] + 6);
    *(u16 *)(output + 0x1C) = flags;
}

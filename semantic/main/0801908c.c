typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_08003d28(void *arg);

void Func_0801908c(void *object)
{
    u8 *source = object;
    u8 *target = source + 0x10;
    s32 value = 0x100;
    u16 index9;
    u16 index10;
    u16 index11;
    u16 index12;
    u16 sampled;
    s32 args[2];
    s32 merged;
    s32 repeated;
    s32 bits;
    s32 clear63;
    s32 clear4;
    s32 generated;

    switch (source[5]) {
    case 9:
        index9 = *(u16 *)(source + 0xc);
        *(u16 *)(source + 0xc) = index9 + 1;
        value = *(u16 *)(0x080366f8 + ((index9 & 0x1f) * 2));
        break;
    case 10:
        index10 = *(u16 *)(source + 0xc);
        *(u16 *)(source + 0xc) = index10 + 1;
        sampled = *(u16 *)(0x080366f8 + ((index10 & 0x1f) * 2));
        value = sampled >> 1;
        break;
    case 11:
        index11 = *(u16 *)(source + 0xc);
        if (index11 <= 7) {
            *(u16 *)(source + 0xc) = index11 + 1;
            value = *(u16 *)(0x080366f8 + 0x20 + index11 * 4);
        }
        break;
    case 12:
        index12 = *(u16 *)(source + 0xc);
        if (index12 <= 7) {
            *(u16 *)(source + 0xc) = index12 + 1;
            sampled = *(u16 *)(0x080366f8 + 0x20 + index12 * 4);
            value = sampled >> 1;
        }
        break;
    }

    if (value == 0x100) {
        bits = target[7];
        clear63 = -0x3f;
        bits &= clear63;
        target[7] = bits;
        bits = target[5];
        clear4 = -4;
        bits &= clear4;
        target[5] = bits;
    } else {
        merged = args[0];
        repeated = value;
        merged &= 0xffff0000;
        merged |= repeated;
        merged &= 0xffff;
        repeated <<= 16;
        merged |= repeated;
        args[0] = merged;
        args[1] &= 0xffff0000;
        generated = Func_08003d28(args) & 0x1f;
        bits = target[7];
        clear63 = -0x3f;
        bits &= clear63;
        bits |= generated * 2;
        target[7] = bits;
        if (value > 0x100) {
            target[5] |= 3;
            *(u16 *)(target + 6) = (*(u16 *)(target + 6) & 0xfffffe00) |
                ((*(u16 *)(source + 6) - 8) & 0x1ff);
            target[4] = source[8] - 8;
            return;
        }
        bits = target[5];
        clear4 = -4;
        bits &= clear4;
        bits |= 1;
        target[5] = bits;
    }

    *(u16 *)(target + 6) = (*(u16 *)(target + 6) & 0xfffffe00) |
        (*(u16 *)(source + 6) & 0x1ff);
    target[4] = source[8];
}

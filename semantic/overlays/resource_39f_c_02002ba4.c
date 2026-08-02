typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

/* resource_39f owner at 0x02002ba4, 104 bytes. The nominal ranked gap begins
 * 60 bytes earlier inside the preceding owner's table/pool; only this prologue
 * through the return at 0x02002c02-0x02002c08 is claimed. Five calls across
 * three established trig/random targets match independently. */
s32 Func_08000118();
s32 Func_08000120();
u32 Func_080000f8(void);

s32 Func_02002ba4(u8 *object)
{
    u8 *handle = *(u8 **)(object + 80);
    s32 phase = *(s32 *)(object + 48);
    s32 vertical = Func_08000118(phase) * 2;
    s32 angle;
    u32 first;
    u32 second;

    if (vertical > 0)
        vertical = -vertical;

    *(s32 *)(object + 8) = *(s32 *)(object + 56)
        + Func_08000120(phase) * 2;
    *(s32 *)(object + 12) = *(s32 *)(object + 60) + vertical;

    angle = Func_08000120(phase + 0x10000);
    if (angle < 0)
        angle += 7;
    *(s16 *)(handle + 30) = (s16)(angle >> 3);

    first = Func_080000f8();
    second = Func_080000f8();
    *(s32 *)(object + 48) = phase
        + ((first << 9) >> 16)
        + ((second << 9) >> 16)
        + 0x400;
    return 0;
}

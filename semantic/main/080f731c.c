typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_0800231c(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);

/*
 * Initialize the presentation workspace: clear the particle pool, seed eight
 * starting records, build the mirrored 40-sample curve, reset scene state,
 * and create the two text resources used by the sequence.
 */
void Func_080f731c(void) {
    u8 *scene;
    u8 *runtime;
    u8 *record;
    const u8 *spacing;
    s16 *curve;
    s32 accumulated_x;
    s32 y;
    s32 angle;
    s32 handle;
    s32 index;

    scene = *(u8 **)0x03001F04;
    runtime = *(u8 **)0x03001EEC;

    record = (u8 *)0x02010018;
    for (index = 0; index < 0x800; index++, record += 0x1C) {
        *(s32 *)record = 0;
    }

    accumulated_x = 0;
    y = (s32)0xFFE00000;
    spacing = (const u8 *)0x080F8736;
    record = runtime + 0x7080;
    for (index = 0; index < 8; index++, record += 0x1C) {
        *(s32 *)(record + 0x00) = (accumulated_x + 0x18) << 16;
        *(s32 *)(record + 0x04) = y;
        *(s32 *)(record + 0x10) = 0;
        *(s32 *)(record + 0x18) = 0;
        accumulated_x += spacing[index];
        y += (s32)0xFFF80000;
    }

    curve = (s16 *)(scene + 0x4D8);
    for (index = 0; index < 0xA0; index++) {
        curve[index] = 0;
    }
    for (index = 0; index < 0x28; index++) {
        angle = index * 0x199;
        curve[0x17 + index] =
            (s16)((u32)(Func_0800231c(angle) * 3) >> 15);
        curve[0x6E - index] =
            (s16)((u32)(Func_0800231c(angle) * 3) >> 15);
    }

    *(s32 *)(scene + 0x8C) = 0;
    *(s32 *)(scene + 0x90) = 0;
    *(s32 *)(scene + 0x94) = 0;
    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    *(volatile u16 *)0x04000050 = 0;

    handle = Func_08015010(0x12, 0, 0xC, 4, 6);
    *(s32 *)(scene + 0x4CC) = handle;
    Func_08015080(0x905, handle, 0, 8);
    Func_08015080(0x904, *(s32 *)(scene + 0x4CC), 0, 0);
}

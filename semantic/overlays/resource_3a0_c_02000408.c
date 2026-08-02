typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_020003cc(s32 *a, s32 *b);
extern u16 Func_08000100(s32 zDifference, s32 xDifference);
extern void Func_08009080(u8 *actor, s32 mode);

/* Complete 248-byte owner through the adjustment word at 0x020004fc. */
s32 Func_02000408(u8 *actor, u8 *target, s32 distanceLimit, s32 force)
{
    s32 result = 0;
    u8 *moving = actor + 0x5b;
    s16 *state = (s16 *)(actor + 0x64);

    if (*moving == 1 && *state == 0) {
        Func_08009080(actor, 1);
        return 1;
    }

    if (Func_020003cc((s32 *)(target + 8), (s32 *)(actor + 8)) < distanceLimit ||
        force != 0) {
        u16 angle = Func_08000100(*(s32 *)(target + 16) - *(s32 *)(actor + 16),
                                  *(s32 *)(target + 8) - *(s32 *)(actor + 8));
        u16 facing = *(u16 *)(actor + 6) & 0xf000;
        u16 forward = angle & 0xf000;
        u16 clockwise = (angle + 0x1000) & 0xf000;
        u16 counterclockwise = (angle - 0x1000) & 0xf000;

        if (forward != facing && clockwise != facing &&
            counterclockwise != facing && force == 0) {
            *moving = 0;
            Func_08009080(actor, 2);
            *state = 0;
        } else {
            *moving = 1;
            Func_08009080(actor, 1);
            *state = 1;
            result = 1;
        }
    } else {
        *moving = 0;
        Func_08009080(actor, 2);
        *state = 0;
    }
    return result;
}

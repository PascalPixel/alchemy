typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

extern void Func_08009098(u8 *object, const void *script);

/*
 * Per-frame callback for the randomized child effect.  Its signed vertical
 * step advances Y and the remaining lifetime selects one of three symmetric
 * X/Z velocities before the terminal script swap.
 */
void Func_02003144(u8 *object)
{
    s32 velocity = 0;
    u16 *lifetime = (u16 *)(object + 102);
    s32 quadrant;

    *(s32 *)(object + 12) += (s32)*(s16 *)(object + 100) << 12;
    *(s32 *)(object + 60) = *(s32 *)(object + 12);
    quadrant = (*lifetime >> 2) & 3;
    if (quadrant == 0)
        velocity = 0x10000;
    else if (quadrant == 1 || quadrant == 3)
        velocity = 0xcccc;
    else if (quadrant == 2)
        velocity = 0x9999;
    *(s32 *)(object + 24) = velocity;
    *(s32 *)(object + 28) = velocity;

    (*lifetime)--;
    if ((s16)*lifetime <= 0)
        Func_08009098(object, (const void *)0x0200c18c);
}

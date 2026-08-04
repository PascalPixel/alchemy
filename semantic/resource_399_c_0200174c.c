typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

/*
 * resource_399 kind-222 effect update at 0x0200174c, complete 72-byte span
 * through its one-word pool. The signed phase accelerates horizontal motion,
 * the sprite rises and grows each frame, and its 60-frame lifetime ends via
 * the standard object-release import.
 */

extern void Func_080090d0(u8 *effect);

void Func_0200174c(u8 *effect)
{
    s16 *phase = (s16 *)(effect + 100);

    *(s32 *)(effect + 8) += (s32)*phase << 8;
    *(s32 *)(effect + 12) += 0x8000;
    *(s32 *)(effect + 24) += 0x7ae;
    *(s32 *)(effect + 28) += 0x7ae;
    *phase = (s16)(*phase + 2);

    if (--*(s32 *)(effect + 104) == 0) {
        Func_080090d0(effect);
    }
}

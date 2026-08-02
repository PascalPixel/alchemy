typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Complete published prologue-less effect integrator. The adjacent spawner
 * installs its Thumb address as a per-frame callback. The explicit preload
 * order preserves the reference scheduler's increment-before-value updates.
 */
void Func_02000ab0(u8 *effect)
{
    s32 increment;
    s32 value;
    u8 *owner;

    value = *(s32 *)(effect + 8);
    increment = *(s32 *)(effect + 68);
    *(s32 *)(effect + 8) = value + increment;

    increment = *(s32 *)(effect + 72);
    value = *(s32 *)(effect + 12);
    *(s32 *)(effect + 12) = value + increment;

    increment = *(s32 *)(effect + 76);
    value = *(s32 *)(effect + 16);
    *(s32 *)(effect + 16) = value + increment;

    increment = *(s32 *)(effect + 48);
    value = *(s32 *)(effect + 24);
    *(s32 *)(effect + 24) = value + increment;

    increment = *(s32 *)(effect + 52);
    value = *(s32 *)(effect + 28);
    *(s32 *)(effect + 28) = value + increment;

    owner = *(u8 **)(effect + 80);
    *(u16 *)(owner + 30) =
        (u16)(*(u16 *)(owner + 30) + *(u16 *)(effect + 100));
}

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_399 five-lane kind-222 emitter at 0x02001794, complete 304-byte
 * span through its four-word pool. Each lane tests a different phase of the
 * same 60-frame cadence, spawns at its own fixed-point coordinate, applies
 * the common initializer, installs the 60-frame updater, and selects mode 5.
 *
 * The five blocks remain explicit because they are five distinct machine call
 * sites with different phase/position constants; collapsing them into a data
 * loop would hide the source/machine call-order evidence.
 */

extern s32 Func_030003e0(s32 value, s32 divisor);
extern void Func_08009080(u8 *effect, s32 mode);
extern u8 *Func_080090c8(s32 kind, s32 x, s32 y, s32 z);
extern void Func_02001704(u8 *effect);

void Func_02001794(void)
{
    s32 frame = *(volatile s32 *)0x03001e40;
    u8 *effect;

    if (Func_030003e0(frame, 60) == 0) {
        effect = Func_080090c8(222, 0x01cf0000, 0, 146 << 17);
        if (effect != 0) {
            Func_02001704(effect);
            *(s32 *)(effect + 104) = 60;
            *(u32 *)(effect + 108) = 0x0200974d;
            Func_08009080(effect, 5);
        }
    }

    if (Func_030003e0(frame + 30, 60) == 0) {
        effect = Func_080090c8(222, 160 << 17, 128 << 14, 178 << 17);
        if (effect != 0) {
            Func_02001704(effect);
            *(s32 *)(effect + 104) = 60;
            *(u32 *)(effect + 108) = 0x0200974d;
            Func_08009080(effect, 5);
        }
    }

    if (Func_030003e0(frame + 10, 60) == 0) {
        effect = Func_080090c8(222, 236 << 15, 0, 140 << 15);
        if (effect != 0) {
            Func_02001704(effect);
            *(s32 *)(effect + 104) = 60;
            *(u32 *)(effect + 108) = 0x0200974d;
            Func_08009080(effect, 5);
        }
    }

    if (Func_030003e0(frame + 50, 60) == 0) {
        effect = Func_080090c8(222, 171 << 17, 0, 248 << 15);
        if (effect != 0) {
            Func_02001704(effect);
            *(s32 *)(effect + 104) = 60;
            *(u32 *)(effect + 108) = 0x0200974d;
            Func_08009080(effect, 5);
        }
    }

    if (Func_030003e0(frame + 80, 60) == 0) {
        effect = Func_080090c8(222, 0x01af0000, 0, 171 << 16);
        if (effect != 0) {
            Func_02001704(effect);
            *(s32 *)(effect + 104) = 60;
            *(u32 *)(effect + 108) = 0x0200974d;
            Func_08009080(effect, 5);
        }
    }
}

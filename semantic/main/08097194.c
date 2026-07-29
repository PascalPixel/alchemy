#include "effect_0809b11c.h"

void Func_08002dd8(s32);
void Func_080030f8(u32);
void Func_08003f3c(u32);
void Func_08004278(const void *);
void Func_0809202c(void);

/*
 * Tear down the battle presentation: clear every live effect slot, wait up to
 * thirty frames for the two transient object callbacks to disappear, restore
 * the saved world position, publish the event flags, play the queued sound,
 * and release the presentation workspace.
 */
void Func_08097194(void) {
    u8 *scene;
    u8 *battle;
    u8 *runtime;
    struct EffectSlot *effect;
    u8 *object;
    s32 effect_index;
    s32 object_index;
    s32 wait_frames;
    s32 transient_found;

    scene = *(u8 **)0x03001F30;
    battle = *(u8 **)0x03001EBC;
    runtime = *(u8 **)0x03001E70;

    effect = (struct EffectSlot *)(scene + 0x58);
    for (effect_index = 0; effect_index < 24; effect_index++, effect++) {
        if (effect->active != 0) {
            Func_0809bb34(effect);
        }
    }

    if ((s8)battle[0xCC6] != 0) {
        return;
    }

    wait_frames = 0;
    do {
        object = *(u8 **)0x03001E64;
        transient_found = 0;
        for (object_index = 0; object_index < 64; object_index++, object += 0x70) {
            const void *callback = *(const void **)(object + 0x6C);
            if (callback == (const void *)0x08096D85 ||
                callback == (const void *)0x08096D2D) {
                transient_found = 1;
                break;
            }
        }
        if (transient_found != 0) {
            wait_frames++;
            Func_080030f8(1);
        }
    } while (transient_found != 0 && wait_frames <= 29);

    battle[0xCC7] = 0;
    Func_08004278((const void *)0x08096F8D);
    Func_08003f3c((u32)*(s16 *)(scene + 0x46));
    *(s32 *)(runtime + 4) = *(s32 *)(scene + 0x4C);
    *(s32 *)(runtime + 8) = *(s32 *)(scene + 0x50);
    *(s32 *)(runtime + 0xC) = *(s32 *)(scene + 0x54);
    if (*(s16 *)(scene + 0x1E) != 8) {
        *(s16 *)(battle + 0xCC0) = 1;
    }
    Func_0809202c();
    Func_08002dd8(0x38);
}

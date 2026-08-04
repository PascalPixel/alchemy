#include "types.h"

struct SceneState_080941e0 {
    u8 unknown_000[0x19e];
    s16 scene_mode;
    u8 unknown_1a0[0x26];
    s16 fade_active;
};

void Func_080f9010(s32 sound);
void Func_080901c0(s32 effect, s32 frames);
void Func_080030f8(s32 frames);

/*
 * Play the transition sounds, start the appropriate scene fade, and darken
 * its reserved palette entry from white to black over sixteen frames.
 */
void Func_080941e0(void)
{
    struct SceneState_080941e0 *scene =
        *(struct SceneState_080941e0 **)0x03001ebc;

    Func_080f9010(*(s16 *)0x0200042e);
    Func_080f9010(0x120);
    Func_080f9010(0x93);

    if (scene->scene_mode == 3) {
        volatile u16 *palette = (volatile u16 *)0x050001e6;
        s32 blue = 0x7800;
        s32 green = 0x03c0;
        s32 red = 0x001e;
        s32 frame;

        *palette = 0x7fff;
        Func_080901c0(0x401, 16);
        scene->fade_active = 0;
        Func_080030f8(16);

        for (frame = 0; frame < 16; frame++) {
            *palette = (u16)(blue | green | red);
            Func_080030f8(1);
            blue -= 0x0800;
            green -= 0x0040;
            red -= 2;
        }
        return;
    }

    {
        volatile u16 *palette = (volatile u16 *)0x05000000;
        s32 blue = 0x7800;
        s32 green = 0x03c0;
        s32 red = 0x001e;
        s32 frame;

        *palette = 0x7fff;
        Func_080901c0(0x207, 16);
        scene->fade_active = 0;
        Func_080030f8(16);

        for (frame = 0; frame < 16; frame++) {
            *palette = (u16)(blue | green | red);
            Func_080030f8(1);
            blue -= 0x0800;
            green -= 0x0040;
            red -= 2;
        }
    }
}

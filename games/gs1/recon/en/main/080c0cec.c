#include "types.h"

#define BattlePresentation_SetupTransitionScene Func_080c0cec

void BattlePresentation_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode)
{
    u8 *scene = *(u8 **)0x03001e80;
    s32 *position = (s32 *)(scene + 12);
    s32 scale = Func_080022ec(mode << 16, 100);
    s32 source_bounds[3];
    s32 measured_bounds[3];
    s32 render_bounds[3];
    s32 width;

    position[0] = x;
    position[1] = depth;
    position[2] = y;
    source_bounds[0] = 0;
    source_bounds[1] = 0;
    source_bounds[2] = 0;

    Func_08005258(
        0x01fe0000,
        Func_08007310(0x01fe0000, 0xc000),
        0x03fc0000);
    Func_080049ac();
    Func_08004cb4(position);
    Func_08004c1c(*(s16 *)(scene + 0x36));
    Func_08004bd4(*(s16 *)(scene + 0x34));
    render_bounds[0] = 0;
    render_bounds[1] = 0;
    render_bounds[2] = 0x01fe0000;
    Func_080072f0(render_bounds, scene);
    *(s32 *)0x03001cec = 120;
    *(s32 *)0x03001cf0 = 120;
    Func_080049ac();
    Func_080051d8(scene, position);
    Func_08005268(source_bounds, measured_bounds);

    width = scale * 0x1fe;
    Func_080c0a24(
        0x780000,
        0x780000,
        (120 - measured_bounds[0]) << 8,
        (120 - measured_bounds[1]) << 8,
        scale);
    Func_08005258(
        width,
        Func_08007310(width, 0xc000),
        scale * 0x3fc);
}

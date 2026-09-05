#include "types.h"

void Func_080051d8();
void Func_080c0a24();

void BattlePresentation_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode)
{
    u8 *scene = *(u8 **)0x03001e80;
    s32 *position = (s32 *)(scene + 12);
    s32 *hud = (s32 *)0x03001ce0;
    s32 scale = Func_080022ec(mode << 16, 100);
    s32 render_bounds[3];
    s32 measured_bounds[3];
    s32 source_bounds[3];
    s32 span = 0x01fe0000;
    u32 (*blend)(u32, u32);
    s32 alpha;
    s32 half;
    s32 width;
    u32 result;

    position[0] = x;
    position[1] = depth;
    position[2] = y;
    source_bounds[0] = 0;
    alpha = 0xc000;
    source_bounds[1] = 0;
    source_bounds[2] = 0;

    blend = (u32 (*)(u32, u32))0x0300013c;
    result = blend(span, alpha);
    Func_08005258(span, result, span * 2);
    Func_080049ac();
    Func_08004cb4(position);
    Func_08004c1c(*(s16 *)(scene + 0x36));
    Func_08004bd4(*(s16 *)(scene + 0x34));
    render_bounds[0] = 0;
    render_bounds[1] = 0;
    render_bounds[2] = span;
    ((void (*)())0x03000250)(render_bounds, scene);
    hud[3] = 120;
    hud[4] = 120;
    Func_080049ac();
    Func_080051d8(scene, position);
    Func_08005268(source_bounds, measured_bounds);

    Func_080c0a24(
        0x780000,
        0x780000,
        (120 - measured_bounds[0]) << 8,
        (120 - measured_bounds[1]) << 8,
        scale);
    half = scale * 0xff;
    width = half * 2;
    result = blend(width, alpha);
    Func_08005258(width, result, half * 4);
}

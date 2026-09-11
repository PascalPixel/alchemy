#include "types.h"
#include "scene.h"

/* battle/presentation/trans/setup.c */
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode)
{
    u8 *scene = *(u8 **)0x03001e80;
    s32 *position = (s32 *)(scene + 12);
    s32 *hud = (s32 *)0x03001ce0;
    s32 scale = Battle_Run(mode << 16, 100);
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
    Camera_StoreSceneParameters(span, result, span * 2);
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(position);
    SceneTransform_ApplyYaw(*(s16 *)(scene + 0x36));
    SceneTransform_ApplyPitch(*(s16 *)(scene + 0x34));
    render_bounds[0] = 0;
    render_bounds[1] = 0;
    render_bounds[2] = span;
    ((void (*)())0x03000250)(render_bounds, scene);
    hud[3] = 120;
    hud[4] = 120;
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(scene, position);
    Battle_unk8_2(source_bounds, measured_bounds);

    Battle_unk9_2(
        0x780000,
        0x780000,
        (120 - measured_bounds[0]) << 8,
        (120 - measured_bounds[1]) << 8,
        scale);
    half = scale * 0xff;
    width = half * 2;
    result = blend(width, alpha);
    Camera_StoreSceneParameters(width, result, half * 4);
}

/* battle/presentation/trans/midpoint.c */
struct Point {
    u8 unknown0[8];
    s32 x;
    u8 unknown1[4];
    s32 y;
};

struct Point **GetBattleObjectSlot(s32);

void BattlePres_SetupTransitionAtPairMidpoint(s32 first, s32 second, s32 mode)
{
    struct Point *left = *GetBattleObjectSlot(first);
    struct Point *right = *GetBattleObjectSlot(second);
    s32 left_x = left->x;
    s32 right_x = right->x;
    s32 left_y = left->y;
    s32 right_y = right->y;
    s32 x = (right_x + left_x) / 2;
    s32 y = (right_y + left_y) / 2;

    Battle_SetMode(x, 0, y, mode);
}

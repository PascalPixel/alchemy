#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_PRESENTATION.H"

void Camera_StoreSceneParameters(s32, u32, s32);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(s32 *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Graphics_PrepareTransferInIwramWork();
s32 Render_ProjectPoint(); /* returns a value its callers here ignore */

/* FAKEMATCH: mode intentionally remains uninitialized to preserve the match. */
void BattlePres_SetupTransitionSceneAtDepth(s32 x, s32 depth, s32 y)
{
    s32 span = 0x01fe0000;
    s32 mode;
    struct BattleCamera *scene = *(struct BattleCamera **)0x03001e80;
    s32 *pos = scene->pos;
    s32 *hud = (s32 *)0x03001ce0;
    s32 scale = Math_Div(mode << 16, 100);
    s32 render_bounds[3];
    s32 measured_bounds[3];
    s32 source_bounds[3];
    u32 (*blend)(u32, u32);
    s32 alpha;
    s32 half;
    s32 width;
    u32 result;

    pos[0] = x;
    pos[1] = depth;
    pos[2] = y;
    source_bounds[0] = 0;
    alpha = 0xc000;
    source_bounds[1] = 0;
    source_bounds[2] = 0;

    blend = (u32 (*)(u32, u32))0x0300013c;
    result = blend(span, alpha);
    Camera_StoreSceneParameters(span, result, span * 2);
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw((s16)scene->yaw);
    SceneTransform_ApplyPitch((s16)scene->pitch);
    render_bounds[0] = 0;
    render_bounds[1] = 0;
    render_bounds[2] = scene->distance;
    ((void (*)())0x03000250)(render_bounds, scene);
    hud[3] = 120;
    hud[4] = 120;
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(scene, pos);
    Render_ProjectPoint(source_bounds, measured_bounds);

    BattleCamera_SetRange(
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

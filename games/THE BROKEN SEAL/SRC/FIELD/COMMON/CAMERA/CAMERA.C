#include "IO_REG.H"
#include "FIXED_POINT_POSITION.H"
#include "BATTLE_PRESENTATION.H"
#include "PROJECT.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"

s32 LuckyDice_Run();
s32 Audio_PlayCue(s32);

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Camera_StoreSceneParameters(u32, u32, u32);

s32 Runtime_BlankDisplayAndRun(void)
{
    REG_DISPCNT = 0x40;
    Audio_PlayCue(9);
    LuckyDice_Run();
    return 0;
}

void Camera_ResetSceneDefaults(void)
{
    struct BattleCamera *state = gCameraWork;
    struct FixedPointPosition local;
    u32 result;
    u32 param1;

    state->pitch = 152 << 8;
    state->distance = 255 << 17;
    state->pos[0] = 0;
    state->pos[1] = 0;
    state->pos[2] = 0;
    state->yaw = 0;
    state->follow_pos = 0;
    gProjection.center_x = 0;
    gProjection.center_y = 0;
    state->unknown_18 = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(&state->pos[0]);
    SceneTransform_ApplyYaw((s16)state->yaw);
    SceneTransform_ApplyPitch((s16)state->pitch);

    local.x = 0;
    local.y = 0;
    local.z = state->distance;
    Iwram_TransformVector((s32 *)&local, (s32 *)state);

    param1 = 250;
    param1 = param1 << 16;
    result = Iwram_RatioMulQ14(param1, 192 << 8);

    param1 = 250;
    param1 = param1 << 16;
    Camera_StoreSceneParameters(param1, result, 0x7fff0000);
}

s32 FixedPoint_Multiply8_8(s32 arg0, s32 arg1)
{
    s16 left = arg0;
    s16 right = arg1;
    s32 product;
    s32 adjusted;
    s32 multiplier;

    multiplier = right;
    product = left;
    product *= multiplier;
    if (product >= 0) {
        adjusted = product;
    } else {
        adjusted = product + 255;
    }
    return (s16)(adjusted >> 8);
}

s16 Math_ScaleByRatio(s16 arg0, s16 arg1)
{
    return (arg0 << 8) / arg1;
}

s16 FixedPoint_Reciprocal(s16 value)
{
    return 0x10000 / value;
}

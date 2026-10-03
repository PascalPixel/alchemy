#include "PROJECT.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"

s32 LuckyDice_Run();
s32 Audio_PlayCue(s32);

/* The interrupt master-enable word (IME) before and after the blanked frame. */
#define REG_IME (*(volatile u16 *)0x04000000)

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Camera_StoreSceneParameters(u32, u32, u32);

struct SceneCameraState {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct SceneCameraRuntime {
    struct SceneCameraState *state;
    u8 filler04[124];
    void *secondary;
};

struct SceneCameraTransfer {
    s32 first;
    s32 second;
    s32 third;
};


extern struct SceneCameraRuntime gCameraWork;



s32 Runtime_BlankDisplayAndRun(void)
{
    REG_IME = 0x40;
    Audio_PlayCue(9);
    LuckyDice_Run();
    return 0;
}

void Camera_ResetSceneDefaults(void)
{
    struct SceneCameraState *state = gCameraWork.state;
    struct SceneCameraTransfer local;
    u32 result;
    u32 param1;

    state->field34 = 152 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field36 = 0;
    state->field1c = 0;
    gProjection.center_x = 0;
    gProjection.center_y = 0;
    state->field18 = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(&state->field0c);
    SceneTransform_ApplyYaw(state->field36);
    SceneTransform_ApplyPitch(state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
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

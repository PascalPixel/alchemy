#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);

struct State_080b7f9c {
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

struct Local_080b7f9c {
    s32 first;
    s32 second;
    s32 third;
};

extern struct State_080b7f9c *gCameraWork;

void Camera_InitDefaultTransform(void)
{
    struct State_080b7f9c *state = gCameraWork;
    struct Local_080b7f9c transfer;

    state->field36 = 192 << 6;
    state->field34 = 254 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field1c = 0;
    state->field18 = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(&state->field0c);
    SceneTransform_ApplyYaw(state->field36);
    SceneTransform_ApplyPitch(state->field34);

    transfer.first = 0;
    transfer.second = 0;
    transfer.third = state->field20;
    Iwram_TransformVector((s32 *)&transfer, (s32 *)state);
}

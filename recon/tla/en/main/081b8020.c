/* Draft, differ 2 of 54: after the two zero stores to the local vector the
 * native copies the state pointer for the call (adds r1, r5, #0) before it
 * loads the distance; this loads first. Measured: the pointer as a
 * temporary behind a "+l" barrier moves the copy after the routine's
 * address instead (differ 2). */
#include "io_reg.h"
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"

void Func_081b83c4(void);
void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Camera_StoreSceneParameters(u32, u32, u32);

struct SceneCamera {
    u8 unknown_00[0x0c];
    s32 pos[3];
    s32 tilt;
    s32 follow_pos;
    s32 distance;
    u8 unknown_24[0x10];
    s16 pitch;
    s16 yaw;
};

struct SceneProjection {
    u32 unknown_00[3];
    s32 center_x;
    s32 center_y;
};

extern struct SceneProjection gCameraSceneParameters;

void Camera_ResetSceneDefaults(void)
{
    struct SceneCamera *state = Ram_HeapSlots->camera_work;
    s32 local[3];
    u32 result;
    u32 param1;

    state->pitch = 147 << 8;
    state->distance = 255 << 17;
    state->pos[0] = 0;
    state->pos[1] = 0;
    state->pos[2] = 0;
    state->yaw = 0;
    state->follow_pos = 0;
    gCameraSceneParameters.center_x = 0;
    gCameraSceneParameters.center_y = 0;
    state->tilt = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(state->pos);
    SceneTransform_ApplyYaw(state->yaw);
    SceneTransform_ApplyPitch(state->pitch);

    local[0] = 0;
    local[1] = 0;
    local[2] = state->distance;
    Iwram_TransformVector(local, (s32 *)state);

    param1 = 250;
    param1 = param1 << 16;
    result = Iwram_RatioMulQ14(param1, 192 << 8);

    param1 = 250;
    param1 = param1 << 16;
    Camera_StoreSceneParameters(param1, result, 0x7fff0000);
}

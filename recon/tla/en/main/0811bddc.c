/* Draft: Camera_InitDefaultTransform, 96-byte EN extent.
 * 2026-10-04: differ 2 with ☀️'s body reading Ram_HeapSlots->camera_work.
 * The native moves the result pointer into r1 before loading the distance
 * for the transfer's z; this build loads first. Tried: an s32 array, a
 * pointer to the transfer, the result pointer as a local before the z store
 * (all 2), an initialised block (29) and devsearch 300 iterations (2).
 * GetBattleObjectSlot, its ☀️ neighbour, matches and is C.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIXED_POINT_POSITION.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PRESENTATION.H"

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);

void Camera_InitDefaultTransform(void)
{
    struct BattleCamera *state = Ram_HeapSlots->camera_work;
    struct FixedPointPosition transfer;

    state->yaw = 192 << 6;
    state->pitch = 254 << 8;
    state->distance = 255 << 17;
    state->pos[0] = 0;
    state->pos[1] = 0;
    state->pos[2] = 0;
    state->follow_pos = 0;
    state->unknown_18 = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(&state->pos[0]);
    SceneTransform_ApplyYaw((s16)state->yaw);
    SceneTransform_ApplyPitch((s16)state->pitch);

    transfer.x = 0;
    transfer.y = 0;
    transfer.z = state->distance;
    Iwram_TransformVector((s32 *)&transfer, (s32 *)state);
}


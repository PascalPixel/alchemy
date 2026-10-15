#include "types.h"
#include "scene.h"
#include "motobj.h"
#include "iwram_call.h"
#include "fixed_point_position.h"
#include "battle_work.h"
#include "battle_display.h"

void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
    s32 storage_kind;

    if (object == NULL)
        return NULL;
    storage_kind = object->recordStorageKind & 0xF;
    if (storage_kind == 1) {
        if (record_index == 0) {
            return object->records;
        }
    } else if (storage_kind == 2) {
        return ((void **)object->records)[record_index];
    }
    return NULL;
}

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);

void Camera_InitDefaultTransform(void)
{
    struct BattleCamera *state = Ram_HeapSlots->cameraWork;
    struct FixedPointPosition transfer;

    state->yaw = 192 << 6;
    state->pitch = 254 << 8;
    state->distance = 255 << 17;
    state->pos[0] = 0;
    state->pos[1] = 0;
    state->pos[2] = 0;
    state->followPos = 0;
    state->unk18 = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(&state->pos[0]);
    SceneTransform_ApplyYaw((s16)state->yaw);
    SceneTransform_ApplyPitch((s16)state->pitch);

    transfer.x = 0;
    transfer.y = 0;
    {
        struct BattleCamera *result = state;
        /* FAKEMATCH: pass the camera before loading the distance; removing the constraint swaps two rows. */
        asm volatile("" : "+l"(result));
        transfer.z = state->distance;
        Iwram_TransformVector((s32 *)&transfer, (s32 *)result);
        /* FAKEMATCH: canceling XORs keep the state pointer live across the call;
         * without them the unpinned result changes two instruction rows. */
        state->distance ^= 1;
        state->distance ^= 1;
    }
}

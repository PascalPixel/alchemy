#include "types.h"
#include "scene.h"
#include "motobj.h"
#include "iwram_call.h"
#include "fixed_point_position.h"
#include "btlwork.h"
#include "btldisp.h"

void *GetMotionRecord(struct MotionObject *object, s32 record_index)
{
    s32 storage_kind;

    if (object == NULL)
        return NULL;
    storage_kind = object->record_storage_kind & 0xF;
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
    {
        /* FAKEMATCH: ⚓️ passes the camera before it loads the distance */
        register struct BattleCamera *result asm("r1") = state;
        asm volatile("" : "+l"(result)); /* FAKEMATCH: as above */
        transfer.z = state->distance;
        Iwram_TransformVector((s32 *)&transfer, (s32 *)result);
    }
}

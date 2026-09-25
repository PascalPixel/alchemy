#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_PRESENTATION.H"

struct SceneCameraTransfer {
    s32 x;
    s32 y;
    s32 z;
};

struct LinkWork {
    u8 pad0[0x44];
    u8 enabled;
    u8 pad1[0x0b];
    u8 side;
    u8 miss;
    u8 paused;
};

#define LINK_STAT (*(u16 *)0x03001f64)
#define REG_SIOCNT (*(volatile u32 *)0x04000128)

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);

void BattlePresentation_UpdateCamera(void)
{
    void **slot = (void **)ADDR_03001E80;
    struct BattleCamera *state = slot[0];
    struct BattlePresentationTransition *transition = slot[32];
    struct LinkWork *work = slot[-3];
    struct SceneCameraTransfer local;
    s32 *pos;
    s16 delta;
    u32 id;

    if (work->enabled != 0) {
        if ((LINK_STAT & 3) != 3) {
            work->miss++;
            if (work->miss > 24) {
                work->paused = 1;
            }
        } else {
            id = (REG_SIOCNT << 0x1A) >> 0x1E;
            if (work->side != id) {
                work->paused = 1;
            }
            work->miss = 0;
        }
    }

    if (transition->frames != 0) {
        delta = transition->target_yaw - state->yaw;
        delta /= 16;
        state->yaw += delta;
        transition->frames--;
    }

    pos = state->pos;
    if (state->follow_pos != 0) {
        pos = state->follow_pos;
    }

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(pos);
    SceneTransform_ApplyYaw((s16)state->yaw);
    SceneTransform_ApplyPitch((s16)state->pitch);

    local.x = 0;
    local.y = 0;
    local.z = state->distance;
    ((void (*)(struct SceneCameraTransfer *, struct BattleCamera *))0x03000250)(&local, state);

    if (transition->flag == 0) {
        BattleCamera_SetRange(0x780000, 0x780000, 0, 0, 0x10000);
    }
}

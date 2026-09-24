#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"

struct BattleEffectSceneObject {
    u8 reserved_00[0x45];
    s8 active;
    u8 reserved_46[2];
};

struct BattleEffectScene {
    u8 reserved_000[0x1e];
    s16 scene_mode;
    u8 reserved_020[0x26];
    s16 audio_handle;
    u8 reserved_048[4];
    s32 x;
    s32 y;
    s32 z;
    struct BattleEffectSceneObject objects[24];
};

struct BattleEffectRuntime {
    u8 reserved_000[0xcc0];
    s16 restore_requested;
    u8 reserved_cc2[4];
    s8 teardown_blocked;
    s8 teardown_state;
};

struct BattleEffectPosition {
    u8 reserved_00[4];
    s32 x;
    s32 y;
    s32 z;
};

struct BattleObjectSlot {
    u8 reserved_00[0x6c];
    void (*update)(void);
};

#define BATTLE_OBJECT_SLOTS (*(struct BattleObjectSlot **)0x03001e64)

void BattleFx_ClearOwnedSlot(struct BattleEffectSceneObject *object);
void WaitFrames(s32 frames);
void Resource_ResetEntry(s32 handle);
void BattleFx_PlayQueuedSound(void);
void Runtime_ReleaseHeapBlock(s32 asset_id);


/* Drain effect objects, restore the scene position, and release effect data. */
void BattleEffect_CleanupSceneObjects(void)
{
    struct BattleEffectScene **scene_cell;
    struct BattleEffectScene *scene;
    struct BattleEffectRuntime *runtime;
    struct BattleEffectPosition *position;
    struct BattleEffectSceneObject *scene_object;
    s32 remaining;

    scene_cell = (struct BattleEffectScene **)0x03001f30;
    scene = *scene_cell;
    runtime = *(struct BattleEffectRuntime **)((u8 *)scene_cell - 116);
    position = *(struct BattleEffectPosition **)((u8 *)scene_cell - 192);
    for (remaining = 0; remaining < 24; remaining++) {
        scene_object = &scene->objects[remaining];
        if (scene_object->active != 0)
            BattleFx_ClearOwnedSlot(scene_object);
    }

    if (runtime->teardown_blocked == 0) {
        s32 waited = 0;
        void (*first_active_update)(void) =
            (void (*)(void))0x08096d85;
        void (*second_active_update)(void) =
            (void (*)(void))0x08096d2d;
        s32 active;

        do {
            struct BattleObjectSlot *slot = BATTLE_OBJECT_SLOTS;

            active = 0;
            remaining = 0;
            while (remaining <= 63) {
                void (*update)(void) = slot->update;

                if (update == first_active_update ||
                    update == second_active_update) {
                    active = 1;
                    break;
                }
                remaining++;
                slot++;
            }

            if (active != 0) {
                waited++;
                WaitFrames(1);
            }
        } while (active != 0 && waited <= 29);

        runtime->teardown_state = 0;
        Scheduler_RemoveCallback(0x08096f8d);
        Resource_ResetEntry(scene->audio_handle);
        position->x = scene->x;
        position->y = scene->y;
        position->z = scene->z;
        if (scene->scene_mode != 8)
            runtime->restore_requested = 1;
        BattleFx_PlayQueuedSound();
        Runtime_ReleaseHeapBlock(0x38);
    }
}

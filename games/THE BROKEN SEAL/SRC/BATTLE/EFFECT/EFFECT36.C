#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "SCENE.H"
#include "RAM_BUFFER.H"
#include "DMA.H"
#include "GLOBAL_CELLS.H"

extern u8 gObjectSlots[];
extern u8 gEffectWork[];
void BattleFx_UpdateScaledArcObjectB(void);
void BattleFx_UpdateScaledArcObjectA(void);
void BattleFx_UpdateAllEffectSlots(void);

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

#define BATTLE_OBJECT_SLOTS (*(struct BattleObjectSlot **)gObjectSlots)
void BattleFx_ClearOwnedSlot(struct BattleEffectSceneObject *object);
void Resource_ResetEntry(s32 handle);
void BattleFx_PlayQueuedSound(void);

void ObjectGroup_SetActionForOthers(void *object, s32 mode, s32 value);
void BattleFx_FlickerObjectAndTick();
void BattleFx_CycleObjectValueByCounter();
#define CALLBACK_1      (u32)BattleFx_FlickerObjectAndTick
#define CALLBACK_2      (u32)BattleFx_CycleObjectValueByCounter
extern u8 MsgAbilityAnnounce[];

struct BattleSceneBuffers {
    u8 *scene;
    u8 unknown_04[16];
    u8 *work;
};

extern struct BattleSceneBuffers Data_03001ebc;
extern s32 Data_03001e40;
extern const s32 Data_080a0108[];
s32 GameFlag_TestFar(s32);
void BattleFx_ApplyColorToTargetBuffer(s32, s32);
void BattleFx_StartBufferInterpolation(s32);

void BattleFx_ApplyColorToTargetBuffer(s32 arg0, s32 arg1);
void BattleFx_StartBufferInterpolation(s32 value);

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

s32 Render_ProjectPoint(struct EffectVector *position, struct EffectVector *result);

extern u8 gFrameCount[];
void Func_08097644(void);
void *Runtime_AllocateBlock(s32 slot, s32 size);
void BattleEffect_InitializeSharedScene(void);
s32 Math_ModU(s32, s32);
void BattleFx_AdvanceHueCycle(void);
void BattleFx_ApplyColorToTargetBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Ui_FillBank15PaletteGrey(void);

/* Drain effect objects, restore the scene position, and release effect data. */
void BattleEffect_CleanupSceneObjects(void)
{
    struct BattleEffectScene **scene_cell;
    struct BattleEffectScene *scene;
    struct BattleEffectRuntime *runtime;
    struct BattleEffectPosition *position;
    struct BattleEffectSceneObject *scene_object;
    s32 remaining;

    scene_cell = (struct BattleEffectScene **)gEffectWork;
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
            (void (*)(void))BattleFx_UpdateScaledArcObjectB;
        void (*second_active_update)(void) =
            (void (*)(void))BattleFx_UpdateScaledArcObjectA;
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
        Scheduler_RemoveCallback((s32)BattleFx_UpdateAllEffectSlots);
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

void EventObject_Initialize(void)
{
    s32 zero;
    s32 event_value;
    s32 event_index;
    void *event_object;
    void *event_state;
    void *render_state;

    event_state = *(void **)Ram_EffectWork;
    render_state = *(void **)Ram_EventWork;
    zero = 0;
    event_object = *(void **)((u8 *)event_state + 0x10);
    event_value = (s32)(*(s16 *)((u8 *)(event_state) + 0x1C));
    event_index = event_value;
    Object_SetMode(event_object, 0x14);
    FIELD_AT_OFFSET(event_object, u32 *, 0x38) = (s32)*(s32 *)((u8 *)(event_object) + 8);
    FIELD_AT_OFFSET(event_object, s32 *, 0x3C) = (s32)FIELD_AT_OFFSET(event_object, s32 *, 0xC);
    FIELD_AT_OFFSET(event_object, s32 *, 0x40) = (s32)FIELD_AT_OFFSET(event_object, s32 *, 0x10);
    FIELD_AT_OFFSET(event_object, u32 *, 0x24) = 0;
    FIELD_AT_OFFSET(event_object, u32 *, 0x28) = 0;
    *(s32 *)((u8 *)(event_object) + 0x2C) = 0;
    if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x22) != 0) {
        Audio_PlayCue(212);
        FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_1;
    }
    if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x23) != 0) {
        ObjectGroup_SetActionForOthers(event_object, 1, 0);
        UiWork_PushValueSlotFar((s32)event_index, 4);
        if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x21) != 0) {
            UiText_ShowPositionedMessageAndWaitFar((void *)MsgAbilityAnnounce, (s32)*(s8 *)((u8 *)(event_state) + 0x71C));
        } else {
            UiText_ShowPositionedMessageAndWaitFar((void *)MsgAbilityAnnounce, (s32)*(s8 *)((u8 *)(event_state) + 0x71C));
        }
        ObjectGroup_SetActionForOthers(event_object, 0, 0x10);
    }
    if (GameFlag_TestFar(0x140) != 0) {
        if ((s8)FIELD_AT_OFFSET(event_state, s8 *, 0x22) != 0) {
            FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_2;
        }
        Object_SetMode(event_object, 0x15);
    } else {
        EffectRuntime_StopCurrentObject();
    }
    FIELD_AT_OFFSET(render_state, s8 *, 0xCC7) = 1;
}

void BattleEffect_InitializeSharedScene(void)
{
    u8 *scene;
    u8 *work;
    s32 no;

    scene = Data_03001ebc.scene;
    work = Data_03001ebc.work;
    Dma_Set(work + 0x1340, scene + 0x776, 0x84000150, (volatile u32 *)0x040000d4);
    if (*(s16 *)(scene + 0xcb8) == 0)
        Dma_Set(work + 0xe00, scene + 0x236, 0x84000150, (volatile u32 *)0x040000d4);
    Dma_Set(work + 0xe00, work + 0x380, 0x840002a0, (volatile u32 *)0x040000d4);

    no = Data_03001e40 & 7;
    if (GameFlag_TestFar(0x148)) no = 0;
    if (GameFlag_TestFar(0x149)) no = 1;
    if (GameFlag_TestFar(0x14a)) no = 2;
    if (GameFlag_TestFar(0x14b)) no = 3;
    if (GameFlag_TestFar(0x14c)) no = 4;
    if (GameFlag_TestFar(0x14d)) no = 5;
    if (GameFlag_TestFar(0x14e)) no = 6;
    if (GameFlag_TestFar(0x14f)) no = 7;
    BattleFx_ApplyColorToTargetBuffer(Data_080a0108[no], 1);
    BattleFx_StartBufferInterpolation(8);
}

void BattleFx_PrepareBufferInterpolation(void)
{
    s32 value;
    u8 *state;

    state = *(u8 **)((u32)&Data_03001ebc);
    value = (s32)(state + 0x236);
    BattleFx_ApplyColorToTargetBuffer(value, 2);
    if (FIELD_AT_OFFSET(state, s16 *, 0xCB8) != 0) {
        BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
    } else {
        BattleFx_ApplyColorToTargetBuffer(value, 1);
    }
    BattleFx_StartBufferInterpolation(8);
}

/* Camera: turn a world position into screen coordinates in place. In the
   projected camera mode (3) the point goes through the projection; otherwise
   it is taken relative to the camera's whole-unit x and z, with height folded
   into z. Either way the height is cleared. */
void Camera_WorldToScreen(struct EffectVector *position)
{
    u8 **data = &Data_03001ebc;

    if (*(s16 *)(*data + 0x19e) == 3) {
        struct EffectVector result;

        Render_ProjectPoint(position, &result);
        position->x = result.x << 16;
        position->z = result.y << 16;
        position->y = 0;
    } else {
        u8 *state = *(u8 **)((u8 *)data - 76);
        s32 x = *(s32 *)(state + 228) & 0xffff0000;
        s32 z = *(s32 *)(state + 232) & 0xffff0000;

        position->x -= x;
        position->z -= position->y;
        position->z -= z;
        position->y = 0;
    }
}

/* Allocates and clears the 0x298-byte transition work, tints the target
   buffer with the current hue, records the origin and starts the
   transition task. */
void RunSceneTransitionEffect(s32 x, s32 y)
{
    u8 *work;
    volatile u32 zero;

    work = Runtime_AllocateBlock(22, 0x298);
    BattleEffect_InitializeSharedScene();
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x850000a6, (volatile u32 *)0x040000d4);
    *(u16 *)(work + 0x28e) = Math_ModU(*(s32 *)gFrameCount << 1, 360);
    BattleFx_AdvanceHueCycle();
    BattleFx_ApplyColorToTargetBuffer(((s8)work[0x28d] << 10) | ((s8)work[0x28c] << 5) |
                                      (s8)work[0x28b] | 0x200000, 1);
    BattleFx_StartBufferInterpolation(8);
    *(u16 *)(work + 0x290) = x;
    *(u16 *)(work + 0x292) = y;
    work[0x294] = 8;
    Ui_FillBank15PaletteGrey();
    Scheduler_AddOrUpdateCallback((void (*)(void))Func_08097644, 0xc80);
}

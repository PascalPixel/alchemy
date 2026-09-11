#include "scene.h"
#include "battle_effect_runtime.h"
#include "types.h"
#include "global_cells.h"
#include "event_runtime.h"
#include "object_lookup.h"

/* battle/effects/fx_apply_color_to_target_buffer.c */
/* battle/effects/interpolate/apply_color_to_target_buffer.c */
void BattleFx_ApplyColorToTargetBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = gIw;

    if (buffers != NULL) {
        Battle_SetMode(value, (s32)buffers, (s32)buffers->buffer_e00, mode);
    }
}

/* battle/effects/interpolate/apply_color_to_source_buffer.c */
void BattleFx_ApplyColorToSourceBuffer(s32 value, s32 mode)
{
    struct BattleEffectBuffers *buffers = gIw;

    if (buffers != NULL) {
        Battle_SetMode(value, (s32)buffers, (s32)buffers->buffer_380, mode);
    }
}

/* battle/effects/buffers/set_primary_value.c */
void BattleFx_SetPrimaryBufferValue(unsigned int value)
{
  s16 *primary_buffer;
  primary_buffer = *((s16 **)ADDR_03001ED0);
  if (0 != primary_buffer)
  {
    *primary_buffer = value;
  }
}

/* battle/effects/interpolate/start_buffer.c */
void BattleFx_InterpolateBuffers(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3);

void BattleFx_StartBufferInterpolation(s32 mode)
{
    struct BattleEffectBuffers *buffers = gIw;

    if (buffers != NULL) {
        buffers->mode_2a01 = mode;
        buffers->unknown_2a02 = 0;
        BattleFx_InterpolateBuffers((s16 *)buffers->buffer_380,
                                        (s16 *)buffers->buffer_e00,
                                        (s16 *)buffers->buffer_1880,
                                        mode);
    }
}

/* battle/effects/color/clamp_rgb555_channel.c */
s32 BattleFx_ClampRgb555Channel(s32 value)
{
    if (value > 31)
        return 31;
    if (value < 0)
        value = 0;
    return value;
}

/* battle/effects/color/clamp_rgb555_component.c */
s32 BattleFx_ClampRgb555Component(s32 value)
{
    if (value > 31744)
        value = 31744;
    return value;
}

/* object/effects/enable_effect_spawn_callback.c */
/* object/effects/enable_spawn_callback.c */
void Object_EnableEffectSpawnCallback(void)
{
    Obj_Check((u32)Obj_Run);
}

/* object/effects/disable_spawn_callback.c */
void Object_DisableEffectSpawnCallback(void)
{
    Obj_Check((u32)Obj_Run);
}

/* battle/fx_find_definition.c */
/* battle/effects/runtime/misc/find_definition.c */
extern struct BattleEffectEntry gRom[];

struct BattleEffectEntry *BattleFx_FindDefinition(u32 id)
{
    struct BattleEffectEntry *entry = gRom;
    u32 index = 0;

    if (entry->id != id) {
        do {
            index += 1;
            entry++;
            if (index > 0x81)
                break;
        } while (entry->id != id);
    }
    return entry;
}

/* battle/effects/runtime/misc/get_animation_value.c */
s32 BattleFx_GetAnimationValue(void)
{
    struct BattleRenderObject *object = ObjectTable_Get();

    if (object->kind != 1 ||
        object->animation == NULL ||
        object->animation->value_28 == NULL) {
        return 0;
    }
    return *object->animation->value_28;
}

/* battle/effects/runtime/misc/get_resource_id.c */
struct BattleEffectEntry *BattleFx_FindDefinition(u32 id);

s32 BattleFx_GetResourceId(u32 id)
{
    u8 value;

    if (gCell.enabled_20a == 0 ||
        (value = BattleFx_FindDefinition(id)->value) == 0xFF) {
        return 0;
    }
    return value + 0x100;
}

/* battle/effects/runtime/misc/get_flags.c */
struct BattleEffectEntry *BattleFx_FindDefinition(u32 id);
s32 BattleFx_GetAnimationValue(void);

u8 BattleFx_GetFlags(void)
{
    return BattleFx_FindDefinition(BattleFx_GetAnimationValue())->flags;
}

/* battle/runtime/update_mode_from_shoulder_buttons.c */
extern volatile u8 gIwFxFindDefinition;
extern volatile s32 gIw2;

void Battle_UpdateModeFromShoulderButtons(void)
{
    struct BattleRuntime *runtime = gWork;

    /*
     * 0x03001c94 is the button latch. 0x200 is L and 0x100 is R in the GBA
     * key layout, so the two arms are the shoulder buttons setting the mode
     * word either way round.
     */
    if (gIwFxFindDefinition != 0) {
        if (gIw2 & 0x200) {
            runtime->mode_1cc = 0;
        }
        if (gIw2 & 0x100) {
            runtime->mode_1cc = -1;
        }
    }
}

/* battle/runtime/wait_if_mode_zero.c */
void WaitFrames(void);

void Battle_WaitMode0(s32 should_wait)
{
    if (gWork->mode_1cc == 0 && should_wait != 0) {
        WaitFrames();
    }
}

/* battle/runtime/initialize_render_object.c */
s32 Object_SetMode(s32, s32);

void Battle_InitializeRenderObject(void)
{
    struct BattleRenderObject *object;

    object = ObjectTable_Get(gCell.object_id);
    object->unknown_30 = 0x10000;
    object->unknown_34 = 0x8000;
    object->unknown_38 = 0x80000000;
    object->unknown_40 = 0x80000000;
    object->unknown_24 = 0;
    object->unknown_2c = 0;
    if (gCell.mode_1f2 == 1) {
        Object_SetMode((s32)object, 0xC);
        return;
    }
    Object_SetMode((s32)object, 1);
}

/* battle/runtime/reset.c */
void Battle_InitializeRenderObject(void);

void ScheduleCallbackAfterFrames(const void *, s32);
void Battle_UpdateModeFromShoulderButtons(void);
u32 GameFlag_Clear(s32);

void Battle_Reset(void)
{
    struct BattleRuntime *runtime = gWork;

    Battle_Run();
    Battle_InitializeRenderObject();
    if (runtime->unknown_cb6 != 0) {
        Battle_ResetEffectCounter();
    }
    {
        s32 zero = 0;
        runtime->unknown_cc2 = zero;
        runtime->unknown_cc4 = zero;
        runtime->unknown_1c8 = 0x10;
        runtime->mode_1cc = zero;
        runtime->unknown_1da = 0xFFFF;
        runtime->unknown_1dc = -1;
        runtime->unknown_1de = -1;
        ScheduleCallbackAfterFrames((const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
        GameFlag_Clear(0x132);
        runtime->object_id = gCell.object_id;
        runtime->unknown_1f8 = zero;
    }
}

/* battle/effects/misc/finish_action.c */
extern struct EventValueWork gCell;

void ScheduleCallback(u32);

void Battle_UpdateModeFromShoulderButtons(void);

void BattleFx_FinishAction(void)
{
    ScheduleCallback((u32)Battle_UpdateModeFromShoulderButtons);
    Battle_Apply(gCell.value, 1);
    Battle_Run();
}

/* event/event_no_op_hook.c */
/* event/no_op_hook.c */
void Event_NoOpHook(void)
{
    /* Event hook intentionally left empty. */
}

/* event/run_object_hook_and_wait.c */
extern struct EventValueWork gCell;

void WaitFrames(s32);

void Event_RunObjectHookAndWait(s32 object_id)
{
    Sys_Run();
    Sys_Do(object_id);
    WaitFrames(1);
    ObjectTable_Get(gCell.value);
}

/* event/call_with_last_active_object_id.c */
s32 ObjectTable_FindLastActiveId(void);

void Event_CallWithLastActiveObjectId(s32 event_id)
{
    Sys_Apply(event_id, ObjectTable_FindLastActiveId());
}

/* event/prepare_object_and_apply_value.c */
s32 Runtime_GetObject(void);

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
    Runtime_GetObject();
    Sys_Check(event_id);
    if (value != 0) {
        Sys_Apply(event_id, value);
    }
}

/* event/prepare_two_objects_and_apply.c */
void Event_PrepareTwoObjectsAndApply(void *first, void *second)
{
    Sys_Check(first);
    Sys_Check(second);
    Sys_Apply(first, second);
}

/* event/validate_packed_id.c */
s32 GameFlag_IsSet(s32 group);

s32 Event_ValidatePackedId(u32 packed_id)
{
    s32 group = (packed_id >> 10) & 0xf;
    s32 entry = packed_id & 0x3ff;

    if (group > 7)
        return -1;
    if (GameFlag_IsSet(group) == 0)
        return -2;
    if (Sys_Apply(group, entry) == 0)
        return -3;
    return 0;
}

/* event/clear_invalid_packed_values.c */
extern struct EventValidationWork gCell;

s32 Event_ValidatePackedId(u32 packed_id);

void Event_ClearInvalidPackedValues(void)
{
    if (Event_ValidatePackedId(gCell.values[0]) != 0)
        gCell.values[0] = 0;
    if (Event_ValidatePackedId(gCell.values[1]) != 0)
        gCell.values[1] = 0;
}

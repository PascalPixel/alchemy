#include "scene.h"
#include "battle_effect_runtime.h"
#include "event_runtime.h"

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
extern volatile u8 gIw;
extern volatile s32 gIw2;

void Battle_UpdateModeFromShoulderButtons(void)
{
    struct BattleRuntime *runtime = gWork;

    /*
     * 0x03001c94 is the button latch. 0x200 is L and 0x100 is R in the GBA
     * key layout, so the two arms are the shoulder buttons setting the mode
     * word either way round.
     */
    if (gIw != 0) {
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
        Battle_unk2_4();
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

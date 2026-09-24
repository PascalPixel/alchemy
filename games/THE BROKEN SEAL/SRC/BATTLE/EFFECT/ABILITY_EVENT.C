#include "TYPES.H"
#include "EFFECT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#define PARTY_STATE gGameState

u8 *BattleAction_Get(s32);
void *ObjectTable_Get(u32);

struct BattleEffectEventRecord {
    s32 flags;
    u16 metadata;
    s16 action_id;
    u8 unknown_08[4];
};

struct BattleEffectCharacter {
    u8 unknown_00[12];
    u8 group;
};

struct BattleEffectRuntime {
    u8 unknown_00[16];
    struct BattleEffectEventRecord *events;
};

struct BattleEffectValueRecord {
    u8 unknown_00[6];
    u16 value;
};

s32 BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result)
{
    struct BattleEffectRuntime *runtime =
        (struct BattleEffectRuntime *)gEventWork;
    struct BattleEffectEventRecord *event = runtime->events;
    s32 reference = ((struct BattleEffectValueRecord *)ObjectTable_Get(
        PARTY_STATE.object_id))->value;
    s32 selected = BattleEffect_SelectNearbyTargetObject(PARTY_STATE.object_id, group);
    s32 alternate;
    s32 ignore_flags = 0;

    *(s32 *)result = selected;
    alternate = GetFocusedObjectCollision();
    if (requested_flags == 0x70000005)
        ignore_flags = 1;

    while (event->flags != -1) {
        s32 high_value = (s16)event->metadata & 0xf000;
        s16 has_reference = event->metadata & 0x0800;
        s32 low_value = event->metadata & 0xff;

        if ((event->flags & 0x0f) == 5 &&
            GameFlag_IsConditionActive(event->action_id) != 0 &&
            (has_reference == 0 ||
             (u16)(high_value - reference + 0x17ff) <= 0x2ffe) &&
            ((struct BattleEffectCharacter *)(void *)BattleAction_Get(
                ((u8 *)&event->flags)[1]))->group == group &&
            (ignore_flags ||
             (event->flags & 0x7000000f) == requested_flags)) {
            if ((event->flags & 0x80) != 0)
                return (s32)event;
            if ((event->flags & 0x10) != 0) {
                if (low_value == selected)
                    return (s32)event;
            } else if (low_value == alternate) {
                return (s32)event;
            }
        }
        event++;
    }
    return 0;
}

/* gGameState is struct BattleWork (battle_effect_runtime.h); its
 * object_id field sits at the same 0x1f4 offset this owner reads as
 * selected_object, matching main:0808e23c's use of the same shared symbol. */

u8 *Ability_GetData(s32);
void Func_08096fb0(s32, s32);
void BattleFx_SetupObjectPair(s32, s32);
s32 BattleFx_RunEventAction(void *, s32, s32);
void FieldEvent_RunTypeHandler(void);
void EffectRuntime_StopCurrentObject(void);
void BattleEffect_CleanupSceneObjects(void);

s32 BattleFx_ExecutePackedAbilityEffect(s32 packed)
{
    s32 output;
    s32 index;
    s32 mode;
    u8 object;
    void *first;
    void *second;

    index = packed & 0x3FF;
    mode = ((u32)packed >> 10) & 0xF;
    object = Ability_GetData(index)[0xC];
    ObjectTable_Get(PARTY_STATE.object_id);
    first = (void *)BattleFx_FindMatchingEvent(0x30000005, object, &output);
    second = (void *)BattleFx_FindMatchingEvent(0x20000005, object, &output);
    Func_08096fb0(index, 0);
    BattleFx_SetupObjectPair(PARTY_STATE.object_id, output);
    BattleFx_RunEventAction(first, mode, output);
    FieldEvent_RunTypeHandler();
    EffectRuntime_StopCurrentObject();
    BattleFx_RunEventAction(second, mode, output);
    BattleEffect_CleanupSceneObjects();
    return 0;
}

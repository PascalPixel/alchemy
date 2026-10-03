#include "BATTLE_EFFECT_RUNTIME.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

s32 BattleEffect_SelectNearbyTargetObject(s32 obj_id, s32 battle_mode);
s32 BattleFx_FindDescriptorWithOverride(s32 obj_id);
void BattleFx_MarkChildAndRunFallbackTransition(s32 obj_id);
void BattleEffect_RunFallbackObjectTransition(void);
void FunctionHead_08097c3c(s32);
void BattleEffect_RunTargetedItemBreak(s32);
void BattleFx_SteerLiftedTarget(s32);
void BattleFx_CallEffect04(s32);
void BattleFx_CallEffect05(s32);
void BattleFx_StartOrbitingParticles(s32);
void BattleFx_RunBurstParticleMainObject(s32);
void BattleFx_CallEffect03AndStop(s32);
void BattleFx_CallEffect14(s32);
void BattleFx_RunEffect13Hook(s32);
void BattleFx_FinishSceneAndReleaseHeapBlock(void);

/* battle/effects/run/run_effect.c */



extern struct BattleFxScene *gEffectWork;
void BattleFx_RunItemBreakSequence(void);
void RunSceneTransitionEffect(s32 source_id, s32 target_id);
void RunBattleEffect03(void);
void RunBattleEffect04(void);
void RunBattleEffect05(void);
void BattleFx_RunOrbitingParticles(void);
void RunBattleEffect07(void);
void RunBattleEffect08(void);
void BattleFx_RunFlashingCallbackSequence(void);
void RunBattleEffect11(void);
void BattleFx_RunBurstParticles(void);
void RunBattleEffect13(void);
void RunBattleEffect14(void);
void BattleFx_RunEffect15(void);
void RunBattleEffect16(void);
void BattleFx_ResumeObject(s32 obj_id);
s32 BattleFx_FilterObjectIdByFlags(s32 obj_id);
void BattleFx_SetupObjectPair(s32 selected_object, s32 obj_id);
void BattleEffect_PauseObject(s32 obj_id);
void ResetSceneTransitionEffect(void);

extern u8 Data_03001f30[];
void MapEvent_RunTileTriggerSequence(void);
void FieldEvent_ShowStatusMessage(void);

void Event_SetValue1d8(s32);
void BattleEv_RunWait(s32, s32);
void BattleFx_FinishAction();
void _call_via_r3(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(s32, s32);
s32 GameFlag_TestFar(s32);
void Battle_Reset();
extern char MsgNothingHappens;

/* object/group/ObjectGroup_ApplyRandomChildValues.c */
extern volatile s32 gFrameCount;

/* object/motion/pos/Motion_SetTargetPositionFromMagnitudeAngle.c */

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);

void BattleFx_Run(void)
{
    struct BattleFxScene *request;
    struct BattleRuntime *battle;
    s32 battle_mode;
    s32 target_id;
    s32 obj_id;

    request = gEffectWork;
    battle = (struct BattleRuntime *)((union HeapState *)gWorkSlot)->slots[27];
    battle_mode = request->animation;
    target_id = request->second_object_id;

    switch (battle_mode) {
    case 1:
        BattleFx_RunItemBreakSequence();
        return;
    case 7:
        RunBattleEffect07();
        return;
    case 11:
        RunBattleEffect11();
        return;
    case 4:
        RunBattleEffect04();
        return;
    case 5:
        RunBattleEffect05();
        return;
    case 14:
        RunBattleEffect14();
        return;
    case 6:
        BattleFx_RunOrbitingParticles();
        return;
    case 3:
        RunBattleEffect03();
        return;
    case 12:
        BattleFx_RunBurstParticles();
        return;
    case 13:
        RunBattleEffect13();
        return;
    case 9:
        if (gGameState.unknown_24a != -1) {
            BattleFx_ResumeObject(gGameState.unknown_24a);
            gGameState.unknown_24a = -1;
        }

        obj_id = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, battle_mode);
        obj_id = BattleFx_FilterObjectIdByFlags(obj_id);
        if (BattleFx_FindDescriptorWithOverride(obj_id)!= 0) {
            BattleFx_SetupObjectPair(gGameState.selected_actor, obj_id);
            BattleFx_MarkChildAndRunFallbackTransition(obj_id);
            BattleEffect_PauseObject(obj_id);
            gGameState.unknown_24a = obj_id;
        } else {
            BattleEffect_RunFallbackObjectTransition();
        }
        return;
    case 2:
        if (*(s16 *)&battle->unknown_cb8[0] != 0)
            ResetSceneTransitionEffect();
        RunSceneTransitionEffect(request->first_object_id, target_id);
        return;
    case 8:
        RunBattleEffect08();
        return;
    case 10:
        BattleFx_RunFlashingCallbackSequence();
        return;
    case 15:
        BattleFx_RunEffect15();
        return;
    case 16:
        RunBattleEffect16();
        return;
    }
}

/* battle/effects/set/dispatch_request_kind.c */
void BattleFx_DispatchRequestKind(void)
{
    struct BattleFxScene *request = gEffectWork;
    struct BattleRuntime *battle = (struct BattleRuntime *)((union HeapState *)gWorkSlot)->slots[27];
    s32 battle_mode = request->animation;
    s32 target_id = request->second_object_id;

    request->enabled = 0;
    switch (battle_mode) {
    case 2:
        if (*(s16 *)&battle->unknown_cb8[0] != 0)
            ResetSceneTransitionEffect();
        if (gGameState.unknown_24a != request->second_object_id)
            ((struct ObjectRuntime *)request->child)->movement_state = 1;
        RunSceneTransitionEffect(request->first_object_id, target_id);
        break;
    case 1:
        FunctionHead_08097c3c(target_id);
        break;
    case 7:
        BattleEffect_RunTargetedItemBreak(target_id);
        break;
    case 11:
        BattleFx_SteerLiftedTarget(target_id);
        break;
    case 4:
        BattleFx_CallEffect04(target_id);
        break;
    case 5:
        BattleFx_CallEffect05(target_id);
        break;
    case 6:
        BattleFx_StartOrbitingParticles(target_id);
        break;
    case 12:
        BattleFx_RunBurstParticleMainObject(target_id);
        break;
    case 9:
        if (gGameState.unknown_24a != -1) {
            BattleFx_ResumeObject(gGameState.unknown_24a);
            gGameState.unknown_24a = -1;
        }
        BattleEffect_PauseObject(target_id);
        gGameState.unknown_24a = target_id;
        BattleFx_MarkChildAndRunFallbackTransition(target_id);
        break;
    case 3:
        BattleFx_CallEffect03AndStop(target_id);
        break;
    case 14:
        BattleFx_CallEffect14(target_id);
        break;
    case 13:
        BattleFx_RunEffect13Hook(target_id);
        break;
    case 8:
        RunBattleEffect08();
        break;
    case 10:
        BattleFx_RunFlashingCallbackSequence();
        break;
    case 15:
        BattleFx_RunEffect15();
        break;
    case 16:
        RunBattleEffect16();
        break;
    }
}

/* battle/effects/misc/clear_child_value_on_mismatch.c */
void BattleFx_ClearChildValueOnMismatch(void)
{
    struct BattleFxScene *request = gEffectWork;

    if (request->animation == 2) {
        BattleFx_FinishSceneAndReleaseHeapBlock();
        if (gGameState.unknown_24a != request->second_object_id) {
            ((struct ObjectRuntime *)request->child)->movement_state = 0;
        }
    }
}

void FieldEvent_RunTypeHandler(void)
{
    u32 type;

    type = (s16)FIELD_AT_OFFSET(*(void **)((u32)&Data_03001f30), s16 *, 0x1E);
    switch (type) {
    case 8:
        ResetSceneTransitionEffect();
        return;
    case 10:
        MapEvent_RunTileTriggerSequence();
        return;
    case 16:
        FieldEvent_ShowStatusMessage();
        return;
    }
}

/* battle/effects/run/run_event_action.c */
/*
 * _call_via_r3 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it. The callee is whatever
 * BattleFx_FinishAction returned, not a fixed address.
 */
s32 BattleFx_RunEventAction(void *arg0, s32 arg1, s32 arg2)
{
    s32 resource;

    if (arg0 != NULL) {
        resource = FIELD_AT_OFFSET(arg0, s32 *, 8);
        if (resource != 0) {
            if (resource < 0x10000) {
                Battle_Reset();
                Event_SetValue1d8(FIELD_AT_OFFSET(arg0, s32 *, 8));
                BattleEv_RunWait(arg2, 0);
                BattleFx_FinishAction();
            } else {
                _call_via_r3(arg1, arg2);
            }
        }
        if (GameFlag_TestFar(0x142) != 0) {
            Battle_Reset();
            UiText_ShowPositionedMessageAndWaitFar((s32)&MsgNothingHappens, 1);
            BattleFx_FinishAction();
        }
    }
    return 0;
}

/* object/ObjectGroup_ApplyRandomChildValues.c */
void ObjectGroup_ApplyRandomChildValues(void *owner)
{
    s32 state;
    void *target;
    s32 initial_count;
    s32 count;
    void **entry;
    void *current;
    volatile s32 *global;
    s32 value;

    state = ((struct ObjectRuntime *)owner)->animation_kind;
    /* 有効な所有物へ共有値を6で割った余りを配る。 */
    if (state == 1) {
        target = ((struct ObjectRuntime *)owner)->animation;
        if (target != 0 && (((struct AnimationObject *)target)->display_flags & state) == 0) {
            initial_count = ((struct AnimationObject *)target)->count;
            if (initial_count != 0) {
                global = &gFrameCount;
                entry = (void **)((struct AnimationObject *)target)->entries;
                count = initial_count;
                do {
                    current = *entry++;
                    value = (u32)*global % 6;
                    count--;
                    ((struct AnimationEntry *)current)->param = value;
                } while (count != 0);
            }
            ((struct AnimationObject *)target)->dirty = 1;
        }
    }
}

void Motion_SetTargetPositionFromMagnitudeAngle(
    struct ObjectRuntime *object, s32 magnitude, s32 angle)
{
    s32 values[3];

    /* 座標3成分を一括変換して書き戻す。 */
    if (object != 0) {
        values[0] = object->x;
        values[1] = object->y;
        values[2] = object->z;
        Vector_AddPolarOffset(magnitude, angle, values);
        Object_SetPosition(object, values[0], values[1], values[2]);
    }
}

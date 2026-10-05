#include "PROTO.H"
#include "BATTLE_EFFECT_RUNTIME.H"
#include "HEAP_STATE.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_RUNTIME.H"
#include "text/MSG_IDS.H"

#define BattleFx_MarkChildAndRunFallbackTransition ((void (*)(s32))BattleFx_MarkChildAndRunFallbackTransition)
#define BattleEffect_RunTargetedItemBreak ((void (*)(s32))BattleEffect_RunTargetedItemBreak)
#define BattleFx_CallEffect04 ((void (*)(s32))BattleFx_CallEffect04)
#define BattleFx_CallEffect05 ((void (*)(s32))BattleFx_CallEffect05)
#define BattleFx_StartOrbitingParticles ((void (*)(s32))BattleFx_StartOrbitingParticles)
#define BattleFx_RunBurstParticleMainObject ((void (*)(s32))BattleFx_RunBurstParticleMainObject)
#define BattleFx_CallEffect03AndStop ((void (*)(s32))BattleFx_CallEffect03AndStop)
#define BattleFx_RunEffect13Hook ((void (*)(s32))BattleFx_RunEffect13Hook)
#define BattleFx_ResumeObject ((void (*)(s32))BattleFx_ResumeObject)

void BattleEffect_RunFallbackObjectTransition(void);
void BattleFx_CallEffect14(s32);

extern struct BattleFxScene *gEffectWork;
void RunSceneTransitionEffect(s32 source_id, s32 target_id);
void RunBattleEffect04(void);
void RunBattleEffect14(void);
void BattleFx_SetupObjectPair(s32 selected_object, s32 obj_id);
void BattleEffect_PauseObject(s32 obj_id);


void Event_SetMessageId(s32);
void BattleFx_FinishAction();
void _call_via_r3(s32, s32);
void UiText_ShowPositionedMessageAndWaitFar(s32, s32);
void Battle_Reset();

extern volatile s32 gFrameCount;

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetPosition(struct ObjectRuntime *, s32, s32, s32);

void BattleFx_Run(void)
{
    struct BattleFxScene *request;
    struct BattleRuntime *battle;
    s32 battle_mode;
    s32 target_id;
    s32 obj_id;

    request = *(gWorkSlot + 56);
    battle = *(gWorkSlot + HEAP_SLOT_EVENT);
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
        if (gGameState.paused_object != -1) {
            BattleFx_ResumeObject(gGameState.paused_object);
            gGameState.paused_object = -1;
        }

        obj_id = BattleEffect_SelectNearbyTargetObject(gGameState.selected_actor, battle_mode);
        obj_id = BattleFx_FilterObjectIdByFlags(obj_id);
        if (BattleFx_FindDescriptorWithOverride(obj_id)!= 0) {
            BattleFx_SetupObjectPair(gGameState.selected_actor, obj_id);
            BattleFx_MarkChildAndRunFallbackTransition(obj_id);
            BattleEffect_PauseObject(obj_id);
            gGameState.paused_object = obj_id;
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

void BattleFx_DispatchRequestKind(void)
{
    struct BattleFxScene *request = *(gWorkSlot + 56);
    struct BattleRuntime *battle = *(gWorkSlot + HEAP_SLOT_EVENT);
    s32 battle_mode = request->animation;
    s32 target_id = request->second_object_id;

    request->enabled = 0;
    switch (battle_mode) {
    case 2:
        if (*(s16 *)&battle->unknown_cb8[0] != 0)
            ResetSceneTransitionEffect();
        if (gGameState.paused_object != request->second_object_id)
            ((struct ObjectRuntime *)request->child)->movement_state = 1;
        RunSceneTransitionEffect(request->first_object_id, target_id);
        break;
    case 1:
        BattleFx_SteerPushedTarget(target_id);
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
        if (gGameState.paused_object != -1) {
            BattleFx_ResumeObject(gGameState.paused_object);
            gGameState.paused_object = -1;
        }
        BattleEffect_PauseObject(target_id);
        gGameState.paused_object = target_id;
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

void BattleFx_ClearChildValueOnMismatch(void)
{
    struct BattleFxScene *request = gEffectWork;

    if (request->animation == 2) {
        BattleFx_FinishSceneAndReleaseHeapBlock();
        if (gGameState.paused_object != request->second_object_id) {
            ((struct ObjectRuntime *)request->child)->movement_state = 0;
        }
    }
}

void FieldEvent_RunTypeHandler(void)
{
    u32 type;

    type = gEffectWork->animation;
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

s32 BattleFx_RunEventAction(void *arg0, s32 arg1, s32 arg2)
{
    s32 resource;

    if (arg0 != NULL) {
        resource = FIELD_AT_OFFSET(arg0, s32 *, 8);
        if (resource != 0) {
            if (resource < 0x10000) {
                Battle_Reset();
                Event_SetMessageId(FIELD_AT_OFFSET(arg0, s32 *, 8));
                BattleEv_RunWait(arg2, 0);
                BattleFx_FinishAction();
            } else {
                _call_via_r3(arg1, arg2);
            }
        }
        if (GameFlag_TestFar(0x142) != 0) {
            Battle_Reset();
            UiText_ShowPositionedMessageAndWaitFar((s32)MsgNothingHappens, 1);
            BattleFx_FinishAction();
        }
    }
    return 0;
}

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

    if (object != 0) {
        values[0] = object->x;
        values[1] = object->y;
        values[2] = object->z;
        Vector_AddPolarOffset(magnitude, angle, values);
        Object_SetPosition(object, values[0], values[1], values[2]);
    }
}

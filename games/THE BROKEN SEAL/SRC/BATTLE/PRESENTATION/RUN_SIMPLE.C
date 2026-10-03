#include "TYPES.H"
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "BATTLE_MOTION.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_RUNTIME.H"
#include "SYSTEM.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PRESENTATION.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
s32 ResourceMetadata_SumCommandLengthsFar(s32 battle_value, s32 second, s32 third);
void BattlePres_SetActorModes(u16 *actors, s32 mode);
void BattleMotion_ResetObjectAtScaledAnchor(s32 id);
void BattleEv_DispatchQueued(void);

extern s32 *gTransitionWork;

s32 ArcTan2(s32 first, s32 second);

void ObjectDispatch_ApplyValueToChildrenFar(struct MotionObject *object, s32 action);

void Actor_ResetMotionAtAnchor(s32 id);
void BattleFx_DispatchByIdRangeFar(struct BattlePresentationWork *work);
void BattleFx_DispatchModeFar(struct BattlePresentationWork *work);
u32 Battle_GetEntryField2HighBits(u32 value);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
s32 BattleActor_SpawnObjectsForList(void *list, s32 mode);
void BattlePres_SetupTransitionAtPairMidpoint(s32 first, s32 second, s32 mode);
void BattlePres_SetupTransitionScene(s32 x, s32 depth, s32 y, s32 mode);
void BattleParty_ListAllUnitsAndSubmit(void);
void BattleEvent_Playback(void);

/* Play an actor's action against its first target: turn to face it, walk
   up, run the action's effect and walk back. */
s32 RunBattlePresentation(struct BattlePlan *plan, s32 mode)
{
    struct BattlePresentationWork work;
    struct MotionObject *object;
    struct AnimationObject *animation;
    struct BattleUnit *unit;
    s16 position[3];
    s32 scripted;
    s32 *facing;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 x;
    s32 z;
    s32 divisor;
    s32 direct;
    s32 index;
    s32 phase;

    /* FAKEMATCH: r0 forgets the plan, so the first read reloads it */
    asm volatile("" : : : "r0");
    facing = gTransitionWork;
    object = GetBattleObjectSlot(plan->actor_id)->object;
    z = object->z;
    x = object->x;
    angle = (u16)ArcTan2(x, z);
    adjusted = angle - 0x2000;
    if (plan->actor_id > 7)
        adjusted = angle + 0x6000;
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (*facing == facing_angle) {
        *facing = facing_angle;
        WaitFrames(5);
    } else {
        *facing = facing_angle;
        WaitFrames(10);
    }

    BattlePres_SetActorModes(0, 0);
    BattlePres_BuildTargetList(plan, &work);
    if (work.kind == 0x87)
        UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode & ~1);

    unit = Owner_GetStateFar(work.actor);
    Owner_GetStateFar(work.actors[0]);
    scripted = plan->target_modifiers[0];
    direct = plan->target_adjustments[0] == 0;

    animation = GetMotionRecord(
        GetBattleObjectSlot(plan->actor_id)->object, 0);
    divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
    BattleMotion_ApproachTarget(
        work.actor,
        work.actors[0],
        divisor,
        Battle_GetEntryField2HighBits(unit->class_id) << 16);
    ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(work.actor)->object, 16);
    GetBattleObjectSlot(work.actors[0]);

    if ((u16)work.actors[0] <= 7)
        work.side = 1;
    else
        work.side = 0;

    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    *(volatile u16 *)0x04000000 |= 0x6000;

    if (direct != 0) {
        WaitFrames(10);
        BattleMotion_ResetObjectAtScaledAnchor(work.actors[0]);
        WaitFrames(2);
        WaitFrames(4);
        WaitFrames(10);
        BattleEv_Push(0, plan->target_ids[0]);
        BattleEv_Push(4, (s32)&MsgNimblyDodgesBlow);
        BattleEv_DispatchQueued();
        Actor_ResetMotionAtAnchor(work.actors[0]);
    } else {
        phase = 0;
        work.flags = 0;
        if (plan->presentation_flags != 0)
            work.flags = 1;

        if (scripted != 0) {
            work.kind += 200;
            phase = 1;
            facing[5] = 1;
            position[0] = work.actor;
            position[1] = work.first_target;
            position[2] = 0xff;
            BattleActor_SpawnObjectsForList(position, 0);
        }

        divisor -= 8;
        if (divisor <= 0)
            divisor = 1;
        {
            s32 first;
            s32 second;

            for (index = 0; index != divisor; index++) {
                if (phase != 0) {
                    first = work.actor;
                    second = work.first_target;
                    BattlePres_SetupTransitionAtPairMidpoint(
                        first,
                        second,
                        index * 30 / divisor + 100);
                }
                WaitFrames(1);
            }
        }

        Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xc80);
        if (work.kind != 0) {
            if (plan->presentation_flags & 0x4000)
                BattleFx_DispatchByIdRangeFar(&work);
            else
                BattleFx_DispatchModeFar(&work);
        }
        BattleEventRuntime_WaitForReady();
        if (scripted != 0) {
            facing[5] = 0;
            BattleParty_ListAllUnitsAndSubmit();
            BattlePres_SetupTransitionScene(0, 0, 0, 100);
        }
        Actor_ResetMotionAtAnchor(work.actors[0]);
    }
    Actor_ResetMotionAtAnchor(work.actor);
    return 0;
}

s32 BattlePres_RunSimple(struct BattlePlan *input, s32 flags)
{
    struct BattlePresentationWork work;
    struct BattlePlan *saved_input;
    struct MotionObject *object;
    struct AnimationObject *animation;
    s32 *facing;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 x;
    s32 z;
    s32 divisor;
    s32 scripted;

    facing = gTransitionWork;
    saved_input = input;
    object = GetBattleObjectSlot(saved_input->actor_id)->object;
    z = object->z;
    x = object->x;
    angle = (u16)ArcTan2(x, z);
    adjusted = angle - 0x2000;
    if (saved_input->actor_id > 7)
        adjusted = angle + 0x6000;
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (*facing == facing_angle) {
        *facing = facing_angle;
        WaitFrames(5);
    } else {
        *facing = facing_angle;
        WaitFrames(20);
    }

    BattlePres_SetActorModes(0, 0);
    BattlePres_BuildTargetList(saved_input, &work);
    Owner_GetStateFar(work.actor);
    Owner_GetStateFar(saved_input->target_ids[0]);

    scripted = flags & 2;
    animation = GetMotionRecord(
        GetBattleObjectSlot(saved_input->actor_id)->object, 0);
    divisor = ResourceMetadata_SumCommandLengthsFar(animation->entries[0]->anim_id, 2, 1);
    BattleMotion_ApproachTarget(
        work.actor,
        saved_input->target_ids[0],
        divisor,
        0);
    ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(work.actor)->object, 16);
    GetBattleObjectSlot(saved_input->target_ids[0]);

    if (saved_input->target_ids[0] <= 7)
        work.side = 1;
    else
        work.side = 0;
    if (scripted != 0) {
        WaitFrames(10);
        BattleMotion_ResetObjectAtScaledAnchor(saved_input->target_ids[0]);
        WaitFrames(2);
        WaitFrames(4);
        WaitFrames(10);
        Actor_ResetMotionAtAnchor(saved_input->target_ids[0]);
    } else {
        BattleFx_DispatchByIdRangeFar(&work);
        BattleEv_DispatchQueued();
        Actor_ResetMotionAtAnchor(saved_input->target_ids[0]);
    }
    Actor_ResetMotionAtAnchor(work.actor);
    return 0;
}

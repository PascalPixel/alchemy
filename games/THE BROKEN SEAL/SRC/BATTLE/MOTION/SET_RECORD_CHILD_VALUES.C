#include "GLOBAL.H"
#include "PROTO.H"
#include "TYPES.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "OBJDISP.H"
#include "CALLBACK_SCHEDULER.H"
#include "FIXED_MATH.H"
#include "BATTLE_MSG.H"
#include "FIELD_EVENT.H"
#include "BATTLE_ACTOR.H"
#include "MENU_LIST.H"

s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value)
{
    s32 index = 0;
    s32 child_index;
    s32 count;
    struct AnimationObject *record;
    struct AnimationEntry *child;
    struct AnimationEntry **children;
    /* FAKEMATCH: cached compound assignment keeps 94 bytes but changes registers and store order. The cached byte read preserves allocation. */
    u8 mask = 0xff;
    u8 frame;

    while ((record = GetMotionRecord(object, index)) != NULL) {
        children = &record->entries[1];
        child = record->entries[0];
        frame = child->frame;
        child->frame = frame | mask;
        count = record->count;
        child->param = value;
        for (child_index = 1; child_index < count; child_index++) {
            child = *children++;
            child->param = 0;
            child->frame = 0xff;
        }
        index++;
    }
}

u16 ArcTan2(s32 x, s32 y);
void ObjectDispatch_ApplyValueToChildrenFar(struct DispatchObject *object, s32 value);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void BattleFx_PlayUnitElementEffect(s32 actor, s32 kind, s32 mode, s32 variant);
void BattleFx_DispatchByIdRangeFar(s32 *work);
void BattleFx_DispatchModeFar(s32 *work);

s32 BattlePres_RunPsynergyCast(struct BattlePlan *plan, s32 mode)
{
    struct BattlePresentationWork work;
    struct BattlePresentationTransition *transition = gTransitionWork;
    struct MotionObject *object;
    s32 scripted;
    s32 i;

    if (plan->presentation_flags & 0x40000) {
        transition->target_yaw = plan->actor_id <= 7 ? -0x2000 : 0x5000;
        transition->frames = 60;
    } else {
        struct MotionObject *actor = GetBattleObjectSlot(plan->actor_id)->object;
        s32 angle = ArcTan2(actor->x, actor->z);
        s32 current;

        if (plan->actor_id <= 7)
            current = angle - 0x1800;
        else
            current = angle + 0x1800;
        current = (s16)current;
        current += ((plan->actor_id <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
        if (!((plan->actor_id <= 7) ^ (plan->target_ids[0] <= 7)))
            current = plan->actor_id <= 7 ? 0x2400 : -0x2400;
        if (transition->target_yaw != current)
            transition->target_yaw = current;
    }
    if (plan->presentation_flags & 0x80000) {
        transition->target_yaw = plan->actor_id <= 7 ? -0x2000 : 0x2000;
        transition->frames = 60;
    }

    BattlePres_BuildTargetList(plan, &work);
    scripted = mode & 1;
    if (scripted)
        work.flags = 1;
    BattlePres_SetActorModes(0, 0);
    UiWindow_DrawPartyStatusContentsFar(gBattleWork->party_status_mode & ~1);
    object = GetBattleObjectSlot(work.actor)->object;
    Object_SetMode(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
    AudioCommand_PlayFar(0x9a);
    if (mode & 2)
        BattleFx_PlayUnitElementEffect(work.actor, plan->range_index, 1, 0);
    else if (!scripted)
        BattleFx_PlayUnitElementEffect(work.actor, plan->range_index, 0, 0);
    if (plan->target_ids[0] <= 7)
        work.side = 1;
    else
        work.side = 0;

    for (i = 0; i != work.count; i++) {
        struct AnimationObject *animation = GetMotionRecord(
            GetBattleObjectSlot(work.actors[i])->object, 0);
        s32 j;

        for (j = 0; j != animation->count - 1; j++)
            work.params[i][j] = animation->entries[j]->param;
    }
    if (plan->failure != 0) {
        if (plan->failure == 1) {
            BattleEv_Push(0, plan->actor_id);
            BattleEv_Push(4, (s32)MsgPsynergyBlocked);
        } else {
            BattleEv_Push(4, (s32)MsgCastLacksPp);
        }
        BattleEv_DispatchQueued();
        BattlePres_RunWithZeroArguments();
    } else {
        Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
        if (work.kind) {
            if (plan->presentation_flags & 0x4000)
                BattleFx_DispatchByIdRangeFar((s32 *)&work);
            else
                BattleFx_DispatchModeFar((s32 *)&work);
        } else {
            BattlePres_RunWithZeroArguments();
        }
        BattleEventRuntime_WaitForReady();
        Object_SetMode(object, 1);
        for (i = 0; i != work.count; i++)
            Actor_ResetMotionAtAnchor(work.actors[i]);
    }
    return 0;
}

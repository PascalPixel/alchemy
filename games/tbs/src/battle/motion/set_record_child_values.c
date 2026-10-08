#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "motobj.h"
#include "animspr.h"
#include "btlev.h"
#include "btlwork.h"
#include "btldisp.h"
#include "objdisp.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "btlmes.h"
#include "field_event.h"
#include "btlobj.h"
#include "menu_list.h"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

s32 BattleMotion_SetRecordChildValues(struct MotionObject *object, s32 value)
{
    s32 index = 0;
    s32 child_index;
    s32 count;
    struct AnimationObject *record;
    struct AnimationEntry *child;
    struct AnimationEntry **children;
    /* FAKEMATCH: needed to match: cached compound assignment keeps 94 bytes but changes registers and store order */
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

s32 BattlePres_RunPsynergyCast(struct BtlPlan *plan, s32 mode)
{
    struct BattlePresentationWork work;
    struct BattlePresentationTransition *transition = gTransitionWork;
    struct MotionObject *object;
    s32 scripted;
    s32 i;

    if (plan->presentation_flags & 0x40000) {
        transition->target_yaw = plan->aid <= 7 ? -0x2000 : 0x5000;
        transition->frames = 60;
    } else {
        struct MotionObject *actor = BtlObjGet(plan->aid)->object;
        s32 angle = ArcTan2(actor->x, actor->z);
        s32 current;

        if (plan->aid <= 7)
            current = angle - 0x1800;
        else
            current = angle + 0x1800;
        current = (s16)current;
        current += ((plan->aid <= 7 ? 0x2000 : -0x2000) - current) * 3 / 4;
        if (!((plan->aid <= 7) ^ (plan->tgt_no[0] <= 7)))
            current = plan->aid <= 7 ? 0x2400 : -0x2400;
        if (transition->target_yaw != current)
            transition->target_yaw = current;
    }
    if (plan->presentation_flags & 0x80000) {
        transition->target_yaw = plan->aid <= 7 ? -0x2000 : 0x2000;
        transition->frames = 60;
    }

    BattlePres_BuildTargetList(plan, &work);
    scripted = mode & 1;
    if (scripted)
        work.flags = 1;
    BattlePres_SetActorModes(0, 0);
    PtyStDrawFar(BtlWk->pty_st & ~1);
    object = BtlObjGet(work.actor)->object;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
    ObjectDispatch_ApplyValueToChildrenFar((struct DispatchObject *)object, 16);
    AudioCommand_PlayFar(0x9a);
    if (mode & 2)
        BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 1, 0);
    else if (!scripted)
        BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 0, 0);
    if (plan->tgt_no[0] <= 7)
        work.side = 1;
    else
        work.side = 0;

    for (i = 0; i != work.count; i++) {
        struct AnimationObject *animation = GetMotionRecord(
            BtlObjGet(work.actors[i])->object, 0);
        s32 j;

        for (j = 0; j != animation->count - 1; j++)
            work.params[i][j] = animation->entries[j]->param;
    }
    if (plan->failure != 0) {
        if (plan->failure == 1) {
            BtlEvPush(0, plan->aid);
            BtlEvPush(4, (s32)MsgPsynergyBlocked);
        } else {
            BtlEvPush(4, (s32)MsgCastLacksPp);
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
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
        for (i = 0; i != work.count; i++)
            ObjMotRst(work.actors[i]);
    }
    return 0;
}

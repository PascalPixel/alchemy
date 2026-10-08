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
#include "heap_state.h"
#include "scene.h"
#include "iwram_call.h"
#include "battle_command.h"
#include "btlsys.h"
#include "btlpty.h"
#include "btltype.h"
#include "btlmes.h"
#include "btldisp.h"
#include "system.h"
#include "btlobj.h"
#include "io_reg.h"
#include "btlwork.h"
#include "motobj.h"
#include "animspr.h"
#include "callback_scheduler.h"
#include "transform.h"

#define Camera_StoreSceneParameters ((void (*)(s32,u32,s32))Camera_StoreSceneParameters)
#define BattleUnit_ProcessTurnEnd ((void (*)(struct BtlPlan*))BattleUnit_ProcessTurnEnd)
#define BattleFx_PlayUnitElementEffect ((s32 (*)(s32,s32,s32,s32))BattleFx_PlayUnitElementEffect)

static __inline__ s32 DivQ16(s32 divisor, s32 value)
{
    /* FAKEMATCH: needed to match: a direct call keeps the divisor in r5 for the camera call */
    return Iwram_RatioMulQ14(divisor, value);
}

s32 BattlePresentation_DispatchAction(struct BattleActionRecord *request, s32 delay)
{
    struct BattlePresentationTransition *view;
    struct BtlWork *battle;
    struct BattleCamera *camera;
    struct BtlUnit *unit;
    u16 actor[2];
    u16 ids[14];
    s32 kept;
    s32 result;
    s32 count;
    s32 i;

    kept = 0;
    if (request->unit_id == 0xff)
        return 0;
    unit = UnitGetFar(request->unit_id);
    if (unit->hp == 0)
        return -1;
    if (unit->class_index == 0)
        BattleCommand_SelectAutomatic(request, 1);
    view = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    view->frames = 60;
    battle = *(gWorkSlot + HEAP_SLOT_BATTLE);
    view->flag = 0;
    battle->brightness = 0x10000;
    camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
    Camera_StoreSceneParameters(0x01fe0000, DivQ16(0x01fe0000, 0xc000), 0x7fff0000);
    if (delay != 0) {
        view->target_yaw = 0x2000;
        WaitFrames(delay);
    }
    actor[0] = request->unit_id;
    actor[1] = 0xff;
    BattlePres_SetActorModes(actor, 1);
    result = BattleCommand_BuildPlan(request, &battle->plan);
    if (result == 0) {
        switch (battle->plan.outcome) {
        case 1:
            BattlePres_RunActorEntries(&battle->plan, 0);
            break;
        case 2:
            RunBattlePresentation(&battle->plan, 0);
            break;
        case 5:
            BattlePresentation_RunUnitTransition(&battle->plan, 1);
            break;
        case 9:
            BattlePresentation_RunUnitTransition(&battle->plan, 0);
            break;
        case 3:
            BattlePres_RunPsynergyCast(&battle->plan, 0);
            break;
        case 6:
            BattlePres_RunPsynergyCast(&battle->plan, 1);
            break;
        case 8:
            BattlePres_RunPsynergyCast(&battle->plan, 2);
            break;
        case 4:
            BattlePres_RunItem(&battle->plan, 0, request);
            break;
        case 7:
            if (BattlePresentation_RunEncounterOrUnitTrigger(&battle->plan) != 0)
                kept = 1;
            if (kept != 0)
                goto finish;
            break;
        }
    } else {
        if (result == -1) {
            BattlePresentation_WaitForAdvance();
            WaitFrames(3);
        }
        BattlePres_SetActorModes(0, 0);
    }
    BattleMotion_DestroyAllSlotObjects();
    BattleUnit_ProcessTurnEnd(&battle->plan);
    BtlObjPut();
    count = BattleParty_ListActorIds(3, (u16 *)ids);
    for (i = 0; i < count; i++)
        ObjMotRst(ids[i]);
    request->unit_id = 0xff;
finish:
    BattlePresentation_ConfigurePaletteFade(2, BtlWk->background, 0);
    return kept;
}

s32 BattlePres_BuildTargetList(
    struct BtlPlan *plan, struct BattlePresentationWork *work)
{
    s16 *dst;
    u32 flags;
    s32 count;
    s32 i;
    u8 *target;

    count = 0;
    work->flags = 0;
    flags = plan->presentation_flags;
    work->kind = flags & 0xfff;
    work->variant = (flags & 0x3000) >> 12;
    work->actor = plan->aid;
    i = 0;
    if (i < plan->target_count) {
        target = plan->tgt_no;
        dst = work->actors;
        do {
            if (UnitGetFar(*target)->hp != 0 ||
                (plan->presentation_flags & 0x10000)) {
                count++;
                *dst++ = *target;
            }
            i++;
            target++;
        } while (i < plan->target_count);
    }
    if (count == 0) {
        work->actors[0] = plan->tgt_no[0];
        count = 1;
    }
    work->first_target = plan->tgt_no[0];
    work->count = count;
    work->initial_value = 1;
}

s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BtlPlan *plan)
{
    struct BattlePresentationTransition *presentation = *(gWorkSlot + HEAP_SLOT_BATTLE_BACKGROUND);
    struct BtlWork *scene = *(gWorkSlot + HEAP_SLOT_BATTLE);
    s32 completed = 0;
    s32 party_mode;

    presentation->target_yaw = 0x2000;
    presentation->active = 1;
    BattlePres_SetActorModes(0, 0);

    if (plan->aid <= 7) {
        party_mode = completed;
        /* FAKEMATCH: empty asm barrier on party_mode: cse no longer knows the copy of completed is zero, so it is adds r2, r7, #0 as native, not movs r2, #0 (1 of 108 rows differ without) */
        asm("" : "+r"(party_mode));

        if (scene->encounter_mode != 2) {
            party_mode = 1;
        }

        if (!party_mode) {
            UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
            BattlePresentation_WaitForAdvance();
        } else {
            s16 unit_ids[14];
            s32 index = BattleParty_ListLivingUnits(1, (u16 *)unit_ids) - 1;

            while (index != -1) {
                struct BtlUnit *unit = UnitGetFar(unit_ids[index]);

                if (unit->shibire == 0 && unit->nemuri == 0) {
                    BattleMotion_SetupEscapeObject(unit_ids[index]);
                    WaitFrames(8);
                }
                index--;
            }
            WaitFrames(22);
            completed = 1;
        }
    } else if (((Random16() * 10) >> 16) <= 6) {
        s16 event[2];

        event[0] = plan->aid;
        event[1] = 0xff;
        BattleMotion_SetupEscapeObject(event[0]);
        WaitFrames(8);
        BattleActor_RemoveFromLists(plan->aid);
        ActivateBattleObjectSlot(plan->aid);
    } else {
        UiText_ShowMessageAndWaitCoreFar((s32)MsgNoEscape);
        BattlePresentation_WaitForAdvance();
    }

    presentation->active = 0;
    return completed;
}

void ObjectDispatch_ApplyArgumentToChildrenFar(void *object, s32 mode);
void AudioCommand_PlayFar(s32 cue);

s32 BattlePresentation_RunUnitTransition(struct BtlPlan *plan, s32 mode)
{
    u16 units[14];
    struct BattlePresentationWork work;
    u32 actor;
    u32 target;
    u32 count;
    s32 i;
    s32 n;
    u32 kept;
    struct MotionObject *object;
    void **slots = (void **)&BtlWk;

    BattlePres_BuildTargetList(plan, &work);
    actor = plan->aid;
    target = plan->tgt_no[0];
    if (plan->presentation_flags & 0x8000) {
        struct BattlePresentationTransition *transition;
        s32 yaw;
        transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
        yaw = 0x2000;
        if (actor > 7)
            yaw = 0x5000;
        transition->target_yaw = yaw;
        transition->frames = 60;
    } else {
        struct BattlePresentationTransition *transition;
        s32 yaw;
        transition = slots[HEAP_SLOT_BATTLE_BACKGROUND - HEAP_SLOT_BATTLE];
        yaw = -0x2000;
        if (actor <= 7)
            yaw = 0x2000;
        if (transition->target_yaw != yaw)
            transition->target_yaw = yaw;
    }
    BattlePres_SetActorModes(0, 0);
    PtyStDrawFar(BtlWk->pty_st & ~1);
    object = BtlObjGet(actor)->object;
    REG_BLDCNT = 0x3f40;
    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
    for (i = 0; i != count; i++) {
        if (units[i] == BATTLE_UNIT_REMOVED)
            continue;
        if (units[i] != actor) {
            if ((target <= 7) != (units[i] <= 7))
                BattlePres_SetActorRecordMode(units[i], 1);
        } else {
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 3);
        }
    }
    AudioCommand_PlayFar(0x9a);
    BattleFx_PlayUnitElementEffect(work.actor, plan->elm, 0, 0);
    if (mode & 1)
        BattlePres_SetActorRecordMode(actor, 1);
    for (i = 0; i != 16; i++) {
        REG_BLDALPHA = (16 - i) | 0x1000;
        WaitFrames(1);
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
        kept = 0;
        for (i = 0; i < count; i++) {
            u32 unit = units[i];
            if (unit == actor) {
                if (!(mode & 1))
                    units[kept++] = actor;
            } else if ((target <= 7) == (unit <= 7)) {
                units[kept++] = unit;
            }
        }
        units[kept] = BATTLE_UNIT_LIST_END;
        BattleActor_SpawnObjectsForList((s16 *)units, 0);
        for (n = 0; n < plan->target_count; n++)
            units[n] = plan->tgt_no[n];
        units[n] = BATTLE_UNIT_LIST_END;
        for (i = 0; i != work.count; i++) {
            struct AnimationObject *animation;
            u32 child;

            animation = GetMotionRecord(BtlObjGet(work.actors[i])->object, 0);
            for (child = 0; child != animation->count - 1; child++)
                work.params[i][child] = animation->entries[child]->param;
        }
        if (plan->presentation_flags & 0x8000) {
            if (target > 7)
                work.side = 0;
            else
                work.side = 1;
        } else {
            if (plan->tgt_no[0] <= 7)
                work.side = 1;
            else
                work.side = 0;
        }
        if (plan->presentation_flags & 0x20000)
            work.side ^= 1;
        Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
        if (plan->presentation_flags & 0x8000)
            BattleFx_InitializeModeFar((s32 *)&work);
        else if (plan->presentation_flags & 0x4000)
            BattleFx_DispatchByIdRangeFar((s32 *)&work);
        else
            BattleFx_DispatchModeFar((s32 *)&work);
        BattleEventRuntime_WaitForReady();
    }
    BtlObjPut();
    count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, units);
    REG_BLDCNT = 0x3f40;
    for (i = 0; i != count; i++) {
        if (units[i] == BATTLE_UNIT_REMOVED || units[i] == actor)
            continue;
        if ((target <= 7) != (units[i] <= 7))
            BattlePres_SetActorRecordMode(units[i], 1);
    }
    for (i = 0; i != 16; i++) {
        REG_BLDALPHA = i | 0x1000;
        WaitFrames(1);
    }
    for (i = 0; i != count; i++)
        BattlePres_SetActorRecordMode(units[i], 0);
    BattlePres_SetupTransitionScene(0, 0, 0, 100);
    WaitFrames(1);
    return 0;
}

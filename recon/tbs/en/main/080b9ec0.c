/* Draft: complete native extent [080b9ec0,080ba27c), 956 bytes.
 * Earlier untyped attempts: 2026-09-24 retained 956 bytes with 437 differing
 * halfwords using Value_ symbols for pool constants; those symbols and the
 * scheduling-only wrappers are not retained. The 2026-09-30 named-cell,
 * integer-constant form was 960 bytes with 380 differing listing lines:
 * native frame 124/plan r9 versus draft frame 128/plan sl. Native reaches
 * the transition cell 140 bytes beyond the battle cell.
 * 2026-10-03 typed baseline: 970/956 bytes, frame 128/124, work at
 * sp+16/sp+12 and plan in r10/r9. Relocation-normalized full comparison
 * differs at 883 of the native 956 bytes, with 14 extra bytes; this is not
 * an aligned instruction score. All relocation symbols resolve, and all
 * 31 call targets and multiplicities agree. Explicit transition-cell loads
 * add one data relocation and pool word (18/17); instructions are 415/406.
 * Stores (20), branches including calls (91), and calls (31) are unchanged.
 * Typed owners retain signed target/count accesses, integer range_index,
 * overlapping child parameters, same-side kept units and the zero result.
 * Stopped after this one natural baseline: no device or matching-C credit.
 */
#include "TYPES.H"
#include "IO_REG.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_PRESENTATION.H"
#include "MOTION_OBJECT.H"
#include "ANIMSPR.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"

extern struct BattlePresentationTransition *gTransitionWork;
void BattleEvent_Playback(void);
void BattlePres_SetActorModes(u16 *actors, s32 mode);
void UiWindow_DrawPartyStatusContentsFar(s32 mode);
void Object_SetMode(void *object, s32 mode);
void AudioCommand_PlayFar(s32 cue);
void BattleFx_PlayUnitElementEffect(s32 unit, s32 kind, s32 mode, s32 variant);
void BattlePres_RunWithZeroArguments(void);
void BattleActor_SpawnObjectsForList(s16 *actors, s32 mode);
void BattleFx_DispatchByIdRangeFar(s32 *work);
void BattleFx_DispatchModeFar(s32 *work);

s32 BattlePresentation_RunUnitTransition(
    struct BattlePlan *plan,
    s32 mode)
{
    u16 visible_units[14];
    struct BattlePresentationWork work;
    u32 primary_unit;
    u32 opposing_unit;
    u32 visible_count;
    u32 refreshed_count;
    s32 index;
    u32 kept_count;
    struct MotionObject *object;

    BattlePres_BuildTargetList(plan, &work);
    primary_unit = plan->actor_id;
    opposing_unit = plan->target_ids[0];

    if (plan->presentation_flags & 0x8000) {
        struct BattlePresentationTransition *transition = gTransitionWork;
        transition->target_yaw = primary_unit <= 7 ? 0x2000 : 0x00005000;
        transition->frames = 60;
    } else {
        struct BattlePresentationTransition *transition = gTransitionWork;
        u32 target = primary_unit <= 7 ? 0x00002000 : 0xffffe000;
        if (transition->target_yaw != target) {
            transition->target_yaw = target;
        }
    }

    BattlePres_SetActorModes(0, 0);
    UiWindow_DrawPartyStatusContentsFar((gBattleWork->party_status_mode) & ~1);
    object = GetBattleObjectSlot(primary_unit)->object;
    REG_BLDCNT = 0x3f40;
    visible_count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, visible_units);

    for (index = 0; index < visible_count; index++) {
        u16 unit = visible_units[index];
        if (unit != BATTLE_UNIT_REMOVED) {
            if (unit == primary_unit) {
                Object_SetMode(object, 3);
            } else if ((opposing_unit <= 7) != (unit <= 7)) {
                BattlePres_SetActorRecordMode(unit, 1);
            }
        }
    }

    AudioCommand_PlayFar(0x9a);
    BattleFx_PlayUnitElementEffect(work.actor, plan->range_index, 0, 0);
    if (mode & 1) {
        BattlePres_SetActorRecordMode(primary_unit, 1);
    }

    for (index = 0; index < 16; index++) {
        REG_BLDALPHA = (16 - index) | 0x1000;
        WaitFrames(1);
    }

    if (plan->failure != 0) {
        if (plan->failure == 1) {
            BattleEv_Push(0, primary_unit);
            BattleEv_Push(4, 0x856);
        } else {
            BattleEv_Push(4, 0x855);
        }
        BattleEv_DispatchQueued();
        BattlePres_RunWithZeroArguments();
    } else {
        kept_count = 0;
        for (index = 0; index < visible_count; index++) {
            u16 unit = visible_units[index];
            if (unit == primary_unit) {
                if (!(mode & 1)) {
                    visible_units[kept_count++] = primary_unit;
                }
            } else if ((opposing_unit > 7) == (unit > 7)) {
                visible_units[kept_count++] = unit;
            }
        }
        visible_units[kept_count] = BATTLE_UNIT_LIST_END;
        BattleActor_SpawnObjectsForList((s16 *)visible_units, 0);

        for (index = 0; index < plan->target_count; index++) {
            visible_units[index] = plan->target_ids[index];
        }
        visible_units[index] = BATTLE_UNIT_LIST_END;

        {
            /* The effect uses the tail of the actor storage for four child
               parameters per target, beginning at work + 0x34. */
            u8 *params = (u8 *)&work.actors[8];

            for (index = 0; index < work.count; index++) {
                struct AnimationObject *animation;
                u32 child;
                u32 child_count;
                s32 unit = work.actors[index];

                animation = GetMotionRecord(GetBattleObjectSlot(unit)->object, 0);
                child_count = animation->count - 1;
                for (child = 0; child < child_count; child++)
                    params[index * 4 + child] = animation->entries[child]->param;
            }
        }

        if (plan->presentation_flags & 0x8000) {
            work.side = opposing_unit > 7 ? 0 : 1;
        } else {
            work.side = plan->target_ids[0] <= 7 ? 1 : 0;
        }
        if (plan->presentation_flags & 0x20000) {
            work.side ^= 1;
        }

        Scheduler_AddOrUpdateCallback((s32)BattleEvent_Playback, 0xc80);
        if (plan->presentation_flags & 0x8000) {
            BattleFx_InitializeModeFar((s32 *)&work);
        } else if (plan->presentation_flags & 0x4000) {
            BattleFx_DispatchByIdRangeFar((s32 *)&work);
        } else {
            BattleFx_DispatchModeFar((s32 *)&work);
        }
        BattleEventRuntime_WaitForReady();
    }

    BattleActor_CommitPlacement();
    refreshed_count = BattleParty_ListActorIds(BATTLE_SIDE_BOTH, visible_units);
    REG_BLDCNT = 0x00003f40;
    for (index = 0; index < refreshed_count; index++) {
        u16 unit = visible_units[index];
        if (unit != BATTLE_UNIT_REMOVED && unit != primary_unit &&
            ((opposing_unit <= 7) != (unit <= 7))) {
            BattlePres_SetActorRecordMode(unit, 1);
        }
    }
    for (index = 0; index < 16; index++) {
        REG_BLDALPHA = index | 0x1000;
        WaitFrames(1);
    }
    for (index = 0; index < refreshed_count; index++) {
        BattlePres_SetActorRecordMode(visible_units[index], 0);
    }
    BattlePres_SetupTransitionScene(0, 0, 0, 0x64);
    WaitFrames(1);
    return 0;
}

#include "TYPES.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "BATTLE_COMMAND.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PRESENTATION.H"
#include "SYSTEM.H"

void BattlePres_SetActorModes(u16 *, s32);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void BattlePresentation_WaitForAdvance(void);
void BattleMotion_SetupEscapeObject(s32 unit_id);
void BattleActor_RemoveFromLists(s32 unit_id);
void ActivateBattleObjectSlot(s32 unit_id);
extern u8 gTransitionWork[];
void BattleCommand_SelectAutomatic(struct BattleCommandRequest *request, s32 mode);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 source, s32 destination);
void Camera_StoreSceneParameters(s32, u32, s32);
void BattlePres_RunActorEntries(struct BattlePlan *plan, s32 mode);
s32 RunBattlePresentation(struct BattlePlan *plan, s32 mode);
void BattlePresentation_RunUnitTransition(struct BattlePlan *plan, s32 mode);
void Func_080ba978(struct BattlePlan *plan, s32 mode);
void Func_080ba6ac(struct BattlePlan *plan, s32 mode, struct BattleCommandRequest *request);
s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattlePlan *plan);
void BattleMotion_DestroyAllSlotObjects(void);
void BattleUnit_ProcessTurnEnd(struct BattlePlan *plan);
void BattleActor_CommitPlacement(void);
s32 BattleParty_ListActorIds(s32 mode, u16 *ids);
void Actor_ResetMotionAtAnchor(s32 id);
void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 background, s32 level);

/* Inline, so the divisor is built again for the camera call. */
static __inline__ s32 DivQ16(s32 divisor, s32 value)
{
    /* FAKEMATCH: a direct call keeps the divisor in r5 for the camera call; the reference builds it twice. */
    return Iwram_RatioMulQ14(divisor, value);
}

/* Plays one queued action: resolves it into the session plan, runs the
   presentation its outcome selects, then ends the turn. Returns 1 when the
   action stays queued, -1 when its actor is down. */
s32 BattlePresentation_DispatchAction(struct BattleCommandRequest *request, s32 delay)
{
    struct BattlePresentationTransition *view;
    struct BattleSession *battle;
    struct BattleCamera *camera;
    void **cache;
    struct BattleUnit *unit;
    u16 actor[2];
    u16 ids[14];
    s32 kept;
    s32 result;
    s32 count;
    s32 i;

    kept = 0;
    if (request->actor_id == 0xff)
        return 0;
    unit = Owner_GetStateFar(request->actor_id);
    if (unit->hp == 0)
        return -1;
    if (unit->class_index == 0)
        BattleCommand_SelectAutomatic(request, 1);
    cache = (void **)gTransitionWork;
    view = cache[0];
    view->frames = 60;
    battle = cache[9 - 44];
    view->flag = 0;
    battle->brightness = 0x10000;
    camera = cache[12 - 44];
    Render_ResetTransformState();
    Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
    Camera_StoreSceneParameters(0x01fe0000, DivQ16(0x01fe0000, 0xc000), 0x7fff0000);
    if (delay != 0) {
        view->target_yaw = 0x2000;
        WaitFrames(delay);
    }
    actor[0] = request->actor_id;
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
            Func_080ba978(&battle->plan, 0);
            break;
        case 6:
            Func_080ba978(&battle->plan, 1);
            break;
        case 8:
            Func_080ba978(&battle->plan, 2);
            break;
        case 4:
            Func_080ba6ac(&battle->plan, 0, request);
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
    BattleActor_CommitPlacement();
    count = BattleParty_ListActorIds(3, ids);
    for (i = 0; i < count; i++)
        Actor_ResetMotionAtAnchor(ids[i]);
    request->actor_id = 0xff;
finish:
    BattlePresentation_ConfigurePaletteFade(2, gBattleWork->background, 0);
    return kept;
}


s32 BattlePres_BuildTargetList(
    struct BattlePlan *plan, struct BattlePresentationWork *work)
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
    work->actor = plan->actor_id;
    i = 0;
    if (i < plan->target_count) {
        target = plan->target_ids;
        dst = work->actors;
        do {
            if (Owner_GetStateFar(*target)->hp != 0 ||
                (plan->presentation_flags & 0x10000)) {
                count++;
                *dst++ = *target;
            }
            i++;
            target++;
        } while (i < plan->target_count);
    }
    if (count == 0) {
        work->actors[0] = plan->target_ids[0];
        count = 1;
    }
    work->first_target = plan->target_ids[0];
    work->count = count;
    work->initial_value = 1;
}

s32 BattlePresentation_RunEncounterOrUnitTrigger(struct BattlePlan *plan)
{
    u8 *presentation_addr = gTransitionWork;
    void **cache = (void **)presentation_addr;
    struct BattlePresentationTransition *presentation = cache[0];
    struct BattleSession *scene = cache[9 - 44];
    s32 completed = 0;
    s32 party_mode;

    presentation->target_yaw = 0x2000;
    presentation->active = 1;
    BattlePres_SetActorModes(0, 0);

    if (plan->actor_id <= 7) {
        /* FAKEMATCH: an opaque copy keeps cse from folding party_mode to the constant zero */
        asm("mov %0, %1" : "=r"(party_mode) : "r"(completed));

        if (scene->encounter_mode != 2) {
            party_mode = 1;
        }

        if (!party_mode) {
            UiText_ShowMessageAndWaitCoreFar((s32)&MsgNoEscape);
            BattlePresentation_WaitForAdvance();
        } else {
            s16 unit_ids[14];
            s32 index = BattleParty_ListLivingUnits(1, (u16 *)unit_ids) - 1;

            while (index != -1) {
                struct BattleUnit *unit = Owner_GetStateFar(unit_ids[index]);

                if (unit->stun == 0 && unit->sleep == 0) {
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

        event[0] = plan->actor_id;
        event[1] = 0xff;
        BattleMotion_SetupEscapeObject(event[0]);
        WaitFrames(8);
        BattleActor_RemoveFromLists(plan->actor_id);
        ActivateBattleObjectSlot(plan->actor_id);
    } else {
        UiText_ShowMessageAndWaitCoreFar((s32)&MsgNoEscape);
        BattlePresentation_WaitForAdvance();
    }

    presentation->active = 0;
    return completed;
}

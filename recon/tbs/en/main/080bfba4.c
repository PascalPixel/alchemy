#include "RUNTIME_MEM.H"
/* DRAFT (score 120): 12 of 454 instructions differ, register names only. In the
 * loop that orders the recovering Djinn the ROM keeps the counter of the owner
 * and order scans in r4 and the -1 it compares the owner with in r3; here both
 * are r1, and the copy of that -1 into the order lands one instruction later.
 * The ROM's allocator ranks the counter and the order below the hoisted base
 * of the counts array; every spelling tried ranks them above it (the counters
 * shared or separate across the four loops, all 256 combinations; the order
 * shared with the gain or not; five minutes of alchemy permute).
 * Settled on the way: the three scans index the list (no entry pointer), the
 * counts are cleared by an ascending loop the compiler reverses, the first
 * scan's counter is the variable that later holds the best element, and the
 * order is the variable that later holds the gain.
 * The listing calls BattleUnit_Recalculate, Audio_PlayCue and Sys_Free
 * Owner_RecalculateStatsFar, AudioCommand_PlayFar and Runtime_BumpFree.
 * 2026-10-02: a fresh linked baseline remains 120. A 45-second search
 * (11,592 candidates) did not improve it. A void return plus an r1 clobber
 * scored 170; binding the first scan counter to r4 scored 7131; a record
 * for each element's count with the void return scored 130. All discarded.
 * The declaration's return type still needs to agree with the callers. */
#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_TYPES.H"
#include "IWRAM_CALL.H"

struct UnitBoosts {
    u8 unknown_000[0x12c];
    s8 boost[4];
};

struct BattleMotionSlot {
    void *object;
};

struct BattleState {
    u8 unknown_000[0x820];
    s32 poison_cue;
};

extern struct BattleState *gBattleWork;

/* The KO messages link as offsets from "goes down": the Grim Reaper's
   call three after it, the enemy's "strength is exhausted" three after
   that. */
#define MSG_REAPER_CALLS ((s32)&MsgGoesDown + 3)
#define MSG_EXHAUSTED ((s32)&MsgGoesDown + 6)

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void BattleUnit_Recalculate(s32 unit_id);
s32 Owner_AdjustFirstValueFar(s32 unit_id, s32 amount);
void Djinn_DeactivateFar(s32 unit_id, s32 element, s32 index);
void Sys_Free(void *block);
s32 __divsi3(s32 numerator, s32 denominator);
void Object_SetMode(void *object, s32 animation);
void ObjectDispatch_ApplyValueToChildrenFar(void *object, s32 flags);
void Audio_PlayCue(s32 cue);
struct BattleMotionSlot *GetBattleObjectSlot(s32 unit_id);
s32 BattleEventRuntime_SchedulePhase(s32 frames);
u32 BattleEventRuntime_Reset(void);
s32 BattleEventRuntime_WaitForReady(void);
s32 BattleFx_PlayUnitElementEffect(s32 unit_id, s32 element, s32 mode, s32 arg);

/* End of one unit's turn. The Djinn it summoned with join its side's
   recovery order and raise their element's level; the power each element
   gains is announced. When both sides still stand, curse, poison and the
   Grim Reaper's count then take their toll. */
s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan)
{
    s32 id;
    struct BattleUnit *unit;
    s32 both_sides;
    s32 counts[4];
    struct DjinnRecoveryList *list;
    s32 best;
    s32 order;

    id = plan->actor_id;
    both_sides = 0;
    unit = Owner_GetStateFar(id);
    list = &Trade_GetOfferStateFar((u32)id > 7)->list;
    for (best = 0; best < list->count; best++) {
        if (list->entries[best].unit_id == id && list->entries[best].turns == -1) {
            Djinn_DeactivateFar(id, list->entries[best].element, list->entries[best].index);
        }
    }
    if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) != 0
        && BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) != 0) {
        both_sides = 1;
    }

    {
        s32 i;
        s32 j;
        s32 owner;

        list = &Trade_GetOfferStateFar((u32)id > 7)->list;
        for (i = 0; i < 4; i++) {
            counts[i] = 0;
        }
        for (;;) {
            owner = -1;
            for (i = 0; i < list->count; i++) {
                if (list->entries[i].turns == -2) {
                    owner = list->entries[i].unit_id;
                    break;
                }
            }
            if (owner == -1) {
                break;
            }
            order = -1;
            for (i = 0; i < list->count; i++) {
                if (list->entries[i].unit_id == owner && list->entries[i].turns > order) {
                    order = list->entries[i].turns;
                }
            }
            order++;
            if (order <= 1) {
                order = 2;
            }
            for (j = 0; j < list->count; j++) {
                if (list->entries[j].unit_id == owner && list->entries[j].turns == -2) {
                    list->entries[j].turns = order;
                    counts[list->entries[j].element]++;
                    order++;
                }
            }
        }
    }

    if (both_sides != 0) {
        struct BattleUnit *before;
        s32 most;
        s32 i;

        most = 0;
        before = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
        Iwram_CopyWords(before, unit, sizeof(struct BattleUnit));
        best = -1;
        for (i = 0; i <= 3; i++) {
            if (counts[i] > most) {
                most = counts[i];
                best = i;
            }
        }
        if (best >= 0 && ((struct UnitBoosts *)unit)->boost[best] < most) {
            ((struct UnitBoosts *)unit)->boost[best] = most;
        }
        BattleUnit_Recalculate(id);
        for (i = 0; i <= 3; i++) {
            order = unit->elements[i].power - before->elements[i].power;
            if (order > 0) {
                BattleEventRuntime_Reset();
                BattleEventRuntime_SchedulePhase(25);
                BattleEv_Push(BATTLE_EVENT_UNIT, id);
                BattleEv_Push(BATTLE_EVENT_VALUE, order);
                BattleEv_Push(BATTLE_EVENT_SOUND, 175);
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgEarthPowerUp + i);
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, id);
                Audio_PlayCue(212);
                Object_SetMode(GetBattleObjectSlot(id)->object, 3);
                ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(id)->object, 32);
                BattleFx_PlayUnitElementEffect(id, i, 2, most - 1);
                BattleEventRuntime_WaitForReady();
            }
        }
        Sys_Free(before);
    }

    if (both_sides != 0) {
        s8 *poison;

        BattleEventRuntime_Reset();
        if (plan->pending_amount_60 != 0) {
            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, id);
            BattleEv_Push(BATTLE_EVENT_UNIT, id);
            BattleEv_Push(BATTLE_EVENT_VALUE, plan->pending_amount_60);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgCurseDamage);
            if (Owner_AdjustFirstValueFar(id, -plan->pending_amount_60) == 0) {
                s32 text;

                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, id);
                BattleEv_Push(BATTLE_EVENT_UNIT, id);
                if ((u32)id <= 7) {
                    text = (s32)&MsgGoesDown;
                } else {
                    text = MSG_EXHAUSTED;
                }
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
            } else {
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, id);
            }
        }
        BattleEv_DispatchQueued();

        BattleEventRuntime_Reset();
        poison = &unit->poison;
        if (*poison != 0) {
            s32 damage = __divsi3(unit->max_hp * *poison, 10);
            struct BattleState *state = gBattleWork;

            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, id);
            BattleEv_Push(BATTLE_EVENT_UNIT, id);
            BattleEv_Push(BATTLE_EVENT_VALUE, damage);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPoisonDamage);
            if (*poison != 0) {
                state->poison_cue = 134;
            } else {
                state->poison_cue = 133;
            }
            if (Owner_AdjustFirstValueFar(id, -damage) == 0) {
                s32 text;

                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, id);
                BattleEv_Push(BATTLE_EVENT_UNIT, id);
                if ((u32)id <= 7) {
                    text = (s32)&MsgGoesDown;
                } else {
                    text = MSG_EXHAUSTED;
                }
                BattleEv_Push(BATTLE_EVENT_TEXT, text);
            } else {
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, id);
            }
        }
        BattleEv_DispatchQueued();

        BattleEventRuntime_Reset();
        if (unit->death_count != 0) {
            if (--unit->death_count == 0
                && Owner_AdjustFirstValueFar(id, 0xc0000000) == 0) {
                BattleEv_Push(BATTLE_EVENT_UNIT, id);
                BattleEv_Push(BATTLE_EVENT_TEXT, MSG_REAPER_CALLS);
                BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, id);
                BattleEv_Push(BATTLE_EVENT_ACTOR_RESOLVE, id);
                BattleEv_Push(BATTLE_EVENT_UNIT, id);
                if ((u32)id <= 7) {
                    BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgGoesDown);
                } else {
                    BattleEv_Push(BATTLE_EVENT_TEXT, MSG_EXHAUSTED);
                }
            }
        }
        BattleEv_DispatchQueued();
    }
    BattleUnit_Recalculate(id);
}

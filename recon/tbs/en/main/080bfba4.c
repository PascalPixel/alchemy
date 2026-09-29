/* NONMATCHING: shared callee return types audited on 2026-09-26.
 * 1044 of 1044 bytes, 434 differing halfwords, 175 aligned edits.
 * Canonical declarations are retained; the remaining source model is not exact. */
/*
 * NONMATCHING: complete 1,044-byte owner including pools; prior score
 * 1,044 candidate bytes, 434 halfword edits. Count clearing keeps a counter
 * where the ROM compares the element pointer against the array base.
 * The copy call uses _call_via_r3, which is the ROM's 080072f0 veneer;
 * that earlier apparent mismatch was a naming error, not a residual.
 * The diagnostic frame is 28 bytes instead of 32. List, entry and index
 * registers also differ. Audit stopped before rewrite; no adoption.
 * 2026-09-26 bounded trials: signed address comparison while clearing from
 * counts+3 down to the array base fixes the opening r6/r7 allocation but
 * gives 1056 bytes/224 aligned edits rather than 1044/175; frame stays 28.
 * Declaring the IWRAM copier value-returning changes no bytes. A volatile
 * count pointer with phase-local base reloads gives the 32-byte frame but
 * wrong slot order, 1076 bytes and 215 edits. These three trials are not
 * kept. The missing array-base spill is not explained by array padding.
 */
#include "TYPES.H"
#include "BATTLE_EVENT.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_TYPES.H"

struct BattleMotionSlot {
    void *object;
};

struct BattleState {
    u8 unknown_000[0x820];
    s32 poison_cue;
};

extern struct BattleState *Data_03001e74;

/* The KO messages link as offsets from "goes down": the Grim Reaper's
   call three after it, the enemy's "strength is exhausted" three after
   that. */
#define MSG_REAPER_CALLS ((s32)&MsgGoesDown + 3)
#define MSG_EXHAUSTED ((s32)&MsgGoesDown + 6)

typedef void (*BlockCopy)(void *destination, const void *source, s32 size);

struct DjinnRecoveryTable *Func_08077000(s32 side);
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
void BattleUnit_Recalculate(s32 unit_id);
s32 Func_08077118(s32 unit_id, s32 amount);
void Func_080771b8(s32 unit_id, s32 element, s32 index);
void *Runtime_BumpAllocateAlternatePool(s32 size);
void Sys_Free(void *block);
s32 Math_Div(s32 numerator, s32 denominator);
void Func_08009080(void *object, s32 animation);
void ObjectDispatch_ApplyValueToChildrenFar(void *object, s32 flags);
void Func_080f9010(s32 cue);
struct BattleMotionSlot *GetBattleObjectSlot(s32 unit_id);
u32 BattleEv_DispatchQueued(void);
void BattleEventRuntime_SchedulePhase(s32 phase);
u32 BattleEventRuntime_Reset(void);
void BattleEventRuntime_WaitForReady(void);
s32 Func_080c1798(s32 unit_id, s32 element, s32 mode, s32 arg);

/* End of one unit's turn. The Djinn it summoned with join its side's
   recovery order and raise their element's level; the power each element
   gains is announced. When both sides still stand, curse, poison and the
   Grim Reaper's count then take their toll. */
s32 BattleUnit_ProcessTurnEnd(struct BattlePlan *plan)
{
    s32 id;
    s32 both_sides;
    struct BattleUnit *unit;
    struct DjinnRecoveryList *list;
    struct DjinnRecoveryEntry *entry;
    s32 i;
    s32 counts[4];
    s32 *count;
    s32 owner;
    s32 order;

    id = plan->actor_id;
    both_sides = 0;
    unit = Owner_GetStateFar(id);
    list = &Func_08077000((u32)id > 7)->list;
    i = 0;
    if (i < list->count) {
        entry = list->entries;
        do {
            if (entry->unit_id == id && entry->turns == -1) {
                Func_080771b8(id, entry->element, entry->index);
            }
            i++;
            entry++;
        } while (i < list->count);
    }
    if (BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, 0) != 0
        && BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, 0) != 0) {
        both_sides = 1;
    }

    list = &Func_08077000((u32)id > 7)->list;
    count = counts;
    for (i = 3; i >= 0; i--) {
        count[i] = 0;
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
        if (list->count > 0) {
            s32 n = list->count;

            entry = list->entries;
            do {
                if (entry->unit_id == owner && entry->turns > order) {
                    order = entry->turns;
                }
                n--;
                entry++;
            } while (n != 0);
        }
        order++;
        if (order <= 1) {
            order = 2;
        }
        for (i = 0; i < list->count; i++) {
            entry = &list->entries[i];
            if (entry->unit_id == owner && entry->turns == -2) {
                entry->turns = order;
                count[entry->element]++;
                order++;
            }
        }
    }

    if (both_sides != 0) {
        struct BattleUnit *before;
        s32 best;
        s32 most;
        s32 gain;

        most = 0;
        before = Runtime_BumpAllocateAlternatePool(sizeof(struct BattleUnit));
        ((BlockCopy)0x03001388)(before, unit, sizeof(struct BattleUnit));
        best = -1;
        for (i = 0; i <= 3; i++) {
            if (count[i] > most) {
                most = count[i];
                best = i;
            }
        }
        if (best >= 0 && (&((s8 *)&unit->status_12c)[best])[0] < most) {
            ((s8 *)&unit->status_12c)[best] = most;
        }
        BattleUnit_Recalculate(id);
        for (i = 0; i <= 3; i++) {
            gain = unit->elements[i].power - before->elements[i].power;
            if (gain > 0) {
                BattleEventRuntime_Reset();
                BattleEventRuntime_SchedulePhase(25);
                BattleEv_Push(BATTLE_EVENT_UNIT, id);
                BattleEv_Push(BATTLE_EVENT_VALUE, gain);
                BattleEv_Push(BATTLE_EVENT_SOUND, 175);
                BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgEarthPowerUp + i);
                BattleEv_Push(BATTLE_EVENT_ACTOR_FINISH, id);
                Func_080f9010(212);
                Func_08009080(GetBattleObjectSlot(id)->object, 3);
                ObjectDispatch_ApplyValueToChildrenFar(GetBattleObjectSlot(id)->object, 32);
                Func_080c1798(id, i, 2, most - 1);
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
            if (Func_08077118(id, -plan->pending_amount_60) == 0) {
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
            s32 damage = Math_Div(*poison * unit->max_hp, 10);
            struct BattleState *state = Data_03001e74;

            BattleEv_Push(BATTLE_EVENT_ACTOR_BEGIN, id);
            BattleEv_Push(BATTLE_EVENT_UNIT, id);
            BattleEv_Push(BATTLE_EVENT_VALUE, damage);
            BattleEv_Push(BATTLE_EVENT_TEXT, (s32)&MsgPoisonDamage);
            if (*poison != 0) {
                state->poison_cue = 134;
            } else {
                state->poison_cue = 133;
            }
            if (Func_08077118(id, -damage) == 0) {
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
                && Func_08077118(id, 0xc0000000) == 0) {
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

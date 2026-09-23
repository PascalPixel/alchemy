/*
 * Draft, 1,044 of 1,044 bytes, 434 halfword edits. Structure and calls
 * follow the reference; open: the count-clearing loop keeps its counter
 * where the reference compares the element pointer against the array base,
 * and the list, entry and index registers are allocated differently.
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
#define MSG_REAPER_CALLS (MSG_GOES_DOWN + 3)
#define MSG_EXHAUSTED (MSG_GOES_DOWN + 6)

typedef void (*BlockCopy)(void *destination, const void *source, s32 size);

struct DjinnRecoveryTable *Func_08077000(s32 side);
struct BattleUnit *Func_08077008(s32 unit_id);
void Func_08077010(s32 unit_id);
s32 Func_08077118(s32 unit_id, s32 amount);
void Func_080771b8(s32 unit_id, s32 element, s32 index);
void *Func_08004970(s32 size);
void Func_08002df0(void *block);
s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_08009080(void *object, s32 animation);
void Func_08009088(void *object, s32 flags);
void Func_080f9010(s32 cue);
struct BattleMotionSlot *Func_080b7dd0(s32 unit_id);
void Func_080bb938(void);
void Func_080bd808(s32 phase);
u32 Func_080bdfec(void);
void Func_080be02c(void);
s32 Func_080c1798(s32 unit_id, s32 element, s32 mode, s32 arg);

#define BattleUnit_ProcessTurnEnd Func_080bfba4

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
    unit = Func_08077008(id);
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
        before = Func_08004970(sizeof(struct BattleUnit));
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
        Func_08077010(id);
        for (i = 0; i <= 3; i++) {
            gain = unit->elements[i].power - before->elements[i].power;
            if (gain > 0) {
                Func_080bdfec();
                Func_080bd808(25);
                BattleEvent_Push(BATTLE_EVENT_UNIT, id);
                BattleEvent_Push(BATTLE_EVENT_VALUE, gain);
                BattleEvent_Push(BATTLE_EVENT_SOUND, 175);
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_EARTH_POWER_UP + i);
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, id);
                Func_080f9010(212);
                Func_08009080(Func_080b7dd0(id)->object, 3);
                Func_08009088(Func_080b7dd0(id)->object, 32);
                Func_080c1798(id, i, 2, most - 1);
                Func_080be02c();
            }
        }
        Func_08002df0(before);
    }

    if (both_sides != 0) {
        s8 *poison;

        Func_080bdfec();
        if (plan->pending_amount_60 != 0) {
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, plan->pending_amount_60);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURSE_DAMAGE);
            if (Func_08077118(id, -plan->pending_amount_60) == 0) {
                s32 text;

                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, id);
                if ((u32)id <= 7) {
                    text = MSG_GOES_DOWN;
                } else {
                    text = MSG_EXHAUSTED;
                }
                BattleEvent_Push(BATTLE_EVENT_TEXT, text);
            } else {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, id);
            }
        }
        Func_080bb938();

        Func_080bdfec();
        poison = &unit->poison;
        if (*poison != 0) {
            s32 damage = Func_080022ec(*poison * unit->max_hp, 10);
            struct BattleState *state = Data_03001e74;

            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, damage);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_POISON_DAMAGE);
            if (*poison != 0) {
                state->poison_cue = 134;
            } else {
                state->poison_cue = 133;
            }
            if (Func_08077118(id, -damage) == 0) {
                s32 text;

                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, id);
                if ((u32)id <= 7) {
                    text = MSG_GOES_DOWN;
                } else {
                    text = MSG_EXHAUSTED;
                }
                BattleEvent_Push(BATTLE_EVENT_TEXT, text);
            } else {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, id);
            }
        }
        Func_080bb938();

        Func_080bdfec();
        if (unit->death_count != 0) {
            if (--unit->death_count == 0
                && Func_08077118(id, 0xc0000000) == 0) {
                BattleEvent_Push(BATTLE_EVENT_UNIT, id);
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REAPER_CALLS);
                BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, id);
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, id);
                if ((u32)id <= 7) {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_GOES_DOWN);
                } else {
                    BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_EXHAUSTED);
                }
            }
        }
        Func_080bb938();
    }
    Func_08077010(id);
}

#include "TYPES.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_TYPES.H"

struct BattleMotionSlot {
    void *object;
};

struct BattleState {
    u8 unknown_00[0x44];
    u8 two_sided;
    u8 unknown_45[0x0b];
    u8 flag_50;
    u8 unknown_51[0x5f7];
    u16 palette_648;
};

extern struct BattleState *Data_03001e74;

/* The two party groups in the order their round-end effects resolve. */
struct BattleGroupOrder {
    s32 group[2];
};

extern const struct BattleGroupOrder Data_080c35bc;

struct DjinnRecoveryTable *Func_08077000(s32 side);
struct BattleUnit *Func_08077008(s32 unit_id);
void Func_08077010(s32 unit_id);
s32 Func_08077118(s32 unit_id, s32 amount);
s32 Func_08077120(s32 unit_id, s32 amount);
s32 Func_080771b0(u8 unit_id, u8 element, u8 index);
void Func_080771c0(u8 unit_id, u8 element, u8 index);
void Func_08009080(void *object, s32 animation);
void Func_08009088(void *object, s32 flags);
void Func_08015120(s32 value, s32 slot);
void Func_080151c8(s32 message_id);
void Func_080f9010(s32 cue);
s32 Func_080b6c08(s32 group, u16 *ids);
void Func_080b78e4(s32 unit_id, void *slot);
void Func_080b7aac(s32 unit_id);
struct BattleMotionSlot *Func_080b7dd0(s32 unit_id);
void Func_080bb65c(void);
s32 Func_080bbabc(u32 opcode, u32 operand);
void Func_080bd808(s32 phase);
u32 Func_080bdfec(void);
void Func_080be02c(void);
s32 Func_080bf250(s32 unit_id);
s32 Func_080bf2b4(s32 unit_id);
s32 Func_080bf318(s32 unit_id);
s32 Func_080bf37c(s32 unit_id);
s32 Func_080bf3bc(s32 unit_id);
s32 Func_080bf400(s32 unit_id);
s32 Func_080bf440(s32 unit_id);
s32 Func_080bf484(s32 unit_id);
s32 Func_080bf4c4(s32 unit_id);
s32 Func_080bf524(s32 unit_id);
s32 Func_080bf54c(s32 unit_id);
s32 Func_080bf574(s32 unit_id);
void Func_080c0774(s32 mode, s32 value, s32 arg);
s32 Func_080c1798(s32 unit_id, s32 element, s32 mode, s32 arg);

/* Round-end processing. Each side's set-aside Djinn count down and return
   to their owners; then every listed unit ticks its counters, regenerates
   HP and PP and recovers from expiring conditions, each with its message.
   Declared int like its callers expect, though it returns nothing. */
s32 Battle_ProcessRoundEnd(void)
{
    struct BattleState *work;
    s32 side;
    s32 sides;
    s32 count;
    s32 i;
    struct DjinnRecoveryList *list;
    struct DjinnRecoveryEntry *timed_entry;
    struct DjinnRecoveryEntry *expired_entry;

    work = Data_03001e74;
    sides = (work->two_sided != 0) + 1;
    for (side = 0; side < sides; side++) {
        list = &Func_08077000(side)->list;
        i = 0;
        if (i < list->count) {
            timed_entry = list->entries;
            do {
                if (timed_entry->turns > 0 && Func_080b7dd0(timed_entry->unit_id) != 0
                    && Func_08077008(timed_entry->unit_id)->hp != 0) {
                    timed_entry->turns--;
                }
                i++;
                timed_entry++;
            } while (i < list->count);
        }
        i = 0;
        if (i < list->count) {
            expired_entry = list->entries;
            do {
                if (expired_entry->turns == 0) {
                    u8 id = expired_entry->unit_id;
                    s32 element;

                    if (Func_080b7dd0(id) != 0) {
                        Func_080bdfec();
                        Func_080bd808(30);
                        Func_080bbabc(0, id);
                        Func_080bbabc(3, expired_entry->element * 20 + expired_entry->index + 300);
                        Func_080bbabc(14, 175);
                        Func_080bbabc(10, 0);
                        Func_080bbabc(4, MSG_DJINN_SET);
                        Func_080bbabc(11, id);
                        Func_080f9010(212);
                        Func_08009080(Func_080b7dd0(id)->object, 3);
                        Func_08009088(Func_080b7dd0(id)->object, 32);
                        element = expired_entry->element;
                        Func_080771b0(id, element, expired_entry->index);
                        Func_080771c0(id, expired_entry->element, expired_entry->index);
                        Func_08077010(id);
                        Func_080c1798(id, element, 3, 0);
                        Func_080be02c();
                    }
                } else {
                    expired_entry++;
                    i++;
                }
            } while (i < list->count);
        }
    }
    Func_080c0774(2, Data_03001e74->palette_648, 0);
    {
    s32 *group_list;
    struct BattleGroupOrder order = Data_080c35bc;
    u16 ids[14];
    s32 group;
    s32 j;

    if (work->two_sided != 0 && work->flag_50 != 0) {
        order.group[0] = 2;
        order.group[1] = 1;
    }
    group_list = order.group;
    for (group = 0; group <= 1; group++) {
        count = Func_080b6c08(group_list[group], ids);
        for (j = 0; j < count; j++) {
            s32 hp_amount;
            s32 pp_amount;
            s32 id = ids[j];
            struct BattleUnit *unit = Func_08077008(id);

            if (unit->ready_pose != 0) {
                unit->ready_pose--;
            }
            if (unit->hp != 0) {
                if (unit->hp_regen != 0 && unit->hp != unit->max_hp) {
                    hp_amount = unit->hp_regen;
                    if (unit->hp + hp_amount > unit->max_hp) {
                        hp_amount = unit->max_hp - unit->hp;
                    }
                    Func_08077118(id, hp_amount);
                    Func_08015120(id, 1);
                    Func_08015120(hp_amount, 5);
                    if (unit->hp == unit->max_hp) {
                        Func_080151c8(MSG_HP_FULL);
                    } else {
                        Func_080151c8(MSG_HP_RECOVER);
                    }
                    Func_080f9010(175);
                    Func_080bb65c();
                }
                if (unit->pp_regen != 0 && unit->pp != unit->max_pp) {
                    pp_amount = unit->pp_regen;
                    if (unit->pp + pp_amount > unit->max_pp) {
                        pp_amount = unit->max_pp - unit->pp;
                    }
                    Func_08077120(id, pp_amount);
                    Func_08015120(id, 1);
                    Func_08015120(pp_amount, 5);
                    if (unit->pp == unit->max_pp) {
                        Func_080151c8(MSG_PP_FULL);
                    } else {
                        Func_080151c8(MSG_PP_RECOVER);
                    }
                    Func_080f9010(175);
                    Func_080bb65c();
                }
            }
            if (Func_080bf574(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_AGI_NORMAL);
                Func_080bb65c();
            }
            if (Func_080bf250(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_ATK_NORMAL);
                Func_080bb65c();
            }
            if (Func_080bf2b4(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_DEF_NORMAL);
                Func_080bb65c();
            }
            if (Func_080bf318(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_RES_NORMAL);
                Func_080bb65c();
            }
            if (Func_080bf37c(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_CURE_DELUSION);
                Func_080bb65c();
            }
            if (Func_080bf3bc(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_CONSCIOUS);
                Func_080bb65c();
            }
            if (Func_080bf400(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_CONSCIOUS_2);
                Func_080bb65c();
            }
            if (Func_080bf440(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080b7aac(id);
                Func_080151c8(MSG_CURE_STUN);
                Func_080bb65c();
            }
            if (Func_080bf484(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080b7aac(id);
                Func_080151c8(MSG_WAKES);
                Func_080bb65c();
            }
            if (Func_080bf4c4(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_CURE_SEAL);
                Func_080bb65c();
            }
            if (Func_080bf524(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_REFRAIN_END);
                Func_080bb65c();
            }
            if (Func_080bf54c(id) != 0) {
                Func_080b78e4(id, Func_080b7dd0(id));
                Func_08015120(id, 1);
                Func_080151c8(MSG_REFLECT_END);
                Func_080bb65c();
            }
        }
    }
    }
}

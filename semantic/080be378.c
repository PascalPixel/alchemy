#include "types.h"

/*
 * Battle command resolver.
 *
 * The manifest advertises only the 1,012-byte prologue row at 080be378, but
 * the owner keeps one live 48-byte frame and its r8..r11 lifetimes across six
 * manifest rows.  The executable rows are
 *
 *     080be378 1012  prologue, 100-entry command jump table, literal pool
 *     080be76c  816  commands 0..4
 *     080beb08 1104  command 5, command 6, shared tail at 080bee00
 *     080bef88  608  per-action post-processing and the sole epilogue
 *
 * with the two remaining rows, 080bea9c (108 bytes) and 080bef58 (48 bytes),
 * being nothing but the literal pools of the rows in front of them.  A third
 * pool for the last row lives at 080bf1e8.
 *
 * The original dispatches the command through a jump table and reaches its one
 * epilogue at 080bf1d4/080bf1d6 by branching (and, where the displacement did
 * not fit, by `bl`) into it from every row.  Those inter-row edges are ordinary
 * control flow here, not calls.
 */

/* ------------------------------------------------------------------ */
/* Types                                                              */
/* ------------------------------------------------------------------ */

/* The caller's request block; r10 points at the slot that holds it. */
struct BattleRequest_080be378 {
    s16 actor;        /* 0x00 unit handle for every helper below           */
    s16 unknown_02;
    s16 unknown_04;
    s16 command;      /* 0x06 jump-table selector, 0..99                   */
    s16 param;        /* 0x08 command operand: ability, slot or packed pos */
    s16 unknown_0a;   /* 0x0a passed to Func_080b9a44                      */
};

/* The plan block the caller receives; this function fills it in. */
struct BattlePlan_080be378 {
    u8 actor;             /* 0x00 low byte of request->actor            */
    s8 member_count;      /* 0x01 number of participating members       */
    u8 members[28];       /* 0x02 member unit handles                   */
    u8 delays[14];        /* 0x1e per-member delay adjustment           */
    s8 effects[14];       /* 0x2c per-member effect, cleared to 0       */
    s8 results[14];       /* 0x3a per-member result, preset to -1       */
    u16 command;          /* 0x48 copy of request->command              */
    u8 padding_4a[2];
    u32 action_id;        /* 0x4c resolved action                       */
    u32 action_kind;      /* 0x50 ability kind / initiative class       */
    u32 status;           /* 0x54 outcome code consumed by the caller   */
    u32 flags;            /* 0x58 presentation flags                    */
    u32 failure;          /* 0x5c non-zero when the command was refused */
    u32 unknown_60;       /* 0x60 cleared on entry                      */
};

/*
 * The unit record returned by Func_08077008.  Only the fields this owner
 * touches are named; the rest is padding sized from the raw displacements.
 */
struct BattleUnit_080be378 {
    u8 unknown_000[54];
    s16 pp_max;             /* 0x036 */
    s16 hp;                 /* 0x038 */
    s16 pp;                 /* 0x03a */
    u8 unknown_03c[156];
    u16 abilities[40];      /* 0x0d8 equipped ability ids                */
    u8 class_index;         /* 0x128 */
    u8 djinn_locked;        /* 0x129 non-zero suppresses the class bonus */
    u8 unknown_12a;
    u8 warned_no_target;    /* 0x12b sticky "cannot act" warning latch   */
    u8 unknown_12c[4];
    u8 restraint;           /* 0x130 bit 0 blocks non-defend commands    */
    u8 unknown_131[7];
    u8 delay_penalty;       /* 0x138 */
    u8 unknown_139;
    u8 auto_battle;         /* 0x13a */
    u8 stunned;             /* 0x13b */
    u8 downed;              /* 0x13c */
    u8 out_of_pp;           /* 0x13d */
    u8 unknown_13e[7];
    u8 forced_action;       /* 0x145 consumed once, then cleared         */
};

/* Static ability description returned by Func_08077080. */
struct AbilityInfo_080be378 {
    u8 target_mode;   /* 0x0 */
    u8 element;       /* 0x1 low nibble selects the message variant */
    u8 kind;          /* 0x2 */
    u8 effect;        /* 0x3 */
    u8 unknown_04[5];
    u8 pp_cost;       /* 0x9 */
};

/* Summon/roster entry returned by Func_080771e0. */
struct SummonEntry_080be378 {
    u16 action_id;    /* 0x0 */
    u16 unknown_02;
    u8 cost[4];       /* 0x4 one requirement per element */
};

/* Item/equipment record returned by Func_08077018. */
struct ItemInfo_080be378 {
    u8 unknown_00[2];
    u8 kind;          /* 0x02 */
    u8 unknown_03[9];
    u8 usability;     /* 0x0c */
    u8 unknown_0d[27];
    u16 action_id;    /* 0x28 */
};

/* Pending-action table shared with Func_080be0b4. */
struct PendingEntry_080be378 {
    u8 element;
    u8 unknown_01;
    u8 object_id;
    s8 state;
};

struct PendingTable_080be378 {
    u8 unknown_000[8];
    struct PendingEntry_080be378 entries[64];
    s32 count;
};

/*
 * Rotation lists published through 0x03001e74.  The two s16 arrays are the
 * normal and the mirrored turn order; both are terminated by 255.
 */
struct TurnOrder_080be378 {
    u8 unknown_00[88];
    s16 order_a[7];   /* 0x58, read from element (index - 6) upwards */
    s16 order_b[7];   /* 0x66 */
};

/* ------------------------------------------------------------------ */
/* External entry points                                              */
/* ------------------------------------------------------------------ */

s32 Func_080022ec(s32 numerator, s32 denominator);
void Func_080030f8(u32 frames);
void Func_08009080(u32 handle, s32 mode);
void Func_08009088(u32 handle, s32 value);
void Func_08015118(void);
void Func_08015120(s32 value, s32 slot);
void Func_080151c8(s32 message_id);
struct PendingTable_080be378 *Func_08077000(s32 side);
struct BattleUnit_080be378 *Func_08077008(s32 unit);
void Func_08077010(s32 unit);
struct ItemInfo_080be378 *Func_08077018(u32 item);
s32 Func_08077078(struct BattleUnit_080be378 *unit, s32 mode);
struct AbilityInfo_080be378 *Func_08077080(s32 action_id);
s32 Func_080770c0(s32 flag_id);
s32 Func_08077118(s32 action_id, u32 mask);
void Func_08077128(s32 unit);
s32 Func_08077160(struct BattleUnit_080be378 *unit);
s32 Func_08077170(s32 unit);
s8 Func_08077178(s32 actor, s32 member, s32 kind, s32 effect, s32 scale);
s32 Func_080771a0(void);
void Func_080771b0(s32 unit, s32 row, s32 column);
void Func_080771c0(s32 unit, s32 row, s32 column);
void Func_080771c8(s32 unit, s32 row, s32 column);
struct SummonEntry_080be378 *Func_080771e0(s32 slot);
s32 Func_080771e8(s32 row, s32 column);
s32 Func_08077208(s32 unit, s32 row, s32 column);
s32 Func_08077210(s32 unit, s32 row, s32 column);
s32 Func_080772b8(s32 effect);
s32 Func_080772f8(struct BattleUnit_080be378 *unit);
struct BattleUnit_080be378 **Func_080b7dd0(s32 unit);
void *Func_080b9a44(s32 value);
void Func_080bb65c(void);
void Func_080bb8d8(void);
void Func_080bb938(void);
void Func_080bbabc(s32 selector, s32 value);
s32 Func_080bd3c8(s32 action_id);
void Func_080bd808(s32 frames);
void Func_080bdfec(void);
void Func_080be02c(void);
s32 Func_080be070(s32 object_id);
s32 Func_080be0b4(u32 object_id, u8 *element_counts);
void Func_080c10e8(s32 mode, s32 value);
void Func_080c1798(s32 unit, s32 row, s32 mode, s32 value);
u32 Func_080c23e8(s32 class_index);
void Func_080f9010(s32 sound_id);

/*
 * UNCERTAIN (ABI): Func_080be18c takes its action id in r0 and a *second*,
 * register-passed argument in r9 which points one word past the five-word
 * context block this function keeps at frame offset 0x1c.  The callee reads
 * context_end[-1] .. context_end[-5], i.e. the plan, the Func_080b9a44 handle,
 * the turn-order pointer, the request and the unit, in that order.  Freestanding
 * C cannot pin r9 without inline assembly, so the semantic reconstruction spells the
 * hidden input as an explicit trailing parameter.  A byte-exact promotion has
 * to restore the register convention.
 */
s32 Func_080be18c(s32 action_id, void *const *context_end);

#define TURN_ORDER_080BE378   (*(struct TurnOrder_080be378 **)0x03001e74)
#define AUTO_BATTLE_080BE378  (*(volatile u8 *)0x03001f54)
#define BATTLE_OPTIONS_080BE378 (*(volatile u32 *)0x03001ae8)

/* Element-keyed flag word table and status table indexed by action id. */
#define ACTION_FLAGS_080BE378  ((const u32 *)0x080c2da0)
#define ACTION_STATUS_080BE378 ((const u8 *)0x080c2b98)

/* ------------------------------------------------------------------ */

s32 Func_080be378(struct BattleRequest_080be378 *request,
                  struct BattlePlan_080be378 *plan)
{
    /*
     * The five-word context block.  Func_080be18c reads it backwards through
     * the pointer built below, so the order of these slots is load bearing.
     */
    void *context[5];
    void *const *context_end = &context[5];

    struct BattleUnit_080be378 *unit;
    struct BattleUnit_080be378 *target_unit = 0;
    struct AbilityInfo_080be378 *ability = 0;
    struct AbilityInfo_080be378 *cost_info;
    struct ItemInfo_080be378 *item;
    struct SummonEntry_080be378 *summon;
    struct PendingTable_080be378 *pending;
    struct PendingEntry_080be378 *entry;
    u8 element_counts[4];
    s32 action_id = 1;
    s32 message = 0;
    u32 index;
    s32 count;
    s32 value;
    s32 threshold;
    s32 step;

    unit = Func_08077008(request->actor);

    context[0] = unit;
    context[1] = request;
    context[2] = TURN_ORDER_080BE378;
    context[3] = Func_080b9a44(request->unknown_0a);
    context[4] = plan;

    Func_080bdfec();

    plan->actor = (u8)request->actor;
    plan->unknown_60 = 0;
    plan->member_count = 0;
    plan->flags = 0;
    plan->failure = 0;
    plan->action_kind = 4;

    Func_08015118();

    if (unit->hp == 0) {
        return -2;
    }

    /*
     * Auto-battle sweep.  With the auto flag live and option 0x16d set, walk
     * the published turn order and re-queue every entry the action filter does
     * not already reject.  Bit 2 of the option word selects the mirrored list.
     */
    if (AUTO_BATTLE_080BE378 != 0 && Func_080770c0(0x16d) != 0 &&
        (BATTLE_OPTIONS_080BE378 & 0x100) != 0) {
        s32 use_mirror = (BATTLE_OPTIONS_080BE378 & 4) != 0;
        s32 offset = 100;
        s32 order_id;
        u8 *order_base = (u8 *)context[2];

        for (;;) {
            if (use_mirror) {
                order_id = *(s16 *)(order_base + 2 + offset);
            } else {
                order_id = *(s16 *)(order_base + offset - 12);
            }
            if (order_id == 255) {
                break;
            }
            if (order_id != 254 &&
                Func_08077118(order_id, 0xc0000000) == 0) {
                Func_080bbabc(8, order_id);
                Func_080bbabc(9, order_id);
            }
            offset += 2;
        }

        Func_080bb938();
        return -2;
    }

    Func_08015118();

    unit = context[0];

    /* A queued forced action consumes itself and aborts the manual command. */
    if (unit->forced_action != 0) {
        unit->forced_action = 0;
        Func_08015120(request->actor, 1);
        Func_080151c8(0x880);
        return -1;
    }

    if (unit->downed != 0) {
        Func_08015120(request->actor, 1);
        Func_080151c8(0x858);
        return -1;
    }

    if (unit->stunned != 0) {
        Func_08015120(request->actor, 1);
        Func_080151c8(0x857);
        return -1;
    }

    /*
     * Restrained units may only defend (command 3); the restraint lapses on
     * three quarters of the frames.
     */
    if ((unit->restraint & 1) != 0 && request->command != 3 &&
        (Func_080771a0() & 3) == 0) {
        Func_08015120(request->actor, 1);
        Func_080151c8(0x859);
        return -1;
    }

    if (request->command == 8) {
        return -2;
    }

    for (index = 0; index < 14; index++) {
        plan->effects[index] = 0;
    }
    for (index = 0; index < 14; index++) {
        plan->results[index] = -1;
    }

    /*
     * The original indexes a 100-entry jump table with the command and calls
     * the out-of-range handler (the 080bee00 tail) for anything above 99.
     * Entries 9..98 all point at that same tail.
     */
    if ((u32)request->command > 99) {
        goto action_selected;
    }

    switch (request->command) {
    case 0:
        /* ------------------------------------------------ attack */
        unit = context[0];
        action_id = Func_08077160(unit);
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        if (action_id != 1) {
            goto resolve_default_action;
        }
        Func_08015120(request->actor, 1);
        value = Func_08077078(unit, 1);
        Func_08015120(value, 2);
        Func_080151c8(0x819);
        Func_080bb8d8();
        Func_08015120(action_id, 4);
        message = 0x81a;
        goto emit_message;

    case 1:
        /* -------------------------------------------- use ability */
        action_id = request->param;
        cost_info = Func_08077080(action_id);
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        Func_08015120(request->actor, 1);
        Func_08015120(action_id, 4);
        Func_080151c8(0x83e);

        unit = context[0];
        value = 1;
        if (unit->pp < (s16)cost_info->pp_cost) {
            plan->failure = 2;
            value = 0;
        }
        if (unit->out_of_pp != 0) {
            plan->failure = 1;
            value = 0;
        }
        if (value == 0) {
            goto action_selected;
        }

        plan->failure = 0;
        unit = context[0];
        unit->pp = (s16)((u16)unit->pp - cost_info->pp_cost);
        Func_08077128(request->actor);
        if (unit->pp < 0) {
            unit->pp = 0;
        }
        if (unit->pp > unit->pp_max) {
            unit->pp = unit->pp_max;
        }
        goto action_selected;

    case 2:
        /* ----------------------------------------------- use item */
        if (request->param < 0) {
            Func_08015120(request->actor, 1);
            Func_080151c8(0x81b);
            return -1;
        }

        unit = context[0];
        item = Func_08077018(unit->abilities[request->param]);
        action_id = item->action_id;

        /*
         * An item is usable when it publishes an action and its slot is not
         * marked broken/equipped-locked (bit 0x400 of the inventory word).
         */
        if (action_id == 0 ||
            (((struct BattleUnit_080be378 *)context[0])
                 ->abilities[request->param] & 0x400) != 0) {
            Func_08015120(request->actor, 1);
            Func_080151c8(0x816);
            unit = context[0];
            if ((s8)unit->warned_no_target != 0) {
                return -1;
            }
            unit->warned_no_target = 1;
            return -1;
        }

        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        Func_08015120(request->actor, 1);
        unit = context[0];
        Func_08015120(unit->abilities[request->param], 2);

        message = 0x817;
        if (item->usability == 2 || item->usability == 0) {
            if (item->kind == 3 || item->kind == 1 ||
                (item->kind >= 6 && item->kind <= 8)) {
                message = 0x818;
            }
        }
        goto emit_message;

    case 3:
    case 7:
        /* Defend and the alias of it that shares the jump-table slot. */
        Func_08015120(request->actor, 1);
        Func_080151c8(0x816);
        return -1;

    case 4:
        /* --------------------------------------------- use djinni */
        action_id = request->param;
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }
        Func_08015120(request->actor, 1);
        Func_08015120(action_id, 4);

        ability = Func_08077080(action_id);
        message = ((ability->element & 15) == 6) ? 0x8f1 : 0x8f0;

        /*
         * Per-djinni announcement message.  The original is a comparison tree
         * over the action id; the ranges below reproduce it exactly.
         */
        if (action_id == 224) {
            message = 0x83e;
        } else if (action_id >= 435 && action_id <= 436) {
            message = 0x8f2;
        } else if (action_id >= 437 && action_id <= 441) {
            message = 0x8fb;
        } else if (action_id >= 442 && action_id <= 444) {
            message = 0x8f0;
        } else if (action_id == 472) {
            message = 0x8fc;
        } else if (action_id == 488) {
            message = 0x8fd;
        } else {
            switch (action_id) {
            case 492: message = 0x8ff; break;
            case 494: message = 0x8fa; break;
            case 495: message = 0x8fe; break;
            case 499: message = 0x8f9; break;
            case 500: message = 0x8f7; break;
            case 501: message = 0x8f8; break;
            case 503: message = 0x900; break;
            case 504: message = 0x901; break;
            case 508: message = 0x902; break;
            default:  break;
            }
        }
        goto emit_message;

    case 5: {
        /* ------------------------------------------------ retreat */
        s32 row = ((s32)((u32)request->param << 16) >> 24) & 15;
        s32 column = request->param & 255;

        action_id = Func_080771e8(row, column);

        if (Func_08077208(request->actor, row, column) != 0) {
            /* The destination is already reachable; treat it as an action. */
            if (Func_080be18c(action_id, context_end) == -1) {
                return -1;
            }
            Func_080771c8(request->actor, row, column);
            ability = Func_08077080(action_id);
            Func_08015120(request->actor, 1);
            Func_08015120(action_id, 4);
            Func_080151c8(0x83f);
            plan->action_kind = ability->kind;
            goto action_selected;
        }

        if (Func_08077210(request->actor, row, column) == 0) {
            Func_08015120(request->actor, 1);
            Func_08015120(action_id, 4);
            Func_080f9010(114);
            Func_080151c8(0x85b);
            Func_080030f8(60);
            return -1;
        }

        /* Leaving the encounter: tear the battle scene down and unwind. */
        Func_08077080(action_id);
        Func_080c10e8(0, 0);
        Func_080771b0(request->actor, row, column);
        Func_080771c0(request->actor, row, column);
        Func_08077010(request->actor);
        Func_080bdfec();
        Func_080bd808(30);
        Func_080bbabc(0, request->actor);
        Func_080bbabc(3, ((row * 5) << 2) + column + 300);
        Func_080bbabc(14, 175);
        Func_080bbabc(10, 0);
        Func_080bbabc(4, 0x897);
        Func_080bbabc(11, request->actor);
        Func_080f9010(212);
        Func_08009080(*(u32 *)Func_080b7dd0(request->actor), 3);
        Func_08009088(*(u32 *)Func_080b7dd0(request->actor), 32);
        Func_080c1798(request->actor, row, 3, 0);
        Func_080be02c();
        return -2;
    }

    case 6: {
        /* ------------------------------------------------- summon */
        s32 matched = 0;

        summon = Func_080771e0(request->param);
        Func_080be0b4(request->actor, element_counts);

        pending = Func_08077000((u16)request->actor > 7 ? 1 : 0);
        entry = pending->entries;

        /*
         * Pay the summon's per-element requirement.  The loop stops at the
         * first element that is short, so a partial match leaves the already
         * overwritten counters behind exactly as the original does.
         */
        if (element_counts[0] >= summon->cost[0]) {
            index = 0;
            for (;;) {
                element_counts[index] = summon->cost[index];
                matched++;
                if (matched > 3) {
                    break;
                }
                index++;
                if (element_counts[index] < summon->cost[index]) {
                    break;
                }
            }
        }

        action_id = summon->action_id;
        if (Func_080be18c(action_id, context_end) == -1) {
            return -1;
        }

        if (matched != 4) {
            Func_08015120(request->actor, 1);
            Func_08015120(action_id, 4);
            Func_080151c8(0x842);
            return -1;
        }

        Func_08015120(request->actor, 1);
        Func_08015120(action_id, 4);
        Func_080151c8(0x841);

        /* Reserve the pending djinn that fund the summon. */
        count = pending->count;
        for (index = 0; index < count; index++) {
            entry = &pending->entries[index];
            if (entry->state == -1 && Func_080be070(entry->object_id) != 0) {
                u8 remaining = element_counts[entry->element];

                if (remaining != 0) {
                    entry->state = (s8)254;
                    element_counts[entry->element] = (u8)(remaining - 1);
                }
            }
            count = pending->count;
        }
        goto action_selected;
    }

    case 8:
        return -2;

    case 99:
        /* -------------------------------------------- flee attempt */
        if ((u16)request->actor <= 7) {
            Func_080151c8(0x843);
        } else {
            Func_08015120(request->actor, 1);
            Func_080151c8(0x846);
        }
        Func_080bb65c();
        plan->status = 7;
        return 0;

    default:
        break;
    }

    goto action_selected;

emit_message:
    Func_080151c8(message);

action_selected:
    /* 080bee00 */
    if (action_id != 1) {
        goto resolve_default_action;
    }

    /* 080bee08: the plain attack path. */
    plan = context[4];
    target_unit = Func_08077008(plan->members[0]);
    plan->action_id = 1;
    plan->action_kind = Func_08077170(request->actor);
    plan->status = 2;

    unit = context[0];
    if (unit->djinn_locked == 0) {
        plan->flags = Func_080c23e8(unit->class_index) | 0x4000;
    } else {
        plan->flags = 0;
        if (unit->class_index <= 5) {
            switch (unit->class_index) {
            case 0:
            case 5:
                plan->flags = 0x4001;
                break;
            case 1:
                plan->flags = 0x4001;
                break;
            case 2:
            case 3:
                plan->flags = 0x4004;
                break;
            default:
                /* class 4 leaves the flags at zero */
                break;
            }
        }
    }

    Func_08015120(request->actor, 1);
    Func_080151c8(0x814);

    /*
     * Auto-battle delay tuning.  Only a live, unafflicted, auto-flagged target
     * enters the adjustment; note that the adjustment is only reached when the
     * auto flag itself reads zero, so both stores below write zero.  That is
     * what the original does and it is preserved rather than folded away.
     */
    if (target_unit->hp != 0 && target_unit->downed == 0 &&
        target_unit->stunned == 0 && target_unit->forced_action == 0) {
        u8 auto_flag = target_unit->auto_battle;

        if (auto_flag == 0) {
            unit = context[0];
            if (unit->delay_penalty != 0 &&
                (Func_080771a0() & 255) <= 152) {
                plan->delays[0] = auto_flag;
            }
            if ((Func_080771a0() & 31) == 0) {
                plan->delays[0] = 0;
            }
        }
    }

    if (Func_080770c0(366) != 0) {
        plan->delays[0] = 0;
    }

    if (target_unit->hp == 0) {
        goto finish;
    }

    if ((Func_080771a0() & 31) != 0) {
        /*
         * 080bef88: rare stumble check.  The per-unit rate is scaled into the
         * 16-bit random range before comparing.
         */
        value = Func_080022ec((s32)((u32)Func_080772f8(context[0]) << 16), 200);
        if (value > (Func_080771a0() & 0xffff)) {
            plan->effects[0] = 1;
        }
        goto finish;
    }

    plan->effects[0] = 1;
    goto finish;

resolve_default_action:
    /* 080befb4: everything that is not the plain attack. */
    ability = Func_08077080(action_id);
    plan->action_kind = ability->kind;
    plan->flags = 0;
    plan->action_id = action_id;

    switch (ability->effect) {
    case 41:
    case 42:
    case 43:
    case 44:
    case 65:
    case 68:
        if (ability->effect == 65 || ability->effect == 68) {
            threshold = 153;
        } else if (ability->effect == 41 || ability->effect == 43) {
            threshold = 32;
        } else {
            threshold = 64;
        }

        if (ability->effect == 65 || ability->effect == 41 ||
            ability->effect == 42) {
            step = 1;
        } else {
            step = 2;
        }

        if ((Func_080771a0() & 255) < threshold) {
            for (index = 0; index < plan->member_count; index++) {
                plan->delays[index] = (u8)(plan->delays[index] + step);
            }
        }
        break;

    default:
        if ((u8)(ability->effect - 36) <= 4) {
            switch (ability->effect - 36) {
            case 0: threshold = 63; break;
            case 1: threshold = 31; break;
            case 2: threshold = 15; break;
            case 3: threshold = 7; break;
            default: threshold = 3; break;
            }
            if ((Func_080771a0() & threshold) == 0) {
                for (index = 0; index < plan->member_count; index++) {
                    plan->effects[index] = 2;
                }
            }
        } else if (action_id == 178) {
            for (index = 0; index < plan->member_count; index++) {
                plan->results[index] =
                    Func_08077178(request->actor, plan->members[index],
                                  ability->kind, ability->effect, 100);
            }
        }
        break;
    }

    /* 080bf0f8 */
    if (action_id <= 0x206) {
        plan->flags = ACTION_FLAGS_080BE378[action_id];
        if ((s8)plan->delays[0] > 1) {
            plan->flags = ACTION_FLAGS_080BE378[action_id] +
                          ((s8)plan->delays[0] << 12) - 0x1000;
        }
    }

    if (action_id <= 0x205 && ACTION_STATUS_080BE378[action_id] != 0) {
        plan->status = ACTION_STATUS_080BE378[action_id];
    } else if (Func_080bd3c8(action_id) != 0) {
        plan->status = 3;
    } else if (plan->flags != 0) {
        unit = context[0];
        plan->status = (unit->djinn_locked == 0) ? 8 : 3;
    } else {
        plan->status = 1;
    }

    if (Func_080772b8(ability->effect) != 0) {
        plan->flags |= 0x10000;
    }

    if (action_id == 178 && (s8)plan->results[0] != 0) {
        plan->flags |= 0x1000;
    }

finish:
    /* 080bf1a8 */
    if (request->command == 2) {
        if (plan->status != 5 && plan->status != 9) {
            plan->status = 4;
        }
    }

    plan->command = (u16)request->command;
    return 0;
}

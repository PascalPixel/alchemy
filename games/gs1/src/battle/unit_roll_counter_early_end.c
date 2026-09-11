#include "scene.h"
#include "counter_runtime.h"
#include "types.h"
#include "fixed_math.h"

/* battle/unit_roll_counter_early_end.c */
/* battle/unit/roll_counter_early_end.c */
s32 BattleUnit_RollCounterEarlyEnd(s32 object_id, s32 count, s32 bias)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (count <= 5) {
        s32 threshold = ((state->value_42 * 3 - count * 5) + bias) * 0x28f;
        if (threshold >= (Battle_Check() & 0xffff))
            return 1;
    }
    return 0;
}

/* battle/unit/tick_counter_132.c */
s32 BattleUnit_TickCounter132(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_132 != 0) {
        s32 zero;
        state->counter_132--;
        zero = 0;
        if ((s8)state->counter_132 == 0) {
            state->state_133 = zero;
            return 1;
        }
        if (state->state_133 < 0) {
            if (Battle_Run(value, state->counter_132, 30) != 0) {
                state->state_133 = zero;
                state->counter_132 = zero;
                return 1;
            }
        }
    }
    return 0;
}

/* battle/unit/tick_counter_134.c */
s32 BattleUnit_TickCounter134(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_134 != 0) {
        state->counter_134--;
        if (state->counter_134 == 0) {
            state->state_135 = 0;
            return 1;
        }
        if (state->state_135 < 0 &&
            Battle_Run(value, state->counter_134, 20) != 0) {
            state->state_135 = 0;
            state->counter_134 = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_136.c */
s32 BattleUnit_TickCounter136(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();

    if (state->counter_136 != 0) {
        state->counter_136--;
        if (state->counter_136 == 0) {
            state->state_137 = 0;
            return 1;
        }
        if (state->state_137 < 0 &&
            Battle_Run(value, state->counter_136, 20) != 0) {
            state->state_137 = 0;
            state->counter_136 = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_138.c */
s32 BattleUnit_TickCounter138(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_138 != 0) {
        state->counter_138--;
        if (state->counter_138 == 0)
            return 1;
        if (Battle_Run(value, state->counter_138, 30) != 0) {
            state->counter_138 = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_139.c */
s32 BattleUnit_TickCounter139(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_139 != 0) {
        state->counter_139--;
        if (state->counter_139 == 0)
            return 1;
        if (Battle_Run(value, state->counter_139, 60) != 0) {
            state->counter_139 = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13a.c */
s32 BattleUnit_TickCounter13a(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13a != 0) {
        state->counter_13a--;
        if (state->counter_13a == 0)
            return 1;
        if (Battle_Run(value, state->counter_13a, 70) != 0) {
            state->counter_13a = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13b.c */
s32 BattleUnit_TickCounter13b(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13b != 0) {
        state->counter_13b--;
        if (state->counter_13b == 0)
            return 1;
        if (Battle_Run(value, state->counter_13b, 40) != 0) {
            state->counter_13b = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13c.c */
s32 BattleUnit_TickCounter13c(s32 value)
{
    struct CounterRuntime *state = Runtime_GetObject();
    if (state->counter_13c != 0) {
        state->counter_13c--;
        if (state->counter_13c == 0)
            return 1;
        if (Battle_Run(value, state->counter_13c, 50) != 0) {
            state->counter_13c = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/advance_counter_and_check_chance.c */
void *Runtime_GetObject(s32);

s32 Battle_AdvanceCounterAndCheckChance(s32 id)
{
    s32 t2;
    s32 ret;
    u8 t3;
    s32 t;
    s32 cnt;
    void *obj;

    obj = Runtime_GetObject(id);
    cnt = FIELD_AT_OFFSET(obj, u8 *, 0x13D);
    t = cnt & 0xFF;
    if (t != 0) {
        if ((u32)t > 7U) {
            t += 0xF8;
            FIELD_AT_OFFSET(obj, u8 *, 0x13D) = t;
            cnt = t;
        }
        if (cnt & 7) {
            t2 = cnt + 0xFF;
            FIELD_AT_OFFSET(obj, u8 *, 0x13D) = t2;
            cnt = t2;
        }
        ret = 1;
        t3 = cnt;
        if (t3 != 0) {
            if ((u32)t3 <= 7U &&
                Battle_Place(id, FIELD_AT_OFFSET(obj, u8 *, 0x13D), 0x1E) != 0) {
                FIELD_AT_OFFSET(obj, u8 *, 0x13D) = 0U;
                return 1;
            }
            goto block_9;
        }
        return ret;
    }
block_9:
    ret = 0;
    return ret;
}

/* battle/unit/tick_counter_13e.c */
u8 *Runtime_GetObject(void);

s32 BattleUnit_TickCounter13e(void)
{
    u8 *value = Runtime_GetObject() + 0x13E;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_13f.c */
u8 *Runtime_GetObject(void);

s32 BattleUnit_TickCounter13f(void)
{
    u8 *value = Runtime_GetObject() + 0x13F;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}

/* battle/unit/tick_counter_146.c */
u8 *Runtime_GetObject(void);

s32 BattleUnit_TickCounter146(void)
{
    u8 *base = Runtime_GetObject();
    u8 *value = base + 0x146;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            base[0x147] = 0;
            return 1;
        }
    }
    return 0;
}

/* battle/placement/update_timed_entries.c */
struct PlacementEntry {
    u8 x;
    u8 y;
    u8 id;
    s8 timer;
};

struct PlacementList {
    struct PlacementEntry entries[64];
    s32 count;
};

struct PlacementTable {
    u8 padding[8];
    struct PlacementList list;
};

struct BattleObject {
    u8 padding[0x38];
    s16 active;
};

struct PlacementTable *Battle_Run(s32 owner);
struct BattleObject *Runtime_GetObject(u8 id);
void BattleUnit_Recalculate(u8 id);

s32 BattlePlacement_UpdateTimedEntries(void)
{
    struct PlacementList *list;
    struct PlacementEntry *timed_entry;
    struct PlacementEntry *expired_entry;
    s32 index;
    s32 removed;
    s32 initial_count;

    list = &Battle_Run(0)->list;
    initial_count = list->count;
    index = 0;
    removed = 0;
    if (index < initial_count) {
        timed_entry = list->entries;
        do {
            if (timed_entry->timer > 0 &&
                Runtime_GetObject(timed_entry->id)->active != 0) {
                timed_entry->timer--;
            }
            index++;
            timed_entry++;
        } while (index < list->count);
    }
    index = 0;
    if (index < list->count) {
        expired_entry = list->entries;
        do {
            if (expired_entry->timer == 0) {
                u8 id = expired_entry->id;

                Battle_Place(id, expired_entry->x, expired_entry->y);
                Battle_unk2_3(id, expired_entry->x, expired_entry->y);
                BattleUnit_Recalculate(id);
                removed = 1;
            } else {
                expired_entry++;
                index++;
            }
        } while (index < list->count);
    }
    return removed;
}

/* battle/placement/update_timed_entries_twenty_times.c */
s32 BattlePlacement_UpdateTimedEntriesTwentyTimes(void)
{
    s32 cnt;

    cnt = 0x13;
    do {
        cnt -= 1;
        Battle_Check();
    } while (cnt >= 0);
    return 0;
}

/* battle/runtime/reserved_no_op_c.c */
void Battle_ReservedNoOpF674(void)
{
}

/* battle/presentation/act/mosaic_fade.c */
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void WaitFrames(s32);

s32 BattlePres_RunMosaicFadeOut(void)
{
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
    s32 i;
    volatile u16 *reg;

    bg0cnt = *(volatile u16 *)0x04000008;
    *(volatile u16 *)0x04000008 = bg0cnt | 0x40;
    reg = (volatile u16 *)0x04000008;
    bg1cnt = *++reg;
    *reg = bg1cnt | 0x40;
    bg2cnt = *++reg;
    *reg = bg2cnt | 0x40;
    bg3cnt = *++reg;
    *reg = bg3cnt | 0x40;
    reg += 0x21;
    *reg = 0x3eee;

    Battle_Do(16);
    for (i = 0; i <= 15; i++) {
        Rand();
        Rand();
        Rand();
        Rand();
        *(volatile u16 *)0x0400004c = (i << 8) | i;
        WaitFrames(1);
    }

    *(volatile u16 *)0x04000000 = 1;
    WaitFrames(4);
    reg = (volatile u16 *)0x04000008;
    *reg++ = bg0cnt;
    *reg++ = bg1cnt;
    *reg++ = bg2cnt;
    *reg = bg3cnt;
    return 0;
}

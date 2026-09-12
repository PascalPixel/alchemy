#include "types.h"
#include "scene.h"
#include "runtime_1e74.h"
#include "global_cells.h"

/* battle/select_weighted_index.c */
/*
 * The r9 static-chain setup at the call sites and the callee's matching r9
 * save identify this as a GNU C nested function. The public alias gives the
 * reconstructed entry its address-derived name without changing its code.
 */
extern s32 Battle_SelectWeightedIndex(u8 *)
    __attribute__((alias("Select_080bd3e4.0")));

static __inline__ s32 Scope_080bd3e4(void)
{
    s32 Select_080bd3e4(u8 *weights)
    {
        s32 value;
        s32 total;
        s32 result;
        s32 index;

        value = Battle_Check() & 0xFF;
        total = weights[0];
        result = 0;
        index = 0;
        if (value >= total) {
loop:
            index++;
            if (index <= 7) {
                total += weights[index];
                if (value < total)
                    result = index;
                else
                    goto loop;
            }
        }
        return result;
    }

    return 0;
}

/* battle/event_runtime/begin_phase.c */
void BattleEventRuntime_BeginPhase(s32 parameter)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    if (runtime->phase == 0) {
        runtime->phase = 1;
        if (parameter != 0) {
            runtime->parameter = parameter;
        }
    }
}

/* battle/event_runtime/schedule_phase.c */
s32 ScheduleCallbackAfterFrames(s32, s32);

s32 BattleEventRuntime_SchedulePhase(s32 parameter)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    runtime->value_7fc = 0;
    runtime->value_804 = 0;
    runtime->value_808 = parameter;
    runtime->phase = 2;
    runtime->flag_655 = 0;
    return ScheduleCallbackAfterFrames((s32)Battle_Run, 0xC80);
}

/* battle/event_runtime/reset.c */
s32 BattleEventRuntime_Reset(void)
{
    struct Runtime1e74 *runtime;

    runtime = Runtime1e74_Get();
    runtime->phase = 0;
    runtime->value_7fc = 0;
    runtime->value_804 = 0;
    runtime->value_808 = 0;
    runtime->value_7f8 = 0;
    runtime->parameter = 0x86;
    runtime->value_824 = 0;
}

/* battle/event_runtime/wait_for_ready.c */
void WaitFrames(u32);
void ScheduleCallback(void *);
s32 BattleEventRuntime_Reset(void);

s32 BattleEventRuntime_WaitForReady(void)
{
    s32 state;
    void *runtime;

    runtime = *(void **)ADDR_03001E74;
    state = FIELD_AT_OFFSET(runtime, s32 *, 0x800);
    if (state == 0) {
        FIELD_AT_OFFSET(runtime, s32 *, 0x800) = 1;
        state = 1;
    }
    if (state != 4) {
        do {
            WaitFrames(1U);
        } while (FIELD_AT_OFFSET(runtime, s32 *, 0x800) != 4);
    }
    ScheduleCallback((void *)Battle_Run);
    return BattleEventRuntime_Reset();
}

/* battle/party/is_unit_listed.c */
u32 BattleParty_IsUnitListed(u32 arg0)
{
    u16 values[8];
    s32 count;
    s32 total;
    s32 i;

    count = 1;
    if (arg0 > 7) {
        count = 2;
    }
    total = Battle_Apply(count, values);

    for (i = 0; i < total; i++) {
        if (values[i] == arg0) {
            break;
        }
    }

    return i != total;
}

/* battle/placement/count_valid_entries.c */
struct PlacementEntry { u8 x; u8 y; u8 id; s8 timer; };
struct PlacementList { struct PlacementEntry entries[64]; s32 count; };
struct PlacementTable { u8 padding[8]; struct PlacementList list; };

struct PlacementTable *Battle_Run(s32 owner);

s32 BattlePlacement_CountValidEntries(u32 arg0, u8 *counts)
{
    u16 values[8];
    struct PlacementList *list;
    s32 total;
    s32 found;
    s32 i;
    s32 j;
    s32 kind;
    s32 owner;

    found = 0;
    kind = 1;
    if (arg0 > 7)
        kind = 2;
    total = Battle_Apply(kind, values);
    owner = 0;
    if (arg0 > 7)
        owner = 1;
    list = &Battle_Run(owner)->list;
    if (counts != 0)
        for (j = 3; j >= 0; j--)
            counts[j] = 0;
    i = 0;
    if (list->count != 0) {
        do {
            if (list->entries[i].timer == -1) {
                for (j = 0; j < total; j++)
                    if (values[j] == list->entries[i].id)
                        break;
                if (j != total) {
                    if (counts != 0)
                        counts[list->entries[i].x]++;
                    found++;
                }
            }
            i++;
        } while (i != list->count);
    }
    return found;
}

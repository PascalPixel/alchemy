#include "scene.h"
#include "event_runtime.h"
#include "types.h"
#include "object_lookup.h"

/* event/event_set_status1c6.c */
/* event/set_status_1c6.c */
void Event_SetStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    Sys_Apply(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 1;
}

/* event/clear_status_1c6.c */
void Event_ClearStatus1c6(void)
{
    struct EventRuntime *runtime = gWork;

    Sys_Apply(runtime->value_1c0, runtime->value_1c8);
    runtime->status_1c6 = 0;
}

/* event/wait_value_1c8_frames.c */
void WaitFrames(s32);

void Event_WaitValue1c8Frames(void)
{
    WaitFrames(gWork->value_1c8);
}

/* event/set_pair_work_1c0.c */
extern struct SharedData_08091f14 gCell;

void Event_SetPairWork1c0(u16 first, u16 second)
{
    gWork->value_170 = 999;
    gCell.first = first;
    gCell.second = second;
}

/* event/set_pair1c4_and_reset_value170.c */
struct State_08091e6c {
    u8 filler0[0x170];
    u16 value;
};

struct gRom {
    u8 filler0[0x1C4];
    u16 first;
    u16 second;
};

extern struct State_08091e6c *volatile gWork;

void Event_SetPair1c4AndResetValue170(u16 first, u16 second)
{
    struct State_08091e6c *state = gWork;
    state->value = 999;
    gCell.first = first;
    gCell.second = second;
}

void Sys_Run(u16 value)
{
    struct State_08091e6c *state = gWork;
    state->value = value;
}

/* battle/effects/set/fx_set_weighted_result.c */
/* battle/effects/set/set_weighted_result.c */
extern u8 Value_00000021;
u16 BattleFx_GetWeightedResult(s32 arg0, s32 arg1);
s32 BattleFx_LookupResult(void *object);

void BattleFx_SetWeightedResult(s32 arg0, s32 arg1)
{
    register s32 first = arg0;
    register s32 second = arg1;
    register struct EventRuntime *runtime = gWork;

    runtime->value_17c = BattleFx_GetWeightedResult(first, second);
    if (first == 98 && second == 0)
        gCell.special = (u16)(s32)&Value_00000021;
    if (runtime->mode_19e == 3)
        BattleFx_LookupResult((u8 *)ObjectTable_Get(gCell.object_id) + 8);
    Battle_Apply(first, second);
}

/* battle/effects/set/set_phase_request.c */
struct RuntimeState_08091f14 {
    u8 data[0x17c];
    s16 value;
    u8 padding17e[0x20];
    s16 mode;
};

struct SharedData_08091f14 {
    u8 data[0x1ce];
    u16 first;
    u16 second;
    u8 padding1d2[0x22];
    u32 object;
    u8 padding1f8[0x3c];
    u16 request;
};

extern struct RuntimeState_08091f14 *gWork;

s16 BattleFx_GetPhaseResult(s32 value);
void *ObjectTable_Get(u32 object);
/* Object table: 192 pointers at gWork + 0x14 (object/table/get.c). */

void BattleFx_SetPhaseRequest(s32 flags, s32 value)
{
    struct RuntimeState_08091f14 *state;
    struct SharedData_08091f14 *shared;
    s32 high;

    state = gWork;
    high = flags & 0x800;
    flags &= 0xff;

    if (high == 0)
        Battle_Do(flags);

    shared = &gCell;
    shared->request = (value + 0x12c) | high;
    state->value = BattleFx_GetPhaseResult(value);
    if (state->mode == 3) {
        void *object = ObjectTable_Get(shared->object);

        BattleFx_LookupResult((u8 *)object + 8);
    }
    Battle_Apply(0, 0);
}

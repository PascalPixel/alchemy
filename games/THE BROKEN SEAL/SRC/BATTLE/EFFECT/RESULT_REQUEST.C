#include "EVENT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"

struct ResultWork {
    u8 unknown_000[0x1d6];
    u16 special;
    u8 unknown_1d8[0x1c];
    u32 object_id;
    u8 unknown_1f8[0x3c];
    u16 request;
};

extern struct ResultWork Data_02000240;
#define PARTY_STATE Data_02000240
extern u8 Value_00000021;
u16 Func_0808b05c(s32 arg0, s32 arg1);
#define BattleFx_GetWeightedResult Func_0808b05c
s16 Func_0808b074(s32 value);
#define BattleFx_GetPhaseResult Func_0808b074
s32 Func_0808adf0(void *arg0);
#define BattleFx_LookupResult Func_0808adf0
void Func_0809537c(s32 flags);
s32 Func_0808b320(s32, s32);

void BattleFx_SetWeightedResult(s32 arg0, s32 arg1)
{
    register s32 first = arg0;
    register s32 second = arg1;
    register struct EventRuntime *runtime = Data_03001ebc;

    runtime->value_17c = BattleFx_GetWeightedResult(first, second);
    if (first == 98 && second == 0)
        PARTY_STATE.special = (u16)(s32)&Value_00000021;
    if (runtime->mode_19e == 3)
        BattleFx_LookupResult((u8 *)ObjectTable_Get(PARTY_STATE.object_id) + 8);
    Func_0808b320(first, second);
}

void BattleFx_SetPhaseRequest(s32 flags, s32 value)
{
    struct EventRuntime *state;
    struct ResultWork *shared;
    s32 high;

    state = Data_03001ebc;
    high = flags & 0x800;
    flags &= 0xff;

    if (high == 0)
        Func_0809537c(flags);

    shared = &PARTY_STATE;
    shared->request = (value + 0x12c) | high;
    state->value_17c = BattleFx_GetPhaseResult(value);
    if (state->mode_19e == 3) {
        void *object = ObjectTable_Get(shared->object_id);

        BattleFx_LookupResult((u8 *)object + 8);
    }
    Func_0808b320(0, 0);
}

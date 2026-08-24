#include "types.h"

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

extern struct RuntimeState_08091f14 *Data_03001ebc;
extern struct SharedData_08091f14 Data_02000240;

void Func_0809537c(s32 flags);
s16 BattleEffect_GetPhaseResult(s32 value);
s32 BattleEffect_LookupResult(void *object);
void *ObjectTable_Get(u32 object);
/* Object table: 192 pointers at Data_03001ebc + 0x14 (object/table/get.c). */
#define ObjectTable_Get Func_0808ba1c
s32 Func_0808b320(s32 first, s32 second);

void Func_08091f14(s32 flags, s32 value)
{
    struct RuntimeState_08091f14 *state;
    struct SharedData_08091f14 *shared;
    s32 high;

    state = Data_03001ebc;
    high = flags & 0x800;
    flags &= 0xff;

    if (high == 0)
        Func_0809537c(flags);

    shared = &Data_02000240;
    shared->request = (value + 0x12c) | high;
    state->value = BattleEffect_GetPhaseResult(value);
    if (state->mode == 3) {
        void *object = ObjectTable_Get(shared->object);

        BattleEffect_LookupResult((u8 *)object + 8);
    }
    Func_0808b320(0, 0);
}

#include "types.h"
#include "scene.h"
#include "abi/battle/effects/set/set_phase_request.h"

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
extern struct SharedData_08091f14 gCell;

s16 BattleFx_GetPhaseResult(s32 value);
s32 BattleFx_LookupResult(void *object);
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

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
s16 Func_0808b074(s32 value);
void *Func_0808ba1c(u32 object);
/* Object table: 192 pointers at Data_03001ebc + 0x14 (games/gs1/src/0808ba1c.c). */
#define GetObject Func_0808ba1c
void Func_0808adf0(void *object);
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
    state->value = Func_0808b074(value);
    if (state->mode == 3) {
        void *object = GetObject(shared->object);

        Func_0808adf0((u8 *)object + 8);
    }
    Func_0808b320(0, 0);
}

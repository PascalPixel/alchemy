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
    u8 padding1d2[0x62];
    u16 request;
    u8 padding236[0x1f8];
    u32 object;
};

extern struct RuntimeState_08091f14 * volatile Data_03001ebc;
extern struct SharedData_08091f14 Data_02000240;

void Func_0809537c(s32);
s16 Func_0808b074(s32);
void *Func_0808ba1c(u32);
void Func_0808adf0(void *);
void Func_0808b320(s32, s32);

void Func_08091f14(s32 flags, s32 value)
{
    struct RuntimeState_08091f14 *state = Data_03001ebc;
    s32 high = flags & 0x800;

    if (high == 0) {
        Func_0809537c(flags & 0xff);
    }
    Data_02000240.request = (value + 0x12c) | high;
    state->value = Func_0808b074(value);
    if (state->mode == 3) {
        void *object = Func_0808ba1c(Data_02000240.object);
        Func_0808adf0((u8 *)object + 8);
    }
    Func_0808b320(0, 0);
}

void Func_08091f90(u16 first, u16 second)
{
    Data_02000240.first = first;
    Data_02000240.second = second;
}

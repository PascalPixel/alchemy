#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void *Func_0808fecc(void);
void Func_080907b0(s32);
void Func_08003bb4(s32);
void Func_08003b70(s32);
void Func_08091200(s32, s32);
void Func_08091254(s32);
void Func_080041d8(const void *, s32);
void Func_0800307c(s32, s32, const void *);
void Func_080030f8(s32);

/*
 * Start one of five field-transition profiles encoded in the high byte of
 * effect. The low byte selects the profile variant and frames supplies the
 * duration recorded for the active transition.
 */
void Func_080901c0(s32 effect, s32 frames)
{
    u16 profile = (u16)((effect >> 8) & 0xFF);
    u16 variant = (u16)(effect & 0xFF);
    u8 *state;

    switch (profile) {
    case 0:
        Func_08003bb4(0);
        Func_08003b70(frames);
        break;

    case 1:
        Func_08091200(0x8000, 0);
        Func_08091254(frames);
        break;

    case 2:
        state = Func_0808fecc();
        M2C_FIELD(state, u16 *, 0x528) = variant;
        M2C_FIELD(state, u16 *, 0x52A) = 0x20;
        M2C_FIELD(state, u16 *, 0x534) = 0x3F;
        M2C_FIELD(state, u16 *, 0x536) = 1;
        Func_080041d8((const void *)0x0808F52D, 0xC80);
        Func_080041d8((const void *)0x0808F499, 0x480);
        Func_080030f8(1);
        state[0x53A] = 0x20;
        state[0x53B] = 0x40;
        state[0x53C] = (u8)frames;
        state[0x53D] = 0;
        break;

    case 3:
        state = Func_0808fecc();
        M2C_FIELD(state, u16 *, 0x528) = variant;
        M2C_FIELD(state, u16 *, 0x52A) = 0x20;
        Func_080907b0(0);
        Func_080030f8(1);
        Func_080041d8((const void *)0x08090659, 0xC80);
        state[0x53A] = 0x20;
        state[0x53B] = 0x40;
        state[0x53C] = (u8)frames;
        state[0x53D] = 0;
        break;

    case 4:
        state = Func_0808fecc();
        if (variant == 0) {
            Func_080041d8((const void *)0x080903BD, 0xC80);
            Func_0800307c(1, 0, (const void *)0x08090585);
            state[0x53A] = 0;
            state[0x53B] = 0x50;
            state[0x53C] = (u8)frames;
            state[0x53D] = 0;
        } else {
            Func_080041d8((const void *)0x08090489, 0xC80);
            Func_0800307c(1, 0, (const void *)0x08090585);
            state[0x53A] = 0;
            state[0x53B] = 0x50;
            state[0x53C] = (u8)frames;
            state[0x53D] = 0;
        }
        break;
    }
}

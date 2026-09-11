#include "types.h"
#include "scene.h"

struct State_080b7f9c {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct Local_080b7f9c {
    s32 first;
    s32 second;
    s32 third;
};

extern struct State_080b7f9c *gIw;

/*
 * The tail call below is a typed indirect call to the relocated routine at
 * 0x03000250, whose argument count is not established. Sys_Apply names
 * the bx rN veneer slot that reaches it, not a routine at that address.
 */

void Camera_InitDefaultTransform(void)
{
    struct State_080b7f9c *state = gIw;
    struct Local_080b7f9c transfer;

    state->field36 = 192 << 6;
    state->field34 = 254 << 8;
    state->field20 = 255 << 17;
    state->field0c = 0;
    state->field10 = 0;
    state->field14 = 0;
    state->field1c = 0;
    state->field18 = 0;

    Sys_Run();
    Sys_Do(&state->field0c);
    Sys_unk2_2(state->field36);
    Sys_unk3_2(state->field34);

    transfer.first = 0;
    transfer.second = 0;
    transfer.third = state->field20;
    ((void (*)(struct Local_080b7f9c *, struct State_080b7f9c *))0x03000250)(&transfer, state);
}

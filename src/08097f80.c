#include "effect_0809b11c.h"

struct Output_08097f80 {
    s32 x;
    s32 y;
    s32 z;
};

u32 Func_08004458(void);
void Func_0800447c(s32, s32, struct Output_08097f80 *);
u32 Func_0809ba34(struct Effect_0809b11c *);
void Func_0809bb34(void *);

void Func_08097f80(struct Effect_0809b11c *effect)
{
    struct Output_08097f80 position;
    s8 *state_pointer = &effect->state;
    s32 state;

next_state:
    state = *state_pointer;
    if (state == 0) {
        u32 angle;

        position.x = effect->saved_x;
        position.z = effect->saved_z;
        angle = Func_08004458();
        Func_0800447c(0x1e0000, (u16)angle, &position);
        effect->result_x = position.x;
        effect->result_z = position.z;
        effect->speed_z = 0x40000;
        effect->speed_x = 0x40000;
        effect->initial_state = state;
        (*state_pointer)++;
        return;
    }

    if (state == 1) {
        if (Func_0809ba34(effect) == 0) {
            (*state_pointer)++;
            goto next_state;
        }
        return;
    }

    if (state == 2) {
        effect->result_x = effect->saved_x;
        effect->result_z = effect->saved_z;
        {
            u32 value = 0x400;

            effect->unknown32 = value;
        }
        effect->initial_state = 1;
        (*state_pointer)++;
        return;
    }

    if (state == 3 && Func_0809ba34(effect) == 0)
        Func_0809bb34(effect);
}

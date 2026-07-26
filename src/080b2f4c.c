#include "types.h"

struct Position {
    s32 x;
    s32 y;
    s32 z;
};

struct Effect {
    u8 filler_00[0xC];
    s32 x;
    s32 z;
    s32 source_x;
    s32 source_z;
    s32 filler_1C;
    s32 velocity;
    s32 acceleration;
    u8 filler_28[0x18];
    s8 state;
    u8 filler_41;
    s8 flag;
};

u32 Func_08004458(void);
void Func_0800447c(s32, s32, struct Position *);
void Func_0808a510(struct Effect *, s32, s32);
s32 Func_0808a508(struct Effect *);
void Func_0808a530(struct Effect *);

void Func_080b2f4c(struct Effect *effect)
{
    struct Position position;
    s8 *state_pointer;
    s32 state;
    s32 result;

    state_pointer = &effect->state;
    state = *state_pointer;
    if (state == 0) {
        position.x = effect->source_x;
        position.z = effect->source_z;
        Func_0800447c(0x280000, Func_08004458(), &position);
        Func_0808a510(effect, position.x, position.z);
        position.x = effect->source_x;
        position.z = effect->source_z;
        Func_0800447c(0x40000, Func_08004458(), &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->velocity = 0x20000;
        effect->acceleration = 0x6666;
        effect->flag = state;
        *state_pointer = (u8)*state_pointer + 1;
    } else if (state == 1) {
        result = Func_0808a508(effect);
        if (result == 0)
            *state_pointer = result;
    } else if (state == 2) {
        if (Func_0808a508(effect) == 0)
            Func_0808a530(effect);
    }
}

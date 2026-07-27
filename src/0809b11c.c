#include "effect_0809b11c.h"

struct Output {
    s32 x;
    s32 y;
    s32 z;
};

struct PositionSource_0809b11c {
    u8 padding00[8];
    struct Output position;
};

struct RuntimeState_0809b11c {
    u8 padding00[0x10];
    struct PositionSource_0809b11c *position_source;
};

extern struct RuntimeState_0809b11c *Data_03001f30;
extern u32 Data_03001e40;

void Func_080974d8(struct Output *);
u32 Func_08004458(void);
void Func_0800447c(s32, s32, struct Output *);
u32 Func_0809ba34(struct Effect_0809b11c *);
void Func_0809bb34(void *);
void Func_080f9010(s32);

void Func_0809b11c(struct Effect_0809b11c *effect)
{
    struct Output position;
    struct PositionSource_0809b11c *source;
    s32 state;
    u32 first_random;

    source = Data_03001f30->position_source;
    state = effect->state;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y;
        position.z = source->position.z;
        Func_080974d8(&position);

        effect->initial_x = position.x;
        effect->initial_z = (s32)((u32)position.z + 0x80000);
        effect->saved_z = effect->initial_z;
        effect->saved_x = effect->initial_x;
        position.z = effect->saved_z;
        position.x = effect->saved_x;

        first_random = Func_08004458();
        Func_0800447c(
            0x780000,
            ((first_random << 13) >> 16)
                - ((Func_08004458() << 13) >> 16)
                + 0xc000,
            &position);

        effect->result_x = position.x;
        effect->result_z = position.z;
        effect->speed_z = 0x50000;
        effect->speed_x = 0x50000;
        effect->initial_state = state;
        effect->state++;

        if ((Data_03001e40 & 2) != 0)
            Func_080f9010(0xf6);
    } else if (state == 1) {
        if (Func_0809ba34(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (Func_0809ba34(effect) == 0)
            Func_0809bb34(effect);
    }
}

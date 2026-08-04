#include "types.h"

struct Position_0809aa98 {
    s32 x;
    s32 y;
    s32 z;
};

struct Camera_0809aa98 {
    s32 filler_00;
    s32 x;
    s32 y;
    s32 z;
};

struct Effect_0809aa98 {
    u8 filler_00[0xC];
    s32 x;
    s32 z;
    s32 source_x;
    s32 source_z;
    s32 filler_1c;
    s32 velocity;
    s32 acceleration;
    u8 filler_28[0xA];
    u16 field_32;
    u8 filler_34[0xC];
    s8 state;
    u8 filler_41;
    u8 flag;
};

extern struct Camera_0809aa98 *Data_03001f30;

extern u32 Func_08004458(void);
extern void Func_0800447c(s32, s32, struct Position_0809aa98 *);
extern void Func_080974d8(struct Position_0809aa98 *);
extern s32 Func_0809ba34(struct Effect_0809aa98 *);
extern void Func_0809bb34(struct Effect_0809aa98 *);

void Func_0809aa98(struct Effect_0809aa98 *effect)
{
    struct Camera_0809aa98 *camera;
    struct Position_0809aa98 position;
    s8 *state_pointer;
    s16 angle;
    s32 state;

    camera = Data_03001f30;
    state_pointer = &effect->state;
top:
    state = *state_pointer;
    if (state == 0) {
        position.x = effect->source_x;
        position.z = effect->source_z;
        angle = Func_08004458();
        Func_0800447c(Func_08004458() * 30 + 0x280000, (u16)angle, &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->acceleration = 0x40000;
        effect->velocity = 0x40000;
        effect->flag = state;
        goto advance;
    } else if (state == 1) {
        if (Func_0809ba34(effect) != 0)
            return;
        *state_pointer = (u8)*state_pointer + 1;
        goto top;
    } else if (state == 2) {
        position.x = camera->x;
        position.y = camera->y + 0x80000;
        position.z = camera->z;
        Func_080974d8(&position);
        Func_0800447c(0x40000, Func_08004458(), &position);
        effect->x = position.x;
        effect->z = position.z;
        effect->field_32 = 0x1000;
        effect->flag = 1;
advance:
        *state_pointer = (u8)*state_pointer + 1;
        return;
    } else if (state == 3) {
        if (Func_0809ba34(effect) == 0)
            Func_0809bb34(effect);
        return;
    } else {
        return;
    }
}

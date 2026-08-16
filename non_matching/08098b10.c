#include "types.h"

struct Vector_08098b10 {
    s32 x;
    s32 y;
    s32 z;
};

struct Target_08098b10 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Runtime_08098b10 {
    s32 angle;
    u8 unknown_04[12];
    struct Target_08098b10 *target;
};

struct Object_08098b10 {
    u8 unknown_00[12];
    s32 x;
    s32 z;
    s32 source_x;
    s32 source_z;
    u8 unknown_1c[4];
    s32 velocity_x;
    s32 velocity_z;
    u8 unknown_28[10];
    u16 scale;
    u8 unknown_34[12];
    s8 state;
    u8 unknown_41;
    u8 enabled;
};

extern struct Runtime_08098b10 *Data_03001f30;

s32 Func_08004458(void);
void Func_0800447c(s32 magnitude, s32 angle, struct Vector_08098b10 *output);
void Func_080974d8(struct Vector_08098b10 *value);
s32 Func_0809ba34(struct Object_08098b10 *object);
void Func_0809bb34(struct Object_08098b10 *object);

void Func_08098b10(struct Object_08098b10 *object)
{
    struct Runtime_08098b10 *runtime = Data_03001f30;
    struct Vector_08098b10 value;
    s32 state;

again:
    state = object->state;
    if (state == 0) {
        value.x = object->source_x;
        value.z = object->source_z;
        Func_0800447c(0x190000, (u16)Func_08004458(), &value);
        object->x = value.x;
        object->z = value.z;
        object->velocity_z = 0x30000;
        object->velocity_x = 0x30000;
        object->enabled = 0;
        goto advance;
    }

    if (state == 1) {
        if (Func_0809ba34(object) == 0) {
            s8 *stateAddress = &object->state;

            *stateAddress = *stateAddress + 1;
            goto again;
        }
        return;
    }

    if (state == 2) {
        struct Target_08098b10 *target = runtime->target;

        value.x = target->x;
        value.y = target->y + 0x100000;
        value.z = target->z;
        Func_0800447c(0x80000, runtime->angle, &value);
        Func_080974d8(&value);
        Func_0800447c(0x40000, Func_08004458(), &value);
        object->x = value.x;
        object->z = value.z;
        object->scale = 0x800;
        object->enabled = 1;
advance:
        object->state++;
        return;
    }

    if (state == 3 && Func_0809ba34(object) == 0)
        Func_0809bb34(object);
}

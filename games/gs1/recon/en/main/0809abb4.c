#include "types.h"

#define BattleEffect_RunFallbackObjectTransition Func_0809abb4

struct BattleEffectMotionObject {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
};

struct BattleEffectMotionRecord {
    struct BattleEffectMotionObject *object;
    u8 reserved_04[0x44];
};

struct BattleEffectMotionState {
    s32 reserved_00;
    s32 x;
    s32 y;
    s32 z;
    struct BattleEffectMotionObject *target;
    u8 reserved_14[0x44];
    struct BattleEffectMotionRecord records[16];
};

extern struct BattleEffectMotionState *Data_03001f30;

struct BattleEffectMotionObject *Func_08096c80(s32, s32, s32, s32);
void Func_08009080(struct BattleEffectMotionObject *, s32);
void Func_08097384(void);
s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
void Func_080974d8(s32 *);
u32 Func_08004458(void);
void Func_0800447c(s32, u32, s32 *);
void Func_0809ba90(struct BattleEffectMotionRecord *, s32, s32, s32);
void Func_0809ba7c(struct BattleEffectMotionRecord *, void *);
void Func_08009248(struct BattleEffectMotionObject *, s32);
void Func_080090d0(struct BattleEffectMotionObject *);
extern u8 Data_0809aa99;

void BattleEffect_RunFallbackObjectTransition(void)
{
    struct BattleEffectMotionState *state = Data_03001f30;
    struct BattleEffectMotionObject *target = state->target;
    struct BattleEffectMotionObject *object;
    struct BattleEffectMotionRecord *record;
    s32 position[3];
    s32 origin[3];
    s32 destination[3];
    s32 step;
    s32 index;

    state->y = target->y;
    object = Func_08096c80(0xfa, 0, 0, 0);
    step = 0;
    Func_08009080(object, 0);
    if (object == 0)
        return;

    Func_08097384();
    origin[0] = target->x;
    origin[1] = target->y + 0x100000;
    origin[2] = target->z;
    destination[0] = state->x;
    destination[1] = state->y + 0x80000;
    destination[2] = state->z;
    do {
        s32 scale;

        object->x = origin[0] +
            Func_080022ec(step * (destination[0] - origin[0]), 10);
        object->y = origin[1] +
            Func_080022ec(step * (destination[1] - origin[1]), 10);
        object->z = origin[2] +
            Func_080022ec(step * (destination[2] - origin[2]), 10);
        scale = Func_080022ec(step * 0xc000, 10) + 0x4000;
        object->scale_x = scale;
        object->scale_y = scale;
        step++;
        Func_080030f8(1);
    } while (step < 11);

    Func_080030f8(5);
    Func_08009080(object, 1);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6d);

    record = &state->records[0];
    index = 15;
    do {
        position[0] = object->x;
        position[1] = object->y + 0x80000;
        position[2] = object->z;
        Func_080974d8(position);
        Func_0800447c(0x40000, Func_08004458(), position);
        Func_0809ba90(record, 0x11d, position[0], position[2]);
        Func_0809ba7c(record, &Data_0809aa99);
        Func_08009248(record->object, 7);
        record++;
        index--;
    } while (index >= 0);

    position[0] = object->x;
    position[1] = object->y + 0x80000;
    position[2] = object->z;
    Func_080030f8(8);
    Func_080090d0(object);
    Func_080030f8(4);
    Func_080030f8(30);
    Func_0809748c();
}

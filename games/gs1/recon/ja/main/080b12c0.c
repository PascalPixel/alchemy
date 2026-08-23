#include "types.h"

struct Input_080b12c0 {
    u8 primary_id;
    u8 padding_01;
    u8 secondary_id;
    u8 padding_03[0x1b];
    s8 target_adjustment;
    u8 padding_1f[0x0d];
    s8 target_modifier;
    u8 padding_2d[0x2b];
    u32 presentation_flags;
};

struct Work_080b12c0 {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 one;
    s32 count;
    s32 mode;
    s32 unknown_1c;
    u8 padding_20[4];
    s16 members[24];
};

struct Motion_080b12c0 {
    u8 padding_00[8];
    s32 x;
    u8 padding_0c[4];
    s32 z;
};

struct Slot_080b12c0 {
    struct Motion_080b12c0 *object;
};

struct Child_080b12c0 {
    s16 value;
};

struct Record_080b12c0 {
    u8 padding_00[40];
    struct Child_080b12c0 *child;
};

struct Unit_080b12c0 {
    u8 padding_00[0x128];
    u8 class_id;
};

extern s32 *Data_03001f00;
extern void *Data_03001e74;

struct Slot_080b12c0 *Func_080b7dd0(s32 id);
s32 Func_080044d0(s32 first, s32 second);
void Func_080030f8(s32 frames);
void Func_080c10e8(s32 first, s32 second);
void Func_080b9d34(void *input, struct Work_080b12c0 *work);
struct Unit_080b12c0 *Func_08077008(s32 id);
struct Record_080b12c0 *Func_080b7f70(
    struct Motion_080b12c0 *object, s32 index);
s32 Func_08009260(s32 value, s32 second, s32 third);
u32 Func_080c2410(u32 value);
void Func_080b82c4(s32 first, s32 second, s32 divisor, s32 initial_y);
void Func_08009088(struct Motion_080b12c0 *object, s32 action);
void Func_08015130(s32 value);
void Func_080b8178(s32 id);
void Func_080b8000(s32 id);
u32 Func_080bbabc(u32 opcode, u32 operand);
void Func_080bb938(void);
s32 Func_080b7b6c(void *position, s32 mode);
s32 Func_080022ec(s32 angle, s32 count);
void Func_080c0df4(s32 first, s32 second, s32 mode);
void Func_080041d8(void *callback, s32 interval);
void Func_080c9008(struct Work_080b12c0 *work);
void Func_080c9018(struct Work_080b12c0 *work);
void Func_080be02c(void);
void Func_080b6cb0(void);
void Func_080c0cec(s32 first, s32 second, s32 third, s32 fourth);

#ifndef BATTLE_SEQUENCE_OWNER
#define RunBattlePresentation Func_080b12c0
#define BATTLE_SEQUENCE_OWNER RunBattlePresentation
#endif
#ifndef BATTLE_SEQUENCE_CALLBACK
#define BATTLE_SEQUENCE_CALLBACK ((void *)0x080b4899)
#endif

s32 BATTLE_SEQUENCE_OWNER(struct Input_080b12c0 *input)
{
    struct Work_080b12c0 work;
    struct Input_080b12c0 *saved_input;
    struct Motion_080b12c0 *object;
    struct Record_080b12c0 *record;
    struct Unit_080b12c0 *unit;
    s16 position[3];
    s32 scripted;
    s32 *facing;
    s32 angle;
    s32 adjusted;
    s32 facing_angle;
    s32 first_coordinate;
    s32 second_coordinate;
    s32 divisor;
    s32 direct;
    s32 index;
    s32 phase;

    facing = Data_03001f00;
    saved_input = input;
    object = Func_080b7dd0(saved_input->primary_id)->object;
    second_coordinate = object->z;
    first_coordinate = object->x;
    angle = (u16)Func_080044d0(first_coordinate, second_coordinate);
    adjusted = angle - 0x2000;
    if (saved_input->primary_id > 7)
        adjusted = angle + 0x6000;
    adjusted &= 0x7fff;
    facing_angle = (adjusted - 0x2000) / 2 + 0x2000;

    if (*facing == facing_angle) {
        *facing = facing_angle;
        Func_080030f8(5);
    } else {
        *facing = facing_angle;
        Func_080030f8(10);
    }

    Func_080c10e8(0, 0);
    Func_080b9d34(saved_input, &work);
    if (work.flags == 0x87)
        Func_08015130(*((u8 *)Data_03001e74 + 0x41) & ~1);

    unit = Func_08077008(work.primary_id);
    Func_08077008(work.members[0]);
    scripted = saved_input->target_modifier;
    direct = saved_input->target_adjustment == 0;

    record = Func_080b7f70(
        Func_080b7dd0(saved_input->primary_id)->object, 0);
    divisor = Func_08009260(record->child->value, 2, 1);
    Func_080b82c4(
        work.primary_id,
        work.members[0],
        divisor,
        Func_080c2410(unit->class_id) << 16);
    Func_08009088(Func_080b7dd0(work.primary_id)->object, 16);
    Func_080b7dd0(work.members[0]);

    if ((u16)work.members[0] <= 7)
        work.secondary_is_low_id = 1;
    else
        work.secondary_is_low_id = 0;

    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    *(volatile u16 *)0x04000000 |= 0x6000;

    if (direct != 0) {
        Func_080030f8(10);
        Func_080b8178(work.members[0]);
        Func_080030f8(2);
        Func_080030f8(4);
        Func_080030f8(10);
        Func_080bbabc(0, saved_input->secondary_id);
        Func_080bbabc(4, 0x853);
        Func_080bb938();
        Func_080b8000(work.members[0]);
    } else {
        phase = 0;
        work.unknown_1c = 0;
        if (saved_input->presentation_flags != 0)
            work.unknown_1c = 1;

        if (scripted != 0) {
            work.flags += 200;
            phase = 1;
            facing[5] = 1;
            position[0] = work.primary_id;
            position[1] = work.secondary_id;
            position[2] = 0xff;
            Func_080b7b6c(position, 0);
        }

        divisor -= 8;
        if (divisor <= 0)
            divisor = 1;
        {
            s32 loop_first;
            s32 loop_second;

            for (index = 0; index != divisor; index++) {
                if (phase != 0) {
                    loop_first = work.primary_id;
                    loop_second = work.secondary_id;
                    Func_080c0df4(
                        loop_first,
                        loop_second,
                        Func_080022ec(index * 30, divisor) + 100);
                }
                Func_080030f8(1);
            }
        }

        Func_080041d8(BATTLE_SEQUENCE_CALLBACK, 0xc80);
        if (work.flags != 0) {
            if (saved_input->presentation_flags & 0x4000)
                Func_080c9008(&work);
            else
                Func_080c9018(&work);
        }
        Func_080be02c();
        if (scripted != 0) {
            facing[5] = 0;
            Func_080b6cb0();
            Func_080c0cec(0, 0, 0, 100);
        }
        Func_080b8000(work.members[0]);
    }
    Func_080b8000(work.primary_id);
    return 0;
}

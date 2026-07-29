#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef struct BattleInput {
    u8 primary_id;
    u8 unknown01;
    u8 secondary_id;
    u8 padding03[0x1b];
    s8 mode;
    u8 padding1f[0x0d];
    s8 effect_flag;
    u8 padding2d[0x2b];
    s32 flags;
} BattleInput;

typedef struct BattleWork {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 one;
    s32 count;
    s32 mode;
    s32 unknown_1c;
    u8 padding20[4];
    s16 members[24];
} BattleWork;

typedef struct Motion {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 z;
} Motion;

typedef struct Slot {
    Motion *object;
} Slot;

typedef struct Child {
    s16 value;
} Child;

typedef struct Record {
    u8 padding00[40];
    Child *child;
} Record;

s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);
void Func_080041d8(u32, s32);
u16 Func_080044d0(s32, s32);
void Func_08009088(Motion *, s32);
s32 Func_08009260(s16, s32, s32);
void Func_08015130(s32);
u8 *Func_08077008(s32);
void Func_080b6cb0(void);
void Func_080b7b6c(s16 *, s32);
Slot *Func_080b7dd0(u8);
Record *Func_080b7f70(Motion *, s32);
void Func_080b8000(s16);
void Func_080b8178(s16);
void Func_080b82c4(s32, s16, s32, s32);
void Func_080b9d34(void *, BattleWork *);
void Func_080bb938(void);
void Func_080bbabc(s32, u16);
void Func_080be02c(void);
void Func_080c0cec(s32, s32, s32, s32);
void Func_080c0df4(s32, s32, s32);
void Func_080c10e8(s32, s32);
s32 Func_080c2410(u8);
void Func_080c9008(BattleWork *);
void Func_080c9018(BattleWork *);

/*
 * Configure and execute a two-object battle action, including its optional
 * interpolated effect and the scripted alternate presentation.
 */
s32 Func_080ba2c0(BattleInput *input)
{
    BattleWork work;
    s32 *global = *(s32 **)0x03001f00;
    Motion *primary_object = Func_080b7dd0(input->primary_id)->object;
    u16 angle = Func_080044d0(primary_object->x, primary_object->z);
    s32 adjusted;
    s32 facing;
    s32 divisor;
    s32 scripted;
    s32 has_temporary_effect = 0;
    u8 *primary_record;

    if (input->primary_id > 7)
        adjusted = angle + 0x6000;
    else
        adjusted = angle - 0x2000;
    adjusted &= 0x7fff;
    facing = ((adjusted - 0x2000) / 2) + 0x2000;

    if (global[0] == facing) {
        global[0] = facing;
        Func_080030f8(5);
    } else {
        global[0] = facing;
        Func_080030f8(0x0a);
    }

    Func_080c10e8(0, 0);
    Func_080b9d34(input, &work);
    if (work.flags == 0x87) {
        u8 *state = *(u8 **)0x03001e74;
        Func_08015130(state[0x41] & ~1);
    }

    primary_record = Func_08077008(work.primary_id);
    Func_08077008(work.members[0]);
    scripted = input->mode == 0;

    divisor = Func_08009260(
        Func_080b7f70(
            Func_080b7dd0(input->primary_id)->object, 0)->child->value,
        2, 1);
    Func_080b82c4(
        work.primary_id,
        work.members[0],
        divisor,
        Func_080c2410(primary_record[0x128]) << 16);
    Func_08009088(Func_080b7dd0((u8)work.primary_id)->object, 0x10);
    Func_080b7dd0((u8)work.members[0]);
    work.secondary_is_low_id = (u16)work.members[0] <= 7;

    *(volatile u16 *)0x04000040 = 0x00f0;
    *(volatile u16 *)0x04000042 = 0x00f0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    *(volatile u16 *)0x04000000 |= 0x6000;

    if (scripted != 0) {
        Func_080030f8(0x0a);
        Func_080b8178(work.members[0]);
        Func_080030f8(2);
        Func_080030f8(4);
        Func_080030f8(0x0a);
        Func_080bbabc(0, input->secondary_id);
        Func_080bbabc(4, 0x853);
        Func_080bb938();
        Func_080b8000(work.members[0]);
    } else {
        s32 ticks;
        s32 i;

        work.unknown_1c = input->flags != 0;
        if (input->effect_flag != 0) {
            s16 members[3];

            work.flags += 0xc8;
            has_temporary_effect = 1;
            global[5] = 1;
            members[0] = work.primary_id;
            members[1] = work.secondary_id;
            members[2] = 0xff;
            Func_080b7b6c(members, 0);
        }

        ticks = divisor - 8;
        if (ticks <= 0)
            ticks = 1;
        for (i = 0; i < ticks; i++) {
            if (has_temporary_effect != 0) {
                Func_080c0df4(
                    work.primary_id,
                    work.secondary_id,
                    Func_080022ec(i * 0x1e, ticks) + 0x64);
            }
            Func_080030f8(1);
        }

        Func_080041d8(0x080bd899, 0x0c80);
        if (work.flags != 0) {
            if ((input->flags & 0x4000) != 0)
                Func_080c9008(&work);
            else
                Func_080c9018(&work);
        }
        Func_080be02c();

        if (input->effect_flag != 0) {
            global[5] = 0;
            Func_080b6cb0();
            Func_080c0cec(0, 0, 0, 0x64);
        }
        Func_080b8000(work.members[0]);
    }

    Func_080b8000((s16)work.primary_id);
    return 0;
}

#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef struct ActionInput_080ba978 {
    u8 primary_id;
    u8 padding01;
    u8 secondary_id;
    u8 padding03[0x4d];
    s32 transition_value;
    u8 padding54[4];
    u32 flags;
    s32 message_mode;
} ActionInput_080ba978;

typedef struct BattleWork_080ba978 {
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
} BattleWork_080ba978;

typedef struct Motion_080ba978 {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 z;
} Motion_080ba978;

typedef struct Slot_080ba978 {
    Motion_080ba978 *object;
} Slot_080ba978;

typedef struct Child_080ba978 {
    u8 padding00[5];
    u8 value;
} Child_080ba978;

typedef struct Record_080ba978 {
    u8 padding00[0x27];
    u8 child_count;
    Child_080ba978 **children;
} Record_080ba978;

void Func_080041d8(u32, s32);
u16 Func_080044d0(s32, s32);
void Func_08009080(Motion_080ba978 *, s32);
void Func_08009088(Motion_080ba978 *, s32);
void Func_08015130(s32);
Slot_080ba978 *Func_080b7dd0(s32);
Record_080ba978 *Func_080b7f70(Motion_080ba978 *, s32);
void Func_080b8000(s16);
void Func_080b9d34(void *, BattleWork_080ba978 *);
void Func_080bb938(void);
void Func_080bbabc(s32, u16);
void Func_080be02c(void);
void Func_080c10e8(s32, s32);
void Func_080c1798(s32, s32, s32, s32);
void Func_080c1a14(void);
void Func_080c9008(BattleWork_080ba978 *);
void Func_080c9018(BattleWork_080ba978 *);
void Func_080f9010(s32);

/*
 * Orient the battle camera, build the participant work area, run the selected
 * transition or message path, and release every actor used by the action.
 */
s32 Func_080ba978(ActionInput_080ba978 *input, s32 options)
{
    BattleWork_080ba978 work;
    s32 *facing = *(s32 **)0x03001f00;
    u8 *world = *(u8 **)0x03001e74;
    Motion_080ba978 *primary_object;
    s32 i;

    if ((input->flags & 0x40000) != 0) {
        facing[0] = input->primary_id <= 7 ? -0x2000 : 0x5000;
        facing[1] = 60;
    } else {
        Motion_080ba978 *object =
            Func_080b7dd0(input->primary_id)->object;
        s32 angle = Func_080044d0(object->x, object->z);
        s32 adjusted;
        s32 target;
        s32 delta;
        s32 next;

        if (input->primary_id <= 7) {
            adjusted = angle - 0x1800;
            target = 0x2000;
        } else {
            adjusted = angle + 0x1800;
            target = -0x2000;
        }

        delta = (target - (s16)adjusted) * 3;
        if (delta < 0)
            delta += 3;
        next = (s16)adjusted + (delta >> 2);

        if ((input->primary_id <= 7) ==
            (input->secondary_id <= 7)) {
            next = input->primary_id <= 7 ? 0x2400 : -0x2400;
        }
        if (facing[0] != next)
            facing[0] = next;
    }

    if ((input->flags & 0x80000) != 0) {
        facing[0] = input->primary_id <= 7 ? -0x2000 : 0x2000;
        facing[1] = 60;
    }

    Func_080b9d34(input, &work);
    if ((options & 1) != 0)
        work.unknown_1c = 1;

    Func_080c10e8(0, 0);
    Func_08015130(world[0x41] & ~1);
    primary_object = Func_080b7dd0(work.primary_id)->object;
    Func_08009080(primary_object, 3);
    Func_08009088(primary_object, 0x10);
    Func_080f9010(0x9a);

    if ((options & 2) != 0)
        Func_080c1798(work.primary_id, input->transition_value, 1, 0);
    else if ((options & 1) == 0)
        Func_080c1798(work.primary_id, input->transition_value, 0, 0);

    work.secondary_is_low_id = input->secondary_id <= 7;
    for (i = 0; i < work.count; i++) {
        Record_080ba978 *record = Func_080b7f70(
            Func_080b7dd0(work.members[i])->object,
            0);
        s32 children = record->child_count - 1;
        s32 child;

        for (child = 0; child < children; child++) {
            FIELD(&work, u8, 0x34 + i * 4 + child) =
                record->children[child]->value;
        }
    }

    if (input->message_mode != 0) {
        if (input->message_mode == 1) {
            Func_080bbabc(0, input->primary_id);
            Func_080bbabc(4, 0x856);
        } else {
            Func_080bbabc(4, 0x855);
        }
        Func_080bb938();
        Func_080c1a14();
    } else {
        Func_080041d8(0x080bd899, 0x0c80);
        if (work.flags != 0) {
            if ((input->flags & 0x4000) != 0)
                Func_080c9008(&work);
            else
                Func_080c9018(&work);
        } else {
            Func_080c1a14();
        }
        Func_080be02c();
        Func_08009080(primary_object, 1);
        for (i = 0; i < work.count; i++)
            Func_080b8000(work.members[i]);
    }
    return 0;
}

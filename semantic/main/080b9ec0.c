#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

typedef struct ActionInput_080b9ec0 {
    u8 primary_id;
    s8 count;
    u8 ids[14];
    u8 padding10[0x40];
    s32 transition_value;
    u8 padding54[4];
    u32 flags;
    s32 message_mode;
} ActionInput_080b9ec0;

typedef struct BattleWork_080b9ec0 {
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
} BattleWork_080b9ec0;

typedef struct Motion_080b9ec0 {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 z;
} Motion_080b9ec0;

typedef struct Slot_080b9ec0 {
    Motion_080b9ec0 *object;
} Slot_080b9ec0;

typedef struct Child_080b9ec0 {
    u8 padding00[5];
    u8 value;
} Child_080b9ec0;

typedef struct Record_080b9ec0 {
    u8 padding00[0x27];
    u8 child_count;
    Child_080b9ec0 **children;
} Record_080b9ec0;

void Func_080030f8(s32);
void Func_080041d8(u32, s32);
void Func_08009080(Motion_080b9ec0 *, s32);
void Func_08015130(s32);
u32 Func_080b6c08(s32, u16 *);
void Func_080b6c90(void);
void Func_080b7b6c(u16 *, s32);
Slot_080b9ec0 *Func_080b7dd0(s32);
Record_080b9ec0 *Func_080b7f70(Motion_080b9ec0 *, s32);
void Func_080b9d34(void *, BattleWork_080b9ec0 *);
void Func_080bb938(void);
void Func_080bbabc(s32, u16);
void Func_080be02c(void);
void Func_080c0cec(s32, s32, s32, s32);
void Func_080c0f98(u16, s32);
void Func_080c10e8(s32, s32);
void Func_080c1798(s32, s32, s32, s32);
void Func_080c1a14(void);
void Func_080c9008(BattleWork_080b9ec0 *);
void Func_080c9010(BattleWork_080b9ec0 *);
void Func_080c9018(BattleWork_080b9ec0 *);
void Func_080f9010(s32);

static s32 IsLowSide_080b9ec0(u16 id)
{
    return id <= 7;
}

/*
 * Stage a battle-side transition, execute either its message-only path or the
 * filtered participant action, then restore every visible participant while
 * fading the scene back in.
 */
s32 Func_080b9ec0(ActionInput_080b9ec0 *input, s32 options)
{
    BattleWork_080b9ec0 work;
    u16 actors[14];
    s32 *facing = *(s32 **)0x03001f00;
    u8 *world = *(u8 **)0x03001e74;
    u16 primary = input->primary_id;
    u16 secondary = input->ids[0];
    Motion_080b9ec0 *primary_object;
    u32 actor_count;
    u32 i;

    Func_080b9d34(input, &work);
    if ((input->flags & 0x8000) != 0) {
        facing[0] = primary <= 7 ? 0x2000 : 0x5000;
        facing[1] = 60;
    } else {
        s32 desired = primary <= 7 ? 0x2000 : -0x2000;

        if (facing[0] != desired)
            facing[0] = desired;
    }

    Func_080c10e8(0, 0);
    Func_08015130(world[0x41] & ~1);
    primary_object = Func_080b7dd0(primary)->object;
    *(volatile u16 *)0x04000050 = 0x3f40;

    actor_count = Func_080b6c08(3, actors);
    for (i = 0; i < actor_count; i++) {
        u16 actor = actors[i];

        if (actor == 0xfe)
            continue;
        if (actor == primary) {
            Func_08009080(primary_object, 3);
        } else if (IsLowSide_080b9ec0(actor) !=
                   IsLowSide_080b9ec0(secondary)) {
            Func_080c0f98(actor, 1);
        }
    }

    Func_080f9010(0x9a);
    Func_080c1798(work.primary_id, input->transition_value, 0, 0);
    if ((options & 1) != 0)
        Func_080c0f98(primary, 1);

    for (i = 0; i < 16; i++) {
        *(volatile u16 *)0x04000052 = (u16)(0x1000 | (16 - i));
        Func_080030f8(1);
    }

    if (input->message_mode != 0) {
        if (input->message_mode == 1) {
            Func_080bbabc(0, primary);
            Func_080bbabc(4, 0x856);
        } else {
            Func_080bbabc(4, 0x855);
        }
        Func_080bb938();
        Func_080c1a14();
    } else {
        u32 filtered_count = 0;

        for (i = 0; i < actor_count; i++) {
            u16 actor = actors[i];

            if (actor == 0xfe)
                continue;
            if (actor == primary) {
                if ((options & 1) == 0)
                    actors[filtered_count++] = primary;
            } else if (IsLowSide_080b9ec0(actor) ==
                       IsLowSide_080b9ec0(secondary)) {
                actors[filtered_count++] = actor;
            }
        }
        actors[filtered_count] = 0xff;
        Func_080b7b6c(actors, 0);

        filtered_count = 0;
        if (input->count > 0) {
            for (i = 0; i < (u32)input->count; i++)
                actors[filtered_count++] = input->ids[i];
        }
        actors[filtered_count] = 0xff;

        for (i = 0; i < (u32)work.count; i++) {
            Record_080b9ec0 *record = Func_080b7f70(
                Func_080b7dd0(work.members[i])->object,
                0);
            s32 children = record->child_count - 1;
            s32 child;

            for (child = 0; child < children; child++) {
                FIELD(&work, u8, 0x34 + i * 4 + child) =
                    record->children[child]->value;
            }
        }

        work.secondary_is_low_id = secondary <= 7;
        if ((input->flags & 0x20000) != 0)
            work.secondary_is_low_id ^= 1;

        Func_080041d8(0x080bd899, 0x0c80);
        if ((input->flags & 0x8000) != 0)
            Func_080c9010(&work);
        else if ((input->flags & 0x4000) != 0)
            Func_080c9008(&work);
        else
            Func_080c9018(&work);
        Func_080be02c();
    }

    Func_080b6c90();
    actor_count = Func_080b6c08(3, actors);
    *(volatile u16 *)0x04000050 = 0x3f40;
    for (i = 0; i < actor_count; i++) {
        u16 actor = actors[i];

        if (actor != 0xfe &&
            actor != primary &&
            IsLowSide_080b9ec0(actor) !=
                IsLowSide_080b9ec0(secondary)) {
            Func_080c0f98(actor, 1);
        }
    }

    for (i = 0; i < 16; i++) {
        *(volatile u16 *)0x04000052 = (u16)(0x1000 | i);
        Func_080030f8(1);
    }
    for (i = 0; i < actor_count; i++)
        Func_080c0f98(actors[i], 0);

    Func_080c0cec(0, 0, 0, 100);
    Func_080030f8(1);
    return 0;
}

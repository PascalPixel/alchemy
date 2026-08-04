#include "types.h"

struct EncounterPattern_080c1ffc {
    u8 order;
    u8 category[5];
    u8 initial_count[5];
    u8 desired_count[5];
};

struct EncounterState_080c1ffc {
    u8 unknown_00[0x3c];
    u16 remaining;
    u16 current;
    u8 active;
    u8 unknown_41;
    u8 order;
};

typedef void (*InitializeOwner_080c1ffc)(void *, s32);

extern struct EncounterState_080c1ffc *Data_03001e74;

s32 Func_080022ec(s32, s32);
u32 Func_08004458(void);
void *Func_08077008(s32);
s32 Func_080770c0(s32);
void Func_08077140(s32, s32, s32);
s16 Func_080c1afc(s32 *);
void Func_080c1c54(s32, s32);
s32 Func_080c1df4(s32, s32);
s32 Func_080c1f50(s32);
s32 Func_080c23c0(s32);

s32 Func_080c1ffc(s32 pattern_id)
{
    static const struct EncounterPattern_080c1ffc *const patterns =
        (const struct EncounterPattern_080c1ffc *)0x080c5c38;
    InitializeOwner_080c1ffc initialize_owner =
        (InitializeOwner_080c1ffc)0x03000164;
    struct EncounterState_080c1ffc *state = Data_03001e74;
    const struct EncounterPattern_080c1ffc *pattern;
    s32 count[5];
    s32 opportunity[5];
    s32 available[5];
    u16 selected[14];
    s32 selection_count = 0;
    s32 difficulty_delta = 0;
    s32 budget = 6;
    s32 index;

    state->active = 0;
    if (Func_080770c0(0x173) != 0)
        pattern_id = Func_080c1afc(&difficulty_delta);
    if ((u32)pattern_id >= 0x17c)
        pattern_id = 1;

    pattern = &patterns[pattern_id];
    for (index = 0; index < 5; index++) {
        if (pattern->initial_count[index] != 0)
            break;
    }
    if (index == 5)
        pattern = &patterns[1];

    for (index = 0; index < 5; index++) {
        if (pattern->initial_count[index] != 0) {
            s32 cost =
                2 - !!Func_080c23c0(pattern->category[index] + 8);

            budget -= cost * pattern->initial_count[index];
        }
    }

    for (index = 0; index < 5; index++) {
        s32 extra =
            pattern->desired_count[index] - pattern->initial_count[index];

        count[index] = pattern->initial_count[index];
        if (extra > 0) {
            s32 cost =
                2 - !!Func_080c23c0(pattern->category[index] + 8);
            s32 affordable = Func_080022ec(budget, cost);

            if (affordable < extra)
                extra = affordable;
            opportunity[index] =
                ((u32)(extra + 1) * Func_08004458()) >> 16;
        } else {
            opportunity[index] = 0;
        }
    }

    for (;;) {
        s32 changed = 0;

        for (index = 0; index < 5; index++) {
            s32 cost;

            if (opportunity[index] == 0)
                continue;
            cost = 2 - !!Func_080c23c0(pattern->category[index] + 8);
            if (cost > budget) {
                opportunity[index] = 0;
                continue;
            }
            count[index]++;
            opportunity[index]--;
            budget -= cost;
            changed = 1;
        }
        if (!changed)
            break;
    }

    state->order = pattern->order;
    if (state->order == 0) {
        s32 order[5];

        for (index = 0; index < 5; index++)
            order[index] = index;
        for (index = 0; index < 10; index++) {
            s32 first = ((u32)(Func_08004458() * 5)) >> 16;
            s32 second = ((u32)(Func_08004458() * 5)) >> 16;
            s32 saved = order[first];

            order[first] = order[second];
            order[second] = saved;
        }
        for (index = 0; index < 5; index++) {
            s32 category = order[index];
            s32 repeat;

            for (repeat = 0; repeat < count[category]; repeat++)
                selected[selection_count++] = pattern->category[category] + 8;
        }
    } else if (state->order == 1) {
        for (;;) {
            s32 available_count = 0;
            s32 category;

            for (index = 0; index < 5; index++) {
                if (count[index] != 0)
                    available[available_count++] = index;
            }
            if (available_count == 0)
                break;
            category =
                available[((u32)(available_count * Func_08004458())) >> 16];
            selected[selection_count++] = pattern->category[category] + 8;
            count[category]--;
        }
    } else {
        for (index = 0; index < 5; index++) {
            s32 repeat;

            for (repeat = 0; repeat < count[index]; repeat++)
                selected[selection_count++] = pattern->category[index] + 8;
        }
    }

    selected[selection_count] = 0;
    state->remaining = 6;
    state->current = 0;

    /*
     * 080072f8 is _call_via_r5 here.  The caller fixes r5 to the resident
     * two-argument owner initializer at 03000164 before clearing slots 80..85.
     */
    for (index = 0x80; index <= 0x85; index++)
        initialize_owner(Func_08077008(index), 0x14c);

    for (index = 0; index < 6 && selected[index] != 0; index++) {
        s32 flags = Func_080c1df4(selected[index], 1);
        s32 owner = index + 0x80;

        if (flags & 0x8000)
            Func_080c1f50(selected[index]);
        Func_08077140(owner, selected[index], flags & 0x7fff);
        Func_08077008(owner);
        if (difficulty_delta != 0)
            Func_080c1c54(owner, difficulty_delta);
    }

    return index;
}

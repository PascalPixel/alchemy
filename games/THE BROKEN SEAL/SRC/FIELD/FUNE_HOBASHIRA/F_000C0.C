#include "TYPES.H"
#include "FIELD_EVENT.H"

/* A swaying object: the actor record with its drift speed and the two sway
 * states (0 rising, 1 falling, 9 stopped) in the free words. */
struct SwayActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    u8 unknown_10[0x3c];
    s32 drift;
    u8 unknown_50[0x14];
    s16 x_state;
    s16 y_state;
};

/* Sways the object back and forth by a random drift within its range and
 * bobs it up and down between heights 0 and one cell. */
s32 FuneHobashira_Func020000c0(struct SwayActor *actor)
{
    s16 *state = &actor->x_state;
    s32 next;

    if (*state == 9) {
        actor->drift = 0;
    } else if (*state != 0) {
        actor->drift -= (u32)(Engine_RandomNext() << 11) >> 16;
        if (actor->drift < -0xc000) {
            next = 0;
            *state = next;
        }
    } else {
        actor->drift += (u32)(Engine_RandomNext() << 11) >> 16;
        if (actor->drift > 0xc000) {
            next = 1;
            *state = next;
        }
    }
    if (actor->x > 0x280000 && actor->x < 0x1400000) {
        actor->x += actor->drift;
    }
    state = &actor->y_state;
    if (*state == 9) {
        actor->y = 0;
    } else if (*state != 0) {
        actor->y -= (u32)(Engine_RandomNext() * 3 << 14) >> 16;
        if (actor->y < 0) {
            next = 0;
            *state = next;
        }
    } else {
        actor->y += (u32)(Engine_RandomNext() * 3 << 14) >> 16;
        if (actor->y > 0x100000) {
            next = 1;
            *state = next;
        }
    }
    return 1;
}

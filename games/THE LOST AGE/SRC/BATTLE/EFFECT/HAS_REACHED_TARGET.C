#include "TYPES.H"

/* EffectSlot_SetPosition's slot, as far as this test reads it. The Spanish
   and Italian editions call this place through their own far stub, so it
   stays a file of its own beside SLOT_SET_POSITION.C. */
struct EffectSlot {
    /* 0x00 */ void *object;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 z;
    /* 0x0c */ s32 target_x;
    /* 0x10 */ s32 target_z;
    /* 0x14 */ s32 origin_x;
    /* 0x18 */ s32 origin_z;
    /* 0x1c */ s32 speed;
    /* 0x20 */ u8 unknown_20[0x21];
    /* 0x41 */ s8 moving;
};

/* Whether a moving effect slot has a target to reach: ☀️'s test. */
u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
    u32 value;

    if (effect->moving == 0) {
        return 0;
    }
    value = (u32)effect->target_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}

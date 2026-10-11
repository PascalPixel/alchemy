/* Near miss (4 instructions differ, order only): the native loads the
 * pooled zero for kind after the two halfword stores and before state;
 * the draft loads it after the state store. One-pass blocks move it too
 * early. The zero is pooled because a byte constant stored through this
 * struct field loads as a literal. */
#include "types.h"

struct Ov676Actor {
    u8 unknown_00[98];
    u8 state;
    u8 kind;
    s16 timer;
    s16 count;
    u8 unknown_68[4];
    void (*update)(struct Ov676Actor *actor);
};

struct Ov676Actor *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Object_SetModeById(u32 id, s32 mode);

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

void Func_02001904(s32 id)
{
    struct Ov676Actor *actor = Object_GetById(id);

    ObjectMotion_SetSpeedParameters_(id, 0x13333, 0x13333);
    Object_SetModeById(id, 2);
    actor->timer = 0;
    actor->count = 0;
    actor->state = 1;
    actor->kind = 0;
}

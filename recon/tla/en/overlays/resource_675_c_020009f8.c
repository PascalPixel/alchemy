/* Near miss, differ 4 of 103: The Broken Seal's
 * StagedActor_PlacePairAtOffsetAndRun with this overlay's stubs. The native
 * spills dx before dz (str r1, [sp, #4]; str r2, [sp, #0]) after the party
 * state's pool load; this spills dz first. Measured: an empty asm barrier or
 * a one-pass block at entry gives the native spill order but copies the id
 * to r5 before the pool load, which the native does after the offset is
 * added (differ 4). */
#include "types.h"
#include "party_state.h"

struct FieldActor {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x1c];
    s32 speed;
    s32 acceleration;
};

struct FieldActor *Object_GetById(s32 id);
void Func_02001254(void);
void Func_02001324(s32 skip);
void Func_0200121c(struct FieldActor *actor, s32 x, s32 y, s32 z);
void Func_02001214(struct FieldActor *actor, s32 mode);
void Func_02001224(struct FieldActor *actor);
void Func_02001344(s32 cue);
void Func_0200125c(void);

void Func_020009f8(s32 actor_id, s32 dx, s32 dz)
{
    struct FieldActor *leader;
    struct FieldActor *actor;
    s32 x;
    s32 z;

    leader = Object_GetById(gPartyState.current_owner);
    actor = Object_GetById(actor_id);
    Func_02001254();
    Func_02001324(0);
    {
        x = ((leader->x + (dx << 16)) & 0xfff00000) + 0x80000;
        z = ((leader->z + (dz << 16)) & 0xfff00000) + 0x80000;

        leader->speed = 0x10000;
        leader->acceleration = 0x8000;
        Func_0200121c(leader, x, leader->y, z);
    }
    Func_02001214(leader, 27);
    {
        x = ((actor->x + (dx << 16)) & 0xfff00000) + 0x80000;
        z = ((actor->z + (dz << 16)) & 0xfff00000) + 0x80000;

        actor->speed = 0x10000;
        actor->acceleration = 0x8000;
        Func_0200121c(actor, x, actor->y, z);
    }
    if (dx < 0 || dz < 0) {
        Func_02001214(actor, 4);
    } else {
        Func_02001214(actor, 3);
    }
    Func_02001344(226);
    Func_02001224(leader);
    Func_02001214(actor, 2);
    Func_02001344(298);
    Func_0200125c();
}

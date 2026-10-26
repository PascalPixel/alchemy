// Under -mthumb-slow-loads (2026-10-26): differ 2 of 103; left: the pool load
// still rises above sub sp, as recorded below.
// Near miss, differ 2 of 103: The Broken Seal's
// StagedActor_PlacePairAtOffsetAndRun with this overlay's stubs. The native
// order is sub sp; ldr r3, =gPartyState; str r1, [sp, #4]; str r2, [sp];
// movs r2, #133 ... adds r5, r0, #0. Plain (differ 4) dz is spilled first
// and dx after the offset is built. With the loop note after the party
// address and the id pinned to r5 after it (below) the spills and the id
// copy fall into place but the pool load rises above sub sp (differ 2).
// Tried 2026-10-15: a second note before the address (the load then
// follows both spills), an asm barrier instead of the note (the same),
// dx and dz copied to locals after the note (the copies coalesce with the
// parameters, whose spills stay at entry), the parameters pinned to r1 and
// r2 (dx moves to r8: differ 32); permute 3 minutes on the plain form found
// nothing below 4.
#include "types.h"
#include "party_state.h"

struct FieldActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
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

void Func_020009f8(s32 actorId, s32 dx, s32 dz) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 x;
  s32 z;
  struct PartyState *party;
  s32 *owner;

  party = &gPartyState;
  do {
  } while (0);
  owner = &party->currentOwner;
  {
    register s32 id asm("r5") = actorId;

    leader = Object_GetById(*owner);
    actor = Object_GetById(id);
  }
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

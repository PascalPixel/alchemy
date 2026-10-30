// Near miss, differ 2 of 103 (tla-en, 2026-10-30), now with no device: The
// Broken Seal's StagedActor_PlacePairAtOffsetAndRun with this overlay's stubs.
// Left: the parameter spills. The native order is sub sp; ldr r3,
// =gMainRoutineWork; str r1, [sp, #4]; str r2, [sp]; movs r2, #133; here dx's
// spill (str r1) falls after movs r2, #133. In sched2, dz's spill takes its
// priority from the anti dependence on movs r2, #133 (174), dx's only from
// the currentOwner load it may alias (-mthumb-slow-loads's store rule, cost 2:
// 173), so dz's spill and the constant go first; the native's order needs the
// two spills tied, as the parameter order then breaks the tie.
// The earlier form (a one-pass block after the party address and the id pinned
// to r5) tied them, through the note's dependences, but the note also made the
// pool load outrank sub sp (177 against 175), so it rose above it: differ 2
// either way, and the plain C needs no device. Tried before: a second note
// before the address (the load then follows both spills), an asm barrier
// instead of the note (the same), dx and dz copied to locals after the note
// (the copies coalesce with the parameters, whose spills stay at entry), the
// parameters pinned to r1 and r2 (dx moves to r8: differ 32); &dx taken (12);
// the owner read through a party pointer or a pointer to currentOwner (2, the
// same); permute 3 minutes on the plain form found nothing below 4 under the
// old options, and 4 minutes on this form (21,000 candidates, 2026-10-30)
// nothing below 2.
#include "types.h"
#include "main_routine_work.h"

struct FieldActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x1c];
  s32 speed;
  s32 acceleration;
};

struct FieldActor *Actor_GetById(s32 id);
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

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  actor = Actor_GetById(actorId);
  Func_02001254();
  Func_02001324(0);
  x = ((leader->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((leader->z + (dz << 16)) & 0xfff00000) + 0x80000;
  leader->speed = 0x10000;
  leader->acceleration = 0x8000;
  Func_0200121c(leader, x, leader->y, z);
  Func_02001214(leader, 27);
  x = ((actor->x + (dx << 16)) & 0xfff00000) + 0x80000;
  z = ((actor->z + (dz << 16)) & 0xfff00000) + 0x80000;
  actor->speed = 0x10000;
  actor->acceleration = 0x8000;
  Func_0200121c(actor, x, actor->y, z);
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

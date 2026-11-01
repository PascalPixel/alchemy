// Matches: differ 0 (tla-en, 2026-11-01)
// Twin of The Broken Seal's LamakanDesert_SlideDownDune, and overlay 67b's copy
// of overlay 679's Func_020011a8 (the same listing). The same probes, in the
// same order: 2 pixels ahead, a fall at a ledge with cue 0x102
// (ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE there), 8 pixels ahead, both
// diagonals at 5.7 pixels, then the leader and anchor advance together. Here
// the sand carries the leader 2 pixels and turns it south, and Ov67bRoom1
// leaves a grounded leader alone.
// The last pair (movs r1, #129; mov r0, r8; lsls r1, r1, #1) closed with the
// twin's own form: event.h's Actor_SetAttachedEffect is an inline, whose
// parameter takes r0 before the constant loads. The Lost Age has no event.h
// yet, so the inline stands here over its import; it is that layer, S2's
// source and no device, and belongs in a header on adoption. The other forms
// tried (a direct call, wider and narrower prototypes, a cue local, a block
// copy of the owner) all left that pair.
#include "../../../../games/tla/src/scene/apojii_islands/local.h"
#include "scene_ids.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

void Func_020029f8(s32 id, s32 effect);

static inline void Actor_SetAttachedEffect(s32 actorId, s32 effect) {
  Func_020029f8(actorId, effect);
}

void Func_02001e00(void) {
  struct Actor *anchor;
  s32 owner;
  struct Actor *leader;
  s32 position[3];
  s32 result;

  {
    struct EventWork1cc *work = Ram_HeapSlots->eventWork;

    owner = gMainRoutineWork.currentOwner;
    anchor = work->object;
  }
  leader = Actor_GetById(owner);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov67bRoom1 &&
      leader->terrainHeight == 0)
    return;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x20000;
  result = Func_02002970(leader, position);
  if ((gFrameCount & 4) == 0)
    Func_02001d30(leader);
  if (result < 0) {
    Actor_SetAttachedEffect(owner, 0x102);
    Func_02002930(leader, leader->x, leader->y, leader->z + 0x80000);
    Func_02002910(leader, 49);
    Func_02002938(leader);
    do {
      wait(1);
    } while (leader->y != leader->terrainHeight);
    Func_02001d30(leader);
    Func_02002910(leader, 49);
    wait(3);
    return;
  }
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x80000;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x + 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  position[0] = leader->x - 0x5b333;
  position[1] = leader->y;
  position[2] = leader->z + 0x5b333;
  result = Func_02002970(leader, position);
  if (result > 0)
    return;
  anchor->z += 0x20000;
  leader->z += 0x20000;
  leader->angle = 0x4000;
}

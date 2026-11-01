// Draft, differ 84 of 145, skeleton gap 13/15 (tla-en, 2026-11-01)
// Overlay 6ad's Func_02002624, the twin of overlay 6aa's Func_020022e4: unless
// flag 0x109 is set it records up to four objects from a 0-ended list of ids
// and flags as the beam's targets and up to four stops from a second list,
// then sets the beam's two values, the blend if none is set, and adds
// Func_0200242c as a task. Control flow, loads and constants agree: the stop
// pointer is loop.c's reduction of stops[i], and the blend values are s32
// locals because a halfword constant comes from the pool, unsplit.
// Left is one allocation fact: the listing keeps id in sl and stops in r8 and
// leaves height on the stack, with no r4 at all; here id takes r4, saved around
// calls (10 refs over 2 calls), and height gets fp. Calls crossed decide it: an
// extra call anywhere before the mask of flags gives id 3 calls and stops 5
// (stops has 17 refs, more than 4 calls allow), and then every register agrees
// and the gap is that call plus "movs #16" moving (differ 84 is the same but
// the gap falls to 2/4). So the source seems to have one more call site, or
// two fewer refs, that does not reach the listing; nothing found shows which.
// Tried without result: a goto loop (refs are weighted by CFG depth, so 10
// again), a loaded value copied into id (copy propagation removes it; kept at
// the loop bottom only, id leaves r4 but stops takes it), separate counters
// (worse: fp holds both as one variable), both arms of the 0x80 test with the
// tail repeated (cross-jumping does not merge them), and a dead test on flags
// (folded before life, so it counts no call). The remaining pairs (object + 98
// then - 9 is right; the second list's i = 0 runs late; the pool load before
// the lsls of 0x480) should be read again once that is settled.
#include "types.h"
#include "io_reg.h"
#include "actor.h"

/// One object the beam reaches, as Func_02002624 records it.
struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct Actor *object;
  s32 height; ///< 16.16 pixels
};

struct MarsLighthouseBeam {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  struct {
    u16 unk00;
    u16 unk02;
  } stops[4];
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_0200242c(void);

void Func_02002624(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;
  s32 value;
  s32 one;
  s32 flags;
  s32 height;
  struct Actor *object;
  s32 alpha;
  s32 control;

  if (GameFlag_Test(0x109) == 0) {
    for (i = 0; i < 4 && (id = *targets++) > 0; i++) {
      flags = *targets;
      height = flags & 15;
      flags &= 0xf0;
      object = Actor_GetById(id);
      targets++;
      if (object != NULL) {
        Actor_SetSingleChildFlags(object, 0);
        ((u8 *)object)[98] = one = 1;
        if ((flags & 0x80) == 0)
          object->collisionFlags = 0;
        target->active = one;
        target->flags = flags;
        target->object = Actor_GetById(id);
        target->height = height << 16;
        target->index = i;
        target++;
      }
    }
    if (stops != NULL) {
      for (i = 0; i < 4; i++) {
        value = *stops++;
        beam->stops[i].unk00 = value;
        if (value <= 0)
          break;
        beam->stops[i].unk02 = *stops++;
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    alpha = 0xc08;
    REG_BLDALPHA = alpha;
    control = 0x3f10;
    REG_BLDCNT = control;
  }
  Task_Add(Func_0200242c, 0x480);
}

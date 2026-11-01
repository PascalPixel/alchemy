// Matches: differ 0 (tla-en, 2026-11-01)
// Overlay 6ab's Func_02001df0, the twin of 6aa's Func_020022e4 and 6ac's
// Func_0200218c: unless flag 0x109 is set it records up to four objects from a
// list of ids and flags as the beam's targets and up to four stops from a second
// list, then sets the beam's two values, the blend if none is set, and
// Func_02001bf8 as a task.
// The two dead tests are the point of the shape. The native's allocation is what
// global.c gives with every REG_N_REFS unweighted by loop depth: priority order
// object, targets, flags, stops, id, target, i, height, and nothing takes r4 as a
// caller save (weighted, stops, id and i all pass 4 * calls < refs). Each test is
// an edge into the middle of a loop that exists at flow, so the cycle has no
// natural header and depth stays 0; combine then folds it and no code remains.
// The weights come from flow_loops_find, which sees only this CFG. Plain C does
// not reach this: without the tests differ 82. flow.c also leaves refs unweighted
// under optimize_size, which would explain the listing with no device; that is a
// compiler question (K3) for Pascal, not settled here.
// The bodies must stay non-empty, so the fold is a taken jump (an lt jump does not
// fold), and the mask must be one CSE cannot reuse, so the temporary dies at it.
// stopOffset is a one-constant local: it gives the native's reg+reg strh. Separate
// alpha and control locals give its r3 and r2 in the blend.
#include "types.h"
#include "io_reg.h"
#include "actor.h"

/// One object the beam reaches, as Func_02001df0 records it.
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
  u16 stopCount;
  u16 stops[7];
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02001bf8(void);

void Func_02001df0(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;

  if (GameFlag_Test(0x109) == 0) {
    i = 0;
    id = *targets++;
    {
      /* FAKEMATCH(dead): a test combine folds, keeping the first loop's refs
       * unweighted; as no test, 90 of 145 instructions differ in tla-en. */
      s32 never = id & 0x40;
      if (never < 0) {
        id = 0;
        goto afterTarget;
      }
    }
    if (id > 0) {
    nextTarget:
      {
        s32 flags = *targets;
        s32 height = flags & 15;
        struct Actor *object;
        s32 one;

        flags &= 0xf0;
        object = Actor_GetById(id);
        targets++;
        if (object != NULL) {
          Actor_SetSingleChildFlags(object, 0);
          ((u8 *)object)[98] = one = 1;
          if ((flags & 0x80) == 0)
            object->collisionFlags = 0;
          /* FAKEMATCH(one-pass): the record in a once-only block, which keeps
           * target++ after its stores; as plain statements, 2 of 145
           * instructions differ in tla-en. */
          do {
            target->active = one;
            target->flags = flags;
            target->object = Actor_GetById(id);
            target->height = height << 16;
            target->index = i;
          } while (0);
          target++;
        }
      }
    afterTarget:
      i++;
      if (i <= 3) {
        if ((id = *targets++) > 0)
          goto nextTarget;
      }
    }
    if (stops != NULL) {
      s32 count;
      s32 stopOffset;
      u16 *p;

      i = 0;
      count = *stops++;
      stopOffset = 84;
      *(u16 *)((u8 *)beam + stopOffset) = count;
      {
        /* FAKEMATCH(dead): a test combine folds, keeping the second loop's refs
         * unweighted; as no test, 133 of 145 instructions differ in tla-en. */
        s32 never = count & 0x40;
        if (never < 0) {
          p = stops;
          goto afterStop;
        }
      }
      if (count > 0) {
        p = &beam->stopCount;
      nextStop:
        p[1] = *stops;
      afterStop:
        stops++;
        i++;
        p += 2;
        if (i <= 3) {
          s32 stop = *stops++;

          *p = stop;
          if (stop > 0)
            goto nextStop;
        }
      }
    }
  }
  beam->unk10 = a2;
  beam->unk0a = a3;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 control = 0x3f10;
      REG_BLDCNT = control;
    }
  }
  Task_Add(Func_02001bf8, 0x480);
}

// Draft, differ 28 of 145 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Overlay 6ac's Func_0200218c, the twin of overlay 6aa's Func_020022e4.
// Left: one allocation, and nearly every difference follows from it. The native
// uses no r4: stops r8, id sl, target r9, i fp, height on the stack at sp+0. Here
// id takes r4 with caller-saves around each call (10 refs, 2 calls crossed, so
// 4 * calls < refs), height then gets fp, and reload's scratch registers rotate
// through the first list. The second list and the tail already match instruction
// for instruction apart from fp/sl. Spelling id's read as statements instead
// hands r4 to stops (17 refs, 4 calls) and gives differ 133.
// Proven: the second list is a real do-while over beam->stops[i], whose reduced
// giv is the native r2; indexing the first store by i (known 0) gives the native
// movs r3, #84; strh r2, [r1, r3], where a constant index shares the address and
// cross-jumps; the entry and loop values need separate locals (r2, r3); the blend
// is 0xc08, not 0x608, in one-constant locals, each in its own block as in
// aqua_rock_inside/s3ab0.c (sharing one block gives differ 114).
// Also seen: the native's target outranks i, which here it does not (i 16 refs
// over 120 insns, target 15 over 96), and its strh of 1 reuses the movs r1, #1
// made for byte 98, across the if; that may settle with the allocation.
// Tried without gain: a temporary compared in id's place (two give id 8 refs, but
// stops then takes r4 and the loop tail cross-jumps), the tail written in both
// arms of the 0x80 test (target takes r4, differ 146), declaration order, id in
// an inner block, height outer, u32 flags, the while and do-while and goto forms
// of the first list (identical code). For either r4 candidate to refuse, id needs
// 8 refs or a third call crossed, and stops 16 refs or a fifth; no plain source
// found gives both.
#include "types.h"
#include "call.h"
#include "io_reg.h"
#include "actor.h"

struct MarsLighthouseBeamTarget {
  u16 active;
  u16 index;
  u16 flags;
  u16 unk06;
  struct Actor *object;
  s32 height; ///< 16.16 pixels
};

/// One pair of the beam's second list.
struct MarsLighthouseBeamStop {
  u16 unk00;
  u16 unk02;
};

struct MarsLighthouseBeam {
  u8 unk00[10];
  u16 unk0a;
  u8 unk0c[4];
  s32 unk10;
  struct MarsLighthouseBeamTarget targets[4];
  struct MarsLighthouseBeamStop stops[4];
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Task_Add(void (*func)(void), s32 priority);
void Func_02001f94(void);

void Func_0200218c(u16 *targets, u16 *stops, s32 a2, s32 a3) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target = beam->targets;
  s32 id;
  s32 i;

  if (GameFlag_Test(0x109) == 0) {
    i = 0;
    while ((id = *targets++) > 0) {
      s32 flags = *targets;
      s32 height = flags & 15;
      struct Actor *object;

      flags &= 0xf0;
      object = Actor_GetById(id);
      targets++;
      if (object != NULL) {
        Actor_SetSingleChildFlags(object, 0);
        ((u8 *)object)[98] = 1;
        if ((flags & 0x80) == 0)
          object->collisionFlags = 0;
        target->active = 1;
        target->flags = flags;
        target->object = Actor_GetById(id);
        target->height = height << 16;
        target->index = i;
        target++;
      }
      i++;
      if (i > 3)
        break;
    }
    if (stops != NULL) {
      s32 count;
      s32 value;

      i = 0;
      count = *stops++;
      beam->stops[i].unk00 = count;
      if (count > 0) {
        do {
          beam->stops[i].unk02 = *stops++;
          i++;
          if (i > 3)
            break;
          value = *stops++;
          beam->stops[i].unk00 = value;
        } while (value > 0);
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
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  ((s32 (*)())Task_Add)(Func_02001f94, 0x480);
}

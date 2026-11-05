// Measured under -Os, 5 November 2026: differ 101 (tla-en); notes below may predate it.
// Draft, differ 197 of 280, skeleton gap 21 (tla-en, 2026-11-01): control flow,
// calls, fields and constants are the native's; what remains is allocation.
// Re-measured with the 1 November compiler: unchanged, so none of it was
// slow-loads, and this draft carries no device to remove. The native keeps dx in
// r11, dz in r9, &spark in r10, &gFrameCount (then gFrameCount & 4) in r7, and
// 0xb0000 in r5 once per arm, spilling gFrameCount & 3 to sp+16. This draft has
// &spark r7, gFrameCount & 3 r10, &gFrameCount r9, dz r11, and spills dx (sp+24,
// shifting targetX and targetZ down a slot), with 0xb0000 rebuilt per call.
// Three causes found, in the .08.loop and .17.lreg dumps:
// 1. 0xb0000: loop.c hoists it from the inner loop (savings 2, life 31), so it
//    gets no register. It stays in the arms only with neither loop valid for
//    loop.c, and then it goes to local-alloc and takes r5 as the native's does.
//    Plain outer gotos do not work: cleanup_cfg moves the body after the test,
//    because the body has one predecessor and ends in a jump (differ 269). What
//    works is `goto outerTest; do { ...; goto innerTest; innerTop: ...; innerTest:
//    if (Func_02001f9c(actor) == 0) goto innerTop; outerTest:; } while (...)`,
//    the jump in from outside making the do-while invalid for loop.c. Alone that
//    is differ 255, gap 25.
// 2. r6 and r7: with (1) and `flip = gFrameCount & 4` read before x0, y0 and z0,
//    flip's live length goes 24 to 27, pos outranks it, and pos r6, flip r7 and
//    &gFrameCount r7 all land. Differ 221, gap 25: the whole arms then line up
//    apart from what (3) leaves. flip alone with real loops: 199, gap 27.
// 3. Still open at that point: &spark ranks above actor (1.10 against 1.00, so
//    r8 and r10 are swapped; it needs live length 108, it has 98, or one ref
//    fewer) and gFrameCount & 3 ranks above dz and dx (0.56 against 0.17), taking
//    r9. Its reads are 24 weighted refs over 172 insns, so priority cannot put it
//    under dx unless refs or live length differ by three times, which no form
//    tried does. Likely reload evicts it from a low register in the native
//    (r5 is the spill register all through there), which would also change who
//    gets r10. The kept form is the one with the best measure, (1) and (2) being
//    worse until (3) closes; start from them.
// Tried, no change from (2): gFrameCount & 3 as a user local, as `phase = 3;
// phase &= gFrameCount`, gFrameCount in a local, `sp = &spark`, flip declared
// first. Worse: dx and dz declared first (229), reading the arguments in the arms
// (243), one `size` local set in both arms (213), goto on the inner loop only (no
// change, the outer loop hoists it), `s32 size` per arm (no change, hoisted).
// In the offset block the native gives cosine * 12 r3 and the first actor->x
// load r1; here the load takes r3. Names are still placeholders.
#include "actor.h"
#include "ram_buffer.h"
#include "types.h"

struct Spark {
  s32 mode;
  s32 kind;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[0x18];
};

struct EventWork {
  u8 unk000[0x154];
  s16 unk154;
};

extern u32 gFrameCount;
struct Actor *Actor_GetById(s32 id);
void Func_02001fac(void);
void Func_02001fb4(void);
void Func_02002044(s32 skip);
void Func_02002054(s32 cue);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
s32 Func_0200034c(u32 layer, s32 x, s32 y);
void Func_02001f6c(struct Actor *actor, s32 x, s32 y, s32 z);
s32 Func_02001f9c(struct Actor *actor);
void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d,
                   struct Spark *spark);
void wait(s32 frames);

void Func_02000818(void) {
  s32 pos[4];
  struct Spark spark;
  struct EventWork *work;
  struct Actor *actor;
  /* FAKEMATCH(pin): angle in r7, as the native keeps it; as a plain local,
   * 136 of 280 instructions differ in tla-en. */
  register s16 angle asm("r7");
  s32 x, z, dx, dz, targetX, targetZ;
  s32 cosine, sine;
  s32 x0, y0, z0;

  work = Ram_HeapSlots->eventWork;
  actor = Actor_GetById(8);
  angle = (work->unk154 - 45) << 14;
  Func_02001fac();
  Func_02002044(0);
  x = actor->x >> 20;
  z = actor->z >> 20;
  dx = Math_Cosine(angle) >> 16;
  dz = Math_Sine(angle) >> 16;
  actor->speedLimit = 0x40000;
  actor->acceleration = 0x40000;
  targetZ = 0;
  targetX = 0;
  if (Func_0200034c(1, x + (dz < 0 ? -dz : dz), z + (dx < 0 ? -dx : dx)) >> 8 ==
      work->unk154) {
    targetX = (dz < 0 ? -dz : dz) * 15 << 16;
    targetZ = (dx < 0 ? -dx : dx) * 15 << 16;
  }
  targetX += x << 20;
  targetZ += z << 20;
  while (Func_0200034c(1, x, z) >> 8 == work->unk154) {
    x += dx;
    z += dz;
    targetX += dx << 20;
    targetZ += dz << 20;
    Func_02001f6c(actor, targetX, actor->y, targetZ);
    while (Func_02001f9c(actor) == 0) {
      if ((gFrameCount & 3) == 0) {
        if ((gFrameCount & 10) == 0) {
          Func_02002054(0x255);
        }
        spark.mode = 2;
        spark.kind = 7;
        spark.scaleX = 0x16666;
        spark.scaleY = 0x13333;
        cosine = Math_Cosine(actor->angle);
        sine = Math_Sine(actor->angle);
        pos[0] = cosine * 12;
        pos[1] = sine * 12;
        pos[2] = -(cosine * 12);
        pos[3] = -(sine * 12);
        pos[0] += actor->x;
        pos[1] += actor->z;
        pos[2] += actor->x;
        pos[3] += actor->z;
        if (pos[1] != actor->z) {
          pos[1] -= 0xc0000;
          pos[3] -= 0xc0000;
        }
        y0 = actor->y;
        x0 = pos[0];
        z0 = pos[1];
        if (gFrameCount & 4) {
          Func_0200015c(x0 + 0x40000, y0, z0, 0x8000, 0, 0, 0xb0000, &spark);
          Func_0200015c(pos[2] - 0x40000, actor->y, pos[3], -0x8000, 0, 0,
                        0xb0000, &spark);
        } else {
          Func_0200015c(x0 - 0x40000, y0, z0, -0x8000, 0, 0, 0xb0000, &spark);
          Func_0200015c(pos[2] + 0x40000, actor->y, pos[3], 0x8000, 0, 0,
                        0xb0000, &spark);
        }
      }
      wait(1);
    }
  }
  Func_02001fb4();
}

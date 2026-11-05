// Draft, differ 206 of 315, skeleton gap 54 (tla-en, 2026-11-01): control flow,
// calls, the frame (88 bytes, same slots) and memory accesses follow the
// listing; what is left comes from two things, both GCSE's. The listing
// materialises center three times (`add r1, sp, #76` before the switch, `add
// r5, sp, #76` after it, `add r3, sp, #76` / r11 before Func_0200034c, each used
// to its own last read) and keeps 0x8000 in r11 across the flip test. Here PRE
// makes the later addresses copies and cprop folds them into one pseudo live
// through the function (r7), and cprop puts the constant back at each use.
// Measured with the build's options: explicit pointers p = center and q = center
// at those sites leave the output unchanged (differ 206), and a `speed` local
// gives differ 204 but gap 60. The same pointers and local with -fno-gcse, as a
// diagnostic only, do give the three addresses, r5 and r11; about 100 lines
// still differ there (pos r7, not r6; IwramMulQ16 r6, not r8), so that option is
// not shown to close it. sfp is never killed, so no plain C form keeps (plus sfp
// 76) from PRE, and no device can: a tagged local is undone the same way. This
// is a compiler question (79 listing hits of a repeated `add rN, sp, #K`, none
// matched; K3, Pascal's call). The landing bursts' frame address rebuilt in a
// loop may be the same thing. The consequences: object r9, not r11; sine r9, not
// r7; IwramMulQ16 and x swap r6/r8; the x nudge takes r5 for 0x8000 where the
// listing, with r5 busy, has r1 in both arms and cross-jumps.
// Re-measured with the 1 November compiler: same 206 and gap 54, so none of this
// was slow-loads. How it folds, from the gcse and cse2 dumps of three explicit
// pointers: PRE turns the later `plus sfp -12` sets into copies of a reaching reg,
// the last cprop rewrites their uses in later blocks to it, and cse2 merges the
// rest into the first pointer. The same last cprop folds a `speed` local's 0x8000,
// which is why r11's constant cannot appear either. No gate in gcse_main applies.
// Tried this round, no gain: speed in the outer scope, first declared, set at the
// block's top or declared in the block (all differ 204, gap 60); cosine as
// `s32 turn[1]` as Func_0200043c has it (no change; speed with it 204, gap 60);
// the z nudge in its own local, x and z declared before dx and dz, dx and dz in a
// block, cosine and sine declared last, offset for center throughout (all no
// change); *pos in the rotation (265); offset set again at each stretch, one
// pointer with three sets, as a diagnostic (250).
// Tried, none better on gap: x nudge through a local or a ternary, which pulls x
// into a long pseudo (88+); `object->x` for *pos, differ 192 but gap 62, kept
// out as the gap is the aim; z through the field, gap 59 (z read once and stored
// once is the best form); rotation stores written inline (76) or summed in one
// expression (58); z terms swapped (57); pos set before offset (56); pos and
// offset declared the other way (no change); the switch writing center directly
// (73; 233 and gap 73 re-measured). px, py and pz are read before the flip test because the listing loads
// them there.
// Shape follows Func_0200043c (s043c.c), including its forwarded SeaOfTime_Cos
// and SeaOfTime_Sin; plain Math_Cosine/Math_Sine there gave 242, 223 and gap 61 now.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"

struct SeaOfTimeEventWork0154 {
  u8 unk000[0x154];
  s16 unk154;
};

void Func_0200054c(void) {
  struct SeaOfTimeEventWork0154 *work =
      (struct SeaOfTimeEventWork0154 *)Ram_HeapSlots->eventWork;
  struct SeaOfTimeObject *object = Actor_GetById(8);
  s32 center[3];
  s32 ends[4];
  struct SeaOfTimeSpark spark;
  s32 *pos;
  s32 *offset;
  s32 cosine;
  s32 sine;
  s32 dx;
  s32 dz;
  s32 x;
  s32 z;
  s32 phase;
  s32 c;
  s32 s;
  s32 flip;
  s32 px;
  s32 py;
  s32 pz;

  offset = center;
  pos = &object->x;
  switch (work->unk154) {
  case 30:
    offset[0] = 0;
    offset[2] = -1;
    break;
  case 32:
    offset[0] = -1;
    offset[2] = 0;
    break;
  case 34:
    offset[0] = 0;
    offset[2] = 1;
    break;
  case 36:
    offset[0] = 1;
    offset[2] = 0;
    break;
  case 31:
    offset[0] = -1;
    offset[2] = -1;
    break;
  case 33:
    offset[0] = -1;
    offset[2] = 1;
    break;
  case 35:
    offset[0] = 1;
    offset[2] = 1;
    break;
  case 37:
    offset[0] = 1;
    offset[2] = -1;
    break;
  }
  center[0] = (((object->x >> 21) + center[0]) << 21) + 0x100000;
  center[2] = (((object->z >> 21) + center[2]) << 21) + 0x100000;
  if (Func_020003d0(pos, center) > 46) {
    if (*pos < center[0])
      *pos += 0x8000;
    else
      *pos -= 0x8000;
    z = object->z;
    if (z < center[2])
      z += 0x8000;
    else
      z -= 0x8000;
    object->z = z;
  }
  if (Func_0200034c(1, center[0] >> 20, center[2] >> 20) >> 8 == 40) {
    cosine = SeaOfTime_Cos(-0x200);
    sine = SeaOfTime_Sin(-0x200);
  } else {
    cosine = SeaOfTime_Cos(0x200);
    sine = SeaOfTime_Sin(0x200);
  }
  dx = object->x - center[0];
  dz = object->z - center[2];
  z = Iwram_MulQ16(dx, sine) + Iwram_MulQ16(dz, cosine);
  x = Iwram_MulQ16(dx, cosine) - Iwram_MulQ16(dz, sine);
  x += center[0];
  z += center[2];
  object->z = z;
  object->x = x;

  phase = gFrameCount & 3;
  if (phase == 0) {
    if ((gFrameCount & 10) == 0)
      Func_02002054(0x255);
    spark.mode = 2;
    spark.kind = 7;
    spark.scaleX = 0xcccc;
    spark.scaleY = 0xcccc;
    c = Math_Cosine(object->facing);
    s = Math_Sine(object->facing);
    ends[0] = c * 10;
    ends[1] = s * 10;
    ends[2] = -(c * 10);
    ends[3] = -(s * 10);
    ends[0] += object->x;
    ends[1] += object->z;
    ends[2] += object->x;
    ends[3] += object->z;
    if (ends[1] != object->z) {
      ends[1] += -0xc0000;
      ends[3] += -0xc0000;
    }
    flip = gFrameCount & 4;
    px = ends[0];
    py = object->y;
    pz = ends[1];
    if (flip) {
      Func_0200015c(px + 0x30000, py, pz, 0x8000, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] - 0x30000, object->y, ends[3], -0x8000, 0, 0,
                    0xb0000, &spark);
    } else {
      Func_0200015c(px - 0x30000, py, pz, -0x8000, 0, 0, 0xb0000, &spark);
      Func_0200015c(ends[2] + 0x30000, object->y, ends[3], 0x8000, 0, 0,
                    0xb0000, &spark);
    }
  }
}

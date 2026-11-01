// Draft, differ 219 of 350, skeleton gap 87 (tla-en, 2026-11-01). Destined for
// scene/sea_of_time. Control flow, calls, frame (the fill word at sp+98 above
// the two effect options at sp+56 and sp+16) and data accesses follow the
// listing. Nearly all of what is left comes from one cause: the listing shows
// no GCSE PRE anywhere in this function, and here PRE runs.
// The evidence: (1) the native zero-extends each channel afresh at both compares
// and at the pack, keeps it sign-extended (lsls/asrs) at its stores; here PRE
// treats `channel << 16` as partly redundant (gcse dump: 7 substs, edges out of
// each step), combine drops the asrs, the draft is 19 instructions short and the
// shorter loop lets loop.c hoist 0xffff, which the native rebuilds in the body.
// Red's first compare and step lose their extensions in the native only because
// CSE sees the single-set `color & 31` there; here that compare feeds PRE.
// (2) The shower's options address is rebuilt in the native's body with no entry
// jump; PRE hoists it here, so the shower enters at its bottom test (one `b` the
// native lacks). An `options = &rain` local does not stop it; dropped.
// This matches the -Os lead in the shared notes (classic GCSE, not PRE). Is that
// a compiler question for Pascal (K3)? Out of scope here; not tried.
// Tried against PRE in plain C, all worse: if/else stage variables r1, r2 in both
// arms (differ 314, moves stay in the else arms), alternating variables copied
// at each join (278, CSE rewrites the copy back), s16 channels and a shared
// zero-extension (earlier round, gap 92). Empty asm or one-pass devices cannot
// kill a register expression, so none were added.
// Forms found: `mask` (one-constant local) gives the native's single #31 in
// the spark and shower blocks, though it takes r1 where the native has r6; the
// outer fade loop is goto, which keeps gData_020026c0 in its body as the native
// does; the palette is a stepped `volatile u16 *` set in that body, since an
// indexed BG_PLTT_COLOR there loses the strength reduction (222) and the do loop
// costs 228. RGB(r, g, b) is the native's pack order. With the pack written
// (g << 5) | (b << 10) | r the gap is 71 but differ 261, as PRE then folds less.
// Registers: native i r8, palette r10, offset r9, table r11; here the table
// takes r11's place late. The native's mask 31 is reg:HI, reloaded from the pool.
// beq+b at the outer branch comes back once the body has the native's length.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"
#include "main_routine_work.h"
#include "io_reg.h"
#include "call.h"
#include "scene_ids.h"
#include "scene_tables.h"

/// The 14 colours of background palette 3 (entries 1 to 14) that the fade
/// returns to, copied from palette RAM by Func_02001cdc.
extern u16 gData_020026c0[];

void Func_020010a8(struct SeaOfTimeObject *object);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02001f74(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f4c(struct SeaOfTimeObject *object, s32 value);
void Party_SetFields1eeAnd1f0(s32 scene, s32 entrance);
void Func_0200200c(s32 a, s32 b);
void wait(s32 frames);

void Func_02001108(void) {
  volatile u16 fill;
  struct SeaOfTimeSpark burst;
  struct SeaOfTimeSpark rain;
  struct SeaOfTimeObject *object;
  s32 changed;
  volatile u16 *palette;
  u32 i;

  Func_02001fac();
  Func_02002044(0);
  GameFlag_SetBit(0x8dd);
  Call3((void (*)())Actor_SetSpeedParameters, 8, 0x10000, 0x8000);
  Actor_GetById(8)->facing = FACING_NORTH;
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 8, 352, 264);
  Func_02001fec(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(352 << 16, -1, 196 << 16, 1);
  Func_02001ffc();
  Actor_WaitForMoveById(8);
  Func_02002054(78);
  Func_02002054(556);
  // Fills palette 3's entries 1 to 14 with one colour.
  fill = RGB(11, 6, 23);
  Dma_Set((const void *)&fill, (void *)&BG_PLTT_COLOR(3, 1),
          (DMA_ENABLE | DMA_SRC_FIXED) << 16 | 14, REG_DMA3);
  Func_02001f74(49, 70, 19, 73, 6, 6);
  // Steps each colour's red, green and blue by one toward the saved palette
  // every 10 frames until none changes.
fade:
    wait(10);
    changed = 0;
    palette = &BG_PLTT_COLOR(3, 1);
    for (i = 1; i != 15; i++, palette++) {
      u16 color = *palette;
      u16 target = gData_020026c0[i - 1];
      u16 r = color & 31;
      u16 g = (color >> 5) & 31;
      u16 b = (color >> 10) & 31;
      u16 tr = target & 31;
      u16 tg = (target >> 5) & 31;
      u16 tb = (target >> 10) & 31;

      if (r < tr) {
        r++;
        changed = 1;
      }
      if (r > tr) {
        r--;
        changed = 1;
      }
      if (g < tg) {
        g++;
        changed = 1;
      }
      if (g > tg) {
        g--;
        changed = 1;
      }
      if (b < tb) {
        b++;
        changed = 1;
      }
      if (b > tb) {
        b--;
        changed = 1;
      }
      *palette = RGB(r, g, b);
    }
    if (changed)
      goto fade;
  FieldEvent_Wait(30);
  // For 360 frames, spawns an effect every 16 frames, then every 8, then
  // every other frame, and plays sound 164 every 64 frames. As a C loop,
  // loop.c moves the mask, the palette and the options address out of it,
  // which the native does not (differ 310).
  i = 0;
sparks:
  if ((i & 63) == 0)
    Func_02002054(164);
  if (i < 120) {
    if (i & 15)
      goto next;
  } else if (i < 240) {
    if (i & 7)
      goto next;
  } else if (i & 1)
    goto next;
  {
    s32 x = Random16Far();
    s32 z = Random16Far();
    s32 mask = 31;

    burst.kind = 7;
    Func_0200015c(((x & mask) << 16) + 0x1500000, 0,
                  ((z & mask) << 16) + 0xb00000, 0, 0, 0, 0x10000, &burst);
    wait(1);
  }
next:
  i++;
  if (i != 360)
    goto sparks;
  // Then 16 larger effects, one a frame. As a C loop, loop.c also moves the
  // mask and the options address out, which the native does not (differ 265).
  // Entered at its test so that PRE leaves the address in the body.
  i = 0;
  goto showerTest;
shower:
  {
    s32 x = Random16Far();
    s32 z = Random16Far();
    s32 mask = 31;

    rain.scaleX = 0x18000;
    rain.scaleY = 0x18000;
    rain.kind = 7;
    rain.mode = 1;
    Func_0200015c(((x & mask) << 16) + 0x1500000, 0,
                  ((z & mask) << 16) + 0xb00000, 0, 0x10000, 0, 0xb0000, &rain);
    wait(1);
  }
  i++;
showerTest:
  if (i != 16)
    goto shower;
  object = Import_ActorCreate(159, 0x15c0000, 0, 0xb80000);
  // The object's per-frame hook, struct Actor's update.
  *(void (**)(struct SeaOfTimeObject *))((u8 *)object + 0x6c) = Func_020010a8;
  FieldEvent_Wait(60);
  Func_02001f4c(object, 1);
  FieldEvent_Wait(40);
  gMainRoutineWork.battleRule24b = 2;
  Party_SetFields1eeAnd1f0((s32)&SceneId_Ov687Room2, 4);
  Func_0200200c(12, 6);
  Func_02001fb4();
}

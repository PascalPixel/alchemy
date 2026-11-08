// Draft, differ 2 of 350 (tla-en, 2026-11-08)
// Differ 2 in all six editions, the same two rows. scene/sea_of_time. Left: in the fade loop's entry block the native sets the
// table offset (r9) before the palette pointer (r10); here the palette comes
// first: `mov sl, r4` and `mov r9, r0` swap. Both are reduced givs of i, and
// sched2 breaks their tie by RTL order. Here loop.c's first pass reduces the
// palette (its base is a constant), and the offset only in the second pass,
// once the table address hoisted by the first pass counts as invariant; so the
// palette's init comes first. The native's order needs both reduced in one
// pass with the offset first, yet its increments still palette first: that
// fits only a palette giv of a second biv incremented before i, or one placed
// by AUTO_INC_DEC after its store. No plain form found gives either.
// Round 4, 8 November: 12 minutes of permute at seed 31 (98,000 candidates),
// nothing below 2.
// Round 3, 8 November: palette indexed from a base pointer, palette[i], instead
// of a stepped pointer (14 to 4: loop.c now reduces it as a giv of i, as the
// native's register order shows); the shower's two offsets as locals, z's
// declared first (4 to 2: sched2 then takes `ands r2, r6` before `lsls r5`).
// Tried in round 3, none better than 2: an entry pointer &palette[i] (same),
// palette base set before the fade label (same), palette[i - 1] from entry 1
// (same), BG_PLTT_COLOR(3, i) for the read or the store (31, 193), a pointer
// local per entry (31), non-volatile palette (same), palette as a struct of 16
// colours (137, 240), table through a `u16 *` local (157 to 160: one pointer
// giv), *(gData_020026c0 + (i - 1)) (155), (gData_020026c0 - 1)[i] (155),
// palette[(s32)i], do-while or bottom i++ (same), i < 15 (6); shower: masks in
// place (10), z masked in place (36), two masked locals (10), offsets in x
// then z order (4). Permuter 10 minutes on the differ-2 draft: nothing better.
// Older rounds (before round 3, other forms): table read first in the body
// (128), indexed BG_PLTT_COLOR (31), do or while fade loops, s16 table, x and z
// read before mask (20), rain stores reversed (26); removing devices:
// non-volatile fill (228), direct calls without Call3 (229); loop forms for the
// sparks or shower (228 to 252); no `mask` (269), one `mask` for the function
// (261), s32 i (223), s32 color (264), red as else-if (225).
// Forms kept: `mask` (one-constant local) gives the native's single #31 in the
// spark and shower blocks; the outer fade loop is goto, which keeps
// gData_020026c0 in its body as the native does; RGB(r, g, b) is the native's
// pack order. Registers: native i r8, palette r10, offset r9, table r11. The
// native's mask 31 is reg:HI, reloaded from the pool.
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
    palette = &BG_PLTT_COLOR(3, 0);
    for (i = 1; i != 15; i++) {
      u16 color = palette[i];
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
      palette[i] = RGB(r, g, b);
    }
    if (changed)
      goto fade;
  FieldEvent_Wait(30);
  // For 360 frames, spawns an effect every 16 frames, then every 8, then
  // every other frame, and plays sound 164 every 64 frames. As a C loop,
  // loop.c moves the mask, the palette and the options address out of it,
  // which the native does not (differ 252).
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
    s32 mask = 31;
    s32 z = Random16Far();

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
  // mask and the options address out, which the native does not (differ 228).
  // Entered at its test so that PRE leaves the address in the body.
  i = 0;
  while (i != 16) {
  {
    s32 x = Random16Far();
    s32 mask = 31;
    s32 z = Random16Far();
    s32 offsetZ = (z & mask) << 16;
    s32 offsetX = (x & mask) << 16;

    rain.scaleX = 0x18000;
    rain.scaleY = 0x18000;
    rain.kind = 7;
    rain.mode = 1;
    Func_0200015c(offsetX + 0x1500000, 0, offsetZ + 0xb00000, 0,
                  0x10000, 0, 0xb0000, &rain);
    wait(1);
  }
  i++;
  }
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

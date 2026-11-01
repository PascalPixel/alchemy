// Draft, differ 11 of 33, skeleton gap 1 (tla-en, 2026-11-01): Func_02000f6c.
// Func_02001414 and Func_02000fb8: Matches: differ 0 (tla-en, 2026-11-01).
// Overlay 6B5 near misses from ☀️ link_lobby/lobby.c. Measure with --symbol:
// the draft shares three names with the listing. Both loops are label and goto
// loops: as for loops loop.c strength reduces them and hoists the base and the
// constant, which the listing does not. Func_02000fb8 is the empty function the
// listing keeps after 0f6c.
// Left in Func_02000f6c: the listing reads activeOwners[i] twice from one
// address (r0 + r2) and this form reads it once, so the base lives past ret = 1
// and takes r4 instead of r0; every other difference follows from that. The
// if/else around ret = 1 gives the listing's movs r0, #1 before the compare
// (jump.c); `return 0` at the 0xff test keeps the reread but moves movs r0, #0
// up to that test too (gap 1 the other way, differ 11); all-constant returns
// keep the reread and lose the early movs (differ 9, gap 5). Spelling the second
// read *(p + i) rereads from a separately built address (differ 25). The reread
// needs a label between the two tests when CSE runs, with the 0xff test jumping
// straight to the shared zero: a dead store or a helper inline in that test
// should give it, and neither was tried, as each would be a new device.
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "input.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "serial.h"
#include "text/msg_ids.h"

s32 Func_02000f6c(s32 id);
s32 Func_02000fbc(s32 actor);
void Func_0200191c(void);
void Func_0200197c(void);
void Func_02001984(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_020019e4(s32 value, s32 digits);
void Func_020019ec(void);
void Func_02001a14(void);
void Func_02001a1c(void);
void Func_02001a64(s32 message);
void Func_02001a74(s32 actor, s32 flags);
void Func_02001a7c(s32 actor, s32 animation, s32 flags);
s32 Func_02001a84(s32 scene, s32 entrance);
void Func_02001ab4(s32 skip);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void FieldMessage_Show(const u8 *message, s32 flags);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02001984(a, b, c, d, e, f);
}

s32 Func_02000f6c(s32 id) {
  s32 count;
  s32 max;
  s32 i;
  s32 ret;

  count = Party_CountActiveOwners();
  max = 3;
  if (GameFlag_Test(0x172) == 0) {
    max = 4;
  }
  if (count > max) {
    count = max;
  }
  i = 0;
  if (i < count) {
  loop:
    if (gMainRoutineWork.activeOwners[i] == 0xff)
      goto zero;
    if (gMainRoutineWork.activeOwners[i] == id)
      ret = 1;
    else {
      i++;
      if (i < count)
        goto loop;
      goto zero;
    }
    goto done;
  }
zero:
  ret = 0;
done:
  return ret;
}

void Func_02000fb8(void) {}

s32 Func_02001414(s32 value) {
  s32 digit;

  if (value > 999)
    value = 999;
  digit = 0;
loop:
  Map_CopyCells(27, value % 10, 16 - digit, 8, 1, 1);
  value = value / 10;
  digit++;
  if (digit <= 2)
    goto loop;
  Func_0200197c();
}

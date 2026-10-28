#include "types.h"
#include "scene_tables.h"

void Func_02001134(s32 actor, s32 x, s32 z);
void Func_0200113c(s32 actor, s32 target);
void Actor_SetModeById(s32 actor, s32 mode);
void Func_020010dc(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void wait(s32 frames);

static inline void MadraCatacombs_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as
   * direct calls the coordinates are shared in callee-saved registers (119
   * differ). */
  Func_02001134(actor, x, z);
}

static inline void MadraCatacombs_CopyAttributes(s32 a, s32 b, s32 c, s32 d,
                                                 s32 e, s32 f) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as a
   * direct call its stack arguments take other registers. */
  Func_020010dc(a, b, c, d, e, f);
}

static inline void MadraCatacombs_Wait(s32 frames) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer, as
   * the draft of this function had it. */
  wait(frames);
}

/// Places actors 10 to 19 in two columns, then sends actors 20 to 29 to them
/// one each.
void Func_02000958(void) {
  u32 i;
  /* FAKEMATCH: the native keeps the counter in r5 and the actor in r6; unpinned
   * the two swap (14 differ), and a do-while or a counter-derived actor
   * measured 14 to 29. */
  register s32 actor __asm__("r6");
  s32 target;

  MadraCatacombs_ObjPlace(14, PIXELS(136), PIXELS(258));
  MadraCatacombs_ObjPlace(19, PIXELS(184), PIXELS(258));
  MadraCatacombs_ObjPlace(13, PIXELS(120), PIXELS(226));
  MadraCatacombs_ObjPlace(18, PIXELS(200), PIXELS(226));
  MadraCatacombs_ObjPlace(12, PIXELS(120), PIXELS(194));
  MadraCatacombs_ObjPlace(17, PIXELS(200), PIXELS(194));
  MadraCatacombs_ObjPlace(11, PIXELS(136), PIXELS(162));
  MadraCatacombs_ObjPlace(16, PIXELS(184), PIXELS(162));
  MadraCatacombs_ObjPlace(10, PIXELS(136), PIXELS(138));
  MadraCatacombs_ObjPlace(15, PIXELS(184), PIXELS(138));
  target = 10;
  actor = 20;
  for (i = 0; i < 10; i++) {
    Actor_SetModeById(actor, 0);
    Func_0200113c(actor, target);
    target++;
    actor++;
  }
  MadraCatacombs_CopyAttributes(0, 0, 2, 2, 9, 22);
  MadraCatacombs_Wait(1);
}

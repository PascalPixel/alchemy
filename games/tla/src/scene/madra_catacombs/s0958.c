#include "types.h"

void Func_02001134(s32 actor, s32 x, s32 z);
void Func_0200113c(s32 actor, s32 target);
void Actor_SetModeById(s32 actor, s32 mode);
void Func_020010dc(s32 srcX, s32 srcY, s32 width, s32 height, s32 destX,
                   s32 destY);
void WaitFrames(s32 frames);

static inline void MadraCatacombs_ObjPlace(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as direct
   * calls the coordinates are shared in callee-saved registers (119 differ). */
  Func_02001134(actor, x, z);
}

static inline void MadraCatacombs_CopyAttributes(s32 a, s32 b, s32 c, s32 d,
                                                 s32 e, s32 f) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer; as a
   * direct call its stack arguments take other registers. */
  Func_020010dc(a, b, c, d, e, f);
}

static inline void MadraCatacombs_Wait(s32 frames) {
  /* FAKEMATCH: scene calls reach the main image through one inline layer, as the
   * draft of this function had it. */
  WaitFrames(frames);
}

/// Places actors 10 to 19 in two columns, then sends actors 20 to 29 to them one
/// each.
void Func_02000958(void) {
  u32 i;
  /* FAKEMATCH: the native keeps the counter in r5 and the actor in r6; unpinned
   * the two swap (14 differ), and a do-while or a counter-derived actor
   * measured 14 to 29. */
  register s32 actor __asm__("r6");
  s32 target;

  MadraCatacombs_ObjPlace(14, 136 << 16, 258 << 16);
  MadraCatacombs_ObjPlace(19, 184 << 16, 258 << 16);
  MadraCatacombs_ObjPlace(13, 120 << 16, 226 << 16);
  MadraCatacombs_ObjPlace(18, 200 << 16, 226 << 16);
  MadraCatacombs_ObjPlace(12, 120 << 16, 194 << 16);
  MadraCatacombs_ObjPlace(17, 200 << 16, 194 << 16);
  MadraCatacombs_ObjPlace(11, 136 << 16, 162 << 16);
  MadraCatacombs_ObjPlace(16, 184 << 16, 162 << 16);
  MadraCatacombs_ObjPlace(10, 136 << 16, 138 << 16);
  MadraCatacombs_ObjPlace(15, 184 << 16, 138 << 16);
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

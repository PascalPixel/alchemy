// 26 October 2026, under -mthumb-slow-loads: differ 4 of 198: the gMainRoutineWork
// load at the start, where the native issues a load one slot later after a
// store, where alchemy-gcc issues it at once; alchemy permute (5 to 6 minutes)
// and an empty asm or one-pass block after any one statement do not move it.
// Draft, differ 4 of 198: stores its argument and makes each room's calls by
// gMainRoutineWork.scene. Its Func_02005674 calls go through an inline that keeps
// each call's own constants, as ☀️'s scene layer does (differ 96 called
// directly). Left: the native copies the direction to r5 and stores it to
// gData_02005af0 before building the scene field's offset; here the copy and
// store fall two rows later. An empty asm after the store puts it in place but
// moves the gMainRoutineWork load after it and the stack frame's sub sp first (still 4).
// Tried: scene as s32 (4) or initialised at its declaration (6), the store
// after the scene read (6), the direction through a temporary (4), an s16
// direction (9), gData_02005af0 as u32 (4), permuter 4 minutes (4).
// Pinned: the direction in r5 held by an asm that clobbers r1 after the store
// puts the copy and store in place but loads gMainRoutineWork after the store
// (2); with the store's address pinned to r3 and the party pointer an input
// of an asm before the store, gMainRoutineWork is loaded before the copy (2); a
// volatile barrier moves the stack frame's sub sp first (6).
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern s32 gData_02005af0;
struct Actor *Actor_GetById(s32 id);
void Func_020020b0(s32 id, s32 direction, s32 value);
void Func_02005674(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);

static __inline__ void Func_02005674_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02005674(a, b, c, d, e, f);
}

void Func_020020f0(s32 direction) {
  s16 scene;

  gData_02005af0 = direction;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov671Room2) {
    Func_020020b0(14, direction, 0);
    Func_020020b0(21, direction, 1);
    Func_020020b0(26, direction, 2);
    if ((u16)(gMainRoutineWork.entrance - 3) > 1)
      Func_020020b0(27, direction, 3);
    if (direction == 0)
      Func_02005674_(23, 21, 1, 3, 23, 9);
    else if (direction == -1)
      Func_02005674_(24, 21, 1, 3, 23, 9);
  } else if (scene == (s32)&SceneId_Ov671Room3) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(15, direction, 1);
    Func_020020b0(20, direction, 2);
    Func_020020b0(25, direction, 3);
    Func_020020b0(29, direction, 4);
  } else if (scene == (s32)&SceneId_Ov671Room4) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(21, direction, 1);
    Func_020020b0(29, direction, 2);
    Func_020020b0(35, direction, 3);
    Func_020020b0(38, direction, 4);
  } else if (scene == (s32)&SceneId_Ov671Room5) {
    Func_020020b0(10, direction, 0);
    Func_020020b0(12, direction, 1);
    if (direction == 0)
      Func_02005674_(34, 12, 1, 3, 27, 12);
    else if (direction == -1)
      Func_02005674_(35, 12, 1, 3, 27, 12);
  } else if (scene == (s32)&SceneId_Ov671Room6) {
    Actor_SetModeById(13, 5);
    Actor_SetSingleChildFlags(Actor_GetById(13), 0);
  } else if (scene == (s32)&SceneId_Ov671Room7) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(9, direction, 1);
    Func_020020b0(10, direction, 2);
    Func_020020b0(11, direction, 3);
    Func_020020b0(12, direction, 4);
    if (direction == 0)
      Func_02005674_(0, 0, 1, 3, 9, 27);
    else if (direction == -1)
      Func_02005674_(1, 0, 1, 3, 9, 27);
  }
}

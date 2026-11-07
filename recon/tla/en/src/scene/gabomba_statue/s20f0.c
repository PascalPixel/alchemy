#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern s32 gData_02005af0;
struct Actor *Actor_GetById(s32 id);
void Func_020020b0(s32 id, s32 direction, s32 value);
void Func_02005674(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

static __inline__ void Func_02005674_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH(wrapper): Func_02005674_ forwarded inline; as direct calls to
   * Func_02005674, 24 of 198 instructions differ in tla-en. */
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

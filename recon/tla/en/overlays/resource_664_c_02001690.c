// Draft, differ 66 of 155: every eighth frame pair, while none of the event
// work's four busy counters is set, starts a drip (Func_0200160c) at a place
// chosen by scene: in scene 0x57 one of two spots by the frame and flag
// 0x89b; in scene 0x5c one of six by the frame, the leader's position and
// flags 0x200 to 0x202. Left: the native shares one call for each place
// through jumps between the branches and lays the blocks out in another
// order.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room5, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;
#include "ram_buffer.h"

extern u32 gFrameCount;
struct Actor *Actor_GetById(u32 id);
s32 GameFlag_Test(s32 flag);
void Func_0200160c(s32 x, s32 y, s32 z);

void Func_02001690(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 frame;
  s32 scene;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0)
    return;
  frame = gFrameCount;
  if ((frame & 6) != 0)
    return;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov664Room3) {
    s32 x;

    if ((frame & 1) && !GameFlag_Test(0x89b))
      x = 0x1780000;
    else
      x = 0x1180000;
    Func_0200160c(x, 0, 0x2680000);
  } else if (scene == (s32)&SceneId_Ov664Room8) {
    if (frame & 1) {
      if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.currentOwner)->x >> 19 <= 44)
          Func_0200160c(0xa80000, 0, 0x25b0000);
        else if (!GameFlag_Test(0x201))
          Func_0200160c(0x1f80000, 0, 0x1c80000);
        else
          Func_0200160c(0x2280000, 0, 0x1c80000);
      } else {
        Func_0200160c(0x2680000, -0x200000, 0x880000);
      }
    } else {
      if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 19 > 41) {
        if (Actor_GetById(gMainRoutineWork.currentOwner)->x >> 19 > 49)
          Func_0200160c(0x2280000, 0, 0x1c80000);
        else if (GameFlag_Test(0x200))
          Func_0200160c(0xa80000, 0, 0x25b0000);
        else
          Func_0200160c(0x1080000, 0, 0x2580000);
      } else if (GameFlag_Test(0x202)) {
        Func_0200160c(0x2680000, -0x200000, 0x880000);
      } else {
        Func_0200160c(0x2880000, -0x200000, 0x880000);
      }
    }
  }
}

#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "scene_ids.h"

void Func_02003c28(void);
void Func_02003d78(s32 skip);
void Func_02003df8(s32 cue);
void Func_02003d00(s32 value);
void Actor_SetSpritePriority(s32 id, s32 variant);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020030e4(void);
void Func_02003164(void);
void Func_02003284(void);
void Func_0200369c(void);
void Func_0200390c(void);
void Func_02003aa0(void);

void Func_02002ff4(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02003c28();
  Func_02003d78(0);
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov655Room1)
    Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 3);
  Func_02003df8(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003d00(*(s16 *)(work + 0x154));
}

s32 Func_0200304c(void) {
  s16 scene = gSequenceParentWork.scene;

  if (scene == (s32)&SceneId_Ov655Room1)
    Func_020030e4();
  else if (scene == (s32)&SceneId_Ov655Room2)
    Func_02003164();
  else if (scene == (s32)&SceneId_Ov655Room3)
    Func_02003284();
  else if (scene == (s32)&SceneId_Ov655Room4)
    Func_0200369c();
  else if (scene == (s32)&SceneId_Ov655Room5)
    Func_0200390c();
  else if (scene == (s32)&SceneId_Ov655Room6)
    Func_02003aa0();
  return 0;
}

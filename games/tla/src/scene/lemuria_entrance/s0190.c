/// @file
/// The scene's setup: places its actors, then plays what the entrance it was
/// reached by calls for.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"

extern char SceneId_Ov688;

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200058c(s32 id, s32 x, s32 z);
void Func_020005a4(s32 scene, s32 entrance);
void Func_020003cc(void);

s32 Func_02000190(void) {
  s32 *screen = &((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect;
  s32 effect = 0x204;

  *screen = effect;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Actor_SetSingleChildFlags(Actor_GetById(19), 0);
  GameFlag_SetBit(0x907);
  GameFlag_ClearBit(0x9b4);
  if (GameFlag_Test(0x909))
    Func_0200058c(19, 0, 0);
  Actor_SetSingleChildFlags(Actor_GetById(9), 0);
  Actor_SetSingleChildFlags(Actor_GetById(10), 0);
  Actor_SetSingleChildFlags(Actor_GetById(11), 0);
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Actor_SetSingleChildFlags(Actor_GetById(13), 0);
  Actor_SetSingleChildFlags(Actor_GetById(14), 0);
  Actor_SetSingleChildFlags(Actor_GetById(15), 0);
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
  Actor_SetSingleChildFlags(Actor_GetById(17), 0);
  Actor_SetSingleChildFlags(Actor_GetById(18), 0);
  if (gMainRoutineWork.entrance == 90) {
    gMainRoutineWork.ownerOverride = 1;
    Func_020005a4((s32)&SceneId_Ov688, 1);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5) {
    gMainRoutineWork.currentOwner = 8;
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gMainRoutineWork.currentOwner = 4;
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = effect;
  }
  /* FAKEMATCH(wrapper): forwarded, so that the flag number is built for each call; as
   * GameFlag_Test(0x96b), 15 of 153 instructions differ in tla-en. */
  if (gMainRoutineWork.entrance == 3 && Value1((s32 (*)())GameFlag_Test, 0x96b)) {
    GameFlag_ClearBit(0x96b);
    Func_020003cc();
  }
  return 0;
}

#include "yallam.h"
#include "event_inline.h"
#include "types.h"
#include "main_routine_work.h"

extern char SceneId_Ov676Room1;
extern char SceneId_Ov676Room2;
extern const u8 gData_02002ac4[];
extern const u8 gData_02002be4[];
extern const u8 gData_02002bfc[];

const u8 *Func_02000218(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room1 &&
      gMainRoutineWork.entrance == 15)
    return gData_02002bfc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov676Room2)
    return gData_02002be4;
  return gData_02002ac4;
}

void Func_02000270(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  u32 i;
  s32 index;

  Func_02002488();
  FieldEvent_ResolvePendingActions(0);
  for (i = 8; i <= 63; i++) {
    u8 *actor = (u8 *)Actor_Get(i);

    if (actor != NULL) {
      u8 *flags = &actor[85];
      /* FAKEMATCH: the zero pinned to r3 is rebuilt on every pass as the native
       * does; loop.c hoists a plain zero out of the loop into a saved register.
       */
      register s32 zero asm("r3") = 0;

      *flags = zero;
    }
  }
  index = *(s16 *)(event + 340);
  /* FAKEMATCH: the empty asms on each side of the call keep the stop's
   * decrement after it, as in overlay 66d's Func_02000368 (differ 2 without the
   * first). */
  asm("");
  Event_PlaySound(158);
  asm("");
  index--;
  MapGrid_AnimateCells(gData_02002ce4[index].a, gData_02002ce4[index].b,
                       gData_02002ce4[index].c);
  /* FAKEMATCH: the empty asm ends the shift's output dependences on the later
   * calls (r4 is call-used), so sched2 takes the table address first as the
   * native does (differ 2). */
  asm("");
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  {
    s32 owner = gMainRoutineWork.currentOwner;

    if (index == 7)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  FieldEvent_Wait(10);
  FieldEvent_SetExitRequest(*(s16 *)(event + 340));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002490();
}

#include "yallam.h"
#include "types.h"
#include "sequence_parent_work.h"

extern char SceneId_Ov676Room1;
extern char SceneId_Ov676Room2;
extern const u8 gData_02002ac4[];
extern const u8 gData_02002be4[];
extern const u8 gData_02002bfc[];

const u8 *Func_02000218(void) {
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov676Room1 &&
      gSequenceParentWork.entrance == 15)
    return gData_02002bfc;
  if (gSequenceParentWork.scene == (s32)&SceneId_Ov676Room2)
    return gData_02002be4;
  return gData_02002ac4;
}

static __inline__ void Offset(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline sets the arguments up in their own
   * order; the direct call negates the offset last (differ 2). */
  Actor_OffsetPositionAndResetMotion(id, x, z);
}

static __inline__ void CenterOn(s32 id, s32 column, s32 offset) {
  /* FAKEMATCH: forwarding through the inline sets the arguments up in their own
   * order; the direct call negates the offset last (differ 2). */
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Func_02002460_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarding through the inline loads the stop's fields as the
   * native does, the first word last (differ 12 as a direct call). */
  Func_02002460(a, b, c);
}

void Func_02000270(void) {
  u8 *event = Ram_HeapSlots->eventWork;
  u32 i;
  s32 index;

  Func_02002488();
  Func_02002590(0);
  for (i = 8; i <= 63; i++) {
    u8 *actor = (u8 *)Actor_GetById(i);

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
  Func_020025a0(158);
  asm("");
  index--;
  Func_02002460_(gData_02002ce4[index].a, gData_02002ce4[index].b,
                 gData_02002ce4[index].c);
  /* FAKEMATCH: the empty asm ends the shift's output dependences on the later
   * calls (r4 is call-used), so sched2 takes the table address first as the
   * native does (differ 2). */
  asm("");
  Actor_SetModeById(gSequenceParentWork.currentOwner, 2);
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0xcccc, 0x6666);
  {
    s32 owner = gSequenceParentWork.currentOwner;

    if (index == 7)
      Offset(owner, 0, -4);
    else
      CenterOn(owner, 2, -4);
  }
  FieldEvent_Wait(10);
  Func_02002578(*(s16 *)(event + 340));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002490();
}

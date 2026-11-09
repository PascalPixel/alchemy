#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"

#include "scene_tables.h"
#include "event_inline.h"
void Func_02001f5c(void);
void Func_02001f64(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

static __inline__ void Func_02001f2c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Map_CopyCellPictureIndicesRect(a, b, c, d, e, f);
}

void Func_02000078(void) {
  if (Event_GetFlag(0x985)) {
    u8 *work = Ram_HeapSlots->eventWork;
    u32 i;

    Func_02001f5c();
    FieldEvent_ResolvePendingActions(0);
    /* FAKEMATCH: the loop is written with goto so that it carries no loop
     * notes: the bytes rebuild the zero and the height on every pass, and a for
     * loop hoists both (differ 39). */
    i = 8;
  loop:
    {
      struct Actor *object = Actor_Get(i);

      if (object != 0) {
        object->motionFlags = 0;
        object->y = PIXELS(32);
      }
    }
    i++;
    if (i <= 63)
      goto loop;
    if (!Event_GetFlag(0x201)) {
      Event_PlaySound(158);
      Func_02001f2c_(51, 38, 70, 22, 1, 2);
    }
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -4);
    FieldEvent_Wait(8);
    Event_PlaySound(123);
    FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02001f64();
  }
}

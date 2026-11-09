#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

extern const u8 MsgThenLetS[];

void Func_02001f5c(void);
void Func_02001f64(void);
void Func_02001d60(struct Actor *object);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

static __inline__ void Func_020020b4_(s32 id, s32 a, s32 b, s32 c) {
  /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the
   * arguments up in their own order and keeps each call's constants; a direct
   * call builds the costly constant first or shares it (differ 2 or more). */
  Func_080d3070(id, a, b, c);
}

void Func_02001da0(void) {
  if (Event_GetFlag(7)) {
    struct Actor *object;

    Func_02001f5c();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetSpeed(4, 0xcccc, 0x6666);
    Actor_Get(7)->update = (void (*)(void))Func_02001d60;
    Func_020020b4_(7, 16, 0, FACING_WEST);
    Actor_SetSingleChildFlags(Actor_Get(7), 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    FieldEvent_SetMessageId(MsgThenLetS);
    FieldActor_WaitForMoveById(7);
    Actor_Get(7)->update = 0;
    Event_ShowMessage(7, 0);
    Actor_SetAnimationAndWait(4, 3);
    Actor_Get(gMainRoutineWork.currentOwner)->update =
        (void (*)(void))Func_02001d60;
    Actor_WalkByAndWait(4, 0, 16);
    Actor_Get(gMainRoutineWork.currentOwner)->update = 0;
    Actor_Get(7)->update = (void (*)(void))Func_02001d60;
    Actor_SetAnimation(7, 2);
    object = Actor_Get(4);
    if (object != 0)
      Actor_SetDestination(7, *(s16 *)((u8 *)object + 10),
                                *(s16 *)((u8 *)object + 18));
    FieldActor_WaitForMoveById(7);
    Actor_SetPosition(7, 0, 0);
    Actor_Get(7)->update = 0;
    Actor_SetSingleChildFlags(Actor_Get(7), 1);
    Func_02001f64();
  }
}

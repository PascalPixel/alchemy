#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "version.h"
#include "scene_tables.h"
#include "event_inline.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) ((struct Actor *)((s32)Actor_Get((id))))

extern const u8 gData_02004508[];
extern const u8 gData_02004554[];
extern const u8 MsgWellHereWe[];
void Func_02003eec();
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003dec();
void Func_02003fb4(void);
void Func_020034d8(void);
void Func_02001278(void);
void Func_02000df8(void);

/// The Islets' arrival by entrance 4: actor 9 joins the leader and actor 10
/// is placed, the screen fades in, both run their action scripts
/// (Func_02000df8 updates) until actor 9's byte 0x63 is set, then actor 9
/// speaks (MsgWellHereWe) and the leader is launched up to (152, 742) and
/// turned; ends with Func_020034d8. The Japanese edition does not set the
/// event work's transition word 0x1ac.
void Func_020029c4(void) {
  struct Actor *leader = OBJECT(gMainRoutineWork.currentOwner);
  struct Actor *guide = OBJECT(9);
  u8 *done;

  Func_02003eec(9, gMainRoutineWork.currentOwner);
  Actor_SetPosition(10, PIXELS(240), PIXELS(540));
  System_Wait(1);
  guide->angle = FACING_SOUTH;
  leader->update = Func_02001278;
  Actor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)),
                         15);
  Actor_SetAnimation(9, 3);
  System_Wait(1);
#if TLA_PATCH_1 /* The Japanese release leaves the transition word. */
  *(s32 *)(EVENT_WORK + 0x1ac) = 513;
#endif
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  OBJECT(10)->update = Func_02000df8;
  guide->update = Func_02000df8;
  done = (u8 *)guide + 0x63;
  *done = 0;
  Func_02003dec((s32)guide, gData_02004508);
  Actor_SetScript(10, gData_02004554);
  do
    System_Wait(1);
  while (*done == 0);
  leader->update = 0;
  guide->update = 0;
  OBJECT(10)->update = 0;
  Actor_FaceDirectionAndWait(9, FACING_NORTH);
  FieldPsynergy_StartMindReadWave(gMainRoutineWork.currentOwner, 9);
  FieldEvent_SetMessageId(MsgWellHereWe);
  Event_ShowMessage(9, 0);
  Actor_SetAnimation(9, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetSpriteColor(((s32)Actor_Get(gMainRoutineWork.currentOwner)),
                         0);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 742);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, FACING_SOUTH);
  Event_ShowMessage(9, 0);
  Func_02003fb4();
  Actor_FaceDirectionAndWait(9, FACING_SOUTH);
  Func_020034d8();
#if TLA_PATCH_1
  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
#endif
}

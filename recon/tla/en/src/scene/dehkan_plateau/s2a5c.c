#include "types.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"

void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait(s32 frames);
void Actor_SetAngleToward(s32 id, s32 target, s32 mode);
void Func_020021b4(void);
void Func_02002bcc(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_02003c28(void);
void Func_02003c70(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc8(s32 id, s32 emote, s32 frames);
void Func_02003d00(s32 exit);
void Func_02003d78(s32 skip);
void Func_02003df8(s32 cue);

void Func_02002a5c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;
  s32 angle;

  Func_02003c28();
  Func_02003d78(0);
  /* FAKEMATCH(temporary): the angle named before the call, so it is built
   * before the two coordinates; as the literal argument FACING_NORTH, 4 of
   * 102 instructions differ in tla-en. */
  angle = FACING_NORTH;
  Func_02003c70(gMainRoutineWork.currentOwner, PIXELS(248), PIXELS(136), angle);
  Func_02003be0(65, 64, 15, 8, 1, 1);
  GameFlag_SetBit(0x245);
  Func_02003df8(161);
  Func_020021b4();
  Actor_SetAngleToward(9, gMainRoutineWork.currentOwner, 0);
  Func_02003cc8(9, 0x100, 20);
  Func_02002bcc();
  FieldEvent_Wait(20);
  leader->angle = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  for (i = 0; i < 40; i++) {
    if (i == 5)
      Func_02003df8(204);
    leader->scaleX -= 0x400;
    leader->scaleY -= 0x300;
    leader->y -= 0x9999;
    wait(1);
  }
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003d00(23);
}

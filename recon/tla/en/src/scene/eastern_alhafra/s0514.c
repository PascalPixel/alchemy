#define OWN_Actor_SetSingleChildFlags
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

struct Unk_02004240 {
  s32 value[6];
};

s32 GameFlag_Test();
void GameFlag_SetBit();
struct Actor *Actor_GetById();
s32 Actor_SetSingleChildFlags();
void Actor_Stop();
void Actor_OffsetPositionAndResetMotion();
void Actor_SetAngleToward();
void Actor_SetSpeedParameters();
void Actor_ArmCallback();
void Actor_SetPositionAndReset();
void Actor_WaitForMoveById();
void Motion_SetVarCbAndRefresh();
void FieldEvent_Wait();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02000fa4(void);
void Func_0200112c(void);
void Func_020017ec(void);
void Func_02001cb0(void);
void Func_02003f48(s32, s32, s32, s32);
void Func_02004004(void);
void Func_020040c0(void);
s32 Func_02004240(struct Unk_02004240 *);
void Func_020044c4(struct Unk_02004240);
void Func_02004868(void *, s32, s32);
s32 Func_02004a0c();
struct Actor *Func_02004a1c();
s32 Func_02004a24();
s32 Func_02004a34();
s32 Func_02004a44();
s32 Func_02004a7c();
s32 Func_02004a84();
void Func_02004a94();
void Func_02004a9c();
s32 Func_02004b04();
s32 Func_02004b3c();
s32 Func_02004b4c();
s32 Func_02004b5c();
s32 Func_02004b6c();
s32 Func_02004bac();
s32 Func_02004bb4();
s32 Func_02004be4();
s32 Func_02004bec();
s32 Func_02004c24();
s32 Func_02004c44();
s32 Func_02004c4c();
void Func_02004c9c(s32);
s32 Func_02004c7c();
s32 Func_02004c8c();

void Camera_MoveWithinMapBounds();
void Func_02000514(void) {
  struct Actor *actor15;
  struct Actor *actor16;
  struct Unk_02004240 unk;
  s32 facing;
  s32 i;

  actor15 = Actor_GetById(15);
  actor16 = Actor_GetById(16);
  facing = (Actor_GetById(gMainRoutineWork.currentOwner)->angle + 0x2000) & 0xc000;
  if (actor16 != NULL && Value1(GameFlag_Test, SPLIT_CONSTANT(0x8a4)) == 0)
    Func_02004c7c(actor16->x, actor16->z, 2, 255);
  if (Func_02004240(&unk) != 0) {
    if (Value1(GameFlag_Test, SPLIT_CONSTANT(0x8a4)) == 0 || facing == 0xc000) {
      Func_020044c4(unk);
    } else {
      GameFlag_SetBit(SPLIT_CONSTANT(0x8a6));
      Func_02004a94();
      Func_02004be4(0);
      Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(336), -1, PIXELS(560), 1);
      Func_020044c4(unk);
      Func_02004c7c(actor15->x - PIXELS(24), actor15->z, 0, 0);
      Func_02004c7c(actor15->x + PIXELS(8), actor15->z, 0, 0);
      Func_02004c7c(actor15->x - PIXELS(24), actor15->z, 2, 0);
      Func_02004c7c(actor15->x - PIXELS(8), actor15->z, 2, 0);
      Func_02004c7c(actor15->x + PIXELS(8), actor15->z, 2, 0);
      Func_02004c7c(actor15->x + PIXELS(24), actor15->z, 2, 0);
      Actor_SetModeById(15, 3);
      Actor_OffsetPositionAndResetMotion(15, 0, 16);
      FieldEvent_Wait(20);
      actor15->gravity = SPLIT_CONSTANT(0xccc);
      actor15->motionFlags = 3;
      i = 0;
      if (actor15->y > 0) {
        do {
          wait(1);
          i++;
        } while (i <= 59 && actor15->y > 0);
      }
      Func_02000fa4();
      Func_02004c9c(240);
      Actor_SetModeById(15, 8);
      FieldEvent_Wait(60);
      Camera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Func_02004a9c();
    }
  }
  if (actor16 != NULL && Value1(GameFlag_Test, SPLIT_CONSTANT(0x8a4)) == 0)
    Func_02004c7c(actor16->x, actor16->z, 2, 0);
}

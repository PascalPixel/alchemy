#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct EventWorkRoute {
  u8 unk000[0x1ac];
  s32 route;
  u8 unk1b0[4];
  s32 countdown;
};

struct EventWork1c4 {
  u8 unk000[0x1c4];
  s16 message;
};

struct AnimationRecord26 {
  u8 unk00[0x1a];
  u8 frame;
};

extern u8 MsgFieldAddressCompanion;
extern const u8 gData_020005e4[];

void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 BattleEffect_GetResourceId(s32 id);
void Text_GetMessageSize(s32 id, s32 *a, s32 *b, s32 *c, s32 *d);
void Message_OpenWindow(s32 id, s32 x, s32 y, u32 flags);
s32 Message_AllTyped(void);
void Window_Close(s32 work, s32 mode);
void Message_CloseFinishedWindows(void);
s32 Func_0200053c(s32 a, s32 b, s32 c, s32 d);
void Func_0200055c(void);
void Func_02000564(void);
void Func_02000310(s32 kind, s32 frames);

void Func_02000120(void) {
  if (Event_GetFlag(0x800) != 0) {
    Func_0200055c();
    FieldEvent_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    {
      struct MainRoutineWork *state = &gMainRoutineWork;


      state->renderMode = 6;
      state->ownerOverride = 1;
    }
    Actor_SetAnimation(8, 7);
    Func_02000564();
  } else {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->currentOwner = 4;
    party->renderMode = 0;
    party->ownerOverride = 0;
  }
}

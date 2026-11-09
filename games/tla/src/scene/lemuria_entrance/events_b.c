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

void Func_02000060(s32 arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  /* FAKEMATCH: pinned to r6 and assigned after the barrier, the countdown's
   * copy follows gMainRoutineWork's address, as native; as a plain parameter, 2
   * instructions differ in tla-en. */
  register s32 countdown asm("r6");

  asm("");
  countdown = arg;
  party->ownerOverride = 0;
  GameFlag_ClearBit(0x960);
  Sound_Play(123);
  party->currentOwner = 8;
  ((struct EventWorkRoute *)Ram_HeapSlots->eventWork)->route = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->currentOwner = 4;
  Import_EventSetExitRequest(countdown);
}

void Func_020000b0(s32 arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  /* FAKEMATCH: as in Func_02000060, the pin and the barrier put gMainRoutineWork's
   * address before the countdown's copy (differ 2 without them). */
  register s32 countdown asm("r6");

  asm("");
  countdown = arg;
  party->ownerOverride = 0;
  GameFlag_SetBit(0x960);
  Sound_Play(123);
  party->currentOwner = 8;
  ((struct EventWorkRoute *)Ram_HeapSlots->eventWork)->route = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->currentOwner = 4;
  Import_EventSetExitRequest(countdown);
}

void Func_02000100(void) {
  GameFlag_SetBit(0x909);
  Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
}

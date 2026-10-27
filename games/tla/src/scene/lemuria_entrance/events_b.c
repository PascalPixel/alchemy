#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

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

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitTicks(s32 ticks);
struct ObjectRuntime *Object_GetById(s32 id);
void FieldEvent_WaitTicks(s32 ticks);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Object_RefreshSelectorById(u32 id);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 BattleEffect_GetResourceId(s32 id);
void UiText_GetResourceDimensions(s32 id, s32 *a, s32 *b, s32 *c, s32 *d);
void UiText_OpenMessageWindow(s32 id, s32 x, s32 y, u32 flags);
s32 UiWork_IsComplete(void);
void UiWork_Finalize(s32 work, s32 mode);
void UiWork_FinalizePendingCore(void);
s32 Func_0200053c(s32 a, s32 b, s32 c, s32 d);
void Func_0200055c(void);
void Func_02000564(void);
void Func_0200058c(s32 id, s32 x, s32 z);
void Func_0200059c(s32 message);
void Func_020005ac(s32 countdown);
void Func_020005cc(s32 skip);
void Func_020005dc(s32 cue);
void Func_02000310(s32 kind, s32 frames);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_ClearBit(flag);
}

static __inline__ void Show(s32 kind, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000310(kind, frames);
}

void Func_02000060(s32 arg) {
  struct PartyState *party = &gPartyState;
  /* FAKEMATCH: pinned to r6 and assigned after the barrier, the countdown's
   * copy follows gPartyState's address, as native; as a plain parameter, 2
   * instructions differ in tla-en. */
  register s32 countdown asm("r6");

  asm("");
  countdown = arg;
  party->ownerOverride = 0;
  GameFlag_ClearBit(0x960);
  Func_020005dc(123);
  party->currentOwner = 8;
  ((struct EventWorkRoute *)Ram_HeapSlots->eventWork)->route = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->currentOwner = 4;
  Func_020005ac(countdown);
}

void Func_020000b0(s32 arg) {
  struct PartyState *party = &gPartyState;
  /* FAKEMATCH: as in Func_02000060, the pin and the barrier put gPartyState's
   * address before the countdown's copy (differ 2 without them). */
  register s32 countdown asm("r6");

  asm("");
  countdown = arg;
  party->ownerOverride = 0;
  GameFlag_SetBit(0x960);
  Func_020005dc(123);
  party->currentOwner = 8;
  ((struct EventWorkRoute *)Ram_HeapSlots->eventWork)->route = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  party->currentOwner = 4;
  Func_020005ac(countdown);
}

void Func_02000100(void) {
  Flag_Set(0x909);
  Func_0200058c(19, 0, 0);
}

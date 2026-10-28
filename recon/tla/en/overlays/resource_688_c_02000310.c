#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"

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
void wait(s32 frames);
struct Actor *Actor_GetById(s32 id);
void FieldEvent_Wait(s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Actor_RefreshSelectorById(u32 id);
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
void Func_0200058c(s32 id, s32 x, s32 z);
void Func_0200059c(s32 message);
void Func_020005ac(s32 countdown);
void Func_020005cc(s32 skip);
void Func_020005dc(s32 cue);
void Func_02000310(s32 kind, s32 frames);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  GameFlag_ClearBit(flag);
}

static __inline__ void Show(s32 kind, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02000310(kind, frames);
}

void Func_02000310(s32 kind, s32 frames) {
  struct EventWork1c4 *work = Ram_HeapSlots->eventWork;
  s32 b;
  s32 a;
  s32 height;
  s32 width;
  s32 resource;
  s32 id;
  s32 x;

  width = 0;
  height = 0;
  id = 56;
  if (kind != 8)
    id = kind;
  resource = BattleEffect_GetResourceId(id);
  if (kind == 7) {
    width = 10;
    height = 10;
    Text_GetMessageSize(work->message, &width, &height, &a, &b);
    x = width + a - 5;
  } else {
    width = 5;
    height = 10;
    x = 5;
  }
  Message_OpenWindow(work->message++, width, height,
                           (resource << 16) | 2);
  x = Func_0200053c(id, 0, x, 5);
  while (Message_AllTyped() == 0)
    wait(1);
  FieldEvent_Wait(frames);
  Window_Close(x, 2);
  Message_CloseFinishedWindows();
}

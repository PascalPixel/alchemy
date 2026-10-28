#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"

void Actor_SetModeById(u32 id, s32 mode);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_CenterOnColumn(s32 id, s32 column, s32 offset);
void FieldEvent_WaitFrames(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000ff0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001010(void);
void Func_020010f0(s32 value);
void Func_02001108(s32 value);
void Func_02001128(s32 value);

static __inline__ void Func_02000ff0_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02000ff0(a, b, c, d, e, f);
}

static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column,
                                                    s32 offset) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_CenterOnColumn(id, column, offset);
}

void Func_02000c8c(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  Func_02001010();
  Func_02001108(0);
  Func_02001128(158);
  Func_02000ff0_(51, 38, 70, 22, 1, 2);
  Actor_SetModeById(gAdventure.currentOwner, 2);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x8000, 0x4000);
  Actor_CenterOnColumn_(gAdventure.currentOwner, 2, -4);
  FieldEvent_WaitFrames(8);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020010f0(*(s16 *)(work + 340));
}

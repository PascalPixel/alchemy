#include "types.h"
#include "object_runtime.h"
#include "text/msg_ids.h"

void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Func_02002594(void);
void Func_02002640(s32 a, s32 b, s32 c, s32 d);
void Func_02003d44(s32 value);
void Func_02004b78(s32 a, s32 b, s32 c);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 actor, s32 flags);
void Func_02004d30(void);
void Func_02004d40(s32 value);
void Func_02004d78(s32 a, s32 b);
void Func_02004d80(s32 a, s32 b);
void Func_02004d88(s32 a);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004df8(s32 value, s32 mode);
void Func_02004e30(s32 cue);

static __inline__ void Camera_MoveWithinMapBounds_(s32 a, s32 b, s32 c, s32 d) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(a, b, c, d);
}

static __inline__ void Func_02004b78_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004b78(a, b, c);
}

static __inline__ void Func_02004d78_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004d78(a, b);
}

static __inline__ void Func_02004d80_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004d80(a, b);
}

static __inline__ void Func_02004df8_(s32 value, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02004df8(value, mode);
}

void Func_02002698(void) {
  Func_02003d44(1);
  Func_02004cd8((s32)MsgTheyVeDone);
  Func_02004cf0(8, 0);
  Func_02004b78_(0x40000, 0x40000, 0x10000);
  FieldEvent_WaitFrames(30);
  Func_02004cf0(5, 0);
  Func_02004db0();
  Func_02004e30(145);
  Func_02004d80_(0x7fff, 0);
  Func_02004d78_(0x10000, 0);
  Func_02004d88(60);
  Scheduler_AddOrUpdateCallback(Func_02002594, 0x480);
  Func_02004b78_(0x50000, 0x50000, 0x10000);
  FieldEvent_WaitFrames(30);
  Func_02004b78_(0x70000, 0x60000, 0x10000);
  FieldEvent_WaitFrames(30);
  Func_02004da8();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Func_02004cf0(9, 0);
  Func_02004db0();
  Func_02004b78_(0x70000, 0x70000, 0x10000);
  Camera_MoveWithinMapBounds_(0x07c40000, -1, 0x1d810000, 1);
  Func_02004df8_(0x9999, 60);
  Func_02004d30();
  FieldEvent_WaitFrames(30);
  Func_02004da8();
  Func_02004cf0(5, 0);
  Func_02004cf0(8, 0);
  Func_02004db0();
  Func_02004e30(145);
  Func_02002640(0, 128, 0, 0);
  Object_SetModeById(5, 19);
  Object_SetModeById(8, 5);
  Object_SetModeById(9, 7);
  WaitFrames(4);
  Func_02004e30(145);
  Func_02002640(32, 128, 32, 0);
  WaitFrames(4);
  Func_02002640(64, 128, 32, 0);
  WaitFrames(4);
  Camera_MoveWithinMapBounds_(0x08040000, -1, 0x1d810000, 1);
  Func_02004d30();
  Func_02004da8();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Func_02004cf0(9, 0);
  Func_02004cf0(5, 0);
  Func_02004db0();
  Func_02004b78_(0x30000, 0x30000, 0x10000);
  Func_02002640(96, 128, 32, 0);
  WaitFrames(20);
  Func_02002640(0, 160, 32, 0);
  WaitFrames(20);
  Func_02002640(32, 160, 32, 0);
  Camera_MoveWithinMapBounds_(0x08640000, -1, 0x1d810000, 1);
  Func_02004d30();
  Func_02004da8();
  Func_02004cf0(8, 0);
  Func_02004cf0(5, 0);
  Func_02004e30(78);
  Func_02004e30(390);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d40(64);
  FieldEvent_WaitFrames(120);
}

#include "types.h"
#include "text/msg_ids.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

extern const u8 gData_02005944[];
extern const u8 gData_02005d7c[];
extern u32 gData_02005ccc[];
extern u32 gData_02005d24[];
extern u32 gData_02005d8c[];
struct ObjectRuntime *Object_GetById(s32 id);
u8 *UnitGet(s32 owner);
void Func_0200533c(void);
void Func_02005364(void);
void Func_02005724(s32 id, s32 target);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void WaitFrames(s32 frames);
void Func_02005754(const u8 *message);
void Func_02005764(s32 id, s32 value);
void Func_02005714(s32 id, s32 a, s32 b);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02001b38(void);
void Func_02001b68(void);
void Func_02002394(void);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
void Func_02005674(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020053bc(const u8 *data);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);
void Func_0200582c(s32 index, s32 a, s32 b);
s32 Func_02003604(void);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Func_020048a4(s32 x, s32 z, s32 value, s32 id);
void Func_0200571c(s32 id, s32 x, s32 z, s32 speed);
void Func_0200564c(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057a4(s32 speed, s32 acceleration);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void Func_02005774(s32 id, s32 value);
void Func_020057b4(void);
void Func_02004f40(s32 *x_out, s32 *z_out);
struct ObjectRuntime *Func_0200563c(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_02005624(struct ObjectRuntime *object, s32 mode);
s32 ArcTan2(s32 y, s32 x);
void Func_02005644(struct ObjectRuntime *object);
void Func_02005794(s32 id, s32 value);
void FieldEvent_WaitFrames(s32 frames);
void ObjectMotion_Launch(s32 id, s32 a, s32 b);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Func_0200578c(s32 id, s32 a, s32 b);
void Func_020057e4(s32 a, s32 b);
void Func_020057dc(s32 a, s32 b);
void Func_020057ec(s32 a);
void Func_020057c4(s32 exit);

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Slide(s32 id, s32 x, s32 z, s32 speed) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_0200571c(id, x, z, speed);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Walk(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Func_02005774_(s32 id, s32 value) {
  /* FAKEMATCH: forwarding through the inline sets the id between the halves of
   * the split constant, as the native does; called directly the constant is
   * built first (one reordered pair, differ 2). */
  Func_02005774(id, value);
}

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_02002f70(void) {
  s32 index = Func_02003604();
  s32 x;
  s32 z;
  struct ObjectRuntime *spark;
  struct ObjectRuntime *actor;
  s32 fast;
  s32 slow;

  Func_020056b4();
  Func_0200580c(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_020048a4(19, 11, gData_02005d8c[index], 16);
  Object_SetModeById(15, 5);
  Func_02005714(11, 0, 0);
  Func_02005714(12, 0, 0);
  Func_02005714(13, 0, 0);
  Func_02005714(14, 0, 0);
  Func_02005714(16, 0, 0);
  Func_02005714(17, 0, 0);
  Func_02005714(18, 0, 0);
  Func_02005714(19, 0, 0);
  Func_02005714(20, 0, 0);
  Func_02005714(21, 0, 0);
  Func_02005714(22, 0, 0);
  WaitFrames(1);
  Camera_SetBounds(220 << 17, -1, 182 << 16, 0);
  Func_02005714(gPartyState.currentOwner, 0, 0);
  fast = 0x4000;
  Actor_Slide(5, 236 << 17, 216 << 16, fast);
  slow = 0x3000;
  Actor_Slide(24, 236 << 17, 200 << 16, slow);
  WaitFrames(1);
  Func_0200564c();
  WaitFrames(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057a4(0x13333, 0x2666);
  Camera_SetBounds(220 << 17, -1, 156 << 17, 1);
  Actor_SetSpeed(5, 0x10000, 0x8000);
  Actor_SetSpeed(24, 0x10000, 0x8000);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02005ccc);
  Object_SetActionCallbackAndRefreshById(24, gData_02005d24);
  Func_02005754(MsgGabombaSawEnergyFlow);
  Func_02005764(5, 0);
  Actor_Turn(24, 0xb000, 0);
  Actor_Walk(5, 440, 296);
  Func_02005774_(5, 0);
  Func_02005764(5, 0);
  Actor_Turn(5, 0xa000, 0);
  Camera_SetBounds(207 << 17, -1, 136 << 17, 1);
  Func_020057b4();
  Func_02005764(5, 0);
  Func_02004f40(&x, &z);
  x = (x << 20) + (156 << 17);
  z = (z << 20) + (184 << 16);
  spark = Func_0200563c(434, x, 0, z);
  spark->motionFlags |= 4;
  ObjectDispatch_SetSingleChildFlags(spark, 0);
  Func_02005624(spark, 1);
  WaitFrames(1);
  actor = Object_GetById(5);
  actor->angle = ArcTan2(spark->z - actor->z, spark->x - actor->x);
  /* FAKEMATCH: the native stores the angle before it starts the camera call's
   * constants; without the barrier the -1 is started first (one reordered pair,
   * differ 2; with x and z read into locals first, differ 16). */
  asm volatile("");
  Camera_SetBounds(spark->x, -1, spark->z, 1);
  Func_020057b4();
  Func_02005764(5, 0);
  Func_02005644(spark);
  Camera_SetBounds(220 << 17, -1, 152 << 17, 1);
  Func_020057b4();
  Func_02005774_(5, 0x2000);
  Func_02005764(5, 0);
  Func_02005794(24, 258);
  FieldEvent_WaitFrames(20);
  Func_02005764(24, 0);
  Actor_Slide(gPartyState.currentOwner, 236 << 17, 204 << 16, fast);
  Actor_Slide(7, 220 << 17, 204 << 16, fast);
  Actor_Slide(23, 228 << 17, 204 << 16, slow);
  Actor_Slide(6, 244 << 17, 204 << 16, fast);
  WaitFrames(1);
  Func_02005764(23, 0);
  Actor_Turn(24, 0xd000, 0);
  Camera_SetBounds(220 << 17, -1, 132 << 17, 1);
  Func_020057b4();
  ObjectMotion_Launch(24, 4, 10);
  Func_02005764(0x4018, 0);
  Motion_SetModeAndWaitAnimation(7, 4);
  Func_02005764(0x8007, 0);
  Func_0200578c(24, 257, 40);
  Func_02005764(0x8006, 0);
  Object_SetModeById(23, 4);
  Func_02005764(0x9017, 0);
  FieldEvent_WaitFrames(20);
  Func_020057e4(0x10000, 0);
  Func_020057dc(0, 0);
  Func_020057ec(60);
  GameFlag_ClearBit(265);
  WaitFrames(60);
  Func_020057c4(18);
}

void Func_02003338(void) {
  Func_0200533c();
  Func_02005724(23, gPartyState.currentOwner);
  Object_SetPartAttribute(Object_GetById(23), 15);
  WaitFrames(10);
  Func_02005754(MsgGabombaChannelTiming);
  Func_02005764(23, 0);
  Func_02005714(23, 0, 0);
  WaitFrames(1);
  GameFlag_SetBit(0x9ec);
  Func_02005364();
}

#include "types.h"
#include "text/msg_ids.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct Wanderer {
  u8 unk00[98];
  u8 timer;
};

extern u32 gData_02005af4[];
extern u32 gData_02005b1c[];
extern u32 gData_02005b60[];
extern u32 gData_02005ba4[];
extern u32 gData_02005be8[];
extern s32 gData_02005c74[];
extern s32 gData_02005ca0[];
struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct ObjectRuntime *Func_020057bc(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_WaitTicks(s32 ticks);
void Func_02005754(const u8 *message);
void Func_02005764(s32 id, s32 value);
void Func_02005724(s32 id, s32 target);
void WaitTicks(s32 ticks);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *callback);
void Object_SetActionCallbackAndRefreshById(s32 actor, const void *callback);
void Func_020057a4(s32 speed, s32 acceleration);
void Func_02005774(s32 id, s32 value);
void Func_020057b4(void);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Func_020057e4(s32 a, s32 b);
void Func_020057dc(s32 a, s32 b);
void Func_020057ec(s32 a);
void Func_020057c4(s32 exit);
void Func_02005854(s32 sound);
void Func_0200571c(s32 id, s32 x, s32 z, s32 speed);
void Func_02000f6c(void);
void Func_0200578c(s32 id, s32 a, s32 b);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
u32 Random16Far(void);
s32 Func_02004d48(s32 a, s32 b);
void GameFlag_SetBit(s32 flag);
void Event_FinishScreenTransition(void);

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 mode) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void Camera_SetSpeed(s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020057a4(speed, acceleration);
}

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  ObjectMotion_ResetAndSetPositionInMode2(id, x, z);
}

static __inline__ void Func_02005764_(s32 id, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005764(id, value);
}

static __inline__ void Func_02005774_(s32 id, s32 value) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02005774(id, value);
}

static __inline__ void Func_020057e4_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020057e4(a, b);
}

static __inline__ void Func_020057dc_(s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020057dc(a, b);
}

static __inline__ void Actor_Slide(s32 id, s32 x, s32 z, s32 speed) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200571c(id, x, z, speed);
}

static __inline__ void Func_0200578c_(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200578c(id, a, b);
}

static __inline__ void Func_0200578c_ArgsFirst(s32 id, s32 a, s32 b) {
  /* FAKEMATCH: the second and third arguments pinned to r1 and r2 are set
   * before the id at four of Func_020027cc's five calls; as direct calls, 24
   * instructions differ in tla-en. */
  register s32 a_ asm("r1") = a;
  register s32 b_ asm("r2") = b;

  Func_0200578c(id, a_, b_);
}

void Func_0200253c(void) {
  Func_020056b4();
  Func_0200580c(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_020057bc()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitTicks(40);
  Func_02005754(MsgGabombaFocusEnergies);
  Func_02005764_(24, 0);
  Func_02005724(23, gPartyState.currentOwner);
  Func_02005724(5, gPartyState.currentOwner);
  Func_02005724(6, gPartyState.currentOwner);
  Func_02005724(7, gPartyState.currentOwner);
  WaitTicks(1);
  Actor_SetSpeed(gPartyState.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(23, 0xcccc, 0x6666);
  ObjectMotion_EnableActionAndSetCallback(23, gData_02005ba4);
  WaitTicks(10);
  ObjectMotion_EnableActionAndSetCallback(gPartyState.currentOwner,
                                          gData_02005af4);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02005b1c);
  ObjectMotion_EnableActionAndSetCallback(7, gData_02005b60);
  Object_RefreshSelectorById(23);
  FieldEvent_WaitTicks(20);
  Camera_SetSpeed(0x13333, 0x2666);
  Camera_SetBounds(217 << 17, -1, 143 << 17, 1);
  Actor_SetSpeed(23, 0x13333, 0x9999);
  Object_SetActionCallbackAndRefreshById(23, gData_02005be8);
  FieldEvent_WaitTicks(40);
  Func_02005774_(23, 0xd000);
  Func_02005764_(23, 0);
  Camera_SetSpeed(0x4cccc, 0x9999);
  Camera_SetBounds(217 << 17, -1, 168 << 16, 1);
  Func_020057b4();
  FieldEvent_WaitTicks(20);
  Actor_Turn(6, 0x2000, 0);
  Actor_Turn(5, 0xa000, 0);
  Actor_Turn(gPartyState.currentOwner, 0xa000, 0);
  Actor_Turn(7, 0x2000, 40);
  Object_GetById(7)->angle = 0x4000;
  Object_GetById(gPartyState.currentOwner)->angle = 0x4000;
  Object_GetById(6)->angle = 0x4000;
  Object_GetById(5)->angle = 0x4000;
  Object_GetById(7)->actionFlags &= 0xfe;
  Object_GetById(gPartyState.currentOwner)->actionFlags &= 0xfe;
  Object_GetById(6)->actionFlags &= 0xfe;
  Object_GetById(5)->actionFlags &= 0xfe;
  Actor_Place(7, 400, 272);
  Actor_Place(gPartyState.currentOwner, 420, 272);
  Actor_Place(6, 444, 272);
  Actor_Place(5, 468, 272);
  FieldEvent_WaitTicks(20);
  Func_020057e4(0x10000, 0);
  Func_020057dc(0, 0);
  Func_020057ec(80);
  WaitTicks(80);
  Func_020057c4(11);
}

void Func_020027cc(void) {
  s32 speed;

  Func_020056b4();
  Func_0200580c(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_02005854(78);
  speed = 0x4000;
  Actor_Slide(gPartyState.currentOwner, 174 << 17, 156 << 17, speed);
  Actor_Slide(7, 165 << 17, 156 << 17, speed);
  {
    s32 slow = 0x3000;

    Actor_Slide(23, 216 << 17, 156 << 17, slow);
  }
  Actor_Slide(6, 224 << 17, 156 << 17, speed);
  Actor_Slide(5, 234 << 17, 156 << 17, speed);
  WaitTicks(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitTicks(40);
  Func_02005854(220);
  WaitTicks(1);
  Func_020057e4_(0x10000, 0);
  Func_020057dc_(0x7fff, 0);
  Func_020057ec(40);
  WaitTicks(80);
  Func_02000f6c();
  WaitTicks(1);
  Func_020057dc_(0x10000, 0);
  Func_020057ec(10);
  WaitTicks(20);
  Func_0200578c_(gPartyState.currentOwner, 256, 0);
  Func_0200578c_ArgsFirst(5, 256, 0);
  Func_0200578c_ArgsFirst(6, 256, 0);
  Func_0200578c_ArgsFirst(7, 256, 0);
  Func_0200578c_ArgsFirst(23, 256, 0);
  Actor_Turn(gPartyState.currentOwner, 0xc000, 0);
  Actor_Turn(5, 0xc000, 0);
  Actor_Turn(6, 0xc000, 0);
  Actor_Turn(7, 0xc000, 0);
  Actor_Turn(23, 0xb000, 60);
  Func_02005854(130);
  Object_SetPartAttribute(Object_GetById(9), 7);
  Object_SetPartAttribute(Object_GetById(10), 7);
  WaitTicks(8);
  Object_SetPartAttribute(Object_GetById(10), 3);
  Object_SetPartAttribute(Object_GetById(9), 0);
  WaitTicks(8);
  ((struct Wanderer *)Object_GetById(gPartyState.currentOwner))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Object_GetById(7))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Object_GetById(5))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Object_GetById(6))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Object_GetById(23))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ObjectMotion_EnableActionAndSetCallback(gPartyState.currentOwner,
                                          gData_02005c74);
  ObjectMotion_EnableActionAndSetCallback(7, gData_02005c74);
  ObjectMotion_EnableActionAndSetCallback(5, gData_02005ca0);
  ObjectMotion_EnableActionAndSetCallback(6, gData_02005ca0);
  ObjectMotion_EnableActionAndSetCallback(23, gData_02005ca0);
  Func_02005854(11);
  Func_02004d48(11, 0);
  FieldEvent_WaitTicks(80);
  GameFlag_SetBit(0x8f7);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057c4(12);
}

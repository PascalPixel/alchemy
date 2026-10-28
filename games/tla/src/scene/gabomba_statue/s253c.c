#include "types.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_tables.h"

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
struct Actor *Actor_GetById(s32 id);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
struct Actor *Func_020057bc(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_Wait(s32 frames);
void Func_02005754(const u8 *message);
void Func_02005764(s32 id, s32 value);
void Func_02005724(s32 id, s32 target);
void wait(s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetMovementTypeById(s32 id, const void *callback);
void Actor_SetMovementTypeAndWaitById(s32 actor, const void *callback);
void Func_020057a4(s32 speed, s32 acceleration);
void Func_02005774(s32 id, s32 value);
void Func_020057b4(void);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void Func_020057e4(s32 a, s32 b);
void Func_020057dc(s32 a, s32 b);
void Func_020057ec(s32 a);
void Func_020057c4(s32 exit);
void Func_02005854(s32 sound);
void Func_0200571c(s32 id, s32 x, s32 z, s32 speed);
void Func_02000f6c(void);
void Func_0200578c(s32 id, s32 a, s32 b);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
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
  Actor_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Place(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ResetAndSetPositionInMode2(id, x, z);
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
  FieldEvent_Wait(40);
  Func_02005754(MsgGabombaFocusEnergies);
  Func_02005764_(24, 0);
  Func_02005724(23, gMainRoutineWork.currentOwner);
  Func_02005724(5, gMainRoutineWork.currentOwner);
  Func_02005724(6, gMainRoutineWork.currentOwner);
  Func_02005724(7, gMainRoutineWork.currentOwner);
  wait(1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_SetSpeed(5, 0xcccc, 0x6666);
  Actor_SetSpeed(6, 0xcccc, 0x6666);
  Actor_SetSpeed(7, 0xcccc, 0x6666);
  Actor_SetSpeed(23, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(23, gData_02005ba4);
  wait(10);
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02005af4);
  Actor_SetMovementTypeById(5, gData_02005b1c);
  Actor_SetMovementTypeById(7, gData_02005b60);
  Actor_RefreshSelectorById(23);
  FieldEvent_Wait(20);
  Camera_SetSpeed(0x13333, 0x2666);
  Camera_SetBounds(PIXELS(434), -1, PIXELS(286), 1);
  Actor_SetSpeed(23, 0x13333, 0x9999);
  Actor_SetMovementTypeAndWaitById(23, gData_02005be8);
  FieldEvent_Wait(40);
  Func_02005774_(23, FACING_NORTH + FACING_STEP);
  Func_02005764_(23, 0);
  Camera_SetSpeed(0x4cccc, 0x9999);
  Camera_SetBounds(PIXELS(434), -1, PIXELS(168), 1);
  Func_020057b4();
  FieldEvent_Wait(20);
  Actor_Turn(6, FACING_SOUTHEAST, 0);
  Actor_Turn(5, FACING_NORTHWEST, 0);
  Actor_Turn(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_Turn(7, FACING_SOUTHEAST, 40);
  Actor_GetById(7)->angle = FACING_SOUTH;
  Actor_GetById(gMainRoutineWork.currentOwner)->angle = 0x4000;
  Actor_GetById(6)->angle = FACING_SOUTH;
  Actor_GetById(5)->angle = FACING_SOUTH;
  Actor_GetById(7)->actionFlags &= 0xfe;
  Actor_GetById(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_GetById(6)->actionFlags &= 0xfe;
  Actor_GetById(5)->actionFlags &= 0xfe;
  Actor_Place(7, 400, 272);
  Actor_Place(gMainRoutineWork.currentOwner, 420, 272);
  Actor_Place(6, 444, 272);
  Actor_Place(5, 468, 272);
  FieldEvent_Wait(20);
  Func_020057e4(0x10000, 0);
  Func_020057dc(0, 0);
  Func_020057ec(80);
  wait(80);
  Func_020057c4(11);
}

void Func_020027cc(void) {
  s32 speed;

  Func_020056b4();
  Func_0200580c(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_02005854(78);
  speed = 0x4000;
  Actor_Slide(gMainRoutineWork.currentOwner, PIXELS(348), PIXELS(312), speed);
  Actor_Slide(7, PIXELS(330), PIXELS(312), speed);
  {
    s32 slow = 0x3000;

    Actor_Slide(23, PIXELS(432), PIXELS(312), slow);
  }
  Actor_Slide(6, PIXELS(448), PIXELS(312), speed);
  Actor_Slide(5, PIXELS(468), PIXELS(312), speed);
  wait(1);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x202;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Func_02005854(220);
  wait(1);
  Func_020057e4_(0x10000, 0);
  Func_020057dc_(0x7fff, 0);
  Func_020057ec(40);
  wait(80);
  Func_02000f6c();
  wait(1);
  Func_020057dc_(0x10000, 0);
  Func_020057ec(10);
  wait(20);
  Func_0200578c_(gMainRoutineWork.currentOwner, 256, 0);
  Func_0200578c_ArgsFirst(5, 256, 0);
  Func_0200578c_ArgsFirst(6, 256, 0);
  Func_0200578c_ArgsFirst(7, 256, 0);
  Func_0200578c_ArgsFirst(23, 256, 0);
  Actor_Turn(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_Turn(5, FACING_NORTH, 0);
  Actor_Turn(6, FACING_NORTH, 0);
  Actor_Turn(7, FACING_NORTH, 0);
  Actor_Turn(23, FACING_NORTHWEST + FACING_STEP, 60);
  Func_02005854(130);
  Actor_SetPartAttribute(Actor_GetById(9), 7);
  Actor_SetPartAttribute(Actor_GetById(10), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(10), 3);
  Actor_SetPartAttribute(Actor_GetById(9), 0);
  wait(8);
  ((struct Wanderer *)Actor_GetById(gMainRoutineWork.currentOwner))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(7))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(5))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(6))->timer =
      (Random16Far() * 40 >> 16) + 10;
  ((struct Wanderer *)Actor_GetById(23))->timer =
      (Random16Far() * 40 >> 16) + 10;
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner, gData_02005c74);
  Actor_SetMovementTypeById(7, gData_02005c74);
  Actor_SetMovementTypeById(5, gData_02005ca0);
  Actor_SetMovementTypeById(6, gData_02005ca0);
  Actor_SetMovementTypeById(23, gData_02005ca0);
  Func_02005854(11);
  Func_02004d48(11, 0);
  FieldEvent_Wait(80);
  GameFlag_SetBit(0x8f7);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020057c4(12);
}

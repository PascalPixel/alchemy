// Draft, differ 148 of 303: the rescue by boat (actor 10): it sails in,
// the camera zooms, and the party lands on the far shore. Left: the native
// keeps the map work in r7 (this draft uses r8 and saves another register)
// and places its pool at the end; the -1 arguments go through Call4.
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"

extern char MsgWeReSaved[];

void GameFlag_SetBit(s32 flag);
void WaitTicks(s32 ticks);
void FieldEvent_WaitTicks(s32 ticks);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPositionInMode2(s32 id, s32 x, s32 z);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void ObjectMotion_SetAngleToward(s32 id, s32 target, s32 mode);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020042e0(s32 mode);
void Func_02004b40(void);
void Func_02004b50(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02004b68(s32 a, s32 b, s32 c, s32 d);
void Func_02004b78(s32 a, s32 b, s32 c);
void Func_02004c20(void);
void Func_02004c88(s32 id, s32 x, s32 z);
void Func_02004cd8(s32 message);
void Func_02004cf0(s32 id, s32 mode);
void Func_02004d10(s32 id, s32 animation);
void Func_02004d20(s32 a, s32 b);
void Func_02004d40(s32 entrance);
void Func_02004da8(void);
void Func_02004db0(void);
void Func_02004db8(s32 value);
void Func_02004df8(s32 a, s32 b);
void Func_02004e30(s32 cue);

#define MAP_HALF(map, offset) (*(u16 *)((map) + (offset)))

void Func_02002ab0(void) {
  struct ObjectRuntime *boat = Object_GetById(10);
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 i;

  Func_02004c20();
  Func_02004db8(0);
  Func_02004b68(64, 176, 304, 240);
  Func_02004b68(80, 176, 320, 240);
  boat->scaleX = 0x13333;
  boat->scaleY = 0x13333;
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  WaitTicks(1);
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 15);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(10), 0);
  ObjectMotion_SetSpeedParameters(10, 0x6666, 0x3333);
  boat->action = 0;
  boat->acceleration = 0x2000;
  boat->speedLimit = 0x2000;
  Func_02004b50(boat, 0x27ff0000, boat->y, 0x1ecc0000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004da8();
  Func_02004cd8((s32)MsgWeReSaved);
  Func_02004cf0(5, 0);
  Func_02004cf0(8, 0);
  Func_02004b50(boat, 0x280f0000, boat->y, 0x1ebc0000);
  Func_02004cf0(6, 0);
  Func_02004cf0(5, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(8, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(5, 0);
  Func_02004e30(78);
  Func_02004e30(162);
  Func_02004cf0(gPartyState.currentOwner, 0);
  Func_02004cf0(8, 0);
  Func_02004e30(61);
  Func_02004db0();
  Func_02004d20(0x40000, 0x8000);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x283f0000, -1, 0x1e8c0000, 1);
  for (i = 31; i >= 0; i--) {
    MAP_HALF(map, 0x11e) += 0xff80;
    MAP_HALF(map, 0x11c) += 128;
    Object_GetById(10)->scaleY += -0x300;
    WaitTicks(1);
  }
  Func_02004da8();
  GameFlag_SetBit(0x200);
  Func_02004cf0(5, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(5, 0);
  Func_02004cf0(6, 0);
  Func_02004cf0(8, 0);
  Func_02004b78(0x10000, 0x20000, 0x10000);
  Func_02004db0();
  Func_02004df8(0x9999, 1);
  MAP_HALF(map, 0x11e) = 0;
  MAP_HALF(map, 0x11c) = 0xe000;
  WaitTicks(1);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x0e470000, -1, 0x1dd00000, 0);
  Func_02004b40();
  Func_02004c88(8, 0x0e6a0000, 0x1dc00000);
  Func_02004c88(9, 0x0e3e0000, 0x1dff0000);
  Func_02004c88(4, 0x0e550000, 0x1df00000);
  Func_02004c88(5, 0x0e490000, 0x1dc50000);
  Func_02004c88(6, 0x0e770000, 0x1de30000);
  Object_SetPartAttribute(Object_GetById(gPartyState.currentOwner), 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     1);
  WaitTicks(1);
  Func_02004da8();
  Func_02004d10(5, 0x102);
  FieldEvent_WaitTicks(20);
  Func_02004cf0(5, 0);
  Motion_SetModeAndWaitAnimation(9, 4);
  Func_02004cf0(9, 0);
  ObjectMotion_Launch(6, 6, 15);
  ObjectMotion_Launch(6, 6, 23);
  ObjectMotion_SetAngleToward(6, 4, 0);
  Func_02004cf0(6, 0);
  Func_02004d10(8, 0x101);
  Func_02004cf0(8, 0);
  ObjectMotion_SetSpeedParameters(8, 0x10000, 0x8000);
  ObjectMotion_ResetAndSetPositionInMode2(8, 0xe18, 0x1e0f);
  Func_02004e30(163);
  Func_020042e0(0);
  Func_02004d40(65);
}

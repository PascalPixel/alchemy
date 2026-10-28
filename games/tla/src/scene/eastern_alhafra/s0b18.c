#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) ((struct Actor *)Value1(Actor_GetById, (id)))

extern const u8 gData_02004d0c[];
void Func_02000af8(void);
s32 Actor_GetById();
s32 GameFlag_Test();
void Func_02004b04();
void Actor_ArmCallback();
void Func_02004c5c();
void Func_02004c54();
void Actor_SetSingleChildFlags();
void Actor_SetModeById();
void Func_02004a5c();
void Func_02004a54();
void Actor_SetSpritePriority();
void Func_02003274(void);
void Func_02000fa4(void);
void Func_020040ec();
void Func_02004c14(void);
void Func_02004c2c();
void Func_02004c1c(void);
void Func_02004c24();
void Func_020047fc();
void Func_02004c94();

s32 Func_02000b18(void) {
  struct Actor *object;
  s32 flag;

  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Func_02000af8();
  OBJECT(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (Value1(GameFlag_Test, 0x8a7) != 0) {
    if (Value1(GameFlag_Test, 0x905) != 0 &&
        Value1(GameFlag_Test, 0x90f) == 0) {
      Call3(Func_02004b04, 30, PIXELS(184), PIXELS(616));
      Call3(Func_02004b04, 31, PIXELS(136), PIXELS(600));
      Call3(Func_02004b04, 28, PIXELS(168), PIXELS(584));
      Call3(Actor_ArmCallback, 28, FACING_SOUTH, 0);
      Call3(Func_02004c5c, 30, 0, 13);
      Call3(Func_02004c5c, 31, 0x8000, 13);
      Call3(Func_02004c5c, 28, 0, 13);
      Call3(Func_02004b04, 25, PIXELS(112), PIXELS(432));
      Call3(Func_02004b04, 27, PIXELS(176), PIXELS(408));
      Call3(Func_02004b04, 29, PIXELS(152), PIXELS(376));
      Call3(Actor_ArmCallback, 29, FACING_SOUTH, 0);
      Call2(Func_02004c54, 25, 0x4000);
      Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 25), 0);
      Call2(Func_02004c54, 27, 0xc000);
      Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 27), 0);
      Call2(Func_02004c54, 29, 0x4000);
      Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 29), 0);
      Call2(Actor_SetModeById, 25, 13);
      Call2(Actor_SetModeById, 27, 13);
      Call2(Actor_SetModeById, 29, 13);
    }
    if (Value1(GameFlag_Test, 0x905) != 0) {
      Call6(Func_02004a5c, 40, 0, 24, 31, 11, 6);
      Call6(Func_02004a5c, 84, 15, 10, 10, 75, 15);
      Call6(Func_02004a54, 10, 24, 1, 1, 11, 24);
      Call3(Func_02004b04, 9, 0, 0);
      Call3(Func_02004b04, 8, 0, 0);
      Call3(Func_02004b04, 10, 0, 0);
      Call3(Func_02004b04, 11, 0, 0);
      Call3(Func_02004b04, 15, 0, 0);
    }
  }
  if (gMainRoutineWork.entrance == 99) {
    gMainRoutineWork.secondary = 1;
    gMainRoutineWork.entrance = 1;
    Call3(Func_02004b04, 4, PIXELS(136), PIXELS(552));
    Call3(Func_02004b04, 26, PIXELS(120), PIXELS(552));
    Call3(Func_02004b04, 30, PIXELS(184), PIXELS(616));
    Call3(Func_02004b04, 31, PIXELS(136), PIXELS(600));
    Call3(Func_02004b04, 28, PIXELS(160), PIXELS(584));
    Call2(Func_02004c54, 30, 0x4000);
    Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 30), 0);
    Call2(Func_02004c54, 31, 0xc000);
    Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 31), 0);
    Call2(Func_02004c54, 28, 0x4000);
    Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 28), 0);
    Call2(Actor_SetSpritePriority, 27, 2);
    Call2(Actor_SetSpritePriority, 25, 2);
    Call2(Actor_SetSpritePriority, 29, 2);
    Call3(Func_02004b04, 25, PIXELS(160), PIXELS(392));
    Call3(Func_02004b04, 27, PIXELS(176), PIXELS(408));
    Call3(Func_02004b04, 29, PIXELS(152), PIXELS(376));
    Call2(Func_02004c54, 27, 0xc000);
    Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 27), 0);
    Call2(Func_02004c54, 29, 0x4000);
    Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 29), 0);
    Call3(Actor_ArmCallback, 25, 0xd000, 0);
    Call3(Actor_ArmCallback, 29, FACING_SOUTH, 0);
    Call2(Actor_SetModeById, 27, 13);
    Call2(Actor_SetModeById, 29, 13);
    Call2(Actor_SetModeById, 30, 13);
    Call2(Actor_SetModeById, 31, 13);
    Call2(Actor_SetModeById, 28, 13);
    Func_02003274();
  }
  Func_02000fa4();
  if (Value1(GameFlag_Test, 0x8a6) == 0)
    Call1(Func_020040ec, 15);
  Func_02004c14();
  Call4(Func_02004c2c, 0, 0x8a3, 8, 9);
  OBJECT(8)->priorityFlags |= 32;
  OBJECT(9)->priorityFlags |= 32;
  Func_02004c1c();
  Call3(Func_02004c24, 0, 12, 13);
  Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 12), 0);
  Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 13), 0);
  Call1(Func_020047fc, gData_02004d0c);
  Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 14), 0);
  flag = Value1(GameFlag_Test, 0x8a2);
  if (flag == 0) {
    object = OBJECT(18);
    /* FAKEMATCH(one-pass): the solid bit set in a once-only block, which lays
     * out the branch over this block as the native does; as a plain
     * statement, 18 of 467 instructions differ in tla-en. */
    do {
      object->collisionFlags |= 8;
    } while (0);
    object = OBJECT(19);
    object->motionFlags = flag;
    object->y = 0x100000;
    Call3(Func_02004c24, 1, 18, 19);
    Call1(Func_02004c94, 1);
    object = OBJECT(20);
    object->collisionFlags |= 8;
    object = OBJECT(21);
    object->motionFlags = flag;
    object->y = 0x100000;
    Call3(Func_02004c24, 2, 20, 21);
  }
  Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 16), 0);
  Call2(Actor_SetSingleChildFlags, Value1(Actor_GetById, 17), 0);
  Call2(Actor_SetSpritePriority, 10, 2);
  ((u8 *)OBJECT(23))[0x62] = 1;
  if (Value1(GameFlag_Test, 0x8ab) != 0 && Value1(GameFlag_Test, 0x8a7) == 0) {
    Call3(Func_02004b04, 22, 0, 0);
    Call3(Func_02004b04, 23, 0, 0);
    Call3(Func_02004b04, 24, 0, 0);
    Call3(Func_02004b04, 25, 0, 0);
  }
  return 0;
}

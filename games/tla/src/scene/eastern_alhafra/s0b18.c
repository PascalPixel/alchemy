#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "call.h"

#define EVENT_WORK ((u8 *)Ram_HeapSlots->eventWork)
#define OBJECT(id) ((struct ObjectRuntime *)Value1(Object_GetById, (id)))

extern const u8 gData_02004d0c[];
void Func_02000af8(void);
s32 Object_GetById();
s32 GameFlag_Test();
void Func_02004b04();
void ObjectMotion_ArmCallback();
void Func_02004c5c();
void Func_02004c54();
void ObjectDispatch_SetSingleChildFlags();
void Object_SetModeById();
void Func_02004a5c();
void Func_02004a54();
void ObjectMotion_SetActionVariant();
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
  struct ObjectRuntime *object;
  s32 flag;

  *(s32 *)(EVENT_WORK + 0x1ac) = 0x100;
  Func_02000af8();
  OBJECT(gAdventure.currentOwner)->priorityFlags |= 32;
  if (Value1(GameFlag_Test, 0x8a7) != 0) {
    if (Value1(GameFlag_Test, 0x905) != 0 &&
        Value1(GameFlag_Test, 0x90f) == 0) {
      Call3(Func_02004b04, 30, 0xb80000, 0x2680000);
      Call3(Func_02004b04, 31, 0x880000, 0x2580000);
      Call3(Func_02004b04, 28, 0xa80000, 0x2480000);
      Call3(ObjectMotion_ArmCallback, 28, 0x4000, 0);
      Call3(Func_02004c5c, 30, 0, 13);
      Call3(Func_02004c5c, 31, 0x8000, 13);
      Call3(Func_02004c5c, 28, 0, 13);
      Call3(Func_02004b04, 25, 0x700000, 0x1b00000);
      Call3(Func_02004b04, 27, 0xb00000, 0x1980000);
      Call3(Func_02004b04, 29, 0x980000, 0x1780000);
      Call3(ObjectMotion_ArmCallback, 29, 0x4000, 0);
      Call2(Func_02004c54, 25, 0x4000);
      Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 25), 0);
      Call2(Func_02004c54, 27, 0xc000);
      Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 27), 0);
      Call2(Func_02004c54, 29, 0x4000);
      Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 29), 0);
      Call2(Object_SetModeById, 25, 13);
      Call2(Object_SetModeById, 27, 13);
      Call2(Object_SetModeById, 29, 13);
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
  if (gAdventure.entrance == 99) {
    gAdventure.secondary = 1;
    gAdventure.entrance = 1;
    Call3(Func_02004b04, 4, 0x880000, 0x2280000);
    Call3(Func_02004b04, 26, 0x780000, 0x2280000);
    Call3(Func_02004b04, 30, 0xb80000, 0x2680000);
    Call3(Func_02004b04, 31, 0x880000, 0x2580000);
    Call3(Func_02004b04, 28, 0xa00000, 0x2480000);
    Call2(Func_02004c54, 30, 0x4000);
    Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 30), 0);
    Call2(Func_02004c54, 31, 0xc000);
    Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 31), 0);
    Call2(Func_02004c54, 28, 0x4000);
    Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 28), 0);
    Call2(ObjectMotion_SetActionVariant, 27, 2);
    Call2(ObjectMotion_SetActionVariant, 25, 2);
    Call2(ObjectMotion_SetActionVariant, 29, 2);
    Call3(Func_02004b04, 25, 0xa00000, 0x1880000);
    Call3(Func_02004b04, 27, 0xb00000, 0x1980000);
    Call3(Func_02004b04, 29, 0x980000, 0x1780000);
    Call2(Func_02004c54, 27, 0xc000);
    Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 27), 0);
    Call2(Func_02004c54, 29, 0x4000);
    Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 29), 0);
    Call3(ObjectMotion_ArmCallback, 25, 0xd000, 0);
    Call3(ObjectMotion_ArmCallback, 29, 0x4000, 0);
    Call2(Object_SetModeById, 27, 13);
    Call2(Object_SetModeById, 29, 13);
    Call2(Object_SetModeById, 30, 13);
    Call2(Object_SetModeById, 31, 13);
    Call2(Object_SetModeById, 28, 13);
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
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 12), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 13), 0);
  Call1(Func_020047fc, gData_02004d0c);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 14), 0);
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
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 16), 0);
  Call2(ObjectDispatch_SetSingleChildFlags, Value1(Object_GetById, 17), 0);
  Call2(ObjectMotion_SetActionVariant, 10, 2);
  ((u8 *)OBJECT(23))[0x62] = 1;
  if (Value1(GameFlag_Test, 0x8ab) != 0 && Value1(GameFlag_Test, 0x8a7) == 0) {
    Call3(Func_02004b04, 22, 0, 0);
    Call3(Func_02004b04, 23, 0, 0);
    Call3(Func_02004b04, 24, 0, 0);
    Call3(Func_02004b04, 25, 0, 0);
  }
  return 0;
}

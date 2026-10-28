#include "local.h"

struct ShamanVillageCaveSceneWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

s32 GameFlag_Test(s32 flag);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Func_020010e8(s32 id, s32 x, s32 z);
void Func_020003cc(void);
void Func_02000404(void);
void Func_02000a24(void);
void Func_0200087c(void);

static __inline__ void ShamanVillageCave_ObjPlace(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_020010e8(id, x, z);
}

s32 Func_02000e04(void) {
  struct ObjectRuntime *object;

  ((struct ShamanVillageCaveSceneWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 516;
  if (GameFlag_Test(0x9a9))
    Func_020010e8(8, 0, 0);
  if (GameFlag_Test(0x9aa))
    Func_020010e8(10, 0, 0);
  if (GameFlag_Test(83) || GameFlag_Test(163))
    GameFlag_SetBit(0x205);
  if (GameFlag_Test(0x9a8)) {
    object = Object_GetById(15);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->x = 0x2680000;
    object->y = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02001060_(40, 19, 1, 1, 38, 19);
  }
  ObjectDispatch_SetSingleChildFlags(Object_GetById(16), 0);
  Object_SetModeById(16, 1);
  if (GameFlag_Test(770)) {
    object = Object_GetById(16);
    Object_SetModeById(16, 5);
    Object_SetPartAttribute(Object_GetById(16), 0);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->y = 0;
    Func_02001060_(8, 42, 1, 1, 6, 42);
  }
  if (GameFlag_Test(518))
    ShamanVillageCave_ObjPlace(17, 0x1480000, 0x3180000);
  Object_SetPartAttribute(Object_GetById(19), 1);
  Object_SetPartAttribute(Object_GetById(20), 0);
  Object_SetPartAttribute(Object_GetById(21), 2);
  if ((!GameFlag_Test(265) &&
       (gAdventure.entrance == 20 || gAdventure.entrance == 21)) ||
      GameFlag_Test(0xa71)) {
    Func_020003cc();
    if (!GameFlag_Test(265) && gAdventure.entrance == 20) {
      Func_02000a24();
    } else if (!GameFlag_Test(265) && gAdventure.entrance == 21) {
      GameFlag_SetBit(0xa71);
      Func_0200087c();
    }
  }
  if (GameFlag_Test(0xa73))
    Func_02000404();
  return 0;
}

#include "kibombo.h"

void Func_020031f0(s32 id) {
  Func_02005b8c(id);
  Object_SetPartAttribute(Object_GetById(id), 15);
  ObjectMotion_SetActionVariant(id, 1);
  Object_GetById(id)->priorityFlags |= 2;
}

void Func_02003220(void) {
  if (GameFlag_Test(265)) {
    if (GameFlag_Test(0x200) == 0)
      Func_0200057c();
  } else {
    GameFlag_SetBit(0x200);
  }
}

void Func_0200324c(void) {
  u32 i;
  /* FAKEMATCH: the zero is pinned to r6 and passes an empty asm on every pass:
   * the native builds it in the loop and stores it again after the loop; with
   * the pin alone it is copied before the byte store (differ 9), and as a plain
   * 0 loop.c hoists it (differ 17). */
  register s32 zero asm("r6");

  for (i = 0; i <= 1; i++) {
    s32 id = i + 15;
    struct KibomboObject *object = Object_GetById(id);

    Func_020031f0(id);
    zero = 0;
    asm("" : "+r"(zero));
    object->motionFlags = zero;
  }
  Object_GetById(15)->y = 0x200000;
  Object_GetById(16)->y = zero;
  Func_02003220();
}

void Func_02003288(void) {
  u32 i;

  for (i = 0; i <= 1; i++) {
    s32 id = i + 26;
    struct KibomboObject *object = Object_GetById(id);

    Func_020031f0(id);
    {
      /* FAKEMATCH: the zero is pinned to r3 inside the loop, where the native
       * builds it on every pass; as a plain 0 loop.c hoists it to r8 (differ
       * 8). */
      register s32 zero asm("r3") = 0;

      object->motionFlags = zero;
    }
  }
  Func_02003220();
}

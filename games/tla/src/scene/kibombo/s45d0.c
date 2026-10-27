#include "kibombo.h"

extern const u8 gData_020061a8[];

void ObjectMotion_EnableActionAndResetMotion(s32 actor);

void Func_020045d0(void) {
  Func_02005ba4(0x4cccc, 0x9999);
  Kibombo_CamMove(132 << 17, 0, 200 << 17, 1);
  Func_02005bb4();
  Kibombo_Wait(10);
  ObjectMotion_Launch(18, 6, 0);
  ObjectMotion_Launch(19, 6, 0);
  ObjectMotion_Launch(20, 6, 0);
  ObjectMotion_Launch(21, 6, 0);
  ObjectMotion_Launch(22, 6, 0);
  ObjectMotion_Launch(23, 6, 0);
  ObjectMotion_Launch(24, 6, 0);
  ObjectMotion_Launch(25, 6, 40);
  Kibombo_ObjTurn(20, 0x5000);
  ObjectMotion_Launch(20, 2, 0);
  Kibombo_MessagePut(0x4000 + 20, 0);
  Kibombo_ObjTurn(23, 0x3000);
  ObjectMotion_Launch(23, 2, 0);
  Kibombo_MessagePut(0x4000 + 23, 0);
  ObjectMotion_EnableActionAndSetCallback(18, gData_020061a8);
  ObjectMotion_EnableActionAndSetCallback(19, gData_020061a8);
  ObjectMotion_EnableActionAndSetCallback(20, gData_020061a8);
  ObjectMotion_EnableActionAndSetCallback(21, gData_020061a8);
  ObjectMotion_EnableActionAndSetCallback(22, gData_020061a8);
  ObjectMotion_EnableActionAndSetCallback(23, gData_020061a8);
  ObjectMotion_EnableActionAndSetCallback(24, gData_020061a8);
  ObjectMotion_EnableActionAndSetCallback(25, gData_020061a8);
  Kibombo_Wait(40);
  Kibombo_ObjTurn(17, 0x3000);
  ObjectMotion_Launch(17, 2, 10);
  ObjectMotion_Launch(17, 4, 10);
  Kibombo_MessagePut(17, 0);
  ObjectMotion_EnableActionAndResetMotion(18);
  ObjectMotion_EnableActionAndResetMotion(19);
  ObjectMotion_EnableActionAndResetMotion(20);
  ObjectMotion_EnableActionAndResetMotion(21);
  ObjectMotion_EnableActionAndResetMotion(22);
  ObjectMotion_EnableActionAndResetMotion(23);
  ObjectMotion_EnableActionAndResetMotion(24);
  ObjectMotion_EnableActionAndResetMotion(25);
  Kibombo_Wait(40);
  Kibombo_ObjArm(18, 0xd000, 0);
  Kibombo_ObjArm(19, 0xd000, 0);
  Kibombo_ObjArm(20, 0xd000, 0);
  Kibombo_ObjArm(21, 0xd000, 0);
  Kibombo_ObjArm(22, 0xb000, 0);
  Kibombo_ObjArm(23, 0xb000, 0);
  Kibombo_ObjArm(24, 0xb000, 0);
  Kibombo_ObjArm(25, 0xb000, 0);
  Func_02005b94(17, 0x107, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Kibombo_MessagePut(0x8000 + 17, 0);
  Motion_SetModeAndWaitAnimation(17, 3);
  Kibombo_MessagePut(0x8000 + 17, 0);
  ObjectMotion_Launch(17, 4, 10);
  Kibombo_MessagePut(0x8000 + 17, 0);
}

void Func_020047e4(void) {
  /* FAKEMATCH(pin): the slot in r5, outside global CSE, so the loop loads its
   * address each pass, as the native does; as a plain local, 12 of 71
   * instructions differ in tla-en. */
  register struct KibomboObject **slot asm("r5");
  struct KibomboObject *object;

  Func_02004524();
  Func_02005c9c(161);
  Func_02003640(0, 1);
  FieldEvent_WaitFrames(20);
  Func_02005b4c((s32)MsgYouAreStillNot);
  Func_02005b64(17, 0);
  object = gData_02007950;
  if (object != 0) {
    // Lowers the object 0x4000 a frame down to y 0x240000.
    object->motionFlags = 0;
    {
      /* FAKEMATCH(pin): 0x240000 built as 144 << 14 in r1, held by the
       * barrier below; as a plain local behind the barrier, 10 of 71
       * instructions differ in tla-en. */
      register s32 limit asm("r1") = 144;

      /* FAKEMATCH(barrier): the limit is started before y is loaded and
       * finished after it, as the native does; as object->y > 0x240000, 2 of
       * 71 instructions differ in tla-en. */
      __asm__("" : "+l"(limit), "+l"(object));
      if (object->y > limit << 14) {
        do {
          Kibombo_ObjArm(17, 0xd000, 0);
          slot = &gData_02007950;
          object = *slot;
          object->y += 0xffffc000;
          WaitFrames(1);
        } while ((*slot)->y > 0x240000);
      }
    }
  }
  Motion_SetModeAndWaitAnimation(17, 4);
  Func_02005b64(17, 0);
  Kibombo_ObjTurn(17, 0x3000);
  Func_02005b64(0x4011, 0);
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Func_02005bc4(12);
}

#include "types.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02004464();
s32 Func_020045ec();
s32 Func_0200456c();
s32 Camera_MoveWithinMapBounds();
s32 Func_0200457c();
s32 FieldEvent_Wait();
s32 Motion_SetVarCbAndRefresh();
s32 Func_0200454c();
s32 Func_0200451c();
s32 Func_02004534();
s32 Func_02004564();
s32 Actor_SetModeById();
s32 Actor_ArmCallback();
s32 Func_0200455c();
s32 Motion_SetModeAndWaitAnimation();
s32 Actor_Jump();
s32 GameFlag_SetBit();
s32 Func_0200446c();
extern u8 MsgProx62[];

void Func_02000ad8(void) {
  Func_02004464();
  Call1(Func_020045ec, 0);
  Call2(Func_0200456c, 0x4cccc, 0x9999);
  Call4(Camera_MoveWithinMapBounds, PIXELS(160), -1, PIXELS(160), 1);
  Func_0200457c();
  Call1(FieldEvent_Wait, 40);
  Call2(Motion_SetVarCbAndRefresh, 0xa, 1);
  Call2(Func_0200454c, 10, 0xb000);
  Call1(Func_0200451c, (s32)MsgProx62);
  Call2(Func_02004534, 10, 0);
  Call2(Func_02004564, 0xc, 0x102);
  Call2(Func_02004564, 0xd, 0x102);
  Call2(Func_02004564, 0xe, 0x102);
  Call2(Func_02004564, 0xf, 0x102);
  Call2(Func_02004564, 0x10, 0x102);
  Call2(Func_02004564, 0x11, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_0200454c, 10, 0x5000);
  Call2(Func_02004534, 10, 0);
  Call2(Func_02004534, 10, 0);
  Call2(Func_02004564, 0xc, 0x102);
  Call2(Func_02004564, 0xd, 0x102);
  Call2(Func_02004564, 0xe, 0x102);
  Call2(Func_02004564, 0xf, 0x102);
  Call2(Func_02004564, 0x10, 0x102);
  Call2(Func_02004564, 0x11, 0x102);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_SetModeById, 12, 4);
  Call2(Func_02004534, 12, 0);
  Call3(Actor_ArmCallback, 10, 0x3000, 0);
  {
    s32 anim = 0x80;
    s32 zero = 0;
    /* FAKEMATCH: forced temporaries load the zero before 0x100's shift and the
     * actor id after it; as plain constants, 2 differ in tla-en. */
    __asm__ volatile("" : "+l"(anim));
    /* FAKEMATCH: the second forced temporary of the same call, as above. */
    __asm__ volatile("" : "+l"(zero));
    Call3(Func_0200455c, 10, anim << 1, zero);
  }
  Call2(Motion_SetModeAndWaitAnimation, 13, 4);
  Call2(Func_02004534, 13, 0);
  Call3(Actor_Jump, 15, 4, 10);
  Call2(Func_02004534, 15, 0);
  Call3(Actor_ArmCallback, 10, 0x5000, 0);
  Func_0200455c(10, 0x105, 40);
  Call2(Func_02004534, 10, 0);
  Func_0200455c(16, 0x107, 20);
  Call2(Func_02004534, 16, 0);
  Call3(Actor_Jump, 14, 2, 0);
  Call2(Actor_SetModeById, 14, 3);
  Call2(Func_02004534, 14, 0);
  Call2(Func_02004534, 11, 0);
  Call2(Func_0200454c, 10, FACING_WEST);
  Func_0200455c(10, 0x108, 40);
  Call2(Func_0200454c, 10, 0x5000);
  Call2(Func_02004534, 10, 0);
  Call2(Func_0200454c, 11, 0);
  Call2(Func_02004534, 11, 0);
  Func_0200455c(10, 0x100, 20);
  Call2(Func_0200454c, 10, FACING_WEST);
  Call2(Actor_SetModeById, 11, 4);
  Call2(Func_02004534, 11, 0);
  Call2(Func_02004564, 0xa, 0x102);
  Call3(Actor_ArmCallback, 12, FACING_WEST, 0);
  Call3(Actor_ArmCallback, 13, 0, 0);
  Call3(Actor_ArmCallback, 14, FACING_WEST, 0);
  Call3(Actor_ArmCallback, 15, 0, 0);
  Call3(Actor_ArmCallback, 16, FACING_WEST, 0);
  Call3(Actor_ArmCallback, 17, 0, 40);
  Call3(Actor_ArmCallback, 12, 0xb000, 0);
  Call3(Actor_ArmCallback, 13, 0xb000, 0);
  Call3(Actor_ArmCallback, 14, 0xd000, 0);
  Call3(Actor_ArmCallback, 15, 0xd000, 0);
  Call3(Actor_ArmCallback, 16, 0xd000, 0);
  Call3(Actor_ArmCallback, 17, 0xd000, 20);
  Call2(Func_02004534, 10, 0);
  Func_0200455c(11, 0x109, 40);
  Call2(Func_02004534, 11, 0);
  Call3(Actor_ArmCallback, 10, 0xb000, 80);
  Call2(Motion_SetModeAndWaitAnimation, 10, 4);
  Call2(Func_02004534, 10, 0);
  Call1(GameFlag_SetBit, 0x9c1);
  Func_0200446c();
}

#include "types.h"
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
  Call4(Camera_MoveWithinMapBounds, 0xa00000, -1, 0xa00000, 1);
  Func_0200457c();
  Call1(FieldEvent_Wait, 0x28);
  Call2(Motion_SetVarCbAndRefresh, 0xa, 1);
  Call2(Func_0200454c, 0xa, 0xb000);
  Call1(Func_0200451c, (s32)MsgProx62);
  Call2(Func_02004534, 0xa, 0);
  Call2(Func_02004564, 0xc, 0x102);
  Call2(Func_02004564, 0xd, 0x102);
  Call2(Func_02004564, 0xe, 0x102);
  Call2(Func_02004564, 0xf, 0x102);
  Call2(Func_02004564, 0x10, 0x102);
  Call2(Func_02004564, 0x11, 0x102);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Func_0200454c, 0xa, 0x5000);
  Call2(Func_02004534, 0xa, 0);
  Call2(Func_02004534, 0xa, 0);
  Call2(Func_02004564, 0xc, 0x102);
  Call2(Func_02004564, 0xd, 0x102);
  Call2(Func_02004564, 0xe, 0x102);
  Call2(Func_02004564, 0xf, 0x102);
  Call2(Func_02004564, 0x10, 0x102);
  Call2(Func_02004564, 0x11, 0x102);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Actor_SetModeById, 0xc, 4);
  Call2(Func_02004534, 0xc, 0);
  Call3(Actor_ArmCallback, 0xa, 0x3000, 0);
  {
    s32 anim = 0x80;
    s32 zero = 0;
    /* FAKEMATCH: forced temporaries load the zero before 0x100's shift and the
     * actor id after it; as plain constants, 2 differ in tla-en. */
    __asm__ volatile("" : "+l"(anim));
    /* FAKEMATCH: the second forced temporary of the same call, as above. */
    __asm__ volatile("" : "+l"(zero));
    Call3(Func_0200455c, 0xa, anim << 1, zero);
  }
  Call2(Motion_SetModeAndWaitAnimation, 0xd, 4);
  Call2(Func_02004534, 0xd, 0);
  Call3(Actor_Jump, 0xf, 4, 0xa);
  Call2(Func_02004534, 0xf, 0);
  Call3(Actor_ArmCallback, 0xa, 0x5000, 0);
  Func_0200455c(0xa, 0x105, 0x28);
  Call2(Func_02004534, 0xa, 0);
  Func_0200455c(0x10, 0x107, 0x14);
  Call2(Func_02004534, 0x10, 0);
  Call3(Actor_Jump, 0xe, 2, 0);
  Call2(Actor_SetModeById, 0xe, 3);
  Call2(Func_02004534, 0xe, 0);
  Call2(Func_02004534, 0xb, 0);
  Call2(Func_0200454c, 0xa, 0x8000);
  Func_0200455c(0xa, 0x108, 0x28);
  Call2(Func_0200454c, 0xa, 0x5000);
  Call2(Func_02004534, 0xa, 0);
  Call2(Func_0200454c, 0xb, 0);
  Call2(Func_02004534, 0xb, 0);
  Func_0200455c(0xa, 0x100, 0x14);
  Call2(Func_0200454c, 0xa, 0x8000);
  Call2(Actor_SetModeById, 0xb, 4);
  Call2(Func_02004534, 0xb, 0);
  Call2(Func_02004564, 0xa, 0x102);
  Call3(Actor_ArmCallback, 0xc, 0x8000, 0);
  Call3(Actor_ArmCallback, 0xd, 0, 0);
  Call3(Actor_ArmCallback, 0xe, 0x8000, 0);
  Call3(Actor_ArmCallback, 0xf, 0, 0);
  Call3(Actor_ArmCallback, 0x10, 0x8000, 0);
  Call3(Actor_ArmCallback, 0x11, 0, 0x28);
  Call3(Actor_ArmCallback, 0xc, 0xb000, 0);
  Call3(Actor_ArmCallback, 0xd, 0xb000, 0);
  Call3(Actor_ArmCallback, 0xe, 0xd000, 0);
  Call3(Actor_ArmCallback, 0xf, 0xd000, 0);
  Call3(Actor_ArmCallback, 0x10, 0xd000, 0);
  Call3(Actor_ArmCallback, 0x11, 0xd000, 0x14);
  Call2(Func_02004534, 0xa, 0);
  Func_0200455c(0xb, 0x109, 0x28);
  Call2(Func_02004534, 0xb, 0);
  Call3(Actor_ArmCallback, 0xa, 0xb000, 0x50);
  Call2(Motion_SetModeAndWaitAnimation, 0xa, 4);
  Call2(Func_02004534, 0xa, 0);
  Call1(GameFlag_SetBit, 0x9c1);
  Func_0200446c();
}

#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 Actor_SetModeById();
s32 Actor_SetSpritePriority();
s32 GameFlag_Test();
s32 Func_0200290c();
s32 Func_020028b4();
s32 Func_020029cc();
s32 Func_0200030c();
s32 Func_020028bc();
s32 Func_020028a4();
s32 Func_02001c30();
s32 GameFlag_SetBit();
s32 Party_RemoveActiveOwner();
s32 Func_0200299c();
s32 Func_02002a04();
s32 Actor_SetActionCallback();
extern u8 gMainRoutineWork[];
extern char SceneId_VenusLighthouseEntrance;

s32 Func_02000128(void) {
  s32 t37;
  s32 t0;
  s32 t7;
  s32 t10;
  s32 t21;
  s32 t33;

  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x100;
  t0 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  FIELD_AT((t0 + 0x23), u8, 0) = (0x20 | FIELD_AT((t0 + 0x23), u8, 0));
  Call2(Actor_SetModeById, 10, 5);
  Call2(Actor_SetSpritePriority, 10, 1);
  Call2(Actor_SetModeById, 11, 5);
  Call2(Actor_SetSpritePriority, 11, 1);
  Call2(Actor_SetSpritePriority, 14, 2);
  Call2(Actor_SetSpritePriority, 19, 2);
  t7 = Value1(GameFlag_Test, 0x892);
  if (t7 != 0) {
    Call3(Func_0200290c, 10, 0, 0);
    Call3(Func_0200290c, 11, 0, 0);
  }
  t10 = Value1(GameFlag_Test, 0x890);
  if (t10 != 0) {
    Call3(Func_0200290c, 12, 0, 0);
    Call3(Func_0200290c, 13, 0, 0);
    Call3(Func_0200290c, 14, 0, 0);
    Call3(Func_0200290c, 15, 0, 0);
    Call3(Func_0200290c, 16, 0, 0);
    Call3(Func_0200290c, 17, 0, 0);
    Call3(Func_0200290c, 18, 0, 0);
    Call3(Func_0200290c, 19, 0, 0);
    Call3(Func_0200290c, 20, 0, 0);
    Call3(Func_0200290c, 21, 0, 0);
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 9) {
    t21 = Value1(GameFlag_Test, 0x891);
    if (t21 == 0) {
      Func_020028b4();
      Call1(Func_020029cc, 0);
      Func_0200030c();
      Func_020028bc();
    }
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x62) {
    FIELD_AT(gMainRoutineWork, u16, 490) = 1;
    *(u16 *)(gMainRoutineWork + 482) = 1;
    Call1(Func_020028a4, 1);
    Func_02001c30();
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x63) {
    Call1(GameFlag_SetBit, 0x893);
  }
  if (FIELD_AT(gMainRoutineWork, s16, 482) == 0x5a) {
    FIELD_AT(gMainRoutineWork, s32, 532) = 5;
    Call1(Party_RemoveActiveOwner, 4);
    Call1(Party_RemoveActiveOwner, 6);
    Call1(Party_RemoveActiveOwner, 7);
    Call2(Func_0200299c, (s32)&SceneId_VenusLighthouseEntrance, 1);
  }
  t33 = Value1(GameFlag_Test, 0x893);
  if (t33 == 0) {
    Call2(Actor_SetModeById, 22, 5);
    Call2(Actor_SetSpritePriority, 22, 1);
  } else {
    Call3(Func_0200290c, 22, 40 << 16, 248 << 16);
    t37 = Value1(Actor_GetById, 22);
    FIELD_AT(t37, u16, 6) = 0x4000;
    Call3(Func_02002a04, 0x16, 0x4000, 0);
    Call2(Actor_SetSpritePriority, 22, 3);
    Call2(Actor_SetActionCallback, Value1(Actor_GetById, 22), 8);
  }
  return 0;
}

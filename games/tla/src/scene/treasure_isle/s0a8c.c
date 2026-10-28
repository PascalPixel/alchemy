#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02000fcc();
s32 Func_0200104c();
s32 Actor_StartRepeatedMotion();
s32 FieldEvent_Wait();
s32 Func_0200103c();
s32 Func_02001034();
s32 Func_02001044();
s32 Func_02000fbc();
s32 Actor_GetById();
s32 Actor_ResetAndSetPosition();
s32 Func_0200101c();
extern u8 MsgIAmTheGuardian[];
extern u8 gAdventure[];

void Func_02000a8c(void) {
  s32 t12;

  Func_02000fcc();
  Call1(Func_0200104c, 0);
  Call2(Actor_StartRepeatedMotion, 8, 2);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Func_0200103c, 0x10000, 0);
  Call2(Func_02001034, 0x10003, 1);
  Call1(Func_02001044, 0x3c);
  Call1(FieldEvent_Wait, 0x3c);
  Call3(Func_02000fbc, (s32)MsgIAmTheGuardian, 0, 6);
  Call2(Func_02001034, 0x10000, 0);
  Call1(Func_02001044, 0x3c);
  Call1(FieldEvent_Wait, 0x3c);
  t12 = Value1(Actor_GetById, FIELD_AT(gAdventure, s32, 532));
  if (t12 != 0) {
    Call3(Actor_ResetAndSetPosition, 8, FIELD_AT(t12, s16, 10),
          FIELD_AT(t12, s16, 18));
  }
  Call1(FieldEvent_Wait, 0x14);
  FIELD_AT(gAdventure, u16, 596) = 0x19bb;
  FIELD_AT(gAdventure, u8, 587) = 2;
  Call2(Func_0200101c, 0x67, 0);
}

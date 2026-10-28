#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_Test();
s32 GameFlag_SetBit();
s32 Func_02000070();
s32 Func_02001380();
s32 Func_020013f8();
s32 Actor_SetPositionAndReset();
s32 Actor_ArmCallback();
s32 Func_020001e4();
s32 Func_02001388();
extern u8 gSequenceParentWork[];

void Func_020000d0(void) {
  s32 t0;
  s32 actor = FIELD_AT(gSequenceParentWork, s32, 532);

  t0 = Value1(GameFlag_Test, 0x200);
  if (t0 == 0) {
    Call1(GameFlag_SetBit, 0x200);
    Func_02000070();
  }
  Func_02001380();
  Call1(Func_020013f8, 0);
  Call3(Actor_SetPositionAndReset, actor, 0x78, 0x98);
  Call3(Actor_ArmCallback, actor, 0x4000, 0);
  Func_020001e4();
  Func_02001388();
}

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
s32 Func_02005814();
s32 Func_02005984();
s32 Func_020058c4();
s32 Func_020058fc();
s32 Func_020058dc();
s32 Actor_SetModeById();
s32 FieldEvent_Wait();
s32 Func_0200581c();
extern u8 MsgYouReLate[];

void Func_0200045c(void) {
  s32 t0;
  s32 t2;

  t0 = Value1(GameFlag_Test, 0xa23);
  if (t0 != 0) {
    Call1(GameFlag_SetBit, 0x200);
    t2 = Value1(GameFlag_Test, 0xa21);
    if (t2 == 0) {
      Func_02005814();
      Call1(Func_02005984, 0);
      Call1(Func_020058c4, (s32)MsgYouReLate);
      Func_020058fc(0x1c, 0x103, 0x1e);
      Call2(Func_020058dc, 0x1c, 0);
      Call2(Actor_SetModeById, 27, 3);
      /* FAKEMATCH: the one-pass block ends the scheduling block after the mode
       * call: ⚓️ sets its id (r0) before its mode (r1); without it the mode is
       * set first (differ 2). */
      do {
      } while (0);
      Call1(FieldEvent_Wait, 20);
      Call2(Func_020058dc, 0x1b, 0);
      Func_0200581c();
    }
  }
}

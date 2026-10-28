#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02003398();
s32 Func_02003488();
s32 Actor_SetPositionAndReset();
s32 FieldEvent_Wait();
s32 Func_02003328();
s32 Func_020033e0();
s32 Func_020034b8();
s32 GameFlag_SetBit();
s32 Func_020033a0();
extern u8 gMainRoutineWork[];

void Func_02001660(void) {
  Func_02003398();
  Call1(Func_02003488, 0);
  Call3(Actor_SetPositionAndReset, FIELD_AT(gMainRoutineWork, s32, 532), 536,
        360);
  Call1(FieldEvent_Wait, 8);
  Call6(Func_02003328, 50, 22, 33, 22, 1, 1);
  Call3(Func_020033e0, 14, 0, 0);
  Call1(Func_020034b8, 0x248);
  Call1(FieldEvent_Wait, 30);
  Call1(Func_020034b8, 0xbc);
  Call6(Func_02003328, 50, 20, 31, 20, 1, 2);
  Call1(FieldEvent_Wait, 5);
  Call6(Func_02003328, 51, 20, 31, 20, 1, 2);
  Call1(FieldEvent_Wait, 5);
  Call1(GameFlag_SetBit, 0x200);
  Func_020033a0();
}

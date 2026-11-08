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
s32 Func_02002adc();
s32 Func_02002af4();
extern u8 MsgChampa35[];
extern u8 MsgChampa36[];

void Func_02000554(s32 a0) {
  s32 t0;

  t0 = Value1(GameFlag_Test, 0x972);
  if (t0 == 0) {
    GameFlag_SetBit(0x972);
    Func_02002adc((s32)MsgChampa35);
  } else {
    Func_02002adc((s32)MsgChampa36);
  }
  Func_02002af4(a0, 0);
}

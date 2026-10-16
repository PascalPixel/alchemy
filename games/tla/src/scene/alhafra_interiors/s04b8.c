#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();
s32 Func_02000b24();
s32 Func_02000b3c();
s32 FieldEvent_WaitFrames();
s32 Func_02000b5c();
extern u8 MsgAlhafraInteriors55[];

void Func_020004b8(s32 a0) {
  Call1(GameFlag_SetBit, 0x8b8);
  Call1(Func_02000b24, (s32)MsgAlhafraInteriors55);
  Call2(Func_02000b3c, a0, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Call3(Func_02000b5c, a0, 0x108, 0x1e);
  Call2(Func_02000b3c, a0, 0);
}

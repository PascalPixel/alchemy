#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Object_LinkPair();
s32 Func_02000b24();
s32 Func_02000b3c();
s32 ObjectMotion_ArmCallback();
extern u8 gAdventure[];
extern u8 MsgAlhafraInteriors48[];

void Func_0200047c(s32 a0, s32 a1) {
  Call3(Object_LinkPair, a1, FIELD_AT(gAdventure, s32, 532), 0);
  Call1(Func_02000b24, (s32)MsgAlhafraInteriors48);
  Call2(Func_02000b3c, a1, 0);
  Call3(ObjectMotion_ArmCallback, a1, 0xe000, 0);
}

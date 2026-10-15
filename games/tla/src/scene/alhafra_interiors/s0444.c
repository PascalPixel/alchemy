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
extern u8 gPartyState[];
extern u8 MsgAlhafraInteriors08[];

void Func_02000444(s32 a0) {
  Call3(Object_LinkPair, a0, FIELD_AT(gPartyState, s32, 532), 0);
  Call1(Func_02000b24, (s32)MsgAlhafraInteriors08);
  Call2(Func_02000b3c, a0, 0);
  Call3(ObjectMotion_ArmCallback, 0xe, 0x3000, 0);
}

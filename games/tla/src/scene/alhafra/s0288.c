#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02001c74();
s32 Func_02001c84();
s32 Actor_ArmCallback();
s32 FieldEvent_WaitFrames();
s32 Func_02001ca4();
s32 Actor_StartRepeatedMotion();
s32 Func_02001cac();
extern u8 MsgAlhafra50[];

void Func_02000288(void) {
  Call1(Func_02001c74, (s32)MsgAlhafra50);
  Call3(Func_02001c84, 0x18, 0, 0xa);
  Call3(Actor_ArmCallback, 0x13, -256, 0);
  Call1(FieldEvent_WaitFrames, 0xa);
  Func_02001ca4(0x13, 0x107, 0x1e);
  Call3(Func_02001c84, 0x13, 0, 0xa);
  Call2(Actor_StartRepeatedMotion, 0x18, 3);
  Call2(Func_02001cac, 0x18, 0x102);
  Call1(FieldEvent_WaitFrames, 0x28);
  Call3(Actor_ArmCallback, 0x13, 0x8000, 0);
  Call3(Func_02001c84, 0x18, 0, 0);
}

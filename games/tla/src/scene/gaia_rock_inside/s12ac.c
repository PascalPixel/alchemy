#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02004e5c();
s32 Func_02004fcc();
s32 PartyInventory_FindOwner();
s32 Func_02004f0c();
s32 Func_02004f14();
s32 Object_GetById();
s32 Object_RefreshSelectorById();
s32 Func_02000f00();
s32 Func_02000fac();
s32 Func_02004e64();
extern u8 MsgGaiaRockInside07[];
extern u8 gData_02005610[];
extern u8 MsgGaiaRockInside04[];

void Func_020012ac(void) {
  s32 t5;
  s32 t2;

  Func_02004e5c();
  Call1(Func_02004fcc, 0);
  t2 = Value1(PartyInventory_FindOwner, 0x1c3);
  if (t2 != -1) {
    Call1(Func_02004f0c, (s32)MsgGaiaRockInside07);
    Call2(Func_02004f14, 0xa, 0);
  } else {
    t5 = Value1(Object_GetById, 9);
    if (FIELD_AT(t5, s32, 0) == (s32)gData_02005610) {
      Call1(Object_RefreshSelectorById, 9);
    }
    Func_02000f00();
    Call1(Func_02004f0c, (s32)MsgGaiaRockInside04);
    Call2(Func_02004f14, 0xa, 0);
    Func_02000fac();
  }
  Func_02004e64();
}

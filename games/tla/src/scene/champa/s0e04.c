#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 ObjectTable_ReadActiveValue();
s32 UiWork_FinalizeEntityMatchingLocalizedId();

void Func_02000e04(s32 a0, s32 a1) {
  s32 t0;
  s32 t1;

  t0 = ObjectTable_ReadActiveValue();
  t1 = Value1(ObjectTable_ReadActiveValue, a1);
  Call1(UiWork_FinalizeEntityMatchingLocalizedId, t0);
  Call1(UiWork_FinalizeEntityMatchingLocalizedId, t1);
}

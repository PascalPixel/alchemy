#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
void Func_020009b0(void);
void Func_02000a50(s32 a);
s32 GameFlag_Test(s32 a);
void Func_02000a18(s32 a);
void Func_02000a28(s32 a, s32 b);
void Func_020009b8(void);
extern u8 MsgYallamInteriors38[];
extern u8 MsgYallamInteriors34[];

s32 Func_020000cc(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 21 instructions
   * differ in tla-en. */
  s32 t2;

  Func_020009b0();
  Func_02000a50(0);
  t2 = GameFlag_Test(SPLIT_CONSTANT(0x301));
  if (t2 != 0) {
    Func_02000a18((s32)MsgYallamInteriors38);
  } else {
    Func_02000a18((s32)MsgYallamInteriors34);
  }
  Func_02000a28(15, 0);
  Func_020009b8();
  return 0;
}

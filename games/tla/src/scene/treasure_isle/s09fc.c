#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Actor_GetById();
s32 GameFlag_SetBit();

void Func_020009fc(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 21 instructions
   * differ in tla-en. */
  s32 t0;

  t0 = Actor_GetById(14);
  FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9) =
      ((-13 & FIELD_AT(FIELD_AT(t0, s32, 80), s8, 9)) | 4);
  FIELD_AT((t0 + 0x23), u8, 0) = (0xfe & FIELD_AT((t0 + 0x23), u8, 0));
  GameFlag_SetBit(SPLIT_CONSTANT(0x205));
}

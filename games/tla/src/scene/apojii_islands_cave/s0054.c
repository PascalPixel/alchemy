#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_SetBit();

void Func_02000054(s32 a0, s32 a1) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 8 instructions differ
   * in tla-en. */
  GameFlag_SetBit((a1 + SPLIT_CONSTANT(0x8e1)));
}

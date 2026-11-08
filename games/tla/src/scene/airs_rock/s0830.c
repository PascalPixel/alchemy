#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"

void Func_02000830(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 4 of 8 instructions differ
   * in tla-en. */
  FIELD_AT((FIELD_AT(SPLIT_CONSTANT(0x30000e0), s32, 0) + 0x34), u8, 0) = 1;
}

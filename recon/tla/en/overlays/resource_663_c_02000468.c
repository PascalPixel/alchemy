#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"

void Func_02000468(s32 a0) {
  if (FIELD_AT((0x63 + a0), u8, 0) != 0) {
    if (FIELD_AT(0x3000000, s32, 32) != 0) {
      FIELD_AT((FIELD_AT((FIELD_AT(0x3000000, s32, 32) + 0x1a8), s32, 0) +
                (((FIELD_AT(a0, s32, 8) >> 20) +
                  ((((FIELD_AT(a0, s32, 16) - FIELD_AT(a0, s32, 12)) >> 20) + 1)
                   << 7))
                 << 2)),
               u8, 2) = 0;
      FIELD_AT((FIELD_AT((FIELD_AT(0x3000000, s32, 32) + 0x1a8), s32, 0) +
                (((FIELD_AT(a0, s32, 8) >> 20) +
                  ((((FIELD_AT(a0, s32, 16) - FIELD_AT(a0, s32, 12)) >> 20) - 1)
                   << 7))
                 << 2)),
               u8, 2) = 0;
      FIELD_AT((0x63 + a0), u8, 0) = 0;
    }
  }
}

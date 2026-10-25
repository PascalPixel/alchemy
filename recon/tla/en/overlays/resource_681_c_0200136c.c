// Measured against its copies: overlay 67C's Func_020013bc (differ 101 of 89).
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Vector_AddPolarOffsetFar();
s32 Func_02001314();

void Func_0200136c(s32 a0) {
  FIELD_AT((a0 + 0x64), u16, 0) =
      (FIELD_AT((a0 + 0x64), u16, 0) +
       ((FIELD_AT((0x66 + a0), u16, 0) << 16) >> 17));
  FIELD_AT(a0, s32, 8) = FIELD_AT(FIELD_AT(a0, s32, 104), s32, 8);
  FIELD_AT(a0, s32, 12) = (FIELD_AT(FIELD_AT(a0, s32, 104), s32, 12) + 0x80000);
  FIELD_AT(a0, s32, 16) = FIELD_AT(FIELD_AT(a0, s32, 104), s32, 16);
  Call2(Vector_AddPolarOffsetFar, FIELD_AT(a0, s32, 76),
        FIELD_AT((a0 + 0x64), u16, 0));
  FIELD_AT((a0 + 0x62), u8, 0) = (FIELD_AT((a0 + 0x62), u8, 0) + 0xff);
  if (((FIELD_AT((a0 + 0x62), u8, 0) + 0xff) << 24) != 0) {
    if (FIELD_AT(a0, s32, 76) != 0x20000) {
      Call1(Func_02001314, a0);
      FIELD_AT(a0, s32, 76) = (FIELD_AT(a0, s32, 76) + -262144);
      FIELD_AT((a0 + 0x64), u16, 0) =
          (FIELD_AT((a0 + 0x64), u16, 0) +
           ((FIELD_AT((0x66 + a0), u16, 0) << 16) >> 17));
      FIELD_AT((8 + a0), s32, 0) = FIELD_AT(FIELD_AT(a0, s32, 104), s32, 8);
      FIELD_AT(a0, s32, 12) =
          (FIELD_AT(FIELD_AT(a0, s32, 104), s32, 12) + 0x80000);
      FIELD_AT(a0, s32, 16) = FIELD_AT(FIELD_AT(a0, s32, 104), s32, 16);
      Call3(Vector_AddPolarOffsetFar, FIELD_AT(a0, s32, 76),
            FIELD_AT((a0 + 0x64), u16, 0), (8 + a0));
      Call1(Func_02001314, a0);
      FIELD_AT(a0, s32, 76) = (FIELD_AT(a0, s32, 76) + -262144);
    }
  }
}

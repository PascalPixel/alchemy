// Draft, differ 101 of 89: overlay 692's copy of the function drafted in
// resource_681_c_0200136c.c (overlay 681's Func_0200136c), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Vector_AddPolarOffsetFar();
s32 Func_02002a14();

void Func_02002a6c(s32 a0) {
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
      Call1(Func_02002a14, a0);
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
      Call1(Func_02002a14, a0);
      FIELD_AT(a0, s32, 76) = (FIELD_AT(a0, s32, 76) + -262144);
    }
  }
}

// Draft of overlay 6aa's Func_020012e4, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)



void Func_020012e4(s32 p0) {
  s32 v_r1, v_r2, v_r3, v_r4;

  v_r2 = FIELD_AT(p0, s32, 80);
  v_r3 = FIELD_AT(v_r2, u16, 18);
  v_r3 = v_r3 + 0x800;
  v_r1 = p0 + 100;
  FIELD_AT(v_r2, u16, 18) = v_r3;
  v_r3 = FIELD_AT(v_r1, u16, 0);
  v_r3 = v_r3 + 1;
  FIELD_AT(v_r1, u16, 0) = v_r3;
  v_r3 = v_r3 << 16;
  if (v_r3 != 0x80000)
    goto L_0200930e;
  FIELD_AT(p0, s32, 40) = 0x40000;
L_0200930e:
  ;
  v_r3 = FIELD_AT(v_r1, s16, 0);
  if (v_r3 <= 9)
    goto L_0200932e;
  v_r3 = FIELD_AT(p0, s32, 8);
  v_r3 = v_r3 + 0x40000;
  FIELD_AT(p0, s32, 8) = v_r3;
  v_r3 = FIELD_AT(p0, s32, 24);
  v_r3 = v_r3 + -0x800;
  FIELD_AT(p0, s32, 24) = v_r3;
  v_r3 = FIELD_AT(p0, s32, 28);
  v_r3 = v_r3 + -0x800;
  FIELD_AT(p0, s32, 28) = v_r3;
L_0200932e:
  ;
  v_r3 = FIELD_AT(v_r1, s16, 0);
  if (v_r3 != 32)
    goto L_0200933c;
  FIELD_AT(p0, s32, 8) = v_r4;
  FIELD_AT(p0, s32, 16) = v_r4;
  FIELD_AT(p0, s32, 108) = v_r4;
L_0200933c:
  ;
}

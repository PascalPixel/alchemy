// Draft, differ 25 of 33 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Draft of overlay 6a0's Func_020005a0, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_02002c64[];

void Func_020005a0(s32 p0) {
  s32 v_r0, v_r1, v_r2, v_r3, v_r4, v_r5;

  v_r5 = p0;
  v_r4 = (s32)gData_02002c64;
  v_r3 = FIELD_AT(v_r4, s16, 0);
  if (v_r3 == -1)
    goto L_020085de;
L_020085b8:
  ;
  v_r2 = FIELD_AT(v_r4 + v_r1, s16, 0);
  v_r3 = FIELD_AT(v_r5, s32, 8);
  v_r3 = v_r3 >> 20;
  if (v_r2 != v_r3)
    goto L_020085d0;
  v_r2 = FIELD_AT(v_r0 + v_r4, s16, 0);
  v_r3 = FIELD_AT(v_r5, s32, 16);
  v_r3 = v_r3 >> 20;
  if (v_r2 != v_r3)
    goto L_020085d0;
  goto L_020085e0;
L_020085d0:
  ;
  v_r1 = v_r1 + 4;
  v_r3 = FIELD_AT(v_r4 + v_r1, s16, 0);
  v_r0 = v_r0 + 4;
  if (v_r3 != -1)
    goto L_020085b8;
L_020085de:
  ;
L_020085e0:
  ;
}

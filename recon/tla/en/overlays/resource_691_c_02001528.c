// Draft of overlay 691's Func_02001528, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_02003a6c();


void Func_02001528(s32 p0) {
  s32 v_ip, v_r1, v_r2, v_r3, v_r4, v_r5, v_r6;

  v_r5 = FIELD_AT(p0, s32, 104);
  v_r4 = v_r5;
  v_r4 = v_r4 + 100;
  v_r2 = FIELD_AT(v_r4, s16, 0);
  if (v_r2 <= 35)
    goto L_0200955c;
  v_r3 = p0 + 98;
  v_r3 = FIELD_AT(v_r3, u8, 0);
  v_r3 = v_r3 << 1;
  v_r3 = v_r3 + 36;
  if (v_r2 >= v_r3)
    goto L_0200955c;
  v_r2 = FIELD_AT(v_r5, s32, 12);
  v_r2 = v_r2 + 0x200000;
  FIELD_AT(p0, s32, 12) = v_r2;
  v_r6 = 0;
  v_r3 = FIELD_AT(v_r4 + v_r6, s16, 0);
  v_r3 = v_r3 << 19;
  v_r2 = v_r2 + v_r3;
  goto L_020095be;
L_0200955c:
  ;
  v_r2 = 98 + p0;
  v_ip = v_r2;
  v_r2 = FIELD_AT(v_r2, u8, 0);
  v_r6 = 0;
  v_r1 = FIELD_AT(v_r4 + v_r6, s16, 0);
  v_r3 = v_r2 << 1;
  v_r3 = v_r3 + 36;
  if (v_r3 >= v_r1)
    goto L_02009588;
  if (v_r1 >= 0x14c)
    goto L_02009588;
  v_r3 = FIELD_AT(v_r5, s32, 12);
  v_r2 = v_r2 << 20;
  v_r6 = 128;
  v_r3 = v_r3 + v_r2;
  v_r6 = v_r6 << 14;
  v_r3 = v_r3 + v_r6;
  FIELD_AT(p0, s32, 12) = v_r3;
  goto L_020095de;
L_02009588:
  ;
  v_r1 = FIELD_AT(v_r4, s16, 0);
  if (v_r1 <= 0x14c)
    goto L_020095c4;
  v_r6 = v_ip;
  v_r2 = FIELD_AT(v_r6, u8, 0);
  v_r6 = 78;
  v_r3 = v_r2 << 1;
  v_r6 = v_r6 + 255;
  v_r3 = v_r3 + v_r6;
  if (v_r1 >= v_r3)
    goto L_020095c4;
  v_r3 = v_r2 << 20;
  v_r2 = FIELD_AT(v_r5, s32, 12);
  v_r2 = v_r2 + v_r3;
  v_r2 = v_r2 + 0x200000;
  FIELD_AT(p0, s32, 12) = v_r2;
  v_r6 = 0;
  v_r3 = FIELD_AT(v_r4 + v_r6, s16, 0);
  v_r3 = v_r3 << 19;
  v_r2 = v_r2 - v_r3;
L_020095be:
  ;
  v_r2 = v_r2 + v_r1;
  FIELD_AT(p0, s32, 12) = v_r2;
  goto L_020095de;
L_020095c4:
  ;
  v_r3 = p0 + 98;
  v_r3 = FIELD_AT(v_r3, u8, 0);
  v_r6 = 78;
  v_r3 = v_r3 << 1;
  v_r6 = v_r6 + 255;
  v_r2 = v_r3 + v_r6;
  v_r3 = FIELD_AT(v_r4, s16, 0);
  if (v_r2 >= v_r3)
    goto L_020095de;
  Call4((void (*)())Func_02003a6c, p0, 0, v_r2, v_r3);
L_020095de:
  ;
}

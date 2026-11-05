// Draft of overlay 69d's Func_02001c68, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Actor_GetById();
s32 GameFlag_GetByte();
s32 GameFlag_SetByte();
s32 Func_02001dd0();
s32 Func_02001a20();
extern u8 gMainRoutineWork[];
extern u8 gData_02002864[];

void Func_02001c68(s32 p0, s32 p1, s32 p2) {
  u8 frame[4];
  s32 s0, v_r0, v_r1, v_r10, v_r11, v_r2, v_r3, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  /* XXX stack argument */;
  v_r3 = (s32)gMainRoutineWork + 0x214;
  v_r9 = p0;
  v_r0 = FIELD_AT(v_r3, s32, 0);
  v_r11 = p1;
  v_r0 = Actor_GetById(v_r0);
  v_r5 = (s32)gData_02002864;
  v_r3 = FIELD_AT(0x3000000, s32, 108);
  v_r6 = v_r0;
  v_r0 = FIELD_AT(v_r5, s16, 2);
  v_r10 = v_r3;
  v_r0 = GameFlag_GetByte(v_r0);
  v_r7 = v_r0;
  v_r0 = FIELD_AT(v_r5, s16, 2);
  v_r0 = v_r0 + 8;
  v_r0 = GameFlag_GetByte(v_r0);
  v_r8 = v_r0;
  if (v_r7 != 0)
    goto L_02009cb6;
  if (v_r0 == 0)
    goto L_02009d0a;
L_02009cb6:
  ;
  v_r0 = FIELD_AT(v_r5, s16, 2);
  v_r0 = GameFlag_SetByte(v_r0, 0, 2);
  v_r0 = FIELD_AT(v_r5, s16, 2);
  v_r0 = v_r0 + 8;
  v_r0 = GameFlag_SetByte(v_r0, 0);
  v_r2 = v_r7 + v_r9;
  v_r3 = v_r8 + v_r11;
  v_r3 = v_r3 << 20;
  v_r3 = v_r3 + 0x80000;
  FIELD_AT(v_r6, s32, 16) = v_r3;
  v_r3 = 0x1cc + v_r10;
  v_r2 = v_r2 << 20;
  v_r2 = v_r2 + 0x80000;
  v_r1 = FIELD_AT(v_r3, s32, 0);
  FIELD_AT(v_r6, s32, 8) = v_r2;
  FIELD_AT(v_r1, s32, 8) = v_r2;
  v_r3 = FIELD_AT(v_r6, s32, 16);
  FIELD_AT(v_r1, s32, 16) = v_r3;
  Func_02001dd0();
  Func_02001a20();
  v_r3 = FIELD_AT(0x3000000, s32, 108);
  v_r3 = v_r3 + 0x1ac;
  v_r2 = s0;
  FIELD_AT(v_r3, s32, 0) = v_r2;
L_02009d0a:
  ;
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
}

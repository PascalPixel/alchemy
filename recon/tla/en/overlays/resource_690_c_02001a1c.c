// Draft of overlay 690's Func_02001a1c, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 ObjectTable_Get();
s32 Func_0200173c();
s32 Func_02002028();
s32 ArcTan2();
s32 Vector_AddPolarOffsetFar();
s32 Map_GetTerrainHeight();
s32 Func_02002008();
s32 Func_02001fe0();
s32 Actor_SetAnimationSpeed();
s32 Func_02002010();
s32 Func_02002050();
s32 wait();
s32 Func_02002048();
extern u8 gMainRoutineWork[];
extern u8 gData_02002ce8[];
extern u8 IwramMulQ16[];
extern u8 gInput[];
extern u8 gData_020022b6[];
extern u8 Func_020016b0[];

void Func_02001a1c(void) {
  u8 frame[-40];
  s32 s0, s12, s16, s20, s24, s4, s8, v_r0, v_r1, v_r10, v_r11, v_r2, v_r3, v_r5, v_r6, v_r7, v_r8, v_r9;

  v_r7 = v_r11;
  v_r6 = v_r10;
  v_r5 = v_r9;
  v_r7 = v_r8;
  v_r3 = (s32)gMainRoutineWork + 0x214;
  v_r0 = FIELD_AT(v_r3, s32, 0);
  v_r0 = ObjectTable_Get(v_r0);
  v_r7 = v_r0;
  FIELD_AT((s32)gData_02002ce8, u16, 0) = 0;
L_ja_020099da:
  ;
  Func_0200173c();
  FIELD_AT(v_r7, s32, 48) = 0x10000;
  FIELD_AT(v_r7, s32, 52) = 0x8000;
  FIELD_AT(v_r7, s32, 56) = -0x80000000;
  FIELD_AT(v_r7, s32, 64) = -0x80000000;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  v_r3 = FIELD_AT(v_r7, s32, 8);
  v_r2 = (s32)frame + 28;
  v_r10 = v_r2;
  v_r3 = v_r3 & -0x100000;
  v_r3 = v_r3 + 0x80000;
  /* XXX stack argument */;
  FIELD_AT(v_r10, s32, 0) = v_r3;
  goto L_ja_02009a20;
L_ja_02009a20:
  ;
  v_r3 = FIELD_AT(v_r7, s32, 12);
  FIELD_AT(v_r10, s32, 4) = v_r3;
  v_r3 = FIELD_AT(v_r7, s32, 16);
  v_r3 = v_r3 & v_r1;
  v_r3 = v_r3 + v_r2;
  /* XXX stack argument */;
  FIELD_AT(v_r10, s32, 8) = v_r3;
  v_r2 = s12;
  /* XXX stack argument */;
  v_r3 = v_r7 + 34;
  /* XXX stack argument */;
  /* XXX stack argument */;
  v_r0 = FIELD_AT(v_r3, u8, 0);
  v_r2 = s16;
  v_r0 = Func_02002028(v_r0, v_r2, v_r2, v_r3);
  /* XXX stack argument */;
  v_r2 = s20;
  v_r1 = FIELD_AT(v_r7, s32, 8);
  v_r0 = s16;
  v_r6 = FIELD_AT(v_r7, s32, 16);
  v_r1 = v_r2 - v_r1;
  v_r6 = v_r0 - v_r6;
  v_r8 = (s32)IwramMulQ16;
  v_r5 = v_r1;
  v_r5 = v_r5 + v_r6;
  if (v_r5 >= 0x40000)
    goto L_ja_02009a9c;
  v_r3 = FIELD_AT((s32)gInput, s32, 0);
  v_r3 = (u32)v_r3 >> 4;
  v_r3 = v_r3 & 15;
  v_r3 = v_r3 << 1;
  v_r2 = FIELD_AT((s32)gData_020022b6 + v_r3, s16, 0);
  v_r9 = v_r2;
  v_r3 = v_r2 << 16;
  if (v_r3 != -0x10000)
    goto L_ja_02009ac8;
  goto L_ja_02009c90;
L_ja_02009a9c:
  ;
  v_r3 = s16;
  v_r2 = s20;
  v_r0 = FIELD_AT(v_r7, s32, 16);
  v_r1 = FIELD_AT(v_r7, s32, 8);
  v_r0 = v_r3 - v_r0;
  v_r1 = v_r2 - v_r1;
  v_r0 = ArcTan2(v_r0, v_r1, v_r2, v_r3);
  v_r0 = v_r0 << 16;
  v_r0 = v_r0 >> 16;
  v_r9 = v_r0;
  v_r3 = 0x2000 + v_r9;
  v_r3 = v_r3 & -0x4000;
  v_r3 = v_r3 << 16;
  v_r3 = v_r3 >> 16;
  v_r9 = v_r3;
  goto L_ja_02009ac8;
L_ja_02009ac8:
  ;
  v_r2 = (s32)frame + 28;
  v_r0 = Vector_AddPolarOffsetFar(0x100000, v_r9, v_r2, v_r3);
  v_r3 = s4;
  v_r1 = FIELD_AT(v_r10, s32, 0);
  v_r0 = FIELD_AT(v_r3, u8, 0);
  v_r2 = FIELD_AT(v_r10, s32, 8);
  v_r0 = Func_02002028(v_r0, v_r1, v_r2, v_r3);
  v_r11 = v_r0;
  if (v_r0 == 255)
    goto L_ja_02009b4a;
  v_r3 = s4;
  v_r0 = FIELD_AT(v_r3, u8, 0);
  v_r1 = FIELD_AT(v_r10, s32, 0);
  v_r2 = FIELD_AT(v_r10, s32, 8);
  v_r0 = Map_GetTerrainHeight(v_r0, v_r1, v_r2, v_r3);
  v_r3 = FIELD_AT(v_r7, s32, 12);
  v_r0 = v_r0 - v_r3;
  if (v_r0 > 0x80000)
    goto L_ja_02009b4a;
  v_r0 = s12;
  FIELD_AT(v_r10, s32, 0) = v_r0;
  v_r3 = s8;
  FIELD_AT(v_r10, s32, 8) = v_r3;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  v_r2 = v_r7 + 100;
  FIELD_AT(v_r2, u16, 0) = 0;
  v_r1 = s12;
  v_r3 = s8;
  v_r2 = FIELD_AT(v_r7, s32, 12);
  v_r0 = Func_02002008(v_r7, v_r1, v_r2, v_r3);
  v_r0 = Func_02001fe0(v_r7, 2);
  v_r0 = Actor_SetAnimationSpeed(v_r7, 48);
  Call1((void (*)())Func_02002010, v_r7);
  FIELD_AT(v_r7, s32, 108) = (s32)Func_020016b0;
  goto L_ja_02009c26;
L_ja_02009b4a:
  ;
  FIELD_AT(v_r7, u16, 6) = v_r9;
  FIELD_AT(v_r7, s32, 36) = 0;
  FIELD_AT(v_r7, s32, 44) = 0;
  v_r2 = s12;
  FIELD_AT(v_r7, s32, 8) = v_r2;
  v_r3 = s8;
  FIELD_AT(v_r7, s32, 16) = v_r3;
  goto L_ja_02009c72;
L_ja_02009b5e:
  ;
  v_r2 = s4;
  v_r1 = FIELD_AT(v_r6, s32, 0);
  v_r0 = FIELD_AT(v_r2, u8, 0);
  v_r2 = FIELD_AT(v_r6, s32, 8);
  v_r0 = Map_GetTerrainHeight(v_r0, v_r1, v_r2, v_r3);
  v_r3 = FIELD_AT(v_r7, s32, 12);
  v_r0 = v_r0 - v_r3;
  if (v_r0 > 0x80000)
    goto L_ja_02009c46;
  v_r3 = FIELD_AT((s32)gData_02002ce8, u16, 0);
  v_r3 = v_r3 << 16;
  if (v_r3 != 0)
    goto L_ja_02009c46;
  v_r3 = FIELD_AT(v_r7, u16, 32);
  v_r3 = v_r3 - 2;
  /* XXX stack argument */;
  v_r5 = FIELD_AT(0x3000000, s32, 20);
  v_r2 = 89 + v_r5;
  v_r10 = 0;
  v_r8 = v_r2;
L_ja_02009b96:
  ;
  v_r3 = FIELD_AT(v_r5, s32, 0);
  if (v_r3 == 0)
    goto L_ja_02009bc0;
  v_r2 = FIELD_AT(v_r8, u8, 0);
  v_r3 = 1 & v_r2;
  if (v_r3 == 0)
    goto L_ja_02009bc0;
  if (v_r5 == v_r7)
    goto L_ja_02009bc0;
  v_r3 = FIELD_AT(v_r5, u16, 32);
  v_r0 = v_r5 + 8;
  v_r3 = v_r3 - 2;
  v_r1 = s0;
  v_r2 = (s32)frame + 28;
  v_r0 = Func_02002050(v_r0, v_r1, v_r2, v_r3);
  if (v_r0 >= 0)
    goto L_ja_02009c46;
L_ja_02009bc0:
  ;
  v_r10 = v_r10 + 1;
  v_r8 = v_r8 + 128;
  v_r5 = v_r5 + 128;
  if (v_r10 <= 63)
    goto L_ja_02009b96;
  v_r0 = FIELD_AT(v_r6, s32, 0);
  /* XXX stack argument */;
  v_r2 = FIELD_AT(v_r6, s32, 8);
  /* XXX stack argument */;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x1999;
  v_r5 = 0;
  v_r1 = FIELD_AT(v_r6, s32, 0);
  v_r2 = FIELD_AT(v_r6, s32, 4);
  v_r3 = FIELD_AT(v_r6, s32, 8);
  v_r0 = Func_02002008(v_r7, v_r1, v_r2, v_r3);
  goto L_ja_02009bfe;
L_ja_02009bf6:
  ;
  v_r0 = wait(1);
  v_r5 = v_r5 + 1;
L_ja_02009bfe:
  ;
  if (v_r5 > 179)
    goto L_ja_02009c16;
  v_r0 = Func_02002048(v_r7);
  if (v_r0 != 0)
    goto L_ja_02009c16;
  v_r3 = FIELD_AT((s32)gData_02002ce8, u16, 0);
  v_r3 = v_r3 << 16;
  if (v_r3 == 0)
    goto L_ja_02009bf6;
L_ja_02009c16:
  ;
  v_r3 = FIELD_AT((s32)gData_02002ce8, u16, 0);
  v_r3 = v_r3 << 16;
  if (v_r3 != 0)
    goto L_ja_02009c72;
  v_r3 = s24;
  if (v_r11 != v_r3)
    goto L_ja_02009c6c;
L_ja_02009c26:
  ;
  v_r2 = (s32)frame + 28;
  v_r0 = Vector_AddPolarOffsetFar(0x100000, v_r9, v_r2, v_r3);
  v_r2 = s4;
  v_r6 = (s32)frame + 28;
  v_r0 = FIELD_AT(v_r2, u8, 0);
  v_r1 = FIELD_AT(v_r6, s32, 0);
  v_r2 = FIELD_AT(v_r6, s32, 8);
  v_r0 = Func_02002028(v_r0, v_r1, v_r2);
  v_r11 = v_r0;
  if (v_r0 != 255)
    goto L_ja_02009b5e;
L_ja_02009c46:
  ;
  FIELD_AT(v_r7, s32, 48) = 0x20000;
  FIELD_AT(v_r7, s32, 52) = 0x10000;
  v_r2 = FIELD_AT(v_r7, s32, 12);
  v_r1 = s20;
  v_r3 = s16;
  v_r0 = Func_02002008(v_r7, v_r1, v_r2, v_r3);
  Call1((void (*)())Func_02002010, v_r7);
  v_r0 = wait(2);
  goto L_ja_020099da;
L_ja_02009c6c:
  ;
  v_r0 = wait(10);
L_ja_02009c72:
  ;
  FIELD_AT(v_r7, s32, 108) = 0;
  v_r1 = v_r7 + 90;
  v_r2 = FIELD_AT(v_r1, u8, 0);
  v_r3 = 1 | v_r2;
  FIELD_AT(v_r1, u8, 0) = v_r3;
  FIELD_AT(v_r7, s32, 52) = 0x4000;
  v_r0 = Func_02001fe0(v_r7, 1, v_r2, 0x4000);
L_ja_02009c90:
  ;
  v_r8 = v_r3;
  v_r9 = v_r5;
  v_r10 = v_r6;
  v_r11 = v_r7;
}

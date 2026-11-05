// Draft of overlay 668's Func_02004004, translated from its listing.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 HeapSlot_AllocFar();
s32 Resource_GetTableEntry();
s32 Func_020049d4();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
s32 Func_02004c64();
s32 Task_Add();
extern u8 Func_02003ec4[];

void Func_02004004(void) {
  u8 frame[4];
  s32 v_r0, v_r10, v_r2, v_r3, v_r5, v_r6, v_r7, v_r8;

  v_r7 = v_r10;
  v_r6 = v_r8;
  v_r0 = HeapSlot_AllocFar(220, 0x1608);
  v_r6 = v_r0;
  Call1((void (*)())Resource_GetTableEntry, 0x1f0);
  Func_020049d4();
  v_r0 = Resource_FindFreeEntry();
  v_r5 = v_r0;
  v_r0 = VramCache_Load();
  v_r10 = v_r0;
  v_r3 = v_r6 + 0x1602;
  FIELD_AT(v_r3, u16, 0) = v_r10;
  v_r3 = v_r6 + 0x1604;
  FIELD_AT(v_r3, u16, 0) = v_r5;
  v_r7 = v_r6 + 0x500;
  v_r5 = v_r6 + 0xc00;
  v_r8 = 63;
L_0200c05c:
  ;
  /* XXX stack argument */;
  Call4((void (*)())Func_02004c64, v_r5, 8, 8, 0x40000000);
  v_r3 = FIELD_AT(v_r5, u8, 5);
  v_r3 = v_r3 | 32;
  v_r2 = FIELD_AT(v_r5, u8, 9);
  FIELD_AT(v_r5, u8, 5) = v_r3;
  v_r3 = 15 & v_r2;
  v_r3 = v_r3 & -13;
  FIELD_AT(v_r5, u8, 9) = v_r3;
  FIELD_AT(v_r5, u16, 30) = 240;
  v_r8 = v_r8 + -1;
  FIELD_AT(v_r7, s32, 24) = -1;
  v_r5 = v_r5 + 40;
  v_r7 = v_r7 + 28;
  if (v_r8 >= 0)
    goto L_0200c05c;
  v_r2 = v_r6 + 0x1600;
  FIELD_AT(v_r2, u16, 0) = 0;
  Call4((void (*)())Task_Add, (s32)Func_02003ec4, 0x480, v_r2, 0);
  v_r8 = v_r3;
  v_r10 = v_r5;
}

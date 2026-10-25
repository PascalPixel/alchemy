// Draft of overlay 6b3's Func_02000098, translated from its listing: differ
// 84 of 69. Left: its shape: 85 instructions against the listing's 69.
#include "types.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Func_020007ac();
s32 Func_02000834();
s32 MemAlloc();
s32 MemFree();
s32 Resource_FindFreeEntry();
s32 Resource_GetTableEntry();
s32 Scheduler_AddOrUpdateCallback();
s32 VramCache_Load();
extern u8 Data_02000874[];
extern u8 Data_020008f0[];
extern u8 Data_02000a30[];
extern u8 Func_0200005c[];
extern u8 gData_02000a00[];
extern u8 gData_02000a34[];

void Func_02000098(s32 p0, s32 p1, s32 p2, s32 p3) {
  s32 t0, t1, t3, t4, t6, t7;

  t0 = Value4((s32 (*)())MemAlloc, 0x800, p1, p2, p3);
  *(s32 *)0x40000d4 = (s32)Data_02000874;
  *(s32 *)(0x40000d4 + 4) = 0x50003c0;
  *(s32 *)(0x40000d4 + 8) = 0x80000010;
  t1 = Value4((s32 (*)())Resource_GetTableEntry, 0x1f8, 0x50003c0, 0x80000010,
              0x40000d4);
  Call2((void (*)())Func_020007ac, t1, t0);
  t3 = Value0((s32 (*)())Resource_FindFreeEntry);
  *(s32 *)(s32)Data_020008f0 = t3;
  t4 = Value3((s32 (*)())VramCache_Load, t3, 0x800, t0);
  *(s32 *)(s32)Data_02000a30 = t4;
  Func_02000834((s32)gData_02000a00, 0, 0, 0xc0000000, (s32)gData_02000a00);
  FIELD_AT((s32)gData_02000a00, u16, 30) = 240;
  t6 = FIELD_AT((s32)gData_02000a00, u8, 9);
  t7 = FIELD_AT((s32)gData_02000a00, u8, 5);
  FIELD_AT((s32)gData_02000a00, u8, 5) = (-0x21 & t7);
  FIELD_AT((s32)gData_02000a00, u8, 9) = (((-13 & t6) & 15) | 224);
  Call4((void (*)())MemFree, t0, t7, (((-13 & t6) & 15) | 224), 224);
  *(u8 *)(s32)gData_02000a34 = 0;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_0200005c, 0x480);
  goto L_02008150;
L_02008150:
  return;
}

// Draft of overlay 6a7's Func_020006b0, translated from its listing: differ
// 282 of 323. Left: its shape: 337 instructions against the listing's 323.
#include "types.h"
#include "dma.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 FieldEvent_Wait();
s32 Func_020009f0();
s32 Func_02000a10();
s32 Func_02000a50();
s32 Func_02000b10();
s32 Func_02000b18();
s32 Func_02000b40();
s32 Func_02000b48();
s32 Func_02000b50();
s32 Func_02000b58();
s32 Heap_Alloc();
s32 Heap_Release();
s32 Random16Far();
s32 Resource_FindFreeEntry();
s32 Resource_GetTableEntry();
s32 Resource_ResetEntry();
s32 HeapSlot_AllocFar();
s32 Heap_AllocEwram();
s32 HeapSlot_Release();
s32 VramCache_Load();
s32 wait();
s32 __divsi3();
s32 __modsi3();
extern u8 Field_Palette685[];

void Func_020006b0(s32 p0, s32 p1, s32 p2) {
  s32 sp_[1];
  s32 spo_[1];
  s32 v_r8, v_r6, v_r10, v_r7, v_r9, v_r5, v_r11, v_r2, v_r3, t0;
  s32 t1, t3, t4, t5, t7, t8, t9, t10, t11, t12;
  s32 t13, t14, t15, t16, t17, t18, t20, t21, t23, t28;
  s32 t32, t33, t36, t37, t38, t39, t40, t41, t42, t43;
  s32 t44, t46, t47, t53;

  FIELD_AT(sp_, s32, 16) = p2;
  t0 = Value2((s32 (*)())HeapSlot_AllocFar, 220, 0x1e58);
  FIELD_AT(sp_, s32, 4) = 0;
  t1 = Value2((s32 (*)())Resource_GetTableEntry, 0x1f2, 0);
  Call2((void (*)())Func_020009f0, t1, t0);
  t3 = Value0((s32 (*)())Resource_FindFreeEntry);
  FIELD_AT(sp_, s32, 12) = t3;
  t4 = Value3((s32 (*)())VramCache_Load, t3, 0x500, t0);
  FIELD_AT(sp_, s32, 8) = t4;
  v_r9 = t0;
  v_r7 = (t0 + 0x1870);
  v_r10 = p1;
  v_r6 = (t0 + 0x1000);
  v_r8 = 0;
L_02008706:
  t5 = FIELD_AT(sp_, s32, 8);
  Func_02000b40(v_r6, 0, 0, 0x40000000, (((3 & v_r8) << 3) + t5));
  t7 = FIELD_AT(v_r6, u8, 9);
  FIELD_AT(v_r6, u16, 30) = 250;
  t8 = FIELD_AT(v_r6, u8, 5);
  FIELD_AT(v_r6, u8, 5) = (t8 | 32);
  FIELD_AT(v_r6, u8, 9) = ((t7 & -13) & 15);
  t9 = Value2((s32 (*)())__divsi3, v_r8, 6);
  t10 = *(s32 *)v_r10;
  *(s32 *)v_r7 = (t10 + (t9 << 20));
  t11 = Value2((s32 (*)())__modsi3, v_r8, 6);
  t12 = FIELD_AT(v_r10, s32, 4);
  FIELD_AT(v_r7, s32, 4) = (t12 - (t11 << 20));
  t13 = FIELD_AT(v_r10, s32, 8);
  FIELD_AT(v_r7, s32, 8) = t13;
  t14 = Value0((s32 (*)())Random16Far);
  v_r5 = (t9 - 4);
  FIELD_AT(v_r7, s32, 12) = (((u32)((t14 << 1) + t14) >> 1) * v_r5);
  t15 = Value0((s32 (*)())Random16Far);
  v_r8 = (v_r8 + 1);
  FIELD_AT(v_r7, s32, 16) = ((t15 << 2) + 0x20000);
  FIELD_AT(v_r7, s32, 20) = 0;
  FIELD_AT(v_r7, s32, 24) = 0;
  v_r7 = (v_r7 + 28);
  v_r6 = (v_r6 + 40);
  if (v_r8 <= 53)
    goto L_02008706;
  t16 = Value1((s32 (*)())Heap_AllocEwram, 0x600);
  t17 = Value1((s32 (*)())Heap_Alloc, 0x1000);
  t18 = Value1((s32 (*)())Resource_GetTableEntry, 0x1e8);
  Call2((void (*)())Func_020009f0, t18, t17);
  t20 = Value0((s32 (*)())Resource_FindFreeEntry);
  t21 = Value3((s32 (*)())VramCache_Load, t20, 0x400, t17);
  *(s32 *)sp_ = t21;
  Call1((void (*)())Heap_Release, t17);
  v_r11 = t20;
  v_r10 = t16;
  v_r5 = (t16 + 12);
  v_r8 = 95;
L_020087e0:
  t23 = Value0((s32 (*)())Random16Far);
  v_r8 = (v_r8 - 1);
  *(s32 *)v_r5 = -((u32)(t23 << 5) >> 16);
  v_r5 = (v_r5 + 16);
  if (v_r8 >= 0)
    goto L_020087e0;
  Dma_Set((const void *)(s32)Field_Palette685, (void *)0x50003e0, 0x80000010,
          (volatile u32 *)0x40000d4);
  v_r6 = 0;
L_02008810:
  if ((31 & v_r6) != 0)
    goto L_0200881e;
  Call1((void (*)())Func_02000b58, 145);
L_0200881e:
  if (v_r6 != 60)
    goto L_0200883a;
  Call2((void (*)())Func_02000b10, 0x7fff, 0);
  Call1((void (*)())Func_02000b18, 90);
  Call1((void (*)())Func_02000b58, 141);
L_0200883a:
  if (v_r6 != 105)
    goto L_0200885e;
  t28 = FIELD_AT(sp_, s32, 16);
  ((s32 (*)())t28)();
  FIELD_AT(sp_, s32, 4) = 1;
  Call3((void (*)())Func_02000a50, 0x60000, 0x60000, 0x10000);
  Call1((void (*)())Func_02000b58, 228);
L_0200885e:
  if (v_r6 != 130)
    goto L_02008866;
  FIELD_AT(sp_, s32, 4) = 1;
L_02008866:
  t32 = FIELD_AT(sp_, s32, 4);
  if (t32 == 0)
    goto L_020088ac;
  v_r5 = (v_r9 + 0x1000);
  v_r8 = 53;
  v_r7 = (v_r9 + 0x1870);
L_0200887e:
  t33 = FIELD_AT(v_r7, s32, 24);
  if ((u32)t33 > (u32)59)
    goto L_0200889c;
  Call2((void (*)())Func_02000b48, v_r5, v_r7);
  Call3((void (*)())Func_02000b50, v_r7, 63, -0x3000);
  t36 = FIELD_AT(v_r7, s32, 24);
  FIELD_AT(v_r7, s32, 24) = (t36 + 1);
L_0200889c:
  v_r8 = (v_r8 - 1);
  v_r5 = (v_r5 + 40);
  v_r7 = (v_r7 + 28);
  if (v_r8 >= 0)
    goto L_0200887e;
L_020088ac:
  v_r5 = v_r10;
L_020088ae:
  t37 = FIELD_AT(v_r5, s32, 12);
  v_r2 = t37;
  if (v_r2 != 0)
    goto L_02008910;
  FIELD_AT(v_r5, s32, 4) = 0x40000000;
  FIELD_AT(v_r5, s32, 8) = v_r2;
  t38 = Value0((s32 (*)())Random16Far);
  t39 = FIELD_AT(v_r5, u16, 6);
  FIELD_AT(v_r5, u16, 6) =
      ((t39 & -0x200) | ((((u32)(t38 << 7) >> 16) + 52) & 0x1ff));
  t40 = Value0((s32 (*)())Random16Far);
  t41 = FIELD_AT(v_r5, u8, 9);
  FIELD_AT(v_r5, u8, 4) = (((u32)(t40 << 6) >> 16) + 16);
  FIELD_AT(v_r5, u8, 9) = ((t41 | 240) & -13);
  goto L_02008910;
L_02008910:
  t42 = FIELD_AT(v_r5, s32, 12);
  v_r3 = t42;
  if (v_r3 < 0)
    goto L_02008934;
  t43 = *(s32 *)sp_;
  t44 = FIELD_AT(v_r5, u16, 8);
  FIELD_AT(v_r5, u16, 8) =
      ((t44 & -0x400) | ((t43 + ((v_r3 >> 2) << 2)) & 0x3ff));
  Call2((void (*)())Func_02000a10, v_r5, 100);
L_02008934:
  t46 = FIELD_AT(v_r5, s32, 12);
  FIELD_AT(v_r5, s32, 12) = (t46 + 1);
  if ((t46 + 1) <= 31)
    goto L_02008958;
  t47 = Value0((s32 (*)())Random16Far);
  FIELD_AT(v_r5, s32, 12) = -((u32)((t47 << 3) - t47) >> 16);
  goto L_02008958;
L_02008958:
  v_r5 = (v_r5 + 16);
  if (v_r5 <= (v_r10 + 0x5f0))
    goto L_020088ae;
  Call1((void (*)())wait, 1);
  v_r6 = (v_r6 + 1);
  if (v_r6 > 149)
    goto L_02008972;
  goto L_02008810;
L_02008972:
  Call1((void (*)())Resource_ResetEntry, v_r11);
  Call1((void (*)())Heap_Release, v_r10);
  Call1((void (*)())FieldEvent_Wait, 40);
  Func_02000a50(0, 0, 0);
  t53 = FIELD_AT(sp_, s32, 12);
  Call1((void (*)())Resource_ResetEntry, t53);
  Call1((void (*)())HeapSlot_Release, 220);
  return;
}

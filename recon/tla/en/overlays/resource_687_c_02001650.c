// Draft, differ 317 of 272: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Data_02002580[];
extern u8 gFrameCount[];
extern u8 gData_020022c8[];
extern u8 gData_020022cc[];
extern u8 gData_020026c0[];
extern u8 gData_0200257c[];
s32 GameFlag_Test();
void Func_0200201c();
s32 Math_Sine();

s32 Func_02001650(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r10, r8, r2, r1, r0, r3, r5, r4, r7, r6, v0, v1;

  r10 = (s32)Data_02002580;
  r8 = 376 + ((s32)Ram_HeapSlots->mapWork);
  r2 = (s32)Ram_HeapSlots->eventWork;
  r1 = FIELD_AT(50331776, s32, 0);
  r0 = 0;
  r3 = FIELD_AT(((s32)Ram_HeapSlots->eventWork), s16, 358);
  if ((FIELD_AT(((s32)Ram_HeapSlots->eventWork), s16, 358)) == 0)
    goto L_02009682;
  goto L_02009894;
L_02009682:
  r0 = 0x2a01;
  r3 = ((FIELD_AT(r1, u8, 10753)) << 24) >> 24;
  if ((((FIELD_AT(r1, u8, 10753)) << 24) >> 24) == 0)
    goto L_02009696;
  goto L_02009894;
L_02009696:
  r3 = FIELD_AT(r2, u16, 434);
  if ((FIELD_AT(r2, u16, 434)) != 0)
    goto L_020096a4;
  goto L_02009894;
L_020096a4:
  if ((u32)((FIELD_AT((s32)gFrameCount, s32, 0)) -
            (FIELD_AT((s32)gData_020022c8, s32, 0))) <=
      (u32)(FIELD_AT((s32)gData_020022cc, s32, 0)))
    goto L_0200970c;
  r5 = 14;
  r4 = ((FIELD_AT(83886204, u16, 0)) << 16) >> 16;
  r7 = *(s16 *)(((s32)gData_020026c0) + 26);
  r6 = (s32)gData_020026c0;
  r1 = 26;
  r0 = 24;
L_020096d0:
  FIELD_AT(83886176 + (r5 << 1), u16, 0) = FIELD_AT((r5 << 1), u16, 83886174);
  r5 = r5 - 1;
  *(u16 *)(r6 + r1) = *(u16 *)(r6 + r0);
  r1 = r1 - 2;
  r0 = r0 - 2;
  if (r5 != 0)
    goto L_020096d0;
  FIELD_AT(83886178, u16, 0) = r4;
  FIELD_AT((s32)gData_020026c0, u16, 0) = r7;
  FIELD_AT((s32)gData_020022c8, s32, 0) = FIELD_AT((s32)gFrameCount, s32, 0);
L_0200970c:
  if (((FIELD_AT((s32)gFrameCount, s32, 0)) & 1) == 0)
    goto L_0200971a;
  goto L_0200981c;
L_0200971a:
  r5 = 14;
  r4 = ((FIELD_AT(83886236, u16, 0)) << 16) >> 16;
L_02009728:
  r5 = r5 - 1;
  FIELD_AT((r5 << 1), u16, 83886208) = FIELD_AT((r5 << 1), u16, 83886206);
  if (r5 != 0)
    goto L_02009728;
  FIELD_AT(83886210, u16, 0) = r4;
  if (((FIELD_AT((s32)gFrameCount, s32, 0)) & 3) != 0)
    goto L_0200981c;
  r5 = (FIELD_AT(83886110, u16, 0)) << 16;
  r6 = (u32)r5 >> 21;
  r7 = 31;
  r5 = (u32)r5 >> 26;
  r7 = r7 & (FIELD_AT(83886110, u16, 0));
  r6 = r6 & (31);
  r5 = r5 & (31);
  v0 = GameFlag_Test(0x200);
  r6 = r6 << 16;
  r4 = r7 << 16;
  r1 = r5 << 16;
  if (v0 != 0)
    goto L_020097d0;
  r7 =
      ((((u32)r4 >> 16) + (FIELD_AT((s32)gData_0200257c, u16, 0))) << 16) >> 16;
  r0 = (s32)gData_0200257c;
  r3 = (u32)((((u32)r4 >> 16) + (FIELD_AT((s32)gData_0200257c, u16, 0)))
             << 16) >>
       16;
  goto L_020097b0;
L_020097b0:
  if ((u32)r3 > (u32)17)
    goto L_020097bc;
  r7 = 18;
  FIELD_AT(r0, u16, 0) = 1;
  goto L_02009804;
L_020097bc:
  if ((u32)r3 <= (u32)31)
    goto L_02009804;
  r7 = 31;
  FIELD_AT(r0, u16, 0) = -1;
  goto L_02009804;
L_020097d0:
  r3 = FIELD_AT(((s32)Ram_HeapSlots->eventWork), s16, 362);
  if ((FIELD_AT(((s32)Ram_HeapSlots->eventWork), s16, 362)) != 200)
    goto L_020097f8;
  r3 = (u32)r4 >> 16;
  if (((u32)r4 >> 16) == 0)
    goto L_02009804;
  r7 = ((r3 + 65535) << 16) >> 16;
  goto L_02009804;
L_020097f8:
  if (r3 != 201)
    goto L_02009804;
  r3 = (u32)r4 >> 16;
  if ((u32)((u32)r4 >> 16) > (u32)29)
    goto L_02009804;
  r7 = r3 + 1;
L_02009804:
  FIELD_AT(83886110, u16, 0) =
      (((u32)r1 >> 6) | ((u32)r6 >> 11)) | (((u32)(r7 << 16) >> 16) & 31);
L_0200981c:
  /* stmia r3!, {r0, r1, r2} */
  Call4((void (*)())Func_0200201c, 0x10000, 0, -0x7bffff90, r3 - 12);
  FIELD_AT(r8, s32, 28) = 0;
  r5 = 0;
L_0200984a:
  v1 = Math_Sine(
      ((r5 + (FIELD_AT((s32)gFrameCount, u8, 0))) + (*(s16 *)(r8 + 6))) << 9);
  r5 = r5 + 1;
  FIELD_AT(r10, u16, 0) = (*(s16 *)(r8 + 2)) + (v1 >> 15);
  r10 = r10 + 2;
  if (r5 != 160)
    goto L_0200984a;
  r10 = (s32)Data_02002580;
  FIELD_AT(67108884, u16, 0) = FIELD_AT((s32)Data_02002580, u16, 0);
  /* stmia r3!, {r0, r1, r2} */
  r0 = r10 + 2;
  r3 = r3 - 12;
L_02009894:
  return r0;
}

// Draft, differ 156 of 179: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Data_02004030[];
extern u8 Func_020012c4[];
s32 MemAlloc();
s32 GameFlag_Test();
void IwramClearWords();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
void MemFree();
s32 Scheduler_AddOrUpdateCallback();

s32 Func_02001420(void) {
  s32 r7, v0, r6, r8, r5, v1, r2, v2, r0, ip, r3;
  s32 stack[1];

  /* sub (s32)stack, #4 */
  r7 = (s32)Data_02004030;
  v0 = MemAlloc(128);
  r6 = v0;
  stack[0] = r6;
  r8 = 320 + ((s32)Ram_HeapSlots->mapWork);
  r5 = r7;
  r5 = r5 + 16;
  v1 = GameFlag_Test(0x109);
  if (v1 != 0)
    goto L_0200945c;
  IwramClearWords(r7, 240);
L_0200945c:
  r2 = 31;
L_0200945e:
  /* stmia r0!, {r3} */
  stack[0] = r0;
  r2 = r2 - 1;
  if ((r2 - 1) >= 0)
    goto L_0200945e;
  v2 = Resource_FindFreeEntry();
  FIELD_AT(r7, u16, 2) = v2;
  VramCache_Load((v2 << 16) >> 16, 128, r6);
  MemFree(r6);
  FIELD_AT(r7, s32, 8) = FIELD_AT(r8, s32, 8);
  FIELD_AT(r7, s32, 12) = FIELD_AT(r8, s32, 12);
  FIELD_AT(67108938, u16, 0) = 0x1b1f;
  FIELD_AT(67108864, u16, 0) = (FIELD_AT(67108864, u16, 0)) | (32768);
  Scheduler_AddOrUpdateCallback((s32)Func_020012c4, 0x480);
  r0 = 0x10000;
  ip = 482 + (s32)(s32)&gPartyState;
  if ((u32)(((FIELD_AT(482 + (s32)(s32)&gPartyState, u16, 0)) - 1) << 16) >
      (u32)65536)
    goto L_0200953e;
  goto L_020094e8;
L_020094e8:
  FIELD_AT(r5, s32, 0) = 0x280000;
  FIELD_AT(r5, s32, 12) = 1;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 4) = 0x780000;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0x380000;
  FIELD_AT(r5, s32, 12) = 1;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 4) = 0x780000;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0xd80000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 4) = 0x780000;
  FIELD_AT(r5, s32, 12) = 2;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0xe80000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 4) = 0x780000;
  FIELD_AT(r5, s32, 12) = 2;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0xb80000;
  FIELD_AT(r5, s32, 4) = 0x580000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 12) = 4;
  r5 = r5 + 28;
  r0 = 2;
L_0200953e:
  r3 = ((FIELD_AT(ip, u16, 0)) - 3) << 16;
  if ((u32)(((FIELD_AT(ip, u16, 0)) - 3) << 16) > (u32)65536)
    goto L_020095a4;
  FIELD_AT(r5, s32, 0) = 0x280000;
  FIELD_AT(r5, s32, 4) = 0x1b80000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 12) = 1;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0x380000;
  FIELD_AT(r5, s32, 4) = 0x1b80000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 12) = 1;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0xd80000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 4) = 0x1980000;
  FIELD_AT(r5, s32, 12) = 0;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0xe80000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 4) = 0x1980000;
  FIELD_AT(r5, s32, 12) = 1;
  r5 = r5 + 28;
  FIELD_AT(r5, s32, 0) = 0x480000;
  FIELD_AT(r5, s32, 4) = 0x1780000;
  FIELD_AT(r5, s32, 8) = 0;
  FIELD_AT(r5, s32, 12) = 4;
  r0 = 1;
  r3 = 4;
L_020095a4:
  /* add (s32)stack, #4 */
  return r0;
}

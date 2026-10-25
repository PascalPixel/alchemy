// Draft, differ 92 of 83: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Func_02002cc0[];
s32 Runtime_AllocateHeapBlockFar();
s32 Resource_GetTableEntry();
void Func_02002f40();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
void Func_02003190();
s32 Scheduler_AddOrUpdateCallback();

s32 Func_02002e00(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r6_1, r5_1, r10_1, r7_1, r5_2, r8_1;
  s32 v0, r6, v1, v2, r5, v3, r10, r7, r8, v4, r0;
  s32 stack[1];

  /* sub (s32)stack, #4 */
  v0 = Runtime_AllocateHeapBlockFar(220, 0x1608);
  r6_1 = v0;
  v1 = Resource_GetTableEntry(496);
  Func_02002f40(v1, r6_1);
  v2 = Resource_FindFreeEntry();
  r5_1 = v2;
  v3 = VramCache_Load(v2, 0x500, r6_1);
  r10_1 = v3;
  FIELD_AT(r6_1, u16, 5634) = r10_1;
  FIELD_AT(r6_1, u16, 5636) = r5_1;
  r7_1 = r6_1 + 1280;
  r5_2 = r6_1 + 3072;
  r8_1 = 63;
L_0200ae58:
  Func_02003190(r5_2, 8, 8, 0x40000000, r10_1);
  FIELD_AT(r5_2, u8, 5) = (FIELD_AT(r5_2, u8, 5)) | 32;
  FIELD_AT(r5_2, u8, 9) = (15 & (FIELD_AT(r5_2, u8, 9))) & -13;
  FIELD_AT(r5_2, u16, 30) = 240;
  r8_1 = r8_1 - 1;
  FIELD_AT(r7_1, s32, 24) = -1;
  r5_2 = r5_2 + 40;
  r7_1 = r7_1 + 28;
  if (r8_1 >= 0)
    goto L_0200ae58;
  FIELD_AT(r6_1, u16, 5632) = 0;
  v4 = Scheduler_AddOrUpdateCallback((s32)Func_02002cc0, 0x480);
  /* add (s32)stack, #4 */
  return v4;
}

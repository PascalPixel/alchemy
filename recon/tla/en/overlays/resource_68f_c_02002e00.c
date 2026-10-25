// Draft, differ 108 of 83: translated from its listing; names and structure
// still to do. Measured against its copies: overlay 691's Func_020038dc (differ
// 108 of 83).
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

extern u8 Func_02002cc0[];
s32 Runtime_AllocateHeapBlockFar();
s32 Resource_GetTableEntry();
s32 Func_02002f40();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
s32 Func_02003190();
s32 Scheduler_AddOrUpdateCallback();

s32 Func_02002e00(void) {
  s32 v0, r6, v1, v2, r5, v3, r10, r7, r8, v4, r0;
  s32 stack[1];

  /* sub (s32)stack, #4 */
  v0 = Runtime_AllocateHeapBlockFar(220, 0x1608);
  r6 = v0;
  v1 = Resource_GetTableEntry(496);
  Func_02002f40(v1, r6);
  v2 = Resource_FindFreeEntry();
  r5 = v2;
  v3 = VramCache_Load(v2, 0x500, r6);
  r10 = v3;
  FIELD_AT(r6, u16, 5634) = r10;
  FIELD_AT(r6, u16, 5636) = r5;
  r7 = r6 + 1280;
  r5 = r6 + 3072;
  r8 = 63;
L_0200ae58:
  Func_02003190(r5, 8, 8, 0x40000000, r10);
  FIELD_AT(r5, u8, 5) = (FIELD_AT(r5, u8, 5)) | 32;
  FIELD_AT(r5, u8, 9) = (15 & (FIELD_AT(r5, u8, 9))) & -13;
  FIELD_AT(r5, u16, 30) = 240;
  r8 = r8 - 1;
  FIELD_AT(r7, s32, 24) = -1;
  r5 = r5 + 40;
  r7 = r7 + 28;
  if (r8 >= 0)
    goto L_0200ae58;
  FIELD_AT(r6, u16, 5632) = 0;
  v4 = Scheduler_AddOrUpdateCallback((s32)Func_02002cc0, 0x480);
  /* add (s32)stack, #4 */
  return v4;
}

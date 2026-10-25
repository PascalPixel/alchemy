// Draft, differ 120 of 136: shows the glow at a spot: allocates its work, loads
// its picture and sprites, starts its frame callback and a shake, then grows
// its first sprite, waits ten frames and grows its second in two stages.
// Translated from its listing; names and structure still to do.
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

extern u8 Func_02002de4[];
s32 Runtime_AllocateHeapBlockFar();
s32 Func_02002c88();
void Func_02002d34();
s32 Scheduler_AddOrUpdateCallback();
void Func_02003434();
void WaitFrames();

s32 Func_02002f58(s32 a0, s32 a1, s32 a2) {
  s32 r5_1, r6_1, r7_1, r5_2, r7_2, r5_3, r7_3, r5_4, r7_4, r5_5, r5_6;
  s32 r5, v0, r6, r7, v1, r2, v2, r0;

  r5_1 = a0;
  v0 = Runtime_AllocateHeapBlockFar(220, 0x1078);
  r6_1 = v0;
  FIELD_AT(r6_1, s32, 4096) = FIELD_AT(r5_1, s32, 0);
  FIELD_AT(r6_1, s32, 4100) = FIELD_AT(r5_1, s32, 4);
  r7_1 = 0;
  FIELD_AT(r6_1, s32, 4104) = FIELD_AT(r5_1, s32, 8);
  v1 = Func_02002c88();
  Func_02002d34();
  FIELD_AT(r6_1, u16, 4212) = 0;
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02002de4, 0x480);
  Call3((void (*)())Func_02003434, 0x40000, 0x40000, 0x10000);
  r5_2 = 0;
L_0200afba:
  if (r5_2 == 1)
    goto L_0200affe;
  if (r5_2 > 1)
    goto L_0200afc8;
  if (r5_2 == 0)
    goto L_0200afd2;
  goto L_0200b060;
L_0200afc8:
  if (r5_2 == 2)
    goto L_0200b00a;
  if (r5_2 == 3)
    goto L_0200b036;
  goto L_0200b060;
L_0200afd2:
  FIELD_AT(r6_1, s32, 4196) = (FIELD_AT(r6_1, s32, 4196)) + 1536;
  r2 = r6_1 + 4196;
  if (((FIELD_AT(r6_1, s32, 4196)) + 1536) <= 0x14ccb)
    goto L_0200b060;
  r7_2 = 1;
  FIELD_AT(r2, s32, 0) = 0x14ccc;
  r5_3 = 1;
  r7_2 = -r7_2;
  goto L_0200b060;
L_0200affe:
  if (r7_2 != 10)
    goto L_0200b060;
  r7_3 = 1;
  r5_4 = 2;
  r7_3 = -r7_3;
  goto L_0200b060;
L_0200b00a:
  FIELD_AT(r6_1, s32, 4204) = (FIELD_AT(r6_1, s32, 4204)) + 1024;
  r2 = r6_1 + 4204;
  if (((FIELD_AT(r6_1, s32, 4204)) + 1024) <= 0x6665)
    goto L_0200b060;
  r7_4 = 1;
  FIELD_AT(r2, s32, 0) = 0x6666;
  r5_5 = 3;
  r7_4 = -r7_4;
  goto L_0200b060;
L_0200b036:
  FIELD_AT(r6_1, s32, 4204) = (FIELD_AT(r6_1, s32, 4204)) + 12288;
  r2 = r6_1 + 4204;
  if (((FIELD_AT(r6_1, s32, 4204)) + 12288) <= 0x14ccb)
    goto L_0200b060;
  r5_6 = 186;
  FIELD_AT(r2, s32, 0) = 0x14ccc;
  r5_6 = r5_6 << 2;
  r5_6 = r5_6 + 255;
L_0200b060:
  Call1((void (*)())WaitFrames, 1);
  r7_4 = r7_4 + 1;
  r0 = v2;
  if (r5_6 != 0x3e7)
    goto L_0200afba;
  return r0;
}

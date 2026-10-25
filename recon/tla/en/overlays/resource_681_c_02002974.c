// Draft, differ 88 of 75: records which of eleven listed cells persons 10 and
// 11 stand on, as the two halves of the byte flag 0x210. Translated from its
// listing; names and structure still to do.
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

extern u8 gData_0200530c[];
s32 Object_GetById();
s32 GameFlag_SetByte();

s32 Func_02002974(void) {
  s32 v0, r8, r9, r10, r7, v1, r6, r4, r11, r1, ip, r0, r2, r5, v2;

  v0 = Object_GetById(10);
  r8 = (FIELD_AT(v0, s32, 8)) >> 19;
  r9 = 0;
  r10 = 0;
  r7 = (FIELD_AT(v0, s32, 16)) >> 19;
  v1 = Object_GetById(11);
  r6 = (FIELD_AT(v1, s32, 8)) >> 19;
  r4 = 1;
  r11 = (FIELD_AT(v1, s32, 16)) >> 19;
  r1 = r4 << 24;
  ip = (s32)gData_0200530c;
L_0200a9b0:
  r0 = ip;
  r2 = r1 >> 22;
  if (r8 != (*(s16 *)(ip + (r1 >> 22))))
    goto L_0200a9c8;
  if (r7 != (*(s16 *)(r0 + (r2 + 2))))
    goto L_0200a9c8;
  r9 = (u32)r1 >> 24;
L_0200a9c8:
  r2 = r4 << 24;
  r1 = (r4 << 24) >> 22;
  if (r6 != (*(s16 *)(r0 + ((r4 << 24) >> 22))))
    goto L_0200a9de;
  if (r11 != (*(s16 *)(r0 + (r1 + 2))))
    goto L_0200a9de;
  r10 = (u32)r2 >> 24;
L_0200a9de:
  r4 = 128;
  r4 = r4 << 17;
  r4 = (u32)(r2 + r4) >> 24;
  r5 = 176;
  r5 = r5 << 20;
  r1 = r4 << 24;
  if ((r4 << 24) <= r5)
    goto L_0200a9b0;
  v2 = GameFlag_SetByte(0x210, ((r9 << 24) | (r10 << 28)) >> 24);
  return v2;
}

// Draft, differ 193 of 150: translated from its listing; names and structure
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

extern u8 Func_02000c50[];
extern u8 gData_020022cc[];
extern u8 gData_020026c0[];
void Scheduler_RemoveCallbackFar();
s32 Func_02001fac();
s32 Func_02002044();
s32 Func_02002054();
void WaitFrames();
void GameFlag_ClearBit();
s32 Func_02001fb4();

s32 Func_02000da0(void) {
  s32 v0, r4, r9, r11, r10, r8, r5, r6, r7, r1, r0, r2, r3, ip, v1, v2, lr;

  Scheduler_RemoveCallbackFar((s32)Func_02000c50);
  Func_02001fac();
  Func_02002044(0);
  Func_02002054(0x22c);
  FIELD_AT((s32)gData_020022cc, s32, 0) = 1;
L_02008dcc:
  WaitFrames(10);
  r4 = 160;
  r4 = r4 << 19;
  r4 = r4 + 98;
  r9 = 1;
  r11 = (s32)gData_020026c0;
  r10 = 0;
  r8 = r4;
L_02008de8:
  r4 = 31;
  r5 = r4;
  r5 = r5 & (FIELD_AT(r8, u16, 0));
  r6 = (u32)((*(s16 *)(r10 + r11)) << 16) >> 21;
  r7 = (u32)((*(s16 *)(r10 + r11)) << 16) >> 26;
  r6 = r6 & (31);
  r7 = r7 & (31);
  r1 = ((u32)((FIELD_AT(r8, u16, 0)) << 16) >> 21) & (31);
  r0 = ((u32)((FIELD_AT(r8, u16, 0)) << 16) >> 26) & (31);
  r2 = ((u32)((*(s16 *)(r10 + r11)) << 16) >> 16) & r4;
  r3 = r5;
  if ((u32)r5 >= (u32)(((u32)((*(s16 *)(r10 + r11)) << 16) >> 16) & r4))
    goto L_02008e30;
  r5 = r3 + 1;
  goto L_02008e30;
L_02008e30:
  r3 = (u32)(r5 << 16) >> 16;
  if ((u32)((u32)(r5 << 16) >> 16) <= (u32)r2)
    goto L_02008e48;
  r4 = 255;
  r4 = r4 << 8;
  r4 = r4 + 255;
  r5 = ((r3 + r4) << 16) >> 16;
L_02008e48:
  ip = r6;
  r3 = (u32)(r1 << 16) >> 16;
  if ((u32)((u32)(r1 << 16) >> 16) >= (u32)r6)
    goto L_02008e5c;
  r1 = ((r3 + 1) << 16) >> 16;
L_02008e5c:
  r3 = (u32)(r1 << 16) >> 16;
  if ((u32)((u32)(r1 << 16) >> 16) <= (u32)ip)
    goto L_02008e74;
  r4 = 255;
  r4 = r4 << 8;
  r4 = r4 + 255;
  r1 = ((r3 + r4) << 16) >> 16;
L_02008e74:
  ip = r7;
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) >= (u32)r7)
    goto L_02008e88;
  r0 = ((r3 + 1) << 16) >> 16;
L_02008e88:
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) <= (u32)ip)
    goto L_02008ea0;
  r4 = 255;
  r4 = r4 << 8;
  r4 = r4 + 255;
  r0 = ((r3 + r4) << 16) >> 16;
L_02008ea0:
  r9 = r9 + 1;
  r4 = 2;
  FIELD_AT(r8, u16, 0) = ((u32)(r5 << 16) >> 16) |
                         (((u32)(r0 << 16) >> 6) | ((u32)(r1 << 16) >> 11));
  r8 = r8 + r4;
  r10 = r10 + r4;
  if (r9 != 15)
    goto L_02008de8;
  if (lr == 0)
    goto L_02008ecc;
  goto L_02008dcc;
L_02008ecc:
  FIELD_AT((s32)gData_020022cc, s32, 0) = 1;
  GameFlag_ClearBit(0x200);
  v2 = Func_02001fb4();
  return v2;
}

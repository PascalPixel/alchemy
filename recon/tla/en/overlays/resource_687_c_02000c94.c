// Draft, differ 113 of 116: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "call.h"
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

extern u8 gFrameCount[];
extern u8 gData_020022cc[];
extern u8 gData_020022c8[];
extern u8 Func_02000c50[];
void Func_02001fac();
void Func_02002044();
void GameFlag_SetBit();
void Func_02002054();
void wait();
s32 Task_Add();
s32 Func_02001fb4();

s32 Func_02000c94(s32 a0) {
  s32 r6, r5, r7, r4, r0, r2, r3, r1, v0, v1;

  Func_02001fac();
  Call1((void (*)())Func_02002044, 0);
  Call1((void (*)())GameFlag_SetBit, 0x200);
  Call1((void (*)())Func_02002054, 0x22d);
L_02008cb2:
  Call1((void (*)())wait, 10);
  r6 = 160;
  r6 = r6 << 19;
  r5 = 0;
  r7 = 1;
  r6 = r6 + 98;
L_02008cc2:
  r4 = 31;
  r4 = r4 & (FIELD_AT(r6, u16, 0));
  r0 = ((u32)((FIELD_AT(r6, u16, 0)) << 16) >> 21) & (31);
  r2 = ((u32)((FIELD_AT(r6, u16, 0)) << 16) >> 26) & (31);
  r3 = r4;
  if ((u32)r4 <= (u32)10) {
    r4 = r3 + 1;
    r5 = 1;
  }
  r3 = (u32)(r4 << 16) >> 16;
  if ((u32)((u32)(r4 << 16) >> 16) <= (u32)11)
    goto L_02008cfc;
  r4 = ((r3 + 65535) << 16) >> 16;
  r5 = 1;
  goto L_02008cfc;
L_02008cfc:
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) <= (u32)5) {
    r5 = 1;
    r0 = ((r3 + 1) << 16) >> 16;
  }
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) > (u32)6) {
    r5 = 1;
    r0 = ((r3 + 65535) << 16) >> 16;
  }
  r3 = (u32)(r2 << 16) >> 16;
  if ((u32)((u32)(r2 << 16) >> 16) <= (u32)22) {
    r5 = 1;
    r2 = ((r3 + 1) << 16) >> 16;
  }
  r3 = (u32)(r2 << 16) >> 16;
  if ((u32)((u32)(r2 << 16) >> 16) > (u32)23) {
    r5 = 1;
    r2 = ((r3 + 65535) << 16) >> 16;
  }
  r7 = r7 + 1;
  FIELD_AT(r6, u16, 0) = ((u32)(r4 << 16) >> 16) |
                         (((u32)(r2 << 16) >> 6) | ((u32)(r0 << 16) >> 11));
  r6 = r6 + 2;
  if (r7 != 14)
    goto L_02008cc2;
  FIELD_AT((s32)gData_020022cc, s32, 0) =
      (FIELD_AT((s32)gData_020022cc, s32, 0)) +
      (1 & (FIELD_AT((s32)gFrameCount, s32, 0)));
  r0 = FIELD_AT((s32)gFrameCount, s32, 0);
  r1 = (s32)gData_020022cc;
  if (r5 != 0)
    goto L_02008cb2;
  FIELD_AT(r1, s32, 0) = 0x168;
  FIELD_AT((s32)gData_020022c8, s32, 0) = r0;
  v0 = Task_Add((s32)Func_02000c50, 0x480);
  v1 = Func_02001fb4();
  return v1;
}

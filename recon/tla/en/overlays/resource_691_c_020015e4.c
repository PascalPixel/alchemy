// Draft, differ 184 of 153: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "adventure.h"
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
extern u8 Func_020011b8[];
extern u8 gData_02003d50[];
s32 Random16Far();
void Func_0200015c();
s32 Func_020005cc();

s32 Func_020015e4(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r6, r8, r4, r2, r0, r1, r7, r10, r5, v0, v1;
  s32 stack[14];

  r6 = a0;
  r8 = FIELD_AT(r6, s32, 104);
  r4 = r8;
  r4 = r4 + 100;
  /* sub (s32)stack, #56 */
  r2 = FIELD_AT(r4, u16, 0);
  if ((u32)(((FIELD_AT(r4, u16, 0)) - 35) << 16) > (u32)458752)
    goto L_02009624;
  FIELD_AT(r6, s32, 12) = (FIELD_AT(r8, s32, 12)) + 1572864;
  FIELD_AT(r6, s32, 12) =
      (((FIELD_AT(r8, s32, 12)) + 1572864) + ((FIELD_AT(r4, s16, 0)) << 19)) -
      17825792;
  goto L_020096a4;
L_02009624:
  r0 = r6 + 98;
  if ((u32)((r2 - 9) << 16) > (u32)21102592)
    goto L_0200964a;
  FIELD_AT(r6, s32, 12) =
      ((FIELD_AT(r8, s32, 12)) + ((FIELD_AT(r0, u8, 0)) << 20)) + 1572864;
  goto L_020096a4;
L_0200964a:
  r1 = (r2 << 16) >> 16;
  if (((r2 << 16) >> 16) <= 0x14c)
    goto L_02009686;
  r2 = FIELD_AT(r0, u8, 0);
  if (r1 >= (((FIELD_AT(r0, u8, 0)) << 1) + 333))
    goto L_02009686;
  FIELD_AT(r6, s32, 12) = ((FIELD_AT(r8, s32, 12)) + (r2 << 20)) + 1572864;
  FIELD_AT(r6, s32, 12) = ((((FIELD_AT(r8, s32, 12)) + (r2 << 20)) + 1572864) -
                           ((FIELD_AT(r4, s16, 0)) << 19)) +
                          174063616;
  goto L_020096a4;
L_02009686:
  if ((((FIELD_AT(r6, u8, 98)) << 1) + 333) >= (FIELD_AT(r4, s16, 0)))
    goto L_020096a4;
  FIELD_AT(r6, s32, 12) = FIELD_AT(r6, s32, 20);
  FIELD_AT(r6, s32, 108) = 0;
L_020096a4:
  r7 = FIELD_AT((s32)gFrameCount, s32, 0);
  r10 = 7;
  r7 = r7 & 7;
  if (r7 == 0) {
    r5 = (s32)((u8 *)stack + 16);
    FIELD_AT(r5, u16, 24) = 0x193;
    FIELD_AT(r5, s32, 4) = 10;
    FIELD_AT(r5, s32, 0) = 2;
    FIELD_AT(r5, s32, 36) = (s32)Func_020011b8;
    FIELD_AT(r5, s32, 28) = (s32)gData_02003d50;
    v0 = Random16Far();
    r4 = -0x20000;
    stack[0] = r4;
    r4 = 0x1330000;
    Func_0200015c((FIELD_AT(r6, s32, 8)) + 65536,
                  (FIELD_AT(r6, s32, 12)) - 262144, FIELD_AT(r6, s32, 16),
                  ((v0 & r10) - 3) << 14, r4, r7, r4, r5);
  }
  v1 = Func_020005cc(FIELD_AT(r8, u8, 34), (FIELD_AT(r8, s32, 8)) >> 20,
                     (FIELD_AT(r8, s32, 16)) >> 20,
                     ((FIELD_AT(r6, s32, 12)) >> 19) + 1);
  /* add (s32)stack, #56 */
  return v1;
}

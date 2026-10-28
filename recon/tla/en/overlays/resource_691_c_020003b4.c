// Draft, differ 210 of 175: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_02003d10[];
s32 Actor_GetById();
s32 Actor_FindInCell();
s32 Func_02003aac();
s32 Func_02003a54();
void wait();
void Func_02003c54();
void Func_02003a84();
s32 Func_02003a8c();
void Func_02003bdc();

s32 Func_020003b4(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 v0, r5, r9, r10, r7, r8, v1, r6, r0, v2, r3, v3, v4, v5, v6;
  s32 stack[3];

  /* sub (s32)stack, #12 */
  v0 = Actor_GetById(FIELD_AT((s32)(s32)&gSequenceParentWork, s32, 532));
  r5 = ((u32)(FIELD_AT(v0, u16, 6)) >> 12) << 2;
  r9 = (s32)gData_02003d10;
  r10 = -0x10000;
  r7 = (s32)stack;
  FIELD_AT(r7, s32, 0) =
      (FIELD_AT(v0, s32, 8)) + ((*(s32 *)(((s32)gData_02003d10) + r5)) & r10);
  r8 = v0;
  FIELD_AT(r7, s32, 4) = FIELD_AT(v0, s32, 12);
  FIELD_AT(r7, s32, 8) =
      (FIELD_AT(v0, s32, 16)) + ((*(s32 *)(((s32)gData_02003d10) + r5)) << 16);
  v1 = Actor_FindInCell(r7, r8);
  r6 = v1;
  r0 = v1;
  if (r6 != 0)
    goto L_0200840a;
  goto L_02008520;
L_0200840a:
  FIELD_AT(r7, s32, 0) = (FIELD_AT(r6, s32, 8)) + ((*(s32 *)(r9 + r5)) & r10);
  FIELD_AT(r7, s32, 4) = FIELD_AT(r6, s32, 12);
  FIELD_AT(r7, s32, 8) = (FIELD_AT(r6, s32, 16)) + ((*(s32 *)(r9 + r5)) << 16);
  v2 = Actor_FindInCell(r7, r6);
  r0 = v2;
  if (v2 != 0) {
    r3 = 1 & (FIELD_AT(r0, u8, 89));
    if ((1 & (FIELD_AT(r0, u8, 89))) != 0)
      goto L_02008520;
  }
  FIELD_AT(r7, s32, 0) = FIELD_AT(r6, s32, 8);
  FIELD_AT(r7, s32, 4) = (FIELD_AT(r6, s32, 12)) + 1048576;
  FIELD_AT(r7, s32, 8) = FIELD_AT(r6, s32, 16);
  v3 = Actor_FindInCell(r7, r6);
  r0 = v3;
  if (v3 != 0) {
    r3 = 1 & (FIELD_AT(r0, u8, 89));
    if ((1 & (FIELD_AT(r0, u8, 89))) != 0)
      goto L_02008520;
  }
  FIELD_AT(r6, u8, 34) = 2;
  FIELD_AT(r7, s32, 0) = (FIELD_AT(r6, s32, 8)) + ((*(s32 *)(r9 + r5)) & r10);
  FIELD_AT(r7, s32, 4) = FIELD_AT(r6, s32, 12);
  FIELD_AT(r7, s32, 8) = (FIELD_AT(r6, s32, 16)) + ((*(s32 *)(r9 + r5)) << 16);
  v4 = Func_02003aac(r6, r7);
  r0 = v4;
  if (v4 > 0)
    goto L_02008520;
  r10 = FIELD_AT(r6, u8, 98);
  r3 = FIELD_AT(r6, u8, 98);
  if ((FIELD_AT(r6, u8, 98)) != 0)
    goto L_02008520;
  r5 = 204;
  Call2((void (*)())Func_02003a54, r8, 8);
  r5 = r5 << 6;
  Call1((void (*)())wait, 15);
  r5 = r5 + 51;
  Call1((void (*)())Func_02003c54, 185);
  FIELD_AT(r6, s32, 48) = r5;
  FIELD_AT(r6, s32, 52) = r5;
  Func_02003a84(r6, FIELD_AT(r7, s32, 0), FIELD_AT(r7, s32, 4),
                FIELD_AT(r7, s32, 8));
  FIELD_AT(r8, s32, 48) = r5;
  FIELD_AT(r8, s32, 52) = r5;
  Func_02003a84(r8, FIELD_AT(r7, s32, 0), FIELD_AT(r7, s32, 4),
                FIELD_AT(r7, s32, 8));
  v5 = Func_02003a8c(r6);
  Func_02003bdc();
  FIELD_AT(r6, s32, 8) = FIELD_AT(r7, s32, 0);
  FIELD_AT(r6, s32, 36) = r10;
  FIELD_AT(r6, s32, 16) = FIELD_AT(r7, s32, 8);
  FIELD_AT(r6, s32, 44) = r10;
  FIELD_AT(r8, s32, 56) = -0x80000000;
  FIELD_AT(r8, s32, 64) = -0x80000000;
  FIELD_AT(r8, s32, 36) = r10;
  FIELD_AT(r8, s32, 44) = r10;
  FIELD_AT(r8, s32, 8) = (*(s16 *)(r8 + 10)) << 16;
  FIELD_AT(r8, s32, 16) = (*(s16 *)(r8 + 18)) << 16;
  v6 = Func_02003a54(r8, 1);
  r0 = v6;
L_02008520:
  /* add (s32)stack, #12 */
  return r0;
}

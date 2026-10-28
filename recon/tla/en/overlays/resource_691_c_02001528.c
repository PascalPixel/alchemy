// Draft, differ 150 of 91: translated from its listing; names and structure still
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

s32 Func_02003a6c();

s32 Func_02001528(s32 a0) {
  s32 r5, r4, r0, r2, r6, r1, ip, r3, v0;

  r5 = FIELD_AT(a0, s32, 104);
  r4 = r5;
  r4 = r4 + 100;
  r0 = a0;
  r2 = FIELD_AT(r4, s16, 0);
  if ((FIELD_AT(r4, s16, 0)) <= 35)
    goto L_0200955c;
  if (r2 >= (((FIELD_AT(r0, u8, 98)) << 1) + 36))
    goto L_0200955c;
  FIELD_AT(r0, s32, 12) = (FIELD_AT(r5, s32, 12)) + 2097152;
  r6 = 0;
  r2 = ((FIELD_AT(r5, s32, 12)) + 2097152) + ((*(s16 *)(r4 + r6)) << 19);
  r1 = -0x1180000;
  goto L_020095be;
L_0200955c:
  r6 = 0;
  ip = 98 + r0;
  r2 = FIELD_AT(r0, u8, 98);
  r1 = *(s16 *)(r4 + r6);
  if ((((FIELD_AT(r0, u8, 98)) << 1) + 36) >= (*(s16 *)(r4 + r6)))
    goto L_02009588;
  if (r1 >= 0x14c)
    goto L_02009588;
  r6 = 128;
  r6 = r6 << 14;
  FIELD_AT(r0, s32, 12) = ((FIELD_AT(r5, s32, 12)) + (r2 << 20)) + r6;
  goto L_020095de;
L_02009588:
  r1 = FIELD_AT(r4, s16, 0);
  if ((FIELD_AT(r4, s16, 0)) <= 0x14c)
    goto L_020095c4;
  r6 = ip;
  r6 = 78;
  r6 = r6 + 255;
  r2 = FIELD_AT(r6, u8, 0);
  if (r1 >= (((FIELD_AT(r6, u8, 0)) << 1) + r6))
    goto L_020095c4;
  FIELD_AT(r0, s32, 12) = ((FIELD_AT(r5, s32, 12)) + (r2 << 20)) + 2097152;
  r6 = 0;
  r2 = (((FIELD_AT(r5, s32, 12)) + (r2 << 20)) + 2097152) -
       ((*(s16 *)(r4 + r6)) << 19);
  r1 = 0xa600000;
L_020095be:
  FIELD_AT(r0, s32, 12) = r2 + r1;
  goto L_020095de;
L_020095c4:
  r6 = 78;
  r6 = r6 + 255;
  r1 = 0;
  r2 = ((FIELD_AT(r0, u8, 98)) << 1) + r6;
  r3 = FIELD_AT(r4, s16, 0);
  if ((((FIELD_AT(r0, u8, 98)) << 1) + r6) >= (FIELD_AT(r4, s16, 0)))
    goto L_020095de;
  v0 = Func_02003a6c();
  r0 = v0;
L_020095de:
  return r0;
}

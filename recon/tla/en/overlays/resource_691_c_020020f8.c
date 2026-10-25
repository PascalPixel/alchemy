// Draft, differ 67 of 104: translated from its listing; names and structure still
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

extern u8 Func_020020d0[];
s32 Func_020038dc();
void Func_02002024();
s32 Func_02003820();
void Func_02003aa4();
s32 Func_02003a94();
s32 Func_02003998();

s32 Func_020020f8(s32 a0, s32 a1, s32 a2) {
  s32 r7, r6, r5, r0, r1, r2, r3, v0, v1, v2, v3;
  s32 stack[2];

  r7 = a1;
  /* sub (s32)stack, #8 */
  r6 = a0;
  r5 = FIELD_AT(50331872, s32, 0);
  r0 = a0;
  r1 = a1;
  r2 = a2;
  r3 = 0x30000e0;
  if (r7 != 0)
    goto L_0200a11e;
  v0 = Func_020038dc(r0, r1, r2);
  FIELD_AT((FIELD_AT(r5, s32, 20)), u8, 99) = 4;
  FIELD_AT(FIELD_AT(r5, s32, 20), s32, 108) = (s32)Func_020020d0;
  r0 = v0;
  r1 = (FIELD_AT(r5, s32, 20)) + 99;
  r2 = (s32)Func_020020d0;
L_0200a11e:
  if (r6 != 2)
    goto L_0200a140;
  Func_02002024(FIELD_AT(FIELD_AT(r5, s32, 20), s32, 8),
                FIELD_AT(FIELD_AT(r5, s32, 20), s32, 16));
  v1 = Func_02003820(FIELD_AT(FIELD_AT(r5, s32, 20), s32, 8),
                     (FIELD_AT(FIELD_AT(r5, s32, 20), s32, 12)) + 983040,
                     FIELD_AT(FIELD_AT(r5, s32, 20), s32, 16), 1);
  r0 = v1;
L_0200a140:
  if (r6 != 3)
    goto L_0200a1ce;
  r6 = 8;
  Func_02002024(FIELD_AT(FIELD_AT(r5, s32, 20), s32, 8),
                FIELD_AT(FIELD_AT(r5, s32, 20), s32, 16));
  Func_02002024(FIELD_AT(FIELD_AT(r5, s32, 20), s32, 8),
                FIELD_AT(FIELD_AT(r5, s32, 20), s32, 16));
  Func_02002024(FIELD_AT(FIELD_AT(r5, s32, 20), s32, 8),
                (FIELD_AT(FIELD_AT(r5, s32, 20), s32, 16)) - 2097152);
  Call4((void (*)())Func_02003820, FIELD_AT(FIELD_AT(r5, s32, 20), s32, 8),
        (FIELD_AT(FIELD_AT(r5, s32, 20), s32, 12)) + 983040,
        FIELD_AT(FIELD_AT(r5, s32, 20), s32, 16), 16);
  FIELD_AT(FIELD_AT(r5, s32, 20), s32, 8) = 0;
  FIELD_AT(FIELD_AT(r5, s32, 20), s32, 12) = 0;
  FIELD_AT(FIELD_AT(r5, s32, 20), s32, 16) = 0;
  stack[1] = 17;
  r5 = 42;
  Call6((void (*)())Func_02003aa4, 0, 32, 5, 8, r5, 17);
  Call6((void (*)())Func_02003aa4, 0, 96, 5, 8, r5, 81);
  r5 = 5;
  Call6((void (*)())Func_02003a94, 0, 32, 42, 17, r5, r6);
  v2 = Func_02003a94(0, 96, 42, 81, r5, r6);
  r0 = v2;
L_0200a1ce:
  r3 = 0x3e7;
  if (r7 != 0x3e7)
    goto L_0200a1dc;
  v3 = Func_02003998(r0, r1, r2);
  r0 = v3;
L_0200a1dc:
  /* add (s32)stack, #8 */
  return r0;
}

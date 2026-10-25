// Draft, differ 158 of 235: translated from its listing; names and structure still
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

s32 Object_GetById();
s32 Func_02002dfc();
void GameFlag_SetBit();

s32 Func_02000850(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r5, v0, r7, r0, v1, r6, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11;
  s32 stack[16];

  r5 = a1;
  /* sub (s32)stack, #8 */
  v0 = Object_GetById(r5);
  r7 = v0;
  r0 = v0;
  if (r5 != 8)
    goto L_02008924;
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) == (189)) {
    if (((FIELD_AT(r7, s32, 8)) >> 20) == 4) {
      Call6((void (*)())Func_02002dfc, 4, 68, 1, 1, 2, 69);
      Call1((void (*)())GameFlag_SetBit, 0x9c6);
    }
    Call6((void (*)())Func_02002dfc, 3, 68, 1, 1, 4, 69);
    v1 = Func_02002dfc(3, 69, 1, 1, 3, 70);
    r0 = v1;
  }
  if (r5 != 8)
    goto L_02008924;
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) != (191))
    goto L_02008924;
  r6 = 33;
  Call6((void (*)())Func_02002dfc, 32, 71, 1, 1, r6, 71);
  v2 = Func_02002dfc(31, 70, 1, 1, 32, 70);
  r0 = v2;
  if (((FIELD_AT(r7, s32, 8)) >> 20) != 32)
    goto L_02008924;
  Call6((void (*)())Func_02002dfc, 32, 69, 1, 1, r6, 69);
  Call1((void (*)())GameFlag_SetBit, 0x9c7);
  r0 = v3;
L_02008924:
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) != (192))
    goto L_020089da;
  if (r5 != 8)
    goto L_02008960;
  v4 = Func_02002dfc(3, 114, 1, 1, 3, 113);
  r0 = v4;
  if (((FIELD_AT(r7, s32, 8)) >> 20) != 2)
    goto L_02008960;
  Call1((void (*)())GameFlag_SetBit, 0x9c8);
  r0 = v5;
L_02008960:
  if (r5 != 18)
    goto L_0200898a;
  v6 = Func_02002dfc(40, 94, 1, 1, 40, 93);
  r0 = v6;
  if (((FIELD_AT(r7, s32, 8)) >> 20) != 39)
    goto L_0200898a;
  Call1((void (*)())GameFlag_SetBit, 0x9c9);
  r0 = v7;
L_0200898a:
  if (r5 != 19)
    goto L_020089da;
  Call6((void (*)())Func_02002dfc, 36, 72, 1, 1, 38, 72);
  r6 = 37;
  v8 = Func_02002dfc(37, 74, 1, 1, r6, 73);
  r0 = v8;
  if (((FIELD_AT(r7, s32, 8)) >> 20) != 38)
    goto L_020089da;
  Call6((void (*)())Func_02002dfc, 37, 70, 1, 1, r6, 71);
  Call1((void (*)())GameFlag_SetBit, 0x9ca);
  r0 = v9;
L_020089da:
  if (r5 != 8)
    goto L_02008a4e;
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) != (193))
    goto L_02008a4e;
  r5 = 83;
  Call6((void (*)())Func_02002dfc, 16, 83, 1, 1, 17, r5);
  r6 = 18;
  Call6((void (*)())Func_02002dfc, 19, 82, 1, 1, r6, 82);
  v10 = Func_02002dfc(19, 82, 1, 1, 19, r5);
  r0 = v10;
  if (((FIELD_AT(r7, s32, 8)) >> 20) != 17)
    goto L_02008a4e;
  Call6((void (*)())Func_02002dfc, 16, 84, 1, 1, r6, 84);
  Call1((void (*)())GameFlag_SetBit, 0x9cb);
  r0 = v11;
L_02008a4e:
  /* add (s32)stack, #8 */
  return r0;
}

// Draft, differ 78 of 168: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "lib/call.h"
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

s32 Func_02002dfc();
s32 GameFlag_Test(s32 flag);
s32 Func_02002df4();

s32 Func_020006d0(s32 a0, s32 a1) {
  s32 r6, r0, v0, v1, v2, v3, v4, r5, v5, v6, v7;
  s32 stack[2];

  r6 = a1;
  /* sub sp, #8 */
  r0 = a0;
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) != (189))
    goto L_02008714;
  if (r6 != 8)
    goto L_02008714;
  Call6((void (*)())Func_02002dfc, 4, 68, 1, 1, 4, 69);
  v0 = Func_02002dfc(3, 68, 1, 1, 3, 70);
  r0 = v0;
L_02008714:
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) != (192))
    goto L_02008782;
  if (r6 != 19)
    goto L_02008752;
  Call6((void (*)())Func_02002dfc, 35, 72, 1, 1, 38, 72);
  v1 = Func_02002dfc(36, 72, 1, 1, 37, 73);
  r0 = v1;
L_02008752:
  if (r6 != 8)
    goto L_0200876a;
  v2 = Func_02002dfc(2, 113, 1, 1, 3, 113);
  r0 = v2;
L_0200876a:
  if (r6 != 18)
    goto L_02008782;
  v3 = Func_02002dfc(39, 93, 1, 1, 40, 93);
  r0 = v3;
L_02008782:
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) != (191))
    goto L_020087c0;
  if (r6 != 8)
    goto L_020087c0;
  Call6((void (*)())Func_02002dfc, 32, 69, 1, 1, 32, 70);
  v4 = Func_02002dfc(33, 69, 1, 1, 33, 71);
  r0 = v4;
L_020087c0:
  if ((FIELD_AT((s32)(s32)&gPartyState, s16, 480)) != (193))
    goto L_02008836;
  if (r6 != 8)
    goto L_02008810;
  stack[0] = 17;
  r5 = 83;
  Call6((void (*)())Func_02002dfc, 17, 85, 1, 1, 17, r5);
  Call6((void (*)())Func_02002dfc, 19, 84, 1, 1, 18, 82);
  v5 = Func_02002dfc(19, 84, 1, 1, 19, r5);
  r0 = v5;
L_02008810:
  if (r6 != 9)
    goto L_02008836;
  v6 = GameFlag_Test(0x205);
  r0 = v6;
  if (v6 == 0)
    goto L_02008836;
  v7 = Func_02002df4(35, 98, 3, 5, 25, 98);
  r0 = v7;
L_02008836:
  /* add sp, #8 */
  return r0;
}

// Draft, differ 111 of 82: translated from its listing; names and structure
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

s32 Random16Far();
s32 Func_0200015c();
void FieldEvent_WaitFrames();

s32 Func_02000b78(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r11, r9, r8, r0, r3, r10, r6, r7, v0, r5, v1, v2;
  s32 stack[17];

  /* sub (s32)stack, #68 */
  stack[5] = a2;
  stack[4] = a3;
  stack[6] = a1;
  FIELD_AT((s32)stack, s32, 32) = 1;
  FIELD_AT((s32)stack, s32, 36) = 0xc000;
  FIELD_AT((s32)stack, s32, 40) = 0xc000;
  r11 = a0;
  r9 = 0;
  r8 = 28 + (s32)stack;
  r0 = a0;
  r3 = 0;
  if (r9 >= r11)
    goto L_02008c16;
  r10 = stack[25];
L_02008bae:
  r3 = stack[26];
  if (stack[26] <= 0)
    goto L_02008c04;
  r6 = r3;
  r7 = stack[5] << 16;
L_02008bba:
  v0 = Random16Far();
  r5 = 15;
  r5 = r5 & v0;
  v1 = Random16Far();
  stack[0] = 0;
  stack[1] = 0x100;
  r5 = r5 - 8;
  r5 = r5 << 16;
  r5 = r7 + r5;
  Func_0200015c(r5, (stack[4] + (31 & v1)) << 16, r10 << 16, 0, 0, 0x100,
                0x80001, r8);
  r6 = r6 - 1;
  r7 = r7 + 1048576;
  if (r6 != 0)
    goto L_02008bba;
L_02008c04:
  FieldEvent_WaitFrames(stack[6]);
  r9 = r9 + 1;
  r10 = r10 + 2;
  r0 = v2;
  r3 = 2;
  if (r9 < r11)
    goto L_02008bae;
L_02008c16:
  /* add (s32)stack, #68 */
  return r0;
}

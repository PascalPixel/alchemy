// Draft, differ 111 of 92: translated from its listing; names and structure
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

extern u8 gData_02003de4[];
s32 Object_GetById();
s32 Object_FindInCell();
s32 Func_0200074c();
s32 Func_020005cc();
s32 Func_02003aa4();
s32 Func_020003b4();
s32 Func_020006cc();
void GameFlag_SetBit();

s32 Func_02001e08(void) {
  s32 v0, r10, r6, v1, r5, r7, r8, v2, v3, r0, r3, v4;
  s32 stack[5];

  /* sub (s32)stack, #20 */
  v0 = Object_GetById(FIELD_AT((s32)(s32)&gPartyState, s32, 532));
  r10 = 8 + (s32)stack;
  FIELD_AT(r10, s32, 0) = (FIELD_AT(v0, s32, 8)) + 1048576;
  r6 = 79;
  FIELD_AT(r10, s32, 4) = FIELD_AT(v0, s32, 12);
  FIELD_AT(r10, s32, 8) = FIELD_AT(v0, s32, 16);
  v1 = Object_FindInCell(r10, v0);
  r5 = v1;
  Func_0200074c();
  r7 = r5;
  r7 = r7 + 34;
  Func_020005cc(FIELD_AT(r7, u8, 0), (FIELD_AT(r5, s32, 8)) >> 20,
                (FIELD_AT(r5, s32, 16)) >> 20, (FIELD_AT(r5, s32, 20)) >> 19);
  stack[1] = 36;
  r8 = 36;
  v2 = Func_02003aa4(79, 46, 2, 1, r6, 36);
  Func_020003b4();
  FIELD_AT(r7, u8, 0) = 0;
  Call6((void (*)())Func_02003aa4, 79, 37, 2, 1, r6, r8);
  Func_020005cc(FIELD_AT(r7, u8, 0), (FIELD_AT(r5, s32, 8)) >> 20,
                (FIELD_AT(r5, s32, 16)) >> 20,
                ((FIELD_AT(r5, s32, 20)) >> 19) + 4);
  v3 = Func_020006cc((s32)gData_02003de4);
  FIELD_AT(r10, s32, 0) = (FIELD_AT(r5, s32, 8)) >> 20;
  FIELD_AT(r10, s32, 8) = (FIELD_AT(r5, s32, 16)) >> 20;
  r0 = v3;
  r3 = (FIELD_AT(r5, s32, 16)) >> 20;
  if (((FIELD_AT(r5, s32, 8)) >> 20) != 16)
    goto L_02009eca;
  if (r3 != 36)
    goto L_02009eca;
  Call1((void (*)())GameFlag_SetBit, 0x8d9);
  r0 = v4;
L_02009eca:
  /* add (s32)stack, #20 */
  return r0;
}

// Draft, differ 110 of 90: translated from its listing; names and structure
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

extern u8 gData_02003de8[];
void *Object_GetById(s32 id);
struct ObjectRuntime *Object_FindInCell(s32 *position,
                                        struct ObjectRuntime *self);
s32 Func_0200074c();
s32 Func_020005cc();
s32 Func_02003aa4();
s32 Func_020003b4();
s32 Func_020006cc();
void GameFlag_SetBit(s32 flag);

s32 Func_02001edc(void) {
  s32 v0, r10, r6, v1, r5, r7, r8, v2, v3, r0, r3, v4;
  s32 stack[5];

  /* sub sp, #20 */
  v0 = Object_GetById(FIELD_AT((s32)(s32)&gPartyState, s32, 532));
  r10 = (s32)((u8 *)stack + 8);
  FIELD_AT(r10, s32, 0) = (FIELD_AT(v0, s32, 8)) - 1048576;
  r6 = 99;
  FIELD_AT(r10, s32, 4) = FIELD_AT(v0, s32, 12);
  FIELD_AT(r10, s32, 8) = FIELD_AT(v0, s32, 16);
  v1 = Object_FindInCell(r10, v0);
  r5 = v1;
  Func_0200074c();
  r7 = r5;
  r7 = r7 + 34;
  Func_020005cc(FIELD_AT(r7, u8, 0), (FIELD_AT(r5, s32, 8)) >> 20,
                (FIELD_AT(r5, s32, 16)) >> 20, (FIELD_AT(r5, s32, 20)) >> 19);
  stack[1] = 38;
  r8 = 38;
  v2 = Func_02003aa4(94, 44, 5, 1, r6, 38);
  Func_020003b4();
  FIELD_AT(r7, u8, 0) = 0;
  Call6((void (*)())Func_02003aa4, 99, 42, 5, 1, r6, r8);
  Func_020005cc(FIELD_AT(r7, u8, 0), (FIELD_AT(r5, s32, 8)) >> 20,
                (FIELD_AT(r5, s32, 16)) >> 20,
                ((FIELD_AT(r5, s32, 20)) >> 19) + 4);
  v3 = Func_020006cc((s32)gData_02003de8);
  FIELD_AT(r10, s32, 0) = (FIELD_AT(r5, s32, 8)) >> 20;
  FIELD_AT(r10, s32, 8) = (FIELD_AT(r5, s32, 16)) >> 20;
  r0 = v3;
  r3 = (FIELD_AT(r5, s32, 16)) >> 20;
  if (((FIELD_AT(r5, s32, 8)) >> 20) != 35)
    goto L_02009f9a;
  if (r3 != 38)
    goto L_02009f9a;
  Call1((void (*)())GameFlag_SetBit, 0x8db);
  r0 = v4;
L_02009f9a:
  /* add sp, #20 */
  return r0;
}

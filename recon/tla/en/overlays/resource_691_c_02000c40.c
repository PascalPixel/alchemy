// Draft, differ 101 of 113: translated from its listing; names and structure still
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

extern u8 gFrameCount[];
extern u8 gData_02003d74[];
extern u8 Func_02000dc0[];
s32 Random16Far();
void Func_0200015c();
void Object_SetPartAttribute();

s32 Func_02000c40(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r7, r5, r10, r3, r6, r9, r8, v0, v1, r2, v2, r0, v3, v4, v5;
  s32 stack[14];

  r7 = a0;
  /* sub (s32)stack, #56 */
  r5 = FIELD_AT(r7, s32, 104);
  r10 = 100 + r7;
  r3 = FIELD_AT(r7, s16, 100);
  if ((FIELD_AT(r7, s16, 100)) > 49)
    goto L_02008cc2;
  r6 = FIELD_AT((s32)gFrameCount, s32, 0);
  r9 = (s32)gFrameCount;
  r6 = r6 & 3;
  if (r6 == 0) {
    r8 = (s32)((u8 *)stack + 16);
    FIELD_AT(r8, u16, 24) = 0x193;
    FIELD_AT(r8, s32, 0) = 1;
    r5 = 15;
    FIELD_AT(r8, s32, 28) = (s32)gData_02003d74;
    v0 = Random16Far();
    r5 = r5 & v0;
    v1 = Random16Far();
    r5 = r5 - 8;
    r5 = r5 << 16;
    Func_0200015c((FIELD_AT(r7, s32, 8)) + r5,
                  ((FIELD_AT(r7, s32, 12)) + ((31 & v1) << 16)) - 524288,
                  FIELD_AT(r7, s32, 16), 0, r6, r6, 0x320001, r8);
  }
  r3 = FIELD_AT(r9, s32, 0);
  r2 = 4;
  goto L_02008cd6;
L_02008cc2:
  if (r3 <= 99) {
    r2 = 4;
    r3 = (s32)gFrameCount;
  } else {
    if (r3 > 139)
      goto L_02008cf0;
    r2 = 2;
    r3 = (s32)gFrameCount;
  }
  r3 = FIELD_AT(r3, s32, 0);
L_02008cd6:
  if ((r3 & r2) == 0)
    goto L_02008ce6;
  Value2((s32 (*)())Object_SetPartAttribute, r7, 10);
  r0 = v2;
  goto L_02008d1a;
L_02008ce6:
  Value2((s32 (*)())Object_SetPartAttribute, r7, 0);
  r0 = v3;
  goto L_02008d1a;
L_02008cf0:
  if (r3 > 143)
    goto L_02008cfe;
  Value2((s32 (*)())Object_SetPartAttribute, r7, 7);
  r0 = v4;
  goto L_02008d1a;
L_02008cfe:
  Value2((s32 (*)())Object_SetPartAttribute, r7, 0);
  FIELD_AT(r7, s32, 108) = 0;
  FIELD_AT(r5, s32, 8) = FIELD_AT(r7, s32, 8);
  FIELD_AT(r5, s32, 12) = FIELD_AT(r7, s32, 12);
  FIELD_AT(r5, s32, 16) = FIELD_AT(r7, s32, 16);
  FIELD_AT(r5, s32, 108) = (s32)Func_02000dc0;
  r0 = v5;
L_02008d1a:
  FIELD_AT(r10, u16, 0) = (FIELD_AT(r10, u16, 0)) + 1;
  /* add (s32)stack, #56 */
  return r0;
}

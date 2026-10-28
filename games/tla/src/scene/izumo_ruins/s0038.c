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

s32 Actor_GetById();
void Actor_SetSingleChildFlags();
s32 Func_02000128();

s32 Func_02000038(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r0, r3, r7, r8, v0, r5, r6, r4, v1;
  s32 stack[1];

  /* sub (s32)stack, #4 */
  r0 = a0;
  r3 = FIELD_AT(a0, u16, 0);
  if ((FIELD_AT(a0, u16, 0)) == 0xffff)
    goto L_0200809c;
  r7 = r0;
L_0200804e:
  r8 = FIELD_AT(r7, u16, 0);
  v0 = Actor_GetById(r8);
  r5 = v0;
  r6 = *(s16 *)(r7 + 2);
  r4 = *(s16 *)(r7 + 4);
  FIELD_AT(r5, u8, 89) = 254 & (FIELD_AT(r5, u8, 89));
  stack[0] = r4;
  Call2((void (*)())Actor_SetSingleChildFlags, v0, 0);
  r4 = stack[0];
  r6 = r6 << 16;
  r4 = r4 << 16;
  r4 = (u32)r4 >> 16;
  r6 = (u32)r6 >> 16;
  r5 = r5 + 34;
  v1 = Func_02000128(r8, r6, r4, FIELD_AT(r5, u8, 0));
  r7 = r7 + 6;
  r0 = v1;
  r3 = FIELD_AT(r7, u16, 0);
  if ((FIELD_AT(r7, u16, 0)) != 0xffff)
    goto L_0200804e;
L_0200809c:
  /* add (s32)stack, #4 */
  return r0;
}

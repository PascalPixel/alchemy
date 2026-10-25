// Draft, differ 124 of 104: translated from its listing; names and structure
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

void *Object_GetById(s32 id);
u32 Random16Far(void);
s32 Func_020000b8();
void GameFlag_SetBit(s32 flag);
s32 Func_02000338();
s32 Func_02002fc0();

s32 Func_02000a48(s32 a0, s32 a1) {
  s32 r6, r10, r9, v0, r7, r0, r5, r8, v1, v2, r4, v3, r3, v4, v5;
  s32 stack[14];

  r6 = a1;
  r10 = a0;
  /* sub sp, #56 */
  r9 = FIELD_AT(50331872, s32, 0);
  v0 = Object_GetById(*(s16 *)((FIELD_AT(50331872, s32, 0)) + 26));
  r7 = v0;
  r0 = v0;
  if (r6 > 7)
    goto L_02008adc;
  r5 = (s32)((u8 *)stack + 16);
  FIELD_AT(r5, u16, 24) = 0x117;
  FIELD_AT(r5, s32, 12) = 0x10000;
  FIELD_AT(r5, s32, 8) = 0x10000;
  r8 = 0;
  FIELD_AT(r5, s32, 0) = 0;
  FIELD_AT(r5, s32, 20) = 0x8000;
  FIELD_AT(r5, s32, 16) = 0x8000;
  v1 = Random16Far();
  FIELD_AT(r5, u16, 32) = (v1 & (15)) << 12;
  v2 = Random16Far();
  r4 = 128;
  r4 = r4 << 13;
  r4 = r8;
  stack[0] = r4;
  stack[1] = r4;
  r4 = 180;
  r4 = r4 << 15;
  v3 = Func_020000b8(FIELD_AT(r7, s32, 8),
                     ((FIELD_AT(r7, s32, 12)) - (((r6 << 1) + r6) << 16)) + r4,
                     FIELD_AT(r7, s32, 16), ((15 & v2) - 8) << 13, r4, r4, r4,
                     r5);
  r0 = v3;
  goto L_02008ad4;
L_02008ad4:
  FIELD_AT(r7, s32, 28) = (FIELD_AT(r7, s32, 28)) - 7936;
  r3 = (FIELD_AT(r7, s32, 28)) - 7936;
L_02008adc:
  if (r10 != 1)
    goto L_02008b22;
  FIELD_AT(r7, s32, 28) = 0x10000;
  r4 = 0;
  GameFlag_SetBit(*(s16 *)((r7 + 100) + r4));
  v4 = Object_GetById(*(s16 *)(r9 + 26));
  Func_02000338();
  r4 = r10;
  v5 = Func_02002fc0(67, 23, ((FIELD_AT(r7, s32, 8)) >> 20) + 64,
                     (FIELD_AT(r7, s32, 16)) >> 20, r4, r4);
  FIELD_AT(r7, s32, 16) = 0;
  FIELD_AT(r7, s32, 12) = 0;
  FIELD_AT(r7, s32, 8) = 0;
  r0 = v5;
  r3 = 0;
L_02008b22:
  /* add sp, #56 */
  return r0;
}

// Draft, differ 97 of 88: translated from its listing; names and structure
// still to do. Measured against its copies: overlay 691's Func_02003820 (differ
// 97 of 88).
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

u32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle,
                              union FieldCoordinate *position);

s32 Func_02002d44(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r11, r0, r3, r2, r7, v0, r10, r6, r5, r8, r9, v1, v2, v3;
  s32 stack[3];

  /* sub sp, #12 */
  stack[0] = a2;
  stack[2] = a0;
  stack[1] = a1;
  r11 = FIELD_AT(50331868, s32, 0);
  r0 = a0;
  r3 = FIELD_AT(50331868, s32, 0);
  r2 = a3;
  if (a3 <= 0)
    goto L_0200adf0;
  r7 = r2;
L_0200ad6c:
  v0 = Random16Far();
  r10 = 5632 + r11;
  r6 = (FIELD_AT(r11, s16, 5632)) << 3;
  r6 = r6 - (FIELD_AT(r11, s16, 5632));
  r6 = r6 << 2;
  r6 = r6 + r11;
  r5 = r6 + 1280;
  FIELD_AT(r5, s32, 24) = 0;
  r8 = 0;
  FIELD_AT(r5, s32, 0) = stack[2];
  r9 = v0;
  FIELD_AT(r5, s32, 4) = stack[1];
  r7 = r7 - 1;
  FIELD_AT(r5, s32, 8) = stack[0];
  v1 = Random16Far();
  Vector_AddPolarOffsetFar((v1 << 3) + 524288, r9, r5);
  FIELD_AT(r5, s32, 12) = r8;
  FIELD_AT(r5, s32, 16) = 0x50000;
  FIELD_AT(r5, s32, 20) = r8;
  v2 = Random16Far();
  r6 = r6 + 1292;
  Vector_AddPolarOffsetFar((v2 << 1) + 131072, r9, r6);
  FIELD_AT(r10, u16, 0) = ((FIELD_AT(r10, u16, 0)) + 1) & 63;
  r0 = v3;
  r3 = ((FIELD_AT(r10, u16, 0)) + 1) & 63;
  if (r7 != 0)
    goto L_0200ad6c;
L_0200adf0:
  /* add sp, #12 */
  return r0;
}

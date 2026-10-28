// Draft, differ 80 of 96: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_0200308c[];
extern u8 Data_02003098[];
extern u8 Data_02002fd7[];
extern u8 gData_02002fe8[];
extern u8 Data_02003090[];
extern u8 Data_02003024[];
extern u8 Data_02003060[];
extern u8 Data_02003030[];
s32 __modsi3();
void Func_02002124();
s32 Func_020020d4();
s32 Func_0200210c();

s32 Func_020002b4(s32 a0, s32 a1, s32 a2) {
  s32 r1, r2, r3, r4, r6, v0, r5, v1, r0, v2, v3;
  s32 stack[3];

  /* sub (s32)stack, #12 */
  r1 = a1;
  r2 = a2;
  r3 = ((FIELD_AT((s32)gData_0200308c, u8, 0)) << 24) >> 24;
  if ((((FIELD_AT((s32)gData_0200308c, u8, 0)) << 24) >> 24) != 1)
    goto L_020082e8;
  r4 = (s32)Data_02003098;
  FIELD_AT(67108946, u16, 0) =
      ((*(u8 *)(((s32)Data_02002fd7) + (((FIELD_AT(r4, s32, 0)) >> 3) & 15)))
       << 8) |
      (16 -
       (*(u8 *)(((s32)Data_02002fd7) + (((FIELD_AT(r4, s32, 0)) >> 3) & 15))));
  FIELD_AT(r4, s32, 0) = (FIELD_AT(r4, s32, 0)) + 1;
  r1 = *(u8 *)(((s32)Data_02002fd7) + (((FIELD_AT(r4, s32, 0)) >> 3) & 15));
  r2 = ((*(u8 *)(((s32)Data_02002fd7) + (((FIELD_AT(r4, s32, 0)) >> 3) & 15)))
        << 8) |
       (16 -
        (*(u8 *)(((s32)Data_02002fd7) + (((FIELD_AT(r4, s32, 0)) >> 3) & 15))));
  goto L_02008338;
L_020082e8:
  if (r3 != 2)
    goto L_02008338;
  r6 = (s32)Data_02003098;
  v0 = __modsi3((FIELD_AT(r6, s32, 0)) >> 2, 28);
  r5 = v0;
  FIELD_AT(67108946, u16, 0) = *(u16 *)(((s32)gData_02002fe8) + (r5 << 1));
  r2 = *(u16 *)(((s32)gData_02002fe8) + (r5 << 1));
  if (r5 == 0) {
    FIELD_AT((s32)Data_02003090, s32, 0) = 1;
    FIELD_AT((s32)Data_02003024, s32, 0) = r5;
    Call1((void (*)())Func_02002124, 153);
  }
  if (r5 == 14) {
    FIELD_AT((s32)Data_02003090, s32, 0) = 0;
    FIELD_AT((s32)Data_02003024, s32, 0) = 1;
    Call1((void (*)())Func_02002124, 153);
  }
  FIELD_AT(r6, s32, 0) = (FIELD_AT(r6, s32, 0)) + 1;
L_02008338:
  r5 = (s32)stack;
  FIELD_AT(r5, s32, 0) = 0x3400000;
  FIELD_AT(r5, s32, 4) = 0;
  FIELD_AT(r5, s32, 8) = 0x1c00000;
  v1 = Func_020020d4(r5);
  r0 = v1;
  if ((FIELD_AT((s32)Data_02003090, s32, 0)) != 0) {
    FIELD_AT((s32)Data_02003060, s32, 12) = FIELD_AT(r5, s32, 0);
    FIELD_AT((s32)Data_02003060, s32, 16) = FIELD_AT(r5, s32, 8);
    v2 = Func_0200210c((s32)Data_02003060, r1);
    r0 = v2;
  }
  if ((FIELD_AT((s32)Data_02003024, s32, 0)) != 0) {
    FIELD_AT((s32)Data_02003030, s32, 12) = FIELD_AT(r5, s32, 0);
    FIELD_AT((s32)Data_02003030, s32, 16) = FIELD_AT(r5, s32, 8);
    v3 = Func_0200210c((s32)Data_02003030, r1);
    r0 = v3;
  }
  /* add (s32)stack, #12 */
  return r0;
}

// Draft, differ 55 of 95: translated from its listing; names and structure still
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

extern u8 gMapCellBuffer[];
s32 Object_GetById();
s32 GameFlag_Test();
s32 Func_02002e0c();

s32 Func_020010f4(s32 a0) {
  s32 v0, r7, r5, r6, r8, v1, r0, r1, r4, r3, v2;
  s32 stack[3];

  /* sub (s32)stack, #12 */
  v0 = Object_GetById(FIELD_AT((s32)(s32)&gPartyState, s32, 532));
  r7 = (s32)Ram_HeapSlots->eventWork;
  r5 = (s32)Ram_HeapSlots->mapWork;
  r6 = v0;
  r8 = FIELD_AT(r7, s32, 460);
  v1 = GameFlag_Test(0x104);
  r0 = v1;
  if (v1 != 0)
    goto L_020091b0;
  if (r5 == 0) {
    r1 = (s32)gMapCellBuffer;
    r0 = (s32)gMapCellBuffer;
  } else {
    r1 = FIELD_AT(r5, s32, 424);
    r0 = FIELD_AT(r5, s32, 312);
  }
  r4 = FIELD_AT(r6, s32, 8);
  r0 = r0 + (((r4 >> 20) + (((FIELD_AT(r6, s32, 16)) >> 20) << 7)) << 2);
  r3 = 232 &
       (FIELD_AT(
           r1 + (((r4 >> 20) + (((FIELD_AT(r6, s32, 16)) >> 20) << 7)) << 2),
           u8, 2));
  if (((4 &
        (FIELD_AT(
            r0 + (((r4 >> 20) + (((FIELD_AT(r6, s32, 16)) >> 20) << 7)) << 2),
            u8, 3))) |
       (232 &
        (FIELD_AT(
            r1 + (((r4 >> 20) + (((FIELD_AT(r6, s32, 16)) >> 20) << 7)) << 2),
            u8, 2)))) != 236)
    goto L_020091b0;
  r3 = FIELD_AT((s32)(s32)&gPartyState, u8, 530);
  if ((FIELD_AT((s32)(s32)&gPartyState, u8, 530)) == 5)
    goto L_020091b0;
  FIELD_AT((s32)stack, s32, 0) = r4;
  FIELD_AT((s32)stack, s32, 4) = FIELD_AT(r6, s32, 12);
  FIELD_AT((s32)stack, s32, 8) = (FIELD_AT(r6, s32, 16)) + 524288;
  v2 = Func_02002e0c(r6);
  r0 = v2;
  if (v2 < 0) {
    FIELD_AT(r7, u16, 340) = 66;
  }
  FIELD_AT(r6, s32, 16) = (FIELD_AT(r6, s32, 16)) + 32768;
  FIELD_AT(r8, s32, 16) = (FIELD_AT(r8, s32, 16)) + 32768;
  r3 = (FIELD_AT(r8, s32, 16)) + 32768;
L_020091b0:
  /* add (s32)stack, #12 */
  return r0;
}

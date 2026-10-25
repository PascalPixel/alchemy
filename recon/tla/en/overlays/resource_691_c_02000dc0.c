// Draft, differ 138 of 131: translated from its listing; names and structure still
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

extern u8 Func_02000c40[];
s32 Math_Cosine();
s32 Object_GetById();
void Func_02000d40();
s32 Func_020005a0();

s32 Func_02000dc0(s32 a0, s32 a1) {
  s32 r6_1, r5_1, r10_1, r8_1, r7_1, r5_2;
  s32 r6, v0, r5, r10, v1, r4, r8, v2, r7, r2, r3, r0, r1, v3;

  r6_1 = a0;
  v0 = Math_Cosine(FIELD_AT(r6_1, u16, 6));
  r5_1 = v0;
  r10_1 = 98 + r6_1;
  v1 = Object_GetById((FIELD_AT(r6_1, s16, 102)) + (FIELD_AT(r6_1, u8, 98)));
  r4 = 133;
  r4 = r4 << 2;
  r8_1 = v1;
  v2 = Object_GetById(FIELD_AT((s32)(s32)&gPartyState + r4, s32, 0));
  r5_1 = r5_1 << 1;
  r7_1 = v2;
  FIELD_AT(r6_1, s32, 8) = (FIELD_AT(r6_1, s32, 8)) + r5_1;
  Func_02000d40(r6_1);
  r4 = 192;
  r4 = r4 << 11;
  r2 = (FIELD_AT(r7_1, s32, 8)) - 524288;
  r3 = (FIELD_AT(r6_1, s32, 8)) + r4;
  r0 = FIELD_AT(r7_1, s32, 8);
  r1 = FIELD_AT(r6_1, s32, 8);
  if (((FIELD_AT(r7_1, s32, 8)) - 524288) >= ((FIELD_AT(r6_1, s32, 8)) + r4))
    goto L_02008e7a;
  r4 = 128;
  r4 = r4 << 12;
  r2 = r1 - 393216;
  r3 = r0 + r4;
  if ((r1 - 393216) >= (r0 + r4))
    goto L_02008e7a;
  r4 = 192;
  r4 = r4 << 11;
  r2 = (FIELD_AT(r7_1, s32, 16)) - 524288;
  r3 = (FIELD_AT(r6_1, s32, 16)) + r4;
  r0 = FIELD_AT(r7_1, s32, 16);
  r1 = FIELD_AT(r6_1, s32, 16);
  if (((FIELD_AT(r7_1, s32, 16)) - 524288) >= ((FIELD_AT(r6_1, s32, 16)) + r4))
    goto L_02008e7a;
  r4 = 128;
  r4 = r4 << 12;
  r2 = r1 - 393216;
  r3 = r0 + r4;
  if ((r1 - 393216) >= (r0 + r4))
    goto L_02008e7a;
  r2 = FIELD_AT(r6_1, s32, 12);
  r3 = (FIELD_AT(r6_1, s32, 12)) + r4;
  r0 = FIELD_AT(r7_1, s32, 12);
  if ((FIELD_AT(r7_1, s32, 12)) >= ((FIELD_AT(r6_1, s32, 12)) + r4))
    goto L_02008e7a;
  r4 = 240;
  r4 = r4 << 12;
  r2 = r2 - 524288;
  r3 = r0 + r4;
  if ((r2 - 524288) >= (r0 + r4))
    goto L_02008e7a;
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 362) = 200;
  FIELD_AT(r6_1, s32, 108) = 0;
  FIELD_AT(r6_1, s32, 8) = 0;
  FIELD_AT(r6_1, s32, 12) = 0;
  FIELD_AT(r6_1, s32, 16) = 0;
  r2 = 0;
  r3 = 200;
L_02008e7a:
  r5_2 = r6_1;
  r5_2 = r5_2 + 8;
  v3 = Func_020005a0(r5_2, r8_1 + 8);
  r0 = v3;
  if (v3 > 8)
    goto L_02008edc;
  r4 = r8_1;
  r3 = (FIELD_AT(r4, s32, 12)) >> 20;
  if (((FIELD_AT(r6_1, s32, 12)) >> 20) != ((FIELD_AT(r4, s32, 12)) >> 20))
    goto L_02008edc;
  FIELD_AT(r5_2, s32, 0) = 0;
  FIELD_AT(r6_1, s32, 108) = 0;
  FIELD_AT(r6_1, u16, 6) = (FIELD_AT(r6_1, u16, 6)) ^ (36864);
  FIELD_AT(r6_1, s32, 12) = 0;
  FIELD_AT(r6_1, s32, 16) = 0;
  r4 = r10_1;
  FIELD_AT(r4, u8, 0) = (FIELD_AT(r10_1, u8, 0)) ^ 1;
  FIELD_AT(r8_1, u16, 100) = 0;
  FIELD_AT(r8_1, s32, 108) = (s32)Func_02000c40;
  FIELD_AT(r8_1, s32, 104) = r6_1;
  r3 = (s32)Func_02000c40;
  goto L_02008edc;
L_02008edc:
  return r0;
}

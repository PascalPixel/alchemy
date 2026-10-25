// Draft, differ 46 of 78: loads resource 0x1d9's picture into the menu work,
// moves its colour-15 pixels to palette 6, uploads it to a free video slot and
// sets up its sprite. Translated from its listing; names and structure still to
// do.
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

s32 Resource_GetTableEntry();
void Func_020033dc();
s32 Resource_FindFreeEntry();
s32 VramCache_Load();
s32 Func_020035fc();

s32 Func_02002c88(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r7, v0, r2, r0, r1, v1, r5, v2, v3, r6;
  s32 stack[1];

  r7 = FIELD_AT(50331868, s32, 0);
  /* sub (s32)stack, #4 */
  v0 = Resource_GetTableEntry(473);
  Func_020033dc(v0, r7);
  r2 = r7;
  r0 = 0;
L_0200aca4:
  r1 = FIELD_AT(r2, u8, 0);
  if ((240 & (FIELD_AT(r2, u8, 0))) == 240) {
    FIELD_AT(r2, u8, 0) = (15 & r1) + 96;
  }
  r2 = r2 + 1;
  r0 = r0 + 1;
  if ((r0 + 1) < 0x1000)
    goto L_0200aca4;
  v1 = Resource_FindFreeEntry();
  r5 = v1;
  v2 = VramCache_Load(v1, 0x1000, r7);
  FIELD_AT(r7, u16, 4188) = r5;
  FIELD_AT(r7, u16, 4190) = v2;
  r5 = r7 + 4108;
  v3 = Func_020035fc(r5, 31, 31, -0x40000000, v2);
  FIELD_AT(r5, u8, 5) = (FIELD_AT(r5, u8, 5)) | 32;
  FIELD_AT(r5, u8, 9) = ((15 & (FIELD_AT(r5, u8, 9))) & -13) | 8;
  FIELD_AT(r5, u16, 30) = 1;
  r6 = 0;
  FIELD_AT(r7, s32, 4196) = r6;
  FIELD_AT(r7, s32, 4200) = r6;
  /* add (s32)stack, #4 */
  return v3;
}

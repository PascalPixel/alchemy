// Draft, differ 135 of 112: translated from its listing; names and structure still
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

extern u8 Func_020015e4[];
extern u8 Func_02001528[];
s32 Object_GetById();
s32 Engine_ObjectCreate();
void ObjectDispatch_SetSingleChildFlags();
void Func_02003a54();
void Object_SetPartAttribute();
void Object_SetSpritePriority();

s32 Func_02002518(s32 a0, s32 a1) {
  s32 r5, v0, r0, r3, v1, r8, v2, r7, r10, r6, v3, v4;
  s32 stack[4];

  r5 = (u32)(a0 << 16) >> 16;
  /* sub (s32)stack, #16 */
  v0 = Object_GetById(r5);
  r0 = v0;
  r3 = (s32)Func_020015e4;
  if ((FIELD_AT(v0, s32, 108)) != ((s32)Func_020015e4))
    goto L_0200a604;
  v1 = Object_GetById(r5);
  r8 = FIELD_AT(v1, s32, 104);
  v2 = Engine_ObjectCreate(
      0x15f, (FIELD_AT(FIELD_AT(v1, s32, 104), s32, 8)) + 65536,
      (FIELD_AT(r8, s32, 12)) + 2097152, FIELD_AT(r8, s32, 16));
  r7 = (s32)stack;
  FIELD_AT(r7, s32, 0) = v2;
  r10 = 1;
L_0200a562:
  r6 = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 80);
  v3 = Engine_ObjectCreate(0x15f, 0, 0, 0);
  r5 = r10 << 2;
  FIELD_AT(FIELD_AT(v3, s32, 80), u8, 16) = FIELD_AT(r6, u8, 16);
  *(s32 *)(r7 + r5) = v3;
  FIELD_AT(FIELD_AT(v3, s32, 80), u8, 17) =
      (FIELD_AT(FIELD_AT(v3, s32, 80), u8, 17)) | 1;
  r6 = 0;
  FIELD_AT(v3, s32, 8) = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 8);
  FIELD_AT(v3, s32, 12) = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 12);
  FIELD_AT(v3, s32, 16) = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 16);
  FIELD_AT(v3, u8, 85) = r6;
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v3, 0);
  Call2((void (*)())Func_02003a54, *(s32 *)(r7 + r5), 3);
  r5 = (s32)Func_02001528;
  FIELD_AT((*(s32 *)(r7 + r5)), u8, 98) = r10 + 255;
  FIELD_AT(*(s32 *)(r7 + r5), s32, 104) = r8;
  FIELD_AT(*(s32 *)(r7 + r5), s32, 108) = r5;
  Call2((void (*)())Object_SetPartAttribute, *(s32 *)(r7 + r5), 3);
  r10 = r10 + 1;
  if (r10 != 4)
    goto L_0200a562;
  FIELD_AT((FIELD_AT(r7, s32, 0)), u8, 85) = r6;
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, FIELD_AT(r7, s32, 0),
        0);
  Call2((void (*)())Func_02003a54, FIELD_AT(r7, s32, 0), 3);
  Call2((void (*)())Object_SetSpritePriority, FIELD_AT(r7, s32, 0), 3);
  FIELD_AT((FIELD_AT(r7, s32, 0)), u8, 98) = 3;
  FIELD_AT(FIELD_AT(r7, s32, 0), s32, 104) = r8;
  FIELD_AT(FIELD_AT(r7, s32, 0), s32, 108) = r5;
  Call2((void (*)())Object_SetPartAttribute, FIELD_AT(r7, s32, 0), 3);
  r0 = v4;
L_0200a604:
  /* add (s32)stack, #16 */
  return r0;
}

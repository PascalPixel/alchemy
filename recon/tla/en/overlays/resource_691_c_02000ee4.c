// Draft, differ 103 of 104: translated from its listing; names and structure
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

extern u8 Func_02000dc0[];
void *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);

s32 Func_02000ee4(s32 a0, s32 a1, s32 a2) {
  s32 r8, r9, v0, r6, v1, r11, r0, r7, r5, r10, v2, v3, v4, v5, v6, r3, v7, v8;

  r8 = (a1 << 16) >> 16;
  r9 = (a2 << 16) >> 16;
  v0 = Object_GetById((u32)(a0 << 16) >> 16);
  r6 = v0;
  v1 = GameFlag_Test(0x109);
  r11 = v1;
  r0 = v1;
  if (v1 != 0)
    goto L_02008fbc;
  r7 = (u32)(r8 << 16) >> 16;
  r5 = r7 + ((u32)(r9 << 16) >> 16);
  r10 = (u32)(r9 << 16) >> 16;
  v2 = Object_GetById(r5);
  FIELD_AT(r6, s32, 8) = FIELD_AT(v2, s32, 8);
  v3 = Object_GetById(r5);
  FIELD_AT(r6, s32, 12) = FIELD_AT(v3, s32, 12);
  v4 = Object_GetById(r5);
  FIELD_AT(r6, s32, 16) = FIELD_AT(v4, s32, 16);
  Object_SetSpritePriority(r6, 2);
  FIELD_AT(r6, u16, 102) = r8;
  FIELD_AT(r6, u8, 98) = r9 ^ 1;
  FIELD_AT(r6, s32, 108) = (s32)Func_02000dc0;
  r8 = r7 + 1;
  r9 = 85 + r6;
  if (r10 == 0)
    goto L_02008f96;
  v5 = Object_GetById(r7);
  r5 = v5;
  v6 = Object_GetById(r8);
  r0 = v6;
  if ((FIELD_AT(r5, s32, 8)) >= (FIELD_AT(v6, s32, 8)))
    goto L_02008f90;
  r3 = 0x8000;
  goto L_02008fb4;
L_02008f90:
  FIELD_AT(r6, u16, 6) = r11;
  goto L_02008fb6;
L_02008f96:
  v7 = Object_GetById(r7);
  r5 = v7;
  v8 = Object_GetById(r8);
  r0 = v8;
  if ((FIELD_AT(r5, s32, 8)) <= (FIELD_AT(v8, s32, 8)))
    goto L_02008fb2;
  r3 = 0x8000;
  goto L_02008fb4;
L_02008fb2:
  r3 = r10;
L_02008fb4:
  FIELD_AT(r6, u16, 6) = r3;
L_02008fb6:
  FIELD_AT(r9, u8, 0) = 0;
  r3 = 0;
L_02008fbc:
  return r0;
}

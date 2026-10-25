// Draft, differ 348 of 294: translated from its listing; names and structure
// still to do.
#include "types.h"
#include "lib/call.h"
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

extern u8 gFrameCount[];
extern u8 Func_020011b8[];
extern u8 gData_02003d50[];
extern u8 Func_02001528[];
extern u8 gData_02003d9e[];
extern u8 gData_02003db2[];
extern u8 gData_02003dc4[];
extern u8 gData_02003dd6[];
extern u8 gData_02003dee[];
extern u8 Func_020015e4[];
s32 Func_02003c4c();
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
u32 Random16Far(void);
s32 Func_0200015c();
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
s32 Func_02003a54();
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
s32 Func_020010cc();
void *Object_GetById(s32 id);

s32 Func_02001738(s32 a0, s32 a1, s32 a2) {
  s32 r5, r9, r8, r10, r2, r3, v0, r11, r0, r7, r6, v1, v2, r4, v3, v4, v5, r1,
      v6, v7, v8, v9, v10, v11, v12;
  s32 stack[18];

  r5 = (s32)gFrameCount;
  r9 = (s32)Ram_HeapSlots->eventWork;
  r8 = a0;
  /* sub sp, #72 */
  r10 = 15;
  r2 = a2;
  r3 = (FIELD_AT(r5, s32, 0)) & 15;
  if (((FIELD_AT(r5, s32, 0)) & 15) != 0)
    goto L_02009768;
  Call2((void (*)())Func_02003c4c, r8, 136);
L_02009768:
  Call2((void (*)())Object_SetPartAttribute, r8, 10);
  r11 = 100 + r8;
  r2 = 0;
  r3 = FIELD_AT(r8, s16, 100);
  r0 = v0;
  if ((FIELD_AT(r8, s16, 100)) > 31)
    goto L_020097e6;
  r7 = FIELD_AT(r5, s32, 0);
  r7 = r7 & 3;
  if (r7 == 0)
    goto L_0200978a;
  goto L_02009954;
L_0200978a:
  r6 = (s32)((u8 *)stack + 32);
  FIELD_AT(r6, u16, 24) = 0x193;
  FIELD_AT(r6, s32, 4) = 10;
  FIELD_AT(r6, s32, 0) = 2;
  FIELD_AT(r6, s32, 36) = (s32)Func_020011b8;
  FIELD_AT(r6, s32, 28) = (s32)gData_02003d50;
  v1 = Random16Far();
  r5 = v1;
  r5 = r5 & r10;
  v2 = Random16Far();
  r4 = r10;
  r4 = FIELD_AT(r8, s32, 8);
  r4 = r4 + 65536;
  r5 = r5 - 8;
  r5 = r5 << 14;
  v3 = Func_0200015c(r4, (FIELD_AT(r8, s32, 12)) + 1572864,
                     FIELD_AT(r8, s32, 16), r5, r7, ((v2 & r4) - 8) << 14,
                     0x1330000, r6);
  r0 = v3;
  goto L_02009954;
L_020097e6:
  if (r3 == 33)
    goto L_020097ec;
  goto L_0200993e;
L_020097ec:
  Call2((void (*)())Func_02003c4c, r8, 144);
  r4 = r8;
  v4 = Engine_ObjectCreate(0x15f, (FIELD_AT(r4, s32, 8)) + 65536,
                           (FIELD_AT(r4, s32, 12)) + 2097152,
                           FIELD_AT(r4, s32, 16));
  r7 = (s32)((u8 *)stack + 16);
  FIELD_AT(r7, s32, 0) = v4;
  r4 = 1;
  r10 = r4;
L_02009818:
  r6 = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 80);
  v5 = Engine_ObjectCreate(0x15f, 0, 0, 0);
  r5 = r10 << 2;
  FIELD_AT(FIELD_AT(v5, s32, 80), u8, 16) = FIELD_AT(r6, u8, 16);
  *(s32 *)(r7 + r5) = v5;
  FIELD_AT(FIELD_AT(v5, s32, 80), u8, 17) =
      (FIELD_AT(FIELD_AT(v5, s32, 80), u8, 17)) | 1;
  r6 = 0;
  FIELD_AT(v5, s32, 8) = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 8);
  FIELD_AT(v5, s32, 12) = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 12);
  FIELD_AT(v5, s32, 16) = FIELD_AT(FIELD_AT(r7, s32, 0), s32, 16);
  FIELD_AT(v5, u8, 85) = r6;
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, v5, 0);
  Call2((void (*)())Func_02003a54, *(s32 *)(r7 + r5), 3);
  r5 = (s32)Func_02001528;
  FIELD_AT((*(s32 *)(r7 + r5)), u8, 98) = 3 - r10;
  FIELD_AT(*(s32 *)(r7 + r5), s32, 104) = r8;
  FIELD_AT(*(s32 *)(r7 + r5), s32, 108) = r5;
  Call2((void (*)())Object_SetPartAttribute, *(s32 *)(r7 + r5), 3);
  r4 = 1;
  r10 = r10 + r4;
  if (r10 != 4)
    goto L_02009818;
  FIELD_AT((FIELD_AT(r7, s32, 0)), u8, 85) = r6;
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, FIELD_AT(r7, s32, 0),
        0);
  Call2((void (*)())Func_02003a54, FIELD_AT(r7, s32, 0), 3);
  Call2((void (*)())Object_SetSpritePriority, FIELD_AT(r7, s32, 0), 3);
  FIELD_AT((FIELD_AT(r7, s32, 0)), u8, 98) = 3;
  FIELD_AT(FIELD_AT(r7, s32, 0), s32, 104) = r8;
  FIELD_AT(FIELD_AT(r7, s32, 0), s32, 108) = r5;
  Call2((void (*)())Object_SetPartAttribute, FIELD_AT(r7, s32, 0), 3);
  r4 = 0;
  r0 = 0;
  r1 = (s32)&gPartyState;
  r2 = *(s16 *)(((s32)(s32)&gPartyState + 480) + r4);
  r3 = 223;
  if ((*(s16 *)(((s32)(s32)&gPartyState + 480) + r4)) != (223))
    goto L_020098de;
  v6 = Func_020010cc(FIELD_AT(r8, s32, 8), FIELD_AT(r8, s32, 16),
                     (s32)gData_02003d9e);
  r0 = v6;
  goto L_0200991e;
L_020098de:
  if (r2 != (224))
    goto L_0200991e;
  r4 = 0;
  r0 = FIELD_AT(r8, s32, 8);
  r1 = FIELD_AT(r8, s32, 16);
  r3 = *(s16 *)((r1 + 482) + r4);
  if ((*(s16 *)((r1 + 482) + r4)) > 3)
    goto L_02009900;
  v7 = Func_020010cc(r0, r1, (s32)gData_02003db2);
  r0 = v7;
  goto L_0200991e;
L_02009900:
  if (r3 > 8)
    goto L_0200990c;
  v8 = Func_020010cc(r0, r1, (s32)gData_02003dc4);
  r0 = v8;
  goto L_0200991e;
L_0200990c:
  if (r3 > 12)
    goto L_02009918;
  v9 = Func_020010cc(r0, r1, (s32)gData_02003dd6);
  r0 = v9;
  goto L_0200991e;
L_02009918:
  v10 = Func_020010cc(r0, r1, (s32)gData_02003dee);
  r0 = v10;
L_0200991e:
  v11 = Object_GetById(r0);
  FIELD_AT(v11, u8, 98) = 4;
  FIELD_AT(v11, s32, 104) = r8;
  FIELD_AT(v11, s32, 108) = (s32)Func_020015e4;
  r4 = r11;
  FIELD_AT(r11, u16, 0) = (FIELD_AT(r4, u16, 0)) + 1;
  r0 = r11;
  goto L_02009954;
L_0200993e:
  if (r3 <= 0x16c)
    goto L_02009954;
  Call2((void (*)())Object_SetPartAttribute, r8, 0);
  FIELD_AT(r8, s32, 108) = 0;
  r0 = v12;
L_02009954:
  r4 = 0;
  r3 = *(s16 *)((358 + r9) + r4);
  if ((*(s16 *)((358 + r9) + r4)) != 0)
    goto L_020099a2;
  r0 = 0;
  r3 = FIELD_AT(r9, s16, 346);
  if ((FIELD_AT(r9, s16, 346)) != 0)
    goto L_020099a2;
  r3 = FIELD_AT(r9, s16, 350);
  if ((FIELD_AT(r9, s16, 350)) != 0)
    goto L_020099a2;
  r3 = FIELD_AT(r9, s16, 360);
  if ((FIELD_AT(r9, s16, 360)) != 0)
    goto L_020099a2;
  r3 = FIELD_AT(r9, u16, 434);
  if ((FIELD_AT(r9, u16, 434)) == 0)
    goto L_020099a2;
  FIELD_AT(r8, u16, 100) = (FIELD_AT(r8, u16, 100)) + 1;
  r3 = (FIELD_AT(r8, u16, 100)) + 1;
L_020099a2:
  /* add sp, #72 */
  return r0;
}

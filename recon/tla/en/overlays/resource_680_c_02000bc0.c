// Draft, differ 140 of 218: a pillar the party pushed off a ledge drops onto
// the floor below with sixteen dust puffs, a shake and the leader riding it
// down (gravity lowered until it lands, at most 180 frames), then sets its
// flag. Overlay 681's Func_02004af0 is a copy with small changes. Translated
// from its listing; names and structure still to do.
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

extern u8 gData_020016cc[];
extern u8 gData_02001688[];
extern u8 IwramMulQ16[];
s32 Object_GetById();
void Func_02000e74();
void Func_02000ef4();
void Camera_MoveWithinMapBounds();
void Func_02000b2c();
void Func_02000e2c();
s32 Func_02000e34();
s32 Random16Far();
s32 Math_Cosine();
s32 Math_Sine();
void Func_02000f24();
void Func_02000e54();
void Func_02000ebc();
void Func_02000e24();
void FieldEvent_WaitFrames();
void WaitFrames();
void Func_02000ecc();
void ObjectDispatch_SetSingleChildFlags();
void Func_02000e7c();
void GameFlag_SetBit();

s32 Func_02000bc0(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r6_1, r5_1, r7_1, r11_1, r7_2, r10_1, r6_2, r9_1, r8_1, r6_3, r6_4;
  s32 r6, v0, r5, v1, r7, r11, v2, r0, v3, r10, v4, v5, r9, v6, r8, v7, v8, v9,
      r2, r3, v10, v11, v12;
  s32 stack[4];

  /* sub (s32)stack, #16 */
  stack[3] = a1;
  r6_1 = a0;
  v0 = Object_GetById(FIELD_AT((s32)(s32)&gPartyState, s32, 532));
  r5_1 = v0;
  stack[2] = FIELD_AT(r5_1, s32, 72);
  Func_02000e74();
  Call1((void (*)())Func_02000ef4, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  v1 = Object_GetById(r6_1);
  r7_1 = v1;
  Func_02000b2c(r6_1, FIELD_AT(r5_1, s32, 8), FIELD_AT(r5_1, s32, 12),
                FIELD_AT(r5_1, s32, 16));
  Func_02000e2c(r7_1, (s32)gData_020016cc);
  r11_1 = 15;
L_02008c24:
  v2 = Func_02000e34(255, FIELD_AT(r5_1, s32, 8),
                     (FIELD_AT(r5_1, s32, 12)) + 1048576,
                     FIELD_AT(r5_1, s32, 16));
  r7_2 = v2;
  r0 = v2;
  if (r7_2 != 0) {
    Func_02000e2c(r0, (s32)gData_02001688);
    v3 = Random16Far();
    r10_1 = v3;
    v4 = Random16Far();
    r6_2 = v4;
    v5 = Random16Far();
    stack[1] = v5;
    r9_1 = ((u32)v5 >> 1) + 8192;
    v6 = Math_Cosine(r10_1);
    r6_2 = r6_2 << 3;
    r8_1 = (s32)IwramMulQ16;
    v7 = Iwram_MulQ16(r6_2, v6);
    FIELD_AT(r7_2, s32, 40) = v7;
    v8 = Math_Sine(r10_1);
    v9 = Iwram_MulQ16(r6_2, v8);
    FIELD_AT(r7_2, s32, 52) = 0;
    FIELD_AT(r7_2, s32, 24) = r9_1;
    FIELD_AT(r7_2, s32, 28) = r9_1;
    FIELD_AT(r7_2, s32, 72) = 0x547a;
    FIELD_AT(r7_2, s32, 36) = v9;
    FIELD_AT(r7_2, s32, 68) = 0x8000;
  }
  r11_1 = r11_1 - 1;
  r2 = -1;
  r3 = r11_1;
  if (r11_1 >= 0)
    goto L_02008c24;
  Call1((void (*)())Func_02000f24, 0x24e);
  Call1((void (*)())Func_02000f24, 217);
  Call3((void (*)())Func_02000e54, 0x20000, 0x20000, 0xe666);
  Call2((void (*)())Func_02000ebc, FIELD_AT((s32)(s32)&gPartyState, s32, 532),
        0x101);
  Call2((void (*)())Func_02000e24, r5_1, 40);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Call1((void (*)())Func_02000f24, 204);
  FIELD_AT(r5_1, u8, 85) = 3;
  r6_3 = 0;
  FIELD_AT(r5_1, s32, 72) = 0x547a;
  r2 = r5_1 + 85;
  r3 = 0x547a;
  goto L_02008d06;
L_02008d04:
  r6_3 = r6_3 + 1;
L_02008d06:
  if (r6_3 <= 179) {
    Call1((void (*)())WaitFrames, 1);
    r2 = FIELD_AT(r5_1, s32, 12);
    r3 = FIELD_AT(r5_1, s32, 20);
    if ((FIELD_AT(r5_1, s32, 12)) > (FIELD_AT(r5_1, s32, 20)))
      goto L_02008d04;
  }
  Call1((void (*)())Func_02000f24, 188);
  r6_4 = (s32)&gPartyState;
  FIELD_AT(r5_1, s32, 72) = stack[2];
  Call2((void (*)())Func_02000ebc, FIELD_AT(r6_4, s32, 532), 0);
  Call2((void (*)())Func_02000e24, r5_1, 38);
  Func_02000e54(0x10000, 0x40000, 0x10000);
  Call1((void (*)())WaitFrames, 10);
  Call3((void (*)())Func_02000e54, -1, -1, 0xe666);
  Call1((void (*)())WaitFrames, 20);
  Call4((void (*)())Camera_MoveWithinMapBounds, FIELD_AT(r5_1, s32, 8),
        FIELD_AT(r5_1, s32, 12), FIELD_AT(r5_1, s32, 16), 1);
  Func_02000ecc();
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, r5_1, 1);
  Call2((void (*)())Func_02000e24, r5_1, 1);
  FIELD_AT(r5_1, s32, 40) = 0x40000;
  Call1((void (*)())WaitFrames, 20);
  FIELD_AT(r6_4, u8, 530) = 0;
  Func_02000e7c();
  GameFlag_SetBit(stack[3]);
  /* add (s32)stack, #16 */
  return v12;
}

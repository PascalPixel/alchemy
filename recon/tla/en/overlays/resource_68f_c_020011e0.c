// Draft, differ 79 of 172: translated from its listing; names and structure still
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

extern u8 Effect_AdvanceMotion[];
s32 Object_GetById();
void Func_02003048();
void Func_02003148();
void Camera_MoveWithinMapBounds();
void Func_02002fa8();
void WaitFrames();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_020031a8();
void FieldEvent_WaitFrames();
s32 Math_Cosine();
s32 Math_Sine();
s32 Random16Far();
void Func_020000b8();
s32 Func_020030c8();
s32 Func_02003000();
void Func_02003008();
s32 Func_02003050();

s32 Func_020011e0(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 v0, r7, v1, r5, r4, r8, r10, v2, r6, v3, v4, v5, r2, r3, v6, v7, v8, r0;
  s32 stack[17];

  /* sub (s32)stack, #68 */
  v0 = Object_GetById(FIELD_AT((s32)(s32)&gPartyState, s32, 532));
  r7 = v0;
  Func_02003048();
  Call1((void (*)())Func_02003148, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02002fa8();
  Call1((void (*)())WaitFrames, 1);
  FIELD_AT(r7, s32, 12) = 0x820000;
  r5 = r7;
  FIELD_AT(r7, s32, 72) = 0x8000;
  r5 = r5 + 85;
  FIELD_AT(r7, s32, 68) = 0;
  FIELD_AT(r5, u8, 0) = 0;
  r4 = 214;
  r4 = r4 << 1;
  FIELD_AT(((s32)Ram_HeapSlots->eventWork) + r4, s32, 0) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1((void (*)())Func_020031a8, 204);
  FIELD_AT(r5, u8, 0) = 3;
  Call1((void (*)())FieldEvent_WaitFrames, 24);
  FIELD_AT((s32)((u8 *)stack + 28), s32, 4) = 7;
  r8 = (s32)((u8 *)stack + 28);
  FIELD_AT((s32)((u8 *)stack + 28), s32, 36) = (s32)Effect_AdvanceMotion;
  FIELD_AT((s32)((u8 *)stack + 28), s32, 8) = 0xcccc;
  FIELD_AT((s32)((u8 *)stack + 28), s32, 12) = 0xcccc;
  r10 = 0;
L_0200927a:
  r4 = r10;
  r5 = r4 << 12;
  v2 = Math_Cosine(r5);
  r6 = (s32)((u8 *)stack + 16);
  FIELD_AT(r6, s32, 0) = v2;
  FIELD_AT(r6, s32, 4) = 0;
  v3 = Math_Sine(r5);
  FIELD_AT(r6, s32, 8) = v3;
  FIELD_AT(r6, s32, 0) = (FIELD_AT(r6, s32, 0)) + ((FIELD_AT(r6, s32, 0)) >> 1);
  v4 = Random16Far();
  FIELD_AT(r6, s32, 0) =
      ((FIELD_AT(r6, s32, 0)) + ((u32)(((v4 << 1) + v4) << 14) >> 16)) - 24576;
  v5 = Random16Far();
  r5 = FIELD_AT(r6, s32, 8);
  r4 = -0x3000;
  r5 = r5 + ((u32)(((v5 << 1) + v5) << 13) >> 16);
  r5 = r5 + r4;
  r4 = FIELD_AT(r6, s32, 4);
  FIELD_AT(r6, s32, 8) = r5;
  stack[0] = r4;
  r4 = 0x1090001;
  stack[1] = r5;
  stack[2] = r4;
  r4 = r8;
  Func_020000b8(FIELD_AT(r7, s32, 8), FIELD_AT(r7, s32, 12),
                FIELD_AT(r7, s32, 16), FIELD_AT(r6, s32, 0), r4, r5, r4, r4);
  r10 = r10 + 1;
  r2 = 1;
  r3 = r10;
  if ((u32)r10 <= (u32)16)
    goto L_0200927a;
  Call1((void (*)())Func_020031a8, 188);
  r5 = (s32)&gPartyState;
  r4 = 133;
  r4 = r4 << 2;
  r5 = r5 + r4;
  Call2((void (*)())Func_020030c8, FIELD_AT(r5, s32, 0), 0x101);
  Call2((void (*)())Object_SetModeById, FIELD_AT(r5, s32, 0), 22);
  Call3((void (*)())Func_02003000, 0x50000, 0x50000, 0x10000);
  v6 = Func_02003000(-1, -1, 0xe666);
  Func_02003008();
  v7 = Func_020030c8(FIELD_AT(r5, s32, 0), 0x100);
  FIELD_AT(r7, s32, 72) = 0x10000;
  FIELD_AT(r7, s32, 68) = 0x4000;
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), s32, 428) = 0x204;
  v8 = Func_02003050();
  /* add (s32)stack, #68 */
  return v8;
}

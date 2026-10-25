// Draft, differ 235 of 307: translated from its listing; names and structure still
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

extern u8 Func_020010a8[];
extern u8 Func_02001650[];
extern u8 gFrameCount[];
extern u8 gData_020022cc[];
void Func_02001fac();
void Func_02002044();
void Func_02002054();
void Func_02001f74();
s32 Engine_ObjectCreate();
void Camera_MoveWithinMapBounds();
void Event_StartScreenTransition();
void WaitFrames();
s32 Scheduler_AddOrUpdateCallback();
void Event_WaitTransitionFrames();
void FieldEvent_WaitFrames();
s32 Func_02001928();
void Func_02001f94();
void Func_02001ffc();
void GameFlag_SetBit();
s32 Random16Far();
void Func_0200015c();
void Func_02001f64();
void Scheduler_RemoveCallbackFar();
void Func_0200204c();
s32 Func_02001fb4();

s32 Func_02001a14(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r8_1, r7_1, r5_1, r6_1, r7_2, r5_2, r6_2, r6_3, r5_3, r7_3, r5_4, r5_5,
      r5_6, r5_7, r5_8, r5_9;
  s32 v0, r8, v1, v2, r7, r3, v3, r5, v4, r6, r1, v5, v6, r4, r0, r2, v7, v8,
      v9;
  s32 stack[24];

  /* sub (s32)stack, #96 */
  Func_02001fac();
  Call1((void (*)())Func_02002044, 0);
  Call1((void (*)())Func_02002054, 0);
  Call6((void (*)())Func_02001f74, 49, 70, 19, 73, 6, 6);
  v0 = Engine_ObjectCreate(159, 0x15c0000, 0, 0xb80000);
  r8_1 = v0;
  FIELD_AT(v0, s32, 108) = (s32)Func_020010a8;
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1600000, -1, 0xc40000, 0);
  Event_StartScreenTransition();
  Call1((void (*)())WaitFrames, 1);
  Call2((void (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02001650, 0xc80);
  Event_WaitTransitionFrames();
  Call1((void (*)())FieldEvent_WaitFrames, 60);
  Call3((void (*)())Func_02001928, 21, 23, 9);
  Call3((void (*)())Func_02001928, 37, 17, 10);
  Call3((void (*)())Func_02001928, 47, 27, 12);
  v1 = Func_02001928(59, 19, 13);
  Func_02001f94();
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1600000, -1, 0xc40000, 1);
  Func_02001ffc();
  Call1((void (*)())GameFlag_SetBit, 0x200);
  FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 362) = 200;
  Call1((void (*)())Func_02002054, 0x212);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  r7_1 = 0;
L_02009af0:
  r3 = 31 & r7_1;
  if ((31 & r7_1) == 0) {
    Call1((void (*)())Func_02002054, 164);
  }
  if ((u32)r7_1 <= (u32)119) {
    r3 = 15;
  } else {
    if ((u32)r7_1 > (u32)239)
      goto L_02009b1c;
    r3 = 7;
  }
  if ((r3 & r7_1) == 0)
    goto L_02009b24;
  goto L_02009b78;
L_02009b1c:
  if ((1 & r7_1) != 0)
    goto L_02009b78;
L_02009b24:
  v3 = Random16Far();
  r5_1 = v3;
  v4 = Random16Far();
  FIELD_AT((s32)((u8 *)stack + 56), s32, 8) = 0xcccc;
  FIELD_AT((s32)((u8 *)stack + 56), s32, 12) = 0xcccc;
  FIELD_AT((s32)((u8 *)stack + 56), s32, 0) = 1;
  r6_1 = 31;
  FIELD_AT((s32)((u8 *)stack + 56), s32, 4) = 7;
  r5_1 = r5_1 & r6_1;
  r5_1 = r5_1 << 16;
  r5_1 = r5_1 + 22020096;
  Func_0200015c(r5_1, 0, ((v4 & r6_1) << 16) + 11534336, 0, 0, 0, 0xb0000,
                (s32)((u8 *)stack + 56));
  Call1((void (*)())WaitFrames, 1);
L_02009b78:
  r7_1 = r7_1 + 1;
  r1 = 0x168;
  if (r7_1 != 0x168)
    goto L_02009af0;
  r7_2 = 0;
L_02009b84:
  v5 = Random16Far();
  r5_2 = v5;
  v6 = Random16Far();
  FIELD_AT((s32)((u8 *)stack + 16), s32, 8) = 0x18000;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 12) = 0x18000;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 4) = 7;
  r6_2 = 31;
  FIELD_AT((s32)((u8 *)stack + 16), s32, 0) = 1;
  r5_2 = r5_2 & r6_2;
  r5_2 = r5_2 << 16;
  r5_2 = r5_2 + 22020096;
  Func_0200015c(r5_2, 0, ((v6 & r6_2) << 16) + 11534336, 0, 0x10000, 0, 0xb0000,
                (s32)((u8 *)stack + 16));
  r7_2 = r7_2 + 1;
  Call1((void (*)())WaitFrames, 1);
  if (r7_2 != 16)
    goto L_02009b84;
  Func_02001f64(r8_1);
  Call1((void (*)())Func_02002054, 0x22d);
L_02009bf0:
  Call1((void (*)())WaitFrames, 10);
  r6_3 = 160;
  r6_3 = r6_3 << 19;
  r5_3 = 0;
  r7_3 = 1;
  r6_3 = r6_3 + 98;
L_02009c00:
  r4 = 31;
  r4 = r4 & (FIELD_AT(r6_3, u16, 0));
  r0 = ((u32)((FIELD_AT(r6_3, u16, 0)) << 16) >> 21) & (31);
  r2 = ((u32)((FIELD_AT(r6_3, u16, 0)) << 16) >> 26) & (31);
  r3 = r4;
  if ((u32)r4 <= (u32)10) {
    r4 = r3 + 1;
    r5_4 = 1;
  }
  r3 = (u32)(r4 << 16) >> 16;
  if ((u32)((u32)(r4 << 16) >> 16) <= (u32)11)
    goto L_02009c38;
  r4 = ((r3 + 65535) << 16) >> 16;
  r5_5 = 1;
  goto L_02009c38;
L_02009c38:
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) <= (u32)5) {
    r5_6 = 1;
    r0 = ((r3 + 1) << 16) >> 16;
  }
  r3 = (u32)(r0 << 16) >> 16;
  if ((u32)((u32)(r0 << 16) >> 16) > (u32)6) {
    r5_7 = 1;
    r0 = ((r3 + 65535) << 16) >> 16;
  }
  r3 = (u32)(r2 << 16) >> 16;
  if ((u32)((u32)(r2 << 16) >> 16) <= (u32)22) {
    r5_8 = 1;
    r2 = ((r3 + 1) << 16) >> 16;
  }
  r3 = (u32)(r2 << 16) >> 16;
  if ((u32)((u32)(r2 << 16) >> 16) > (u32)23) {
    r5_9 = 1;
    r2 = ((r3 + 65535) << 16) >> 16;
  }
  r7_3 = r7_3 + 1;
  FIELD_AT(r6_3, u16, 0) = ((u32)(r4 << 16) >> 16) |
                           (((u32)(r2 << 16) >> 6) | ((u32)(r0 << 16) >> 11));
  r6_3 = r6_3 + 2;
  if (r7_3 != 14)
    goto L_02009c00;
  FIELD_AT((s32)gData_020022cc, s32, 0) =
      (FIELD_AT((s32)gData_020022cc, s32, 0)) +
      ((FIELD_AT((s32)gFrameCount, s32, 0)) & 1);
  if (r5_9 != 0)
    goto L_02009bf0;
  Scheduler_RemoveCallbackFar((s32)Func_02001650);
  Func_0200204c();
  Call1((void (*)())GameFlag_SetBit, 0x940);
  v9 = Func_02001fb4();
  /* add (s32)stack, #96 */
  return v9;
}

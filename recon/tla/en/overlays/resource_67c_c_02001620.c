// Draft, differ 233 of 402: the Psynergy Stone's power passing between two
// objects: the first rises glowing while sixteen sparks burst from it with the
// screen flashing, sixteen sparks then spiral down onto the second as it rises,
// sixteen orbiting sparks circle it, the screen whitens and both settle back
// while the second keeps sparkling for two seconds. Overlay 681's Func_020015d0
// is a near copy. Translated from its listing; names and structure still to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 gData_02004034[];
extern u8 gData_02003fe8[];
extern u8 gData_02004004[];
extern u8 gData_02004058[];
extern u8 gData_02004014[];
extern u8 gData_0200407c[];
extern u8 gData_020040ac[];
extern u8 Func_02001598[];
s32 Object_GetById();
s32 Func_02003ef4();
void FieldEvent_WaitFrames();
void Func_02003e44();
void Camera_MoveWithinMapBounds();
void Func_02003d44();
void Func_02003e7c();
void Func_02003e74();
void Func_02003e84();
void ObjectMotion_EnableActionAndSetCallback();
void Func_02003e04();
s32 Engine_ObjectCreate();
s32 Random16Far();
void Object_SetPartAttribute();
void Object_SetSpritePriority();
void Func_02003cc4();
void ObjectDispatch_SetSingleChildFlags();
void Animation_SetStateFlags();
void Func_02003ccc();
void WaitFrames();
void Vector_AddPolarOffsetFar();
void Func_02003ebc();

s32 Func_02001620(s32 a0, s32 a1, s32 a2) {
  s32 r11, r9, v0, r7, v1, r10, r8, v2, r5, v3, v4, r6, r3, v5, v6, r0, v7, r1,
      v8, v9, r2, v10, v11;
  s32 stack[16];

  /* sub (s32)stack, #12 */
  r11 = a1;
  r9 = a0;
  v0 = Object_GetById(a0);
  r7 = v0;
  v1 = Object_GetById(r11);
  r10 = v1;
  Call1((void (*)())Func_02003ef4, 78);
  Call1((void (*)())FieldEvent_WaitFrames, 30);
  Call2((void (*)())Func_02003e44, 0x8000, 0x1000);
  Call4((void (*)())Camera_MoveWithinMapBounds, (*(s16 *)(r7 + 10)) << 16,
        (*(s16 *)(r7 + 14)) << 16, (*(s16 *)(r7 + 18)) << 16, 1);
  Call1((void (*)())Func_02003ef4, 141);
  Call3((void (*)())Func_02003d44, 0x20000, 0x20000, 0x10000);
  Call2((void (*)())Func_02003e7c, 0x10000, 0);
  Call2((void (*)())Func_02003e74, 0x10003, 1);
  Call1((void (*)())Func_02003e84, 60);
  FIELD_AT(r7, u8, 85) = 0;
  FIELD_AT(r7, s32, 52) = 0x3333;
  FIELD_AT(r7, s32, 48) = 0x3333;
  ObjectMotion_EnableActionAndSetCallback(r9, (s32)gData_02004034);
  Call1((void (*)())Func_02003ef4, 194);
  Call1((void (*)())FieldEvent_WaitFrames, 45);
  Call2((void (*)())Func_02003e04, r9, 0x100);
  FIELD_AT(67108946, u16, 0) = 4104;
  FIELD_AT(67108944, u16, 0) = 16144;
  r8 = 0;
  goto L_020096f8;
L_020096f8:
  v2 = Engine_ObjectCreate(0x2a0, FIELD_AT(r7, s32, 8), FIELD_AT(r7, s32, 12),
                           FIELD_AT(r7, s32, 16));
  r5 = v2;
  Call1((void (*)())Func_02003ef4, 246);
  v3 = Random16Far();
  FIELD_AT(r5, s32, 8) = (FIELD_AT(r5, s32, 8)) + (((31 & v3) - 16) << 16);
  v4 = Random16Far();
  FIELD_AT(r5, s32, 48) = 0x80000;
  FIELD_AT(r5, s32, 12) = (FIELD_AT(r5, s32, 12)) + (((15 & v4) - 8) << 16);
  FIELD_AT(r5, s32, 52) = 0x3333;
  FIELD_AT(r5, u8, 85) = 0;
  Call2((void (*)())Object_SetPartAttribute, r5, 2);
  Call2((void (*)())Object_SetSpritePriority, r5, 1);
  Call2((void (*)())Func_02003cc4, r5, 7);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, r5, 0);
  Call2((void (*)())Animation_SetStateFlags, r5, 1);
  Func_02003ccc(r5, (s32)gData_02003fe8);
  r6 = 128;
  r5 = 4106;
  r6 = r6 << 19;
  r6 = r6 + 82;
  FIELD_AT(r6, u16, 0) = 4100;
  Call1((void (*)())WaitFrames, 2);
  FIELD_AT(r6, u16, 0) = r5;
  Call1((void (*)())WaitFrames, 2);
  FIELD_AT(r6, u16, 0) = 4112;
  Call1((void (*)())WaitFrames, 2);
  FIELD_AT(r6, u16, 0) = r5;
  Call1((void (*)())WaitFrames, 2);
  r8 = r8 + 1;
  r3 = r8;
  goto L_020097bc;
L_020097bc:
  if (r3 != 16)
    goto L_020096f8;
  FIELD_AT(r6, u16, 0) = 4104;
  Call1((void (*)())WaitFrames, 30);
  r8 = 0;
L_020097ce:
  v5 = Engine_ObjectCreate(0x2a0, FIELD_AT(r10, s32, 8), FIELD_AT(r10, s32, 12),
                           FIELD_AT(r10, s32, 16));
  r5 = v5;
  Call1((void (*)())Func_02003ef4, 195);
  v6 = Random16Far();
  r6 = r5;
  r6 = r6 + 100;
  r0 = (v6 & (32767)) + 32768;
  goto L_02009804;
L_02009804:
  FIELD_AT(r6, u16, 0) = r0;
  v7 = Random16Far();
  r7 = 0;
  FIELD_AT(r5, u16, 102) = (((1 & r8) << 1) - 1) << ((3 & v7) + 9);
  FIELD_AT((r5 + 102) - 17, u8, 0) = r7;
  FIELD_AT(r5, s32, 76) = 0x7a0000;
  FIELD_AT(r5, s32, 104) = r10;
  FIELD_AT(r5, u8, 98) = ((16 - r8) << 3) + 15;
  Call2((void (*)())Object_SetPartAttribute, r5, 2);
  Call2((void (*)())Object_SetSpritePriority, r5, 1);
  r0 = r5;
  r1 = 7;
  goto L_0200985c;
L_0200985c:
  Call2((void (*)())Func_02003cc4, r0, r1);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, r5, 0);
  Call2((void (*)())Animation_SetStateFlags, r5, 1);
  Call3((void (*)())Vector_AddPolarOffsetFar, FIELD_AT(r5, s32, 76),
        FIELD_AT(r6, s16, 0), r5 + 8);
  Func_02003ccc(r5, (s32)gData_02004004);
  if (r8 == 3) {
    Call2((void (*)())Func_02003e04, r11, 0x100);
    FIELD_AT(r10, u8, 85) = r7;
    FIELD_AT(r10, s32, 52) = 0x3333;
    FIELD_AT(r10, s32, 48) = 0x3333;
    ObjectMotion_EnableActionAndSetCallback(r11, (s32)gData_02004058);
  }
  Call1((void (*)())WaitFrames, 8);
  r8 = r8 + 1;
  r3 = 1;
  if (r8 == 16)
    goto L_020098c2;
  goto L_020097ce;
L_020098c2:
  Call1((void (*)())Func_02003ef4, 220);
  Call1((void (*)())WaitFrames, 16);
  r8 = 2;
L_020098d2:
  v8 = Engine_ObjectCreate(0x2a0, FIELD_AT(r10, s32, 8),
                           ((r8 << 16) + (FIELD_AT(r10, s32, 12))) - 524288,
                           FIELD_AT(r10, s32, 16));
  r5 = v8;
  v9 = Random16Far();
  FIELD_AT(r5, u16, 100) = v9;
  FIELD_AT(r5, u16, 102) = 0x800;
  FIELD_AT(r5, u8, 98) = r8;
  FIELD_AT((r5 + 98), u8, 1) = 1;
  r6 = 0;
  FIELD_AT(r5, s32, 68) = FIELD_AT(r5, s32, 8);
  FIELD_AT(r5, s32, 76) = r6;
  FIELD_AT(r5, s32, 72) = FIELD_AT(r5, s32, 16);
  FIELD_AT(r5, s32, 104) = r10;
  FIELD_AT(r5, u8, 85) = 0;
  FIELD_AT((r5 + 85) - 50, u8, 0) = 1;
  Call2((void (*)())Object_SetPartAttribute, r5, 2);
  Call2((void (*)())Func_02003cc4, r5, 7);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, r5, 0);
  Call2((void (*)())Animation_SetStateFlags, r5, 1);
  goto L_02009960;
L_02009960:
  Func_02003ccc(r5, (s32)gData_02004014);
  r8 = r8 + 2;
  r2 = r8;
  if (r8 != 32)
    goto L_020098d2;
  Call1((void (*)())Func_02003ef4, 220);
  Call1((void (*)())WaitFrames, 50);
  Call2((void (*)())Func_02003e74, 0x7fff, 0);
  Call1((void (*)())Func_02003e84, 8);
  Call1((void (*)())WaitFrames, 16);
  Call3((void (*)())Func_02003d44, 0, 0, 0);
  Call2((void (*)())Func_02003e04, r9, 0);
  Call2((void (*)())Func_02003e04, r11, 0);
  Call1((void (*)())Func_02003ef4, 0x23e);
  Call2((void (*)())Func_02003e74, 0x10000, 0);
  Call1((void (*)())Func_02003e84, 80);
  ObjectMotion_EnableActionAndSetCallback(r9, (s32)gData_0200407c);
  ObjectMotion_EnableActionAndSetCallback(r11, (s32)gData_020040ac);
  FIELD_AT(r10, s32, 108) = (s32)Func_02001598;
  Call1((void (*)())WaitFrames, 120);
  FIELD_AT(r10, s32, 108) = r6;
  v10 = Func_02003ef4(0x186);
  Func_02003ebc();
  Call1((void (*)())WaitFrames, 1);
  /* add (s32)stack, #12 */
  return v11;
}

// Draft, differ 266 of 260: translated from its listing; names and structure still
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

extern u8 Func_02000d40[];
extern u8 gFrameCount[];
extern u8 Func_020011b8[];
extern u8 gData_02003d50[];
s32 Engine_ObjectCreate();
s32 Math_Cosine();
void Func_02003a54();
void ObjectDispatch_SetSingleChildFlags();
void Func_02003aec();
void Func_02003bcc();
s32 MapCell_GetAttributes();
s32 Func_020005a0();
void Func_02003a84();
void Func_02003b5c();
void Camera_MoveWithinMapBounds();
void Func_02003a8c();
void Func_02003a6c();
s32 Random16Far();
void Func_0200015c();
void WaitTicks();
void Camera_FollowActor();
void Func_02003b6c();
s32 Func_02003af4();
void GameFlag_SetBit();
void Func_02000c08();
void GameFlag_ClearBit();

s32 Func_020011e4(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r8, v0, r6, v1, r10, v2, r7, v3, r0, r9, r5, r2, r3, r1, v4, v5, v6, r4,
      v7, v8, v9, v10, v11, v12;
  s32 stack[17];

  r8 = a0;
  /* sub (s32)stack, #68 */
  v0 = Engine_ObjectCreate(0x193, FIELD_AT(a0, s32, 8), FIELD_AT(a0, s32, 12),
                           FIELD_AT(a0, s32, 16));
  r6 = v0;
  v1 = Math_Cosine(FIELD_AT(r8, u16, 6));
  FIELD_AT(r6, u8, 85) = 0;
  r10 = v1 << 1;
  Call2((void (*)())Func_02003a54, r6, 1);
  Call2((void (*)())ObjectDispatch_SetSingleChildFlags, r6, 0);
  Func_02003aec();
  Call1((void (*)())Func_02003bcc, 0);
  FIELD_AT((s32)((u8 *)stack + 56), s32, 0) = FIELD_AT(r6, s32, 8);
  FIELD_AT((s32)((u8 *)stack + 56), s32, 4) = FIELD_AT(r6, s32, 12);
  FIELD_AT((s32)((u8 *)stack + 56), s32, 8) = FIELD_AT(r6, s32, 16);
L_0200923e:
  r7 = (s32)((u8 *)stack + 56);
  FIELD_AT(r7, s32, 0) = (FIELD_AT(r7, s32, 0)) + r10;
  v3 = MapCell_GetAttributes(
      2, ((FIELD_AT(r7, s32, 0)) + r10) >> 20,
      ((FIELD_AT(r7, s32, 8)) - (FIELD_AT(r7, s32, 4))) >> 20);
  r0 = v3 >> 8;
  if ((v3 >> 8) == 212)
    goto L_020092d6;
  if (r0 == 222)
    goto L_020092d6;
  r9 = 8;
L_02009264:
  r5 = *(s32 *)(((s32)Ram_HeapSlots->eventWork) + ((r9 << 2) + 20));
  if ((FIELD_AT(r5, s32, 0)) == 0)
    goto L_020092ca;
  r2 = (FIELD_AT(r8, s32, 8)) >> 20;
  r3 = (FIELD_AT(r5, s32, 8)) >> 20;
  r1 = r8;
  if (((FIELD_AT(r8, s32, 8)) >> 20) != ((FIELD_AT(r5, s32, 8)) >> 20))
    goto L_0200929e;
  r2 = (FIELD_AT(r1, s32, 12)) >> 20;
  r3 = (FIELD_AT(r5, s32, 12)) >> 20;
  if (((FIELD_AT(r1, s32, 12)) >> 20) != ((FIELD_AT(r5, s32, 12)) >> 20))
    goto L_0200929e;
  r2 = (FIELD_AT(r1, s32, 16)) >> 20;
  r3 = (FIELD_AT(r5, s32, 16)) >> 20;
  if (((FIELD_AT(r1, s32, 16)) >> 20) == ((FIELD_AT(r5, s32, 16)) >> 20))
    goto L_020092ca;
L_0200929e:
  v4 = Func_020005a0(r5 + 8, r7);
  if (v4 > 8)
    goto L_020092ca;
  if (((FIELD_AT(r5, s32, 12)) >> 20) != ((FIELD_AT(r7, s32, 4)) >> 20))
    goto L_020092ca;
  if ((FIELD_AT(FIELD_AT(FIELD_AT(r5, s32, 80), s32, 40), s16, 0)) != 0x110)
    goto L_020093ae;
  goto L_020092d6;
L_020092ca:
  r9 = r9 + 1;
  if (r9 != 64)
    goto L_02009264;
  goto L_0200923e;
L_020092d6:
  r5 = 128;
  r5 = r5 << 10;
  r7 = (s32)((u8 *)stack + 56);
  FIELD_AT(r6, s32, 52) = r5;
  FIELD_AT(r6, s32, 48) = r5;
  FIELD_AT(r6, s32, 108) = (s32)Func_02000d40;
  Func_02003a84(r6, FIELD_AT(r7, s32, 0), FIELD_AT(r7, s32, 4),
                FIELD_AT(r7, s32, 8));
  Call2((void (*)())Func_02003b5c, r5, 0x4000);
  Call4((void (*)())Camera_MoveWithinMapBounds, FIELD_AT(r7, s32, 0),
        FIELD_AT(r7, s32, 4), FIELD_AT(r7, s32, 8), 1);
  Func_02003a8c(r6);
  FIELD_AT(r6, s32, 108) = 0;
  Func_02003a6c(r6);
  r9 = 0;
L_0200931a:
  r10 = FIELD_AT((s32)gFrameCount, s32, 0);
  r10 = r10 & 1;
  r2 = 1;
  if ((r10 & 1) == 0) {
    r8 = (s32)((u8 *)stack + 16);
    FIELD_AT(r8, u16, 24) = 0x193;
    FIELD_AT(r8, s32, 0) = r2;
    FIELD_AT(r8, s32, 36) = (s32)Func_020011b8;
    r6 = 31;
    FIELD_AT(r8, s32, 28) = (s32)gData_02003d50;
    v5 = Random16Far();
    r5 = v5;
    v6 = Random16Far();
    r4 = FIELD_AT(r7, s32, 0);
    r5 = r5 & r6;
    r5 = r5 - 16;
    stack[1] = r10;
    r5 = r5 << 13;
    Func_0200015c(r4, FIELD_AT(r7, s32, 4), FIELD_AT(r7, s32, 8), r5,
                  ((v6 & r6) - 16) << 13, r10, 0x1320000, r8);
  }
  Call1((void (*)())WaitTicks, 1);
  r9 = r9 + 1;
  if (r9 != 16)
    goto L_0200931a;
  Call1((void (*)())WaitTicks, 24);
  Call2((void (*)())Camera_FollowActor,
        FIELD_AT((s32)(s32)&gPartyState, s32, 532), 1);
  Func_02003b6c();
  Call1((void (*)())WaitTicks, 10);
  v9 = Func_02003af4();
  r0 = v9;
  goto L_02009424;
L_020093ae:
  r5 = 128;
  r5 = r5 << 10;
  FIELD_AT(r6, s32, 52) = r5;
  FIELD_AT(r6, s32, 48) = r5;
  FIELD_AT(r6, s32, 108) = (s32)Func_02000d40;
  Func_02003a84(r6, FIELD_AT(r7, s32, 0), FIELD_AT(r7, s32, 4),
                FIELD_AT(r7, s32, 8));
  Call2((void (*)())Func_02003b5c, r5, 0x4000);
  Call4((void (*)())Camera_MoveWithinMapBounds, FIELD_AT(r7, s32, 0),
        FIELD_AT(r7, s32, 4), FIELD_AT(r7, s32, 8), 1);
  Func_02003a8c(r6);
  FIELD_AT(r6, s32, 108) = 0;
  Func_02003a6c(r6);
  Call1((void (*)())GameFlag_SetBit, 0x2f0);
  Func_02000c08((u32)(r9 << 16) >> 16);
  Call1((void (*)())GameFlag_ClearBit, 0x2f0);
  Call2((void (*)())Camera_FollowActor,
        FIELD_AT((s32)(s32)&gPartyState, s32, 532), 1);
  Func_02003b6c();
  Call1((void (*)())WaitTicks, 10);
  v12 = Func_02003af4();
  r0 = v12;
L_02009424:
  /* add (s32)stack, #68 */
  return r0;
}

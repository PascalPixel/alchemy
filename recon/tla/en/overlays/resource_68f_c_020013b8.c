// Draft, differ 221 of 416: translated from its listing; names and structure still
// to do.
#include "iwram_call.h"
#include "lib/call.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 Func_02000840[];
extern u8 gData_02003340[];
extern u8 gData_0200335a[];
extern u8 gData_02003264[];
extern u8 Data_02003ccc[];
extern u8 Func_020010d4[];
extern u8 Func_0200137c[];
extern u8 gData_0200326e[];
s32 GameFlag_Test();
void Func_02000548();
void Func_0200058c();
s32 Actor_GetById();
void Actor_SetSingleChildFlags();
void Func_02003020();
void Func_02002fc0();
void Func_02002fe8();
void Func_02003088();
s32 MapCell_SetAttributes();
void Actor_WatchAndSetScript();
void Func_020011e0();
void Func_02000404();
s32 Func_02002f98();
void Scene_SetActorSpritePriority();
void Actor_SetPartAttribute();
void Animation_SetStateFlags();
void Func_02003168();
void GameFlag_SetBit();
s32 Task_Add();

s32 Func_020013b8(s32 a0, s32 a1, s32 a2, s32 a3) {
  s32 r1, r2, r3, v0, r0, v1, v2, r6, v3, r5, v4, v5, v6, v7, v8, v9, v10, v11,
      v12, v13, v14, v15, v16, v17, v18, v19;
  s32 stack[16];

  FIELD_AT(((s32)Ram_HeapSlots->eventWork), s32, 428) = 0x204;
  /* sub (s32)stack, #8 */
  r1 = (s32)&gSequenceParentWork;
  r2 = FIELD_AT((s32)(s32)&gSequenceParentWork, s16, 480);
  if ((FIELD_AT((s32)(s32)&gSequenceParentWork, s16, 480)) != (206))
    goto L_020093e0;
  goto L_020097c4;
L_020093e0:
  if (r2 == (207))
    goto L_020093e8;
  goto L_0200950a;
L_020093e8:
  r3 = FIELD_AT(r1, s16, 482);
  if ((FIELD_AT(r1, s16, 482)) >= 1)
    goto L_020093f8;
  goto L_020097c4;
L_020093f8:
  if (r3 <= 4)
    goto L_02009402;
  if (r3 == 11)
    goto L_0200942a;
  goto L_020097c4;
L_02009402:
  v0 = GameFlag_Test(0x942);
  r0 = v0;
  if (v0 != 0) {
    Func_02000548();
  }
  v1 = GameFlag_Test(0x943);
  r0 = v1;
  if (v1 != 0)
    goto L_02009424;
  goto L_020097c4;
L_02009424:
  Func_0200058c();
  goto L_020097c4;
L_0200942a:
  v2 = GameFlag_Test(0x945);
  r6 = v2;
  if (r6 != 0)
    goto L_020094c6;
  v3 = Actor_GetById(8);
  r5 = v3;
  v4 = Actor_GetById(8);
  Call2((void (*)())Actor_SetSingleChildFlags, v4, 0);
  FIELD_AT(r5, u8, 89) = r6;
  FIELD_AT((r5 + 89) - 4, u8, 0) = r6;
  FIELD_AT(r5, s32, 20) = 0x80000;
  FIELD_AT(r5, s32, 12) = 0x80000;
  Call2((void (*)())Func_02003020, r5, 0x1b9);
  v5 = GameFlag_Test(0x944);
  if (v5 != 0)
    goto L_0200947a;
  goto L_020097c4;
L_0200947a:
  Call6((void (*)())Func_02002fc0, 31, 85, 20, 85, 3, 4);
  Call6((void (*)())Func_02002fe8, 11, 22, 3, 2, 20, 22);
  Call3((void (*)())Func_02003088, 68, 0x1580000, 0x1740000);
  v6 = Actor_GetById(68);
  FIELD_AT(v6, s32, 108) = (s32)Func_02000840;
  Call3((void (*)())Func_02003088, 8, 0, 0);
  goto L_020097c4;
L_020094c6:
  v7 = GameFlag_Test(0x944);
  if (v7 != 0)
    goto L_020094d6;
  goto L_020097c4;
L_020094d6:
  Call6((void (*)())Func_02002fc0, 31, 85, 20, 85, 3, 4);
  Call6((void (*)())Func_02002fe8, 11, 22, 3, 2, 20, 22);
  Call3((void (*)())Func_02003088, 8, 0, 0);
  goto L_020097c4;
L_0200950a:
  if (r2 != (209))
    goto L_020095ba;
  r3 = FIELD_AT(r1, s16, 482);
  if ((FIELD_AT(r1, s16, 482)) <= 7)
    goto L_02009520;
  goto L_020097c4;
L_02009520:
  if (r3 >= 6)
    goto L_02009526;
  goto L_020097c4;
L_02009526:
  v8 = GameFlag_Test(100);
  if (v8 != 0)
    goto L_02009594;
  v9 = GameFlag_Test(0x200);
  if (v9 != 0)
    goto L_02009568;
  Call3((void (*)())Func_02003088, 8, 0x1580000, 0x2180000);
  MapCell_SetAttributes(0, 21, 33, 0x3300);
  MapCell_SetAttributes(0, 22, 33, 0xff00);
  goto L_02009584;
L_02009568:
  v10 = GameFlag_Test(0x201);
  if (v10 != 0)
    goto L_02009584;
  MapCell_SetAttributes(0, 26, 33, 0xff00);
L_02009584:
  Call2((void (*)())Actor_WatchAndSetScript, 8,
        FIELD_AT((s32)(s32)&gSequenceParentWork, s32, 532));
L_02009594:
  v11 = GameFlag_Test(0x109);
  if (v11 == 0)
    goto L_020095a2;
  goto L_020097c4;
L_020095a2:
  r0 = 0x1e2;
  r2 = 0;
  r3 = FIELD_AT((s32)(s32)&gSequenceParentWork, s16, 482);
  if ((FIELD_AT((s32)(s32)&gSequenceParentWork, s16, 482)) == 7)
    goto L_020095b4;
  goto L_020097c4;
L_020095b4:
  Func_020011e0(r0, r1);
  goto L_020097c4;
L_020095ba:
  r3 = 210;
  if (r2 != (210))
    goto L_020095c8;
  Func_02000404((s32)gData_02003340);
  goto L_020097c4;
L_020095c8:
  if (r2 == (211))
    goto L_020095d0;
  goto L_02009770;
L_020095d0:
  r3 = (FIELD_AT(r1, s16, 482)) - 1;
  if ((u32)((FIELD_AT(r1, s16, 482)) - 1) <= (u32)9)
    goto L_020095e2;
  goto L_020097c4;
L_020095e2:
  switch (r3) {
  case 0:
    goto L_02009656;
  case 1:
    goto L_02009656;
  case 2:
    goto L_02009614;
  case 3:
    goto L_020096a4;
  case 4:
    goto L_020096a4;
  case 5:
    goto L_020096fe;
  case 6:
    goto L_020096fe;
  case 7:
    goto L_020097c4;
  case 8:
    goto L_020097c4;
  case 9:
    goto L_02009728;
  }
L_02009614:
  v12 = Func_02002f98(0x11e, 0x1d80000, 0, 0xc80000);
  r5 = v12;
  r0 = v12;
  if (r5 != 0)
    goto L_0200962e;
  goto L_020097c4;
L_0200962e:
  Call2((void (*)())Scene_SetActorSpritePriority, r0, 1);
  FIELD_AT(r5, u8, 85) = 0;
  Call2((void (*)())Actor_SetSingleChildFlags, r5, 0);
  Call2((void (*)())Actor_SetPartAttribute, r5, 14);
  Call2((void (*)())Animation_SetStateFlags, r5, 1);
  goto L_020097c4;
L_02009656:
  Func_02000404((s32)gData_0200335a);
  v13 = GameFlag_Test(0x947);
  if (v13 == 0)
    goto L_02009694;
  Call3((void (*)())Func_02003088, 13, 0xd80000, 0x880000);
  v14 = GameFlag_Test(0x946);
  if (v14 == 0)
    goto L_02009694;
  Call3((void (*)())Func_02003088, 12, 0xc80000, 0x880000);
L_02009694:
  Func_02003168((s32)gData_02003264);
  Call2((void (*)())Actor_SetSpritePriority, 12, 2);
  goto L_020097c4;
L_020096a4:
  v15 = GameFlag_Test(0x94b);
  if (v15 != 0) {
    r5 = 3;
    Call6((void (*)())Func_02002fc0, 18, 97, 6, 97, r5, 4);
    Call6((void (*)())Func_02002fc0, 82, 34, 70, 34, r5, 2);
    Call6((void (*)())Func_02002fe8, 7, 36, 1, 1, 7, 35);
  }
  r5 = (s32)Data_02003ccc;
  v16 = Func_02002f98(4, 0, 0, 0);
  r0 = v16;
  r3 = 144;
  goto L_0200970e;
L_020096fe:
  r5 = (s32)Data_02003ccc;
  v17 = Func_02002f98(4, 0, 0, 0);
  r0 = v17;
  r3 = 156;
L_0200970e:
  FIELD_AT(r0, s32, 76) = r3 << 18;
  FIELD_AT(r0, s32, 108) = (s32)Func_020010d4;
  FIELD_AT(r5, s32, 0) = r0;
  FIELD_AT(r0, u8, 85) = 0;
  Call2((void (*)())Scene_SetActorSpritePriority, r0, 3);
  goto L_020097c4;
L_02009728:
  v18 = GameFlag_Test(0x109);
  r0 = v18;
  if (v18 != 0)
    goto L_020097c4;
  Func_020011e0(r0, r1);
  goto L_020097c4;
L_02009770:
  if (r2 != (212))
    goto L_020097c4;
  Call1((void (*)())GameFlag_SetBit, 0x144);
  v19 = GameFlag_Test(0x948);
  if (v19 != 0) {
    Call6((void (*)())Func_02002fc0, 13, 26, 13, 16, 3, 3);
    Call6((void (*)())Func_02002fe8, 13, 26, 3, 3, 13, 16);
  } else {
    Call2((void (*)())Task_Add, (s32)Func_0200137c, 0x480);
  }
  Func_02003168((s32)gData_0200326e);
L_020097c4:
  /* add (s32)stack, #8 */
  return 0;
}

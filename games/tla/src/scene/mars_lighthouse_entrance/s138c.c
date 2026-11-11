#include "types.h"
#include "ram_buffer.h"
#include "version.h"
#include "call.h"
#include "event_inline.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
s32 Event_StartScreenTransition();
s32 Event_WaitTransitionFrames();
s32 Func_020002bc();
s32 Func_02000414();
void Func_02000798();
s32 Func_02000c1c();
s32 Func_02001234();
s32 Func_02002270();
s32 Func_020022d8();
s32 Func_020022e4();
s32 Func_0200246c();
s32 Func_0200248c();
s32 Func_020045f8();
s32 Func_02004ca8();
s32 Func_02004d88();
s32 Func_02004dd8();
extern u8 Data_02006878[];
extern u8 Func_02001254[];
extern u8 Func_020012bc[];
extern u8 Func_02001344[];
extern u8 Func_02001378[];
extern u8 Func_0200196c[];
extern u8 Func_02001988[];
extern u8 Func_020019a4[];
extern u8 gData_02005314[];
extern u8 gData_020053d8[];
extern u8 gData_020053e6[];
extern u8 gData_020053f8[];
extern u8 gData_020053fe[];
extern u8 gData_0200540c[];
extern u8 gData_02005412[];
extern u8 gData_02005418[];
extern u8 gData_02005438[];
extern u8 gData_0200543e[];
extern u8 gData_02005448[];
extern u8 gData_0200544e[];
extern u8 gData_02005454[];
extern u8 gData_0200586c[];
extern u8 gData_0200587c[];
extern u8 gData_020058a8[];
extern u8 gData_020058d4[];
extern u8 gMainRoutineWork[];
extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;

s32 Func_0200138c(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 103 of 549 instructions differ in
   * tla-en. */
  s32 v_r8, v_r7, v_r5, t0, t1, t2, t8, t12, t17, t21;
  s32 t23, t24, t26, t27, t29, t34, t38, t40, t41, t48;
  s32 t51, t52, t55, t56, t57, t72, t73, t74, t78, t81;
  s32 t93, t96, t97, t98, t99, t100, t101, t102, t103, t104;
  s32 t105, t106, t107, t108, t109, t110, t111;

  t0 = FIELD_AT((s32)gMainRoutineWork, s16, 480);
  t1 = FIELD_AT((s32)gMainRoutineWork, s16, 482);
  *(s32 *)(s32)Data_02006878 = 0;
  t2 = (s32)Ram_HeapSlots->eventWork;
  FIELD_AT(t2, s32, 428) = 0x204;
  Event_SetFlag(0x112);
  Func_02002270();
  v_r7 = t1;
  v_r8 = t0;
  if (v_r8 != (s32)&SceneId_Ov6aaRoom1)
    goto L_020093dc;
  Call1((void (*)())Func_02004dd8, 1);
  goto L_020098be;
L_020093dc:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom2)
    goto L_0200944c;
  Call1((void (*)())Func_02004d88, 0);
#if !TLA_PATCH_1
  FieldActor_SetSpritePriority(11, 3);
#endif
  Call1((void (*)())Func_02001234, 10);
  t8 = Event_GetFlag(0xa30);
  if (t8 == 0)
    goto L_02009420;
  MapGrid_CopyCellsLayered(35, 41, 24, 10, 3, 53);
  MapGrid_CopyCellsLayered(35, 105, 24, 10, 3, 117);
L_02009420:
  Call1((void (*)())Func_020002bc, (s32)gData_020053d8);
  t12 = Event_GetFlag(0xa3e);
  if (t12 != 0)
    goto L_02009436;
  goto L_020098be;
L_02009436:
  MapGrid_CopyCellsLayered(50, 125, 3, 3, 39, 85);
  goto L_020098be;
L_0200944c:
  if (v_r8 == (s32)&SceneId_Ov6aaRoom3)
    goto L_02009454;
  goto L_02009680;
L_02009454:
  Call1((void (*)())Func_02004d88, 0);
  Call4((void (*)())Func_020022e4, (s32)gData_020053e6, (s32)gData_020053f8, 0,
        88);
  Call1((void (*)())Func_020002bc, (s32)gData_020053fe);
  t17 = Event_GetFlag(0xa3f);
  if (t17 == 0)
    goto L_020094ae;
  MapGrid_CopyCellsLayered(47, 94, 10, 6, 47, 100);
  MapGrid_CopyCellAttributes(110, 40, 12, 4, 46, 40);
  Actor_SetPosition(18, 0, 0);
  goto L_020094e0;
L_020094ae:
  t21 = Event_GetFlag(0xa3e);
  if (t21 == 0)
    goto L_020094d6;
  Event_SetFlag(0x230);
  t23 = (s32)Actor_Get(18);
  FIELD_AT(t23, u8, 35) |= 2;
  goto L_020094e0;
L_020094d6:
  Actor_SetPosition(18, 0, 0);
L_020094e0:
  t26 = (s32)Actor_Get(20);
  FIELD_AT(t26, u8, 35) |= 2;
  FieldActor_SetSpritePriority(20, 2);
  t29 = Event_GetFlag(0xa47);
  if (t29 == 0)
    goto L_0200953c;
  MapGrid_CopyCellsLayered(28, 92, 8, 8, 28, 100);
  MapGrid_CopyCellAttributes(92, 38, 8, 8, 28, 38);
  Func_02000798();
  Actor_SetPosition(20, 0, 0);
  goto L_02009564;
L_0200953c:
  t34 = Event_GetFlag(0xa48);
  if (t34 == 0)
    goto L_0200955a;
  Func_02000798();
  Event_SetFlag(0x231);
  goto L_02009564;
L_0200955a:
  Actor_SetPosition(20, 0, 0);
L_02009564:
  t38 = Event_GetFlag(0xa4f);
  if (t38 == 0)
    goto L_0200958c;
  Actor_SetPosition(8, 0x1480000, 0x780000);
  t40 = (s32)Actor_Get(8);
  FIELD_AT(t40, u8, 98) = 1;
L_0200958c:
  t41 = Event_GetFlag(0xa30);
  if (t41 == 0)
    goto L_020095a4;
  Actor_SetPosition(19, 0, 0);
  goto L_020095ae;
L_020095a4:
  Actor_SetPosition(15, 0, 0);
L_020095ae:
  if (v_r7 == 20)
    goto L_020095b4;
  goto L_020098be;
L_020095b4:
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  MapGrid_SetShake(0, 0x20000, 0x10000);
  t48 = FIELD_AT((s32)gMainRoutineWork, s32, 532);
  Actor_SetPosition(t48, 0, 0);
  Actor_SetPosition(18, 0, 0);
  t51 = (s32)Actor_Get(18);
  FIELD_AT(t51, u8, 35) |= 2;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  t55 = (s32)Actor_Get(18);
  t56 = FIELD_AT(t55, s16, 100);
  t57 = FIELD_AT(t55, s16, 102);
  Actor_SetPosition(18, (t56 << 16), (t57 << 16));
  Event_PlaySound(0x23d);
  v_r5 = 2;
L_0200963c:
  Call1((void (*)())Func_02000414, 18);
  System_Wait(1);
  v_r5 = (v_r5 - 1);
  if (v_r5 >= 0)
    goto L_0200963c;
  Event_SetFlag(0xa3e);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldEvent_Wait(120);
  FieldEvent_SetExitRequest(20);
  Event_SetFlag(0x109);
  goto L_020098be;
L_02009680:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom4)
    goto L_020096b0;
  Call1((void (*)())Func_02004d88, 0);
  if (v_r7 == 10)
    goto L_02009694;
  if (v_r7 != 1)
    goto L_020096a8;
L_02009694:
  Call4((void (*)())Func_020022e4, (s32)gData_0200540c, (s32)gData_02005412,
        (s32)Func_020019a4, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
L_020096a8:
  Call1((void (*)())Func_020002bc, (s32)gData_02005418);
  goto L_020098be;
L_020096b0:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom5)
    goto L_02009784;
  Call1((void (*)())Func_02004d88, 0);
  t72 = (s32)Actor_Get(12);
  FIELD_AT(t72, u8, 35) |= 2;
  t74 = Event_GetFlag(0xa4d);
  if (t74 == 0)
    goto L_0200970c;
  MapGrid_CopyCellsLayered(12, 95, 8, 6, 12, 101);
  MapGrid_CopyCellAttributes(76, 40, 8, 5, 12, 40);
  Actor_SetPosition(12, 0, 0);
  goto L_0200976e;
L_0200970c:
  t78 = Event_GetFlag(0xa4c);
  if (t78 == 0)
    goto L_02009764;
  Event_SetFlag(0x232);
  goto L_0200976e;
L_02009764:
  Actor_SetPosition(12, 0, 0);
L_0200976e:
  t81 = Event_GetFlag(0xa49);
  if (t81 != 0)
    goto L_0200977e;
  goto L_020098be;
L_0200977e:
  Func_02000c1c();
  goto L_020098be;
L_02009784:
  if (v_r8 == (s32)&SceneId_Ov6aaRoom6)
    goto L_0200978c;
  goto L_020098b2;
L_0200978c:
  Call1((void (*)())Func_02004d88, 0);
  if (v_r7 == 2)
    goto L_020097a2;
  if (v_r7 == 4)
    goto L_020097a2;
  if (v_r7 == 6)
    goto L_020097a2;
  if (v_r7 != 13)
    goto L_020097b6;
L_020097a2:
  Call4((void (*)())Func_020022e4, (s32)gData_02005438, (s32)gData_0200543e,
        (s32)Func_0200196c, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
L_020097b6:
  if ((u32)(v_r7 - 7) <= (u32)1)
    goto L_020097c0;
  if (v_r7 != 11)
    goto L_02009884;
L_020097c0:
  Call4((void (*)())Func_020022e4, (s32)gData_02005448, (s32)gData_0200544e,
        (s32)Func_02001988, 88);
  Call2((void (*)())Func_0200248c, 0, 1);
  if (v_r7 != 11)
    goto L_020097fa;
  Actor_SetScript(11, gData_02005314);
  Func_0200246c(0, 3);
  Task_AddFrame(Func_02001254, 0x480);
  Event_SetFlag(0x17d);
L_020097fa:
  Task_AddFrame(Func_02001378, 0x480);
  t93 = (s32)Actor_Get(11);
  FIELD_AT(t93, s32, 108) = (s32)Func_020012bc;
  Func_020022d8(1);
  Call1((void (*)())Func_020002bc, (s32)gData_02005454);
  t96 = (s32)Actor_Get(20);
  t97 = (s32)Actor_Get(13);
  FIELD_AT(t97, s32, 108) = (s32)Func_02001344;
  t98 = (s32)Actor_Get(14);
  FIELD_AT(t98, s32, 108) = (s32)Func_02001344;
  t99 = (s32)Actor_Get(15);
  FIELD_AT(t99, s32, 108) = (s32)Func_02001344;
  t100 = (s32)Actor_Get(16);
  FIELD_AT(t100, s32, 108) = (s32)Func_02001344;
  t101 = (s32)Actor_Get(17);
  FIELD_AT(t101, s32, 108) = (s32)Func_02001344;
  t102 = (s32)Actor_Get(18);
  FIELD_AT(t102, s32, 108) = (s32)Func_02001344;
  t103 = (s32)Actor_Get(13);
  FIELD_AT(t103, s32, 104) = t96;
  t104 = (s32)Actor_Get(14);
  FIELD_AT(t104, s32, 104) = t96;
  t105 = (s32)Actor_Get(15);
  FIELD_AT(t105, s32, 104) = t96;
  t106 = (s32)Actor_Get(16);
  FIELD_AT(t106, s32, 104) = t96;
  t107 = (s32)Actor_Get(17);
  FIELD_AT(t107, s32, 104) = t96;
  t108 = (s32)Actor_Get(18);
  FIELD_AT(t108, s32, 104) = t96;
L_02009884:
  t109 = (s32)Actor_Get(9);
  FIELD_AT(t109, u8, 35) |= 2;
  t111 = Event_GetFlag(0xa4e);
  if (t111 == 0)
    goto L_020098be;
  Actor_SetPosition(8, 0x580000, 0x2b80000);
  goto L_020098be;
L_020098b2:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom7)
    goto L_020098be;
  Call1((void (*)())Func_02004d88, 0);
L_020098be:
  if (v_r8 != (s32)&SceneId_Ov6aaRoom6)
    goto L_020098d0;
  if (v_r7 != 13)
    goto L_020098d0;
  Event_ClearFlag(0x104);
L_020098d0:
  Call4((void (*)())Func_020045f8, (s32)gData_0200586c, (s32)gData_0200587c,
        (s32)gData_020058a8, (s32)gData_020058d4);
  return 0;
}

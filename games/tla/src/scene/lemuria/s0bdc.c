#include "types.h"
#include "main_routine_work.h"
#include "call.h"
#include "scene_tables.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000560();
void Func_0802b450();
void Actor_SetHorizontalPositionWithTerrain();
void Func_02001cfc();
void Func_02001d24();
void Func_02001d3c();
void Func_02001d4c();
s32 GameFlag_Test();
void Actor_SetSingleChildFlags();
void Actor_SetMovementTypeById();
void Actor_SetSpritePriority();
s32 Actor_GetById();
void Actor_SetModeById();
void Actor_SetWideSprite();
void Task_Add();
void Func_020000e8();
void Func_02000a6c();
void Func_02001b3c();
extern u8 gData_02001d84[];
extern u8 gData_02001e04[];

s32 Func_02000bdc(void) {
  s32 t3, t7, t11, t12, t15, t16, t17, t19, t20, t23, t25, t26, t27, t32, t33,
      t34, t40, t42, t45, t47, t49, t51, t53, t55, v5, v6;

  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x201;
  Task_Add((s32)Func_02000a6c, 0xc80);
  Func_02001cfc(0);
  t3 = Actor_GetById(11);
  Actor_SetSingleChildFlags(t3, 0);
  Actor_SetSpritePriority(9, 3);
  Actor_SetSpritePriority(29, 3);
  t7 = Actor_GetById(21);
  FIELD_AT(t7, s32, 108) = (s32)Func_02001b3c;
  Actor_SetModeById(22, 3);
  Call3(Actor_SetHorizontalPositionWithTerrain, 18, PIXELS(40), PIXELS(952));
  Actor_SetMovementTypeById(18, (s32)&gData_02001d84);
  t11 = Actor_GetById(18);
  v6 = t11;
  t12 = Actor_GetById(19);
  FIELD_AT(v6, s32, 104) = t12;
  FIELD_AT(v6, s32, 108) = (s32)Func_020000e8;
  Call3(Actor_SetHorizontalPositionWithTerrain, 19, PIXELS(88), PIXELS(904));
  Actor_SetMovementTypeById(19, (s32)&gData_02001e04);
  t15 = Actor_GetById(19);
  v6 = t15;
  t16 = Actor_GetById(18);
  FIELD_AT(v6, s32, 104) = t16;
  FIELD_AT(v6, s32, 108) = (s32)Func_020000e8;
  t17 = GameFlag_Test(0x908);
  if (t17 == 0)
    goto L_02008c9c;
  Call3(Actor_SetHorizontalPositionWithTerrain, 30, PIXELS(440), PIXELS(1048));
L_02008c9c:
  t19 = GameFlag_Test(0x964);
  if (t19 == 0)
    goto L_02008cd4;
  t20 = GameFlag_Test(0x969);
  if (t20 != 0)
    goto L_02008cd4;
  Call3(Actor_SetHorizontalPositionWithTerrain, 24, PIXELS(456), PIXELS(520));
  Call3(Actor_SetHorizontalPositionWithTerrain, 25, PIXELS(472), PIXELS(520));
L_02008cd4:
  t23 = GameFlag_Test(0x969);
  if (t23 != 0)
    goto L_02008cee;
  Actor_SetHorizontalPositionWithTerrain(26, 0, 0);
  goto L_02008cf8;
L_02008cee:
  t25 = Actor_GetById(26);
  FIELD_AT(t25, s32, 108) = (s32)Func_02001b3c;
L_02008cf8:
  t26 = GameFlag_Test(0x966);
  if (t26 == 0)
    goto L_02008d2a;
  t27 = GameFlag_Test(0x969);
  if (t27 != 0)
    goto L_02008d2a;
  Call3(Actor_SetHorizontalPositionWithTerrain, 7, PIXELS(56), PIXELS(1016));
  Actor_SetModeById(7, 9);
L_02008d2a:
  Func_02001d24();
  Func_02001d3c(0, 0x962, 8, 9);
  t32 = Actor_GetById(8);
  FIELD_AT(t32, u8, 35) = (FIELD_AT(t32, u8, 35) | 32);
  t33 = Actor_GetById(9);
  v5 = (32 | FIELD_AT(t33, u8, 35));
  FIELD_AT(t33, u8, 35) = v5;
  t34 = Value1(GameFlag_Test, 0x96f);
  if (t34 == 0)
    goto L_02008da6;
  Actor_SetSpritePriority(28, 2);
  Actor_SetSpritePriority(27, 2);
  Call3(Actor_SetHorizontalPositionWithTerrain, 28, PIXELS(840), PIXELS(920));
  Call3(Actor_SetHorizontalPositionWithTerrain, 27, PIXELS(840), PIXELS(936));
  Call4(Func_02001d3c, 1, 0x96f, 28, 27);
L_02008da6:
  t40 = Actor_GetById(28);
  Actor_SetSingleChildFlags(t40, 0);
  t42 = GameFlag_Test(0x963);
  if (t42 == 0)
    goto L_02008dcc;
  Func_02001d4c(10, 1);
  Func_02000560();
L_02008dcc:
  t45 = GameFlag_Test(0x303);
  if (t45 != 0)
    goto L_02008de2;
  Actor_SetWideSprite(67, 0);
L_02008de2:
  t47 = GameFlag_Test(0x304);
  if (t47 != 0)
    goto L_02008df6;
  Actor_SetWideSprite(68, 0);
L_02008df6:
  t49 = GameFlag_Test(0x306);
  if (t49 != 0)
    goto L_02008e0c;
  Actor_SetWideSprite(70, 0);
L_02008e0c:
  t51 = GameFlag_Test(0x307);
  if (t51 != 0)
    goto L_02008e22;
  Actor_SetWideSprite(71, 0);
L_02008e22:
  t53 = GameFlag_Test(0x966);
  if (t53 == 0)
    goto L_02008e44;
  Call6(Func_0802b450, 3, 1, 1, 1, 54, 45);
L_02008e44:
  t55 = Actor_GetById(31);
  v6 = t55;
  FIELD_AT(v6, u8, 85) = 0;
  FIELD_AT(v6, s32, 20) = 0x400000;
  FIELD_AT(v6, s32, 12) = 0x400000;
  return 0;
}

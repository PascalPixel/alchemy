#include "types.h"
#include "adventure.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000560();
void Func_02001b8c();
void Func_02001c2c();
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
  Call2(Task_Add, (s32)Func_02000a6c, 0xc80);
  Call1(Func_02001cfc, 0);
  t3 = Value1(Actor_GetById, 11);
  Call2(Actor_SetSingleChildFlags, t3, 0);
  Call2(Actor_SetSpritePriority, 9, 3);
  Call2(Actor_SetSpritePriority, 29, 3);
  t7 = Value1(Actor_GetById, 21);
  FIELD_AT(t7, s32, 108) = (s32)Func_02001b3c;
  Call2(Actor_SetModeById, 22, 3);
  Call3(Func_02001c2c, 18, 0x280000, 0x3b80000);
  Call2(Actor_SetMovementTypeById, 18, (s32)&gData_02001d84);
  t11 = Value1(Actor_GetById, 18);
  v6 = t11;
  t12 = Value1(Actor_GetById, 19);
  FIELD_AT(v6, s32, 104) = t12;
  FIELD_AT(v6, s32, 108) = (s32)Func_020000e8;
  Call3(Func_02001c2c, 19, 0x580000, 0x3880000);
  Call2(Actor_SetMovementTypeById, 19, (s32)&gData_02001e04);
  t15 = Value1(Actor_GetById, 19);
  v6 = t15;
  t16 = Value1(Actor_GetById, 18);
  FIELD_AT(v6, s32, 104) = t16;
  FIELD_AT(v6, s32, 108) = (s32)Func_020000e8;
  t17 = Value1(GameFlag_Test, 0x908);
  if (t17 == 0)
    goto L_02008c9c;
  Call3(Func_02001c2c, 30, 0x1b80000, 0x4180000);
L_02008c9c:
  t19 = Value1(GameFlag_Test, 0x964);
  if (t19 == 0)
    goto L_02008cd4;
  t20 = Value1(GameFlag_Test, 0x969);
  if (t20 != 0)
    goto L_02008cd4;
  Call3(Func_02001c2c, 24, 0x1c80000, 0x2080000);
  Call3(Func_02001c2c, 25, 0x1d80000, 0x2080000);
L_02008cd4:
  t23 = Value1(GameFlag_Test, 0x969);
  if (t23 != 0)
    goto L_02008cee;
  Call3(Func_02001c2c, 26, 0, 0);
  goto L_02008cf8;
L_02008cee:
  t25 = Value1(Actor_GetById, 26);
  FIELD_AT(t25, s32, 108) = (s32)Func_02001b3c;
L_02008cf8:
  t26 = Value1(GameFlag_Test, 0x966);
  if (t26 == 0)
    goto L_02008d2a;
  t27 = Value1(GameFlag_Test, 0x969);
  if (t27 != 0)
    goto L_02008d2a;
  Call3(Func_02001c2c, 7, 0x380000, 0x3f80000);
  Call2(Actor_SetModeById, 7, 9);
L_02008d2a:
  Func_02001d24();
  Call4(Func_02001d3c, 0, 0x962, 8, 9);
  t32 = Value1(Actor_GetById, 8);
  FIELD_AT(t32, u8, 35) = (FIELD_AT(t32, u8, 35) | 32);
  t33 = Value1(Actor_GetById, 9);
  v5 = (32 | FIELD_AT(t33, u8, 35));
  FIELD_AT(t33, u8, 35) = v5;
  t34 = Value1(GameFlag_Test, 0x96f);
  if (t34 == 0)
    goto L_02008da6;
  Call2(Actor_SetSpritePriority, 28, 2);
  Call2(Actor_SetSpritePriority, 27, 2);
  Call3(Func_02001c2c, 28, 0x3480000, 0x3980000);
  Call3(Func_02001c2c, 27, 0x3480000, 0x3a80000);
  Call4(Func_02001d3c, 1, 0x96f, 28, 27);
L_02008da6:
  t40 = Value1(Actor_GetById, 28);
  Call2(Actor_SetSingleChildFlags, t40, 0);
  t42 = Value1(GameFlag_Test, 0x963);
  if (t42 == 0)
    goto L_02008dcc;
  Call2(Func_02001d4c, 10, 1);
  Func_02000560();
L_02008dcc:
  t45 = Value1(GameFlag_Test, 0x303);
  if (t45 != 0)
    goto L_02008de2;
  Call2(Actor_SetWideSprite, 67, 0);
L_02008de2:
  t47 = Value1(GameFlag_Test, 0x304);
  if (t47 != 0)
    goto L_02008df6;
  Call2(Actor_SetWideSprite, 68, 0);
L_02008df6:
  t49 = Value1(GameFlag_Test, 0x306);
  if (t49 != 0)
    goto L_02008e0c;
  Call2(Actor_SetWideSprite, 70, 0);
L_02008e0c:
  t51 = Value1(GameFlag_Test, 0x307);
  if (t51 != 0)
    goto L_02008e22;
  Call2(Actor_SetWideSprite, 71, 0);
L_02008e22:
  t53 = Value1(GameFlag_Test, 0x966);
  if (t53 == 0)
    goto L_02008e44;
  Call6(Func_02001b8c, 3, 1, 1, 1, 54, 45);
L_02008e44:
  t55 = Value1(Actor_GetById, 31);
  v6 = t55;
  FIELD_AT(v6, u8, 85) = 0;
  FIELD_AT(v6, s32, 20) = 0x400000;
  FIELD_AT(v6, s32, 12) = 0x400000;
  return 0;
}

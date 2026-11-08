#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02001c3c();
void Func_02001d24();
s32 GameFlag_Test();
void Actor_ArmCallback();
void Actor_SetSpritePriority();
s32 Actor_GetById();
void Actor_SetActionCallback();
void Actor_SetModeById();
void Func_02000a98();
extern u8 gData_02001d54[];

s32 Func_020007d4(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 42 of 224 instructions
   * differ in tla-en. */
  s32 t1, t2, t3, t4, t9, t10, t11, t23, t24, t25, t37, t39, t41, t44, t50, t51,
      t53, t56, t58, t60;

  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1ac), s32, 0) = 0x100;
  t1 = GameFlag_Test(SPLIT_CONSTANT(0x7a8, 255));
  if (t1 == 0)
    goto L_020088aa;
  t2 = GameFlag_Test(SPLIT_CONSTANT(0x810, 255));
  if (t2 != 0)
    goto L_02008836;
  t3 = Actor_GetById(24);
  FIELD_AT(t3, s32, 108) = (s32)Func_02000a98;
  t4 = Actor_GetById(19);
  FIELD_AT(t4, s32, 108) = (s32)Func_02000a98;
  Actor_SetSpritePriority(20, 2);
  Actor_SetSpritePriority(14, 3);
  Actor_SetSpritePriority(16, 3);
  Actor_SetSpritePriority(10, 1);
  goto L_02008a0a;
L_02008836:
  t9 = Value1(Actor_GetById, 14);
  FIELD_AT(t9, s32, 108) = (s32)Func_02000a98;
  t10 = Actor_GetById(20);
  FIELD_AT(t10, s32, 108) = (s32)Func_02000a98;
  t11 = Actor_GetById(22);
  FIELD_AT(t11, s32, 108) = (s32)Func_02000a98;
  Actor_SetSpritePriority(13, 2);
  Actor_SetSpritePriority(19, 3);
  Actor_SetSpritePriority(23, 2);
  Actor_SetSpritePriority(24, 3);
  Actor_SetSpritePriority(27, 2);
  Actor_SetSpritePriority(28, 3);
  Actor_SetSpritePriority(29, 1);
  Actor_SetSpritePriority(30, 3);
  Actor_SetSpritePriority(31, 3);
  Actor_SetSpritePriority(12, 3);
  Actor_SetSpritePriority(10, 3);
  goto L_02008a0a;
L_020088aa:
  t23 = GameFlag_Test(SPLIT_CONSTANT(0x800, 171));
  if (t23 == 0)
    goto L_0200898a;
  t24 = Actor_GetById(14);
  FIELD_AT(t24, s32, 108) = (s32)Func_02000a98;
  t25 = Actor_GetById(20);
  FIELD_AT(t25, s32, 108) = (s32)Func_02000a98;
  Actor_SetSpritePriority(13, 2);
  Actor_SetSpritePriority(19, 3);
  Actor_SetSpritePriority(23, 3);
  Actor_SetSpritePriority(24, 3);
  Actor_SetSpritePriority(22, 2);
  Actor_SetSpritePriority(27, 2);
  Actor_SetSpritePriority(28, 3);
  Actor_SetSpritePriority(29, 1);
  Actor_SetSpritePriority(30, 3);
  Actor_SetSpritePriority(31, 3);
  Actor_SetSpritePriority(12, 3);
  t37 = Actor_GetById(26);
  Actor_SetActionCallback(t37, 9);
  t39 = GameFlag_Test(SPLIT_CONSTANT(0x800, 181));
  if (t39 != 0)
    goto L_02008944;
  Actor_SetModeById(27, 5);
L_02008944:
  t41 = GameFlag_Test(SPLIT_CONSTANT(0x7b8, 255));
  if (t41 == 0)
    goto L_02008962;
  Actor_SetModeById(30, 6);
  Actor_SetModeById(31, 6);
L_02008962:
  t44 = GameFlag_Test(SPLIT_CONSTANT(0x800, 185));
  if (t44 == 0)
    goto L_02008a0a;
  Call3(Func_02001c3c, 12, PIXELS(56), PIXELS(328));
  Actor_ArmCallback(12, FACING_EAST, 0);
  goto L_02008a0a;
L_0200898a:
  Actor_SetSpritePriority(13, 2);
  Actor_SetSpritePriority(23, 2);
  Actor_SetSpritePriority(10, 3);
  t50 = Actor_GetById(24);
  FIELD_AT(t50, s32, 108) = (s32)Func_02000a98;
  t51 = GameFlag_Test(SPLIT_CONSTANT(0x800, 179));
  if (t51 == 0)
    goto L_020089c8;
  Call3(Func_02001c3c, 14, PIXELS(536), PIXELS(760));
L_020089c8:
  t53 = GameFlag_Test(SPLIT_CONSTANT(0x800, 180));
  if (t53 == 0)
    goto L_020089e0;
  Func_02001c3c(25, 0, 0);
L_020089e0:
  Func_02001d24((s32)&gData_02001d54);
  t56 = Actor_GetById(26);
  Actor_SetActionCallback(t56, 9);
  t58 = Actor_GetById(27);
  Actor_SetActionCallback(t58, 9);
  t60 = Actor_GetById(28);
  Actor_SetActionCallback(t60, 9);
L_02008a0a:
  return 0;
}

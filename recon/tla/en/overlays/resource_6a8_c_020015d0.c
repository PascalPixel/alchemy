// Under -mthumb-slow-loads (2026-10-26): differ 2 of 172; left: as recorded
// below.
// Near miss, differ 2 of 172 (4 without the two barriers). The two barriers put
// each flag store ahead of the next call's arguments, as the native has them,
// but the first one moves an earlier pair: where the 1 for the first byte store
// is also kept in r8, the native copies it before stepping to the field (mov
// r8, r3; adds r0, #34) and this form after. One-pass blocks in their place end
// the CSE block and the 1 is rebuilt later (differ 6 to 8); a third barrier
// before that store moves another pair (differ 4).
#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 Func_02004464();
s32 Func_020045ec();
s32 Actor_SetSpeedParameters();
s32 Actor_GetById();
s32 Actor_SetSpritePriority();
s32 Actor_SetPositionAndReset();
s32 FieldEvent_WaitFrames();
s32 Actor_ArmCallback();
s32 Func_0200463c();
s32 Func_02004584();
s32 Func_020044cc();
s32 Func_02004444();
s32 Func_0200455c();
s32 Func_02004404();
s32 Func_0200442c();
s32 GameFlag_SetBit();
s32 Func_0200446c();
extern u8 gAdventure[];

void Func_020015d0(void) {
  s32 t12;
  s32 t4;
  s32 t6;
  s32 t9;
  s32 t13;
  s32 t16;
  s32 t28;

  Func_02004464();
  Call1(Func_020045ec, 0);
  Call3(Actor_SetSpeedParameters, FIELD_AT(gAdventure, s32, 532),
        0x20000, 0x10000);
  Call3(Actor_SetSpeedParameters, 0x14, 0x20000, 0x10000);
  t4 = Value1(Actor_GetById, 0x14);
  FIELD_AT((t4 + 0x22), u8, 0) = 1;
  Call2(Actor_SetSpritePriority, 0x14, 3);
  t6 = Value1(Actor_GetById, FIELD_AT(gAdventure, s32, 532));
  FIELD_AT((t6 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t6 + 0x5a), u8, 0));
  /* FAKEMATCH: the barrier keeps the flag store ahead of the next call's
   * arguments; without it the store follows the first two (one reordered row).
   */
  asm volatile("");
  Call3(Actor_SetPositionAndReset, FIELD_AT(gAdventure, s32, 532),
        0x198, 0x146);
  Call1(FieldEvent_WaitFrames, 1);
  t9 = Value1(Actor_GetById, FIELD_AT(gAdventure, s32, 532));
  FIELD_AT((t9 + 0x5a), u8, 0) = (FIELD_AT((t9 + 0x5a), u8, 0) | 1);
  /* FAKEMATCH: the barrier keeps the flag store ahead of the next call's
   * arguments; without it the store follows the first (one reordered row). */
  asm volatile("");
  Call3(Actor_ArmCallback, FIELD_AT(gAdventure, s32, 532), 0xc000, 0);
  Call1(Func_0200463c, 0xa1);
  t12 = Func_02004584();
  FIELD_AT(t12, s32, 12) = (FIELD_AT(t12, s32, 12) + 0x20000);
  t13 = Value1(Actor_GetById, 0x14);
  FIELD_AT((t13 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t13 + 0x5a), u8, 0));
  Call3(Actor_SetPositionAndReset, 0x14, 0x198, 0xc8);
  Call1(FieldEvent_WaitFrames, 1);
  t16 = Value1(Actor_GetById, 0x14);
  FIELD_AT((t16 + 0x5a), u8, 0) = (FIELD_AT((t16 + 0x5a), u8, 0) | 1);
  Call3(Func_020044cc, 0x14, 0, 0);
  Call1(Func_0200463c, 0xb0);
  Call3(Func_02004444, 0x10000, 0x20000, 0x10000);
  Call1(FieldEvent_WaitFrames, 0x1e);
  Call3(Func_02004444, -1, -1, 0xe666);
  Call1(Func_0200463c, 0x7f);
  Call1(FieldEvent_WaitFrames, 0x1e);
  Call1(Func_0200463c, 0x92);
  Call1(FieldEvent_WaitFrames, 0x14);
  Call3(Func_0200455c, FIELD_AT(gAdventure, s32, 532), 0x105, 0x28);
  Call2(Actor_SetSpritePriority, 0x14, 2);
  t28 = Value1(Actor_GetById, 0x14);
  FIELD_AT((t28 + 0x23), u8, 0) = (1 | FIELD_AT((t28 + 0x23), u8, 0));
  Call6(Func_02004404, 0x48, 3, 0x45, 3, 3, 3);
  Call6(Func_02004404, 0x30, 0x24, 0x45, 8, 3, 2);
  Call6(Func_0200442c, 0x48, 0xd, 1, 3, 0x46, 0x14);
  Call1(GameFlag_SetBit, 0xa2d);
  Func_0200446c();
}

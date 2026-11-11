#include "mars_lighthouse_room_m.h"
#include "version.h"
#include "scene_tables.h"
#include "io_reg.h"
#include "event_inline.h"

void Func_02003228(void);
void Func_0200080c(void);

void Func_020010e4(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgMankindWithThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_02001154(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgFishWithCold, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_020011c4(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgDragonsBurningWith, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_02001234(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgBirdOnThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_020012a4(s32 x, s32 z, s32 kind) {
  struct Actor *object;

  /* FAKEMATCH(temporary): z shifted ahead of the call; as z << 16 in its
   * arguments, 2 of 53 instructions differ in tla-en. */
  z <<= 16;
  object = (struct Actor *)Import_ActorCreate(403, x << 16, 0, z);

  if (object != 0) {
    Actor_SetSingleChildFlags(object, 0);
    Func_02003118(object, 3);
    if (kind != 0) {
      ((struct MarsLighthouseRoomMAnimation *)object->animation)->layer = 2;
      Func_02003120(object, gData_0200387c);
    } else {
      ((struct MarsLighthouseRoomMAnimation *)object->animation)->layer = 1;
      object->velocityX = (((u32)(Random16Far() * 8) >> 16) - 4) << 16;
      object->velocityY = PIXELS(4);
      Func_02003120(object, gData_02003900);
    }
  }
}

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f) {
  u32 i;
  s32 px;
  s32 pz;

  Sound_Play(144);
  Func_020012a4(x, z + 12, 1);
  for (i = 0; i < 3; i++) {
    px = x + ((u32)(Random16Far() * 12) >> 16) - 6;
    pz = z + ((u32)(Random16Far() * 12) >> 16) - 6;
    Func_020012a4(px, pz, 0);
  }
  wait(4);
  Map_CopyCellPictureIndicesRect(c, d, e + 64, f, 1, 1);
}

/// No one holding item 222: MsgIfYouDo, once (flag 0xa2f). Someone holding it
/// (and flag 0xa30 set, from patch 1): MsgYouWhoHold and the
/// message after it, the camera moves on and Func_0200080c runs, then flag
/// 0xa2e is set. Each opens as Func_020010e4 does.
void Func_020013a0(void) {
  if (PartyInventory_FindOwner(222) == -1) {
    if (!Event_GetFlag(0xa2f)) {
      u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

      Func_020031e0();
      Event_ResolvePendingActions(0);
      FieldCamera_SetSpeed(0x20000, 0x4000);
      FieldCamera_MoveTo(PIXELS(224), PIXELS(64), PIXELS(216), 1);
      Func_02003228();
      FieldEvent_Wait(20);
      /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
       * call; as a direct call, 12 of 155 instructions differ in tla-en. */
      PaletteFade_ApplySource(0x10000, 0);
      BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
      BattleEffect_StartBufferInterpolation(20);
      wait(20);
      *color = 0x7fff;
      wait(1);
      Func_0803ce64((s32)MsgIfYouDo, 0, 4);
      BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
      BattleEffect_StartBufferInterpolation(20);
      wait(20);
      Event_SetFlag(0xa2f);
      Func_020031e8();
    }
  }
#if !TLA_PATCH_1
  else {
#else
  else if (Event_GetFlag(0xa30)) {
#endif
    u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

    Func_020031e0();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0x20000, 0x4000);
    /* FAKEMATCH(wrapper): forwarded so this branch builds its own positions;
     * as a direct call, 16 of 155 instructions differ in tla-en. */
    FieldCamera_MoveTo(PIXELS(224), PIXELS(64), PIXELS(216), 1);
    Func_02003228();
    FieldEvent_Wait(20);
    /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
     * call, as in the branch above; as a direct call at this site, 22 of 155
     * instructions differ in tla-en. */
    PaletteFade_ApplySource(0x10000, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(20);
    *color = 0x7fff;
    wait(1);
    Func_0803ce64((s32)MsgYouWhoHold, 0, 4);
    wait(10);
    Func_0803ce64((s32)MsgYouWhoHold + 1, 0, 3);
    FieldCamera_MoveTo(PIXELS(224), PIXELS(88), PIXELS(216), 1);
    Func_02003228();
    FieldEvent_Wait(20);
    Sound_Play(560);
    Func_0200080c();
    FieldEvent_Wait(20);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(40);
    Event_SetFlag(0xa2e);
    Func_020031e8();
  }
}

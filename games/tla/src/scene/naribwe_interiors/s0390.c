#include "version.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

s32 Func_020011a0(s32 *unit, s32 *slot);
struct UnitData *Unit_Get(s32 unit);
const u8 *Func_020010b8(s32 item);
void Func_020010b0(s32 item, s32 a);
void Actor_WaitForAnimationChange(u32 actor);
void Func_02000920(void);
void Func_020007b4(void);
void Func_02000760(void);
void Func_02000838(void);
void Func_02000878(void);
void Party_AdjustCoins(s32 coins);

struct UnitData {
  u8 filler[216];
  u16 items[15];
};

/// Object 10 steps to (x, 96) without turning, then dances (mode 5).
#define DANCE_TO(x)                                                            \
  (Actor_Get(10)->actionFlags &= ~1, Actor_WalkTo(10, x, 96),              \
   Actor_SetAnimation(10, 5))

void Func_02000390(void) {
  s32 slot, unit;
  struct UnitData *unitData;
  const u8 *message, *refuse, *info;
#if !TLA_PATCH_1
  s32 item;
#else
  u32 item;
#endif
  u8 kind;
  s32 rawKind;

  FieldEvent_SetMessageId(MsgVeryWellShow);
  Event_ShowMessage(10, 0);
  FieldEvent_Wait(30);
ask:
#if !TLA_PATCH_1
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  /* FAKEMATCH(temporary): item also holds this call's result; as a separate
   * test of the call, 23 of 374 instructions differ in tla-ja. */
  item = Func_020011a0(&unit, &slot);
  if (item == -1) {
#else
  /* FAKEMATCH(barrier): the empty asm clobbering sfp; as no asm, 63 of 376
   * instructions differ in tla-en. */
  asm("" : : : "sfp");
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  if (Func_020011a0(&unit, &slot) == -1) {
#endif
    refuse = MsgShallIGive;
    FieldEvent_SetMessageId(refuse);
    Event_OpenMessage(10, 0);
    if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) != 1)
      goto ask;
    FieldEvent_SetMessageId(refuse + 1);
    Event_ShowMessage(10, 0);
    return;
  }
  unitData = Unit_Get(unit);
  Func_020010b8(unitData->items[slot]);
  Actor_SetItemIcon(Actor_Get(18), unitData->items[slot]);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through
   * Call3; as a direct call, 70 of 376 instructions differ in tla-en. */
  Actor_SetPosition(18, 0x2a00000, 0x680000);
  FieldEvent_Wait(60);
  Func_020010b0(unitData->items[slot], 2);
  Message_ShowCentered(MsgPlacedOnThe, 1);
  message = MsgAreYouPrepared;
  FieldEvent_SetMessageId(message);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) != 1)
    goto read;
  Actor_SetPosition(18, 0, 0);
  Func_020010b0(unitData->items[slot], 2);
  Message_ShowCentered(message + 1, 1);
  FieldEvent_SetMessageId(message + 2);
  Event_OpenMessage(10, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) != 1)
    goto ask;
  FieldEvent_SetMessageId(message + 3);
  Event_ShowMessage(10, 0);
  return;
read:
  FieldEvent_SetMessageId(message + 4);
  Event_ShowMessage(10, 0);
  /* FAKEMATCH(wrapper): BattleEffect_ApplyColorToSourceBuffer called through
   * Call2; as a direct call, 59 of 376 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x40250d, 1);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(120);
  Event_PlaySound(11);
  Actor_SetAnimation(10, 5);
  Actor_WaitForAnimationChange(10);
  /* FAKEMATCH(wrapper): DANCE_TO calls through Call3 at its five sites; as a
   * direct call, 67 of 376 instructions differ in tla-en. */
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  FieldActor_Jump(10, 4, 90);
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  FieldActor_Jump(10, 4, 90);
  DANCE_TO(0x2a0);
  FieldActor_Jump(10, 4, 30);
  FieldActor_Jump(10, 4, 30);
  Actor_SetAnimation(10, 5);
  Actor_SetAnimation(10, 0);
  Event_PlaySound(11);
  PaletteFade_ApplyTarget(0x10000, 1);
  PaletteFade_Interpolate(30);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through
   * Call3; as a direct call, 62 of 376 instructions differ in tla-en. */
  Actor_SetPosition(20, 0x2a00000, 0x680000);
  Event_PlaySound(0x90);
  Actor_SetAnimation(20, 2);
  Actor_WaitForAnimationChange(20);
  Actor_SetPosition(20, 0, 0);
  FieldEvent_Wait(4);
  /* FAKEMATCH(wrapper): Actor_SetHorizontalPositionWithTerrain called through
   * Call3; as a direct call, 2 of 376 instructions differ in tla-en. */
  Actor_SetPosition(19, 0x2a00000, 0x680000);
  Actor_SetAnimation(19, 4);
  Actor_WaitForAnimationChange(19);
  FieldEvent_Wait(15);
  Actor_SetPosition(19, 0, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 258, 0);
  FieldEvent_Wait(60);
  Event_PlaySound(10);
  Actor_ShowEmote(10, 0x100, 60);
  Event_ShowMessage(10, 0);
  unitData = Unit_Get(unit);
  info = Func_020010b8(unitData->items[slot]);
#if !TLA_PATCH_1
  item = unitData->items[slot];
#else
  item = unitData->items[slot] & 0x1ff;
#endif
  if ((u16)(item - 0x1b7) <= 2) {
    Func_02000920();
  } else if ((u16)item == 326) {
    FieldEvent_SetMessageId(message + 29);
    Event_ShowMessage(10, 0);
    if (!Event_GetFlag(0x940)) {
      FieldEvent_SetMessageId(message + 30);
      Event_ShowMessage(10, 0);
    } else {
      FieldEvent_SetMessageId(message + 31);
      Event_ShowMessage(10, 0);
    }
  } else {
    rawKind = info[2];
    kind = rawKind;
    if (kind == 6)
      Func_020007b4();
    else if (kind == 1)
      Func_02000760();
    else if ((u8)((u8)rawKind - 2) <= 3)
      Func_02000838();
    else
      Func_02000878();
  }
  Actor_SetPosition(18, 0, 0);
  Party_AdjustCoins(-20);
}

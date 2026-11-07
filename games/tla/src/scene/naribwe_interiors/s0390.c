#include "version.h"
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "call.h"

void Func_02001138(const u8 *message);
void Func_02001148(s32 speaker, s32 flags);
void FieldEvent_Wait(s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
s32 Func_020011a0(s32 *unit, s32 *slot);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Func_02001188(void);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
struct UnitData *Unit_Get(s32 unit);
const u8 *Func_020010b8(s32 item);
struct Actor *Actor_GetById(u32 id);
void Func_020010a0(struct Actor *object, s32 item);
void Func_02001108(s32 actor, s32 x, s32 z);
void Func_020010b0(s32 item, s32 a);
void FieldMessage_Show(const u8 *message, s32 a);
void Func_02001170(s32 a, s32 b);
s32 Func_02001168(s32 a, s32 b);
s32 Func_02001178(s32 frames);
void Func_020011a8(s32 cue);
void Actor_WaitForAnimationChange(u32 actor);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
s32 Func_02001160(s32 actor, s32 emote, s32 frames);
void Func_02000920(void);
s32 GameFlag_Test(s32 flag);
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
  (Actor_GetById(10)->actionFlags &= ~1,                                       \
   Call3((void (*)())Actor_ResetAndSetPositionInMode2, 10, x, 96),             \
   Actor_SetModeById(10, 5))

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

  Func_02001138(MsgVeryWellShow);
  Func_02001148(10, 0);
  FieldEvent_Wait(30);
ask:
#if !TLA_PATCH_1
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  /* FAKEMATCH(temporary): item also holds this call's result; as a separate
   * test of the call, 23 of 374 instructions differ in tla-ja. */
  item = Func_020011a0(&unit, &slot);
  if (item == -1) {
#else
  /* FAKEMATCH(barrier): the empty asm clobbering sfp; as no asm, 63 of 376
   * instructions differ in tla-en. */
  asm("" : : : "sfp");
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  if (Func_020011a0(&unit, &slot) == -1) {
#endif
    refuse = MsgShallIGive;
    Func_02001138(refuse);
    FieldMessage_ShowAtActor(10, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) != 1)
      goto ask;
    Func_02001138(refuse + 1);
    Func_02001148(10, 0);
    return;
  }
  unitData = Unit_Get(unit);
  Func_020010b8(unitData->items[slot]);
  Func_020010a0(Actor_GetById(18), unitData->items[slot]);
  /* FAKEMATCH(wrapper): Func_02001108 called through Call3; as a direct call,
   * 70 of 376 instructions differ in tla-en. */
  Call3((void (*)())Func_02001108, 18, 0x2a00000, 0x680000);
  FieldEvent_Wait(60);
  Func_020010b0(unitData->items[slot], 2);
  FieldMessage_Show(MsgPlacedOnThe, 1);
  message = MsgAreYouPrepared;
  Func_02001138(message);
  FieldMessage_ShowAtActor(10, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) != 1)
    goto read;
  Func_02001108(18, 0, 0);
  Func_020010b0(unitData->items[slot], 2);
  FieldMessage_Show(message + 1, 1);
  Func_02001138(message + 2);
  FieldMessage_ShowAtActor(10, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) != 1)
    goto ask;
  Func_02001138(message + 3);
  Func_02001148(10, 0);
  return;
read:
  Func_02001138(message + 4);
  Func_02001148(10, 0);
  /* FAKEMATCH(wrapper): Func_02001170 called through Call2; as a direct call,
   * 59 of 376 instructions differ in tla-en. */
  Call2((void (*)())Func_02001170, 0x10000, 0);
  FieldEvent_Wait(30);
  Func_02001168(0x40250d, 1);
  Func_02001178(30);
  FieldEvent_Wait(120);
  Func_020011a8(11);
  Actor_SetModeById(10, 5);
  Actor_WaitForAnimationChange(10);
  /* FAKEMATCH(wrapper): DANCE_TO calls through Call3 at its five sites; as a
   * direct call, 67 of 376 instructions differ in tla-en. */
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  Actor_Jump(10, 4, 90);
  DANCE_TO(664);
  FieldEvent_Wait(30);
  DANCE_TO(680);
  Actor_Jump(10, 4, 90);
  DANCE_TO(0x2a0);
  Actor_Jump(10, 4, 30);
  Actor_Jump(10, 4, 30);
  Actor_SetModeById(10, 5);
  Actor_SetModeById(10, 0);
  Func_020011a8(11);
  Func_02001168(0x10000, 1);
  Func_02001178(30);
  /* FAKEMATCH(wrapper): Func_02001108 called through Call3; as a direct call,
   * 62 of 376 instructions differ in tla-en. */
  Call3((void (*)())Func_02001108, 20, 0x2a00000, 0x680000);
  Func_020011a8(0x90);
  Actor_SetModeById(20, 2);
  Actor_WaitForAnimationChange(20);
  Func_02001108(20, 0, 0);
  FieldEvent_Wait(4);
  /* FAKEMATCH(wrapper): Func_02001108 called through Call3; as a direct call, 2
   * of 376 instructions differ in tla-en. */
  Call3((void (*)())Func_02001108, 19, 0x2a00000, 0x680000);
  Actor_SetModeById(19, 4);
  Actor_WaitForAnimationChange(19);
  FieldEvent_Wait(15);
  Func_02001108(19, 0, 0);
  FieldEvent_Wait(30);
  Func_02001160(gMainRoutineWork.currentOwner, 258, 0);
  FieldEvent_Wait(60);
  Func_020011a8(10);
  Func_02001160(10, 0x100, 60);
  Func_02001148(10, 0);
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
    Func_02001138(message + 29);
    Func_02001148(10, 0);
    if (!GameFlag_Test(0x940)) {
      Func_02001138(message + 30);
      Func_02001148(10, 0);
    } else {
      Func_02001138(message + 31);
      Func_02001148(10, 0);
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
  Func_02001108(18, 0, 0);
  Party_AdjustCoins(-20);
}

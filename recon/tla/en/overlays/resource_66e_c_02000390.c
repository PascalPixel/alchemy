// Draft, differ 37 of 376 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 40 of 376 (tla-en); older figures below predate it.
// Matches: differ 0 (tla-ja, 2026-11-02)
// Japanese-only: the listing's section sits under .ifdef TLA_EDITION_JA, the
// other editions link games/tla/src/scene/naribwe_interiors/s0390.c, so measure
// with --edition tla-ja (plain tla-en compares this against that file's form and
// reads differ 42, which means nothing). This is that file's
// C with the two Japanese differences: the item is not masked with 0x1ff, and
// the Msg ids stay pool loads. Plain numbers split into movs chains, so they
// are symbols here, but the Japanese catalog has no such names (the pool holds
// 0x1ec1, 0x1ec4, 0x1ebb, 0x1ec2): adopting this needs them in ja.po or a
// decision on spelling numbers. Every device was removed and measured in
// tla-ja, and all are needed; the tags below give the figures. The compare
// (lsls #16 against 326 << 16) needs item's upper bits unknown to combine:
// u16, s16, (s16)item and & 0xffff all fold it (23 of 374), hence the reuse.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern u8 MsgVeryWellShow[], MsgShallIGive[], MsgPlacedOnThe[], MsgAreYouPrepared[];

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
  s32 item;
  u8 kind;
  s32 rawKind;

  Func_02001138(MsgVeryWellShow);
  Func_02001148(10, 0);
  FieldEvent_Wait(30);
ask:
  /* FAKEMATCH(barrier): the empty asm clobbering sfp; as no asm, 63 of 374
   * instructions differ in tla-ja. */
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  /* FAKEMATCH(temporary): item also holds this call's result; as a separate
   * test of the call, 23 of 374 instructions differ in tla-ja. */
  item = Func_020011a0(&unit, &slot);
  if (item == -1) {
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
   * 70 of 374 instructions differ in tla-ja. */
  Call3((void (*)())Func_02001108, 18, 0x2a00000, 0x680000);
  FieldEvent_Wait(60);
  Func_020010b0(unitData->items[slot], 2);
  message = MsgAreYouPrepared;
  FieldMessage_Show(MsgPlacedOnThe, 1);
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
   * 59 of 374 instructions differ in tla-ja. */
  Call2((void (*)())Func_02001170, 0x10000, 0);
  FieldEvent_Wait(30);
  Func_02001168(0x40250d, 1);
  Func_02001178(30);
  FieldEvent_Wait(120);
  Func_020011a8(11);
  Actor_SetModeById(10, 5);
  Actor_WaitForAnimationChange(10);
  /* FAKEMATCH(wrapper): DANCE_TO calls through Call3 at its five sites; as a
   * direct call, 67 of 374 instructions differ in tla-ja. */
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
   * 62 of 374 instructions differ in tla-ja. */
  Call3((void (*)())Func_02001108, 20, 0x2a00000, 0x680000);
  Func_020011a8(0x90);
  Actor_SetModeById(20, 2);
  Actor_WaitForAnimationChange(20);
  Func_02001108(20, 0, 0);
  FieldEvent_Wait(4);
  /* FAKEMATCH(wrapper): Func_02001108 called through Call3; as a direct call, 2
   * of 374 instructions differ in tla-ja. */
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
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 39 of 376 instructions differ
   * in tla-en. */
  do {
    item = unitData->items[slot];
  } while (0);
  if ((u16)(item - 0x1b7) <= 2) {
    Func_02000920();
  } else if ((u16)item == 326) {
    Func_02001138(message + 29);
    Func_02001148(10, 0);
    if (!GameFlag_Test(0x940)) {
      ((s32 (*)())Func_02001138)(message + 30);
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


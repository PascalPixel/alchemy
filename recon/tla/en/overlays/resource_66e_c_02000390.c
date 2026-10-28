// Draft, differ 131 of 376 (was 150): the Naribwe fortune teller's reading of
// an item (object 10): the player picks an item to show (Func_020011a0, its
// unit and slot; cancelling asks MsgShallIGive until refused); the item is set
// on the table (object 18, Func_020010a0) and named (MsgPlacedOnThe); she
// asks MsgAreYouPrepared, and a no takes the item back and asks whether to
// show another (MsgAreYouPrepared + 2); on a yes the screen darkens, she dances
// to her table and back three times while objects 20 and 19 flash, and reads
// the item: items 0x1b7 to 0x1b9 by Func_02000920, item 326 with its own lines
// (MsgAreYouPrepared + 29, then + 30 or + 31 by flag 0x940), others by their
// kind (6 Func_020007b4, 1 Func_02000760, 2 to 5 Func_02000838, else
// Func_02000878); the reading costs 20 coins. The Japanese edition is the same
// with its own message numbers. The calls whose constants the native builds
// afresh go through CallN. Left: the native saves only r5 to r7 (the unit's
// data in r5, MsgAreYouPrepared in r6, &gAdventure.currentOwner in r7),
// forms &unit and &slot at each call and reads the item as base plus offset,
// where the draft keeps them in r8 to r11 and adds the slot to the base first.
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "text/msg_ids.h"
#include "lib/call.h"

void Func_02001138(const u8 *message);
void Func_02001148(s32 speaker, s32 flags);
void FieldEvent_Wait(s32 frames);
void Motion_SetModeAndWaitAnimation(s32 actor, s32 mode);
s32 Func_020011a0(s32 *unit, s32 *slot);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Func_02001188(void);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
u8 *Unit_Get(s32 unit);
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

#define ITEM(unitData, slot) (*(u16 *)&(unitData)[(slot) * 2 + 216])

/// Object 10 steps to (x, 96) without turning, then dances (mode 5).
#define DANCE_TO(x)                                                            \
  (Actor_GetById(10)->actionFlags &= ~1,                                      \
   Actor_ResetAndSetPositionInMode2(10, x, 96),                         \
   Actor_SetModeById(10, 5))

void Func_02000390(void) {
  s32 slot, unit;
  u8 *unitData;
  const u8 *message, *info;
  u32 item;
  s32 kind;

  Func_02001138(MsgVeryWellShow);
  Func_02001148(10, 0);
  FieldEvent_Wait(30);
ask:
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  if (Func_020011a0(&unit, &slot) == -1) {
    Func_02001138(MsgShallIGive);
    FieldMessage_ShowAtActor(10, 0);
    if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) != 1)
      goto ask;
    message = MsgShallIGive + 1;
    goto bye;
  }
  unitData = Unit_Get(unit);
  Func_020010b8(ITEM(unitData, slot));
  Func_020010a0(Actor_GetById(18), ITEM(unitData, slot));
  Call3((void (*)())Func_02001108, 18, 0x2a00000, 0x680000);
  FieldEvent_Wait(60);
  Func_020010b0(ITEM(unitData, slot), 2);
  FieldMessage_Show(MsgPlacedOnThe, 1);
  message = MsgAreYouPrepared;
  Func_02001138(message);
  FieldMessage_ShowAtActor(10, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) != 1)
    goto read;
  Func_02001108(18, 0, 0);
  Func_020010b0(ITEM(unitData, slot), 2);
  FieldMessage_Show(message + 1, 1);
  Func_02001138(message + 2);
  FieldMessage_ShowAtActor(10, 0);
  if (Event_AskYesNoWithActorAnimation(Func_02001188(), 0) != 1)
    goto ask;
  message += 3;
bye:
  Func_02001138(message);
  Func_02001148(10, 0);
  return;
read:
  Func_02001138(message + 4);
  Func_02001148(10, 0);
  Call2((void (*)())Func_02001170, 0x10000, 0);
  FieldEvent_Wait(30);
  Func_02001168(0x40250d, 1);
  Func_02001178(30);
  FieldEvent_Wait(120);
  Func_020011a8(11);
  Actor_SetModeById(10, 5);
  Actor_WaitForAnimationChange(10);
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
  Call3((void (*)())Func_02001108, 20, 0x2a00000, 0x680000);
  Func_020011a8(0x90);
  Actor_SetModeById(20, 2);
  Actor_WaitForAnimationChange(20);
  Func_02001108(20, 0, 0);
  FieldEvent_Wait(4);
  Call3((void (*)())Func_02001108, 19, 0x2a00000, 0x680000);
  Actor_SetModeById(19, 4);
  Actor_WaitForAnimationChange(19);
  FieldEvent_Wait(15);
  Func_02001108(19, 0, 0);
  FieldEvent_Wait(30);
  Func_02001160(gAdventure.currentOwner, 258, 0);
  FieldEvent_Wait(60);
  Func_020011a8(10);
  Func_02001160(10, 0x100, 60);
  Func_02001148(10, 0);
  unitData = Unit_Get(unit);
  info = Func_020010b8(ITEM(unitData, slot));
  item = ITEM(unitData, slot) & 0x1ff;
  if ((u16)(item - 0x1b7) <= 2) {
    Func_02000920();
  } else if (item == 326) {
    Func_02001138(message + 29);
    Func_02001148(10, 0);
    if (!GameFlag_Test(0x940))
      Func_02001138(message + 30);
    else
      Func_02001138(message + 31);
    Func_02001148(10, 0);
  } else {
    kind = info[2];
    if (kind == 6)
      Func_020007b4();
    else if (kind == 1)
      Func_02000760();
    else if ((u8)(kind - 2) <= 3)
      Func_02000838();
    else
      Func_02000878();
  }
  Func_02001108(18, 0, 0);
  Party_AdjustCoins(-20);
}

#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "call.h"

extern u8 MsgAreWeSetting[];

void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void wait(s32 frames);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001060(s32 id, s32 x, s32 z);
void Func_02001070(s32 id, s32 target);
void Func_02001098(u8 *message);
void Func_020010a8(s32 id, s32 value);
void Func_020010b8(s32 id, s32 value);
void Func_020010f0(s32 value);
void Func_02001108(s32 value);

/// Object 18 comes to the party leader and asks whether to set out, as in
/// Func_02000120; after a no the leader steps back.
void Func_02000640(void) {
  struct Actor *object;

  Func_02001010();
  /* FAKEMATCH: a one-pass block ends the call's scheduling block: without it
   * the adventure offset is built before its address is loaded (differ 2; an
   * empty asm does the same, found by alchemy permute). */
  do {
    Func_02001108(0);
  } while (0);
  Func_02001070(18, gAdventure.currentOwner);
  wait(1);
  Actor_ArmCallback(gAdventure.currentOwner, 0xa000, 0);
  Call3(Actor_SetSpeedParameters, 18, 0xcccc, 0x6666);
  Actor_SetPositionAndReset(18, 532, 248);
  Func_020010b8(18, 0x3000);
  Func_02001098(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Func_020010a8(18, 0);
    gAdventure.ownerOverride = 1;
    if (Value1(GameFlag_Test, 0x9d0)) {
      GameFlag_SetBit(0x8df);
      GameFlag_SetBit(0x9b2);
    }
    Func_020010f0(4);
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_020010a8(18, 0);
    Call3(Actor_SetSpeedParameters, 18, 0x19999, 0xcccc);
    Actor_SetModeById(18, 2);
    object = Actor_GetById(gAdventure.currentOwner);
    if (object != NULL)
      Actor_ResetAndSetPosition(18, *(s16 *)((u8 *)object + 10),
                                       *(s16 *)((u8 *)object + 18));
    Actor_WaitForMoveById(18);
    Func_02001060(18, 0, 0);
    Call3(Actor_SetSpeedParameters, gAdventure.currentOwner, 0xcccc,
          0x6666);
    Actor_SetPositionAndReset(gAdventure.currentOwner, 528, 276);
    Func_02001018();
  }
}

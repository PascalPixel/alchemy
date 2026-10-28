#include "types.h"
#include "ram_buffer.h"
#include "adventure.h"
#include "actor.h"
#include "call.h"

extern u8 MsgAreWeSetting[];

void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Actor_SetModeById(u32 id, s32 mode);
s32 FieldMessage_ShowAtActor(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_ResetAndSetPosition(s32 id, s32 x, s32 z);
void Actor_WaitForMoveById(s32 id);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001060(s32 id, s32 x, s32 z);
void Func_02001098(u8 *message);
void Func_020010a8(s32 id, s32 value);
void Func_020010f0(s32 value);
void Func_02001108(s32 value);

/// Object 18 asks whether to set out: a yes sets the owner override and, once
/// flag 0x9d0 is set, flags 0x8df and 0x9b2; a no counts the refusal and
/// object 18 walks back to the party leader.
void Func_02000120(void) {
  struct Actor *object;

  Func_02001010();
  Func_02001108(0);
  Func_02001098(MsgAreWeSetting);
  FieldMessage_ShowAtActor(18, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Actor_SetModeById(18, 3);
    Func_020010a8(18, 0);
    gAdventure.ownerOverride = 1;
    /* FAKEMATCH: the flag test forwards through call.h's Value1; called
     * directly, the adventure's offsets take r3 instead of r2 (differ 22,
     * found by alchemy permute). */
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
    GameFlag_SetBit(0x201);
    Func_02001018();
  }
}

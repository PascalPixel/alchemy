#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"

extern u8 MsgAreWeSetting[];

void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
s32 UiText_OpenMessageAtObject(s32 speaker, s32 flags);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_ResetAndSetPosition(s32 id, s32 x, s32 z);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void Func_02001010(void);
void Func_02001018(void);
void Func_02001060(s32 id, s32 x, s32 z);
void Func_02001098(u8 *message);
void Func_020010a8(s32 id, s32 value);
void Func_020010f0(s32 value);
void Func_02001108(s32 value);

// Object 18 asks whether to set out: a yes sets the owner override and, once
// flag 0x9d0 is set, flags 0x8df and 0x9b2; a no counts the refusal and
// object 18 walks back to the party leader.
void Func_02000120(void) {
  struct ObjectRuntime *object;

  Func_02001010();
  Func_02001108(0);
  Func_02001098(MsgAreWeSetting);
  UiText_OpenMessageAtObject(18, 0);
  if (Event_AskYesNoWithActorAnimation(gPartyState.currentOwner, 0) == 0) {
    Object_SetModeById(18, 3);
    Func_020010a8(18, 0);
    gPartyState.ownerOverride = 1;
    /* FAKEMATCH: the flag test forwards through call.h's Value1; called
     * directly, the party state's offsets take r3 instead of r2 (differ 22,
     * found by alchemy permute). */
    if (Value1(GameFlag_Test, 0x9d0)) {
      GameFlag_SetBit(0x8df);
      GameFlag_SetBit(0x9b2);
    }
    Func_020010f0(4);
  } else {
    (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
    Func_020010a8(18, 0);
    Call3(ObjectMotion_SetSpeedParameters, 18, 0x19999, 0xcccc);
    Object_SetModeById(18, 2);
    object = Object_GetById(gPartyState.currentOwner);
    if (object != NULL)
      ObjectMotion_ResetAndSetPosition(18, *(s16 *)((u8 *)object + 10),
                                       *(s16 *)((u8 *)object + 18));
    ObjectMotion_CommitCurrentPositionAndActivate(18);
    Func_02001060(18, 0, 0);
    GameFlag_SetBit(0x201);
    Func_02001018();
  }
}

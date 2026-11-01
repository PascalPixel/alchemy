// Matches: differ 0 (tla-en, 2026-11-01)
// An Islets scene, the payoff of a gift: actor 9
// speaks (MsgAtLongLast) and walks over, the object is set down by its
// spot, item 455 leaves the inventory, a byte of block 88 is set to 99, the
// leader is launched over to the guide, the guide steps aside, the object
// moves again and the screen fades out to Func_02003f7c with flag 0xaa5
// set. The Japanese edition sets the transition word to 521, the others
// to 513. The call layer (call.h) marks the calls that keep their constants
// to themselves; the direct calls share 0xcccc and 0x19999 with the gift's
// stores. Japanese (the listing's other arm) is not measured here.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "edition.h"
#include "call.h"

#if (LANGUAGE == LANGUAGE_JAPANESE)
#define ISLETS_GIFT_TRANSITION 521
#else
#define ISLETS_GIFT_TRANSITION 513
#endif

struct ActionScript {
  u32 words[9];
};

extern struct ActionScript gData_02004004;
extern u8 MsgAtLongLast[];
struct Actor *Actor_GetById(u32 id);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02003fbc(void);
void Func_02003fc4(void);
s32 Func_02003f54(s32 id, s32 emote, s32 frames);
void Func_02000df8(void);
void Func_02001278(void);
void Actor_SetMovementTypeAndWaitById(s32 id, void *script);
void Func_02003f24(const u8 *message);
void Func_02003f2c(s32 id, s32 flags);
void Func_02003f3c(s32 id, s32 angle);
void Actor_SetSpeedParameters(u32 id, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(u32 id, s32 x, s32 z);
void Actor_ResetAndSetPosition(u32 id, s32 x, s32 z);
void Actor_ArmCallback(s32 id, s32 angle, s32 frames);
void Func_02003de4(struct Actor *object, s32 mode);
void Func_02003e0c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003e14(struct Actor *object);
void PartyInventory_Remove(s32 item);
u8 *HeapSlot_AllocEwram(s32 block, s32 size);
void Motion_SetModeAndWaitAnimation(s32 id, s32 mode);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_SetPositionAndCommit(u32 id, s32 x, s32 z);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetModeById(u32 id, s32 mode);
void FieldEvent_Wait(s32 frames);
void Func_020034ec(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003f7c(s32 countdown);
void GameFlag_SetBit(s32 flag);

void Func_0200129c(struct Actor *gift) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *guide = Actor_GetById(9);
  struct ActionScript script = gData_02004004;

  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02003fbc();
  Func_02003f54(9, 264, 20);
  guide->update = Func_02000df8;
  Actor_SetMovementTypeAndWaitById(9, &script);
  guide->update = 0;
  Func_02003f24(MsgAtLongLast);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call2((void (*)())Func_02003f3c, 9, 0x5000);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x19999, 0xcccc);
  guide->update = Func_02000df8;
  Call3((void (*)())Actor_SetPositionAndReset, 9, 296, 452);
  guide->update = 0;
  Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
  gift->speedLimit = 0x19999;
  gift->acceleration = 0xcccc;
  gift->update = Func_02000df8;
  Func_02003de4(gift, 2);
  Call4((void (*)())Func_02003e0c, (s32)gift, 0x1160000, 0, 0x1c40000);
  Func_02003e14(gift);
  Func_02003de4(gift, 1);
  gift->update = 0;
  PartyInventory_Remove(455);
  Call2((void (*)())Func_02003f2c, 9, 0);
  HeapSlot_AllocEwram(88, 664)[661] = 99;
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  leader->update = Func_02000df8;
  Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 312, 446);
  leader->update = 0;
  Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 296, 452);
  leader->update = Func_02001278;
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetModeById(9, 3);
  FieldEvent_Wait(20);
  Call2((void (*)())Func_02003f2c, 9, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0xcccc, 0x6666);
  gift->speedLimit = 0xcccc;
  gift->acceleration = 0x6666;
  guide->update = Func_02000df8;
  Call3((void (*)())Actor_ResetAndSetPosition, 9, 288, 538);
  FieldEvent_Wait(20);
  Func_02003fc4();
  Func_020034ec();
  gift->update = Func_02000df8;
  Func_02003de4(gift, 2);
  Call4((void (*)())Func_02003e0c, (s32)gift, 0x1200000, 0, 0x21a0000);
  FieldEvent_Wait(20);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = ISLETS_GIFT_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003f7c(3);
  GameFlag_SetBit(0xaa5);
}

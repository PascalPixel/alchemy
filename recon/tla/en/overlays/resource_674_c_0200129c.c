// Draft, differ 174 of 234: an Islets scene, the payoff of a gift: actor 9
// speaks (MsgAtLongLast) and walks over, the object is set down by its
// spot, item 455 leaves the inventory, a byte of block 88 is set to 99, the
// leader is launched over to the guide, the guide steps aside, the object
// moves again and the screen fades out to Func_02003f7c with flag 0xaa5
// set. The Japanese edition sets the transition word to 521, the others
// to 513. Left: the native mixes the call layer (each -1 and the emote's arguments built at the call) with direct calls that share 0, 0xcccc, 0x19999 and 0x5000 in high registers; registers differ from the start (the leader in r7, the guide in r11).
#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "edition.h"

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
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  struct Actor *guide = Actor_GetById(9);
  struct ActionScript script = gData_02004004;

  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  Func_02003fbc();
  Func_02003f54(9, 264, 20);
  guide->update = Func_02000df8;
  Actor_SetMovementTypeAndWaitById(9, &script);
  guide->update = 0;
  Func_02003f24(MsgAtLongLast);
  Func_02003f2c(9, 0);
  Func_02003f2c(9, 0);
  Func_02003f3c(9, 0x5000);
  Actor_SetSpeedParameters(9, 0x19999, 0xcccc);
  guide->update = Func_02000df8;
  Actor_SetPositionAndReset(9, 296, 452);
  guide->update = 0;
  Actor_ArmCallback(9, 0x5000, 0);
  gift->acceleration = 0xcccc;
  gift->update = Func_02000df8;
  gift->speedLimit = 0x19999;
  Func_02003de4(gift, 2);
  Func_02003e0c(gift, 0x1160000, 0, 0x1c40000);
  Func_02003e14(gift);
  Func_02003de4(gift, 1);
  gift->update = 0;
  PartyInventory_Remove(455);
  Func_02003f2c(9, 0);
  HeapSlot_AllocEwram(88, 664)[661] = 99;
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x19999, 0xcccc);
  leader->update = Func_02000df8;
  Actor_SetPositionAndReset(gAdventure.currentOwner, 312, 446);
  leader->update = 0;
  Actor_Jump(gAdventure.currentOwner, 6, 0);
  Actor_SetPositionAndCommit(gAdventure.currentOwner, 296, 452);
  leader->update = Func_02001278;
  Actor_SetPartAttribute(Actor_GetById(gAdventure.currentOwner), 15);
  Actor_SetModeById(9, 3);
  FieldEvent_Wait(20);
  Func_02003f2c(9, 0);
  Actor_SetSpeedParameters(9, 0xcccc, 0x6666);
  gift->acceleration = 0x6666;
  guide->update = Func_02000df8;
  gift->speedLimit = 0xcccc;
  Actor_ResetAndSetPosition(9, 288, 538);
  FieldEvent_Wait(20);
  Func_02003fc4();
  Func_020034ec();
  gift->update = Func_02000df8;
  Func_02003de4(gift, 2);
  Func_02003e0c(gift, 0x1200000, 0, 0x21a0000);
  FieldEvent_Wait(20);
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = ISLETS_GIFT_TRANSITION;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003f7c(3);
  GameFlag_SetBit(0xaa5);
}

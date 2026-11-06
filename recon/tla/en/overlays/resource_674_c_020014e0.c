// Near miss, differ 2 of 569 (tla-en, 2026-11-06): one Motion call's order.
// Left: in the 0xaa5 branch, Motion_SetModeAndWaitAnimation loads the leader
// id before its 3 (native: movs r1, #3 first); called directly or by Call2,
// Value2 for the allocation, a block local for it, an s8 or u8 return type
// all keep that order.
// Overlay 674's Func_020014e0, written from its listing. With item 455 in the
// party: with flag 513 set, object 9 says MsgOhYouRe and asks; on answer 0 the
// leader steps to 302 and back while objects 163 and 14 are made
// (Func_02003df4), otherwise MsgOhYouRe - 1 is shown. Without the flag object
// 9 says MsgIMSoVery, the same steps run, object 9 hops and asks; a nonzero
// answer sets flag 513. Without the item, with flag 0xaa5 set object 9 asks
// MsgWantToGo; on answer 0 the event work's halfword 0x1c4 is counted, objects
// 9 and 10 move off and 513 is stored at the event work's 0x1ac (tla-ja's
// listing stores 521 there). Otherwise object 9 says MsgIMSoVeryVery.
// Calls with constants go through call.h, as in the other Islets drafts;
// called directly, nearby calls share 0xcccc, 424 and 0x1280000.
// Func_02003f54 and Motion_SetModeAndWaitAnimation are called directly:
// through Call3 or Call2 the actor id is loaded first (differ 8 with both).
// The FAKEMATCH(temporary) below: the native keeps Func_02000df8 in r7, the
// register a takes later, with the owner address in r6. Written plainly the
// owner address takes r7 and the hook and a r6 (differ 94). Tried without
// effect: a hook local, an owner pointer local, declaration order, a = 0 moved
// within the other branch, one Func_0200129c call reached by goto. A 0 default
// for a early in that branch gives differ 30 (the hook then in r9, 0 in r7).
// The 0xaa5 branch's leader is its own local: shared with the first branch's
// it takes r8 and the IWRAM base r6.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "call.h"

extern const u8 MsgOhYouRe[], MsgIMSoVery[], MsgWantToGo[], MsgIMSoVeryVery[];
extern const u8 gData_020044f0[];
void Func_02003e64(void);
void Func_02003e6c(void);
void Func_02003fa4(s32 skip);
s32 PartyInventory_FindOwner(s32 item);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(u32 id);
void Actor_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void Actor_Jump(s32 actor, s32 speed, s32 frames);
void Func_02003f3c(s32 actor, s32 angle);
void Func_02003f24(const u8 *message);
void Func_02003f2c(s32 speaker, s32 flags);
void Func_02003fc4(void);
void Func_02003fbc(void);
s32 Event_AskYesNoWithActorAnimation(s32 actor, s32 flags);
void FieldEvent_Wait(s32 frames);
struct Actor *Func_02003df4(s32 type, s32 x, s32 y, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02003de4(struct Actor *object, s32 a);
void Func_02003dec(struct Actor *object, const u8 *script);
void Func_02003dfc(struct Actor *object);
s32 Func_02003f54(s32 actor, s32 emote, s32 frames);
void Func_0200129c(struct Actor *object);
void Func_02000df8(void);
void Func_02001278(void);
void *HeapSlot_AllocEwram(s32 slot, s32 size);
void Motion_SetModeAndWaitAnimation(u32 objectId, s32 action);
void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void Actor_ResetAndSetPositionInMode2(u32 objectId, s32 x, s32 z);
void Func_020034ec(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02003f7c(s32 a);

#define STEP(actor, x, z)                                                      \
  (Actor_GetById(actor)->actionFlags &= ~1,                                    \
   Call3((void (*)())Actor_SetPositionAndReset, actor, x, z),                  \
   Call1((void (*)())FieldEvent_Wait, 1),                                      \
   Actor_GetById(actor)->actionFlags |= 1)

#define SPAWN(a, b, answer)                                                    \
  {                                                                            \
    a = (struct Actor *)Value4((s32 (*)())Func_02003df4, 163, 0x1280000, 0,    \
                               0x1a80000);                                     \
    b = (struct Actor *)Value4((s32 (*)())Func_02003df4, 14, 0x1280000, 0,     \
                               0x1a80000);                                     \
    if (a != 0) {                                                              \
      a->motionFlags = answer;                                                 \
      Actor_SetSingleChildFlags(a, 0);                                         \
    }                                                                          \
    if (b != 0) {                                                              \
      b->scaleY = 0x20000;                                                     \
      Actor_SetSingleChildFlags(b, 0);                                         \
      Func_02003de4(b, 1);                                                     \
      Func_02003dec(b, gData_020044f0);                                        \
    }                                                                          \
  }

void Func_020014e0(void) {
  struct Actor *leader, *a, *b, *other;
  const u8 *message;
  s32 answer;

  Func_02003e64();
  Call1((void (*)())Func_02003fa4, 0);
  if (PartyInventory_FindOwner(455) != -1) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (GameFlag_Test(513) != 0) {
      Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
            0xcccc, 0x6666);
      /* FAKEMATCH(temporary): the form kept; as leader->update = Func_02000df8,
       * 94 of 569 instructions differ in tla-en. */
      a = (struct Actor *)Func_02000df8;
      leader->update = (void (*)(void))a;
      Call3((void (*)())Actor_SetPositionAndReset,
            gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
            0x4000, 0);
      Call2((void (*)())Func_02003f3c, 9, 0xd000);
      message = MsgOhYouRe;
      Func_02003f24(message);
      Call2((void (*)())Func_02003f2c, 9, 0);
      Func_02003fc4();
      answer =
          Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
      if (answer == 0) {
        Call2((void (*)())Func_02003f3c, gMainRoutineWork.currentOwner, 0x8000);
        Call3((void (*)())Actor_SetSpeedParameters,
              gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
        leader->update = (void (*)(void))a;
        STEP(gMainRoutineWork.currentOwner, 302, 424);
        SPAWN(a, b, answer)
        STEP(gMainRoutineWork.currentOwner, 318, 424);
        leader->update = 0;
        Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
              0x6000, 0);
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Func_02003f24(message - 1);
        goto refused;
      }
    } else {
      Func_02003f24(MsgIMSoVery);
      Call2((void (*)())Func_02003f2c, 9, 0);
      Func_02003f54(gMainRoutineWork.currentOwner, 262, 20);
      Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
            0xcccc, 0x6666);
      leader->update = Func_02000df8;
      // Default: a is set for real when object 163 is made.
      a = 0;
      Call3((void (*)())Actor_SetPositionAndReset,
            gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Call2((void (*)())Func_02003f3c, gMainRoutineWork.currentOwner, 0x8000);
      Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
            0x19999, 0xcccc);
      leader->update = Func_02000df8;
      STEP(gMainRoutineWork.currentOwner, 302, 424);
      SPAWN(a, b, 0)
      STEP(gMainRoutineWork.currentOwner, 318, 424);
      leader->update = 0;
      Call3((void (*)())Actor_ArmCallback, gMainRoutineWork.currentOwner,
            0x4000, 0);
      Func_02003f54(9, 256, 20);
      Call2((void (*)())Func_02003f3c, 9, 0xb000);
      other = Actor_GetById(9);
      other->update = Func_02000df8;
      Call3((void (*)())Actor_Jump, 9, 4, 20);
      other->update = 0;
      Call2((void (*)())Func_02003f2c, 9, 0);
      Call2((void (*)())Func_02003f3c, 9, 0xd000);
      Call2((void (*)())Func_02003f2c, 9, 0);
      Func_02003fc4();
      if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
          0) {
        Func_0200129c(a);
      } else {
        Func_02003fbc();
        Func_02003f54(9, 265, 0);
        Call2((void (*)())Func_02003f2c, 9, 0);
        Call3((void (*)())Actor_ArmCallback, 9, 0x5000, 0);
        Call2((void (*)())Func_02003f3c, gMainRoutineWork.currentOwner, 0x8000);
        leader->update = Func_02000df8;
        Call3((void (*)())Actor_SetPositionAndReset,
              gMainRoutineWork.currentOwner, 302, 424);
        leader->update = 0;
        if (a != 0)
          Func_02003dfc(a);
        Call1((void (*)())GameFlag_SetBit, 513);
      }
    }
  } else if (GameFlag_Test(0xaa5)) {
    Call3((void (*)())Actor_SetAngleToward, 9, gMainRoutineWork.currentOwner,
          0);
    Func_02003f24(MsgWantToGo);
    Call2((void (*)())Func_02003f2c, 9, 0);
    Func_02003fc4();
    if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
        0) {
      struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
      Func_02003fbc();
      (*(u16 *)((u8 *)Ram_HeapSlots->eventWork + 0x1c4))++;
      Call2((void (*)())Func_02003f2c, 9, 0);
      ((u8 *)HeapSlot_AllocEwram(88, 664))[0x295] = 99;
      Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
      Call2((void (*)())Func_02003f3c, 9, 0x5000);
      Call3((void (*)())Actor_SetSpeedParameters, gMainRoutineWork.currentOwner,
            0x19999, 0xcccc);
      Call3((void (*)())Actor_Jump, gMainRoutineWork.currentOwner, 6, 0);
      Call3((void (*)())Actor_SetPositionAndCommit,
            gMainRoutineWork.currentOwner, 296, 452);
      leader->update = Func_02001278;
      Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
      Call2((void (*)())Actor_SetModeById, 9, 3);
      Call1((void (*)())FieldEvent_Wait, 20);
      Call2((void (*)())Func_02003f2c, 9, 0);
      Func_02003fc4();
      Call3((void (*)())Actor_SetSpeedParameters, 9, 0xcccc, 0x6666);
      Call3((void (*)())Actor_SetSpeedParameters, 10, 0xcccc, 0x6666);
      Actor_GetById(9)->update = Func_02000df8;
      Call3((void (*)())Actor_ResetAndSetPosition, 9, 288, 538);
      Call1((void (*)())FieldEvent_Wait, 40);
      Func_02003fc4();
      Func_020034ec();
      Actor_GetById(10)->update = Func_02000df8;
      Call3((void (*)())Actor_ResetAndSetPositionInMode2, 10, 288, 538);
      Call1((void (*)())FieldEvent_Wait, 20);
      *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Func_02003fbc();
      Call1((void (*)())Func_02003f7c, 3);
    } else {
      Func_02003fbc();
    refused:
      Call2((void (*)())Func_02003f2c, 9, 0);
      Call2((void (*)())Func_02003f3c, 9, 0x5000);
    }
  } else {
    Func_02003f24(MsgIMSoVeryVery);
    Call2((void (*)())Func_02003f2c, 9, 0);
  }
  Func_02003e6c();
}

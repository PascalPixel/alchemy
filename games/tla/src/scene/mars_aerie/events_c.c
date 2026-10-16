#include "mars_aerie.h"
#include "event_work.h"

extern const u8 MsgWhatSTheMatter[];
struct ObjectRuntime *Func_0200573c(void);
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void Func_020053e4();
void Func_020054e4();
void Func_020055ac();
void Func_020055fc();
void Func_0200567c();
void Func_020056ec();
void Func_02005724();
void Func_02005734();
void Func_020057dc();
void GameFlag_SetBit();
void Event_AskYesNoWithActorAnimation();
void Motion_CamBounds();
void Motion_SetModeAndWaitAnimation();
void ObjectMotion_Launch();
void ObjectMotion_SetActionVariant();
void ObjectMotion_SetPositionAndCommit();
void Object_SetModeById();
void Object_SetPositionAndResetMotion();
void Party_RemoveOwnerRestored();
void UiText_OpenMessageAtObject(s32 actor, s32 flags);
void WaitFrames();
extern u8 gData_02006b70[];
extern u8 gData_02006c20[];

void Func_02005714(s32 actor, s32 a, s32 b);

static __inline__ void Func_02005714_(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: pinned, the second and third arguments load before the actor, as
   * the native does at every call here (one reordered row per call without). */
  register s32 second asm("r1") = a;
  /* FAKEMATCH: the third argument, pinned with the second. */
  register s32 third asm("r2") = b;

  Func_02005714(actor, second, third);
}

void Func_0200298c(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  object->collisionFlags = 0;
  object->motionFlags = 0;
  ObjectMotion_SetActionVariant(id, 1);
}

// The scene that opens with MsgWhatSTheMatter; either answer to its prompt
// steps the event's message number.
void Func_020029ac(void) {
  ((void (*)())Func_0200560c)();
  Call1((void (*)())Func_02005784, 0);
  Call4((void (*)())Motion_CamBounds, -1, -1, -1, 0);
  Call1((void (*)())WaitFrames, 1);
  Call3((void (*)())Func_0200567c, gPartyState.currentOwner, 0x1fa0000,
        0x1180000);
  Call1((void (*)())WaitFrames, 1);
  ((void (*)())Event_StartScreenTransition)();
  ((void (*)())Event_WaitTransitionFrames)();
  Call2((void (*)())ObjectMotion_SetActionVariant, 3, 1);
  Call2((void (*)())Func_020053e4, (s32)Object_GetById(7), (s32)gData_02006c20);
  Call2((void (*)())Func_020053e4, (s32)Object_GetById(3), (s32)gData_02006b70);
  Call1((void (*)())Battle_WaitMode0, 0x78);
  Call1((void (*)())Func_020054e4, (s32)Object_GetById(7));
  Call1((void (*)())Func_020054e4, (s32)Object_GetById(3));
  Call1((void (*)())Battle_WaitMode0, 0x28);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call2((void (*)())Func_02005704, 6, 0xc000);
  Call1((void (*)())Func_020056dc, (s32)MsgWhatSTheMatter);
  Call2((void (*)())Func_020056f4, 6, 0);
  Call2((void (*)())Func_02005704, 2, 0xa000);
  Call2((void (*)())Func_020056f4, 0x8002, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 3, 0x2000, 0x14);
  Func_02005714_(3, 0x109, 0x14);
  Call2((void (*)())Func_020056f4, 0x1003, 0);
  Call2((void (*)())Func_02005704, 0, 0xe000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call2((void (*)())Func_020056f4, 0x2007, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x8000,
        0);
  Call2((void (*)())Func_02005704, 5, 0xe000);
  Func_02005714_(5, 0x107, 0);
  Call3((void (*)())ObjectMotion_Launch, 5, 2, 0xa);
  Call3((void (*)())ObjectMotion_Launch, 5, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x6000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0xa000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 0, 0x8000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 1, 0x8000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 2, 0x8000, 0);
  Call2((void (*)())Func_02005704, 3, 0x8000);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call2((void (*)())Object_SetModeById, 7, 4);
  Call2((void (*)())Func_020056f4, 0x2007, 0);
  Func_02005714_(5, 0x109, 0x14);
  Call3((void (*)())Func_020056ec, 0x8005, 0, 0x28);
  Call2((void (*)())Func_020056f4, 0x2006, 0);
  Call2((void (*)())Func_02005704, 5, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
  Call2((void (*)())Func_020056f4, 0x8005, 0);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 5, 0x19999, 0xcccc);
  Call2((void (*)())Object_SetModeById, 5, 5);
  Call3((void (*)())ObjectMotion_SetPositionAndCommit, 5, 0x1cc, 0x120);
  Call2((void (*)())Object_SetModeById, 5, 0x16);
  Call2((void (*)())Func_020056f4, 5, 0);
  Func_02005714_(0x13, 0x109, 0);
  Call1((void (*)())Func_020057dc, 0x4e);
  Call2((void (*)())Func_02005724, 0x3333, 0x666);
  Call4((void (*)())Motion_CamBounds, 0x2000000, -1, 0x1400000, 1);
  ((void (*)())Func_02005734)();
  Call2((void (*)())Func_020056f4, 0x2013, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x4000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x4000,
        0);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x2000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 0, 0x6000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 1, 0x6000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 2, 0x6000, 0);
  Call2((void (*)())Func_02005704, 3, 0x6000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x13, 4);
  Call3((void (*)())Func_020056ec, 0x2013, 0, 0x14);
  Call2((void (*)())Object_SetModeById, 5, 1);
  Call1((void (*)())Battle_WaitMode0, 0x14);
  Call2((void (*)())Func_02005704, 5, 0x2000);
  Call2((void (*)())Func_020056f4, 5, 0);
  Call3((void (*)())Func_020056ec, 0x2013, 0, 0x14);
  Call2((void (*)())Func_020056f4, 5, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 0x13, 3);
  Call2((void (*)())Func_020056f4, 0x2013, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 5, 0, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x6000,
        0);
  Call3((void (*)())ObjectMotion_ArmCallback, 0, 0, 0x28);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 1, 0xcccc, 0x6666);
  Object_GetById(1)->actionFlags &= 0xfe;
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 1, 0x21e, 0x148);
  Call1((void (*)())Battle_WaitMode0, 1);
  Object_GetById(1)->actionFlags |= 1;
  Call2((void (*)())Func_02005724, 0xcccc, 0x1999);
  Func_0200573c()->motionFlags = 0;
  Call4((void (*)())Motion_CamBounds, 0x21c0000, 0x4c0000, 0x1540000, 1);
  ((void (*)())Func_02005734)();
  Call2((void (*)())Func_02005704, 1, 0x4000);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 1, 0x21e, 0x154);
  Call3((void (*)())ObjectMotion_Launch, 1, 2, 0xa);
  Call3((void (*)())ObjectMotion_Launch, 1, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x5001, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 1, 0x6000, 0x28);
  Call3((void (*)())ObjectMotion_ArmCallback, 1, 0x2000, 0x14);
  Call3((void (*)())ObjectMotion_Launch, 1, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x5001, 0);
  Call2((void (*)())Func_02005724, 0x4cccc, 0x9999);
  Call4((void (*)())Motion_CamBounds, 0x1de0000, 0x600000, 0x1380000, 1);
  ((void (*)())Func_02005734)();
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0x4000, 0x28);
  Call2((void (*)())Func_02005704, 6, 0x8000);
  Func_02005714_(6, 0x103, 0x14);
  Call2((void (*)())Func_02005704, 6, 0x6000);
  Call3((void (*)())ObjectMotion_Launch, 6, 4, 0xa);
  Call2((void (*)())Func_020056f4, 0x6006, 0);
  Call4((void (*)())Motion_CamBounds, 0x1e00000, -1, 0x1140000, 1);
  ((void (*)())Func_02005734)();
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xe000, 0x14);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0xa000, 0x14);
  Call2((void (*)())Func_02005704, 7, 0xc000);
  Call2((void (*)())Func_020056f4, 0x8007, 0);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 7, 4);
  Call2((void (*)())Func_020056f4, 0x8007, 0);
  Call2((void (*)())Func_020056f4, 0x5000, 0);
  Call4((void (*)())Motion_CamBounds, 0x2000000, -1, 0x1380000, 1);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x2000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, gPartyState.currentOwner, 0x2000,
        0);
  Call3((void (*)())ObjectMotion_ArmCallback, 3, 0x6000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 0x13, 0xd000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 7, 0x2000, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 1, 0xc000, 0x28);
  Call2((void (*)())Func_02005704, 0, 0xc000);
  Call3((void (*)())Func_020056ec, 0x8000, 0, 0x14);
  {
    /* FAKEMATCH: here the leader loads after both constants, pinned with the
     * actor (one reordered row through the forwarder). */
    register s32 third asm("r2") = 0x28;
    /* FAKEMATCH: the second argument, pinned with the third. */
    register s32 second asm("r1") = 0x105;
    /* FAKEMATCH: the leader, pinned after both. */
    register s32 actor asm("r0");

    actor = gPartyState.currentOwner;
    Func_02005714(actor, second, third);
  }
  Call2((void (*)())Func_02005704, 0, 0);
  Func_02005714_(0, 0x109, 0x14);
  Call2((void (*)())Func_020056f4, 0x9000, 0);
  Call2((void (*)())Func_02005704, 0, 0xa000);
  {
    /* FAKEMATCH: pinned, the flags load before the actor (one reordered row
     * without). */
    register s32 flags asm("r1") = 0;

    UiText_OpenMessageAtObject(0x8000, flags);
  }
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation,
             gPartyState.currentOwner, 0) == 0) {
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    Call2((void (*)())Func_020056f4, 0x8005, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 3);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call2((void (*)())Func_020056f4, 5, 0);
  }
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 1, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 1, 0x224, 0x140);
  Call2((void (*)())Func_02005704, 1, 0xa000);
  Call2((void (*)())Object_SetModeById, 1, 3);
  Call2((void (*)())Func_020056f4, 1, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 6, 0xe000, 0);
  Func_02005714_(6, 0x109, 0x28);
  Call2((void (*)())Func_020056f4, 6, 0);
  Call3((void (*)())ObjectMotion_ArmCallback, 2, 0x8000, 0);
  Func_02005714_(2, 0x105, 0x28);
  Call2((void (*)())Func_020056f4, 0x8002, 0);
  Call2((void (*)())Object_SetModeById, 7, 3);
  Call2((void (*)())Func_020056f4, 7, 0);
  Call2((void (*)())Object_SetModeById, 3, 4);
  Call2((void (*)())Func_020056f4, 3, 0);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 0x13, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 0x13, 0x1f2, 0x140);
  Call2((void (*)())Object_SetModeById, 0x13, 3);
  Call2((void (*)())Func_020056f4, 0x2013, 0);
  Call2((void (*)())Func_02005704, gPartyState.currentOwner, 0x4000);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, gPartyState.currentOwner,
        3);
  Call1((void (*)())GameFlag_SetBit, 0xa3d);
  Call1((void (*)())Func_020057dc, 0x1c);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 0, 0xcccc, 0x6666);
  Call3((void (*)())ObjectMotion_SetPositionAndReset, 0, 0x21c, 0x11e);
  Call2((void (*)())Func_02005704, 0, 0x2000);
  Call1((void (*)())Func_0200298c, 7);
  Call1((void (*)())Func_0200298c, 5);
  Call1((void (*)())Func_0200298c, 6);
  Call1((void (*)())Func_0200298c, 0x13);
  Call1((void (*)())Func_0200298c, 0);
  Call1((void (*)())Func_0200298c, 3);
  Call1((void (*)())Func_0200298c, 2);
  Call1((void (*)())Func_0200298c, 1);
  Call2((void (*)())ObjectMotion_SetActionVariant, gPartyState.currentOwner, 1);
  Call1((void (*)())GameFlag_SetBit, 0x202);
  Call6((void (*)())Func_020055ac, 0x27, 0x28, 0x12, 0x10, 0x17, 0xa);
  Call1((void (*)())Func_020055fc, 1);
  Call1((void (*)())Party_RemoveOwnerRestored, 7);
  Call1((void (*)())Party_RemoveOwnerRestored, 5);
  Call1((void (*)())Party_RemoveOwnerRestored, 6);
  Call1((void (*)())Party_RemoveOwnerRestored, 0);
  Call1((void (*)())Party_RemoveOwnerRestored, 3);
  Call1((void (*)())Party_RemoveOwnerRestored, 2);
  Call1((void (*)())Party_RemoveOwnerRestored, 1);
  Object_GetById(0x14)->collisionFlags |= 8;
  Object_GetById(0x15)->collisionFlags |= 8;
  Call2((void (*)())Object_SetPartAttribute, (s32)Object_GetById(0x1c), 0xf);
  Call3((void (*)())Func_0200567c, 0x1c, 0x2380000, 0x1120000);
  Object_GetById(0x11)->motionFlags = 0;
  Call4((void (*)())Object_SetPositionAndResetMotion, (s32)Object_GetById(0x11),
        0x1ec0000, 0x600000, 0x11c0000);
  Object_GetById(0x1d)->motionFlags = 0;
  Call4((void (*)())Object_SetPositionAndResetMotion, (s32)Object_GetById(0x1d),
        0x1fe0000, 0x600000, 0x1270000);
  Object_GetById(0x1e)->motionFlags = 0;
  Call4((void (*)())Object_SetPositionAndResetMotion, (s32)Object_GetById(0x1e),
        0x21c0000, 0x600000, 0x12c0000);
  Call1((void (*)())WaitFrames, 1);
  Call1((void (*)())GameFlag_SetBit, 0x17c);
  Call1((void (*)())GameFlag_SetBit, 0x144);
  ((void (*)())Func_02005614)();
}

// 26 October 2026, under -mthumb-slow-loads: differ 12 of 783: the third
// placement's constants in another order and, after Actor_SetModeById(12, 7),
// the native issues a load one slot later after a store, where alchemy-gcc
// issues it at once; alchemy permute (5 to 6 minutes) and an empty asm or
// one-pass block after any one statement do not move it.
// Draft, differ 16 of 783 (also in tla-ja, whose listing has 773):
// actor 12's offer after MsgKandoreanTemple07, its leap and the party's
// follow; English also sets screenEffect 0x209 at the end. Left: the third
// placement's constants and the stores after Actor_SetModeById(12, 7) in
// another order.
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"
#include "event_work.h"
#include "edition.h"
#include "call.h"

extern u8 MsgKandoreanTemple07;
extern const u8 gData_02005564[];
extern const u8 gData_020055a0[];
extern const u8 gData_020055e4[];

void Func_02005238(s32 id, s32 x, s32 z, s32 angle);
void Func_02005290(s32 id, s32 a, s32 b);
void Func_02000bcc(void);
void Func_020047f0(void);
void Actor_Jump(s32 id, s32 height, s32 frames);
void Motion_SetVarCbAndRefresh(s32 id, s32 value);
void FieldMessage_ShowAtActor(s32 id, s32 mode);
s32 Event_AskYesNoWithActorAnimation(s32 id, s32 mode);
void Actor_SetMovementTypeById(s32 id, const void *callback);
void Actor_SetMovementTypeAndWaitById(s32 id, const void *callback);
void Actor_Stop(s32 id);
void Actor_WatchAndSetScript(s32 id, s32 target);

#define WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

void Func_0200482c(void) {
  struct Actor *object;
  struct FieldSprite *sprite;

  Func_020051c0();
  Call1((void (*)())Func_02005370, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_020052f0()->motionFlags = 0;
  Call1((void (*)())WaitFrames, 1);
  Call4((void (*)())Func_02005238, 16, 0x1780000, 0x1880000, 0);
  Call4((void (*)())Func_02005238, gAdventure.currentOwner, 0x1780000,
        0x1980000, 0);
  Call4((void (*)())Func_02005238, 12, 0x1d80000, 0x1c60000, 0xb000);
  WORK->screenEffect = 0x202;
  Event_StartScreenTransition();
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x13333, 0x9999);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 464, 428);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 432, 408);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 0);
  Call2((void (*)())Func_020052d0, 12, 0x102);
  Func_02005280((s32)&MsgKandoreanTemple07);
  Call2((void (*)())Func_02005298, 12, 0);
  Value3((s32 (*)())Func_020052c8, 12, 0x105, 40);
  Call2((void (*)())Func_02005298, 12, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0, 40);
  Call2((void (*)())Func_02005298, 12, 0);
  Call2((void (*)())Func_02005298, 16, 0);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 12, 2);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 20);
  Call2((void (*)())Actor_SetModeById, 12, 4);
  Call2((void (*)())Func_02005298, 12, 0);
  Call3((void (*)())Actor_ArmCallback, 16, 0x3000, 10);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0xc000,
        0);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 16, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Call2((void (*)())Actor_SetModeById, 12, 4);
    Call2((void (*)())Func_02005298, 12, 0);
    WORK->messageId++;
  } else {
    WORK->messageId++;
    Call2((void (*)())Actor_SetModeById, 12, 3);
    Call2((void (*)())Func_02005298, 12, 0);
  }
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 16, 0, 0);
  Value3((s32 (*)())Func_020052c8, 16, 0x101, 20);
  Call2((void (*)())Func_02005298, 16, 0);
  Call2((void (*)())Func_020052d0, 12, 0x102);
  Call2((void (*)())Func_02005298, 12, 0);
  Call2((void (*)())Actor_SetModeById, 16, 3);
  Call3((void (*)())Func_02005290, 16, 0, 40);
  Call3((void (*)())Actor_ArmCallback, 12, 0x5000, 40);
  Call2((void (*)())Func_02005298, 12, 0);
  Call3((void (*)())Actor_ArmCallback, 16, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0xc000,
        40);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 20);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, 12, 0);
  Call3((void (*)())Actor_ArmCallback, 16, 0, 0);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Value3((s32 (*)())Func_020052c8, 16, 0x101, 0);
  Call2((void (*)())Func_02005298, 16, 0);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, 12, 0);
  Call2((void (*)())Func_020052d8, 0xcccc, 0x1999);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1a80000, 0x100000, 0x1800000,
        1);
  Call3((void (*)())Actor_SetSpeedParameters, gAdventure.currentOwner,
        0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 16, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0xcccc, 0x6666);
  Actor_SetMovementTypeById(gAdventure.currentOwner,
                                          gData_02005564);
  Actor_SetMovementTypeById(16, gData_020055a0);
  Actor_SetMovementTypeAndWaitById(12, gData_020055e4);
  Call1((void (*)())FieldEvent_WaitFrames, 40);
  Func_02000bcc();
  Call3((void (*)())Actor_ArmCallback, 16, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0xa000,
        20);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Func_020047f0();
    WORK->messageId++;
  } else {
    WORK->messageId++;
    Func_020047f0();
  }
  Call3((void (*)())Actor_ArmCallback, 16, 0, 0);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, 0x400c, 0);
  Call2((void (*)())Actor_SetModeById, gAdventure.currentOwner, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 16, 3);
  Call2((void (*)())Func_02005298, 0x400c, 0);
  Value3((s32 (*)())Func_020052c8, 16, 0x100, 40);
  Call2((void (*)())Func_02005298, 0x5010, 0);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, 0x400c, 0);
  Call3((void (*)())Actor_ArmCallback, 16, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0xa000,
        0);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x5010, 0);
  if (Event_AskYesNoWithActorAnimation(gAdventure.currentOwner, 0) == 0) {
    Call2((void (*)())Actor_SetModeById, 12, 3);
    Call2((void (*)())Func_02005298, 0x400c, 0);
    Call3((void (*)())Actor_ArmCallback, 16, 0, 0);
    Value3((s32 (*)())Func_020052c8, 16, 0x107, 20);
    Call2((void (*)())Func_02005298, 0x5010, 0);
    WORK->messageId += 2;
  } else {
    WORK->messageId += 2;
    Call2((void (*)())Actor_SetModeById, 12, 4);
    Call2((void (*)())Func_02005298, 0x400c, 0);
    Call3((void (*)())Actor_ArmCallback, 16, 0, 0);
    Value3((s32 (*)())Func_020052c8, 16, 0x107, 20);
    Call2((void (*)())Func_02005298, 0x5010, 0);
  }
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1a80000, 0, 0x1900000, 1);
  Call2((void (*)())Func_020052d8, 0x9999, 0x1333);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x9999, 0x4ccc);
  Call2((void (*)())Actor_WatchAndSetScript, gAdventure.currentOwner,
        12);
  Call2((void (*)())Actor_WatchAndSetScript, 16, 12);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 488, 416);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 472, 432);
  Call3((void (*)())Actor_ArmCallback, 12, 0x5000, 0);
  Call2((void (*)())Func_02005298, 0x400c, 0);
  Call3((void (*)())Func_02005290, 0x1010, 0, 80);
  Value3((s32 (*)())Func_020052c8, 12, 0x108, 0);
  Call3((void (*)())Actor_ArmCallback, 12, 0xb000, 20);
  Call2((void (*)())Func_02005298, 0x400c, 0);
  Call2((void (*)())Actor_SetModeById, 12, 3);
  Call2((void (*)())Func_02005298, 0x400c, 0);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1a80000, 0x100000, 0x1800000,
        1);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x13333, 0x9999);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 488, 416);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 504, 376);
  Call3((void (*)())Actor_ArmCallback, 12, 0x8000, 20);
  object = Actor_GetById(12);
  sprite = object->animation;
  Call2((void (*)())Actor_SetSpritePriority, 12, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x19999, 0xcccc);
  Call3((void (*)())Actor_Jump, 12, 4, 0);
  Call3((void (*)())Actor_SetPositionAndCommit, 12, 492, 376);
  Call2((void (*)())Actor_SetModeById, 12, 7);
  object->scaleX = 0xffff0000;
  sprite->rotation = 0xe000;
  object->motionFlags = 0;
  object->y += 0x40000;
  Actor_SetSingleChildFlags(Actor_GetById(12), 0);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x6666, 0x3333);
  Call4((void (*)())Func_02005160, (s32)object, 0x1bc0000, 0x280000, 0x1780000);
  Call1((void (*)())Actor_WaitForMoveById, 12);
  object->motionFlags = 3;
  Actor_SetSingleChildFlags(Actor_GetById(12), 1);
  sprite->rotation = 0;
  object->scaleX = 0x10000;
  object->terrainHeight = 0x200000;
  Call2((void (*)())Actor_SetModeById, 12, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x19999, 0xcccc);
  Call3((void (*)())Actor_Jump, 12, 4, 0);
  Call3((void (*)())Actor_SetPositionAndCommit, 12, 424, 376);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1980000, 0x100000, 0x1900000,
        1);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 392, 360);
  Call3((void (*)())Actor_ArmCallback, 12, 0x3000, 0);
  Call2((void (*)())Actor_SetSpritePriority, 12, 2);
  Call1((void (*)())Actor_Stop, 16);
  Call1((void (*)())Actor_Stop,
        gAdventure.currentOwner);
  Call1((void (*)())WaitFrames, 1);
  Call3((void (*)())Actor_ArmCallback, 16, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, gAdventure.currentOwner, 0xa000,
        10);
  Call2((void (*)())Actor_SetModeById, gAdventure.currentOwner, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 16, 3);
  Call2((void (*)())Actor_SetModeById, 16, 2);
  {
    struct Actor *leader = Actor_GetById(gAdventure.currentOwner);

    if (leader != NULL)
      Actor_ResetAndSetPosition(16, *(s16 *)((u8 *)leader + 10),
                                       *(s16 *)((u8 *)leader + 18));
  }
  Call1((void (*)())Actor_WaitForMoveById, 16);
  Call3((void (*)())Func_02005230, 16, 0, 0);
  Call1((void (*)())GameFlag_SetBit, 0x849);
#if LANGUAGE != LANGUAGE_JAPANESE
  WORK->screenEffect = 0x209;
#endif
  Func_020051c8();
}

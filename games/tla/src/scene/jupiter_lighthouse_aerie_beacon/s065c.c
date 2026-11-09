#include "types.h"
#include "call.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/main_routine_work.h"
#include "text/msg_ids.h"
s32 Camera_MoveWithinMapBounds();
s32 Event_AskYesNoWithActorAnimation();
s32 Event_Wait();
struct Actor *Import_ActorCreate();
s32 Actor_SetMoveTarget();
s32 Script_WaitForEventTimeout();
s32 Map_SetScreenShake();
s32 Icon_DrawItem();
s32 Func_02005814();
s32 Func_0200581c();
s32 Actor_SetHorizontalPositionWithTerrain();
s32 Func_02005894();
s32 Event_SetMessageId();
s32 Actor_ArmCallbackAndRefresh();
void Func_02005924();
s32 Import_EventSetExitRequest();
s32 BattleEffect_ApplyColorToTargetBuffer();
s32 BattleEffect_ApplyColorToSourceBuffer();
s32 BattleEffect_StartBufferInterpolation();
s32 Event_ResolvePendingActions();
s32 Sound_Play();
s32 GameFlag_ClearBit();
s32 GameFlag_SetBit();
s32 GameFlag_Test();
s32 Actor_SetSingleChildFlags();
s32 Actor_ArmCallback();
s32 Actor_WaitForMoveById();
s32 Actor_CommitPositionAndActivate();
s32 Actor_Stop();
s32 Actor_SetMovementTypeById();
s32 Actor_OffsetPositionAndReset();
s32 Actor_SetPositionAndReset();
s32 Actor_SetSpeedParameters();
struct Actor *Actor_GetById();
s32 Actor_SetPartAttribute();
s32 PartyInventory_Remove();
void *HeapSlot_AllocFar();
s32 HeapSlot_Release();
s32 Task_Add();
s32 Task_RemoveFar();
s32 FieldMessage_Show();
s32 VramCache_Load();
s32 wait();
extern u8 Func_020004c8[];
extern u8 gData_02006338[];

void Func_0200065c(void) {
  s32 answer, state;
  struct Actor *actor4, *created, *actor37, *actor31, *actor32;
  struct FieldSprite *sprite;
  u8 *buffer;
  u8 *flags;

  if (GameFlag_Test(0xa23) != 0) {
    Func_02005814();
    Call1((void (*)())Event_ResolvePendingActions, 0);
    Call2((void (*)())FieldMessage_Show, (s32)MsgThereSeemsTo, 5);
    answer = Event_AskYesNoWithActorAnimation(4, 0);
    if (answer != 0) {
      Call3((void (*)())Actor_SetSpeedParameters, 4, 0x10000, 0x8000);
      Call3((void (*)())Actor_SetPositionAndReset, 4, 0x202, 0x135);
    } else {
      Call1((void (*)())GameFlag_SetBit, 0xa21);
      Call1((void (*)())GameFlag_SetBit, 0x8df);
      Call1((void (*)())GameFlag_ClearBit, 0xa25);
      Call1((void (*)())GameFlag_ClearBit, 0x161);
      Call1((void (*)())GameFlag_ClearBit, 0x144);
      Call3((void (*)())Actor_ArmCallback, 27, 0xd000, 0);
      Call3((void (*)())Actor_ArmCallback, 28, 0xd000, 0);
      Call3((void (*)())Actor_SetSpeedParameters, 4, 0x10000, 0x8000);
      Call3((void (*)())Actor_SetPositionAndReset, 4, 0x200, 0x126);
      Call2((void (*)())Actor_ArmCallbackAndRefresh, 4, 0xc000);
      Call1((void (*)())Event_Wait, 20);
      Call2((void (*)())BattleEffect_ApplyColorToSourceBuffer, 0x10000, 0);
      Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x10003, 1);
      Call1((void (*)())BattleEffect_StartBufferInterpolation, 20);
      Call1((void (*)())wait, 20);
      Call1((void (*)())PartyInventory_Remove, 245);
      actor4 = Actor_GetById(4);
      created =
          (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 0x1e9,
                                 actor4->x, actor4->y + 0x80000, actor4->z);
      if (created != 0) {
        sprite = created->animation;
        sprite->unk1b[0] = 0;
        sprite->fullColor = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        created->priorityFlags &= 254;
        created->motionFlags = 0;
        created->unk5c[0] = 1;
        created->speedLimit = 0x6666;
        created->acceleration = 0xccc;
        buffer = HeapSlot_AllocFar(68, 0x608);
        Call1((void (*)())Icon_DrawItem, 246);
        Call3((void (*)())VramCache_Load, sprite->vramBlock, 128,
              (s32)(buffer + 0x400));
        Call1((void (*)())HeapSlot_Release, 68);
      }
      Call2((void (*)())Actor_SetSpritePriority, 37, 1);
      actor37 = Actor_GetById(37);
      actor37->x = actor4->x;
      actor37->y = 0x600000;
      actor37->z = actor4->z;
      actor37->motionFlags = 3;
      actor37->acceleration = 0xccc;
      actor37->speedLimit = 0x6666;
      actor37->scaleX = 0x2000;
      actor37->scaleY = 0x2000;
      Call1((void (*)())Event_SetMessageId, (s32)MsgThereSeemsTo + 1);
      Call1((void (*)())Sound_Play, 36);
      if (created != 0) {
        flags = &created->motionFlags;
        *flags = 0;
        created->gravity = 0x9999;
        created->bounce = 0xcccc;
        created->velocityY = 0x80000;
        Call4((void (*)())Actor_SetMoveTarget, created, 0x2000000, 0x800000,
              0x1260000);
        Call4((void (*)())Actor_SetMoveTarget, actor37, 0x2000000, -1,
              0x1260000);
        Call2((void (*)())Func_02005894, 7, gMainRoutineWork.currentOwner);
        Call1((void (*)())wait, 1);
        Call3((void (*)())Actor_SetSpeedParameters, 7, 0xcccc, 0x6666);
        Call3((void (*)())Actor_OffsetPositionAndReset, 7, 12, 0);
        Call3((void (*)())Actor_CommitPositionAndActivate, 4, -12, 0);
        Call1((void (*)())Actor_WaitForMoveById, 7);
        Call1((void (*)())Actor_Stop, 7);
        Call1((void (*)())wait, 1);
        Call2((void (*)())Actor_SetModeById, 7, 1);
        Call1((void (*)())wait, 1);
        Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
        Call3((void (*)())Actor_ArmCallback, 7, 0xc000, 0);
        Call1((void (*)())Script_WaitForEventTimeout, created);
        state = 4;
        *flags = state;
        Call1((void (*)())Event_Wait, 90);
        *flags = 0;
        Call4((void (*)())Actor_SetMoveTarget, created, 0x2000000, 0x800000,
              0x1080000);
        Call4((void (*)())Actor_SetMoveTarget, actor37, 0x2000000, -1,
              0x1080000);
        *flags = state;
        Call1((void (*)())Event_Wait, 60);
        *flags = 0;
        actor37->x = 0;
        actor37->targetX = 0x80000000;
        actor37->targetY = 0x80000000;
        actor37->targetZ = 0x80000000;

        actor37->y = 0;
        actor37->z = 0;
        actor37->velocityX = 0;
        actor37->velocityY = 0;
        actor37->velocityZ = 0;
        actor37->action = 0;
        Call1((void (*)())Script_WaitForEventTimeout, created);
        *flags = state;
        Call1((void (*)())Event_Wait, 90);
        *flags = 0;
        Call4((void (*)())Actor_SetMoveTarget, created, 0x2000000, 0x5c0000,
              0x1080000);
        Call1((void (*)())Script_WaitForEventTimeout, created);
        created->x = 0;
        created->targetX = 0x80000000;
        created->targetY = 0x80000000;
        created->targetZ = 0x80000000;
        created->y = 0;
        created->z = 0;
        created->velocityX = 0;
        created->velocityY = 0;
        created->velocityZ = 0;
        created->action = 0;
      }
      Call1((void (*)())Event_Wait, 40);
      Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x10000, 0);
      Call1((void (*)())BattleEffect_StartBufferInterpolation, 20);
      Call1((void (*)())wait, 20);
      Call3((void (*)())Actor_OffsetPositionAndReset, 4, 0, -16);
      Call3((void (*)())Actor_OffsetPositionAndReset, 7, 0, -16);
      Call1((void (*)())Actor_WaitForMoveById, gMainRoutineWork.currentOwner);
      Call2((void (*)())Actor_SetModeById, gMainRoutineWork.currentOwner, 1);
      Call2((void (*)())Actor_SetModeById, 7, 1);
      Call1((void (*)())Event_Wait, 50);
      Call1((void (*)())Sound_Play, 141);
      Call1((void (*)())Event_Wait, 30);
      Call3((void (*)())Map_SetScreenShake, 0x10000, 0x10000, 0x10000);
      Call1((void (*)())Event_Wait, 30);
      Call3((void (*)())Map_SetScreenShake, 0x20000, 0x20000, 0x10000);
      Call1((void (*)())Event_Wait, 30);
      Actor_GetById(0)->priorityFlags |= 1;
      Actor_GetById(1)->priorityFlags |= 1;
      Actor_GetById(2)->priorityFlags |= 1;
      Actor_GetById(3)->priorityFlags |= 1;
      Actor_GetById(4)->priorityFlags |= 1;
      Actor_GetById(5)->priorityFlags |= 1;
      Actor_GetById(7)->priorityFlags |= 1;
      Actor_GetById(6)->priorityFlags |= 1;
      Actor_GetById(30)->priorityFlags |= 1;
      Actor_GetById(27)->priorityFlags |= 1;
      Actor_GetById(28)->priorityFlags |= 1;
      Actor_GetById(31)->priorityFlags |= 1;
      Actor_GetById(32)->priorityFlags |= 1;
      Call4((void (*)())Camera_MoveWithinMapBounds, 0x2000000, 0x600000,
            0x1080000, 1);
      Func_02005924();
      Call2((void (*)())Actor_SetSpritePriority, 31, 1);
      Call2((void (*)())Actor_SetSpritePriority, 32, 1);
      Call2((void (*)())Actor_SetSpritePriority, gMainRoutineWork.currentOwner,
            1);
      Call2((void (*)())Actor_SetSpritePriority, 7, 1);
      Call1((void (*)())Event_Wait, 20);
      Actor_SetSingleChildFlags(Actor_GetById(31), 0);
      Actor_SetPartAttribute(Actor_GetById(31), 7);
      actor31 = Actor_GetById(31);
      actor31->scaleY = 0xffff0000;
      actor31->scaleX = 0x1999;
      actor31->motionFlags = 0;
      actor31->x = 0x2000000;
      actor31->y = 0x800000;
      actor31->z = 0x1080000;
      actor31->priorityFlags |= 2;
      Actor_SetSingleChildFlags(Actor_GetById(32), 0);
      Actor_SetPartAttribute(Actor_GetById(32), 7);
      actor32 = Actor_GetById(32);
      actor32->scaleY = 0xffff0000;
      actor32->scaleX = 0x1999;
      actor32->motionFlags = 0;
      actor32->x = 0x2000000;
      actor32->y = 0xa00000;
      actor32->z = 0x1080000;
      actor32->priorityFlags |= 2;
      Call2((void (*)())Actor_SetMovementTypeById, 31, (s32)gData_02006338);
      Call2((void (*)())Actor_SetMovementTypeById, 32, (s32)gData_02006338);
      Task_Add((s32)Func_020004c8, 0x480);
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 9, 0, 0);
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 10, 0, 0);
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 23, 0, 0);
      Call3((void (*)())Actor_SetHorizontalPositionWithTerrain, 24, 0, 0);
      Call1((void (*)())Sound_Play, 145);
      Call3((void (*)())Map_SetScreenShake, 0x60000, 0x60000, 0x10000);
      Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x4063ff, 0);
      Call1((void (*)())BattleEffect_StartBufferInterpolation, 16);
      Call1((void (*)())wait, 16);
      Call1((void (*)())Sound_Play, 63);
      Call1((void (*)())wait, 180);
      Call2((void (*)())BattleEffect_ApplyColorToTargetBuffer, 0x7fff, 0);
      Call1((void (*)())BattleEffect_StartBufferInterpolation, 60);
      Call1((void (*)())Sound_Play, 141);
      Call1((void (*)())wait, 60);
      Call1((void (*)())Task_RemoveFar, (s32)Func_020004c8);
      Call1((void (*)())GameFlag_SetBit, 0x11a);
      Call1((void (*)())Import_EventSetExitRequest, 2);
    }
    Func_0200581c();
  }
}

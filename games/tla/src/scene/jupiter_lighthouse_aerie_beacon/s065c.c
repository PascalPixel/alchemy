#include "types.h"
#include "call.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"
s32 Func_02005814();
s32 Func_0200581c();
s32 Func_02005894();
void Func_02005924();
s32 Actor_SetSingleChildFlags();
s32 PartyInventory_Remove();
void *HeapSlot_AllocFar();
s32 HeapSlot_Release();
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
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered((s32)MsgThereSeemsTo, 5);
    answer = Event_AskYesNoWithActorAnimation(4, 0);
    if (answer != 0) {
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_WalkToAndWait(4, 0x202, 0x135);
    } else {
      Event_SetFlag(0xa21);
      Event_SetFlag(0x8df);
      Event_ClearFlag(0xa25);
      Event_ClearFlag(0x161);
      Event_ClearFlag(0x144);
      Actor_FaceDirection(27, 0xd000, 0);
      Actor_FaceDirection(28, 0xd000, 0);
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_WalkToAndWait(4, 0x200, 0x126);
      Actor_FaceDirectionAndWait(4, 0xc000);
      FieldEvent_Wait(20);
      PaletteFade_ApplySource(0x10000, 0);
      PaletteFade_ApplyTarget(0x10003, 1);
      PaletteFade_Interpolate(20);
      System_Wait(20);
      Call1((void (*)())PartyInventory_Remove, 245);
      actor4 = Actor_GetById(4);
      created =
          FieldActor_Create(0x1e9, actor4->x, actor4->y + 0x80000, actor4->z);
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
        Item_LoadIcon(246);
        Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
        Call1((void (*)())HeapSlot_Release, 68);
      }
      FieldActor_SetSpritePriority(37, 1);
      actor37 = Actor_GetById(37);
      actor37->x = actor4->x;
      actor37->y = 0x600000;
      actor37->z = actor4->z;
      actor37->motionFlags = 3;
      actor37->acceleration = 0xccc;
      actor37->speedLimit = 0x6666;
      actor37->scaleX = 0x2000;
      actor37->scaleY = 0x2000;
      FieldEvent_SetMessageId((s32)MsgThereSeemsTo + 1);
      Event_PlaySound(36);
      if (created != 0) {
        flags = &created->motionFlags;
        *flags = 0;
        created->gravity = 0x9999;
        created->bounce = 0xcccc;
        created->velocityY = 0x80000;
        FieldActor_SetMoveTarget(created, 0x2000000, 0x800000, 0x1260000);
        FieldActor_SetMoveTarget(actor37, 0x2000000, -1, 0x1260000);
        Func_02005894(7, gMainRoutineWork.currentOwner);
        System_Wait(1);
        Actor_SetSpeed(7, 0xcccc, 0x6666);
        Actor_WalkBy(7, 12, 0);
        Actor_WalkByAndWait(4, -12, 0);
        FieldActor_WaitForMoveById(7);
        FieldActor_Stop(7);
        System_Wait(1);
        Actor_SetAnimation(7, 1);
        System_Wait(1);
        Actor_FaceDirection(4, 0xc000, 0);
        Actor_FaceDirection(7, 0xc000, 0);
        Actor_WaitForMove(created);
        state = 4;
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(created, 0x2000000, 0x800000, 0x1080000);
        FieldActor_SetMoveTarget(actor37, 0x2000000, -1, 0x1080000);
        *flags = state;
        FieldEvent_Wait(60);
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
        Actor_WaitForMove(created);
        *flags = state;
        FieldEvent_Wait(90);
        *flags = 0;
        FieldActor_SetMoveTarget(created, 0x2000000, 0x5c0000, 0x1080000);
        Actor_WaitForMove(created);
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
      FieldEvent_Wait(40);
      PaletteFade_ApplyTarget(0x10000, 0);
      PaletteFade_Interpolate(20);
      System_Wait(20);
      Actor_WalkBy(4, 0, -16);
      Actor_WalkBy(7, 0, -16);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
      Actor_SetAnimation(7, 1);
      FieldEvent_Wait(50);
      Event_PlaySound(141);
      FieldEvent_Wait(30);
      MapGrid_SetShake(0x10000, 0x10000, 0x10000);
      FieldEvent_Wait(30);
      MapGrid_SetShake(0x20000, 0x20000, 0x10000);
      FieldEvent_Wait(30);
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
      FieldCamera_MoveTo(0x2000000, 0x600000, 0x1080000, 1);
      Func_02005924();
      FieldActor_SetSpritePriority(31, 1);
      FieldActor_SetSpritePriority(32, 1);
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
      FieldActor_SetSpritePriority(7, 1);
      FieldEvent_Wait(20);
      Actor_SetSingleChildFlags(Actor_GetById(31), 0);
      Actor_SetSpriteColor(Actor_GetById(31), 7);
      actor31 = Actor_GetById(31);
      actor31->scaleY = 0xffff0000;
      actor31->scaleX = 0x1999;
      actor31->motionFlags = 0;
      actor31->x = 0x2000000;
      actor31->y = 0x800000;
      actor31->z = 0x1080000;
      actor31->priorityFlags |= 2;
      Actor_SetSingleChildFlags(Actor_GetById(32), 0);
      Actor_SetSpriteColor(Actor_GetById(32), 7);
      actor32 = Actor_GetById(32);
      actor32->scaleY = 0xffff0000;
      actor32->scaleX = 0x1999;
      actor32->motionFlags = 0;
      actor32->x = 0x2000000;
      actor32->y = 0xa00000;
      actor32->z = 0x1080000;
      actor32->priorityFlags |= 2;
      Actor_SetScript(31, gData_02006338);
      Actor_SetScript(32, gData_02006338);
      Task_AddFrame((s32)Func_020004c8, 0x480);
      Actor_SetPosition(9, 0, 0);
      Actor_SetPosition(10, 0, 0);
      Actor_SetPosition(23, 0, 0);
      Actor_SetPosition(24, 0, 0);
      Event_PlaySound(145);
      MapGrid_SetShake(0x60000, 0x60000, 0x10000);
      PaletteFade_ApplyTarget(0x4063ff, 0);
      PaletteFade_Interpolate(16);
      System_Wait(16);
      Event_PlaySound(63);
      System_Wait(180);
      PaletteFade_ApplyTarget(0x7fff, 0);
      PaletteFade_Interpolate(60);
      Event_PlaySound(141);
      System_Wait(60);
      FieldTask_Remove(Func_020004c8);
      Event_SetFlag(0x11a);
      FieldEvent_SetExitRequest(2);
    }
    Func_0200581c();
  }
}

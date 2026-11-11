#include "version.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov66d;

s32 Func_020041ec();
s32 Func_02004294();
void Func_020042dc(void);
void Func_020042e4(void);
void Func_020043d4(void);
void Func_0200445c(void);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
extern struct Actor *Data_02004cec;
extern u8 Func_02001424[];
extern u8 MsgMarsStar09[];

#if !TLA_PATCH_1

void Func_02000e80(void) {
  struct Actor **slot;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *buffer;
  u16 *screen;
  s32 zero;

  screen = (u16 *)Ram_HeapSlots->block32;
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot = &Data_02004cec;
  object = FieldActor_Create(0x1e9, 0x2a80000, 0x80000, 0x930000);
  *slot = object;
  if (object != 0) {
    sprite = object->animation;
    sprite->nextFrame = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = 0;
    object->unk5c[0] = 1;
    sprite->unk1b[0] = 0;
    object->priorityFlags |= 2;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Item_LoadIcon(247);
    Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 40);
  Event_PlaySound(0);
  Event_PlaySound(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10006, 0);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(Func_02001424, 0x480);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(0x2a80000, -1, 0x1e00000, 1);
  Func_020043d4();
  FieldEvent_Wait(80);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(0x186);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove((s32)Func_02001424);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Func_02004294(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                (s32)screen);
  // The zero sits in a local: as a plain `*screen = 0` 21 instructions differ.
  zero = 0;
  *screen = zero;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(0x2a80000, -1, 0xba0000, 1);
  Func_020043d4();
  FieldEvent_Wait(40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = 3;
    object->velocityY = 0x20000;
    object->speedLimit = 0x20000;
    object->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2a00000, 0, 0xa00000);
    Actor_WaitForMove((s32)*slot);
    Event_PlaySound(135);
    object = *slot;
    object->acceleration = 0x8000;
    object->velocityY = 0x40000;
    object->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2980000, 0, 0x9e0000);
    Actor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Func_020041ec(object);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Func_0200445c();
  Event_SetFlag(0xa30);
  gMainRoutineWork.special = (s32)&SceneId_Ov66d;
  Func_020042e4();
}

#else

void Func_02000e80(void) {
  s32 flag;
  struct Actor **slot, **slot2;
  struct Actor *object, *object2;
  struct FieldSprite *sprite, *sprite2;
  u8 *buffer, *buffer2;
  u16 *screen;

  flag = GameFlag_Test(0xa9e);
  if (flag != 0)
    goto L_02008ea2;
  goto L_0200905c;
L_02008ea2:
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot = &Data_02004cec;
  object = (struct Actor *)FieldActor_Create(0x1e9, 0x2a80000, 0x80000, 0x930000);
  *slot = object;
  if (object != 0) {
    sprite = object->animation;
    sprite->nextFrame = 0;
    sprite->fullColor = 0;
    sprite->palette = 0;
    sprite->priority = 2;
    object->motionFlags = 0;
    object->unk5c[0] = 1;
    sprite->unk1b[0] = 0;
    object->priorityFlags |= 2;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Item_LoadIcon(247);
    Vram_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 80);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x101, 40);
  object = *slot;
  if (object != 0) {
    object->motionFlags = 3;
    object->velocityY = 0x20000;
    object->speedLimit = 0x20000;
    object->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2a00000, 0, 0xa00000);
    Actor_WaitForMove((s32)*slot);
    Event_PlaySound(135);
    object = *slot;
    object->acceleration = 0x8000;
    object->velocityY = 0x40000;
    object->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object, 0x2980000, 0, 0x9e0000);
    Actor_WaitForMove((s32)*slot);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object = *slot;
  if (object != 0)
    Func_020041ec(object);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Func_0200445c();
  Func_020042e4();
  goto L_020093b0;
L_0200905c:
  screen = (u16 *)Ram_HeapSlots->block32;
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar09, 1);
  Event_PlaySound(78);
  System_Wait(1);
  Event_PlaySound(182);
  slot2 = &Data_02004cec;
  object2 = FieldActor_Create(0x1e9, 0x2a80000, 0x80000, 0x930000);
  *slot2 = object2;
  if (object2 != 0) {
    sprite2 = object2->animation;
    sprite2->nextFrame = 0;
    sprite2->unk1b[0] = 0;
    sprite2->fullColor = 0;
    sprite2->palette = 0;
    sprite2->priority = 2;
    object2->motionFlags = 0;
    object2->unk5c[0] = 1;
    object2->priorityFlags |= 2;
    buffer2 = HeapSlot_AllocFar(68, 0x608);
    Item_LoadIcon(247);
    Vram_Load(sprite2->vramBlock, 128, buffer2 + 0x400);
    HeapSlot_Release(68);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x2a8, 186);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 40);
  Event_PlaySound(0);
  Event_PlaySound(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10006, 0);
  PaletteFade_Interpolate(80);
  FieldEvent_Wait(80);
  Task_AddFrame(Func_02001424, 0x480);
  FieldCamera_SetSpeed(0x4cccc, 0x9999);
  FieldCamera_MoveTo(0x2a80000, -1, 0x1e00000, 1);
  Func_020043d4();
  FieldEvent_Wait(80);
  MapGrid_SetShake(-1, -1, 0xe666);
  Event_PlaySound(0x186);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(80);
  System_Wait(60);
  FieldTask_Remove((s32)Func_02001424);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Func_02004294(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                (s32)screen);
  *screen = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(120);
  System_Wait(160);
  FieldCamera_MoveTo(0x2a80000, -1, 0xba0000, 1);
  Func_020043d4();
  FieldEvent_Wait(40);
  object2 = *slot2;
  if (object2 != 0) {
    object2->motionFlags = 3;
    object2->velocityY = 0x20000;
    object2->speedLimit = 0x20000;
    object2->acceleration = 0x10000;
    FieldActor_SetMoveTarget(object2, 0x2a00000, 0, 0xa00000);
    Actor_WaitForMove((s32)*slot2);
    Event_PlaySound(135);
    object2 = *slot2;
    object2->acceleration = 0x8000;
    object2->velocityY = 0x40000;
    object2->speedLimit = 0x10000;
    FieldActor_SetMoveTarget(object2, 0x2980000, 0, 0x9e0000);
    Actor_WaitForMove((s32)*slot2);
  }
  FieldEvent_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x29e, 170);
  FieldEvent_Wait(1);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  object2 = *slot2;
  if (object2 != 0)
    Func_020041ec(object2);
  Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
  Func_0200445c();
  Event_SetFlag(0xa30);
  Event_SetFlag(0xa9e);
  gMainRoutineWork.special = (s32)&SceneId_Ov66d;
  Func_020042e4();
L_020093b0:
  return;
}

#endif

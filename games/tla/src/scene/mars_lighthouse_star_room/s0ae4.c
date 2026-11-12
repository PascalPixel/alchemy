#include "mars_lighthouse_star_room.h"

extern struct Actor *Data_02004cec;

void Func_02000ab4(void);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);

void Func_02000ae4(s32 rise) {
  struct Actor **slot = &Data_02004cec;
  u8 *block;
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 zero;
  s32 size;

  zero = 0;
  object = FieldActor_Create(0x1e9, 0x2be0000, 0, 0xe00000);
  *slot = object;
  if (object == NULL)
    return;
  sprite = object->sprite;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->priority = 1;
  object->motionFlags = 3;
  object->unk5c[0] = 1;
  object->update = Func_02000ab4;
  size = 0x608;
  sprite->nextFrame = zero;
  sprite->unk1b[0] = zero;
  block = HeapSlot_AllocFar(68, size);
  Item_LoadIcon(247);
  block += 0x400;
  Vram_Load(sprite->vramBlock, 128, block);
  HeapSlot_Release(68);
  if (rise) {
    System_Wait(1);
    (*slot)->velocityY = 0x28000;
    System_Wait(4);
    Event_PlaySound(135);
    System_Wait(8);
    Event_PlaySound(135);
  }
}

#include "types.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020042dc(void);
void Func_020042e4(void);

void Func_02000ba4(void) {
  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(14, 5);
  FieldEvent_SetMessageId((s32)MsgMarsStar03);
  Event_ShowMessage(14, 0);
  Actor_SetAnimation(14, 9);
  Func_020042e4();
}

void PartyInventory_Remove(s32 item);
s32 Inventory_AddItem(s32 owner, s32 item);
void *Unit_Get(s32 owner);
void Func_020042c4(s32 owner, s32 slot);
void Func_020041ec(struct Actor *object);
void Func_020043d4(void);

static __inline__ void Func_0200440c_(s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 10 of 155
   * instructions differ in tla-en. */
  BattleEffect_ApplyColorToSourceBuffer(a, b);
}

void Func_02000bd8(void) {
  struct Actor *leader;
  struct Actor *object;
  s32 i;
  s32 offset;

  Func_020042dc();
  Event_ResolvePendingActions(0);
  Actor_SetModeById(13, 5);
  Event_SetMessageId((s32)MsgMarsStar02);
  Event_ShowMessageAndWait(13, 0);
  Actor_SetModeById(13, 10);
  if (!GameFlag_Test(0xa3a)) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    Actor_FaceDirectionAndWait(gMainRoutineWork.currentOwner, 0x4000);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
    object = Data_02004cec;
    object->motionFlags = 0;
    object->speedLimit = 0x40000;
    object->acceleration = 0x20000;
    FieldActor_SetMoveTarget(object, leader->x, leader->y + 0x200000, leader->z);
    Actor_WaitForMove(Data_02004cec);
    Event_PlaySound(83);
    PartyInventory_Remove(224);
    Inventory_AddItem(4, 222);
    Message_ShowCentered((s32)MsgMarsStar08, 1);
    i = 0;
    offset = 216;
    for (; i <= 14; offset += 2, i++) {
      if ((*(u16 *)(Unit_Get(4) + offset) & 0x1ff) == 222) {
        Func_020042c4(4, i);
        break;
      }
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
    Event_SetFlag(0xa3a);
    FieldActor_SetPositionAndResetMotion(Data_02004cec, 0, 0, 0);
    System_Wait(1);
    Func_020041ec(Data_02004cec);
    System_Wait(1);
    FieldCamera_SetSpeed(0x4cccc, 0x9999);
    FieldCamera_MoveTo(170 << 18, -1, 146 << 16, 1);
    Func_020043d4();
    Func_0200440c_(0x10000, 0);
    PaletteFade_ApplyTarget(0x10003, 0);
    PaletteFade_Interpolate(20);
    System_Wait(20);
    {
      // A halfword in its own local, so the address takes the register first.
      struct {
        u16 value;
      } white;

      white.value = 0x7fff;
      *(u16 *)0x0500021e = white.value;
    }
    System_Wait(1);
    FieldMessage_ShowCentered((s32)MsgMarsStar06, 0, 0);
    PaletteFade_ApplyTarget(0x10000, 0);
    PaletteFade_Interpolate(20);
    System_Wait(20);
  }
  Func_020042e4();
}

void Func_02000d88(void) {
  MapGrid_CopyCellsTo(46, 68, 40, 73, 5, 6);
  MapGrid_CopyCellAttributes(104, 9, 5, 6, 40, 9);
}

#include "scene_tables.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_0200432c_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Func_02000db8(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSingleChildFlags(Actor_Get(id), 0);
  FieldActor_SetSpritePriority(id, 3);
  object->motionFlags = 0;
  object->priorityFlags |= 2;
  object->collisionFlags = 0;
  if (id == 13) {
    Func_0200432c_(13, 175 << 18, PIXELS(224));
    Actor_SetAnimation(13, 10);
    MapGrid_CopyCellAttributes(39, 22, 2, 1, 43, id);
  } else {
    Func_0200432c_(14, 165 << 18, PIXELS(216));
    Actor_SetAnimation(14, 9);
    Func_0200423c_(31, 21, 2, 2, 40, 12);
  }
}

#include "ram_buffer.h"
#include "event_work.h"

struct EventNote {
  u8 unk000[0x15a];
  u16 unk15a;
};

void Func_02000e50(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Func_020042dc();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered((s32)MsgMarsStar07, 1);
  ((struct EventNote *)work)->unk15a = 1;
  Func_020042e4();
}

#include "version.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "scene_ids.h"

extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov66d;

void Func_02004294(s32 map, void *destination);
void Func_0200445c(void);
void Func_02001424(void);

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
    sprite = object->sprite;
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
  FieldTask_Remove(Func_02001424);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Func_02004294(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                screen);
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
    sprite = object->sprite;
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
    sprite2 = object2->sprite;
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
  FieldTask_Remove(Func_02001424);
  System_Wait(20);
  MapGrid_SetShake(0, 0, 0);
  FieldEvent_Wait(20);
  MapGrid_CopyCellAttributes(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(65, 65, 9, 21, 38, 24);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 38, 87);
  MapGrid_CopyCellsLayered(76, 64, 9, 22, 102, 23);
  Func_02004294(gMainRoutineWork.map +
                    ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                screen);
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

struct RiseTimer {
  u8 unk00[0x68];
  s32 frames;
};

void Func_020013e4(struct Actor *object) {
  struct RiseTimer *timer = (struct RiseTimer *)object;

  s32 growth;

  object->y += 0x8000;
  growth = 0x51e;
  object->scaleX += growth;
  object->scaleY += growth;
  timer->frames--;
  if (timer->frames == 1)
    FieldActor_SetPositionAndResetMotion(object, 0, 0, 0);
  else if (timer->frames == 0)
    Func_020041ec(object);
}

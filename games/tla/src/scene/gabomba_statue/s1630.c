/// @file
/// Scene code of overlay 0x671, in ROM order, from s1630's offset up to the next object the overlay links.

#include "field/field.h"

#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 exit;
};

void Func_020056b4(struct Actor *object);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Jumps down the hole.
void Func_02001630(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct Actor *leader;
  struct EventWork154 *work;

  if (x == 28 && z == 9 && !(gInput.held & 0x10))
    return;
  if (x == 29 && z == 10 && !(gInput.held & 0x40))
    return;
  work = Ram_HeapSlots->eventWork;
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  Func_020056b4(leader);
  FieldEvent_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x10000, 0x8000);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 472, 144);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  FieldActor_RefreshSelectorById(gMainRoutineWork.currentOwner);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 13);
  FieldActor_SetMoveTarget(leader, PIXELS(472), 0, PIXELS(156));
  Actor_WaitForMove(leader);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 10);
  Event_PlaySound(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(work->exit);
}

void Func_020056bc(void);

void Func_02001728(struct Actor *actor) {
  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 424);
  Func_020056bc();
}

void Func_0200176c(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  s32 speed;

  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  actor->motionFlags = 0;
  speed = 0x19999;
  /* FAKEMATCH(barrier): an empty asm on speed, so the speed is loaded before
   * the owner, whose base is already in a register; as the plain call, 4 of 40
   * instructions differ in tla-en. */
  asm volatile("" : "+l"(speed));
  Actor_SetSpeed(gMainRoutineWork.currentOwner, speed, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 152, 488);
  actor->motionFlags = 3;
  FieldEvent_Wait(8);
  Event_PlaySound(161);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  FieldEvent_Wait(20);
  Func_020056bc();
}

void Func_020017dc(struct Actor *actor) {
  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 376, 200);
  Func_020056bc();
}

void Func_02001820(struct Actor *actor) {
  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 376, 136);
  Func_020056bc();
}

void Func_02001864(struct Actor *actor) {
  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 440, 248);
  Func_020056bc();
}

void Func_020018a8(struct Actor *actor) {
  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(581);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x19999, 0x19999);
  Actor_MoveToAndWait(gMainRoutineWork.currentOwner, 440, 184);
  Func_020056bc();
}

#include "scene_ids.h"

void Func_02001950(s32 show, s32 first, u32 count);

void Func_020018ec(s32 show) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov671Room1)
    Func_02001950(show, 8, 1);
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov671Room2)
    Func_02001950(show, 29, 2);
}

struct GateEventWork {
  u8 unk000[362];
  u16 gateTimer;
};

extern const u8 gData_02005d7c[];
void Func_020018ec(s32 open);
void Func_02005428(const u8 *data, s32 a, s32 b);

void Func_02001928(void) {
  Func_020018ec(1);
  Event_SetFlag(512);
}

void Func_0200193c(void) {
  Func_020018ec(0);
  Event_ClearFlag(512);
}

void Func_02001950(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_Get(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void Func_02001990(s32 a, s32 b) {
  Func_02005428(gData_02005d7c, a, b);
  if (Event_GetFlag(0x8ce) && Event_GetFlag(0x8cf))
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 200;
}

void Func_020057b4(void);
void Func_020020f0(s32 value);

void Func_020019d0(void) {
  struct Actor *actor = Actor_Get(26);

  Func_020056b4(actor);
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0x9999, 0x1333);
  FieldCamera_MoveTo(180 << 17, 0, 248 << 17, 1);
  Func_020057b4();
  Event_PlaySound(586);
  Func_020020f0(0);
  FieldEvent_Wait(40);
  Event_PlaySound(582);
  actor->motionFlags = 0;
  actor->y = PIXELS(8);
  FieldEvent_Wait(10);
  actor->y = 0;
  FieldEvent_Wait(10);
  Func_020020f0(-1);
  Event_SetFlag(0x8cd);
  FieldEvent_Wait(20);
  Func_020056bc();
}

struct FieldPart {
  s16 kind;
};

struct FieldAnimation {
  u8 unk00[40];
  struct FieldPart *part;
};

void Func_020043a4(s32 value);
void Func_02005624(struct Actor *object, s32 mode);
s32 Func_020053a0(void);
void Func_02005784(s32 id);
void Func_0200193c(void);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02001a6c(s32 value) {
  Func_020043a4(value);
  if (value == 1)
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 200;
}

void Func_02001a8c(void) {
  struct Actor *object = Actor_Get(15);

  Func_020056b4(object);
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(20);
  Event_PlaySound(214);
  Func_02005624(object, 5);
  object->scaleY = 0x10000;
  Func_020056bc();
}

void Func_02001ac0(void) {
  if (Func_020053a0() == 0)
    ((struct GateEventWork *)Ram_HeapSlots->eventWork)->gateTimer = 201;
}

void Func_02001adc(s32 id) {
  Func_02005784(id);
  Actor_SetSpriteColor(Actor_Get(id), 15);
  FieldActor_SetSpritePriority(id, 1);
  Actor_Get(id)->priorityFlags |= 2;
}

void Func_02001b0c(void) {
  if (Event_GetFlag(265)) {
    if (!Event_GetFlag(512))
      Func_0200193c();
  } else {
    Event_SetFlag(512);
  }
}
void Func_02001b38(void) {
  u32 i = 0;

next:
  {
    s32 id = i + 8;
    struct Actor *object = Actor_Get(id);

    Func_02001adc(id);
    object->motionFlags = 0;
    object->y = PIXELS(36);
    i++;
  }
  if (i < 1)
    goto next;
  Func_02001b0c();
}

void Func_02001b68(void) {
  u32 i = 0;

next:
  {
    s32 id = i + 29;
    struct Actor *object = Actor_Get(id);

    Func_02001adc(id);
    object->motionFlags = 0;
    object->y = PIXELS(36);
    i++;
  }
  if (i < 2)
    goto next;
  Func_02001b0c();
}
void Func_02001b98(void) {
  s32 value = 16;
  s32 open = 0;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 i;

  if (Event_GetFlag(262) || *(s16 *)(event + 358) != 0) {
    open = 1;
    value = 0;
  }
  i = 8;
next:
  {
    struct Actor *object = Actor_Get(i);

    if (object != NULL && object->animationKind == 1) {
      struct FieldPart *part =
          ((struct FieldAnimation *)object->animation)->part;

      if (part != NULL) {
        switch (part->kind) {
        case 357:
        case 358:
        case 359:
        case 360:
        case 361:
        case 362:
          Actor_SetAnimationSpeed(object, value);
          break;
        case 407:
          object->movementState = open;
          break;
        }
      }
    }
  }
  i++;
  if (i < 64)
    goto next;
}

#include "task.h"

extern u32 gData_02005910[];
extern u8 Data_0202c001[];
void Func_02001b98(void);

#define MARK(at, cellPicture, shape)                                           \
  (cell = (at), layerA[cell] = (cellPicture), layerB[cell] = (cellPicture),    \
   word = &shapesA[cell], *word |= (shape), word = &shapesB[cell],             \
   *word |= (shape))

void Func_02001c10(void) {
  u32 *dest = (u32 *)(Data_0202c001 + 0x1cf);
  u32 *src = gData_02005910;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *layerA = *(u8 **)(map + 0x13c);
  u8 *layerB = *(u8 **)(map + 0x174);
  u32 *shapesA = *(u32 **)(map + 0x138);
  u32 *shapesB = *(u32 **)(map + 0x170);
  struct Actor *object;
  s16 *frames;
  u32 *word;
  u32 i;
  s32 x, z, cell, found;

  i = 0;
  do {
    *dest++ = *src++;
  } while (++i <= 11);
  for (i = 8; (s32)i <= 63; i++) {
    object = Actor_GetById(i);
    if (object == 0)
      continue;
    if (object->animationKind != 1)
      continue;
    frames = *(s16 **)((u8 *)object->animation + 40);
    if (frames == 0)
      continue;
    found = 0;
    x = object->x >> 20;
    z = object->z >> 20;
    switch (*frames - 357) {
    case 1:
      cell = x + (z << 7);
      layerA[cell] = 116;
      layerB[cell] = 117;
      word = &shapesA[cell];
      *word |= 0x80500000;
      word = &shapesB[cell];
      *word |= 0x80500000;
      found = 1;
      break;
    case 0:
      MARK(x + (z << 7) - 129, 118, 0x510000);
      MARK(x + (z << 7) - 128, 117, 0x80510000);
      MARK(x + (z << 7) - 127, 119, 0x510000);
      MARK(x + (z << 7) - 1, 117, 0x80510000);
      MARK(x + (z << 7), 117, 0x510000);
      MARK(x + (z << 7) + 1, 117, 0x80510000);
      MARK(x + (z << 7) + 127, 120, 0x510000);
      MARK(x + (z << 7) + 128, 117, 0x80510000);
      MARK(x + (z << 7) + 129, 121, 0x510000);
      found = 1;
      break;
    case 2:
      MARK(x + (z << 7) - 2, 127, 0x540000);
      MARK(x + (z << 7) - 1, 122, 0x529000);
      MARK(x + (z << 7), 117, 0x80526000);
      MARK(x + (z << 7) + 1, 123, 0x529000);
      MARK(x + (z << 7) + 2, 127, 0x540000);
      found = 1;
      break;
    case 4:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 126, 0x80550000);
      found = 1;
      break;
    case 3:
      MARK(x + (z << 7) - 256, 127, 0x540000);
      MARK(x + (z << 7) - 128, 124, 0x539000);
      MARK(x + (z << 7), 117, 0x539000);
      MARK(x + (z << 7) + 128, 125, 0x539000);
      MARK(x + (z << 7) + 256, 127, 0x540000);
      found = 1;
      break;
    case 5:
      found = 1;
      break;
    }
    if (found) {
      object->collisionFlags = 0;
      object->motionFlags = 0;
      object->priorityFlags = 0;
      Actor_SetSingleChildFlags(object, 0);
    }
  }
  Task_AddFrame(Func_02001b98, TASK_PASS_VBLANK);
}

#include "macros.h"

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)

/// Makes the scene more translucent the farther inside the object is.
void Func_02002074(void) {
  s32 blend;
  struct Actor *object =
      ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->object;
  s32 level = object->z;

  level = (level - 0xe80000) >> 19;
  CLAMP(level, 0, 16);
  blend = ((16 - level) << 8) + level;
  REG_BLDALPHA->value = blend;
}

extern const s8 gData_02005940[];

void Func_0200561c(void *animation, s32 a);

void Func_020020b0(s32 id, s32 direction, s32 value) {
  struct Actor *object = Actor_Get(id);
  void *animation = object->animation;

  Func_02005624(object, gData_02005940[(direction + 1) & 3]);
  object->unk5c[0] = 2;
  Func_0200561c(animation, value);
}

extern s32 gData_02005af0;
void Func_020020b0(s32 id, s32 direction, s32 value);

void Func_020020f0(s32 direction) {
  s16 scene;

  gData_02005af0 = direction;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov671Room2) {
    Func_020020b0(14, direction, 0);
    Func_020020b0(21, direction, 1);
    Func_020020b0(26, direction, 2);
    if ((u16)(gMainRoutineWork.entrance - 3) > 1)
      Func_020020b0(27, direction, 3);
    if (direction == 0)
      MapGrid_CopyCellAttributes(23, 21, 1, 3, 23, 9);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(24, 21, 1, 3, 23, 9);
  } else if (scene == (s32)&SceneId_Ov671Room3) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(15, direction, 1);
    Func_020020b0(20, direction, 2);
    Func_020020b0(25, direction, 3);
    Func_020020b0(29, direction, 4);
  } else if (scene == (s32)&SceneId_Ov671Room4) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(21, direction, 1);
    Func_020020b0(29, direction, 2);
    Func_020020b0(35, direction, 3);
    Func_020020b0(38, direction, 4);
  } else if (scene == (s32)&SceneId_Ov671Room5) {
    Func_020020b0(10, direction, 0);
    Func_020020b0(12, direction, 1);
    if (direction == 0)
      MapGrid_CopyCellAttributes(34, 12, 1, 3, 27, 12);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(35, 12, 1, 3, 27, 12);
  } else if (scene == (s32)&SceneId_Ov671Room6) {
    Actor_SetAnimation(13, 5);
    Actor_SetSingleChildFlags(Actor_Get(13), 0);
  } else if (scene == (s32)&SceneId_Ov671Room7) {
    Func_020020b0(8, direction, 0);
    Func_020020b0(9, direction, 1);
    Func_020020b0(10, direction, 2);
    Func_020020b0(11, direction, 3);
    Func_020020b0(12, direction, 4);
    if (direction == 0)
      MapGrid_CopyCellAttributes(0, 0, 1, 3, 9, 27);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(1, 0, 1, 3, 9, 27);
  }
}

struct Orbiter {
  u8 unk00[85];
  u8 motionFlags;
  u8 unk56[12];
  u8 phase;
  u8 slot;
  s16 centerX;
  s16 centerZ;
  struct Orbiter *link;
};

extern const u8 gData_0200e88c[];

void Func_02005634(struct Orbiter *object, const u8 *script);
void Animation_ApplyChildValues(struct Orbiter *object, s32 value);

void Func_020022d0(s32 slot, s32 value, s32 targetId, s32 angle) {
  struct Orbiter *object = ((struct Orbiter *)Actor_Get(slot));
  struct Orbiter *target = ((struct Orbiter *)Actor_Get(targetId));

  Func_02005624(object, 0);
  Func_02005634(object, gData_0200e88c);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->link = target;
  object->phase = angle / 256;
  object->slot = slot;
  Animation_ApplyChildValues(object, value);
}


/// @file
/// The scene code of overlay 0x69b that its scene tables belong to, in ROM
/// order; the rest of the overlay is in jupiter_lighthouse_room_2/.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#include "main_routine_work.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000054(void) {
  s32 t0;

  t0 = ((s32)Actor_Get(gMainRoutineWork.currentOwner));
  if (FIELD_AT((t0 + 0x22), u8, 0) == 1) {
    MapGrid_SetCellBehavior(0x900000, 0x2400000, 2, 0xea);
    MapGrid_SetCellBehavior(0x1000000, 0x2300000, 2, 0xea);
  } else {
    MapGrid_SetCellBehavior(0x900000, 0x2400000, 2, 0xeb);
    MapGrid_SetCellBehavior(0x1000000, 0x2300000, 2, 0xeb);
  }
}

s32 Func_0200231c();
extern s32 gData_02002b88[3];
extern const u8 gData_02002b60[];

void Func_020000b4(void) {
  Func_0200231c((s32)gData_02002b60, gData_02002b88[0], gData_02002b88[1],
                gData_02002b88[2]);
}

#include "actor.h"
#include "field_sprite.h"
#include "ram_buffer.h"
#include "event_work.h"

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Func_020021b4(struct Actor *object, s32 a);
void Func_020021d4(struct Actor *object);
void Func_0200224c(void);
void Func_02002254(void);

void Func_020000d0(s32 kind) {
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 i;
  s32 *target;

  Func_0200224c();
  FieldEvent_ResolvePendingActions(0);
  target = gData_02002b88;
  if (kind == 32) {
    target[0] = 136 << 16;
    target[1] = 144 << 16;
    target[2] = 148 << 18;
  } else {
    target[0] = 248 << 16;
    target[1] = 144 << 16;
    target[2] = 144 << 18;
  }
  {
    s32 priority = 0x480;

    Task_AddFrame(Func_020000b4, priority);
  }
  FieldCamera_FollowActor(owner, 1);
  for (i = 35; i >= 0; i--) {
    leader->y += -0x10000;
    System_Wait(1);
  }
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
  Func_020021b4(effect, 2);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  ((struct FieldSprite *)effect->animation)->priority = 1;
  while (leader->y > 0x200000) {
    leader->y += -0x10000;
    leader->angle += 0x2000;
    effect->x = leader->x;
    effect->y = leader->y + -0x60000;
    effect->z = leader->z;
    System_Wait(1);
  }
  Func_020021b4(effect, 6);
  i = 14;
  do {
    leader->angle += 0x2000;
    System_Wait(1);
    i--;
  } while (i >= 0);
  Func_020021d4(effect);
  Actor_SetSingleChildFlags(Actor_Get(owner), 1);
  leader->angle = 0x4000;
  leader->terrainId = 0;
  leader->motionFlags = 3;
  FieldTask_Remove(Func_020000b4);
  Func_02000054();
  Func_02002254();
}

void Func_02000230(s32 kind) {
  s32 owner = gMainRoutineWork.currentOwner;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 *target;
  s32 i;

  Func_0200224c();
  FieldEvent_ResolvePendingActions(0);
  target = gData_02002b88;
  if (kind == 30) {
    target[0] = 136 << 16;
    target[1] = 144 << 16;
    target[2] = 148 << 18;
  } else {
    target[0] = 248 << 16;
    target[1] = 144 << 16;
    target[2] = 144 << 18;
  }
  {
    s32 priority = 0x480;

    Task_AddFrame(Func_020000b4, priority);
  }
  FieldCamera_FollowActor(owner, 1);
  leader->scaleX = 0x10000;
  leader->scaleY = 0x10000;
  effect = Actor_Spawn(280, leader->x, leader->y, leader->z);
  Func_020021b4(effect, 2);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  ((struct FieldSprite *)effect->animation)->priority = 1;
  while (leader->y < 0x6c0000) {
    leader->y += 0x10000;
    leader->angle += 0x2000;
    effect->x = leader->x;
    effect->y = leader->y + -0x60000;
    effect->z = leader->z;
    System_Wait(1);
  }
  leader->scaleX = 0;
  effect->scaleX = 0;
  while (leader->y < 0x900000) {
    leader->y += 0x10000;
    effect->y += 0x10000;
    System_Wait(1);
  }
  effect->scaleX = 0x8000;
  effect->scaleY = 0xc000;
  for (i = 31; i >= 0; i--) {
    effect->scaleX += 0x800;
    effect->scaleY += 0x200;
    System_Wait(1);
  }
  effect->scaleX = 0x18000;
  effect->scaleY = 0x10000;
  Func_020021b4(effect, 6);
  leader->scaleX = 0x6000;
  leader->scaleY = 0x6000;
  i = 19;
spin:
  leader->angle += 0x2000;
  leader->scaleX += 0x800;
  leader->scaleY = leader->scaleX;
  System_Wait(1);
  i--;
  if (i >= 0)
    goto spin;
  Func_020021d4(effect);
  Actor_SetSingleChildFlags(Actor_Get(owner), 1);
  leader->angle = 0x4000;
  leader->terrainId = 1;
  leader->motionFlags = 3;
  leader->y = 0x900000;
  leader->terrainHeight = 0x900000;
  FieldTask_Remove(Func_020000b4);
  Func_02000054();
  Func_02002254();
}

void Func_020003f4(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;

  event->unk16a = 30;
  effect[52] = 1;
}

void Func_02000410(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;

  event->unk16a = 31;
  effect[52] = 1;
}

void Func_0200042c(void) {
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x16a), u16, 0) = 0x20;
}

void Func_02000440(void) {
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x16a), u16, 0) = 0x21;
}

void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

/// Once the leader stands at height 32 or more, it rises and spins for 110
/// frames.
void Func_02000454(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 i;
  s32 angle;
  s32 y;

  y = object->y;
  if (y >= 0x200000) {
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_WalkTo(gMainRoutineWork.currentOwner, 216, 680);
    Actor_SetAnimationSpeed(object, 64);
    object->motionFlags = 0;
    for (i = 109; i >= 0; i--) {
      object->y += PIXELS(3);
      angle = object->angle;
      angle += 0x1800;
      object->angle = angle;
      System_Wait(2);
    }
    FieldEvent_SetExitRequest(12);
  }
}

void Func_020004d0(s32 a0, s32 a1) {
  s32 t0;

  t0 = ((s32)Actor_Get(a1));
  FIELD_AT((t0 + 0x55), u8, 0) = 0;
  FieldActor_SetSpritePriority(a1, 1);
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
}

#include "field_event.h"
#include "iwram_call.h"
#include "macros.h"
#include "io_reg.h"
#include "io_write.h"
#include "call.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

static inline void CopyCells(s32 srcX, s32 srcY, s32 width, s32 height,
                             s32 destX, s32 destY) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so each stack argument keeps its own register; as a direct
   * call, 10 of 157 instructions of Func_02000500 differ in tla-en. */
  Func_0802b450(srcX, srcY, width, height, destX, destY);
}

/// Actor id comes down: once it rests on the terrain (or after 30 frames) cue
/// 188 plays, and at cell (20, 39) it sets flag 0xa02 and copies map cells in
/// three steps, each with cue 167.
void Func_02000500(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MainRoutineWork *party;
  s32 i;

  object->motionFlags = 3;
  Actor_Get(id)->priorityFlags |= 1;
  party = &gMainRoutineWork;
  Actor_Get(party->currentOwner)->priorityFlags |= 1;
  if (object->x >> 20 == 20 && object->z >> 20 == 39)
    FieldActor_SetSpritePriority(id, 3);
  if (object->terrainHeight !=
      Map_GetTerrainHeight(object->terrainId, object->x, object->z)) {
    System_Wait(2);
    for (i = 0; i <= 29 &&
                (object->y > object->terrainHeight || object->velocityY != 0);
         i++)
      System_Wait(1);
    Event_PlaySound(188);
  }
  if (object->x >> 20 == 20 && object->z >> 20 == 39) {
    Event_SetFlag(0xa02);
    Actor_SetSingleChildFlags(Actor_Get(id), 0);
    FieldActor_SetSpritePriority(id, 3);
    Actor_SetPosition(id, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(92, 39, 1, 1, 84, 39);
    FieldEvent_Wait(30);
    Event_PlaySound(167);
    MapGrid_CopyCellsLayered(92, 40, 1, 1, 84, 40);
    FieldEvent_Wait(20);
    Event_PlaySound(167);
    CopyCells(92, 41, 1, 1, 84, 41);
    FieldEvent_Wait(20);
    Event_PlaySound(167);
    MapGrid_CopyCellsLayered(93, 41, 1, 1, 85, 41);
    MapGrid_SetCellBehavior(168 << 17, 164 << 18, 2, 236);
  }
}

void Func_02000674(s32 a0, s32 a1) { Actor_SetSpritePriority(a1, 2); }

void Func_02000b28(s32 id);

void Func_02000680(s32 unused, s32 id) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));
  s32 i;

  if (Event_GetFlag(0xa00) && object->x >> 20 == 18 && object->z >> 20 == 6) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Func_02000b28(id);
    Event_SetFlag(0xa03);
  }
}

void Func_020006dc(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->renderMode == 10)
    MapGrid_SetCellBehavior(0x1500000, 0x2500000, 0, 255);
}

void Func_02000704(void) { Func_080dbdd4(0x1500000, 0x2500000, 0, 4); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Math_Cosine(s32 angle);

/// A particle's step: it circles at its angle (action) around x 214 and
/// rises with its phase (unk66), drawn in front while the sine is
/// negative and behind otherwise.
void Func_02000720(struct Actor *object) {
  s16 angle = object->action;
  s16 height = object->unk66;

  SET_POSITION(object,
               Iwram_MulQ16(Math_Cosine(angle), object->speedLimit) +
                   (214 << 16),
               (height * 3 << 16) + 0x80000, 170 << 18);
  // Read signed, as the phase is above; the field is declared unsigned.
  if ((s16)object->unk66 == 0)
    Func_020021b4(object, 7);
  if (object->y > 170 << 17)
    object->unk66 = 0xffff;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)object->animation)->priority = 1;
  else
    ((struct FieldSprite *)object->animation)->priority = 2;
  object->action += object->acceleration;
  object->unk66++;
}

void Func_020007d0(void) {
  s32 i;
  s32 phase;
  struct Actor *object;

  for (i = 0, phase = 0; i != 22; i++, phase += 5) {
    object = (FieldActor_Create(672, 0, 0, 0));
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16Far() << 2) + 0x60000;
    object->acceleration = ((Random16Far() << 11) >> 16) + 0x800;
    object->update = Func_02000720;
    object->action = Random16Far();
    object->unk66 = phase;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->animation)->priority = 1;
  }
}

void Func_02001498(void);
void Func_020015a0(s32 id, s32 value);
void Func_02002314(s32 a);
void Func_02002324(const void *script, s32 a, s32 b, s32 c);

s32 Func_02000aa4();

/// The scene's set-up entry: screen effect 0x204, actors 13 and 14 at action
/// variant 3, the blend registers, the particles, and the map and actors by
/// flags 0xa02, 0xa00 and 0xa03 and the entrance.
s32 Func_02000854(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  FieldActor_SetSpritePriority(13, 3);
  FieldActor_SetSpritePriority(14, 3);
  Io_Write16(0x3f06, &REG_BLDCNT);
  Io_Write16(0x1000, &REG_BLDALPHA);
  Func_02002314(0);
  Call4((void (*)())Func_02002324, (s32)gData_02002b60, 0, 0, 0x80000000);
  Func_02000054();
  Func_020007d0();
  if (Event_GetFlag(0xa02)) {
    Actor_SetPosition(17, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(92, 39, 2, 3, 84, 39);
    MapGrid_SetCellBehavior(168 << 17, 164 << 18, 2, 236);
  } else {
    Func_02001498();
    Func_020015a0(17, 0);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 6 ||
      gMainRoutineWork.entrance == 7)
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 64;
  if (Event_GetFlag(0xa00)) {
    Func_02000b28(8);
    Func_02000b28(9);
    Func_02000b28(10);
    Func_02000b28(11);
  }
  if (Event_GetFlag(0xa03)) {
    Actor_SetPosition(12, 148 << 17, 208 << 15);
    Func_02000b28(12);
  }
  return 0;
}

s32 Func_0200099c(void) {
  Scene_SetArrivalFlags(0);
  Func_02000aa4(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

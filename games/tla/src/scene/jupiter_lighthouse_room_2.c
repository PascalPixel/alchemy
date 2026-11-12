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

void Func_02000aa4(s32 mode);

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

extern s32 gData_02002b38;

void Func_020009dc(s32 a0) { gData_02002b38 = a0; }

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "dma.h"

extern s32 gData_02002b34;
extern u16 gData_02002b98[];

void Func_020009e8(void) {
  u16 colors[16];
  s32 phase = (gData_02002b34 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002b38 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;
    s16 *state = (s16 *)(work + 358);

    if (*state != 0 && (*state & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  i = 0;
  offset = 4;
next:
  p = colors;
  {
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02002b98;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02002b34++;
}

extern u16 gData_02002bb8[];
extern u16 gData_02002bbc[];

void Func_02000aa4(s32 mode) {
  gData_02002b38 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02002b98, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002bb8, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_02002b34 = zero;
    Task_Add(Func_020009e8, 1152);
  } else {
    Dma_Set(gData_02002bbc, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_0200223c();
void Func_0200230c();

void Func_02000b28(s32 id) {
  struct FieldActor *object;
  s32 height;

  object = ((struct FieldActor *)Actor_Get(id));
  object->motionFlags = 4;
  height = object->terrainHeight;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  {
    u8 *flags = &object->priorityFlags;
    /* FAKEMATCH(temporary): a local holding 128, so the bit is built in the
     * register it is ORed into; as *flags |= 128, 4 of 38 instructions differ
     * in tla-en. */
    s32 bit = 128;

    *flags = bit | *flags;
  }
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_0200223c(object->x, object->z, 0, (height >> 19) + 6);
  Func_0200230c(object->x, object->z, 0, 128);
}

#include "frame.h"

void Func_02000b7c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Func_0200224c();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = 0;
  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);
  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  facing = linked->angle;
  actor->x = (((linked->x >> 19) & ~1) + (facing >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((facing >> 13) & 2) + 1) << 19;
  actor->velocityY = 0x60000;
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSingleChildFlags(actor, 1);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = 0x80000;
  shadow->acceleration = 0x10000;
  shadow->motionFlags = 0;
  FieldActor_SetMoveTarget(
      shadow, actor->x, Map_GetTerrainHeight(0, actor->x, actor->z), actor->z);
  i = 0;
  while (i <= 59 && actor->y > actor->terrainHeight + 0x100000) {
    i++;
    System_Wait(1);
  }
  Event_PlaySound(127);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    System_Wait(1);
  }
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Func_02002254();
}

s32 Func_02000d68(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

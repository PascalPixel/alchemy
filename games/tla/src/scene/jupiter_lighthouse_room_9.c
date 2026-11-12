/// @file
/// The scene code of overlay 0x6a2 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_9/.

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
#include "main_routine_work.h"
#include "event_inline.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern s32 gData_02002618;

s32 Func_02001978();
struct Actor *Func_02000910();

void Func_02000054(void) {
  if (gMainRoutineWork.renderMode == 10)
    MapGrid_SetCellBehavior(0x2200000, 0x1100000, 0, 255);
}

void Func_0200007c(void) { Func_080dbdd4(0x2200000, 0x1100000, 0, 6); }

void Func_02000090(s32 id) {
  struct Actor *object;

  if (gData_02002618 == 0) {
    object = Actor_Get(8);
    if (id == 30) {
      Actor_SetPosition(8, PIXELS(296), PIXELS(600));
      object->y = 0;
      object->angle = FACING_WEST;
    } else if (id == 31) {
      Actor_SetPosition(8, PIXELS(136), PIXELS(680));
      object->y = 0;
      object->angle = 0;
    }
    Func_02000910(object);
    gData_02002618 = 1;
  }
}

void Func_020000e8(s32 id) {
  struct Actor *object;
  struct Actor *motion;

  if (gData_02002618 == 0 &&
      Actor_Get(gMainRoutineWork.currentOwner)->y > 0) {
    object = Actor_Get(8);
    if (id == 50) {
      Actor_SetPosition(8, PIXELS(488), PIXELS(280));
      object->y = PIXELS(32);
      object->angle = FACING_WEST;
    } else if (id == 51) {
      Actor_SetPosition(8, PIXELS(472), PIXELS(184));
      object->y = PIXELS(32);
      object->angle = FACING_WEST;
    } else if (id == 52) {
      Actor_SetPosition(8, PIXELS(616), PIXELS(280));
      object->y = PIXELS(32);
      object->angle = 0;
    } else if (id == 53) {
      Actor_SetPosition(8, PIXELS(632), PIXELS(184));
      object->y = PIXELS(32);
      object->angle = 0;
    }
    motion = Func_02000910(object);
    if (motion != NULL)
      motion->speedLimit = 0x10000;
    gData_02002618 = 1;
  }
}

void Func_0200018c(s32 arg) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 &&
      Func_02001978(arg) != 0)
    FieldEvent_SetExitRequest(13);
}

void Func_020001bc(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 18 && z == 34)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (x == 8 && z == 39)
      FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
    if (Func_02001978(arg) != 0)
      FieldEvent_SetExitRequest(14);
  }
}

void Func_02000228(s32 arg) {
  struct Actor *leader;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader->y <= 0) {
    Func_0200018c(arg);
  } else {
    x = leader->x >> 20;
    z = leader->z >> 20;
    if (x == 25) {
      if (z == 17)
        Func_020000e8(50);
      if (z == 11)
        Func_020000e8(51);
    }
    if (x == 43) {
      if (z == 17)
        Func_020000e8(52);
      if (z == 11)
        Func_020000e8(53);
    }
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_0200130c(s32 kind, s32 x, s32 z, s32 a, s32 b);
void Func_02001a28(s32 dx, s32 dz, s32 effect);
void Func_02000460(s32 mode);

/// The scene's set-up: screen effect 0x204, actor 64's action variant 3 and
/// actor 8 held still, then Func_0200130c and Func_02001a28 by the entrance.
s32 Func_02000290(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  FieldActor_SetSpritePriority(64, 3);
  gData_02002618 = 0;
  Actor_Get(8)->motionFlags = 0;
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Func_0200130c(33, 0x280, 0x380, 48, 1);
    Func_02001a28(0, 0, 0x204);
  } else if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5 ||
             gMainRoutineWork.entrance == 13) {
    Func_0200130c(43, 0x280, 0x380, 49, 1);
    Func_02001a28(-21, 32, 0x204);
  } else {
    Func_0200130c(43, 0x280, 0x380, 0, 0);
    Func_02001a28(-2, -27, 0x204);
  }
  return 0;
}

s32 Func_02000358(void) {
  Scene_SetArrivalFlags(0);
  Func_02000460(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "field_event.h"

extern s32 gData_020025f4;

void Func_02000398(s32 value) { gData_020025f4 = value; }

extern s32 gData_020025f0;
extern u16 gData_0200261c[];

void Func_020003a4(void) {
  u16 colors[16];
  s32 phase = (gData_020025f0 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_020025f4 != 0) {
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
    /* FAKEMATCH(temporary): the slot and the table address held in locals;
     * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_0200261c + offset),
     * 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_0200261c;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_020025f0++;
}

extern u16 gData_0200263c[];
extern u16 gData_02002640[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001bd8();
void Func_02001c98();

void Func_02000460(s32 mode) {
  gData_020025f4 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_0200261c, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_0200263c, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_020025f0 = zero;
    Task_Add(Func_020003a4, 1152);
  } else {
    Dma_Set(gData_02002640, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_020004e4(s32 id) {
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
  Func_02001bd8(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001c98(object->x, object->z, 0, 128);
}

#include "frame.h"
#include "field_sprite.h"

void Func_02001be8(void);
void Func_02001bf0(void);

void Func_02000538(void) {
  /* FAKEMATCH(wrapper): calls forwarded through inlines (call.h's Call and
   * Value, this file's own); as direct calls, 8 of 216 instructions differ in
   * tla-en. */
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Func_02001be8();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->unkd46 = i;

  actor->motionFlags = 4;
  Actor_SetSingleChildFlags(actor, 0);

  while (*((u8 *)linked + 0x63) != 0) {
    actor->x = linked->x;
    actor->y = linked->y + i;
    actor->z = linked->z;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->angle = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  angle = linked->angle;
  actor->x = (((linked->x >> 19) & ~1) + (angle >> 13) - 1) << 19;
  actor->z = (((linked->z >> 19) & ~1) - ((angle >> 13) & 2) + 1) << 19;
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
  if (actor->y > actor->terrainHeight + 0x100000) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->y > actor->terrainHeight + 0x100000);
  }
  Event_PlaySound(127);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
      if (i > 59)
        break;
    } while (actor->velocityY != 0);
  }
  Actor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  work->unkd46 = 1;
  work->exit = 0;
  Func_02001bf0();
}

s32 Func_02000724(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_020025f8[];

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02001c88(s32 x, s32 z, s32 layer);
void Func_02001c78(s32 *position);
s32 Math_Cosine(s32 angle);
void Func_02001b78(struct Actor *object, const s32 *script);

void Func_02000750(struct Actor *self) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->action & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *to = self->animation;
    s8 *from = leader->animation;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (Func_02001c88(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_02001c78(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Func_02000724(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->angle = self->angle;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          {
            s32 delay = 200;
            *(u16 *)(work + 362) = delay;
          }
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      {
        s32 (*multiply)(s32, s32);
        s32 cosine = Math_Cosine(self->angle);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->angle), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Func_02001b78(self, gData_020025f8);
    Event_PlaySound(228);
    gData_02002618 = zero;
  }
}

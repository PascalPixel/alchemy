/// @file
/// The scene code of overlay 0x6a1 that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_8/.

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
#include "event_inline.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_event.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

struct Probe {
  s32 unk00[6];
};

s32 Func_02001f60(void);
s32 Func_02000c44(struct Probe *probe);
void Func_02000ec8(struct Probe probe);

void Func_02000054(void) {
  if (Func_02001f60())
    FieldEvent_SetExitRequest(13);
}

void Func_02000068(void) {
  struct Probe probe;

  if (Func_02000c44(&probe))
    Func_02000ec8(probe);
}

void Func_02000090(void) {}

void Func_020021e0(s32 x, s32 z, s32 a, s32 b);
void Func_020022c0(s32 x, s32 z, s32 a, s32 b);

/// On cell (6, 14), sets flag 0xa18, waits until the object finishes falling
/// and stops (at most 30 frames), then releases it.
void Func_02000094(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 i;
  s32 y;
  s32 floor;

  if (object->x >> 20 == 6 && object->z >> 20 == 14) {
    Event_SetFlag(0xa18);
    System_Wait(10);
    Event_PlaySound(134);
    y = object->y;
    floor = object->terrainHeight;
    /* FAKEMATCH(dead): the height and the floor are read before the counter
     * is cleared and again at the end of each pass; as plain C, 8 of 50
     * instructions differ in tla-en. */
    do {
    } while (0);
    i = 0;
    goto test;
    do {
      System_Wait(1);
      if (++i > 29)
        break;
      y = object->y;
      floor = object->terrainHeight;
    test:;
    } while (y > floor || object->velocityY != 0);
    {
      /* FAKEMATCH(temporary): the motion flags cleared from a zero of their
       * own; as a plain 0 store, 2 of 50 instructions differ in tla-en. */
      s32 cleared = 0;

      object->motionFlags = cleared;
      Func_020021e0(object->x, object->z, 0, 0);
    }
    Func_020022c0(object->x, object->z, 0, 128);
  }
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

struct JupiterLighthouseRoom8EventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_02002248(s32 id);
void Func_02000af0(s32 id);
void Func_020018f4(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_02002010(s32 x, s32 z, s32 a);

s32 Func_02000110(void) {
  ((struct JupiterLighthouseRoom8EventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x204;
  Actor_Get(8)->scaleX = 0xc000;
  Actor_Get(8)->scaleY = 0xc000;
  FieldActor_SetSpritePriority(12, 3);
  FieldActor_SetSpritePriority(14, 3);
  Func_02002248(11);
  Func_02002248(12);
  Func_02002248(13);
  Func_02002248(14);
  Func_02002248(15);
  Func_02000af0(11);
  Func_02000af0(12);
  Func_02000af0(13);
  Func_02000af0(14);
  Func_02000af0(15);
  Func_020018f4(33, 0x280, 0x380, 0, 1);
  if (gMainRoutineWork.entrance == 13)
    Func_02002010(19, 0, 0x204);
  else if (gMainRoutineWork.entrance == 14)
    Func_02002010(-20, -22, 0x204);
  return 0;
}

s32 Func_020001d4(void) {
  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(0x109) == 0)
    Event_ClearFlag(0xa18);
  if (Event_GetFlag(0xa18)) {
    struct Actor *object = Actor_Get(10);

    object->motionFlags = 0;
    object->terrainHeight = -0x200000;
    object->y = -0x200000;
    Actor_SetPosition(10, PIXELS(104), PIXELS(232));
    Func_020021e0(object->x, object->z, 0, 0);
    Func_020022c0(object->x, object->z, 0, 128);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002a88;

void Func_02000270(s32 value) { gData_02002a88 = value; }

extern s32 gData_02002a84;
extern u16 gData_02002ab0[];

void Func_0200027c(void) {
  u16 colors[16];
  s32 phase = (gData_02002a84 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002a88 != 0) {
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
     * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_02002ab0 + offset),
     * 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_02002ab0;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02002a84++;
}

extern u16 gData_02002ad0[];
extern u16 gData_02002ad4[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_02000338(s32 mode) {
  gData_02002a88 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02002ab0, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002ad0, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_02002a84 = zero;
    Task_Add(Func_0200027c, 1152);
  } else {
    Dma_Set(gData_02002ad4, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_020003bc(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
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
  Func_020021e0(object->x, object->z, 0, (height >> 19) + 6);
  Func_020022c0(object->x, object->z, 0, 128);
}

#include "frame.h"
#include "field_sprite.h"
#include "event_work.h"

void Func_020021f0(void);
void Func_020021f8(void);

void Func_02000410(void) {
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

  Func_020021f0();
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
    actor->facing = (u8)gFrameCount << 12;
    p = &actor->priorityFlags;
    *p ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  angle = linked->facing;
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
  Func_020021f8();
}

s32 Func_020005fc(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_02002a8c[];
extern s32 Data_02002aac;

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_020022b8(s32 x, s32 z, s32 layer);
void Func_02002298(s32 *position);
s32 Math_Cosine(s32 angle);
void Func_02002170(struct Actor *object, const s32 *script);

void Func_02000628(struct Actor *self) {
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
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(231);
  {
    s8 *to = self->sprite;
    s8 *from = leader->sprite;
    u32 a = from[9] & 12;
    to[9] = (s8)((to[9] & ~12) | a);
  }
  if (Func_020022b8(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Func_02002298(position);
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
        if (Func_020005fc(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
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
        s32 cosine = Math_Cosine(self->facing);
        s32 speed = self->speedLimit;
        /* FAKEMATCH(dead): the routine address taken inside a once-only
         * block, so the speed loads first; as a plain call, 2 of 197
         * instructions differ in tla-en. */
        do {
          multiply = Iwram_MulQ16;
        } while (0);
        self->x += multiply(cosine, speed);
      }
      self->z += Iwram_MulQ16(Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;
    *((u8 *)self + 0x63) = zero;
    self->update = zero;
    Func_02002170(self, gData_02002a8c);
    Event_PlaySound(228);
    Data_02002aac = zero;
  }
}

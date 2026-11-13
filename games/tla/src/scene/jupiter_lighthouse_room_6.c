/// @file
/// The scene code of overlay 0x69f that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_6/.

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
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern s32 gData_020023c0;
extern s16 gData_0200242a;
extern struct Actor *gData_0200242c[];
extern u16 gData_0200252e;
s32 Func_020018bc(s32 value);
void Func_020000b4(s32 value);
void Func_02000108(void);
void Func_02000154(void);
void Func_020003a4(s32 value);

void Func_02000054(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 && Func_020018bc(value))
    FieldEvent_SetExitRequest(13);
}

void Func_02000084(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 && Func_020018bc(value))
    FieldEvent_SetExitRequest(14);
}

#include "jupiter_lighthouse_room_6/jupiter_lighthouse_room_6.h"

void Func_020000b4(s32 mode) {
  if (mode == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(84, 36, 2, 3, 78, 40);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 39, 2, 3, 78, 40);
  MapGrid_CopyCellAttributes(20, 38, 2, 1, 14, 42);
}

void Func_02000108(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(84, 36, 2, 3, 78, 40);
  System_Wait(6);
  MapGrid_CopyCellsLayered(84, 42, 2, 3, 78, 40);
  MapGrid_CopyCellAttributes(20, 44, 2, 1, 14, 42);
}

void Func_02000154(void) {
  struct JupiterLighthouseRoom6Object *object =
      ((struct JupiterLighthouseRoom6Object *)Actor_Get(12));

  if (!Event_GetFlag(0xa19) && object->x >> 20 == 4 && object->z >> 20 == 37) {
    Event_SetFlag(0xa19);
    Func_020000b4(1);
  }
}

void Func_02000190(void) {
  if (!Event_GetFlag(512)) {
    Event_SetFlag(512);
    Func_020000b4(1);
  }
}

void Func_020001b0(void) {
  if (Event_GetFlag(512) == 1) {
    Event_ClearFlag(512);
    Func_02000108();
  }
  Func_02000154();
}

void Func_020001d0(void) {
  System_Wait(10);
  Func_02000154();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020023bc;
extern u16 gData_020023e8[];

s32 __modsi3();
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001b34();
void Func_02001be4();
void Func_02001250(s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_0200196c(s32 a, s32 b, s32 c);

s32 Scene_Initialize(void) {
  struct JupiterLighthouseRoom6EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  if (Event_GetFlag(0xa19)) {
    Actor_SetPosition(12, PIXELS(72), PIXELS(600));
    Func_020000b4(0);
  }
  if (Event_GetFlag(0x200))
    Func_020000b4(0);
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    Func_02001250(33, 0x280, 0x380, 20, 31);
  } else {
    Func_02001250(43, 0x280, 0x380, 21, 31);
    Func_0200196c(21, -3, 0x204);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  Func_020003a4(0);
  if (!Event_GetFlag(265))
    Event_ClearFlag(0xa19);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "ram_buffer.h"
#include "field_event.h"

void Func_020002dc(s32 value) { gData_020023c0 = value; }

void Func_020002e8(void) {
  u16 colors[16];
  s32 phase = (gData_020023bc / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_020023c0 != 0) {
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
     * as colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_020023e8 + offset),
     * 2 of 83 instructions differ in tla-en. */
    s32 slot = (phase + i) % 5;
    u8 *table = (u8 *)gData_020023e8;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_020023bc++;
}

extern u16 gData_02002408[];
extern u16 gData_0200240c[];

void Func_020003a4(s32 mode) {
  gData_020023c0 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_020023e8, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002408, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_020023bc = zero;
    Task_Add(Func_020002e8, 1152);
  } else {
    Dma_Set(gData_0200240c, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_02000428(s32 id) {
  struct JupiterLighthouseRoom6Object *object;
  s32 height;

  object = ((struct JupiterLighthouseRoom6Object *)Actor_Get(id));
  object->motionFlags = 4;
  height = object->height;
  WORD_AT(object, 68) = 0;
  object->y = height + 0x200000;
  object->priorityFlags |= 128;
  height = Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z);
  Func_02001b34(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001be4(object->x, object->z, 0, 128);
}

#include "frame.h"
#include "field_sprite.h"
#include "event_work.h"

void Func_0200047c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  u8 *p;
  s32 angle;
  s32 i;

  Event_Start();
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
  Event_End();
}

s32 Func_02000668(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern const s32 gData_0200a3c4[];
extern s32 Data_020023e4;

void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02001bdc(s32 x, s32 z, s32 layer);
void Camera_WorldToScreen(s32 *position);
s32 Math_Cosine(s32 angle);
void Actor_StartScript(struct Actor *object, const s32 *script);

void Func_02000694(struct Actor *self) {
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
  if (Func_02001bdc(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
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
        if (Func_02000668(&leader->x, &self->x) <= 12 && distance < 0xc0000)
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
    Actor_StartScript(self, gData_0200a3c4);
    Event_PlaySound(228);
    Data_020023e4 = zero;
  }
}

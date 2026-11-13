/// @file
/// The scene code of overlay 0x69a that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_1/.

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
#include "field_event.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "debug.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(void) { Import_EventSetExitRequest(6); }

void Func_02000060(s32 animate) {
  if (animate == 1) {
    Event_PlaySound(133);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 39);
}

void Func_020000ec(void) {
  Event_PlaySound(134);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
  System_Wait(6);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 39);
}

void Func_02000178(void) {
  struct Actor *object = Actor_Get(10);

  if (!Event_GetFlag(0xa01) && object->x >> 20 == 17 && object->z >> 20 == 33) {
    Event_SetFlag(0xa01);
    Func_02000060(1);
  }
}

void Func_020001b4(void) {
  if (!Event_GetFlag(0x200)) {
    Event_SetFlag(0x200);
    Func_02000060(1);
  }
}

void Func_020001d4(void) {
  if (Event_GetFlag(0x200) == 1) {
    Event_ClearFlag(0x200);
    Func_020000ec();
  }
  Func_02000178();
}

void Func_020001f4(void) {
  System_Wait(10);
  Func_02000178();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Func_02001bcc(s32 owner);

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  if (!Event_GetFlag(0x109) && gMainRoutineWork.entrance == 6)
    Func_02001bcc(gMainRoutineWork.currentOwner);
  if (Event_GetFlag(0xa01)) {
    Actor_SetPosition(10, PIXELS(280), PIXELS(536));
    Func_02000060(0);
  }
  if (Event_GetFlag(0x200))
    Func_02000060(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002230;

void Func_020002bc(s32 value) { gData_02002230 = value; }

extern s32 gData_0200222c;
extern u16 gData_02002258[];

void Func_020002c8(void) {
  u16 colors[16];
  s32 phase = (gData_0200222c / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002230 != 0) {
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
    u8 *table = (u8 *)gData_02002258;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_0200222c++;
}

extern u16 gData_02002278[];
extern u16 gData_0200227c[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001b14();
void Func_02001bc4();

void Func_02000384(s32 mode) {
  gData_02002230 = 1;
  if (mode != 2) {
    Iwram_CopyWords(gData_02002258, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWords(gData_02002278, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_0200222c = zero;
    Task_Add(Func_020002c8, 1152);
  } else {
    Dma_Set(gData_0200227c, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_02000408(s32 id) {
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
  Func_02001b14(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001bc4(object->x, object->z, 0, 128);
}

#include "actor.h"
#include "frame.h"
#include "event_work.h"

void Event_Start(void);
void Event_End(void);

void Func_0200045c(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Event_Start();
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
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->unkd44 = 1;
  work->unkd46 = 1;
  facing = linked->facing;
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
  Event_End();
}

s32 Func_02000648(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

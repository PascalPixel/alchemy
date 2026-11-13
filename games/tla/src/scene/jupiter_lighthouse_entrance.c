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

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"
s32 Func_02001b1c();

void Func_02000054(void) { Call7(Func_02001b1c, 0x38, 0, 4, 2, 0x46, 0x13, 1); }

void Func_02000074(void) { Call7(Func_02001b1c, 0x38, 0, 4, 2, 0x45, 0x16, 1); }

void Func_02000094(void) { Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1); }

void Func_020000b4(void) { Call7(Func_02001b1c, 0x38, 0, 5, 3, 0x55, 0x1a, 1); }

void Func_020000d4(void) { Call7(Func_02001b1c, 0x38, 0, 5, 3, 0x5e, 0x1a, 1); }

#include "main_routine_work.h"

void Func_02001a7c(void);
void Func_02001a84(void);

void Func_020000f4(void) {
  s32 t0;
  s32 t2;

  t0 = Actor_Get(gMainRoutineWork.currentOwner);
  Call7(Func_02001b1c, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
  t2 = Event_GetFlag(0xfe2);
  if (t2 == 0) {
    Actor_SetPosition(0x40, 0x1180000, 0x1b80000);
    if ((FIELD_AT(t0, s32, 8) >> 20) == 0x11) {
      if ((FIELD_AT(t0, s32, 16) >> 20) == 0x1b) {
        Func_02001a7c();
        FieldEvent_ResolvePendingActions(0);
        Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
        Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, -16, 0);
        FIELD_AT(t0, s32, 40) = 0x60000;
        FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
        Func_02001a84();
      }
    }
  }
}

#include "ram_buffer.h"
#include "scene_ids.h"
#include "debug.h"

void Func_02000184(void) { Import_EventSetExitRequest(4); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_02001b3c();

s32 Func_02000198(void) {
  s32 clear;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  clear = Event_GetFlag(0x109);
  if (clear == 0) {
    if (gMainRoutineWork.entrance == 4)
      Func_02001b3c(gMainRoutineWork.currentOwner);
  }
  gMainRoutineWork.savedScene = (s32)&SceneId_Ov694Room2;
  gMainRoutineWork.savedEntrance = 6;
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

extern s32 gData_02002198;

void Func_0200022c(s32 a0) { gData_02002198 = a0; }

#include "jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

extern s32 gData_02002194;
extern u16 gData_020021c0[];

void Func_02000238(void) {
  u16 colors[16];
  s32 phase = (gData_02002194 / 4) % 5;
  s32 i;
  s32 offset;
  u16 *p;

  if (gData_02002198 != 0) {
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
    u8 *table = (u8 *)gData_020021c0;

    colors[slot] = *(u16 *)(table + offset);
  }
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(p, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_02002194++;
}

#include "field_event.h"

extern u16 gData_020021e0[];
extern u16 gData_020021e4[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02001a6c();
void Func_02001b34();

void Func_020002f4(s32 mode) {
  gData_02002198 = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gData_020021c0, (void *)&BG_PLTT_COLOR(12, 0), 32);
    Iwram_CopyWordsEntry(gData_020021e0, (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(0xa00) || mode == 1) {
    /* FAKEMATCH(temporary): a local holding 0, so the zero is built in r2
     * and the counter's address in r3; as the 0 stored directly, 6 of 44
     * instructions differ in tla-en. */
    s32 zero = 0;

    gData_02002194 = zero;
    Task_Add(Func_02000238, 1152);
  } else {
    Dma_Set(gData_020021e4, (void *)&BG_PLTT_COLOR(12, 2), DMA_COPY_WORDS(2),
            REG_DMA3);
  }
}

void Func_02000378(s32 id) {
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
  Func_02001a6c(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001b34(object->x, object->z, 0, 128);
}

#include "actor.h"
#include "frame.h"
#include "event_work.h"

void Func_020003cc(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct Actor *linked = actor->watchedObject;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Func_02001a7c();
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
  Func_02001a84();
}

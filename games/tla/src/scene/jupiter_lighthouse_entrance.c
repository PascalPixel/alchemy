#include "shared/routines.h"
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
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_entrance.h"
#include "module/scene/scene.h"
#include "module/tla.h"

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
        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x102);
        Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, -16, 0);
        FIELD_AT(t0, s32, 40) = 0x60000;
        FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
        Event_End();
      }
    }
  }
}

#include "ram_buffer.h"
#include "scene_ids.h"
#include "debug.h"

void Func_02000184(void) { Import_EventSetExitRequest(4); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }


s32 Scene_Initialize(void) {
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

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gData_02002194 = value; }

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

void Func_02001a6c();

DEFINE_FUNC_020002F4(Func_020002f4, gData_02002198, gData_020021c0,
    gData_020021e0, Func_02000238, gData_020021e4)

DEFINE_FUNC_02000378(Func_02000378, Func_02001a6c, Func_02001b34)

#include "actor.h"
#include "frame.h"
#include "event_work.h"

DEFINE_FUNC_020003CC(Func_020003cc)

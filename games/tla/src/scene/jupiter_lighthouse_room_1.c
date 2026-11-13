/// @file
/// The scene code of overlay 0x69a that its scene tables belong to, in ROM order; the rest of the overlay is in jupiter_lighthouse_room_1/.

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
#include "event_inline.h"
#include "field_event.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "debug.h"
#include "event_work.h"
#include "module/scene/jupiter_lighthouse_room_1.h"
#include "module/scene/scene.h"
#include "module/tla.h"

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

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gData_0200222c = value; }

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

void Func_02001b14();
void Func_02001bc4();

DEFINE_FUNC_020002F4(Func_02000384, gData_02002230, gData_02002258,
    gData_02002278, Func_020002c8, gData_0200227c)

DEFINE_FUNC_02000378(Func_02000408, Func_02001b14, Func_02001bc4)

#include "actor.h"
#include "frame.h"
#include "event_work.h"

DEFINE_FUNC_020003CC(Func_0200045c)

DEFINE_FUNC_020003D0(Func_02000648)

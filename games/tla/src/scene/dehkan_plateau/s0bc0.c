/// @file
/// Scene code of overlay 0x655, in ROM order.

#include "../shared/map_cells.h"
#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "map.h"
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "field.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"

DEFINE_ACTOR_SET_CELL_SHAPE(Actor_SetCellShape, Func_02003c10, Func_02003dd0)

extern const u8 gData_02003f44[];
extern const u8 gData_02003f52[];
extern const u8 gData_02003f80[];
extern const u8 gData_02003f92[];
extern const u8 gData_02003f98[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fa8[];
extern const u8 gData_02003fb0[];
extern const u8 gData_02003fb8[];
extern const u8 gData_02003fc0[];
extern const u8 gData_02003fc4[];

void Func_020002f8(const void *data);

void Func_02002180(void);
void Func_02001804(void);

extern const u16 gData_02003fd2[];
extern const struct CellRecord gData_02004066[];


void Func_02000c4c(s32 a, s32 b) {
  Actor_SetCellShape(a, b, gMainRoutineWork.currentOwner);
}

void Func_020006ec(struct Actor *object, struct Actor *other);
void Actor_StartScript(struct Actor *object, const void *script);
void Actor_MoveToObject(s32 id, s32 target);

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
DEFINE_PILLARS_RAISE(Pillars_Raise, Func_02003c10, Func_02003db0, Func_02003dd0)

#define HELD_OBJECT(work) (*(struct Actor **)((u8 *)(work) + 0xcbc))

void Func_02000d78(u16 *list) {
  struct Actor *held = Func_02003df0();
  void *work = Ram_HeapSlots->eventWork;
  u8 *shapeGrid = gMapShapeGrid;

  if (held == NULL) {
    s32 id = BattleEffect_FindNearestObject(gMainRoutineWork.currentOwner, 1);

    if (id != -1)
      held = Actor_GetById(id);
  }
  HELD_OBJECT(work) = NULL;
  if (*list != 0xffff) {
  loop:
    if (held == Actor_GetById(*list)) {
      u8 *layer = &held->terrainId;
      struct MapCell *cells = Ram_HeapSlots->mapWork->layers[*layer].cells;
      u8 *shapes = shapeGrid + (cells - (struct MapCell *)Ram_MapCellBuffer);
      s32 shape = Func_02003c10(*layer, held->x, held->z);
      s32 level = Map_GetTerrainHeight(*layer, held->x, held->z);
      s32 index = Func_02003db0(held);
      s32 value;

      shapes += index;
      value = Func_02003dd0(*layer, shape, (level >> 19) - 4);
      cells += index;
      /* FAKEMATCH(signature): the shape grid byte stored through a bit-field
         view (struct ShapeBits), whose alias set lets the cell's attribute be
         read first; as `*shapes = value;`, 4 of 131 instructions differ in
         tla-en. */
      ((struct ShapeBits *)shapes)->shape = value;
      cells->attributeB &= 63;
      cells += 128;
      cells->attributeB &= 191;
      HELD_OBJECT(work) = held;
    }
    list++;
    if (*list != 0xffff)
      goto loop;
  }
}

// One-field view: the shape store is memory of its own to sched2, which sinks
// it past the cell read; a plain store keeps its order.

DEFINE_HELD_ACTOR_RAISE_CELL(HeldActor_RaiseCell, Func_02003c10, Func_02003db0, Func_02003dd0)

#include "../../field/object/set_sprite_priority.c"

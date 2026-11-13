/// @file
/// The room of grey blocks and locked doors in Jupiter Lighthouse: the cracked
/// tiles' steps and the fall to the floor below.

#include "../shared/tables.h"
#include "../shared/teleporter.h"
#include "types.h"
#include "cell_target.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_effect.h"
#include "event_inline.h"
#include "event_work.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"

struct CellTarget;
struct CellTargetEntry;
extern struct CellTarget gTeleporters;
void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByFirstCyclone(void);
void JupiterLighthouse_LeaveBySecondCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
extern u32 gFieldEffect_LongScript[];

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_TELEPORTER_DROP_IN(Teleporter_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);
void Teleporter_Sink(struct CellTarget *puzzle, struct CellTargetEntry *tile);

s32 Teleporters_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *puzzle = &gTeleporters;
  struct CellTargetEntry *tile = puzzle->entries;
  s32 i;

  for (i = 0; i < puzzle->count; i++, tile++) {
    if (tile->x == x && tile->z == z) {
      if (!Event_GetFlag(tile->flag)) {
        Teleporter_Uncover(puzzle, tile);
        Event_SetFlag(tile->flag);
        puzzle->current = i;
        break;
      }
      if (i == puzzle->current)
        break;
      puzzle->current = i;
      Teleporter_Sink(puzzle, tile);
      GameFlag_SetByte(puzzle->flagByte, x);
      GameFlag_SetByte(puzzle->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void Teleporter_PlaceArrival(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct CellTarget *work = &gTeleporters;
  s32 x = ((s32)FieldGameFlag_GetByte(work->flagByte));
  s32 z = ((s32)FieldGameFlag_GetByte(work->flagByte + 8));

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Map_CenterOnFollowedPoint();
    Teleporter_DropIn();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = value;
  }
}

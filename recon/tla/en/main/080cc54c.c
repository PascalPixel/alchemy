// Draft, differ 46 of 138: as ☀️ Object_GetTriggerTileAheadOfCurrent
// (games/tbs/src/object/object.c), returns the collision code of the cell
// 16 pixels ahead of the party leader when there is something to search:
// codes 242 to 247 whose ground is no more than 16 pixels above the leader,
// or a code with a search event (Func_080ccd78(3, code)). New in ⚓️: on the
// world map with an owner override, world map collision 3 there gives 99.
// Left: instruction order and register choice in the cell address: the
// native loads the probe's x and z (and in a town the cell table pointer)
// before the divisions, where the draft loads each beside its use, and the
// 31 mask and rounding constants take r0/r1 the other way round.
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

struct LeaderObject {
  u8 unk00[6];
  u16 angle;
  struct Vector3 pos;
  u8 unk14[0x0e];
  u8 kind;
};

struct FieldWorkView {
  u8 unk000[0x18a];
  s8 mode;
};

struct MapWorkView {
  u8 unk000[0x138];
  u8 *cells;
};

enum { MODE_WORLD_MAP = 3 };

struct LeaderObject *Func_080cb8e8(s32 actor);
void Vector_AddPolarOffset(s32 radius, s32 heading, s32 *position);
s32 Map_GetTerrainHeightFar(s32 kind, s32 x, s32 z);
s32 Func_080ccd78(s32 group, s32 id);
s32 GetWorldMapCollisionFar(struct Vector3 *position);

extern u8 gMapBlocks[];

s32 Func_080cc54c(void) {
  s32 result = 0;
  struct LeaderObject *leader = Func_080cb8e8(0);
  struct FieldWorkView *work = Ram_HeapSlots->eventWork;
  struct MapWorkView *map = (struct MapWorkView *)Ram_HeapSlots->mapWork;
  struct Vector3 probe;
  u8 *cell;
  s32 code;
  s32 height;

  if (leader != NULL) {
    probe.x = leader->pos.x;
    probe.y = leader->pos.y;
    probe.z = leader->pos.z;
    Vector_AddPolarOffset(0x100000, leader->angle, &probe.x);
    if (work->mode == MODE_WORLD_MAP) {
      cell = gMapBlocks + ((((probe.x / 0x200000) & 31) +
                            (((probe.z / 0x200000) & 31) << 5))
                           << 2);
    } else {
      cell = map->cells +
             (((probe.x / 0x100000) + ((probe.z / 0x100000) << 7)) << 2);
    }
    code = cell[2];
    if (code >= 242 && code <= 247) {
      height = Map_GetTerrainHeightFar(leader->kind, probe.x, probe.z);
      if (height >= leader->pos.y && height <= leader->pos.y + 0x100000)
        result = code;
    } else if (Func_080ccd78(3, code) != 0) {
      result = code;
    }
    if (result == 0 && work->mode == MODE_WORLD_MAP &&
        gPartyState.ownerOverride != 0 &&
        GetWorldMapCollisionFar(&probe) == 3)
      result = 99;
  }
  return result;
}

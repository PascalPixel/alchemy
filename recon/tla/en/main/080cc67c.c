// Draft, differ 6 of 154: as ☀️ Scene_ApplyEntrance
// (games/tbs/src/object/object.c), places the party at its entrance and sets
// the map's scroll bounds, at least a screen wide and tall. In ⚓️ it takes a
// flag: when set, the entrance must be found (an entrance with its required
// flag set, or -1) or it returns 0 without change; when clear the first
// entry is used. Flag 0x109 keeps the party position. Returns 1. The party
// position is in PartyState's unk1fc, read through a view of gPartyState
// (with a byte-offset cast the five stores share one folded base, differ
// 56). Left: instruction order only (the pool load of gPartyState against
// the argument copy, the -1 build and the 240 << 16 shift).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "field_scene.h"

s32 GameFlag_Test(s32 flag);

// The scene overlay's entry table; its fourth entry returns the entrances.
extern void *gOverlayArea[];

struct MapScrollView {
  u8 unk000[0xec];
  s32 minX;
  s32 minY;
  s32 maxX;
  s32 maxY;
};

#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))
// The party position fields in unk1fc of struct PartyState.
struct PartyPositionView {
  u8 unk000[0x1fc];
  s32 x;
  s32 y;
  s32 z;
  u32 heading;
  u16 turn;
};

#define gPartyPosition (*(struct PartyPositionView *)&gPartyState)

s32 Func_080cc67c(s32 search) {
  struct MapScrollView *map = (struct MapScrollView *)Ram_HeapSlots->mapWork;
  s32 entrance = gPartyState.entrance;
  const struct SceneEntrance *entry =
      ((const struct SceneEntrance *(*)(void))gOverlayArea[3])();
  s32 found = 0;

  if (search != 0) {
    while (entry->entrance != -1) {
      if (entry->entrance == entrance &&
          (entry->requiredFlag == -1 || GameFlag_Test(entry->requiredFlag))) {
        found = 1;
        break;
      }
      entry++;
    }
    if (!found)
      return 0;
  }

  if (!GameFlag_Test(0x109)) {
    gPartyPosition.x = REGION_FIXED(entry->x);
    gPartyPosition.y = REGION_FIXED(entry->y);
    gPartyPosition.z = REGION_FIXED(entry->z);
    gPartyPosition.heading = entry->facing;
    gPartyPosition.turn = 0;
  }

  if (entry->cameraLeft != -1)
    map->minX = REGION_FIXED(entry->cameraLeft);
  if (entry->cameraTop != -1)
    map->minY = REGION_FIXED(entry->cameraTop);
  if (entry->cameraRight != -1)
    map->maxX = REGION_FIXED(entry->cameraRight);
  if (entry->cameraBottom != -1)
    map->maxY = REGION_FIXED(entry->cameraBottom);

  if (map->minX + (240 << 16) > map->maxX)
    map->minX = map->maxX - (240 << 16);
  if (map->minY + (160 << 16) > map->maxY)
    map->minY = map->maxY - (160 << 16);
  return 1;
}

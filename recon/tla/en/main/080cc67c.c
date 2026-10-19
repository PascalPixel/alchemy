// Near miss, differ 4 of 154 (2026-10-18): the twin of ☀️'s
// Scene_ApplyEntrance, here with an argument (1: find the entrance and fail
// with 0 when there is none; 0: take the first). The empty asm barrier after
// the camera's left edge is read moves the -1 constant below the load as the
// native has it (differ 6 without it; tag it under S2 before adoption). Left:
// the gPartyState literal is loaded after the argument copy (the native loads
// it first), and the 240 << 16 shift is finished before map->maxX is loaded.
// Tried: the comparison written the other way round (8), a width variable
// behind a "+r" asm (87), six minutes of permute (nothing below 4 without the
// barrier).
#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_scene.h"

struct OverlayHeader {
  void *unk00[3];
  const struct SceneEntrance *(*entrances)(void);
};

extern struct OverlayHeader gOverlayArea;
s32 GameFlag_Test(s32 flag);

s32 Func_080cc67c(s32 findEntrance) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 entrance = gPartyState.entrance;
  const struct SceneEntrance *entry = gOverlayArea.entrances();
  s32 found = 0;
  s32 left;

  if (findEntrance) {
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
    gPartyState.x = entry->x << 16;
    gPartyState.y = entry->y << 16;
    gPartyState.z = entry->z << 16;
    gPartyState.heading = entry->facing;
    gPartyState.turn = 0;
  }

  left = entry->cameraLeft;
  __asm__ volatile("");
  if (left != -1)
    map->minX = left << 16;
  if (entry->cameraTop != -1)
    map->minZ = entry->cameraTop << 16;
  if (entry->cameraRight != -1)
    map->maxX = entry->cameraRight << 16;
  if (entry->cameraBottom != -1)
    map->maxZ = entry->cameraBottom << 16;

  if (map->minX + (240 << 16) > map->maxX)
    map->minX = map->maxX - (240 << 16);
  if (map->minZ + (160 << 16) > map->maxZ)
    map->minZ = map->maxZ - (160 << 16);
  return 1;
}

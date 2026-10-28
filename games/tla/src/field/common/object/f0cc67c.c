#include "types.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "map.h"
#include "field_scene.h"

struct OverlayHeader {
  void *unk00[3];
  const struct SceneEntrance *(*entrances)(void);
};

extern struct OverlayHeader gOverlayArea;
s32 GameFlag_Test(s32 flag);

/// The twin of The Broken Seal's Scene_ApplyEntrance: places the party at
/// its entrance from the area overlay's entrance list and clamps the camera
/// to that entrance's bounds. With findEntrance it looks the entrance up and
/// returns 0 when there is none; without, it takes the first row.
s32 Func_080cc67c(s32 findEntrance) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 entrance = gAdventure.entrance;
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
    gAdventure.x = entry->x << 16;
    gAdventure.y = entry->y << 16;
    gAdventure.z = entry->z << 16;
    gAdventure.heading = entry->facing;
    gAdventure.turn = 0;
  }

  left = entry->cameraLeft;
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

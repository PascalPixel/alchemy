#include "types.h"

struct MapFocusObject {
  u8 padding00[6];
  u16 kind;
  s32 x;
  s32 unk0c;
  s32 y;
};

struct MapFocusPosition {
  s32 x;
  s32 unk04;
  s32 y;
};

void Vector_AddPolarOffset(s32, u32, struct MapFocusPosition *);

extern struct EventPairWork1d6 gGameState;
extern struct EventRuntime *gEventWork;

void UpdateMapRegionAtPosition(s32 positionX, s32 positionY, s32 positionZ) {
  s32 selectedValue;
  struct EventRuntime *runtime;
  s32 z;
  s32 y;
  s32 x;
  s16 condition;
  s16 minZ;
  s16 maxY;
  s16 minY;
  s32 maxZ;
  s16 maxX;
  s16 minX;
  struct MapRegion *region;

  x = positionX;
  y = positionY;
  z = positionZ;
  region = gOverlayArea.region_provider();
  if (region != 0 && (runtime = gEventWork, minX = region->minX, minX != -1)) {
  loop:
    minY = region->minY;
    minZ = region->minZ;
    maxX = region->maxX;
    maxY = region->maxY;
    maxZ = region->maxZ;
    condition = region->condition;
    selectedValue = region->value;
    if (GameFlag_IsConditionActive(condition) != 0 && y >= REGION_FIXED(minY) &&
        y < REGION_FIXED(maxY) && x >= REGION_FIXED(minX) &&
        x < REGION_FIXED(maxX) && z >= REGION_FIXED(minZ) &&
        z < REGION_FIXED(maxZ)) {
      runtime->value170 = (u16)selectedValue;
      Audio_PlayCue(123);
      Battle_InitializeRenderObject();
      return;
    }
    runtime = gEventWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

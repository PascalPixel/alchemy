// Under -mthumb-slow-loads (2026-10-26): differ 6 of 104 in tla-en, instruction
// order and register choice. An empty asm before the loop lowers it to 4.
// Draft of Func_080cb1dc (UpdateMapRegionAtPosition here): differ 8 of 104
// (tla-en, 2026-10-18). It asks the overlay for the map's region list and,
// at the first region whose condition holds and whose box holds the
// position, stores the region's value in the event work, plays cue 123 and
// prepares the current object.
//
// Left: instruction order only (the overlay table's load, a negation and two
// register copies each sit one or two places apart).

#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"

// A box of the map in whole cells (16.16 when compared) and the value it
// selects while its condition holds; a row with minX -1 ends the list.
struct MapRegion {
  s16 minX;
  s16 minY;
  s16 minZ;
  s16 maxX;
  s16 maxY;
  s16 maxZ;
  s16 condition;
  s16 value;
};

#define REGION_FIXED(value) ((value) << 16)

// The overlay's entry table; entry 11 returns the map's region list.
extern void *gOverlayArea[];

s32 GameFlag_IsConditionActive(s32 condition);
void Sound_PlayFar(s32 cue);

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


void UpdateMapRegionAtPosition(s32 positionX, s32 positionY, s32 positionZ) {
  s32 selectedValue;
  struct EventWork *runtime;
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
  region = ((struct MapRegion *(*)(void))gOverlayArea[11])();
  if (region != 0 && (runtime = Ram_HeapSlots->eventWork, minX = region->minX, minX != -1)) {
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
      runtime->countdown = (u16)selectedValue;
      Sound_PlayFar(123);
      Leader_StopMoving();
      return;
    }
    runtime = Ram_HeapSlots->eventWork;
    region++;
    minX = region->minX;
    if (minX != -1)
      goto loop;
  }
}

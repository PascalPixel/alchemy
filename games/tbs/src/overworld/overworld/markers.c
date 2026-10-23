/// @file
/// The map's treasure spots: chests and Psynergy Stones.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Sets an object's y and groundHeight to the map's height at its x and z.
/// Called by Field_PlaceTreasureSpots for each treasure spot it places.
void ObjectMotion_SnapToTerrain(void *object) {
  struct Object *objectEvent = object;
  s32 groundHeight;

  groundHeight = MapGrid_GetTerrainHeightFar(0, objectEvent->x, objectEvent->z);
  objectEvent->y = groundHeight;
  objectEvent->groundHeight = groundHeight;
}

/// The map grid's work (gMapGridWork[0], HEAP_SLOT_MAP_GRID), seen as its
/// treasure spot list.
struct MapGridMarkers {
  /// MapGridScrollWork's origin and shake; not named through this view.
  ///@{
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  ///@}
  /// MapGridScrollWork's script, read here as (column, row, id) triples, ended
  /// by 255, 255: a 16-pixel cell and the trigger of the map event there; NULL
  /// for a map with none.
  u8 *markers;
};

/// The sprites of the objects a map's treasure spots place.
enum {
  TREASURE_SPOT_CHEST_SPRITE = 20,
  TREASURE_SPOT_PSYNERGY_STONE_SPRITE = 28
};

/// The chest sprite's animations.
enum {
  TREASURE_SPOT_CHEST_OPEN = 2,    ///< standing open
  TREASURE_SPOT_CHEST_OPENING = 4, ///< opening
  TREASURE_SPOT_CHEST_CLOSING = 5, ///< closing again on what nobody could carry
  TREASURE_SPOT_CHEST_MIMIC = 7    ///< jumping up as a mimic
};

/// Places an object at each of the map's treasure spots (ids 100 to 239) that
/// the map has an event for, up to ten: a chest for a chest event, shown open
/// once its flag is set and gone once its mimic was beaten, and a Psynergy
/// Stone for a bg event giving one, until it is taken. A treasure spot with no
/// such event takes no slot. The objects are kept in work->treasureSpots, not
/// in object event slots. Called by Map_Run on map load; clears and fills
/// work->treasureSpots.
void Field_PlaceTreasureSpots(void) {
  u32 id;
  struct MapGridMarkers *grid = gMapGridWork[0];
#ifdef UBFIX
  struct FieldWork *work = gFieldWork;
#else
  // UB: the event cell is a separate global, beyond gMapGridWork's one pointer.
  // Reading gFieldWork plainly grows 0x254 to 0x25c bytes in English.
  struct FieldWork *work = gMapGridWork[HEAP_SLOT_FIELD - HEAP_SLOT_MAP_GRID];
#endif
  s32 markerCount = 0;
  u8 *list = grid->markers;
  struct TreasureSpot *marker = work->treasureSpots;
  const struct MapEvent *event;
  struct Object *markerObjectEvent;
  u32 column;
  u32 row;

  Dma_Fill32(0, marker, sizeof(struct TreasureSpot) * TREASURE_SPOT_MAX);
  // A map without a treasure spot list places nothing.
  if (list == NULL)
    return;

  column = *list++;
  row = *list++;
  while (column != 255 || row != 255) {
    id = *list++;
    if (id >= 100 && id <= 239) {
      // The first event with this trigger that places something wins; the
      // event's value says what the search gives (its FIND_ type, as
      // Field_SearchCell reads it).
      event = gOverlayArea.events();
      for (; event->kind != -1; event++) {
        if (event->trigger != id)
          continue;
        if ((event->kind & 0x1ff) == BG_EVENT(BG_EVENT_CHEST)) {
          // A chest, at the centre of its cell (cells are 16 pixels, and
          // positions 16.16).
          markerObjectEvent = (struct Object *)Object_CreateFar(
              TREASURE_SPOT_CHEST_SPRITE, (column << 20) + 0x80000, 0,
              (row << 20) + 0x80000);
          if (markerObjectEvent == NULL)
            continue;
          ObjectMotion_SnapToTerrain(markerObjectEvent);
          ObjectDispatch_SetSingleChildFlagsFar(
              (struct Object *)markerObjectEvent, 0);
          // An opened chest shows open; an opened mimic's chest
          // is not placed at all. A mimic's flag is set only after its battle,
          // when the one enemy was defeated (Djinn_ResolvePendingEvent).
          if (GameFlag_GetFar(event->condition)) {
            if ((event->value & 0xfff00000) == 0x500000) {
              ObjectDispatch_ReleaseFar((struct Object *)markerObjectEvent);
              continue;
            }
            ObjectDispatch_SelectAnimationFar(markerObjectEvent,
                                              TREASURE_SPOT_CHEST_OPEN);
          }
          ObjectDispatch_StartIdleFar((struct Object *)markerObjectEvent);
          markerObjectEvent->homeX = markerObjectEvent->x / 0x10000;
          markerObjectEvent->homeZ = markerObjectEvent->z / 0x10000;
          markerObjectEvent->priorityFlags = 1;
          markerObjectEvent->collisionFlags = 1;
          marker->id = event->trigger;
          marker->object = (struct Object *)markerObjectEvent;
          marker->column = markerObjectEvent->x / 0x100000;
          marker->row = markerObjectEvent->z / 0x100000;
          marker++;
          markerCount++;
          if (markerCount >= TREASURE_SPOT_MAX)
            return;
        } else if ((event->kind & 0x1ff) == MAP_EVENT_BG) {
          // A Psynergy Stone not yet taken.
          if ((event->value & 0xfff00000) != FIND_PSYNERGY_STONE)
            continue;
          if (GameFlag_GetFar(event->condition))
            continue;
          markerObjectEvent = (struct Object *)Object_CreateFar(
              TREASURE_SPOT_PSYNERGY_STONE_SPRITE, (column << 20) + 0x80000, 0,
              (row << 20) + 0x80000);
          if (markerObjectEvent == NULL)
            continue;
          ObjectMotion_SnapToTerrain(markerObjectEvent);
          ObjectDispatch_SetSingleChildFlagsFar(
              (struct Object *)markerObjectEvent, 0);
          ObjectDispatch_StartIdleFar((struct Object *)markerObjectEvent);
          ObjectDispatch_SelectAnimationFar(markerObjectEvent, 1);
          markerObjectEvent->homeX = markerObjectEvent->x / 0x10000;
          markerObjectEvent->homeZ = markerObjectEvent->z / 0x10000;
          markerObjectEvent->collisionFlags = 1;
          markerObjectEvent->priorityFlags = 1;
          marker->object = (struct Object *)markerObjectEvent;
          marker->id = event->trigger;
          marker->column = markerObjectEvent->x / 0x100000;
          marker->row = markerObjectEvent->z / 0x100000;
          marker++;
          markerCount++;
          if (markerCount >= TREASURE_SPOT_MAX)
            return;
        } else {
          continue;
        }
        break;
      }
    }
    column = *list++;
    row = *list++;
  }
}

/// Returns the treasure spot slot (below TREASURE_SPOT_MAX) whose event trigger
/// is id, or -1. Called by the TreasureSpot_ functions below. Unused slots hold
/// id 0, which no treasure spot has (ids are 100 to 239).
s32 TreasureSpot_FindSlot(s32 id) {
  s32 slotIndex;
  s32 matchingSlot;
  struct TreasureSpot *marker;

  marker = gFieldWork->treasureSpots;
  matchingSlot = -1;
  for (slotIndex = 0; slotIndex <= TREASURE_SPOT_MAX - 1;
       slotIndex++, marker++) {
    if (marker->id == id) {
      matchingSlot = slotIndex;
      break;
    }
  }
  return matchingSlot;
}

/// Unused: an empty function nothing calls.
UNUSED void TreasureSpot_ReservedNoOp(void) {}

/// Gives the object of treasure spot id animation, plays sound and waits 12
/// frames.
static __inline__ void SetMarkerAnimationAndPlaySound(s32 id, s32 animation,
                                                      s32 sound) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Object *object;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    object = marker->object;
    if (object != NULL)
      ObjectDispatch_SelectAnimationFar(object, animation);
    Sound_PlaySongFar(sound);
    WaitFrames(12);
  }
}

/// Closes the chest of treasure spot id again (its item could not be carried),
/// with SE_125. Called by Field_SearchCell.
void TreasureSpot_CloseChest(s32 id) {
  SetMarkerAnimationAndPlaySound(id, TREASURE_SPOT_CHEST_CLOSING, SE_125);
}

/// Makes the chest of treasure spot id jump up as a mimic: after 18 frames it
/// takes TREASURE_SPOT_CHEST_MIMIC, plays SE_146 and gets an upward velocityY
/// of 8 << 16. Called by Field_SearchCell before the mimic's battle.
void TreasureSpot_LaunchMimic(s32 id) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Object *object;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    object = marker->object;
    WaitFrames(18);
    if (object != NULL)
      ObjectDispatch_SelectAnimationFar(object, TREASURE_SPOT_CHEST_MIMIC);
    Sound_PlaySongFar(SE_146);
    if (object != NULL) {
      object->velocityY = 8 << 16;
      ObjectDispatch_SetSingleChildFlagsFar((struct Object *)object, 1);
    }
  }
}

/// Opens the chest of treasure spot id with SE_124, and waits 12
/// frames. Called by Field_SearchCell when a chest is searched.
void TreasureSpot_OpenChest(s32 id) {
  SetMarkerAnimationAndPlaySound(id, TREASURE_SPOT_CHEST_OPENING, SE_124);
}

/// Shows the chest of treasure spot id open, with no sound. Called by
/// Field_SearchCell once a chest's contents are taken.
void TreasureSpot_ShowOpened(s32 id) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Object *object;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    object = marker->object;
    if (object != NULL)
      ObjectDispatch_SelectAnimationFar(object, TREASURE_SPOT_CHEST_OPEN);
  }
}

/// Returns the object of treasure spot id, or NULL when no slot has it. Called
/// by Field_SearchCell, and by Reveal's marks (field_fx.c) to skip a treasure
/// spot already placed.
struct Object *TreasureSpot_GetObjectEvent(s32 id) {
  s32 slotIndex = TreasureSpot_FindSlot(id);
  struct TreasureSpot *marker;

  if (slotIndex == -1)
    return NULL;
  marker = &gFieldWork->treasureSpots[slotIndex];
  return marker->object;
}

/// Hides the object of treasure spot id by clearing its animationKind:
/// OBJECT_SPRITE_NONE draws no sprite, and the object stays in its slot. Called
/// by Field_SearchCell once a Psynergy Stone is taken.
void TreasureSpot_Hide(s32 id) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Object *object;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    object = marker->object;
    if (object != NULL)
      object->spriteStorageKind = OBJECT_SPRITE_NONE;
  }
}

/// Moves the object of treasure spot id to (x, z) on the ground; -1 keeps the
/// centre of its cell on that axis. Positions are 16.16. Map overlays call it.
void TreasureSpot_SetObjectEventPosition(s32 id, s32 x, s32 z) {
  s32 slotIndex;
  s32 groundHeight;
  s32 objectX;
  s32 objectZ;
  struct TreasureSpot *marker;
  struct Object *object;

  objectZ = z;
  objectX = x;
  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    object = marker->object;
    if (object != NULL) {
      if (objectX == -1)
        objectX = (marker->column << 20) + 0x80000;
      if (objectZ == -1)
        objectZ = (marker->row << 20) + 0x80000;
      object->x = objectX;
      object->z = objectZ;
      groundHeight = MapGrid_GetTerrainHeightFar(0, objectX, objectZ);
      object->groundHeight = groundHeight;
      object->y = groundHeight;
    }
  }
}

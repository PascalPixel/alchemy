/// @file
/// The map's markers: chests and Psynergy stones.
// Part of object.c.

#include "../object_internal.h"

/// Sets an object's y and terrainHeight to the map's height at its x and z.
/// Called by Field_PlaceMapMarkers for each marker it places.
void ObjectMotion_SnapToTerrain(void *object) {
  struct ObjectRuntime *objectEvent = object;
  s32 terrainHeight;

  terrainHeight =
      MapGrid_GetTerrainHeightFar(0, objectEvent->x, objectEvent->z);
  objectEvent->y = terrainHeight;
  objectEvent->terrainHeight = terrainHeight;
}

/// The map grid's work (gMapGridWork[0], HEAP_SLOT_MAP_GRID), seen as its
/// marker list.
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

/// The sprites of the objects a map's markers place.
enum { MARKER_CHEST_SPRITE = 20, MARKER_PSYNERGY_STONE_SPRITE = 28 };

/// The chest sprite's animations.
enum {
  MARKER_CHEST_OPEN = 2,    ///< standing open
  MARKER_CHEST_OPENING = 4, ///< opening
  MARKER_CHEST_CLOSING = 5, ///< closing again on what nobody could carry
  MARKER_CHEST_MIMIC = 7    ///< jumping up as a mimic
};

/// Places an object at each of the map's markers (ids 100 to 239) that the
/// map has an event for, up to ten: a chest for a chest event,
/// shown open once its flag is set and gone once its mimic was beaten, and a
/// Psynergy Stone for a bg event giving one, until it is
/// taken. A marker with no such event takes no slot. The objects are kept in
/// work->markers, not in object event slots. Called by Field_RunMap on map
/// load; clears and fills work->markers.
void Field_PlaceMapMarkers(void) {
  u32 id;
  struct MapGridMarkers *grid = gMapGridWork[0];
#ifdef UBFIX
  struct FieldWork *work = (struct FieldWork *)gEventWork;
#else
  // UB: the event cell is a separate global, beyond gMapGridWork's one pointer.
  // Reading gEventWork plainly grows 0x254 to 0x25c bytes in English.
  struct FieldWork *work = gMapGridWork[HEAP_SLOT_EVENT - HEAP_SLOT_MAP_GRID];
#endif
  s32 markerCount = 0;
  u8 *list = grid->markers;
  struct MarkerSlot *marker = work->markers;
  const struct MapEvent *event;
  struct ScriptObjectRuntime *markerObjectEvent;
  u32 column;
  u32 row;

  Dma_Fill32(0, marker, sizeof(struct MarkerSlot) * MARKER_MAX);
  // A map without a marker list places nothing.
  if (list == NULL)
    return;

  column = *list++;
  row = *list++;
  while (column != 255 || row != 255) {
    id = *list++;
    if (id >= 100 && id <= 239) {
      // The first event with this trigger that places something wins; the
      // event's value says what the search gives (its FIND_ type, as
      // Field_SearchTile reads it).
      event = gOverlayArea.events();
      for (; event->control != -1; event++) {
        if (event->trigger != id)
          continue;
        if ((event->control & 0x1ff) == BG_EVENT(BG_EVENT_CHEST)) {
          // A chest, at the centre of its cell (cells are 16 pixels, and
          // positions 16.16).
          markerObjectEvent = (struct ScriptObjectRuntime *)Object_CreateFar(
              MARKER_CHEST_SPRITE, (column << 20) + 0x80000, 0,
              (row << 20) + 0x80000);
          if (markerObjectEvent == NULL)
            continue;
          ObjectMotion_SnapToTerrain(markerObjectEvent);
          ObjectDispatch_SetSingleChildFlagsFar(
              (struct DispatchObject *)markerObjectEvent, 0);
          // An opened chest shows open; an opened mimic's chest
          // is not placed at all. A mimic's flag is set only after its battle,
          // when the one enemy was defeated (Djinn_ResolvePendingEvent).
          if (GameFlag_TestFar(event->condition)) {
            if ((event->value & 0xfff00000) == 0x500000) {
              ObjectDispatch_ReleaseFar(
                  (struct DispatchObject *)markerObjectEvent);
              continue;
            }
            ObjectDispatch_SelectAnimationFar(markerObjectEvent,
                                              MARKER_CHEST_OPEN);
          }
          ObjectDispatch_StartIdleFar(
              (struct DispatchObject *)markerObjectEvent);
          markerObjectEvent->homeX = markerObjectEvent->x / 0x10000;
          markerObjectEvent->homeZ = markerObjectEvent->z / 0x10000;
          markerObjectEvent->priorityFlags = 1;
          markerObjectEvent->flags59 = 1;
          marker->id = event->trigger;
          marker->object = (struct ObjectRuntime *)markerObjectEvent;
          marker->column = markerObjectEvent->x / 0x100000;
          marker->row = markerObjectEvent->z / 0x100000;
          marker++;
          markerCount++;
          if (markerCount >= MARKER_MAX)
            return;
        } else if ((event->control & 0x1ff) == MAP_EVENT_BG) {
          // A Psynergy Stone not yet taken.
          if ((event->value & 0xfff00000) != FIND_PSYNERGY_STONE)
            continue;
          if (GameFlag_TestFar(event->condition))
            continue;
          markerObjectEvent = (struct ScriptObjectRuntime *)Object_CreateFar(
              MARKER_PSYNERGY_STONE_SPRITE, (column << 20) + 0x80000, 0,
              (row << 20) + 0x80000);
          if (markerObjectEvent == NULL)
            continue;
          ObjectMotion_SnapToTerrain(markerObjectEvent);
          ObjectDispatch_SetSingleChildFlagsFar(
              (struct DispatchObject *)markerObjectEvent, 0);
          ObjectDispatch_StartIdleFar(
              (struct DispatchObject *)markerObjectEvent);
          ObjectDispatch_SelectAnimationFar(markerObjectEvent, 1);
          markerObjectEvent->homeX = markerObjectEvent->x / 0x10000;
          markerObjectEvent->homeZ = markerObjectEvent->z / 0x10000;
          markerObjectEvent->flags59 = 1;
          markerObjectEvent->priorityFlags = 1;
          marker->object = (struct ObjectRuntime *)markerObjectEvent;
          marker->id = event->trigger;
          marker->column = markerObjectEvent->x / 0x100000;
          marker->row = markerObjectEvent->z / 0x100000;
          marker++;
          markerCount++;
          if (markerCount >= MARKER_MAX)
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

/// Returns the marker slot (below MARKER_MAX) whose event trigger is id, or -1.
/// Called by the Marker_ functions below. Unused slots hold id 0, which no
/// marker has (ids are 100 to 239).
s32 Marker_FindSlot(s32 id) {
  s32 slotIndex;
  s32 matchingSlot;
  struct MarkerSlot *marker;

  marker = ((struct FieldWork *)gEventWork)->markers;
  matchingSlot = -1;
  for (slotIndex = 0; slotIndex <= MARKER_MAX - 1; slotIndex++, marker++) {
    if (marker->id == id) {
      matchingSlot = slotIndex;
      break;
    }
  }
  return matchingSlot;
}

/// Unused: an empty function nothing calls.
UNUSED void Marker_ReservedNoOp(void) {}

/// Gives the object of marker id animation, plays sound and waits 12 frames.
static __inline__ void SetMarkerAnimationAndPlaySound(s32 id, s32 animation,
                                                      s32 sound) {
  s32 slotIndex;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slotIndex = Marker_FindSlot(id);
  if (slotIndex != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
    object = marker->object;
    if (object != NULL)
      ObjectDispatch_SelectAnimationFar(object, animation);
    AudioCommand_PlayFar(sound);
    WaitFrames(12);
  }
}

/// Closes the chest of marker id again (its item could not be carried), with
/// SE_125. Called by Field_SearchTile.
void Marker_CloseChest(s32 id) {
  SetMarkerAnimationAndPlaySound(id, MARKER_CHEST_CLOSING, SE_125);
}

/// Makes the chest of marker id jump up as a mimic: after 18 frames it takes
/// MARKER_CHEST_MIMIC, plays SE_146 and gets an upward velocityY of 8 << 16.
/// Called by Field_SearchTile before the mimic's battle.
void Marker_LaunchMimic(s32 id) {
  s32 slotIndex;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slotIndex = Marker_FindSlot(id);
  if (slotIndex != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
    object = marker->object;
    WaitFrames(18);
    if (object != NULL)
      ObjectDispatch_SelectAnimationFar(object, MARKER_CHEST_MIMIC);
    AudioCommand_PlayFar(SE_146);
    if (object != NULL) {
      object->velocityY = 8 << 16;
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 1);
    }
  }
}

/// Opens the chest of marker id with SE_124, and waits 12
/// frames. Called by Field_SearchTile when a chest is searched.
void Marker_OpenChest(s32 id) {
  SetMarkerAnimationAndPlaySound(id, MARKER_CHEST_OPENING, SE_124);
}

/// Shows the chest of marker id open, with no sound. Called by
/// Field_SearchTile once a chest's contents are taken.
void Marker_ShowOpened(s32 id) {
  s32 slotIndex;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slotIndex = Marker_FindSlot(id);
  if (slotIndex != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
    object = marker->object;
    if (object != NULL)
      ObjectDispatch_SelectAnimationFar(object, MARKER_CHEST_OPEN);
  }
}

/// Returns the object of marker id, or NULL when no slot has it. Called by
/// Field_SearchTile, and by the world map markers (field_fx.c) to skip a
/// marker already placed.
struct ObjectRuntime *Marker_GetObjectEvent(s32 id) {
  s32 slotIndex = Marker_FindSlot(id);
  struct MarkerSlot *marker;

  if (slotIndex == -1)
    return NULL;
  marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
  return marker->object;
}

/// Hides the object of marker id by clearing its animationKind:
/// OBJECT_SPRITE_NONE draws no sprite, and the object stays in its slot. Called
/// by Field_SearchTile once a Psynergy Stone is taken.
void Marker_Hide(s32 id) {
  s32 slotIndex;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slotIndex = Marker_FindSlot(id);
  if (slotIndex != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
    object = marker->object;
    if (object != NULL)
      object->animationKind = OBJECT_SPRITE_NONE;
  }
}

/// Moves the object of marker id to (x, z) on the ground; -1 keeps the centre
/// of its cell on that axis. Positions are 16.16. Map overlays call it as
/// Engine_MarkerSetPosition.
void Marker_SetObjectEventPosition(s32 id, s32 x, s32 z) {
  s32 slotIndex;
  s32 terrainHeight;
  s32 objectX;
  s32 objectZ;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  objectZ = z;
  objectX = x;
  slotIndex = Marker_FindSlot(id);
  if (slotIndex != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
    object = marker->object;
    if (object != NULL) {
      if (objectX == -1)
        objectX = (marker->column << 20) + 0x80000;
      if (objectZ == -1)
        objectZ = (marker->row << 20) + 0x80000;
      object->x = objectX;
      object->z = objectZ;
      terrainHeight = MapGrid_GetTerrainHeightFar(0, objectX, objectZ);
      object->terrainHeight = terrainHeight;
      object->y = terrainHeight;
    }
  }
}

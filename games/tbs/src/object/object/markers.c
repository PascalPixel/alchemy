/// @file
/// The map's markers: chests and Psynergy stones.
/// Part of object.c, the field object, compiled only through it.

#include "../object_internal.h"

/// Sets an actor's y and terrainHeight to the map's height at its x and z.
/// Called by Field_PlaceMapMarkers for each marker it places.
void ObjectMotion_SnapToTerrain(void *object) {
  struct ObjectRuntime *actor = object;
  s32 terrainHeight;

  terrainHeight = Map_GetTerrainHeightFar(0, actor->x, actor->z);
  actor->y = terrainHeight;
  actor->terrainHeight = terrainHeight;
}

/// The map's work (gMapWork[0], heap slot 8), seen as its marker list.
struct MarkerMap {
  u8 unk00[0x10];
  /// (column, row, id) triples, ended by 255, 255: a 16-pixel cell and the
  /// trigger of the scene event there; NULL for a map with none.
  u8 *markers;
};

/// Places an object at each of the map's markers (ids 100 to 239) that the
/// scene has an event for, up to ten: a chest (sprite 20) for a chest event,
/// shown open once its flag is set and gone once its mimic was beaten, and a
/// Psynergy stone (sprite 28) for a search event giving one, until it is
/// taken. A marker with no such event takes no slot. The objects are kept in
/// work->markers, not in actor slots. Called by Field_RunScene on scene load;
/// clears and fills work->markers.
void Field_PlaceMapMarkers(void) {
  u32 id;
  struct MarkerMap *map = gMapWork[0];
#ifdef UBFIX
  struct FieldWork *work = (struct FieldWork *)gEventWork;
#else
  // UB: the event cell is a separate global, beyond gMapWork's one pointer.
  // Reading gEventWork plainly grows 0x254 to 0x25c bytes in English.
  struct FieldWork *work = gMapWork[HEAP_SLOT_EVENT - 8];
#endif
  s32 markerCount = 0;
  u8 *list = map->markers;
  struct MarkerSlot *slot = work->markers;
  const struct SceneEvent *event;
  struct ScriptObjectRuntime *markerActor;
  u32 column;
  u32 row;

  Dma_Fill32(0, slot, sizeof(struct MarkerSlot) * MARKER_MAX);
  // A map without a marker list places nothing.
  if (list == NULL)
    return;

  column = *list++;
  row = *list++;
  while (column != 255 || row != 255) {
    id = *list++;
    if (id >= 100 && id <= 239) {
      // The first event with this trigger that places something wins; the
      // event's value says what the search gives (bits 31..20: 0x2 coins,
      // 0x3 a Psynergy stone, 0x5 a mimic, as Field_SearchTile reads them).
      event = gOverlayArea.events();
      for (; event->control != -1; event++) {
        if (event->trigger != id)
          continue;
        // 19 is SEARCH(SEARCH_CHEST).
        if ((event->control & 0x1ff) == 19) {
          // A chest, at the centre of its cell (cells are 16 pixels, and
          // positions 16.16).
          markerActor = (struct ScriptObjectRuntime *)Object_CreateFar(
              20, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
          if (markerActor == NULL)
            continue;
          ObjectMotion_SnapToTerrain(markerActor);
          ObjectDispatch_SetSingleChildFlagsFar(
              (struct DispatchObject *)markerActor, 0);
          // An opened chest shows open (animation 2); an opened mimic's chest
          // is not placed at all. A mimic's flag is set only after its battle,
          // when the one enemy was defeated (Djinn_ResolvePendingEvent).
          if (GameFlag_TestFar(event->condition)) {
            if ((event->value & 0xfff00000) == 0x500000) {
              ObjectDispatch_ReleaseFar((struct DispatchObject *)markerActor);
              continue;
            }
            ObjectDispatch_SelectAnimationFar(markerActor, 2);
          }
          ObjectDispatch_StartIdleFar((struct DispatchObject *)markerActor);
          markerActor->homeX = markerActor->x / 0x10000;
          markerActor->homeZ = markerActor->z / 0x10000;
          markerActor->unk23 = 1;
          markerActor->flags59 = 1;
          slot->id = event->trigger;
          slot->object = (struct ObjectRuntime *)markerActor;
          slot->column = markerActor->x / 0x100000;
          slot->row = markerActor->z / 0x100000;
          slot++;
          markerCount++;
          if (markerCount >= MARKER_MAX)
            return;
        } else if ((event->control & 0x1ff) == EVENT_SEARCH) {
          // A Psynergy stone not yet taken.
          if ((event->value & 0xfff00000) != 0x300000)
            continue;
          if (GameFlag_TestFar(event->condition))
            continue;
          markerActor = (struct ScriptObjectRuntime *)Object_CreateFar(
              28, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
          if (markerActor == NULL)
            continue;
          ObjectMotion_SnapToTerrain(markerActor);
          ObjectDispatch_SetSingleChildFlagsFar(
              (struct DispatchObject *)markerActor, 0);
          ObjectDispatch_StartIdleFar((struct DispatchObject *)markerActor);
          ObjectDispatch_SelectAnimationFar(markerActor, 1);
          markerActor->homeX = markerActor->x / 0x10000;
          markerActor->homeZ = markerActor->z / 0x10000;
          markerActor->flags59 = 1;
          markerActor->unk23 = 1;
          slot->object = (struct ObjectRuntime *)markerActor;
          slot->id = event->trigger;
          slot->column = markerActor->x / 0x100000;
          slot->row = markerActor->z / 0x100000;
          slot++;
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

/// Returns the marker slot (0 to 9) whose event trigger is id, or -1. Called
/// by the Marker_ functions below. Unused slots hold id 0, which no marker
/// has (ids are 100 to 239).
s32 Marker_FindSlot(s32 id) {
  s32 slotIndex;
  s32 matchingSlot;
  struct MarkerSlot *slot;

  slot = ((struct FieldWork *)gEventWork)->markers;
  matchingSlot = -1;
  for (slotIndex = 0; slotIndex <= 9; slotIndex++, slot++) {
    if (slot->id == id) {
      matchingSlot = slotIndex;
      break;
    }
  }
  return matchingSlot;
}

UNUSED void EffectRuntime_ReservedNoOp(void) {}

/// Gives the actor of marker id animation, plays sound and waits 12 frames.
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
  SetMarkerAnimationAndPlaySound(id, 5, SE_125);
}

/// Makes the chest of marker id jump up as a mimic: after 18 frames it takes
/// animation 7, plays SE_146 and gets an upward velocityY of 8 << 16.
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
      ObjectDispatch_SelectAnimationFar(object, 7);
    AudioCommand_PlayFar(SE_146);
    if (object != NULL) {
      object->velocityY = 8 << 16;
      ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 1);
    }
  }
}

/// Opens the chest of marker id (animation 4) with SE_124, and waits 12
/// frames. Called by Field_SearchTile when a chest is searched.
void Marker_OpenChest(s32 id) { SetMarkerAnimationAndPlaySound(id, 4, SE_124); }

/// Shows the chest of marker id open (animation 2), with no sound. Called by
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
      ObjectDispatch_SelectAnimationFar(object, 2);
  }
}

/// Returns the actor of marker id, or NULL when no slot has it. Called by
/// Field_SearchTile, and by the world map markers (field_fx.c) to skip a
/// marker already placed.
struct ObjectRuntime *Marker_GetActor(s32 id) {
  s32 slotIndex = Marker_FindSlot(id);
  struct MarkerSlot *marker;

  if (slotIndex == -1)
    return NULL;
  marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
  return marker->object;
}

/// Hides the actor of marker id by clearing its animationKind: kind 0 draws no
/// sprite, and the object stays in its slot. Called by Field_SearchTile once a
/// Psynergy stone is taken.
void Marker_Hide(s32 id) {
  s32 slotIndex;
  struct MarkerSlot *marker;
  struct ObjectRuntime *object;

  slotIndex = Marker_FindSlot(id);
  if (slotIndex != -1) {
    marker = &((struct FieldWork *)gEventWork)->markers[slotIndex];
    object = marker->object;
    if (object != NULL)
      object->animationKind = 0;
  }
}

/// Moves the actor of marker id to (x, z) on the ground; -1 keeps the centre
/// of its cell on that axis. Positions are 16.16. Scene overlays call it as
/// Engine_MapObjectSetPosition.
void Marker_SetActorPosition(s32 id, s32 x, s32 z) {
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
      terrainHeight = Map_GetTerrainHeightFar(0, objectX, objectZ);
      object->terrainHeight = terrainHeight;
      object->y = terrainHeight;
    }
  }
}

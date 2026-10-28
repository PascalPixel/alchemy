/// @file
/// The map's treasure spots: chests and Psynergy Stones.
// Part of overworld.c.

#include "../overworld_internal.h"

/// Sets an actor's y and groundHeight to the map's height at its x and z.
/// Called by Field_PlaceTreasureSpots for each treasure spot it places.
void Actor_SnapToTerrain(void *object) {
  struct Actor *actor = object;
  s32 groundHeight;

  groundHeight = MapGrid_GetTerrainHeightFar(0, actor->x.fixed, actor->z.fixed);
  actor->y.fixed = groundHeight;
  actor->groundHeight = groundHeight;
}

/// The map grid's work (gMapGridWork[0], HEAP_SLOT_MAP_GRID), seen as its
/// treasure spot list.
struct MapGridMarkers {
  /// MapGridScrollWork's origin and shake; not named through this view.
  ///@{
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  ///@}
  /// MapGridScrollWork's markers, read here as (column, row, id) triples, ended
  /// by 255, 255: a 16-pixel cell and the subject of the trigger there; NULL
  /// for a map with none.
  u8 *markers;
};

/// The sprites of the actors a map's treasure spots place.
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

/// Places an actor at each of the map's treasure spots (ids 100 to 239) that
/// the scene has an event for, up to ten: a chest for a chest event, shown open
/// once its flag is set and gone once its mimic was beaten, and a Psynergy
/// Stone for an examine trigger giving one, until it is taken. A treasure spot
/// with no such event takes no slot. The actors are kept in
/// work->treasureSpots, not in actor slots. Called by SceneSequence_Run on
/// scene load; clears and fills work->treasureSpots.
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
  const struct Trigger *trigger;
  struct Actor *markerActor;
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
      // Field_ExamineCell reads it).
      trigger = gOverlayArea.triggers();
      for (; trigger->kind != -1; trigger++) {
        if (trigger->subject != id)
          continue;
        if ((trigger->kind & 0x1ff) == EXAMINE_TRIGGER(EXAMINE_CHEST)) {
          // A chest, at the centre of its cell (cells are 16 pixels, and
          // positions 16.16).
          markerActor = Actor_CreateFar(TREASURE_SPOT_CHEST_SPRITE,
                                        (column << 20) + 0x80000, 0,
                                        (row << 20) + 0x80000);
          if (markerActor == NULL)
            continue;
          Actor_SnapToTerrain(markerActor);
          Actor_SetSpriteFlagsFar(markerActor, 0);
          // An opened chest shows open; an opened mimic's chest
          // is not placed at all. A mimic's flag is set only after its battle,
          // when the one enemy was defeated (Djinn_ResolvePending).
          if (GameFlag_GetFar(trigger->condition)) {
            if ((trigger->value & 0xfff00000) == 0x500000) {
              Actor_ReleaseFar(markerActor);
              continue;
            }
            Actor_SelectAnimationFar(markerActor, TREASURE_SPOT_CHEST_OPEN);
          }
          Actor_StartIdleFar(markerActor);
          markerActor->homeX = markerActor->x.fixed / Q16_ONE;
          markerActor->homeZ = markerActor->z.fixed / Q16_ONE;
          markerActor->priorityFlags = 1;
          markerActor->collisionFlags = 1;
          marker->id = trigger->subject;
          marker->actor = markerActor;
          // Plain words: through .fixed, the union moves the store above.
          marker->column = *(s32 *)&markerActor->x / 0x100000;
          marker->row = *(s32 *)&markerActor->z / 0x100000;
          marker++;
          markerCount++;
          if (markerCount >= TREASURE_SPOT_MAX)
            return;
        } else if ((trigger->kind & 0x1ff) == TRIGGER_EXAMINE) {
          // A Psynergy Stone not yet taken.
          if ((trigger->value & 0xfff00000) != FIND_PSYNERGY_STONE)
            continue;
          if (GameFlag_GetFar(trigger->condition))
            continue;
          markerActor = Actor_CreateFar(TREASURE_SPOT_PSYNERGY_STONE_SPRITE,
                                        (column << 20) + 0x80000, 0,
                                        (row << 20) + 0x80000);
          if (markerActor == NULL)
            continue;
          Actor_SnapToTerrain(markerActor);
          Actor_SetSpriteFlagsFar(markerActor, 0);
          Actor_StartIdleFar(markerActor);
          Actor_SelectAnimationFar(markerActor, 1);
          markerActor->homeX = markerActor->x.fixed / Q16_ONE;
          markerActor->homeZ = markerActor->z.fixed / Q16_ONE;
          markerActor->collisionFlags = 1;
          markerActor->priorityFlags = 1;
          marker->actor = markerActor;
          marker->id = trigger->subject;
          // Plain words: through .fixed, the union moves the store above.
          marker->column = *(s32 *)&markerActor->x / 0x100000;
          marker->row = *(s32 *)&markerActor->z / 0x100000;
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

UNUSED void TreasureSpot_ReservedNoOp(void) {}

/// Gives the actor of treasure spot id animation, plays sound and waits 12
/// frames.
static __inline__ void SetMarkerAnimationAndPlaySound(s32 id, s32 animation,
                                                      s32 sound) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Actor *actor;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    actor = marker->actor;
    if (actor != NULL)
      Actor_SelectAnimationFar(actor, animation);
    Sound_PlaySongFar(sound);
    WaitFrames(12);
  }
}

/// Closes the chest of treasure spot id again (its item could not be carried),
/// with SE_CHEST_CLOSE. Called by Field_ExamineCell.
void TreasureSpot_CloseChest(s32 id) {
  SetMarkerAnimationAndPlaySound(id, TREASURE_SPOT_CHEST_CLOSING,
                                 SE_CHEST_CLOSE);
}

/// Makes the chest of treasure spot id jump up as a mimic: after 18 frames it
/// takes TREASURE_SPOT_CHEST_MIMIC, plays SE_ENEMY_CRY and gets an upward
/// velocityY of 8 << 16. Called by Field_ExamineCell before the mimic's battle.
void TreasureSpot_LaunchMimic(s32 id) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Actor *actor;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    actor = marker->actor;
    WaitFrames(18);
    if (actor != NULL)
      Actor_SelectAnimationFar(actor, TREASURE_SPOT_CHEST_MIMIC);
    Sound_PlaySongFar(SE_ENEMY_CRY);
    if (actor != NULL) {
      actor->velocityY = 8 << 16;
      Actor_SetSpriteFlagsFar(actor, 1);
    }
  }
}

/// Opens the chest of treasure spot id with SE_CHEST_OPEN, and waits 12
/// frames. Called by Field_ExamineCell when a chest is searched.
void TreasureSpot_OpenChest(s32 id) {
  SetMarkerAnimationAndPlaySound(id, TREASURE_SPOT_CHEST_OPENING,
                                 SE_CHEST_OPEN);
}

/// Shows the chest of treasure spot id open, with no sound. Called by
/// Field_ExamineCell once a chest's contents are taken.
void TreasureSpot_ShowOpened(s32 id) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Actor *actor;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    actor = marker->actor;
    if (actor != NULL)
      Actor_SelectAnimationFar(actor, TREASURE_SPOT_CHEST_OPEN);
  }
}

/// Returns the actor of treasure spot id, or NULL when no slot has it. Called
/// by Field_ExamineCell, and by Reveal's marks (field_fx.c) to skip a treasure
/// spot already placed.
struct Actor *TreasureSpot_GetActor(s32 id) {
  s32 slotIndex = TreasureSpot_FindSlot(id);
  struct TreasureSpot *marker;

  if (slotIndex == -1)
    return NULL;
  marker = &gFieldWork->treasureSpots[slotIndex];
  return marker->actor;
}

/// Hides the actor of treasure spot id by clearing its animationKind:
/// ACTOR_SPRITE_NONE draws no sprite, and the actor stays in its slot. Called
/// by Field_ExamineCell once a Psynergy Stone is taken.
void TreasureSpot_Hide(s32 id) {
  s32 slotIndex;
  struct TreasureSpot *marker;
  struct Actor *actor;

  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    actor = marker->actor;
    if (actor != NULL)
      actor->spriteStorageKind = ACTOR_SPRITE_NONE;
  }
}

/// Moves the actor of treasure spot id to (x, z) on the ground; -1 keeps the
/// centre of its cell on that axis. Positions are 16.16. Scene overlays call
/// it.
void TreasureSpot_SetActorPosition(s32 id, s32 x, s32 z) {
  s32 slotIndex;
  s32 groundHeight;
  s32 objectX;
  s32 objectZ;
  struct TreasureSpot *marker;
  struct Actor *actor;

  objectZ = z;
  objectX = x;
  slotIndex = TreasureSpot_FindSlot(id);
  if (slotIndex != -1) {
    marker = &gFieldWork->treasureSpots[slotIndex];
    actor = marker->actor;
    if (actor != NULL) {
      if (objectX == -1)
        objectX = (marker->column << 20) + 0x80000;
      if (objectZ == -1)
        objectZ = (marker->row << 20) + 0x80000;
      actor->x.fixed = objectX;
      actor->z.fixed = objectZ;
      groundHeight = MapGrid_GetTerrainHeightFar(0, objectX, objectZ);
      actor->groundHeight = groundHeight;
      actor->y.fixed = groundHeight;
    }
  }
}

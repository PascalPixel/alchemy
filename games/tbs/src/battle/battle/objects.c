/// @file
/// Each frame's sprite priorities, the status icons, the poses and the
/// sprites of the battle objects.
/// Part of battle.c, the battle object, compiled only through it.

#include "../battle_internal.h"

#define UNIT_MAX 14 ///< the party's and the enemies' units together

/// The two side walks stay separate: a shared priority helper changes EN
/// register allocation despite the same 0x1ac-byte extent.
/// Runs each frame of a battle: advances every unit's status icon, lifts an
/// airborne unit's effect sprite above the others, and draws whichever side
/// is nearer the camera in front.
void BattleActor_UpdateSpritePriorities(void) {
  u16 unitIds[UNIT_MAX];
  s32 sidePriorities[2]; // the party's, the enemies'
  s32 unitIndex;
  s32 recordIndex;
  s32 count;
  struct MotionObject *object;
  struct AnimationObject *record;
  struct AnimationObject **records;

  // Every unit's status icon.
  BattleParty_ListActorIds(BATTLE_SIDE_BOTH, unitIds);
  unitIndex = 0;
  if (unitIds[unitIndex] != BATTLE_UNIT_LIST_END) {
    for (;;) {
      struct BattleObj *slot = BattleObjGet(unitIds[unitIndex]);

      if (slot != 0) {
        object = slot->object;

        BattleStatusIcon_Cycle(slot);
        if (slot->effectEntry != 0) {
          record = GetMotionRecord(object, 0);

          if (record != 0) {
            struct SpriteEntry *effect;
            s32 effectPriority = 0;

            // Off the ground.
            if (object->y != 0)
              effectPriority = 9;
            effect = slot->effectEntry;
            if (effect->priority != effectPriority) {
              effect->priority = effectPriority;
              record->dirty = 1;
            }
          }
        }
      }
      unitIndex++;
      if (unitIndex >= UNIT_MAX || unitIds[unitIndex] == BATTLE_UNIT_LIST_END)
        break;
    }
  }

  // The camera's turn decides which side is in front.
  if ((s16)gCameraWork->yaw >= 0) {
    sidePriorities[0] = 1;
    sidePriorities[1] = 2;
  } else {
    sidePriorities[0] = 2;
    sidePriorities[1] = 1;
  }

  // The party.
  {
    count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, unitIds);
    for (unitIndex = 0; unitIndex < count; unitIndex++) {
      struct BattleObj *slot = BattleObjGet(unitIds[unitIndex]);

      if (slot != 0) {
        object = slot->object;

        switch (object->recordStorageKind & RECORDS_KIND_MASK) {
        case RECORDS_SINGLE: {
          record = object->records;
          record->image.priority = sidePriorities[0];
          break;
        }
        case RECORDS_TABLE: {
          records = (struct AnimationObject **)object->records;
          for (recordIndex = 0; recordIndex < RECORDS_TABLE_SIZE;
               recordIndex++) {
            record = *records++;
            if (record != 0)
              record->image.priority = sidePriorities[0];
          }
          break;
        }
        }
      }
    }
  }

  // The enemies.
  {
    count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, unitIds);
    for (unitIndex = 0; unitIndex < count; unitIndex++) {
      struct BattleObj *slot = BattleObjGet(unitIds[unitIndex]);

      if (slot != 0) {
        object = slot->object;

        switch (object->recordStorageKind & RECORDS_KIND_MASK) {
        case RECORDS_SINGLE: {
          record = object->records;
          record->image.priority = sidePriorities[1];
          break;
        }
        case RECORDS_TABLE: {
          records = (struct AnimationObject **)object->records;
          for (recordIndex = 0; recordIndex < RECORDS_TABLE_SIZE;
               recordIndex++) {
            record = *records++;
            if (record != 0)
              record->image.priority = sidePriorities[1];
          }
          break;
        }
        }
      }
    }
  }
}

/// The status icons' bits in BattleObj.activeConditions
#define ICON_POISON 0x01
#define ICON_VENOM 0x02 ///< deadly poison
#define ICON_STUN 0x04
#define ICON_PSY_SEAL 0x08
#define ICON_SLEEP 0x10
#define ICON_DELUSION 0x20
#define ICON_HAUNT 0x40
#define ICON_GRIM_REAPER 6 ///< the curse's turns left, 1 to 7, as bits 7 to 13

#define ICON_MAX 14 ///< the kinds of icon

/// Builds the status icon bits of unit id's ailments into slot.
s32 BattleUnit_BuildStatusFlags(s32 unitId, struct BattleObj *battleObject) {
  struct BattleUnit *unit = UnitGetFar(unitId);
  s8 poison = unit->poison;
  u32 flags = 0;

  if (poison == POISON_NORMAL)
    flags = ICON_POISON;
  // Deadly poison: poison is ICON_VENOM.
  if (poison == POISON_DEADLY)
    flags |= poison;
  if (unit->delusion != 0)
    flags |= ICON_DELUSION;
  if (unit->stun != 0) {
    s32 classId;
    flags |= ICON_STUN;
    classId = unit->classId;
    // Two classes never show it.
    if (classId == 121 || classId == 148)
      flags &= ~ICON_STUN;
  }
  if (unit->psySeal != 0)
    flags |= ICON_PSY_SEAL;
  if (unit->haunt != 0)
    flags |= ICON_HAUNT;
  if (unit->sleep != 0)
    flags |= ICON_SLEEP;
  if (unit->grimReaper != 0)
    flags |= 1 << (unit->grimReaper + ICON_GRIM_REAPER);
  battleObject->activeConditions = flags;
}

/// The frames each status icon shows before the next one's turn.
#define ICON_CYCLE_FRAMES 80

/// The icons' first animations, for units 32 pixels wide and for the rest.
#define ICON_ANIM_NARROW 340
#define ICON_ANIM_WIDE 355

/// Shows a unit's status icons one after another, each for
/// ICON_CYCLE_FRAMES frames, switching at once when the shown status ends,
/// and removes the icon when no status is left.
s32 BattleStatusIcon_Cycle(struct BattleObj *battleObject) {
  struct AnimationEntry *shownIcon;
  struct AnimationEntry *icon;
  struct AnimationObject *record;
  struct MotionObject *owner;
  s32 iconAnimation;
  s32 iconHandle;
  s32 previousCondition;
  s32 changed = 0;

  if (battleObject->cycleTimer >= 0)
    battleObject->cycleTimer--;

  // Move on when the time is up, when the shown status has ended, or when a
  // status appears with no icon shown.
  shownIcon = (struct AnimationEntry *)battleObject->animationEntry;
  if ((shownIcon == 0 ? (s16)battleObject->activeConditions != 0
                      : (((s16)battleObject->activeConditions >>
                          battleObject->selectedCondition) &
                         1) == 0) ||
      battleObject->cycleTimer == 0) {
    iconAnimation = -1;
    owner = battleObject->object;
    if ((s16)battleObject->activeConditions != 0) {
      // The next status after the shown one, wrapping around.
      previousCondition = battleObject->selectedCondition;
      for (iconAnimation = previousCondition + 1;; iconAnimation++) {
        if (iconAnimation > ICON_MAX - 1)
          iconAnimation = 0;
        if ((((s16)battleObject->activeConditions >> iconAnimation) & 1) != 0)
          break;
      }

      if (previousCondition != iconAnimation || shownIcon == 0) {
        battleObject->selectedCondition = iconAnimation;
        changed = 1;
      }
      battleObject->cycleTimer = ICON_CYCLE_FRAMES;
    } else {
      changed = 1;
    }

    record = GetMotionRecord(owner, 0);
    if (record != 0) {
      // Units 32 pixels wide take their own set of icon animations.
      if (iconAnimation >= 0) {
        if (record->width == 32)
          iconAnimation += ICON_ANIM_NARROW;
        else
          iconAnimation += ICON_ANIM_WIDE;
      }

      if (battleObject->animationEntry != 0 && changed != 0) {
        ResourceMetadata_UnregisterFar(record,
                                       (s32)battleObject->animationEntry);
        battleObject->animationEntry = 0;
      }

      if (iconAnimation >= 0 && changed != 0) {
        // The handle is stored first and cleared again when registration fails.
        iconHandle = ResourceMetadata_RegisterFar(record, iconAnimation);
        battleObject->animationEntry = (struct SpriteEntry *)iconHandle;
        if (iconHandle == -1)
          battleObject->animationEntry = 0;
        icon = (struct AnimationEntry *)battleObject->animationEntry;
        if (icon != 0) {
          icon->priority = 3;
          Animation_SetWorkEntryFar(icon, 0);
        }
      }

      record->dirty = 1;
      if (iconAnimation >= 0)
        battleObject->animation = iconAnimation;
      else
        battleObject->animation = 0;
    }
  }
}

/// Sets unit id's pose to match its state: held (asleep, stunned or unable
/// to move) or down, else standing; then gives its sprites their palette
/// row, 14 to 17 by unit.
s32 BattlePres_SetActorModeAndAction(s32 unitId) {
  struct BattleUnit *unit;
  s32 pose;

  unit = UnitGetFar(unitId);
  pose = POSE_STANDING;
  if (unit->hp != 0) {
    if (unit->sleep != 0 || unit->stun != 0 || unit->immobile != 0)
      pose = (unit->side != BATTLE_UNIT_ENEMY) * POSE_PARTY_HELD;
  } else {
    // 4 for a party member, 5 for an enemy.
    s32 notEnemy = unit->side ^ pose;
    pose = (u32)(-notEnemy | notEnemy) >> 31;
    pose = POSE_ENEMY_DOWN - pose;
  }

  ObjectDispatch_SelectAnimationFar(BattleObjGet(unitId)->object, pose);
  ObjectDispatch_SetAnimationSpeedFar(BattleObjGet(unitId)->object,
                                      (unitId & 3) + 14);
}

/// Releases every sprite unitId's object holds and leaves it with none.
void ReleaseBattleObjectRecords(s32 unitId) {
  struct BattleObj *battleObject;
  struct MotionObject *object;
  s32 recordIndex;
  struct AnimationObject *record;

  battleObject = BattleObjGet(unitId);
  if (battleObject != NULL) {
    object = battleObject->object;
    if (object != NULL) {
      battleObject->animationEntry = 0;
      battleObject->effectEntry = 0;
      recordIndex = 0;
      while ((record = GetMotionRecord(object, recordIndex)) != NULL) {
        ResourceObject_ReleaseFar(record);
        recordIndex += 1;
      }
      object->recordStorageKind = RECORDS_NONE;
      object->records = NULL;
    }
  }
}

/// The sprite unit 32 pixels wide use for an effect.
#define NARROW_EFFECT_SPRITE 0x1ff

/// Readies the sprites of the units in list: releases those of every place
/// list lacks, then creates each listed unit's sprites (body, weapon, status
/// icon and effect; or the two parts of a two-part enemy) unless it has
/// them, building its status icons and setting its pose. With refresh, sets
/// every listed unit's pose again afterwards.
void BattleActor_SpawnObjectsForList(s16 *unitIds, s32 refreshPoses) {
  s32 unitIndex;
  s32 unitId;
  struct BattleObj *battleObject;
  struct MotionObject *object;
  struct AnimationObject *record;
  struct SpriteEntry *entry;
  struct AnimationObject **records;
  s32 spriteId;
  s32 releasedUnitId;

  // Release the sprites of the places no listed unit holds.
  for (unitIndex = 0; unitIndex < UNIT_MAX; unitIndex++) {
    if (!BattlePlacement_ContainsId(unitIds, unitIndex)) {
      releasedUnitId = unitIndex + FIRST_ENEMY_UNIT - 8;
      if (unitIndex <= 7)
        releasedUnitId = unitIndex;
      ReleaseBattleObjectRecords(releasedUnitId);
    }
  }
  if (gSchedulerStatus == 0)
    WaitFrames(1);

  for (unitIndex = 0; unitIndex < UNIT_MAX &&
                      (unitId = unitIds[unitIndex]) != BATTLE_UNIT_LIST_END;
       unitIndex++) {
    if (unitId == BATTLE_UNIT_REMOVED)
      continue;
    battleObject = BattleObjGet(unitId);
    if (battleObject == 0)
      continue;
    BattleUnit_BuildStatusFlags(unitId, battleObject);
    object = battleObject->object;
    if (object == 0)
      continue;
    if (object->recordStorageKind != RECORDS_NONE)
      continue;
    if ((battleObject->resource & SPRITE_ID_MASK) ==
            SPRITE_WITH_SECOND_PART_A ||
        (battleObject->resource & SPRITE_ID_MASK) ==
            SPRITE_WITH_SECOND_PART_B) {
      // A two-part enemy: a table of its two sprites, kept in the menu
      // control work.
      records = (struct AnimationObject **)gMenuCtrlWork->unk08 +
                gMenuCtrlWork->count;
      spriteId = battleObject->resource;
      object->recordStorageKind = RECORDS_TABLE;
      object->records = records;
      Iwram_ClearWords(records, 16);
      record = ResourceObject_CreateFar(spriteId);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        object->height = Resource_GetMetadataRecordFar(spriteId)->boxY >> 1;
        *records = record;
        records++;
      }
      record->flags = 0;
      record = ResourceObject_CreateFar(spriteId + SPRITE_SECOND_PART);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        *records = record;
      }
      record->flags = 0;
    } else {
      // The body, then the weapon, the status icon and the effect on it.
      record = ResourceObject_CreateFar(battleObject->resource);
      if (record != 0) {
        object->recordStorageKind = RECORDS_SINGLE;
        object->records = record;
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        entry = (struct SpriteEntry *)record->entries[0];
        entry->priority = 1;
        entry->palette = battleObject->palette;
        if ((spriteId = battleObject->overlay) != 0) {
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(record,
                                                                     spriteId);
          entry->priority = 1;
        }
        if ((spriteId = battleObject->animation) != 0) {
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(record,
                                                                     spriteId);
          battleObject->animationEntry = entry;
          Animation_SetWorkEntryFar((struct AnimationEntry *)entry, 0);
          entry->priority = 3;
        }
        if ((spriteId = battleObject->effect) != 0) {
          if (record->width == 32)
            spriteId = NARROW_EFFECT_SPRITE;
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(record,
                                                                     spriteId);
          battleObject->effectEntry = entry;
          entry->priority = 0;
          record->flags = 0;
        }
      }
    }
    BattlePres_SetActorModeAndAction(unitId);
  }

  if (refreshPoses) {
    unitIndex = 0;
    if ((spriteId = unitIds[0]) != BATTLE_UNIT_LIST_END) {
      for (;;) {
        if (unitIds[unitIndex] != BATTLE_UNIT_REMOVED &&
            (battleObject = BattleObjGet(spriteId)) != 0 &&
            (object = battleObject->object) != 0)
          BattlePres_SetActorModeAndAction(spriteId);
        unitIndex++;
        if (unitIndex >= UNIT_MAX ||
            (spriteId = unitIds[unitIndex]) == BATTLE_UNIT_LIST_END)
          break;
      }
    }
  }
}

/// Returns unitId's battle object, through its place, or null for a
/// unit with no place.
struct BattleObj *BattleObjGet(s32 unitId) {
  struct BattleWork *battle = gBattleWork;
  struct BattleObj *objects = battle->slots;
  s32 placementIndex;
  if (unitId > 7) {
    unitId -= FIRST_ENEMY_UNIT - 8;
  }
  placementIndex = unitId;
  if (battle->placement[placementIndex] == 0xFF) {
    return NULL;
  }
  return &objects[battle->placement[placementIndex]];
}

/// Detaches the scripts from the four sprite entries of one sprite.
void ResetMotionRecordGroup(void *recordGroup) {
  struct AnimationObject *record = recordGroup;
  s32 entryIndex;

  if (record != NULL) {
    for (entryIndex = 0; entryIndex < 4; entryIndex++) {
      if (record->entries[entryIndex] != NULL)
        record->entries[entryIndex]->script = NULL;
    }
  }
}

/// Detaches the scripts from every sprite of a battle object.
void ResetBattleObjectRecordGroups(struct MotionObject *object) {
  s32 storageKind;
  s32 remaining;
  void **recordGroups;
  void *recordGroup;

  if (object != NULL) {
    storageKind = object->recordStorageKind & RECORDS_KIND_MASK;
    switch (storageKind) {
    case RECORDS_SINGLE:
      ResetMotionRecordGroup(object->records);
      return;
    case RECORDS_TABLE:
      recordGroups = object->records;
      remaining = RECORDS_TABLE_SIZE - 1;
      do {
        recordGroup = *recordGroups++;
        if (recordGroup != NULL) {
          ResetMotionRecordGroup(recordGroup);
        }
        remaining -= 1;
      } while (remaining >= 0);
      break;
    }
  }
}

/// Frees the enemy sprite slots unitId holds and marks its object
/// for release at the end of the round (BattleMotion_DestroyAllSlotObjects).
s32 BattleObj_MarkForRelease(s32 unitId) {
  struct BattleObj *battleObject;
  BattleMotion_ReleaseObjectSlotByValue(unitId);
  battleObject = BattleObjGet(unitId);
  battleObject->active = 1;
  return 0;
}

/// Releases the object of every unit marked by BattleObj_MarkForRelease.
/// Declared to return a pointer, but it returns nothing.
void *BattleMotion_DestroyAllSlotObjects(void) {
  s32 unitId;
  s32 placementIndex;
  struct BattleObj *battleObject;

  placementIndex = 0;
  do {
    unitId = placementIndex + FIRST_ENEMY_UNIT - 8;
    if (placementIndex <= 7) {
      unitId = placementIndex;
    }
    battleObject = BattleObjGet(unitId);
    if ((battleObject != NULL) && (battleObject->active != 0)) {
      ObjectDispatch_ReleaseFar((s32)battleObject->object);
      battleObject->object = NULL;
      battleObject->active = 0;
    }
    placementIndex += 1;
  } while (placementIndex < UNIT_MAX);
}

/// Writes the battlefield spot of place (its anchor, on the ground) to
/// out.
s32 Battle_GetWorkEntryPair(s32 place, struct FixedPointPosition *position) {
  struct BattleWork *battle = gBattleWork;

  SET_POSITION(position, battle->slots[place].anchorX, 0,
               battle->slots[place].anchorZ);
  return 0;
}

/// Sets up the camera's view for projecting points; under
/// FLAG_CHOOSING_COMMANDS, through the extra transform
/// gCamera_FlagTransformWork first.
s32 Camera_ApplyTransformByFlag(void) {
  struct BattleCamera *camera = gCameraWork;
  Render_ResetTransformState();
  if (GameFlag_TestFar(FLAG_CHOOSING_COMMANDS) != 0) {
    Iwram_TransformMatrix((s32 *)gCamera_FlagTransformWork);
    Transform_ApplyLookAt(camera->eye, camera->pos);
  } else {
    Transform_SetLookAt(camera->eye, camera->pos);
  }
}

/// Projects unit id's position onto the screen, into projected.
s32 BattleMotion_ProjectPosition(s32 unitId, s32 *projected) {
  struct MotionObject *object = BattleObjGet(unitId)->object;
  struct AnimationObject *record = GetMotionRecord(object, 0);
  s32 position[3];
  s32 projectionScale;

  Camera_ApplyTransformByFlag();
  SET_VECTOR(position, object->x, object->y, object->z);
  projectionScale = Render_ProjectPoint(position, projected);
  (void)Iwram_MulQ16(projectionScale, record->scale);
  return 0;
}

/// Returns sprite recordIndex of an object (0 only, for a single sprite), or
/// null when it has no such sprite.
void *GetMotionRecord(struct MotionObject *object, s32 recordIndex) {
  s32 storageKind = object->recordStorageKind & RECORDS_KIND_MASK;
  if (storageKind == RECORDS_SINGLE) {
    if (recordIndex == 0) {
      return object->records;
    }
  } else if (storageKind == RECORDS_TABLE) {
    return ((void **)object->records)[recordIndex];
  }
  return NULL;
}

/// Puts the battle camera at its default view of the battlefield's centre.
void Camera_InitDefaultTransform(void) {
  struct BattleCamera *camera = gCameraWork;
  struct FixedPointPosition eyeOffset;

  camera->yaw = 192 << 6;
  camera->pitch = 254 << 8;
  camera->distance = 255 << 17;
  SET_VECTOR(camera->pos, 0, 0, 0);
  camera->followPos = 0;
  camera->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&camera->pos[0]);
  SceneTransform_ApplyYaw((s16)camera->yaw);
  SceneTransform_ApplyPitch((s16)camera->pitch);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
}

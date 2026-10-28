/// @file
/// Each frame's sprite priorities, the status icons, the poses and the
/// sprites of the battle objects.
// Part of battle.c.

#include "../battle_internal.h"

#define UNIT_MAX 14 ///< the party's and the enemies' units together

/// Runs each frame of a battle: advances every unit's status icon, gives an
/// airborne unit's shadow (BATTLE_SHADOW_SPRITE, placement.c) priority
/// 9 and a grounded one's 0, and draws whichever side is nearer the camera in
/// front. Runs as a task (priority TASK_PASS_FRAME)
/// that Battle_Run adds after the entry transition and removes as
/// the battle ends. Writes each battle object's icon state and its sprites'
/// priorities.
void BattleObject_UpdateSpritePriorities(void) {
  // The two side walks stay separate: a shared priority helper changes EN
  // register allocation despite the same 0x1ac-byte extent.
  u16 unitIds[UNIT_MAX];
  // The party's, then the enemies': sprite priority 1 for the side in front,
  // 2 behind (the lower number draws over the higher).
  s32 sidePriorities[2];
  s32 unitIndex;
  s32 recordIndex;
  s32 count;
  struct Actor *actor;
  struct Sprite *record;
  struct Sprite **records;

  // Every unit's status icon.
  BattleUnit_ListIds(BATTLE_SIDE_MASK_BOTH, unitIds);
  unitIndex = 0;
  if (unitIds[unitIndex] != BATTLE_UNIT_LIST_END) {
    for (;;) {
      struct BattleObject *slot = BattleObject_Get(unitIds[unitIndex]);

      if (slot != 0) {
        actor = slot->actor;

        BattleStatusIcon_Cycle(slot);
        if (slot->shadowEntry != 0) {
          record = GetMotionRecord(actor, 0);

          if (record != 0) {
            struct SpriteAnimationHeadView *shadow;
            s32 shadowPriority = 0;

            // Off the ground.
            if (actor->y.fixed != 0)
              shadowPriority = 9;
            shadow = slot->shadowEntry;
            if (shadow->priority != shadowPriority) {
              shadow->priority = shadowPriority;
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

  // The camera's turn decides which side is in front: with the yaw in the
  // first half turn (as a signed halfword), the party.
  if ((s16)gBattleCamera->yaw >= 0) {
    sidePriorities[0] = 1;
    sidePriorities[1] = 2;
  } else {
    sidePriorities[0] = 2;
    sidePriorities[1] = 1;
  }

  // The party.
  {
    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_PARTY, unitIds);
    for (unitIndex = 0; unitIndex < count; unitIndex++) {
      struct BattleObject *slot = BattleObject_Get(unitIds[unitIndex]);

      if (slot != 0) {
        actor = slot->actor;

        switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
        case ACTOR_SPRITE_ONE: {
          record = actor->sprite;
          record->image.priority = sidePriorities[0];
          break;
        }
        case ACTOR_SPRITE_GROUP: {
          records = (struct Sprite **)actor->sprite;
          for (recordIndex = 0; recordIndex < ACTOR_SPRITE_GROUP_SIZE;
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
    count = BattleUnit_ListIds(BATTLE_SIDE_MASK_ENEMIES, unitIds);
    for (unitIndex = 0; unitIndex < count; unitIndex++) {
      struct BattleObject *slot = BattleObject_Get(unitIds[unitIndex]);

      if (slot != 0) {
        actor = slot->actor;

        switch (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) {
        case ACTOR_SPRITE_ONE: {
          record = actor->sprite;
          record->image.priority = sidePriorities[1];
          break;
        }
        case ACTOR_SPRITE_GROUP: {
          records = (struct Sprite **)actor->sprite;
          for (recordIndex = 0; recordIndex < ACTOR_SPRITE_GROUP_SIZE;
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

// The status icons' bits in BattleObject.activeStatuses
#define ICON_POISON 0x01
#define ICON_VENOM 0x02 ///< deadly poison
#define ICON_STUN 0x04
#define ICON_PSY_SEAL 0x08
#define ICON_SLEEP 0x10
#define ICON_DELUSION 0x20
#define ICON_HAUNT 0x40
#define ICON_GRIM_REAPER 6 ///< the curse's turns left, 1 to 7, as bits 7 to 13

#define ICON_MAX 14 ///< the kinds of icon

/// Builds the status icon bits of unit id's statuses into its battle
/// object's activeStatuses, which BattleStatusIcon_Cycle shows. Called
/// wherever a unit's statuses may have changed: BattlePlayback_RunAll,
/// BattlePlayback_Run, Battle_ProcessRoundEnd,
/// BattleObject_ResetRuntimeFields and BattleObject_SpawnObjectsForList.
#ifdef UBFIX
void BattleUnit_BuildStatusFlags(s32 unitId,
                                 struct BattleObject *battleObject) {
#else
s32 BattleUnit_BuildStatusFlags(s32 unitId, struct BattleObject *battleObject) {
#endif
  struct Unit *unit = Unit_GetFar(unitId);
  s8 poison = unit->poison;
  u32 flags = 0;

  if (poison == POISON_NORMAL)
    flags = ICON_POISON;
  // Deadly poison: poison is ICON_VENOM.
  if (poison == POISON_VENOM)
    flags |= poison;
  if (unit->delusion != 0)
    flags |= ICON_DELUSION;
  if (unit->stun != 0) {
    s32 classId;
    flags |= ICON_STUN;
    classId = unit->id;
    // BattlePlacement_LayoutEnemies also tests these two classes.
    if (classId == ENEMY_KRAKEN || classId == ENEMY_FUSION_DRAGON)
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
  battleObject->activeStatuses = flags;
}

/// The frames each status icon shows before the next one's turn.
#define ICON_CYCLE_FRAMES 80

/// The icons' first animations, for units 32 pixels wide and for the rest:
/// each adds the icon's bit number (ICON_POISON's 0 up), so sprites 340 to
/// 353 and 355 to 368.
#define ICON_ANIMATION_NARROW 340
#define ICON_ANIMATION_WIDE 355

/// Shows a unit's status icons one after another, each for
/// ICON_CYCLE_FRAMES frames, switching at once when the shown status ends,
/// and removes the icon when no status is left. Called each frame for every
/// unit by BattleObject_UpdateSpritePriorities. Changes the battle object's
/// cycleTimer, shownStatus, animation and animationEntry, and
/// registers or unregisters the icon on its first sprite.
#ifdef UBFIX
void BattleStatusIcon_Cycle(struct BattleObject *battleObject) {
#else
s32 BattleStatusIcon_Cycle(struct BattleObject *battleObject) {
#endif
  struct SpriteAnimation *shownIcon;
  struct SpriteAnimation *icon;
  struct Sprite *record;
  struct Actor *actor;
  s32 iconAnimation;
  s32 iconHandle;
  s32 previousCondition;
  s32 changed = 0;

  if (battleObject->cycleTimer >= 0)
    battleObject->cycleTimer--;

  // Move on when the time is up, when the shown status has ended, or when a
  // status appears with no icon shown.
  shownIcon = (struct SpriteAnimation *)battleObject->animationEntry;
  if ((shownIcon == 0
           ? (s16)battleObject->activeStatuses != 0
           : (((s16)battleObject->activeStatuses >> battleObject->shownStatus) &
              1) == 0) ||
      battleObject->cycleTimer == 0) {
    iconAnimation = -1;
    actor = battleObject->actor;
    if ((s16)battleObject->activeStatuses != 0) {
      // The next status after the shown one, wrapping around.
      previousCondition = battleObject->shownStatus;
      for (iconAnimation = previousCondition + 1;; iconAnimation++) {
        if (iconAnimation > ICON_MAX - 1)
          iconAnimation = 0;
        if ((((s16)battleObject->activeStatuses >> iconAnimation) & 1) != 0)
          break;
      }

      if (previousCondition != iconAnimation || shownIcon == 0) {
        battleObject->shownStatus = iconAnimation;
        changed = 1;
      }
      battleObject->cycleTimer = ICON_CYCLE_FRAMES;
    } else {
      changed = 1;
    }

    // Swap the icon on the unit's first sprite; iconAnimation is -1 when no
    // status is left.
    record = GetMotionRecord(actor, 0);
    if (record != 0) {
      // Units 32 pixels wide take their own set of icon animations.
      if (iconAnimation >= 0) {
        if (record->width == 32)
          iconAnimation += ICON_ANIMATION_NARROW;
        else
          iconAnimation += ICON_ANIMATION_WIDE;
      }

      if (battleObject->animationEntry != 0 && changed != 0) {
        Sprite_RemoveAnimationFar(record, (s32)battleObject->animationEntry);
        battleObject->animationEntry = 0;
      }

      if (iconAnimation >= 0 && changed != 0) {
        // The handle is stored first and cleared again when registration fails.
        iconHandle = Sprite_AddAnimationFar(record, iconAnimation);
        battleObject->animationEntry =
            (struct SpriteAnimationHeadView *)iconHandle;
        if (iconHandle == -1)
          battleObject->animationEntry = 0;
        icon = (struct SpriteAnimation *)battleObject->animationEntry;
        if (icon != 0) {
          icon->priority = 3;
          SpriteAnimation_StartFar(icon, 0);
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

/// Sets unit id's pose to match its state: held (asleep, stunned or unable to
/// move) or down, else standing; then its sprites' animation speed, a timer
/// step of 14 to 17 by the unit id's low two bits (16 is normal speed), so
/// units side by side, even two of one kind, sway out of step.
/// Called by BattleObject_SpawnObjectsForList, BattlePlayback_RunAll,
/// BattlePlayback_Run and Battle_ProcessRoundEnd.
#ifdef UBFIX
void BattleObject_UpdatePose(s32 unitId) {
#else
s32 BattleObject_UpdatePose(s32 unitId) {
#endif
  struct Unit *unit;
  s32 pose;

  unit = Unit_GetFar(unitId);
  pose = POSE_STANDING;
  if (unit->hp != 0) {
    if (unit->sleep != 0 || unit->stun != 0 || unit->immobile != 0)
      pose = (unit->side != BATTLE_SIDE_ENEMY) * POSE_PARTY_HELD;
  } else {
    // 4 for a party member, 5 for an enemy: pose is POSE_STANDING (1, the
    // enemy side's value) here, so the xor is 0 only for an enemy.
    s32 notEnemy = unit->side ^ pose;
    pose = (u32)(-notEnemy | notEnemy) >> 31;
    pose = POSE_ENEMY_DOWN - pose;
  }

  Actor_SelectAnimationFar(BattleObject_Get(unitId)->actor, pose);
  Actor_SetAnimationSpeedFar(BattleObject_Get(unitId)->actor,
                             (unitId & 3) + 14);
}

/// Releases every sprite unitId's actor holds and leaves it with none,
/// clearing the battle object's animationEntry and shadowEntry. Called by
/// BattleObject_SpawnObjectsForList; BATTLE.S also gives it an unnamed
/// overlay veneer, which nothing in the source calls.
void ReleaseBattleObjectRecords(s32 unitId) {
  struct BattleObject *battleObject;
  struct Actor *actor;
  s32 recordIndex;
  struct Sprite *record;

  battleObject = BattleObject_Get(unitId);
  if (battleObject != NULL) {
    actor = battleObject->actor;
    if (actor != NULL) {
      battleObject->animationEntry = 0;
      battleObject->shadowEntry = 0;
      recordIndex = 0;
      while ((record = GetMotionRecord(actor, recordIndex)) != NULL) {
        Sprite_DestroyFar(record);
        recordIndex += 1;
      }
      actor->spriteStorageKind = ACTOR_SPRITE_NONE;
      actor->sprite = NULL;
    }
  }
}

/// The shadow a body 32 pixels wide takes instead of
/// BattleObject.shadow's (see BATTLE_SHADOW_SPRITE, placement.c).
#define NARROW_SHADOW_SPRITE 0x1ff

/// Readies the sprites of the units in list: releases those of every place
/// list lacks, then creates each listed unit's sprites (body, weapon, status
/// icon and shadow; or the two parts of a two-part enemy) unless it has
/// them, building its status icons and setting its pose. With refreshPoses,
/// sets every listed unit's pose again afterwards. Called by BattleObject_Put
/// (refreshing) and BattleUnit_ListAllAndSubmit, by the staging and
/// entry transition code, and through its far stub by
/// BattleAnim_SelectLivingTargets. Writes the actors' sprites, their storage
/// kind and height, and the battle objects' icon and shadow entries.
void BattleObject_SpawnObjectsForList(s16 *unitIds, s32 refreshPoses) {
  s32 unitIndex;
  s32 unitId;
  struct BattleObject *battleObject;
  struct BattleActor *actor;
  struct Sprite *record;
  struct SpriteAnimationHeadView *entry;
  struct Sprite **records;
  s32 spriteId;
  s32 releasedUnitId;

  // Release the sprites of the places no listed unit holds.
  for (unitIndex = 0; unitIndex < UNIT_MAX; unitIndex++) {
    if (!BattlePlacement_ContainsId(unitIds, unitIndex)) {
      releasedUnitId = unitIndex + FIRST_ENEMY_UNIT - 8;
      if (unitIndex <= ALLY_MAX)
        releasedUnitId = unitIndex;
      ReleaseBattleObjectRecords(releasedUnitId);
    }
  }
  // A frame passes between releasing and creating sprites, except inside a
  // frame-pass task (gTasksRunning is set while WaitFrames runs them),
  // which cannot wait.
  if (gTasksRunning == 0)
    WaitFrames(1);

  for (unitIndex = 0; unitIndex < UNIT_MAX &&
                      (unitId = unitIds[unitIndex]) != BATTLE_UNIT_LIST_END;
       unitIndex++) {
    if (unitId == BATTLE_UNIT_REMOVED)
      continue;
    battleObject = BattleObject_Get(unitId);
    if (battleObject == 0)
      continue;
    BattleUnit_BuildStatusFlags(unitId, battleObject);
    actor = (struct BattleActor *)battleObject->actor;
    if (actor == 0)
      continue;
    if (actor->spriteStorageKind != ACTOR_SPRITE_NONE)
      continue;
    if ((battleObject->resource & SPRITE_ID_MASK) ==
            SPRITE_WITH_SECOND_PART_A ||
        (battleObject->resource & SPRITE_ID_MASK) ==
            SPRITE_WITH_SECOND_PART_B) {
      // A two-part enemy: a table of its two sprites, in the actor system's
      // next sprite group table (gActorSystemWork is struct ActorSystemWork;
      // its count is not advanced here). Its height is half the first
      // part's box.
      // BUG: a failed create leaves record null, and its flags are still
      // written.
      records = (struct Sprite **)gActorSystemWork->spriteGroups +
                gActorSystemWork->count;
      spriteId = battleObject->resource;
      actor->spriteStorageKind = ACTOR_SPRITE_GROUP;
      actor->sprite = records;
      Iwram_ClearWords(records, 16);
      record = Sprite_CreateFar(spriteId);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        actor->radius = Resource_GetMetadataRecordFar(spriteId)->boxY >> 1;
        *records = record;
        records++;
      }
      record->flags = 0;
      record = Sprite_CreateFar(spriteId + SPRITE_SECOND_PART);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        *records = record;
      }
      record->flags = 0;
    } else {
      // The body, then the weapon, the status icon and the shadow.
      // Entry priorities: 0 the shadow, 1 the body and weapon, 3 the icon.
      // Adding the shadow layer clears the sprite's flags, its own shadow
      // (bit 0) among them, as the two-part enemies' are cleared above.
      record = Sprite_CreateFar(battleObject->resource);
      if (record != 0) {
        actor->spriteStorageKind = ACTOR_SPRITE_ONE;
        actor->sprite = record;
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        entry = (struct SpriteAnimationHeadView *)record->entries[0];
        entry->priority = 1;
        entry->palette = battleObject->palette;
        if ((spriteId = battleObject->overlay) != 0) {
          entry = (struct SpriteAnimationHeadView *)Sprite_AddAnimationFar(
              record, spriteId);
          entry->priority = 1;
        }
        if ((spriteId = battleObject->animation) != 0) {
          entry = (struct SpriteAnimationHeadView *)Sprite_AddAnimationFar(
              record, spriteId);
          battleObject->animationEntry = entry;
          SpriteAnimation_StartFar((struct SpriteAnimation *)entry, 0);
          entry->priority = 3;
        }
        if ((spriteId = battleObject->shadow) != 0) {
          if (record->width == 32)
            spriteId = NARROW_SHADOW_SPRITE;
          entry = (struct SpriteAnimationHeadView *)Sprite_AddAnimationFar(
              record, spriteId);
          battleObject->shadowEntry = entry;
          entry->priority = 0;
          record->flags = 0;
        }
      }
    }
    BattleObject_UpdatePose(unitId);
  }

  if (refreshPoses) {
    unitIndex = 0;
    if ((spriteId = unitIds[0]) != BATTLE_UNIT_LIST_END) {
      for (;;) {
        if (unitIds[unitIndex] != BATTLE_UNIT_REMOVED &&
            (battleObject = BattleObject_Get(spriteId)) != 0 &&
            (actor = (struct BattleActor *)battleObject->actor) != 0)
          BattleObject_UpdatePose(spriteId);
        unitIndex++;
        if (unitIndex >= UNIT_MAX ||
            (spriteId = unitIds[unitIndex]) == BATTLE_UNIT_LIST_END)
          break;
      }
    }
  }
}

/// Returns unitId's battle object, through its place, or null for a
/// unit with no place. Called throughout the battle parts.
struct BattleObject *BattleObject_Get(s32 unitId) {
  struct BattleWork *battle = gBattleWork;
  struct BattleObject *battleObjects = battle->objects;
  s32 placementIndex;
  // Enemy unit ids 128 to 133 index placement 8 to 13.
  if (unitId > ALLY_MAX) {
    unitId -= FIRST_ENEMY_UNIT - 8;
  }
  placementIndex = unitId;
  if (battle->objectIndex[placementIndex] == 0xFF) {
    return NULL;
  }
  return &battleObjects[battle->objectIndex[placementIndex]];
}

/// Detaches the scripts from the four sprite entries of one sprite. Called
/// only by ResetBattleObjectRecordGroups.
void ResetMotionRecordGroup(void *recordGroup) {
  struct Sprite *record = recordGroup;
  s32 entryIndex;

  if (record != NULL) {
    for (entryIndex = 0; entryIndex < 4; entryIndex++) {
      if (record->entries[entryIndex] != NULL)
        record->entries[entryIndex]->script = NULL;
    }
  }
}

/// Detaches the scripts from every sprite of a unit's actor. Nothing in the
/// source calls it.
void ResetBattleObjectRecordGroups(struct Actor *actor) {
  s32 storageKind;
  s32 remaining;
  void **recordGroups;
  void *recordGroup;

  if (actor != NULL) {
    storageKind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
    switch (storageKind) {
    case ACTOR_SPRITE_ONE:
      ResetMotionRecordGroup(actor->sprite);
      return;
    case ACTOR_SPRITE_GROUP:
      recordGroups = (void **)actor->sprite;
      remaining = ACTOR_SPRITE_GROUP_SIZE - 1;
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

/// Frees the enemy sprite slots unitId holds and marks its actor
/// for release at the end of the action or round
/// (BattleObject_DestroyAll), setting its active field to 1.
/// Returns 0. Called when a unit leaves the battle: by
/// BattlePlayback_Run, BattleStaging_RunFlee, BattleMotion_ShowDowned,
/// BattleObject_DestroyTemporaryObject and the auto battle.
s32 BattleObject_MarkForRelease(s32 unitId) {
  struct BattleObject *battleObject;
  BattleObject_ReleaseSlotByValue(unitId);
  battleObject = BattleObject_Get(unitId);
  battleObject->active = 1;
  return 0;
}

/// Releases the actor of every unit marked by BattleObject_MarkForRelease,
/// leaving its battle object with no actor. Called by Battle_Run
/// at the end of each round and by BattleStaging_DispatchAction.
#ifdef UBFIX
void BattleObject_DestroyAll(void) {
#else
void *BattleObject_DestroyAll(void) {
#endif
  s32 unitId;
  s32 placementIndex;
  struct BattleObject *battleObject;

  placementIndex = 0;
  do {
    unitId = placementIndex + FIRST_ENEMY_UNIT - 8;
    if (placementIndex <= ALLY_MAX) {
      unitId = placementIndex;
    }
    battleObject = BattleObject_Get(unitId);
    if ((battleObject != NULL) && (battleObject->active != 0)) {
      Actor_ReleaseFar(battleObject->actor);
      battleObject->actor = NULL;
      battleObject->active = 0;
    }
    placementIndex += 1;
  } while (placementIndex < UNIT_MAX);
}

/// Writes the battlefield spot of place (its anchor, on the ground) to
/// position, and returns 0. Nothing in the source calls it.
s32 Battle_GetWorkEntryPair(s32 place, struct FixedPointPosition *position) {
  struct BattleWork *battle = gBattleWork;

  SET_POSITION(position, battle->objects[place].anchorX, 0,
               battle->objects[place].anchorZ);
  return 0;
}

/// Sets up the camera's view for projecting points; under
/// FLAG_CHOOSING_COMMANDS, through the extra transform
/// gBattleCamera_CommandMatrix first. Called by the three projections,
/// BattleMotion_ProjectPosition, BattleMotion_ProjectScaledPosition and
/// BattleMotion_ProjectConditionalPosition. Replaces the transform state.
#ifdef UBFIX
void BattleCamera_ApplyTransform(void) {
#else
s32 BattleCamera_ApplyTransform(void) {
#endif
  struct BattleCamera *camera = gBattleCamera;
  Transform_Reset();
  if (GameFlag_GetFar(FLAG_CHOOSING_COMMANDS) != 0) {
    Iwram_TransformMatrix((s32 *)gBattleCamera_CommandMatrix);
    Transform_ApplyLookAt(camera->eye, camera->focus);
  } else {
    Transform_SetLookAt(camera->eye, camera->focus);
  }
}

/// Projects unit id's position onto the screen, into projected (its pixel x
/// and y, then its depth; Transform_ProjectPoint). Returns 0. Reached through
/// its far stub from Projection_Unit.
s32 BattleMotion_ProjectPosition(s32 unitId, s32 *projected) {
  struct Actor *actor = BattleObject_Get(unitId)->actor;
  struct Sprite *record = GetMotionRecord(actor, 0);
  s32 position[3];
  s32 projectionScale;

  BattleCamera_ApplyTransform();
  SET_VECTOR(position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  projectionScale = Transform_ProjectPoint(position, projected);
  // The sprite's projected scale is worked out and thrown away; compare
  // BattleMotion_ProjectScaledPosition, which keeps it.
  (void)Iwram_MulQ16(projectionScale, record->scale);
  return 0;
}

/// Returns sprite recordIndex of an actor (0 only, for a single sprite), or
/// null when it has no such sprite. Called throughout the battle parts.
void *GetMotionRecord(struct Actor *actor, s32 recordIndex) {
  s32 storageKind = actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK;
  if (storageKind == ACTOR_SPRITE_ONE) {
    if (recordIndex == 0) {
      return actor->sprite;
    }
  } else if (storageKind == ACTOR_SPRITE_GROUP) {
    return ((void **)actor->sprite)[recordIndex];
  }
  return NULL;
}

/// Puts the battle camera at its default view of the battlefield's centre.
/// Called by Battle_Run as the battle is set up and by
/// Battle_GatherActions. Writes the camera's yaw, pitch,
/// distance, position and eye, and the transform state.
void BattleCamera_SetDefaultView(void) {
  struct BattleCamera *camera = gBattleCamera;
  struct FixedPointPosition eyeOffset;

  // Yaw 0x3000 (in 1/65536 turns), pitch 0xfe00, and 510 pixels away
  // (0x1fe0000 in 16.16), the distance BattleCamera_SetView
  // uses.
  camera->yaw = 192 << 6;
  camera->pitch = 254 << 8;
  camera->distance = 255 << 17;
  SET_VECTOR(camera->focus, 0, 0, 0);
  camera->followPosition = 0;
  camera->followEye = 0;

  Transform_Reset();
  Transform_ApplyPosition(&camera->focus[0]);
  Transform_ApplyYaw((s16)camera->yaw);
  Transform_ApplyPitch((s16)camera->pitch);

  eyeOffset.x = 0;
  eyeOffset.y = 0;
  eyeOffset.z = camera->distance;
  Iwram_TransformVector(&eyeOffset, (s32 *)camera);
}

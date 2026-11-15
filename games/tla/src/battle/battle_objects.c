/// @file
/// Each frame's sprite priorities, the status icons, the poses and the
/// sprites of the battle objects, and the camera they are projected through.
// Part of battle.c.

#include "battle_internal.h"
#include "battle/battle.h"
#include "gamepak/far_call/field.h"
#include "system/system_bank.h"
#include "macros.h"

// The status icons' bits in BattleObject.activeStatuses
#define ICON_POISON 0x01
#define ICON_VENOM 0x02 ///< deadly poison
#define ICON_STUN 0x04
#define ICON_PSY_SEAL 0x08
#define ICON_SLEEP 0x10
#define ICON_DELUSION 0x20
#define ICON_HAUNT 0x40
#define ICON_GRIM_REAPER 6 ///< the curse's turns left, 1 to 7, as bits 7 to 13

/// Builds the status icon bits of unit id's statuses into its battle object's
/// activeStatuses, which BattleStatusIcon_Cycle shows; an object marked
/// visible keeps its bits. A stun shows no icon on the Serpent, the Kraken or
/// the Fusion Dragon.
void BattleUnit_BuildStatusFlags(s32 id, struct BattleObject *output) {
  u32 flags = 0;

  if (output->visible == 0) {
    struct Unit *state = Unit_Get(id);
    s8 mode = state->poison;

    if (mode == POISON_NORMAL)
      flags = ICON_POISON;
    // Deadly poison: poison is ICON_VENOM.
    if (mode == POISON_VENOM)
      flags |= mode;
    if (state->delusion != 0)
      flags |= ICON_DELUSION;
    if (state->stun != 0) {
      flags |= ICON_STUN;
      switch (state->enemyId) {
      case ENEMY_SERPENT:
      case ENEMY_SERPENT_2:
      case ENEMY_SERPENT_3:
      case ENEMY_SERPENT_4:
      case ENEMY_SERPENT_5:
      case ENEMY_SERPENT_6:
      case ENEMY_SERPENT_7:
      case ENEMY_KRAKEN:
      case ENEMY_FUSION_DRAGON:
        flags &= ~ICON_STUN;
        break;
      }
    }
    if (state->psySeal != 0)
      flags |= ICON_PSY_SEAL;
    if (state->haunt != 0)
      flags |= ICON_HAUNT;
    if (state->sleep != 0)
      flags |= ICON_SLEEP;
    if (state->grimReaper != 0)
      flags |= 1 << (state->grimReaper + ICON_GRIM_REAPER);
    output->activeStatuses = flags;
  }
}

#define ICON_MAX 14 ///< the kinds of icon

/// The frames each status icon shows before the next one's turn.
#define ICON_CYCLE_FRAMES 80

/// Cycles the status icon over the slot's conditions every 80 frames, and
/// switches at once when the shown condition ends.
void BattleStatusIcon_Cycle(struct BattleObject *slot) {
  struct SpriteAnimation *icon;
  struct Sprite *rec;
  struct MotionObject *owner;
  s32 anim;
  s32 handle;
  s32 prev;
  s32 changed = 0;

  if (slot->cycleTimer >= 0)
    slot->cycleTimer--;

  if (((s16)slot->activeStatuses != 0 && slot->animationEntry == 0) ||
      (slot->animationEntry != 0 &&
       (((s16)slot->activeStatuses >> slot->selectedCondition) & 1) == 0) ||
      slot->cycleTimer == 0) {
    anim = -1;
    owner = slot->object;
    if ((s16)slot->activeStatuses != 0) {
      prev = slot->selectedCondition;
      for (anim = prev + 1;; anim++) {
        if (anim > ICON_MAX - 1)
          anim = 0;
        if ((((s16)slot->activeStatuses >> anim) & 1) != 0)
          break;
      }

      if (prev != anim || slot->animationEntry == 0) {
        slot->selectedCondition = anim;
        changed = 1;
      }
      slot->cycleTimer = ICON_CYCLE_FRAMES;
    } else {
      changed = 1;
    }

    rec = GetMotionRecord(owner, 0);
    if (rec != 0) {
      if (anim >= 0) {
        if (rec->width == 32)
          anim += 702;
        else
          anim += 688;
      }

      if (slot->animationEntry != 0 && changed != 0) {
        Sprite_RemoveAnimationFar(rec, (s32)slot->animationEntry);
        slot->animationEntry = 0;
      }

      if (anim >= 0 && changed != 0) {
        handle = Sprite_AddAnimationFar(rec, anim);
        slot->animationEntry = (struct SpriteAnimation *)handle;
        if (handle == -1)
          slot->animationEntry = 0;
        icon = (struct SpriteAnimation *)slot->animationEntry;
        if (icon != 0) {
          icon->priority = 3;
          SpriteAnimation_StartFar(icon, 0);
        }
      }

      rec->dirty = 1;
      if (anim >= 0)
        slot->animation = anim;
      else
        slot->animation = 0;
    }
  }
}

/// Sets a unit's pose to match its state, held, down or standing, and sets its
/// sway a little out of step with its neighbours'.
void BattleObject_UpdatePose(s32 id) {
  struct Unit *state;
  s32 value;

  state = Unit_Get(id);
  value = POSE_STANDING;
  if (state->hp != 0) {
    if (state->sleep != 0 || state->stun != 0 || state->immobile != 0)
      value = (state->side != 1) * 4;
  } else {
    s32 changed = state->side ^ value;
    value = NONZERO(changed);
    value = 5 - value;
  }

  Actor_SelectAnimationFar((struct Actor *)BattleObject_Get(id)->object, value);
  Actor_SetAnimationSpeedFar((struct Actor *)BattleObject_Get(id)->object,
                             (id & 3) + 14);
}

/// Releases all graphics the object owns and marks it as having none.
/// no: object ID.
void ReleaseBattleObjectRecords(s32 no) {
  struct BattleObject *slot;
  struct MotionObject *obj;
  s32 i;
  void *rec;

  slot = BattleObject_Get(no);
  if (slot != NULL) {
    obj = slot->object;
    if (obj != NULL) {
      slot->animationEntry = 0;
      slot->shadowEntry = 0;
      i = 0;
      while ((rec = GetMotionRecord(obj, i)) != NULL) {
        Sprite_DestroyFar(rec);
        i += 1;
      }
      obj->recordStorageKind = 0; // No graphics.
      obj->records = NULL;
    }
  }
}

/// Readies the sprites of the units in list: releases those of every place
/// list lacks, then creates each listed unit's sprites (body, weapon, status
/// icon and shadow; or the two parts of a two-part enemy) unless it has
/// them, building its status icons and setting its pose. With refreshPoses,
/// sets every listed unit's pose again afterwards. Called by BattleObject_Put
/// (refreshing) and BattleUnit_ListAllAndSubmit, by the staging and
/// entry transition code, and through its far stub by
/// BattleEffect_SelectLivingTargets. Writes the actors' sprites, their storage
/// kind and height, and the battle objects' icon and shadow entries.
void BattleObject_SpawnObjectsForList(s16 *unitIds, s32 refreshPoses,
                                      s32 releaseOthers) {
  s32 unitIndex;
  s32 unitId;
  struct BattleObject *battleObject;
  struct MotionObject *object;
  struct Sprite *record;
  struct SpriteAnimation *entry;
  struct Sprite **records;
  s32 spriteId;
  s32 releasedUnitId;
  u8 boxY;

  if (releaseOthers) {
    for (unitIndex = 0; unitIndex < UNIT_MAX; unitIndex++) {
      if (!BattlePlacement_ContainsId(unitIds, unitIndex)) {
        releasedUnitId = unitIndex + FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
        if (unitIndex <= ALLY_MAX)
          releasedUnitId = unitIndex;
        ReleaseBattleObjectRecords(releasedUnitId);
      }
    }
  }
  if (gTasksRunning == 0)
    wait(1);

  for (unitIndex = 0; unitIndex < UNIT_MAX &&
                      (unitId = unitIds[unitIndex]) != BATTLE_UNIT_LIST_END;
       unitIndex++) {
    if (unitId == BATTLE_UNIT_REMOVED)
      continue;
    battleObject = BattleObject_Get(unitId);
    if (battleObject == 0)
      continue;
    if (battleObject->fading != 0)
      continue;
    BattleUnit_BuildStatusFlags(unitId, battleObject);
    object = battleObject->object;
    if (object == 0)
      continue;
    if (object->recordStorageKind != 0)
      continue;
    if ((battleObject->resource & 0xfff) == 561 ||
        (battleObject->resource & 0xfff) == 631 ||
        (battleObject->resource & 0xfff) == 638 ||
        (battleObject->resource & 0xfff) == 585 ||
        (battleObject->resource & 0xfff) == 579 ||
        (battleObject->resource & 0xfff) == 581 ||
        (battleObject->resource & SPRITE_ID_MASK) == 583) {
      records = (struct Sprite **)Ram_HeapSlots->actorSystem->unk08 +
                Ram_HeapSlots->actorSystem->count;
      spriteId = battleObject->resource;
      object->recordStorageKind = 2;
      object->records = records;
      Iwram_ClearWordsEntry(records, 16);
      record = (struct Sprite *)Sprite_CreateFar(spriteId);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        boxY = Resource_GetMetadataRecordFarFar(spriteId)->boxY;
        object->height = boxY >> 1;
        *records = record;
        records++;
      }
      record->flags = 0;
      record = (struct Sprite *)Sprite_CreateFar(spriteId + SPRITE_SECOND_PART);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        *records = record;
      }
      record->flags = 0;
    } else {
      record = (struct Sprite *)Sprite_CreateFar(battleObject->resource);
      if (record != 0) {
        object->recordStorageKind = 1;
        object->records = record;
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        entry = record->entries[0];
        entry->colorMode = battleObject->palette;
        entry->priority = 1;
        if ((spriteId = battleObject->overlay) != 0) {
          entry = (struct SpriteAnimation *)Sprite_AddAnimationFar(record,
                                                                   spriteId);
          entry->priority = 1;
        }
        if ((spriteId = battleObject->animation) != 0) {
          entry = (struct SpriteAnimation *)Sprite_AddAnimationFar(record,
                                                                   spriteId);
          battleObject->animationEntry = entry;
          SpriteAnimation_StartFar(entry, 0);
          entry->priority = 3;
        }
        if ((spriteId = battleObject->shadow) != 0) {
          if (record->width == 32)
            spriteId = SPRITE_BATTLE_SHADOW_NARROW;
          entry = (struct SpriteAnimation *)Sprite_AddAnimationFar(record,
                                                                   spriteId);
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
            (object = battleObject->object) != 0)
          BattleObject_UpdatePose(spriteId);
        unitIndex++;
        if (unitIndex >= UNIT_MAX ||
            (spriteId = unitIds[unitIndex]) == BATTLE_UNIT_LIST_END)
          break;
      }
    }
  }
}

/// Spawns the battle objects of the units in list, ended by 0xff, refreshing
/// those that exist when refresh is set.
void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh) {
  BattleObject_SpawnObjectsForList(list, refresh, 1);
}

/// Detaches the scripts from the four sprite entries of one sprite.
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

/// Returns a unit's battle object, through its place, or null for a unit with
/// no place.
static inline struct BattleObject *BattleObject_GetInline(s32 unitId) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct BattleObject *objects = battle->slots;
  s32 placementIndex;

  if (unitId > ALLY_MAX)
    unitId -= 120;
  placementIndex = unitId;
  if (battle->placement[placementIndex] == 0xff)
    return 0;
  return &objects[battle->placement[placementIndex]];
}

/// Task, each frame of a battle: advances every unit's status icon, sets its
/// shadow's priority by whether it is airborne and draws the nearer side in
/// front.
void BattleObject_UpdateSpritePriorities(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 unitIds[UNIT_MAX];
  struct MotionObject *objects[14];
  s32 partyCount;
  s32 enemyCount;
  s32 partyPriority, enemyPriority;
  s32 unitIndex;
  s32 recordIndex;
  struct MotionObject *object;
  struct Sprite *record;
  struct Sprite **records;

  partyCount = BattleUnit_ListIds(1, unitIds);
  enemyCount = BattleUnit_ListIds(2, &unitIds[partyCount]);
  unitIndex = 0;
  if (unitIds[unitIndex] != BATTLE_UNIT_LIST_END) {
    for (;;) {
      /* FAKEMATCH(pin): slot held in r5. */
      register struct BattleObject *slot asm("r5");

      object = 0;
      slot = BattleObject_GetInline(unitIds[unitIndex]);
      if (slot != 0) {
        object = slot->object;
        BattleStatusIcon_Cycle(slot);
        if (slot->shadowEntry != 0) {
          struct Sprite *record = GetMotionRecord(object, 0);

          if (record != 0) {
            struct SpriteAnimation *effect;
            s32 effectPriority = 0;

            if (object->y != 0)
              effectPriority = 9;
            effect = slot->shadowEntry;
            if (effect->priority != effectPriority) {
              effect->priority = effectPriority;
              record->dirty = 1;
            }
          }
        }
      }
      objects[unitIndex] = object;
      unitIndex++;
      if (unitIndex >= 14 || unitIds[unitIndex] == BATTLE_UNIT_LIST_END)
        break;
    }
  }

  if (work->keepObjectPriorities != 0)
    return;

  if ((s16)((struct BattleCamera *)Ram_HeapSlots->cameraWork)->yaw >= 0) {
    partyPriority = 1;
    enemyPriority = 2;
  } else {
    partyPriority = 2;
    enemyPriority = 1;
  }

  for (unitIndex = 0; unitIndex < partyCount; unitIndex++) {
    object = objects[unitIndex];
    if (object != 0) {
      switch (object->recordStorageKind & 15) {
      case 1:
        record = object->records;
        record->image.priority = partyPriority;
        if (Unit_Get(unitIds[unitIndex])->defendLevel == 4)
          record->flags |= 2;
        else
          record->flags &= ~2;
        break;
      case 2:
        records = object->records;
        for (recordIndex = 0; recordIndex < 4; recordIndex++) {
          struct Sprite *part = *records++;

          if (part != 0) {
            record = part;
            record->image.priority = partyPriority;
          }
        }
        break;
      }
    }
  }

  for (unitIndex = partyCount; unitIndex < partyCount + enemyCount;
       unitIndex++) {
    object = objects[unitIndex];
    if (object != 0) {
      switch (object->recordStorageKind & 15) {
      case 1:
        record = object->records;
        record->image.priority = enemyPriority;
        if (Unit_Get(unitIds[unitIndex])->defendLevel == 4)
          record->flags |= 2;
        else
          record->flags &= ~2;
        break;
      case 2:
        records = object->records;
        for (recordIndex = 0; recordIndex < 4; recordIndex++) {
          struct Sprite *part = *records++;

          if (part != 0) {
            record = part;
            record->image.priority = enemyPriority;
          }
        }
        break;
      }
    }
  }
}

/// Detaches the scripts from every sprite of a unit's actor. Nothing in the
/// source calls it.
void ResetBattleObjectRecordGroups(struct MotionObject *object) {
  s32 storageKind;
  s32 remaining;
  void **recordGroups;
  void *recordGroup;

  if (object != NULL) {
    storageKind = object->recordStorageKind & 0xF;
    switch (storageKind) {
    case 1:
      ResetMotionRecordGroup(object->records);
      return;
    case 2:
      recordGroups = object->records;
      remaining = 3;
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

/// Returns a unit's battle object, through its place in BattleWork.placement,
/// or null for a unit with no place.
static __inline__ struct BattleObject *BattleObject_FindSlot(s32 id) {
  u8 *work = (u8 *)Ram_HeapSlots->battleWork;
  struct BattleObject *slots = ((struct BattleWork *)work)->slots;

  if (id > ALLY_MAX)
    id -= 0x78;
  id += 0x74;
  if (work[id] == 0xff)
    return NULL;
  return &slots[work[id]];
}

/// Frees the enemy sprite slots a unit holds and marks its actor for release at
/// the end of the action. Returns 0.
s32 BattleObject_MarkForRelease(s32 objectId) {
  struct BattleObject *slot;

  BattleObject_ReleaseSpriteSlots(objectId);
  slot = BattleObject_FindSlot(objectId);
  slot->active = 1;
  return 0;
}

/// Releases the actor of every battle object marked for release.
void BattleMotion_DestroyAllSlotObjects(void) {
  s32 no;
  s32 i;
  struct BattleObject *slot;

  i = 0;
  do {
    no = i + 0x78;
    if (i <= 7) {
      no = i;
    }
    slot = BattleObject_FindSlot(no);
    if ((slot != NULL) && (slot->active != 0)) {
      Actor_ReleaseFar((struct Actor *)slot->object);
      slot->object = NULL;
      slot->active = 0;
    }
    i += 1;
  } while (i <= 0xD);
}

/// Writes a battle object's anchor to out, at height 0.
s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out) {
  struct BattleWork *state = Ram_HeapSlots->battleWork;
  s32 result;

  out->x = state->slots[no].anchorX;
  out->y = 0;
  result = 0;
  out->z = state->slots[no].anchorZ;
  return result;
}

/// Sets up the camera's view for projecting points, tilted further while
/// commands are chosen.
UNREAD_RESULT BattleCamera_ApplyTransform(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;

  Transform_Reset();
  if (GameFlag_Get(FLAG_CHOOSING_COMMANDS) != 0) {
    Iwram_TransformMatrix(gBattleCamera_CommandMatrix);
    Transform_ApplyLookAt(state->eye, state->focus);
  } else {
    Transform_SetLookAt(state->eye, state->focus);
  }
}

/// Projects a battle object's position to the screen, in projected.
s32 BattleMotion_ProjectPosition(s32 id, s32 *projected) {
  struct MotionObject *object = BattleObject_FindSlot(id)->object;
  struct Sprite *record = GetMotionRecord(object, 0);
  s32 position[3];
  s32 scaled;

  BattleCamera_ApplyTransform();
  {
    s32 *pos = position;
    s32 *out;

    pos[0] = object->x;
    out = projected;
    pos[1] = object->y;
    pos[2] = object->z;
    scaled = Render_ProjectPoint(pos, out);
  }
  (void)Iwram_MulQ16(scaled, record->scale);
  return 0;
}

/// Returns sprite recordIndex of an actor (0 only, for a single sprite), or
/// null when it has no such sprite.
void *GetMotionRecord(struct MotionObject *object, s32 recordIndex) {
  s32 storageKind;

  if (object == NULL)
    return NULL;
  storageKind = object->recordStorageKind & 0xF;
  if (storageKind == 1) {
    if (recordIndex == 0) {
      return object->records;
    }
  } else if (storageKind == 2) {
    return ((void **)object->records)[recordIndex];
  }
  return NULL;
}

/// Puts the battle camera at its default view of the battlefield's centre.
void BattleCamera_SetDefaultView(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct FixedPointPosition transfer;

  // A pitch of -1/128 of a turn, 510 pixels away.
  state->yaw = TRIG_TURN * 3 / 16;
  state->pitch = TRIG_TURN - TRIG_TURN / 128;
  state->distance = BATTLE_CAMERA_DISTANCE;
  SET_VECTOR(state->focus, 0, 0, 0);
  state->followPosition = 0;
  state->followEye = 0;

  Transform_Reset();
  Transform_ApplyPosition(&state->focus[0]);
  Transform_ApplyYaw((s16)state->yaw);
  Transform_ApplyPitch((s16)state->pitch);

  SET_POSITION(&transfer, 0, 0, state->distance);
  Iwram_TransformVector(&transfer, (s32 *)state);
}

/// Returns a unit's battle object, through its place, or null for a unit with
/// no place.
struct BattleObject *BattleObject_Get(s32 objectId) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  struct BattleObject *slots = work->slots;
  s32 index;

  if (objectId > ALLY_MAX) {
    objectId -= 0x78;
  }
  index = objectId;
  if (work->placement[index] == BATTLE_OBJECT_NONE) {
    return NULL;
  }
  return &slots[work->placement[index]];
}

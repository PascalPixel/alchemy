/// @file
/// Each frame's sprite priorities, the status icons, the poses and the
/// sprites of the battle objects, and the camera they are projected through.
// Part of battle.c.

#include "../battle_internal.h"

/// Builds a battle object's status icon flags from its unit: poison
/// (1) or deadly poison (2), delusion (0x20), stun (4, not for nine large
/// enemies), Psy Seal (8), haunt (0x40), sleep (0x10) and the Grim Reaper's
/// countdown (bit 6 + count); an object marked visible keeps its flags.
void BattleUnit_BuildStatusFlags(s32 id, struct TlaBattleObj *output) {
  u32 flags = 0;

  if (output->visible == 0) {
    struct BattleUnit *state = Unit_Get(id);
    s8 mode = state->poison;

    if (mode == 1)
      flags = 1;
    if (mode == 2)
      flags |= mode;
    if (state->delusion != 0)
      flags |= 0x20;
    if (state->stun != 0) {
      flags |= 4;
      switch (state->enemyId) {
      case 91:
      case 92:
      case 93:
      case 94:
      case 95:
      case 96:
      case 97:
      case 348:
      case 357:
        flags &= ~4;
        break;
      }
    }
    if (state->psySeal != 0)
      flags |= 8;
    if (state->haunt != 0)
      flags |= 0x40;
    if (state->sleep != 0)
      flags |= 0x10;
    if (state->grimReaper != 0)
      flags |= 1 << (state->grimReaper + 6);
    output->activeConditions = flags;
  }
}

struct StatusIconRecord {
  u8 unk00[0x14];
  u8 width;
  u8 unk15[4];
  u8 dirty;
};

struct StatusIconEntry {
  u8 unk00[6];
  u8 priority;
};

void *GetMotionRecord(struct MotionObject *object, s32 record_index);

#define ICON_MAX 14

/// Cycles the status icon over the slot's conditions every 80 frames, and
/// switches at once when the shown condition ends.
void BattleStatusIcon_Cycle(struct BattleObj *slot) {
  struct StatusIconEntry *icon;
  struct StatusIconRecord *rec;
  struct MotionObject *owner;
  s32 anim;
  s32 handle;
  s32 prev;
  s32 changed = 0;

  if (slot->cycleTimer >= 0)
    slot->cycleTimer--;

  if (((s16)slot->activeConditions != 0 && slot->animationEntry == 0) ||
      (slot->animationEntry != 0 &&
       (((s16)slot->activeConditions >> slot->selectedCondition) & 1) == 0) ||
      slot->cycleTimer == 0) {
    anim = -1;
    owner = slot->object;
    if ((s16)slot->activeConditions != 0) {
      prev = slot->selectedCondition;
      for (anim = prev + 1;; anim++) {
        if (anim > ICON_MAX - 1)
          anim = 0;
        if ((((s16)slot->activeConditions >> anim) & 1) != 0)
          break;
      }

      if (prev != anim || slot->animationEntry == 0) {
        slot->selectedCondition = anim;
        changed = 1;
      }
      slot->cycleTimer = 80;
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
        ResourceMetadata_UnregisterFar(rec, (s32)slot->animationEntry);
        slot->animationEntry = 0;
      }

      if (anim >= 0 && changed != 0) {
        handle = ResourceMetadata_RegisterFar(rec, anim);
        slot->animationEntry = (struct SpriteEntry *)handle;
        if (handle == -1)
          slot->animationEntry = 0;
        icon = (struct StatusIconEntry *)slot->animationEntry;
        if (icon != 0) {
          icon->priority = 3;
          Animation_SetWorkEntryFar(icon, 0);
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

void BattleObject_UpdatePose(s32 id) {
  struct BattleUnit *state;
  s32 value;

  state = Unit_Get(id);
  value = 1;
  if (state->hp != 0) {
    if (state->sleep != 0 || state->stun != 0 || state->immobile != 0)
      value = (state->side != 1) * 4;
  } else {
    s32 changed = state->side ^ value;
    value = (u32)(-changed | changed) >> 31;
    value = 5 - value;
  }

  Actor_SetMode((struct Actor *)BattleObject_Get(id)->object, value);
  Actor_SetAnimationSpeedFar((struct ActorView *)BattleObject_Get(id)->object,
                             (id & 3) + 14);
}

/// Releases all graphics the object owns and marks it as having none.
/// no: object ID.
void ReleaseBattleObjectRecords(s32 no) {
  struct BattleObj *slot;
  struct MotionObject *obj;
  s32 i;
  void *rec;

  slot = BattleObject_Get(no);
  if (slot != NULL) {
    obj = slot->object;
    if (obj != NULL) {
      slot->animationEntry = 0;
      slot->effectEntry = 0;
      i = 0;
      while ((rec = GetMotionRecord(obj, i)) != NULL) {
        ResourceObject_ReleaseFar(rec);
        i += 1;
      }
      obj->recordStorageKind = 0; // No graphics.
      obj->records = NULL;
    }
  }
}

struct TlaSpriteRecord {
  struct AnimationSpritePart image;
  s32 scale;
  u8 unk10[4];
  u8 width;
  u8 unk15[4];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct SpriteEntry *entries[1];
};

s32 BattlePlacement_ContainsId(s16 *unitIds, s32 id);

void Func_0811b75c(s16 *unitIds, s32 refreshPoses, s32 releaseOthers) {
  s32 unitIndex;
  s32 unitId;
  struct TlaBattleObj *battleObject;
  struct MotionObject *object;
  struct TlaSpriteRecord *record;
  struct SpriteEntry *entry;
  struct TlaSpriteRecord **records;
  s32 spriteId;
  s32 releasedUnitId;
  u8 boxY;

  if (releaseOthers) {
    for (unitIndex = 0; unitIndex < 14; unitIndex++) {
      if (!BattlePlacement_ContainsId(unitIds, unitIndex)) {
        releasedUnitId = unitIndex + 120;
        if (unitIndex <= 7)
          releasedUnitId = unitIndex;
        ReleaseBattleObjectRecords(releasedUnitId);
      }
    }
  }
  if (gTasksRunning == 0)
    wait(1);

  for (unitIndex = 0; unitIndex < 14 && (unitId = unitIds[unitIndex]) != 255;
       unitIndex++) {
    if (unitId == 254)
      continue;
    battleObject = (struct TlaBattleObj *)BattleObject_Get(unitId);
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
        (battleObject->resource & 0xfff) == 583) {
      records = (struct TlaSpriteRecord **)Ram_HeapSlots->objectSystem->unk08 +
                Ram_HeapSlots->objectSystem->count;
      spriteId = battleObject->resource;
      object->recordStorageKind = 2;
      object->records = records;
      Iwram_ClearWords(records, 16);
      record = (struct TlaSpriteRecord *)ResourceObject_CreateFar(spriteId);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        boxY = Resource_GetMetadataRecordFarFar(spriteId)->boxY;
        object->height = boxY >> 1;
        *records = record;
        records++;
      }
      record->flags = 0;
      record = (struct TlaSpriteRecord *)ResourceObject_CreateFar(spriteId + 0x2001);
      if (record != 0) {
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        *records = record;
      }
      record->flags = 0;
    } else {
      record = (struct TlaSpriteRecord *)ResourceObject_CreateFar(battleObject->resource);
      if (record != 0) {
        object->recordStorageKind = 1;
        object->records = record;
        record->scale = Iwram_MulQ16(record->scale, battleObject->scale);
        entry = record->entries[0];
        entry->palette = battleObject->palette;
        entry->priority = 1;
        if ((spriteId = battleObject->overlay) != 0) {
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(record, spriteId);
          entry->priority = 1;
        }
        if ((spriteId = battleObject->animation) != 0) {
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(record, spriteId);
          battleObject->animationEntry = entry;
          Animation_SetWorkEntryFar(entry, 0);
          entry->priority = 3;
        }
        if ((spriteId = battleObject->effect) != 0) {
          if (record->width == 32)
            spriteId = 687;
          entry = (struct SpriteEntry *)ResourceMetadata_RegisterFar(record, spriteId);
          battleObject->effectEntry = entry;
          entry->priority = 0;
          record->flags = 0;
        }
      }
    }
    BattleObject_UpdatePose(unitId);
  }

  if (refreshPoses) {
    unitIndex = 0;
    if ((spriteId = unitIds[0]) != 255) {
      for (;;) {
        if (unitIds[unitIndex] != 254 &&
            (battleObject =
                 (struct TlaBattleObj *)BattleObject_Get(spriteId)) != 0 &&
            (object = battleObject->object) != 0)
          BattleObject_UpdatePose(spriteId);
        unitIndex++;
        if (unitIndex >= 14 || (spriteId = unitIds[unitIndex]) == 255)
          break;
      }
    }
  }
}

void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh) {
  Func_0811b75c(list, refresh, 1);
}

struct AnimationEntryGroup {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

void ResetMotionRecordGroup(void *owner) {
  struct AnimationEntryGroup *object = owner;
  s32 i;

  if (object != NULL) {
    for (i = 0; i < 4; i++) {
      if (object->entries[i] != NULL)
        object->entries[i]->script = NULL;
    }
  }
}

static inline struct TlaBattleObj *BattleObject_GetInline(s32 unitId) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  struct TlaBattleObj *objects = (struct TlaBattleObj *)battle->slots;
  s32 placementIndex;

  if (unitId > 7)
    unitId -= 120;
  placementIndex = unitId;
  if (battle->placement[placementIndex] == 0xff)
    return 0;
  return &objects[battle->placement[placementIndex]];
}

void Func_0811b9fc(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 unitIds[14];
  struct MotionObject *objects[14];
  s32 partyCount;
  s32 enemyCount;
  s32 partyPriority, enemyPriority;
  s32 unitIndex;
  s32 recordIndex;
  struct MotionObject *object;
  struct TlaSpriteRecord *record;
  struct TlaSpriteRecord **records;

  partyCount = BattleParty_ListActorIds(1, unitIds);
  enemyCount = BattleParty_ListActorIds(2, &unitIds[partyCount]);
  unitIndex = 0;
  if (unitIds[unitIndex] != 255) {
    for (;;) {
      /* FAKEMATCH(pin): slot held in r5; as a plain local, 27 of 273
       * instructions differ in tla-en. */
      register struct TlaBattleObj *slot asm("r5");

      object = 0;
      slot = BattleObject_GetInline(unitIds[unitIndex]);
      if (slot != 0) {
        object = slot->object;
        BattleStatusIcon_Cycle((struct BattleObj *)slot);
        if (slot->effectEntry != 0) {
          struct TlaSpriteRecord *record = GetMotionRecord(object, 0);

          if (record != 0) {
            struct SpriteEntry *effect;
            s32 effectPriority = 0;

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
      objects[unitIndex] = object;
      unitIndex++;
      if (unitIndex >= 14 || unitIds[unitIndex] == 255)
        break;
    }
  }

  if (((u8 *)work)[0x866] != 0)
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
        if (((struct BattleUnit *)Unit_Get(unitIds[unitIndex]))->guardLevel == 4)
          record->flags |= 2;
        else
          record->flags &= ~2;
        break;
      case 2:
        records = object->records;
        for (recordIndex = 0; recordIndex < 4; recordIndex++) {
          struct TlaSpriteRecord *part = *records++;

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
        if (((struct BattleUnit *)Unit_Get(unitIds[unitIndex]))->guardLevel == 4)
          record->flags |= 2;
        else
          record->flags &= ~2;
        break;
      case 2:
        records = object->records;
        for (recordIndex = 0; recordIndex < 4; recordIndex++) {
          struct TlaSpriteRecord *part = *records++;

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

// BattleObject_Get, as the native expands it here.
static __inline__ struct BattleObj *BattleObject_FindSlot(s32 id) {
  u8 *work = (u8 *)Ram_HeapSlots->battleWork;
  struct BattleObj *slots = ((struct BattleWork *)work)->slots;

  if (id > 7)
    id -= 0x78;
  id += 0x74;
  if (work[id] == 0xff)
    return NULL;
  return &slots[work[id]];
}

s32 BattleObject_MarkForRelease(s32 objectId) {
  struct BattleObj *slot;

  BattleMotion_ReleaseObjectSlotByValue(objectId);
  slot = BattleObject_FindSlot(objectId);
  slot->active = 1;
  return 0;
}

void BattleMotion_DestroyAllSlotObjects(void) {
  s32 no;
  s32 i;
  struct BattleObj *slot;

  i = 0;
  do {
    no = i + 0x78;
    if (i <= 7) {
      no = i;
    }
    slot = BattleObject_FindSlot(no);
    if ((slot != NULL) && (slot->active != 0)) {
      Actor_Destroy((struct ActorView *)slot->object);
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

void Render_ResetTransformState(void);
s32 GameFlag_Test(s32 flag);
s32 Transform_ApplyLookAt(void *, void *);
s32 Transform_SetLookAt(void *, void *);
s32 Render_ProjectPoint(const void *, s32 *);
extern const s32 gData_08128868[];

struct MotionRecordScale {
  u8 unk00[0xc];
  s32 scale;
};

s32 Camera_ApplyTransformByFlag(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;

  Render_ResetTransformState();
  if (GameFlag_Test(0x16B) != 0) {
    Iwram_TransformMatrix(gData_08128868);
    return Transform_ApplyLookAt(state, state->pos);
  } else {
    return Transform_SetLookAt(state, state->pos);
  }
}

/// Projects a battle object's position to the screen, in projected.
s32 BattleMotion_ProjectPosition(s32 id, s32 *projected) {
  struct MotionObject *object = BattleObject_FindSlot(id)->object;
  struct MotionRecordScale *record = GetMotionRecord(object, 0);
  s32 position[3];
  s32 scaled;

  Camera_ApplyTransformByFlag();
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

void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);

void Camera_InitializeDefaultTransform(void) {
  struct BattleCamera *state = Ram_HeapSlots->cameraWork;
  struct FixedPointPosition transfer;

  state->yaw = 192 << 6;
  state->pitch = 254 << 8;
  state->distance = 255 << 17;
  SET_VECTOR(state->pos, 0, 0, 0);
  state->followPos = 0;
  state->unk18 = 0;

  Render_ResetTransformState();
  SceneTransform_ApplyPosition(&state->pos[0]);
  SceneTransform_ApplyYaw((s16)state->yaw);
  SceneTransform_ApplyPitch((s16)state->pitch);

  transfer.x = 0;
  transfer.y = 0;
  transfer.z = state->distance;
  Iwram_TransformVector(&transfer, (s32 *)state);
}

struct BattleObj *BattleObject_Get(s32 objectId) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  struct BattleObj *slots = work->slots;
  s32 index;

  if (objectId > 7) {
    objectId -= 0x78;
  }
  index = objectId;
  if (work->placement[index] == 0xFF) {
    return NULL;
  }
  return &slots[work->placement[index]];
}

/* Each tick's sprite priorities, the status icons, the poses and the sprites
   of the battle objects, and the camera they are projected through. Part of
   battle.c. */

/* A sprite animation's priority above 3, which is not drawn: a shadow's while
   its unit is in the air. */
#define SHADOW_HIDDEN_PRIORITY 9

/* The status icons' bits in BATTLE_OBJECT.activeStatuses. */
#define ICON_POISON			   0x01
#define ICON_VENOM			   0x02 /* deadly poison */
#define ICON_STUN			   0x04
#define ICON_PSY_SEAL		   0x08
#define ICON_SLEEP			   0x10
#define ICON_DELUSION		   0x20
#define ICON_HAUNT			   0x40
/* The curse's turns left, 1 to 7, shown as bits 7 to 13. */
#define ICON_GRIM_REAPER_SHIFT 6

/* Builds the status icon bits of unit id's statuses into its battle object's
   activeStatuses, which btStatIconCycle shows; an object marked visible keeps
   its bits. A stun shows no icon on the Serpent, the Kraken or the Fusion
   Dragon. */
void btUnitBuildStatFlags(Sint32 id, BATTLE_OBJECT *object)
{
	Uint32 flg = 0;

	if(object->visible == 0) {
		UNIT *unit = syUnitGet(id);
		Sint8 poison = unit->poison;

		if(poison == POISON_NORMAL)
			flg = ICON_POISON;
		/* Deadly poison: poison is ICON_VENOM. */
		if(poison == POISON_VENOM)
			flg |= poison;
		if(unit->delusion != 0)
			flg |= ICON_DELUSION;
		if(unit->stun != 0) {
			flg |= ICON_STUN;
			switch(unit->enemyId) {
			case ENEMY_SERPENT:
			case ENEMY_SERPENT_ONE_BEAM:
			case ENEMY_SERPENT_TWO_BEAMS:
			case ENEMY_SERPENT_THREE_BEAMS:
			case ENEMY_SERPENT_FOUR_BEAMS:
			case ENEMY_SERPENT_6:
			case ENEMY_SERPENT_7:
			case ENEMY_KRAKEN:
			case ENEMY_FUSION_DRAGON:
				flg &= ~ICON_STUN;
				break;
			}
		}
		if(unit->psySeal != 0)
			flg |= ICON_PSY_SEAL;
		if(unit->haunt != 0)
			flg |= ICON_HAUNT;
		if(unit->sleep != 0)
			flg |= ICON_SLEEP;
		if(unit->grimReaper != 0)
			flg |= 1 << (unit->grimReaper + ICON_GRIM_REAPER_SHIFT);
		object->activeStatuses = flg;
	}
}

#define ICON_MAX 14 /* the kinds of icon */

/* The ticks each status icon shows before the next one's turn. */
#define ICON_CYCLE_TICKS 80

/* The icons' first animations, for units 32 pixels wide and for the rest: each
   adds the icon's bit number. */
#define ICON_ANIMATION_NARROW SPRITE_STATUS_POISON
#define ICON_ANIMATION_WIDE	  SPRITE_STATUS_WIDE_POISON

/* Shows the unit's status icons in turn, each for ICON_CYCLE_TICKS, and moves
   on at once when the shown status ends. */
void btStatIconCycle(BATTLE_OBJECT *slot)
{
	SPR_ANIMATION *icon;
	SPR			  *spr;
	MOTION_OBJECT *owner;
	Sint32		   anm;
	Sint32		   handle;
	Sint32		   shown;
	Sint32		   changed = 0;

	if(slot->cycleTmr >= 0)
		slot->cycleTmr--;

	if(((Sint16)slot->activeStatuses != 0 && slot->anmEntry == 0) ||
		(slot->anmEntry != 0 &&
			(((Sint16)slot->activeStatuses >> slot->selectedCondition) & 1) ==
				0) ||
		slot->cycleTmr == 0) {
		anm = -1;
		owner = slot->obj;
		if((Sint16)slot->activeStatuses != 0) {
			shown = slot->selectedCondition;
			for(anm = shown + 1;; anm++) {
				if(anm > ICON_MAX - 1)
					anm = 0;
				if((((Sint16)slot->activeStatuses >> anm) & 1) != 0)
					break;
			}

			if(shown != anm || slot->anmEntry == 0) {
				slot->selectedCondition = anm;
				changed = 1;
			}
			slot->cycleTmr = ICON_CYCLE_TICKS;
		} else {
			changed = 1;
		}

		spr = btActorGetSpr(owner, 0);
		if(spr != 0) {
			if(anm >= 0) {
				if(spr->width == 32)
					anm += ICON_ANIMATION_NARROW;
				else
					anm += ICON_ANIMATION_WIDE;
			}

			if(slot->anmEntry != 0 && changed != 0) {
				fdSprRemoveAnimationStub(spr, (Sint32)slot->anmEntry);
				slot->anmEntry = 0;
			}

			if(anm >= 0 && changed != 0) {
				handle = fdSprAddAnimationStub(spr, anm);
				slot->anmEntry = (SPR_ANIMATION *)handle;
				if(handle == -1)
					slot->anmEntry = 0;
				icon = (SPR_ANIMATION *)slot->anmEntry;
				if(icon != 0) {
					icon->pri = 3;
					fdSprAnimationStartStub(icon, 0);
				}
			}

			spr->dirty = 1;
			if(anm >= 0)
				slot->anm = anm;
			else
				slot->anm = 0;
		}
	}
}

/* Sets a unit's pose to match its state, held, down or standing, and sets its
   sway a little out of step with its neighbours': from two steps below normal
   speed to one above, by its id. */
void btObjectUpdatePose(Sint32 id)
{
	UNIT  *unit;
	Sint32 value;

	unit = syUnitGet(id);
	value = POSE_STANDING;
	if(unit->hp != 0) {
		if(unit->sleep != 0 || unit->stun != 0 || unit->immobile != 0)
			value =
				unit->side == BATTLE_SIDE_ENEMY ? POSE_STILL : POSE_PARTY_HELD;
	} else if(unit->side == BATTLE_SIDE_ENEMY) {
		value = POSE_ENEMY_DOWN;
	} else {
		value = POSE_PARTY_HELD;
	}

	fdActorSelectAnimationStub((ACTOR *)btObjectGet(id)->obj, value);
	fdActorSetAnimationSpeedStub(
		(ACTOR *)btObjectGet(id)->obj, ANIM_STEP_NORMAL - 2 + (id & 3));
}

/* Destroys a unit's sprites, its status icon and shadow with them, leaving its
   battle object with none. */
void btObjectDestroySprs(Sint32 unitId)
{
	BATTLE_OBJECT *slot;
	MOTION_OBJECT *object;
	Sint32		   i;
	void		  *spr;

	slot = btObjectGet(unitId);
	if(slot != NULL) {
		object = slot->obj;
		if(object != NULL) {
			slot->anmEntry = 0;
			slot->shadowEntry = 0;
			for(i = 0; (spr = btActorGetSpr(object, i)) != NULL; i += 1) {
				fdSprDestroyStub(spr);
			}
			object->recordStorageKind = ACTOR_SPRITE_NONE;
			object->records = NULL;
		}
	}
}

/* Readies the sprites of the units in unitIds (ended by BATTLE_UNIT_LIST_END):
   with releaseOthers, first destroys those of every unit not listed; then
   creates each listed unit's sprites (body, overlay, status icon and shadow; or
   the two parts of a two-part enemy) unless it has them, builds its status
   icons and sets its pose. With refreshPoses, sets every listed unit's pose
   again afterwards. */
void btObjectSpawnObjectsForList(
	Sint16 *unitIds, Sint32 refreshPoses, Sint32 releaseOthers)
{
	Sint32		   unitIdx;
	Sint32		   unitId;
	BATTLE_OBJECT *battleObject;
	MOTION_OBJECT *object;
	SPR			  *record;
	SPR_ANIMATION *entry;
	SPR			 **records;
	Sint32		   sprId;
	Sint32		   releasedUnitId;
	Uint8		   footprint;

	if(releaseOthers) {
		for(unitIdx = 0; unitIdx < UNIT_MAX; unitIdx++) {
			if(!btPlacementContainsId(unitIds, unitIdx)) {
				releasedUnitId = unitIdx + FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
				if(unitIdx <= ALLY_MAX)
					releasedUnitId = unitIdx;
				btObjectDestroySprs(releasedUnitId);
			}
		}
	}
	if(EventsRunning == 0)
		sySynch(1);

	for(unitIdx = 0; unitIdx < UNIT_MAX &&
		(unitId = unitIds[unitIdx]) != BATTLE_UNIT_LIST_END;
		unitIdx++) {
		if(unitId == BATTLE_UNIT_REMOVED)
			continue;
		battleObject = btObjectGet(unitId);
		if(battleObject == 0)
			continue;
		if(battleObject->fading != 0)
			continue;
		btUnitBuildStatFlags(unitId, battleObject);
		object = battleObject->obj;
		if(object == 0)
			continue;
		if(object->recordStorageKind != ACTOR_SPRITE_NONE)
			continue;
		if((battleObject->resource & SPRITE_ID_MASK) == SPRITE_BATTLE_SERPENT ||
			(battleObject->resource & SPRITE_ID_MASK) == SPRITE_BATTLE_KRAKEN ||
			(battleObject->resource & SPRITE_ID_MASK) ==
				SPRITE_BATTLE_FUSION_DRAGON ||
			(battleObject->resource & SPRITE_ID_MASK) ==
				SPRITE_BATTLE_POSEIDON ||
			(battleObject->resource & SPRITE_ID_MASK) ==
				SPRITE_BATTLE_DOOM_DRAGON_A ||
			(battleObject->resource & SPRITE_ID_MASK) ==
				SPRITE_BATTLE_DOOM_DRAGON_E ||
			(battleObject->resource & SPRITE_ID_MASK) ==
				SPRITE_BATTLE_DOOM_DRAGON_H) {
			/* A two-part enemy: its two sprites in the actor system's next
			   group table. BUG: a failed create leaves record null, and its
			   flags are still written. */
			records = (SPR **)RamHeapSlots->actorSystem->sprGroups +
				RamHeapSlots->actorSystem->cnt;
			sprId = battleObject->resource;
			object->recordStorageKind = ACTOR_SPRITE_GROUP;
			object->records = records;
			syWordsClearEntry(records, ACTOR_SPRITE_GROUP_SIZE * sizeof(SPR *));
			record = (SPR *)fdSprCreateStub(sprId);
			if(record != 0) {
				record->scale = syMulFX(record->scale, battleObject->scale);
				footprint =
					fdResourceGetMetadataRecordStubStub(sprId)->footprint;
				object->height = footprint >> 1;
				*records = record;
				records++;
			}
			record->flg = 0;
			record = (SPR *)fdSprCreateStub(sprId + SPRITE_SECOND_PART);
			if(record != 0) {
				record->scale = syMulFX(record->scale, battleObject->scale);
				*records = record;
			}
			record->flg = 0;
		} else {
			record = (SPR *)fdSprCreateStub(battleObject->resource);
			if(record != 0) {
				object->recordStorageKind = ACTOR_SPRITE_ONE;
				object->records = record;
				record->scale = syMulFX(record->scale, battleObject->scale);
				entry = record->entries[0];
				entry->colMode = battleObject->pal;
				entry->pri = 1;
				if((sprId = battleObject->overlay) != 0) {
					entry =
						(SPR_ANIMATION *)fdSprAddAnimationStub(record, sprId);
					entry->pri = 1;
				}
				if((sprId = battleObject->anm) != 0) {
					entry =
						(SPR_ANIMATION *)fdSprAddAnimationStub(record, sprId);
					battleObject->anmEntry = entry;
					fdSprAnimationStartStub(entry, 0);
					entry->pri = 3;
				}
				if((sprId = battleObject->shadow) != 0) {
					if(record->width == 32)
						sprId = SPRITE_BATTLE_SHADOW_NARROW;
					entry =
						(SPR_ANIMATION *)fdSprAddAnimationStub(record, sprId);
					battleObject->shadowEntry = entry;
					entry->pri = 0;
					record->flg = 0;
				}
			}
		}
		btObjectUpdatePose(unitId);
	}

	if(refreshPoses) {
		unitIdx = 0;
		if((sprId = unitIds[0]) != BATTLE_UNIT_LIST_END) {
			for(;;) {
				/* DEAD: object = battleObject->obj in the test, never read
				   after it; the ROM keeps the store. */
				if(unitIds[unitIdx] != BATTLE_UNIT_REMOVED &&
					(battleObject = btObjectGet(sprId)) != 0 &&
					(object = battleObject->obj) != 0)
					btObjectUpdatePose(sprId);
				unitIdx++;
				if(unitIdx >= UNIT_MAX ||
					(sprId = unitIds[unitIdx]) == BATTLE_UNIT_LIST_END)
					break;
			}
		}
	}
}

/* Readies the sprites of the units in list, ended by BATTLE_UNIT_LIST_END, and
   destroys every other unit's; with refresh, sets the listed units' poses
   again. */
void btActorSpawnObjectsForList(Sint16 *list, Sint32 refresh)
{
	btObjectSpawnObjectsForList(list, refresh, 1);
}

/* Detaches the scripts from the four sprite entries of one sprite. */
void btSprDetachScripts(void *recordGroup)
{
	SPR	  *record = recordGroup;
	Sint32 entryIdx;

	if(record != NULL) {
		for(entryIdx = 0; entryIdx < (Sint32)ARRAY_COUNT(record->entries);
			entryIdx++) {
			if(record->entries[entryIdx] != NULL)
				record->entries[entryIdx]->script = NULL;
		}
	}
}

/* Returns a unit's battle object, through its place in BATTLE_STATE.placement,
   or null for a unit with no place. */
static __inline__ BATTLE_OBJECT *btObjectFindSlot(Sint32 id)
{
	BATTLE_STATE  *battle = RamHeapSlots->battleState;
	BATTLE_OBJECT *slots = battle->slots;

	if(id > ALLY_MAX)
		id -= FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
	if(battle->placement[id] == BATTLE_OBJECT_NONE)
		return NULL;
	return &slots[battle->placement[id]];
}

/* Event, each tick of a battle: advances every unit's status icon, sets its
   shadow's priority by whether it is airborne, draws the nearer side in front
   and draws a unit under Haze (DEFEND_VANISHED) as an outline. */
void btObjectUpdateSprPriorities(void)
{
	BATTLE_STATE  *battle = RamHeapSlots->battleState;
	Uint16		   unitIds[UNIT_MAX];
	MOTION_OBJECT *objects[UNIT_MAX];
	Sint32		   partyCnt;
	Sint32		   enemyCnt;
	Sint32		   partyPri, enemyPri;
	Sint32		   unitIdx;
	Sint32		   recordIdx;
	MOTION_OBJECT *object;
	SPR			  *record;
	SPR			 **records;
	SPR			  *part;

	partyCnt = btUnitListIds(BATTLE_SIDE_MASK_PARTY, unitIds);
	enemyCnt = btUnitListIds(BATTLE_SIDE_MASK_ENEMIES, &unitIds[partyCnt]);
	unitIdx = 0;
	if(unitIds[unitIdx] != BATTLE_UNIT_LIST_END) {
		for(;;) {
			BATTLE_OBJECT *slot;

			object = 0;
			slot = btObjectFindSlot(unitIds[unitIdx]);
			if(slot != 0) {
				object = slot->obj;
				btStatIconCycle(slot);
				if(slot->shadowEntry != 0) {
					part = btActorGetSpr(object, 0);

					if(part != 0) {
						SPR_ANIMATION *effect;
						Sint32		   effectPri = 0;

						/* An airborne unit's shadow is hidden. */
						if(object->y != 0)
							effectPri = SHADOW_HIDDEN_PRIORITY;
						effect = slot->shadowEntry;
						if(effect->pri != effectPri) {
							effect->pri = effectPri;
							part->dirty = 1;
						}
					}
				}
			}
			objects[unitIdx] = object;
			unitIdx++;
			if(unitIdx >= UNIT_MAX || unitIds[unitIdx] == BATTLE_UNIT_LIST_END)
				break;
		}
	}

	if(battle->keepObjPriorities != 0)
		return;

	if(((BATTLE_CAMERA *)RamHeapSlots->cameraState)->yaw >= 0) {
		partyPri = 1;
		enemyPri = 2;
	} else {
		partyPri = 2;
		enemyPri = 1;
	}

	for(unitIdx = 0; unitIdx < partyCnt; unitIdx++) {
		object = objects[unitIdx];
		if(object != 0) {
			switch(object->recordStorageKind & ACTOR_SPRITE_KIND_MASK) {
			case ACTOR_SPRITE_ONE:
				record = object->records;
				record->image.pri = partyPri;
				if(syUnitGet(unitIds[unitIdx])->defendLevel == DEFEND_VANISHED)
					record->flg |= SPRITE_FLAG_OUTLINED;
				else
					record->flg &= ~SPRITE_FLAG_OUTLINED;
				break;
			case ACTOR_SPRITE_GROUP:
				records = object->records;
				for(recordIdx = 0; recordIdx < ACTOR_SPRITE_GROUP_SIZE;
					recordIdx++) {
					part = *records++;

					if(part != 0) {
						record = part;
						record->image.pri = partyPri;
					}
				}
				break;
			}
		}
	}

	for(unitIdx = partyCnt; unitIdx < partyCnt + enemyCnt; unitIdx++) {
		object = objects[unitIdx];
		if(object != 0) {
			switch(object->recordStorageKind & ACTOR_SPRITE_KIND_MASK) {
			case ACTOR_SPRITE_ONE:
				record = object->records;
				record->image.pri = enemyPri;
				if(syUnitGet(unitIds[unitIdx])->defendLevel == DEFEND_VANISHED)
					record->flg |= SPRITE_FLAG_OUTLINED;
				else
					record->flg &= ~SPRITE_FLAG_OUTLINED;
				break;
			case ACTOR_SPRITE_GROUP:
				records = object->records;
				for(recordIdx = 0; recordIdx < ACTOR_SPRITE_GROUP_SIZE;
					recordIdx++) {
					part = *records++;

					if(part != 0) {
						record = part;
						record->image.pri = enemyPri;
					}
				}
				break;
			}
		}
	}
}

/* Unused: detaches the scripts from every sprite of a unit's actor. */
void btActorDetachSprScripts(MOTION_OBJECT *object)
{
	Sint32 storageKind;
	Sint32 remaining;
	void **recordGroups;
	void  *recordGroup;

	if(object != NULL) {
		storageKind = object->recordStorageKind & ACTOR_SPRITE_KIND_MASK;
		switch(storageKind) {
		case ACTOR_SPRITE_ONE:
			btSprDetachScripts(object->records);
			return;
		case ACTOR_SPRITE_GROUP:
			recordGroups = object->records;
			for(remaining = 0; remaining < 4; remaining++) {
				recordGroup = *recordGroups++;
				if(recordGroup != NULL) {
					btSprDetachScripts(recordGroup);
				}
			}
			break;
		}
	}
}

/* Frees the enemy sprite slots a unit holds and marks its actor for release at
   the end of the action. Returns 0. */
Sint32 btObjectMarkForRelease(Sint32 objectId)
{
	BATTLE_OBJECT *slot;

	btObjectReleaseSprSlots(objectId);
	slot = btObjectFindSlot(objectId);
	slot->active = 1;
	return 0;
}

/* Releases the actor of every battle object marked for release. */
void btMotionDestroyAllSlotObjects(void)
{
	Sint32		   unitId;
	Sint32		   i;
	BATTLE_OBJECT *slot;

	for(i = 0; i < UNIT_MAX; i += 1) {
		unitId = i + FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
		if(i <= ALLY_MAX) {
			unitId = i;
		}
		slot = btObjectFindSlot(unitId);
		if((slot != NULL) && (slot->active != 0)) {
			fdActorReleaseStub((ACTOR *)slot->obj);
			slot->obj = NULL;
			slot->active = 0;
		}
	}
}

/* Unused: writes the anchor of the battle object in place `slot` to out, at
   height 0. */
Sint32 btObjectGetAnchor(Sint32 slot, POS *out)
{
	BATTLE_STATE *battle = RamHeapSlots->battleState;
	Sint32		  result;

	out->x = battle->slots[slot].anchorX;
	out->y = 0;
	result = 0;
	out->z = battle->slots[slot].anchorZ;
	return result;
}

/* Sets up the camera's view for projecting points, tilted further while
   commands are chosen. */
UNREAD_RESULT btCameraApplyTransform(void)
{
	BATTLE_CAMERA *camera = RamHeapSlots->cameraState;

	syInitMatrix();
	if(syGameFlagGet(FLAG_CHOOSING_COMMANDS) != 0) {
		syMultiMatrix(BattleCameraCommandMatrix);
		syLookAt(camera->eye, camera->focus);
	} else {
		syLookAtLoad(camera->eye, camera->focus);
	}
}

/* Projects a battle object's position to the screen, in projected. */
Sint32 btMotionProjectPos(Sint32 id, Sint32 *projected)
{
	MOTION_OBJECT *object = btObjectFindSlot(id)->obj;
	SPR			  *record = btActorGetSpr(object, 0);
	Sint32		   pos[3];
	Sint32		   scaled;

	btCameraApplyTransform();
	{
		Sint32 *out;

		pos[0] = object->x;
		out = projected;
		pos[1] = object->y;
		pos[2] = object->z;
		scaled = syConvert3Dto2D(pos, out);
	}
	(void)syMulFX(scaled, record->scale);
	return 0;
}

/* Returns sprite recordIdx of an actor (0 only, for a single sprite), or null
   when it has no such sprite. */
void *btActorGetSpr(MOTION_OBJECT *object, Sint32 recordIdx)
{
	Sint32 storageKind;

	if(object == NULL)
		return NULL;
	storageKind = object->recordStorageKind & ACTOR_SPRITE_KIND_MASK;
	if(storageKind == ACTOR_SPRITE_ONE) {
		if(recordIdx == 0) {
			return object->records;
		}
	} else if(storageKind == ACTOR_SPRITE_GROUP) {
		return ((void **)object->records)[recordIdx];
	}
	return NULL;
}

/* Puts the battle camera at its default view of the battlefield's centre. */
void btCameraSetDefaultView(void)
{
	BATTLE_CAMERA *camera = RamHeapSlots->cameraState;
	POINT		   eyeOfs;

	/* A pitch of -1/128 of a turn, 510 pixels away. */
	camera->yaw = TRIG_TURN * 3 / 16;
	camera->pitch = TRIG_TURN - TRIG_TURN / 128;
	camera->distance = BATTLE_CAMERA_DISTANCE;
	SET_VECTOR(camera->focus, 0, 0, 0);
	camera->followPos = 0;
	camera->followEye = 0;

	syInitMatrix();
	syMatrixTranslateVector(&camera->focus[0]);
	syRotY(camera->yaw);
	syRotX(camera->pitch);

	SET_VECTOR(eyeOfs, 0, 0, camera->distance);
	syCalcPoint(eyeOfs, (Sint32 *)camera);
}

/* Returns a unit's battle object, through its place, or null for a unit with no
   place. btObjectFindSlot's lookup written out: calling it compiles
   otherwise. */
BATTLE_OBJECT *btObjectGet(Sint32 objectId)
{
	BATTLE_STATE  *battle = RamHeapSlots->battleState;
	BATTLE_OBJECT *slots = battle->slots;
	Sint32		   idx;

	if(objectId > ALLY_MAX) {
		objectId -= FIRST_ENEMY_UNIT - PARTY_UNIT_COUNT;
	}
	idx = objectId;
	if(battle->placement[idx] == BATTLE_OBJECT_NONE) {
		return NULL;
	}
	return &slots[battle->placement[idx]];
}

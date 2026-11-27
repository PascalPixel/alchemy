/* A rolling fireball, which four Mars Lighthouse rooms keep a copy of. The
   includer defines ABS and its own scActorScatterCrumbs before it. */

/* Ice a rolling fireball breaks beside its path: the small crystals on the
   middle layer crack, the tall ones on the overhead layer crumble. */
enum { METATILE_BEHAVIOR_BREAKABLE_ICE = 77 };

/* A fireball rolls forward, breaking the ice in the metatiles around it, and
   carries the leader along if it hits him. Where the ground 8 pixels ahead is a
   rounded square its path ends: it crashes, calls the function its path left in
   watchedActor, then shrinks away in crumbs. */
void scActorRollAndCrumble(ACTOR *actor)
{
	VECTOR3		 pos;
	FIELD_STATE *field = RamHeapSlots->fieldState;
	Sint32		 state = actor->timer;

	if(state == 0) {
		Sint32			 dz;
		Sint32			 dx;
		Sint32			 tileX;
		MAP_METATILE	*middleMetatiles;
		MAP_METATILE	*overheadMetatiles;
		Sint32			 middleOffsetX;
		Sint32			 overheadOffsetX;
		Sint32			 offset;
		Sint32			 overheadZ;
		Sint32			 tileZ;
		Sint32			 middleOffsetZ;
		Sint32			 overheadOffsetZ;
		Sint32			 x;
		Sint32			 z;
		Sint32			 middleX;
		Sint32			 overheadX;
		Sint32			 middleZ;
		Sint32			 paused;
		MAP_STATE		*map;
		MAP_METATILE	*metatile;
		FIELD_STATE		*fieldState;
		FLAME_HIT_STATE *hit;
		ACTOR			*leader;

		paused = fdScriptGetFlag(FLAG_MENU_OPEN);
		if(field->psynergy != 0)
			paused |= 1;
		actor->paused = paused;

		pos.x = actor->x.fixed;
		pos.y = actor->y.fixed;
		pos.z = actor->z.fixed;
		syFieldVectorAddPolarOffset(8, actor->facing, &pos);
		if(fdMapGridGetTerrainKindOrBumpAt(0, pos.x, pos.z) ==
			TERRAIN_ROUNDED_SQUARE) {
			actor->targetZ = ACTOR_NO_TARGET;
			actor->targetY = ACTOR_NO_TARGET;
			actor->targetX = ACTOR_NO_TARGET;
			actor->acceleration = toFIXED(1) / 4;
			actor->timer = 1;
			fdScriptPlaySound(SE_CRASH);
			fdMapGridSetShake(5, 5, 1);
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
			if(actor->watchedActor != NULL)
				((void (*)(ACTOR *))actor->watchedActor)(actor);
		}

		map = RamHeapSlots->mapGrid;
		tileX = FIELD_METATILE(actor->x.fixed);
		tileZ = FIELD_METATILE(actor->z.fixed);
		middleMetatiles = &map->layers[MAPGRID_LAYER_MIDDLE]
							   .metatiles[tileX + tileZ * MAPGRID_SIZE];
		overheadMetatiles = &map->layers[MAPGRID_LAYER_OVERHEAD]
								 .metatiles[tileX + tileZ * MAPGRID_SIZE];
		middleOffsetX =
			FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
		middleOffsetZ =
			FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
		overheadOffsetX =
			FIELD_METATILE(map->layers[MAPGRID_LAYER_OVERHEAD].offsetX);
		overheadOffsetZ =
			FIELD_METATILE(map->layers[MAPGRID_LAYER_OVERHEAD].offsetY);

		for(dz = -1, overheadZ = tileZ + overheadOffsetZ - 2, z = tileZ - 1,
		middleZ = tileZ + middleOffsetZ - 1;
			dz <= 1; overheadZ++, z++, middleZ++, dz++) {
			for(dx = -1, overheadX = tileX + overheadOffsetX - 1, x = tileX - 1,
			middleX = tileX + middleOffsetX - 1;
				dx <= 1; overheadX++, x++, middleX++, dx++) {
				offset = ((dz << 7) + dx) * 4;
				metatile = (MAP_METATILE *)(offset + (Sint32)middleMetatiles);
				if(metatile->collisionCode == METATILE_BEHAVIOR_BREAKABLE_ICE) {
					metatile->collisionCode = METATILE_BEHAVIOR_NORMAL;
					fdMapGridCopyMetatilesLayered(
						64, 64, 1, 1, middleX, middleZ);
					fdMapGridCopyMetatileAttributes(64, 64, 1, 1, x, z);
					fdScriptPlaySound(SE_ICE_CRACK);
					scIceChipsSpawn(FIELD_METATILE_CENTER(x), actor->y.fixed,
						FIELD_METATILE_CENTER(z), actor->facing);
				}
				metatile = (MAP_METATILE *)(offset + (Sint32)overheadMetatiles);
				if(metatile->collisionCode == METATILE_BEHAVIOR_BREAKABLE_ICE) {
					metatile->collisionCode = METATILE_BEHAVIOR_NORMAL;
					fdMapGridCopyMetatilesLayered(
						64, 64, 1, 2, overheadX, overheadZ);
					fdMapGridCopyMetatileAttributes(64, 64, 1, 1, x, z);
					fdScriptPlaySound(SE_CRUMBLE);
					scIceChipsSpawn(FIELD_METATILE_CENTER(x), actor->y.fixed,
						FIELD_METATILE_CENTER(z), actor->facing);
				}
			}
		}

		if(actor->scaleX < toFIXED(1)) {
			actor->scaleX += toFIXED(1) / 16;
			actor->scaleY += toFIXED(1) / 16;
		}
		scFieldActorSetMoveTargetFX(actor, pos.x, pos.y, pos.z);

		fieldState = RamHeapSlots->fieldState;
		hit = FLAME_THROWERS_STATE;
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		if(ABS(actor->x.fixed - leader->x.fixed) < toFIXED(20) &&
			(Uint32)(actor->y.fixed - leader->y.fixed + (toFIXED(8) - 1)) <=
				(toFIXED(32) - 2) &&
			ABS(actor->z.fixed - leader->z.fixed) < toFIXED(20)) {
			/* A fireball's hit (a flame's is 1) raises
			   STEP_CUE_FIREBALL_HIT. */
			hit->active = 2;
			hit->car = actor;
			fieldState->stepCue =
				hit->hitCue + (STEP_CUE_FIREBALL_HIT - STEP_CUE_FLAME_HIT);
		}
	} else if(state == 1) {
		actor->paused = 0;
		if(actor->scaleX > 0) {
			actor->scaleX -= toFIXED(1) / 16;
			actor->scaleY -= toFIXED(1) / 16;
			scActorScatterCrumbs(actor);
		} else {
			actor->z.fixed = 0;
			actor->y.fixed = 0;
			actor->x.fixed = 0;
			actor->velocityZ = 0;
			actor->velocityY = 0;
			actor->velocityX = 0;
			actor->update = NULL;
		}
	}
}

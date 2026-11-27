/* Scoop: the giant hand digging, the actor dropped from it and the item dug up.
   Part of field_psynergy.c. */

/* A dig at nothing draws the first SCOOP_DRAWN_DUST of the particles of dust
   and sparkles Scoop throws up, a dig on a target the first
   SCOOP_DRAWN_TARGET_DUST. */
#define SCOOP_DRAWN_DUST		18
#define SCOOP_DRAWN_TARGET_DUST 20
/* The sparkles the hand bursts into when it cannot dig. */
#define SCOOP_SPARKLES			16
/* A dig at nothing turns something up when syRandom() falls below this, one
   time in sixteen; what turns up is an item below SCOOP_ITEM_CHANCE, three
   times in four, and coins otherwise. */
#define SCOOP_FIND_CHANCE		(toFIXED(1) / 16)
#define SCOOP_ITEM_CHANCE		(toFIXED(3) / 4)

/* The kinds of dig (SCOOP_STATE.mode). */
typedef enum SCOOP_MODE {
	SCOOP_EARTH = 0,  /* at nothing: earth is thrown up */
	SCOOP_FINDS = 1,  /* at nothing, one time in sixteen: something turns up */
	SCOOP_TARGET = 2, /* on a target: the place's Scoop script runs */
	SCOOP_SPARKLE = 3 /* at nothing where lit: sparkles rise */
} SCOOP_MODE;

/* The stages of a dig (SCOOP_STATE.phase). */
typedef enum SCOOP_PHASE {
	SCOOP_DIGS = 0,
	SCOOP_HAND_RISES = 1,
	SCOOP_SETTLES = 2 /* what was dug up shows, then the dig ends */
} SCOOP_PHASE;

/* Why the hand cannot dig. */
typedef enum SCOOP_FAILURE {
	SCOOP_CAN_DIG = 0,
	SCOOP_ACTOR_IN_WAY = 1,
	SCOOP_OUT_OF_REACH = 2,
	SCOOP_HARD_GROUND = 3,
	SCOOP_WALL = 4
} SCOOP_FAILURE;

/* Moves the state on to its next phase, its timer restarted. */
#define NEXT_PHASE(state) ((state)->phase++, (state)->timer = -1)

/* Event of the Scoop: the hand digs and rises, then the earth, sparkles or dust
   it threw up show, each kind of dig at its own pace. */
void fdScoopUpdate(void)
{
	SCOOP_STATE				*state;
	ACTOR					*hand;
	EFFECT_SPR				*sprite;
	FIELD_PSYNERGY_PARTICLE *particle;
	Uint8					*vram;
	Uint8					*picture;
	Sint16					*phase;
	Sint32					 jitter;
	Sint32					 i;

	state = RamHeapSlots->battleEffect;
	hand = state->actor;
	if(state->mode == SCOOP_EARTH) {
		switch(state->phase) {
		case SCOOP_DIGS:
			if(state->timer == 1)
				state->other->scaleX = toFIXED(1);
			if(state->timer == 4)
				NEXT_PHASE(state);
			break;
		case SCOOP_HAND_RISES:
			if(state->timer == 8)
				fdActorSelectAnimationStub(hand, 3);
			if(state->timer <= 15)
				hand->y.fixed += toFIXED(23) / 10;
			if(state->timer == 28)
				NEXT_PHASE(state);
			break;
		case SCOOP_SETTLES:
			if(state->timer == 25)
				state->other->scaleX = 0;
			if(state->timer == 50)
				state->phase = FIELD_PSYNERGY_FINISHED;
			break;
		}
		phase = &state->phase;
		if(*phase == SCOOP_SETTLES) {
			picture = state->picture;
			vram = (Uint8 *)OBJ_VRAM0 + state->pictureTile * TILE_SIZE_4BPP;
			fdPictureShiftColumnsByWave(
				state->tiles, picture, state->timer + 14);
			((const Uint8 *(*)(const Uint8 *, Sint32, Sint32,
				void *))RamHeapSlots->pictureUploader)(
				picture, SCOOP_PICTURE_SIZE, SCOOP_PICTURE_SIZE, vram);
			sprite = &state->pictureSprite;
			fdEffectSpriteDraw(sprite);
		}
		if(*phase == SCOOP_SETTLES) {
			for(i = 0, particle = state->particles; i < SCOOP_DRAWN_DUST;
				i++, particle++) {
				sprite = &state->sprites[i];
				if(particle->timer >= 0 && particle->timer < 32)
					fdScoopDustUpdateGrain(
						sprite, particle, state->particleTile);
				particle->timer++;
			}
		}
	} else if(state->mode == SCOOP_SPARKLE) {
		switch(state->phase) {
		case SCOOP_DIGS:
			if(state->timer == 1)
				state->other->scaleX = toFIXED(1);
			if(state->timer == 4)
				NEXT_PHASE(state);
			break;
		case SCOOP_HAND_RISES:
			if(state->timer == 8)
				fdActorSelectAnimationStub(state->actor, 3);
			if(state->timer <= 15)
				hand->y.fixed += toFIXED(23) / 10;
			if(state->timer == 28)
				NEXT_PHASE(state);
			break;
		case SCOOP_SETTLES:
			if(state->timer == 70)
				state->other->scaleX = 0;
			if(state->timer == 90)
				state->phase = FIELD_PSYNERGY_FINISHED;
			break;
		}
		if(state->phase == SCOOP_SETTLES) {
			for(i = 0, particle = state->particles; i < SCOOP_PARTICLE_COUNT;
				i++, particle++) {
				if(particle->timer >= 0 && particle->timer < 45) {
					sprite = &state->sprites[i];
					fdEffectSpriteDrawAtWorldPos(sprite, particle);
					if(particle->timer < 42) {
						particle->y -= toFIXED(1);
						jitter = syRandom();
						particle->x += jitter - (Sint32)syRandom();
					}
				}
				particle->timer++;
			}
		}
	} else if(state->mode == SCOOP_FINDS) {
		switch(state->phase) {
		case SCOOP_DIGS:
			if(state->timer == 8)
				NEXT_PHASE(state);
			break;
		case SCOOP_HAND_RISES:
			if(state->timer == 0)
				fdActorSelectAnimationStub(state->actor, 4);
			hand->y.fixed += toFIXED(23) / 10;
			if(state->timer == 15)
				NEXT_PHASE(state);
			break;
		case SCOOP_SETTLES:
			if(state->timer == 20)
				state->phase = FIELD_PSYNERGY_FINISHED;
			break;
		}
	} else {
		switch(state->phase) {
		case SCOOP_DIGS:
			if(state->timer == 8)
				NEXT_PHASE(state);
			break;
		case SCOOP_HAND_RISES:
			if(state->timer == 0)
				fdActorSelectAnimationStub(state->actor, 4);
			hand->y.fixed += toFIXED(23) / 10;
			if(state->timer == 15)
				NEXT_PHASE(state);
			break;
		case SCOOP_SETTLES:
			if(state->timer == 30)
				state->phase = FIELD_PSYNERGY_FINISHED;
			break;
		}
		particle = state->particles;
		sprite = state->sprites;
		for(i = 0; i < SCOOP_DRAWN_TARGET_DUST; i++, sprite++, particle++) {
			if(particle->timer >= 0 && particle->timer < 32)
				fdScoopDustUpdateGrain(sprite, particle, state->particleTile);
			particle->timer++;
		}
	}
	state->timer++;
}

/* Scoop cast on a target: runs fdPsynergyScoop. */
void fdPsynergyScoopOnTarget(void)
{
	fdPsynergyScoop();
}

Sint32 fdMapGridCanScoop(Sint32 x, Sint32 z, Sint32 layer);
void   fdScoopDigUpItem(void);

/* Scoop ("Dig in soft ground."): a giant hand rises before the caster and digs
   the metatile 16 pixels ahead. With an actor in the way, out of reach or on
   ground that cannot be dug it stops with the error sound; else earth (or, in a
   lit place, sparkles) is thrown up and the place's Scoop script runs. */
void fdPsynergyScoop(void)
{
	SCOOP_STATE *state =
		syHeapSlotAllocate(HEAP_SLOT_BATTLE_EFFECT, sizeof(*state));
	FIELD_EFFECT_STATE		*scene = RamHeapSlots->fieldEffectState;
	FIELD_STATE				*field = RamHeapSlots->fieldState;
	ACTOR					*caster = scene->caster;
	ACTOR					*boulder = NULL;
	ACTOR					*hand;
	ACTOR					*target;
	EFFECT_SPR				*sprite;
	FIELD_PSYNERGY_PARTICLE *particle;
	void					*pending;
	void					*data;
	Sint32					 particleEntry = VRAM_CACHE_ENTRY_COUNT;
	Sint32					 particleTile;
	Sint32					 pictureEntry = VRAM_CACHE_ENTRY_COUNT;
	Sint32					 pictureTile;
	FIXED					 pos[3];
	Sint32					 failed;
	Sint8					*lit;
	Sint32					 stage;
	Sint32					 frame;
	Sint32					 i;

	if(field->mode != ACTOR_SYSTEM_WORLD_MAP) {
		SET_POSITION(scene,
			(caster->x.fixed & FIELD_METATILE_MASK) + toFIXED(8),
			caster->y.fixed,
			(caster->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
		syVectorAddPolarOffset(toFIXED(16), scene->angle, &scene->x);
	}
	fdPsynergyFadeToCastColor();
	hand = fdActorSpawn(SPRITE_SCOOP_HAND, 0, 0, 0);
	state->actor = hand;
	if(hand != NULL) {
		hand->motionFlags = 0;
		if(field->psynergyDrawFlags & PSYNERGY_DRAW_FIXED_PRIORITY) {
			if((hand->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) ==
				ACTOR_SPRITE_ONE) {
				((EFFECT_SPR *)hand->sprite)->oam.priority = 1;
				hand->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
			}
		} else {
			hand->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
		}
		hand->facing = caster->facing;
		fdActorSelectAnimationStub(hand, 0);
		lit = &scene->lit;
		if(*lit != 0)
			fdActorSetSpriteColorModeStub(hand, SPRITE_COLOR_GREY);
		if(*lit == 0)
			boulder = fdActorSpawn(
				SPRITE_BROWN_BOULDER, scene->x, scene->y, scene->z);
		else
			boulder =
				fdActorSpawn(SPRITE_GREY_BOULDER, scene->x, scene->y, scene->z);
		state->other = boulder;
		if(boulder != NULL) {
			boulder->z.fixed += toFIXED(3);
			boulder->motionFlags = 0;
			boulder->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
			boulder->scaleX = 0;
			if(!(field->psynergyDrawFlags & PSYNERGY_DRAW_NO_SCENE_SPRITE))
				fdPsynergyStartSceneSprite();
			sySoundPlayStub(SE_ITEM_BREAK);
			fdPsynergyGrowFromCaster(
				hand, scene->x, scene->y + toFIXED(32), scene->z);
			sySynch(10);
			fdActorSelectAnimationStub(hand, 1);
			sySynch(15);
			for(i = 0; i < 10; i++) {
				hand->y.fixed += -(toFIXED(5) / 2);
				sySynch(1);
			}
			fdActorSelectAnimationStub(hand, 2);
			sySoundPlayStub(SE_WATER);

			failed = SCOOP_CAN_DIG;
			for(i = 0; i < FIELD_ACTOR_COUNT; i++) {
				target = field->actors[i];
				if(target != NULL && target->script != NULL &&
					(fdActorGetSpriteIdStub(target) == SPRITE_STAKE ||
						(target->collisionFlags & ACTOR_SOLID)) &&
					target != scene->caster &&
					((SPR *)target->sprite)->count != 0 &&
					fdPsynergyCheckOverlap(&scene->x, 4, &target->x.fixed,
						target->radius - 2) >= 0) {
					failed = SCOOP_ACTOR_IN_WAY;
					break;
				}
			}
			if(fdMapGridGetMetatileBehavior(
				   scene->x, scene->z, caster->layer) == METATILE_BEHAVIOR_WALL)
				failed = SCOOP_WALL;
			if(fdMeasurePsynergyHeight(
				   scene->fieldPsynergy, caster, &scene->x) == -1)
				failed = SCOOP_OUT_OF_REACH;
			if(fdMapGridCanScoop(scene->x, scene->z, caster->layer) == 0)
				failed = SCOOP_HARD_GROUND;
			if(failed) {
				sySynch(2);
				for(i = 0; i < SCOOP_SPARKLES; i++)
					fdBattleEffectSpawnScatterParticle(
						hand->x.fixed, hand->y.fixed, hand->z.fixed);
				sySoundPlayStub(SE_MENU_ERROR);
				sySynch(1);
			} else {

				if(scene->aimAhead != 0) {
					if(syRandom() < SCOOP_FIND_CHANCE)
						state->mode = SCOOP_FINDS;
					else if(*lit == 0)
						state->mode = SCOOP_EARTH;
					else
						state->mode = SCOOP_SPARKLE;
				} else {
					state->mode = SCOOP_TARGET;
				}

				if(state->mode == SCOOP_EARTH) {
					syDecompressTaggedLZ(
						syResourceGetTableEntry(RESOURCE_DUST_PUFF_A), state);
					particleEntry = syVRAMCacheFindFreeEntry();
					particleTile = syVRAMCacheLoad(particleEntry, 2048, state);
					state->particleTile = particleTile;
					i = 0;
					particle = state->particles;
					for(; i < SCOOP_PARTICLE_COUNT; i++, particle++) {
						sprite = &state->sprites[i];
						fdEffectSpriteInit(
							sprite, 8, 8, OBJ_SIZE_16X16, particleTile);
						sprite->oam.fullColor = 1;
						sprite->oam.palette = 0;
						sprite->oam.priority =
							fdActorGetSpritePriority(boulder);
						sprite->depth = fdActorGetSpriteDepth(boulder) + 1;
						sprite->scaleY = sprite->scaleX = toFIXED(1) / 5;
						SET_POSITION(particle, scene->x, scene->y, scene->z);
						syVectorAddPolarOffset(
							toFIXED(2), syRandom(), &particle->x);
						particle->velocityX = 0;
						particle->velocityY = 0;
						particle->velocityZ = 0;
						syVectorAddPolarOffset(
							toFIXED(1.2), syRandom(), &particle->velocityX);
						particle->timer = -i * 2;
					}
				} else if(state->mode == SCOOP_TARGET) {
					if(*lit == 0)
						data = syResourceGetTableEntry(RESOURCE_DUST_PUFF_A);
					else
						data = syResourceGetTableEntry(RESOURCE_DUST_PUFF_B);
					syDecompressTaggedLZ(data, state);
					particleEntry = syVRAMCacheFindFreeEntry();
					particleTile = syVRAMCacheLoad(particleEntry, 2048, state);
					state->particleTile = particleTile;
					i = 0;
					particle = state->particles;
					for(; i < SCOOP_PARTICLE_COUNT; i++, particle++) {
						sprite = &state->sprites[i];
						fdEffectSpriteInit(
							sprite, 8, 8, OBJ_SIZE_16X16, particleTile);
						sprite->oam.fullColor = 1;
						sprite->oam.palette = 0;
						sprite->oam.priority =
							fdActorGetSpritePriority(boulder);
						sprite->depth = fdActorGetSpriteDepth(boulder) + 1;
						SET_POSITION(particle, scene->x, scene->y, scene->z);
						syVectorAddPolarOffset(
							toFIXED(2), syRandom(), &particle->x);
						particle->velocityX = 0;
						particle->velocityY = 0;
						particle->velocityZ = 0;
						syVectorAddPolarOffset(
							toFIXED(1), syRandom(), &particle->velocityX);
						particle->timer = -i;
					}
				} else if(state->mode == SCOOP_SPARKLE) {
					syDecompressTaggedLZ(
						syResourceGetTableEntry(RESOURCE_WHITE_SPARKLE), state);
					particleEntry = syVRAMCacheFindFreeEntry();
					particleTile = syVRAMCacheLoad(particleEntry, 128, state);
					state->particleTile = particleTile;
					i = 0;
					particle = state->particles;
					sprite = state->sprites;
					for(; i < SCOOP_PARTICLE_COUNT; i++) {
						fdEffectSpriteInit(
							sprite, 4, 4, 0, particleTile + (i & 1) * 2);
						sprite->oam.fullColor = 1;
						sprite->oam.palette = 0;
						sprite->oam.priority = fdActorGetSpritePriority(hand);
						sprite->depth = fdActorGetSpriteDepth(hand) - 1;
						SET_POSITION(particle, scene->x, scene->y + toFIXED(40),
							scene->z);
						syVectorAddPolarOffset(syRandom() * 4 + toFIXED(3),
							syRandom(), &particle->x);
						particle->timer = -i * 3 / 4;
						sprite++;
						particle++;
					}
				}

				if(state->mode == SCOOP_EARTH) {
					syDecompressTaggedLZ(
						syResourceGetTableEntry(RESOURCE_SCOOP_EARTH), state);
					pictureEntry = syVRAMCacheFindFreeEntry();
					pictureTile = syVRAMCacheLoad(pictureEntry, 1024, NULL);
					state->pictureTile = pictureTile;
					sprite = &state->pictureSprite;
					fdEffectSpriteInit(
						sprite, 16, 0, OBJ_SIZE_32X32, pictureTile);
					sprite->depth = fdActorGetSpriteDepth(scene->caster);
					sprite->oam.priority =
						fdActorGetSpritePriority(scene->caster);
					sprite->oam.fullColor = 1;
					sprite->oam.palette = 0;
					pos[0] = hand->x.fixed;
					pos[1] = hand->y.fixed + toFIXED(32);
					pos[2] = hand->z.fixed;
					fdCameraWorldToScreen(pos);
					sprite->x = pos[0];
					sprite->y = pos[2];
				}
				if(state->mode == SCOOP_EARTH || state->mode == SCOOP_SPARKLE) {
					sySynch(6);
					state->other->scaleX = toFIXED(1);
					sySynch(4);
				} else {
					sySynch(10);
				}

				state->phase = SCOOP_DIGS;
				state->timer = 0;
				{
					Sint32 order = EVENT_PASS_TICK;

					sySetEvent(fdScoopUpdate, order);
				}
				if(state->mode == SCOOP_FINDS) {
					frame = 0;
					while(state->phase != FIELD_PSYNERGY_FINISHED) {
						if(frame == 24)
							fdScoopDigUpItem();
						sySynch(1);
						frame++;
					}
				} else {
					pending = fdFindPsynergyCue(
						PSYNERGY_CUE(PSYNERGY_PHASE_ACTS), scene->fieldPsynergy,
						fdSelectPsynergyTarget(scene->fieldPsynergy));
					frame = 0;
					while(state->phase != FIELD_PSYNERGY_FINISHED) {
						stage = 0;
						sySynch(1);
						if(frame == SCOOP_DUG_TICK)
							stage = SCOOP_STAGE_DUG;
						if(frame == 50)
							stage = SCOOP_STAGE_DUG_UP;
						scene->stage = stage;
						scene->frame = frame;
						fdRunPsynergyScript(pending, stage, frame);
						frame++;
					}
					fdRunPsynergyScript(
						pending, FIELD_PSYNERGY_FINISHED, frame);
				}
				syCloseEvent(fdScoopUpdate);
				sySoundPlayStub(SE_MENU_ERROR);
				fdPsynergyShrinkToCaster(
					hand, hand->x.fixed, hand->y.fixed, hand->z.fixed);
				if(particleEntry != VRAM_CACHE_ENTRY_COUNT)
					syVRAMCacheFreeEntry(particleEntry);
				if(pictureEntry != VRAM_CACHE_ENTRY_COUNT)
					syVRAMCacheFreeEntry(pictureEntry);
			}
		}
	}
	fdActorReleaseStub(hand);
	fdActorReleaseStub(boulder);
	fdPsynergyFadeFromCastColor();
	syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
}

/* Drops an actor from Scoop's hand (if there is one) to fall to the ground,
   waits 50 ticks, then asks for its talk, which opens it without its "checked
   the ..." message (FIELD_TALK_SILENT). */
void fdScoopDropFromHand(Sint32 id)
{
	FIELD_STATE *field = RamHeapSlots->fieldState;
	ACTOR		*hand;
	ACTOR		*actor;

	hand = ((SCOOP_STATE *)RamHeapSlots->battleEffect)->actor;
	actor = fdActorGetById(id);
	if(hand != NULL) {
		SET_ACTOR_POSITION(actor, hand->x.fixed, hand->y.fixed, hand->z.fixed);
	}
	if(field->mode == ACTOR_SYSTEM_WORLD_MAP) {
		actor->motionFlags = ACTOR_FALLS;
		actor->groundHeight = 0;
	} else {
		actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	}
	actor->gravity = toFIXED(9) / 100;
	sySynch(50);
	field->talk = id | FIELD_TALK_SILENT;
}

extern const Uint16 ScoopDugItems[16];

/* SCOOPED_ITEM_ACTOR at the hand, fetched or spawned (SPRITE_UNSEEN_MARKER),
   shows what Scoop dug up: three times in four one of ScoopDugItems's sixteen
   items (a coin when there is no room for it), else coins up to twice the
   leader's level; it then drops (fdScoopDropFromHand). */
void fdScoopDigUpItem(void)
{
	SCOOP_STATE *state = RamHeapSlots->battleEffect;
	ACTOR		*hand;
	FIELD_STATE *field;
	UNIT		*unit;
	ACTOR		*actor;
	Sint32		 item;
	Sint32		 id = SCOOPED_ITEM_ACTOR;

	hand = state->actor;
	field = RamHeapSlots->fieldState;
	unit = syUnitGet(MainRoutineStateWork.leaderActorId);

	actor = fdActorGetById(id);
	if(actor == NULL) {
		actor = fdActorCreateStub(
			SPRITE_UNSEEN_MARKER, hand->x.fixed, hand->y.fixed, hand->z.fixed);
		if(actor == NULL)
			return;
		field->actors[id] = actor;
	}
	if(syRandom() < SCOOP_ITEM_CHANCE) {
		item = ScoopDugItems[syRandomBelow(ARRAY_COUNT(ScoopDugItems))];
		if(gmPartyInventoryFindRoomStub(item) == -1)
			item = SCOOPED_COINS | 1;
	} else {
		item = (syRandomBelow(unit->level * 2) + 1) | SCOOPED_COINS;
	}
	SET_ACTOR_POSITION(actor, hand->x.fixed, hand->y.fixed, hand->z.fixed);
	fdActorShowItemIconById(id, item);
	field->scoopedItem = item;
	fdScoopDropFromHand(id);
}

#undef NEXT_PHASE
#undef SCOOP_DRAWN_DUST
#undef SCOOP_DRAWN_TARGET_DUST
#undef SCOOP_PICTURE_SIZE
#undef SCOOP_SPARKLES
#undef SCOOP_FIND_CHANCE
#undef SCOOP_ITEM_CHANCE

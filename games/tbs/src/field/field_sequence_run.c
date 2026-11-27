/* @file */
/* The field sequence: running a scene until the party leaves it, and its */
/* entrance. */
/* Part of field_sequence.c. */

#include "field_sequence_internal.h"

/* The hazard count's limit each scene starts with, in steps on hazard cells: */
/* the hazard deals its damage each time the count reaches it. */
#define HAZARD_LIMIT_DEFAULT 16
/* FIELD_STATE.talk's request bit, set above the actor id so that a talk to */
/* actor 0 is still a request (The Lost Age sets bit 14). */
#define FIELD_TALK_REQUEST (1 << 12)
/* Isaac's pose when the whole party has fallen: lying flat, the picture */
/* ANIM_LEADER_FALL ends on (isaac.tsv); any other leader plays ANIM_COLLAPSE.
 */
#define ANIM_LEADER_LIE_FLAT 37
/* Isaac's pose for the same as a child (FLAG_CHILDHOOD): standing feet apart,
 */
/* the picture isaac_child.tsv's ANIM_LEADER_STRIDE holds. */
#define ISAAC_CHILD_ANIMATION_STAND_BRACED 21
/* The sprite VRAM blocks the Pause menu needs free, or the field's sprites */
/* stop drawing while it runs. */
#define PAUSE_MENU_VRAM_BLOCKS 136

/* The field sequence: runs the current scene, which the party entered by */
/* entrance, until it leaves; returns the exit kind it left by, for */
/* scTakeExit, or EXIT_KEEP_DESTINATION when the next scene is already in */
/* MainRoutineStateWork: a random battle, a party that fell outside battle, */
/* Retreat or an event's destination. */
s32 fdSequenceRun(s32 entrance) {
	FIELD_STATE *state;
	ACTOR *actor;
	s32 mode;
	s32 result = 0;
	s32 code;

	/* Set up the scene. */
	state = syHeapSlotAllocEwram(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	btDmaFill32(0, state, sizeof(FIELD_STATE));
	gmFlagClearStub(FLAG_IN_BATTLE);
	MainRoutineStateWork.returnScene = MainRoutineStateWork.next;
	MainRoutineStateWork.returnEntrance = MainRoutineStateWork.entrance;
	MainRoutineStateWork.afterBattleScene = -1;
	MainRoutineStateWork.afterBattleEntrance = -1;
	MainRoutineStateWork.defeatScene = -1;
	MainRoutineStateWork.defeatEntrance = -1;
	syInitEvent();
	fdDjinnResolvePending(0);
	if(MainRoutineStateWork.mapId == MAP_INDEX_WORLD) {
		scWorldMapInitPerspectiveStub();
		MainRoutineStateWork.perspectiveMap = 1;
		mode = ACTOR_SYSTEM_WORLD_MAP;
		fdEncounterSelectMapZones(0);
	}
	else {
		fdMapGridLoadMapStub(MainRoutineStateWork.mapId);
		mode = ACTOR_SYSTEM_MAP;
		fdEncounterSelectMapZones(1);
	}
	state->mode = mode;
	fdActorSystemInitStub(mode);
	mnWindowSystemInitFieldStub();
	fdClearRequests();
	state->triggers = OverlayArea.triggers();
	scApplyEntrance();
	fdActorResetForLeader((ACTOR_PLACE *)OverlayArea.actorPlacements());
	if(gmFlagGetStub(FLAG_KEEP_PARTY_POSITION) != 0)
		fdActorRestore();
	if(MainRoutineStateWork.pendingDjinni != 0)
		fdDjinnRestoreActorAfterBattle();
	if(mode == ACTOR_SYSTEM_WORLD_MAP) {
		scWorldMapUpdateCurrentTileBlockStub();
		scWorldMapLoadGraphicsStub(MainRoutineStateWork.x,
								   MainRoutineStateWork.z);
	}
	else {
		fdMapGridCenterOnFollowedPointStub();
	}
	fdPlaceTreasureSpots();
	fdPaletteFadeInit();
	fdPaletteFadeSetTarget(PALETTE_TINT_NONE, 0);
	/* The default fade is from the backdrop colour over 16 frames; the scene's
	 */
	/* initialization may choose another. */
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = MAP_TRANSITION_FRAMES;
	state->transitionActive = 0;
	/* fdUpdateWalking scales the leader's speed by this (16.16) into the */
	/* distance counted towards PP, Djinn recovery and poison: FIXED_ONE / 160
	 */
	/* counts each of them on by one about every 160 pixels walked. */
	state->walkingDistanceScale = FIXED_ONE / 160;
	state->walkingDistance = 0;
	if(MainRoutineStateWork.sceneType == SCENE_TYPE_TOWN) {
		state->walkingDistanceScale = 0;
		gmFlagSetStub(FLAG_RETREAT_BLOCKED);
	}
	state->prevCell = (MAP_GRID_CELL *)Ram_MapGridCellBuffer;
	/* A fresh entry (not a return from battle) resets the hazard count to its
	 */
	/* defaults (damage 1 every HAZARD_LIMIT_DEFAULT steps), the stopped actor
	 * and */
	/* Cloak. */
	if(gmFlagGetStub(FLAG_KEEP_PARTY_POSITION) == 0) {
		scSetRestartPoint();
		MainRoutineStateWork.hazardLimit = HAZARD_LIMIT_DEFAULT;
		MainRoutineStateWork.hazardMode = HAZARD_COUNT_ON_CELLS;
		MainRoutineStateWork.hazardDamage = 1;
		MainRoutineStateWork.haltedActor = -1;
		MainRoutineStateWork.cloaked = 0;
	}
	fdPsynergyResumeCloakCheck();
	state->queuedSnd = -1;
	OverlayArea.initialize();

	/* The scene's initialization may already have taken an exit. */
	code = state->exit;
	if(code != 0) {
		result = code;
		state->exit = 0;
	}
	else {
		gmFlagClearStub(FLAG_KEEP_PARTY_POSITION);
		/* Fade in, show the area name and give what was left pending. */
		if(fdCountRequests() == 0) {
			if(state->transitionActive == 0) {
				fdScrTransitionStart(state->transition, state->transitionFrm);
				state->transitionActive = 1;
				PLTT_BACKDROP = 0;
				/* An arrival the last scene asked for (fdEntrancePlayArrival)
				 * plays */
				/* through the fade; with none, wait out half of it. */
				if(fdEntrancePlayArrival() == 0)
					sySynch((state->transitionFrm + 1) / 2);
			}
			if(gmFlagGetStub(FLAG_SHOW_AREA_NAME) != 0) {
				gmFlagClearStub(FLAG_SHOW_AREA_NAME);
				mnAreaNameWindowShowStub(MainRoutineStateWork.next);
			}
			if(MainRoutineStateWork.pendingDjinni != 0) {
				fdDjinnResolvePending(MainRoutineStateWork.pendingDjinni, 1);
				MainRoutineStateWork.pendingDjinni = 0;
			}
			if(MainRoutineStateWork.pendingItem != 0) {
				fdWorldMapFreeze();
				if(fdPartyCheckItemLimit(MainRoutineStateWork.pendingItem) == 0)
					fdPartyInventoryGiveItem(MainRoutineStateWork.pendingItem,
											 0);
				fdWorldMapUnfreeze();
				MainRoutineStateWork.pendingItem = 0;
			}
		}

		for(;;) {
			/* Note where the leader is, for the requests to use. */
			gmFlagSetStub(FLAG_FIELD_HANDLING_REQUESTS);
			actor = state->actors[MainRoutineStateWork.leaderActorId];
			fdActorStartIdleStub(actor);
			MainRoutineStateWork.x = actor->x.fixed;
			MainRoutineStateWork.y = 0;
			MainRoutineStateWork.z = actor->z.fixed;
			MainRoutineStateWork.heading = actor->facing;
			MainRoutineStateWork.layer = actor->layer;

			/* Handle the pending requests, most urgent first. */
			while(fdCountRequests() != 0) {
				s32 next = state->exit;

				if(next != 0) {
					result = next;
					state->exit = 0;
					goto done;
				}
				if(state->raisedTrigger != 0) {
					state->busy = 1;
					if(state->raisedTrigger ==
					   FIELD_RAISED_TRIGGER_PARTY_FELL) {
						/* Characters fell to poison or hazardous ground. */
						u32 i;

						fdLeaderStopMoving();
						fdWorldMapFreeze();
						for(i = 0; i < state->fallenCnt; i++) {
							fdActorSelectAnimationStub(actor,
													   ANIM_LEADER_LEAN_BACK);
							mnMessageArgumentPushStub(state->fallenOwners[i],
													  MESSAGE_ARGUMENT_ACTOR);
							/* "[member]'s strength is exhausted..." */
							mnFieldMessageShowStub(MsgFieldStrengthExhausted,
												   FIELD_MESSAGE_ZOOM);
						}
						if(state->survivorCnt == 0) {
							/* The whole party fell: back to the last sanctum
							 * with 1 HP. */
							UNIT *vitals;

							if(MainRoutineStateWork.leaderActorId == 0) {
								if(gmFlagGetStub(FLAG_CHILDHOOD) != 0)
									fdActorSelectAnimationStub(
										actor,
										ISAAC_CHILD_ANIMATION_STAND_BRACED);
								else
									fdActorSelectAnimationStub(
										actor, ANIM_LEADER_LIE_FLAT);
							}
							else {
								fdActorSelectAnimationStub(actor,
														   ANIM_COLLAPSE);
							}
							sySndPlayStub(MUS_DEFEATED);
							/* "Isaac's party has been annihilated..." */
							mnFieldMessageShowStub(MsgFieldPartyAnnihilated,
												   FIELD_MESSAGE_ZOOM);
							vitals = gmUnitGetStub(
								MainRoutineStateWork.leaderActorId);
							vitals->hp = 1;
							gmUnitUpdateGauges(vitals);
							MainRoutineStateWork.next =
								MainRoutineStateWork.restartScene;
							MainRoutineStateWork.entrance =
								MainRoutineStateWork.restartEntrance;
							fdWorldMapUnfreeze();
							result = EXIT_KEEP_DESTINATION;
							goto done;
						}
						fdWorldMapUnfreeze();
					}
					else if(state->raisedTrigger ==
							FIELD_RAISED_TRIGGER_MAP_SCREEN) {
						fdLeaderStopMoving();
						fdMapScrShow(RESOURCE_WORLD_MAP_PICTURE);
					}
					else if(state->raisedTrigger ==
							FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM) {
						fdLeaderStopMoving();
						fdWorldMapZoomOutWhileLHeld();
					}
					else {
						scRunRaisedTrigger(state->raisedTrigger);
					}
					state->busy = 0;
					state->raisedTrigger = 0;
				}
				else if(state->encounter != 0) {
					/* A random encounter: save the actors and go to the battle.
					 */
					fdLeaderStopMoving();
					fdActorSnapshot();
					MainRoutineStateWork.returnMusic = -1;
					MainRoutineStateWork.next = SEQUENCE_BATTLE;
					MainRoutineStateWork.entrance = state->encounter;
					state->exit = EXIT_KEEP_DESTINATION;
					fdStartEncounterTransition(state->encounter);
					state->encounterBias = 0;
					state->encounter = 0;
					MainRoutineStateWork.encounterSteps = 0;
				}
				else if(state->stepTrigger != 0) {
					state->busy = 1;
					scRunStepTriggerOrExit(state->stepTrigger);
					state->busy = 0;
					state->stepTrigger = 0;
				}
				else if(state->pushCell != 0) {
					fdRunPushCell(state->pushCell);
					state->pushCell = 0;
				}
				else if(state->aButton != 0) {
					/* Examine the actor or cell ahead, or open the menu if
					 * neither */
					/* has an event. */
					s32 object;

					mnAreaNameWindowHideStub();
					object = fdFindNearbyTalkTarget(
						MainRoutineStateWork.leaderActorId);
					/* mode, done with, holds whether there is something to talk
					 * to, then */
					/* the cell behavior ahead. */
					mode = 0;
					if(object != -1) {
						mode = scFindActorTalkTrigger(object) != 0;
					}
					if(mode != 0) {
						state->talk = object | FIELD_TALK_REQUEST;
						state->fieldMenu = 0;
					}
					else {
						mode = fdActorGetTriggerCellAheadOfCurrent();
						if(mode != 0) {
							state->examineCell = mode;
							state->fieldMenu = 0;
						}
						else {
							state->fieldMenu = 1;
						}
					}
					state->aButton = 0;
				}
				else if(state->fieldMenu != 0) {
					/* The field menu (in debug mode, B + Select opens the debug
					 * flag */
					/* editor). */
					mnAreaNameWindowHideStub();
					fdLeaderStopMoving();
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					fdWorldMapFreeze();
					gmFlagSetStub(FLAG_MENU_OPEN);
					if(DebugMode != 0 && (KeysHeld & KEY_B) != 0 &&
					   (KeysHeld & KEY_SELECT) != 0) {
						mnDebugFlagEditorRunStub();
					}
					else if(gmFlagGetStub(FLAG_MENUS_RAISE_TRIGGER) != 0) {
						state->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
					}
					else {
						state->effTmrRunning = 0;
						mnFieldMenuRunStub();
						state->effTmrRunning = 1;
					}
					fdWorldMapUnfreeze();
					gmFlagClearStub(FLAG_MENU_OPEN);
					gmFlagRefreshLureCapStub();
					state->fieldMenu = 0;
				}
				else if((s16)state->talk != 0) {
					fdWorldMapFreeze();
					scTalkToActor(state->talk & (FIELD_TALK_REQUEST - 1));
					fdWorldMapUnfreeze();
					state->talk = 0;
				}
				else if(state->examineCell != 0) {
					fdWorldMapFreeze();
					fdExamineCell(state->examineCell);
					fdWorldMapUnfreeze();
					state->examineCell = 0;
				}
				else if(state->psynergy != 0) {
					mnAreaNameWindowHideStub();
					fdWorldMapFreeze();
					fdUsePsynergy(state->psynergy);
					fdWorldMapUnfreeze();
					state->psynergy = 0;
				}
				else if(state->item != 0) {
					fdWorldMapFreeze();
					fdUseItem(state->item, state->itemSlot);
					fdWorldMapUnfreeze();
					state->item = 0;
				}
				else if(state->pauseMenu != 0) {
					/* The Pause menu (in debug mode, B opens the debug scene
					 * select and L */
					/* the palette editor). */
					sySndPlayStub(SE_MENU_CURSOR_MOVE);
					fdLeaderStopMoving();
					fdWorldMapFreeze();
					gmFlagSetStub(FLAG_MENU_OPEN);
					if(DebugMode != 0 && (KeysHeld & KEY_B) != 0) {
						mnDebugSceneSelectRunStub();
					}
					else if(DebugMode != 0 && (KeysHeld & KEY_L) != 0) {
						fdPaletteEditorRun();
					}
					else if(gmFlagGetStub(FLAG_MENUS_RAISE_TRIGGER) != 0) {
						/* As for the field menu above. */
						state->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
					}
					else {
						fdEndBusyAndReveal();
						mnAreaNameWindowHideStub();
						MainRoutineStateWork.returnMusic =
							sySndGetCurrentBgmStub();
						if(gmFlagGetStub(FLAG_IN_BATTLE_ARENA) == 0) {
							/* With fewer than PAUSE_MENU_VRAM_BLOCKS of the
							 * sprite VRAM */
							/* blocks free, sprite drawing stays suspended while
							 * the menu */
							/* runs, and a frame passes so that the field's
							 * sprites give */
							/* their VRAM back before it opens. */
							ACTOR_SYSTEM_STATE *system = ActorSystemStateWork;
							u8 *owner = VramCacheBlockOwners;
							s32 free = 0;
							s32 n;

							for(n = 0; n < VRAM_BLOCK_COUNT; n++) {
								if(*owner++ == VRAM_BLOCK_OWNER_FREE)
									free++;
							}
							if(free - PAUSE_MENU_VRAM_BLOCKS < 0) {
								system->suspended = 1;
								sySynch(1);
							}
							mnPauseMenuRunStub(0);
							system->suspended = 0;
						}
						else {
							/* "The Pause menu is not available now." */
							mnFieldMessageShowStub(MsgFieldPauseUnavailable,
												   FIELD_MESSAGE_ZOOM);
						}
					}
					fdWorldMapUnfreeze();
					gmFlagClearStub(FLAG_MENU_OPEN);
					state->pauseMenu = 0;
				}
			}

			/* Hand the leader back to the player, as climbing, carried through
			 * the */
			/* air (only a key check runs), on the World Map or walking, and run
			 * the */
			/* walking update until the next request. */
			gmFlagClearStub(FLAG_FIELD_HANDLING_REQUESTS);
			actor = state->actors[MainRoutineStateWork.leaderActorId];
			if(actor != NULL) {
				if(MainRoutineStateWork.movementMode ==
				   LEADER_MOVEMENT_AIRBORNE)
					fdActorStartKeyCheckStub(actor);
				else if(MainRoutineStateWork.movementMode ==
						LEADER_MOVEMENT_CLIMB)
					fdActorStartMoveByKeysStub(actor);
				else if(state->mode == ACTOR_SYSTEM_WORLD_MAP)
					fdActorStartWorldMapCtrlStub(actor);
				else
					fdActorStartPlayerCtrlStub(actor);
			}
			do {
				sySynch(1);
				actor = state->actors[MainRoutineStateWork.leaderActorId];
				if(DebugMode == 0 || gmFlagGetStub(FLAG_DEBUG_STEPS_OFF) == 0)
					fdUpdateWalking(actor->layer, actor->x.fixed,
									actor->y.fixed, actor->z.fixed);
			} while(fdCountRequests() == 0);
		}
	}

done:
	if(state->transitionActive != 0) {
		fdScrTransitionFinish(state->transition, state->transitionFrm);
		state->transitionActive = 0;
		sySynch(state->transitionFrm);
	}
	syHeapSlotRelease(HEAP_SLOT_FIELD);
	return result;
}

/* Returns the cell behavior of the map cell 16 pixels ahead of the party */
/* leader when there is something to search there: a fixed search spot */
/* (CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL) whose ground is level with the */
/* leader or up to 64 pixels above it, or a behavior the scene has an examine */
/* trigger for. Returns 0 otherwise. */
s32 fdActorGetTriggerCellAheadOfCurrent(void) {
	FIELD_STATE *state;
	MAP_GRID_SCROLL_STATE *grid;
	ACTOR *leader;
	POLAR_OFFSET_POS probePosition;
	u8 *cell;
	s32 behavior;
	s32 groundHeight;
	s32 result;

	result = 0;
	leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	state = FieldStateWork;
	grid = MapGridState[0];
	if(leader != 0) {
		probePosition.vector.x = leader->x.fixed;
		probePosition.vector.y = leader->y.fixed;
		probePosition.vector.z = leader->z.fixed;
		syVectorAddPolarOffset(toFIXED(16), leader->facing,
							   probePosition.coordinates);
		/* World Map cell tiles are 32 pixels square, 32 to a row; map cells 16
		 */
		/* pixels square, 128 to a row, four bytes each. */
		if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
			cell = Ram_CellTiles +
				   ((((probePosition.vector.x / toFIXED(32)) & 31) +
					 (((probePosition.vector.z / toFIXED(32)) & 31) << 5))
					<< 2);
		}
		else {
			u8 *cells = (u8 *)grid->layers[MAPGRID_LAYER_GROUND].cells;

			cell = cells +
				   (((probePosition.vector.x / toFIXED(MAPGRID_CELL_PIXELS)) +
					 ((probePosition.vector.z / toFIXED(MAPGRID_CELL_PIXELS)) *
					  MAPGRID_SIZE))
					<< 2);
		}
		behavior = cell[2];
		if(behavior >= CELL_BEHAVIOR_JAR && behavior <= CELL_BEHAVIOR_WELL) {
			groundHeight = fdMapGridGetTerrainHeightStub(
				leader->layer, probePosition.vector.x, probePosition.vector.z);
			if(groundHeight >= leader->y.fixed &&
			   groundHeight <= leader->y.fixed + toFIXED(64)) {
				result = behavior;
			}
		}
		else if(scFindTrigger(TRIGGER_EXAMINE, behavior) != 0) {
			result = behavior;
		}
	}
	return result;
}

/* Places the party at the entrance it came in by (the first matching */
/* entry, or the scene's first one) unless it keeps its position, and sets the
 */
/* map's scroll bounds, at least a screen (240 x 160 pixels) wide and tall. */
void scApplyEntrance(void) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	s32 entrance = MainRoutineStateWork.entrance;
	const SCENE_ENTRANCE *entranceEntry = OverlayArea.entrances();
	s32 foundEntrance = 0;

	for(; entranceEntry->entrance != -1; entranceEntry++) {
		if(entranceEntry->entrance == entrance &&
		   (entranceEntry->requiredFlg == -1 ||
			gmFlagGetStub(entranceEntry->requiredFlg))) {
			foundEntrance = 1;
			break;
		}
	}
	if(!foundEntrance)
		entranceEntry = OverlayArea.entrances();

	/* Camera bounds apply even when returning to a saved party position. */
	if(!gmFlagGetStub(FLAG_KEEP_PARTY_POSITION)) {
		MainRoutineStateWork.x = INTtoFIXED(entranceEntry->x);
		MainRoutineStateWork.y = INTtoFIXED(entranceEntry->y);
		MainRoutineStateWork.z = INTtoFIXED(entranceEntry->z);
		MainRoutineStateWork.heading = entranceEntry->facing;
		MainRoutineStateWork.layer = 0;
	}

	if(entranceEntry->cameraLeft != -1)
		grid->minX = INTtoFIXED(entranceEntry->cameraLeft);
	if(entranceEntry->cameraTop != -1)
		grid->minY = INTtoFIXED(entranceEntry->cameraTop);
	if(entranceEntry->cameraRight != -1)
		grid->maxX = INTtoFIXED(entranceEntry->cameraRight);
	if(entranceEntry->cameraBottom != -1)
		grid->maxY = INTtoFIXED(entranceEntry->cameraBottom);

	if(grid->minX + toFIXED(DISPLAY_WIDTH) > grid->maxX)
		grid->minX = grid->maxX - toFIXED(DISPLAY_WIDTH);
	if(grid->minY + toFIXED(DISPLAY_HEIGHT) > grid->maxY)
		grid->minY = grid->maxY - toFIXED(DISPLAY_HEIGHT);
}

#undef HAZARD_LIMIT_DEFAULT
#undef FIELD_TALK_REQUEST
#undef ANIM_LEADER_LIE_FLAT
#undef ISAAC_CHILD_ANIMATION_STAND_BRACED
#undef PAUSE_MENU_VRAM_BLOCKS

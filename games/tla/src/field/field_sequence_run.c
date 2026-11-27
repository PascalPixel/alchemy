/* @file */
/* The field sequence: running a scene until the party leaves it, and its */
/* entrance. */
/* Part of field_sequence.c. */

#include "sound/stubs.h"

extern u8 MapGridCellBuffer[];

/* UB: declared returning s32; the definition (main_routine.c) returns s16. */
#ifdef UBFIX
s16 gmFieldGetSceneAttributes(s32 scene);
#else
s32 gmFieldGetSceneAttributes(s32 scene);
#endif
s32 fdLeaderStepOntoRope(void);
s32 fdLeaderStepOffRope(void);
s32 fdLeaderStepOntoTightrope(void);
s32 fdLeaderStepOffTightrope(void);

/* The two nibbles of a scene's attributes (gmFieldGetSceneAttributes). */
#define NIBBLE_LOW(value)  ((value) & 15)
#define NIBBLE_HIGH(value) (((value) >> 4) & 15)
/* The hazard count's limit each scene starts with, in steps on hazard cells: */
/* the hazard deals its damage each time the count reaches it. */
#define HAZARD_LIMIT_DEFAULT 16
/* A Teleport destination's map screen marker (WORLD_MAP_PLACE.value's high */
/* nibble): 1 for a place the party walks to; 2 for Lemuria's, which it */
/* reaches aboard the ship. */
#define PLACE_MARKER_ON_FOOT (1 << 4)
#define PLACE_MARKER_ABOARD	 (2 << 4)
/* The sprite VRAM blocks the Pause menu needs free, or the field's sprites */
/* stop drawing while it runs. */
#define PAUSE_MENU_VRAM_BLOCKS 136
/* A map entrance stream's row: the entrance, its x and z, and its flags byte.
 */
#define MAP_ENTRANCE_ROW_BYTES 6

/* The field sequence: runs the current scene, which the party entered by */
/* entrance, until it leaves; returns the exit kind it left by, for */
/* scTakeExit, or EXIT_KEEP_DESTINATION when the next scene is already in */
/* MainRoutineStateWork: a random battle, a party that fell outside battle, */
/* Retreat or an event's destination. */
s32 fdSequenceRun(void) {
	FIELD_STATE *state;
	MAIN_ROUTINE_STATE *party;
	ACTOR *leader;
	FIELD_UNIT *unit;
	s32 result;
	s32 arrivalState;
	s32 *leaderId;
	s32 *steps;
	s16 *entrance;
	u16 *layer;
	s8 *pending;
	s32 code;
	s32 mode;
	u32 value;
	s32 talk;
	s32 packed;

	/* Set up the scene. */
	state = syHeapSlotAllocEwram(HEAP_SLOT_FIELD, sizeof(FIELD_STATE));
	syWordsClearEntry(state, sizeof(FIELD_STATE));
	syGameFlagClear(FLAG_IN_BATTLE);
	MainRoutineStateWork.returnScene = MainRoutineStateWork.next;
	MainRoutineStateWork.returnEntrance = MainRoutineStateWork.entrance;
	MainRoutineStateWork.afterBattleScene = -1;
	MainRoutineStateWork.afterBattleEntrance = -1;
	MainRoutineStateWork.defeatScene = -1;
	MainRoutineStateWork.defeatEntrance = -1;
	MainRoutineStateWork.onUpperLevel = 0;
	syInitEvent();
	fdDjinnResolvePending(0);
	if(MainRoutineStateWork.mapId == SCENE_CLEAR) {
		scWorldMapInitPerspectiveStub();
		MainRoutineStateWork.perspectiveMap = 1;
		mode = ACTOR_SYSTEM_WORLD_MAP;
		fdEncounterSelectMapZones(FALSE);
	}
	else {
		fdMapGridLoadMapStub(MainRoutineStateWork.mapId);
		mode = ACTOR_SYSTEM_MAP;
		fdEncounterSelectMapZones(TRUE);
	}
	state->mode = mode;
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
		TouchCopiesWork.cnt = 0;
	else
		fdTileCopiesRestore();
	mnWindowSystemInitFieldStub();
	fdActorSystemInitStub(mode);
	fdClearRequests();
	state->triggers = (const void *)SCENE_ENTRIES->triggers();
	fdApplyEntrance();
	fdActorResetForLeader((s32)SCENE_ENTRIES->placements());
	fdPlaceTreasureSpots();
	if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
		fdActorRestore();
	if(MainRoutineStateWork.pendingDjinni != 0)
		fdDjinnRestoreActorAfterBattle();
	state->queuedSnd = -1;
	if(mode == ACTOR_SYSTEM_WORLD_MAP) {
		scWorldMapUpdateCurrentTileBlockStub();
		scWorldMapLoadGraphicsStub(MainRoutineStateWork.x,
								   MainRoutineStateWork.z);
	}
	else {
		fdMapApplySetFlagEditsStub();
		SCENE_ENTRIES->prepareMap();
		fdMapGridCenterOnFollowedPointStub();
	}
	/* XXX (no evidence): what flag 0x11f means; every scene start clears it, */
	/* The Broken Seal's does not, and nothing in either game's code, tables, */
	/* debug flag lists or cleared ranges of flags sets or reads it. */
	syGameFlagClear(0x11f);
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
		syGameFlagSet(FLAG_RETREAT_BLOCKED);
	}
	state->prevCell = MapGridCellBuffer;
	/* A fresh entry (not a return from battle) resets the hazard count to its
	 */
	/* defaults (damage 1 every HAZARD_LIMIT_DEFAULT steps), the stopped actor
	 * and */
	/* Cloak. */
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		scSetRestartPoint();
		MainRoutineStateWork.hazardLimit = HAZARD_LIMIT_DEFAULT;
		MainRoutineStateWork.hazardMode = HAZARD_COUNT_ON_CELLS;
		MainRoutineStateWork.hazardDamage = 1;
		MainRoutineStateWork.haltedActor = -1;
		MainRoutineStateWork.cloaked = 0;
	}
	fdPsynergyResumeCloakCheck();
	value = gmFieldGetSceneAttributes(MainRoutineStateWork.next);
	state->sceneAttributeHigh = NIBBLE_HIGH(value);
	state->sceneAttributeLow = NIBBLE_LOW(value);

	if(MainRoutineStateWork.teleportState == TELEPORT_ARRIVING) {
		fdActorGetById(fdPartyGetLeaderActor())->scaleX = 0;
		fdActorSetSprFlagsStub(fdActorGetById(fdPartyGetLeaderActor()), 0);
	}
	fdUpdateHoverReady();
	sySynch(2);
	SCENE_ENTRIES->initialize();
	/* The scene's initialization may already have taken an exit. */
	code = state->exit;
	if(code != 0) {
		result = code;
		state->exit = 0;
		goto done;
	}
	syGameFlagClear(FLAG_KEEP_PARTY_POSITION);
	/* Fade in, show the area name and give what was left pending. */
	if(fdCountRequests() == 0) {
		value = state->transitionActive;
		if(value == 0) {
			/* XXX: why a Teleport arrival holds 6584 bytes of the battle
			 * effect's */
			/* slot through its two 20-frame pauses. No state of that size is
			 * known */
			/* (Teleport's own take 7824), and what the scene's tasks take from
			 * the */
			/* heap in those frames is freed with it (syHeapSlotRelease). A
			 * trace of */
			/* HEAP_SLOTS.nextIwram through a Teleport chosen on the map screen,
			 */
			/* which no render here stages, would settle it. */
			if(MainRoutineStateWork.teleportState == TELEPORT_CHOSEN) {
				syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, 6584);
				sySynch(20);
				syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
			}
			fdScrTransitionStart(state->transition, state->transitionFrm);
			state->transitionActive = 1;
			if(MainRoutineStateWork.teleportState == TELEPORT_CHOSEN) {
				syHeapSlotAlloc(HEAP_SLOT_BATTLE_EFFECT, 6584);
				sySynch(20);
				syHeapSlotRelease(HEAP_SLOT_BATTLE_EFFECT);
			}
			PLTT_BACKDROP = 0;
			/* An arrival the last scene asked for (fdEntrancePlayArrival) plays
			 */
			/* through the fade; with none, wait out half of it. */
			if(!fdEntrancePlayArrival())
				sySynch((state->transitionFrm + 1) / 2);
		}
		arrivalState = MainRoutineStateWork.teleportState;
		if(arrivalState == TELEPORT_CHOSEN) {
			MainRoutineStateWork.teleportState = TELEPORT_ARRIVING;
			fdTeleportPlaySparksAndFade();
			if(MainRoutineStateWork.teleportScene != SCENE_WORLD_MAP)
				MainRoutineStateWork.ownerOverride = 0;
			else if((value = fdFlagEntriesGetValue(
						 MainRoutineStateWork.teleportEntrance)) &
					PLACE_MARKER_ON_FOOT)
				MainRoutineStateWork.ownerOverride = 0;
			else if(value & PLACE_MARKER_ABOARD)
				MainRoutineStateWork.ownerOverride = 1;
			fdEventSetDestination(MainRoutineStateWork.teleportScene,
								  MainRoutineStateWork.teleportEntrance);
		}
		else if(arrivalState == TELEPORT_ARRIVING) {
			MainRoutineStateWork.teleportState = TELEPORT_NONE;
			fdTeleportPlayWhirl();
		}
		if(syGameFlagGet(FLAG_SHOW_AREA_NAME)) {
			syGameFlagClear(FLAG_SHOW_AREA_NAME);
			mnAreaNameWindowShowStub(MainRoutineStateWork.next);
		}
		if(MainRoutineStateWork.pendingDjinni != 0) {
			/* UB: called with a second argument (1) its definition does not
			 * take. */
#ifdef UBFIX
			fdDjinnResolvePending(MainRoutineStateWork.pendingDjinni);
#else
			((void (*)(s32, s32))fdDjinnResolvePending)(
				MainRoutineStateWork.pendingDjinni, 1);
#endif
			MainRoutineStateWork.pendingDjinni = 0;
		}
		if(MainRoutineStateWork.pendingItem != 0) {
			fdWorldMapFreeze();
			if(fdPartyCheckItemLimit(MainRoutineStateWork.pendingItem) == 0) {
				fdPartyInventoryGiveItem(MainRoutineStateWork.pendingItem, 0);
			}
			fdWorldMapReloadPendingAreaGraphics();
			MainRoutineStateWork.pendingItem = 0;
		}
	}

	party = &MainRoutineStateWork;
	leaderId = &MainRoutineStateWork.leaderActorId;
	steps = &MainRoutineStateWork.encounterSteps;
	entrance = &MainRoutineStateWork.entrance;
	layer = &MainRoutineStateWork.layer;
	for(;;) {
		/* Note where the leader is, for the requests to use. */
		syGameFlagSet(FLAG_FIELD_HANDLING_REQUESTS);
		leader = fdGetPartyActor();
		fdActorStartIdleStub(leader);
		SET_POSITION(party, leader->x.fixed, 0, leader->z.fixed);
		party->heading = leader->facing;
		*layer = leader->layer;
		/* Handle the pending requests, most urgent first. */
		while(fdCountRequests() != 0) {
			if(state->exit != 0) {
				result = state->exit;
				state->exit = 0;
				goto done;
			}
			if(state->raisedTrigger != 0) {
				state->busy = 1;
				if(state->raisedTrigger == FIELD_RAISED_TRIGGER_PARTY_FELL) {
					/* Characters fell to poison or hazardous ground. */
					fdLeaderStopMoving();
					fdWorldMapFreeze();
					for(value = 0; value < state->fallenCnt; value++) {
						fdActorSelectAnimationStub(leader,
												   ANIM_LEADER_LEAN_BACK);
						mnMessageArgumentPushStub(state->fallenOwners[value],
												  MESSAGE_ARGUMENT_ACTOR);
						fdMessageShowStub((s32)MsgFieldStrengthExhausted,
										  FIELD_MESSAGE_ZOOM);
					}
					if(state->survivorCnt == 0) {
						/* The whole party fell: back to the restart point with
						 * 1 HP. */
						if(*leaderId == ACTOR_FELIX)
							fdActorSelectAnimationStub(
								leader, ANIM_LEADER_FALL_ON_FACE);
						else
							fdActorSelectAnimationStub(leader, ANIM_COLLAPSE);
						sySndPlayStub(MUS_DEFEATED);
						fdMessageShowStub((s32)MsgFieldPartyAnnihilated,
										  FIELD_MESSAGE_ZOOM);
						unit = syUnitGet(*leaderId);
						unit->hp = 1;
						gmUnitUpdateGauges(unit);
						{
							s32 restartScene = party->restartScene;
							s32 restartEntrance;

							party->next = restartScene;
							restartEntrance = party->restartEntrance;
							*entrance = restartEntrance;
							fdWorldMapSetReturnLeavingScene(
								(s16)restartScene, (s16)restartEntrance);
						}
						result = EXIT_KEEP_DESTINATION;
						fdWorldMapReloadPendingAreaGraphics();
						goto done;
					}
					fdWorldMapReloadPendingAreaGraphics();
				}
				else if(state->raisedTrigger == FIELD_RAISED_TRIGGER_TELEPORT) {
					fdLeaderStopMoving();
					fdMapScrShow(MAP_SCREEN_TELEPORT);
					if(MainRoutineStateWork.teleportScene != -1)
						MainRoutineStateWork.teleportState = TELEPORT_CHOSEN;
				}
				else if(state->raisedTrigger ==
						FIELD_RAISED_TRIGGER_MAP_SCREEN) {
					fdLeaderStopMoving();
					/* Shown once, to look: fdMapScrShow always returns 0, so
					 * the mode */
					/* is never flipped to Teleport's. */
					value = MAP_SCREEN_LOOK;
					while(fdMapScrShow(value))
						value ^= 1;
				}
				else if(state->raisedTrigger ==
						FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM) {
					fdLeaderStopMoving();
					fdWorldMapZoomOutWhileLHeld();
				}
				else if(state->raisedTrigger == FIELD_RAISED_TRIGGER_ROPE_ON) {
					if(state->heldKeys == InputWork.held) {
						fdLeaderStepOntoRope();
						state->fieldMenu = 0;
						state->pauseMenu = 0;
						state->aButton = 0;
						state->encounter = 0;
						state->psynergy = 0;
					}
				}
				else if(state->raisedTrigger == FIELD_RAISED_TRIGGER_ROPE_OFF) {
					if(state->heldKeys == InputWork.held) {
						fdLeaderStepOffRope();
						state->fieldMenu = 0;
						state->pauseMenu = 0;
						state->aButton = 0;
						state->encounter = 0;
						state->psynergy = 0;
					}
				}
				else if(state->raisedTrigger ==
						FIELD_RAISED_TRIGGER_TIGHTROPE_ON) {
					if(state->heldKeys == InputWork.held) {
						fdLeaderStepOntoTightrope();
						state->fieldMenu = 0;
						state->pauseMenu = 0;
						state->aButton = 0;
						state->encounter = 0;
						state->psynergy = 0;
					}
				}
				else if(state->raisedTrigger ==
						FIELD_RAISED_TRIGGER_TIGHTROPE_OFF) {
					if(state->heldKeys == InputWork.held) {
						fdLeaderStepOffTightrope();
						state->fieldMenu = 0;
						state->pauseMenu = 0;
						state->aButton = 0;
						state->encounter = 0;
						state->psynergy = 0;
					}
				}
				else {
					scRunRaisedTrigger(state->raisedTrigger);
				}
				state->busy = 0;
				state->raisedTrigger = 0;
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
			else if(state->encounter != 0) {
				/* A random encounter: save the actors and go to the battle. */
				fdLeaderStopMoving();
				fdActorTableSnapshot();
				party->returnMusic = -1;
				party->next = SEQUENCE_BATTLE;
				*entrance = state->encounter;
				state->exit = EXIT_KEEP_DESTINATION;
				fdStartEncounterTransition(state->encounter);
				state->encounterBias = 0;
				state->encounter = 0;
				*steps = 0;
			}
			else if(state->aButton != 0) {
				/* Talk to the actor ahead, or examine the cell ahead, or open
				 * the field */
				/* menu if neither has an event. */
				mnAreaNameWindowHideStub();
				talk = 0;
				if(MainRoutineStateWork.ownerOverride == 0)
					value = fdFindNearbyTalkTarget(*leaderId);
				else
					value = fdFindNearbyTalkTarget(ACTOR_FIRST_PLACED);
				if(value != -1) {
					talk = 1;
					if((s32)value <= FIELD_TREASURE_ACTOR - 1)
						talk = scFindActorTalkTrigger(value) != 0;
				}
				if(talk) {
					state->talk = value | FIELD_TALK_REQUEST;
					state->fieldMenu = 0;
				}
				else if((talk = fdActorGetTriggerCellAheadOfCurrent()) != 0) {
					state->examineCell = talk;
					state->fieldMenu = 0;
				}
				else {
					state->fieldMenu = 1;
				}
				state->aButton = 0;
			}
			else if(state->fieldMenu != 0) {
				/* The field menu (B + Select in debug mode opens the flag
				 * editor). */
				mnAreaNameWindowHideStub();
				fdLeaderStopMoving();
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				fdWorldMapFreeze();
				syGameFlagSet(FLAG_MENU_OPEN);
				if(DebugMode && (InputWork.held & KEY_B) != 0 &&
				   ((InputWork.held & KEY_SELECT) != 0)) {
					mnDebugFlagEditorRunStub();
				}
				else if(syGameFlagGet(FLAG_MENUS_RAISE_TRIGGER)) {
					state->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
				}
				else {
					state->effTmrRunning = 0;
					mnFieldMenuRunStub();
					state->effTmrRunning = 1;
					fdUpdateHoverReady();
				}
				fdWorldMapReloadPendingAreaGraphics();
				syGameFlagClear(FLAG_MENU_OPEN);
				syGameFlagRefreshLureCapStub();
				state->fieldMenu = 0;
			}
			else if(state->talk != 0) {
				fdWorldMapFreeze();
				scTalkToActor(state->talk & (FIELD_TALK_REQUEST - 1));
				fdWorldMapReloadPendingAreaGraphics();
				state->talk = 0;
			}
			else if(state->examineCell != 0) {
				fdWorldMapFreeze();
				fdExamineCell(state->examineCell);
				fdWorldMapReloadPendingAreaGraphics();
				state->examineCell = 0;
			}
			else if(state->psynergy != 0) {
				packed = state->psynergy & ACT_PACKED;
				mnAreaNameWindowHideStub();
				if(packed == 0) {
					fdWorldMapFreeze();
					if(TLA_PATCH_1)
						sySynch(1);
				}
				fdUsePsynergy(state->psynergy);
				if(packed == 0)
					fdWorldMapReloadPendingAreaGraphics();
				state->psynergy = 0;
			}
			else if(state->item != 0) {
				fdWorldMapFreeze();
				fdUseItem(state->item, state->itemSlot);
				fdWorldMapReloadPendingAreaGraphics();
				state->item = 0;
			}
			else if(state->pauseMenu != 0) {
				/* The Pause menu (in debug mode, B opens the scene select and L
				 * the */
				/* palette editor). */
				sySndPlayStub(SE_MENU_CURSOR_MOVE);
				fdLeaderStopMoving();
				fdWorldMapFreeze();
				syGameFlagSet(FLAG_MENU_OPEN);
				if(*(volatile u8 *)&DebugMode && (InputWork.held & KEY_B)) {
					mnDebugSceneSelectRunStub();
				}
				else if(*(volatile u8 *)&DebugMode &&
						(InputWork.held & KEY_L)) {
					fdPaletteEditorRun();
				}
				else if(syGameFlagGet(FLAG_MENUS_RAISE_TRIGGER)) {
					state->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
				}
				else {
					fdEventEndAllLastingPsynergy();
					mnAreaNameWindowHideStub();
					party->returnMusic = sySndGetCurrentBgmStub();
					if(!syGameFlagGet(FLAG_IN_BATTLE_ARENA)) {
						/* With fewer than PAUSE_MENU_VRAM_BLOCKS of the sprite
						 * VRAM blocks */
						/* free, sprite drawing stays suspended while the menu
						 * runs, and a */
						/* frame passes so that the field's sprites give their
						 * VRAM back */
						/* before it opens. */
						ACTOR_SYSTEM_STATE *system = Ram_HeapSlots->actorSystem;
						u8 *owner = VramCacheBlockOwners;
						s32 freeBlocks = 0;
						s32 n;

						for(n = 0; n < VRAM_BLOCK_COUNT; n++) {
							if(*owner++ == VRAM_BLOCK_OWNER_FREE)
								freeBlocks++;
						}
						if(freeBlocks - PAUSE_MENU_VRAM_BLOCKS < 0) {
							system->suspended = 1;
							sySynch(1);
						}
						mnPauseMenuRunStub(0);
						system->suspended = 0;
					}
					else {
						/* "The Pause Menu is not available now." */
						fdMessageShowStub((s32)MsgFieldPauseUnavailable,
										  FIELD_MESSAGE_ZOOM);
					}
				}
				fdWorldMapReloadPendingAreaGraphics();
				syGameFlagClear(FLAG_MENU_OPEN);
				state->pauseMenu = 0;
			}
		}
		/* Hand the leader back to the player, with the control order for how
		 * the */
		/* party moves, and run the walking update until the next request. */
		syGameFlagClear(FLAG_FIELD_HANDLING_REQUESTS);
		if((leader = fdGetPartyActor()) != NULL)
			fdActorStartMovementModeScriptStub(leader);
		pending = &MainRoutineStateWork.sandActive;
		do {
			sySynch(1);
			leader = fdGetPartyActor();
			if(DebugMode == 0 || !syGameFlagGet(FLAG_DEBUG_STEPS_OFF)) {
				if(*pending == 0 ||
				   (leader->tmr == 0 && *(s16 *)&leader->userValue == 0))
					fdUpdateWalking(leader->layer, leader->x.fixed,
									leader->y.fixed, leader->z.fixed);
			}
		} while(fdCountRequests() == 0);
	}

done:
	if(state->transitionActive != 0) {
		fdScrTransitionFinish(state->transition, state->transitionFrm);
		state->transitionActive = 0;
		sySynch(state->transitionFrm);
	}
	sySynch(1);
	syHeapSlotRelease(HEAP_SLOT_FIELD);
	return result;
}

/* Returns the cell behavior of the map cell 16 pixels ahead of the party */
/* leader when there is something to search there: a fixed search spot */
/* (CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL) whose ground is level with the */
/* leader or up to 16 pixels above it, or a behavior the scene has an examine */
/* trigger for; aboard the ship on the World Map, SHIP_SEA_EXAMINE_CODE before
 */
/* sea of collision 3. Returns 0 otherwise. */
s32 fdActorGetTriggerCellAheadOfCurrent(void) {
	FIELD_STATE *state;
	MAP_STATE *map;
	ACTOR *leader;
	POLAR_OFFSET_POS probePosition;
	MAP_CELL *cell;
	s32 behavior;
	s32 groundHeight;
	s32 result;

	result = 0;
	leader = fdGetPartyActor();
	state = FieldStateWork;
	map = Ram_HeapSlots->mapGrid;
	if(leader != 0) {
		probePosition.vector.x = leader->x.fixed;
		probePosition.vector.y = leader->y.fixed;
		probePosition.vector.z = leader->z.fixed;
		{
			s32 *position = probePosition.coordinates;

			syVectorAddPolarOffset(toFIXED(16), leader->facing, position);
		}
		/* World Map cells are 32 pixels square, 32 to a row; map cells 16
		 * pixels */
		/* square, MAPGRID_SIZE to a row. */
		if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
			s32 x, z;

			x = probePosition.vector.x;
			z = probePosition.vector.z;
			cell =
				&((MAP_CELL *)
					  syRamMapBlocks)[((x / toFIXED(WORLD_MAP_CELL_PIXELS)) &
									   (WORLD_MAP_VIEW_CELLS - 1)) +
									  (((z / toFIXED(WORLD_MAP_CELL_PIXELS)) &
										(WORLD_MAP_VIEW_CELLS - 1)) *
									   WORLD_MAP_VIEW_CELLS)];
		}
		else {
			s32 x, z;

			cell = map->layers[0].cells;
			x = probePosition.vector.x;
			z = probePosition.vector.z;
			cell += (x / toFIXED(MAPGRID_CELL_PIXELS)) +
					(z / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE;
		}
		behavior = cell->collisionCode;
		if(behavior >= CELL_BEHAVIOR_JAR && behavior <= CELL_BEHAVIOR_WELL) {
			groundHeight = fdMapGridGetTerrainHeightStub(
				leader->layer, probePosition.vector.x, probePosition.vector.z);
			if(groundHeight >= leader->y.fixed &&
			   groundHeight <= leader->y.fixed + toFIXED(16)) {
				result = behavior;
			}
		}
		else if(scFindTrigger(TRIGGER_EXAMINE, behavior) != 0) {
			result = behavior;
		}
		if(result == 0 && state->mode == ACTOR_SYSTEM_WORLD_MAP) {
			if(MainRoutineStateWork.ownerOverride != 0 &&
			   scWorldMapGetCollisionStub(probePosition.coordinates) == 3)
				result = SHIP_SEA_EXAMINE_CODE;
		}
	}
	return result;
}

/* Places the party at the entrance it came in by, from the scene's entrance */
/* rows, unless it keeps its position, and sets the map's scroll bounds, at */
/* least a screen (240 x 160 pixels) wide and tall. With findEntrance it looks
 */
/* the entrance up and returns 0 when there is none; without, it takes the */
/* scene's first row. */
s32 scApplyEntrance(s32 findEntrance) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	s32 entrance = MainRoutineStateWork.entrance;
	const SCENE_ENTRANCE *entranceEntry = SCENE_ENTRIES->entrances();
	s32 foundEntrance = 0;
	s32 left;

	if(findEntrance) {
		for(; entranceEntry->entrance != -1; entranceEntry++) {
			if(entranceEntry->entrance == entrance &&
			   (entranceEntry->requiredFlg == -1 ||
				syGameFlagGet(entranceEntry->requiredFlg))) {
				foundEntrance = 1;
				break;
			}
		}
		if(!foundEntrance)
			return 0;
	}

	/* Camera bounds apply even when returning to a saved party position. */
	if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		SET_POSITION(&MainRoutineStateWork, toFIXED(entranceEntry->x),
					 toFIXED(entranceEntry->y), toFIXED(entranceEntry->z));
		MainRoutineStateWork.heading = entranceEntry->facing;
		MainRoutineStateWork.layer = 0;
	}

	left = entranceEntry->cameraLeft;
	if(left != -1)
		map->minX = toFIXED(left);
	if(entranceEntry->cameraTop != -1)
		map->minZ = toFIXED(entranceEntry->cameraTop);
	if(entranceEntry->cameraRight != -1)
		map->maxX = toFIXED(entranceEntry->cameraRight);
	if(entranceEntry->cameraBottom != -1)
		map->maxZ = toFIXED(entranceEntry->cameraBottom);

	if(map->minX + toFIXED(DISPLAY_WIDTH) > map->maxX)
		map->minX = map->maxX - toFIXED(DISPLAY_WIDTH);
	if(map->minZ + toFIXED(DISPLAY_HEIGHT) > map->maxZ)
		map->minZ = map->maxZ - toFIXED(DISPLAY_HEIGHT);
	return 1;
}

/* A map entrance's flags byte: the party's facing in sixteenths of a turn, */
/* then whether it stands 8 pixels up and whether the camera keeps the bounds */
/* it has. */
#define MAP_ENTRANCE_FACING_MASK 15
#define MAP_ENTRANCE_RAISED		 (1 << 4)
#define MAP_ENTRANCE_KEEP_BOUNDS (1 << 5)

/* Reads the map's entrance stream (MapScene) for the current entrance, */
/* places the party there unless FLAG_KEEP_PARTY_POSITION keeps it, then sets */
/* the camera bounds from the first box that holds it. */
s32 fdPlaceAtEntrance(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAIN_ROUTINE_STATE *state = &MainRoutineStateWork;
	FIXED xFixed;
	u8 *cursor = MapScene;
	s32 *angle;
	u16 *layer;
	s32 entrance = state->entrance;
	s32 height;
	s32 x;
	s32 z;
	FIXED zFixed;
	s32 base;
	s32 flags;
	s32 left;
	s32 top;
	s32 bottom;
	s32 right;
	s32 leftEdge, topEdge, rightEdge, bottomEdge;

	cursor = MapScene + (u16)fdStreamReadU16(&cursor);
	angle = &MainRoutineStateWork.heading;
	layer = &MainRoutineStateWork.layer;
	for(;; cursor += MAP_ENTRANCE_ROW_BYTES) {
		if(*(s8 *)cursor == -1) {
			*angle = FACING_SOUTH;
			{
				u32 cleared = 0;

				*layer = cleared;
			}
			return 0;
		}
		if(*(s8 *)cursor == entrance) {
			cursor++;
			x = fdStreamReadS16(&cursor);
			z = fdStreamReadS16(&cursor);
			xFixed = toFIXED(x);
			zFixed = toFIXED(z);
			height =
				FIXEDtoINT(fdMapGridGetTerrainHeightStub(0, xFixed, zFixed));
			flags = *cursor;
			base = (s32)MapScene;
			if(!syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
				MainRoutineStateWork.x = xFixed;
				MainRoutineStateWork.z = zFixed;
				*angle = (flags & MAP_ENTRANCE_FACING_MASK) * (TRIG_TURN / 16);
				*layer = 0;
				if(flags & MAP_ENTRANCE_RAISED)
					MainRoutineStateWork.y = toFIXED(8);
			}
			if(flags & MAP_ENTRANCE_KEEP_BOUNDS)
				return 1;

			cursor = MapSceneBoundsOffset;
			cursor = (u8 *)((u16)fdStreamReadU16(&cursor) + base);
			while(*(s8 *)cursor != -1) {
				{
					left = *(s8 *)cursor;
					leftEdge = left * MAPGRID_CELL_PIXELS;
					cursor++;
					top = *(s8 *)cursor;
					topEdge = top * MAPGRID_CELL_PIXELS;
					cursor++;
					right = *(s8 *)cursor;
					rightEdge = right * MAPGRID_CELL_PIXELS;
					cursor++;
					bottom = *(s8 *)cursor;
					bottomEdge = bottom * MAPGRID_CELL_PIXELS;
					cursor++;
					if(x >= leftEdge && x <= rightEdge &&
					   z - height >= topEdge && z - height <= bottomEdge) {
						map->minX = FIELD_CELL_ORIGIN(left);
						map->minZ = FIELD_CELL_ORIGIN(top);
						map->maxX = FIELD_CELL_ORIGIN(right);
						map->maxZ = FIELD_CELL_ORIGIN(bottom);
						if(map->minX + toFIXED(DISPLAY_WIDTH) > map->maxX)
							map->minX = map->maxX - toFIXED(DISPLAY_WIDTH);
						if(map->minZ + toFIXED(DISPLAY_HEIGHT) > map->maxZ)
							map->minZ = map->maxZ - toFIXED(DISPLAY_HEIGHT);
						break;
					}
				}
			}
			return 1;
		}
	}
}

/* Reads a little-endian halfword at *cursor and moves the cursor past it. */
u32 fdStreamReadU16(u8 **cursor) {
	u32 low = *(*cursor)++;
	u32 high = *(*cursor)++;

	return low | (high << 8);
}

/* Reads a signed little-endian halfword at *cursor and moves the cursor past */
/* it. */
s16 fdStreamReadS16(u8 **cursor) {
	u32 value;

	value = *(*cursor)++;
	value |= *(*cursor)++ << 8;
	return value;
}

/* Places the party where it enters: by the scene's entrance row for */
/* MainRoutineStateWork.entrance, else on a map by the map's own entrance */
/* stream (fdPlaceAtEntrance), else by the scene's first entrance row. */
void fdApplyEntrance(void) {
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	s32 done;

	done = scApplyEntrance(TRUE);
	if(state->mode == ACTOR_SYSTEM_MAP && done == 0)
		done = fdPlaceAtEntrance();
	if(done == 0)
		scApplyEntrance(FALSE);
}
#undef NIBBLE_LOW
#undef NIBBLE_HIGH
#undef HAZARD_LIMIT_DEFAULT
#undef PLACE_MARKER_ON_FOOT
#undef PLACE_MARKER_ABOARD
#undef PAUSE_MENU_VRAM_BLOCKS
#undef MAP_ENTRANCE_FACING_MASK
#undef MAP_ENTRANCE_RAISED
#undef MAP_ENTRANCE_KEEP_BOUNDS
#undef MAP_ENTRANCE_ROW_BYTES

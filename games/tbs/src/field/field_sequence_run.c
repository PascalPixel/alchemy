/// @file
/// The field sequence: running a scene until the party leaves it, and its
/// entrance.
// Part of field_sequence.c.

#include "field_sequence_internal.h"

/// The hazard count's limit each scene starts with, in steps on hazard cells:
/// the hazard deals its damage each time the count reaches it.
#define HAZARD_LIMIT_DEFAULT 16
/// FieldState.talk's request bit, set above the actor id so that a talk to
/// actor 0 is still a request (The Lost Age sets bit 14).
#define FIELD_TALK_REQUEST (1 << 12)
/// Isaac's pose when the whole party has fallen: lying flat, the picture
/// ANIM_LEADER_FALL ends on (isaac.tsv); any other leader plays ANIM_COLLAPSE.
#define ANIM_LEADER_LIE_FLAT 37
/// Isaac's pose for the same as a child (FLAG_CHILDHOOD): standing feet apart,
/// the picture isaac_child.tsv's ANIM_LEADER_STRIDE holds.
#define ISAAC_CHILD_ANIMATION_STAND_BRACED 21
/// The sprite VRAM blocks the Pause menu needs free, or the field's sprites
/// stop drawing while it runs.
#define PAUSE_MENU_VRAM_BLOCKS 136

/// The field sequence: runs the current scene, which the party entered by
/// entrance, until it leaves; returns the exit kind it left by, for
/// Scene_TakeExit, or EXIT_KEEP_DESTINATION when the next scene is already in
/// gMainRoutineState: a random battle, a party that fell outside battle,
/// Retreat or an event's destination.
s32 FieldSequence_Run(s32 entrance) {
  struct FieldState *state;
  struct Actor *actor;
  s32 mode;
  s32 result = 0;
  s32 code;

  // Set up the scene.
  state = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  Dma_Fill32(0, state, sizeof(struct FieldState));
  GameFlag_ClearFar(FLAG_IN_BATTLE);
  gMainRoutineState.returnScene = gMainRoutineState.next;
  gMainRoutineState.returnEntrance = gMainRoutineState.entrance;
  gMainRoutineState.afterBattleScene = -1;
  gMainRoutineState.afterBattleEntrance = -1;
  gMainRoutineState.defeatScene = -1;
  gMainRoutineState.defeatEntrance = -1;
  Task_ResetAll();
  Djinn_ResolvePending(0);
  if (gMainRoutineState.mapId == MAP_INDEX_WORLD) {
    WorldMap_InitializePerspectiveFar();
    gMainRoutineState.perspectiveMap = 1;
    mode = ACTOR_SYSTEM_WORLD_MAP;
    Encounter_SelectMapZones(0);
  } else {
    MapGrid_LoadMapFar(gMainRoutineState.mapId);
    mode = ACTOR_SYSTEM_MAP;
    Encounter_SelectMapZones(1);
  }
  state->mode = mode;
  ActorSystem_InitializeFar(mode);
  WindowSystem_InitFieldFar();
  Field_ClearRequests();
  state->triggers = gOverlayArea.triggers();
  Scene_ApplyEntrance();
  Actor_ResetForLeader((struct ActorPlacement *)gOverlayArea.actorPlacements());
  if (GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION) != 0)
    Actor_Restore();
  if (gMainRoutineState.pendingDjinni != 0)
    Djinn_RestoreActorAfterBattle();
  if (mode == ACTOR_SYSTEM_WORLD_MAP) {
    WorldMap_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gMainRoutineState.x, gMainRoutineState.z);
  } else {
    MapGrid_CenterOnFollowedPointFar();
  }
  Field_PlaceTreasureSpots();
  PaletteFade_Initialize();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  // The default fade is from the backdrop colour over 16 frames; the scene's
  // initialization may choose another.
  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  state->transitionFrames = MAP_TRANSITION_FRAMES;
  state->transitionActive = 0;
  // Field_UpdateWalking scales the leader's speed by this (16.16) into the
  // distance counted towards PP, Djinn recovery and poison: FIXED_ONE / 160
  // counts each of them on by one about every 160 pixels walked.
  state->walkingDistanceScale = FIXED_ONE / 160;
  state->walkingDistance = 0;
  if (gMainRoutineState.sceneType == SCENE_TYPE_TOWN) {
    state->walkingDistanceScale = 0;
    GameFlag_SetFar(FLAG_RETREAT_BLOCKED);
  }
  state->previousCell = (struct MapGridCell *)Ram_MapGridCellBuffer;
  // A fresh entry (not a return from battle) resets the hazard count to its
  // defaults (damage 1 every HAZARD_LIMIT_DEFAULT steps), the stopped actor and
  // Cloak.
  if (GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION) == 0) {
    Scene_SetRestartPoint();
    gMainRoutineState.hazardLimit = HAZARD_LIMIT_DEFAULT;
    gMainRoutineState.hazardMode = HAZARD_COUNT_ON_CELLS;
    gMainRoutineState.hazardDamage = 1;
    gMainRoutineState.haltedActor = -1;
    gMainRoutineState.cloaked = 0;
  }
  FieldPsynergy_ResumeCloakCheck();
  state->queuedSound = -1;
  gOverlayArea.initialize();

  // The scene's initialization may already have taken an exit.
  code = state->exit;
  if (code != 0) {
    result = code;
    state->exit = 0;
  } else {
    GameFlag_ClearFar(FLAG_KEEP_PARTY_POSITION);
    // Fade in, show the area name and give what was left pending.
    if (Field_CountRequests() == 0) {
      if (state->transitionActive == 0) {
        ScreenTransition_Start(state->transition, state->transitionFrames);
        state->transitionActive = 1;
        PLTT_BACKDROP = 0;
        // An arrival the last scene asked for (Entrance_PlayArrival) plays
        // through the fade; with none, wait out half of it.
        if (Entrance_PlayArrival() == 0)
          wait((state->transitionFrames + 1) / 2);
      }
      if (GameFlag_GetFar(FLAG_SHOW_AREA_NAME) != 0) {
        GameFlag_ClearFar(FLAG_SHOW_AREA_NAME);
        AreaNameWindow_ShowFar(gMainRoutineState.next);
      }
      if (gMainRoutineState.pendingDjinni != 0) {
        Djinn_ResolvePending(gMainRoutineState.pendingDjinni, 1);
        gMainRoutineState.pendingDjinni = 0;
      }
      if (gMainRoutineState.pendingItem != 0) {
        WorldMap_Freeze();
        if (Party_CheckItemLimit(gMainRoutineState.pendingItem) == 0)
          PartyInventory_GiveItem(gMainRoutineState.pendingItem, 0);
        WorldMap_Unfreeze();
        gMainRoutineState.pendingItem = 0;
      }
    }

    for (;;) {
      // Note where the leader is, for the requests to use.
      GameFlag_SetFar(FLAG_FIELD_HANDLING_REQUESTS);
      actor = state->actors[gMainRoutineState.leaderActorId];
      Actor_StartIdleFar(actor);
      gMainRoutineState.x = actor->x.fixed;
      gMainRoutineState.y = 0;
      gMainRoutineState.z = actor->z.fixed;
      gMainRoutineState.heading = actor->facing;
      gMainRoutineState.layer = actor->layer;

      // Handle the pending requests, most urgent first.
      while (Field_CountRequests() != 0) {
        s32 next = state->exit;

        if (next != 0) {
          result = next;
          state->exit = 0;
          goto done;
        }
        if (state->raisedTrigger != 0) {
          state->busy = 1;
          if (state->raisedTrigger == FIELD_RAISED_TRIGGER_PARTY_FELL) {
            // Characters fell to poison or hazardous ground.
            u32 i;

            Leader_StopMoving();
            WorldMap_Freeze();
            for (i = 0; i < state->fallenCount; i++) {
              Actor_SelectAnimationFar(actor, ANIM_LEADER_LEAN_BACK);
              MessageArgument_PushFar(state->fallenOwners[i],
                                      MESSAGE_ARGUMENT_ACTOR);
              // "[member]'s strength is exhausted..."
              FieldMessage_ShowFar(MsgFieldStrengthExhausted,
                                   FIELD_MESSAGE_ZOOM);
            }
            if (state->survivorCount == 0) {
              // The whole party fell: back to the last sanctum with 1 HP.
              struct Unit *vitals;

              if (gMainRoutineState.leaderActorId == 0) {
                if (GameFlag_GetFar(FLAG_CHILDHOOD) != 0)
                  Actor_SelectAnimationFar(actor,
                                           ISAAC_CHILD_ANIMATION_STAND_BRACED);
                else
                  Actor_SelectAnimationFar(actor, ANIM_LEADER_LIE_FLAT);
              } else {
                Actor_SelectAnimationFar(actor, ANIM_COLLAPSE);
              }
              Sound_PlayFar(MUS_DEFEATED);
              // "Isaac's party has been annihilated..."
              FieldMessage_ShowFar(MsgFieldPartyAnnihilated,
                                   FIELD_MESSAGE_ZOOM);
              vitals = Unit_GetFar(gMainRoutineState.leaderActorId);
              vitals->hp = 1;
              Unit_UpdateGauges(vitals);
              gMainRoutineState.next = gMainRoutineState.restartScene;
              gMainRoutineState.entrance = gMainRoutineState.restartEntrance;
              WorldMap_Unfreeze();
              result = EXIT_KEEP_DESTINATION;
              goto done;
            }
            WorldMap_Unfreeze();
          } else if (state->raisedTrigger == FIELD_RAISED_TRIGGER_MAP_SCREEN) {
            Leader_StopMoving();
            MapScreen_Show(RESOURCE_WORLD_MAP_PICTURE);
          } else if (state->raisedTrigger ==
                     FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM) {
            Leader_StopMoving();
            WorldMap_ZoomOutWhileLHeld();
          } else {
            Scene_RunRaisedTrigger(state->raisedTrigger);
          }
          state->busy = 0;
          state->raisedTrigger = 0;
        } else if (state->encounter != 0) {
          // A random encounter: save the actors and go to the battle.
          Leader_StopMoving();
          Actor_Snapshot();
          gMainRoutineState.returnMusic = -1;
          gMainRoutineState.next = SEQUENCE_BATTLE;
          gMainRoutineState.entrance = state->encounter;
          state->exit = EXIT_KEEP_DESTINATION;
          Field_StartEncounterTransition(state->encounter);
          state->encounterBias = 0;
          state->encounter = 0;
          gMainRoutineState.encounterSteps = 0;
        } else if (state->stepTrigger != 0) {
          state->busy = 1;
          Scene_RunStepTriggerOrExit(state->stepTrigger);
          state->busy = 0;
          state->stepTrigger = 0;
        } else if (state->pushCell != 0) {
          Field_RunPushCell(state->pushCell);
          state->pushCell = 0;
        } else if (state->aButton != 0) {
          // Examine the actor or cell ahead, or open the menu if neither
          // has an event.
          s32 object;

          AreaNameWindow_HideFar();
          object = Field_FindNearbyTalkTarget(gMainRoutineState.leaderActorId);
          // mode, done with, holds whether there is something to talk to, then
          // the cell behavior ahead.
          mode = 0;
          if (object != -1) {
            mode = Scene_FindActorTalkTrigger(object) != 0;
          }
          if (mode != 0) {
            state->talk = object | FIELD_TALK_REQUEST;
            state->fieldMenu = 0;
          } else {
            mode = Actor_GetTriggerCellAheadOfCurrent();
            if (mode != 0) {
              state->examineCell = mode;
              state->fieldMenu = 0;
            } else {
              state->fieldMenu = 1;
            }
          }
          state->aButton = 0;
        } else if (state->fieldMenu != 0) {
          // The field menu (in debug mode, B + Select opens the debug flag
          // editor).
          AreaNameWindow_HideFar();
          Leader_StopMoving();
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          WorldMap_Freeze();
          GameFlag_SetFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0 &&
              (gKeysHeld & KEY_SELECT) != 0) {
            DebugFlagEditor_RunFar();
          } else if (GameFlag_GetFar(FLAG_MENUS_RAISE_TRIGGER) != 0) {
            state->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
          } else {
            state->effectTimerRunning = 0;
            FieldMenu_RunFar();
            state->effectTimerRunning = 1;
          }
          WorldMap_Unfreeze();
          GameFlag_ClearFar(FLAG_MENU_OPEN);
          GameFlag_RefreshLureCapFar();
          state->fieldMenu = 0;
        } else if ((s16)state->talk != 0) {
          WorldMap_Freeze();
          Scene_TalkToActor(state->talk & (FIELD_TALK_REQUEST - 1));
          WorldMap_Unfreeze();
          state->talk = 0;
        } else if (state->examineCell != 0) {
          WorldMap_Freeze();
          Field_ExamineCell(state->examineCell);
          WorldMap_Unfreeze();
          state->examineCell = 0;
        } else if (state->psynergy != 0) {
          AreaNameWindow_HideFar();
          WorldMap_Freeze();
          Field_UsePsynergy(state->psynergy);
          WorldMap_Unfreeze();
          state->psynergy = 0;
        } else if (state->item != 0) {
          WorldMap_Freeze();
          Field_UseItem(state->item, state->itemSlot);
          WorldMap_Unfreeze();
          state->item = 0;
        } else if (state->pauseMenu != 0) {
          // The Pause menu (in debug mode, B opens the debug scene select and L
          // the palette editor).
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          Leader_StopMoving();
          WorldMap_Freeze();
          GameFlag_SetFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0) {
            DebugSceneSelect_RunFar();
          } else if (gDebugMode != 0 && (gKeysHeld & KEY_L) != 0) {
            PaletteEditor_Run();
          } else if (GameFlag_GetFar(FLAG_MENUS_RAISE_TRIGGER) != 0) {
            // As for the field menu above.
            state->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
          } else {
            Field_EndBusyAndReveal();
            AreaNameWindow_HideFar();
            gMainRoutineState.returnMusic = Sound_GetCurrentBgmFar();
            if (GameFlag_GetFar(FLAG_IN_BATTLE_ARENA) == 0) {
              // With fewer than PAUSE_MENU_VRAM_BLOCKS of the sprite VRAM
              // blocks free, sprite drawing stays suspended while the menu
              // runs, and a frame passes so that the field's sprites give
              // their VRAM back before it opens.
              struct ActorSystemState *system = gActorSystemState;
              u8 *owner = gVramCacheBlockOwners;
              s32 free = 0;
              s32 n;

              for (n = 0; n < VRAM_BLOCK_COUNT; n++) {
                if (*owner++ == VRAM_BLOCK_OWNER_FREE)
                  free++;
              }
              if (free - PAUSE_MENU_VRAM_BLOCKS < 0) {
                system->suspended = 1;
                wait(1);
              }
              PauseMenu_RunFar(0);
              system->suspended = 0;
            } else {
              // "The Pause menu is not available now."
              FieldMessage_ShowFar(MsgFieldPauseUnavailable,
                                   FIELD_MESSAGE_ZOOM);
            }
          }
          WorldMap_Unfreeze();
          GameFlag_ClearFar(FLAG_MENU_OPEN);
          state->pauseMenu = 0;
        }
      }

      // Hand the leader back to the player, as climbing, carried through the
      // air (only a key check runs), on the World Map or walking, and run the
      // walking update until the next request.
      GameFlag_ClearFar(FLAG_FIELD_HANDLING_REQUESTS);
      actor = state->actors[gMainRoutineState.leaderActorId];
      if (actor != NULL) {
        if (gMainRoutineState.movementMode == LEADER_MOVEMENT_AIRBORNE)
          Actor_StartKeyCheckFar(actor);
        else if (gMainRoutineState.movementMode == LEADER_MOVEMENT_CLIMB)
          Actor_StartMoveByKeysFar(actor);
        else if (state->mode == ACTOR_SYSTEM_WORLD_MAP)
          Actor_StartWorldMapControlFar(actor);
        else
          Actor_StartPlayerControlFar(actor);
      }
      do {
        wait(1);
        actor = state->actors[gMainRoutineState.leaderActorId];
        if (gDebugMode == 0 || GameFlag_GetFar(FLAG_DEBUG_STEPS_OFF) == 0)
          Field_UpdateWalking(actor->layer, actor->x.fixed, actor->y.fixed,
                              actor->z.fixed);
      } while (Field_CountRequests() == 0);
    }
  }

done:
  if (state->transitionActive != 0) {
    ScreenTransition_Finish(state->transition, state->transitionFrames);
    state->transitionActive = 0;
    wait(state->transitionFrames);
  }
  HeapSlot_Release(HEAP_SLOT_FIELD);
  return result;
}

/// Returns the cell behavior of the map cell 16 pixels ahead of the party
/// leader when there is something to search there: a fixed search spot
/// (CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL) whose ground is level with the
/// leader or up to 64 pixels above it, or a behavior the scene has an examine
/// trigger for. Returns 0 otherwise.
s32 Actor_GetTriggerCellAheadOfCurrent(void) {
  struct FieldState *state;
  struct MapGridScrollState *grid;
  struct Actor *leader;
  union PolarOffsetPosition probePosition;
  u8 *cell;
  s32 behavior;
  s32 groundHeight;
  s32 result;

  result = 0;
  leader = Actor_Lookup(gMainRoutineState.leaderActorId);
  state = gFieldState;
  grid = gMapGridState[0];
  if (leader != 0) {
    probePosition.vector.x = leader->x.fixed;
    probePosition.vector.y = leader->y.fixed;
    probePosition.vector.z = leader->z.fixed;
    Vector_AddPolarOffset(toFIXED(16), leader->facing,
                          probePosition.coordinates);
    // World Map cell pictures are 32 pixels square, 32 to a row; map cells 16
    // pixels square, 128 to a row, four bytes each.
    if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
      cell = Ram_CellPictures +
             ((((probePosition.vector.x / toFIXED(32)) & 31) +
               (((probePosition.vector.z / toFIXED(32)) & 31) << 5))
              << 2);
    } else {
      u8 *cells = (u8 *)grid->layers[MAPGRID_LAYER_GROUND].cells;

      cell =
          cells + (((probePosition.vector.x / toFIXED(MAPGRID_CELL_PIXELS)) +
                    ((probePosition.vector.z / toFIXED(MAPGRID_CELL_PIXELS)) *
                     MAPGRID_SIZE))
                   << 2);
    }
    behavior = cell[2];
    if (behavior >= CELL_BEHAVIOR_JAR && behavior <= CELL_BEHAVIOR_WELL) {
      groundHeight = MapGrid_GetTerrainHeightFar(
          leader->layer, probePosition.vector.x, probePosition.vector.z);
      if (groundHeight >= leader->y.fixed &&
          groundHeight <= leader->y.fixed + toFIXED(64)) {
        result = behavior;
      }
    } else if (Scene_FindTrigger(TRIGGER_EXAMINE, behavior) != 0) {
      result = behavior;
    }
  }
  return result;
}

/// Places the party at the entrance it came in by (the first matching
/// entry, or the scene's first one) unless it keeps its position, and sets the
/// map's scroll bounds, at least a screen (240 x 160 pixels) wide and tall.
void Scene_ApplyEntrance(void) {
  struct MapGridScrollState *grid = gMapGridState[0];
  s32 entrance = gMainRoutineState.entrance;
  const struct SceneEntrance *entranceEntry = gOverlayArea.entrances();
  s32 foundEntrance = 0;

  for (; entranceEntry->entrance != -1; entranceEntry++) {
    if (entranceEntry->entrance == entrance &&
        (entranceEntry->requiredFlag == -1 ||
         GameFlag_GetFar(entranceEntry->requiredFlag))) {
      foundEntrance = 1;
      break;
    }
  }
  if (!foundEntrance)
    entranceEntry = gOverlayArea.entrances();

  // Camera bounds apply even when returning to a saved party position.
  if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
    gMainRoutineState.x = INTtoFIXED(entranceEntry->x);
    gMainRoutineState.y = INTtoFIXED(entranceEntry->y);
    gMainRoutineState.z = INTtoFIXED(entranceEntry->z);
    gMainRoutineState.heading = entranceEntry->facing;
    gMainRoutineState.layer = 0;
  }

  if (entranceEntry->cameraLeft != -1)
    grid->minX = INTtoFIXED(entranceEntry->cameraLeft);
  if (entranceEntry->cameraTop != -1)
    grid->minY = INTtoFIXED(entranceEntry->cameraTop);
  if (entranceEntry->cameraRight != -1)
    grid->maxX = INTtoFIXED(entranceEntry->cameraRight);
  if (entranceEntry->cameraBottom != -1)
    grid->maxY = INTtoFIXED(entranceEntry->cameraBottom);

  if (grid->minX + toFIXED(DISPLAY_WIDTH) > grid->maxX)
    grid->minX = grid->maxX - toFIXED(DISPLAY_WIDTH);
  if (grid->minY + toFIXED(DISPLAY_HEIGHT) > grid->maxY)
    grid->minY = grid->maxY - toFIXED(DISPLAY_HEIGHT);
}

#undef HAZARD_LIMIT_DEFAULT
#undef FIELD_TALK_REQUEST
#undef ANIM_LEADER_LIE_FLAT
#undef ISAAC_CHILD_ANIMATION_STAND_BRACED
#undef PAUSE_MENU_VRAM_BLOCKS

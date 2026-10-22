/// @file
/// Running a field scene until the party leaves it, and its entrance.
// Part of object.c.

#include "../object_internal.h"

/// The object system's work (struct ObjectSystemWork, gMenuCtrlWork in
/// HEAP_SLOT_MENU_CONTROL), seen as the flag that suspends sprite projection
/// while the menu opens.
struct MenuCtrl {
  u8 unk00[4];
  /// ObjectSystemWork's suspended: while nonzero, sprites not held are not
  /// drawn and give up their VRAM (object_camera.c, sprite_draw.c).
  u16 wait;
};

/// Runs the current field scene, which the party entered by entrance, until it
/// leaves; returns the exit kind it left by, for Scene_TakeExit, or EXIT_NONE
/// when the next scene is already in gGameState: a random battle, a
/// party that fell outside battle, Retreat or an event's destination. Sets the
/// scene up (map, actors, encounters, return points), runs its initialization,
/// fades it in, then runs the field loop: the party walks (Field_ProcessStep
/// each frame) until a request is made, and then the requests are handled
/// until none is left.
///
/// Called from Game_Run's main loop for every field scene, which
/// passes the result to Scene_TakeExit. Allocates the field work in
/// HEAP_SLOT_EVENT, cleared, and frees it on the way out, with every
/// block the scene took after it from the same pool; sets gGameState's default,
/// next and defeat destinations, and clears FLAG_IN_BATTLE and, once the scene
/// has started, FLAG_KEEP_PARTY_POSITION.
s32 Field_RunScene(s32 entrance) {
  struct FieldWork *work;
  struct FieldActor *actor;
  s32 mode;
  s32 result = 0;
  s32 code;

  // Set up the scene.
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  Dma_Fill32(0, work, sizeof(struct FieldWork));
  GameFlag_ClearBitFar(FLAG_IN_BATTLE);
  gGameState.defaultScene = gGameState.scene;
  gGameState.defaultEntrance = gGameState.entrance;
  gGameState.nextScene = -1;
  gGameState.nextEntrance = -1;
  gGameState.defeatScene = -1;
  gGameState.defeatEntrance = -1;
  Scheduler_ResetTaskTable();
  Djinn_ResolvePendingEvent(0);
  if (gGameState.map == (s32)&SceneId_Clear) {
    Map_InitializePerspectiveSceneFar();
    gGameState.perspectiveScene = 1;
    mode = MODE_WORLD_MAP;
    Encounter_SelectSceneZones(0);
  } else {
    Map_LoadLayeredSceneFar(gGameState.map);
    mode = MODE_LAYERED_MAP;
    Encounter_SelectSceneZones(1);
  }
  work->mode = mode;
  ObjectSystem_InitializeFar(mode);
  UiWork_InitializeWithResourceCountersFar();
  Field_ClearRequests();
  work->events = gOverlayArea.events();
  Scene_ApplyEntrance();
  ObjectTable_ResetForObject(
      (struct ScenePlacement *)gOverlayArea.placements());
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) != 0)
    ObjectTable_Restore();
  if (gGameState.pendingDjinnEvent != 0)
    FieldObject_PlaceSceneActors();
  if (mode == MODE_WORLD_MAP) {
    Map_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gGameState.x, gGameState.z);
  } else {
    Map_CenterOnFollowedPointFar();
  }
  Field_PlaceMapMarkers();
  ColorBuffer_Initialize();
  ColorBuffer_SetTarget(0x10000, 0);
  // The default fade is from the backdrop colour over 16 frames; the scene's
  // initialization may choose another.
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 16;
  work->transitionActive = 0;
  // Field_ProcessStep scales the leader's speed by this (16.16) into the
  // distance counted towards PP and poison ticks: 0x10000 / 0x199 makes a
  // tick about every 160 pixels walked. Towns turn it off and
  // block Retreat.
  work->walkingDistanceScale = 0x199;
  work->walkingDistance = 0;
  if (gGameState.sceneVariant == SCENE_VARIANT_TOWN) {
    work->walkingDistanceScale = 0;
    GameFlag_SetBitFar(FLAG_RETREAT_BLOCKED);
  }
  work->previousTile = (struct MapCell *)Ram_MapCellBuffer;
  // A fresh entry (not a return from battle) resets the hazard count to its
  // defaults (damage 1 every 16 steps), the stopped actor and Cloak.
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) == 0) {
    Scene_SetDefeatReturnPoint();
    gGameState.hazardLimit = 16;
    gGameState.hazardMode = HAZARD_COUNT_ON_CELLS;
    gGameState.hazardDamage = 1;
    gGameState.pausedObject = -1;
    gGameState.cloaked = 0;
  }
  FieldFx_ResumeCloakCheck();
  work->queuedSound = -1;
  gOverlayArea.initialize();

  // The scene's initialization may already have taken an exit.
  code = work->exit;
  if (code != 0) {
    result = code;
    work->exit = 0;
  } else {
    GameFlag_ClearBitFar(FLAG_KEEP_PARTY_POSITION);
    // Fade in, show the place name and give what was left pending.
    if (Field_CountRequests() == 0) {
      if (work->transitionActive == 0) {
        DisplayTransition_Start(work->transition, work->transitionFrames);
        work->transitionActive = 1;
        PLTT_BACKDROP = 0;
        // An arrival the last scene asked for
        // (ObjectEffect_RunPendingFlagEvent) plays through the fade; with none,
        // wait out half of it.
        if (ObjectEffect_RunPendingFlagEvent() == 0)
          WaitFrames((work->transitionFrames + 1) / 2);
      }
      if (GameFlag_TestFar(FLAG_SHOW_LOCATION_NAME) != 0) {
        GameFlag_ClearBitFar(FLAG_SHOW_LOCATION_NAME);
        UiTimedNotice_CreateFar(gGameState.scene);
      }
      if (gGameState.pendingDjinnEvent != 0) {
        Djinn_ResolvePendingEvent(gGameState.pendingDjinnEvent, 1);
        gGameState.pendingDjinnEvent = 0;
      }
      if (gGameState.pendingItem != 0) {
        WorldMap_BeginEvent();
        if (Party_CheckItemLimit(gGameState.pendingItem) == 0)
          PartyInventory_GiveItem(gGameState.pendingItem, 0);
        WorldMap_EndEvent();
        gGameState.pendingItem = 0;
      }
    }

    for (;;) {
      // Note where the leader is, for the requests to use.
      GameFlag_SetBitFar(FLAG_FIELD_HANDLING_REQUESTS);
      actor = work->actors[gGameState.selectedActor];
      ObjectDispatch_StartIdleFar((struct DispatchObject *)actor);
      gGameState.x = actor->x.fixed;
      gGameState.y = 0;
      gGameState.z = actor->z.fixed;
      gGameState.heading = actor->facing;
      gGameState.turn = actor->terrainId;

      // Handle the pending requests, most urgent first.
      while (Field_CountRequests() != 0) {
        s32 next = work->exit;

        if (next != 0) {
          result = next;
          work->exit = 0;
          goto done;
        }
        if (work->event != 0) {
          work->busy = 1;
          if (work->event == FIELD_EVENT_PARTY_FELL) {
            // Characters fell to poison or hazardous ground.
            u32 i;

            Leader_StopMoving();
            WorldMap_BeginEvent();
            for (i = 0; i < work->fallenCount; i++) {
              ObjectDispatch_SelectAnimationFar(actor, 22);
              UiWork_PushValueSlotFar(work->fallenOwners[i], 1);
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldStrengthExhausted,
                                                     1);
            }
            if (work->survivorCount == 0) {
              // The whole party fell: back to the last sanctum with 1 HP.
              struct BattleUnit *vitals;

              if (gGameState.selectedActor == 0) {
                if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0)
                  ObjectDispatch_SelectAnimationFar(actor, 21);
                else
                  ObjectDispatch_SelectAnimationFar(actor, 37);
              } else {
                ObjectDispatch_SelectAnimationFar(actor, 19);
              }
              AudioCommand_PlayFar(MUS_DEFEAT);
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldPartyAnnihilated,
                                                     1);
              vitals = UnitGetFar(gGameState.selectedActor);
              vitals->hp = 1;
              Unit_UpdateGauges(vitals);
              gGameState.scene = gGameState.savedScene;
              gGameState.entrance = gGameState.savedEntrance;
              WorldMap_EndEvent();
              result = EXIT_NONE;
              goto done;
            }
            WorldMap_EndEvent();
          } else if (work->event == FIELD_EVENT_WORLD_MAP_PICTURE) {
            Leader_StopMoving();
            Map_ShowWorldMap((s32)&ResourceId_WorldMapPicture);
          } else if (work->event == FIELD_EVENT_WORLD_MAP_ZOOM) {
            Leader_StopMoving();
            FieldFx_RunVisibilityTransition();
          } else {
            Scene_RunRaisedEvent(work->event);
          }
          work->busy = 0;
          work->event = 0;
        } else if (work->battle != 0) {
          // A random encounter: save the actors and go to the battle scene.
          // Field_StartEncounterTransition closes the screen itself and
          // clears transitionActive, so the scene ends without another fade.
          Leader_StopMoving();
          ObjectTable_Snapshot();
          gGameState.returnCue = -1;
          gGameState.scene = SCENE_BATTLE;
          gGameState.entrance = work->battle;
          work->exit = EXIT_NONE;
          Field_StartEncounterTransition(work->battle);
          work->encounterBias = 0;
          work->battle = 0;
          gGameState.encounterSteps = 0;
        } else if (work->action != 0) {
          work->busy = 1;
          Scene_RunTouchOrExit(work->action);
          work->busy = 0;
          work->action = 0;
        } else if (work->pushTile != 0) {
          Field_RunPushTileEvent(work->pushTile);
          work->pushTile = 0;
        } else if (work->examine != 0) {
          // Examine the actor or tile ahead, or open the menu if neither has an
          // event.
          s32 object;

          UiTimedNotice_CloseIfActiveFar();
          object = Field_FindNearbyTalkTarget(gGameState.selectedActor);
          mode = 0;
          if (object != -1) {
            mode = Scene_FindActorTalkEvent(object) != 0;
          }
          if (mode != 0) {
            // 0x1000 keeps actor 0's request non-zero; the handler masks it.
            work->talk = object | 0x1000;
            work->menu = 0;
          } else {
            mode = Object_GetTriggerTileAheadOfCurrent();
            if (mode != 0) {
              work->tile = mode;
              work->menu = 0;
            } else {
              work->menu = 1;
            }
          }
          work->examine = 0;
        } else if (work->menu != 0) {
          // The main menu (B + Select in debug mode opens the debug menu).
          UiTimedNotice_CloseIfActiveFar();
          Leader_StopMoving();
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          WorldMap_BeginEvent();
          GameFlag_SetBitFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0 &&
              (gKeysHeld & KEY_SELECT) != 0) {
            Menu_RunSelectionWithCursorObjectFar();
          } else if (GameFlag_TestFar(FLAG_MENUS_RAISE_EVENT) != 0) {
            work->event = FIELD_EVENT_MENUS_RAISED;
          } else {
            work->effectTimerRunning = 0;
            Menu_RunTopSelectionFar();
            work->effectTimerRunning = 1;
          }
          WorldMap_EndEvent();
          GameFlag_ClearBitFar(FLAG_MENU_OPEN);
          GameFlag_RefreshLureCapFar();
          work->menu = 0;
        } else if ((s16)work->talk != 0) {
          WorldMap_BeginEvent();
          Scene_TalkToActor(work->talk & 0xfff);
          WorldMap_EndEvent();
          work->talk = 0;
        } else if (work->tile != 0) {
          WorldMap_BeginEvent();
          Field_SearchTile(work->tile);
          WorldMap_EndEvent();
          work->tile = 0;
        } else if (work->psynergy != 0) {
          UiTimedNotice_CloseIfActiveFar();
          WorldMap_BeginEvent();
          Field_UsePsynergy(work->psynergy);
          WorldMap_EndEvent();
          work->psynergy = 0;
        } else if (work->item != 0) {
          WorldMap_BeginEvent();
          Field_UseItem(work->item, work->itemArgument);
          WorldMap_EndEvent();
          work->item = 0;
        } else if (work->start != 0) {
          // The pause menu (in debug mode, B opens the debug menu and L the
          // palette editor).
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          Leader_StopMoving();
          WorldMap_BeginEvent();
          GameFlag_SetBitFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0) {
            Menu_RunSelectionFar();
          }
          /* FAKEMATCH(volatile): gDebugMode read through a volatile pointer,
             so this second test loads it again; as gDebugMode, 48 of 965
             instructions differ in tbs-en. */
          else if (*(volatile u8 *)&gDebugMode != 0 &&
                   (gKeysHeld & KEY_L) != 0) {
            Debug_RunPaletteEditor();
          } else if (GameFlag_TestFar(FLAG_MENUS_RAISE_EVENT) != 0) {
            // As for the main menu above.
            work->event = FIELD_EVENT_MENUS_RAISED;
          } else {
            Field_EndBusyAndReveal();
            UiTimedNotice_CloseIfActiveFar();
            gGameState.returnCue = AudioCommand_GetSecondaryStateByteFar();
            if (GameFlag_TestFar(FLAG_IN_BATTLE_ARENA) == 0) {
              // With fewer than 136 of the 512 sprite VRAM blocks (64 bytes
              // each) free, sprite drawing stays suspended while the menu
              // runs, and a frame passes so that the field's sprites give
              // their VRAM back before it opens.
              struct MenuCtrl *ctrl = (struct MenuCtrl *)gMenuCtrlWork;
              u8 *owner = gResourceBlockOwners;
              s32 free = 0;
              s32 n = 512;

              do {
                if (*owner++ == 0xff)
                  free++;
              } while (--n != 0);
              if (free - 136 < 0) {
                ctrl->wait = 1;
                WaitFrames(1);
              }
              Menu_RunWorkspaceResultLoopFar(0);
              ctrl->wait = 0;
            } else {
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldPauseUnavailable,
                                                     1);
            }
          }
          WorldMap_EndEvent();
          GameFlag_ClearBitFar(FLAG_MENU_OPEN);
          work->start = 0;
        }
      }

      // Hand the leader back to the player, as climbing, carried through the
      // air (only a key check runs), on the world map or walking, and step
      // until the next request.
      GameFlag_ClearBitFar(FLAG_FIELD_HANDLING_REQUESTS);
      actor = work->actors[gGameState.selectedActor];
      if (actor != NULL) {
        if (gGameState.movementMode == LEADER_MOVEMENT_AIRBORNE)
          ObjectDispatch_StartKeyCheckFar((struct DispatchObject *)actor);
        else if (gGameState.movementMode == LEADER_MOVEMENT_CLIMB)
          ObjectDispatch_StartMoveByKeysFar((struct DispatchObject *)actor);
        else if (work->mode == MODE_WORLD_MAP)
          ObjectDispatch_StartWorldMapControlFar(
              (struct DispatchObject *)actor);
        else
          ObjectDispatch_StartPlayerControlFar((struct DispatchObject *)actor);
      }
      do {
        WaitFrames(1);
        actor = work->actors[gGameState.selectedActor];
        if (gDebugMode == 0 || GameFlag_TestFar(FLAG_DEBUG_STEPS_OFF) == 0)
          Field_ProcessStep(actor->terrainId, actor->x.fixed, actor->y.fixed,
                            actor->z.fixed);
      } while (Field_CountRequests() == 0);
    }
  }

done:
  if (work->transitionActive != 0) {
    DisplayTransition_Finish(work->transition, work->transitionFrames);
    work->transitionActive = 0;
    WaitFrames(work->transitionFrames);
  }
  Runtime_ReleaseHeapBlock(HEAP_SLOT_EVENT);
  return result;
}

/// A field actor (struct FieldActor), seen with its position as one vector.
struct Object {
  u8 pad0[6];
  u16 angle;               ///< 1/65536 turns
  struct EffectVector pos; ///< 16.16 fixed-point pixels
  u8 pad1[14];
  u8 kind; ///< ObjectRuntime's terrainId: the map layer of its ground
  u8 pad2[0x2d];
  struct AnimationObject *sprite;
};

/// Returns the collision code of the map cell 16 pixels ahead of the party
/// leader when there is something to search there: a fixed search spot
/// (MAP_CELL_JAR to MAP_CELL_WELL) whose ground is level with the leader or up
/// to 64 pixels above it, or a code the scene has a search event for. Returns 0
/// otherwise.
///
/// Called by Field_RunScene for the A button (a non-zero result becomes
/// work->tile, for Field_SearchTile), and by Menu_RunTopSelection's search
/// entry through Object_GetTriggerTileAheadOfCurrentFar. Changes nothing.
s32 Object_GetTriggerTileAheadOfCurrent(void) {
  u8 *workBytes;
  u8 *mapBytes;
  struct Object *leader;
  union PolarOffsetPosition probePosition;
  u8 *cell;
  u8 *cells;
  s32 collisionCode;
  s32 terrainHeight;
  s32 result;

  result = 0;
  leader = ObjectTable_Get(gGameState.selectedActor);
  workBytes = *(u8 **)Ram_EventWork;
  mapBytes = *(u8 **)Ram_MapWork;
  if (leader != 0) {
    probePosition.vector.x = leader->pos.x;
    probePosition.vector.y = leader->pos.y;
    probePosition.vector.z = leader->pos.z;
    Vector_AddPolarOffset(0x100000, leader->angle, probePosition.coordinates);
    // World map blocks are 32 pixels square, 32 to a row; map cells 16
    // pixels square, 128 to a row, four bytes each.
    if (((struct FieldWork *)workBytes)->mode == MODE_WORLD_MAP) {
      cell =
          Ram_MapBlocks + ((((probePosition.vector.x / 0x200000) & 31) +
                            (((probePosition.vector.z / 0x200000) & 31) << 5))
                           << 2);
    } else {
      // MAP_LAYER_GROUND's cells, MapState's layers[0].cells at 0x130.
      cells = *(u8 **)(mapBytes + 0x130);
      cell = cells + (((probePosition.vector.x / 0x100000) +
                       ((probePosition.vector.z / 0x100000) << 7))
                      << 2);
    }
    collisionCode = cell[2];
    if (collisionCode >= MAP_CELL_JAR && collisionCode <= MAP_CELL_WELL) {
      terrainHeight = Map_GetTerrainHeightFar(
          leader->kind, probePosition.vector.x, probePosition.vector.z);
      if (terrainHeight >= leader->pos.y &&
          terrainHeight <= leader->pos.y + 0x400000) {
        result = collisionCode;
      }
    } else if (Scene_FindEvent(EVENT_SEARCH, collisionCode) != 0) {
      result = collisionCode;
    }
  }
  return result;
}

/// Places the party at the entrance it came in by (the first matching entry, or
/// the scene's first one) unless it keeps its position, and sets the map's
/// scroll bounds, at least a screen (240 x 160 pixels) wide and tall.
///
/// Called by Field_RunScene once the scene's events are set. Writes
/// gGameState's position, heading and turn, and the map's minX to maxY.
void Scene_ApplyEntrance(void) {
  struct MapScrollWork *map = gMapWork[0];
  s32 entrance = gGameState.entrance;
  const struct SceneEntrance *entranceEntry = gOverlayArea.entrances();
  s32 foundEntrance = 0;

  while (entranceEntry->entrance != -1) {
    if (entranceEntry->entrance == entrance &&
        (entranceEntry->requiredFlag == -1 ||
         GameFlag_TestFar(entranceEntry->requiredFlag))) {
      foundEntrance = 1;
      break;
    }
    entranceEntry++;
  }
  if (!foundEntrance)
    entranceEntry = gOverlayArea.entrances();

  // Camera bounds apply even when returning to a saved party position.
  if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
    gGameState.x = REGION_FIXED(entranceEntry->x);
    gGameState.y = REGION_FIXED(entranceEntry->y);
    gGameState.z = REGION_FIXED(entranceEntry->z);
    gGameState.heading = entranceEntry->facing;
    gGameState.turn = 0;
  }

  if (entranceEntry->cameraLeft != -1)
    map->minX = REGION_FIXED(entranceEntry->cameraLeft);
  if (entranceEntry->cameraTop != -1)
    map->minY = REGION_FIXED(entranceEntry->cameraTop);
  if (entranceEntry->cameraRight != -1)
    map->maxX = REGION_FIXED(entranceEntry->cameraRight);
  if (entranceEntry->cameraBottom != -1)
    map->maxY = REGION_FIXED(entranceEntry->cameraBottom);

  if (map->minX + (240 << 16) > map->maxX)
    map->minX = map->maxX - (240 << 16);
  if (map->minY + (160 << 16) > map->maxY)
    map->minY = map->maxY - (160 << 16);
}

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
/// leaves; returns the exit kind it left by, for Map_TakeWarp, or WARP_NONE
/// when the next scene is already in gGameState: a random battle, a
/// party that fell outside battle, Retreat or an event's destination. Sets the
/// scene up (map, actors, encounters, return points), runs its initialization,
/// fades it in, then runs the field loop: the party walks (Field_ProcessStep
/// each frame) until a request is made, and then the requests are handled
/// until none is left.
///
/// Called from Game_Run's main loop for every field scene, which
/// passes the result to Map_TakeWarp. Allocates the field work in
/// HEAP_SLOT_EVENT, cleared, and frees it on the way out, with every
/// block the scene took after it from the same pool; sets gGameState's default,
/// next and defeat destinations, and clears FLAG_IN_BATTLE and, once the scene
/// has started, FLAG_KEEP_PARTY_POSITION.
s32 Field_RunMap(s32 warpDest) {
  struct FieldWork *work;
  struct ObjectEvent *objectEvent;
  s32 mode;
  s32 result = 0;
  s32 code;

  // Set up the scene.
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  Dma_Fill32(0, work, sizeof(struct FieldWork));
  GameFlag_ClearBitFar(FLAG_IN_BATTLE);
  gGameState.defaultMap = gGameState.mapId;
  gGameState.defaultWarpDest = gGameState.warpDest;
  gGameState.nextMap = -1;
  gGameState.nextWarpDest = -1;
  gGameState.defeatMap = -1;
  gGameState.defeatWarpDest = -1;
  Scheduler_ResetTaskTable();
  Djinn_ResolvePendingEvent(0);
  if (gGameState.layoutId == (s32)&MapId_Clear) {
    WorldMap_InitializePerspectiveFar();
    gGameState.perspectiveMap = 1;
    mode = MODE_WORLD_MAP;
    Encounter_SelectMapZones(0);
  } else {
    MapGrid_LoadLayoutFar(gGameState.layoutId);
    mode = MODE_LAYERED_MAP;
    Encounter_SelectMapZones(1);
  }
  work->mode = mode;
  ObjectSystem_InitializeFar(mode);
  UiWork_InitializeWithResourceCountersFar();
  Field_ClearRequests();
  work->events = gOverlayArea.events();
  Map_ApplyWarpDest();
  ObjectTable_ResetForObject(
      (struct ObjectEventTemplate *)gOverlayArea.objectEventTemplates());
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) != 0)
    ObjectTable_Restore();
  if (gGameState.pendingDjinnEvent != 0)
    ObjectEvent_PlaceMapObjectEvents();
  if (mode == MODE_WORLD_MAP) {
    WorldMap_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gGameState.x, gGameState.z);
  } else {
    MapGrid_CenterOnFollowedPointFar();
  }
  Field_PlaceMapMarkers();
  ColorBuffer_Initialize();
  ColorBuffer_SetTarget(0x10000, 0);
  // The default fade is from the backdrop colour over 16 frames; the scene's
  // initialization may choose another.
  work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 16;
  work->transitionActive = 0;
  // Field_ProcessStep scales the leader's speed by this (16.16) into the
  // distance counted towards PP and poison ticks: 0x10000 / 0x199 makes a
  // tick about every 160 pixels walked. Towns turn it off and
  // block Retreat.
  work->walkingDistanceScale = 0x199;
  work->walkingDistance = 0;
  if (gGameState.mapType == MAP_TYPE_TOWN) {
    work->walkingDistanceScale = 0;
    GameFlag_SetBitFar(FLAG_RETREAT_BLOCKED);
  }
  work->previousTile = (struct MapGridCell *)Ram_MapGridCellBuffer;
  // A fresh entry (not a return from battle) resets the hazard count to its
  // defaults (damage 1 every 16 steps), the stopped actor and Cloak.
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) == 0) {
    Map_SetDefeatReturnPoint();
    gGameState.hazardLimit = 16;
    gGameState.hazardMode = HAZARD_COUNT_ON_CELLS;
    gGameState.hazardDamage = 1;
    gGameState.pausedObject = -1;
    gGameState.cloaked = 0;
  }
  FieldEffect_ResumeCloakCheck();
  work->queuedSound = -1;
  gOverlayArea.initialize();

  // The scene's initialization may already have taken an exit.
  code = work->warp;
  if (code != 0) {
    result = code;
    work->warp = 0;
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
        UiTimedNotice_CreateFar(gGameState.mapId);
      }
      if (gGameState.pendingDjinnEvent != 0) {
        Djinn_ResolvePendingEvent(gGameState.pendingDjinnEvent, 1);
        gGameState.pendingDjinnEvent = 0;
      }
      if (gGameState.pendingItem != 0) {
        WorldMap_StartScript();
        if (Party_CheckItemLimit(gGameState.pendingItem) == 0)
          PartyInventory_GiveItem(gGameState.pendingItem, 0);
        WorldMap_EndScript();
        gGameState.pendingItem = 0;
      }
    }

    for (;;) {
      // Note where the leader is, for the requests to use.
      GameFlag_SetBitFar(FLAG_FIELD_HANDLING_REQUESTS);
      objectEvent = work->objectEvents[gGameState.selectedObjectEvent];
      ObjectDispatch_StartIdleFar((struct DispatchObject *)objectEvent);
      gGameState.x = objectEvent->x.fixed;
      gGameState.y = 0;
      gGameState.z = objectEvent->z.fixed;
      gGameState.heading = objectEvent->facing;
      gGameState.turn = objectEvent->terrainId;

      // Handle the pending requests, most urgent first.
      while (Field_CountRequests() != 0) {
        s32 next = work->warp;

        if (next != 0) {
          result = next;
          work->warp = 0;
          goto done;
        }
        if (work->event != 0) {
          work->busy = 1;
          if (work->event == FIELD_EVENT_PARTY_FELL) {
            // Characters fell to poison or hazardous ground.
            u32 i;

            Leader_StopMoving();
            WorldMap_StartScript();
            for (i = 0; i < work->fallenCount; i++) {
              ObjectDispatch_SelectAnimationFar(objectEvent, 22);
              UiWork_PushValueSlotFar(work->fallenOwners[i], 1);
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldStrengthExhausted,
                                                     1);
            }
            if (work->survivorCount == 0) {
              // The whole party fell: back to the last sanctum with 1 HP.
              struct BattleUnit *vitals;

              if (gGameState.selectedObjectEvent == 0) {
                if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0)
                  ObjectDispatch_SelectAnimationFar(objectEvent, 21);
                else
                  ObjectDispatch_SelectAnimationFar(objectEvent, 37);
              } else {
                ObjectDispatch_SelectAnimationFar(objectEvent, 19);
              }
              AudioCommand_PlayFar(MUS_DEFEAT);
              UiText_ShowPositionedMessageAndWaitFar(MsgFieldPartyAnnihilated,
                                                     1);
              vitals = Unit_GetFar(gGameState.selectedObjectEvent);
              vitals->hp = 1;
              Unit_UpdateGauges(vitals);
              gGameState.mapId = gGameState.savedMap;
              gGameState.warpDest = gGameState.savedWarpDest;
              WorldMap_EndScript();
              result = WARP_NONE;
              goto done;
            }
            WorldMap_EndScript();
          } else if (work->event == FIELD_EVENT_WORLD_MAP_PICTURE) {
            Leader_StopMoving();
            WorldMap_Show((s32)&ResourceId_WorldMapPicture);
          } else if (work->event == FIELD_EVENT_WORLD_MAP_ZOOM) {
            Leader_StopMoving();
            FieldEffect_RunVisibilityTransition();
          } else {
            Map_RunRaisedEvent(work->event);
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
          gGameState.mapId = MAP_BATTLE;
          gGameState.warpDest = work->battle;
          work->warp = WARP_NONE;
          Field_StartEncounterTransition(work->battle);
          work->encounterBias = 0;
          work->battle = 0;
          gGameState.encounterSteps = 0;
        } else if (work->action != 0) {
          work->busy = 1;
          Map_RunCoordEventOrWarp(work->action);
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
          object = Field_FindNearbyTalkTarget(gGameState.selectedObjectEvent);
          mode = 0;
          if (object != -1) {
            mode = Map_FindObjectEventTalkEvent(object) != 0;
          }
          if (mode != 0) {
            // 0x1000 keeps actor 0's request non-zero; the handler masks it.
            work->talk = object | 0x1000;
            work->menu = 0;
          } else {
            mode = ObjectEvent_GetTriggerTileAheadOfCurrent();
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
          WorldMap_StartScript();
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
          WorldMap_EndScript();
          GameFlag_ClearBitFar(FLAG_MENU_OPEN);
          GameFlag_RefreshLureCapFar();
          work->menu = 0;
        } else if ((s16)work->talk != 0) {
          WorldMap_StartScript();
          Map_TalkToObjectEvent(work->talk & 0xfff);
          WorldMap_EndScript();
          work->talk = 0;
        } else if (work->tile != 0) {
          WorldMap_StartScript();
          Field_SearchTile(work->tile);
          WorldMap_EndScript();
          work->tile = 0;
        } else if (work->psynergy != 0) {
          UiTimedNotice_CloseIfActiveFar();
          WorldMap_StartScript();
          Field_UsePsynergy(work->psynergy);
          WorldMap_EndScript();
          work->psynergy = 0;
        } else if (work->item != 0) {
          WorldMap_StartScript();
          Field_UseItem(work->item, work->itemArgument);
          WorldMap_EndScript();
          work->item = 0;
        } else if (work->start != 0) {
          // The pause menu (in debug mode, B opens the debug menu and L the
          // palette editor).
          AudioCommand_PlayFar(SE_MENU_CURSOR_MOVE);
          Leader_StopMoving();
          WorldMap_StartScript();
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
          WorldMap_EndScript();
          GameFlag_ClearBitFar(FLAG_MENU_OPEN);
          work->start = 0;
        }
      }

      // Hand the leader back to the player, as climbing, carried through the
      // air (only a key check runs), on the world map or walking, and step
      // until the next request.
      GameFlag_ClearBitFar(FLAG_FIELD_HANDLING_REQUESTS);
      objectEvent = work->objectEvents[gGameState.selectedObjectEvent];
      if (objectEvent != NULL) {
        if (gGameState.movementMode == LEADER_MOVEMENT_AIRBORNE)
          ObjectDispatch_StartKeyCheckFar((struct DispatchObject *)objectEvent);
        else if (gGameState.movementMode == LEADER_MOVEMENT_CLIMB)
          ObjectDispatch_StartMoveByKeysFar(
              (struct DispatchObject *)objectEvent);
        else if (work->mode == MODE_WORLD_MAP)
          ObjectDispatch_StartWorldMapControlFar(
              (struct DispatchObject *)objectEvent);
        else
          ObjectDispatch_StartPlayerControlFar(
              (struct DispatchObject *)objectEvent);
      }
      do {
        WaitFrames(1);
        objectEvent = work->objectEvents[gGameState.selectedObjectEvent];
        if (gDebugMode == 0 || GameFlag_TestFar(FLAG_DEBUG_STEPS_OFF) == 0)
          Field_ProcessStep(objectEvent->terrainId, objectEvent->x.fixed,
                            objectEvent->y.fixed, objectEvent->z.fixed);
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

/// A field actor (struct ObjectEvent), seen with its position as one vector.
struct Object {
  u8 pad0[6];
  u16 angle;                    ///< 1/65536 turns
  struct EffectVector position; ///< 16.16 fixed-point pixels
  u8 pad1[14];
  u8 kind; ///< ObjectRuntime's terrainId: the map layer of its ground
  u8 pad2[0x2d];
  struct AnimationObject *sprite;
};

/// Returns the collision code of the map cell 16 pixels ahead of the party
/// leader when there is something to search there: a fixed search spot
/// (MB_JAR to MB_WELL) whose ground is level with the leader or up
/// to 64 pixels above it, or a code the scene has a search event for. Returns 0
/// otherwise.
///
/// Called by Field_RunMap for the A button (a non-zero result becomes
/// work->tile, for Field_SearchTile), and by Menu_RunTopSelection's search
/// entry through ObjectEvent_GetTriggerTileAheadOfCurrentFar. Changes nothing.
s32 ObjectEvent_GetTriggerTileAheadOfCurrent(void) {
  u8 *workBytes;
  u8 *gridBytes;
  struct Object *leader;
  union PolarOffsetPosition probePosition;
  u8 *cell;
  u8 *cells;
  s32 collisionCode;
  s32 terrainHeight;
  s32 result;

  result = 0;
  leader = ObjectTable_Get(gGameState.selectedObjectEvent);
  workBytes = *(u8 **)Ram_EventWork;
  gridBytes = *(u8 **)Ram_MapGridWork;
  if (leader != 0) {
    probePosition.vector.x = leader->position.x;
    probePosition.vector.y = leader->position.y;
    probePosition.vector.z = leader->position.z;
    Vector_AddPolarOffset(0x100000, leader->angle, probePosition.coordinates);
    // World map blocks are 32 pixels square, 32 to a row; map cells 16
    // pixels square, 128 to a row, four bytes each.
    if (((struct FieldWork *)workBytes)->mode == MODE_WORLD_MAP) {
      cell =
          Ram_Metatiles + ((((probePosition.vector.x / 0x200000) & 31) +
                            (((probePosition.vector.z / 0x200000) & 31) << 5))
                           << 2);
    } else {
      // MAPGRID_LAYER_GROUND's cells, MapGridState's layers[0].cells at 0x130.
      cells = *(u8 **)(gridBytes + 0x130);
      cell = cells + (((probePosition.vector.x / 0x100000) +
                       ((probePosition.vector.z / 0x100000) << 7))
                      << 2);
    }
    collisionCode = cell[2];
    if (collisionCode >= MB_JAR && collisionCode <= MB_WELL) {
      terrainHeight = MapGrid_GetTerrainHeightFar(
          leader->kind, probePosition.vector.x, probePosition.vector.z);
      if (terrainHeight >= leader->position.y &&
          terrainHeight <= leader->position.y + 0x400000) {
        result = collisionCode;
      }
    } else if (Map_FindEvent(MAP_EVENT_BG, collisionCode) != 0) {
      result = collisionCode;
    }
  }
  return result;
}

/// Places the party at the entrance it came in by (the first matching entry, or
/// the scene's first one) unless it keeps its position, and sets the map's
/// scroll bounds, at least a screen (240 x 160 pixels) wide and tall.
///
/// Called by Field_RunMap once the scene's events are set. Writes
/// gGameState's position, heading and turn, and the map's minX to maxY.
void Map_ApplyWarpDest(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  s32 warpDest = gGameState.warpDest;
  const struct MapWarpDest *warpDestEntry = gOverlayArea.warpDests();
  s32 foundWarpDest = 0;

  while (warpDestEntry->warpDest != -1) {
    if (warpDestEntry->warpDest == warpDest &&
        (warpDestEntry->requiredFlag == -1 ||
         GameFlag_TestFar(warpDestEntry->requiredFlag))) {
      foundWarpDest = 1;
      break;
    }
    warpDestEntry++;
  }
  if (!foundWarpDest)
    warpDestEntry = gOverlayArea.warpDests();

  // Camera bounds apply even when returning to a saved party position.
  if (!GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION)) {
    gGameState.x = REGION_FIXED(warpDestEntry->x);
    gGameState.y = REGION_FIXED(warpDestEntry->y);
    gGameState.z = REGION_FIXED(warpDestEntry->z);
    gGameState.heading = warpDestEntry->facing;
    gGameState.turn = 0;
  }

  if (warpDestEntry->cameraLeft != -1)
    grid->minX = REGION_FIXED(warpDestEntry->cameraLeft);
  if (warpDestEntry->cameraTop != -1)
    grid->minY = REGION_FIXED(warpDestEntry->cameraTop);
  if (warpDestEntry->cameraRight != -1)
    grid->maxX = REGION_FIXED(warpDestEntry->cameraRight);
  if (warpDestEntry->cameraBottom != -1)
    grid->maxY = REGION_FIXED(warpDestEntry->cameraBottom);

  if (grid->minX + (240 << 16) > grid->maxX)
    grid->minX = grid->maxX - (240 << 16);
  if (grid->minY + (160 << 16) > grid->maxY)
    grid->minY = grid->maxY - (160 << 16);
}

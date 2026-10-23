/// @file
/// Running a map until the party leaves it, and its warp destination.
// Part of object.c.

#include "../object_internal.h"

/// The object system's work (struct ObjectSystemWork, gObjectSystemWork in
/// HEAP_SLOT_OBJECT_SYSTEM), seen as the flag that suspends sprite projection
/// while the menu opens.
struct MenuCtrl {
  u16 used;     ///< ObjectSystemWork's; not named here
  u8 unused[2]; ///< ObjectSystemWork's; no code names it
  /// ObjectSystemWork's suspended: while nonzero, sprites not held are not
  /// drawn and give up their VRAM (object_camera.c, sprite_draw.c).
  u16 wait;
};

/// Runs the current map, which the party entered by warpDest, until it
/// leaves; returns the warp kind it left by, for Map_TakeWarp, or WARP_NONE
/// when the next map is already in gGameState: a random battle, a
/// party that fell outside battle, Retreat or a script's destination. Sets the
/// map up (layout, object events, encounters, return points), runs its
/// initialization, fades it in, then runs the field loop: the party walks
/// (Field_ProcessStep each frame) until a request is made, and then the
/// requests are handled until none is left.
///
/// Called from Game_Run's main loop for every map, which
/// passes the result to Map_TakeWarp. Allocates the field work in
/// HEAP_SLOT_FIELD, cleared, and frees it on the way out, with every
/// block the map took after it from the same pool; sets gGameState's default,
/// next and defeat destinations, and clears FLAG_IN_BATTLE and, once the map
/// has started, FLAG_KEEP_PARTY_POSITION.
s32 Map_Run(s32 warpDest) {
  struct FieldWork *work;
  struct ObjectEvent *objectEvent;
  s32 mode;
  s32 result = 0;
  s32 code;

  // Set up the map.
  work = AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  Dma_Fill32(0, work, sizeof(struct FieldWork));
  GameFlag_ClearBitFar(FLAG_IN_BATTLE);
  gGameState.defaultMap = gGameState.mapId;
  gGameState.defaultWarpDest = gGameState.warpDest;
  gGameState.nextMap = -1;
  gGameState.nextWarpDest = -1;
  gGameState.defeatMap = -1;
  gGameState.defeatWarpDest = -1;
  Task_ResetAll();
  Djinn_ResolvePendingEvent(0);
  if (gGameState.layoutId == (s32)&MapId_MainMenu) {
    WorldMap_InitializePerspectiveFar();
    gGameState.perspectiveMap = 1;
    mode = OBJECT_SYSTEM_WORLD_MAP;
    Encounter_SelectMapZones(0);
  } else {
    MapGrid_LoadLayoutFar(gGameState.layoutId);
    mode = OBJECT_SYSTEM_LAYOUT;
    Encounter_SelectMapZones(1);
  }
  work->mode = mode;
  ObjectSystem_InitializeFar(mode);
  UiWork_InitializeWithResourceCountersFar();
  Field_ClearRequests();
  work->events = gOverlayArea.events();
  Map_ApplyWarpDest();
  ObjectEvent_ResetForObject(
      (struct ObjectEventTemplate *)gOverlayArea.objectEventTemplates());
  if (GameFlag_TestFar(FLAG_KEEP_PARTY_POSITION) != 0)
    ObjectEvent_Restore();
  if (gGameState.pendingDjinnEvent != 0)
    ObjectEvent_PlaceMapObjectEvents();
  if (mode == OBJECT_SYSTEM_WORLD_MAP) {
    WorldMap_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gGameState.x, gGameState.z);
  } else {
    MapGrid_CenterOnFollowedPointFar();
  }
  Field_PlaceTreasureSpots();
  PaletteFade_Initialize();
  PaletteFade_SetTarget(0x10000, 0);
  // The default fade is from the backdrop colour over 16 frames; the map's
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
  work->previousCell = (struct MapGridCell *)Ram_MapGridCellBuffer;
  // A fresh entry (not a return from battle) resets the hazard count to its
  // defaults (damage 1 every 16 steps), the stopped object event and Cloak.
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

  // The map's initialization may already have taken a warp.
  code = work->warp;
  if (code != 0) {
    result = code;
    work->warp = 0;
  } else {
    GameFlag_ClearBitFar(FLAG_KEEP_PARTY_POSITION);
    // Fade in, show the area name and give what was left pending.
    if (Field_CountRequests() == 0) {
      if (work->transitionActive == 0) {
        ScreenTransition_Start(work->transition, work->transitionFrames);
        work->transitionActive = 1;
        PLTT_BACKDROP = 0;
        // An arrival the last map asked for
        // (Warp_PlayArrival) plays through the fade; with none,
        // wait out half of it.
        if (Warp_PlayArrival() == 0)
          WaitFrames((work->transitionFrames + 1) / 2);
      }
      if (GameFlag_TestFar(FLAG_SHOW_LOCATION_NAME) != 0) {
        GameFlag_ClearBitFar(FLAG_SHOW_LOCATION_NAME);
        MapNamePopup_ShowFar(gGameState.mapId);
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
      objectEvent = work->objectEvents[gGameState.leaderLocalId];
      ObjectDispatch_StartIdleFar((struct DispatchObject *)objectEvent);
      gGameState.x = objectEvent->x.fixed;
      gGameState.y = 0;
      gGameState.z = objectEvent->z.fixed;
      gGameState.heading = objectEvent->facing;
      gGameState.turn = objectEvent->layer;

      // Handle the pending requests, most urgent first.
      while (Field_CountRequests() != 0) {
        s32 next = work->warp;

        if (next != 0) {
          result = next;
          work->warp = 0;
          goto done;
        }
        if (work->raisedEvent != 0) {
          work->busy = 1;
          if (work->raisedEvent == FIELD_RAISED_EVENT_PARTY_FELL) {
            // Characters fell to poison or hazardous ground.
            u32 i;

            Leader_StopMoving();
            WorldMap_StartScript();
            for (i = 0; i < work->fallenCount; i++) {
              ObjectDispatch_SelectAnimationFar(objectEvent, 22);
              MessageArgument_PushFar(work->fallenOwners[i], 1);
              FieldMessage_ShowFar(MsgFieldStrengthExhausted, 1);
            }
            if (work->survivorCount == 0) {
              // The whole party fell: back to the last sanctum with 1 HP.
              struct Unit *vitals;

              if (gGameState.leaderLocalId == 0) {
                if (GameFlag_TestFar(FLAG_CHILDHOOD) != 0)
                  ObjectDispatch_SelectAnimationFar(objectEvent, 21);
                else
                  ObjectDispatch_SelectAnimationFar(objectEvent, 37);
              } else {
                ObjectDispatch_SelectAnimationFar(objectEvent, 19);
              }
              Sound_PlaySongFar(MUS_DEFEAT);
              FieldMessage_ShowFar(MsgFieldPartyAnnihilated, 1);
              vitals = Unit_GetFar(gGameState.leaderLocalId);
              vitals->hp = 1;
              Unit_UpdateGauges(vitals);
              gGameState.mapId = gGameState.savedMap;
              gGameState.warpDest = gGameState.savedWarpDest;
              WorldMap_EndScript();
              result = WARP_NONE;
              goto done;
            }
            WorldMap_EndScript();
          } else if (work->raisedEvent == FIELD_RAISED_EVENT_REGION_MAP) {
            Leader_StopMoving();
            RegionMap_Show((s32)&ResourceId_WorldMapPicture);
          } else if (work->raisedEvent == FIELD_RAISED_EVENT_WORLD_MAP_ZOOM) {
            Leader_StopMoving();
            FieldEffect_RunVisibilityTransition();
          } else {
            Map_RunRaisedEvent(work->raisedEvent);
          }
          work->busy = 0;
          work->raisedEvent = 0;
        } else if (work->battle != 0) {
          // A random encounter: save the object events and go to the battle.
          // Field_StartEncounterTransition closes the screen itself and
          // clears transitionActive, so the map ends without another fade.
          Leader_StopMoving();
          ObjectEvent_Snapshot();
          gGameState.returnMusic = -1;
          gGameState.mapId = RUN_BATTLE;
          gGameState.warpDest = work->battle;
          work->warp = WARP_NONE;
          Field_StartEncounterTransition(work->battle);
          work->encounterBias = 0;
          work->battle = 0;
          gGameState.encounterSteps = 0;
        } else if (work->coordEvent != 0) {
          work->busy = 1;
          Map_RunCoordEventOrWarp(work->coordEvent);
          work->busy = 0;
          work->coordEvent = 0;
        } else if (work->pushCell != 0) {
          Field_RunPushCellEvent(work->pushCell);
          work->pushCell = 0;
        } else if (work->examine != 0) {
          // Examine the object event or tile ahead, or open the menu if neither
          // has an event.
          s32 object;

          MapNamePopup_HideFar();
          object = Field_FindNearbyTalkTarget(gGameState.leaderLocalId);
          mode = 0;
          if (object != -1) {
            mode = Map_FindObjectEventTalkEvent(object) != 0;
          }
          if (mode != 0) {
            // 0x1000 keeps object event 0's request non-zero; the handler
            // masks it.
            work->talk = object | 0x1000;
            work->menu = 0;
          } else {
            mode = ObjectEvent_GetTriggerTileAheadOfCurrent();
            if (mode != 0) {
              work->searchCell = mode;
              work->menu = 0;
            } else {
              work->menu = 1;
            }
          }
          work->examine = 0;
        } else if (work->menu != 0) {
          // The main menu (B + Select in debug mode opens the debug menu).
          MapNamePopup_HideFar();
          Leader_StopMoving();
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          WorldMap_StartScript();
          GameFlag_SetBitFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0 &&
              (gKeysHeld & KEY_SELECT) != 0) {
            DebugFlagEditor_RunFar();
          } else if (GameFlag_TestFar(FLAG_MENUS_RAISE_EVENT) != 0) {
            work->raisedEvent = FIELD_RAISED_EVENT_MENUS;
          } else {
            work->effectTimerRunning = 0;
            FieldMenu_RunFar();
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
        } else if (work->searchCell != 0) {
          WorldMap_StartScript();
          Field_SearchCell(work->searchCell);
          WorldMap_EndScript();
          work->searchCell = 0;
        } else if (work->psynergy != 0) {
          MapNamePopup_HideFar();
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
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          Leader_StopMoving();
          WorldMap_StartScript();
          GameFlag_SetBitFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0) {
            DebugMapSelect_RunFar();
          }
          /* FAKEMATCH(volatile): gDebugMode read through a volatile pointer,
             so this second test loads it again; as gDebugMode, 48 of 965
             instructions differ in tbs-en. */
          else if (*(volatile u8 *)&gDebugMode != 0 &&
                   (gKeysHeld & KEY_L) != 0) {
            PaletteEditor_Run();
          } else if (GameFlag_TestFar(FLAG_MENUS_RAISE_EVENT) != 0) {
            // As for the main menu above.
            work->raisedEvent = FIELD_RAISED_EVENT_MENUS;
          } else {
            Field_EndBusyAndReveal();
            MapNamePopup_HideFar();
            gGameState.returnMusic = Sound_GetCurrentBgmFar();
            if (GameFlag_TestFar(FLAG_IN_BATTLE_ARENA) == 0) {
              // With fewer than 136 of the 512 sprite VRAM blocks (64 bytes
              // each) free, sprite drawing stays suspended while the menu
              // runs, and a frame passes so that the field's sprites give
              // their VRAM back before it opens.
              struct MenuCtrl *ctrl = (struct MenuCtrl *)gObjectSystemWork;
              u8 *owner = gVramCacheBlockOwners;
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
              PauseMenu_RunFar(0);
              ctrl->wait = 0;
            } else {
              FieldMessage_ShowFar(MsgFieldPauseUnavailable, 1);
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
      objectEvent = work->objectEvents[gGameState.leaderLocalId];
      if (objectEvent != NULL) {
        if (gGameState.movementMode == LEADER_MOVEMENT_AIRBORNE)
          ObjectDispatch_StartKeyCheckFar((struct DispatchObject *)objectEvent);
        else if (gGameState.movementMode == LEADER_MOVEMENT_CLIMB)
          ObjectDispatch_StartMoveByKeysFar(
              (struct DispatchObject *)objectEvent);
        else if (work->mode == OBJECT_SYSTEM_WORLD_MAP)
          ObjectDispatch_StartWorldMapControlFar(
              (struct DispatchObject *)objectEvent);
        else
          ObjectDispatch_StartPlayerControlFar(
              (struct DispatchObject *)objectEvent);
      }
      do {
        WaitFrames(1);
        objectEvent = work->objectEvents[gGameState.leaderLocalId];
        if (gDebugMode == 0 || GameFlag_TestFar(FLAG_DEBUG_STEPS_OFF) == 0)
          Field_ProcessStep(objectEvent->layer, objectEvent->x.fixed,
                            objectEvent->y.fixed, objectEvent->z.fixed);
      } while (Field_CountRequests() == 0);
    }
  }

done:
  if (work->transitionActive != 0) {
    ScreenTransition_Finish(work->transition, work->transitionFrames);
    work->transitionActive = 0;
    WaitFrames(work->transitionFrames);
  }
  FreeHeapSlot(HEAP_SLOT_FIELD);
  return result;
}

/// An object event (struct ObjectEvent), seen with its position as one vector.
struct ObjectVectorView {
  u8 pad0[6];
  u16 angle;                    ///< 1/65536 turns
  struct EffectVector position; ///< 16.16 fixed-point pixels
  u8 pad1[14];
  u8 kind; ///< Object's layer: the map grid layer of its ground
  u8 pad2[0x2d];
  struct Sprite *sprite;
};

/// Returns the metatile behavior of the map cell 16 pixels ahead of the party
/// leader when there is something to search there: a fixed search spot
/// (MB_JAR to MB_WELL) whose ground is level with the leader or up
/// to 64 pixels above it, or a behavior the map has a bg event for. Returns 0
/// otherwise.
///
/// Called by Map_Run for the A button (a non-zero result becomes
/// work->tile, for Field_SearchCell), and by FieldMenu_Run's search
/// entry through ObjectEvent_GetTriggerTileAheadOfCurrentFar. Changes nothing.
s32 ObjectEvent_GetTriggerTileAheadOfCurrent(void) {
  u8 *workBytes;
  u8 *gridBytes;
  struct ObjectVectorView *leader;
  union PolarOffsetPosition probePosition;
  u8 *cell;
  u8 *cells;
  s32 collisionCode;
  s32 groundHeight;
  s32 result;

  result = 0;
  leader = ObjectEvent_GetByLocalId(gGameState.leaderLocalId);
  workBytes = *(u8 **)Ram_FieldWork;
  gridBytes = *(u8 **)Ram_MapGridWork;
  if (leader != 0) {
    probePosition.vector.x = leader->position.x;
    probePosition.vector.y = leader->position.y;
    probePosition.vector.z = leader->position.z;
    Vector_AddPolarOffset(0x100000, leader->angle, probePosition.coordinates);
    // World map metatiles are 32 pixels square, 32 to a row; map cells 16
    // pixels square, 128 to a row, four bytes each.
    if (((struct FieldWork *)workBytes)->mode == OBJECT_SYSTEM_WORLD_MAP) {
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
      groundHeight = MapGrid_GetTerrainHeightFar(
          leader->kind, probePosition.vector.x, probePosition.vector.z);
      if (groundHeight >= leader->position.y &&
          groundHeight <= leader->position.y + 0x400000) {
        result = collisionCode;
      }
    } else if (Map_FindEvent(MAP_EVENT_BG, collisionCode) != 0) {
      result = collisionCode;
    }
  }
  return result;
}

/// Places the party at the warp destination it came in by (the first matching
/// entry, or the map's first one) unless it keeps its position, and sets the
/// map's scroll bounds, at least a screen (240 x 160 pixels) wide and tall.
///
/// Called by Map_Run once the map's events are set. Writes
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

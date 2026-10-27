/// @file
/// Running a scene until the party leaves it, and its entrance.
// Part of overworld.c.

#include "../overworld_internal.h"

/// The actor system's work (struct ActorSystemWork, gActorSystemWork in
/// HEAP_SLOT_ACTOR_SYSTEM), seen as the flag that suspends sprite projection
/// while the menu opens.
struct ActorSystemSuspendView {
  u16 used;     ///< ActorSystemWork's; not named here
  u8 unused[2]; ///< ActorSystemWork's; no code names it
  /// ActorSystemWork's suspended: while nonzero, sprites not held are not
  /// drawn and give up their VRAM (actor_camera.c, sprite_draw.c).
  u16 wait;
};

/// Runs the current scene, which the party entered by entrance, until it
/// leaves; returns the exit kind it left by, for Scene_TakeExit, or
/// EXIT_KEEP_DESTINATION when the next scene is already in gCommon: a
/// random battle, a party that fell outside battle, Retreat or an event's
/// destination. Sets the scene up (map, actors, encounters, return points),
/// runs its initialization, fades it in, then runs the field loop: the party
/// walks (Field_UpdateWalking each frame) until a request is made, and then the
/// requests are handled until none is left.
///
/// Called by Monitor_Run for every scene, which
/// passes the result to Scene_TakeExit. Allocates the field work in
/// HEAP_SLOT_FIELD, cleared, and frees it on the way out, with every
/// block the scene took after it from the same pool; sets gCommon's
/// return point, after-battle and defeat destinations, and clears
/// FLAG_IN_BATTLE and, once the scene has started, FLAG_KEEP_PARTY_POSITION.
s32 FieldLoop_Run(s32 entrance) {
  struct FieldWork *work;
  struct Actor *actor;
  s32 mode;
  s32 result = 0;
  s32 code;

  // Set up the scene.
  work = AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  Dma_Fill32(0, work, sizeof(struct FieldWork));
  GameFlag_ClearFar(FLAG_IN_BATTLE);
  gCommon.returnScene = gCommon.sceneId;
  gCommon.returnEntrance = gCommon.entrance;
  gCommon.afterBattleScene = -1;
  gCommon.afterBattleEntrance = -1;
  gCommon.defeatScene = -1;
  gCommon.defeatEntrance = -1;
  Task_ResetAll();
  Djinn_ResolvePending(0);
  if (gCommon.mapId == MAP_INDEX_WORLD) {
    WorldMap_InitializePerspectiveFar();
    gCommon.perspectiveMap = 1;
    mode = ACTOR_SYSTEM_WORLD_MAP;
    Encounter_SelectMapZones(0);
  } else {
    MapGrid_LoadMapFar(gCommon.mapId);
    mode = ACTOR_SYSTEM_MAP;
    Encounter_SelectMapZones(1);
  }
  work->mode = mode;
  ActorSystem_InitializeFar(mode);
  WindowSystem_InitFieldFar();
  Field_ClearRequests();
  work->triggers = gOverlayArea.triggers();
  Scene_ApplyEntrance();
  Actor_ResetForLeader((struct ActorTemplate *)gOverlayArea.actorTemplates());
  if (GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION) != 0)
    Actor_Restore();
  if (gCommon.pendingDjinni != 0)
    Actor_PlaceMapActors();
  if (mode == ACTOR_SYSTEM_WORLD_MAP) {
    WorldMap_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gCommon.x, gCommon.z);
  } else {
    MapGrid_CenterOnFollowedPointFar();
  }
  Field_PlaceTreasureSpots();
  PaletteFade_Initialize();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  // The default fade is from the backdrop colour over 16 frames; the scene's
  // initialization may choose another.
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 16;
  work->transitionActive = 0;
  // Field_UpdateWalking scales the leader's speed by this (16.16) into the
  // distance counted towards PP, Djinn recovery and poison: 0x10000 / 0x199
  // makes one round of them about every 160 pixels walked. Towns turn it off
  // and block Retreat.
  work->walkingDistanceScale = 0x199;
  work->walkingDistance = 0;
  if (gCommon.sceneType == SCENE_TYPE_TOWN) {
    work->walkingDistanceScale = 0;
    GameFlag_SetFar(FLAG_RETREAT_BLOCKED);
  }
  work->previousCell = (struct MapGridCell *)Ram_MapGridCellBuffer;
  // A fresh entry (not a return from battle) resets the hazard count to its
  // defaults (damage 1 every 16 steps), the stopped actor and Cloak.
  if (GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION) == 0) {
    Scene_SetRestartPoint();
    gCommon.hazardLimit = 16;
    gCommon.hazardMode = HAZARD_COUNT_ON_CELLS;
    gCommon.hazardDamage = 1;
    gCommon.haltedActor = -1;
    gCommon.cloaked = 0;
  }
  FieldPsynergy_ResumeCloakCheck();
  work->queuedSound = -1;
  gOverlayArea.initialize();

  // The scene's initialization may already have taken an exit.
  code = work->exit;
  if (code != 0) {
    result = code;
    work->exit = 0;
  } else {
    GameFlag_ClearFar(FLAG_KEEP_PARTY_POSITION);
    // Fade in, show the area name and give what was left pending.
    if (Field_CountRequests() == 0) {
      if (work->transitionActive == 0) {
        ScreenTransition_Start(work->transition, work->transitionFrames);
        work->transitionActive = 1;
        PLTT_BACKDROP = 0;
        // An arrival the last scene asked for
        // (Entrance_PlayArrival) plays through the fade; with none,
        // wait out half of it.
        if (Entrance_PlayArrival() == 0)
          WaitFrames((work->transitionFrames + 1) / 2);
      }
      if (GameFlag_GetFar(FLAG_SHOW_AREA_NAME) != 0) {
        GameFlag_ClearFar(FLAG_SHOW_AREA_NAME);
        AreaNameWindow_ShowFar(gCommon.sceneId);
      }
      if (gCommon.pendingDjinni != 0) {
        Djinn_ResolvePending(gCommon.pendingDjinni, 1);
        gCommon.pendingDjinni = 0;
      }
      if (gCommon.pendingItem != 0) {
        WorldMap_Freeze();
        if (Party_CheckItemLimit(gCommon.pendingItem) == 0)
          PartyInventory_GiveItem(gCommon.pendingItem, 0);
        WorldMap_Unfreeze();
        gCommon.pendingItem = 0;
      }
    }

    for (;;) {
      // Note where the leader is, for the requests to use.
      GameFlag_SetFar(FLAG_FIELD_HANDLING_REQUESTS);
      actor = work->actors[gCommon.leaderActorId];
      Actor_StartIdleFar(actor);
      gCommon.x = actor->x.fixed;
      gCommon.y = 0;
      gCommon.z = actor->z.fixed;
      gCommon.heading = actor->facing;
      gCommon.layer = actor->layer;

      // Handle the pending requests, most urgent first.
      while (Field_CountRequests() != 0) {
        s32 next = work->exit;

        if (next != 0) {
          result = next;
          work->exit = 0;
          goto done;
        }
        if (work->raisedTrigger != 0) {
          work->busy = 1;
          if (work->raisedTrigger == FIELD_RAISED_TRIGGER_PARTY_FELL) {
            // Characters fell to poison or hazardous ground.
            u32 i;

            Leader_StopMoving();
            WorldMap_Freeze();
            for (i = 0; i < work->fallenCount; i++) {
              Actor_SelectAnimationFar(actor, ANIM_LEADER_LEAN_BACK);
              MessageArgument_PushFar(work->fallenOwners[i],
                                      MESSAGE_ARGUMENT_ACTOR);
              FieldMessage_ShowFar(MsgFieldStrengthExhausted,
                                   FIELD_MESSAGE_ZOOM);
            }
            if (work->survivorCount == 0) {
              // The whole party fell: back to the last sanctum with 1 HP.
              struct Unit *vitals;

              if (gCommon.leaderActorId == 0) {
                if (GameFlag_GetFar(FLAG_CHILDHOOD) != 0)
                  Actor_SelectAnimationFar(actor, 21);
                else
                  Actor_SelectAnimationFar(actor, 37);
              } else {
                Actor_SelectAnimationFar(actor, ANIM_COLLAPSE);
              }
              Sound_PlaySongFar(MUS_DEFEAT);
              FieldMessage_ShowFar(MsgFieldPartyAnnihilated,
                                   FIELD_MESSAGE_ZOOM);
              vitals = Unit_GetFar(gCommon.leaderActorId);
              vitals->hp = 1;
              Unit_UpdateGauges(vitals);
              gCommon.sceneId = gCommon.restartScene;
              gCommon.entrance = gCommon.restartEntrance;
              WorldMap_Unfreeze();
              result = EXIT_KEEP_DESTINATION;
              goto done;
            }
            WorldMap_Unfreeze();
          } else if (work->raisedTrigger ==
                     FIELD_RAISED_TRIGGER_WORLD_PICTURE) {
            Leader_StopMoving();
            WorldPicture_Show((s32)&ResourceId_WorldMapPicture);
          } else if (work->raisedTrigger ==
                     FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM) {
            Leader_StopMoving();
            FieldEffect_RunVisibilityTransition();
          } else {
            Scene_RunRaisedTrigger(work->raisedTrigger);
          }
          work->busy = 0;
          work->raisedTrigger = 0;
        } else if (work->encounter != 0) {
          // A random encounter: save the actors and go to the battle.
          // Field_StartEncounterTransition closes the screen itself and
          // clears transitionActive, so the scene ends without another fade.
          Leader_StopMoving();
          Actor_Snapshot();
          gCommon.returnMusic = -1;
          gCommon.sceneId = PROGRAM_BATTLE;
          gCommon.entrance = work->encounter;
          work->exit = EXIT_KEEP_DESTINATION;
          Field_StartEncounterTransition(work->encounter);
          work->encounterBias = 0;
          work->encounter = 0;
          gCommon.encounterSteps = 0;
        } else if (work->stepTrigger != 0) {
          work->busy = 1;
          Scene_RunStepTriggerOrExit(work->stepTrigger);
          work->busy = 0;
          work->stepTrigger = 0;
        } else if (work->pushCell != 0) {
          Field_RunPushCell(work->pushCell);
          work->pushCell = 0;
        } else if (work->aButton != 0) {
          // Examine the actor or cell ahead, or open the menu if neither
          // has an event.
          s32 object;

          AreaNameWindow_HideFar();
          object = Field_FindNearbyTalkTarget(gCommon.leaderActorId);
          mode = 0;
          if (object != -1) {
            mode = Scene_FindActorTalkTrigger(object) != 0;
          }
          if (mode != 0) {
            // 0x1000 keeps actor 0's request non-zero; the handler
            // masks it.
            work->talk = object | 0x1000;
            work->fieldMenu = 0;
          } else {
            mode = Actor_GetTriggerCellAheadOfCurrent();
            if (mode != 0) {
              work->examineCell = mode;
              work->fieldMenu = 0;
            } else {
              work->fieldMenu = 1;
            }
          }
          work->aButton = 0;
        } else if (work->fieldMenu != 0) {
          // The File Screen (B + Select in debug mode opens the debug menu).
          AreaNameWindow_HideFar();
          Leader_StopMoving();
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          WorldMap_Freeze();
          GameFlag_SetFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0 &&
              (gKeysHeld & KEY_SELECT) != 0) {
            DebugFlagEditor_RunFar();
          } else if (GameFlag_GetFar(FLAG_MENUS_RAISE_TRIGGER) != 0) {
            work->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
          } else {
            work->effectTimerRunning = 0;
            FieldMenu_RunFar();
            work->effectTimerRunning = 1;
          }
          WorldMap_Unfreeze();
          GameFlag_ClearFar(FLAG_MENU_OPEN);
          GameFlag_RefreshLureCapFar();
          work->fieldMenu = 0;
        } else if ((s16)work->talk != 0) {
          WorldMap_Freeze();
          Scene_TalkToActor(work->talk & 0xfff);
          WorldMap_Unfreeze();
          work->talk = 0;
        } else if (work->examineCell != 0) {
          WorldMap_Freeze();
          Field_ExamineCell(work->examineCell);
          WorldMap_Unfreeze();
          work->examineCell = 0;
        } else if (work->psynergy != 0) {
          AreaNameWindow_HideFar();
          WorldMap_Freeze();
          Field_UsePsynergy(work->psynergy);
          WorldMap_Unfreeze();
          work->psynergy = 0;
        } else if (work->item != 0) {
          WorldMap_Freeze();
          Field_UseItem(work->item, work->itemSlot);
          WorldMap_Unfreeze();
          work->item = 0;
        } else if (work->pauseMenu != 0) {
          // The Pause menu (in debug mode, B opens the debug menu and L the
          // palette editor).
          Sound_PlaySongFar(SE_MENU_CURSOR_MOVE);
          Leader_StopMoving();
          WorldMap_Freeze();
          GameFlag_SetFar(FLAG_MENU_OPEN);
          if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0) {
            DebugMapSelect_RunFar();
          }
          /* FAKEMATCH(volatile): gDebugMode read through a volatile pointer,
             so this second test loads it again; as gDebugMode, 48 of 965
             instructions differ in tbs-en. */
          else if (*(volatile u8 *)&gDebugMode != 0 &&
                   (gKeysHeld & KEY_L) != 0) {
            PaletteEditor_Run();
          } else if (GameFlag_GetFar(FLAG_MENUS_RAISE_TRIGGER) != 0) {
            // As for the File Screen above.
            work->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
          } else {
            Field_EndBusyAndReveal();
            AreaNameWindow_HideFar();
            gCommon.returnMusic = Sound_GetCurrentBgmFar();
            if (GameFlag_GetFar(FLAG_IN_BATTLE_ARENA) == 0) {
              // With fewer than 136 of the 512 sprite VRAM blocks (64 bytes
              // each) free, sprite drawing stays suspended while the menu
              // runs, and a frame passes so that the field's sprites give
              // their VRAM back before it opens.
              struct ActorSystemSuspendView *ctrl =
                  (struct ActorSystemSuspendView *)gActorSystemWork;
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
              FieldMessage_ShowFar(MsgFieldPauseUnavailable,
                                   FIELD_MESSAGE_ZOOM);
            }
          }
          WorldMap_Unfreeze();
          GameFlag_ClearFar(FLAG_MENU_OPEN);
          work->pauseMenu = 0;
        }
      }

      // Hand the leader back to the player, as climbing, carried through the
      // air (only a key check runs), on the World Map or walking, and run the
      // walking update until the next request.
      GameFlag_ClearFar(FLAG_FIELD_HANDLING_REQUESTS);
      actor = work->actors[gCommon.leaderActorId];
      if (actor != NULL) {
        if (gCommon.movementMode == LEADER_MOVEMENT_AIRBORNE)
          Actor_StartKeyCheckFar(actor);
        else if (gCommon.movementMode == LEADER_MOVEMENT_CLIMB)
          Actor_StartMoveByKeysFar(actor);
        else if (work->mode == ACTOR_SYSTEM_WORLD_MAP)
          Actor_StartWorldMapControlFar(actor);
        else
          Actor_StartPlayerControlFar(actor);
      }
      do {
        WaitFrames(1);
        actor = work->actors[gCommon.leaderActorId];
        if (gDebugMode == 0 || GameFlag_GetFar(FLAG_DEBUG_STEPS_OFF) == 0)
          Field_UpdateWalking(actor->layer, actor->x.fixed, actor->y.fixed,
                              actor->z.fixed);
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

/// Returns the metatile behavior of the map cell 16 pixels ahead of the party
/// leader when there is something to search there: a fixed search spot
/// (MB_JAR to MB_WELL) whose ground is level with the leader or up
/// to 64 pixels above it, or a behavior the scene has an examine trigger for.
/// Returns 0 otherwise.
///
/// Called by FieldLoop_Run for the A button (a non-zero result becomes
/// work->tile, for Field_ExamineCell), and by FieldMenu_Run's search
/// entry through Actor_GetTriggerCellAheadOfCurrentFar. Changes nothing.
s32 Actor_GetTriggerCellAheadOfCurrent(void) {
  u8 *workBytes;
  u8 *gridBytes;
  struct Actor *leader;
  union PolarOffsetPosition probePosition;
  u8 *cell;
  u8 *cells;
  s32 behavior;
  s32 groundHeight;
  s32 result;

  result = 0;
  leader = Actor_Lookup(gCommon.leaderActorId);
  workBytes = *(u8 **)Ram_FieldWork;
  gridBytes = *(u8 **)Ram_MapGridWork;
  if (leader != 0) {
    probePosition.vector.x = leader->x.fixed;
    probePosition.vector.y = leader->y.fixed;
    probePosition.vector.z = leader->z.fixed;
    Vector_AddPolarOffset(0x100000, leader->facing, probePosition.coordinates);
    // World Map metatiles are 32 pixels square, 32 to a row; map cells 16
    // pixels square, 128 to a row, four bytes each.
    if (((struct FieldWork *)workBytes)->mode == ACTOR_SYSTEM_WORLD_MAP) {
      cell =
          Ram_Metatiles + ((((probePosition.vector.x / 0x200000) & 31) +
                            (((probePosition.vector.z / 0x200000) & 31) << 5))
                           << 2);
    } else {
      // MAPGRID_LAYER_GROUND's cells, MapGridScrollAnimView's layers[0].cells
      // at 0x130.
      cells = *(u8 **)(gridBytes + 0x130);
      cell = cells + (((probePosition.vector.x / 0x100000) +
                       ((probePosition.vector.z / 0x100000) << 7))
                      << 2);
    }
    behavior = cell[2];
    if (behavior >= MB_JAR && behavior <= MB_WELL) {
      groundHeight = MapGrid_GetTerrainHeightFar(
          leader->layer, probePosition.vector.x, probePosition.vector.z);
      if (groundHeight >= leader->y.fixed &&
          groundHeight <= leader->y.fixed + 0x400000) {
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
///
/// Called by FieldLoop_Run once the scene's events are set. Writes
/// gCommon's position, heading and turn, and the scene's minX to maxY.
void Scene_ApplyEntrance(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  s32 entrance = gCommon.entrance;
  const struct SceneEntrance *entranceEntry = gOverlayArea.entrances();
  s32 foundEntrance = 0;

  while (entranceEntry->entrance != -1) {
    if (entranceEntry->entrance == entrance &&
        (entranceEntry->requiredFlag == -1 ||
         GameFlag_GetFar(entranceEntry->requiredFlag))) {
      foundEntrance = 1;
      break;
    }
    entranceEntry++;
  }
  if (!foundEntrance)
    entranceEntry = gOverlayArea.entrances();

  // Camera bounds apply even when returning to a saved party position.
  if (!GameFlag_GetFar(FLAG_KEEP_PARTY_POSITION)) {
    gCommon.x = REGION_FIXED(entranceEntry->x);
    gCommon.y = REGION_FIXED(entranceEntry->y);
    gCommon.z = REGION_FIXED(entranceEntry->z);
    gCommon.heading = entranceEntry->facing;
    gCommon.layer = 0;
  }

  if (entranceEntry->cameraLeft != -1)
    grid->minX = REGION_FIXED(entranceEntry->cameraLeft);
  if (entranceEntry->cameraTop != -1)
    grid->minY = REGION_FIXED(entranceEntry->cameraTop);
  if (entranceEntry->cameraRight != -1)
    grid->maxX = REGION_FIXED(entranceEntry->cameraRight);
  if (entranceEntry->cameraBottom != -1)
    grid->maxY = REGION_FIXED(entranceEntry->cameraBottom);

  if (grid->minX + (DISPLAY_WIDTH << 16) > grid->maxX)
    grid->minX = grid->maxX - (DISPLAY_WIDTH << 16);
  if (grid->minY + (DISPLAY_HEIGHT << 16) > grid->maxY)
    grid->minY = grid->maxY - (DISPLAY_HEIGHT << 16);
}

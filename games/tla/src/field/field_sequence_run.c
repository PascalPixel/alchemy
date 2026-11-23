/// @file
/// The field sequence: running a scene until the party leaves it, and its
/// entrance.
// Part of field_sequence.c.

#include "field_sequence_internal.h"
#include "flags.h"
#include "sounds.h"
#include "field/field_work.h"
#include "gamepak/far_call/field.h"
#include "gamepak/far_call/sound.h"
#include "gamepak/far_call/window.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "field/object.h"
#include "macros.h"

extern u8 gMapGridCellBuffer[];

void PaletteFade_SetTarget(s32 color, s32 mode);
// UB: declared returning s32; the definition (main_routine.c) returns s16.
#ifdef UBFIX
s16 Field_GetSceneAttributes(s32 scene);
#else
s32 Field_GetSceneAttributes(s32 scene);
#endif
s32 Leader_StepOntoRope(void);
s32 Leader_StepOffRope(void);
s32 Leader_StepOntoTightrope(void);
s32 Leader_StepOffTightrope(void);

/// The two nibbles of a scene's attributes (Field_GetSceneAttributes).
#define NIBBLE_LOW(value) ((value) & 15)
#define NIBBLE_HIGH(value) (((value) >> 4) & 15)
/// The hazard count's limit each scene starts with, in steps on hazard cells:
/// the hazard deals its damage each time the count reaches it.
#define HAZARD_LIMIT_DEFAULT 16
/// A Teleport destination's World Picture marker (FlagEntry.value's high
/// nibble): 1 for a place the party walks to; 2 for Lemuria's, which it
/// reaches aboard the ship.
#define PLACE_MARKER_ON_FOOT (1 << 4)
#define PLACE_MARKER_ABOARD (2 << 4)
/// The sprite VRAM blocks the Pause menu needs free, or the field's sprites
/// stop drawing while it runs.
#define PAUSE_MENU_VRAM_BLOCKS 136
/// The search code aboard the ship facing sea of world map collision 3: the
/// World Map has no examine trigger for it, so the search finds nothing there.
#define SHIP_SEA_EXAMINE_CODE 99
/// A map entrance stream's row: the entrance, its x and z, and its flags byte.
#define MAP_ENTRANCE_ROW_BYTES 6

/// Teleport's steps (gMainRoutineWork.teleportState): a place was chosen and
/// the scene it lands in is loading, then the party arrives in it.
enum TeleportState {
  TELEPORT_NONE,
  TELEPORT_CHOSEN,
  TELEPORT_ARRIVING,
};

/// The field sequence: runs the current scene, which the party entered by
/// entrance, until it leaves; returns the exit kind it left by, for
/// Scene_TakeExit, or EXIT_KEEP_DESTINATION when the next scene is already in
/// gMainRoutineWork: a random battle, a party that fell outside battle, Retreat
/// or an event's destination.
s32 FieldSequence_Run(void) {
  struct FieldWork *work;
  struct MainRoutineWork *party;
  struct Actor *leader;
  struct FieldUnit *unit;
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

  // Set up the scene.
  work = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  Iwram_ClearWordsEntry(work, sizeof(struct FieldWork));
  GameFlag_Clear(FLAG_IN_BATTLE);
  gMainRoutineWork.returnScene = gMainRoutineWork.next;
  gMainRoutineWork.returnEntrance = gMainRoutineWork.entrance;
  gMainRoutineWork.afterBattleScene = -1;
  gMainRoutineWork.afterBattleEntrance = -1;
  gMainRoutineWork.defeatScene = -1;
  gMainRoutineWork.defeatEntrance = -1;
  gMainRoutineWork.onUpperLevel = 0;
  Task_ResetAll();
  Djinn_ResolvePending(0);
  if (gMainRoutineWork.mapId == SCENE_CLEAR) {
    WorldMap_InitializePerspectiveFar();
    gMainRoutineWork.perspectiveMap = 1;
    mode = ACTOR_SYSTEM_WORLD_MAP;
    Encounter_SelectMapZones(FALSE);
  } else {
    MapGrid_LoadMapFar(gMainRoutineWork.mapId);
    mode = ACTOR_SYSTEM_MAP;
    Encounter_SelectMapZones(TRUE);
  }
  work->mode = mode;
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    gTouchCopies.count = 0;
  else
    TileCopies_Restore();
  WindowSystem_InitFieldFar();
  ActorSystem_InitializeFar(mode);
  Field_ClearRequests();
  work->triggers = (const void *)SCENE_ENTRIES->triggers();
  Field_ApplyEntrance();
  Actor_ResetForLeader((s32)SCENE_ENTRIES->placements());
  Field_PlaceTreasureSpots();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    Actor_Restore();
  if (gMainRoutineWork.pendingDjinni != 0)
    Djinn_RestoreActorAfterBattle();
  work->queuedSound = -1;
  if (mode == ACTOR_SYSTEM_WORLD_MAP) {
    WorldMap_UpdateCurrentTileBlockFar();
    WorldMap_LoadGraphicsFar(gMainRoutineWork.x, gMainRoutineWork.z);
  } else {
    Map_ApplySetFlagEditsFar();
    SCENE_ENTRIES->prepareMap();
    MapGrid_CenterOnFollowedPointFar();
  }
  // XXX (no evidence): what flag 0x11f means; every scene start clears it,
  // The Broken Seal's does not, and nothing in either game's code, tables,
  // debug flag lists or cleared ranges of flags sets or reads it.
  GameFlag_Clear(0x11f);
  PaletteFade_Initialize();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  // The default fade is from the backdrop colour over 16 frames; the scene's
  // initialization may choose another.
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = MAP_TRANSITION_FRAMES;
  work->transitionActive = 0;
  // Field_UpdateWalking scales the leader's speed by this (16.16) into the
  // distance counted towards PP, Djinn recovery and poison: FIXED_ONE / 160
  // counts each of them on by one about every 160 pixels walked.
  work->walkingDistanceScale = FIXED_ONE / 160;
  work->walkingDistance = 0;
  if (gMainRoutineWork.sceneType == SCENE_TYPE_TOWN) {
    work->walkingDistanceScale = 0;
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
  }
  work->previousCell = gMapGridCellBuffer;
  // A fresh entry (not a return from battle) resets the hazard count to its
  // defaults (damage 1 every HAZARD_LIMIT_DEFAULT steps), the stopped actor and
  // Cloak.
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    Scene_SetRestartPoint();
    gMainRoutineWork.hazardLimit = HAZARD_LIMIT_DEFAULT;
    gMainRoutineWork.hazardMode = HAZARD_COUNT_ON_CELLS;
    gMainRoutineWork.hazardDamage = 1;
    gMainRoutineWork.haltedActor = -1;
    gMainRoutineWork.cloaked = 0;
  }
  FieldPsynergy_ResumeCloakCheck();
  value = Field_GetSceneAttributes(gMainRoutineWork.next);
  work->sceneAttributeHigh = NIBBLE_HIGH(value);
  work->sceneAttributeLow = NIBBLE_LOW(value);

  if (gMainRoutineWork.teleportState == TELEPORT_ARRIVING) {
    Actor_GetById(Party_GetLeaderActor())->scaleX = 0;
    Actor_SetSpriteFlagsFar(Actor_GetById(Party_GetLeaderActor()), 0);
  }
  Field_UpdateHoverReady();
  wait(2);
  SCENE_ENTRIES->initialize();
  // The scene's initialization may already have taken an exit.
  code = work->exit;
  if (code != 0) {
    result = code;
    work->exit = 0;
    goto done;
  }
  GameFlag_Clear(FLAG_KEEP_PARTY_POSITION);
  // Fade in, show the area name and give what was left pending.
  if (Field_CountRequests() == 0) {
    value = work->transitionActive;
    if (value == 0) {
      // XXX: why a Teleport arrival holds 6584 bytes of the battle effect's
      // slot through its two 20-frame pauses; a heap trace of an arrival
      // would settle it.
      if (gMainRoutineWork.teleportState == TELEPORT_CHOSEN) {
        HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 6584);
        wait(20);
        HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
      }
      ScreenTransition_Start(work->transition, work->transitionFrames);
      work->transitionActive = 1;
      if (gMainRoutineWork.teleportState == TELEPORT_CHOSEN) {
        HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, 6584);
        wait(20);
        HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
      }
      PLTT_BACKDROP = 0;
      // An arrival the last scene asked for (Entrance_PlayArrival) plays
      // through the fade; with none, wait out half of it.
      if (!Entrance_PlayArrival())
        wait((work->transitionFrames + 1) / 2);
    }
    arrivalState = gMainRoutineWork.teleportState;
    if (arrivalState == TELEPORT_CHOSEN) {
      /* FAKEMATCH(dead): teleportState set through its address. */
      *(s8 *)&gMainRoutineWork.teleportState = TELEPORT_ARRIVING;
      /* FAKEMATCH(dead): a once-only loop round the arrival. */
      do {
        /* FAKEMATCH(temporary): zero, 0 held in a local; as 0 at its use. */
        s32 zero = 0;

        Teleport_PlaySparksAndFade();
        if (gMainRoutineWork.teleportScene != (s16)SCENE_WORLD_MAP)
          goto clearOwner;
        /* FAKEMATCH(barrier): an empty asm. */
        asm("" : : : "r0");
        if ((value = FlagEntries_GetValue(gMainRoutineWork.teleportEntrance)) &
            PLACE_MARKER_ON_FOOT)
        clearOwner:
          gMainRoutineWork.ownerOverride = zero;
        else if (value & PLACE_MARKER_ABOARD)
          gMainRoutineWork.ownerOverride = 1;
      } while (0);
      Event_SetDestination(gMainRoutineWork.teleportScene,
                           gMainRoutineWork.teleportEntrance);
    } else if (arrivalState == TELEPORT_ARRIVING) {
      gMainRoutineWork.teleportState = TELEPORT_NONE;
      Teleport_PlayWhirl();
    }
    if (GameFlag_Get(FLAG_SHOW_AREA_NAME)) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      AreaNameWindow_ShowFar(gMainRoutineWork.next);
    }
    if (gMainRoutineWork.pendingDjinni != 0) {
      // UB: called with a second argument (1) its definition does not take.
#ifdef UBFIX
      Djinn_ResolvePending(gMainRoutineWork.pendingDjinni);
#else
      ((void (*)(s32, s32))Djinn_ResolvePending)(gMainRoutineWork.pendingDjinni,
                                                 1);
#endif
      gMainRoutineWork.pendingDjinni = 0;
    }
    if (gMainRoutineWork.pendingItem != 0) {
      WorldMap_Freeze();
      if (Party_CheckItemLimit(gMainRoutineWork.pendingItem) == 0) {
        PartyInventory_GiveItem(gMainRoutineWork.pendingItem, 0);
      }
      WorldMap_ReloadPendingAreaGraphics();
      gMainRoutineWork.pendingItem = 0;
    }
  }

  party = &gMainRoutineWork;
  leaderId = &gMainRoutineWork.leaderActorId;
  steps = &gMainRoutineWork.encounterSteps;
  entrance = &gMainRoutineWork.entrance;
  layer = &gMainRoutineWork.layer;
  for (;;) {
    // Note where the leader is, for the requests to use.
    GameFlag_Set(FLAG_FIELD_HANDLING_REQUESTS);
    leader = Field_GetPartyActor();
    Actor_StartIdleFar(leader);
    SET_POSITION(party, leader->x.fixed, 0, leader->z.fixed);
    party->heading = leader->facing;
    *layer = leader->layer;
    // Handle the pending requests, most urgent first.
    while (Field_CountRequests() != 0) {
      if (work->exit != 0) {
        result = work->exit;
        work->exit = 0;
        goto done;
      }
      if (work->raisedTrigger != 0) {
        work->busy = 1;
        if (work->raisedTrigger == FIELD_RAISED_TRIGGER_PARTY_FELL) {
          // Characters fell to poison or hazardous ground.
          Leader_StopMoving();
          WorldMap_Freeze();
          for (value = 0; value < work->fallenCount; value++) {
            Actor_SelectAnimationFar(leader, ANIM_LEADER_LEAN_BACK);
            MessageArgument_PushFar(work->fallenOwners[value],
                                    MESSAGE_ARGUMENT_ACTOR);
            FieldMessage_ShowFar((s32)MsgFieldStrengthExhausted,
                                 FIELD_MESSAGE_ZOOM);
          }
          if (work->survivorCount == 0) {
            // The whole party fell: back to the restart point with 1 HP.
            if (*leaderId == ACTOR_FELIX)
              Actor_SelectAnimationFar(leader, ANIM_LEADER_FALL_ON_FACE);
            else
              Actor_SelectAnimationFar(leader, ANIM_COLLAPSE);
            Sound_PlayFar(MUS_DEFEATED);
            FieldMessage_ShowFar((s32)MsgFieldPartyAnnihilated,
                                 FIELD_MESSAGE_ZOOM);
            unit = Unit_Get(*leaderId);
            unit->hp = 1;
            Unit_UpdateGauges(unit);
            {
              s32 restartScene = party->restartScene;
              s32 restartEntrance;

              party->next = restartScene;
              restartEntrance = party->restartEntrance;
              *entrance = restartEntrance;
              WorldMap_SetReturnLeavingScene((s16)restartScene,
                                             (s16)restartEntrance);
            }
            result = EXIT_KEEP_DESTINATION;
            WorldMap_ReloadPendingAreaGraphics();
            goto done;
          }
          WorldMap_ReloadPendingAreaGraphics();
        } else if (work->raisedTrigger == FIELD_RAISED_TRIGGER_TELEPORT) {
          Leader_StopMoving();
          WorldPicture_Show(1);
          if (gMainRoutineWork.teleportScene != -1)
            gMainRoutineWork.teleportState = TELEPORT_CHOSEN;
        } else if (work->raisedTrigger == FIELD_RAISED_TRIGGER_WORLD_PICTURE) {
          Leader_StopMoving();
          value = 0;
          while (WorldPicture_Show(value))
            value ^= 1;
        } else if (work->raisedTrigger == FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM) {
          Leader_StopMoving();
          WorldMap_ZoomOutWhileLHeld();
        } else if (work->raisedTrigger == FIELD_RAISED_TRIGGER_ROPE_ON) {
          if (work->heldKeys == gInput.held) {
            Leader_StepOntoRope();
            work->fieldMenu = 0;
            work->pauseMenu = 0;
            work->aButton = 0;
            work->encounter = 0;
            work->psynergy = 0;
          }
        } else if (work->raisedTrigger == FIELD_RAISED_TRIGGER_ROPE_OFF) {
          if (work->heldKeys == gInput.held) {
            Leader_StepOffRope();
            work->fieldMenu = 0;
            work->pauseMenu = 0;
            work->aButton = 0;
            work->encounter = 0;
            work->psynergy = 0;
          }
        } else if (work->raisedTrigger == FIELD_RAISED_TRIGGER_TIGHTROPE_ON) {
          if (work->heldKeys == gInput.held) {
            Leader_StepOntoTightrope();
            work->fieldMenu = 0;
            work->pauseMenu = 0;
            work->aButton = 0;
            work->encounter = 0;
            work->psynergy = 0;
          }
        } else if (work->raisedTrigger == FIELD_RAISED_TRIGGER_TIGHTROPE_OFF) {
          if (work->heldKeys == gInput.held) {
            Leader_StepOffTightrope();
            work->fieldMenu = 0;
            work->pauseMenu = 0;
            work->aButton = 0;
            work->encounter = 0;
            work->psynergy = 0;
          }
        } else {
          Scene_RunRaisedTrigger(work->raisedTrigger);
        }
        work->busy = 0;
        work->raisedTrigger = 0;
      } else if (work->stepTrigger != 0) {
        work->busy = 1;
        Scene_RunStepTriggerOrExit(work->stepTrigger);
        work->busy = 0;
        work->stepTrigger = 0;
      } else if (work->pushCell != 0) {
        Field_RunPushCell(work->pushCell);
        work->pushCell = 0;
      } else if (work->encounter != 0) {
        // A random encounter: save the actors and go to the battle.
        Leader_StopMoving();
        ObjectTable_Snapshot();
        party->returnMusic = -1;
        party->next = SEQUENCE_BATTLE;
        *entrance = work->encounter;
        work->exit = EXIT_KEEP_DESTINATION;
        Field_StartEncounterTransition(work->encounter);
        work->encounterBias = 0;
        work->encounter = 0;
        *steps = 0;
      } else if (work->aButton != 0) {
        // Talk to the actor ahead, or examine the cell ahead, or open the field
        // menu if neither has an event.
        AreaNameWindow_HideFar();
        talk = 0;
        if (gMainRoutineWork.ownerOverride == 0)
          value = Field_FindNearbyTalkTarget(*leaderId);
        else
          value = Field_FindNearbyTalkTarget(ACTOR_FIRST_PLACED);
        if (value != -1) {
          talk = 1;
          if ((s32)value <= FIELD_TREASURE_ACTOR - 1)
            talk = Scene_FindActorTalkTrigger(value) != 0;
        }
        if (talk) {
          work->talk = value | FIELD_TALK_REQUEST;
          work->fieldMenu = 0;
        } else if ((talk = Actor_GetTriggerCellAheadOfCurrent()) != 0) {
          work->examineCell = talk;
          work->fieldMenu = 0;
        } else {
          work->fieldMenu = 1;
        }
        work->aButton = 0;
      } else if (work->fieldMenu != 0) {
        // The field menu (B + Select in debug mode opens the flag editor).
        AreaNameWindow_HideFar();
        Leader_StopMoving();
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        WorldMap_Freeze();
        GameFlag_Set(FLAG_MENU_OPEN);
        if (gDebugMode && (gInput.held & KEY_B) != 0 &&
            ((gInput.held & KEY_SELECT) != 0)) {
          DebugFlagEditor_RunFar();
        } else if (GameFlag_Get(FLAG_MENUS_RAISE_TRIGGER)) {
          work->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
        } else {
          work->effectTimerRunning = 0;
          FieldMenu_RunFar();
          work->effectTimerRunning = 1;
          Field_UpdateHoverReady();
        }
        WorldMap_ReloadPendingAreaGraphics();
        GameFlag_Clear(FLAG_MENU_OPEN);
        GameFlag_RefreshLureCapFar();
        work->fieldMenu = 0;
      } else if (work->talk != 0) {
        WorldMap_Freeze();
        Scene_TalkToActor(work->talk & (FIELD_TALK_REQUEST - 1));
        WorldMap_ReloadPendingAreaGraphics();
        work->talk = 0;
      } else if (work->examineCell != 0) {
        WorldMap_Freeze();
        Field_ExamineCell(work->examineCell);
        WorldMap_ReloadPendingAreaGraphics();
        work->examineCell = 0;
      } else if (work->psynergy != 0) {
        packed = work->psynergy & ACT_PACKED;
        AreaNameWindow_HideFar();
        if (packed == 0) {
          WorldMap_Freeze();
          if (TLA_PATCH_1)
            wait(1);
        }
        Field_UsePsynergy(work->psynergy);
        if (packed == 0)
          WorldMap_ReloadPendingAreaGraphics();
        work->psynergy = 0;
      } else if (work->item != 0) {
        WorldMap_Freeze();
        Field_UseItem(work->item, work->itemSlot);
        WorldMap_ReloadPendingAreaGraphics();
        work->item = 0;
      } else if (work->pauseMenu != 0) {
        // The Pause menu (in debug mode, B opens the scene select and L the
        // palette editor).
        Sound_PlayFar(SE_MENU_CURSOR_MOVE);
        Leader_StopMoving();
        WorldMap_Freeze();
        GameFlag_Set(FLAG_MENU_OPEN);
        if (*(volatile u8 *)&gDebugMode && (gInput.held & KEY_B)) {
          DebugSceneSelect_RunFar();
        } else if (*(volatile u8 *)&gDebugMode && (gInput.held & KEY_L)) {
          PaletteEditor_Run();
        } else if (GameFlag_Get(FLAG_MENUS_RAISE_TRIGGER)) {
          work->raisedTrigger = FIELD_RAISED_TRIGGER_MENUS;
        } else {
          Event_ResolveAllPendingActions();
          AreaNameWindow_HideFar();
          party->returnMusic = Sound_GetCurrentBgmFar();
          if (!GameFlag_Get(FLAG_IN_BATTLE_ARENA)) {
            // With fewer than PAUSE_MENU_VRAM_BLOCKS of the sprite VRAM blocks
            // free, sprite drawing stays suspended while the menu runs, and a
            // frame passes so that the field's sprites give their VRAM back
            // before it opens.
            struct ActorSystemWork *system = Ram_HeapSlots->actorSystem;
            u8 *owner = gVramCacheBlockOwners;
            s32 freeBlocks = 0;
            s32 n;

            for (n = 0; n < VRAM_BLOCK_COUNT; n++) {
              if (*owner++ == VRAM_BLOCK_OWNER_FREE)
                freeBlocks++;
            }
            if (freeBlocks - PAUSE_MENU_VRAM_BLOCKS < 0) {
              system->suspended = 1;
              wait(1);
            }
            PauseMenu_RunFar(0);
            system->suspended = 0;
          } else {
            // "The Pause Menu is not available now."
            FieldMessage_ShowFar((s32)MsgFieldPauseUnavailable,
                                 FIELD_MESSAGE_ZOOM);
          }
        }
        WorldMap_ReloadPendingAreaGraphics();
        GameFlag_Clear(FLAG_MENU_OPEN);
        work->pauseMenu = 0;
      }
    }
    // Hand the leader back to the player, with the control order for how the
    // party moves, and run the walking update until the next request.
    GameFlag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
    if (Field_GetPartyActor() != NULL)
      Actor_StartRenderModeScriptFar();
    pending = &gMainRoutineWork.sandActive;
    do {
      wait(1);
      leader = Field_GetPartyActor();
      if (gDebugMode == 0 || !GameFlag_Get(FLAG_DEBUG_STEPS_OFF)) {
        if (*pending == 0 ||
            (leader->timer == 0 && *(s16 *)&leader->userValue == 0))
          Field_UpdateWalking(leader->layer, leader->x.fixed, leader->y.fixed,
                              leader->z.fixed);
      }
    } while (Field_CountRequests() == 0);
  }

done:
  if (work->transitionActive != 0) {
    ScreenTransition_Finish(work->transition, work->transitionFrames);
    work->transitionActive = 0;
    wait(work->transitionFrames);
  }
  wait(1);
  HeapSlot_Release(HEAP_SLOT_FIELD);
  return result;
}

/// Returns the cell behavior of the map cell 16 pixels ahead of the party
/// leader when there is something to search there: a fixed search spot
/// (CELL_BEHAVIOR_JAR to CELL_BEHAVIOR_WELL) whose ground is level with the
/// leader or up to 16 pixels above it, or a behavior the scene has an examine
/// trigger for; aboard the ship on the World Map, SHIP_SEA_EXAMINE_CODE before
/// sea of collision 3. Returns 0 otherwise.
s32 Actor_GetTriggerCellAheadOfCurrent(void) {
  struct FieldWork *work;
  struct MapWork *map;
  struct Actor *leader;
  union PolarOffsetPosition probePosition;
  struct MapCell *cell;
  s32 behavior;
  s32 groundHeight;
  s32 result;

  result = 0;
  leader = Field_GetPartyActor();
  work = gFieldWork;
  map = Ram_HeapSlots->mapGrid;
  if (leader != 0) {
    probePosition.vector.x = leader->x.fixed;
    probePosition.vector.y = leader->y.fixed;
    probePosition.vector.z = leader->z.fixed;
    {
      s32 *position = probePosition.coordinates;

      Vector_AddPolarOffset(toFIXED(16), leader->facing, position);
    }
    // World Map cells are 32 pixels square, 32 to a row; map cells 16 pixels
    // square, MAPGRID_SIZE to a row.
    if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
      s32 x, z;

      x = probePosition.vector.x;
      z = probePosition.vector.z;
      cell =
          &((struct MapCell *)Ram_MapBlocks)[((x / toFIXED(32)) & 31) +
                                             (((z / toFIXED(32)) & 31) << 5)];
    } else {
      s32 x, z;

      cell = map->layers[0].cells;
      x = probePosition.vector.x;
      z = probePosition.vector.z;
      cell += (x / toFIXED(MAPGRID_CELL_PIXELS)) +
              (z / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE;
    }
    behavior = cell->collisionCode;
    if (behavior >= CELL_BEHAVIOR_JAR && behavior <= CELL_BEHAVIOR_WELL) {
      groundHeight = MapGrid_GetTerrainHeightFar(
          leader->layer, probePosition.vector.x, probePosition.vector.z);
      if (groundHeight >= leader->y.fixed &&
          groundHeight <= leader->y.fixed + toFIXED(16)) {
        result = behavior;
      }
    } else if (Scene_FindTrigger(TRIGGER_EXAMINE, behavior) != 0) {
      result = behavior;
    }
    if (result == 0 && work->mode == ACTOR_SYSTEM_WORLD_MAP) {
      if (gMainRoutineWork.ownerOverride != 0 &&
          WorldMap_GetCollisionFar(probePosition.coordinates) == 3)
        result = SHIP_SEA_EXAMINE_CODE;
    }
  }
  return result;
}

/// Places the party at the entrance it came in by, from the scene's entrance
/// rows, unless it keeps its position, and sets the map's scroll bounds, at
/// least a screen (240 x 160 pixels) wide and tall. With findEntrance it looks
/// the entrance up and returns 0 when there is none; without, it takes the
/// scene's first row.
s32 Scene_ApplyEntrance(s32 findEntrance) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 entrance = gMainRoutineWork.entrance;
  const struct SceneEntrance *entranceEntry = SCENE_ENTRIES->entrances();
  s32 foundEntrance = 0;
  s32 left;

  if (findEntrance) {
    for (; entranceEntry->entrance != -1; entranceEntry++) {
      if (entranceEntry->entrance == entrance &&
          (entranceEntry->requiredFlag == -1 ||
           GameFlag_Get(entranceEntry->requiredFlag))) {
        foundEntrance = 1;
        break;
      }
    }
    if (!foundEntrance)
      return 0;
  }

  // Camera bounds apply even when returning to a saved party position.
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    SET_POSITION(&gMainRoutineWork, toFIXED(entranceEntry->x),
                 toFIXED(entranceEntry->y), toFIXED(entranceEntry->z));
    gMainRoutineWork.heading = entranceEntry->facing;
    gMainRoutineWork.layer = 0;
  }

  left = entranceEntry->cameraLeft;
  if (left != -1)
    map->minX = toFIXED(left);
  if (entranceEntry->cameraTop != -1)
    map->minZ = toFIXED(entranceEntry->cameraTop);
  if (entranceEntry->cameraRight != -1)
    map->maxX = toFIXED(entranceEntry->cameraRight);
  if (entranceEntry->cameraBottom != -1)
    map->maxZ = toFIXED(entranceEntry->cameraBottom);

  if (map->minX + toFIXED(DISPLAY_WIDTH) > map->maxX)
    map->minX = map->maxX - toFIXED(DISPLAY_WIDTH);
  if (map->minZ + toFIXED(DISPLAY_HEIGHT) > map->maxZ)
    map->minZ = map->maxZ - toFIXED(DISPLAY_HEIGHT);
  return 1;
}

/// A map entrance's flags byte: the party's facing in sixteenths of a turn,
/// then whether it stands 8 pixels up and whether the camera keeps the bounds
/// it has.
#define MAP_ENTRANCE_FACING_MASK 15
#define MAP_ENTRANCE_RAISED (1 << 4)
#define MAP_ENTRANCE_KEEP_BOUNDS (1 << 5)

/// Reads the map's entrance stream (gMapScene) for the current entrance,
/// places the party there unless FLAG_KEEP_PARTY_POSITION keeps it, then sets
/// the camera bounds from the first box that holds it.
s32 Field_PlaceAtEntrance(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MainRoutineWork *work = &gMainRoutineWork;
  FIXED xFixed;
  u8 *cursor = gMapScene;
  s32 *angle;
  u16 *layer;
  s32 entrance = work->entrance;
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

  cursor = gMapScene + (u16)Stream_ReadU16(&cursor);
  angle = &gMainRoutineWork.heading;
  layer = &gMainRoutineWork.layer;
  for (;; cursor += MAP_ENTRANCE_ROW_BYTES) {
    if (*(s8 *)cursor == -1) {
      *angle = FACING_SOUTH;
      {
        u32 cleared = 0;

        *layer = cleared;
      }
      return 0;
    }
    if (*(s8 *)cursor == entrance) {
      cursor++;
      x = Stream_ReadS16(&cursor);
      z = Stream_ReadS16(&cursor);
      xFixed = toFIXED(x);
      zFixed = toFIXED(z);
      height = FIXEDtoINT(MapGrid_GetTerrainHeightFar(0, xFixed, zFixed));
      flags = *cursor;
      base = (s32)gMapScene;
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        gMainRoutineWork.x = xFixed;
        gMainRoutineWork.z = zFixed;
        *angle = (flags & MAP_ENTRANCE_FACING_MASK) * (TRIG_TURN / 16);
        *layer = 0;
        if (flags & MAP_ENTRANCE_RAISED)
          gMainRoutineWork.y = toFIXED(8);
      }
      if (flags & MAP_ENTRANCE_KEEP_BOUNDS)
        return 1;

      cursor = gMapSceneBoundsOffset;
      cursor = (u8 *)((u16)Stream_ReadU16(&cursor) + base);
      while (*(s8 *)cursor != -1) {
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
          if (x >= leftEdge && x <= rightEdge && z - height >= topEdge &&
              z - height <= bottomEdge) {
            map->minX = FIELD_CELL_ORIGIN(left);
            map->minZ = FIELD_CELL_ORIGIN(top);
            map->maxX = FIELD_CELL_ORIGIN(right);
            map->maxZ = FIELD_CELL_ORIGIN(bottom);
            if (map->minX + toFIXED(DISPLAY_WIDTH) > map->maxX)
              map->minX = map->maxX - toFIXED(DISPLAY_WIDTH);
            if (map->minZ + toFIXED(DISPLAY_HEIGHT) > map->maxZ)
              map->minZ = map->maxZ - toFIXED(DISPLAY_HEIGHT);
            break;
          }
        }
      }
      return 1;
    }
  }
}

/// Reads a little-endian halfword at *cursor and moves the cursor past it.
u32 Stream_ReadU16(u8 **cursor) {
  u32 low = *(*cursor)++;
  u32 high = *(*cursor)++;

  return low | (high << 8);
}

/// Reads a signed little-endian halfword at *cursor and moves the cursor past
/// it.
s16 Stream_ReadS16(u8 **cursor) {
  u32 value;

  value = *(*cursor)++;
  value |= *(*cursor)++ << 8;
  return value;
}

/// Places the party where it enters: by the scene's entrance row for
/// gMainRoutineWork.entrance, else on a map by the map's own entrance stream
/// (Field_PlaceAtEntrance), else by the scene's first entrance row.
void Field_ApplyEntrance(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 done;

  done = Scene_ApplyEntrance(TRUE);
  if (work->mode == ACTOR_SYSTEM_MAP && done == 0)
    done = Field_PlaceAtEntrance();
  if (done == 0)
    Scene_ApplyEntrance(FALSE);
}
#undef NIBBLE_LOW
#undef NIBBLE_HIGH
#undef HAZARD_LIMIT_DEFAULT
#undef PLACE_MARKER_ON_FOOT
#undef PLACE_MARKER_ABOARD
#undef PAUSE_MENU_VRAM_BLOCKS
#undef SHIP_SEA_EXAMINE_CODE
#undef MAP_ENTRANCE_FACING_MASK
#undef MAP_ENTRANCE_RAISED
#undef MAP_ENTRANCE_KEEP_BOUNDS
#undef MAP_ENTRANCE_ROW_BYTES

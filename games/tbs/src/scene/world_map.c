#include "script.h"
#include "version.h"
#include "game_state.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/world_map.h"
#include "unit/scene/common/world_map_linked_effects.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "map_render_work.h"
#include "field_service.h"
#include "iwram_call.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "tbs_edition.h"
#include "inventory.h"
#include "resource.h"
#include "mem.h"
#include "field_effect.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "frame.h"
#include "fx_scene.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

// Forward declarations.
static s32 WorldMap_ApplyMapRotation(struct ScriptInterpreter *interpreter);
static void WorldMap_BoardBunzasWagon(void);
static void WorldMap_DepartWithTravelers(void);
static void WorldMap_ShakeTransferStartCamera(void);
static void WorldMap_ShakeTransferLandingCamera(void);
static void WorldMap_UpdateTransferredActorLanding(void);
static void WorldMap_TransferToExit102(void);
static void WorldMap_TransferToExit103(void);
static void WorldMap_TransferToExit104(void);
static void WorldMap_TransferToTolbiShipInteriors(void);
static void WorldMap_TransferToExit106(void);
static void WorldMap_CompleteTransfer(void);
static void WorldMap_DisembarkCarriage(void);
static void WorldMap_RunBlackOrbScene(void);
static void WorldMap_ActivateSite138(s32 actor);
static void Event_StopLeavingShip(void);
static void WorldMap_ShowEndingScene(void);
static void WorldMap_StartTransitionEffect(void);
static void WorldMap_UpdateTransitionEffect(void);
static void WorldMap_MoveActorEightToExit(void);
static s32 WorldMap_ComputeOpposingRewardTotals(void);
static s32 WorldMap_GetSelectedReward(u32 selection);
static void WorldMap_RunOpeningRise(void);
static void WorldMap_UpdateAirborneTravelTimer(void);
static void WorldMap_PlayLighthouseTransition(s32 palette);

// The one actor placed for entrance 75, which travels north to exit 110.
#define ACTOR 8

enum {
  CONTACT_LAST_ACTOR = 65,
  CONTACT_TRIGGER_BASE = 100,
  FLAG_CONTACT_PAUSED = 0x163,
  FLAG_CONTACT_BLOCKED = 0x104
};

// The Djinn met on the world map. The Venus Djinni is Flint, bobbing on the
// map until the party meets him, and later Vine.
enum {
  ACTOR_VENUS_DJINNI = 8,
  ACTOR_MARS_DJINNI = 9,
  ACTOR_MERCURY_DJINNI = 10
};

// The world map's scene flags:
// FLAG_SCENE_TEMP_00: the carried actor has landed on its guide.
// FLAG_SCENE_TEMP_34: the Black Orb has been handed over.
// FLAG_SCENE_TEMP_35: the map's ordinary placements are in use.
// FLAG_SCENE_TEMP_F0: the party is up in the air.
// FLAG_SCENE_TEMP_F1: the airborne journey is over.
// FLAG_SCENE_TEMP_F8: a byte: frames left in the air.

// Walking near Vine, or talking to him: once a battle is nearly due, half the
// time Vine fights; otherwise the next step brings an ordinary battle.
static void Event_MeetVine(void) {
  struct EventWork *work = gEventWork;

  if (gGameState.encounterSteps >=
      Math_Divide(work->encounterThreshold * 9, 10)) {
    if ((u32)Engine_RandomNext() < 0x8000) {
      Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_VENUS_DJINNI, DJINNI_VINE);
      work->encounterBias = 0;
    } else {
      gGameState.encounterSteps = work->encounterThreshold;
    }
  }
}

// Walking near Corona, or talking to him: once a battle is nearly due, Corona
// fights.
static void Event_MeetCorona(void) {
  struct GameState *state = &gGameState;
  s32 *steps = &state->encounterSteps;
  struct EventWork *work = gEventWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_MARS_DJINNI, DJINNI_CORONA);
    work->encounterBias = 0;
  }
}

// Walking near Hail, or talking to him: once a battle is nearly due, Hail
// fights.
static void Event_MeetHail(void) {
  struct GameState *state = &gGameState;
  s32 *steps = &state->encounterSteps;
  struct EventWork *work = gEventWork;

  if (*steps >= Math_Divide(work->encounterThreshold * 9, 10)) {
    Field_RequestDjinnBattle(DJINN_NO_HOP | ACTOR_MERCURY_DJINNI, DJINNI_HAIL);
    work->encounterBias = 0;
  }
}

// Release the spark after its initial timer step.
static void WorldMap_ReleaseSparkAfterDelay(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  if (actor->timer <= 0) {
    actor->timer++;
  } else {
    Engine_ObjectDispatchRelease(actor);
  }
}

// Pulse the orb's scale and emit short-lived sparks.
static void WorldMap_PulseOrbAndSpawnSparks(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *spark;
  s32 x;
  s32 y;
  s32 z;

  if (gFrameCount & 4) {
    actor->scaleX = 13 * Q16_ONE / 10;
    actor->scaleY = 13 * Q16_ONE / 10;
  } else {
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  if (!(gFrameCount & 2))
    return;
  x = actor->x.fixed;
  y = actor->y.fixed;
  z = actor->z.fixed;
  spark = Engine_ObjectCreate(0x11d, x, y, z);
  Engine_AudioPlayCue(SE_246);
  if (spark == 0)
    return;
  spark->motionFlags = 0;
  spark->sprite->image.priority = 1;
  Engine_ActorSetSpriteFlags(spark, 0);
  Object_SetMode(spark, 1);
  spark->timer = 0;
  spark->update = WorldMap_ReleaseSparkAfterDelay;
}

// Cycle the marker's palette and orbit it around its map position.
static void WorldMap_UpdateBobbingMarker(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  s16 *angle;

  if (gFrameCount & 2)
    Engine_ObjectSetPartPalettes(actor, 10);
  else
    Engine_ObjectSetPartPalettes(actor, 7);
  if ((s16)actor->work == 0) {
    angle = (s16 *)&actor->timer;
    actor->x.fixed = PIXELS(5584);
    actor->y.fixed =
        Iwram_MulQ16(Engine_MathSin(*angle << 3), PIXELS(4)) + PIXELS(16);
    actor->z.fixed = PIXELS(1328);
    Vector_AddPolarOffset(PIXELS(16), *angle, &actor->x);
    actor->facing = *angle + FACING_SOUTH;
    *angle += 0x400;
  }
}

// Animate the first Djinni until it has met the party.
static s32
WorldMap_InitializeFirstDjinni(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (Engine_GameFlagIsSet(FLAG_DJINNI_0) != 0) {
    return 0;
  }
  if (Engine_GameFlagIsSet(FLAG_DJINN_TUTORIAL) != 0) {
    return 0;
  }
  actor->update = WorldMap_UpdateBobbingMarker;
  actor->motionFlags = 0;
  actor->timer = 0;
  actor->work = 0;
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  return 0;
}

// Return the world-map entrances.
struct SceneEntrance *WorldMap_GetEntrances(void) { return gWorldMapEntrances; }

// The world map has no additional region table.
struct SceneRegion *WorldMap_GetRegions(void) { return NULL; }

// Return the world-map exits.
u32 *WorldMap_GetExits(void) { return gWorldMapExits; }

// Align the actor to the map and select its flagged animation mode.
static s32
WorldMap_ApplyMapRotationAndMode(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  WorldMap_ApplyMapRotation(interpreter);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_BRIDGE_COLLAPSED) != 0) {
    Object_SetMode(actor, 2);
  }
  return 1;
}

// Align the actor's sprite rotation to the map.
static s32 WorldMap_ApplyMapRotation(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct MapRenderWork *work = gMapWork[0];
  struct AnimationObject *sprite = actor->sprite;

  sprite->rotation = work->rotation;
  sprite->flags = 0;
  return 1;
}

// Enable the actor's collision and align its sprite to the map.
static s32
WorldMap_ApplyMapRotationAndCollision(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  struct MapRenderWork *work = gMapWork[0];
  struct AnimationObject *sprite = actor->sprite;

  actor->collisionFlags |= 1;
  sprite->rotation = work->rotation;
  return 1;
}

// Reset the map marker after the corresponding story progress.
static s32
WorldMap_InitializeProgressMarker(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ObjectSetPartPalettes(actor, 10);
  actor->collisionFlags = 0;
  if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_AT_ISLAND) != 0) {
    Engine_GameFlagSet(FLAG_SCENE_TEMP_F1);
    actor->x.fixed = 0;
    actor->y.fixed = 0;
  }
  return 0;
}

// Blink a visible actor on alternating frames.
static s32 WorldMap_BlinkActorSprite(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if ((actor->spriteStorageKind & 1) && (gFrameCount & 1))
    actor->spriteStorageKind ^= 1;
  return 1;
}

// Select the actors required by the current arrival event.
struct ScenePlacement *WorldMap_GetPlacements(void) {
  switch (gGameState.entrance) {
  case 49:
    if (Engine_GameFlagIsSet(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
        Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
      return gWorldMapPlacements49;
    }
    break;
  case 64:
    if (Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
      return gWorldMapPlacements64;
    }
    break;
  case 65:
  case 70:
    return gWorldMapPlacements65;
  case 71:
    return gWorldMapPlacements71;
  case 72:
    return gWorldMapPlacements72;
  case 73:
    return gWorldMapPlacements73;
  case 66:
  case 67:
  case 68:
  case 69:
  case 75:
    return gWorldMapPlacements66;
  case 80:
    return gWorldMapPlacements80;
  default:
    break;
  }

  Engine_GameFlagSet(FLAG_SCENE_TEMP_35);
  return gWorldMapPlacements;
}

// Choose a passage exit from the side of the actor the leader approaches.
static void WorldMap_SelectExitByX(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct EventWork *work;
  struct FieldActor *subject;
  struct FieldActor *other;

  subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
  other = Object_GetById(gGameState.selectedActor);
  work = gEventWork;
  if (other->x.fixed < subject->x.fixed) {
    work->requestedExit = exitBefore;
  } else {
    work->requestedExit = exitAfter;
  }
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

// Choose a passage exit from the leader's north-south approach.
static void WorldMap_SelectExitByZ(s32 trigger, s32 exitBefore, s32 exitAfter) {
  struct EventWork *work;
  struct FieldActor *subject;
  struct FieldActor *other;

  subject = Object_GetById(trigger - CONTACT_TRIGGER_BASE);
  other = Object_GetById(gGameState.selectedActor);
  work = gEventWork;
  if (other->z.fixed < subject->z.fixed) {
    work->requestedExit = exitBefore;
  } else {
    work->requestedExit = exitAfter;
  }
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

// Walking into a passage: the side the party comes from decides which exit
// takes it through to the other side.
static void Event_PassThroughWestEastPassage(void) {
  WorldMap_SelectExitByX(0x82, 6, 0x2F);
}

// As Event_PassThroughWestEastPassage, for a passage crossed north to south.
static void Event_PassThroughFirstNorthSouthPassage(void) {
  WorldMap_SelectExitByZ(0x96, 0x2E, 0x0B);
}

// As Event_PassThroughFirstNorthSouthPassage.
static void Event_PassThroughSecondNorthSouthPassage(void) {
  WorldMap_SelectExitByZ(0x74, 0x38, 0x15);
}

// As Event_PassThroughFirstNorthSouthPassage.
static void Event_PassThroughThirdNorthSouthPassage(void) {
  WorldMap_SelectExitByZ(0x97, 0x19, 0x36);
}

// As Event_PassThroughFirstNorthSouthPassage.
static void Event_PassThroughFourthNorthSouthPassage(void) {

  WorldMap_SelectExitByZ(0x7D, 0x3B, 0x1E);
}

// Return the world-map interaction table.
struct SceneEvent *WorldMap_GetEvents(void) { return gWorldMapEvents; }

// Cull distant location sprites and detect the leader touching nearby ones.
static void WorldMap_UpdateLocationContacts(void) {
  struct FieldActor *leader;
  struct EventWork *work;
  struct FieldActor *actor;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  s32 leaderReach;
  u32 i;

  leader = Object_GetById(gGameState.selectedActor);
  leaderReach = leader->sprite->scale * leader->radius;
  work = gEventWork;
  actor = work->viewCenter;
  left = actor->x.fixed - PIXELS(160);
  right = actor->x.fixed + PIXELS(160);
  top = actor->z.fixed - PIXELS(300);
  bottom = actor->z.fixed + PIXELS(200);

  for (i = ACTOR_FIRST_PLACED; i <= CONTACT_LAST_ACTOR; i++) {
    s32 x;
    s32 z;
    s32 dx;
    s32 reach;
    s32 scale;

    actor = Engine_ActorLookup(i);
    if (actor == NULL) {
      continue;
    }
    x = actor->x.fixed;
    z = actor->z.fixed;
    if (x < left || x > right || z < top || z > bottom) {
      actor->spriteStorageKind = 0;
      continue;
    }
    actor->spriteStorageKind = 1;
    if (gDebugMode != 0 && Engine_GameFlagIsSet(FLAG_CONTACT_PAUSED) != 0) {
      continue;
    }
    scale = actor->sprite->scale;
    dx = actor->x.fixed - leader->x.fixed;
    if (dx < 0) {
      dx = leader->x.fixed - actor->x.fixed;
    }
    reach = leaderReach + actor->radius * scale;
    if (dx + (actor->z.fixed - leader->z.fixed < 0
                  ? leader->z.fixed - actor->z.fixed
                  : actor->z.fixed - leader->z.fixed) <
            reach &&
        Engine_GameFlagIsSet(FLAG_CONTACT_BLOCKED) == 0) {
      work->touchedTrigger = i + CONTACT_TRIGGER_BASE;
    }
  }
}

// Walking onto this spot: the first time the travel scene plays, later it is
// an ordinary entrance.
static void Event_EnterTransferRoute(void) {
  if (Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
    Engine_EventRequestExit(101);
  } else {
    Engine_AudioPlayCue(SE_MAP_EXIT);
    Engine_EventRequestExit(3);
  }
}

// Walking east off this edge of the map, once allowed.
static void Event_LeaveEastward(void) {

  Engine_EventBegin();
  Engine_EventRequestExit(74);
}

// Restore map state and run the scene selected by the arrival entrance.
s32 WorldMap_Initialize(void) {
  struct GameState *state = &gGameState;
  s16 *entrance = &state->entrance;

  if (*entrance == 99) {
    Engine_GameFlagSet(0x160);
    Engine_GameFlagSet(0x161);
    Engine_GameFlagSet(0x163);
  }
  if (*entrance == 90) {
    WorldMap_PlayLighthouseTransition(0);
    Event_SetDestination((s32)&SceneId_MercuryLighthouseAerie1, 1);
  } else if (*entrance == 91) {
    WorldMap_PlayLighthouseTransition(1);
    Event_SetDestination((s32)&SceneId_VenusLighthouseAerie, 93);
  } else if (*entrance == 78) {
    Engine_EventBegin();
    PartyInventory_Discard(242);
    Engine_EventRequestExit(112);
  } else {
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    gEventWork->startTransition = 0x400;
    gEventWork->transitionFrames = 16;
    Engine_TaskWait(1);
    Engine_CameraSetSpeed(PIXELS(8), PIXELS(1));
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    FieldScheduler_AddCallback(WorldMap_UpdateLocationContacts, 0xc80);
    if (Engine_GameFlagIsSet(FLAG_LAMAKAN_SAND_MONSTER_BEATEN) == 0) {
      FieldMap_SetWindowCellTile(128, 256, 176, 56);
    }
    // Each special entrance resumes its travel or story sequence.
    switch (*entrance) {
    case 1:
      if (GameFlag_IsSet(0x815) == 0) {
        Engine_GameFlagSet(0x815);
        Engine_GameFlagSet(0x85c);
      }
      break;
    case 33:
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
        if (Engine_GameFlagIsSet(FLAG_BLACK_ORB_RETURNED) == 0 &&
            Engine_GameFlagIsSet(FLAG_SCENE_TEMP_34) != 0) {
          gWorldMapTriggerActor = 55;
          Actor_SetPosition(55, PIXELS(6036), PIXELS(3400));
          Object_GetById(gWorldMapTriggerActor)->facing =
              FACING_SOUTHEAST + FACING_STEP;
          WorldMap_ActivateSite138(gWorldMapTriggerActor);
        }
      } else if (Engine_GameFlagIsSet(FLAG_BLACK_ORB_RETURNED) == 0 &&
                 Engine_GameFlagIsSet(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN) ==
                     0) {
        WorldMap_RunBlackOrbScene();
      }
      break;
    case 49:
      if (Engine_GameFlagIsSet(FLAG_BUNZAS_WAGON_LEFT) == 0 &&
          Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        WorldMap_BoardBunzasWagon();
      }
      break;
    case 64:
      if (Engine_GameFlagIsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
        WorldMap_DepartWithTravelers();
      }
      break;
    case 65:
      WorldMap_TransferToExit102();
      break;
    case 66:
      WorldMap_TransferToExit103();
      break;
    case 67:
      WorldMap_TransferToExit104();
      break;
    case 68:
      WorldMap_TransferToTolbiShipInteriors();
      break;
    case 69:
      WorldMap_TransferToExit106();
      break;
    case 70:
      WorldMap_CompleteTransfer();
      break;
    case 71:
      WorldMap_DisembarkCarriage();
      break;
    case 72:
      WorldMap_RunOpeningRise();
      break;
    case 73:
      WorldMap_StartTransitionEffect();
      break;
    case 74:
    case 76:
    case 77:
      Engine_GameFlagSet(FLAG_NOT_ON_MAP);
      if (FieldGameFlag_GetByte(FLAG_SCENE_TEMP_F8) != 0) {
        gGameState.movementMode = 2;
        FieldScheduler_AddCallback(WorldMap_UpdateAirborneTravelTimer, 0xc80);
      }
      break;
    case 75:
      WorldMap_MoveActorEightToExit();
      break;
    case 80:
      WorldMap_ShowEndingScene();
      break;
    default:
      Object_GetById(53)->scaleX = 5 * Q16_ONE / 4;
      Object_GetById(53)->scaleY = 5 * Q16_ONE / 4;
      break;
    }
  }
  return 0;
}

// The actors placed at entrance 49: Bunza's wagon, Bunza and Hammet.
enum { ACTOR_WAGON = 8, ACTOR_BUNZA = 11, ACTOR_HAMMET = 12 };

// Out of West Lunpa Cave by wagon: Bunza and Hammet nod to each other and
// climb aboard with the hero, and the wagon rolls off.
static void WorldMap_BoardBunzasWagon(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Battle_SetObjectFlag5bWhenMode3();
  GameFlag_Set(FLAG_BUNZAS_WAGON_LEFT);
  Actor_SetPosition(ACTOR_BUNZA, PIXELS(5856), PIXELS(1180));
  Actor_SetDestinationOffset(ACTOR_BUNZA, 24, 8);
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Engine_EventWait(60);
  Actor_SetPosition(ACTOR_HAMMET, PIXELS(5856), PIXELS(1180));
  Actor_SetDestinationOffset(ACTOR_HAMMET, 12, 24);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_BUNZA, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_HAMMET, FACING_NORTH + FACING_STEP, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_NOD);
  Engine_EventWait(120);
  Actor_SetPosition(ACTOR_WAGON, PIXELS(5880), PIXELS(1208));
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_HAMMET, ANIM_WALK);
  record = Object_GetById(ACTOR_WAGON);
  if (record != 0) {
    Actor_SetDestination(ACTOR_HAMMET, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_HAMMET);
  Actor_SetPosition(ACTOR_HAMMET, 0, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_BUNZA, ANIM_WALK);
  record = Object_GetById(ACTOR_WAGON);
  if (record != 0) {
    Actor_SetDestination(ACTOR_BUNZA, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_BUNZA);
  Actor_SetPosition(ACTOR_BUNZA, 0, 0);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  record = Object_GetById(ACTOR_WAGON);
  if (record != 0) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, record->x.part.pixel,
                         record->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_EventWait(60);
  Actor_SetSpeed(ACTOR_WAGON, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetDestinationOffset(ACTOR_WAGON, 56, 8);
  Engine_ActorWaitForMove(ACTOR_WAGON);
  Actor_SetDestinationOffset(ACTOR_WAGON, 40, 40);
  Engine_ActorWaitForMove(ACTOR_WAGON);
  Actor_SetDestinationOffset(ACTOR_WAGON, 8, 88);
  Engine_ActorWaitForMove(ACTOR_WAGON);
  Engine_EventCloseScreen();
  Engine_EventRequestExit(108);
  Engine_EventEnd();
}

// Stage the travelers' departure and their transfer to the next scene.
static void WorldMap_DepartWithTravelers(void) {
  struct FieldActor *actor;
  struct FieldActor *spriteActor;
  const s32 *action;

  actor = Object_GetById(15);
  Engine_EventBegin();
  BattleFx_ScheduleRatioTransition(5 * Q16_ONE / 4, 1);
  Engine_TaskWait(4);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Battle_SetObjectFlag5bWhenMode3();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5884, 1576);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_SetPosition(8, PIXELS(5848), PIXELS(1576));
  Engine_TaskWait(1);
  Actor_SetChildValue(8, 15);
  spriteActor = Actor_Get(8);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  Actor_SetSpeed(10, PIXELS(8) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(11, PIXELS(8) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(12, PIXELS(8) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(13, PIXELS(8) / 5, PIXELS(2) / 5);
  Event_PlayCue(SE_141);
  Actor_SetActionScript(10, gTransferArrive10);
  Engine_EventWait(20);
  Actor_SetActionScript(11, gTransferArrive11);
  Engine_EventWait(20);
  Actor_SetActionScript(12, gTransferArrive12);
  Engine_EventWait(20);
  Actor_RunActionScriptAndWait(13, gTransferArrive13);
  Event_PlayCue(SE_289);
  spriteActor = Object_GetById(ACTOR_PARTY_LEADER);
  if (spriteActor != 0) {
    Actor_SetPosition(GARET, spriteActor->x.fixed, spriteActor->z.fixed);
  }
  Actor_WalkToAndWait(GARET, 5892, 1600);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
  Actor_SetPosition(8, PIXELS(5848), PIXELS(1592));
  Engine_TaskWait(1);
  // The travelers decide to depart; gather the actors for the transfer.
  Engine_EventSetMessage(MsgWorldMapWeCantStayAnotherMinute);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 60);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 10);
  Event_PlayCue(SE_141);
  Actor_SetActionScript(10, gTransferDepart10);
  Engine_EventWait(20);
  Actor_SetActionScript(11, gTransferDepart11);
  Engine_EventWait(20);
  Actor_SetActionScript(12, gTransferDepart12);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 10);
  Actor_RunActionScriptAndWait(13, gTransferDepart13);
  Event_PlayCue(SE_289);
  Engine_EventWait(20);
  Battle_ClearObjectFlag5bWhenMode3();
  Engine_ActorSetAnimation(10, 1);
  Engine_ActorSetAnimation(11, 1);
  Engine_ActorSetAnimation(12, 1);
  Engine_ActorSetAnimation(13, 1);
  Camera_MoveTo(PIXELS(5640), -1, PIXELS(1784), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Battle_SetObjectFlag5bWhenMode3();
  Actor_SetPosition(9, PIXELS(5640), PIXELS(1752));
  Engine_TaskWait(1);
  Actor_SetSpeed(9, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_WalkToAndWait(9, 5640, 1736);
  Actor_WalkToAndWait(9, 5624, 1736);
  Actor_WalkToAndWait(9, 5624, 1784);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(9, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Actor_FaceDirection(9, 0, 20);
  Engine_ActorStartRepeatedMotion(9, 3);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_SetPosition(8, PIXELS(5656), PIXELS(1784));
  Engine_TaskWait(1);
  Actor_SetChildValue(8, 0);
  spriteActor = Actor_Get(8);
  Engine_ActorSetSpriteFlags(spriteActor, 1);
  Actor_SetSpeed(8, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(8, 5640, 1784);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 10);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 60);
  Actor_FaceDirection(8, FACING_WEST, 10);
  Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 40);
  Engine_ActorRunRepeatedMotion(9, 1);
  Actor_FaceDirection(9, 0, 10);
  Event_ShowMessageAndWait(9, 0, 10);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 10);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimationAndWait(8, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | 1, 60);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Battle_ClearObjectFlag5bWhenMode3();
  Event_PlayCue(SE_107);
  Camera_SetSpeed(PIXELS(4), PIXELS(4));
  // Focus on the departure effect before moving the group to the landing.
  WorldMap_ShakeTransferStartCamera();
  Event_PlayCue(SE_289);
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(9, EMOTE_IN_FRONT, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(9, 0, 40);
  Actor_FaceDirection(8, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(9, FACING_NORTHWEST + FACING_STEP, 0);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(5608), -1, PIXELS(1736), 1);
  Engine_CameraWaitForMove();
  Actor_SetPosition(14, PIXELS(5544), PIXELS(1704));
  Engine_TaskWait(1);
  Actor_SetSpeed(14, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_SetActionScript(14, gTransferGuide14);
  Engine_EventWait(160);
  // Stage the guide and the transferred actor's landing motion.
  actor->gravity = PIXELS(1) / 10;
  actor->bounce = PIXELS(1) / 10;
  actor->scaleX = 3 * Q16_ONE / 2;
  actor->scaleY = 3 * Q16_ONE / 2;
  actor->timer = 0;
  actor->y.fixed = PIXELS(64);
  actor->sprite->rotation = 0xf000;
  Engine_ActorSetSpriteFlags(actor, 0);
  Object_SetMode(actor, 2);
  Engine_TaskWait(1);
  spriteActor = Actor_Get(15);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  FieldScheduler_AddCallback(WorldMap_UpdateTransferredActorLanding, 0xc80);
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  spriteActor = Actor_Get(15);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  spriteActor = Actor_Get(14);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  Engine_EventWait(10);
  spriteActor = Actor_Get(9);
  spriteActor->velocityY = PIXELS(6);
  spriteActor = Object_GetById(8);
  spriteActor->velocityY = PIXELS(6);
  Event_PlayCue(SE_145);
  Camera_SetSpeed(PIXELS(4), PIXELS(4));
  WorldMap_ShakeTransferLandingCamera();
  WorldMap_ShakeTransferLandingCamera();
  Engine_EventWait(60);
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(5640), -1, PIXELS(1784), 1);
  Engine_CameraWaitForMove();
  Battle_SetObjectFlag5bWhenMode3();
  Actor_SetAttachedEffect(9, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Engine_TaskRemoveCallback(WorldMap_UpdateTransferredActorLanding);
  Engine_TaskWait(1);
  Actor_SetPosition(14, 0, 0);
  Actor_SetPosition(15, 0, 0);
  Actor_FaceDirection(8, FACING_WEST, 10);
  Engine_ActorJump(8, 4, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 10);
  Actor_FaceDirection(9, 0, 10);
  Event_ShowMessageAndWait(9, 0, 20);
  Actor_FaceDirection(8, FACING_NORTH, 40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 20);
  Engine_ActorJump(9, 4, 20);
  Event_ShowMessageAndWait(9, 0, 10);
  Engine_ActorRunRepeatedMotion(8, 1);
  Actor_FaceDirection(8, FACING_WEST, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | 8, 0, 10);
  Actor_SetAttachedEffect(9, EMOTE_IN_FRONT | 2);
  Engine_EventWait(80);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_ActorSetAnimationAndWait(9, 3);
  Actor_WalkToAndWait(8, 5656, 1784);
  Actor_SetPosition(8, 0, 0);
  Actor_WalkToAndWait(9, 5624, 1736);
  Actor_WalkToAndWait(9, 5640, 1736);
  Actor_WalkToAndWait(9, 5640, 1752);
  Actor_SetPosition(9, 0, 0);
  Event_PlayCue(SE_141);
  Actor_SetActionScript(10, gTransferReturn10);
  Actor_SetActionScript(11, gTransferReturn11);
  Engine_EventWait(40);
  Actor_SetActionScript(12, gTransferReturn12);
  Engine_EventWait(40);
  Actor_RunActionScriptAndWait(13, gTransferReturn13);
  Battle_ClearObjectFlag5bWhenMode3();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(5900), PIXELS(1576));
  Actor_SetPosition(GARET, PIXELS(5908), PIXELS(1600));
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(5848), -1, PIXELS(1608), 1);
  Engine_CameraWaitForMove();
  action = gTransferGather;
  Actor_SetActionScript(10, action);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(5848), -1, PIXELS(1544), 1);
  Actor_SetActionScript(11, action);
  Engine_EventWait(20);
  Actor_SetActionScript(12, action);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_SetActionScript(13, action);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Object_RefreshSelectorById(13);
  Event_PlayCue(SE_289);
  Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
  Camera_MoveTo(PIXELS(5880), -1, PIXELS(1608), 1);
  Engine_CameraWaitForMove();
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 80);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  spriteActor = Object_GetById(ACTOR_PARTY_LEADER);
  if (spriteActor != 0) {
    Actor_SetDestination(GARET, spriteActor->x.part.pixel,
                         spriteActor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(5848), -1, PIXELS(1608), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5848, 1576);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_GUARDS_SUSPECT_KALAY);
  Engine_EventRequestExit(3);
  Engine_EventEnd();
}

// Move to the next camera-shake point and hold it for four frames.
static inline void WorldMap_MoveShakeCamera(s32 x, s32 z) {
  Camera_MoveTo(x, -1, z, 1);
  Engine_TaskWait(4);
}

// Shake the camera around the departure point.
static void WorldMap_ShakeTransferStartCamera(void) {
  WorldMap_MoveShakeCamera(PIXELS(5644), PIXELS(1784));
  WorldMap_MoveShakeCamera(PIXELS(5636), PIXELS(1788));
  WorldMap_MoveShakeCamera(PIXELS(5644), PIXELS(1780));
  WorldMap_MoveShakeCamera(PIXELS(5644), PIXELS(1788));
  WorldMap_MoveShakeCamera(PIXELS(5636), PIXELS(1780));
  WorldMap_MoveShakeCamera(PIXELS(5640), PIXELS(1784));
  WorldMap_MoveShakeCamera(PIXELS(5642), PIXELS(1784));
  WorldMap_MoveShakeCamera(PIXELS(5638), PIXELS(1786));
  WorldMap_MoveShakeCamera(PIXELS(5642), PIXELS(1782));
  WorldMap_MoveShakeCamera(PIXELS(5642), PIXELS(1786));
  WorldMap_MoveShakeCamera(PIXELS(5638), PIXELS(1782));
  WorldMap_MoveShakeCamera(PIXELS(5640), PIXELS(1784));
}

// Shake the camera around the landing point.
static void WorldMap_ShakeTransferLandingCamera(void) {
  WorldMap_MoveShakeCamera(PIXELS(5612), PIXELS(1736));
  WorldMap_MoveShakeCamera(PIXELS(5604), PIXELS(1740));
  WorldMap_MoveShakeCamera(PIXELS(5612), PIXELS(1732));
  WorldMap_MoveShakeCamera(PIXELS(5612), PIXELS(1740));
  WorldMap_MoveShakeCamera(PIXELS(5604), PIXELS(1732));
  WorldMap_MoveShakeCamera(PIXELS(5608), PIXELS(1736));
  WorldMap_MoveShakeCamera(PIXELS(5610), PIXELS(1736));
  WorldMap_MoveShakeCamera(PIXELS(5606), PIXELS(1738));
  WorldMap_MoveShakeCamera(PIXELS(5610), PIXELS(1734));
  WorldMap_MoveShakeCamera(PIXELS(5610), PIXELS(1738));
  WorldMap_MoveShakeCamera(PIXELS(5606), PIXELS(1734));
  WorldMap_MoveShakeCamera(PIXELS(5608), PIXELS(1736));
}

// Keep the transferred actor above the guide and trigger its landing once.
static void WorldMap_UpdateTransferredActorLanding(void) {
  struct FieldActor *actor;
  struct FieldActor *guide;

  actor = Object_GetById(15);
  guide = Object_GetById(14);
  actor->x.fixed = guide->x.fixed;
  actor->z.fixed = guide->z.fixed;
  if (actor->y.fixed < PIXELS(10)) {
    actor->y.fixed = PIXELS(10);
    if (GameFlag_IsSet(FLAG_SCENE_TEMP_00) == 0) {
      Event_PlayCue(SE_145);
      Object_SetMode(actor, 3);
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      actor->timer = 1;
    }
  }
}

// Stage the next leg of travel and leave through exit 102.
static void WorldMap_TransferToExit102(void) {
  struct FieldActor *actor;

  actor = Object_GetById(8);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  actor->scaleY = 5 * Q16_ONE / 4;
  actor->scaleX = 5 * Q16_ONE / 4;
  Engine_CameraFollowActor(8, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(8, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(8, 5288, 2328);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(102);
  Engine_EventEnd();
}

// Stage the next leg of travel and leave through exit 103.
static void WorldMap_TransferToExit103(void) {
  struct FieldActor *actor;

  actor = Object_GetById(8);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(8, PIXELS(7944), PIXELS(200));
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(8, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(8, PIXELS(3) / 5, PIXELS(3) / 10);
  actor->timer = 0;
  Actor_SetActionScript(8, gTransferLeaderIdle);
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(103);
  Engine_EventEnd();
}

// Stage the next leg of travel and leave through exit 104.
static void WorldMap_TransferToExit104(void) {
  struct FieldActor *actor;

  actor = Object_GetById(8);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(8, PIXELS(7944), PIXELS(200));
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(8, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(8, PIXELS(3) / 5, PIXELS(3) / 10);
  actor->timer = 0;
  Actor_SetActionScript(8, gTransferLeaderIdle);
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(104);
  Engine_EventEnd();
}

// Stage the travelers' arrival in the ship interior.
static void WorldMap_TransferToTolbiShipInteriors(void) {
  struct FieldActor *actor;

  actor = Object_GetById(8);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(8, PIXELS(7944), PIXELS(200));
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(8, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(8, PIXELS(3) / 5, PIXELS(3) / 10);
  actor->timer = 0;
  Actor_SetActionScript(8, gTransferLeaderIdle);
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(105);
  Engine_EventEnd();
}

// Stage the next leg of travel and leave through exit 106.
static void WorldMap_TransferToExit106(void) {
  struct FieldActor *actor;

  actor = Object_GetById(8);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(8, PIXELS(7944), PIXELS(200));
  actor->scaleX = 5 * Q16_ONE / 4;
  actor->scaleY = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(8, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(8, PIXELS(3) / 5, PIXELS(3) / 10);
  actor->timer = 0;
  if (WorldMap_ComputeOpposingRewardTotals() == 11) {
    Actor_SetActionScript(8, gTransferLeaderTurn);
  } else {
    Actor_SetActionScript(8, gTransferLeaderIdle);
  }
  do {
    Engine_TaskWait(1);
  } while (actor->timer == 0);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(106);
  Engine_EventEnd();
}

// Finish the journey and restore the leader's movement.
static void WorldMap_CompleteTransfer(void) {
  struct FieldActor *actor;

  actor = Object_GetById(8);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(8, PIXELS(5096), PIXELS(2328));
  actor->scaleY = 5 * Q16_ONE / 4;
  actor->scaleX = 5 * Q16_ONE / 4;
  Engine_TaskWait(1);
  Engine_CameraFollowActor(8, 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(8, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_WalkToAndWait(8, 5064, 2328);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_TOLBI_SHIP_REACHED_TOLBI);
  GameFlag_Clear(FLAG_TOLBI_SHIP_ROWING);
  Engine_EventRequestExit(107);
  Engine_EventEnd();
}

// Demonstrate setting a Djinni and placing it on standby.
static void WorldMap_ShowDjinnTutorial(void) {
  Battle_ClearObjectFlag5bWhenMode3();
  BattleFx_ScheduleRatioTransition(Q16_ONE, 6);
  Event_WaitForZoomTransition();
  Battle_SetObjectFlag5bWhenMode3();
  Engine_ActorRunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
  Engine_EventSetMessage(MsgWorldMapAfterBringingDjinniIntoYour);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Engine_EventWait(30);
  Event_PlayCue(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  GameFlag_Clear(0x16f);
  GameFlag_Clear(0x171);
  DjinnMenu_Open();
  Engine_ActorJump(ACTOR_VENUS_DJINNI, 4, 30);
  Engine_EventSetMessage(MsgWorldMapNextIllShowHowCan);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  GameFlag_Clear(0x16f);
  GameFlag_Set(0x171);
  DjinnMenu_Open();
  Engine_EventWait(30);
  Field_RequestBattleFromEncounterTable(12, 6);
}

// Walking up to Flint: he joins the party and shows how Djinn are set and put
// on standby; then comes the tutorial battle.
static void Event_MeetFlint(void) {
  struct FieldActor *djinni;
  struct FieldActor *leader;
  s32 x;
  s32 z;
  s32 count;
  s32 i;

  count = 0;
  djinni = Object_GetById(ACTOR_VENUS_DJINNI);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  x = (leader->x.fixed - PIXELS(5584)) / 2 + PIXELS(5584);
  z = (leader->z.fixed - PIXELS(1328)) / 2 + PIXELS(1328);
  // Introduce the Djinni and demonstrate how it joins the party.
  if (!GameFlag_IsSet(FLAG_DJINN_TUTORIAL)) {
    Party_RestoreHpPp(1);
    GameFlag_Set(FLAG_DJINN_TUTORIAL);
    Event_Begin();
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0)
      Actor_SetPosition(ACTOR_VENUS_DJINNI, leader->x.fixed, leader->z.fixed);
    Djinn_AddToOwner(0, 0, 0);
    DjinnRecovery_AddStandby(0, 0, 0);
    Battle_SetObjectFlag5bWhenMode3();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 60);
    djinni->work = 1;
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    Task_Wait(16);
    Event_SetMessage(MsgWorldMapOh);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Battle_ClearObjectFlag5bWhenMode3();
    BattleFx_ScheduleRatioTransition(6 * Q16_ONE / 5, 6);
    Event_WaitForZoomTransition();
    Battle_SetObjectFlag5bWhenMode3();
    djinni->motionFlags = 2;
    djinni->gravity = PIXELS(1) / 4;
    djinni->speed = PIXELS(1);
    djinni->acceleration = PIXELS(1);
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    FieldObject_SetPosition(djinni, PIXELS(5584), 0, PIXELS(1328));
    for (i = 0; i < 16; i++) {
      djinni->scaleX += Q16_ONE / 32;
      djinni->scaleY += Q16_ONE / 32;
      Task_Wait(1);
    }
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
    Task_Wait(16);
    djinni->update = 0;
    Engine_ObjectSetPartPalettes(djinni, 0);
    djinni->gravity = PIXELS(1);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Event_PlayCue(SE_131);
    Engine_PsynergyBegin(140, 0);
    for (i = 59; i >= 0; i--) {
      if (gFrameCount & 2)
        Engine_ObjectSetPartPalettes(djinni, 7);
      else
        Engine_ObjectSetPartPalettes(djinni, 0);
      if ((gFrameCount & 15) == 0)
        WorldMap_CreateLinkedEffects(djinni);
      Task_Wait(1);
    }
    BattleEffect_CleanupSceneObjects();
    Engine_ObjectSetPartPalettes(djinni, 0);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 30);
    Actor_WalkToAndWait(ACTOR_VENUS_DJINNI, x >> 16, z >> 16);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 40);
    Actor_Jump(ACTOR_VENUS_DJINNI, 4, 30);
    UiWork_PushValueSlot(300, 4);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 30);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    i = 0;
    djinni->motionFlags = 0;
    FieldObject_SetPosition(djinni, x, PIXELS(16), z);
    for (; i < 16; i++) {
      djinni->facing += FACING_STEP;
      Task_Wait(1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    djinni->motionFlags = 2;
    djinni->velocityY = 0;
    djinni->terrainHeight = 0;
    for (i = 7; i >= 0; i--) {
      djinni->facing += FACING_STEP;
      Task_Wait(1);
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_ShowEmote(ACTOR_VENUS_DJINNI, EMOTE_IN_FRONT | 2, 30);
    Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 30);
    Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
    // Each "No" makes Flint plead again; the seventh time he gives up asking.
    count = 0;
  plead:
    if (Event_ChooseYesNo(0, 0) == 1) {
      Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
      Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
      if (count == 6) {
        Event_SetMessage(MsgWorldMapMeanieDontCare);
        Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
        goto listened;
      }
      Event_SetMessage(count + MsgWorldMapComePromiseWont);
      Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
      count++;
      goto plead;
    }
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
    Actor_Jump(ACTOR_VENUS_DJINNI, 4, 20);
    Event_SetMessage(MsgWorldMapSeeWontRegret);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  listened:
    UiWork_PushValueSlot(300, 4);
    Event_PlayCue(MUS_081);
    count = MsgWorldMapAbilityVenusDjinni;
    Message_ShowCentered(count++, 3);
    Event_SetMessage(count);
    Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
    Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
    Event_PlayCue(MUS_009);
    WorldMap_ShowDjinnTutorial();
    Battle_ClearObjectFlag5bWhenMode3();
    return;
  }
  Event_Begin();
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0)
    Actor_SetPosition(ACTOR_VENUS_DJINNI, leader->x.fixed, leader->z.fixed);
  djinni->velocityY = PIXELS(10);
  FieldObject_SetPosition(djinni, x, 0, z);
  Event_Wait(30);
  Battle_SetObjectFlag5bWhenMode3();
  Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_VENUS_DJINNI, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
  // Offer the tutorial again, then demonstrate setting and standby.
  Event_SetMessage(MsgWorldMapSeeDjinnUseful);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Event_PlayCue(SE_MENU_CURSOR_MOVE);
  Menu_AnimateSelectionToEntry(0, 2);
  GameFlag_Set(0x16f);
  GameFlag_Clear(0x171);
  DjinnMenu_Open();
  Event_SetMessage(MsgWorldMapGoSetStandby);
  FieldObject_SetPosition(djinni, PIXELS(5584), 0, PIXELS(1328));
  Event_Wait(30);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Actor_FaceActor(ACTOR_VENUS_DJINNI, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
  if (Event_ChooseYesNo(0, 0) == 1) {
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
    Actor_RunRepeatedMotion(ACTOR_VENUS_DJINNI, 2);
    Event_SetMessage(MsgWorldMapHmmmmExplainAgain);
    Event_OpenMessage(ACTOR_VENUS_DJINNI, 0);
    if (Event_ChooseYesNo(0, 0) != 1) {
      Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
      Actor_WalkToAndWait(ACTOR_VENUS_DJINNI, x >> 16, z >> 16);
      WorldMap_ShowDjinnTutorial();
      Battle_ClearObjectFlag5bWhenMode3();
      return;
    }
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 22);
  Event_SetMessage(MsgWorldMapYeahWantLearn);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_ShowEmote(ACTOR_VENUS_DJINNI, EMOTE_IN_FRONT, 30);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  GameFlag_Set(0x16f);
  GameFlag_Set(0x171);
  DjinnMenu_Open();
  Actor_Jump(ACTOR_VENUS_DJINNI, 2, 20);
  Event_ShowMessage(ACTOR_VENUS_DJINNI, 0);
  Battle_ClearObjectFlag5bWhenMode3();
  Djinn_JoinParty(ACTOR_VENUS_DJINNI, 0, 0);
  Event_PlayCue(MUS_042);
  Event_End();
  GameFlag_Clear(FLAG_DJINN_TUTORIAL);
  GameFlag_Clear(0x16f);
  GameFlag_Clear(0x171);
}

s32 gWorldMapTriggerActor;

// Bring the carriage to a stop and let Isaac disembark.
static void WorldMap_DisembarkCarriage(void) {
  struct FieldActor *actor;

  actor = Object_GetById(10);
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  actor->scaleX = 3 * Q16_ONE / 2;
  actor->scaleY = 3 * Q16_ONE / 2;
  actor->facing = FACING_SOUTH;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_SetPosition(10, PIXELS(5480), PIXELS(2104));
  Engine_TaskWait(1);
  Event_PlayCue(SE_141);
  Actor_SetSpeed(10, PIXELS(8) / 5, PIXELS(2) / 5);
  Engine_ActorSetAnimation(10, 2);
  Actor_MoveToAndWait(10, 5485, 2136);
  Camera_SetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(5560), -1, PIXELS(2136), 1);
  Actor_MoveToAndWait(10, 5534, 2136);
  Actor_MoveToAndWait(10, 5544, 2158);
  Actor_MoveToAndWait(10, 5608, 2168);
  Engine_ActorSetAnimation(10, 1);
  Event_PlayCue(SE_289);
  Engine_EventWait(20);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(5592), PIXELS(2168));
  Engine_TaskWait(1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5576, 2168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Event_PlayCue(SE_141);
  Engine_ActorSetAnimation(10, 2);
  Actor_MoveToAndWait(10, 5624, 2168);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_MoveToAndWait(10, 5624, 2104);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_MoveToAndWait(10, 5565, 2104);
  Actor_MoveToAndWait(10, 5560, 2131);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_MoveToAndWait(10, 5490, 2136);
  Actor_MoveToAndWait(10, 5480, 2104);
  Actor_SetPosition(10, 0, 0);
  Event_PlayCue(SE_289);
  Engine_EventWait(40);
  Camera_MoveTo(PIXELS(5592), -1, PIXELS(2136), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 5592, 2136);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(20);
  Engine_EventEnd();
}

// Introduce the Black Orb and prompt Isaac to use it on the ship.
static void WorldMap_RunBlackOrbScene(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gWorldMapTriggerActor = 55;
  WorldMap_ActivateSite138(55);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Battle_SetObjectFlag5bWhenMode3();
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetPosition(gWorldMapTriggerActor, leader->x.fixed,
                            leader->z.fixed);
  }
  Actor_SetSpeed(gWorldMapTriggerActor, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkToAndWait(gWorldMapTriggerActor, 5992, 3448);
  Engine_ActorFaceDirection(gWorldMapTriggerActor, FACING_EAST, 20);
  Engine_ActorShowEmote(gWorldMapTriggerActor, EMOTE_IN_FRONT, 60);
  Engine_ActorStartRepeatedMotion(gWorldMapTriggerActor, 2);
  Engine_EventSetMessage(MsgWorldMapLook);
  Engine_EventShowMessageAndWait(gWorldMapTriggerActor | SPEAKER_PORTRAIT_LEFT,
                                 0, 10);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Battle_ClearObjectFlag5bWhenMode3();
  BattleFx_ScheduleRatioTransition(7 * Q16_ONE / 5, 10);
  Engine_CameraSetSpeed(PIXELS(8), PIXELS(1));
  Engine_CameraMoveTo(PIXELS(6024), -1, PIXELS(3432), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Battle_SetObjectFlag5bWhenMode3();
  Engine_ActorSetAttachedEffect(gWorldMapTriggerActor, EMOTE_IN_FRONT | 2);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
  Engine_EventShowMessageAndWait(gWorldMapTriggerActor | 0x3000, 0, 40);
  Actor_WalkToAndWait(gWorldMapTriggerActor, 5992, 3400);
  Engine_ActorWalkToAndWait(gWorldMapTriggerActor, 6036, 3400);
  Engine_ActorFaceDirection(gWorldMapTriggerActor,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gBlackOrbLeaderScript);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
  Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
  while (Object_GetById(ACTOR_PARTY_LEADER)->timer == 0) {
    Engine_TaskWait(1);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Engine_ActorShowEmote(gWorldMapTriggerActor, EMOTE_IN_FRONT | 6, 0);
  Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 2);
  Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
  Engine_ActorFaceDirection(gWorldMapTriggerActor, FACING_WEST, 10);
  Engine_EventShowMessage(gWorldMapTriggerActor, 0);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Object_GetById(gWorldMapTriggerActor)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(gWorldMapTriggerActor, 6028, 3400);
  Engine_EventWait(1);
  Object_GetById(gWorldMapTriggerActor)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Object_GetById(gWorldMapTriggerActor)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorWalkToAndWait(gWorldMapTriggerActor, 6036, 3400);
  Engine_EventWait(1);
  Object_GetById(gWorldMapTriggerActor)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ItemShowFound(242, 3);
  Engine_PartyGiveItem(ITEM_BLACK_ORB, 0);
  Engine_ActorRunRepeatedMotion(gWorldMapTriggerActor, 1);
  Engine_EventSetMessage(MsgWorldMapNowUseOnShip);
  Engine_EventShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
  Engine_ActorFaceDirection(gWorldMapTriggerActor,
                            FACING_SOUTHEAST + FACING_STEP, 0);
  Battle_ClearObjectFlag5bWhenMode3();
  BattleFx_ScheduleRatioTransition(Q16_ONE, 10);
  Engine_EventWait(20);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_34);
  Engine_GameFlagSet(FLAG_SAW_SHIP_FROM_VENUS_LIGHTHOUSE);
  gGameState.savedScene = (s32)&SceneId_WorldMap;
  gGameState.savedEntrance = 78;
  Engine_EventEnd();
}

// Enable the site actor and its world-map entrance trigger.
static void WorldMap_ActivateSite138(s32 actor) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldMapEvents[i].control == EVENT_EXIT &&
        gWorldMapEvents[i].trigger == 138) {
      gWorldMapEvents[i].control = EVENT_TOUCH;
      gWorldMapEvents[i].value = (u32)Event_StopLeavingShip;
    }
    if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
      break;
    i++;
  }
  for (i = 0;; i++) {
    if (gWorldMapPlacements[i].sprite == 57) {
      gWorldMapPlacements[i].x = PIXELS(6036);
      gWorldMapPlacements[i].z = PIXELS(3400);
      gWorldMapPlacements[i].facing = FACING_SOUTHEAST + FACING_STEP;
      return;
    }
  }
}

// Restore the exit trigger after the ship event.
static void WorldMap_RestoreExitTrigger(void) {
  s32 i;

  i = 0;
  while (1) {
    if (gWorldMapEvents[i].control == EVENT_TOUCH &&
        gWorldMapEvents[i].trigger == 138) {
      gWorldMapEvents[i].control = EVENT_EXIT;
      gWorldMapEvents[i].value = 33;
      break;
    }
    if (gWorldMapEvents[i].control == SCENE_EVENTS_END)
      break;
    i++;
  }
}

// Talking to the one who gave the Black Orb (or reading his mind): "Now use
// it... on the ship."
static void Event_TalkAboutBlackOrb(void) {
  Engine_EventBegin();
  Actor_FaceActor(55, ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(MsgWorldMapNowUseOnShip);
  Engine_EventShowMessage(gWorldMapTriggerActor, 0);
  Actor_FaceDirection(55, FACING_SOUTHEAST + FACING_STEP, 0);
  Engine_EventEnd();
}

// Walking away from the shipwreck with the Black Orb: "(Hero), where you
// going? I said to use the Orb on Babi's ship!", and the hero walks back.
static void Event_StopLeavingShip(void) {
  Engine_EventBegin();
  Battle_SetObjectFlag5bWhenMode3();
  Engine_EventSetMessage(MsgWorldMapIsaacWhereGoingSaidUse);
  Engine_EventShowMessage(gWorldMapTriggerActor, 0);
  Battle_ClearObjectFlag5bWhenMode3();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 6008, 3400);
  Engine_EventEnd();
}

// Searching the shipwreck: "The wreckage of a ship scuttled off the coast."
static void Event_SearchShipwreck(void) {

  Engine_EventBegin();
#if !TBS_PATCH_3
  Battle_SetObjectFlag5bWhenMode3();
#endif
  Engine_MessageShowCentered(MsgWorldMapWreckageShipScuttledOffCoast, 1);
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_34) != 0) {
    gEventWork->unk172 = 1;
  }
#if !TBS_PATCH_3
  Battle_ClearObjectFlag5bWhenMode3();
#endif
  Engine_EventEnd();
}

#define SITE 55

// Using the Black Orb on the shipwreck: nothing happens, the orb is taken back
// and the party is to press on to Babi Lighthouse.
static void Event_UseBlackOrb(void) {
  struct FieldActor *site;
  struct FieldActor *orb;
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 message;
  struct AnimationObject *sprite;
  u8 *buffer;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Begin();
  Battle_ClearObjectFlag5bWhenMode3();
  BattleFx_ScheduleRatioTransition(7 * Q16_ONE / 5, 6);
  Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
  Camera_MoveTo(PIXELS(6024), -1, PIXELS(3432), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  site = NULL;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  leader->paused = 0;
  ObjectDispatch_StartIdle((struct DispatchObject *)leader);
  if (leader->z.fixed > PIXELS(3432)) {
    if (leader->x.fixed > PIXELS(5998)) {
      FieldObject_SetPosition(leader, PIXELS(5998), leader->y.fixed,
                              PIXELS(3453));
      FieldObject_WaitForMove(leader);
    }
  } else if (leader->x.fixed > PIXELS(6010)) {
    // BUG: site is NULL here; this branch reads its height through null.
    FieldObject_SetPosition(leader, PIXELS(6010), site->y.fixed, PIXELS(3400));
    FieldObject_WaitForMove(leader);
  }
  FieldObject_SetPosition(leader, PIXELS(5993), 0, PIXELS(3432));
  FieldObject_WaitForMove(leader);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
  Battle_SetObjectFlag5bWhenMode3();
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, 28);
  // Present the orb icon before its sparks and the ship's response.
  orb = Object_Create(22, leader->x.fixed + PIXELS(2), PIXELS(38),
                      leader->z.fixed);
  if (orb != NULL) {
    u8 *motion;

    orb->motionFlags = 0;
    sprite = orb->sprite;
    sprite->flags = 0;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    /* FAKEMATCH: a second clear of motionFlags through a byte pointer.
     * Post-reload cse removes it as a no-op (the sprite stores are in
     * another alias set, so the first store's value is still known),
     * and the first store survives flow's dead-store pass because the
     * byte-pointer store conflicts with those sprite stores. Until then
     * it keeps the zero and the address alive across the attribute
     * clears, so the zero takes r1 and the address r0, as the game has
     * them. As plain stores the zero takes r3 and the address a copy in
     * r2 (differ 20 in EN); a repeated struct-field store deletes the
     * first store instead and leaves the clear at the end (differ 6). Held as
     * union FieldObject, as lalivero.c holds the Black Orb, the
     * plain store still differs 20 (2026-10-13). */
    motion = &orb->motionFlags;
    *motion = 0;
    buffer = Heap_Allocate(17, 0x608);
    Item_LoadIcon(ITEM_BLACK_ORB);
    Vram_Load(sprite->slot, 128, buffer + 0x400);
    Heap_Release(17);
    Event_Wait(20);
    orb->update = WorldMap_PulseOrbAndSpawnSparks;
    Event_Wait(80);
  }
  Object_GetById(gWorldMapTriggerActor)->facing =
      FACING_SOUTHEAST + FACING_STEP;
  Actor_ShowEmote(gWorldMapTriggerActor, EMOTE_IN_FRONT, 0);
  Actor_RunRepeatedMotion(gWorldMapTriggerActor, 2);
  message = MsgWorldMapMatter;
  Event_SetMessage(message);
  Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 80);
  if (orb != NULL) {
    Engine_ObjectDispatchRelease(orb);
  }
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(40);
  Actor_Jump(gWorldMapTriggerActor, 6, 40);
  Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gWorldMapTriggerActor, FACING_NORTH + FACING_STEP, 20);
  Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 40);
  Actor_SetAnimationAndWait(gWorldMapTriggerActor, 4);
  Event_ShowMessageAndWait(gWorldMapTriggerActor | 0x9000, 0, 20);
  Actor_FaceDirection(gWorldMapTriggerActor, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
  Actor_SetSpeed(gWorldMapTriggerActor, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetAnimation(gWorldMapTriggerActor, 2);
  site = Actor_Get(SITE);
  FieldObject_SetPosition(site, PIXELS(6010), site->y.fixed, PIXELS(3400));
  FieldObject_WaitForMove(site);
  FieldObject_SetPosition(site, PIXELS(6001), 0, PIXELS(3416));
  FieldObject_WaitForMove(site);
  Actor_SetAnimation(SITE, 1);
  Actor_FaceDirection(gWorldMapTriggerActor, FACING_SOUTH + FACING_STEP, 10);
  Actor_RunRepeatedMotion(gWorldMapTriggerActor, 1);
  Event_ShowMessageAndWait(gWorldMapTriggerActor | SPEAKER_PORTRAIT_LEFT, 0,
                           20);
  Actor_SetSpeed(gWorldMapTriggerActor, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(SITE)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetAnimation(SITE, 2);
  FieldObject_SetPosition(site, PIXELS(5997), 0, PIXELS(3424));
  FieldObject_WaitForMove(site);
  Actor_SetAnimation(SITE, 1);
  Event_Wait(10);
  Actor_SetAnimation(SITE, 2);
  FieldObject_SetPosition(site, PIXELS(6001), 0, PIXELS(3416));
  FieldObject_WaitForMove(site);
  Actor_SetAnimation(SITE, 1);
  Message_ShowCentered(message + 6, 1);
  gEventWork->message++;
  PartyInventory_Discard(ITEM_BLACK_ORB);
  Event_Wait(20);
  Actor_SetAnimation(gWorldMapTriggerActor, 4);
  Event_ShowMessageAndWait(gWorldMapTriggerActor, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(gWorldMapTriggerActor, 3);
  Actor_SetAnimation(gWorldMapTriggerActor, 2);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(gWorldMapTriggerActor, actor->x.part.pixel,
                         actor->z.part.pixel);
  }
  Actor_WaitForMove(gWorldMapTriggerActor);
  Actor_SetPosition(gWorldMapTriggerActor, 0, 0);
  Battle_ClearObjectFlag5bWhenMode3();
  BattleFx_ScheduleRatioTransition(Q16_ONE, 6);
  Event_Wait(20);
  WorldMap_RestoreExitTrigger();
  Engine_ActorDestroy(gWorldMapTriggerActor);
  GameFlag_Clear(FLAG_SCENE_TEMP_34);
  GameFlag_Set(FLAG_BLACK_ORB_RETURNED);
  Event_End();
}

enum {
  TRANSITION_EFFECT_TYPE = 222,
  TRANSITION_EFFECT_LEFT = 6064,
  TRANSITION_EFFECT_TOP = 3148,
  TRANSITION_EFFECT_WIDTH = 40,
  TRANSITION_EFFECT_DEPTH = 30
};

u16 gWorldMapBlend;

#define WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, offset)                       \
  IO_WRITE_QUEUE_PUSH(0x06002000, (u32)((buffer) + (offset)), 0x84000140)

// Show Jenna and Kraden's closing scene before returning to the title.
static void WorldMap_ShowEndingScene(void) {
  PaletteGlow_Update(gGameState.paletteGlow[0], gGameState.paletteGlow[1]);
  Engine_EventBegin();
  BattleFx_ScheduleRatioTransition(Q16_ONE, 0x12c);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_ActorSetAnimation(JENNA, 19);
  Engine_ActorSetAnimation(8, 5);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_TaskWait(1);
  BattleFx_ScheduleRatioTransition(3 * Q16_ONE / 2, 16);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, 1);
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Event_WaitForZoomTransition();
  Battle_SetObjectFlag5bWhenMode3();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(20);
  // Jenna and Kraden discuss the island before their guide arrives.
  Engine_EventSetMessage(MsgWorldMapKradenHowLongWillIsland);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 7, 20);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 5, 80);
  Event_ShowMessageAndWait(8, 0, 10);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 5, 100);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessageAndWait(JENNA, 0, 20);
  Event_ShowMessageAndWait(8, 0, 10);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 2, 20);
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
  Engine_EventWait(80);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 5, 80);
  Event_ShowMessageAndWait(8, 0, 120);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Event_ShowMessageAndWait(JENNA, 0, 40);
  Event_ShowMessageAndWait(8, 0, 20);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessageAndWait(JENNA, 0, 120);
  // Bring the guide into the conversation and lead the group onward.
  Actor_SetSpeed(9, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_SetPosition(9, PIXELS(7644), PIXELS(3460));
  Actor_WalkToAndWait(9, 7572, 3468);
  Actor_WalkToAndWait(9, 7560, 3488);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x6009, 0, 20);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(JENNA, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorSetAnimationAndWait(9, 3);
  Event_ShowMessage(0x6009, 0);
  Map_LoadDefaultCellsAndUpdateBlock();
  Engine_EventWait(20);
  Actor_SetActionScript(9, gPresentGuide9);
  Engine_EventWait(80);
  Engine_ActorSetAnimation(8, 1);
  Engine_ActorJump(8, 4, 40);
  Engine_ActorSetAnimation(JENNA, 1);
  Engine_ActorJump(JENNA, 4, 60);
  Actor_FaceDirection(8, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_SetSpeed(8, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetActionScript(8, gPresentGuide8);
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(7) / 10, PIXELS(7) / 80);
  Camera_MoveTo(PIXELS(7736), -1, PIXELS(3528), 1);
  Actor_SetActionScript(5, gPresentGuide5);
  do {
    Engine_ActorSetAnimation(10, 6);
    Engine_ActorSetAnimation(6, 8);
    Engine_TaskWait(1);
  } while (Object_GetById(5)->timer == 0);
  Engine_EventWait(20);
  Actor_FaceDirection(9, FACING_WEST, 20);
  Actor_SetAttachedEffect(8, EMOTE_IN_FRONT | 2);
  Actor_SetAttachedEffect(JENNA, EMOTE_IN_FRONT | 2);
  Engine_EventWait(40);
  Battle_SetObjectFlag5bWhenMode3();
  Event_ShowTwoMessagesAndWait(5, 7, 13, 2, 12, 8, 9, 4, 4, 3, 0);
  Engine_EventWait(20);
  Map_LoadDefaultCellsAndUpdateBlock();
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(7768), -1, PIXELS(3528), 1);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(8, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_SetSpeed(JENNA, PIXELS(8) / 5, PIXELS(4) / 5);
  Actor_WalkTo(8, 7804, 3512);
  Actor_WalkToAndWait(JENNA, 7788, 3544);
  Engine_ActorSetAnimation(8, 1);
  Battle_SetObjectFlag5bWhenMode3();
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_ShowMessageAndWait(8, 0, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | 5, 0, 40);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessageAndWait(8, 0, 60);
  Map_LoadDefaultCellsAndUpdateBlock();
  Event_PlayCue(MUS_017);
  ColorBuffer_ApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(120);
  Event_SetDestination((s32)&SceneId_Title, 10);
}

// Start the sparkling travel transition and fade out to its destination.
static void WorldMap_StartTransitionEffect(void) {
  Engine_EventBegin();
  Engine_AudioPlayCue(SE_141);
  Engine_ColorBufferApplySource(0, 0);
  Engine_ColorBufferApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(2);
  gEventWork->transitionFrames = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
  Engine_TaskWait(1);
  Engine_CameraSetSpeed(PIXELS(4), PIXELS(1) / 2);
  {
    s32 transitionDelay = 3200;
    SchedulerCallback transitionCallback = WorldMap_UpdateTransitionEffect;
    FieldScheduler_AddCallback(transitionCallback, transitionDelay);
  }
  Engine_ColorBufferApplySource(0, 0);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_MUTED, 1);
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(40);
  Engine_EventWait(240);
  Engine_ColorBufferApplyTarget(0, 0);
  Engine_ColorBufferInterpolate(80);
  Engine_TaskWait(90);
  Engine_EventRequestExit(109);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
  Engine_EventEnd();
}

// Scatter transition particles and jitter the camera.
static void WorldMap_UpdateTransitionEffect(void) {
  u32 x = (u32)Engine_RandomNext() * TRANSITION_EFFECT_WIDTH >> 16;
  u32 z = (u32)Engine_RandomNext() * TRANSITION_EFFECT_DEPTH >> 16;
  struct FieldActor *object;

  object = Engine_ObjectCreate(TRANSITION_EFFECT_TYPE,
                               PIXELS(x) + PIXELS(TRANSITION_EFFECT_LEFT), 0,
                               PIXELS(z) + PIXELS(TRANSITION_EFFECT_TOP));
  if (object != NULL) {
    struct AnimationObject *sprite = object->sprite;
    s32 scale = (((u32)Engine_RandomNext() << 15) >> 16) + 6 * Q16_ONE / 5;

    sprite->flags = 0;
    sprite->image.priority = 2;
    object->motionFlags = 0;
    object->scaleX = scale;
    object->scaleY = scale;
    Object_SetMode(object, 1);
    Engine_ObjectSetScript(object, gTransitionSparkScript);
  }
  if (gFrameCount % 3 == 0) {
    switch (((u32)Engine_RandomNext() << 2) >> 16) {
    case 0:
      Engine_CameraMoveTo(PIXELS(6087), -1, PIXELS(3177), 1);
      break;
    case 1:
      Engine_CameraMoveTo(PIXELS(6089), -1, PIXELS(3175), 1);
      break;
    case 2:
      Engine_CameraMoveTo(PIXELS(6089), -1, PIXELS(3177), 1);
      break;
    case 3:
      Engine_CameraMoveTo(PIXELS(6087), -1, PIXELS(3175), 1);
      break;
    }
  }
}

// Move the arrival actor along the staged route to its exit.
static void WorldMap_MoveActorEightToExit(void) {

  struct FieldActor *actor;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_ActorSetAnimation(ACTOR, 2);
  Actor_SetPosition(ACTOR, PIXELS(4872), PIXELS(808));
  actor = Actor_Get(ACTOR);
  {
    s32 value = FACING_NORTHWEST;

    actor->facing = value;
  }
  Engine_TaskWait(1);
  BattleFx_ScheduleRatioTransition(6 * Q16_ONE / 5, 1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_CameraFollowActor(ACTOR, 1);
  Engine_TaskWait(1);
  gEventWork->startTransition = 0x100;
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR, PIXELS(2) / 5, PIXELS(1) / 5);
  Actor_MoveToAndWait(ACTOR, 4824, 712);
  Actor_MoveToAndWait(ACTOR, 4776, 616);
  Actor_SetSpeed(ACTOR, PIXELS(3) / 10, PIXELS(3) / 20);
  Actor_MoveToAndWait(ACTOR, 4776, 472);
  Actor_SetSpeed(ACTOR, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR, 4760, 456);
  Actor_SetSpeed(ACTOR, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_MoveToAndWait(ACTOR, 4760, 440);
  Engine_ActorSetAnimation(ACTOR, 1);
  Engine_EventWait(40);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(110);
}

// Subtract the opposing pair's rewards from the first pair's total.
static s32 WorldMap_ComputeOpposingRewardTotals(void) {
  s32 positiveTotal = WorldMap_GetSelectedReward(0);
  s32 negativeTotal;

  positiveTotal += WorldMap_GetSelectedReward(2);
  negativeTotal = WorldMap_GetSelectedReward(1);
  negativeTotal += WorldMap_GetSelectedReward(3);
  return positiveTotal - negativeTotal;
}

// Look up the reward selected by the first set progress flag.
static s32 WorldMap_GetSelectedReward(u32 selection) {
  s32 flagBase = 0;
  u32 offset;

  switch (selection) {
  case 0:
    flagBase = 0x92C;
    break;
  case 1:
    flagBase = 0x935;
    break;
  case 2:
    flagBase = 0x917;
    break;
  case 3:
    flagBase = 0x990;
    break;
  }
  for (offset = 0; offset < 9; offset++) {
    if (Engine_GameFlagIsSet(flagBase + offset) != 0)
      return gWorldMapRewards[offset];
  }
  return 0;
}

// Spawn a puff behind the moving actor at staggered intervals.
static void WorldMap_SpawnActorEightPuff(void) {
  struct FieldActor *leader;
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  u32 value;

  if ((*(s32 *)&gFrameCount & 15) != 0)
    return;
  leader = Object_GetById(8);
  actor = Engine_ObjectCreate(222, leader->x.fixed - PIXELS(32),
                              leader->y.fixed, leader->z.fixed - PIXELS(16));
  if (actor == 0)
    return;
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  sprite = actor->sprite;
  if (((u32)Engine_RandomNext() * 2 >> 16)) {
    s32 back = (((u32)Engine_RandomNext() * 48) >> 16) << 16;

    actor->x.fixed -= back >> 1;
    actor->z.fixed -= back;
  } else {
    value = (((u32)Engine_RandomNext() << 5) >> 16) << 16;
    actor->x.fixed += value;
    value = (s32)value >> 1;
    actor->z.fixed += value;
  }
  sprite->flags = 0;
  sprite->image.priority = leader->sprite->image.priority;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = leader->motionFlags;
  ObjectGroup_SetChildValue(actor, 9);
  Object_SetMode(actor, 2);
  Engine_ObjectSetScript(actor, gActorEightPuffScript);
}

// Stage the rising actor and its trailing puffs before the next arrival.
static void WorldMap_RunOpeningRise(void) {
  struct FieldActor *actor;
  struct FieldActor *spriteActor;

  actor = Object_GetById(8);
  Engine_EventWait(60);
  Engine_EventBegin();
  BattleFx_ScheduleRatioTransition(3 * Q16_ONE / 5, 1);
  actor->scaleX = 6 * Q16_ONE / 5;
  actor->scaleY = 6 * Q16_ONE / 5;
  Engine_CameraFollowActor(8, 1);
  Engine_TaskWait(1);
  Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
  spriteActor = Actor_Get(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  spriteActor = Actor_Get(8);
  Engine_ActorSetSpriteFlags(spriteActor, 0);
  Actor_SetSpeed(8, PIXELS(2) / 5, PIXELS(1) / 5);
  actor->timer = 0;
  Actor_SetActionScript(8, gOpeningLeaderRise);
  FieldScheduler_AddCallback(WorldMap_SpawnActorEightPuff, 0xc80);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_WARM, 1);
  gEventWork->transitionFrames = 32;
  Engine_EventOpenScreen();
  Engine_EventWait(120);
  BattleFx_ScheduleRatioTransition(7 * Q16_ONE / 5, 0x12c);
  Engine_EventWait(0x10e);
  gEventWork->transitionFrames = 16;
  PLTT_BACKDROP = 0x7fff;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(111);
}

// Rotate the traveler and count down to the raised travel event.
static void WorldMap_UpdateAirborneTravelTimer(void) {

  struct FieldActor *actor;
  struct EventWork *scene;
  s32 progress;

  actor = Object_GetById(gGameState.selectedActor);
  scene = gEventWork;
  actor->facing = (gFrameCount << 12);

  progress = FieldGameFlag_GetByte(FLAG_SCENE_TEMP_F8);
  if (progress != 0) {
    if (progress == 1) {
      scene->raisedTrigger = 99;
    } else if (Engine_GameFlagIsSet(0x106) == 0) {
      progress -= 1;
    }
  }
  FieldGameFlag_SetByte(FLAG_SCENE_TEMP_F8, progress);
}

// Move the selected actor toward the travel marker and start its timer.
static void WorldMap_StartAirborneTravel(s32 targetActor) {
  s32 alreadyTraveling;
  struct FieldActor *actor;
  struct FieldActor *record;
  s32 id;

  id = gGameState.selectedActor;
  actor = Object_GetById(id);
  alreadyTraveling = GameFlag_IsSet(FLAG_SCENE_TEMP_F0);
  if (alreadyTraveling == 0) {
    Engine_EventBegin();
    Actor_SetAttachedEffect(id, EMOTE_IN_FRONT | 1);
    Engine_ActorSetAnimation(id, 9);
    record = Object_GetById(targetActor);
    if (record != 0) {
      Actor_SetDestination(id, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(id);
    Event_PlayCue(SE_244);
    FieldScheduler_AddCallback(WorldMap_UpdateAirborneTravelTimer, 0xc80);
    actor->motionFlags = alreadyTraveling;
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + PIXELS(32),
                             actor->z.fixed);
    Engine_ActorWaitForMove(id);
    actor->velocityY = alreadyTraveling;
    actor->motionFlags = 4;
    gGameState.movementMode = 2;
    GameFlag_Set(FLAG_SCENE_TEMP_F0);
    FieldGameFlag_SetByte(FLAG_SCENE_TEMP_F8, 180);
    Engine_EventEnd();
    gEventWork->encounterEnemyGroup = 0; // drop any battle this step rolled
  }
}

// Walking onto the marker: the party is lifted up into the air.
static void Event_EnterAirborneTravel(void) {
  WorldMap_StartAirborneTravel(54);
}

// Starting to cast Douse on the marker.
static void Event_StartDousingTravelMarker(void) {
  ((struct BattleFxScene *)gEffectWork)->childOption = 1;
}

// Douse on the marker: the party comes down and a battle begins.
static void Event_DouseTravelMarker(void) {
  struct FieldActor *selectedActor;

  if (gEventWork->raisedTrigger == 99) {
    gEventWork->raisedTrigger = 0;
  }
  Engine_GameFlagClear(FLAG_SCENE_TEMP_F0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_F1);
  FieldGameFlag_SetByte(FLAG_SCENE_TEMP_F8, 0);
  Field_RequestBattleFromEncounterTable(98, 5);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(98, 7);
  selectedActor = Object_GetById(gGameState.selectedActor);
  selectedActor->motionFlags = 2;
}

// When the time in the air runs out: the party and the marker fly off, and
// the map reopens elsewhere.
static void Event_FinishAirborneTravel(void) {
  struct FieldActor *actor = Actor_Get(gGameState.selectedActor);
  struct FieldActor *other = Actor_Get(54);
  s32 frames;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Event_PlayCue(SE_219);
  Engine_ActorSetSpriteFlags(actor, 0);
  other->motionFlags = 0;
  actor->motionFlags = 0;
  actor->velocityY = 0;
  actor->unk61 = 1;
  other->unk61 = 1;
  for (frames = 59; frames >= 0; --frames) {
    actor->velocityY += PIXELS(1) / 5;
    other->velocityY += PIXELS(1) / 5;
    Engine_TaskWait(1);
  }
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
  GameFlag_Set(0x122);
  Event_SetDestination((s32)&SceneId_WorldMap, 27);
}

// Refresh the transition's blend registers on alternating frames.
static void WorldMap_UpdateTransitionBlend(void) {

  QueueIoWriteDelay2(0x04000050, 0x3f41);
  if ((gFrameCount & 2) != 0) {
    QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x0c);
  } else {
    QueueIoWriteDelay2(0x04000052, gWorldMapBlend | 0x10);
  }
}

// Jitter the view around the selected actor during the transition.
static void WorldMap_JitterTransitionCamera(void) {

  struct FieldActor *record;
  s32 sx;
  s32 sy;
  u32 mode;

  record = Actor_Get(gGameState.selectedActor);
  sx = record->x.part.pixel;
  sy = record->z.part.pixel;
  if (gFrameCount % 3 == 0) {
    mode = (u32)(Random_Next() << 2) >> 16;
    switch (mode) {
    case 0:
      Camera_MoveTo((sx << 16) - PIXELS(1), -1, (sy << 16) + PIXELS(1), 1);
      break;
    case 1:
      Engine_CameraMoveTo((sx << 16) + PIXELS(1), -1, (sy << 16) - PIXELS(1),
                          1);
      break;
    case 2:
      Camera_MoveTo((sx << 16) + PIXELS(1), -1, (sy << 16) + PIXELS(1), 1);
      break;
    case 3:
      Camera_MoveTo((sx << 16) - PIXELS(1), -1, (sy << 16) - PIXELS(1), 1);
      break;
    }
  }
}

// Display a transition frame before advancing its animation delay.
static inline void WorldMap_ShowTransitionFrame(u8 *buffer, s32 offset,
                                                s32 frames) {
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, offset);
  Engine_EventWait(frames);
}

// Animate the lighthouse transition frames, palette, and camera.
static void WorldMap_PlayLighthouseTransition(s32 palette) {
  u8 *buffer = Runtime_BumpAllocateAlternatePool(0x4000);

  Engine_TaskWait(1);
  Engine_GameFlagClear(FLAG_KEEP_PARTY_POSITION);
  Map_ResumeAnimation();
  FieldResource_DecodeType01(gWorldMapPackedTiles, buffer);
  FieldResource_DecodeType01(gWorldMapPackedFrames, buffer + 0x1000);
  IO_WRITE_QUEUE_PUSH(0x050001c0, (u32)(gWorldMapPalettes + palette * 32),
                      0x80000010);
  IO_WRITE_QUEUE_PUSH(0x06001000, (u32)buffer, 0x84000400);
  FieldScheduler_AddCallback(WorldMap_UpdateTransitionBlend, 0xc80);
  Engine_EventBegin();
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, 0x3a80);
  Object_GetById(gGameState.selectedActor)->spriteStorageKind = 0;
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_AudioPlayCue(SE_246);

  // Fade through the inward frames, hold, then reverse the animation.
  gWorldMapBlend = 0xe00;
  WorldMap_ShowTransitionFrame(buffer, 0x3480, 2);
  gWorldMapBlend = 0xd00;
  WorldMap_ShowTransitionFrame(buffer, 0x2e80, 2);
  gWorldMapBlend = 0xc00;
  WorldMap_ShowTransitionFrame(buffer, 0x2880, 2);
  gWorldMapBlend = 0xb00;
  WorldMap_ShowTransitionFrame(buffer, 0x2280, 2);
  gWorldMapBlend = 0xa00;
  WorldMap_ShowTransitionFrame(buffer, 0x1c80, 2);
  gWorldMapBlend = 0x900;
  WorldMap_ShowTransitionFrame(buffer, 0x1680, 2);
  gWorldMapBlend = 0x800;
  WorldMap_ShowTransitionFrame(buffer, 0x1080, 140);

  WorldMap_ShowTransitionFrame(buffer, 0x1680, 4);
  WorldMap_ShowTransitionFrame(buffer, 0x1c80, 4);
  WorldMap_ShowTransitionFrame(buffer, 0x2280, 4);
  gWorldMapBlend = 0x900;
  WorldMap_ShowTransitionFrame(buffer, 0x2880, 4);
  gWorldMapBlend = 0xa00;
  WorldMap_ShowTransitionFrame(buffer, 0x2e80, 4);
  gWorldMapBlend = 0xb00;
  WorldMap_ShowTransitionFrame(buffer, 0x3480, 4);
  gWorldMapBlend = 0xc00;
  WORLD_MAP_QUEUE_TRANSITION_FRAME(buffer, 0x3a80);
  Map_LoadAreaGraphics();
  FieldScheduler_AddCallback(WorldMap_JitterTransitionCamera, 0xc80);
  Engine_AudioPlayCue(SE_141);
  gWorldMapBlend = 0xd00;
  Engine_EventWait(4);
  gWorldMapBlend = 0xe00;
  Engine_EventWait(4);
  gWorldMapBlend = 0xf00;
  Engine_EventWait(4);
  gWorldMapBlend = 0x1000;
  Engine_EventWait(45);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  MemFree(buffer);
  Engine_GameFlagSet(0x101);
}

// Blink an unused actor and periodically create its linked effects.
UNUSED static void
WorldMap_UpdateBlinkingLinkedEffects(struct FieldActor *actor) {
  if ((gFrameCount & 2) != 0) {
    Engine_ObjectSetPartPalettes(actor, 7);
  } else {
    Engine_ObjectSetPartPalettes(actor, 0);
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateLinkedEffects(actor);
  }
}

// Cycle an unused actor's palettes and periodically create linked effects.
UNUSED static void
WorldMap_UpdateCyclingLinkedEffects(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    Engine_ObjectSetPartPalettes(actor, ((u32)((gFrameCount >> 1)) % (u32)6));
  }
  if ((gFrameCount & 15) == 0) {
    WorldMap_CreateLinkedEffects(actor);
  }
}

// Advance an unused actor's palette on odd frames.
UNUSED static void WorldMap_CycleActorPalettes(struct FieldActor *actor) {
  if ((gFrameCount & 1) != 0) {
    s32 slot = (gFrameCount >> 1) % 6;

    Engine_ObjectSetPartPalettes(actor, slot);
  }
}

// Shared source is included here in ROM order.
#include "common/effect_vertical.h"
#include "common/world_map_linked_effects.h"

s32 gTransferArrive10[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
    SC_SYNC,
    SC_MOVE(PIXELS(5864), 0, PIXELS(1624)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTransferDepart10[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(5868), 0, PIXELS(1624)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(PIXELS(5656), 0, PIXELS(1800)),
    SC_END,
};

s32 gTransferReturn10[] = {
    SC_ARG(2), SC_MOVE(PIXELS(5800), 0, PIXELS(1800)), SC_SYNC, SC_POS(0, 0, 0),
    SC_END,
};

s32 gTransferGather[] = {
    SC_POS(PIXELS(5868), 0, PIXELS(1704)),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(5872), 0, PIXELS(1528)),
    SC_SYNC,
    SC_MOVE(PIXELS(5880), 0, PIXELS(1352)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_END,
};

s32 gTransferArrive11[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1630)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1640)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTransferDepart11[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1640)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(PIXELS(5640), 0, PIXELS(1768)),
    SC_END,
};

s32 gTransferReturn11[] = {
    SC_ARG(2), SC_MOVE(PIXELS(5640), 0, PIXELS(1795)),
    SC_SYNC,   SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_END,
};

s32 gTransferArrive12[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1614)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1624)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTransferDepart12[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1624)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(PIXELS(5656), 0, PIXELS(1768)),
    SC_END,
};

s32 gTransferReturn12[] = {
    SC_ARG(2), SC_MOVE(PIXELS(5656), 0, PIXELS(1795)),
    SC_SYNC,   SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_END,
};

s32 gTransferArrive13[] = {
    SC_POS(PIXELS(5848), 0, PIXELS(1576)),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(5848), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5795), 0, PIXELS(1544)),
    SC_SYNC,
    SC_MOVE(PIXELS(5800), 0, PIXELS(1598)),
    SC_SYNC,
    SC_MOVE(PIXELS(5832), 0, PIXELS(1608)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gTransferDepart13[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(5863), 0, PIXELS(1608)),
    SC_SYNC,
    SC_MOVE(PIXELS(5868), 0, PIXELS(1736)),
    SC_SYNC,
    SC_ARG(1),
    SC_POS(PIXELS(5672), 0, PIXELS(1768)),
    SC_END,
};

s32 gTransferReturn13[] = {
    SC_ARG(3), SC_MOVE(PIXELS(5672), 0, PIXELS(1795)),
    SC_SYNC,   SC_MOVE(PIXELS(5800), 0, PIXELS(1800)),
    SC_SYNC,   SC_POS(0, 0, 0),
    SC_END,
};

s32 gTransferGuide14[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, Q16_ONE / 10),
    SC_WAIT(1),
    SC_MOVE(PIXELS(5608), 0, PIXELS(1720)),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 655),
    SC_ADD(SW_SCALE_Y, 655),
    SC_WAIT(1),
    SC_LOOP(160, SC_TO(1)),
    SC_END,
};

s32 gBlackOrbLeaderScript[] = {
    SC_SET(0x23, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(6008), 0, PIXELS(3400)),
    SC_SYNC,
    SC_MOVE(PIXELS(6020), 0, PIXELS(3400)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(0x23, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gPresentGuide9[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
    SC_SYNC,
    SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7720), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7752), 0, PIXELS(3496)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

s32 gPresentGuide8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
    SC_SYNC,
    SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7704), 0, PIXELS(3512)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

s32 gPresentGuide5[] = {
    SC_SET(0x23, 0),
    SC_ARG(2),
    SC_MOVE(PIXELS(7564), 0, PIXELS(3476)),
    SC_SYNC,
    SC_MOVE(PIXELS(7576), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7640), 0, PIXELS(3464)),
    SC_SYNC,
    SC_MOVE(PIXELS(7656), 0, PIXELS(3480)),
    SC_SYNC,
    SC_MOVE(PIXELS(7688), 0, PIXELS(3528)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_SET(0x23, 1),
    SC_WAIT(1),
    SC_END,
};

s32 gTransitionSparkScript[] = {
    SC_WAIT(16),
    SC_FREE,
};

s32 gTransferLeaderIdle[] = {
    SC_ARG(2),       SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,         SC_LOOP(40, SC_TOP),
    SC_SET(0x23, 1), SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,         SC_MOVE_BY(PIXELS(-8), 0, 0),
    SC_SYNC,         SC_END,
};

s32 gTransferLeaderTurn[] = {
    SC_ARG(2),       SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
    SC_SYNC,         SC_LOOP(40, SC_TOP),
    SC_SET(0x23, 1), SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
    SC_SYNC,         SC_MOVE_BY(PIXELS(-8), 0, PIXELS(-2)),
    SC_SYNC,         SC_END,
};

s32 gOpeningLeaderRise[] = {
    SC_MOVE_BY(PIXELS(24), 0, 0),
    SC_SYNC,
    SC_LOOP(10, SC_TOP),
    SC_SET(0x23, 1),
    SC_MOVE_BY(PIXELS(24), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(24), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gWorldMapPlacement0[] = {
    SC_CALL(WorldMap_InitializeFirstDjinni),
    SC_END,
};

struct SceneEntrance gWorldMapEntrances[] = {
    {0, CONDITION_ALWAYS, 5656, 0, 1256, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 5680, 0, 1226, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 5688, 0, 1224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 5848, 0, 1608, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 6424, 0, 1320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 6584, 0, 1224, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 6928, 0, 1368, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 7328, 0, 1240, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 7136, 0, 1054, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 5928, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 6024, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 7568, 0, 1416, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 7880, 0, 1592, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 7852, 0, 1688, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {14, CONDITION_ALWAYS, 7544, 0, 2008, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {15, CONDITION_ALWAYS, 7144, 0, 1736, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {16, CONDITION_ALWAYS, 7008, 0, 1624, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {17, CONDITION_ALWAYS, 6888, 0, 1736, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {18, CONDITION_ALWAYS, 6888, 0, 1828, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {19, CONDITION_ALWAYS, 5560, 0, 2136, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {20, CONDITION_ALWAYS, 5592, 0, 2168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {21, CONDITION_ALWAYS, 5424, 0, 2264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {22, CONDITION_ALWAYS, 5004, 0, 2328, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {23, CONDITION_ALWAYS, 4904, 0, 2312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {24, CONDITION_ALWAYS, 4840, 0, 1992, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {25, CONDITION_ALWAYS, 4940, 0, 2648, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {26, CONDITION_ALWAYS, 5656, 0, 2424, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {27, CONDITION_ALWAYS, 5388, 0, 2840, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {28, CONDITION_ALWAYS, 5448, 0, 2920, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {29, CONDITION_ALWAYS, 5864, 0, 3400, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {30, CONDITION_ALWAYS, 5996, 0, 2930, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {31, CONDITION_ALWAYS, 5996, 0, 2840, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {32, CONDITION_ALWAYS, 6032, 0, 3192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {33, CONDITION_ALWAYS, 5976, 0, 3432, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {34, CONDITION_ALWAYS, 5896, 0, 1160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {35, CONDITION_ALWAYS, 5990, 0, 804, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {38, CONDITION_ALWAYS, 7376, 0, 1240, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {39, CONDITION_ALWAYS, 5956, 0, 266, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {40, CONDITION_ALWAYS, 6484, 0, 1212, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {41, CONDITION_ALWAYS, 5480, 0, 2632, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {42, CONDITION_ALWAYS, 5928, 0, 3272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {43, CONDITION_ALWAYS, 7792, 0, 2104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {44, CONDITION_ALWAYS, 5848, 0, 1544, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {45, CONDITION_ALWAYS, 6248, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {46, CONDITION_ALWAYS, 7568, 0, 1352, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {47, CONDITION_ALWAYS, 6976, 0, 1360, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {48, CONDITION_ALWAYS, 6228, 0, 552, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {49, CONDITION_ALWAYS, 5860, 0, 1212, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {50, CONDITION_ALWAYS, 7056, 0, 1656, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {51, CONDITION_ALWAYS, 7000, 0, 1736, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {52, CONDITION_ALWAYS, 6920, 0, 1696, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {53, CONDITION_ALWAYS, 6090, 0, 2232, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {54, CONDITION_ALWAYS, 4940, 0, 2728, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {55, CONDITION_ALWAYS, 5416, 0, 3272, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {56, CONDITION_ALWAYS, 5416, 0, 2808, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {57, CONDITION_ALWAYS, 5444, 0, 2840, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {58, CONDITION_ALWAYS, 5424, 0, 2200, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {59, CONDITION_ALWAYS, 5996, 0, 2856, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {60, CONDITION_ALWAYS, 5480, 0, 2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {61, CONDITION_ALWAYS, 6152, 0, 3212, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {64, CONDITION_ALWAYS, 5848, 0, 1608, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {65, CONDITION_ALWAYS, 5384, 0, 2248, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {66, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {67, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {68, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {69, CONDITION_ALWAYS, 7944, 0, 200, FACING_SOUTH, 0, -1, 128, -1, -1, 0},
    {70, CONDITION_ALWAYS, 5096, 0, 2328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {71, CONDITION_ALWAYS, 5528, 0, 2120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {72, CONDITION_ALWAYS, 7464, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {73, CONDITION_ALWAYS, 6088, 0, 3176, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {74, CONDITION_ALWAYS, 4728, 0, 424, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {75, CONDITION_ALWAYS, 4872, 0, 808, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {76, CONDITION_ALWAYS, 4576, 0, 508, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {77, CONDITION_ALWAYS, 4392, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {78, CONDITION_ALWAYS, 8, 0, 8, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 7552, 0, 3504, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {90, CONDITION_ALWAYS, 6024, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {91, CONDITION_ALWAYS, 6088, 0, 3168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {97, CONDITION_ALWAYS, 6248, 0, 872, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {98, CONDITION_ALWAYS, 4248, 0, 2424, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 5576, 0, 2088, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gWorldMapExits[] = {
    SCENE_EXITS(2),           SCENE_EXIT(1, 5, 10),
    SCENE_EXIT(2, 2, 2),      SCENE_EXIT(3, 20, 1),
    SCENE_EXIT(4, 25, 1),     SCENE_EXIT(5, 30, 1),
    SCENE_EXIT(6, 35, 1),     SCENE_EXIT(7, 36, 1),
    SCENE_EXIT(8, 40, 4),     SCENE_EXIT(9, 50, 1),
    SCENE_EXIT(10, 57, 11),   SCENE_EXIT(11, 39, 1),
    SCENE_EXIT(12, 60, 1),    SCENE_EXIT(13, 68, 1),
    SCENE_EXIT(14, 72, 1),    SCENE_EXIT(15, 74, 1),
    SCENE_EXIT(16, 75, 1),    SCENE_EXIT(17, 88, 1),
    SCENE_EXIT(18, 89, 1),    SCENE_EXIT(19, 2, 19),
    SCENE_EXIT(20, 99, 9),    SCENE_EXIT(21, 107, 1),
    SCENE_EXIT(22, 112, 2),   SCENE_EXIT(23, 135, 1),
    SCENE_EXIT(24, 146, 1),   SCENE_EXIT(25, 152, 1),
    SCENE_EXIT(26, 158, 1),   SCENE_EXIT(27, 167, 10),
    SCENE_EXIT(28, 164, 1),   SCENE_EXIT(29, 169, 1),
    SCENE_EXIT(30, 178, 20),  SCENE_EXIT(31, 177, 1),
    SCENE_EXIT(32, 181, 1),   SCENE_EXIT(33, 171, 4),
    SCENE_EXIT(34, 104, 1),   SCENE_EXIT(35, 49, 1),
    SCENE_EXIT(40, 29, 2),    SCENE_EXIT(41, 158, 6),
    SCENE_EXIT(42, 169, 5),   SCENE_EXIT(43, 70, 13),
    SCENE_EXIT(44, 20, 3),    SCENE_EXIT(45, 49, 1),
    SCENE_EXIT(46, 39, 2),    SCENE_EXIT(47, 35, 2),
    SCENE_EXIT(48, 47, 3),    SCENE_EXIT(49, 106, 1),
    SCENE_EXIT(50, 74, 2),    SCENE_EXIT(51, 74, 3),
    SCENE_EXIT(52, 87, 4),    SCENE_EXIT(53, 92, 4),
    SCENE_EXIT(54, 152, 2),   SCENE_EXIT(55, 165, 2),
    SCENE_EXIT(56, 107, 4),   SCENE_EXIT(59, 178, 21),
    SCENE_EXIT(60, 153, 2),   SCENE_EXIT(61, 181, 3),
    SCENE_EXIT(74, 109, 6),   SCENE_EXIT(75, 113, 1),
    SCENE_EXIT(100, 2, 1),    SCENE_EXIT(101, 2, 64),
    SCENE_EXIT(102, 110, 11), SCENE_EXIT(103, 110, 12),
    SCENE_EXIT(104, 110, 13), SCENE_EXIT(105, 111, 21),
    SCENE_EXIT(106, 110, 14), SCENE_EXIT(107, 109, 18),
    SCENE_EXIT(108, 153, 10), SCENE_EXIT(109, 187, 4),
    SCENE_EXIT(110, 111, 23), SCENE_EXIT(111, 2, 80),
    SCENE_EXIT(112, 105, 10), SCENE_EXITS_END,
};

s32 gWorldMapPlacement2[] = {
    SC_CALL(WorldMap_ApplyMapRotationAndMode),
    SC_END,
};

s32 gWorldMapPlacement1[] = {
    SC_CALL(WorldMap_ApplyMapRotation),
    SC_END,
};

s32 gWorldMapPlacement4[] = {
    SC_CALL(WorldMap_ApplyMapRotationAndCollision),
    SC_END,
};

s32 gWorldMapPlacement3[] = {
    SC_CALL(WorldMap_InitializeProgressMarker),
    SC_CALL(WorldMap_BlinkActorSprite),
    SC_END,
};

// clang-format off
struct ScenePlacement gWorldMapPlacements[] = {
    { 0x5a, FLAG_DJINNI_0 + DJINNI_VINE, (s32)gWorldMapPlacement0,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x5c, FLAG_DJINNI_0 + DJINNI_CORONA, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x5b, FLAG_DJINNI_0 + DJINNI_HAIL, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5678), 0, PIXELS(1200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6976), 0, PIXELS(1624), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5416), 0, PIXELS(2840), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5036), 0, PIXELS(2328), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x109, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5928), 0, PIXELS(270), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x108, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7352), 0, PIXELS(1240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7544), 0, PIXELS(1976), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10b, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6888), 0, PIXELS(1712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10b, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7880), 0, PIXELS(1560), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6584), 0, PIXELS(1192), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5996), 0, PIXELS(2894), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(4904), 0, PIXELS(2280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10d, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6024), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10f, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5656), 0, PIXELS(1112), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x114, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6088), 0, PIXELS(3176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10e, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6952), 0, PIXELS(1362), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x110, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5996), 0, PIXELS(2808), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6424), 0, PIXELS(1302), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6484), 0, PIXELS(1184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(4840), 0, PIXELS(1960), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5656), 0, PIXELS(2392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5480), 0, PIXELS(2600), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x113, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7790), 0, PIXELS(2080), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5976), 0, PIXELS(3400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5988), 0, PIXELS(774), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6224), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6920), 0, PIXELS(1664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6092), 0, PIXELS(2200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x113, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7166), 0, PIXELS(1054), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x113, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7856), 0, PIXELS(1736), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x112, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x112, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x112, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x116, CONDITION_ALWAYS, (s32)gWorldMapPlacement2,
        PIXELS(7568), 0, PIXELS(1384), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x117, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(4942), 0, PIXELS(2690), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f5, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6036), 0, PIXELS(3458), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xa1, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), (s32)gWorldMapPlacement1,
        PIXELS(4760), 0, PIXELS(440), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x121, FLAG_SCENE_TEMP_F1, (s32)gWorldMapPlacement3,
        PIXELS(4392), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x39, CONDITION_ALWAYS, (s32)gWorldMapPlacement4,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
    { GARET, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements64[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x31, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5848), 0, PIXELS(1576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements49[] = {
    { 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x105, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5892), 0, PIXELS(1128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5856), 0, PIXELS(1174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x44, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x31, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements65[] = {
    { 0xa1, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5384), 0, PIXELS(2248), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5422), 0, PIXELS(2232), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x115, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5028), 0, PIXELS(2328), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements66[] = {
    { 0xa1, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5432), 0, PIXELS(2312), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements71[] = {
    { 0x107, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5592), 0, PIXELS(2136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x10c, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(5480), 0, PIXELS(2088), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x9a, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements80[] = {
    { JENNA, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7560), 0, PIXELS(3512), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7816), 0, PIXELS(3528), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x1e, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7544), 0, PIXELS(3496), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2b, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7708), 0, PIXELS(3456), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x23, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7800), 0, PIXELS(3544), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x1f5, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7604), 0, PIXELS(3522), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements72[] = {
    { 0x1f8, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(7464), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gWorldMapPlacements73[] = {
    { 0x114, CONDITION_ALWAYS, (s32)gWorldMapPlacement1,
        PIXELS(6080), 0, PIXELS(3168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gWorldMapEvents[] = {
    { EVENT_TOUCH, 90, FLAG_DJINNI_0, EVENT_SCRIPT(Event_MeetFlint) },
    { EVENT_TOUCH, 95, FLAG_SCENE_TEMP_F1, EVENT_SCRIPT(Event_EnterAirborneTravel) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetVine) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetCorona) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MeetHail) },
    { EVENT_TOUCH, 92, FLAG_DJINNI_0 + DJINNI_VINE, EVENT_SCRIPT(Event_MeetVine) },
    { EVENT_TOUCH, 91, FLAG_DJINNI_0 + DJINNI_CORONA, EVENT_SCRIPT(Event_MeetCorona) },
    { EVENT_TOUCH, 93, FLAG_DJINNI_0 + DJINNI_HAIL, EVENT_SCRIPT(Event_MeetHail) },
    { SEARCH(SEARCH_UNNAMED), 96, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchShipwreck) },
    { USE_ITEM(ITEM_BLACK_ORB), 96, FLAG_BLACK_ORB_RETURNED, EVENT_SCRIPT(Event_UseBlackOrb) },
    { SEARCH(SEARCH_GROUND), 100, 0xfcf, FIND_ITEM(ITEM_TURTLE_BOOTS) },
    { EVENT_TALK, 55, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutBlackOrb) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x437, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutBlackOrb) },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 50, CONDITION_ALWAYS, 50 },
    { EVENT_EXIT, 51, CONDITION_ALWAYS, 51 },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, 28 },
    { EVENT_EXIT, 55, CONDITION_ALWAYS, 55 },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, 29 },
    { EVENT_EXIT, 42, CONDITION_ALWAYS, 42 },
    { EVENT_EXIT, 32, CONDITION_ALWAYS, 32 },
    { EVENT_EXIT, 61, CONDITION_ALWAYS, 61 },
    { EVENT_EXIT, 47, CONDITION_ALWAYS, 47 },
    { EVENT_EXIT, 111, CONDITION_ALWAYS, 1 },
    { EVENT_TOUCH, 112, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterTransferRoute) },
    { EVENT_EXIT, 132, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 133, CONDITION_ALWAYS, 40 },
    { EVENT_EXIT, 123, CONDITION_ALWAYS, 5 },
    { EVENT_TOUCH, 130, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PassThroughWestEastPassage) },
    { EVENT_EXIT, 119, CONDITION_ALWAYS, 7 },
    { EVENT_TOUCH, 150, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PassThroughFirstNorthSouthPassage) },
    { EVENT_EXIT, 144, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 139, CONDITION_ALWAYS, 35 },
    { EVENT_EXIT, 140, CONDITION_ALWAYS, 48 },
    { EVENT_EXIT, 118, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 127, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 122, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 145, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 137, CONDITION_ALWAYS, 43 },
    { EVENT_EXIT, 120, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 113, CONDITION_ALWAYS, 16 },
    { EVENT_EXIT, 142, CONDITION_ALWAYS, 52 },
    { EVENT_EXIT, 121, CONDITION_ALWAYS, 17 },
    { EVENT_EXIT, 143, CONDITION_ALWAYS, 53 },
    { EVENT_EXIT, 124, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 115, CONDITION_ALWAYS, 34 },
    { EVENT_EXIT, 141, CONDITION_ALWAYS, 49 },
    { EVENT_TOUCH, 116, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PassThroughSecondNorthSouthPassage) },
    { EVENT_EXIT, 117, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 126, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 134, CONDITION_ALWAYS, 24 },
    { EVENT_TOUCH, 151, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PassThroughThirdNorthSouthPassage) },
    { EVENT_EXIT, 146, CONDITION_ALWAYS, 60 },
    { EVENT_EXIT, 135, CONDITION_ALWAYS, 26 },
    { EVENT_EXIT, 136, CONDITION_ALWAYS, 41 },
    { EVENT_EXIT, 114, CONDITION_ALWAYS, 27 },
    { EVENT_EXIT, 138, CONDITION_ALWAYS, 33 },
    { EVENT_EXIT, 131, CONDITION_ALWAYS, 31 },
    { EVENT_TOUCH, 125, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PassThroughFourthNorthSouthPassage) },
    { TOUCH_WALKING_INTO(FACING_EAST), 74, IF_FLAG_SET(FLAG_TOLBI_SHIP_AT_ISLAND), EVENT_SCRIPT(Event_LeaveEastward) },
    { 0xc401, 75, CONDITION_ALWAYS, 75 },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, 1), 54, IF_FLAG_SET(FLAG_SCENE_TEMP_F0), EVENT_SCRIPT(Event_StartDousingTravelMarker) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 54, IF_FLAG_SET(FLAG_SCENE_TEMP_F0), EVENT_SCRIPT(Event_DouseTravelMarker) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FinishAirborneTravel) },
    { SCENE_EVENTS_END },
};

u32 gWorldMapRewards[] = {
    7, 5, 8, 4, 9, 12, 6, 3,
    0,
};
// clang-format on

s32 gActorEightPuffScript[] = {
    SC_ADD(SW_X, -PIXELS(1) / 10),
    SC_ADD(SW_Z, PIXELS(1) / 10),
    SC_ADD(SW_SCALE_X, 7 * Q16_ONE / 128),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 512),
    SC_WAIT(1),
    SC_LOOP(48, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

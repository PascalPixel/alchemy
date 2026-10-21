/// @file
/// Crossbone Isle, an island reached from the world map, and the main line of
/// the cave beneath it, in three places by their area names:
/// Crossbone Isle, Crossbone Isle Cave and the Pirate Ship.
///
/// No one on the island speaks: it is a run of Psynergy puzzles and battles
/// that ends with Deadbeard on the Pirate Ship. Its places are met in this
/// order:
///
/// - Crossbone Isle (CrossboneIsle, scene 113), the island itself, reached
///   from the world map (its exit 75; exit 1 leads back to it). Whirlwind
///   blows four ivies away for good (four flags from
///   FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, the first also opening a cell),
///   Growth grows four plants so they can be crossed, for this visit only
///   (FLAG_SCENE_TEMP_40 to _43), two pillars can be pushed, and a Nut circles
///   until Catch takes it (0xfd7). Exit 2 leads down into the cave.
/// - Crossbone Isle Cave (CrossboneIsle2, scene 114), the cave's mouth, with
///   only its two exits.
/// - Crossbone Isle Cave (CrossboneIsle6 to CrossboneIsle14, scenes 126 to
///   134), nine rooms in a row on one map, entered from the east (entrance 1)
///   and left to the west (exit 2). Each has two doors in its north wall into
///   a side room of its own. The first time the party walks into the east door
///   (Event_OpenEastDoor), the monsters behind it attack: encounter table 63
///   for the first four rooms and 84 for the next five, one group per room.
///   Back from the battle (entrance 5) the door opens and the party walks
///   through, and from then on it simply opens (one flag per room from
///   FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED); the west door always opens.
///   Between the doors stands a pillar that, pushed or moved with Move to
///   column 40, sinks into the floor and opens the corridor (one flag per room
///   from FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED). In the first room a chest
///   is a mimic (0xef4). The side rooms of the first eight rooms are in
///   crossbone_isle_pillar_rooms.c and crossbone_isle_platform_rooms.c.
/// - Crossbone Isle Cave (CrossboneIsle3, scene 123), the ninth room's side
///   room: twelve pillars that slide with the leader standing on them until
///   another pillar or the wall stops them, and hops from pillar to pillar,
///   lead to its four chests.
/// - Crossbone Isle Cave (CrossboneIsle4, scene 124), the passage from the
///   ninth room to the Pirate Ship.
/// - Pirate Ship (PirateShip, scene 125), at the end. Stepping on trigger
///   10 (Event_FightDeadbeard) sets off the battle with Deadbeard, encounter
///   table 84's sixth group; when the party comes back (entrance 5)
///   FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED is set and a chest with the Demon
///   Mail appears. A second chest is a mimic (0xef7).
///
/// Scene_Initialize restores each scene as its flags say: the ivy and plants on
/// the island, the sliding pillars' footprints, the corridor's sunk pillars and
/// the chests the mimics leave.
#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "field_effect.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "field_service.h"
#include "call.h"
#include "scene_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "event.h"

static void CrossboneIsle_RestoreSunkPillar(void);
static void CrossboneIsle_OpenEastDoorAfterBattle(void);
static void CrossboneIsle_RestoreIvy(void);
static void CrossboneIsle_DrawPillarPuzzleFootprints(void);

#include "common/object_staged_actor.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

UNUSED static void CrossboneIsle_UnusedCallback(void) {}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle) {
    return gCrossboneIsleEntrances1;
  }
  if (scene == (s32)&SceneId_CrossboneIsle2) {
    return gCrossboneIsleEntrances2;
  }
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    return gCrossboneIsleEntrances3;
  }
  if (scene == (s32)&SceneId_CrossboneIsle4) {
    return gCrossboneIsleEntrances4;
  }
  if (scene == (s32)&SceneId_PirateShip) {
    return gCrossboneIsleEntrances5;
  }
  return &gCrossboneIsleEntrances5[2];
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gCrossboneIsleExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle) {
    return gCrossboneIslePlacements1;
  }
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    return gCrossboneIslePlacements3;
  }
  if (scene <= (s32)&SceneId_CrossboneIsle14 &&
      scene >= (s32)&SceneId_CrossboneIsle6) {
    return gCrossboneIslePlacements6To14;
  }
  return gCrossboneIslePlacementsOther;
}

#include "common/effect_advance.h"

/// Emits a ring of dust around a corridor room's pillar as it sinks.
static void CrossboneIsle_SpawnPillarMovedBurst(s32 objectId) {
  struct FieldActor *actor;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 angleStep;
  s32 angle;
  s32 velocityX;
  s32 velocityZ;

  actor = Object_GetById(objectId);
  options.update = Effect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    velocity.x = Engine_MathCos(angle);
    velocity.y = 0;
    velocityZ = Engine_MathSin(angle);
    velocityX = velocity.x;
    velocity.z = velocityZ;
    velocityX += Math_Divide(velocityX, 3);
    velocity.x = velocityX;
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocityX,
                 velocity.y, velocityZ, 0x01000001, &options);
  }
}

/// Stepping on trigger 10 on the Pirate Ship: Deadbeard attacks (encounter
/// table 84's sixth group), and the party comes back to the ship's entrance 5.
static void Event_FightDeadbeard(void) {

  EV_BEGIN();
  Event_SetNextDestination(gGameState.scene, 5);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(84, 5);
  EV_END();
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsle) {
    return gCrossboneIsleEvents1;
  }
  if (scene == (s32)&SceneId_CrossboneIsle2) {
    return gCrossboneIsleEvents2;
  }
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    return gCrossboneIsleEvents3;
  }
  if (scene == (s32)&SceneId_CrossboneIsle4) {
    return gCrossboneIsleEvents4;
  }
  if (scene == (s32)&SceneId_PirateShip) {
    return gCrossboneIsleEvents5;
  }
  return gCrossboneIsleEventsOther;
}

/// Catch on the Nut circling on Crossbone Isle: the party gets it (0xfd7).
static void Event_CatchNut(void) {
  EV_BEGIN();
  Engine_ActorSetPosition(8, 0, 0);
  Engine_GameFlagSet(0xfd7);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  EV_END();
}

/// Restores each scene's puzzles from its flags: the sliding pillars'
/// footprints in CrossboneIsle3; on the Pirate Ship the mimic's chest and,
/// after Deadbeard, the Demon Mail's; on Crossbone Isle the ivy, plants and
/// Nut; in the corridor rooms the mimic's chest, the sunk pillars and the door
/// opened after a battle.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;
  s32 scene;
  s32 current;
  s32 first;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  // CrossboneIsle3's sliding pillars restore their map footprints alone.
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    CrossboneIsle_DrawPillarPuzzleFootprints();
    return 0;
  }
  if (scene == (s32)&SceneId_PirateShip) {
    if (FLAG_CHECK(0xef7) == 0) {
      Map_CopyCellAttributes(0, 3, 1, 1, 13, 40);
      Map_CopyCellAttributes(0, 2, 1, 1, 15, 40);
      MapObject_SetPosition(101, PIXELS(216), PIXELS(648));
    }
    if (gGameState.scene == scene) {
      if (gGameState.entrance != 5) {
        if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED) == 0) {
          return 0;
        }
      }
      FLAG_SET(FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED);
      Map_CopyCellAttributes(0, 1, 1, 1, 13, 30);
      MapObject_SetPosition(100, PIXELS(216), PIXELS(488));
      return 0;
    }
  }
  current = gGameState.scene;
  if (current == (s32)&SceneId_CrossboneIsle) {
    CrossboneIsle_RestoreIvy();
    Actor_Get(8)->targetX = PIXELS(129);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    if (FLAG_CHECK(FLAG_SCENE_TEMP_40) != 0) {
      actor = Actor_Get(11);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(304), PIXELS(368), 253);
    }
    if (FLAG_CHECK(FLAG_SCENE_TEMP_41) != 0) {
      actor = Actor_Get(12);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(80), PIXELS(368), 253);
    }
    if (FLAG_CHECK(FLAG_SCENE_TEMP_42) != 0) {
      actor = Actor_Get(13);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(96), PIXELS(336), 253);
    }
    if (FLAG_CHECK(FLAG_SCENE_TEMP_43) != 0) {
      actor = Actor_Get(14);
      if (actor != 0) {
        actor->collisionFlags = 0;
        Object_SetMode(actor, 4);
        Engine_ActorSetSpriteFlags(actor, 0);
      }
      Field_SetMapCellCollision(0, PIXELS(144), PIXELS(320), 253);
      Field_SetMapCellCollision(0, PIXELS(752), PIXELS(320), 253);
    }
    if (FLAG_CHECK(0xfd7) != 0) {
      return 0;
    }
    Scene_FloatNut(8);
    return 0;
  }
  // The nine corridor rooms (CrossboneIsle6 to 14) share their door battle,
  // sunk pillar and return entrance.
  first = (s32)&SceneId_CrossboneIsle6;
  if (current == first) {
    if (FLAG_CHECK(0xef4) == 0) {
      Map_CopyCellAttributes(0, 0, 1, 1, 37, 10);
      MapObject_SetPosition(100, PIXELS(600), PIXELS(168));
    }
  }
  current = gGameState.scene;
  if (current >= first) {
    if (current <= (s32)&SceneId_CrossboneIsle14) {
      CrossboneIsle_RestoreSunkPillar();
      if (gGameState.entrance == 5) {
        CrossboneIsle_OpenEastDoorAfterBattle();
      }
    }
  }
  return 0;
}

/// In a corridor room of Crossbone Isle Cave whose pillar has been sunk, it
/// stays sunk in its place.
static void CrossboneIsle_RestoreSunkPillar(void) {
  struct FieldActor *actor;
  s32 cleared;

  actor = Actor_Get(8);
  cleared =
      FLAG_CHECK((gGameState.scene + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                      (s32)&SceneId_CrossboneIsle6)));
  if (cleared != 0) {
    OBJ_POS_SET(8, 650, 168);
    actor->y.fixed = -PIXELS(32);
    Engine_ActorSetSpriteFlags(Actor_Get(8), 0);
    Engine_ActorSetSpritePriority(8, 3);
    actor->motionFlags = 0;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(42, 10, 1, 1, 40, 10);
  } else {
    Object_GetById(8)->motionFlags = cleared;
  }
}

/// Move on a corridor room's pillar in Crossbone Isle Cave, and after each
/// push: once it reaches column 40 it sinks into the floor in a ring of dust,
/// and the corridor is open (a flag per room from
/// FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED).
static void Event_MovePillar(void) {
  struct FieldActor *pillar;
  s16 *sceneNumber;
  struct GameState *state;
  s32 column;

  pillar = Object_GetById(8);
  column = pillar->x.fixed >> 20;
  if (column != 40) {
    return;
  }

#if TBS_PATCH_1
  // Through state, the scene's address is built as the game state's plus an
  // offset, as the bytes have it; &gGameState.scene folds into one literal.
  state = &gGameState;
  sceneNumber = &state->scene;
  if (Engine_GameFlagIsSet(*sceneNumber +
                           (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                            (s32)&SceneId_CrossboneIsle6)) != 0) {
    return;
  }
#endif

  // Play the moved-pillar reaction before hiding it and opening the passage.
  pillar->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;

  WAIT(8);
  CrossboneIsle_SpawnPillarMovedBurst(8);
  Engine_AudioPlayCue(SE_136);
  WAIT(40);

  Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
  Engine_ActorSetSpritePriority(8, 3);

  pillar->motionFlags = 0;
  pillar->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

  // Replace the blocked cell and remember that the room's pillar is sunk.
  Map_CopyCellAttributes(42, 10, 1, 1, column, 10);

#if !TBS_PATCH_1
  state = &gGameState;
  sceneNumber = &state->scene;
#endif
  Engine_GameFlagSet(*sceneNumber + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                     (s32)&SceneId_CrossboneIsle6));
}

/// Walking into a corridor room's pillar in Crossbone Isle Cave (trigger 20):
/// push it a step.
static void Event_PushPillar(void) {
  EV_BEGIN();
  StagedActor_AdvancePair();
  EV_END();
  Event_MovePillar();
}

/// Walking north into a corridor room's west door in Crossbone Isle Cave
/// (trigger 4): it opens, and the party goes through to the side room.
static void Event_OpenWestDoor(void) {
  Engine_MapAnimateCells(gCrossboneIsle_EntranceCells, 37, 7);
  Engine_AudioPlayCue(SE_183);
  Engine_EventRequestExit(4);
}

/// Walking north into a corridor room's east door in Crossbone Isle Cave
/// (trigger 3): the first time, the monsters behind it attack, and the party
/// comes back to entrance 5; once they are beaten, the door opens onto the
/// side room.
static void Event_OpenEastDoor(void) {
  if (Engine_GameFlagIsSet(gGameState.scene +
                           (FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED -
                            (s32)&SceneId_CrossboneIsle6)) == 0) {
    EV_BEGIN();
    Event_SetNextDestination(gGameState.scene, 5);
    Battle_SetStart(3);
    // The first four rooms use encounter table 63; the next five use 84.
    switch (gGameState.scene - (s32)&SceneId_CrossboneIsle6) {
    case 0:
      Field_RequestBattleFromEncounterTable(63, 0);
      break;
    case 1:
      Field_RequestBattleFromEncounterTable(63, 1);
      break;
    case 2:
      Field_RequestBattleFromEncounterTable(63, 2);
      break;
    case 3:
      Field_RequestBattleFromEncounterTable(63, 3);
      break;
    case 4:
      Field_RequestBattleFromEncounterTable(84, 0);
      break;
    case 5:
      Field_RequestBattleFromEncounterTable(84, 1);
      break;
    case 6:
      Field_RequestBattleFromEncounterTable(84, 2);
      break;
    case 7:
      Field_RequestBattleFromEncounterTable(84, 3);
      break;
    case 8:
      Field_RequestBattleFromEncounterTable(84, 4);
      break;
    }
    EV_END();
  } else {
    Engine_MapAnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
    Engine_AudioPlayCue(SE_183);
    Engine_EventRequestExit(3);
  }
}

/// Back from the battle at a corridor room's east door (entrance 5): the door
/// opens, and the party walks through.
static void CrossboneIsle_OpenEastDoorAfterBattle(void) {
  EV_BEGIN();
  OBJ_SPEED(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  FLAG_SET((gGameState.scene + (FLAG_CROSSBONE_ISLE_FIRST_GUARDS_DEFEATED -
                                (s32)&SceneId_CrossboneIsle6)));
  WAIT(30);
  Map_AnimateCells(gCrossboneIsle_EntranceCells, 44, 7);
  Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Engine_EventRequestExit(3);
  EV_END();
}

/// Growth on Crossbone Isle's first plant (actor 11): it grows, and its cell
/// can be crossed.
static void Event_GrowPlant11(s32 actorId) {
  s32 fixedX = PIXELS(304);
  s32 fixedZ = PIXELS(368);
  struct FieldActor *actor = Object_GetById(11);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_40);
}

/// Growth on Crossbone Isle's second plant (actor 12).
static void Event_GrowPlant12(s32 actorId) {
  s32 fixedX = PIXELS(80);
  s32 fixedZ = PIXELS(368);
  struct FieldActor *actor = Object_GetById(12);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_41);
}

/// Growth on Crossbone Isle's third plant (actor 13).
static void Event_GrowPlant13(s32 actorId) {
  s32 fixedX = PIXELS(96);
  s32 fixedZ = PIXELS(336);
  struct FieldActor *actor = Object_GetById(13);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_42);
}

/// Growth on Crossbone Isle's fourth plant (actor 14), which opens two cells.
static void Event_GrowPlant14(s32 actorId) {
  s32 fixedX = PIXELS(144);
  s32 fixedZ = PIXELS(320);
  s32 secondFixedX = PIXELS(752);
  s32 secondFixedZ = PIXELS(320);
  struct FieldActor *actor = Object_GetById(14);

  if (actor != 0) {
    actor->collisionFlags = 0;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  SetMapCellCollision(0, fixedX, fixedZ, 253);
  SetMapCellCollision(0, secondFixedX, secondFixedZ, 253);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_43);
}

/// Whirlwind on Crossbone Isle's first ivy (actor 15): it is blown away for
/// good, and its cell opens.
static void Event_BlowAwayIvy15(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY);
  {

    Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
  }
}

/// Whirlwind on Crossbone Isle's second ivy (actor 16).
static void Event_BlowAwayIvy16(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1);
}

/// Whirlwind on Crossbone Isle's third ivy (actor 17).
static void Event_BlowAwayIvy17(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2);
}

/// Whirlwind on Crossbone Isle's fourth ivy (actor 18).
static void Event_BlowAwayIvy18(void) {
  Engine_GameFlagSet(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3);
}

/// On Crossbone Isle, ivy blown away stays gone; the rest stands tall.
static void CrossboneIsle_RestoreIvy(void) {
  struct FieldActor *actor;

  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY) != 0) {
    Map_CopyCellAttributes(0, 0, 1, 1, 8, 21);
    OBJ_POS_SET(15, 968, 968);
  } else {
    actor = Actor_Get(15);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1) != 0) {
    OBJ_POS_SET(16, 968, 968);
  } else {
    actor = Actor_Get(16);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2) != 0) {
    OBJ_POS_SET(17, 968, 968);
  } else {
    actor = Actor_Get(17);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
  if (FLAG_CHECK(FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3) != 0) {
    OBJ_POS_SET(18, 968, 968);
  } else {
    actor = Actor_Get(18);
    actor->scaleY = 8 * Q16_ONE / 5;
  }
}

/// Walking into one of Crossbone Isle's two pillars: push it if the cell beyond
/// is clear.
static void Event_PushIslePillar(void) {
  struct StagedActorProbe probe;

  EV_BEGIN();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
  }
  EV_END();
}

/// Moves the selected actor and puzzle pillar by the same pixel offset.
static void CrossboneIsle_MoveSelectedActorAndPillar(s32 actorId, s32 xOffset,
                                                     s32 zOffset) {
  struct FieldActor *selectedActor;
  struct FieldActor *pillar;
  s32 fixedX;
  s32 fixedZ;

  selectedActor = Object_GetById(gGameState.selectedActor);
  pillar = Object_GetById(actorId);
  EV_BEGIN();
  {
    fixedX = ((selectedActor->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);
    fixedZ = ((selectedActor->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) +
             PIXELS(8);

    selectedActor->speed = PIXELS(1);
    selectedActor->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(selectedActor, fixedX, selectedActor->y.fixed,
                             fixedZ);
  }
  // Position the pillar on the same cell-center grid as the selected actor.
  Object_SetMode(selectedActor, 27);
  {
    fixedX =
        ((pillar->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);
    fixedZ =
        ((pillar->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);

    pillar->speed = PIXELS(1);
    pillar->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(pillar, fixedX, pillar->y.fixed, fixedZ);
  }
  // The pillar's mode depends on which side of the grid it moves toward.
  if (xOffset < 0 || zOffset < 0) {
    Object_SetMode(pillar, 4);
  } else {
    Object_SetMode(pillar, 3);
  }
  Engine_AudioPlayCue(SE_226);
  FieldObject_WaitForMove(selectedActor);
  Engine_AudioPlayCue(SE_288);
  EV_END();
}

/// Draws the three-cell horizontal footprint centered on this pillar.
static void CrossboneIsle_DrawHorizontalPillarFootprint(s32 actorId,
                                                        s32 sourceColumn,
                                                        s32 sourceRow,
                                                        s32 unused) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    s32 priorityFlags;

    Engine_ActorSetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 row = actor->z.fixed >> 20;
      s32 column = (actor->x.fixed >> 20) - 1;

      Map_CopyCellAttributes(sourceColumn, sourceRow, 3, 1, column, row);
    }
  }
}

/// Draws the three-cell vertical footprint centered on this pillar.
static void CrossboneIsle_DrawVerticalPillarFootprint(s32 actorId,
                                                      s32 sourceColumn,
                                                      s32 sourceRow,
                                                      s32 unused) {
  struct FieldActor *actor = Object_GetById(actorId);

  if (actor != 0) {
    s32 priorityFlags;

    Engine_ActorSetSpritePriority(actorId, 3);
    priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    priorityFlags |= actor->priorityFlags;
    actor->priorityFlags = priorityFlags;
    {
      s32 column = actor->x.fixed >> 20;
      s32 row = (actor->z.fixed >> 20) - 1;

      Map_CopyCellAttributes(sourceColumn, sourceRow, 1, 3, column, row);
    }
  }
}

/// Runs the two animation changes used when touching a puzzle pillar.
static void CrossboneIsle_AnimateTouchedPillar(int actorId) {
  OBJ_MODE(actorId, 1);
  OBJ_MODE(actorId, 2);
}

/// Draws all twelve pillar footprints in Crossbone Isle Cave (CrossboneIsle3).
static void CrossboneIsle_DrawPillarPuzzleFootprints(void) {
  CrossboneIsle_DrawHorizontalPillarFootprint(8, 17, 30, 21);
  CrossboneIsle_DrawHorizontalPillarFootprint(10, 17, 31, 22);
  CrossboneIsle_DrawVerticalPillarFootprint(11, 20, 30, 23);
  CrossboneIsle_DrawVerticalPillarFootprint(12, 21, 30, 24);
  CrossboneIsle_DrawVerticalPillarFootprint(13, 22, 30, 25);
  CrossboneIsle_DrawVerticalPillarFootprint(15, 23, 30, 26);
  CrossboneIsle_DrawHorizontalPillarFootprint(17, 0, 30, 31);
  CrossboneIsle_DrawHorizontalPillarFootprint(18, 0, 31, 32);
  CrossboneIsle_DrawHorizontalPillarFootprint(9, 0, 32, 33);
  CrossboneIsle_DrawVerticalPillarFootprint(19, 4, 30, 34);
  CrossboneIsle_DrawVerticalPillarFootprint(14, 5, 30, 35);
  CrossboneIsle_DrawVerticalPillarFootprint(16, 6, 30, 36);
}

// Crossbone Isle Cave (CrossboneIsle3), the ninth room's side room: its twelve
// sliding pillars. Stepping onto a pillar plays its animation from the start;
// walking onto one in a direction it moves slides it that way with the leader
// on it until another pillar or the wall stops it; walking against a pillar's
// edge hops the leader 32 pixels if the landing is clear.

/// Stepping onto pillar 8.
static void Event_StepOnPillar8(void) { CrossboneIsle_AnimateTouchedPillar(8); }

/// Stepping onto pillar 10.
static void Event_StepOnPillar10(void) {
  CrossboneIsle_AnimateTouchedPillar(10);
}

/// Stepping onto pillar 11.
static void Event_StepOnPillar11(void) {
  CrossboneIsle_AnimateTouchedPillar(11);
}

/// Stepping onto pillar 12.
static void Event_StepOnPillar12(void) {
  CrossboneIsle_AnimateTouchedPillar(12);
}

/// Stepping onto pillar 13.
static void Event_StepOnPillar13(void) {
  CrossboneIsle_AnimateTouchedPillar(13);
}

/// Stepping onto pillar 15.
static void Event_StepOnPillar15(void) {
  CrossboneIsle_AnimateTouchedPillar(15);
}

/// Stepping onto pillar 17.
static void Event_StepOnPillar17(void) {
  CrossboneIsle_AnimateTouchedPillar(17);
}

/// Stepping onto pillar 18.
static void Event_StepOnPillar18(void) {
  CrossboneIsle_AnimateTouchedPillar(18);
}

/// Stepping onto pillar 9.
static void Event_StepOnPillar9(void) { CrossboneIsle_AnimateTouchedPillar(9); }

/// Stepping onto pillar 19.
static void Event_StepOnPillar19(void) {
  CrossboneIsle_AnimateTouchedPillar(19);
}

/// Stepping onto pillar 14.
static void Event_StepOnPillar14(void) {
  CrossboneIsle_AnimateTouchedPillar(14);
}

/// Stepping onto pillar 16.
static void Event_StepOnPillar16(void) {
  CrossboneIsle_AnimateTouchedPillar(16);
}

/// Tests a step, then moves the leader with the pillar-pushing animation.
static s32 CrossboneIsle_TryHop(struct FieldActor *actor,
                                struct FieldPosition *requested) {
  u8 *motionFlags = &actor->motionFlags;
  s32 savedMotionFlags = *motionFlags;
#if TBS_PATCH_1
  struct FieldPosition destination;

  destination.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  destination.y = actor->y.fixed;
  destination.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);
  {
    s32 direction = (actor->facing + 0x2000) & 0xc000;

    Vector_AddPolarOffset(PIXELS(32), direction, &destination);
  }
  // Keep the original motion flags while playing the leader's push step.
  if (FieldObject_CheckMovementCollision(actor, &destination) == 0) {
#else
  if (FieldObject_CheckMovementCollision(actor, requested) == 0) {
#endif
    s32 movingFlags;

    EV_BEGIN();
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(actor, 7);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    movingFlags = 126;
    movingFlags &= *motionFlags;
    *motionFlags = (u8)movingFlags;
    Engine_ActorSetSpriteFlags(actor, 0);
    {
#if TBS_PATCH_1
      s16 *pixelCoordinates = (s16 *)&destination;
#else
      s16 *pixelCoordinates = (s16 *)requested;
#endif

      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, pixelCoordinates[1],
                                pixelCoordinates[5]);
    }
    Object_SetMode(actor, 6);
    Engine_ActorSetSpriteFlags(actor, 1);
    *motionFlags = (u8)savedMotionFlags;
    EV_END();
    return 1;
  }
  return 0;
}

/// Walking north against a pillar's edge.
static s32 Event_HopNorth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + -PIXELS(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking south against a pillar's edge, or at the Pirate Ship's trigger 20.
static s32 Event_HopSouth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + PIXELS(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking west against a pillar's edge.
static s32 Event_HopWest(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + -PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking east against a pillar's edge.
static s32 Event_HopEast(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking north onto pillar 8.
static void Event_SlidePillar8North(void) {
  s32 actorX = Object_GetById(8)->x.fixed >> 20;
  s32 actorZ = Object_GetById(8)->z.fixed >> 20;
  s32 actor12X = Object_GetById(12)->x.fixed >> 20;
  s32 actor15X = Object_GetById(15)->x.fixed >> 20;

  if (actorZ == 19) {
    if (actor12X == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -80);
    } else if (actor15X == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -112);
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -80);
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -112);
    }
  } else if (actorZ == 14) {
    if (actor12X == 24) {
      return;
    }
    if (actor15X == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -64);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -112);
    }
  } else if (actorZ == 10) {
    if (actor15X == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(8, 0, -48);
  } else {
    Event_HopNorth();
    return;
  }
  Engine_TaskWait(2);
  {
    s32 routeEndZ = Object_GetById(8)->z.fixed >> 20;
    s32 routeX = actorX - 1;

    Map_CopyCellAttributes(routeX, actorZ, 3, 1, routeX, routeEndZ);
  }
  Map_CopyCellAttributes(0, 0, 3, 1, actorX - 1, actorZ);
}

/// Copies a horizontal pillar's old footprint to its new row, then clears the
/// old row.
static inline void
CrossboneIsle_RedrawHorizontalPillarRoute(s32 actorId, s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationRow = Object_GetById(actorId)->z.fixed >> 20;
    s32 footprintColumn = column - 1;

    Map_CopyCellAttributes(footprintColumn, row, 3, 1, footprintColumn,
                           destinationRow);
  }
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Copies a vertical pillar's old footprint to its new column, then clears the
/// old one.
static inline void
CrossboneIsle_RedrawVerticalPillarRoute(s32 actorId, s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationColumn = Object_GetById(actorId)->x.fixed >> 20;
    s32 footprintRow = row - 1;

    Map_CopyCellAttributes(column, footprintRow, 1, 3, destinationColumn,
                           footprintRow);
  }
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking south onto pillar 8.
static void Event_SlidePillar8South(void) {
  s32 column = Object_GetById(8)->x.fixed >> 20;
  s32 row = Object_GetById(8)->z.fixed >> 20;
  s32 pillar12Column = Object_GetById(12)->x.fixed >> 20;

  if (row == 7) {
    if (pillar12Column == 24) {
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, 80);
      CrossboneIsle_MoveSelectedActorAndPillar(8, 0, 112);
    }
  } else if (row == 10) {
    if (pillar12Column == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(8, 0, 144);
  } else if (row == 14) {
    CrossboneIsle_MoveSelectedActorAndPillar(8, 0, 80);
  } else {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(8, column, row);
}

/// Walking north onto pillar 10.
static void Event_SlidePillar10North(void) {
  s32 column = Object_GetById(10)->x.fixed >> 20;
  s32 row = Object_GetById(10)->z.fixed >> 20;
  s32 pillar13Column = Object_GetById(13)->x.fixed >> 20;
  s32 pillar15Column = Object_GetById(15)->x.fixed >> 20;

  if (row == 18) {
    if (pillar15Column >= 31 && pillar15Column <= 33) {
      CrossboneIsle_MoveSelectedActorAndPillar(10, 0, -128);
    } else if (pillar13Column >= 31 && pillar13Column <= 33) {
      CrossboneIsle_MoveSelectedActorAndPillar(10, 0, -128);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(10, 0, -112);
      CrossboneIsle_MoveSelectedActorAndPillar(10, 0, -64);
    }
  } else if (row == 10) {
    if (pillar15Column >= 31 && pillar15Column <= 33) {
      return;
    }
    if (pillar13Column >= 31 && pillar13Column <= 33) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(10, 0, -48);
  } else if (row == 7) {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(10, column, row);
}

/// Walking south onto pillar 10.
static void Event_SlidePillar10South(void) {
  s32 column = Object_GetById(10)->x.fixed >> 20;
  s32 row = Object_GetById(10)->z.fixed >> 20;

  if (row != 18) {
    if (row == 10) {
      CrossboneIsle_MoveSelectedActorAndPillar(10, 0, 128);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(10, 0, 112);
      CrossboneIsle_MoveSelectedActorAndPillar(10, 0, 64);
    }
    CrossboneIsle_RedrawHorizontalPillarRoute(10, column, row);
  }
}

/// Walking west onto pillar 11.
static void Event_SlidePillar11West(void) {
  s32 column = Object_GetById(11)->x.fixed >> 20;
  s32 row = Object_GetById(11)->z.fixed >> 20;

  if (column != 30) {
    if (column == 34) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndPillar(11, -64, 0);
    } else if (column == 36) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        CrossboneIsle_MoveSelectedActorAndPillar(11, -32, 0);
      } else {
        CrossboneIsle_MoveSelectedActorAndPillar(11, -96, 0);
      }
    }
    CrossboneIsle_RedrawVerticalPillarRoute(11, column, row);
  }
}

/// Walking east onto pillar 11.
static void Event_SlidePillar11East(void) {
  s32 column = Object_GetById(11)->x.fixed >> 20;
  s32 row = Object_GetById(11)->z.fixed >> 20;

  if (column != 36) {
    if (column == 30) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndPillar(11, 96, 0);
    } else if (column == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(11, 32, 0);
    }
    CrossboneIsle_RedrawVerticalPillarRoute(11, column, row);
  }
}

/// Walking west onto pillar 12.
static void Event_SlidePillar12West(void) {
  s32 column = Object_GetById(12)->x.fixed >> 20;
  s32 row = Object_GetById(12)->z.fixed >> 20;

  if (column == 36) {
    CrossboneIsle_MoveSelectedActorAndPillar(12, -96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(12, -96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(12, -96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(12, -64, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(12, column, row);
}

/// Walking east onto pillar 12.
static void Event_SlidePillar12East(void) {
  s32 column = Object_GetById(12)->x.fixed >> 20;
  s32 row = Object_GetById(12)->z.fixed >> 20;

  if (column == 24) {
    CrossboneIsle_MoveSelectedActorAndPillar(12, 96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(12, 96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(12, 32, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(12, column, row);
}

/// Walking west onto pillar 13.
static void Event_SlidePillar13West(void) {
  s32 column = Object_GetById(13)->x.fixed >> 20;
  s32 row = Object_GetById(13)->z.fixed >> 20;
  s32 pillar10Row = Object_GetById(10)->z.fixed >> 20;
  s32 pillar15Column = Object_GetById(15)->x.fixed >> 20;

  if (column == 36) {
    if (pillar15Column == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -16, 0);
    } else if (pillar10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -32, 0);
    } else if (pillar15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -96, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(13, -80, 0);
    }
  } else if (column == 35) {
    if (pillar15Column == 34) {
      return;
    }
    if (pillar10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -16, 0);
    } else if (pillar15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -80, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(13, -80, 0);
    }
  } else if (column == 34) {
    if (pillar10Row == 7) {
      return;
    }
    if (pillar15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(13, -144, 0);
    }
  } else if (column == 31) {
    if (pillar15Column == 30) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(13, -96, 0);
  } else if (column == 25) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(13, column, row);
}

/// Walking east onto pillar 13.
static void Event_SlidePillar13East(void) {
  s32 column = Object_GetById(13)->x.fixed >> 20;
  s32 row = Object_GetById(13)->z.fixed >> 20;

  Object_GetById(15);
  if (column == 25) {
    CrossboneIsle_MoveSelectedActorAndPillar(13, 96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(13, 80, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndPillar(13, 80, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(13, 32, 0);
  } else if (column == 35) {
    CrossboneIsle_MoveSelectedActorAndPillar(13, 16, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(13, column, row);
}

/// Walking west onto pillar 15.
static void Event_SlidePillar15West(void) {
  s32 column = Object_GetById(15)->x.fixed >> 20;
  s32 row = Object_GetById(15)->z.fixed >> 20;
  s32 pillar8Row = Object_GetById(8)->z.fixed >> 20;
  s32 pillar10Row = Object_GetById(10)->z.fixed >> 20;

  if (column == 35) {
    if (pillar10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, -16, 0);
    } else if (pillar8Row == 7) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(15, -96, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(15, -80, 0);
    }
  } else if (column == 34) {
    if (pillar10Row == 7) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(15, -96, 0);
    CrossboneIsle_MoveSelectedActorAndPillar(15, -64, 0);
  } else if (column == 33) {
    CrossboneIsle_MoveSelectedActorAndPillar(15, -144, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndPillar(15, -80, 0);
  } else if (column == 30) {
    CrossboneIsle_MoveSelectedActorAndPillar(15, -96, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(15, column, row);
}

/// Walking east onto pillar 15.
static void Event_SlidePillar15East(void) {
  s32 column = Object_GetById(15)->x.fixed >> 20;
  s32 row = Object_GetById(15)->z.fixed >> 20;
  s32 pillar10Row = Object_GetById(10)->z.fixed >> 20;
  s32 pillar13Column = Object_GetById(13)->x.fixed >> 20;

  if (column == 24) {
    if (pillar10Row == 7 || pillar13Column == 31) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 96, 0);
    } else if (pillar13Column == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 64, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(15, 80, 0);
    } else if (pillar13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 80, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(15, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 80, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(15, 96, 0);
    }
  } else if (column == 30 || pillar13Column == 31) {
    if (pillar10Row == 7) {
      return;
    }
    if (pillar13Column == 34) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 48, 0);
    } else if (pillar13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 80, 0);
    }
  } else if (column == 33) {
    if (pillar13Column == 34) {
      return;
    }
    if (pillar13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 16, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(15, 32, 0);
    }
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndPillar(15, 16, 0);
  } else if (column == 35) {
    return;
  }
  CrossboneIsle_RedrawVerticalPillarRoute(15, column, row);
}

/// Walking north onto pillar 17.
static void Event_SlidePillar17North(void) {
  s32 column = Object_GetById(17)->x.fixed >> 20;
  s32 row = Object_GetById(17)->z.fixed >> 20;
  s32 pillar19Column = Object_GetById(19)->x.fixed >> 20;

  if (row == 19) {
    if (pillar19Column >= 3 && pillar19Column <= 5) {
      CrossboneIsle_MoveSelectedActorAndPillar(17, 0, -16);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(17, 0, -64);
    }
  } else if (row == 18) {
    if (pillar19Column >= 3 && pillar19Column <= 5) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(17, 0, -48);
  } else if (row == 15) {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(17, column, row);
}

/// Walking south onto pillar 17.
static void Event_SlidePillar17South(void) {
  s32 column = Object_GetById(17)->x.fixed >> 20;
  s32 row = Object_GetById(17)->z.fixed >> 20;

  if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndPillar(17, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndPillar(17, 0, 16);
  } else if (row == 19) {
    return;
  }
  CrossboneIsle_RedrawHorizontalPillarRoute(17, column, row);
}

/// These three columns form the crossing lane for the vertical pillars.
#define IS_IN_PILLAR_CROSSING_COLUMNS(x) ((x) >= 6 && (x) <= 8)

/// Walking north onto pillar 18.
static void Event_SlidePillar18North(void) {
  s32 column;
  s32 row;
  s32 pillar19Column;
  s32 pillar14Column;
  s32 pillar16Column;

  column = Object_GetById(18)->x.fixed >> 20;
  row = Object_GetById(18)->z.fixed >> 20;
  pillar19Column = Object_GetById(19)->x.fixed >> 20;
  pillar14Column = Object_GetById(14)->x.fixed >> 20;
  pillar16Column = Object_GetById(16)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar19Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -16);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -64);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -64);
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar19Column)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -48);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -144);
    }
  } else if (row == 15) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -96);
    }
  } else if (row == 14) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -80);
    }
  } else if (row == 12) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -48);
  } else if (row == 11) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(18, 0, -32);
  } else if (row == 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(18)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_PILLAR_CROSSING_COLUMNS

/// Walking south onto pillar 18.
static void Event_SlidePillar18South(void) {
  s32 column;
  s32 row;
  s32 companion19Column;
  s32 companion14Column;

  column = Object_GetById(18)->x.fixed >> 20;
  row = Object_GetById(18)->z.fixed >> 20;
  companion19Column = Object_GetById(19)->x.fixed >> 20;
  companion14Column = Object_GetById(14)->x.fixed >> 20;

  if (row == 9) {
    if ((u32)(companion14Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 32);
    } else if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 80);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 64);
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(companion14Column - 6) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(companion19Column - 6) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndPillar(18, 0, 16);
  } else if (row == 19) {
    return;
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(18)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_PILLAR_CROSSING_COLUMNS
#define IS_IN_PILLAR_CROSSING_COLUMNS(x) ((x) >= 9 && (x) <= 11)

/// Walking north onto pillar 9.
static void Event_SlidePillar9North(void) {
  s32 column;
  s32 row;
  s32 pillar19Column;
  s32 pillar14Column;
  s32 pillar16Column;

  column = Object_GetById(9)->x.fixed >> 20;
  row = Object_GetById(9)->z.fixed >> 20;
  pillar19Column = Object_GetById(19)->x.fixed >> 20;
  pillar14Column = Object_GetById(14)->x.fixed >> 20;
  pillar16Column = Object_GetById(16)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar19Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -16);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -64);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -80);
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar19Column)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -48);
    } else if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -96);
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -64);
    }
  } else if (row == 15) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -112);
    }
  } else if (row == 14) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar14Column)) {
      return;
    }
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -96);
    }
  } else if (row == 12) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -64);
  } else if (row == 11) {
    if (IS_IN_PILLAR_CROSSING_COLUMNS(pillar16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(9, 0, -48);
  } else if ((u32)row <= 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(9)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_PILLAR_CROSSING_COLUMNS

/// Walking south onto pillar 9.
static void Event_SlidePillar9South(void) {
  s32 column;
  s32 row;
  s32 companion19Column;
  s32 companion14Column;
  s32 companion16Column;

  column = Object_GetById(9)->x.fixed >> 20;
  row = Object_GetById(9)->z.fixed >> 20;
  companion19Column = Object_GetById(19)->x.fixed >> 20;
  companion14Column = Object_GetById(14)->x.fixed >> 20;
  companion16Column = Object_GetById(16)->x.fixed >> 20;

  if (row == 8) {
    if ((u32)(companion16Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion14Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 48);
    } else {
      if ((u32)(companion19Column - 9) > 2) {
        CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 80);
      }
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(companion14Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(companion14Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(companion19Column - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndPillar(9, 0, 16);
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(9)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking west onto pillar 19.
static void Event_SlidePillar19West(void) {
  s32 column;
  s32 row;
  s32 pillar17Row;
  s32 pillar18Row;
  s32 pillar9Row;

  column = Object_GetById(19)->x.fixed >> 20;
  row = Object_GetById(19)->z.fixed >> 20;
  pillar17Row = Object_GetById(17)->z.fixed >> 20;
  pillar18Row = Object_GetById(18)->z.fixed >> 20;
  pillar9Row = Object_GetById(9)->z.fixed >> 20;
  if (column == 3) {
    return;
  }
  if (column == 13) {
    if (pillar9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -16, 0);
    } else if (pillar18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -64, 0);
    } else if (pillar17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -112, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(19, -48, 0);
    }
  } else if (column == 6) {
    if (pillar17Row == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(19, -48, 0);
  } else if (column == 5) {
    CrossboneIsle_MoveSelectedActorAndPillar(19, -32, 0);
  } else if (column == 8) {
    if (pillar18Row == 15) {
      return;
    }
    if (pillar17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -80, 0);
    }
  } else if (column == 9) {
    if (pillar18Row == 15) {
      return;
    }
    if (pillar17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -96, 0);
    }
  } else if (column == 12) {
    if (pillar9Row == 15) {
      return;
    }
    if (pillar18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -48, 0);
    } else if (pillar17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -96, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(19, -144, 0);
    }
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(19)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking east onto pillar 19.
static void Event_SlidePillar19East(void) {
  s32 column;
  s32 row;
  s32 pillar18Row;
  s32 pillar9Row;

  column = Object_GetById(19)->x.fixed >> 20;
  row = Object_GetById(19)->z.fixed >> 20;
  pillar18Row = Object_GetById(18)->z.fixed >> 20;
  pillar9Row = Object_GetById(9)->z.fixed >> 20;
  if (column == 3) {
    if (pillar18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, 32, 0);
    } else if (pillar9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(19, 112, 0);
      CrossboneIsle_MoveSelectedActorAndPillar(19, 48, 0);
    }
  } else if (column == 5) {
    if (pillar18Row == 15) {
      return;
    }
    if (pillar9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, 48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(19, 128, 0);
    }
  } else if (column == 6) {
    if (pillar9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndPillar(19, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(19, 112, 0);
    }
  } else if (column == 8) {
    if (pillar9Row == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(19, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndPillar(19, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndPillar(19, 16, 0);
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(19)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking west onto pillar 14.
static void Event_SlidePillar14West(void) {
  s32 column;
  s32 row;
  s32 companion18Row;
  s32 companion9Row;
  s32 footprintRow;

  column = Object_GetById(14)->x.fixed >> 20;
  row = Object_GetById(14)->z.fixed >> 20;
  companion18Row = Object_GetById(18)->z.fixed >> 20;
  companion9Row = Object_GetById(9)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(companion9Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(14, -16, 0);
    } else if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(14, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(14, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(14, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(14, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(companion18Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(14, -48, 0);
  } else if (column == 8) {
    if ((u32)(companion18Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(14, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(14)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east onto pillar 14.
static void Event_SlidePillar14East(void) {
  s32 column;
  s32 row;
  s32 companion18Row;
  s32 companion9Row;
  s32 footprintRow;

  column = Object_GetById(14)->x.fixed >> 20;
  row = Object_GetById(14)->z.fixed >> 20;
  companion18Row = Object_GetById(18)->z.fixed >> 20;
  companion9Row = Object_GetById(9)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(companion9Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(14, 32, 0);
    } else if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(14, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(14, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(14, 80, 0);
  } else if (column == 9) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(14, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndPillar(14, 16, 0);
  } else if (column == 13) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(14)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking west onto pillar 16.
static void Event_SlidePillar16West(void) {
  s32 column;
  s32 row;
  s32 companion18Row;
  s32 companion9Row;
  s32 footprintRow;

  column = Object_GetById(16)->x.fixed >> 20;
  row = Object_GetById(16)->z.fixed >> 20;
  companion18Row = Object_GetById(18)->z.fixed >> 20;
  companion9Row = Object_GetById(9)->z.fixed >> 20;

  if (column == 13) {
    if ((u32)(companion9Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(16, -16, 0);
    } else if ((u32)(companion18Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(16, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(16, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(companion9Row - 9) <= 2) {
      return;
    }
    if ((u32)(companion18Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(16, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(16, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(companion18Row - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(16, -48, 0);
  } else if (column == 8) {
    CrossboneIsle_MoveSelectedActorAndPillar(16, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(16)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east onto pillar 16.
static void Event_SlidePillar16East(void) {
  s32 column;
  s32 row;
  s32 companionRow;

  column = Object_GetById(16)->x.fixed >> 20;
  row = Object_GetById(16)->z.fixed >> 20;
  companionRow = Object_GetById(9)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(companionRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndPillar(16, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndPillar(16, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companionRow - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndPillar(16, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndPillar(16, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndPillar(16, 16, 0);
  } else if (column == 13) {
    return;
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(16)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

#include "common/object_orbit.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gEffectScript0[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u16 gCrossboneIsle_EntranceCells[] = {
    MAP_CELL_STEP(58, 23, 1, 2, 5), MAP_CELL_STEP(59, 23, 1, 2, 5), MAP_CELL_STEPS_END,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
    (const s32 *)0xffff0000,
    (const s32 *)120,
    (const s32 *)0x40000098,
    (const s32 *)0xffff0000,
    (const s32 *)-1,
    (const s32 *)0xffff,
    (const s32 *)0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct SceneEntrance gCrossboneIsleEntrances1[] = {
    { CROSSBONE_ISLE_FROM_WORLD_MAP, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
    { CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_2, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances2[] = {
    { CROSSBONE_ISLE_2_FROM_CROSSBONE_ISLE, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
    { CROSSBONE_ISLE_2_FROM_CROSSBONE_ISLE_6, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances3[] = {
    { CROSSBONE_ISLE_3_FROM_CROSSBONE_ISLE_14_EAST, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { CROSSBONE_ISLE_3_FROM_CROSSBONE_ISLE_14_WEST, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { 99, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances4[] = {
    { CROSSBONE_ISLE_4_FROM_CROSSBONE_ISLE_14, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { CROSSBONE_ISLE_4_FROM_PIRATE_SHIP, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances5[] = {
    { PIRATE_SHIP_FROM_CROSSBONE_ISLE_4, CONDITION_ALWAYS, 312, 0, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 216, 0, 544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    // The nine corridor rooms (CrossboneIsle6 to 14) enter through this
    // shared tail.

    { PIRATE_SHIP_FROM_CROSSBONE_ISLE_4, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
    { 2, CONDITION_ALWAYS, 488, 0, 176, FACING_EAST, 0, 432, 32, 880, 352, 0 },
    { 3, CONDITION_ALWAYS, 712, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { 4, CONDITION_ALWAYS, 600, 0, 152, FACING_SOUTH, 0, 432, 32, 880, 352, 0 },
    { 5, CONDITION_ALWAYS, 712, 0, 152, FACING_NORTH, 0, 432, 32, 880, 352, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIsleExits[] = {
    SCENE_EXITS(CrossboneIsle),
    SCENE_EXIT(CROSSBONE_ISLE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_CROSSBONE_ISLE),
    SCENE_EXIT(CROSSBONE_ISLE_TO_CROSSBONE_ISLE_2, CrossboneIsle2, CROSSBONE_ISLE_2_FROM_CROSSBONE_ISLE),
    SCENE_EXITS(CrossboneIsle2),
    SCENE_EXIT(CROSSBONE_ISLE_2_TO_CROSSBONE_ISLE, CrossboneIsle, CROSSBONE_ISLE_FROM_CROSSBONE_ISLE_2),
    SCENE_EXIT(CROSSBONE_ISLE_2_TO_CROSSBONE_ISLE_6, CrossboneIsle6, CROSSBONE_ISLE_6_FROM_CROSSBONE_ISLE_2),
    SCENE_EXITS(CrossboneIsle3),
    SCENE_EXIT(CROSSBONE_ISLE_3_TO_CROSSBONE_ISLE_14_EAST, CrossboneIsle14, CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_3_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_3_TO_CROSSBONE_ISLE_14_WEST, CrossboneIsle14, CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_3_WEST),
    SCENE_EXITS(CrossboneIsle4),
    SCENE_EXIT(CROSSBONE_ISLE_4_TO_CROSSBONE_ISLE_14, CrossboneIsle14, CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_4),
    SCENE_EXIT(CROSSBONE_ISLE_4_TO_PIRATE_SHIP, PirateShip, PIRATE_SHIP_FROM_CROSSBONE_ISLE_4),
    SCENE_EXITS(PirateShip),
    SCENE_EXIT(PIRATE_SHIP_TO_CROSSBONE_ISLE_4, CrossboneIsle4, CROSSBONE_ISLE_4_FROM_PIRATE_SHIP),
    SCENE_EXITS(CrossboneIsle6),
    SCENE_EXIT(CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_2, CrossboneIsle2, CROSSBONE_ISLE_2_FROM_CROSSBONE_ISLE_6),
    SCENE_EXIT(CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_7, CrossboneIsle7, CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_6),
    SCENE_EXIT(CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_PILLAR_ROOMS_1_EAST, CrossboneIslePillarRooms1, CROSSBONE_ISLE_PILLAR_ROOMS_1_FROM_CROSSBONE_ISLE_6_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_PILLAR_ROOMS_1_WEST, CrossboneIslePillarRooms1, CROSSBONE_ISLE_PILLAR_ROOMS_1_FROM_CROSSBONE_ISLE_6_WEST),
    SCENE_EXITS(CrossboneIsle7),
    SCENE_EXIT(CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_6, CrossboneIsle6, CROSSBONE_ISLE_6_FROM_CROSSBONE_ISLE_7),
    SCENE_EXIT(CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_8, CrossboneIsle8, CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_7),
    SCENE_EXIT(CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_PILLAR_ROOMS_2_EAST, CrossboneIslePillarRooms2, CROSSBONE_ISLE_PILLAR_ROOMS_2_FROM_CROSSBONE_ISLE_7_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_PILLAR_ROOMS_2_WEST, CrossboneIslePillarRooms2, CROSSBONE_ISLE_PILLAR_ROOMS_2_FROM_CROSSBONE_ISLE_7_WEST),
    SCENE_EXITS(CrossboneIsle8),
    SCENE_EXIT(CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_7, CrossboneIsle7, CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_8),
    SCENE_EXIT(CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_9, CrossboneIsle9, CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_8),
    SCENE_EXIT(CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_KEYS_EAST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_8_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_KEYS_WEST, CrossboneIsleKeys, CROSSBONE_ISLE_KEYS_FROM_CROSSBONE_ISLE_8_WEST),
    SCENE_EXITS(CrossboneIsle9),
    SCENE_EXIT(CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_8, CrossboneIsle8, CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_9),
    SCENE_EXIT(CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_10, CrossboneIsle10, CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_9),
    SCENE_EXIT(CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_PLATFORM_ROOMS_2_EAST, CrossboneIslePlatformRooms2, CROSSBONE_ISLE_PLATFORM_ROOMS_2_FROM_CROSSBONE_ISLE_9_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_PLATFORM_ROOMS_2_WEST, CrossboneIslePlatformRooms2, CROSSBONE_ISLE_PLATFORM_ROOMS_2_FROM_CROSSBONE_ISLE_9_WEST),
    SCENE_EXITS(CrossboneIsle10),
    SCENE_EXIT(CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_9, CrossboneIsle9, CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_10),
    SCENE_EXIT(CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_11, CrossboneIsle11, CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_10),
    SCENE_EXIT(CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_FLOODGATE_EAST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_10_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_FLOODGATE_WEST, CrossboneIsleFloodgate, CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_10_WEST),
    SCENE_EXITS(CrossboneIsle11),
    SCENE_EXIT(CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_10, CrossboneIsle10, CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_11),
    SCENE_EXIT(CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_12, CrossboneIsle12, CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_11),
    SCENE_EXIT(CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_BANE_EAST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_11_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_BANE_WEST, CrossboneIsleBane, CROSSBONE_ISLE_BANE_FROM_CROSSBONE_ISLE_11_WEST),
    SCENE_EXITS(CrossboneIsle12),
    SCENE_EXIT(CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_11, CrossboneIsle11, CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_12),
    SCENE_EXIT(CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_13, CrossboneIsle13, CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_12),
    SCENE_EXIT(CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_PILLAR_ROOMS_4_EAST, CrossboneIslePillarRooms4, CROSSBONE_ISLE_PILLAR_ROOMS_4_FROM_CROSSBONE_ISLE_12_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_PILLAR_ROOMS_4_WEST, CrossboneIslePillarRooms4, CROSSBONE_ISLE_PILLAR_ROOMS_4_FROM_CROSSBONE_ISLE_12_WEST),
    SCENE_EXITS(CrossboneIsle13),
    SCENE_EXIT(CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_12, CrossboneIsle12, CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_13),
    SCENE_EXIT(CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_14, CrossboneIsle14, CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_13),
    SCENE_EXIT(CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_PILLAR_ROOMS_5_EAST, CrossboneIslePillarRooms5, CROSSBONE_ISLE_PILLAR_ROOMS_5_FROM_CROSSBONE_ISLE_13_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_PILLAR_ROOMS_5_WEST, CrossboneIslePillarRooms5, CROSSBONE_ISLE_PILLAR_ROOMS_5_FROM_CROSSBONE_ISLE_13_WEST),
    SCENE_EXITS(CrossboneIsle14),
    SCENE_EXIT(CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_13, CrossboneIsle13, CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_14),
    SCENE_EXIT(CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_4, CrossboneIsle4, CROSSBONE_ISLE_4_FROM_CROSSBONE_ISLE_14),
    SCENE_EXIT(CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_3_EAST, CrossboneIsle3, CROSSBONE_ISLE_3_FROM_CROSSBONE_ISLE_14_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_3_WEST, CrossboneIsle3, CROSSBONE_ISLE_3_FROM_CROSSBONE_ISLE_14_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gCrossboneIslePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements1[] = {
    { 0x16, 0xfd7, 7,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(352), 0, PIXELS(376), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(336), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(320), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(352), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements3[] = {
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(408), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(520), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(584), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(584), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(72), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(120), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePlacements6To14[] = {
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gCrossboneIsleEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenEastDoor) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenWestDoor) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MovePillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xef4, 0x500004 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents1[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, 0xfd7, EVENT_SCRIPT(Event_CatchNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 11, FLAG_SCENE_TEMP_40, EVENT_SCRIPT(Event_GrowPlant11) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 12, FLAG_SCENE_TEMP_41, EVENT_SCRIPT(Event_GrowPlant12) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 13, FLAG_SCENE_TEMP_42, EVENT_SCRIPT(Event_GrowPlant13) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 14, FLAG_SCENE_TEMP_43, EVENT_SCRIPT(Event_GrowPlant14) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY, EVENT_SCRIPT(Event_BlowAwayIvy15) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 16, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 1, EVENT_SCRIPT(Event_BlowAwayIvy16) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 2, EVENT_SCRIPT(Event_BlowAwayIvy17) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 18, FLAG_CROSSBONE_ISLE_FIRST_IVY_BLOWN_AWAY + 3, EVENT_SCRIPT(Event_BlowAwayIvy18) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushIslePillar) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushIslePillar) },
    { 0x8413, 100, 0xec0, FIND_COINS(1) },
    { 0xc413, 100, 0xec0, FIND_COINS(1) },
    { 0xa413, 100, 0xec0, FIND_COINS(1) },
    { 0x8413, 101, 0xec1, FIND_ITEM(ITEM_MINT) },
    { 0xc413, 101, 0xec1, FIND_ITEM(ITEM_MINT) },
    { 0xa413, 101, 0xec1, FIND_ITEM(ITEM_MINT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_WEST), 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 42, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 43, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar8North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar8South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar10North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar10South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar11West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar11East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar12West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar12East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar13West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar13East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar15West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar15East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar17North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar17South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar18North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar18South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar9North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar9South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar19West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar19East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar14West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar14East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar16West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlidePillar16East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar8) },
    { EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar10) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar11) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar12) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar13) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar15) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar17) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar18) },
    { EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar9) },
    { EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar19) },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar14) },
    { EVENT_TOUCH, 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnPillar16) },
    { SEARCH(SEARCH_CHEST), 100, 0xee2, FIND_COINS(999) },
    { SEARCH(SEARCH_CHEST), 101, 0xee3, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SEARCH(SEARCH_CHEST), 102, 0xee4, FIND_ITEM(ITEM_MURAMASA) },
    { SEARCH(SEARCH_CHEST), 103, 0xee5, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossboneIsleEvents4[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SCENE_EVENTS_END},
};

struct SceneEvent gCrossboneIsleEvents5[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_TOUCH, 10, FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED,
     EVENT_SCRIPT(Event_FightDeadbeard)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_HopSouth)},
    {SEARCH(SEARCH_CHEST), 100, 0xee6, FIND_ITEM(ITEM_DEMON_MAIL)},
    {SEARCH(SEARCH_CHEST), 101, 0xef7, 0x500007},
    {SCENE_EVENTS_END},
};

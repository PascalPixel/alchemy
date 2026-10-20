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
///   (FLAG_SCENE_TEMP_40 to _43), two blocks can be pushed, and a Nut circles
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
///   Between the doors stands a block that, pushed or moved with Move to
///   column 40, sinks into the floor and opens the corridor (one flag per room
///   from FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED). In the first room a chest
///   is a mimic (0xef4). The side rooms of the first eight rooms are in
///   crossbone_isle_pillar_rooms.c and crossbone_isle_platform_rooms.c.
/// - Crossbone Isle Cave (CrossboneIsle3, scene 123), the ninth room's side
///   room: twelve blocks that slide with the leader standing on them until
///   another block or the wall stops them, and hops from block to block, lead
///   to its four chests.
/// - Crossbone Isle Cave (CrossboneIsle4, scene 124), the passage from the
///   ninth room to the Pirate Ship.
/// - Pirate Ship (PirateShip, scene 125), at the end. Stepping on trigger
///   10 (Event_FightDeadbeard) sets off the battle with Deadbeard, encounter
///   table 84's sixth group; when the party comes back (entrance 5)
///   FLAG_CROSSBONE_ISLE_DEADBEARD_DEFEATED is set and a chest with the Demon
///   Mail appears. A second chest is a mimic (0xef7).
///
/// Scene_Initialize restores each scene as its flags say: the ivy and plants on
/// the island, the sliding blocks' footprints, the corridor's sunk blocks and
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

static void CrossboneIsle_RestoreSunkBlock(void);
static void CrossboneIsle_OpenEastDoorAfterBattle(void);
static void CrossboneIsle_RestoreIvy(void);
static void CrossboneIsle_DrawBlockPuzzleFootprints(void);

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

/// Emits a ring of dust around a corridor room's block as it sinks.
static void CrossboneIsle_SpawnBlockMovedBurst(s32 objectId) {
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

/// Restores each scene's puzzles from its flags: the sliding blocks' footprints
/// in CrossboneIsle3; on the Pirate Ship the mimic's chest and, after
/// Deadbeard, the Demon Mail's; on Crossbone Isle the ivy, plants and Nut; in
/// the corridor rooms the mimic's chest, the sunk blocks and the door opened
/// after a battle.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;
  s32 scene;
  s32 current;
  s32 first;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  // CrossboneIsle3's sliding blocks restore their map footprints alone.
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_CrossboneIsle3) {
    CrossboneIsle_DrawBlockPuzzleFootprints();
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
  // sunk block and return entrance.
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
      CrossboneIsle_RestoreSunkBlock();
      if (gGameState.entrance == 5) {
        CrossboneIsle_OpenEastDoorAfterBattle();
      }
    }
  }
  return 0;
}

/// In a corridor room of Crossbone Isle Cave whose block has been sunk, it
/// stays sunk in its place.
static void CrossboneIsle_RestoreSunkBlock(void) {
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

/// Move on a corridor room's block in Crossbone Isle Cave, and after each push:
/// once it reaches column 40 it sinks into the floor in a ring of dust, and the
/// corridor is open (a flag per room from
/// FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED).
static void Event_MoveBlock(void) {
  struct FieldActor *block;
  s16 *sceneNumber;
  struct GameState *state;
  s32 column;

  block = Object_GetById(8);
  column = block->x.fixed >> 20;
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

  // Play the moved-block reaction before hiding it and opening the passage.
  block->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;

  WAIT(8);
  CrossboneIsle_SpawnBlockMovedBurst(8);
  Engine_AudioPlayCue(SE_136);
  WAIT(40);

  Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
  Engine_ActorSetSpritePriority(8, 3);

  block->motionFlags = 0;
  block->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

  // Replace the blocked cell and remember that the room's block is sunk.
  Map_CopyCellAttributes(42, 10, 1, 1, column, 10);

#if !TBS_PATCH_1
  state = &gGameState;
  sceneNumber = &state->scene;
#endif
  Engine_GameFlagSet(*sceneNumber + (FLAG_CROSSBONE_ISLE_FIRST_PASSAGE_OPENED -
                                     (s32)&SceneId_CrossboneIsle6));
}

/// Walking into a corridor room's block in Crossbone Isle Cave (trigger 20):
/// push it a step.
static void Event_PushBlock(void) {
  EV_BEGIN();
  StagedActor_AdvancePair();
  EV_END();
  Event_MoveBlock();
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

/// Walking into one of Crossbone Isle's two blocks: push it if the cell beyond
/// is clear.
static void Event_PushIsleBlock(void) {
  struct StagedActorProbe probe;

  EV_BEGIN();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
  }
  EV_END();
}

/// Moves the selected actor and puzzle block by the same pixel offset.
static void CrossboneIsle_MoveSelectedActorAndBlock(s32 actorId, s32 xOffset,
                                                    s32 zOffset) {
  struct FieldActor *selectedActor;
  struct FieldActor *block;
  s32 fixedX;
  s32 fixedZ;

  selectedActor = Object_GetById(gGameState.selectedActor);
  block = Object_GetById(actorId);
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
  // Position the block on the same cell-center grid as the selected actor.
  Object_SetMode(selectedActor, 27);
  {
    fixedX =
        ((block->x.fixed + (xOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);
    fixedZ =
        ((block->z.fixed + (zOffset * PIXELS(1))) & 0xFFF00000) + PIXELS(8);

    block->speed = PIXELS(1);
    block->acceleration = PIXELS(1) / 2;
    Engine_ObjectSetPosition(block, fixedX, block->y.fixed, fixedZ);
  }
  // The block's mode depends on which side of the grid it moves toward.
  if (xOffset < 0 || zOffset < 0) {
    Object_SetMode(block, 4);
  } else {
    Object_SetMode(block, 3);
  }
  Engine_AudioPlayCue(SE_226);
  FieldObject_WaitForMove(selectedActor);
  Engine_AudioPlayCue(SE_288);
  EV_END();
}

/// Draws the three-cell horizontal footprint centered on this block.
static void CrossboneIsle_DrawHorizontalBlockFootprint(s32 actorId,
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

/// Draws the three-cell vertical footprint centered on this block.
static void CrossboneIsle_DrawVerticalBlockFootprint(s32 actorId,
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

/// Runs the two animation changes used when touching a puzzle block.
static void CrossboneIsle_AnimateTouchedBlock(int actorId) {
  OBJ_MODE(actorId, 1);
  OBJ_MODE(actorId, 2);
}

/// Draws all twelve block footprints in Crossbone Isle Cave (CrossboneIsle3).
static void CrossboneIsle_DrawBlockPuzzleFootprints(void) {
  CrossboneIsle_DrawHorizontalBlockFootprint(8, 17, 30, 21);
  CrossboneIsle_DrawHorizontalBlockFootprint(10, 17, 31, 22);
  CrossboneIsle_DrawVerticalBlockFootprint(11, 20, 30, 23);
  CrossboneIsle_DrawVerticalBlockFootprint(12, 21, 30, 24);
  CrossboneIsle_DrawVerticalBlockFootprint(13, 22, 30, 25);
  CrossboneIsle_DrawVerticalBlockFootprint(15, 23, 30, 26);
  CrossboneIsle_DrawHorizontalBlockFootprint(17, 0, 30, 31);
  CrossboneIsle_DrawHorizontalBlockFootprint(18, 0, 31, 32);
  CrossboneIsle_DrawHorizontalBlockFootprint(9, 0, 32, 33);
  CrossboneIsle_DrawVerticalBlockFootprint(19, 4, 30, 34);
  CrossboneIsle_DrawVerticalBlockFootprint(14, 5, 30, 35);
  CrossboneIsle_DrawVerticalBlockFootprint(16, 6, 30, 36);
}

// Crossbone Isle Cave (CrossboneIsle3), the ninth room's side room: its twelve
// sliding blocks. Stepping onto a block plays its animation from the start;
// walking onto one in a direction it moves slides it that way with the leader
// on it until another block or the wall stops it; walking against a block's
// edge hops the leader 32 pixels if the landing is clear.

/// Stepping onto block 8.
static void Event_StepOnBlock8(void) { CrossboneIsle_AnimateTouchedBlock(8); }

/// Stepping onto block 10.
static void Event_StepOnBlock10(void) { CrossboneIsle_AnimateTouchedBlock(10); }

/// Stepping onto block 11.
static void Event_StepOnBlock11(void) { CrossboneIsle_AnimateTouchedBlock(11); }

/// Stepping onto block 12.
static void Event_StepOnBlock12(void) { CrossboneIsle_AnimateTouchedBlock(12); }

/// Stepping onto block 13.
static void Event_StepOnBlock13(void) { CrossboneIsle_AnimateTouchedBlock(13); }

/// Stepping onto block 15.
static void Event_StepOnBlock15(void) { CrossboneIsle_AnimateTouchedBlock(15); }

/// Stepping onto block 17.
static void Event_StepOnBlock17(void) { CrossboneIsle_AnimateTouchedBlock(17); }

/// Stepping onto block 18.
static void Event_StepOnBlock18(void) { CrossboneIsle_AnimateTouchedBlock(18); }

/// Stepping onto block 9.
static void Event_StepOnBlock9(void) { CrossboneIsle_AnimateTouchedBlock(9); }

/// Stepping onto block 19.
static void Event_StepOnBlock19(void) { CrossboneIsle_AnimateTouchedBlock(19); }

/// Stepping onto block 14.
static void Event_StepOnBlock14(void) { CrossboneIsle_AnimateTouchedBlock(14); }

/// Stepping onto block 16.
static void Event_StepOnBlock16(void) { CrossboneIsle_AnimateTouchedBlock(16); }

/// Tests a step, then moves the leader with the block-pushing animation.
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

/// Walking north against a block's edge.
static s32 Event_HopNorth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + -PIXELS(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking south against a block's edge, or at the Pirate Ship's trigger 20.
static s32 Event_HopSouth(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed;
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed + PIXELS(32);
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking west against a block's edge.
static s32 Event_HopWest(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + -PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking east against a block's edge.
static s32 Event_HopEast(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition destination;

  destination.x = actor->x.fixed + PIXELS(32);
  destination.y = actor->y.fixed;
  destination.z = actor->z.fixed;
  return CrossboneIsle_TryHop(actor, &destination);
}

/// Walking north onto block 8.
static void Event_SlideBlock8North(void) {
  s32 actorX = Object_GetById(8)->x.fixed >> 20;
  s32 actorZ = Object_GetById(8)->z.fixed >> 20;
  s32 actor12X = Object_GetById(12)->x.fixed >> 20;
  s32 actor15X = Object_GetById(15)->x.fixed >> 20;

  if (actorZ == 19) {
    if (actor12X == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -80);
    } else if (actor15X == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -112);
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -80);
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -112);
    }
  } else if (actorZ == 14) {
    if (actor12X == 24) {
      return;
    }
    if (actor15X == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -64);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -112);
    }
  } else if (actorZ == 10) {
    if (actor15X == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(8, 0, -48);
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

/// Copies a horizontal block's old footprint to its new row, then clears the
/// old row.
static inline void
CrossboneIsle_RedrawHorizontalBlockRoute(s32 actorId, s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationRow = Object_GetById(actorId)->z.fixed >> 20;
    s32 footprintColumn = column - 1;

    Map_CopyCellAttributes(footprintColumn, row, 3, 1, footprintColumn,
                           destinationRow);
  }
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Copies a vertical block's old footprint to its new column, then clears the
/// old one.
static inline void CrossboneIsle_RedrawVerticalBlockRoute(s32 actorId,
                                                          s32 column, s32 row) {
  Engine_TaskWait(2);
  {
    s32 destinationColumn = Object_GetById(actorId)->x.fixed >> 20;
    s32 footprintRow = row - 1;

    Map_CopyCellAttributes(column, footprintRow, 1, 3, destinationColumn,
                           footprintRow);
  }
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking south onto block 8.
static void Event_SlideBlock8South(void) {
  s32 column = Object_GetById(8)->x.fixed >> 20;
  s32 row = Object_GetById(8)->z.fixed >> 20;
  s32 block12Column = Object_GetById(12)->x.fixed >> 20;

  if (row == 7) {
    if (block12Column == 24) {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 80);
      CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 112);
    }
  } else if (row == 10) {
    if (block12Column == 24) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 144);
  } else if (row == 14) {
    CrossboneIsle_MoveSelectedActorAndBlock(8, 0, 80);
  } else {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(8, column, row);
}

/// Walking north onto block 10.
static void Event_SlideBlock10North(void) {
  s32 column = Object_GetById(10)->x.fixed >> 20;
  s32 row = Object_GetById(10)->z.fixed >> 20;
  s32 block13Column = Object_GetById(13)->x.fixed >> 20;
  s32 block15Column = Object_GetById(15)->x.fixed >> 20;

  if (row == 18) {
    if (block15Column >= 31 && block15Column <= 33) {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -128);
    } else if (block13Column >= 31 && block13Column <= 33) {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -128);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -112);
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -64);
    }
  } else if (row == 10) {
    if (block15Column >= 31 && block15Column <= 33) {
      return;
    }
    if (block13Column >= 31 && block13Column <= 33) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(10, 0, -48);
  } else if (row == 7) {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(10, column, row);
}

/// Walking south onto block 10.
static void Event_SlideBlock10South(void) {
  s32 column = Object_GetById(10)->x.fixed >> 20;
  s32 row = Object_GetById(10)->z.fixed >> 20;

  if (row != 18) {
    if (row == 10) {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, 128);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, 112);
      CrossboneIsle_MoveSelectedActorAndBlock(10, 0, 64);
    }
    CrossboneIsle_RedrawHorizontalBlockRoute(10, column, row);
  }
}

/// Walking west onto block 11.
static void Event_SlideBlock11West(void) {
  s32 column = Object_GetById(11)->x.fixed >> 20;
  s32 row = Object_GetById(11)->z.fixed >> 20;

  if (column != 30) {
    if (column == 34) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndBlock(11, -64, 0);
    } else if (column == 36) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        CrossboneIsle_MoveSelectedActorAndBlock(11, -32, 0);
      } else {
        CrossboneIsle_MoveSelectedActorAndBlock(11, -96, 0);
      }
    }
    CrossboneIsle_RedrawVerticalBlockRoute(11, column, row);
  }
}

/// Walking east onto block 11.
static void Event_SlideBlock11East(void) {
  s32 column = Object_GetById(11)->x.fixed >> 20;
  s32 row = Object_GetById(11)->z.fixed >> 20;

  if (column != 36) {
    if (column == 30) {
      if ((Object_GetById(10)->z.fixed >> 20) == 18) {
        return;
      }
      CrossboneIsle_MoveSelectedActorAndBlock(11, 96, 0);
    } else if (column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(11, 32, 0);
    }
    CrossboneIsle_RedrawVerticalBlockRoute(11, column, row);
  }
}

/// Walking west onto block 12.
static void Event_SlideBlock12West(void) {
  s32 column = Object_GetById(12)->x.fixed >> 20;
  s32 row = Object_GetById(12)->z.fixed >> 20;

  if (column == 36) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, -96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(12, -96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, -96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(12, -64, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(12, column, row);
}

/// Walking east onto block 12.
static void Event_SlideBlock12East(void) {
  s32 column = Object_GetById(12)->x.fixed >> 20;
  s32 row = Object_GetById(12)->z.fixed >> 20;

  if (column == 24) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, 96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(12, 96, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(12, 32, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(12, column, row);
}

/// Walking west onto block 13.
static void Event_SlideBlock13West(void) {
  s32 column = Object_GetById(13)->x.fixed >> 20;
  s32 row = Object_GetById(13)->z.fixed >> 20;
  s32 block10Row = Object_GetById(10)->z.fixed >> 20;
  s32 block15Column = Object_GetById(15)->x.fixed >> 20;

  if (column == 36) {
    if (block15Column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -16, 0);
    } else if (block10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -32, 0);
    } else if (block15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -96, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
    }
  } else if (column == 35) {
    if (block15Column == 34) {
      return;
    }
    if (block10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -16, 0);
    } else if (block15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(13, -80, 0);
    }
  } else if (column == 34) {
    if (block10Row == 7) {
      return;
    }
    if (block15Column == 30) {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(13, -144, 0);
    }
  } else if (column == 31) {
    if (block15Column == 30) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(13, -96, 0);
  } else if (column == 25) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(13, column, row);
}

/// Walking east onto block 13.
static void Event_SlideBlock13East(void) {
  s32 column = Object_GetById(13)->x.fixed >> 20;
  s32 row = Object_GetById(13)->z.fixed >> 20;

  Object_GetById(15);
  if (column == 25) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(13, 80, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 80, 0);
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 32, 0);
  } else if (column == 35) {
    CrossboneIsle_MoveSelectedActorAndBlock(13, 16, 0);
  } else if (column == 36) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(13, column, row);
}

/// Walking west onto block 15.
static void Event_SlideBlock15West(void) {
  s32 column = Object_GetById(15)->x.fixed >> 20;
  s32 row = Object_GetById(15)->z.fixed >> 20;
  s32 block8Row = Object_GetById(8)->z.fixed >> 20;
  s32 block10Row = Object_GetById(10)->z.fixed >> 20;

  if (column == 35) {
    if (block10Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, -16, 0);
    } else if (block8Row == 7) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, -96, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, -80, 0);
    }
  } else if (column == 34) {
    if (block10Row == 7) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(15, -96, 0);
    CrossboneIsle_MoveSelectedActorAndBlock(15, -64, 0);
  } else if (column == 33) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, -144, 0);
  } else if (column == 31) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, -80, 0);
  } else if (column == 30) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, -96, 0);
  } else if (column == 24) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(15, column, row);
}

/// Walking east onto block 15.
static void Event_SlideBlock15East(void) {
  s32 column = Object_GetById(15)->x.fixed >> 20;
  s32 row = Object_GetById(15)->z.fixed >> 20;
  s32 block10Row = Object_GetById(10)->z.fixed >> 20;
  s32 block13Column = Object_GetById(13)->x.fixed >> 20;

  if (column == 24) {
    if (block10Row == 7 || block13Column == 31) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 96, 0);
    } else if (block13Column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 64, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
    } else if (block13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(15, 96, 0);
    }
  } else if (column == 30 || block13Column == 31) {
    if (block10Row == 7) {
      return;
    }
    if (block13Column == 34) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 48, 0);
    } else if (block13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 80, 0);
    }
  } else if (column == 33) {
    if (block13Column == 34) {
      return;
    }
    if (block13Column == 35) {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 16, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(15, 32, 0);
    }
  } else if (column == 34) {
    CrossboneIsle_MoveSelectedActorAndBlock(15, 16, 0);
  } else if (column == 35) {
    return;
  }
  CrossboneIsle_RedrawVerticalBlockRoute(15, column, row);
}

/// Walking north onto block 17.
static void Event_SlideBlock17North(void) {
  s32 column = Object_GetById(17)->x.fixed >> 20;
  s32 row = Object_GetById(17)->z.fixed >> 20;
  s32 block19Column = Object_GetById(19)->x.fixed >> 20;

  if (row == 19) {
    if (block19Column >= 3 && block19Column <= 5) {
      CrossboneIsle_MoveSelectedActorAndBlock(17, 0, -16);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(17, 0, -64);
    }
  } else if (row == 18) {
    if (block19Column >= 3 && block19Column <= 5) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(17, 0, -48);
  } else if (row == 15) {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(17, column, row);
}

/// Walking south onto block 17.
static void Event_SlideBlock17South(void) {
  s32 column = Object_GetById(17)->x.fixed >> 20;
  s32 row = Object_GetById(17)->z.fixed >> 20;

  if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndBlock(17, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndBlock(17, 0, 16);
  } else if (row == 19) {
    return;
  }
  CrossboneIsle_RedrawHorizontalBlockRoute(17, column, row);
}

/// These three columns form the crossing lane for the vertical blocks.
#define IS_IN_BLOCK_CROSSING_COLUMNS(x) ((x) >= 6 && (x) <= 8)

/// Walking north onto block 18.
static void Event_SlideBlock18North(void) {
  s32 column;
  s32 row;
  s32 block19Column;
  s32 block14Column;
  s32 block16Column;

  column = Object_GetById(18)->x.fixed >> 20;
  row = Object_GetById(18)->z.fixed >> 20;
  block19Column = Object_GetById(19)->x.fixed >> 20;
  block14Column = Object_GetById(14)->x.fixed >> 20;
  block16Column = Object_GetById(16)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -16);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -64);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -64);
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -48);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -144);
    }
  } else if (row == 15) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -96);
    }
  } else if (row == 14) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -80);
    }
  } else if (row == 12) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -48);
  } else if (row == 11) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, -32);
  } else if (row == 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(18)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_BLOCK_CROSSING_COLUMNS

/// Walking south onto block 18.
static void Event_SlideBlock18South(void) {
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
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 32);
    } else if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 80);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 64);
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(companion14Column - 6) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(companion19Column - 6) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(companion19Column - 6) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndBlock(18, 0, 16);
  } else if (row == 19) {
    return;
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(18)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_BLOCK_CROSSING_COLUMNS
#define IS_IN_BLOCK_CROSSING_COLUMNS(x) ((x) >= 9 && (x) <= 11)

/// Walking north onto block 9.
static void Event_SlideBlock9North(void) {
  s32 column;
  s32 row;
  s32 block19Column;
  s32 block14Column;
  s32 block16Column;

  column = Object_GetById(9)->x.fixed >> 20;
  row = Object_GetById(9)->z.fixed >> 20;
  block19Column = Object_GetById(19)->x.fixed >> 20;
  block14Column = Object_GetById(14)->x.fixed >> 20;
  block16Column = Object_GetById(16)->x.fixed >> 20;
  if (row == 19) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -16);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -64);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -112);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -80);
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
    }
  } else if (row == 18) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block19Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -48);
    } else if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -64);
    }
  } else if (row == 15) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -112);
    }
  } else if (row == 14) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block14Column)) {
      return;
    }
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -96);
    }
  } else if (row == 12) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -64);
  } else if (row == 11) {
    if (IS_IN_BLOCK_CROSSING_COLUMNS(block16Column)) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, -48);
  } else if ((u32)row <= 9) {
    return;
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(9)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

#undef IS_IN_BLOCK_CROSSING_COLUMNS

/// Walking south onto block 9.
static void Event_SlideBlock9South(void) {
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
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 48);
    } else {
      if ((u32)(companion19Column - 9) > 2) {
        CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 80);
      }
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 96);
    }
  } else if (row == 11) {
    if ((u32)(companion14Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 48);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 128);
    }
  } else if (row == 12) {
    if ((u32)(companion14Column - 9) <= 2) {
      return;
    }
    if ((u32)(companion19Column - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 32);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 112);
    }
  } else if (row == 14) {
    if ((u32)(companion19Column - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 80);
  } else if (row == 15) {
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 64);
  } else if (row == 18) {
    CrossboneIsle_MoveSelectedActorAndBlock(9, 0, 16);
  }

  Engine_TaskWait(2);

  Engine_MapCopyCellAttributes(column - 1, row, 3, 1, column - 1,
                               Object_GetById(9)->z.fixed >> 20);
  Map_CopyCellAttributes(0, 0, 3, 1, column - 1, row);
}

/// Walking west onto block 19.
static void Event_SlideBlock19West(void) {
  s32 column;
  s32 row;
  s32 block17Row;
  s32 block18Row;
  s32 block9Row;

  column = Object_GetById(19)->x.fixed >> 20;
  row = Object_GetById(19)->z.fixed >> 20;
  block17Row = Object_GetById(17)->z.fixed >> 20;
  block18Row = Object_GetById(18)->z.fixed >> 20;
  block9Row = Object_GetById(9)->z.fixed >> 20;
  if (column == 3) {
    return;
  }
  if (column == 13) {
    if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -16, 0);
    } else if (block18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -64, 0);
    } else if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -112, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -112, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
    }
  } else if (column == 6) {
    if (block17Row == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
  } else if (column == 5) {
    CrossboneIsle_MoveSelectedActorAndBlock(19, -32, 0);
  } else if (column == 8) {
    if (block18Row == 15) {
      return;
    }
    if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -80, 0);
    }
  } else if (column == 9) {
    if (block18Row == 15) {
      return;
    }
    if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -96, 0);
    }
  } else if (column == 12) {
    if (block9Row == 15) {
      return;
    }
    if (block18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -48, 0);
    } else if (block17Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -96, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, -144, 0);
    }
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(19)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking east onto block 19.
static void Event_SlideBlock19East(void) {
  s32 column;
  s32 row;
  s32 block18Row;
  s32 block9Row;

  column = Object_GetById(19)->x.fixed >> 20;
  row = Object_GetById(19)->z.fixed >> 20;
  block18Row = Object_GetById(18)->z.fixed >> 20;
  block9Row = Object_GetById(9)->z.fixed >> 20;
  if (column == 3) {
    if (block18Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 32, 0);
    } else if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 80, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 112, 0);
      CrossboneIsle_MoveSelectedActorAndBlock(19, 48, 0);
    }
  } else if (column == 5) {
    if (block18Row == 15) {
      return;
    }
    if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 128, 0);
    }
  } else if (column == 6) {
    if (block9Row == 15) {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(19, 112, 0);
    }
  } else if (column == 8) {
    if (block9Row == 15) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(19, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndBlock(19, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndBlock(19, 16, 0);
  }
  Engine_TaskWait(2);
  Engine_MapCopyCellAttributes(column, row - 1, 1, 3,
                               Object_GetById(19)->x.fixed >> 20, row - 1);
  Map_CopyCellAttributes(0, 0, 1, 3, column, row - 1);
}

/// Walking west onto block 14.
static void Event_SlideBlock14West(void) {
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
      CrossboneIsle_MoveSelectedActorAndBlock(14, -16, 0);
    } else if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(14, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(companion18Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, -48, 0);
  } else if (column == 8) {
    if ((u32)(companion18Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(14)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east onto block 14.
static void Event_SlideBlock14East(void) {
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
      CrossboneIsle_MoveSelectedActorAndBlock(14, 32, 0);
    } else if ((u32)(companion18Row - 12) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(14, 64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(14, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, 80, 0);
  } else if (column == 9) {
    if ((u32)(companion9Row - 12) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(14, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndBlock(14, 16, 0);
  } else if (column == 13) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(14)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking west onto block 16.
static void Event_SlideBlock16West(void) {
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
      CrossboneIsle_MoveSelectedActorAndBlock(16, -16, 0);
    } else if ((u32)(companion18Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -64, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -112, 0);
    }
  } else if (column == 12) {
    if ((u32)(companion9Row - 9) <= 2) {
      return;
    }
    if ((u32)(companion18Row - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -48, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(16, -96, 0);
    }
  } else if (column == 9) {
    if ((u32)(companion18Row - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(16, -48, 0);
  } else if (column == 8) {
    CrossboneIsle_MoveSelectedActorAndBlock(16, -32, 0);
  } else if (column == 6) {
    return;
  }

  Engine_TaskWait(2);

  footprintRow = row - 1;
  Engine_MapCopyCellAttributes(column, footprintRow, 1, 3,
                               Object_GetById(16)->x.fixed >> 20, footprintRow);
  Map_CopyCellAttributes(0, 0, 1, 3, column, footprintRow);
}

/// Walking east onto block 16.
static void Event_SlideBlock16East(void) {
  s32 column;
  s32 row;
  s32 companionRow;

  column = Object_GetById(16)->x.fixed >> 20;
  row = Object_GetById(16)->z.fixed >> 20;
  companionRow = Object_GetById(9)->z.fixed >> 20;

  if (column == 6) {
    if ((u32)(companionRow - 9) <= 2) {
      CrossboneIsle_MoveSelectedActorAndBlock(16, 32, 0);
    } else {
      CrossboneIsle_MoveSelectedActorAndBlock(16, 112, 0);
    }
  } else if (column == 8) {
    if ((u32)(companionRow - 9) <= 2) {
      return;
    }
    CrossboneIsle_MoveSelectedActorAndBlock(16, 80, 0);
  } else if (column == 9) {
    CrossboneIsle_MoveSelectedActorAndBlock(16, 64, 0);
  } else if (column == 12) {
    CrossboneIsle_MoveSelectedActorAndBlock(16, 16, 0);
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
    { 1, CONDITION_ALWAYS, 280, 0, 544, FACING_NORTH, 0, 0, 0, 512, 560, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 360, FACING_SOUTH, 0, 0, 0, 512, 560, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances2[] = {
    { 1, CONDITION_ALWAYS, 312, 0, 520, FACING_NORTH, 0, 16, 64, 416, 544, 0 },
    { 2, CONDITION_ALWAYS, 280, 0, 144, FACING_EAST, 0, 16, 64, 416, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances3[] = {
    { 1, CONDITION_ALWAYS, 312, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { 2, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 0, 16, 640, 464, 0 },
    { 99, CONDITION_ALWAYS, 272, 0, 232, FACING_SOUTH, 0, 0, 16, 640, 464, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances4[] = {
    { 1, CONDITION_ALWAYS, 184, 0, 136, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 40, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleEntrances5[] = {
    { 1, CONDITION_ALWAYS, 312, 0, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 216, 0, 544, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    // The nine corridor rooms (CrossboneIsle6 to 14) enter through this
    // shared tail.

    { 1, CONDITION_ALWAYS, 840, 0, 176, FACING_WEST, 0, 432, 32, 880, 352, 0 },
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
    SCENE_EXIT(2, CrossboneIsle6, CROSSBONE_ISLE_6_FROM_CROSSBONE_ISLE_2),
    SCENE_EXITS(CrossboneIsle3),
    SCENE_EXIT(1, CrossboneIsle14, 3),
    SCENE_EXIT(2, CrossboneIsle14, 4),
    SCENE_EXITS(CrossboneIsle4),
    SCENE_EXIT(CROSSBONE_ISLE_4_TO_CROSSBONE_ISLE_14, CrossboneIsle14, CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_4),
    SCENE_EXIT(CROSSBONE_ISLE_4_TO_PIRATE_SHIP, PirateShip, PIRATE_SHIP_FROM_CROSSBONE_ISLE_4),
    SCENE_EXITS(PirateShip),
    SCENE_EXIT(PIRATE_SHIP_TO_CROSSBONE_ISLE_4, CrossboneIsle4, CROSSBONE_ISLE_4_FROM_PIRATE_SHIP),
    SCENE_EXITS(CrossboneIsle6),
    SCENE_EXIT(CROSSBONE_ISLE_6_TO_CROSSBONE_ISLE_2, CrossboneIsle2, 2),
    SCENE_EXIT(2, CrossboneIsle7, CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_6),
    SCENE_EXIT(3, CrossboneIslePillarRooms1, 1),
    SCENE_EXIT(4, CrossboneIslePillarRooms1, 2),
    SCENE_EXITS(CrossboneIsle7),
    SCENE_EXIT(CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_6, CrossboneIsle6, 2),
    SCENE_EXIT(CROSSBONE_ISLE_7_TO_CROSSBONE_ISLE_8, CrossboneIsle8, CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_7),
    SCENE_EXIT(3, CrossboneIslePillarRooms2, 1),
    SCENE_EXIT(4, CrossboneIslePillarRooms2, 2),
    SCENE_EXITS(CrossboneIsle8),
    SCENE_EXIT(CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_7, CrossboneIsle7, CROSSBONE_ISLE_7_FROM_CROSSBONE_ISLE_8),
    SCENE_EXIT(CROSSBONE_ISLE_8_TO_CROSSBONE_ISLE_9, CrossboneIsle9, CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_8),
    SCENE_EXIT(3, CrossboneIslePlatformRooms1, 1),
    SCENE_EXIT(4, CrossboneIslePlatformRooms1, 2),
    SCENE_EXITS(CrossboneIsle9),
    SCENE_EXIT(CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_8, CrossboneIsle8, CROSSBONE_ISLE_8_FROM_CROSSBONE_ISLE_9),
    SCENE_EXIT(CROSSBONE_ISLE_9_TO_CROSSBONE_ISLE_10, CrossboneIsle10, CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_9),
    SCENE_EXIT(3, CrossboneIslePlatformRooms2, 1),
    SCENE_EXIT(4, CrossboneIslePlatformRooms2, 2),
    SCENE_EXITS(CrossboneIsle10),
    SCENE_EXIT(CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_9, CrossboneIsle9, CROSSBONE_ISLE_9_FROM_CROSSBONE_ISLE_10),
    SCENE_EXIT(CROSSBONE_ISLE_10_TO_CROSSBONE_ISLE_11, CrossboneIsle11, CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_10),
    SCENE_EXIT(3, CrossboneIslePillarRooms3, 1),
    SCENE_EXIT(4, CrossboneIslePillarRooms3, 2),
    SCENE_EXITS(CrossboneIsle11),
    SCENE_EXIT(CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_10, CrossboneIsle10, CROSSBONE_ISLE_10_FROM_CROSSBONE_ISLE_11),
    SCENE_EXIT(CROSSBONE_ISLE_11_TO_CROSSBONE_ISLE_12, CrossboneIsle12, CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_11),
    SCENE_EXIT(3, CrossboneIslePlatformRooms3, 1),
    SCENE_EXIT(4, CrossboneIslePlatformRooms3, 2),
    SCENE_EXITS(CrossboneIsle12),
    SCENE_EXIT(CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_11, CrossboneIsle11, CROSSBONE_ISLE_11_FROM_CROSSBONE_ISLE_12),
    SCENE_EXIT(CROSSBONE_ISLE_12_TO_CROSSBONE_ISLE_13, CrossboneIsle13, CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_12),
    SCENE_EXIT(3, CrossboneIslePillarRooms4, 1),
    SCENE_EXIT(4, CrossboneIslePillarRooms4, 2),
    SCENE_EXITS(CrossboneIsle13),
    SCENE_EXIT(CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_12, CrossboneIsle12, CROSSBONE_ISLE_12_FROM_CROSSBONE_ISLE_13),
    SCENE_EXIT(CROSSBONE_ISLE_13_TO_CROSSBONE_ISLE_14, CrossboneIsle14, CROSSBONE_ISLE_14_FROM_CROSSBONE_ISLE_13),
    SCENE_EXIT(3, CrossboneIslePillarRooms5, 1),
    SCENE_EXIT(4, CrossboneIslePillarRooms5, 2),
    SCENE_EXITS(CrossboneIsle14),
    SCENE_EXIT(CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_13, CrossboneIsle13, CROSSBONE_ISLE_13_FROM_CROSSBONE_ISLE_14),
    SCENE_EXIT(CROSSBONE_ISLE_14_TO_CROSSBONE_ISLE_4, CrossboneIsle4, CROSSBONE_ISLE_4_FROM_CROSSBONE_ISLE_14),
    SCENE_EXIT(3, CrossboneIsle3, 1),
    SCENE_EXIT(4, CrossboneIsle3, 2),
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
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveBlock) },
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
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushIsleBlock) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushIsleBlock) },
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock8North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock8South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock10North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock10South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock11West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock11East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock12West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock12East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock13West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock13East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock15West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock15East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock17North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock17South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock18North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock18South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock9North) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock9South) },
    { TOUCH_WALKING_INTO(FACING_EAST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopWest) },
    { TOUCH_WALKING_INTO(FACING_WEST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock19West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock19East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock14West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock14East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { TOUCH_WALKING_INTO(FACING_WEST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock16West) },
    { TOUCH_WALKING_INTO(FACING_EAST), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SlideBlock16East) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouth) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock8) },
    { EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock10) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock11) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock12) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock13) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock15) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock17) },
    { EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock18) },
    { EVENT_TOUCH, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock9) },
    { EVENT_TOUCH, 34, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock19) },
    { EVENT_TOUCH, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock14) },
    { EVENT_TOUCH, 36, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StepOnBlock16) },
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

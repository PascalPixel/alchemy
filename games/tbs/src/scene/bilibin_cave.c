/// @file
/// The Bilibin Cave Entrance and Bilibin Cave, two places on the way north
/// from Bilibin toward Imil that share this overlay: the Bilibin Cave Entrance
/// is SceneId_BilibinCaveEntrance, and Bilibin Cave is
/// SceneId_BilibinCaveRapier and, beyond it, SceneId_BilibinCaveIce, whose far
/// end opens onto the world map farther north. No one speaks here: each scene
/// is a run of Psynergy puzzles, the getters pick its tables by
/// gGameState.scene, and BilibinCave_Initialize restores what the party has
/// already solved in the scene being entered.
///
/// At the Bilibin Cave Entrance, entered from the world map, Growth on a
/// sprout (GROWTH_TARGET) grows it into a vine and changes the collision of
/// its cell (Event_GrowPlant); place flag FLAG_PLACE_TEMP_05 keeps it grown
/// until the party leaves the Bilibin Cave Entrance, into the cave beyond or
/// out to the world map, since both are other groups of scenes. Two exits lead
/// on into Bilibin Cave. Beside one of them, touch 10 turns its exit cell off
/// and touch 11, a step further out, turns it back on (Event_DisableExitToCave,
/// Event_EnableExitToCave); XXX: likely so that only walking straight in
/// enters the cave, because the touch cells flank the exit on both sides.
///
/// In BilibinCaveRapier a fire burns between two torches (CAVE2_DOUSE_TARGET),
/// and Douse puts it out for good
/// (FLAG_BILIBIN_CAVE_SECOND_DOUSE_TARGET_CLEARED). Reveal shows a hidden path,
/// a strip of tiles and the cells under them, which Event_HideRevealedPath
/// takes away again when Reveal wears off. A chest holds the Elven Rapier.
///
/// BilibinCaveIce holds the cave's one puzzle of several steps, built from
/// fires, torches and puddles that Frost freezes into ice pillars and the heat
/// of a fire or a torch melts back. A fire (CAVE1_DOUSE_TARGET) burns beside
/// the first puddle: while it burns, Frost on the puddle only melts back
/// (BilibinCave_FreezeTarget plays the melting). Douse puts the fire out
/// (FLAG_BILIBIN_CAVE_FIRST_DOUSE_TARGET_CLEARED), and the puddle freezes into
/// a pillar drawn underfoot, which the party walks over. The second puddle
/// stands beside a torch that cannot be moved, and Frost on it always melts
/// back. The third is an ice pillar: two of the six torches
/// (ACTOR_MOVABLE_TORCH_A and ACTOR_MOVABLE_TORCH_B) can be pushed or moved
/// with Move, and while either stands beside the pillar, on cell (35, 23), the
/// pillar melts into a puddle and the cells north and south of it change with
/// it; when both leave it freezes again
/// (BilibinCave_UpdateFrostTargetFromMovableActors). Place flags
/// FLAG_PLACE_TEMP_03 and FLAG_PLACE_TEMP_04 remember which torch stands there
/// and FLAG_PLACE_TEMP_02 that the pillar has melted, so coming back from
/// BilibinCaveRapier puts the torches and the pillar back; leaving Bilibin Cave
/// resets them. Nine more pieces of ice only stand there: five pillars from
/// ACTOR_FIRST_ICE_PILLAR and four lower pieces from ACTOR_FIRST_LOW_ICE. Its
/// chests hold a Vial, a Psy Crystal and Water of Life, and touches 20 and 21
/// set and clear a byte of the map's work (MapState.unk17).

#include "script.h"
#include "game_state.h"
#include "engine_call.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/bilibin_cave.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
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
#include "map.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "flags.h"
#include "songs.h"

// BilibinCave_Initialize calls one of these for the scene being entered.
static void BilibinCave_RestoreEntrance(void);
static void BilibinCave_RestoreRapierRoom(void);
static void BilibinCave_RestoreIceRoom(void);
static void BilibinCave_EmptyActorSetupHook(s32 actorId);

/// Actors of the Bilibin Cave Entrance (BilibinCaveEntrance), named, as in the
/// other two scenes, by the Psynergy that works on them.
enum {
  GROWTH_TARGET = 8 ///< a sprout Growth grows into a vine
};

/// Actors of BilibinCaveRapier, numbered by the rows of
/// gBilibinCavePlacements2.
enum {
  ACTOR_CAVE2_WEST_TORCH = 8, ///< a torch west of the fire
  CAVE2_DOUSE_TARGET = 9,     ///< a fire between two torches
  ACTOR_CAVE2_EAST_TORCH = 10 ///< a torch east of the fire
};

/// Actors of BilibinCaveIce, numbered by the rows of gBilibinCavePlacements1.
enum {
  CAVE1_DOUSE_TARGET = 8, ///< a fire beside the first puddle
  /// Six torches, 9 to 14; the second stands beside
  /// FROST_TARGET_B.
  ACTOR_FIRST_TORCH = 9,
  ACTOR_MOVABLE_TORCH_A = 11, ///< the third torch, moved by Move or pushed
  ACTOR_MOVABLE_TORCH_B = 12, ///< the fourth torch, moved by Move or pushed
  FROST_TARGET_A = 15,        ///< a puddle, walkable once the fire is out
  FROST_TARGET_B = 16,        ///< a puddle beside a torch that stays put
  /// An ice pillar, melted while a torch stands on cell (35, 23).
  FROST_TARGET_C = 17,
  /// Five ice pillars, 18 to 22, drawn as a frozen Frost target is (animation
  /// 5); nothing acts on them.
  ACTOR_FIRST_ICE_PILLAR = 18,
  /// Four pieces of ice, 23 to 26, held at the second step of freezing: their
  /// animation 3 shows frame 2 of the five that a Frost target's animation 2
  /// plays from puddle to pillar. Nothing acts on them.
  ACTOR_FIRST_LOW_ICE = 23
};

/// Clears the scripted actor's sprite flags and completes this script command.
static s32 BilibinCave_ClearScriptedActorSpriteFlags(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

const struct SceneEntrance *BilibinCave_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_BilibinCaveEntrance) {
    return gBilibinCaveEntrances3;
  }
  if (scene == (s32)&SceneId_BilibinCaveRapier) {
    return gBilibinCaveEntrances2;
  }
  if (scene == (s32)&SceneId_BilibinCaveIce) {
    return gBilibinCaveEntrances1;
  }
  return gBilibinCaveEntrancesOther;
}

const struct SceneRegion *BilibinCave_GetRegions(void) { return NULL; }

const u32 *BilibinCave_GetExits(void) { return gBilibinCaveExits; }

const struct ScenePlacement *BilibinCave_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_BilibinCaveEntrance) {
    return gBilibinCavePlacements3;
  }
  if (scene == (s32)&SceneId_BilibinCaveRapier) {
    return gBilibinCavePlacements2;
  }
  if (scene == (s32)&SceneId_BilibinCaveIce) {
    return gBilibinCavePlacements1;
  }
  return gBilibinCavePlacementsOther;
}

const struct SceneEvent *BilibinCave_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_BilibinCaveEntrance) {
    return gBilibinCaveEvents3;
  }
  if (scene == (s32)&SceneId_BilibinCaveRapier) {
    return gBilibinCaveEvents2;
  }
  if (scene == (s32)&SceneId_BilibinCaveIce) {
    return gBilibinCaveEvents1;
  }
  return gBilibinCaveEventsOther;
}

/// Stepping on touch 10 at the Bilibin Cave Entrance, on either side of the
/// way into Bilibin Cave: the way's cell (21, 14), the exit cell of
/// BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH, takes the
/// attributes of source cell (1, 0), which has none.
static void Event_DisableExitToCave(void) {
  Map_CopyCellAttributes(1, 0, 1, 1, 21, 14);
}

/// Stepping on touch 11 at the Bilibin Cave Entrance, a cell further out: the
/// way's cell (21, 14) takes back the exit, from source cell (0, 0).
static void Event_EnableExitToCave(void) {
  Map_CopyCellAttributes(0, 0, 1, 1, 21, 14);
}

/// Reveal in Bilibin Cave (BilibinCaveRapier) shows the hidden path: a strip of
/// tiles and two rows of the cells under it.
static void Event_RevealHiddenPath(void) {
  {
    s32 width = 1;
    s32 height = 3;

    Map_CopyCellsTo(111, 37, 97, 21, width, height);
  }
  {
    s32 destinationCellX = 32;
    s32 destinationCellZ = 24;

    Map_CopyCellAttributes(46, 38, 3, 2, destinationCellX, destinationCellZ);
  }
}

/// As Reveal wears off in Bilibin Cave (BilibinCaveRapier), the path is hidden
/// again: the strip's tiles and one row of its cells are restored.
static void Event_HideRevealedPath(void) {
  {
    s32 width = 1;
    s32 height = 3;

    Map_CopyCellsTo(95, 21, 97, 21, width, height);
  }
  {
    s32 destinationCellX = 32;
    s32 destinationCellZ = 25;

    Map_CopyCellAttributes(46, 38, 3, 1, destinationCellX, destinationCellZ);
  }
}

/// Douse on the fire in Bilibin Cave (BilibinCaveRapier), until it is out: it
/// goes out for good.
static void Event_DouseRapierRoomFire(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(CAVE2_DOUSE_TARGET, 0, 0);
  Engine_GameFlagSet(FLAG_BILIBIN_CAVE_SECOND_DOUSE_TARGET_CLEARED);
  Engine_EventEnd();
}

/// Douse on the fire in Bilibin Cave (BilibinCaveIce), until it is out: it goes
/// out, and the puddle beside it freezes into a pillar the party can walk over.
static void Event_DouseIceRoomFire(void) {
  Engine_EventBegin();
  Actor_SetPosition(CAVE1_DOUSE_TARGET, 0, 0);
  GameFlag_Set(FLAG_BILIBIN_CAVE_FIRST_DOUSE_TARGET_CLEARED);
  Engine_EventWait(40);
  // Match the replacement's drawing priority to its collision cell.
  Engine_ActorSetAnimationAndWait(FROST_TARGET_A, 2);
  Actor_Get(FROST_TARGET_A)->motionFlags = 0;
  Actor_Get(FROST_TARGET_A)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Engine_ActorSetSpritePriority(FROST_TARGET_A, 2);
  Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
  Engine_EventEnd();
}

/// Frost on a puddle in Bilibin Cave (BilibinCaveIce) that heat keeps from
/// freezing: clears the Psynergy's child effect and plays the ice melting back
/// into a puddle.
static inline void BilibinCave_FreezeTarget(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetChildValue(actor, 0);
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_HIDDEN_PASSAGE_OPEN);
  Engine_ActorSetAnimationAndWait(actor, 6);
  Engine_EventEnd();
}

/// Frost on the first puddle in Bilibin Cave (BilibinCaveIce), while the fire
/// beside it burns.
static void Event_FreezeFirstTarget(void) {
  BilibinCave_FreezeTarget(FROST_TARGET_A);
}

/// Frost on the second puddle in Bilibin Cave (BilibinCaveIce), beside a torch
/// that stays put.
static void Event_FreezeSecondTarget(void) {
  BilibinCave_FreezeTarget(FROST_TARGET_B);
}

/// Frost on the third puddle in Bilibin Cave (BilibinCaveIce), while a torch
/// beside it keeps it melted (FLAG_PLACE_TEMP_02).
static void Event_FreezeThirdTarget(void) {
  BilibinCave_FreezeTarget(FROST_TARGET_C);
}

/// After a push or Move in Bilibin Cave (BilibinCaveIce): while either movable
/// torch stands on cell (35, 23) (FLAG_PLACE_TEMP_03 and FLAG_PLACE_TEMP_04),
/// the ice pillar beside it melts with a sound, and FLAG_PLACE_TEMP_02 says so;
/// when both leave, it freezes again.
static void BilibinCave_UpdateFrostTargetFromMovableActors(void) {
  struct FieldActor *firstActor;
  struct FieldActor *secondActor;
  s32 destinationCellX;

  // Track each movable actor independently before changing the Frost target.
  firstActor = Object_GetById(ACTOR_MOVABLE_TORCH_A);
  secondActor = Actor_Get(ACTOR_MOVABLE_TORCH_B);
  if ((firstActor->x.fixed >> 20) == 35 && (firstActor->z.fixed >> 20) == 23) {
    GameFlag_Set(FLAG_PLACE_TEMP_03);
  } else {
    GameFlag_Clear(FLAG_PLACE_TEMP_03);
  }
  if ((secondActor->x.fixed >> 20) == 35 &&
      (secondActor->z.fixed >> 20) == 23) {
    GameFlag_Set(FLAG_PLACE_TEMP_04);
  } else {
    GameFlag_Clear(FLAG_PLACE_TEMP_04);
  }
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_03) != 0 ||
      GameFlag_IsSet(FLAG_PLACE_TEMP_04) != 0) {
    if (GameFlag_IsSet(FLAG_PLACE_TEMP_02) == 0) {
      Engine_EventBegin();
      Engine_EventWait(40);
      Event_PlayCue(SE_HIDDEN_PASSAGE_OPEN);
      destinationCellX = 36;
      Engine_ActorSetAnimationAndWait(FROST_TARGET_C, 6);
      Map_CopyCellAttributes(0, 1, 1, 1, destinationCellX, 22);
      Map_CopyCellAttributes(0, 2, 1, 1, destinationCellX, 24);
      Engine_EventEnd();
    }
    GameFlag_Set(FLAG_PLACE_TEMP_02);
  } else {
    if (GameFlag_IsSet(FLAG_PLACE_TEMP_02) != 0) {
      Engine_EventBegin();
      Engine_EventWait(40);
      Event_PlayCue(SE_220);
      destinationCellX = 36;
      Engine_ActorSetAnimationAndWait(FROST_TARGET_C, 2);
      Map_CopyCellAttributes(1, 1, 1, 1, destinationCellX, 22);
      Map_CopyCellAttributes(1, 2, 1, 1, destinationCellX, 24);
      Engine_EventEnd();
    }
    GameFlag_Clear(FLAG_PLACE_TEMP_02);
  }
}

/// Growth on the sprout at the Bilibin Cave Entrance, until it has grown: it
/// grows into a vine and its cell's collision changes; FLAG_PLACE_TEMP_05 keeps
/// it grown.
static void Event_GrowPlant(void) {

  Map_CopyCellAttributes(31, 0, 1, 1, 8, 13);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_05);
}

/// Stepping on touch 20 in Bilibin Cave (BilibinCaveIce) sets a byte of the
/// map's work.
static void Event_SetMapTriggerFlag(void) {
  struct MapState *map = gMapWork[0];
  map->unk17 = 1;
}

/// Stepping on touch 21 in Bilibin Cave (BilibinCaveIce) clears the same byte.
static void Event_ClearMapTriggerFlag(void) {
  struct MapState *map = gMapWork[0];
  map->unk17 = 0;
}

/// Sets up the scene being entered, the Bilibin Cave Entrance or one of
/// Bilibin Cave's two, with the puzzles already solved there.
s32 BilibinCave_Initialize(void) {
  s16 variant = gGameState.scene;

  if (variant == (s32)&SceneId_BilibinCaveEntrance) {
    BilibinCave_RestoreEntrance();
  } else if (variant == (s32)&SceneId_BilibinCaveRapier) {
    BilibinCave_RestoreRapierRoom();
  } else if (variant == (s32)&SceneId_BilibinCaveIce) {
    BilibinCave_RestoreIceRoom();
  }
  return 0;
}

/// Sets up the Bilibin Cave Entrance: the vine Growth has grown, with its cell.
static void BilibinCave_RestoreEntrance(void) {
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_05) != 0) {

    Map_CopyCellAttributes(31, 0, 1, 1, 8, 13);
    Engine_ActorSetAnimation(GROWTH_TARGET, 0);
  }
}

/// Sets up Bilibin Cave's BilibinCaveRapier: its two torches, and its fire,
/// gone once Douse has put it out.
static void BilibinCave_RestoreRapierRoom(void) {

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);

  Engine_ActorSetAnimation(ACTOR_CAVE2_WEST_TORCH, 1);
  Engine_ActorSetAnimation(ACTOR_CAVE2_EAST_TORCH, 2);

  if (Engine_GameFlagIsSet(FLAG_BILIBIN_CAVE_SECOND_DOUSE_TARGET_CLEARED) !=
      0) {
    Engine_ActorSetPosition(CAVE2_DOUSE_TARGET, 0, 0);
  } else {
    Engine_ActorSetSpriteFlags(Object_GetById(CAVE2_DOUSE_TARGET), 0);
  }
}

/// Clears the drawing flags of an actor used by the cave's animations.
static inline void BilibinCave_ClearActorSpriteFlags(s32 actorId) {
  struct FieldActor *actor = Actor_Get(actorId);
  Engine_ActorSetSpriteFlags(actor, 0);
}

/// Sets up Bilibin Cave's BilibinCaveIce: its torches, puddles and pillars, and
/// the steps of its puzzle already solved.
static void BilibinCave_RestoreIceRoom(void) {
  struct FieldActor *actor;
  s32 destinationCellX;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  // Set up the decorative animations before restoring interactive actors.
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 1);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 2);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 3);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_ICE_PILLAR + 4);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE + 1);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE + 2);
  BilibinCave_ClearActorSpriteFlags(ACTOR_FIRST_LOW_ICE + 3);
  Engine_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR, 5);
  Engine_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 1, 5);
  Engine_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 2, 5);
  Engine_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 3, 5);
  Engine_ActorSetAnimation(ACTOR_FIRST_ICE_PILLAR + 4, 5);
  Engine_ActorSetAnimation(ACTOR_FIRST_LOW_ICE, 3);
  Engine_ActorSetAnimation(ACTOR_FIRST_LOW_ICE + 1, 3);
  Engine_ActorSetAnimation(ACTOR_FIRST_LOW_ICE + 2, 3);
  Engine_ActorSetAnimation(ACTOR_FIRST_LOW_ICE + 3, 3);
  Engine_ActorSetAnimation(ACTOR_FIRST_TORCH, 2);
  Engine_ActorSetAnimation(ACTOR_FIRST_TORCH + 1, 2);
  Engine_ActorSetAnimation(ACTOR_FIRST_TORCH + 2, 2);
  Engine_ActorSetAnimation(ACTOR_FIRST_TORCH + 3, 2);
  Engine_ActorSetAnimation(ACTOR_FIRST_TORCH + 4, 2);
  Engine_ActorSetAnimation(ACTOR_FIRST_TORCH + 5, 2);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 1);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 2);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 3);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_ICE_PILLAR + 4);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 1);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 2);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_LOW_ICE + 3);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 1);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 2);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 3);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 4);
  BilibinCave_EmptyActorSetupHook(ACTOR_FIRST_TORCH + 5);
  // Restore the surface exposed by Douse, including its collision and priority.
  if (GameFlag_IsSet(FLAG_BILIBIN_CAVE_FIRST_DOUSE_TARGET_CLEARED) != 0) {
    Actor_SetPosition(CAVE1_DOUSE_TARGET, 0, 0);
    Engine_ActorSetAnimation(FROST_TARGET_A, 5);
    Actor_Get(FROST_TARGET_A)->motionFlags = 0;
    Actor_Get(FROST_TARGET_A)->y.fixed = -PIXELS(4);
    Actor_Get(FROST_TARGET_A)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpritePriority(FROST_TARGET_A, 2);
    Map_CopyCellAttributes(0, 0, 1, 1, 18, 14);
  } else {
    Engine_ActorSetAnimation(CAVE1_DOUSE_TARGET, 2);
    actor = Actor_Get(CAVE1_DOUSE_TARGET);
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorSetAnimation(FROST_TARGET_A, 1);
  }
  // Rebuild the Frost target and return moved actors to their saved cells.
  Engine_ActorSetAnimation(FROST_TARGET_B, 1);
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_02) != 0) {
    destinationCellX = 36;
    Engine_ActorSetAnimation(FROST_TARGET_C, 1);
    Map_CopyCellAttributes(0, 1, 1, 1, destinationCellX, 22);
    Map_CopyCellAttributes(0, 2, 1, 1, destinationCellX, 24);
  } else {
    destinationCellX = 36;
    Engine_ActorSetAnimation(FROST_TARGET_C, 5);
    Map_CopyCellAttributes(1, 1, 1, 1, destinationCellX, 22);
    Map_CopyCellAttributes(1, 2, 1, 1, destinationCellX, 24);
  }
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_03) != 0) {
    Actor_SetPosition(ACTOR_MOVABLE_TORCH_A, PIXELS(570), PIXELS(376));
  }
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_04) != 0) {
    Actor_SetPosition(ACTOR_MOVABLE_TORCH_B, PIXELS(570), PIXELS(376));
  }
}

// The shared cell lookup retains its position before the push handler.
#include "common/object_findcell.h"

/// Pushing one of the two movable torches in Bilibin Cave (BilibinCaveIce): it
/// moves only when its next cell and movement probe are clear.
static void Event_PushActorAhead(void) {
  struct FieldActor *leader;
  struct FieldActor *target;
  struct FieldActor *blocker;
  s32 heading;
  s32 nextCellX;
  s32 nextCellZ;
  struct FieldPosition destination;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  heading = leader->facing >> 12;

  nextCellX =
      (leader->x.part.pixel + (gBilibinCave_PushSteps[heading] >> 16)) >> 4;
  nextCellZ =
      (leader->z.part.pixel + (s16)gBilibinCave_PushSteps[heading]) >> 4;
  target = SceneActor_FindActorAtCell(nextCellX, nextCellZ);
  if (target == 0)
    return;

  nextCellX =
      (target->x.part.pixel + (gBilibinCave_PushSteps[heading] >> 16)) >> 4;
  nextCellZ =
      (target->z.part.pixel + (s16)gBilibinCave_PushSteps[heading]) >> 4;
  blocker = SceneActor_FindActorAtCell(nextCellX, nextCellZ);
  if (blocker != 0)
    return;

  // Check the destination before moving both actors into their push states.
  target->terrainId = 2;

  destination.x =
      target->x.fixed + (gBilibinCave_PushSteps[heading] & 0xffff0000);
  destination.y = target->y.fixed;
  destination.z =
      target->z.fixed + (s16)gBilibinCave_PushSteps[heading] * PIXELS(1);

  if (FieldObject_CheckMovementCollision(target, &destination) > 0)
    return;

  Object_SetMode(leader, 8);
  Engine_TaskWait(15);
  Engine_AudioPlayCue(SE_185);

  target->speed = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(target, destination.x, destination.y, destination.z);

  leader->speed = PIXELS(1) / 5;
  leader->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(leader, destination.x, destination.y, destination.z);

  // Wait for the target's movement before snapping it to the destination.
  FieldObject_WaitForMove(target);

  target->x.fixed = destination.x;
  target->z.fixed = destination.z;
  target->velocityX = (s32)blocker;
  target->velocityZ = (s32)blocker;

  // Resume the leader and refresh the mechanism affected by the moved actor.
  Object_SetMode(leader, 1);
  BilibinCave_UpdateFrostTargetFromMovableActors();
}

/// The original setup invokes this empty hook for every animated actor.
static void BilibinCave_EmptyActorSetupHook(s32 actorId) {}

s32 gBilibinCave_ClearSpriteScript[] = {
    SC_CALL(BilibinCave_ClearScriptedActorSpriteFlags),
    SC_END,
};

struct SceneEntrance gBilibinCaveEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 72, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinCaveEntrances3[] = {
    {0, CONDITION_ALWAYS, 372, 0, 272, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 99, 0, 447,
     FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH, CONDITION_ALWAYS,
     342, 0, 254, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH, CONDITION_ALWAYS,
     440, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinCaveEntrances2[] = {
    {0, CONDITION_ALWAYS, 864, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST, CONDITION_ALWAYS, 312,
     0, 756, FACING_NORTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST, CONDITION_ALWAYS, 472,
     0, 680, FACING_NORTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST, CONDITION_ALWAYS, 392, 0,
     200, FACING_SOUTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_TORCHES, CONDITION_ALWAYS, 536, 0, 426,
     FACING_SOUTH, 0, 40, 60, 630, 840, 0},
    {BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST, CONDITION_ALWAYS, 856, 0,
     124, FACING_SOUTH, 0, 760, 65, 925, 330, 0},
    {BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER, CONDITION_ALWAYS,
     808, 0, 248, FACING_NORTH, 0, 760, 65, 925, 330, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinCaveEntrances1[] = {
    {0, CONDITION_ALWAYS, 172, 0, 300, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST, CONDITION_ALWAYS, 120, 0,
     472, FACING_NORTH, 0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST, CONDITION_ALWAYS, 312, 0,
     524, FACING_NORTH, 0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_FROM_WORLD_MAP, CONDITION_ALWAYS, 538, 0, 548,
     FACING_NORTH, 0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_FROM_CHESTS, CONDITION_ALWAYS, 344, 0, 176, FACING_SOUTH,
     0, 60, 60, 790, 600, 0},
    {BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE, CONDITION_ALWAYS, 856, 0,
     632, FACING_NORTH, 0, 740, 510, 980, 690, 0},
    {SCENE_TABLE_END},
};

u32 gBilibinCaveExits[] = {
    SCENE_EXITS(BilibinCaveEntrance),
    SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_BILIBIN_CAVE_ENTRANCE),
    SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH,
               BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_WEST),
    SCENE_EXIT(BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH,
               BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ENTRANCE_EAST),
    SCENE_EXITS(BilibinCaveRapier),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST,
               BilibinCaveEntrance,
               BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_SOUTH),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST,
               BilibinCaveEntrance,
               BILIBIN_CAVE_ENTRANCE_FROM_BILIBIN_CAVE_RAPIER_NORTH),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST, BilibinCaveIce,
               BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_WEST),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_TORCHES, BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_TORCHES_FROM_BILIBIN_CAVE_RAPIER),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST, BilibinCaveIce,
               BILIBIN_CAVE_ICE_FROM_BILIBIN_CAVE_RAPIER_EAST),
    SCENE_EXIT(BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER,
               BilibinCaveRapier, BILIBIN_CAVE_RAPIER_FROM_TORCHES),
    SCENE_EXITS(BilibinCaveIce),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST, BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_WEST),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST, BilibinCaveRapier,
               BILIBIN_CAVE_RAPIER_FROM_BILIBIN_CAVE_ICE_EAST),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_BILIBIN_CAVE_ICE),
    SCENE_EXIT(BILIBIN_CAVE_ICE_TO_CHESTS, BilibinCaveIce,
               BILIBIN_CAVE_ICE_CHESTS_FROM_BILIBIN_CAVE_ICE),
    SCENE_EXIT(BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE, BilibinCaveIce,
               BILIBIN_CAVE_ICE_FROM_CHESTS),
    SCENE_EXITS_END,
};

struct ScenePlacement gBilibinCavePlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gBilibinCavePlacements3[] = {
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinCavePlacements2[] = {
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf7, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinCavePlacements1[] = {
    { 0xf7, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(520), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xf6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(584), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(296), 0, PIXELS(230), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(504), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, (s32)gBilibinCave_ClearSpriteScript,
        PIXELS(584), 0, PIXELS(376), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(224), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gBilibinCaveEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinCaveEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_WORLD_MAP },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_SOUTH },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ENTRANCE_TO_BILIBIN_CAVE_RAPIER_NORTH },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DisableExitToCave) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnableExitToCave) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), GROWTH_TARGET, FLAG_PLACE_TEMP_05, EVENT_SCRIPT(Event_GrowPlant) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinCaveEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_WEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ENTRANCE_EAST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_WEST },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_TORCHES },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TO_BILIBIN_CAVE_ICE_EAST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, BILIBIN_CAVE_RAPIER_TORCHES_TO_BILIBIN_CAVE_RAPIER },
    { SEARCH(SEARCH_CHEST), 100, 0xf5d, FIND_ITEM(ITEM_ELVEN_RAPIER) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RevealHiddenPath) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HideRevealedPath) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), CAVE2_DOUSE_TARGET, FLAG_BILIBIN_CAVE_SECOND_DOUSE_TARGET_CLEARED, EVENT_SCRIPT(Event_DouseRapierRoomFire) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinCaveEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_WEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_BILIBIN_CAVE_RAPIER_EAST },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_WORLD_MAP },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_TO_CHESTS },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, BILIBIN_CAVE_ICE_CHESTS_TO_BILIBIN_CAVE_ICE },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetMapTriggerFlag) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClearMapTriggerFlag) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), CAVE1_DOUSE_TARGET, FLAG_BILIBIN_CAVE_FIRST_DOUSE_TARGET_CLEARED, EVENT_SCRIPT(Event_DouseIceRoomFire) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), FROST_TARGET_A, FLAG_BILIBIN_CAVE_FIRST_DOUSE_TARGET_CLEARED, EVENT_SCRIPT(Event_FreezeFirstTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), FROST_TARGET_B, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FreezeSecondTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), FROST_TARGET_C, IF_FLAG_SET(FLAG_PLACE_TEMP_02), EVENT_SCRIPT(Event_FreezeThirdTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MOVABLE_TORCH_A, CONDITION_ALWAYS, EVENT_SCRIPT(BilibinCave_UpdateFrostTargetFromMovableActors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MOVABLE_TORCH_B, CONDITION_ALWAYS, EVENT_SCRIPT(BilibinCave_UpdateFrostTargetFromMovableActors) },
    { TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushActorAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushActorAhead) },
    { SEARCH(SEARCH_CHEST), 100, 0xf5e, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_CHEST), 101, 0xfc6, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { SEARCH(SEARCH_CHEST), 102, 0xfc7, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { SCENE_EVENTS_END },
};
// clang-format on

/// Packs signed pixel steps with X in the upper halfword and Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gBilibinCave_PushSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

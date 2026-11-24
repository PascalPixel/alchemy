/// @file
/// The Alhafran Cave under the town, in two scenes that share this overlay:
/// the cave behind the rear entrance by the mayor's home, which the hungry
/// boy's father lets the party into once they give the boy Large Bread
/// (SceneId_AlhafranCave), and the town's jail with the caves beyond it
/// (SceneId_AlhafraJail).
///
/// After Briggs and his crew are jailed, his wife Chaucha and their son Eoleo
/// visit him as Felix walks into the jail, and the boy lifts the cell's key
/// with Psynergy of his own until his father stops him
/// (AlhafranCave_ChauchaAndEoleoVisit). In the caves, Pound and Lash reach
/// three chests; once Briggs has escaped, a pillar no longer blocks the way,
/// and Frost on a puddle makes a pillar of ice up to three more
/// (AlhafranCave_FreezePuddle).

#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/scene_tables.h"
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/scene.h"
#include "scene/ground_effects.h"
#include "scene/event_layer.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "version.h"
#include "system/task.h"
#include "field/map_grid.h"
#include "field/call.h"
#include "field/effect_sprite.h"
#include "system/io_reg.h"
#include "text/msg_ids.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/object.h"
#include "scene/dehkan_plateau.h"
#include "scene/gabomba_statue.h"

void Pillar_Place(const u16 *table);
void AlhafranCave_TakeLeadersLayer(void);
extern s32 gAlhafranCave_GuardStepsAsideScript[];
extern u32 gAlhafranCave_VisitorsLeaveScript[];
void AlhafranCave_RaiseBridgeCells(void);
void AlhafranCave_LowerBridgeCells(void);
void AlhafranCave_CastSand(void);
void AlhafranCave_StartMovingCrate(void);
void AlhafranCave_FinishMovingCrate(void);
void FlagBlock_PlaceAllMarkingEdges(const void *data);

/// Set while the ground under the cave's bridges stands at their height, so
/// that the leader walks over them rather than under.
#define FLAG_BRIDGE_CELLS_RAISED FLAG_SCENE_TEMP_04
/// Set once Lash has tied the jail's rope.
#define FLAG_JAIL_ROPE_TIED FLAG_SCENE_TEMP_00
/// Set once Pound has sunk the jail's stake.
#define FLAG_JAIL_STAKE_POUNDED FLAG_SCENE_TEMP_01
/// Set once Frost has frozen the cave's puddle.
#define FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN FLAG_SCENE_TEMP_03
/// The cave's actors (SceneId_AlhafranCave), numbering the rows of
/// gAlhafranCave_CaveActorPlacements.
enum AlhafranCaveActorId {
  /// Three stone bridges, raised twelve pixels over the channel and the chasm.
  ACTOR_CAVE_FIRST_BRIDGE = ACTOR_FIRST_PLACED,
  ACTOR_CAVE_SECOND_BRIDGE,
  ACTOR_CAVE_THIRD_BRIDGE,
  /// A broken bridge: its two ends on their legs (SPRITE_WOODEN_SIGN, in its
  /// animations 2 and 3) and its middle fallen below them, turned a sixteenth
  /// of a turn.
  ACTOR_CAVE_BROKEN_BRIDGE_WEST_END,
  ACTOR_CAVE_BROKEN_BRIDGE_EAST_END,
  ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE,
  /// Two stone pillars, one in front of a ladder and one across a passage,
  /// that stand 16 pixels west once the mast is cleared and the jail is empty.
  ACTOR_CAVE_PILLAR_AT_LADDER,
  ACTOR_CAVE_PILLAR_IN_PASSAGE,
  /// The puddle Frost freezes into a pillar of ice.
  ACTOR_CAVE_PUDDLE
};

/// The jail's actors (SceneId_AlhafraJail), numbering the rows of
/// gAlhafranCave_JailActorPlacements.
enum AlhafraJailActorId {
  /// The crate Move pushes.
  ACTOR_CRATE = ACTOR_FIRST_PLACED,
  /// The stake Pound sinks.
  ACTOR_JAIL_STAKE,
  /// The posts of the rope Lash ties, on the floor and on the ledge.
  ACTOR_JAIL_ROPE_LOW_POST,
  ACTOR_JAIL_ROPE_HIGH_POST,
  /// A broken bridge's two ends and its fallen middle, as the cave's.
  ACTOR_JAIL_BROKEN_BRIDGE_WEST_END,
  ACTOR_JAIL_BROKEN_BRIDGE_EAST_END,
  ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE,
  /// The key on the guard's table outside the cell, which Eoleo lifts with
  /// a Move hand of his own.
  ACTOR_JAIL_KEY,
  /// Briggs and his crew in their cell, once Briggs is caught.
  /// "I hate prison. The food is terrible..."
  ACTOR_PIRATE_WHO_HATES_PRISON,
  /// "It sounds like the mayor of Alhafra lost interest in fixing that boat."
  ACTOR_PIRATE_ON_THE_BOAT,
  ACTOR_BRIGGS,
  /// "Everything would have been fine if it hadn't been for you meddling
  /// kids..."
  ACTOR_PIRATE_BLAMING_THE_PARTY,
  /// "I saw you staring at Eoleo..."
  ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
  ACTOR_EOLEO,
  ACTOR_CHAUCHA,
  /// "Everything's so boring here... Why doesn't anybody do anything...?"
  ACTOR_BORED_GUARD,
  ACTOR_GUARD
};

/// The jail's way up through the hole the crate hides, from the tunnel behind
/// the cell.
enum AlhafraJailEntrance { ENTRANCE_UNDER_CRATE = 9 };

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Actor_CycleSpriteColors(void);
void FlagBlock_ApplyFlags(const void *data);
void Actor_TakeLeaderLayer(void);
void EffectSprite_DrawAtWorldXYZ(struct EffectSprite *sprite, s32 x, s32 y,
                                 s32 z);

void AlhafranCave_SetUpCave(void);
void AlhafranCave_SetUpJail(void);

extern s16 gAlhafranCave_Puddles[];
extern u16 gAlhafranCave_PoundStakes[];
extern struct ActorPlacement gAlhafranCave_CaveActorPlacements[];
extern struct ActorPlacement gAlhafranCave_JailActorPlacements[];
extern struct Trigger gAlhafranCave_CaveTriggers[];
extern struct Trigger gAlhafranCave_JailTriggers[];
struct EffectSprite gAlhafranCave_CrateSprite BSS_DATA ALIGNED(16);
/// The frame count of AlhafranCave_IdleTask, cleared as the task is added, as
/// other scenes clear the count their event's task keeps; the task is empty, so
/// nothing counts it or reads it.
s32 gAlhafranCave_IdleTaskFrame BSS_DATA;

/// The current scene's actors: the cave's or the jail's.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_ALHAFRAN_CAVE)
    return gAlhafranCave_CaveActorPlacements;
  if (scene == SCENE_ALHAFRA_JAIL)
    return gAlhafranCave_JailActorPlacements;
  return gAlhafranCave_CaveActorPlacements;
}

/// Felix casts Pound on the stake in the caves beyond the jail and drives it
/// into the ground.
void AlhafranCave_PoundStake(s32 mode, s32 frame) {
  Pound_SinkTarget(gAlhafranCave_PoundStakes, mode, frame);
}

/// Nothing more than the move itself happens as Felix starts casting Move on
/// the crate in the jail.
void AlhafranCave_StartMovingCrate(void) {}

/// Nothing more than the move itself happens once Move has set the jail's
/// crate down.
void AlhafranCave_FinishMovingCrate(void) {}

/// Felix talks to Briggs in his cell, who asks whether he has stopped by for
/// a visit, and answers yes or no.
void AlhafranCave_TalkToBriggs(s32 id) {
  FieldEvent_SetMessageId((s32)MsgOhStoppedBy);
  // "Oh, stopped by for a visit, did you?"
  // Yes: "I wish that I'd met you under different circumstances..."
  // No: "Ah, so you just came here to check up on us? Hah! Well, as you can..."
  Event_OpenMessage(id, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId((s32)MsgOhStoppedBy + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId((s32)MsgOhStoppedBy + 2);
  }
  Event_ShowMessage(id, 0);
}

/// Felix casts Frost on the puddle below the three chests in the cave, and it
/// freezes into a pillar of ice that lifts the way up to them.
void AlhafranCave_FreezePuddle(void) {
  FlagBlock_ApplyFlags(gAlhafranCave_Puddles);
}

/// Nothing visible happens as Felix walks east onto cell (18, 8), on the ledge
/// 32 pixels up along the cave's north wall; the game raises the ground under
/// the southern stone bridge (cells (25, 19) to (27, 19)) and under the broken
/// bridge's two ends (cells (18, 24) and (21, 24)) to the height of the ledges
/// they join, so that from the ledges he walks over the bridges rather than
/// under them. Walking back west along the ledge onto cell (17, 7), the cell
/// north-west of it, lowers them again (AlhafranCave_LowerBridgeCells).
///
/// The ground goes up four steps, and the cell north-west of the southern
/// bridge becomes a wall so that he cannot step off its end.
void AlhafranCave_RaiseBridgeCells(void) {
  Event_SetFlag(FLAG_BRIDGE_CELLS_RAISED);
  FieldMapGrid_SetCellHeight(toFIXED(400), toFIXED(304), 0, 4);
  FieldMapGrid_SetCellHeight(toFIXED(416), toFIXED(304), 0, 4);
  MapGrid_SetCellHeight(toFIXED(432), toFIXED(304), 0, 4);
  FieldMapGrid_SetCellBehavior(toFIXED(384), toFIXED(288), 0,
                               CELL_BEHAVIOR_WALL);
  FieldMapGrid_SetCellHeight(toFIXED(288), toFIXED(384), 0, 4);
  MapGrid_SetCellHeight(toFIXED(336), toFIXED(384), 0, 4);
}

/// Nothing visible happens as Felix walks past cell (17, 7) or (15, 25); the
/// game lowers the ground under the bridges to the cave's floor again, so
/// that down there he walks under them.
void AlhafranCave_LowerBridgeCells(void) {
  Event_ClearFlag(FLAG_BRIDGE_CELLS_RAISED);
  FieldMapGrid_SetCellHeight(toFIXED(400), toFIXED(304), 0, 0);
  FieldMapGrid_SetCellHeight(toFIXED(416), toFIXED(304), 0, 0);
  MapGrid_SetCellHeight(toFIXED(432), toFIXED(304), 0, 0);
  FieldMapGrid_SetCellBehavior(toFIXED(384), toFIXED(288), 0, 0);
  FieldMapGrid_SetCellHeight(toFIXED(288), toFIXED(384), 0, 0);
  MapGrid_SetCellHeight(toFIXED(336), toFIXED(384), 0, 0);
}

/// Felix casts Sand in the cave: the world turns grey and he sinks into the
/// floor as a mound of sand. The game draws the mound's sand trail at
/// priority 2 here rather than the usual 3.
void AlhafranCave_CastSand(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->sandTrailPriority = 2;
}

/// The current scene's triggers: the cave's or the jail's.
const struct Trigger *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_ALHAFRAN_CAVE)
    return gAlhafranCave_CaveTriggers;
  if (scene == SCENE_ALHAFRA_JAIL)
    return gAlhafranCave_JailTriggers;
  return gAlhafranCave_CaveTriggers;
}

/// Nothing visible: each frame the game notes whether Felix stands on the
/// cave's upper level, for wading.
void AlhafranCave_CheckUpperLevel(void) {
  FIXED y = Actor_Get(gMainRoutineState.leaderActorId)->y.fixed;

  if (y < toFIXED(8))
    gMainRoutineState.onUpperLevel = 0;
  else
    gMainRoutineState.onUpperLevel = 1;
}

/// The jail's task: draws the crate's top where the crate stands.
s32 AlhafranCave_DrawCrateSprite(void) {
  struct Actor *actor = Actor_Get(ACTOR_CRATE);

  EffectSprite_DrawAtWorldXYZ(&gAlhafranCave_CrateSprite, actor->x.fixed,
                              actor->y.fixed + toFIXED(28), actor->z.fixed);
  return 0;
}

/// Sets up the cave or the jail.
s32 Scene_Initialize(void) {
#if !TLA_PATCH_1 /* The Japanese edition does not check the entrance. */
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (gMainRoutineState.next == SCENE_ALHAFRAN_CAVE)
    AlhafranCave_SetUpCave();
  else if (gMainRoutineState.next == SCENE_ALHAFRA_JAIL)
    AlhafranCave_SetUpJail();
  return 0;
#else
  s32 scene = gMainRoutineState.next;

  if (scene == SCENE_ALHAFRAN_CAVE) {
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    AlhafranCave_SetUpCave();
  } else if (scene == SCENE_ALHAFRA_JAIL) {
    if (gMainRoutineState.entrance == ALHAFRA_JAIL_FROM_ALHAFRA ||
        gMainRoutineState.entrance == ENTRANCE_UNDER_CRATE)
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    else
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    AlhafranCave_SetUpJail();
  }
  return 0;
#endif
}

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

/// Sets up the cave: its bridges, the broken bridge, the pillars, the puddle,
/// and the cells under the bridges if the leader was on the ledges.
void AlhafranCave_SetUpCave(void) {
  struct MapStateGroundEffects *map =
      (struct MapStateGroundEffects *)Ram_HeapSlots->mapGrid;
  struct Actor *actor;

  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  map->wading = MAP_WADING_UPPER_LEVEL;
  Task_AddFrame(AlhafranCave_CheckUpperLevel, TASK_PASS_FRAME);
  if (Event_GetFlag(FLAG_BRIDGE_CELLS_RAISED))
    AlhafranCave_RaiseBridgeCells();
  actor = Actor_Get(ACTOR_CAVE_FIRST_BRIDGE);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(12);
  actor = Actor_Get(ACTOR_CAVE_SECOND_BRIDGE);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(12);
  actor = Actor_Get(ACTOR_CAVE_THIRD_BRIDGE);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(12);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_FIRST_BRIDGE),
                           SPRITE_COLOR_GREY);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_SECOND_BRIDGE),
                           SPRITE_COLOR_GREY);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_THIRD_BRIDGE),
                           SPRITE_COLOR_GREY);
  actor = Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(16);
  actor = Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(16);
  FieldActor_SetSpriteRotationById(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE,
                                   TRIG_TURN / 16);
  Actor_SetAnimation(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END, 2);
  Actor_SetAnimation(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END, 3);
  Actor_SetAnimation(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE, 2);
  FieldActor_SetSpritePriority(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END, 2);
  FieldActor_SetSpritePriority(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END, 2);
  FieldActor_SetSpritePriority(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE, 3);
  Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_WEST_END),
                           SPRITE_COLOR_GREY);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_EAST_END),
                           SPRITE_COLOR_GREY);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_CAVE_BROKEN_BRIDGE_MIDDLE),
                           SPRITE_COLOR_GREY);
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(ACTOR_CAVE_PILLAR_AT_LADDER, 200, 136);
    Actor_SetPosition(ACTOR_CAVE_PILLAR_IN_PASSAGE, 392, 360);
  }
  actor = Actor_Get(ACTOR_CAVE_PILLAR_AT_LADDER);
  actor->motionFlags = 0;
  actor->groundHeight = 0;
  actor->y.fixed = 0;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 4);
  actor = Actor_Get(ACTOR_CAVE_PILLAR_IN_PASSAGE);
  actor->motionFlags = 0;
  actor->groundHeight = 0;
  actor->y.fixed = 0;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 4);
  FlagBlock_PlaceAllMarkingEdges(gAlhafranCave_Puddles);
  Actor_SetMovementType(Actor_Get(ACTOR_CAVE_PUDDLE),
                        MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  // Until the mast is cleared, the ladder behind the first pillar is drawn as
  // rock, with two more stretches of the cave.
  if (!Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    MapGrid_CopyCellsLayered(0, 58, 12, 7, 13, 58);
    MapGrid_CopyCellsLayered(0, 58, 2, 9, 25, 66);
    MapGrid_CopyCellsLayered(0, 58, 10, 5, 15, 74);
  }
}

void FieldEffect_StartBlankOverlay(s32 value);
void EffectSprite_InitWithFadeOverlay(struct EffectSprite *sprite, s32 pivotX,
                                      s32 pivotY, u32 attributes);

/// Sets up the jail and the cave: who is held and who visits, by how far the
/// story has gone (Briggs caught, his family's visit, the mast cleared), and
/// the crate pushed aside as the party climbs up through the hole under it.
void AlhafranCave_SetUpJail(void) {
  struct EffectSprite *crate;
  struct FieldState *state;

  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_CRATE)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldEffect_StartBlankOverlay(0);
  crate = &gAlhafranCave_CrateSprite;
  FadeOverlay_FillTiles(0);
  Call4((void (*)())EffectSprite_InitWithFadeOverlay, (s32)crate, 8, 15,
        OBJ_SIZE_16X16);
  FieldTask_AddFrame(AlhafranCave_DrawCrateSprite, TASK_PASS_FRAME);
  if (gMainRoutineState.entrance == ALHAFRA_JAIL_FROM_ALHAFRA ||
      gMainRoutineState.entrance == ENTRANCE_UNDER_CRATE) {
    state = Ram_HeapSlots->fieldState;

    state->psynergyDrawFlags =
        PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
  }
  Rope_Initialize();
  Rope_Place(0, FLAG_JAIL_ROPE_TIED, ACTOR_JAIL_ROPE_LOW_POST,
             ACTOR_JAIL_ROPE_HIGH_POST);
  Actor_Get(ACTOR_JAIL_ROPE_LOW_POST)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_JAIL_ROPE_HIGH_POST)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Pillar_Place(gAlhafranCave_PoundStakes);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JAIL_STAKE), 0);
  FieldActor_SetSpriteRotationById(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE,
                                   TRIG_TURN / 16);
  Actor_SetAnimation(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END, 0);
  Actor_SetAnimation(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END, 1);
  Actor_SetAnimation(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE, 2);
  FieldActor_SetSpritePriority(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END, 1);
  FieldActor_SetSpritePriority(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END, 1);
  Actor_Get(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_JAIL_BROKEN_BRIDGE_WEST_END),
                           SPRITE_COLOR_GREY);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_JAIL_BROKEN_BRIDGE_EAST_END),
                           SPRITE_COLOR_GREY);
  Actor_SetSpriteColorMode(Actor_Get(ACTOR_JAIL_BROKEN_BRIDGE_MIDDLE),
                           SPRITE_COLOR_GREY);
  if (Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    Actor_SetPosition(ACTOR_JAIL_KEY, 0, 0);
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_WHO_HATES_PRISON, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_ON_THE_BOAT, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_BLAMING_THE_PARTY, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_GUARD, 0, 0);
    Actor_Get(ACTOR_BORED_GUARD)->update = AlhafranCave_TakeLeadersLayer;
  } else if (Event_GetFlag(FLAG_BRIGGS_CAUGHT)) {
    Actor_SetPosition(ACTOR_BORED_GUARD, 0, 0);
    FieldActor_SetSpritePriority(ACTOR_BRIGGS, 2);
    FieldActor_SetSpritePriority(ACTOR_PIRATE_WHO_HATES_PRISON, 2);
    FieldActor_SetSpritePriority(ACTOR_PIRATE_ON_THE_BOAT, 2);
    FieldActor_SetSpritePriority(ACTOR_PIRATE_BLAMING_THE_PARTY, 2);
    FieldActor_SetSpritePriority(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 2);
    if (!Event_GetFlag(FLAG_FAMILY_VISITED_BRIGGS)) {
      Actor_SetPosition(ACTOR_PIRATE_ON_THE_BOAT, 712, 216);
      Actor_SetPosition(ACTOR_PIRATE_BLAMING_THE_PARTY, 744, 216);
    } else {
      Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
      Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    }
  } else {
    Actor_SetPosition(ACTOR_BRIGGS, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_WHO_HATES_PRISON, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_ON_THE_BOAT, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_BLAMING_THE_PARTY, 0, 0);
    Actor_SetPosition(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 0, 0);
    Actor_SetPosition(ACTOR_EOLEO, 0, 0);
    Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
    Actor_SetPosition(ACTOR_GUARD, 0, 0);
    Actor_Get(ACTOR_BORED_GUARD)->update = AlhafranCave_TakeLeadersLayer;
  }
  if (gMainRoutineState.entrance == ENTRANCE_UNDER_CRATE &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Actor_SetSpeed(ACTOR_CRATE, 0.3);
    Actor_SetDestinationOffset(ACTOR_CRATE, 0, 16);
    FieldActor_WaitForMoveById(ACTOR_CRATE);
  }
}

/// Once the mast is cleared and the jail stands empty, the passage where the
/// tunnel from under the crate comes out (entrance 10) is drawn into the
/// rope's cave.
s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.next == SCENE_ALHAFRA_JAIL &&
      Event_GetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
    MapGrid_CopyCellsLayered(23, 46, 4, 5, 21, 14);
    MapGrid_CopyCellsLayered(86, 44, 5, 6, 84, 12);
    MapGrid_CopyCellAttributes(22, 45, 5, 6, 20, 13);
  }
  return 0;
}

/// An empty task, added as Eoleo starts crying and never removed; it does
/// nothing.
void AlhafranCave_IdleTask(void) {}

extern u8 MsgYouHereTo[];

void AlhafranCave_CycleKeyColors(void);

/// Felix walks west into the jail after Briggs is caught (cells (49, 16) to
/// (49, 19)): the guard asks if he is here to see Briggs, and Briggs's wife
/// Chaucha and their son Eoleo visit him. Eoleo lifts the cell's key off the
/// guard's table with Psynergy of his own until Briggs cries out his name,
/// then bursts into tears, and Chaucha takes him home. It happens once, and
/// not after the mast is cleared.
void AlhafranCave_ChauchaAndEoleoVisit(void) {
  struct Actor *eoleo;
  struct Actor *key;
  struct Actor *effect;
  FIXED position[3];

  if (GameFlag_Get(FLAG_BRIGGS_CAUGHT) == 0)
    return;
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED) == 1)
    return;
  Event_SetFlag(FLAG_FAMILY_VISITED_BRIGGS);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetSpeed(ACTOR_CHAUCHA, 1);
  Actor_SetSpeed(ACTOR_EOLEO, 1);
  Actor_SetSpeed(ACTOR_PIRATE_ON_THE_BOAT, 1);
  Actor_SetSpeed(ACTOR_PIRATE_BLAMING_THE_PARTY, 1);
  Actor_SetSpeed(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 1);
  FieldEvent_SetMessageId((s32)MsgYouHereTo);
  // "You here to see Briggs?"
  FieldEvent_ShowMessageThenWait(ACTOR_GUARD, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 800, 288);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(20);
  // "Chaucha... It's good to see you."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  // "And you, too, Eoleo."
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  FieldActor_Jump(ACTOR_EOLEO, 6, 15);
  FieldActor_Jump(ACTOR_EOLEO, 6, 23);
  // "Goo. Goo goo ga."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "How...is your cell? Is there anything I can do to make this easier..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIRATE_WHO_HATES_PRISON, 2);
  FieldEvent_Wait(10);
  // "I'm fine, Chaucha. My only complaint is that the food here is terrible."
  FieldEvent_ShowMessageThenWait(ACTOR_PIRATE_WHO_HATES_PRISON, 0, 10);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "What!? Are they forcing you to eat this disgusting gruel?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  (void *)Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SCOWL, 50);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 312, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_SetAnimation(ACTOR_EOLEO, ANIM_NOD);
  FieldActor_StartRepeatedMotion(ACTOR_GUARD, 3);
  Actor_SetAttachedEffect(ACTOR_GUARD,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  EventCamera_FollowActor(ACTOR_GUARD, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_GUARD, 1.5);
  Actor_SetScript(ACTOR_GUARD, gAlhafranCave_GuardStepsAsideScript);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_GUARD);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(10);
  Actor_ShowEmote(ACTOR_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "Now, wait a second..."
  FieldEvent_ShowMessageThenWait(ACTOR_GUARD, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GUARD, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  // "First of all, it's not disgusting. It's nutritious! And besides..."
  FieldEvent_ShowMessageThenWait(ACTOR_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_GUARD, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "You're IN JAIL! What did you expect, gourmet cuisine?"
  FieldEvent_ShowMessageThenWait(ACTOR_GUARD, 0, 10);
  eoleo = Actor_Get(ACTOR_EOLEO);
  key = Actor_Get(ACTOR_JAIL_KEY);
  Event_StartColorTransition(8, RGB(16, 16, 4));
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO),
                                   CASTER_LOOK_CYCLING_COLORS);
  Event_PlaySound(SE_ASCEND);
  FieldEvent_Wait(40);
  effect = FieldEffect_SpawnMoveHand(eoleo);
  position[0] = key->x.fixed;
  effect->watchedActor = key;
  position[1] = key->y.fixed + toFIXED(16);
  position[2] = key->z.fixed;
  FieldVector_AddPolarOffset(16, FACING_NORTH, position);
  FieldActor_SetMoveTargetFX(effect, position[0], position[1], position[2]);
  FieldEffect_GrowHandAndWait(effect);
  Actor_SelectAnimation(effect, 4);
  key->update = AlhafranCave_CycleKeyColors;
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_WEST, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIRATE_ON_THE_BOAT, ANIM_NOD);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_PIRATE_BLAMING_THE_PARTY, ANIM_NOD);
  Actor_FaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 288, 1);
  FieldCamera_WaitForArrival();
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_ShowEmote(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldCamera_MoveTo(744, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, -16, 0);
  FieldEvent_Wait(20);
  (void *)Actor_ShowEmote(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
                          EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(40);
  FieldActor_StartRepeatedMotion(ACTOR_BRIGGS, 3);
  Actor_SetAttachedEffect(ACTOR_BRIGGS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  // "Eoleo!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 20);
  Actor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  key->update = 0;
  Actor_SetSpriteColorMode(key, 0);
  FieldEffect_BurstHand(effect);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), CASTER_LOOK_NORMAL);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GUARD, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(ACTOR_EOLEO, 3);
  Actor_SetAttachedEffect(ACTOR_EOLEO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_NORTH, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GUARD, FACING_WEST, 0);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_SHAKE_HEAD);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Field_StartColorTransition(8, RGB(16, 16, 4));
  BattleEffect_StartArcEffect();
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO),
                                   CASTER_LOOK_CYCLING_COLORS);
  Event_PlaySound(SE_ASCEND);
  FieldEvent_Wait(40);
  // "Eoleo!"
  FieldEvent_ShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
  BattleEffect_SetObjectUpdateMode(Actor_Get(ACTOR_EOLEO), CASTER_LOOK_NORMAL);
  BattleEffect_WaitForArcGroups();
  BattleEffect_EndArcEffect();
  Field_StartPaletteTransition(8);
  Actor_FaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GUARD, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_NORTH_NORTHEAST, 0);
  FieldEvent_Wait(20);
  // "Hooo..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  (void *)Actor_ShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_RunRepeatedMotion(ACTOR_EOLEO, 2);
  FieldEvent_Wait(20);
  // "Ahhh waaah..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_FaceDirection(ACTOR_PIRATE_WHO_HATES_PRISON, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHWEST,
                      0);
  Actor_FaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GUARD, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_EOLEO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // "WAHHH!"
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 20);
  gAlhafranCave_IdleTaskFrame = 0;
  Task_AddFrame(AlhafranCave_IdleTask, TASK_PASS_FRAME);
  Actor_ShowEmote(ACTOR_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "I don't know why he's crying! You're his mother! Can't you do..."
  FieldEvent_ShowMessageThenWait(ACTOR_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(ACTOR_CHAUCHA, 0, 8);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  FieldEvent_Wait(20);
  // "There there, Eoleo... What's the matter?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_EOLEO, 2);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(ACTOR_EOLEO,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  // "Waa waa waa!"
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Did the mean old guard frighten you?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Waa waa..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Don't let him bother you... He's just a big bully! Come on! Let's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Waa waa..."
  FieldEvent_ShowMessageThenWait(ACTOR_EOLEO, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_EAST_NORTHEAST, 0);
  FieldEvent_Wait(30);
  // "We'll be back again later. Will that be all right?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_GUARD, ANIM_NOD);
  FieldEvent_Wait(10);
  // "Yeah, yeah. That's fine."
  FieldEvent_ShowMessageThenWait(ACTOR_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_NORTH_NORTHWEST, 0);
  FieldEvent_Wait(20);
  // "We'll come back soon, dear."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_CHAUCHA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_FaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(ACTOR_CHAUCHA, ANIM_NOD);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(ACTOR_GUARD, ACTOR_EOLEO);
  Actor_WalkBy(ACTOR_CHAUCHA, 0, -16);
  Actor_WalkByAndWait(ACTOR_EOLEO, 0, -16);
  Actor_WalkBy(ACTOR_CHAUCHA, 52, 0);
  Actor_WalkByAndWait(ACTOR_EOLEO, 52, 0);
  FieldEvent_Wait(15);
  FieldActor_StartRepeatedMotion(ACTOR_CHAUCHA, 3);
  Actor_SetAttachedEffect(ACTOR_CHAUCHA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  FieldActor_WatchAndSetScript(ACTOR_FELIX, ACTOR_EOLEO);
  FieldActor_SetSpritePriority(ACTOR_CHAUCHA, 2);
  FieldActor_SetSpritePriority(ACTOR_EOLEO, 2);
  Actor_SetScript(ACTOR_CHAUCHA, gAlhafranCave_VisitorsLeaveScript);
  FieldEvent_Wait(10);
  Actor_SetScript(ACTOR_EOLEO, gAlhafranCave_VisitorsLeaveScript);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, 16, 0);
  Actor_FaceDirection(ACTOR_PIRATE_GUARDING_EOLEOS_SECRET,
                      FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkBy(ACTOR_PIRATE_ON_THE_BOAT, 0, 16);
  Actor_WalkByAndWait(ACTOR_PIRATE_BLAMING_THE_PARTY, 0, 16);
  FieldEvent_Wait(5);
  Actor_FaceDirection(ACTOR_PIRATE_ON_THE_BOAT, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIRATE_BLAMING_THE_PARTY, FACING_SOUTH_SOUTHEAST,
                      0);
  Actor_FaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHEAST, 0);
  FieldActor_WaitForScriptEndById(ACTOR_EOLEO);
  FieldActor_Stop(ACTOR_GUARD);
  FieldActor_Stop(ACTOR_FELIX);
  Actor_SetPosition(ACTOR_CHAUCHA, 0, 0);
  Actor_SetPosition(ACTOR_EOLEO, 0, 0);
  Actor_SetMovementType(Actor_Get(ACTOR_GUARD),
                        MOVEMENT_TYPE_TAKE_LEADER_LAYER);
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  Event_End();
}

/// An actor's update: it takes the party leader's display layer.
void AlhafranCave_TakeLeadersLayer(void) { Actor_TakeLeaderLayer(); }

/// The key's update while Eoleo's Psynergy holds it: its colours cycle.
void AlhafranCave_CycleKeyColors(void) { Actor_CycleSpriteColors(); }

#include "shared/pillar_place.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

// clang-format off
/// The puddle Frost freezes into a pillar, and the flag set while it stands.
s16 gAlhafranCave_Puddles[] = {
    ACTOR_CAVE_PUDDLE, FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN, -1, 0,
};

s32 gAlhafranCave_GuardStepsAsideScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -60),
    SC_SYNC,
    SC_MOVE_BY(-4, 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

u32 gAlhafranCave_VisitorsLeaveScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(780, 0, 288),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 12),
    SC_SYNC,
    SC_MOVE_BY(64, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(32, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 54),
    SC_SYNC,
    SC_END,
};

/// The jail's stake that Pound sinks: its actor, its kind and the flag set once
/// it is sunk.
u16 gAlhafranCave_PoundStakes[] = {
    ACTOR_JAIL_STAKE, 1, FLAG_JAIL_STAKE_POUNDED, ACTOR_LIST_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(AlhafranCave),
    SCENE_EXIT_UNLESS(ALHAFRAN_CAVE_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_ALHAFRAN_CAVE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AlhafranCave, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AlhafranCave, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ALHAFRAN_CAVE_TO_ALHAFRA_JAIL_WEST, AlhafraJail, ALHAFRA_JAIL_FROM_ALHAFRAN_CAVE_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ALHAFRAN_CAVE_TO_ALHAFRA_JAIL_EAST, AlhafraJail, ALHAFRA_JAIL_FROM_ALHAFRAN_CAVE_EAST, EXIT_FLAG_NONE),
    SCENE_EXITS(AlhafraJail),
    SCENE_EXIT_UNLESS(ALHAFRA_JAIL_TO_ALHAFRAN_CAVE_WEST, AlhafranCave, ALHAFRAN_CAVE_FROM_ALHAFRA_JAIL_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ALHAFRA_JAIL_TO_ALHAFRAN_CAVE_EAST, AlhafranCave, ALHAFRAN_CAVE_FROM_ALHAFRA_JAIL_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ALHAFRA_JAIL_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_ALHAFRA_JAIL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AlhafraJail, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AlhafraJail, 9, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAlhafranCave_CaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 120, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 472, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 424, 0, 312, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 296, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 344, 0, 392, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 324, 0, 416, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafranCave_JailActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 184, 0, 284, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WOODEN_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 232, 0, 284, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 196, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JAIL_KEY, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 720, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 304, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 296, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 744, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 736, 0, 360, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAlhafranCave_CaveTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(AlhafranCave_LowerBridgeCells)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(AlhafranCave_RaiseBridgeCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CAVE_PUDDLE, FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN, EVENT(AlhafranCave_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AlhafranCave_CastSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAlhafranCave_JailTriggers[] = {
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 20, FLAG_FAMILY_VISITED_BRIGGS, EVENT(AlhafranCave_ChauchaAndEoleoVisit)},
    {TRIGGER_TALK, ACTOR_BORED_GUARD, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgEverythingSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BORED_GUARD, FLAG_ALHAFRA_MAST_CLEARED, MSGNO(MsgIDonTKnow)},
    {TRIGGER_TALK, ACTOR_BORED_GUARD, CONDITION_ALWAYS, MSGNO(MsgBeforeIKnew)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BORED_GUARD, CONDITION_ALWAYS, MSGNO(MsgNoMatterHow)},
    {TRIGGER_TALK, ACTOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgIVeBeen)},
    {TRIGGER_TALK, ACTOR_BRIGGS, CONDITION_ALWAYS, EVENT(AlhafranCave_TalkToBriggs)},
    {TRIGGER_TALK, ACTOR_PIRATE_WHO_HATES_PRISON, CONDITION_ALWAYS, MSGNO(MsgIHatePrison)},
    {TRIGGER_TALK, ACTOR_PIRATE_ON_THE_BOAT, CONDITION_ALWAYS, MSGNO(MsgItSoundsLike)},
    {TRIGGER_TALK, ACTOR_PIRATE_BLAMING_THE_PARTY, CONDITION_ALWAYS, MSGNO(MsgEverythingWouldHave)},
    {TRIGGER_TALK, ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, CONDITION_ALWAYS, MSGNO(MsgISawYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUARD, CONDITION_ALWAYS, MSGNO(MsgItSSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIGGS, CONDITION_ALWAYS, MSGNO(MsgHasStrangePowers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_WHO_HATES_PRISON, CONDITION_ALWAYS, MSGNO(MsgIMNotStaying)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_ON_THE_BOAT, CONDITION_ALWAYS, MSGNO(MsgIfTheyDon)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_BLAMING_THE_PARTY, CONDITION_ALWAYS, MSGNO(MsgOhWhatS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIRATE_GUARDING_EOLEOS_SECRET, CONDITION_ALWAYS, MSGNO(MsgIHearSome)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_JAIL_ROPE_LOW_POST, FLAG_JAIL_ROPE_TIED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_JAIL_STAKE, FLAG_JAIL_STAKE_POUNDED, EVENT(AlhafranCave_PoundStake)},
    // The cave's Frost row, repeated in the jail's table.
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CAVE_PUDDLE, FLAG_ALHAFRAN_CAVE_PUDDLE_FROZEN, EVENT(AlhafranCave_FreezePuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CRATE, CONDITION_ALWAYS, EVENT(AlhafranCave_StartMovingCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE, CONDITION_ALWAYS, EVENT(AlhafranCave_FinishMovingCrate)},
    {TRIGGER_TABLE_END},
};

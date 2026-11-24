/// @file
/// Inside the Gabomba Statue at Kibombo, the seven rooms of this overlay: halls
/// of turning cogs and pinions that Felix rides and hops across.
///
/// From the statue grounds the party comes into the hall of pinions, whose
/// turning pinions in the south-west pull Felix through to the north. Above
/// lie the room of corridors with the Bone Armlet's chest, and the room of the
/// flashing cog, where Lash on a rope stops every cog
/// (GabombaStatue_LashRope) so that Felix can cross to the ladder up to
/// Steel's room and the mouse's room, where a mouse shows how to hop from cog
/// to cog (GabombaStatue_MouseShowsTheWay). Back in the room of the flashing
/// cog, Pound on the two stakes beside it turns all the cogs the other way
/// (GabombaStatue_PoundStake); a pink drum in the mouse's room then drops
/// Felix beside the Venus Djinni Steel. The way on leads through the pinions
/// of the room below the small idol's chamber, up to the chamber and to the
/// room of the arrow panels behind the statue's eyes. There the party watches
/// the ceremony's two magical charges stick in a broken channel and mends it
/// (GabombaStatue_FocusEnergies to GabombaStatue_KradenNoticesTiming): Pound
/// on the stake raises the channel's blocks, the arrow panels turn them, and
/// Pound sets each back in, until the blue and the red orb statues' charges
/// reach the statue in the middle at once.

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#include "flags.h"
#include "djinn_ids.h"

/// Both stakes have been Pounded and the cogs turned the other way.
#define FLAG_GABOMBA_COGS_TURNED_BACK 0x8cd
/// The party has watched how the statue's works run.
#define FLAG_GABOMBA_SAW_HOW_IT_WORKS 0x8f6
/// Kraden has said it is when the charges arrive that matters ("Maybe we need
/// to look at when the two magical charges reach the statue in the middle").
#define FLAG_GABOMBA_TIMING_NOTICED 0x9ec
#define FLAG_GABOMBA_FIRST_STAKE_POUNDED 0x8ce  ///< the west stake
#define FLAG_GABOMBA_SECOND_STAKE_POUNDED 0x8cf ///< the east stake
/// The mouse that shows the way across the cogs of its room has been seen.
#define FLAG_GABOMBA_MOUSE_SEEN 0xa6c
/// The two magical charges reached the statue in the middle at different
/// times.
#define FLAG_GABOMBA_CHARGES_REACHED_APART 0x9eb

/// The ladder-top blocks are drawn (GabombaStatue_RestoreLadderTopBlocks).
#define FLAG_LADDER_TOP_BLOCKS_DRAWN FLAG_SCENE_TEMP_00
#define FLAG_GABOMBA_FIRST_ROPE_TIED FLAG_SCENE_TEMP_01  ///< by Rope_Place
#define FLAG_GABOMBA_SECOND_ROPE_TIED FLAG_SCENE_TEMP_02 ///< by Rope_Place
/// A tied rope has stopped the cogs (GabombaStatue_LashRope).
#define FLAG_COGS_STOPPED FLAG_SCENE_TEMP_03
/// The party has fallen in beside Steel, who can then be talked to.
#define FLAG_LANDED_BESIDE_STEEL FLAG_SCENE_TEMP_04
/// The offering below has been heard (GabombaStatue_HearTheOffering).
#define FLAG_OFFERING_HEARD FLAG_SCENE_TEMP_05

/// The mouse that runs ahead across the cogs of its room and shows the way
/// (GabombaStatue_MouseShowsTheWay).
enum { ACTOR_MOUSE = 39 };

/// The arrows room's Kraden, who works out the channel with the party, and
/// Akafubu, who comes up into the statue once the ceremony has worked
/// (GabombaStatue_SeeHowItWorks, GabombaStatue_SawEnergyFlow).
enum { ACTOR_KRADEN = 23, ACTOR_AKAFUBU = 24 };

/// Each room's placed actors. A row of cogs that share one sprite is named by
/// the first of them: turning its animation turns them all. A Lashed rope runs
/// between two posts, its second post riding a cog's tooth. A ladder-top block
/// is a black square (SPRITE_UNSEEN_BLOCK) at the top of a ladder up to the
/// room above, held 36 pixels up in SPRITE_COLOR_HIDDEN, so that nothing of
/// it shows (GabombaStatue_HideActor).
///
/// Drawn or hidden, the ladder-top blocks change nothing on screen:
/// SPRITE_COLOR_HIDDEN draws every pixel of them transparent, and the display
/// has no sprite window they could cut.
enum { ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK = 8 };

/// The Elixir chest's room.
enum {
  ACTOR_FIRST_ROPE_POST_A = 8,
  ACTOR_FIRST_ROPE_POST_B,
  ACTOR_SECOND_ROPE_POST_A,
  ACTOR_SECOND_ROPE_POST_B,
  ACTOR_FIRST_STAKE,
  ACTOR_SECOND_STAKE,
  ACTOR_ELIXIR_GREEN_COGS,
  ACTOR_ELIXIR_BLUE_COGS = 21,
  /// The blue cog that flashes until both stakes are Pounded.
  ACTOR_FLASHING_COG = 26,
  ACTOR_ELIXIR_PINION_A,
  ACTOR_ELIXIR_PINION_B,
  ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK,
  ACTOR_ELIXIR_EAST_LADDER_TOP_BLOCK
};
/// Steel's room.
enum {
  ACTOR_STEEL_GREEN_COGS = 8,
  ACTOR_STEEL_BLUE_COGS = 15,
  ACTOR_STEEL_RED_COGS = 20,
  ACTOR_STEEL_BLUE_PILLARS = 25,
  ACTOR_STEEL_PINK_DRUMS = 29,
  ACTOR_STEEL_ROPE_POST_A = 31,
  ACTOR_STEEL_ROPE_POST_B
};
/// The mouse's room.
enum {
  ACTOR_MOUSE_GREEN_COGS = 8,
  ACTOR_MOUSE_BLUE_COGS = 21,
  ACTOR_MOUSE_RED_COGS = 29,
  ACTOR_MOUSE_BLUE_PILLAR_A = 34,
  ACTOR_MOUSE_BLUE_PILLAR_B,
  ACTOR_MOUSE_PINK_DRUM_A = 37,
  ACTOR_MOUSE_PINK_DRUM_B
};
/// The cogs' room.
enum {
  ACTOR_COGS_ROPE_POST_A = 8,
  ACTOR_COGS_ROPE_POST_B,
  ACTOR_COGS_RED_COGS,
  ACTOR_COGS_PINION_A = 12,
  ACTOR_COGS_PINION_B
};
/// The arrows room: the statue in the middle, where the charges must arrive,
/// the statues holding the blue and the red orb whose charges run along the
/// channel, the stake whose Pound raises the blocks, and the seven loose blocks
/// of the channel ("It looks like the energy charge is supposed to run over
/// these blocks.").
enum {
  ACTOR_MIDDLE_STATUE = 8,
  ACTOR_BLUE_ORB_STATUE,
  ACTOR_RED_ORB_STATUE,
  ACTOR_CHARGE_A, ///< the blue orb's two charges, then the red orb's
  ACTOR_CHARGE_B,
  ACTOR_CHARGE_C,
  ACTOR_CHARGE_D,
  ACTOR_ARROWS_STAKE,
  ACTOR_BLOCK_A,
  ACTOR_BLOCK_B,
  ACTOR_BLOCK_C,
  ACTOR_BLOCK_D,
  ACTOR_BLOCK_E,
  ACTOR_BLOCK_F,
  ACTOR_BLOCK_G
};
/// The hall of pinions.
enum {
  ACTOR_PINION_A = 8,
  ACTOR_PINION_B,
  ACTOR_PINION_C,
  ACTOR_PINION_D,
  ACTOR_PINION_E
};

/// The cell behaviors of the rooms' trigger tables. A hole is the top of a
/// ladder down to the room below, and its number is that of the exit it
/// leaves by. Between two pinions the leader is pulled through to the
/// other side, the cells taking the second behavior while the cogs turn back
/// (GabombaStatue_TurnCogs copies them in). The cells under a cog
/// (GabombaStatue_DrawCogCells) carry the leader with it, and the cells past a
/// cog's ends hop him across to the next.
enum GabombaStatueCellBehavior {
  CELL_BEHAVIOR_FIRST_LADDER_DOWN = 1,
  CELL_BEHAVIOR_SECOND_LADDER_DOWN,
  CELL_BEHAVIOR_THIRD_LADDER_DOWN,
  CELL_BEHAVIOR_PINION_GAP = 10,
  CELL_BEHAVIOR_PINION_GAP_TURNED_BACK,
  CELL_BEHAVIOR_MOUSE = 77, ///< where the mouse pops up
  CELL_BEHAVIOR_GREEN_COG = 80,
  CELL_BEHAVIOR_BLUE_COG,
  CELL_BEHAVIOR_RED_COG,
  CELL_BEHAVIOR_ROLLER, ///< a roller: a blue pillar or a pink drum
  CELL_BEHAVIOR_COG_END,
  /// The cell past a pink drum's south end, which no trigger reads.
  CELL_BEHAVIOR_PINK_DRUM_SOUTH_END
};

/// The arrows room's: the panel with an arrow, and the cells around it that
/// raise it again once the leader steps off.
enum GabombaStatueArrowCellBehavior {
  CELL_BEHAVIOR_AROUND_LEFT_ARROW = 80,
  CELL_BEHAVIOR_LEFT_ARROW,
  CELL_BEHAVIOR_AROUND_RIGHT_ARROW,
  CELL_BEHAVIOR_RIGHT_ARROW
};

/// The triggers this overlay raises: both stakes Pounded, the arrows room's
/// stake Pounded, and the offering's clock run out.
enum {
  RAISED_COGS_TURN_BACK = 200,
  RAISED_STAKE_STANDS_UP = 200,
  RAISED_OFFERING_HEARD = 201
};
#include "graphics/palette_buffers.h"
#include "system/io_reg.h"
#include "field/field.h"
#include "types.h"
#include "system/main_routine_state.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "field/field_sequence.h"
#include "window/item_icon.h"
#include "scene/gabomba_statue.h"
#include "scene/scene.h"
#include "scene_ids.h"
#include "system/system.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/scene_tables.h"
#include "system/frame.h"
#include "field/field_event.h"
#include "macros.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/iwram_call.h"
#include "system/vector3.h"
#include "system/input.h"
#include "system/task.h"
#include "shared/tables.h"
#include "system/heap_release.h"
#include "field/field_effect.h"
#include "graphics/resource.h"
#include "system/affine.h"
#include "graphics/vram_tab.h"
#include "field/call.h"
#include "field/map_grid.h"
#include "field/map.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "text/msg_ids.h"

extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

/// Steel, the Venus Djinni waiting in Steel's room.
enum { ACTOR_STEEL = 33 };

/// Talking to Steel, beside whom Felix lands after riding a pink drum off the
/// mouse's room, starts its battle.
void GabombaStatue_FightSteel(void) {
  Field_RequestDjinnBattle(ACTOR_STEEL, DJINNI_STEEL);
}

/// A member of the party looking about: it turns between its two headings
/// every 10 to 49 frames.
s32 GabombaStatue_LookAround(struct Actor *actor) {
  if (actor->lookTimer != 0) {
    actor->lookTimer--;
  } else {
    actor->lookTimer = (Random_Below(40)) + 10;
    if (actor->lookTurned != 0) {
      actor->lookTurned = 0;
      actor->facing = actor->lookFacing;
    } else {
      actor->lookTurned = 1;
      actor->facing = actor->lookOtherFacing;
    }
  }
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern struct SceneRegion gGabombaStatue_PinionExitRegions[];
extern struct SceneRegion gGabombaStatue_BoneArmletChestExitRegions[];
extern struct SceneRegion gGabombaStatue_ElixirChestExitRegions[];
extern struct SceneRegion gGabombaStatue_SteelExitRegions[];
extern struct SceneRegion gGabombaStatue_MouseExitRegions[];
extern struct SceneRegion gGabombaStatue_CogsExitRegions[];

const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_GABOMBA_STATUE_PINION)
    return gGabombaStatue_PinionExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    return gGabombaStatue_BoneArmletChestExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    return gGabombaStatue_ElixirChestExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_STEEL)
    return gGabombaStatue_SteelExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_MOUSE)
    return gGabombaStatue_MouseExitRegions;
  if (scene == SCENE_GABOMBA_STATUE_COGS)
    return gGabombaStatue_CogsExitRegions;
  return NULL;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The flashing cog's update: its colours swap between two orders every ten
/// frames.
s32 GabombaStatue_FlashCog(struct Actor *actor) {
  u32 frame = gFrameCount;

  if (frame % 10 == 0)
    ((struct Sprite *)actor->sprite)->entries[0]->colorMode =
        frame % 20 / 10 + SPRITE_COLOR_BGR;
  return 1;
}

/// A room's ropes, by the index the rope routines take; the room of the
/// flashing cog has two.
enum { FIRST_ROPE, SECOND_ROPE };

/// The steps of a cog tooth's turn: its phase, kept in the tooth's riseCounter,
/// counts them, two degrees each.
enum { COG_TOOTH_STEPS = 180 };

/// The arrows room's channel: a grid of blocks, eleven columns by eight rows,
/// from the map's cell (19, 11). The statue in the middle, where the charges
/// must arrive, stands on its cell (5, 3), and the two orb statues on (1, 7)
/// and (9, 7).
enum {
  CHANNEL_COLUMNS = 11,
  CHANNEL_ROWS = 8,
  CHANNEL_CELLS = CHANNEL_COLUMNS * CHANNEL_ROWS,
  CHANNEL_ORIGIN_COLUMN = 19,
  CHANNEL_ORIGIN_ROW = 11,
  CHANNEL_MIDDLE_STATUE_COLUMN = 5,
  CHANNEL_MIDDLE_STATUE_ROW = 3,
  CHANNEL_BLUE_STATUE_COLUMN = 1,
  CHANNEL_RED_STATUE_COLUMN = 9,
  CHANNEL_STATUES_ROW = 7
};

/// The way a charge runs along the channel, and the way a path leaves a cell.
enum ChannelHeading {
  HEADING_EAST,
  HEADING_SOUTH,
  HEADING_WEST,
  HEADING_NORTH
};

/// A heading's bit in a set of headings.
#define HEADING_BIT(heading) (1 << (heading))

/// A path's tried headings once every one has been tried.
#define HEADING_ALL_TRIED                                                      \
  (HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_SOUTH) |                    \
   HEADING_BIT(HEADING_WEST) | HEADING_BIT(HEADING_NORTH))

/// The pieces of the channel, by the sides of the cell they join, which are
/// also the rows of the cell tiles the map keeps for them.
enum ChannelPiece {
  PIECE_NONE,
  PIECE_NORTH_SOUTH,
  PIECE_EAST_WEST,
  PIECE_CROSSING,
  PIECE_EAST_SOUTH,
  PIECE_SOUTH_WEST,
  PIECE_WEST_NORTH,
  PIECE_NORTH_EAST,
  PIECE_EAST_SOUTH_AND_WEST_NORTH, ///< two curves
  PIECE_SOUTH_WEST_AND_NORTH_EAST, ///< two curves
  /// No channel: the statues and a block with no piece on it stop a charge.
  PIECE_STOP,
  PIECE_SOCKET ///< the hole a raised block leaves
};

/// The four charges' paths, by the bit of PuzzleCell.state that marks a cell
/// one runs through (GabombaStatue_LayPath takes the bit's number). Each orb
/// statue sends one charge west and one east.
enum {
  PATH_BIT_BLUE_WEST,
  PATH_BIT_RED_EAST,
  PATH_BIT_BLUE_EAST,
  PATH_BIT_RED_WEST
};

/// The bits of PuzzleCell.state: which of the four charges' paths run through
/// the cell.
enum ChannelPath {
  PATH_BLUE_WEST = 1 << PATH_BIT_BLUE_WEST,
  PATH_RED_EAST = 1 << PATH_BIT_RED_EAST,
  PATH_BLUE_EAST = 1 << PATH_BIT_BLUE_EAST,
  PATH_RED_WEST = 1 << PATH_BIT_RED_WEST,
  PATH_BLUE = PATH_BLUE_WEST | PATH_BLUE_EAST,
  PATH_RED = PATH_RED_WEST | PATH_RED_EAST
};

/// What stands on a cell of the channel (PuzzleCell.occupied). The map keeps
/// each piece's cell tiles in a row of their own (enum ChannelPiece), a
/// column for each of these: the column MapGrid_CopyCells copies from is what
/// stands on the cell.
enum {
  BLOCK_NONE,     ///< its piece is part of the floor
  BLOCK_IN_PLACE, ///< a loose block lies in it, to be turned or raised
  BLOCK_RAISED    ///< its block floats over the room, the socket open
};

/// Actor.chargeState. While a charge circles its orb statue before the run,
/// the same byte holds its actor id (GabombaStatue_SendCharge); the end of its
/// burst or shrink script sets CHARGE_STOPPED.
enum {
  CHARGE_STOPPED = 0,
  CHARGE_RUNNING = 1,
  CHARGE_LEAVING = 2, ///< bursting or shrinking away off the channel
  CHARGE_ARRIVED = 7  ///< at the statue in the middle
};

/// The blocks of the channel, one for each of the room's block actors.
enum { CHANNEL_BLOCKS = 7 };

/// The two ways a block turns (GabombaStatue_TurnBlockPiece).
enum { TURN_COUNTERCLOCKWISE, TURN_CLOCKWISE };

/// A charge's step along the channel: where it heads and the piece under it.
#define CHARGE_STEP(heading, piece) (((heading) << 8) + (piece))
extern s32 gGabombaStatue_CogDirection;

/// Where the cog's tooth an actor rides is now: the rope's target hook, so that
/// a rope tied to it follows the turning cog.
void GabombaStatue_GetToothPosition(struct Actor *actor, FIXED *x, FIXED *z) {
  FIXED position[3];
  s32 angle = actor->riseCounter * TRIG_TURN / COG_TOOTH_STEPS;
  s32 turn;

  SET_VECTOR(position, INTtoFIXED(actor->homeX + 16), toFIXED(16),
             INTtoFIXED(actor->homeZ));
  turn = gGabombaStatue_CogDirection * (TRIG_TURN * 3 / 16) + angle;
  FieldVector_AddPolarOffset(13, turn + DEGtoANG(180), position);
  *x = position[0];
  *z = position[2];
}

/// A cog tooth's update: it turns about the cog's centre by the cogs'
/// direction.
s32 GabombaStatue_TurnTooth(struct Actor *actor) {
  u8 *phase = &actor->riseCounter;
  s32 angle = *phase * TRIG_TURN / COG_TOOTH_STEPS;
  s32 next;

  actor->priorityFlags = 0;
  actor->x.fixed = INTtoFIXED(actor->homeX + 16);
  actor->y.fixed = toFIXED(16);
  actor->z.fixed = INTtoFIXED(actor->homeZ);
  FieldVector_AddPolarOffset(13, angle + DEGtoANG(180), &actor->x.fixed);
  next = *phase + gGabombaStatue_CogDirection;
  if (next < 0)
    next = COG_TOOTH_STEPS - 1;
  if (next > COG_TOOTH_STEPS - 1)
    next = 0;
  *phase = next;
  actor->watchedActor = (struct Actor *)GabombaStatue_GetToothPosition;
  return 1;
}

/// A sprite as the depth queue takes it (struct QueuedSprite), its affine mode
/// and matrix written as bytes.
struct SpriteView {
  u8 link[4]; ///< the depth queue's link to the next sprite
  u8 y;
  u8 affineMode : 2;
  u8 modeAndShape : 6; ///< object mode, mosaic, colours and shape
  u8 xLow;
  u8 xHigh : 1;
  u8 matrix : 5;
  u8 size : 2;
  u32 attribute2;
};

/// The channel puzzle's state: its grid of pieces, laid from a random seed, and
/// the blocks and arrows drawn over it.
struct SceneState {
  struct PuzzleCell cells[CHANNEL_ROWS][CHANNEL_COLUMNS];
  s32 seed; ///< the random seed the channel was laid from
  s16 firstBlock;
  s16 endBlock;
  s16 blockTileEntry; ///< the VRAM cache entry of the blocks' pictures
  s8 leftArrowDown;
  s8 rightArrowDown;
  s16 originX;
  s16 originZ;
  s16 blocksRaised;
  u8 unusedAfterBlocksRaised[2];
  s32 blocksAngle;
  struct SpriteView oam[10];
};

extern struct SceneState gSceneState;
extern s8 gGabombaStatue_CogAnimations[];

/// How far a charge circles its orb statue each frame, in 256ths of a turn.
enum { CIRCLE_STEP = 16 };

/// Each frame, keeps an actor circling the actor it watches.
s32 GabombaStatue_CircleOwner(struct Actor *actor) {
  struct Actor *owner = actor->watchedActor;

  if (owner != NULL) {
    u8 *phase = &actor->riseCounter;

    actor->x.fixed = owner->x.fixed;
    actor->y.fixed = owner->y.fixed + toFIXED(16);
    actor->z.fixed = owner->z.fixed;
    actor->facing = *phase << 8;
    FieldVector_AddPolarOffset(6, *phase << 8, &actor->x.fixed);
    *phase += CIRCLE_STEP;
  }
  return 1;
}

extern struct ActorPlacement gGabombaStatue_Placements[];
extern struct ActorPlacement gGabombaStatue_BoneArmletChestActorPlacements[];
extern struct ActorPlacement gGabombaStatue_ElixirChestActorPlacements[];
extern struct ActorPlacement gGabombaStatue_SteelActorPlacements[];
extern struct ActorPlacement gGabombaStatue_MouseActorPlacements[];
extern struct ActorPlacement gGabombaStatue_CogsActorPlacements[];
extern struct ActorPlacement gGabombaStatue_ArrowsActorPlacements[];
extern struct ActorPlacement gGabombaStatue_PinionActorPlacements[];

const u8 *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;
  const u8 *table = gGabombaStatue_Placements;

  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    table = gGabombaStatue_BoneArmletChestActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    table = gGabombaStatue_ElixirChestActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_STEEL)
    table = gGabombaStatue_SteelActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_MOUSE)
    table = gGabombaStatue_MouseActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_COGS)
    table = gGabombaStatue_CogsActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_ARROWS)
    table = gGabombaStatue_ArrowsActorPlacements;
  else if (scene == SCENE_GABOMBA_STATUE_PINION)
    table = gGabombaStatue_PinionActorPlacements;
  return table;
}

extern s8 gGabombaStatue_RopeStopSteps[];

/// The frames the cog's tooth takes to settle as the rope pulls taut.
enum { ROPE_STOP_FRAMES = 25 };

/// Whether a and b are at most limit apart.
#define DIFF_WITHIN(a, b, limit)                                               \
  ((a) - (b) >= 0 ? (a) - (b) <= (limit) : (b) - (a) <= (limit))

/// Lash on the rope by the cogs in the north-west of the room of the flashing
/// cog, between the posts at cells (18, 5) and (20, 5), ties it to a tooth of
/// the blue cog. As it pulls taut the tooth rocks to a halt, and every cog in
/// the room stops, so Felix can cross them to the ladder on the east side.
void GabombaStatue_LashRope(void) {
  struct Actor *actor = Actor_Get(ACTOR_FIRST_ROPE_POST_B);
  s32 direction;
  s32 i;

  if (Rope_GetState(FIRST_ROPE) == 1) { // tied from post to post
    direction = gGabombaStatue_CogDirection;
    GabombaStatue_TurnCogs(0);
    Event_SetFlag(FLAG_COGS_STOPPED);
    gFieldState->psynergy = 0;
    for (i = 0; i < ROPE_STOP_FRAMES; i++) {
      actor->riseCounter += gGabombaStatue_RopeStopSteps[i] * direction;
      System_Wait(1);
    }
  }
}

/// The cog of sprite id within 24 pixels of origin, or null.
struct Actor *GabombaStatue_FindCogUnder(struct Actor *origin, s32 id) {
  s32 i;

  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct Actor *cog = Actor_Get(i);

    if (cog != NULL && cog->spriteStorageKind == ACTOR_SPRITE_ONE &&
        ((struct Sprite *)cog->sprite)->entries[0] != NULL &&
        ((struct Sprite *)cog->sprite)->entries[0]->animationId == id) {
      if (DIFF_WITHIN(cog->x.fixed, origin->x.fixed, toFIXED(24) - 1) &&
          DIFF_WITHIN(cog->z.fixed, origin->z.fixed, toFIXED(24) - 1))
        return cog;
    }
  }
  return NULL;
}

/// As GabombaStatue_FindCogUnder, but around the point 16 pixels ahead of
/// origin in its facing.
struct Actor *GabombaStatue_FindCogAhead(struct Actor *origin, s32 id) {
  FIXED position[3];
  s32 i;

  position[0] = origin->x.fixed;
  position[2] = origin->z.fixed;
  FieldVector_AddPolarOffset(16, origin->facing, position);
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct Actor *cog = Actor_Get(i);

    if (cog != NULL && cog->spriteStorageKind == ACTOR_SPRITE_ONE &&
        ((struct Sprite *)cog->sprite)->entries[0] != NULL &&
        ((struct Sprite *)cog->sprite)->entries[0]->animationId == id) {
      if (DIFF_WITHIN(cog->x.fixed, position[0], toFIXED(24) - 1) &&
          DIFF_WITHIN(cog->z.fixed, position[2], toFIXED(24) - 1))
        return cog;
    }
  }
  return NULL;
}

/// The leader hops off a cog by dx, dz.
void GabombaStatue_HopOffCog(struct Actor *actor, s32 dx, s32 dz,
                             s32 velocity) {
  u8 *flags = &actor->motionFlags;
  u8 saved;

  Import_LeaderStopMoving(actor);
  saved = *flags;
  Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
  System_Wait(6);
  Event_PlaySound(SE_JUMP);
  Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
  actor->speedLimit = toFIXED(3);
  actor->acceleration = toFIXED(2);
  actor->velocityY = velocity;
  *flags &= ACTOR_JUMP_KEEPS_MOTION;
  Actor_SetSpriteFlags(actor, 0);
  FieldActor_SetMoveTargetFX(actor, actor->x.fixed + dx, actor->y.fixed,
                             actor->z.fixed + dz);
  FieldActor_WaitForMove(actor);
  *flags = saved;
  Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
  Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
  System_Wait(3);
}

/// Standing on a green cog turns the leader with it.
void GabombaStatue_TurnWithCog(void) {
  s32 step = gGabombaStatue_CogDirection;

  if (step != 0) {
    s32 turn = step * DEGtoANG(6);
    s32 delta = -turn;
    struct Actor *leader = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *near = GabombaStatue_FindCogUnder(leader, SPRITE_GREEN_COG);

    if (near != NULL) {
      u16 angle = near->facing;

      if (angle == FACING_WEST)
        delta = turn;
    }
    leader->facing += delta;
  }
}

/// Standing on a blue cog carries the leader round with it, unless a wall
/// stops him.
void GabombaStatue_RideBlueCog(void) {
  struct Actor *actor, *cog;
  struct Vector3 test;
  s32 step = -DEGtoANG(2);
  s32 turn;
  FIXED x, z, dx, dz;
  s32 back;
  FIXED tx, tz;

  if (gGabombaStatue_CogDirection == 0)
    return;
  actor = Actor_GetById(Party_GetLeaderActor());
  cog = GabombaStatue_FindCogUnder(actor, SPRITE_BLUE_COG);
  turn = gGabombaStatue_CogDirection * step;
  if (cog != 0) {
    if (cog->y.fixed + toFIXED(16) > actor->y.fixed)
      return;
    if (cog->facing == FACING_WEST)
      turn = -turn;
    dx = actor->x.fixed - cog->x.fixed;
    dz = actor->z.fixed - cog->z.fixed;
    back = -turn;
    {
      s32 s = Math_Sin(back);
      s32 c = Trig_Cos(back);
      x = cog->x.fixed + Iwram_MulFX(c, dx) + Iwram_MulFX(s, dz);
      z = cog->z.fixed - Iwram_MulFX(s, dx) + Iwram_MulFX(c, dz);
    }
    test.x = x + actor->velocityX;
    test.y = cog->y.fixed;
    test.z = z + actor->velocityZ;
    if (MapGrid_CheckCollision(actor, &test) == 0) {
      s32 s, c, angle;
      angle = DEGtoANG(30) - turn;
      s = Math_Sin(angle);
      c = Trig_Cos(angle);
      tx = cog->x.fixed + Iwram_MulFX(c, dx) + Iwram_MulFX(s, dz);
      tz = cog->z.fixed - Iwram_MulFX(s, dx) + Iwram_MulFX(c, dz);
      test.x = tx + actor->velocityX;
      test.z = tz + actor->velocityZ;
      if (MapGrid_CheckCollision(actor, &test) == 0) {
        FIXED s, c;
        s32 angle;
        angle = back - DEGtoANG(30);
        s = Math_Sin(angle);
        c = Trig_Cos(angle);
        tx = cog->x.fixed + Iwram_MulFX(c, dx) + Iwram_MulFX(s, dz);
        tz = cog->z.fixed - Iwram_MulFX(s, dx) + Iwram_MulFX(c, dz);
        test.x = tx + actor->velocityX;
        test.z = tz + actor->velocityZ;
        if (MapGrid_CheckCollision(actor, &test) == 0) {
          actor->x.fixed = x;
          actor->z.fixed = z;
        }
      }
    }
  }
  actor->facing += turn;
}

/// Standing on a red cog carries the leader east or west as it slides, kept on
/// its middle; 20 pixels from its centre he hops off the end.
void GabombaStatue_RideSlidingCog(void) {
  s32 *speed = &gGabombaStatue_CogDirection;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *cog = GabombaStatue_FindCogUnder(actor, SPRITE_RED_COG);
    s32 dx = *speed * toFIXED(0.5);

    if (cog != NULL) {
      FIXED diff, x, cogX;

      if (cog->facing == FACING_WEST)
        dx = -dx;
      if (cog->z.fixed - actor->z.fixed < -toFIXED(2))
        actor->z.fixed = cog->z.fixed + toFIXED(2);
      if (cog->z.fixed - actor->z.fixed > toFIXED(2))
        actor->z.fixed = cog->z.fixed - toFIXED(2);
      x = actor->x.fixed + dx;
      actor->x.fixed = x;
      cogX = cog->x.fixed;
      diff = cogX - x;
      if (diff >= 0 ? diff >= toFIXED(20) : x - cogX >= toFIXED(20)) {
        s32 sign;
        FIXED offset;

        sign = 0;
        offset = actor->x.fixed - cogX;
        if (offset != 0) {
          sign = 1;
          if (offset <= 0)
            sign = -sign;
        }
        GabombaStatue_HopOffCog(actor, sign * toFIXED(12), 0, toFIXED(2));
      }
    }
  }
}

/// The ride on a pink drum of the mouse's room ends at its south end: the
/// leader drops off it to the room below.
void GabombaStatue_EndRide(struct Actor *actor) {
  u32 i;
  struct MainRoutineState *routine;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(6);
  actor->z.fixed = toFIXED(247.25707);
  actor->facing = FACING_SOUTH;
  routine = &gMainRoutineState;
  Actor_SetAttachedEffect(routine->leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAnimation(routine->leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
  FieldEvent_Wait(30);
  Actor_SetAnimation(routine->leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(10);
  for (i = 0; i < 20; i++) {
    actor->scaleX -= FIXED_ONE * 3 / 256;
    actor->scaleY -= FIXED_ONE * 3 / 256;
    actor->y.fixed -= toFIXED(3) / 2;
    System_Wait(1);
    if (i == 10)
      Event_PlaySound(SE_DUST_PUFF);
    // DEAD: the loop ends before frame 20, so the screen is never taken out
    // here; the ROM keeps the test and the transition.
    if (i == 20) {
      gFieldState->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
      Event_FinishScreenTransition();
    }
  }
}

/// Standing on a roller carries the leader north or south as it rolls, a blue
/// pillar one way and a pink drum the other, kept on its middle; 20 pixels
/// from its centre he hops off the end. A drum of the mouse's room carries
/// him off its south end and down to Steel's room.
void GabombaStatue_RideRoller(void) {
  struct Actor *actor, *roller;
  s32 speed;
  FIXED d;
  s32 sign, x;
  FIXED z, rollerZ, offset;

  if (gGabombaStatue_CogDirection == 0)
    return;
  actor = Actor_Get(FieldParty_GetLeaderActor());
  speed = gGabombaStatue_CogDirection * toFIXED(0.5);
  roller = GabombaStatue_FindCogUnder(actor, SPRITE_BLUE_PILLAR);
  if (roller != 0) {
    if (roller->x.fixed - actor->x.fixed < -toFIXED(2))
      actor->x.fixed = roller->x.fixed + toFIXED(2);
    if (roller->x.fixed - actor->x.fixed > toFIXED(2))
      actor->x.fixed = roller->x.fixed - toFIXED(2);
    z = actor->z.fixed + speed;
    actor->z.fixed = z;
    rollerZ = roller->z.fixed;
    d = rollerZ - z;
    if (d >= 0 ? d >= toFIXED(20) : z - rollerZ >= toFIXED(20)) {
      sign = 0;
      offset = actor->z.fixed - rollerZ;
      if (offset != 0) {
        sign = 1;
        if (offset <= 0)
          sign = -sign;
      }
      GabombaStatue_HopOffCog(actor, 0, sign * toFIXED(12), toFIXED(2));
    }
  }
  roller = GabombaStatue_FindCogUnder(actor, SPRITE_PINK_DRUM);
  if (roller == 0)
    return;
  if (roller->x.fixed - actor->x.fixed < -toFIXED(2))
    actor->x.fixed = roller->x.fixed + toFIXED(2);
  if (roller->x.fixed - actor->x.fixed > toFIXED(2))
    actor->x.fixed = roller->x.fixed - toFIXED(2);
  z = actor->z.fixed - speed;
  actor->z.fixed = z;
  rollerZ = roller->z.fixed;
  d = rollerZ - z;
  if (d >= 0 ? d >= toFIXED(20) : z - rollerZ >= toFIXED(20)) {
    sign = 0;
    offset = actor->z.fixed - rollerZ;
    if (offset != 0) {
      sign = 1;
      if (offset <= 0)
        sign = -sign;
    }
    GabombaStatue_HopOffCog(actor, 0, sign * toFIXED(12), toFIXED(2));
  }
  if (gMainRoutineState.next != SCENE_GABOMBA_STATUE_MOUSE)
    return;
  x = FIELD_CELL(actor->x.fixed);
  if (FIELD_CELL(actor->z.fixed) != 15)
    return;
  if (x == 28) {
    GabombaStatue_EndRide(actor);
    FieldEvent_SetExitRequest(
        GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_A);
  } else if (x == 30) {
    GabombaStatue_EndRide(actor);
    FieldEvent_SetExitRequest(
        GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_B);
  }
}

/// Walking east or west off a sliding cog hops the leader to the next.
void GabombaStatue_HopAcrossEastOrWest(void) {
  s32 *speed = &gGabombaStatue_CogDirection;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = GabombaStatue_FindCogAhead(actor, SPRITE_RED_COG);
    s32 dx = *speed * toFIXED(16);

    if (found != NULL) {
      u16 facing = found->facing;

      if (facing == FACING_WEST)
        dx = -dx;
      facing = actor->facing;
      if ((facing == FACING_EAST && dx > 0) ||
          (facing == FACING_WEST && dx < 0))
        GabombaStatue_HopOffCog(actor, dx, 0, toFIXED(4));
    }
  }
}

/// Walking north or south off a roller hops the leader to the next.
void GabombaStatue_HopAcrossNorthOrSouth(void) {
  s32 *speed = &gGabombaStatue_CogDirection;

  if (*speed != 0) {
    struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
    struct Actor *found = GabombaStatue_FindCogAhead(actor, SPRITE_BLUE_PILLAR);
    s32 dz = *speed * toFIXED(16);
    u16 facing;

    if (found == NULL) {
      found = GabombaStatue_FindCogAhead(actor, SPRITE_PINK_DRUM);
      dz = -dz;
      if (found == NULL)
        return;
    }
    facing = actor->facing;
    if ((facing == FACING_SOUTH && dz > 0) ||
        (facing == FACING_NORTH && dz < 0))
      GabombaStatue_HopOffCog(actor, 0, dz, toFIXED(4));
  }
}

/// When Felix walks to the far west of the mouse's room, cell (14, 10), a
/// little mouse pops up, squeaks, and hops from cog to cog, showing the way
/// across. It does so once.
void GabombaStatue_MouseShowsTheWay(struct Actor *actor) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(1);
  Actor_SetPosition(ACTOR_MOUSE, 328, 296);
  FieldEvent_Wait(30);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(15);
  FieldActor_WatchAndSetScript(gMainRoutineState.leaderActorId, ACTOR_MOUSE);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  EventCamera_FollowActor(ACTOR_MOUSE, 1);
  FieldEvent_Wait(30);
  Actor_Get(ACTOR_MOUSE)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetAnimation(ACTOR_MOUSE, 1);
  Actor_WalkToAndWait(ACTOR_MOUSE, 320, 240);
  Actor_WalkToAndWait(ACTOR_MOUSE, 304, 216);
  Actor_WalkToAndWait(ACTOR_MOUSE, 280, 216);
  Actor_WalkToAndWait(ACTOR_MOUSE, 280, 184);
  FieldEvent_Wait(20);
  Actor_FaceActor(ACTOR_MOUSE, gMainRoutineState.leaderActorId, 0);
  FieldEvent_Wait(10);
  Event_PlaySound(SE_VOICE_GIRL_C);
  {
    s32 emote = EMOTE_IN_FRONT | EMOTE_EXCLAMATION, frames = 30;

    Actor_ShowEmote(ACTOR_MOUSE, emote, frames);
  }
  Actor_WalkToAndWait(ACTOR_MOUSE, 344, 184);
  Actor_WalkToAndWait(ACTOR_MOUSE, 344, 200);
  FieldEvent_Wait(2);
  Actor_FaceDirection(ACTOR_MOUSE, FACING_WEST, 0);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(4);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 312, 200);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 312, 168);
  Actor_SetSpeed(ACTOR_MOUSE, 2);
  Actor_WalkToAndWait(ACTOR_MOUSE, 328, 152);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 360, 152);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 2);
  Actor_WalkToAndWait(ACTOR_MOUSE, 360, 184);
  Actor_SetSpeed(ACTOR_MOUSE, 2);
  Actor_WalkToAndWait(ACTOR_MOUSE, 376, 184);
  Actor_WalkToAndWait(ACTOR_MOUSE, 392, 200);
  Actor_WalkToAndWait(ACTOR_MOUSE, 408, 184);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_JUMP);
  Actor_Get(ACTOR_MOUSE)->velocityY = toFIXED(6);
  Actor_SetSpeed(ACTOR_MOUSE, 4);
  Actor_WalkToAndWait(ACTOR_MOUSE, 440, 184);
  Actor_SetPosition(ACTOR_MOUSE, 0, 0);
  Event_SetFlag(FLAG_GABOMBA_MOUSE_SEEN);
  Event_End();
}

void GabombaStatue_SendCharge(s32 id, s32 colorMode, s32 statueId, s32 angle);
void ObjectTable_Snapshot(void);

/// How the charges reached the statue in the middle (GabombaStatue_RunCharges),
/// which decides how the ceremony outside goes on.
enum ChargeArrival {
  CHARGE_ARRIVAL_TOGETHER = 0,   ///< a blue and a red charge at the same moment
  CHARGE_ARRIVAL_BLUE_FIRST = 1, ///< a blue one alone, or before the red
  CHARGE_ARRIVAL_RED_FIRST = 2,  ///< a red one alone, or before the blue
  CHARGE_ARRIVAL_NONE = 3
};

/// Takes charge id off the map, stopped.
void GabombaStatue_ResetCharge(s32 id) {
  Actor_SetPosition(id, 0, 0);
  FieldActor_Stop(id);
  Actor_SetAnimation(id, 1);
}

/// Sends the four charges circling out of the orb statues, the blue statue's
/// green-tinted and the red's red-tinted.
void GabombaStatue_ReleaseCharges(void) {
  GabombaStatue_ResetCharge(ACTOR_CHARGE_A);
  GabombaStatue_ResetCharge(ACTOR_CHARGE_B);
  GabombaStatue_ResetCharge(ACTOR_CHARGE_C);
  GabombaStatue_ResetCharge(ACTOR_CHARGE_D);
  GabombaStatue_SendCharge(ACTOR_CHARGE_A, SPRITE_COLOR_GREEN_TINT,
                           ACTOR_BLUE_ORB_STATUE, 0);
  GabombaStatue_SendCharge(ACTOR_CHARGE_B, SPRITE_COLOR_GREEN_TINT,
                           ACTOR_BLUE_ORB_STATUE, DEGtoANG(180));
  GabombaStatue_SendCharge(ACTOR_CHARGE_C, SPRITE_COLOR_RED_TINT,
                           ACTOR_RED_ORB_STATUE, DEGtoANG(90));
  GabombaStatue_SendCharge(ACTOR_CHARGE_D, SPRITE_COLOR_RED_TINT,
                           ACTOR_RED_ORB_STATUE, TRIG_TURN * 3 / 4);
}

/// The offering's clock has run out: Akafubu offers the jewel below ("O Great
/// Gabomba! Please accept this, our humble gift!"), the orb statues send their
/// charges along the channel, and how they reach the statue in the middle
/// decides how the ceremony outside goes.
void GabombaStatue_HearTheOffering(void) {
  s32 result;

  Event_SetFlag(FLAG_OFFERING_HEARD);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  GabombaStatue_StopClock();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaAcceptOffering);
  // "O Great Gabomba! Please accept this, our humble gift!"
  Event_ShowMessage(ACTOR_AKAFUBU, 0);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 256, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_SHIMMER);
  System_Wait(1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
  FieldActor_SetSpriteColor(Actor_Get(gMainRoutineState.leaderActorId),
                            SPRITE_COLOR_HIDDEN);
  GabombaStatue_ReleaseCharges();
  System_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Event_PlaySound(SE_PSYNERGY_START);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB_STATUE),
                            SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE),
                            SPRITE_COLOR_WHITE);
  System_Wait(8);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB_STATUE),
                            SPRITE_COLOR_NORMAL);
  System_Wait(8);
  Event_PlaySound(MUS_RITUALS);
  result = GabombaStatue_RunCharges(ACTOR_CHARGE_A, 1);
  ObjectTable_Snapshot();
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  switch (result) {
  case CHARGE_ARRIVAL_NONE:
    FieldEvent_SetExitRequest(
        GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_C);
    break;
  case CHARGE_ARRIVAL_BLUE_FIRST:
    FieldEvent_SetExitRequest(
        GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_D);
    break;
  case CHARGE_ARRIVAL_RED_FIRST:
    FieldEvent_SetExitRequest(
        GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_E);
    break;
  case CHARGE_ARRIVAL_TOGETHER:
    FieldEvent_SetExitRequest(
        GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_A_B);
    break;
  }
}

extern struct Trigger gGabombaStatue_Triggers[];
extern struct Trigger gGabombaStatue_BoneArmletChestTriggers[];
extern struct Trigger gGabombaStatue_ElixirChestTriggers[];
extern struct Trigger gGabombaStatue_SteelTriggers[];
extern struct Trigger gGabombaStatue_MouseTriggers[];
extern struct Trigger gGabombaStatue_CogsTriggers[];
extern struct Trigger gGabombaStatue_ArrowsTriggers[];
extern struct Trigger gGabombaStatue_PinionTriggers[];

const u8 *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;
  const u8 *table = gGabombaStatue_Triggers;

  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    table = gGabombaStatue_BoneArmletChestTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    table = gGabombaStatue_ElixirChestTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_STEEL)
    table = gGabombaStatue_SteelTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_MOUSE)
    table = gGabombaStatue_MouseTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_COGS)
    table = gGabombaStatue_CogsTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_ARROWS)
    table = gGabombaStatue_ArrowsTriggers;
  else if (scene == SCENE_GABOMBA_STATUE_PINION)
    table = gGabombaStatue_PinionTriggers;
  return table;
}

/// Felix walks to the top of the ladder at x, z, turns his back and climbs
/// down it, and the screen closes on him as he leaves for the room below by
/// the ladder's exit.
void GabombaStatue_ClimbDownLadder(s32 x, s32 z) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, x, z);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  FieldActor_WaitForScriptEndById(gMainRoutineState.leaderActorId);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId,
                     ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  FieldActor_SetMoveTarget(actor, x, -12, z);
  FieldActor_WaitForMove(actor);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_CLIMB);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(state->stepTrigger);
}

/// A ladder down that Felix takes only when he walks into it: from the squares
/// beside it on row, pushing east or west, or from the one south of it, pushing
/// north. Brushing past it does nothing.
#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(                                  \
    name, row, westColumn, eastColumn, southColumn, southRow, holeX, holeZ)    \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);         \
    s32 x = FIELD_CELL(leader->x.fixed);                                       \
    s32 z = FIELD_CELL(leader->z.fixed);                                       \
                                                                               \
    if (z == row) {                                                            \
      if (x == westColumn && (gInput.held & KEY_RIGHT) == 0)                   \
        return;                                                                \
      if (x == eastColumn && (gInput.held & KEY_LEFT) == 0)                    \
        return;                                                                \
    } else if (x == southColumn && z == southRow &&                            \
               (gInput.held & KEY_UP) == 0) {                                  \
      return;                                                                  \
    }                                                                          \
    GabombaStatue_ClimbDownLadder(holeX, holeZ);                               \
  }

/// Felix climbs down the ladder in the north of the room of corridors, where
/// the Mimic and the Bone Armlet's chest are, at cell (19, 11), back to the
/// hall of pinions below, when he walks into it from the west, the east or the
/// south.
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(
    GabombaStatue_ClimbDownBoneArmletChestLadder, 11, 18, 20, 19, 12, 312, 180)

/// The same for a ladder down with a square to walk in from on each of its
/// four sides.
#define DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(name, column, northRow,      \
                                                  southRow, row, westColumn,   \
                                                  eastColumn, holeX, holeZ)    \
  void name(void) {                                                            \
    struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);         \
    s32 x = FIELD_CELL(leader->x.fixed);                                       \
    s32 z = FIELD_CELL(leader->z.fixed);                                       \
                                                                               \
    if (x == column) {                                                         \
      if (z == northRow && (gInput.held & KEY_DOWN) == 0)                      \
        return;                                                                \
      if (z == southRow && (gInput.held & KEY_UP) == 0)                        \
        return;                                                                \
    } else if (z == row) {                                                     \
      if (x == westColumn && (gInput.held & KEY_RIGHT) == 0)                   \
        return;                                                                \
      if (x == eastColumn && (gInput.held & KEY_LEFT) == 0)                    \
        return;                                                                \
    }                                                                          \
    GabombaStatue_ClimbDownLadder(holeX, holeZ);                               \
  }

/// Felix climbs down the ladder in the north of the room of the rope, the
/// stakes and the flashing cog, at cell (29, 12), back to the room of
/// corridors below, when he walks into it from any side.
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(
    GabombaStatue_ClimbDownElixirChestLadder, 29, 11, 13, 12, 28, 30, 472, 196)

/// Felix climbs down the ladder in the north-east of Steel's room, at cell
/// (33, 7), to the north-east of the room of the rope and the flashing cog
/// below, when he walks into it from the west or the south.
void GabombaStatue_ClimbDownSteelNortheastLadder(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (z == 7) {
    if (x == 32 && (gInput.held & KEY_RIGHT) == 0)
      return;
  } else if (x == 33 && z == 8 && (gInput.held & KEY_UP) == 0) {
    return;
  }
  GabombaStatue_ClimbDownLadder(536, 112);
}

/// Felix climbs down the ladder in the west of Steel's room, at cell (6, 13),
/// to the west of the room of the rope and the flashing cog below, when he
/// walks into it from any side.
void GabombaStatue_ClimbDownSteelWestLadder(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (z == 13) {
    if (x == 5 && (gInput.held & KEY_RIGHT) == 0)
      return;
    if (x == 7 && (gInput.held & KEY_LEFT) == 0)
      return;
  } else if (x == 6) {
    if (z == 12 && (gInput.held & KEY_DOWN) == 0)
      return;
    if (z == 14 && (gInput.held & KEY_UP) == 0)
      return;
  }
  GabombaStatue_ClimbDownLadder(104, 208);
}

/// Felix climbs down the ladder in the east of Steel's room, at cell (42, 13),
/// to the east of the room of the rope and the flashing cog below, when he
/// walks into it from the north, the west or the south.
void GabombaStatue_ClimbDownSteelEastLadder(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (x == 42) {
    if (z == 12 && (gInput.held & KEY_DOWN) == 0)
      return;
    if (z == 14 && (gInput.held & KEY_UP) == 0)
      return;
#ifdef BUGFIX
  } else if (x == 41 && z == 13 && (gInput.held & KEY_RIGHT) == 0) {
#else
    // BUG: the cell tested is east of the hole, where the map has no hole
    // cell; from the one west of it, (41, 13), the leader drops in without
    // pushing east.
  } else if (x == 43 && z == 13 && (gInput.held & KEY_RIGHT) == 0) {
#endif
    return;
  }
  GabombaStatue_ClimbDownLadder(680, 208);
}

/// Felix climbs down the ladder in the north of the mouse's room, at cell
/// (24, 7), to Steel's room below, when he walks into it from the west, the
/// east or the south.
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(GabombaStatue_ClimbDownMouseNorthLadder, 7,
                                     23, 25, 24, 8, 392, 112)

/// Felix climbs down the ladder in the west of the mouse's room, at cell
/// (6, 9), to the west of Steel's room below, when he walks into it from the
/// west, the east or the south.
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD(GabombaStatue_ClimbDownMouseWestLadder, 9,
                                     5, 7, 6, 10, 104, 146)

/// Felix climbs down the ladder in the east of the mouse's room, beside its
/// east blue cog at cell (42, 9), to the east of Steel's room below, when he
/// walks into it from the east or the south.
void GabombaStatue_ClimbDownMouseEastLadder(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (x == 42 && z == 10 && (gInput.held & KEY_UP) == 0)
    return;
  if (x == 43 && z == 9 && (gInput.held & KEY_LEFT) == 0)
    return;
  GabombaStatue_ClimbDownLadder(680, 146);
}

/// Felix climbs down the ladder in the middle of the room below the small
/// idol's chamber, at cell (27, 18), back to the mouse's room, when he walks
/// into it from any side.
DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR(GabombaStatue_ClimbDownCogsLadder, 27,
                                          17, 19, 18, 26, 28, 440, 288)

/// Felix climbs down the ladder in the north-east of the room of the arrow
/// panels, at cell (29, 9), back to the small idol's chamber below, when he
/// walks into it from the west or the south.
void GabombaStatue_ClimbDownArrowsLadder(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);
  struct Actor *leader;
  struct FieldState *state;

  if (x == 28 && z == 9 && !(gInput.held & KEY_RIGHT))
    return;
  if (x == 29 && z == 10 && !(gInput.held & KEY_UP))
    return;
  state = Ram_HeapSlots->fieldState;
  leader = Actor_Get(gMainRoutineState.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 472, 144);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  FieldActor_WaitForScriptEndById(gMainRoutineState.leaderActorId);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineState.leaderActorId), 0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId,
                     ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  FieldActor_SetMoveTarget(leader, 472, 0, 156);
  FieldActor_WaitForMove(leader);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_CLIMB);
  Event_PlaySound(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(state->stepTrigger);
}

/// Felix walks into the gap between two turning pinions, which grind and pull
/// him through to (x, z) on the other side.
#define DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(name, x, z)                       \
  void name(struct Actor *actor) {                                             \
    Event_Start();                                                             \
    FieldEvent_ResolvePendingActions(0);                                       \
    Event_PlaySound(SE_GABOMBA_STATUE_HOLE);                                   \
    Actor_SetSpeedAndAcceleration(gMainRoutineState.leaderActorId, 1.6, 1.6);  \
    Actor_MoveToAndWait(gMainRoutineState.leaderActorId, x, z);                \
    Event_End();                                                               \
  }

/// Walking into the gap between the pinions in the south-west of the hall of
/// pinions, cell (9, 28), Felix is pulled through them to the north.
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_PinionsCarryLeaderNorth, 152,
                                     424)

/// Once the cogs turn the other way, the pinions in the south-west of the hall
/// of pinions throw Felix back out to the south when he walks into their gap,
/// and he lands with a rattle, leaning back.
void GabombaStatue_PinionsThrowLeaderSouth(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(SE_GABOMBA_STATUE_HOLE);
  actor->motionFlags = 0;
  Actor_SetSpeedAndAcceleration(gMainRoutineState.leaderActorId, 1.6, 1.6);
  Actor_MoveToAndWait(gMainRoutineState.leaderActorId, 152, 488);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(8);
  Event_PlaySound(SE_RATTLE);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_LEAN_BACK);
  FieldEvent_Wait(20);
  Event_End();
}

/// Walking into the gap between the pinions in the north of the room of the
/// flashing cog, cell (23, 9), Felix is pulled through them to the south.
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
    GabombaStatue_ElixirPinionsCarryLeaderSouth, 376, 200)

/// Once the cogs turn the other way, the pinions in the south of the room of
/// the flashing cog, cell (24, 22), pull Felix through them to the north.
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(
    GabombaStatue_ElixirPinionsCarryLeaderNorth, 376, 136)

/// Walking into the gap between the pinions in the room below the small
/// idol's chamber, cell (27, 12), Felix is pulled through them to the south.
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_CogsPinionsCarryLeaderSouth,
                                     440, 248)

/// Once the cogs turn the other way, the pinions east of the first in the room
/// below the small idol's chamber, cell (35, 13), pull Felix through them to
/// the north.
DEFINE_GABOMBA_STATUE_WALK_LEADER_TO(GabombaStatue_CogsPinionsCarryLeaderNorth,
                                     440, 184)

/// Draws the room's ladder-top blocks, or hides them.
void GabombaStatue_SetLadderTopBlocksDrawn(s32 show) {
  if (gMainRoutineState.next == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST)
    Actors_SetDrawn(show, ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK, 1);
  else if (gMainRoutineState.next == SCENE_GABOMBA_STATUE_ELIXIR_CHEST)
    Actors_SetDrawn(show, ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK, 2);
}

extern s16 gGabombaStatue_Stakes[];

/// Unused: draws the ladder-top blocks.
UNUSED void GabombaStatue_DrawLadderTopBlocks(void) {
  GabombaStatue_SetLadderTopBlocksDrawn(1);
  Event_SetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
}

/// Hides the ladder-top blocks.
void GabombaStatue_HideLadderTopBlocks(void) {
  GabombaStatue_SetLadderTopBlocksDrawn(0);
  Event_ClearFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
}

#include "shared/actors_set_hidden.c"

/// Pound on one of the two stakes either side of the flashing cog in the south
/// of its room, cells (20, 30) and (24, 30), drives it into the floor. Once
/// both are down, the cogs turn the other way.
void GabombaStatue_PoundStake(s32 a, s32 b) {
  Pound_SinkTarget(gGabombaStatue_Stakes, a, b);
  if (Event_GetFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED) &&
      Event_GetFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED))
    gFieldState->raisedTrigger = RAISED_COGS_TURN_BACK;
}

/// When the second stake by the flashing cog is Pounded, the view pans to the
/// cog between the two stakes, the cogs stop with a clank, the flashing cog
/// bobs up and settles, and every cog starts turning the other way.
void GabombaStatue_CogsTurnBack(void) {
  struct Actor *actor = Actor_Get(ACTOR_FLASHING_COG);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(360, 0, 496, 1);
  FieldCamera_WaitForArrival();
  Event_PlaySound(SE_GABOMBA_STATUE_COGS);
  GabombaStatue_TurnCogs(0);
  FieldEvent_Wait(40);
  Event_PlaySound(SE_LEMURIAN_SHIP_LIFT_FAST);
  actor->motionFlags = 0;
  actor->y.fixed = toFIXED(8);
  FieldEvent_Wait(10);
  actor->y.fixed = 0;
  FieldEvent_Wait(10);
  GabombaStatue_TurnCogs(-1);
  Event_SetFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
  FieldEvent_Wait(20);
  Event_End();
}

void Actor_SetAnimationSpeed(struct Actor *actor, s32 value);
/// Pound on the arrows room's stake raises the channel's blocks.
void GabombaStatue_PoundArrowsStake(s32 value) {
  GabombaStatue_RaiseBlocks(value);
  if (value == 1)
    gFieldState->raisedTrigger = RAISED_STAKE_STANDS_UP;
}

/// Once the blocks have risen, the Pounded stake stands up again to a rising
/// chime.
void GabombaStatue_StakeStandsUp(void) {
  struct Actor *actor = Actor_Get(ACTOR_ARROWS_STAKE);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(20);
  Event_PlaySound(SE_RISING_CHIMES);
  Actor_SelectAnimation(actor, 5);
  actor->scaleY = FIXED_ONE;
  Event_End();
}

/// Each frame: once the offering's clock has run out, the offering is made.
void GabombaStatue_CheckOfferingClock(void) {
  if (GabombaStatue_GetClock() == 0)
    gFieldState->raisedTrigger = RAISED_OFFERING_HEARD;
}

/// Hides actor id (SPRITE_COLOR_HIDDEN) and fixes it in place, sorted behind
/// the others.
void GabombaStatue_HideActor(s32 id) {
  Actor_DisableById(id);
  FieldActor_SetSpriteColor(Actor_Get(id), SPRITE_COLOR_HIDDEN);
  FieldActor_SetSpritePriority(id, 1);
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/// Coming back to the same spot hides the ladder-top blocks again unless they
/// were drawn when the party left; otherwise they are drawn.
void GabombaStatue_RestoreLadderTopBlocks(void) {
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    if (!Event_GetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN))
      GabombaStatue_HideLadderTopBlocks();
  } else {
    Event_SetFlag(FLAG_LADDER_TOP_BLOCKS_DRAWN);
  }
}

/// Gives a room's ladder-top blocks SPRITE_COLOR_HIDDEN and holds them 36
/// pixels up.
#define DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(name, first, count)       \
  void name(void) {                                                            \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i < count; i++) {                                              \
      s32 id = i + first;                                                      \
      struct Actor *actor = Actor_Get(id);                                     \
                                                                               \
      GabombaStatue_HideActor(id);                                             \
      actor->motionFlags = 0;                                                  \
      actor->y.fixed = toFIXED(36);                                            \
    }                                                                          \
    GabombaStatue_RestoreLadderTopBlocks();                                    \
  }

/// The Bone Armlet chest's room, as it is set up: its one ladder-top block.
DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(
    GabombaStatue_HideBoneArmletChestHoleShadow,
    ACTOR_BONE_ARMLET_LADDER_TOP_BLOCK, 1)

/// The Elixir chest's room, as it is set up: its two ladder-top blocks.
DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS(
    GabombaStatue_HideElixirChestHoleShadows,
    ACTOR_ELIXIR_WEST_LADDER_TOP_BLOCK, 2)

/// Each frame: while a menu or Psynergy is up, the cogs and the rope posts
/// stand still.
void GabombaStatue_PauseCogsInMenus(void) {
  s32 step = ANIM_STEP_NORMAL;
  s32 open = 0;
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 i;

  if (Event_GetFlag(FLAG_MENU_OPEN) || state->psynergy != 0) {
    open = 1;
    step = 0;
  }
  i = ACTOR_FIRST_PLACED;
  do {
    {
      struct Actor *actor = Actor_Get(i);

      if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
        struct SpriteAnimation *part =
            ((struct Sprite *)actor->sprite)->entries[0];

        if (part != NULL) {
          switch (part->animationId) {
          case SPRITE_BLUE_COG:
          case SPRITE_GREEN_COG:
          case SPRITE_RED_COG:
          case SPRITE_BLUE_PILLAR:
          case SPRITE_PINK_DRUM:
          case SPRITE_BROWN_COG:
            Actor_SetAnimationSpeed(actor, step);
            break;
          case SPRITE_TIGHTROPE_POST:
            actor->paused = open;
            break;
          }
        }
      }
    }
    i++;
  } while (i < FIELD_TREASURE_ACTOR);
}

/// The map shapes of the cogs' cells, from COG_SHAPE_FIRST: the scene writes
/// gGabombaStatue_CogShapes over the map's own entries there.
enum {
  COG_SHAPE_GREEN_COG = 116, ///< the green cog's own cell
  COG_SHAPE_TOP,             ///< the flat top every other cog stands on
  COG_SHAPE_NORTHWEST,       ///< a blue cog's four corners
  COG_SHAPE_NORTHEAST,
  COG_SHAPE_SOUTHWEST,
  COG_SHAPE_SOUTHEAST,
  COG_SHAPE_WEST, ///< a red cog's sides
  COG_SHAPE_EAST,
  COG_SHAPE_NORTH, ///< a roller's sides
  COG_SHAPE_SOUTH,
  COG_SHAPE_PINK_DRUM_SOUTH_END,
  COG_SHAPE_END, ///< the cells beyond a red cog's or a roller's ends
  COG_SHAPE_FIRST = COG_SHAPE_GREEN_COG,
  COG_SHAPE_COUNT = COG_SHAPE_END + 1 - COG_SHAPE_FIRST
};

/// A cog's cell word: its collision code (the cell behavior), the attribute
/// bits above it and the flags of its cell tiles, ORed into the map's.
#define COG_CELL(behavior, attributes, pictureFlags)                           \
  ((u32)(attributes) << 24 | (behavior) << 16 | (pictureFlags))

/// Gives a cell under a cog its shape on both layers and adds bits to both its
/// shape words.
#define MARK(at, shape, bits)                                                  \
  (cell = (at), layerA[cell] = (shape), layerB[cell] = (shape),                \
   word = &shapesA[cell], *word |= (bits), word = &shapesB[cell],              \
   *word |= (bits))

/// Gives the cells under every cog of the room their shapes and the cell
/// behaviors that carry the leader with it, and fixes the cogs in place.
void GabombaStatue_DrawCogCells(void) {
  u32 *dest = (u32 *)gMapGridShapes + COG_SHAPE_FIRST;
  u32 *src = gGabombaStatue_CogShapes;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  u8 *layerA = map->layers[0].terrainIds;
  u8 *layerB = map->layers[1].terrainIds;
  u32 *shapesA = (u32 *)map->layers[0].cells;
  u32 *shapesB = (u32 *)map->layers[1].cells;
  struct Actor *actor;
  s16 *frames;
  u32 *word;
  u32 i;
  s32 x, z, cell, found;

  for (i = 0; i < COG_SHAPE_COUNT; i++) {
    *dest++ = *src++;
  }
  for (i = ACTOR_FIRST_PLACED; (s32)i < FIELD_TREASURE_ACTOR; i++) {
    actor = Actor_GetById(i);
    if (actor == 0)
      continue;
    if (actor->spriteStorageKind != ACTOR_SPRITE_ONE)
      continue;
    frames = (s16 *)((struct Sprite *)actor->sprite)->entries[0];
    if (frames == 0)
      continue;
    found = 0;
    x = FIELD_CELL(actor->x.fixed);
    z = FIELD_CELL(actor->z.fixed);
    switch (*frames) {
    case SPRITE_GREEN_COG:
      cell = x + (z << 7);
      layerA[cell] = COG_SHAPE_GREEN_COG;
      layerB[cell] = COG_SHAPE_TOP;
      word = &shapesA[cell];
      *word |= COG_CELL(CELL_BEHAVIOR_GREEN_COG, MAP_CELL_JUMP, 0);
      word = &shapesB[cell];
      *word |= COG_CELL(CELL_BEHAVIOR_GREEN_COG, MAP_CELL_JUMP, 0);
      found = 1;
      break;
    case SPRITE_BLUE_COG:
      MARK(x + (z << 7) - 129, COG_SHAPE_NORTHWEST,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
      MARK(x + (z << 7) - 128, COG_SHAPE_TOP,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
      MARK(x + (z << 7) - 127, COG_SHAPE_NORTHEAST,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
      MARK(x + (z << 7) - 1, COG_SHAPE_TOP,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
      MARK(x + (z << 7), COG_SHAPE_TOP, COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
      MARK(x + (z << 7) + 1, COG_SHAPE_TOP,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
      MARK(x + (z << 7) + 127, COG_SHAPE_SOUTHWEST,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
      MARK(x + (z << 7) + 128, COG_SHAPE_TOP,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, MAP_CELL_JUMP, 0));
      MARK(x + (z << 7) + 129, COG_SHAPE_SOUTHEAST,
           COG_CELL(CELL_BEHAVIOR_BLUE_COG, 0, 0));
      found = 1;
      break;
    case SPRITE_RED_COG:
      MARK(x + (z << 7) - 2, COG_SHAPE_END,
           COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
      MARK(x + (z << 7) - 1, COG_SHAPE_WEST,
           COG_CELL(CELL_BEHAVIOR_RED_COG, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7), COG_SHAPE_TOP,
           COG_CELL(CELL_BEHAVIOR_RED_COG, MAP_CELL_JUMP,
                    CELL_DRAWS_ACTOR(1, 1)));
      MARK(x + (z << 7) + 1, COG_SHAPE_EAST,
           COG_CELL(CELL_BEHAVIOR_RED_COG, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7) + 2, COG_SHAPE_END,
           COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
      found = 1;
      break;
    case SPRITE_PINK_DRUM:
      MARK(x + (z << 7) - 256, COG_SHAPE_END,
           COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
      MARK(x + (z << 7) - 128, COG_SHAPE_NORTH,
           COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7), COG_SHAPE_TOP,
           COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7) + 128, COG_SHAPE_SOUTH,
           COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7) + 256, COG_SHAPE_PINK_DRUM_SOUTH_END,
           COG_CELL(CELL_BEHAVIOR_PINK_DRUM_SOUTH_END, MAP_CELL_JUMP, 0));
      found = 1;
      break;
    case SPRITE_BLUE_PILLAR:
      MARK(x + (z << 7) - 256, COG_SHAPE_END,
           COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
      MARK(x + (z << 7) - 128, COG_SHAPE_NORTH,
           COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7), COG_SHAPE_TOP,
           COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7) + 128, COG_SHAPE_SOUTH,
           COG_CELL(CELL_BEHAVIOR_ROLLER, 0, CELL_DRAWS_ACTOR(2, 0)));
      MARK(x + (z << 7) + 256, COG_SHAPE_END,
           COG_CELL(CELL_BEHAVIOR_COG_END, 0, 0));
      found = 1;
      break;
    case SPRITE_BROWN_COG:
      found = 1;
      break;
    }
    if (found) {
      actor->collisionFlags = 0;
      actor->motionFlags = 0;
      actor->priorityFlags = 0;
      Actor_SetSpriteFlags(actor, 0);
    }
  }
  Task_AddFrame(GabombaStatue_PauseCogsInMenus, TASK_PASS_FRAME);
}

/// Blends the room's layers by how far south the camera is: from 232 pixels,
/// a step of the blend's sixteen every 8 pixels.
void GabombaStatue_FadeByDepth(void) {
  s32 blend;
  struct Actor *camera = gFieldState->camera;
  s32 level = camera->z.fixed;

  level = FIXEDtoINT(level - toFIXED(232)) >> 3;
  CLAMP(level, 0, BLEND_WEIGHT_MAX);
  blend = ((BLEND_WEIGHT_MAX - level) << 8) + level;
  REG_BLDALPHA = blend;
}

/// Turns cog id by direction, its animation timers set to value.
void GabombaStatue_SetCogTurn(s32 id, s32 direction, s32 value) {
  struct Actor *actor = Actor_Get(id);
  void *animation = actor->sprite;

  Actor_SelectAnimation(actor,
                        gGabombaStatue_CogAnimations[(direction + 1) & 3]);
  actor->retainSpriteCache = 2;
  Sprite_SetAnimationTimers(animation, value);
}

/// Sets the cogs' direction and turns every cog of the room that way.
void GabombaStatue_TurnCogs(s32 direction) {
  s16 scene;

  gGabombaStatue_CogDirection = direction;
  scene = gMainRoutineState.next;
  if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST) {
    GabombaStatue_SetCogTurn(ACTOR_ELIXIR_GREEN_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_ELIXIR_BLUE_COGS, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_FLASHING_COG, direction, 2);
    if ((u16)(gMainRoutineState.entrance -
              GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST) > 1)
      GabombaStatue_SetCogTurn(ACTOR_ELIXIR_PINION_A, direction, 3);
    if (direction == 0)
      MapGrid_CopyCellAttributes(23, 21, 1, 3, 23, 9);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(24, 21, 1, 3, 23, 9);
  } else if (scene == SCENE_GABOMBA_STATUE_STEEL) {
    GabombaStatue_SetCogTurn(ACTOR_STEEL_GREEN_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_BLUE_COGS, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_RED_COGS, direction, 2);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_BLUE_PILLARS, direction, 3);
    GabombaStatue_SetCogTurn(ACTOR_STEEL_PINK_DRUMS, direction, 4);
  } else if (scene == SCENE_GABOMBA_STATUE_MOUSE) {
    GabombaStatue_SetCogTurn(ACTOR_MOUSE_GREEN_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_MOUSE_BLUE_COGS, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_MOUSE_RED_COGS, direction, 2);
    GabombaStatue_SetCogTurn(ACTOR_MOUSE_BLUE_PILLAR_B, direction, 3);
    GabombaStatue_SetCogTurn(ACTOR_MOUSE_PINK_DRUM_B, direction, 4);
  } else if (scene == SCENE_GABOMBA_STATUE_COGS) {
    GabombaStatue_SetCogTurn(ACTOR_COGS_RED_COGS, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_COGS_PINION_A, direction, 1);
    if (direction == 0)
      MapGrid_CopyCellAttributes(34, 12, 1, 3, 27, 12);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(35, 12, 1, 3, 27, 12);
  } else if (scene == SCENE_GABOMBA_STATUE_ARROWS) {
    Actor_SetAnimation(ACTOR_CHARGE_C, 5);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_CHARGE_C), 0);
  } else if (scene == SCENE_GABOMBA_STATUE_PINION) {
    GabombaStatue_SetCogTurn(ACTOR_PINION_A, direction, 0);
    GabombaStatue_SetCogTurn(ACTOR_PINION_B, direction, 1);
    GabombaStatue_SetCogTurn(ACTOR_PINION_C, direction, 2);
    GabombaStatue_SetCogTurn(ACTOR_PINION_D, direction, 3);
    GabombaStatue_SetCogTurn(ACTOR_PINION_E, direction, 4);
    if (direction == 0)
      MapGrid_CopyCellAttributes(0, 0, 1, 3, 9, 27);
    else if (direction == -1)
      MapGrid_CopyCellAttributes(1, 0, 1, 3, 9, 27);
  }
}

extern s32 gGabombaStatue_ChargeScript[];

/// As field/actor_view.h declares it; that header cannot be included here,
/// since its Actor_StartScript takes the actor field_effect_spawn.c passes as
/// a field effect.
void Actor_SetSpriteColorMode(struct Actor *actor, u32 colorMode);

/// Sets charge id circling the orb statue statueId from the angle given,
/// tinted colorMode.
void GabombaStatue_SendCharge(s32 id, s32 colorMode, s32 statueId, s32 angle) {
  struct Actor *charge = Actor_Get(id);
  struct Actor *target = Actor_Get(statueId);

  Actor_SelectAnimation(charge, 0);
  Actor_StartScript(charge, gGabombaStatue_ChargeScript);
  Actor_SetSpriteFlags(charge, 0);
  charge->motionFlags = 0;
  charge->watchedActor = target;
  // riseCounter is its place on the circle, in 256ths of a turn, and
  // riseEnabled its own actor id until the run sets chargeState there.
  charge->riseCounter = angle / (TRIG_TURN / 256);
  charge->riseEnabled = id;
  Actor_SetSpriteColorMode(charge, colorMode);
}
#undef DIFF_WITHIN
#undef MARK
#undef COG_CELL
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD
#undef DEFINE_GABOMBA_STATUE_STEP_WHEN_HELD_FOUR
#undef DEFINE_GABOMBA_STATUE_WALK_LEADER_TO
#undef DEFINE_GABOMBA_STATUE_HIDE_LADDER_TOP_BLOCKS

#include "../field/field_effect_advance_motion.c"

/// Felix drops off the south end of a pink drum in the mouse's room above and
/// falls into Steel's room, landing with a thump in a ring of dust; he sits a
/// moment as the screen shakes, then stands. One of the two drums lands him
/// beside Steel, who can then be talked to.
void GabombaStatue_LandFromFall(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  u32 i;
  struct EffectOptions options;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  actor->y.fixed = toFIXED(130);
  actor->gravity = toFIXED(1) / 2;
  actor->bounce = 0;
  actor->motionFlags = 0;
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  for (i = 0; i <= DUST_RING_PUFFS; i++) {
    s32 angle;
    FIXED velocity[3];

    angle = i * FACING_STEP;
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random_Below(toFIXED(3) / 4)) - toFIXED(3) / 8;
    velocity[2] += (Random_Below(toFIXED(3) / 8)) - toFIXED(3) / 16;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_LEADER_SIT);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineState.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  actor->gravity = toFIXED(1);
  actor->bounce = FIXED_ONE / 4;
  System_Wait(10);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_End();
}

extern u32 gGabombaStatue_LeaderFocusScript[];
void Actor_PlaceOnActor(s32 id, s32 target);

/// Coming up the hole into the arrows room as the ceremony starts below:
/// Akafubu calls "Focus your energies!", and Kraden calls the party over to
/// watch.
void GabombaStatue_FocusEnergies(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  EventCamera_GetActor()->motionFlags = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  FieldEvent_SetMessageId(MsgGabombaFocusEnergies);
  // "Focus your energies!"
  Event_ShowMessage(ACTOR_AKAFUBU, 0);
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineState.leaderActorId);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineState.leaderActorId);
  System_Wait(1);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetScript(ACTOR_KRADEN, gGabombaStatue_KradenFocusScript);
  System_Wait(10);
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gGabombaStatue_LeaderFocusScript);
  Actor_SetScript(ACTOR_JENNA, gGabombaStatue_JennaFocusScript);
  Actor_SetScript(ACTOR_PIERS, gGabombaStatue_PiersFocusScript);
  FieldActor_WaitForScriptEndById(ACTOR_KRADEN);
  FieldEvent_Wait(20);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(434, CAMERA_KEEP, 286, 1);
  Actor_SetSpeed(ACTOR_KRADEN, 1.2);
  Actor_RunScriptAndWait(ACTOR_KRADEN, gGabombaStatue_KradenPointsScript);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHEAST);
  // "Everyone! Look over here!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(434, CAMERA_KEEP, 168, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 40);
  Actor_Get(ACTOR_PIERS)->facing = FACING_SOUTH;
  Actor_Get(gMainRoutineState.leaderActorId)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_SHEBA)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_JENNA)->facing = FACING_SOUTH;
  Actor_Get(ACTOR_PIERS)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(gMainRoutineState.leaderActorId)->steeringFlags &=
      ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_SHEBA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_JENNA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkTo(ACTOR_PIERS, 400, 272);
  Actor_WalkTo(gMainRoutineState.leaderActorId, 420, 272);
  Actor_WalkTo(ACTOR_SHEBA, 444, 272);
  Actor_WalkTo(ACTOR_JENNA, 468, 272);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(80);
  System_Wait(80);
  FieldEvent_SetExitRequest(
      GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_B);
}

/// The party watches the orb statues' charges run along the broken channel,
/// and is sent back down as the ceremony fails.
void GabombaStatue_WatchTheChargesStick(void) {
  s32 south = FACING_SOUTH;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_SetPositionAndFacing(gMainRoutineState.leaderActorId, toFIXED(348),
                             toFIXED(312), south);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(330), toFIXED(312), south);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(432), toFIXED(312),
                             FACING_SOUTH_SOUTHEAST);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(448), toFIXED(312), south);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(468), toFIXED(312), south);
  System_Wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  Event_PlaySound(SE_SHIMMER);
  System_Wait(1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(RGB_WHITE, 0);
  PaletteFade_Interpolate(40);
  System_Wait(80);
  GabombaStatue_ReleaseCharges();
  System_Wait(1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_Interpolate(10);
  System_Wait(20);
  Actor_ShowEmote(gMainRoutineState.leaderActorId,
                  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 60);
  Event_PlaySound(SE_PSYNERGY_START);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB_STATUE),
                            SPRITE_COLOR_WHITE);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE),
                            SPRITE_COLOR_WHITE);
  System_Wait(8);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_BLUE_ORB_STATUE),
                            SPRITE_COLOR_NORMAL);
  System_Wait(8);
  Actor_Get(gMainRoutineState.leaderActorId)->lookTimer =
      (Random_Below(40)) + 10;
  Actor_Get(ACTOR_PIERS)->lookTimer = (Random_Below(40)) + 10;
  Actor_Get(ACTOR_JENNA)->lookTimer = (Random_Below(40)) + 10;
  Actor_Get(ACTOR_SHEBA)->lookTimer = (Random_Below(40)) + 10;
  Actor_Get(ACTOR_KRADEN)->lookTimer = (Random_Below(40)) + 10;
  Actor_SetScript(gMainRoutineState.leaderActorId,
                  gGabombaStatue_LookNorthAndEastScript);
  Actor_SetScript(ACTOR_PIERS, gGabombaStatue_LookNorthAndEastScript);
  Actor_SetScript(ACTOR_JENNA, gGabombaStatue_LookNorthAndWestScript);
  Actor_SetScript(ACTOR_SHEBA, gGabombaStatue_LookNorthAndWestScript);
  Actor_SetScript(ACTOR_KRADEN, gGabombaStatue_LookNorthAndWestScript);
  Event_PlaySound(MUS_RITUALS);
  GabombaStatue_RunCharges(ACTOR_CHARGE_A, 0);
  FieldEvent_Wait(80);
  Event_SetFlag(FLAG_GABOMBA_MUSIC_SILENCED);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(
      GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_B);
}

extern u32 gGabombaStatue_ChannelSeeds[16];

/// Kraden sees how the statue works, and the party agrees to mend the channel
/// for Akafubu's ceremony.
void GabombaStatue_SeeHowItWorks(void) {
  s32 piers = SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetPositionAndFacing(gMainRoutineState.leaderActorId, toFIXED(348),
                             toFIXED(312), FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(330), toFIXED(312),
                             FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(432), toFIXED(312),
                             FACING_NORTH_NORTHWEST);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(448), toFIXED(312),
                             FACING_NORTH);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(468), toFIXED(312),
                             FACING_NORTH);
  System_Wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(40);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldEvent_SetMessageId(MsgGabombaUnderstandsChannel);
  // "Ah! So that's how it works!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  EventCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 292, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  // "He's been failing because the conduit that channels his Psynergy is..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  // "Should we reconnect the conduit so he can complete the ceremony?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNA,
                    0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineState.leaderActorId,
                                            0) == 0) {
    FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Actor_SetSpeed(ACTOR_PIERS, 2);
    Actor_WalkToAndWait(ACTOR_PIERS, 340, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 354, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 364, 308);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
    // "I won't help them, Felix! They're the ones who stole my orb from the..."
    Event_ShowMessage(
        SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
    gFieldState->message++;
  } else {
    gFieldState->message++;
    Actor_SetSpeed(ACTOR_PIERS, 2);
    Actor_WalkToAndWait(ACTOR_PIERS, 340, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 354, 296);
    Actor_WalkToAndWait(ACTOR_PIERS, 364, 308);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_NORTHEAST, 0);
    Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
    Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
    // "I agree, Felix! Why should we help this pack of thieves?"
    Event_ShowMessage(piers, 0);
  }
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "Piers, I understand that you're angry at them, but it really is in..."
  Event_ShowMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_EAST, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Think about it, Piers... Akafubu stole the orb solely to perform..."
  Event_ShowMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  // "So, what do you think he intends to do with the orb?"
  Event_ShowMessage(
      SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  // "He means to give it to the Great Gabomba..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Oh! So...if his ceremony succeeds, the orb will be taken into the..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "And we're INSIDE the statue! If the ceremony succeeds, we can go..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 10);
  // "I...don't want to help that Akafubu, but..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_WalkToAndWait(ACTOR_SHEBA, 440, 296);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  // "I don't like the idea of helping him any more than you do, but if we..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 80);
  FieldActor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_EAST);
  // "You're right, Sheba. Let's see if we can't help him finish his..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_NOD);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  // "It looks like the energy charge is supposed to run over these blocks."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 20);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_NORTH_NORTHWEST);
  // "If we can just move these around a little, that might be enough to..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 176, 1);
  FieldCamera_WaitForArrival();
  // "Ah! There we go!"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "These panels have arrows on them... What do you think it all means?"
  Event_ShowMessage(ACTOR_SHEBA, 0);
  // "Look at the directions they point... I'll bet these will rotate the..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  // "But how do we set the blocks back in place?"
  Event_ShowMessage(ACTOR_PIERS, 0);
  // "Just Pound them in!"
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  FieldCamera_MoveTo(392, CAMERA_KEEP, 292, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_NOD);
  // "Won't that do the trick?"
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
  // "We must watch the path his Psynergy takes and move the blocks..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetScript(ACTOR_PIERS, gGabombaStatue_LookAtChannelScript);
  Actor_SetScript(ACTOR_KRADEN, gGabombaStatue_LookAtChannelScript);
  Actor_SetScript(ACTOR_SHEBA, gGabombaStatue_LookAtChannelScript);
  Actor_RunScriptAndWait(ACTOR_JENNA, gGabombaStatue_LookAtChannelScript);
  Event_SetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  ObjectTable_Snapshot();
  FieldEvent_SetExitRequest(
      GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_SOUTH);
}

/// Once the Gabomba has responded, Akafubu comes up into the statue, and the
/// party tells him how it mended the channel and where the orb went.
void GabombaStatue_SawEnergyFlow(void) {
  s32 index = GabombaStatue_GetPuzzleSeed();
  FIXED x;
  FIXED z;
  struct Actor *hand;
  struct Actor *actor;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  GabombaStatue_SetUpChannelPuzzle(CHANNEL_ORIGIN_COLUMN, CHANNEL_ORIGIN_ROW,
                                   gGabombaStatue_ChannelSeeds[index],
                                   ACTOR_BLOCK_A);
  Actor_SetAnimation(ACTOR_ARROWS_STAKE, 5);
  Actor_SetPosition(ACTOR_CHARGE_A, 0, 0);
  Actor_SetPosition(ACTOR_CHARGE_B, 0, 0);
  Actor_SetPosition(ACTOR_CHARGE_C, 0, 0);
  Actor_SetPosition(ACTOR_CHARGE_D, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_A, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_B, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_C, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_D, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_E, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_F, 0, 0);
  Actor_SetPosition(ACTOR_BLOCK_G, 0, 0);
  System_Wait(1);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 182, 0);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 0, 0);
  Actor_SetPositionAndFacing(ACTOR_JENNA, toFIXED(472), toFIXED(216),
                             FACING_SOUTH);
  Actor_SetPositionAndFacing(ACTOR_AKAFUBU, toFIXED(472), toFIXED(200),
                             FACING_SOUTH_SOUTHEAST);
  System_Wait(1);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 312, 1);
  Actor_SetSpeed(ACTOR_JENNA, 1);
  Actor_SetSpeed(ACTOR_AKAFUBU, 1);
  Actor_SetScript(ACTOR_JENNA, gGabombaStatue_JennaWatchesScript);
  Actor_RunScriptAndWait(ACTOR_AKAFUBU, gGabombaStatue_AkafubuComesUpScript);
  FieldEvent_SetMessageId(MsgGabombaSawEnergyFlow);
  // "Yeah, we know... We saw your magic energy flowing past us."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHWEST, 0);
  Actor_WalkToAndWait(ACTOR_JENNA, 440, 296);
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
  // "See how this channel runs along the floor?"
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  FieldCamera_MoveTo(414, CAMERA_KEEP, 272, 1);
  FieldCamera_WaitForArrival();
  // "Here, look... It runs all the way over there!"
  Event_ShowMessage(ACTOR_JENNA, 0);
  GabombaStatue_FindBrokenCell(&x, &z);
  x = FIELD_CELL_ORIGIN(x) + FIELD_CELL_CENTER(CHANNEL_ORIGIN_COLUMN);
  z = FIELD_CELL_ORIGIN(z) + FIELD_CELL_CENTER(CHANNEL_ORIGIN_ROW);
  hand = FieldActor_Create(SPRITE_POINTING_HAND, x, 0, z);
  hand->motionFlags |= ACTOR_FLOATS;
  Actor_SetSpriteFlags(hand, 0);
  Actor_SelectAnimation(hand, 1);
  System_Wait(1);
  actor = Actor_Get(ACTOR_JENNA);
  actor->facing =
      Trig_Atan(hand->z.fixed - actor->z.fixed, hand->x.fixed - actor->x.fixed);
  FieldCamera_MoveToFX(hand->x.fixed, -1, hand->z.fixed, 1);
  FieldCamera_WaitForArrival();
  // "But the thing is, the circuit was broken right there."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Release(hand);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 304, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
  // "So we completed the circuit so that your magic energy could go where..."
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAttachedEffect(ACTOR_AKAFUBU,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  // "But...if you truly did this, then why? Why did you help me?"
  Event_ShowMessage(ACTOR_AKAFUBU, 0);
  Actor_SetPositionAndFacing(gMainRoutineState.leaderActorId, toFIXED(472),
                             toFIXED(204), FACING_SOUTH);
  Actor_SetPositionAndFacing(ACTOR_PIERS, toFIXED(440), toFIXED(204),
                             FACING_SOUTH);
  Actor_SetPositionAndFacing(ACTOR_KRADEN, toFIXED(456), toFIXED(204),
                             FACING_SOUTH_SOUTHEAST);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, toFIXED(488), toFIXED(204),
                             FACING_SOUTH);
  System_Wait(1);
  // "We were sort of hoping that we could take the orb if your ceremony..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_AKAFUBU, FACING_NORTH_NORTHEAST, 0);
  FieldCamera_MoveTo(440, CAMERA_KEEP, 264, 1);
  FieldCamera_WaitForArrival();
  FieldActor_Jump(ACTOR_AKAFUBU, 4, 10);
  // "Ah, so you have taken the orb? Then begone! You have no further..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_AKAFUBU, 0);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  // "But...we didn't get the orb."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_PIERS, 0);
  Actor_ShowEmote(ACTOR_AKAFUBU, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "It rolled through this groove and right through this hole."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "We saw a light through there, so we suspect there must be another..."
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_BELOW | ACTOR_KRADEN,
                    0);
  FieldEvent_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_ApplyTarget(0, 0);
  PaletteFade_Interpolate(60);
  Event_ClearFlag(FLAG_KEEP_PARTY_POSITION);
  System_Wait(60);
  FieldEvent_SetExitRequest(
      GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_WEST);
}

/// Back from a failed ceremony whose charges reached the statue in the middle
/// apart, Kraden notices it is their timing that matters, and the clock starts
/// again.
void GabombaStatue_KradenNoticesTiming(void) {
  GabombaStatue_StopClock();
  Actor_PlaceOnActor(ACTOR_KRADEN, gMainRoutineState.leaderActorId);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_KRADEN), SPRITE_COLOR_HIDDEN);
  System_Wait(10);
  FieldEvent_SetMessageId(MsgGabombaChannelTiming);
  // "It looks like just fixing the channel should do the trick... Maybe..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  System_Wait(1);
  Event_SetFlag(FLAG_GABOMBA_TIMING_NOTICED);
  GabombaStatue_RestartClock();
}

/// Sets up whichever room of the statue is being entered: its cogs turning,
/// stopped by the rope or turned the other way by the stakes, then the room's
/// own things.
s32 Scene_Initialize(void) {
  s16 scene;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  if (gMainRoutineState.next != SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
    if (Event_GetFlag(FLAG_COGS_STOPPED))
      GabombaStatue_TurnCogs(0);
    else if (Event_GetFlag(FLAG_GABOMBA_COGS_TURNED_BACK))
      GabombaStatue_TurnCogs(-1);
    else
      GabombaStatue_TurnCogs(1);
    GabombaStatue_DrawCogCells();
  }
  Actor_Get(FieldParty_GetLeaderActor())->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  scene = gMainRoutineState.next;
  if (scene == SCENE_GABOMBA_STATUE_BONE_ARMLET_CHEST) {
    GabombaStatue_InitializeBoneArmletChest();
  } else if (scene == SCENE_GABOMBA_STATUE_ELIXIR_CHEST) {
    MapGrid_CopyCellAttributes(15, 68, 8, 3, 15, 4);
    GabombaStatue_InitializeElixirChest();
  } else if (scene == SCENE_GABOMBA_STATUE_STEEL) {
    GabombaStatue_InitializeSteel();
  } else if (scene == SCENE_GABOMBA_STATUE_COGS) {
    GabombaStatue_InitializeCogs();
  } else if (scene == SCENE_GABOMBA_STATUE_ARROWS) {
    GabombaStatue_InitializeArrows();
  } else if (scene == SCENE_GABOMBA_STATUE_PINION) {
    GabombaStatue_InitializePinion();
  }
  return 0;
}

/// Coming into the hall of pinions from outside sets the cogs and the stakes
/// back as they were.
s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.next == SCENE_GABOMBA_STATUE_PINION &&
      gMainRoutineState.entrance ==
          GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Event_ClearFlag(FLAG_GABOMBA_COGS_TURNED_BACK);
    Event_ClearFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED);
    Event_ClearFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED);
  }
  return 0;
}

/// Sets up the Bone Armlet chest's room.
void GabombaStatue_InitializeBoneArmletChest(void) {
  GabombaStatue_HideBoneArmletChestHoleShadow();
}

/// Places rope index between two posts, tied when its flag is set, else coiled:
/// the event inline over the scene's import of Rope_Place, as event_inline.h
/// writes them.
static inline void FieldRope_Place(s32 index, s32 flag, s32 firstPost,
                                   s32 secondPost) {
  GabombaStatue_PlaceRope(index, flag, firstPost, secondPost);
}

/// Sets up the Elixir chest's room: its pinions let go of the leader coming
/// in from the west or the east of Steel's room, the stakes, the flashing
/// cog raised until both are Pounded, and the two ropes.
void GabombaStatue_InitializeElixirChest(void) {
  if ((u16)(gMainRoutineState.entrance -
            GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST) <= 1) {
    Actor_SetScript(ACTOR_ELIXIR_PINION_A, gGabombaStatue_PinionReleaseScript);
    Actor_SetScript(ACTOR_ELIXIR_PINION_B, gGabombaStatue_PinionReleaseScript);
    MapGrid_CopyCellAttributes(33, 24, 3, 3, 27, 29);
  }
  GabombaStatue_PlaceSwitchActors(gGabombaStatue_Stakes);
  if (!Event_GetFlag(FLAG_GABOMBA_FIRST_STAKE_POUNDED) ||
      !Event_GetFlag(FLAG_GABOMBA_SECOND_STAKE_POUNDED))
    Actor_Get(ACTOR_FLASHING_COG)->y.fixed = toFIXED(16);
  Rope_Initialize();
  FieldRope_Place(FIRST_ROPE, FLAG_GABOMBA_FIRST_ROPE_TIED,
                  ACTOR_FIRST_ROPE_POST_A, ACTOR_FIRST_ROPE_POST_B);
  FieldRope_Place(SECOND_ROPE, FLAG_GABOMBA_SECOND_ROPE_TIED,
                  ACTOR_SECOND_ROPE_POST_A, ACTOR_SECOND_ROPE_POST_B);
  GabombaStatue_HideElixirChestHoleShadows();
}

/// Sets up Steel's room: its rope, and the fall in from the mouse's room.
void GabombaStatue_InitializeSteel(void) {
  Rope_Initialize();
  FieldRope_Place(FIRST_ROPE, FLAG_GABOMBA_FIRST_ROPE_TIED,
                  ACTOR_STEEL_ROPE_POST_A, ACTOR_STEEL_ROPE_POST_B);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 entrance = (u16)gMainRoutineState.entrance;

    if ((u16)(entrance -
              GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A) <= 1)
      GabombaStatue_LandFromFall();
    if (gMainRoutineState.entrance ==
        GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A)
      Event_SetFlag(FLAG_LANDED_BESIDE_STEEL);
  }
}

/// Sets up the room below the small idol's chamber: its rope, coiled.
void GabombaStatue_InitializeCogs(void) {
  Rope_Initialize();
  Value3((s32 (*)())GabombaStatue_CoilRope, FIRST_ROPE, ACTOR_COGS_ROPE_POST_A,
         ACTOR_COGS_ROPE_POST_B);
}

/// Which of the channel's seeds this game lays it from: the letters of the
/// leader's name added up.
s32 GabombaStatue_GetPuzzleSeed(void) {
  struct Unit *leader = Unit_Get(gMainRoutineState.leaderActorId);
  s32 sum = 0;
  u32 i;

  for (i = 0; i < UNIT_NAME_SIZE - 1; i++)
    sum += leader->name[i];
  return sum & (ARRAY_COUNT(gGabombaStatue_ChannelSeeds) - 1);
}

void GabombaStatue_CheckOfferingClock();

/// Sets up the arrows room: its orb statues, its charges and the channel.
void GabombaStatue_InitializeArrows(void) {
  struct Actor *leader;
  s32 seed;

  FieldActor_SetSpriteColor(Actor_Get(ACTOR_RED_ORB_STATUE), SPRITE_COLOR_BGR);
  Task_AddFrame((s32)GabombaStatue_FadeByDepth, TASK_PASS_FRAME);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_A), SPRITE_COLOR_GREEN_TINT);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_B), SPRITE_COLOR_GREEN_TINT);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_C), SPRITE_COLOR_RED_TINT);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_CHARGE_D), SPRITE_COLOR_RED_TINT);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_A), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_B), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_C), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_D), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_E), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_F), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BLOCK_G), 0);
  if (Event_GetFlag(FLAG_GABOMBA_RESPONDED) &&
      !Event_GetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
    GabombaStatue_SawEnergyFlow();
    return;
  }
  if (Event_GetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
    leader = Actor_Get(gMainRoutineState.leaderActorId);
    Actor_SetPosition(ACTOR_CHARGE_A, 0, 0);
    Actor_SetPosition(ACTOR_CHARGE_B, 0, 0);
    Actor_SetPosition(ACTOR_CHARGE_C, 0, 0);
    Actor_SetPosition(ACTOR_CHARGE_D, 0, 0);
    GabombaStatue_ResetClock();
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldCamera_MoveToFX(leader->x.fixed, -1, leader->z.fixed, 0);
    MapGrid_CenterOnFollowedPoint();
    System_Wait(1);
    Event_End();
  }
  if (Event_GetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS) &&
      !Event_GetFlag(FLAG_AKAFUBU_OFFERED_HELP)) {
    GabombaStatue_StartClock(0);
    Task_AddFrame((s32)GabombaStatue_CheckOfferingClock, TASK_PASS_FRAME);
  }
  seed = GabombaStatue_GetPuzzleSeed();
  GabombaStatue_SetUpChannelPuzzle(CHANNEL_ORIGIN_COLUMN, CHANNEL_ORIGIN_ROW,
                                   gGabombaStatue_ChannelSeeds[seed],
                                   ACTOR_BLOCK_A);
  Actor_SetAnimation(ACTOR_ARROWS_STAKE, 5);
  if (gMainRoutineState.entrance ==
      GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_A) {
    GabombaStatue_WatchTheChargesStick();
    return;
  }
  if (!Event_GetFlag(FLAG_GABOMBA_SAW_HOW_IT_WORKS)) {
    if (gMainRoutineState.entrance ==
        GABOMBA_STATUE_ARROWS_FROM_GABOMBA_STATUE_CHAMBER_IDOL_EAST)
      GabombaStatue_FocusEnergies();
    else if (
        gMainRoutineState.entrance ==
        GABOMBA_STATUE_ARROWS_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_A)
      GabombaStatue_SeeHowItWorks();
  }
  if (Event_GetFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY)) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    Event_ClearFlag(FLAG_GABOMBA_BACK_FROM_CEREMONY);
    if (Event_GetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART) &&
        !Event_GetFlag(FLAG_GABOMBA_TIMING_NOTICED))
      GabombaStatue_KradenNoticesTiming();
    Event_ClearFlag(FLAG_SHOW_AREA_NAME);
    Event_End();
  }
}

/// Sets up the hall of pinions; coming in from outside, Retreat leads back to
/// it.
void GabombaStatue_InitializePinion(void) {
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (gMainRoutineState.entrance ==
      GABOMBA_STATUE_PINION_FROM_KIBOMBO_STATUE_GROUNDS) {
    Event_SetFlag(FLAG_GABOMBA_RETREAT_TO_PINION);
    Event_ClearFlag(FLAG_GABOMBA_RETREAT_TO_IDOL);
  }
}

static __inline__ void *Mem_Alloc(s32 size) { return Heap_Alloc(size); }

/// Loads the blocks' pictures into the VRAM cache.
void GabombaStatue_LoadBlockPictures(void) {
  struct SceneState *state = &gSceneState;
  void *buffer = Mem_Alloc(3072);
  s32 handle;

  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  handle = VramCache_FindFreeEntry();
  VramCache_Load(handle, 3072, buffer);
  state->blockTileEntry = handle;
  Heap_Release(buffer);
}

/// Whether channel cell (x, z) is taken: blocked, holding a block, or a stop;
/// 0 outside the grid.
s32 GabombaStatue_IsCellTaken(u32 x, u32 z) {
  u8 *cells = ((u8 *)&gSceneState);

  if (x >= CHANNEL_COLUMNS || z >= CHANNEL_ROWS)
    return 0;
  {
    struct PuzzleCell *cell =
        (struct PuzzleCell *)(cells + (z * CHANNEL_COLUMNS + x) *
                                          sizeof(struct PuzzleCell));
    s32 code;

    // The same byte, read through its address.
    if (*(u8 *)&cell->occupied != BLOCK_NONE || cell->blocked != 0)
      return 1;
    code = cell->kind;
    code ^= PIECE_STOP;
    return 1 - NONZERO(code);
  }
}

/// Whether a cell beside channel cell (x, z) is taken.
s32 GabombaStatue_HasTakenNeighbor(s32 x, s32 z) {
  s32 found;

  if (GabombaStatue_IsCellTaken(x, z - 1) ||
      GabombaStatue_IsCellTaken(x, z + 1) ||
      GabombaStatue_IsCellTaken(x - 1, z))
    return 1;
  found = GabombaStatue_IsCellTaken(x + 1, z);
  return NONZERO(found);
}

/// Redraws the channel as the party left it: a raised block's socket open, the
/// other blocks in place, and once the Gabomba has responded every block set
/// back in.
void GabombaStatue_RedrawChannel(void) {
  struct SceneState *state = &gSceneState;
  u32 row;
  u32 col;

  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      struct PuzzleCell *cell = state->cells[0] + (row * CHANNEL_COLUMNS + col);
      s32 occupied = cell->occupied;

      if (Event_GetFlag(FLAG_GABOMBA_RESPONDED) &&
          cell->occupied == BLOCK_RAISED) {
        cell->occupied = BLOCK_IN_PLACE;
        occupied = BLOCK_IN_PLACE;
      }
      if (occupied == BLOCK_RAISED)
        MapGrid_CopyCells(BLOCK_NONE, PIECE_SOCKET, 1, 1, state->originX + col,
                          state->originZ + row);
      else if (occupied == BLOCK_IN_PLACE)
        MapGrid_CopyCells(BLOCK_IN_PLACE, state->cells[row][col].kind, 1, 1,
                          state->originX + col, state->originZ + row);
      else
        MapGrid_CopyCells(occupied, state->cells[row][col].kind, 1, 1,
                          state->originX + col, state->originZ + row);
    }
}

s32 GabombaStatue_TurnBlockPiece(s32 x, s32 z, s32 direction);

/// The cells of the charges' paths to the statue in the middle: the two sent
/// east run EAST_PATH_CELLS and the two sent west WEST_PATH_CELLS, so that a
/// blue and a red charge arrive together. The paths from the statues' inner
/// sides meet after PATH_TO_CROSSING_CELLS on the crossing's own cell.
enum { EAST_PATH_CELLS = 17, WEST_PATH_CELLS = 19, PATH_TO_CROSSING_CELLS = 4 };

/// Lays the channel from seed, trying the seeds after it until each charge's
/// path reaches the statue in the middle as the puzzle needs, then breaks the
/// circuit and loosens the seven blocks, turned at random. Returns the seed it
/// was laid from.
s32 GabombaStatue_LayChannel(s32 seed) {
  struct SceneState *state = &gSceneState;
  s32 row, col, n, blueShared, redShared, crossed, tries, blocksLeft, westward;
  s32 piece, paths;
  struct PuzzleCell *cell;

  state->seed = seed - 1;
retry:
  state->seed += 1;
  gRandomSeed = state->seed;
  Iwram_ClearWordsEntry(state, sizeof(state->cells));
  state->cells[CHANNEL_MIDDLE_STATUE_ROW][CHANNEL_MIDDLE_STATUE_COLUMN].kind =
      PIECE_STOP;
  state->cells[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN].kind =
      PIECE_STOP;
  state->cells[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN].kind =
      PIECE_STOP;
  state->cells[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN - 1].blocked = 1;
  state->cells[CHANNEL_STATUES_ROW][CHANNEL_BLUE_STATUE_COLUMN + 1].blocked = 1;
  state->cells[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN - 1].blocked = 1;
  state->cells[CHANNEL_STATUES_ROW][CHANNEL_RED_STATUE_COLUMN + 1].blocked = 1;
  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++)
      MapGrid_CopyCells(BLOCK_NONE, state->cells[row][col].kind, 1, 1,
                        state->originX + col, state->originZ + row);
  // The charges sent east from the blue statue and west from the red cross
  // on a piece of two curves between the statues, the crossing's cell counted
  // in neither part of their paths.
  n = 0;
  if (Random16() < 32768) {
    n |= GabombaStatue_LayPath(2, 7, 5, 7, PATH_TO_CROSSING_CELLS, HEADING_EAST,
                               HEADING_NORTH, PATH_BIT_BLUE_EAST);
    n |= GabombaStatue_LayPath(8, 7, 6, 6, PATH_TO_CROSSING_CELLS, HEADING_WEST,
                               HEADING_WEST, PATH_BIT_RED_WEST);
    GabombaStatue_WritePiece(5, 6, PIECE_SOUTH_WEST_AND_NORTH_EAST,
                             PATH_BLUE_EAST | PATH_RED_WEST);
    n |= GabombaStatue_LayPath(4, 6, 4, 3,
                               EAST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
                               HEADING_WEST, HEADING_EAST, PATH_BIT_BLUE_EAST);
    n |= GabombaStatue_LayPath(5, 5, 5, 2,
                               WEST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
                               HEADING_NORTH, HEADING_SOUTH, PATH_BIT_RED_WEST);
  } else {
    n |= GabombaStatue_LayPath(2, 7, 4, 6, PATH_TO_CROSSING_CELLS, HEADING_EAST,
                               HEADING_EAST, PATH_BIT_BLUE_EAST);
    n |= GabombaStatue_LayPath(8, 7, 5, 7, PATH_TO_CROSSING_CELLS, HEADING_WEST,
                               HEADING_NORTH, PATH_BIT_RED_WEST);
    GabombaStatue_WritePiece(5, 6, PIECE_EAST_SOUTH_AND_WEST_NORTH,
                             PATH_BLUE_EAST | PATH_RED_WEST);
    n |= GabombaStatue_LayPath(5, 5, 4, 3,
                               EAST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
                               HEADING_NORTH, HEADING_EAST, PATH_BIT_BLUE_EAST);
    n |= GabombaStatue_LayPath(6, 6, 5, 2,
                               WEST_PATH_CELLS - PATH_TO_CROSSING_CELLS - 1,
                               HEADING_EAST, HEADING_SOUTH, PATH_BIT_RED_WEST);
  }
  if (n != 0)
    goto retry;
  n = GabombaStatue_LayPath(0, 7, 5, 4, WEST_PATH_CELLS, HEADING_WEST,
                            HEADING_NORTH, PATH_BIT_BLUE_WEST);
  n |= GabombaStatue_LayPath(10, 7, 6, 3, EAST_PATH_CELLS, HEADING_EAST,
                             HEADING_WEST, PATH_BIT_RED_EAST);
  if (n != 0)
    goto retry;
  // The two blue paths must share a cell, the two red ones too, and the
  // crossing must stand.
  blueShared = 0;
  redShared = 0;
  crossed = 0;
  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      piece = state->cells[row][col].kind;
      paths = state->cells[row][col].state;
      if (paths == PATH_BLUE)
        blueShared++;
      if (paths == PATH_RED)
        redShared++;
      if (paths == (PATH_BLUE_EAST | PATH_RED_WEST) &&
          (u32)(piece - PIECE_EAST_SOUTH_AND_WEST_NORTH) <= 1)
        crossed++;
    }
  if (blueShared == 0 || redShared == 0 || crossed == 0)
    goto retry;
  // Breaks the circuit: a straight piece near the statues, on the eastward
  // paths or the westward ones, becomes a block with no piece on it.
  westward = Random_Below(2);
  blocksLeft = CHANNEL_BLOCKS;
  for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
    col = Random_Below(CHANNEL_COLUMNS);
    row = (Random_Below(3)) + 5;
    if (GabombaStatue_HasTakenNeighbor(col, row) == 0) {
      piece = state->cells[row][col].kind;
      if ((u32)(piece - PIECE_NORTH_SOUTH) <= 1) {
        paths = state->cells[row][col].state;
        if ((westward == 0 &&
             (paths & (PATH_RED_EAST | PATH_BLUE_EAST)) != 0) ||
            (westward == 1 &&
             (paths & (PATH_BLUE_WEST | PATH_RED_WEST)) != 0)) {
          state->cells[row][col].kind = PIECE_STOP;
          state->cells[row][col].occupied = BLOCK_IN_PLACE;
          blocksLeft = CHANNEL_BLOCKS - 1;
          break;
        }
      }
    }
  }
  // Four crossings or pieces of two curves lose one of their ways as blocks.
  for (n = 0; n < 4; n++)
    for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
      col = Random_Below(CHANNEL_COLUMNS);
      row = Random_Below(CHANNEL_ROWS);
      if (GabombaStatue_HasTakenNeighbor(col, row) == 0) {
        piece = state->cells[row][col].kind;
        if (piece == PIECE_CROSSING ||
            piece == PIECE_EAST_SOUTH_AND_WEST_NORTH ||
            piece == PIECE_SOUTH_WEST_AND_NORTH_EAST) {
          cell = &state->cells[row][col];
          paths = cell->state;
          if (paths != (PATH_RED_EAST | PATH_BLUE_EAST) &&
              paths != (PATH_BLUE_WEST | PATH_RED_WEST)) {
            switch (piece) {
            case PIECE_CROSSING:
              cell->kind = (Random_Below(2)) + PIECE_NORTH_SOUTH;
              break;
            case PIECE_EAST_SOUTH_AND_WEST_NORTH:
              cell->kind = ((Random_Below(2)) << 1) + PIECE_EAST_SOUTH;
              break;
            case PIECE_SOUTH_WEST_AND_NORTH_EAST:
              cell->kind = ((Random_Below(2)) << 1) + PIECE_SOUTH_WEST;
              break;
            }
            state->cells[row][col].occupied = BLOCK_IN_PLACE;
            blocksLeft--;
            break;
          }
        }
      }
    }
  // One curve, then straight pieces for the blocks blocksLeft.
  for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
    col = Random_Below(CHANNEL_COLUMNS);
    row = Random_Below(CHANNEL_ROWS);
    if (GabombaStatue_HasTakenNeighbor(col, row) == 0) {
      piece = state->cells[row][col].kind;
      if (piece == PIECE_EAST_SOUTH || piece == PIECE_WEST_NORTH ||
          piece == PIECE_SOUTH_WEST || piece == PIECE_NORTH_EAST) {
        state->cells[row][col].occupied = BLOCK_IN_PLACE;
        blocksLeft--;
        break;
      }
    }
  }
  for (n = 0; n < blocksLeft; n++)
    for (tries = 1; tries <= CHANNEL_CELLS; tries++) {
      col = Random_Below(CHANNEL_COLUMNS);
      row = Random_Below(CHANNEL_ROWS);
      if (GabombaStatue_HasTakenNeighbor(col, row) == 0) {
        piece = state->cells[row][col].kind;
        if ((u32)(piece - PIECE_NORTH_SOUTH) <= 1) {
          state->cells[row][col].occupied = BLOCK_IN_PLACE;
          break;
        }
      }
    }
  // Turns each block at random, and draws the channel; paths now holds what
  // stands on each cell.
  for (row = 0; row < CHANNEL_ROWS; row++)
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      piece = state->cells[row][col].kind;
      paths = state->cells[row][col].occupied;
      if (paths != BLOCK_NONE) {
        switch (Random_Below(8)) {
        case 0:
        case 1:
        case 2:
          piece = GabombaStatue_TurnBlockPiece(col, row, TURN_COUNTERCLOCKWISE);
          break;
        case 3:
        case 4:
        case 5:
          piece = GabombaStatue_TurnBlockPiece(col, row, TURN_CLOCKWISE);
          break;
        case 6:
          GabombaStatue_TurnBlockPiece(col, row, TURN_COUNTERCLOCKWISE);
          piece = GabombaStatue_TurnBlockPiece(col, row, TURN_COUNTERCLOCKWISE);
          break;
        }
      }
      MapGrid_CopyCells(paths, piece, 1, 1, state->originX + col,
                        state->originZ + row);
    }
  return state->seed;
}

/// How long GabombaStatue_LayPath searches: the steps it takes in all before
/// it gives up, and the random headings it tries from a cell before backing up.
enum { PATH_ATTEMPTS_MAX = 1280, PATH_TRIES_PER_STEP = 63 };
/// The chance, out of Random16's 65,536, that a try keeps the path's heading;
/// the other half of the time it tries a random one.
#define PATH_KEEP_HEADING_CHANCE 32768

/// GabombaStatue_LayPath's steps so far: each step's heading, cell, piece,
/// headings tried and the paths through its cell before it was joined.
u8 gGabombaStatue_PathHeadings[32] BSS_DATA;
u8 gGabombaStatue_PathColumns[32] BSS_DATA;
u8 gGabombaStatue_PathRows[32] BSS_DATA;
u8 gGabombaStatue_PathPieces[32] BSS_DATA;
u8 gGabombaStatue_PathTried[32] BSS_DATA;
u8 gGabombaStatue_PathCellPaths[32] BSS_DATA;
extern s8 gGabombaStatue_PathSteps[];
extern u8 gGabombaStatue_PieceJoins[];
/// Lays a random path of `length` cells across the channel from (x0, z0) to
/// (endX, endZ), the pieces it passes joined to it, backing up a step when a
/// cell has no free way on. Returns 0, or -1 when the distance's parity rules
/// such a path out or the search gives up.
s32 GabombaStatue_LayPath(s32 x0, s32 z0, s32 endX, s32 endZ, s32 length,
                          s32 direction, s32 last, s32 bit) {
  u8 *cells, *kinds;
  s32 piece;
  s32 spare, step, dir, used, dz, index, one;
  u32 x, z, nextX, nextZ, tries, attempts;
  u8 tried;

  length--;
  dz = endZ - z0;
  spare = length - abs(endX - x0) - abs(dz);
  Iwram_ClearWordsEntry(gGabombaStatue_PathHeadings,
                        sizeof(gGabombaStatue_PathHeadings));
  Iwram_ClearWordsEntry(gGabombaStatue_PathColumns,
                        sizeof(gGabombaStatue_PathColumns));
  Iwram_ClearWordsEntry(gGabombaStatue_PathRows,
                        sizeof(gGabombaStatue_PathRows));
  Iwram_ClearWordsEntry(gGabombaStatue_PathPieces,
                        sizeof(gGabombaStatue_PathPieces));
  Iwram_ClearWordsEntry(gGabombaStatue_PathTried,
                        sizeof(gGabombaStatue_PathTried));
  Iwram_ClearWordsEntry(gGabombaStatue_PathCellPaths,
                        sizeof(gGabombaStatue_PathCellPaths));
  if (spare & 1)
    return -1;
  x = x0;
  cells = (u8 *)((struct PuzzleCell *)&gSceneState);
  one = 1;
  step = 0;
  attempts = 0;
  z = z0;
  while (1) {
    if (++attempts > PATH_ATTEMPTS_MAX)
      return -1;
    {
      s32 offset = (z * CHANNEL_COLUMNS + x) * sizeof(struct PuzzleCell);
      gGabombaStatue_PathPieces[step] =
          ((struct PuzzleCell *)(cells + offset))->kind;
      gGabombaStatue_PathCellPaths[step] =
          ((struct PuzzleCell *)(cells + offset))->state;
    }
    gGabombaStatue_PathColumns[step] = x;
    gGabombaStatue_PathRows[step] = z;
    gGabombaStatue_PathHeadings[step] = direction;
    {
      u8 *joins = gGabombaStatue_PieceJoins;
      kinds = gGabombaStatue_PathPieces;
      gGabombaStatue_PathTried[step] = gGabombaStatue_PathTried[step] |
                                       joins[kinds[step]] |
                                       1 << ((direction + 2) & 3);
    }
    tries = 0;
    while (1) {
      u32 value;
      if (++tries <= PATH_TRIES_PER_STEP) {
        value = Random16();
        dir = direction;
        if (value >= PATH_KEEP_HEADING_CHANCE)
          dir = Random_Below(4);
        tried = gGabombaStatue_PathTried[step];
        if ((tried >> dir) & one)
          continue;
        {
          s8 *steps = gGabombaStatue_PathSteps;
          index = dir * 2;
          nextX = x + steps[index];
          index += 1;
          nextZ = z + steps[index];
        }
        if (nextX < CHANNEL_COLUMNS && nextZ < CHANNEL_ROWS &&
            !(step == length - 1 && (nextX != endX || nextZ != endZ))) {
          struct PuzzleCell *cell =
              (struct PuzzleCell *)(cells + (nextZ * CHANNEL_COLUMNS + nextX) *
                                                sizeof(struct PuzzleCell));
          if (cell->blocked == 0 && (used = (cell->state >> bit) & one) == 0 &&
              cell->kind != PIECE_STOP) {
            tried |= one << dir;
            gGabombaStatue_PathTried[step] = tried;
            kinds = gGabombaStatue_PathPieces;
            piece = GabombaStatue_PickPiece(direction, dir, kinds[step]);
            {
              s32 offset =
                  (z * CHANNEL_COLUMNS + x) * sizeof(struct PuzzleCell);
              GabombaStatue_WritePiece(
                  x, z, piece,
                  ((struct PuzzleCell *)(cells + offset))->state | one << bit);
            }
            step++;
            x = nextX;
            z = nextZ;
            direction = dir;
            if (step == length)
              goto done;
            gGabombaStatue_PathTried[step] = 0;
            break;
          }
        }
        gGabombaStatue_PathTried[step] |= 1 << dir;
        if (gGabombaStatue_PathTried[step] != HEADING_ALL_TRIED)
          continue;
      }
      gGabombaStatue_PathTried[step] = 0;
      if (step == 0)
        return -1;
      step--;
      x = gGabombaStatue_PathColumns[step];
      z = gGabombaStatue_PathRows[step];
      direction = gGabombaStatue_PathHeadings[step];
      GabombaStatue_WritePiece(x, z, gGabombaStatue_PathPieces[step],
                               gGabombaStatue_PathCellPaths[step]);
      break;
    }
  }
done:
  {
    s32 offset = (nextZ * CHANNEL_COLUMNS + nextX) * sizeof(struct PuzzleCell);
    piece = GabombaStatue_PickPiece(
        dir, last, ((struct PuzzleCell *)(cells + offset))->kind);
    GabombaStatue_WritePiece(nextX, nextZ, piece,
                             ((struct PuzzleCell *)(cells + offset))->state |
                                 1 << bit);
  }
  return 0;
}

extern u8 gGabombaStatue_PieceTable[];
extern u8 gGabombaStatue_TurnedPieces[];
/// The piece joining the heading a path enters a cell by to the one it leaves
/// by, merged with the piece already there.
s32 GabombaStatue_PickPiece(s32 enter, s32 leave, s32 existing) {
  s32 value = gGabombaStatue_PieceTable[enter * 4 + leave];

  if (existing == PIECE_EAST_WEST && value == PIECE_NORTH_SOUTH)
    value = PIECE_CROSSING;
  if (existing == PIECE_NORTH_SOUTH && value == PIECE_EAST_WEST)
    value = PIECE_CROSSING;
  if (existing == PIECE_EAST_SOUTH && value == PIECE_WEST_NORTH)
    value = PIECE_EAST_SOUTH_AND_WEST_NORTH;
  if (existing == PIECE_WEST_NORTH && value == PIECE_EAST_SOUTH)
    value = PIECE_EAST_SOUTH_AND_WEST_NORTH;
  if (existing == PIECE_NORTH_EAST && value == PIECE_SOUTH_WEST)
    value = PIECE_SOUTH_WEST_AND_NORTH_EAST;
  if (existing == PIECE_SOUTH_WEST && value == PIECE_NORTH_EAST)
    value = PIECE_SOUTH_WEST_AND_NORTH_EAST;
  return value;
}

/// Puts piece kind, with the paths state, at channel cell (x, z).
void GabombaStatue_WritePiece(s32 x, s32 z, s32 kind, s32 state) {

  gSceneState.cells[z][x].kind = kind;
  gSceneState.cells[z][x].state = state;
}

/// Raises the blocks over the room, their sockets open, or sets them in
/// place: each block's actor, numbered from id, stands over its cell while it
/// is raised.
void GabombaStatue_SetBlocksRaised(s32 id, s32 show) {
  struct SceneState *state = &gSceneState;
  s32 row;
  s32 column;

  state->blocksRaised = show;
  state->firstBlock = id;
  for (row = 0; row < CHANNEL_ROWS; row++) {
    for (column = 0; column < CHANNEL_COLUMNS; column++) {
      {
        struct PuzzleCell *cell =
            state->cells[0] + (row * CHANNEL_COLUMNS + column);

        if (cell->occupied != 0) {
          struct Actor *actor = Actor_Get(id);
          s32 mapX;
          s32 mapZ;
          s32 kind;

          if (show != 0) {
            actor->x.fixed = FIELD_CELL_CENTER(state->originX + column);
            actor->z.fixed = FIELD_CELL_CENTER(state->originZ + row);
            MapGrid_CopyCells(BLOCK_NONE, PIECE_SOCKET, 1, 1,
                              state->originX + column, state->originZ + row);
            cell->occupied = BLOCK_RAISED;
          } else {
            actor->x.fixed = 0;
            actor->z.fixed = 0;
            // The same byte, written through its address.
            *(u8 *)&cell->occupied = BLOCK_IN_PLACE;
            MapGrid_CopyCells(BLOCK_IN_PLACE, cell->kind, 1, 1,
                              state->originX + column, state->originZ + row);
          }
          id++;
        }
      }
    }
  }
  state->endBlock = id;
}

/// Turns the block at channel cell (x, z) a quarter turn, and returns its new
/// piece.
s32 GabombaStatue_TurnBlockPiece(s32 x, s32 z, s32 direction) {

  gSceneState.cells[z][x].kind =
      gGabombaStatue_TurnedPieces[gSceneState.cells[z][x].kind * 2 + direction];
  return gSceneState.cells[z][x].kind;
}

/// A step of the Pound on the arrows room's stake: it sinks, and as it ends the
/// blocks rise.
void GabombaStatue_RaiseBlocks(s32 value, s32 row) {
  struct Actor *actor = Actor_Get(ACTOR_ARROWS_STAKE);

  if (row < CHANNEL_ROWS)
    actor->scaleY += -FIXED_ONE * 31 / 256;
  if (value == 1) {
    GabombaStatue_SetBlocksRaised(gSceneState.firstBlock, 1);
    Actor_SelectAnimation(actor, 6);
    actor->scaleY = FIXED_ONE;
  }
}

/// Pounding a raised block sets it back into its place in the channel.
void GabombaStatue_PoundBlock(s32 mode) {
  struct SceneState *state;
  struct Actor *actor;
  struct PuzzleCell *cell;
  s32 x, z;
  u32 row, col;
  s32 found;

  if (mode != 1)
    return;
  state = &gSceneState;
  actor = Actor_Get(((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)
                        ->secondActorId);
  x = FIELD_CELL(actor->x.fixed) - state->originX;
  z = FIELD_CELL(actor->z.fixed) - state->originZ;
  {
    s32 kind = state->cells[z][x].kind;

    cell = &state->cells[z][x];
    cell->occupied = mode;
    MapGrid_CopyCells(BLOCK_IN_PLACE, kind, 1, 1, state->originX + x,
                      state->originZ + z);
  }
  actor->x.fixed = 0;
  actor->z.fixed = 0;
  found = 0;
  row = 0;
  for (row = 0; row < CHANNEL_ROWS; row++) {
    for (col = 0; col < CHANNEL_COLUMNS; col++) {
      if (state->cells[row][col].occupied == BLOCK_RAISED) {
        found = 1;
        break;
      }
    }
    if (found)
      return;
  }
  state->blocksRaised = found;
}

/// Sets the first block actor's animation.
void GabombaStatue_SetBlockAnimation(s32 mode) {
  Actor_SetAnimationById(gSceneState.firstBlock, mode);
}

/// Turns every raised block a quarter turn.
void GabombaStatue_TurnBlocks(s32 direction) {
  struct SceneState *state = &gSceneState;
  s32 id;

  for (id = state->firstBlock; id < state->endBlock; id++) {
    struct Actor *actor = Actor_Get(id);

    GabombaStatue_TurnBlockPiece(FIELD_CELL(actor->x.fixed) - state->originX,
                                 FIELD_CELL(actor->z.fixed) - state->originZ,
                                 direction);
  }
}

/// The frames the raised blocks take to turn a quarter turn.
enum { BLOCK_TURN_FRAMES = 20 };

/// Stepping onto the left arrow panel presses it, and with the blocks raised
/// turns them a quarter turn counterclockwise.
void GabombaStatue_PressLeftArrowPanel(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->leftArrowDown == 0) {
    MapGrid_CopyCellsLayered(1, 13, 1, 1, state->originX + 3,
                             state->originZ - 2);
    if (state->blocksRaised != 0) {
      Event_PlaySound(SE_GABOMBA_STATUE_ARROW);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      GabombaStatue_SetBlockAnimation(3);
      for (i = 0; i < BLOCK_TURN_FRAMES; i++) {
        s32 *offset = &state->blocksAngle;

        *offset -= DEGtoANG(90) / BLOCK_TURN_FRAMES;
        System_Wait(1);
      }
      GabombaStatue_TurnBlocks(0);
      Event_End();
      state->blocksAngle = 0;
    }
    state->leftArrowDown = 1;
  }
}

/// Stepping off the left arrow panel raises it again.
void GabombaStatue_ReleaseLeftArrowPanel(void) {
  if (gSceneState.leftArrowDown != 0) {
    MapGrid_CopyCellsLayered(1, 12, 1, 1, gSceneState.originX + 3,
                             gSceneState.originZ - 2);
    gSceneState.leftArrowDown = 0;
  }
}

/// The right arrow panel: as the left, turning the blocks clockwise.
void GabombaStatue_PressRightArrowPanel(void) {
  struct SceneState *state = &gSceneState;
  s32 i;

  if (state->rightArrowDown == 0) {
    MapGrid_CopyCellsLayered(0, 13, 1, 1, state->originX + 7,
                             state->originZ - 2);
    if (state->blocksRaised != 0) {
      Event_PlaySound(SE_GABOMBA_STATUE_ARROW);
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      GabombaStatue_SetBlockAnimation(2);
      for (i = 0; i < BLOCK_TURN_FRAMES; i++) {
        s32 *offset = &state->blocksAngle;

        *offset += DEGtoANG(90) / BLOCK_TURN_FRAMES;
        System_Wait(1);
      }
      GabombaStatue_TurnBlocks(1);
      Event_End();
      state->blocksAngle = 0;
    }
    state->rightArrowDown = 1;
  }
}

/// Stepping off the right arrow panel raises it again.
void GabombaStatue_ReleaseRightArrowPanel(void) {
  if (gSceneState.rightArrowDown != 0) {
    MapGrid_CopyCellsLayered(0, 12, 1, 1, gSceneState.originX + 7,
                             gSceneState.originZ - 2);
    gSceneState.rightArrowDown = 0;
  }
}

/// A raised block's slot in the depth list past its position's, as
/// Render_ApplyProjectedPlacement queues an actor's sprite 62 past its own and
/// the actor's shadow 60.
enum { RAISED_BLOCK_DEPTH = 58 };

/// Each frame: draws the raised blocks over the room, turning while an arrow
/// panel turns them.
void GabombaStatue_DrawRaisedBlocks(void) {
  s32 affine = 0;
  s32 *scroll = &((struct MapState *)Ram_HeapSlots->mapGrid)->viewX;
  struct SceneState *state = &gSceneState;
  struct SpriteView *oam = state->oam;
  s32 scrollX = scroll[0] & FIELD_PIXEL_MASK;
  s32 scrollZ = scroll[1] & FIELD_PIXEL_MASK;
  u16 tile = gVramCacheEntries[state->blockTileEntry].offset >> 5;
  struct AffineTransform effect;
  s32 id;

  if (state->blocksAngle != 0) {
    effect.angle = state->blocksAngle;
    effect.scaleX = Q_8_8(1);
    effect.scaleY = Q_8_8(1);
    affine = OamMatrix_AllocFromTransform(&effect);
  }
  for (id = state->firstBlock; id < state->endBlock; id++) {
    struct Actor *piece = Actor_Get(id);
    FIXED px = piece->x.fixed;
    if (px != 0) {
      s32 x = px - scrollX;
      FIXED y = piece->y.fixed + toFIXED(16);
      FIXED pz = piece->z.fixed;
      FIXED z = pz - scrollZ;
      s32 sy = z - y;
      s32 depth = FIXEDtoINT(y + z) + RAISED_BLOCK_DEPTH;
      s32 cellX = FIELD_CELL(px) - state->originX;
      s32 cellZ = FIELD_CELL(pz) - state->originZ;
      s32 kind = state->cells[cellZ][cellX].kind;
      u32 *p;
      x = (FIXEDtoINT(x) - 8) & OBJ_X_MASK;
      sy = (FIXEDtoINT(sy) - 8) & OBJ_Y_MASK;
      p = (u32 *)oam;
      *p++ = 0;
      *p++ = sy | OBJ_X(x) | OBJ_SIZE_16X16 | OBJ_256_COLORS;
      *p = (tile + (kind << 3)) | OBJ_PRIORITY(2);
      if (state->blocksAngle != 0) {
        oam->affineMode = 1;
        oam->matrix = affine;
      }
      Oam_AddSpriteAtDepthClamped(oam, depth);
      oam++;
    }
  }
}

/// Sets up the channel puzzle on entering the room, or redraws it when
/// returning.
s32 GabombaStatue_SetUpChannelPuzzle(s32 x, s32 z, s32 seed, s32 id) {
  struct SceneState *state = &gSceneState;
  s32 laidSeed;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    GabombaStatue_LoadBlockPictures();
    GabombaStatue_RedrawChannel();
    laidSeed = state->seed;
    Task_AddFrame(GabombaStatue_DrawRaisedBlocks, TASK_PASS_FRAME);
  } else {
    Iwram_ClearWordsEntry(state, sizeof(struct SceneState));
    state->originX = x;
    state->originZ = z;
    GabombaStatue_LoadBlockPictures();
    laidSeed = GabombaStatue_LayChannel(seed);
    GabombaStatue_SetBlocksRaised(id, 0);
    Task_AddFrame(GabombaStatue_DrawRaisedBlocks, TASK_PASS_FRAME);
  }
  Actor_Get(id)->retainSpriteCache = 2;
  return laidSeed;
}

extern s32 gGabombaStatue_ChargeShrinkScript[];
extern u32 gGabombaStatue_ChargeBurstScript[];

/// The frames a charge takes to cross a cell of the channel, and the cells a
/// run of the charges crosses at most.
enum { CHARGE_STEPS_PER_CELL = 16, CHARGE_CELLS_MAX = 128 };

/// A frame of a charge's run along the channel: it follows the piece under it,
/// bursts where that piece does not go on its way, and shrinks away where it is
/// stopped short of the statue in the middle or reaches a raised block's
/// socket. The actor id GabombaStatue_RunCharges passes is never read.
void GabombaStatue_MoveCharge(struct Actor *charge, s32 step, s32 id) {
  s16 *colp = &charge->chargeColumn;
  s32 col = *colp;
  s16 *rowp = &charge->chargeRow;
  s32 row = *rowp;
  u8 *headingp = &charge->chargeHeading;
  s32 heading = *headingp;
  s32 angle = heading * DEGtoANG(90);
  s32 cell = row * CHANNEL_COLUMNS + col;
  struct SceneState *state = &gSceneState;
  s32 piece = state->cells[row][col].kind;
  s32 goal = state->cells[row][col].occupied;
  s32 key = CHARGE_STEP(heading, piece);
  s32 d = step - MAPGRID_CELL_PIXELS / 2;
  s32 arc = step * (DEGtoANG(90) / CHARGE_STEPS_PER_CELL);
  FIXED s = Math_MulFX(toFIXED(8), Trig_Sin(arc));
  FIXED c = Math_MulFX(toFIXED(8), Trig_Cos(arc));
  s32 z;
  s32 x;

  x = FIELD_CELL_CENTER(state->originX + col);
  z = FIELD_CELL_CENTER(state->originZ + row);
  if (goal == BLOCK_RAISED) {
    heading = row = col = 0;
    if (charge->chargeState == CHARGE_RUNNING) {
      Sound_Play(SE_DUST_PUFF);
      Actor_StartScript(charge, gGabombaStatue_ChargeShrinkScript);
      charge->chargeState = goal;
    }
  } else {
    switch (key) {
    case CHARGE_STEP(HEADING_WEST, PIECE_EAST_WEST):
    case CHARGE_STEP(HEADING_WEST, PIECE_CROSSING):
      col--;
      x -= INTtoFIXED(d);
      break;
    case CHARGE_STEP(HEADING_EAST, PIECE_EAST_WEST):
    case CHARGE_STEP(HEADING_EAST, PIECE_CROSSING):
      col++;
      x += INTtoFIXED(d);
      break;
    case CHARGE_STEP(HEADING_SOUTH, PIECE_NORTH_SOUTH):
    case CHARGE_STEP(HEADING_SOUTH, PIECE_CROSSING):
      row++;
      z += INTtoFIXED(d);
      break;
    case CHARGE_STEP(HEADING_NORTH, PIECE_NORTH_SOUTH):
    case CHARGE_STEP(HEADING_NORTH, PIECE_CROSSING):
      row--;
      z -= INTtoFIXED(d);
      break;
    case CHARGE_STEP(HEADING_WEST, PIECE_EAST_SOUTH):
    case CHARGE_STEP(HEADING_WEST, PIECE_EAST_SOUTH_AND_WEST_NORTH):
      row++;
      heading = HEADING_SOUTH;
      x = x - s + toFIXED(8);
      z = z - c + toFIXED(8);
      angle = DEGtoANG(180) - arc;
      break;
    case CHARGE_STEP(HEADING_NORTH, PIECE_SOUTH_WEST):
    case CHARGE_STEP(HEADING_NORTH, PIECE_SOUTH_WEST_AND_NORTH_EAST):
      col--;
      heading = HEADING_WEST;
      x = x + c - toFIXED(8);
      z = z - s + toFIXED(8);
      angle = TRIG_TURN * 3 / 4 - arc;
      break;
    case CHARGE_STEP(HEADING_EAST, PIECE_WEST_NORTH):
    case CHARGE_STEP(HEADING_EAST, PIECE_EAST_SOUTH_AND_WEST_NORTH):
      row--;
      heading = HEADING_NORTH;
      x = x + s - toFIXED(8);
      z = z + c - toFIXED(8);
      angle = -arc;
      break;
    case CHARGE_STEP(HEADING_SOUTH, PIECE_NORTH_EAST):
    case CHARGE_STEP(HEADING_SOUTH, PIECE_SOUTH_WEST_AND_NORTH_EAST):
      col++;
      heading = HEADING_EAST;
      x = x - c + toFIXED(8);
      z = z + s - toFIXED(8);
      angle = DEGtoANG(90) - arc;
      break;
    case CHARGE_STEP(HEADING_WEST, PIECE_NORTH_EAST):
    case CHARGE_STEP(HEADING_WEST, PIECE_SOUTH_WEST_AND_NORTH_EAST):
      row--;
      heading = HEADING_NORTH;
      x = x - s + toFIXED(8);
      z = z + c - toFIXED(8);
      angle = arc + DEGtoANG(180);
      break;
    case CHARGE_STEP(HEADING_SOUTH, PIECE_WEST_NORTH):
    case CHARGE_STEP(HEADING_SOUTH, PIECE_EAST_SOUTH_AND_WEST_NORTH):
      col--;
      heading = HEADING_WEST;
      x = x + c - toFIXED(8);
      z = z + s - toFIXED(8);
      angle = arc + DEGtoANG(90);
      break;
    case CHARGE_STEP(HEADING_EAST, PIECE_SOUTH_WEST):
    case CHARGE_STEP(HEADING_EAST, PIECE_SOUTH_WEST_AND_NORTH_EAST):
      row++;
      heading = HEADING_SOUTH;
      x = x + s - toFIXED(8);
      z = z - c + toFIXED(8);
      angle = arc;
      break;
    case CHARGE_STEP(HEADING_NORTH, PIECE_EAST_SOUTH):
    case CHARGE_STEP(HEADING_NORTH, PIECE_EAST_SOUTH_AND_WEST_NORTH):
      col++;
      heading = HEADING_EAST;
      x = x - c + toFIXED(8);
      z = z - s + toFIXED(8);
      angle = arc + TRIG_TURN * 3 / 4;
      break;
    case CHARGE_STEP(HEADING_EAST, PIECE_SOCKET):
      heading = row = col = 0;
      break;
    case CHARGE_STEP(HEADING_SOUTH, PIECE_SOCKET):
      heading = row = col = 0;
      break;
    case CHARGE_STEP(HEADING_WEST, PIECE_SOCKET):
      heading = row = col = 0;
      break;
    case CHARGE_STEP(HEADING_EAST, PIECE_STOP):
    case CHARGE_STEP(HEADING_SOUTH, PIECE_STOP):
    case CHARGE_STEP(HEADING_WEST, PIECE_STOP):
    case CHARGE_STEP(HEADING_NORTH, PIECE_STOP):
      if (charge->chargeState == CHARGE_RUNNING) {
        charge->x.fixed = x;
        charge->y.fixed = 0;
        charge->z.fixed = z;
        charge->facing = angle;
        if (FIELD_CELL(x) ==
                CHANNEL_ORIGIN_COLUMN + CHANNEL_MIDDLE_STATUE_COLUMN &&
            FIELD_CELL(z) == CHANNEL_ORIGIN_ROW + CHANNEL_MIDDLE_STATUE_ROW) {
          charge->chargeState = CHARGE_ARRIVED;
        } else {
          Sound_Play(SE_DUST_PUFF);
          Actor_StartScript(charge, gGabombaStatue_ChargeShrinkScript);
          charge->chargeState = CHARGE_LEAVING;
        }
      }
      break;
    case CHARGE_STEP(HEADING_NORTH, PIECE_SOCKET):
      heading = row = col = 0;
      break;
    default:
      heading = row = col = 0;
      if (charge->chargeState == CHARGE_RUNNING) {
        Sound_Play(SE_WATER);
        Actor_StartScript(charge, gGabombaStatue_ChargeBurstScript);
        charge->chargeState = CHARGE_LEAVING;
      }
      break;
    }
  }
  if (step == CHARGE_STEPS_PER_CELL - 1) {
    *colp = col;
    *rowp = row;
    *headingp = heading;
  }
  if (charge->chargeState == CHARGE_RUNNING) {
    charge->y.fixed = 0;
    charge->x.fixed = x;
    charge->z.fixed = z;
    charge->facing = angle;
    // riseBaseY counts the frames the charge has run.
    charge->riseBaseY++;
  }
}

/// Runs the four charges from the orb statues along the channel until each has
/// stopped and, with check, tells how they reached the statue in the middle.
s32 GabombaStatue_RunCharges(s32 first, s32 check) {
  struct Actor *actor;
  s32 second = first + 1;
  s32 frame, tries, id;
  s32 done;
  s32 result;

  actor = Actor_Get(first);
  actor->chargeColumn = CHANNEL_BLUE_STATUE_COLUMN - 1;
  actor->chargeRow = CHANNEL_STATUES_ROW;
  actor->chargeHeading = HEADING_WEST;
  actor->chargeState = CHARGE_RUNNING;
  actor->riseBaseY = 0;
#ifdef UBFIX
  Actor_StartIdle(actor);
#else
  // UB: called without the actor it starts, which it finds in r0 only because
  // Actor_Get left it there.
  Actor_StartIdle();
#endif
  Actor_SelectAnimation(actor, 2);
  id = second;
  actor = Actor_Get(id++);
  actor->chargeColumn = CHANNEL_BLUE_STATUE_COLUMN + 1;
  actor->chargeRow = CHANNEL_STATUES_ROW;
  actor->chargeHeading = HEADING_EAST;
  actor->chargeState = CHARGE_RUNNING;
  actor->riseBaseY = 0;
  Actor_StartIdle(actor);
  Actor_SelectAnimation(actor, 2);
  actor = Actor_Get(id++);
  actor->chargeColumn = CHANNEL_RED_STATUE_COLUMN - 1;
  actor->chargeRow = CHANNEL_STATUES_ROW;
  actor->chargeHeading = HEADING_WEST;
  actor->chargeState = CHARGE_RUNNING;
  actor->riseBaseY = 0;
  Actor_StartIdle(actor);
  Actor_SelectAnimation(actor, 2);
  actor = Actor_Get(id);
  actor->chargeColumn = CHANNEL_RED_STATUE_COLUMN + 1;
  actor->chargeRow = CHANNEL_STATUES_ROW;
  actor->chargeHeading = HEADING_EAST;
  actor->chargeState = CHARGE_RUNNING;
  actor->riseBaseY = 0;
  Actor_StartIdle(actor);
  Actor_SelectAnimation(actor, 2);
  for (tries = 0; tries < CHARGE_CELLS_MAX; tries++) {
    done = 1;
    for (frame = 0; frame < CHARGE_STEPS_PER_CELL; frame++) {
      for (id = first; id < first + 4; id++) {
        actor = Actor_Get(id);
        if (actor->chargeState != CHARGE_STOPPED &&
            actor->chargeState != CHARGE_ARRIVED)
          done = 0;
        GabombaStatue_MoveCharge(actor, frame, id);
      }
      System_Wait(1);
    }
    if (done)
      break;
  }
  System_Wait(20);
  result = CHARGE_ARRIVAL_NONE;
  if (check) {
    // A charge still on the map has arrived; riseBaseY is its running frames,
    // the sooner of each orb's two.
    u32 blue = 0;
    u32 red = 0;
    {
      struct Actor *r = Actor_Get(first);
      if (r->x.fixed != 0)
        blue = r->riseBaseY;
    }
    {
      struct Actor *r = Actor_Get(second);
      if (r->x.fixed != 0) {
        u32 value = r->riseBaseY;
        if (blue == 0 || blue > value)
          blue = value;
      }
    }
    {
      struct Actor *r = Actor_Get(first + 2);
      if (r->x.fixed != 0)
        red = r->riseBaseY;
    }
    {
      struct Actor *r = Actor_Get(first + 3);
      if (r->x.fixed != 0) {
        u32 value = r->riseBaseY;
        if (red == 0 || red > value)
          red = value;
      }
    }
    if (blue != 0) {
      result = CHARGE_ARRIVAL_BLUE_FIRST;
      if (red != 0) {
        result = CHARGE_ARRIVAL_TOGETHER;
        if (blue != red) {
          if (blue < red) {
            Event_SetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART);
            result = CHARGE_ARRIVAL_BLUE_FIRST;
          } else {
            Event_SetFlag(FLAG_GABOMBA_CHARGES_REACHED_APART);
            result = CHARGE_ARRIVAL_RED_FIRST;
          }
        }
      }
    } else {
      if (red != 0)
        return CHARGE_ARRIVAL_RED_FIRST;
      return CHARGE_ARRIVAL_NONE;
    }
  }
  return result;
}

/// Finds the cell where the circuit is broken: a block with no piece on it.
void GabombaStatue_FindBrokenCell(u32 *x_out, u32 *z_out) {
  u32 z;
  u32 x;

  for (z = 0; z < CHANNEL_ROWS; z++) {
    for (x = 0; x < CHANNEL_COLUMNS; x++) {
      if (gSceneState.cells[z][x].kind == PIECE_STOP &&
          !(x == CHANNEL_MIDDLE_STATUE_COLUMN &&
            z == CHANNEL_MIDDLE_STATUE_ROW) &&
          !(z == CHANNEL_STATUES_ROW && (x == CHANNEL_BLUE_STATUE_COLUMN ||
                                         x == CHANNEL_RED_STATUE_COLUMN))) {
        *x_out = x;
        *z_out = z;
        return;
      }
    }
  }
}

/// The clock's digit sprite sheet: the pictures of 0 to 9 and the colon, 8 by
/// 16 pixels each, then the five shown.
enum {
  CLOCK_PICTURE_BYTES = 64,
  CLOCK_SHEET_PICTURES = 11,
  CLOCK_SHOWN_PICTURES = 5
};

/// The depth the clock is drawn at, in front of the room's actors.
#define CLOCK_DEPTH 236

/// The offering's countdown clock, MM:SS:T drawn as sprites: the digits that
/// change shrink and grow back each second as it ticks.
struct ClockState {
  u8 *tiles;  ///< the digit sprite sheet
  u8 *digits; ///< the five digit pictures, as loaded
  u8 old[8];
  u8 now[8];
  u8 digit[8];
  struct SpriteView oam[8];
  s32 tileEntry;
  s32 timer;
  u8 ringing;
  u8 mode;
};

/// The clock's state, kept in the inventory snapshot's spare room.
#define CLOCK_STATE ((struct ClockState *)(gInventorySnapshot + 648))

/// Converts the remaining frames into minutes, seconds and tenths
/// (`MM:SS:T`).
void GabombaStatue_FormatClock(s32 time, u8 *text) {
  if (time < 0)
    time = 0;
  *text = time / (FRAMES_PER_SECOND * 60 * 10) + '0';
  time %= (FRAMES_PER_SECOND * 60 * 10);
  *++text = time / (FRAMES_PER_SECOND * 60) + '0';
  time %= (FRAMES_PER_SECOND * 60);
  *++text = ':';
  *++text = time / (FRAMES_PER_SECOND * 10) + '0';
  time %= (FRAMES_PER_SECOND * 10);
  *++text = time / FRAMES_PER_SECOND + '0';
  time %= FRAMES_PER_SECOND;
  *++text = ':';
  *++text = time / (FRAMES_PER_SECOND / 10) + '0';
  text[1] = 0;
}

/// Plays the clock's tick each second and returns the scale of the changing
/// digits, which shrink and grow back over the first third of each second.
s32 GabombaStatue_TickClock(s32 time) {
  struct ClockState *clock = CLOCK_STATE;
  s32 frame = time % FRAMES_PER_SECOND;
  s32 scale = Q_8_8(2);
  u8 *ringing = &clock->ringing;

  if (frame != 0) {
    *ringing = 1;
  } else if (*ringing) {
    if (clock->timer != 0)
      Event_PlaySound(SE_COLOSSO_COUNTDOWN);
    *ringing = frame;
  }
  if (frame < FRAMES_PER_SECOND / 3) {
    FIXED cosine = Trig_Cos(frame * (TRIG_TURN / 2) / 10);
    s32 (*mul)(s32 a, s32 b);
    s32 half = Q_8_8(0.5);

    mul = Iwram_MulFX;
    scale = mul(half << 2, cosine);
  }
  if (scale <= Q_8_8(1) / 16)
    scale = Q_8_8(1) / 16;
  return scale;
}

/// Draws the clock's minutes and seconds, doubled in size, the colon and each
/// digit about to change taking the ticking scale.
void GabombaStatue_DrawClock(void) {
  struct ClockState *clock = CLOCK_STATE;
  u8 *digit = clock->digit;
  u8 *old = clock->tiles;
  u8 *now = clock->digits;
  u32 tile = gVramCacheEntries[clock->tileEntry].offset >> 5;
  struct SpriteView *oam = clock->oam;
  s32 *timer = &clock->timer;
  struct AffineTransform effect;
  s32 plain, scaled, i;
  s32 scale;
  u32 *p;

  GabombaStatue_FormatClock(*timer + 50, digit);
  GabombaStatue_FormatClock(*timer + 60, clock->old);
  GabombaStatue_FormatClock(*timer, clock->now);
  // old and now walk the sprite sheet and the pictures here, the digit lists
  // below.
  for (i = 0; i < CLOCK_SHOWN_PICTURES; i++) {
    Iwram_CopyWordsEntry(
        now, old + *digit * CLOCK_PICTURE_BYTES - '0' * CLOCK_PICTURE_BYTES,
        CLOCK_PICTURE_BYTES);
    now += CLOCK_PICTURE_BYTES;
    digit++;
  }
  Vram_Load(clock->tileEntry, CLOCK_SHOWN_PICTURES * CLOCK_PICTURE_BYTES,
            clock->digits);
  effect.angle = 0;
  effect.scaleX = Q_8_8(2);
  effect.scaleY = Q_8_8(2);
  plain = OamMatrix_AllocFromTransform(&effect);
  scale = GabombaStatue_TickClock(*timer);
  effect.angle = 0;
  effect.scaleX = scale;
  effect.scaleY = scale;
  scaled = OamMatrix_AllocFromTransform(&effect);
  old = clock->old;
  now = clock->now;
  for (i = 0; i < CLOCK_SHOWN_PICTURES; i++, tile += 2, oam++) {
    p = (u32 *)oam;
    *p++ = 0;
    *p++ = OBJ_X(i * 16) | OBJ_SIZE_8X16;
    *p = tile | OBJ_PRIORITY(1);
    oam->affineMode = 3;
    if (*old++ != *now++ || i == 2)
      oam->matrix = scaled;
    else
      oam->matrix = plain;
    Oam_AddSpriteAtDepthClamped(oam, CLOCK_DEPTH);
  }
}

/// Counts the clock down while no menu is open, and draws it.
void GabombaStatue_CountDownShowingClock(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct ClockState *clock = CLOCK_STATE;

  if (state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
      clock->timer > 0)
    clock->timer--;
  GabombaStatue_DrawClock();
}

/// Counts the clock down while no menu is open.
void GabombaStatue_CountDown(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct ClockState *clock = CLOCK_STATE;

  if (state->psynergy == 0 && state->fieldMenu == 0 && state->pauseMenu == 0 &&
      clock->timer > 0)
    clock->timer--;
}

/// Starts the clock at one minute, shown or hidden.
void GabombaStatue_StartClock(s32 mode) {
  struct ClockState *clock = CLOCK_STATE;
  u8 *tiles;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    Iwram_ClearWordsEntry(clock, sizeof(struct ClockState));
    clock->timer = FRAMES_PER_SECOND * 60;
    clock->ringing = 1;
  }
  tiles = Heap_AllocEwram(1024);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_CLOCK_DIGITS), tiles);
  clock->tiles = tiles;
  clock->digits = tiles + CLOCK_SHEET_PICTURES * CLOCK_PICTURE_BYTES;
  clock->tileEntry = VramCache_FindFreeEntry();
  Vram_Load(clock->tileEntry, CLOCK_SHOWN_PICTURES * CLOCK_PICTURE_BYTES, NULL);
  clock->mode = mode;
  if (mode)
    Task_AddFrame(GabombaStatue_CountDownShowingClock, TASK_PASS_FRAME);
  else
    Task_AddFrame(GabombaStatue_CountDown, TASK_PASS_FRAME);
}

/// Stops the clock's countdown.
void GabombaStatue_StopClock(void) {
  struct ClockState *clock = CLOCK_STATE;

  if (clock->mode)
    FieldTask_Remove(GabombaStatue_CountDownShowingClock);
  else
    FieldTask_Remove(GabombaStatue_CountDown);
}

/// Starts the clock counting down from a minute.
void GabombaStatue_RestartClock(void) {
  struct ClockState *clock = CLOCK_STATE;

  if (clock->mode)
    Task_AddFrame(GabombaStatue_CountDownShowingClock, TASK_PASS_FRAME);
  else
    Task_AddFrame(GabombaStatue_CountDown, TASK_PASS_FRAME);
  clock->timer = FRAMES_PER_SECOND * 60;
}

/// The frames left on the clock.
s32 GabombaStatue_GetClock(void) { return CLOCK_STATE->timer; }

/// Sets the clock back to a minute.
void GabombaStatue_ResetClock(void) {
  CLOCK_STATE->timer = FRAMES_PER_SECOND * 60;
}

/// Puts out the stakes of a table of rows (actor, mode, flag), ended by -1,
/// opening the cells under each.
void GabombaStatue_PlaceSwitchActors(const u16 *entry) {
  s32 value;

  for (; (value = *entry) != (u16)-1; entry += 2) {
    u16 id = *entry++;
    struct Actor *actor = Actor_Get(id);
    s16 mode = entry[0];
    s16 flag = entry[1];

    actor->collisionFlags &= ~ACTOR_SOLID;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetCellOpen(id, (u16)mode, (u16)flag, actor->layer);
  }
}

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

// clang-format on

/// The shapes GabombaStatue_DrawCogCells writes over the map's from
/// COG_SHAPE_FIRST.
u32 gGabombaStatue_CogShapes[] = {
    [COG_SHAPE_GREEN_COG - COG_SHAPE_FIRST] = MAP_SHAPE(7, 0, 2, 0),
    [COG_SHAPE_TOP - COG_SHAPE_FIRST] = MAP_SHAPE(0, 2, 0, 0),
    [COG_SHAPE_NORTHWEST - COG_SHAPE_FIRST] = MAP_SHAPE(3, 0, 2, 0),
    [COG_SHAPE_NORTHEAST - COG_SHAPE_FIRST] = MAP_SHAPE(4, 0, 2, 0),
    [COG_SHAPE_SOUTHWEST - COG_SHAPE_FIRST] = MAP_SHAPE(4, 2, 0, 0),
    [COG_SHAPE_SOUTHEAST - COG_SHAPE_FIRST] = MAP_SHAPE(3, 2, 0, 0),
    [COG_SHAPE_WEST - COG_SHAPE_FIRST] = MAP_SHAPE(1, 1, 2, 0),
    [COG_SHAPE_EAST - COG_SHAPE_FIRST] = MAP_SHAPE(1, 2, 1, 0),
    [COG_SHAPE_NORTH - COG_SHAPE_FIRST] = MAP_SHAPE(2, 1, 2, 0),
    [COG_SHAPE_SOUTH - COG_SHAPE_FIRST] = MAP_SHAPE(2, 2, 1, 0),
    [COG_SHAPE_PINK_DRUM_SOUTH_END - COG_SHAPE_FIRST] = MAP_SHAPE(0, 248, 0, 0),
    [COG_SHAPE_END - COG_SHAPE_FIRST] = MAP_SHAPE(0, 0, 0, 0),
};

/// Each cog's animation by the cogs' direction + 1: turning back, stopped and
/// turning.
s8 gGabombaStatue_CogAnimations[] = {
    3,
    1,
    2,
    0,
};

u32 gGabombaStatue_PinionReleaseScript[] = {
    SC_RELEASE,
};

// clang-format off
/// Unused: gGabombaStatue_TurnedPieces from its second byte to its 21st.
LATE_DATA u8 gGabombaStatue_UnusedTurnedPieces[] = {
    PIECE_NONE, PIECE_EAST_WEST, PIECE_EAST_WEST, PIECE_NORTH_SOUTH,
    PIECE_NORTH_SOUTH, PIECE_CROSSING, PIECE_CROSSING, PIECE_NORTH_EAST,
    PIECE_SOUTH_WEST, PIECE_EAST_SOUTH, PIECE_WEST_NORTH, PIECE_SOUTH_WEST,
    PIECE_NORTH_EAST, PIECE_WEST_NORTH, PIECE_EAST_SOUTH,
    PIECE_SOUTH_WEST_AND_NORTH_EAST, PIECE_SOUTH_WEST_AND_NORTH_EAST,
    PIECE_EAST_SOUTH_AND_WEST_NORTH, PIECE_EAST_SOUTH_AND_WEST_NORTH,
    PIECE_STOP, PIECE_STOP,
};

/// The step along each heading as (column, row).
LATE_DATA s8 gGabombaStatue_PathSteps[] = {
    1, 0, 0, 1, -1, 0, 0, -1,
};

/// The headings each piece joins, a bit each; a crossing, two curves and a
/// stop let no new path in.
LATE_DATA u8 gGabombaStatue_PieceJoins[] = {
    [PIECE_NONE] = 0,
    [PIECE_NORTH_SOUTH] = HEADING_BIT(HEADING_SOUTH) | HEADING_BIT(HEADING_NORTH),
    [PIECE_EAST_WEST] = HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_WEST),
    [PIECE_CROSSING] = HEADING_ALL_TRIED,
    [PIECE_EAST_SOUTH] = HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_SOUTH),
    [PIECE_SOUTH_WEST] = HEADING_BIT(HEADING_SOUTH) | HEADING_BIT(HEADING_WEST),
    [PIECE_WEST_NORTH] = HEADING_BIT(HEADING_WEST) | HEADING_BIT(HEADING_NORTH),
    [PIECE_NORTH_EAST] = HEADING_BIT(HEADING_EAST) | HEADING_BIT(HEADING_NORTH),
    [PIECE_EAST_SOUTH_AND_WEST_NORTH] = HEADING_ALL_TRIED,
    [PIECE_SOUTH_WEST_AND_NORTH_EAST] = HEADING_ALL_TRIED,
    [PIECE_STOP] = HEADING_ALL_TRIED,
};

/// The piece joining each heading a path enters a cell by (a row of four) to
/// each it leaves by.
LATE_DATA u8 gGabombaStatue_PieceTable[] = {
    // Heading east, in by the west side; out east, south, back west, north.
    PIECE_EAST_WEST, PIECE_SOUTH_WEST, PIECE_STOP, PIECE_WEST_NORTH,
    // Heading south, in by the north side.
    PIECE_NORTH_EAST, PIECE_NORTH_SOUTH, PIECE_WEST_NORTH, PIECE_STOP,
    // Heading west, in by the east side.
    PIECE_STOP, PIECE_EAST_SOUTH, PIECE_EAST_WEST, PIECE_NORTH_EAST,
    // Heading north, in by the south side.
    PIECE_EAST_SOUTH, PIECE_STOP, PIECE_SOUTH_WEST, PIECE_NORTH_SOUTH,
};

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The way the cogs turn, which each tooth an actor rides steps by.
LATE_DATA s32 gGabombaStatue_CogDirection = 0;

LATE_DATA u32 gGabombaStatue_LeaderFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 184),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_JennaFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(488, 0, 176),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_PiersFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(456, 0, 176),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_KradenFocusScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(464, 0, 208),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_KradenPointsScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(432, 0, 286),
    SC_SYNC,
    SC_MOVE(432, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_LookAtChannelScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(348, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s32 gGabombaStatue_LookNorthAndEastScript[] = {
    SC_SET(SW_TIMER, FACING_NORTH),
    SC_SET(SW_USER_VALUE, FACING_NORTHEAST),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_CALL(GabombaStatue_LookAround),
};

LATE_DATA s32 gGabombaStatue_LookNorthAndWestScript[] = {
    SC_SET(SW_TIMER, FACING_NORTH),
    SC_SET(SW_USER_VALUE, FACING_NORTHWEST),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_CALL(GabombaStatue_LookAround),
};

LATE_DATA u32 gGabombaStatue_JennaWatchesScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 308),
    SC_SYNC,
    SC_MOVE(440, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA u32 gGabombaStatue_AkafubuComesUpScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(472, 0, 308),
    SC_SYNC,
    SC_MOVE(460, 0, 312),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(1),
    SC_END,
};

/// The stakes beside the flashing cog: actor, the mode its cells open with, and
/// the flag Pounding it sets.
LATE_DATA s16 gGabombaStatue_Stakes[] = {
    ACTOR_FIRST_STAKE, 2, FLAG_GABOMBA_FIRST_STAKE_POUNDED, ACTOR_SECOND_STAKE, 2, FLAG_GABOMBA_SECOND_STAKE_POUNDED, -1, 0,
};

/// The random seeds a game's channel is laid from, one picked by the leader's
/// name (GabombaStatue_GetPuzzleSeed).
LATE_DATA u32 gGabombaStatue_ChannelSeeds[] = {
    36, 122, 201, 312, 431, 504,
    619, 712, 823, 908, 1054, 1121,
    1207, 1311, 1401, 1555,
};

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_PinionExitRegions[] = {
    {784, 16, 80, 800, 32, 96, CONDITION_ALWAYS, GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS},
    {80, 48, 368, 96, 64, 384, CONDITION_ALWAYS, GABOMBA_STATUE_PINION_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_BoneArmletChestExitRegions[] = {
    {464, 16, 240, 480, 32, 256, CONDITION_ALWAYS, GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_ELIXIR_CHEST},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_ElixirChestExitRegions[] = {
    {528, 16, 64, 544, 32, 80, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_NORTHEAST},
    {96, 16, 160, 112, 32, 176, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_WEST},
    {672, 16, 160, 688, 32, 176, CONDITION_ALWAYS, GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_EAST},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_SteelExitRegions[] = {
    {384, 16, 112, 400, 32, 128, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_NORTHWEST},
    {96, 16, 144, 112, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_WEST},
    {672, 16, 144, 688, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_EAST},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_MouseExitRegions[] = {
    {432, 16, 448, 448, 32, 464, CONDITION_ALWAYS, GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_COGS},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneRegion gGabombaStatue_CogsExitRegions[] = {
    {304, 16, 144, 320, 32, 160, CONDITION_ALWAYS, GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_CHAMBER_IDOL},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(GabombaStatuePinion),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGrounds, KIBOMBO_STATUE_GROUNDS_FROM_GABOMBA_STATUE_PINION, FLAG_KIBOMBO_AT_CEREMONY),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_KIBOMBO_STATUE_GROUNDS, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_PINION, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GABOMBA_STATUE_PINION_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST, GabombaStatueBoneArmletChest, GABOMBA_STATUE_BONE_ARMLET_CHEST_FROM_GABOMBA_STATUE_PINION, EXIT_FLAG_NONE),
    SCENE_EXITS(GabombaStatueBoneArmletChest),
    SCENE_EXIT(GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_PINION, GabombaStatuePinion, GABOMBA_STATUE_PINION_FROM_GABOMBA_STATUE_BONE_ARMLET_CHEST),
    SCENE_EXIT(GABOMBA_STATUE_BONE_ARMLET_CHEST_TO_GABOMBA_STATUE_ELIXIR_CHEST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_BONE_ARMLET_CHEST),
    SCENE_EXITS(GabombaStatueElixirChest),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_BONE_ARMLET_CHEST, GabombaStatueBoneArmletChest, GABOMBA_STATUE_BONE_ARMLET_CHEST_FROM_GABOMBA_STATUE_ELIXIR_CHEST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_NORTHEAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_NORTHEAST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_WEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_WEST),
    SCENE_EXIT(GABOMBA_STATUE_ELIXIR_CHEST_TO_GABOMBA_STATUE_STEEL_EAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_ELIXIR_CHEST_EAST),
    SCENE_EXITS(GabombaStatueSteel),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_NORTHEAST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_NORTHEAST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_WEST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_WEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_ELIXIR_CHEST_EAST, GabombaStatueElixirChest, GABOMBA_STATUE_ELIXIR_CHEST_FROM_GABOMBA_STATUE_STEEL_EAST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_NORTHWEST, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_STEEL_NORTHWEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_WEST, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_STEEL_WEST),
    SCENE_EXIT(GABOMBA_STATUE_STEEL_TO_GABOMBA_STATUE_MOUSE_EAST, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_STEEL_EAST),
    SCENE_EXITS(GabombaStatueMouse),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_NORTHWEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_NORTHWEST),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_WEST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_WEST),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_EAST, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_EAST),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_COGS, GabombaStatueCogs, GABOMBA_STATUE_COGS_FROM_GABOMBA_STATUE_MOUSE),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_A, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_A),
    SCENE_EXIT(GABOMBA_STATUE_MOUSE_TO_GABOMBA_STATUE_STEEL_SOUTHEAST_B, GabombaStatueSteel, GABOMBA_STATUE_STEEL_FROM_GABOMBA_STATUE_MOUSE_SOUTHEAST_B),
    SCENE_EXITS(GabombaStatueCogs),
    SCENE_EXIT(GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_MOUSE, GabombaStatueMouse, GABOMBA_STATUE_MOUSE_FROM_GABOMBA_STATUE_COGS),
    SCENE_EXIT(GABOMBA_STATUE_COGS_TO_GABOMBA_STATUE_CHAMBER_IDOL, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_COGS),
    SCENE_EXITS(GabombaStatueArrows),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_EAST, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_EAST),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_A_A, GabombaStatueFace, GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_B, GabombaStatueFace, GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_B),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_B_B, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_B),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_SOUTH, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_SOUTH),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_C, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_CHAMBER_IDOL_NORTH),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_D, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_D),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_KIBOMBO_STATUE_GROUNDS_CEREMONY_NORTH_E, KibomboStatueGroundsCeremony, KIBOMBO_STATUE_GROUNDS_CEREMONY_FROM_GABOMBA_STATUE_ARROWS_NORTH_E),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_FACE_EAST_A_B, GabombaStatueFace, GABOMBA_STATUE_FACE_FROM_KIBOMBO_STATUE_GROUNDS_CEREMONY_EAST_A),
    SCENE_EXIT(GABOMBA_STATUE_ARROWS_TO_GABOMBA_STATUE_CHAMBER_IDOL_WEST, GabombaStatueChamberIdol, GABOMBA_STATUE_CHAMBER_IDOL_FROM_GABOMBA_STATUE_ARROWS_WEST),
    SCENE_EXITS_END,
};

LATE_DATA s32 gGabombaStatue_FlashCogScript[] = {
    SC_CALL(GabombaStatue_FlashCog),
    SC_END,
};

LATE_DATA s32 gGabombaStatue_TurnToothScript[] = {
    SC_CALL(GabombaStatue_TurnTooth),
    SC_END,
};

LATE_DATA struct ActorPlacement gGabombaStatue_BoneArmletChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_ElixirChestActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, (s32)gGabombaStatue_TurnToothScript, 328, 0, 88, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 72, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 72, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 520, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 552, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 88, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 552, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 488, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, (s32)gGabombaStatue_FlashCogScript, 360, 0, 488, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 352, 0, 168, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_SteelActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 152, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 120, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 264, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 248, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 264, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 296, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 136, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_STEEL), MOVEMENT_TYPE_STAND, 504, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_MouseActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 168, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 120, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 184, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 184, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 136, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 408, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 232, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_GREEN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 248, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 152, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 184, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 136, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 376, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 424, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 168, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 264, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 168, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 328, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 200, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PINK_DRUM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MOUSE, FLAG_GABOMBA_MOUSE_SEEN, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_CogsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RED_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 296, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 464, 0, 216, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA s32 gGabombaStatue_ChargeScript[] = {
    SC_CALL(GabombaStatue_CircleOwner),
    SC_END,
};

LATE_DATA struct ActorPlacement gGabombaStatue_ArrowsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CARVED_FACE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ORB_PEDESTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ORB_PEDESTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ENERGY_CHARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHANNEL_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 8, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_AKAFUBU, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 430, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_PinionActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 288, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 336, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 392, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 456, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BROWN_COG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 456, FACING_WEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gGabombaStatue_Placements[] = {
    {SCENE_TABLE_END},
};

/// How the tooth the first rope is tied to rocks as the rope pulls taut, a
/// step of its turn each frame.
LATE_DATA s8 gGabombaStatue_RopeStopSteps[] = {
    1, 1, 1, 0, 0, 0, -1, -1,
    -1, 0, 0, 0, 1, 1, 0, 0,
    -1, -1, 0, 0, 1, 0, -1, 0,
    0, 0, 0, 0,
};

LATE_DATA struct Trigger gGabombaStatue_BoneArmletChestTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownBoneArmletChestLadder)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_ElixirChestTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownElixirChestLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, EVENT(GabombaStatue_ElixirPinionsCarryLeaderSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, EVENT(GabombaStatue_ElixirPinionsCarryLeaderNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FIRST_ROPE_POST_A, FLAG_GABOMBA_FIRST_ROPE_TIED, EVENT(GabombaStatue_LashRope)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SECOND_ROPE_POST_A, FLAG_GABOMBA_SECOND_ROPE_TIED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FIRST_STAKE, FLAG_GABOMBA_FIRST_STAKE_POUNDED, EVENT(GabombaStatue_PoundStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_SECOND_STAKE, FLAG_GABOMBA_SECOND_STAKE_POUNDED, EVENT(GabombaStatue_PoundStake)},
    {TRIGGER_RAISED, RAISED_COGS_TURN_BACK, FLAG_GABOMBA_COGS_TURNED_BACK, EVENT(GabombaStatue_CogsTurnBack)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_SteelTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownSteelNortheastLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SECOND_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownSteelWestLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_THIRD_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownSteelEastLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_TurnWithCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_RideBlueCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RED_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_RideSlidingCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ROLLER, CONDITION_ALWAYS, EVENT(GabombaStatue_RideRoller)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_STEEL_ROPE_POST_A, FLAG_GABOMBA_FIRST_ROPE_TIED, 0},
    {TRIGGER_TALK, ACTOR_STEEL, IF_FLAG_SET(FLAG_LANDED_BESIDE_STEEL), EVENT(GabombaStatue_FightSteel)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_MouseTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownMouseNorthLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SECOND_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownMouseWestLadder)},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_THIRD_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownMouseEastLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_TurnWithCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_RideBlueCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RED_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_RideSlidingCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ROLLER, CONDITION_ALWAYS, EVENT(GabombaStatue_RideRoller)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MOUSE, FLAG_GABOMBA_MOUSE_SEEN, EVENT(GabombaStatue_MouseShowsTheWay)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_CogsTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownCogsLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_GREEN_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_TurnWithCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BLUE_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_RideBlueCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RED_COG, CONDITION_ALWAYS, EVENT(GabombaStatue_RideSlidingCog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ROLLER, CONDITION_ALWAYS, EVENT(GabombaStatue_RideRoller)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossEastOrWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COG_END, CONDITION_ALWAYS, EVENT(GabombaStatue_HopAcrossNorthOrSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, EVENT(GabombaStatue_CogsPinionsCarryLeaderSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, EVENT(GabombaStatue_CogsPinionsCarryLeaderNorth)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_COGS_ROPE_POST_A, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_ArrowsTriggers[] = {
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FIRST_LADDER_DOWN, CONDITION_ALWAYS, EVENT(GabombaStatue_ClimbDownArrowsLadder)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AROUND_LEFT_ARROW, CONDITION_ALWAYS, EVENT(GabombaStatue_ReleaseLeftArrowPanel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_LEFT_ARROW, CONDITION_ALWAYS, EVENT(GabombaStatue_PressLeftArrowPanel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_AROUND_RIGHT_ARROW, CONDITION_ALWAYS, EVENT(GabombaStatue_ReleaseRightArrowPanel)},
    {TRIGGER_STEP, CELL_BEHAVIOR_RIGHT_ARROW, CONDITION_ALWAYS, EVENT(GabombaStatue_PressRightArrowPanel)},
    {TRIGGER_RAISED, RAISED_OFFERING_HEARD, FLAG_OFFERING_HEARD, EVENT(GabombaStatue_HearTheOffering)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_ARROWS_STAKE, FLAG_GABOMBA_RESPONDED, EVENT(GabombaStatue_PoundArrowsStake)},
    {TRIGGER_RAISED, RAISED_STAKE_STANDS_UP, CONDITION_ALWAYS, EVENT(GabombaStatue_StakeStandsUp)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_A, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_B, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_C, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_D, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_E, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_F, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_BLOCK_G, CONDITION_ALWAYS, EVENT(GabombaStatue_PoundBlock)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_PinionTriggers[] = {
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionsCarryLeaderNorth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_PINION_GAP_TURNED_BACK, CONDITION_ALWAYS, EVENT(GabombaStatue_PinionsThrowLeaderSouth)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gGabombaStatue_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// Each piece turned a quarter turn counterclockwise and clockwise, a pair a
/// piece in enum ChannelPiece's order.
LATE_DATA u8 gGabombaStatue_TurnedPieces[] = {
    PIECE_NONE, PIECE_NONE,
    PIECE_EAST_WEST, PIECE_EAST_WEST,
    PIECE_NORTH_SOUTH, PIECE_NORTH_SOUTH,
    PIECE_CROSSING, PIECE_CROSSING,
    PIECE_NORTH_EAST, PIECE_SOUTH_WEST,
    PIECE_EAST_SOUTH, PIECE_WEST_NORTH,
    PIECE_SOUTH_WEST, PIECE_NORTH_EAST,
    PIECE_WEST_NORTH, PIECE_EAST_SOUTH,
    PIECE_SOUTH_WEST_AND_NORTH_EAST, PIECE_SOUTH_WEST_AND_NORTH_EAST,
    PIECE_EAST_SOUTH_AND_WEST_NORTH, PIECE_EAST_SOUTH_AND_WEST_NORTH,
    PIECE_STOP, PIECE_STOP,
    PIECE_SOCKET, PIECE_SOCKET,
};

LATE_DATA u32 gGabombaStatue_ChargeBurstScript[] = {
    SC_ANIMATION(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 5 / 128),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_WAIT(1),
    SC_END,
};

LATE_DATA s32 gGabombaStatue_ChargeShrinkScript[] = {
    SC_ANIMATION(0),
    SC_SET(SW_SCALE_X, FIXED_ONE * 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 2),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 5 / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 5 / 128),
    SC_WAIT(1),
    SC_LOOP(48, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_SCALE_X, FIXED_ONE),
    SC_SET(SW_SCALE_Y, FIXED_ONE),
    SC_SET(SW_RISE_ENABLED, 0),
    SC_WAIT(1),
    SC_END,
};
// clang-format on

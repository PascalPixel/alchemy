/// @file
/// Jupiter Lighthouse's aerie, the open top of the tower between its two
/// archer statues, with the lid over the well guarded by two angels. A block
/// Move pushes into the gap at the end of each archer's path lights the path
/// (JupiterLighthouse_PlacePathBlock), the archer shoots a golden trident at
/// its angel (JupiterLighthouse_StartBeam), and the angel rises and tilts the
/// lid (JupiterLighthouse_RaiseAngels); with both, the lid is lifted off
/// the well (FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED).

#include "flags.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/effect_sprite.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "gamepak/far_call/system.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "graphics/resource.h"
#include "field/cell_target.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "scene/lighthouse_palettes.h"
#include "fixed_math.h"

/// The placed actors between the archers and the lid (ACTOR_RIGHT_ARCHER and
/// ACTOR_LID, shared/jupiter_lighthouse_aerie.h).
enum JupiterLighthouseAerieActorId {
  /// The blocks Move pushes into the gaps at the ends of the archers' paths.
  ACTOR_LEFT_PATH_BLOCK = ACTOR_RIGHT_ARCHER + 1,
  ACTOR_RIGHT_PATH_BLOCK,
  /// The chains hanging down the wall below the well (HangingVine_Place).
  ACTOR_WEST_CHAIN,
  ACTOR_EAST_CHAIN,
  /// The six platforms floating over the shafts either side of the well, west
  /// to east.
  ACTOR_FLOATING_PLATFORM_A,
  ACTOR_FLOATING_PLATFORM_B,
  ACTOR_FLOATING_PLATFORM_C,
  ACTOR_FLOATING_PLATFORM_D,
  ACTOR_FLOATING_PLATFORM_E,
  ACTOR_FLOATING_PLATFORM_F
};

/// What JupiterLighthouse_RaiseAngels raises: one angel, or the second of
/// the two and then the lid with both angels.
enum AngelRise {
  RISE_LEFT_ANGEL,
  RISE_RIGHT_ANGEL,
  RISE_LEFT_ANGEL_AND_LID,
  RISE_RIGHT_ANGEL_AND_LID
};

/// The archer an archer's beam comes from (JupiterLighthouse_StartBeam).
enum BeamSide { BEAM_LEFT, BEAM_RIGHT };

/// The path blocks' glows, pictures of LightSprites_InitB's set: the lit
/// channel of the path piece each block carries, from east round to south on
/// the left block and from west round to south on the right.
enum { GLOW_PATH_EAST_SOUTH = 3, GLOW_PATH_WEST_SOUTH = 5 };

/// The cells of the gaps at the ends of the archers' paths, which a path block
/// fills.
#define LEFT_GAP_CELL_X 6
#define RIGHT_GAP_CELL_X 57
#define GAP_CELL_Z 12

/// A path block's cell heights, in 8-pixel steps: the ledge it stands on, and
/// its top, 16 pixels higher, which keeps the leader from walking through it.
#define LEDGE_HEIGHT (32 / 8)
#define PATH_BLOCK_TOP_HEIGHT (48 / 8)
/// The frames a path block set down in its gap is given to land.
#define PATH_BLOCK_LANDING_FRAMES 30

/// A path block dropped into its gap: the gap's cells become the path's, in
/// their pictures and as floor Felix can walk on.
#define JupiterLighthouse_FillLeftGap()                                        \
  (MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10),                             \
   MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75),                               \
   MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12))
#define JupiterLighthouse_FillRightGap()                                       \
  (MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10),                            \
   MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75),                             \
   MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12))
/// The path to an archer lit, once its gap is filled.
#define JupiterLighthouse_LightLeftPath()                                      \
  MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10)
#define JupiterLighthouse_LightRightPath()                                     \
  MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10)

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame (gPaletteCycleFrame).
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}
extern s32 gPaletteCyclePausesInEvents;
s32 gWhirlwindRiding BSS_DATA;
/// A copy of the room's cycling palette, BG palette CYCLE_PALETTE, which the
/// cycle takes its colours from (JupiterLighthouse_LoadPalettes).
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;

struct EffectOptions;
struct EffectSprite;
struct LightOam;

void LightSprites_SetPriority(s32 value);

void JupiterLighthouse_StartBeamFlicker(struct Actor *actor);
void JupiterLighthouse_StopBeamFlicker(struct Actor *actor);

void JupiterLighthouse_StartBeam(s32 archerId, s32 angelId, s32 side);
void LightSprites_InitB(void);
void LightSprites_Add(s32 id, s32 value);

void MapGrid_SetCellAttributesAt();

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void EffectSprite_Init(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                       u32 attributes, s32 tile);

void JupiterLighthouse_UpdateBeam(void);

s32 Actor_GetSpritePriority(struct Actor *actor);

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

struct CellTarget;
struct CellTargetEntry;

void JupiterLighthouse_PlaceRaisedActors(void);

void Leader_CenterInCell(void);

void CrackedTile_Crack(struct CellTarget *state, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTarget *state, struct CellTargetEntry *entry);

void CrackedTile_DropIn(void);

struct BeamSpark;
struct FieldEffect;

void FieldCamera_WaitForArrival(void);
/// Draws an effect sprite (EffectSprite_Draw, through the overlay's import).
void Import_EffectSpriteDraw(struct EffectSprite *sprite);
void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct BeamSpark *spark);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// A shot angel rises and tilts the lid away from its side; the second
/// of the two then lifts both angels and the lid off the well, to the puzzle's
/// jingle.
void JupiterLighthouse_RaiseAngels(s32 mode) {
  struct Actor *left = Actor_Get(ACTOR_LEFT_ANGEL);
  struct Actor *right = Actor_Get(ACTOR_RIGHT_ANGEL);
  struct Actor *lid = Actor_Get(ACTOR_LID);
  struct FieldSprite *sprite = lid->sprite;
  s32 i;

  FieldActor_SetPositionAndResetMotion(lid, lid->x.fixed, lid->y.fixed,
                                       lid->z.fixed);
  if (mode == RISE_LEFT_ANGEL || mode == RISE_LEFT_ANGEL_AND_LID) {
    Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
    for (i = 0; i < ANGEL_RISE_FRAMES; i++) {
      left->y.fixed += toFIXED(1) / 10;
      lid->y.fixed += toFIXED(3) / 100;
      lid->x.fixed -= toFIXED(1) / 25;
      sprite->rotation += TRIG_TURN / 32 / ANGEL_RISE_FRAMES;
      System_Wait(1);
    }
  } else {
    Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
    for (i = 0; i < ANGEL_RISE_FRAMES; i++) {
      right->y.fixed += toFIXED(1) / 10;
      lid->y.fixed += toFIXED(3) / 100;
      lid->x.fixed += toFIXED(1) / 25;
      sprite->rotation -= TRIG_TURN / 32 / ANGEL_RISE_FRAMES;
      System_Wait(1);
    }
  }
  if (mode != RISE_LEFT_ANGEL_AND_LID && mode != RISE_RIGHT_ANGEL_AND_LID)
    return;
  FieldEvent_Wait(30);
  JupiterLighthouse_StartBeamFlicker(left);
  JupiterLighthouse_StartBeamFlicker(right);
  Event_PlaySound(SE_JUPITER_LIGHTHOUSE_BEAMS);
  for (i = 0; i < LID_LIFT_FRAMES; i++) {
    Actor_Get(ACTOR_LEFT_ANGEL)->y.fixed += toFIXED(1) / 10;
    Actor_Get(ACTOR_RIGHT_ANGEL)->y.fixed += toFIXED(1) / 10;
    lid->y.fixed += toFIXED(1) / 10;
    System_Wait(1);
  }
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(40);
  Event_PlaySound(MUS_PUZZLE_SOLVED);
  FieldEvent_Wait(10);
  Sound_WaitForJingle();
  left->motionFlags = ACTOR_FLOATS;
  right->motionFlags = ACTOR_FLOATS;
  lid->motionFlags = ACTOR_FLOATS;
}

/// The left archer shoots its beam at the left angel, which rises; the second
/// angel of the two lifts the lid off.
void JupiterLighthouse_ShootAtLeftAngel(void) {
  Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_EAST, 0);
  JupiterLighthouse_StartBeam(ACTOR_LEFT_ARCHER, ACTOR_LEFT_ANGEL, BEAM_LEFT);
  FieldEvent_Wait(40);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT) == 0) {
    JupiterLighthouse_RaiseAngels(RISE_LEFT_ANGEL);
    FieldEvent_Wait(40);
  } else {
    JupiterLighthouse_RaiseAngels(RISE_LEFT_ANGEL_AND_LID);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldEvent_Wait(40);
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_LEFT_ANGEL));
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_RIGHT_ANGEL));
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_LEFT_ARCHER));
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_RIGHT_ARCHER));
  FieldCamera_WaitForArrival();
  Event_End();
}

/// The right archer shoots its beam at the right angel, which rises; the second
/// angel of the two lifts the lid off.
void JupiterLighthouse_ShootAtRightAngel(void) {
  Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_WEST, 0);
  JupiterLighthouse_StartBeam(ACTOR_RIGHT_ARCHER, ACTOR_RIGHT_ANGEL,
                              BEAM_RIGHT);
  FieldEvent_Wait(40);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT) == 0) {
    JupiterLighthouse_RaiseAngels(RISE_RIGHT_ANGEL);
    FieldEvent_Wait(40);
  } else {
    JupiterLighthouse_RaiseAngels(RISE_RIGHT_ANGEL_AND_LID);
  }
  EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
  FieldEvent_Wait(40);
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_LEFT_ANGEL));
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_RIGHT_ANGEL));
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_LEFT_ARCHER));
  JupiterLighthouse_StopBeamFlicker(Actor_Get(ACTOR_RIGHT_ARCHER));
  FieldCamera_WaitForArrival();
  Event_End();
}

/// Move (or a push) takes hold of the block on the ledge by an archer statue:
/// the cell it leaves drops back to the ledge's height.
void JupiterLighthouse_LiftPathBlock(s32 unused, s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);

  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, LEDGE_HEIGHT);
}

/// Move sets down the block on the ledge by the archer statue at the west or
/// east end of the aerie. Pushed into the gap at the end of the archer's path
/// (cell (6, 12) in the west, (57, 12) in the east), it drops in with a heavy
/// thud, the path lights up, and the archer shoots at the angel on its side of
/// the well, which rises and lifts its half of the lid; once both angels have
/// risen the lid is off the well. Set down anywhere else, the block stands in
/// Felix's way.
void JupiterLighthouse_PlacePathBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 i;

  if (id == ACTOR_LEFT_PATH_BLOCK) {
    if (FIELD_CELL(actor->x.fixed) == LEFT_GAP_CELL_X &&
        FIELD_CELL(actor->z.fixed) == GAP_CELL_Z) {
      Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(2);
      i = 0;
      if (actor->y.fixed > actor->groundHeight || actor->velocityY != 0) {
        do {
          System_Wait(1);
          i++;
          if (i > PATH_BLOCK_LANDING_FRAMES - 1)
            break;
        } while (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
      }
      actor->motionFlags = 0;
      FieldActor_Stop(gMainRoutineState.leaderActorId);
      Actor_SetPosition(id, 0, 0);
      Event_PlaySound(SE_HEAVY_IMPACT);
      JupiterLighthouse_FillLeftGap();
      FieldEvent_Wait(30);
      Event_PlaySound(SE_MAP_TRANSITION);
      JupiterLighthouse_LightLeftPath();
      FieldEvent_Wait(20);
      JupiterLighthouse_ShootAtLeftAngel();
    } else {
      MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                            PATH_BLOCK_TOP_HEIGHT);
    }
  }
  if (id == ACTOR_RIGHT_PATH_BLOCK) {
    if (FIELD_CELL(actor->x.fixed) == RIGHT_GAP_CELL_X &&
        FIELD_CELL(actor->z.fixed) == GAP_CELL_Z) {
      Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      System_Wait(2);
      i = 0;
      if (actor->y.fixed > actor->groundHeight || actor->velocityY != 0) {
        do {
          System_Wait(1);
          i++;
          if (i > PATH_BLOCK_LANDING_FRAMES - 1)
            break;
        } while (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
      }
      actor->motionFlags = 0;
      FieldActor_Stop(gMainRoutineState.leaderActorId);
      Actor_SetPosition(id, 0, 0);
      Event_PlaySound(SE_HEAVY_IMPACT);
      JupiterLighthouse_FillRightGap();
      FieldEvent_Wait(30);
      Event_PlaySound(SE_MAP_TRANSITION);
      JupiterLighthouse_LightRightPath();
      FieldEvent_Wait(20);
      JupiterLighthouse_ShootAtRightAngel();
    } else {
      MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                            PATH_BLOCK_TOP_HEIGHT);
    }
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT))
    Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the aerie as the party enters: the window in, the palettes, the
/// chains, the floating platforms the leader may step onto, and the angels and
/// the lid as far as they have risen. Returns 0.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  if (gMainRoutineState.entrance ==
          JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST ||
      gMainRoutineState.entrance ==
          JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST)
    Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  JupiterLighthouse_LoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
  Import_HangingVinePlace(ACTOR_WEST_CHAIN, 1);
  Import_HangingVinePlace(ACTOR_EAST_CHAIN, 1);
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  Actor_Get(ACTOR_FLOATING_PLATFORM_A)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_B)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_C)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_D)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_E)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  Actor_Get(ACTOR_FLOATING_PLATFORM_F)->priorityFlags |=
      ACTOR_PRIORITY_PLATFORM;
  JupiterLighthouse_PlaceRaisedActors();
  return 0;
}

/// Lays out the path blocks: gone into their gaps, their paths lit, for each
/// angel already shot, and glowing where they stand otherwise.
s32 Scene_PrepareMap(void) {
  struct Actor *actor;

  Scene_SetArrivalFlags(0);
  LightSprites_InitB();
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_SHOT)) {
    Actor_SetPosition(ACTOR_LEFT_PATH_BLOCK, 0, 0);
    JupiterLighthouse_FillLeftGap();
    JupiterLighthouse_LightLeftPath();
  } else {
    LightSprites_Add(ACTOR_LEFT_PATH_BLOCK, GLOW_PATH_EAST_SOUTH);
    Actor_Get(ACTOR_LEFT_PATH_BLOCK)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  }
  actor = Actor_Get(ACTOR_LEFT_PATH_BLOCK);
  actor->motionFlags = 0;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                        PATH_BLOCK_TOP_HEIGHT);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_SHOT)) {
    Actor_SetPosition(ACTOR_RIGHT_PATH_BLOCK, 0, 0);
    JupiterLighthouse_FillRightGap();
    JupiterLighthouse_LightRightPath();
  } else {
    LightSprites_Add(ACTOR_RIGHT_PATH_BLOCK, GLOW_PATH_WEST_SOUTH);
    Actor_Get(ACTOR_RIGHT_PATH_BLOCK)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  }
  actor = Actor_Get(ACTOR_RIGHT_PATH_BLOCK);
  actor->motionFlags = 0;
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                        PATH_BLOCK_TOP_HEIGHT);
  Actor_Get(ACTOR_LEFT_ARCHER)->sceneCounter = 1;
  Actor_Get(ACTOR_RIGHT_ARCHER)->sceneCounter = 1;
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update.h"

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/// An actor's update: flickers its sprite between filled and its beam's
/// colours, a step a frame.
void JupiterLighthouse_FlickerBeam(struct Actor *actor) {
  u8 *fill = &gDecodeFillByte;
  u8 *step = &actor->sceneByte;
  s32 phase = *step & 3;

  *fill = 2;
  switch (phase) {
  case 1:
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_FILL);
    break;
  case 0:
  case 2:
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_GBR);
    break;
  case 3:
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_GBR);
    break;
  }
  (*step)++;
}

/// Starts an actor's flicker (JupiterLighthouse_FlickerBeam).
void JupiterLighthouse_StartBeamFlicker(struct Actor *actor) {
  actor->sceneByte = 0;
  actor->update = (ObjectUpdate)JupiterLighthouse_FlickerBeam;
}

/// Stops an actor's flicker and leaves it in its beam's colours.
void JupiterLighthouse_StopBeamFlicker(struct Actor *actor) {
  actor->update = 0;
  Actor_SetSpriteColorMode(actor, SPRITE_COLOR_GBR);
}

/// The beam's pictures, at a byte a pixel: four 8 by 8 sparkles, and the 32 by
/// 16 golden trident.
#define SPARKLE_BYTES (4 * 8 * 8)
#define TRIDENT_BYTES (32 * 16)

/// An archer's beam (JupiterLighthouse_StartBeam), in the menu's heap slot: the
/// sparks' and the trident's pictures and sprites, the beam's path, and where
/// its steps have reached.
struct BeamState {
  u8 tiles[TRIDENT_BYTES];
  struct EffectSprite sprites[32];
  struct BeamSpark sparks[32];
  struct EffectSprite large;
  u8 unusedAfterLarge[28];
  s32 from[3];
  s32 to[3];
  s32 carried[3];
  s32 pointA[3];
  s32 pointB[3];
  s16 step;
  s16 frame;
  u16 largeTile;
  u16 smallTile;
  s8 duration;
  s8 loop;
  s8 still;
  s8 side;
  struct Actor *archer;
  struct Actor *angel;
};

/// The value t of n steps of the way from from to to.
#define LERP(from, to, t, n) ((from) + ((to) - (from)) * (t) / (n))

/// The steps of an archer's beam (BeamState.step).
enum BeamStep {
  BEAM_GATHER, ///< the sparks close in on the archer
  BEAM_GROW,   ///< the trident grows there
  BEAM_SPREAD, ///< the sparks spread out round it
  BEAM_CARRY,  ///< the trident flies to the angel, the camera with it
  BEAM_SHRINK, ///< it shrinks into the angel, which starts to flicker
  BEAM_FADE,   ///< the last sparks die out
  BEAM_DONE = 999
};

/// The frames the trident takes to fly from the archer to the angel.
#define BEAM_CARRY_FRAMES 96
/// The frames a spark lives, as it fades through its four pictures.
#define BEAM_SPARK_FRAMES 16

/// A task: one frame of an archer's beam, step by step: its sparks gather,
/// the golden trident grows and is carried to the angel, and shrinks there
/// as the angel starts to flicker.
void JupiterLighthouse_UpdateBeam(void) {
  struct BeamState *state = Ram_HeapSlots->menuState;
  struct Actor *carrier = gFieldState->camera;
  s32 t = state->frame;
  s32 position[3];
  FIXED a[3];
  FIXED b[3];
  FIXED aside;
  s32 scale;
  s32 *pos;
  struct BeamSpark *spark;
  struct EffectSprite *sprite;
  struct EffectSprite *large;
  s32 i;

  switch (state->step) {
  case BEAM_GATHER:
    state->duration = 30;
    aside = state->side == BEAM_LEFT ? toFIXED(10) : -toFIXED(10);
    a[0] = state->from[0] - aside;
    a[1] = state->from[1] + toFIXED(24);
    a[2] = state->from[2];
    b[0] = state->from[0];
    b[1] = state->from[1];
    b[2] = state->from[2];
    state->pointA[0] = LERP(a[0], b[0], t, state->duration);
    state->pointA[1] = LERP(a[1], b[1], t, state->duration);
    state->pointA[2] = LERP(a[2], b[2], t, state->duration);
    a[1] = state->from[1] - toFIXED(24);
    state->pointB[0] = LERP(a[0], b[0], t, state->duration);
    state->pointB[1] = LERP(a[1], b[1], t, state->duration);
    state->pointB[2] = LERP(a[2], b[2], t, state->duration);
    if (state->frame == state->duration) {
      state->step++;
      state->frame = -1;
    }
    break;
  case BEAM_GROW:
    state->duration = 60;
    aside = state->side == BEAM_LEFT ? toFIXED(30) : -toFIXED(30);
    a[0] = state->from[0];
    a[1] = state->from[1];
    a[2] = state->from[2];
    b[0] = state->from[0] - aside;
    b[1] = state->from[1];
    b[2] = state->from[2];
    state->pointA[0] = LERP(a[0], b[0], t, state->duration);
    state->pointA[1] = LERP(a[1], b[1], t, state->duration);
    state->pointA[2] = LERP(a[2], b[2], t, state->duration);
    state->pointB[0] = state->pointA[0];
    state->pointB[1] = state->pointA[1];
    state->pointB[2] = state->pointA[2];
    large = &state->large;
    large->scaleY = large->scaleX = FIXED_ONE * state->frame / 60;
    if (state->frame == state->duration) {
      state->step++;
      state->frame = -1;
    }
    break;
  case BEAM_SPREAD:
    if (state->frame == 0)
      state->still = 1;
    aside = state->side == BEAM_LEFT ? toFIXED(1) / 2 : -toFIXED(1) / 2;
    state->pointA[0] += aside;
    state->pointB[0] += aside;
    if (state->frame == 14) {
      state->step++;
      state->frame = -1;
    }
    break;
  case BEAM_CARRY:
    if (t == 0)
      Sound_Play(SE_PSYNERGY_SPARKLE);
    state->carried[0] =
        ((state->from[0] >> 2) +
         ((state->to[0] >> 2) - (state->from[0] >> 2)) * t / BEAM_CARRY_FRAMES)
        << 2;
    state->carried[1] =
        LERP(state->from[1], state->to[1], t, BEAM_CARRY_FRAMES);
    state->carried[2] =
        LERP(state->from[2], state->to[2], t, BEAM_CARRY_FRAMES);
    Actor_SetPositionAndResetMotion(carrier, state->carried[0],
                                    state->carried[1], state->carried[2]);
    aside = state->side == BEAM_LEFT ? toFIXED(16) : -toFIXED(16);
    state->pointA[0] = state->carried[0] - aside;
    state->pointA[1] = state->carried[1];
    state->pointA[2] = state->carried[2];
    state->pointB[0] = state->pointA[0];
    state->pointB[1] = state->pointA[1];
    state->pointB[2] = state->pointA[2];
    if (state->frame == BEAM_CARRY_FRAMES) {
      state->step++;
      state->frame = -1;
    }
    break;
  case BEAM_SHRINK:
    if (state->frame == 0) {
      Sound_Play(SE_SHIMMER);
      JupiterLighthouse_StartBeamFlicker(state->angel);
    }
    large = &state->large;
    large->scaleY = large->scaleX = large->scaleX - toFIXED(1) / 32;
    if (large->scaleX < toFIXED(1) / 5) {
      large->scaleX = 0;
      state->step++;
      state->frame = -1;
    }
    break;
  case BEAM_FADE:
    if (state->frame == 0)
      state->loop = 0;
    if (state->frame == BEAM_SPARK_FRAMES)
      state->step = BEAM_DONE;
    break;
  }
  large = &state->large;
  scale = large->scaleX;
  pos = position;
  if (state->side == BEAM_LEFT)
    large->scaleX = scale < 0 ? -scale : scale;
  else
    large->scaleX = -(scale < 0 ? -scale : scale);
  pos[0] = state->carried[0];
  pos[1] = state->carried[1];
  pos[2] = state->carried[2];
  Camera_WorldToScreen(pos);
  large->x = pos[0];
  large->y = pos[2];
  Import_EffectSpriteDraw(large);
  for (i = 0, spark = state->sparks, sprite = state->sprites; i <= 7;
       i++, sprite++, spark++) {
    if (spark->age == 0) {
      if (i & 1) {
        spark->position[0] = state->pointA[0];
        spark->position[1] = state->pointA[1];
        spark->position[2] = state->pointA[2];
      } else {
        spark->position[0] = state->pointB[0];
        spark->position[1] = state->pointB[1];
        spark->position[2] = state->pointB[2];
      }
      if (state->still == 0) {
        spark->position[1] += Trig_Sin(Random16()) << 3;
        spark->drift = toFIXED(3) / 10;
      } else {
        spark->position[1] += Trig_Sin(Random16()) << 1;
        spark->position[0] += Trig_Cos(Random16()) * 10;
        spark->drift = (Random16() - 32768) << 1;
      }
    }
    if (spark->age < BEAM_SPARK_FRAMES) {
      sprite->oam.index = state->smallTile + (spark->age & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, spark);
      spark->position[1] -= spark->drift;
    }
    spark->age++;
    if (state->loop != 0 && spark->age == BEAM_SPARK_FRAMES)
      spark->age = 0;
  }
  state->frame++;
}
#undef LERP

void Actor_StartIdle(struct Actor *actor);

/// An archer shoots its beam at an angel: loads the sparks and the trident
/// into VRAM and holds the event until the beam's task has finished.
void JupiterLighthouse_StartBeam(s32 archerId, s32 angelId, s32 side) {
  struct BeamState *state =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct BeamState));
  struct Actor *carrier = gFieldState->camera;
  struct EffectSprite *sprite;
  struct EffectSprite *large;
  struct BeamSpark *spark;
  s32 smallEntry;
  s32 largeEntry;
  s32 smallTile;
  s32 largeTile;
  s32 i;
  s32 age;
  s32 size;

  state->archer = Actor_Get(archerId);
  state->angel = Actor_Get(angelId);
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_STAR_SPARKLE_FADE),
                      state);
  smallEntry = VramCache_FindFreeEntry();
  smallTile = Vram_Load(smallEntry, SPARKLE_BYTES, state);
  state->smallTile = smallTile;
  spark = state->sparks;
  age = 0;
  sprite = state->sprites;
  i = ARRAY_COUNT(state->sparks) - 1;
  for (; i >= 0; age -= 2, i--, sprite++, spark++) {
    EffectSprite_Init(sprite, 4, 4, OBJ_SIZE_8X8, smallTile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = OAM_DEPTH_WINDOW;
    spark->age = age;
  }
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_GOLDEN_TRIDENT), state);
  /* FAKEMATCH(temporary): size, the trident's size held in a local before
   * its entry is found; as TRIDENT_BYTES at the load. */
  size = TRIDENT_BYTES;
  largeEntry = VramCache_FindFreeEntry();
  largeTile = Vram_Load(largeEntry, size, state);
  state->largeTile = largeTile;
  large = &state->large;
  FieldEffectSprite_Init(large, 30, 7, OBJ_SIZE_32X16, largeTile);
  large->depth = OAM_DEPTH_WINDOW;
  large->oam.priority = 1;
  large->oam.fullColor = 1;
  large->oam.palette = 0;
  large->scaleX = 0;
  large->scaleY = 0;
  if (side == BEAM_LEFT) {
    state->from[0] = toFIXED(152);
    state->from[1] = toFIXED(32);
    state->from[2] = toFIXED(164);
    state->to[0] = toFIXED(470);
    state->to[1] = toFIXED(32);
    state->to[2] = toFIXED(164);
  } else {
    state->from[0] = toFIXED(872);
    state->from[1] = toFIXED(32);
    state->from[2] = toFIXED(164);
    state->to[0] = toFIXED(550);
    state->to[1] = toFIXED(32);
    state->to[2] = toFIXED(164);
  }
  state->carried[0] = state->from[0];
  state->carried[1] = state->from[1];
  state->carried[2] = state->from[2];
  carrier->speedLimit = toFIXED(1) / 2;
  carrier->acceleration = toFIXED(1) / 2;
  Actor_StartIdle(carrier);
  FieldActor_SetMoveTargetFX(carrier, state->carried[0], state->carried[1],
                             state->carried[2]);
  Event_PlaySound(SE_SWIRL);
  JupiterLighthouse_StartBeamFlicker(state->archer);
  FieldEvent_Wait(40);
  state->side = side;
  state->step = BEAM_GATHER;
  state->frame = 0;
  state->loop = 1;
  state->still = 0;
  Task_AddFrame(JupiterLighthouse_UpdateBeam, TASK_PASS_FRAME);
  while (state->step != BEAM_DONE)
    System_Wait(1);
  FieldTask_Remove(JupiterLighthouse_UpdateBeam);
  VramCache_FreeEntry(largeEntry);
  VramCache_FreeEntry(smallEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

#include "shared/cracked_tile_step.h"

/// Unused here: on landing below a cracked tile, puts the leader on the cell
/// the flag bytes saved, offset by (dx, dz), and starts the screen transition,
/// as the rooms with cracked floors do (CrackedTile_PlaceLanding); the aerie
/// has none to fall through.
UNUSED void CrackedTile_PlaceLanding(s32 dx, s32 dz, s32 transition) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct CellTarget *state = &gCrackedTiles;
  s32 x = FieldGameFlag_GetByte(state->flagByte);
  s32 z = FieldGameFlag_GetByte(state->flagByte + 8);

  if (x != 0 || z != 0) {
    struct Actor *camera;

    GameFlag_SetByte(state->flagByte, 0);
    GameFlag_SetByte(state->flagByte + 8, 0);
    actor->x.fixed = FIELD_CELL_CENTER(x + dx);
    actor->z.fixed = FIELD_CELL_CENTER(z + dz);
    camera = field->camera;
    camera->x.fixed = actor->x.fixed;
    camera->z.fixed = actor->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldState->transition = transition;
  }
}

extern u32 gFieldEffect_ShortScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 560, 96, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseAerie),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_AERIE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 256, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ANGEL_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 256, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 200, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 312, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 344, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 376, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 680, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 712, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_BASIN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BASIN, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEFT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {TRIGGER_PUSH, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {TRIGGER_PUSHED, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {TRIGGER_TABLE_END},
};

/// The palette cycle's frame, -1 before it starts, and whether events hold it
/// (PaletteCycle_Step).
s32 gPaletteCycleFrame = -1;
s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

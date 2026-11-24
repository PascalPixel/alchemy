/// @file
/// The Anemos Inner Sanctum, the hidden dungeon under Contigo, reached by
/// Teleport from the circle in the middle of the town.
///
/// The party arrives in a room with four element symbols on its floor.
/// Stepping onto each, Felix lights a plate for every Djinni of its element
/// the party has; with all eighteen of an element, its torch over the north
/// door lights, and with all four lit the door opens
/// (AnemosInnerSanctum_LightSymbol). Beyond it, Charon's slate speaks as the
/// party comes up to it (AnemosInnerSanctum_ApproachCharonSlate). Below, mimic
/// stones copy Felix's steps, mirrored, until they reach a button that opens
/// a door (AnemosInnerSanctum_MimicLeader); seven round switches make a
/// lift float, ready to carry the party up
/// (AnemosInnerSanctum_PressRoundSwitch); pillars are moved onto their
/// places with Move, and a switch holds a door open while something stands
/// on it; cracked floors give way under the party, and Sand lets Felix pass
/// along stretches of sand. The last room, with its six symbol blocks and
/// the Dullahan, is anemos_inner_sanctum_entrance.c.
#include "field/field_sequence.h"
#include "field/map_grid.h"
#include "graphics/window_sprite_list.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "types.h"
#include "system/task.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_state.h"
#include "field/field_sequence_event.h"
#include "field/cell_target.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "fixed_math.h"
#include "macros.h"
#include "field/field.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_write.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "system/iwram_call.h"
#include "system/frame.h"
#include "flags.h"
#include "field/object.h"
#include "field/call.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "scene/scene.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "field/actor_view.h"
#include "field/event_commands.h"
#include "field/map.h"
#include "field/objdisp.h"
#include "scene/event_layer.h"
#include "scene/mars_lighthouse_entrance.h"
#include "scene/mars_lighthouse_star_room.h"
#include "graphics/palette_buffers.h"

struct Actor;
struct CellTarget;
struct CellTargetEntry;

/// A spark sent spiralling in to a Djinni's plate, as an actor's motion fields
/// hold it: the plate it closes on (originX, originZ), the radius, angle and
/// spin of its spiral, and its height, which drops by `fall` each step.
struct Spark {
  const s32 *script;
  s16 scriptCursor;
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
  s32 fall;
  s32 scaleX, scaleY;
  u16 collisionRadius;
  u8 layer;
  u8 priorityFlags;
  s32 velocityX, velocityY, velocityZ;
  s32 step;
  s32 height;
  s32 targetX, targetY, targetZ;
  s32 originX;
  s32 originZ;
  s32 radius;
  void *sprite;
  u8 spriteStorageKind;
  u8 motionFlags;
  u8 arrivalAxis, conditionResult, snapToTarget, collisionFlags;
  u8 steeringFlags, paused, retainSpriteCache, repeatCount;
  s16 wait;
  u8 refused, frozen;
  u8 palette; ///< the palette of its sparks
  u8 sceneByte;
  s16 angle;
  s16 spin;
};

u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);

void Teleport_ArriveWhirling(void);

s32 Actor_LiftTargetBlock();

void Actor_PlaceMovedBlock(void);

void Teleport_StopShimmer(void);

void Teleport_PlaySparks(void);

void Leader_CenterInCell(void);

struct CellTarget gCrackedTiles BSS_DATA;

void AnemosInnerSanctum_LightSymbol(void);
void AnemosInnerSanctum_StepOffSymbol(void);
void AnemosInnerSanctum_StartMovingBlock(void);
void AnemosInnerSanctum_FinishMovingBlock(void);
void AnemosInnerSanctum_PressRoundSwitch(void);
void AnemosInnerSanctum_RideLiftUp(void);
void AnemosInnerSanctum_SinkIntoSand(void);
void AnemosInnerSanctum_EndSand(void);
void AnemosInnerSanctum_PressSwitch(void);
void AnemosInnerSanctum_StepOffSwitch(void);
void AnemosInnerSanctum_FallThroughFirstCracks(void);
void AnemosInnerSanctum_FallThroughSecondCracks(void);

void AnemosInnerSanctum_StartMimicStone(void);
void AnemosInnerSanctum_DropMimicStone(void);
void AnemosInnerSanctum_OpenMimicStoneDoor(void);
void AnemosInnerSanctum_ApproachCharonSlate(void);
void AnemosInnerSanctum_TeleportOut(void);

extern u32 gAnemosInnerSanctum_ShortScript[];
extern u32 gAnemosInnerSanctum_MediumScript[];
extern u32 gAnemosInnerSanctum_LongScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gAnemosInnerSanctum_NoShadowScript[];
extern s16 gAnemosInnerSanctum_MimicStoneDoorFrames[];
extern s32 gAnemosInnerSanctum_OrbScript[];

/// The actors of the rooms Move and the lifts work in, numbering each room's
/// placement table: stone pillars (SPRITE_STONE_PILLAR) Moved onto their
/// places, and the lifts (SPRITE_ARROW_PANEL) the round switches ready.
enum AnemosInnerSanctumElevatorActorId {
  ACTOR_ELEVATOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_ELEVATOR_LIFT,
  ACTOR_ELEVATOR_SECOND_PILLAR
};

/// The round switches' room.
enum AnemosInnerSanctumSwitchesActorId {
  ACTOR_SWITCHES_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_SWITCHES_BOULDER, ///< SPRITE_ROUND_BOULDER
  ACTOR_SWITCHES_SECOND_PILLAR,
  ACTOR_SWITCHES_PILLAR_A,
  ACTOR_SWITCHES_PILLAR_B,
  ACTOR_SWITCHES_THIRD_PILLAR,
  /// An unseen marker standing on the switch, 14 pixels down, as a platform:
  /// taken away as the switch is pressed (AnemosInnerSanctum_PressSwitch), put
  /// back as the leader steps off it (AnemosInnerSanctum_StepOffSwitch).
  ACTOR_SWITCHES_SWITCH_MARKER
};

/// The Orihalcon chest's room.
enum AnemosInnerSanctumOrihalconChestActorId {
  ACTOR_ORIHALCON_CHEST_LIFT = ACTOR_FIRST_PLACED,
  ACTOR_ORIHALCON_CHEST_FIRST_PILLAR,
  ACTOR_ORIHALCON_CHEST_SECOND_PILLAR
};

/// The guardian room's three mimic stones (SPRITE_MIMIC_STONE), one in each
/// room off its north side: entered from the south by entrance 1, 3 or 5, that
/// room's stone mirrors the leader's steps (AnemosInnerSanctum_MimicLeader)
/// until it reaches its button or falls into a hole.
#define ACTOR_MIMIC_STONE_A ACTOR_FIRST_PLACED
#define ACTOR_MIMIC_STONE_B (ACTOR_FIRST_PLACED + 1)
#define ACTOR_MIMIC_STONE_C (ACTOR_FIRST_PLACED + 2)

/// The guardian room's ways in from the south, one below each mimic stone.
enum {
  ENTRANCE_BY_MIMIC_STONE_A = 1,
  ENTRANCE_BY_MIMIC_STONE_B = 3,
  ENTRANCE_BY_MIMIC_STONE_C = 5
};

/// Set as each mimic stone reaches its button and opens its room's north door
/// (AnemosInnerSanctum_OpenMimicStoneDoor), so that the set-up leaves it there.
#define FLAG_MIMIC_STONE_A_ON_BUTTON FLAG_AREA_TEMP_01
#define FLAG_MIMIC_STONE_B_ON_BUTTON FLAG_AREA_TEMP_02
#define FLAG_MIMIC_STONE_C_ON_BUTTON FLAG_AREA_TEMP_03

/// Set while the leader stands on one of the Dragon Skin room's element
/// symbols, so that it lights its plates once a visit to it.
#define FLAG_ON_SYMBOL FLAG_SCENE_TEMP_00
/// Set while the switches room's switch is held down by the leader.
#define FLAG_SWITCH_PRESSED FLAG_SCENE_TEMP_00
/// Set while the guardian room's mimic stone follows the leader or rests on
/// its button, so that stepping onto its floor starts it no more.
#define FLAG_MIMIC_STONE_STARTED FLAG_SCENE_TEMP_00

/// The step triggers of the Dragon Skin room: its four element symbols, whose
/// plates light for the Djinn of the element the party has
/// (AnemosInnerSanctum_LightSymbol), the floor between them, and the Teleport
/// circle in its middle.
enum AnemosInnerSanctumSymbolTrigger {
  TRIGGER_TELEPORT_CIRCLE = 30,
  TRIGGER_VENUS_SYMBOL = 50,
  TRIGGER_MERCURY_SYMBOL,
  TRIGGER_MARS_SYMBOL,
  TRIGGER_JUPITER_SYMBOL,
  TRIGGER_FLOOR_BETWEEN_SYMBOLS
};

/// The first of the seven round switches' step triggers
/// (AnemosInnerSanctum_PressRoundSwitch).
enum { TRIGGER_FIRST_ROUND_SWITCH = 50, ROUND_SWITCHES = 7 };
/// Set as each round switch is pressed, lighting it; the macro takes the
/// switch's place from the first.
#define FLAG_ROUND_SWITCH_PRESSED(n) (FLAG_SCENE_TEMP_00 + (n))
/// Set once all seven round switches are pressed: the room's lift floats,
/// ready to carry the party.
#define FLAG_LIFT_READY FLAG_SCENE_TEMP_07
/// The ready lift's step trigger, in the elevator and Orihalcon chest rooms.
enum { TRIGGER_LIFT = 11 };
/// The switches room's step triggers: the switch, and the cell east of it
/// that a leader stepping off it crosses.
enum { TRIGGER_SWITCH = 50, TRIGGER_EAST_OF_SWITCH };
/// The step trigger before Charon's slate.
enum { TRIGGER_BEFORE_CHARON_SLATE = 30 };

/// The sanctum's lasting flags: each pillar Moved onto its place, each
/// element's torch lit, the Dragon Skin room's north door opened by all four,
/// and Charon's slate's words heard.
#define FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED 0xa80
#define FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED 0xa81
#define FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED 0xa82
#define FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED 0xa83
#define FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED 0xa84
#define FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED 0xa85
#define FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED 0xa86
#define FLAG_ANEMOS_VENUS_TORCH_LIT 0xa87
#define FLAG_ANEMOS_MERCURY_TORCH_LIT 0xa88
#define FLAG_ANEMOS_MARS_TORCH_LIT 0xa89
#define FLAG_ANEMOS_JUPITER_TORCH_LIT 0xa8a
#define FLAG_ANEMOS_NORTH_DOOR_OPEN 0xa8b
#define FLAG_ANEMOS_CHARON_SLATE_SPOKE 0xa8c

/// The guardian room's lift (SPRITE_ARROW_PANEL), which brings the party up
/// from the Orihalcon chest room (AnemosInnerSanctum_ArriveOnGuardianLift).
#define ACTOR_GUARDIAN_LIFT (ACTOR_FIRST_PLACED + 3)

/// The first column of the middle layer's cells in these rooms' maps (their
/// map_layer 128, 0): the Dragon Skin room draws its plates, torches' light and
/// pillar of light there.
#define MIDDLE_LAYER_COLUMN (MAPGRID_SIZE / 2)
/// The first row of the overhead layer's cells in these rooms' maps (their
/// map_layer 0, 128).
#define OVERHEAD_LAYER_ROW (MAPGRID_SIZE / 2)
/// How far below a plate or the pillar of light its lit picture is kept, in
/// cells.
#define LIT_PICTURE_ROWS 26

/// The four torches over the Dragon Skin room's north door, on the wall's row
/// z 152: each element's x and the height its flame burns at.
#define VENUS_TORCH_X 520
#define VENUS_TORCH_Y 48
#define MERCURY_TORCH_X 552
#define MERCURY_TORCH_Y 48
#define MARS_TORCH_X 504
#define MARS_TORCH_Y 40
#define JUPITER_TORCH_X 568
#define JUPITER_TORCH_Y 40
#define TORCH_ROW_Z 152

/// A spark ring's sixteenths of a turn; an orb starts at every second one.
#define SPARK_RING_STEPS 16

/// The ways between the rooms each of these maps holds, named by the room a
/// way leaves and the room it leads to, each room by where it lies on its map;
/// a way's exit and the entrance at its foot share its number, so the way back
/// is the entrance the exit leads to.
///
/// The elevator's: the north-west and south-west halls, the room above the
/// lift, the round switches' room below it, and the east and south-east rooms.
enum AnemosInnerSanctumElevatorWay {
  WAY_ELEVATOR_NORTHWEST_TO_SOUTHWEST = 1,
  WAY_ELEVATOR_NORTHWEST_TO_EAST,
  WAY_ELEVATOR_NORTHWEST_TO_ABOVE_THE_LIFT,
  WAY_ELEVATOR_SOUTHWEST_TO_NORTHWEST = 5,
  WAY_ELEVATOR_SOUTHWEST_TO_EAST = 7,
  WAY_ELEVATOR_SOUTHWEST_TO_SOUTHEAST = 9,
  WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST,
  /// Up the lift, to the room above.
  EXIT_UP_THE_LIFT,
  WAY_ELEVATOR_EAST_TO_NORTHWEST,
  WAY_ELEVATOR_EAST_TO_SOUTHWEST,
  WAY_ELEVATOR_EAST_TO_ROUND_SWITCHES,
  WAY_ELEVATOR_SOUTHEAST_TO_SOUTHWEST,
  WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST = 17,
  /// Where the lift arrives in the room above.
  ENTRANCE_UP_THE_LIFT
};
/// The round switches' room's: its north, north-east, west, middle and south
/// rooms, the north-east one with two ways down, west and east.
enum AnemosInnerSanctumSwitchesWay {
  WAY_SWITCHES_NORTH_TO_MIDDLE = 1,
  WAY_SWITCHES_NORTHEAST_TO_MIDDLE_WEST = 3,
  WAY_SWITCHES_NORTHEAST_TO_MIDDLE_EAST,
  WAY_SWITCHES_WEST_TO_MIDDLE,
  WAY_SWITCHES_MIDDLE_TO_WEST = 10,
  WAY_SWITCHES_MIDDLE_TO_SOUTH,
  WAY_SWITCHES_MIDDLE_TO_NORTH = 13,
  WAY_SWITCHES_MIDDLE_TO_NORTHEAST_WEST,
  WAY_SWITCHES_MIDDLE_TO_NORTHEAST_EAST,
  WAY_SWITCHES_SOUTH_TO_MIDDLE = 17
};
/// The Orihalcon chest room's: its north-east and south-east rooms, and the
/// second cracked floor's room west of them.
enum AnemosInnerSanctumOrihalconChestWay {
  WAY_ORIHALCON_CHEST_NORTHEAST_TO_SOUTHEAST = 3,
  WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST,
  WAY_ORIHALCON_CHEST_SOUTHEAST_TO_CRACKED_FLOOR = 8,
  WAY_ORIHALCON_CHEST_SOUTHEAST_TO_NORTHEAST
};
/// The guardian room's: its south and south-east rooms.
enum AnemosInnerSanctumGuardianWay {
  WAY_GUARDIAN_SOUTH_TO_SOUTHEAST = 8,
  WAY_GUARDIAN_SOUTHEAST_TO_SOUTH = 11
};
/// Set when the elevator room's lift has carried the party up; it stays above.
#define FLAG_ELEVATOR_LIFT_RISEN FLAG_AREA_TEMP_00
/// Set when the Orihalcon chest room's lift has carried the party up to the
/// guardian room; it stays there, and the round switches below stay pressed.
#define FLAG_GUARDIAN_LIFT_RISEN FLAG_AREA_TEMP_04

/// The Orihalcon chest room's two cracked floors: a cell's step trigger is its
/// floor's when whole, the next when cracked.
enum {
  TRIGGER_FIRST_CRACKED_FLOOR = 33,
  TRIGGER_SECOND_CRACKED_FLOOR = TRIGGER_FIRST_CRACKED_FLOOR + 2
};
/// The Orihalcon chest room's ways down through its cracked floors, each an
/// exit and the entrance below of one number.
enum { THROUGH_FIRST_CRACKED_FLOOR = 11, THROUGH_SECOND_CRACKED_FLOOR = 12 };

/// The row the mimic stones start on: one follows the leader's steps only
/// while he is north of it, and never goes south of it.
#define MIMIC_STONE_LINE_Z 248
/// The guardian room's step triggers by the mimic stones: the floor where the
/// leader's steps start one following, and the cell behaviors a following
/// stone stops on, a hole it falls into and the button that opens its door.
enum AnemosInnerSanctumMimicStoneTrigger {
  TRIGGER_MIMIC_STONE_START = 50,
  TRIGGER_MIMIC_STONE_HOLE,
  TRIGGER_MIMIC_STONE_BUTTON,
  /// Raised when the stone falls into a hole
  /// (AnemosInnerSanctum_DropMimicStone).
  TRIGGER_MIMIC_STONE_IN_HOLE = 200,
  /// Raised when the stone reaches the button
  /// (AnemosInnerSanctum_OpenMimicStoneDoor).
  TRIGGER_MIMIC_STONE_ON_BUTTON
};

#include "shared/teleport_shimmer.c"

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_create_silhouette.c"

#include "../field/field_effect_move.c"

extern s32 gAnemosInnerSanctum_DurationScripts[];

/// Spawns a field effect at x, y, z moving at the velocity given, its type,
/// scale, palette, priority, script, rotation, spin and update chosen by the
/// EFFECT_USE_ flags from the options.
///
/// This overlay's own copy of ../field/field_effect_spawn.c, which it does not
/// include: this one takes actor 0's sprite priority rather than the leader's,
/// picks from the overlay's own duration scripts, and sets a requested
/// priority on the sprite while creating the actor through the import.
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party = ((struct FieldEffect *)Actor_GetById(0));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect =
        (struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(
      effect, gAnemosInnerSanctum_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->priority = options->priority;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    FIXED targetX;
    s32 count;

    script = gAnemosInnerSanctum_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[EFFECT_SCRIPT_REPEAT_COUNT];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    } else {
      effect->scaleRateX = (targetX - FIXED_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

/// Clears the actor's sprite flags, as a script's call; always 0.
s32 AnemosInnerSanctum_ClearScriptedActorSpriteFlags(s32 actorId) {
  Actor_SetSpriteFlags(actorId, 0);
  return 0;
}

/// A spark ring's orb: it circles its centre at its own speed while the
/// radius shrinks a pixel a frame; 0 once it reaches the centre.
s32 AnemosInnerSanctum_CircleOrb(struct Actor *actor) {
  FIXED radius;

  actor->timer += actor->userValue;
  SET_ACTOR_POSITION(actor, actor->bounce, actor->gravity,
                     toFIXED(TORCH_ROW_Z));
  FieldVector_AddPolarOffsetFX(actor->sceneWord, actor->timer, &actor->x.fixed);
  radius = actor->sceneWord - toFIXED(1);
  actor->sceneWord = radius;
  if (radius)
    return 1;
  return 0;
}

extern u32 gAnemosInnerSanctum_FadingSparkScript[];

/// Leaves a fading spark of the source's palette where the source is.
void AnemosInnerSanctum_LeaveSpark(struct Actor *source) {
  struct Actor *actor = (FieldActor_Create(SPRITE_PARTICLE, source->x.fixed,
                                           source->y.fixed, source->z.fixed));

  actor->motionFlags = 0;
  FieldActor_SetSpriteColor(actor, source->sceneCounter);
  Scene_SetActorSpritePriority(actor, 2);
  Actor_SelectAnimation(actor, 1);
  Actor_SetSpriteFlags(actor, 0);
  Actor_StartScript(actor, gAnemosInnerSanctum_FadingSparkScript);
  Actor_SetBlendMode(actor, 1);
}

/// A spark's update: two steps a frame along its spiral, closing in and
/// falling, leaving a fading spark at each; 0 once it reaches its plate.
s32 AnemosInnerSanctum_SpiralIn(struct Spark *spark) {
  if (spark->radius < spark->step)
    return 0;
  spark->angle += spark->spin >> 1;
  spark->x = spark->originX;
  spark->y = spark->height;
  spark->z = spark->originZ;
  FieldVector_AddPolarOffsetFX(spark->radius, spark->angle, &spark->x);
  AnemosInnerSanctum_LeaveSpark(spark);
  spark->radius -= spark->step >> 1;
  spark->angle += spark->spin >> 1;
  spark->x = spark->originX;
  spark->y = spark->height;
  spark->z = spark->originZ;
  FieldVector_AddPolarOffsetFX(spark->radius, spark->angle, &spark->x);
  AnemosInnerSanctum_LeaveSpark(spark);
  spark->radius -= spark->step >> 1;
  spark->height -= spark->fall;
  return 1;
}

/// A spark reaches its plate: a puff of sparks of its palette, and the plate
/// shows its lit picture.
s32 AnemosInnerSanctum_LightPlate(struct Actor *actor) {
  struct EffectOptions options;
  s32 x;
  s32 y;

  options.priority = 2;
  options.palette = actor->sceneCounter;
  FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0,
                    toFIXED(1), 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    &options);
  x = FIELD_CELL(actor->bounce) + MIDDLE_LAYER_COLUMN;
  y = FIELD_CELL(actor->gravity);
  MapGrid_CopyCellsTo(x, y + LIT_PICTURE_ROWS, x, y, 1, 1);
  return 0;
}

#include "../field/actor_wait_until_landed.c"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gAnemosInnerSanctum_Placements[];
extern struct ActorPlacement gAnemosInnerSanctum_DragonSkinChestPlacements[];
extern struct ActorPlacement gAnemosInnerSanctum_ElevatorPlacements[];
extern struct ActorPlacement gAnemosInnerSanctum_SwitchesPlacements[];
extern struct ActorPlacement gAnemosInnerSanctum_OrihalconChestPlacements[];
extern struct ActorPlacement gAnemosInnerSanctum_GuardianPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
    return gAnemosInnerSanctum_DragonSkinChestPlacements;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
    return gAnemosInnerSanctum_ElevatorPlacements;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
    return gAnemosInnerSanctum_SwitchesPlacements;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
    return gAnemosInnerSanctum_OrihalconChestPlacements;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
    return gAnemosInnerSanctum_GuardianPlacements;
  return gAnemosInnerSanctum_Placements;
}

void Actor_StartScript();
extern s32 gAnemosInnerSanctum_SparkScript[];

/// Sends a spark of the given palette spiralling from one point to another.
void AnemosInnerSanctum_SendSpark(from, to, palette) struct Vector3 *from;
struct Vector3 *to;
s16 palette;
{
  struct Spark *spark;
  s32 x, z, distance;

  spark = ((struct Spark *)FieldActor_Create(SPRITE_PARTICLE, from->x, from->y,
                                             from->z));
  x = FIXEDtoINT(from->x - to->x);
  z = FIXEDtoINT(from->z - to->z);
  spark->angle = Trig_Atan(from->z - to->z, from->x - to->x);
  spark->spin = (((Random16() & 2) - 1) * (Random16() & 15)) << 8;
  spark->palette = palette;
  spark->motionFlags = 0;
  spark->originX = to->x;
  spark->originZ = to->z;
  distance = INTtoFIXED(Iwram_Sqrt(x * x + z * z));
  spark->radius = distance;
  spark->step = distance / 16;
  spark->height = from->y;
  spark->fall = (from->y - to->y) / 16;
  Actor_SetBlendMode(spark, 1);
  FieldActor_SetSpriteColor(spark, (u16)palette);
  Scene_SetActorSpritePriority(spark, 2);
  Actor_SelectAnimation(spark, 7);
  Actor_SetSpriteFlags(spark, 0);
  FieldVector_AddPolarOffsetFX(spark->radius, spark->angle, &spark->x);
  Actor_StartScript(spark, gAnemosInnerSanctum_SparkScript);
  AnemosInnerSanctum_SpiralIn(spark);
}

/// Rings a torch at x, y (16.16) with eight orbs of the given palette that
/// circle in on it at the given speed. Each orb keeps its circle in its actor's
/// fields: its centre in bounce (x) and gravity (y), its radius in sceneWord,
/// its angle in timer and its speed in userValue.
void AnemosInnerSanctum_SparkRing(x, y, palette, speed) s32 x;
s32 y;
s16 palette;
s16 speed;
{
  s32 i;

  for (i = 0; i != SPARK_RING_STEPS;) {
    struct Actor *orb =
        (FieldActor_Create(SPRITE_PARTICLE, x, y, toFIXED(TORCH_ROW_Z)));
    s32 angle = i * (TRIG_TURN / SPARK_RING_STEPS);

    FieldVector_AddPolarOffset(32, angle, &orb->x.fixed);
    orb->timer = angle;
    orb->userValue = speed;
    orb->sceneCounter = i;
    orb->sceneByte = palette;
    orb->bounce = x;
    orb->sceneWord = toFIXED(32);
    orb->gravity = y;
    orb->motionFlags = 0;
    Actor_SetBlendMode(orb, 1);
    FieldActor_SetSpriteColor(orb, (u16)palette);
    Scene_SetActorSpritePriority(orb, 2);
    Actor_SelectAnimation(orb, 7);
    Actor_SetSpriteFlags(orb, 0);
    Actor_StartScript(orb, gAnemosInnerSanctum_OrbScript);
    i += 2;
  }
}

/// Leaves a flame of the given palette burning on the torch at x and height y
/// on the torches' row.
void AnemosInnerSanctum_LightTorch(x, y, palette) s32 x;
s32 y;
s16 palette;
{
  struct Actor *flame;

  flame = FieldActor_Create(SPRITE_PARTICLE, INTtoFIXED(x), INTtoFIXED(y + 3),
                            toFIXED(TORCH_ROW_Z));
  flame->motionFlags = ACTOR_FLOATS;
  FieldActor_SetSpriteColor(flame, (u16)palette);
  Scene_SetActorSpritePriority(flame, 2);
  Actor_SelectAnimation(flame, 7);
  Actor_SetSpriteFlags(flame, 0);
}

#include "text/msg_ids.h"

extern u32 gAnemosInnerSanctum_DjinniRisesScript[];
extern s32 gAnemosInnerSanctum_DjinniSinksScript[];
extern s32 gAnemosInnerSanctum_PlateOffsets[][2];

/// Stepping onto an element's symbol in the Dragon Skin room: the leader
/// raises his arms in a pillar of light and a Djinni of the element rises over
/// him, and each of the element's Djinn the party has found sends a spark to
/// its own plate. Short of all eighteen, the plates and the light go out
/// again; with all eighteen, the element's torch over the north door is lit,
/// and with all four lit the door opens. Nothing if the torch is lit already
/// or the party has none of the element's Djinn.
void AnemosInnerSanctum_LightSymbol(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *leader = Actor_GetById(gMainRoutineState.leaderActorId);
  u16 alpha;
  s32 x, z;
  s32 targetX;
  s32 targetY;
  s16 type;
  s16 trigger;
  s32 index;
  s32 base;
  u8 count;
  s32 palette;
  s32 column, row, cellX, cellZ;
  s32 i;
  u8 last;
  struct Actor *djinni;

  Io_Read16(&REG_BLDALPHA, &alpha);
  targetX = 0;
  targetY = 0;
  type = state->stepTrigger - TRIGGER_VENUS_SYMBOL + SPRITE_VENUS_DJINNI;
  trigger = state->stepTrigger;
  index = trigger - TRIGGER_VENUS_SYMBOL;
  x = ((index & 1) << 8) + 408;
  base = 0;
  count = 0;
  palette = SPRITE_COLOR_NORMAL;
  z = (index & 2) * 3 * 32 + 168;
  switch (trigger) {
  case TRIGGER_VENUS_SYMBOL:
    base = FLAG_DJINNI(DJINNI_FLINT);
    targetX = VENUS_TORCH_X;
    targetY = VENUS_TORCH_Y;
    if (GameFlag_Get(FLAG_ANEMOS_VENUS_TORCH_LIT)) {
      GameFlag_Set(FLAG_ON_SYMBOL);
      return;
    }
    break;
  case TRIGGER_MERCURY_SYMBOL:
    palette = SPRITE_COLOR_BGR;
    base = FLAG_DJINNI(DJINNI_FIZZ);
    targetX = MERCURY_TORCH_X;
    targetY = MERCURY_TORCH_Y;
    if (GameFlag_Get(FLAG_ANEMOS_MERCURY_TORCH_LIT)) {
      GameFlag_Set(FLAG_ON_SYMBOL);
      return;
    }
    break;
  case TRIGGER_MARS_SYMBOL:
    palette = SPRITE_COLOR_RED_TINT;
    base = FLAG_DJINNI(DJINNI_FORGE);
    targetX = MARS_TORCH_X;
    targetY = MARS_TORCH_Y;
    if (GameFlag_Get(FLAG_ANEMOS_MARS_TORCH_LIT)) {
      GameFlag_Set(FLAG_ON_SYMBOL);
      return;
    }
    break;
  case TRIGGER_JUPITER_SYMBOL:
    palette = SPRITE_COLOR_RBG;
    base = FLAG_DJINNI(DJINNI_GUST);
    targetX = JUPITER_TORCH_X;
    targetY = JUPITER_TORCH_Y;
    if (GameFlag_Get(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
      GameFlag_Set(FLAG_ON_SYMBOL);
      return;
    }
    break;
  }

  /* FAKEMATCH(dead): a block that only declares the Djinn lists and the line's
   * ends after the blend weights, so they take the frame below them. */
  {
    u8 found[20];
    u8 order[20];
    struct Vector3 from;
    struct Vector3 to;

    for (i = (u16)base; i != (u16)base + DJINNI_SPARE_FIRST; i++) {
      if (GameFlag_Get(i)) {
        found[count] = i - base;
        count++;
      }
    }
    if (count == 0)
      return;

    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.8);
    FieldCamera_SetSpeed(0.8);
    Actor_WalkTo(gMainRoutineState.leaderActorId, x, z);
    FieldCamera_MoveToFX(INTtoFIXED(x), -1, INTtoFIXED(z), 1);
    FieldActor_WaitForMoveById(gMainRoutineState.leaderActorId);
    Actor_FaceDirectionAndWait(gMainRoutineState.leaderActorId, FACING_SOUTH);
    Actor_SetAnimation(gMainRoutineState.leaderActorId,
                       ANIM_LEADER_ARMS_RAISED);
    djinni = (FieldActor_Create((u16)type, leader->x.fixed,
                                leader->y.fixed + toFIXED(8), leader->z.fixed));
    djinni->motionFlags = 0;
    djinni->scaleY = FIXED_ONE / 10;
    djinni->scaleX = FIXED_ONE / 10;
    Actor_SetSpriteFlags((s32)djinni, 0);
    Actor_StartScript(djinni, gAnemosInnerSanctum_DjinniRisesScript);
    Actor_SetBlendMode(djinni, 1);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
    PaletteFade_Interpolate(60);
    BlendScript_Suspend();
    alpha = alpha & BLEND_ALPHA_TOP_MASK;
    REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
    cellX = x >> 4;
    cellZ = z >> 4;
    column = cellX;
    row = cellZ;
    MapGrid_CopyCellsTo(MIDDLE_LAYER_COLUMN + cellX - 1,
                        cellZ - 3 + LIT_PICTURE_ROWS,
                        MIDDLE_LAYER_COLUMN + cellX - 1, cellZ - 3, 3, 5);
    for (i = 0; i != alpha; i++) {
      REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
      System_Wait(8);
    }
    BlendScript_Resume();
    Event_PlaySound(SE_GLITTER);
    FieldEvent_Wait(48);

    last = count - 1;
    for (i = 0; i != count; i++) {
      s32 pick = Random16();
      pick &= last;

      order[i] = found[pick];
      found[pick] = found[last];
      last--;
    }
    for (i = 0; i != count;) {
      s32 slot;

      from.x = leader->x.fixed + INTtoFIXED(Random16() & 31) - toFIXED(16);
      from.y = leader->y.fixed + INTtoFIXED(Random16() & 15) + toFIXED(8);
      from.z = leader->z.fixed + INTtoFIXED(Random16() & 15) - toFIXED(8);
      slot = order[i];
      to.x = leader->x.fixed +
             INTtoFIXED(gAnemosInnerSanctum_PlateOffsets[slot][0]);
      to.y = leader->y.fixed;
      to.z = leader->z.fixed +
             INTtoFIXED(gAnemosInnerSanctum_PlateOffsets[slot][1]);
      AnemosInnerSanctum_SendSpark(&from, &to, palette);
      if (i & 1)
        Event_PlaySound(SE_SPARKS);
      i++;
      FieldEvent_Wait(8);
    }

    if (count != DJINNI_SPARE_FIRST) {
      FieldEvent_Wait(96);
      Actor_StartScript(djinni, gAnemosInnerSanctum_DjinniSinksScript);
      BlendScript_Suspend();
      alpha = REG_BLDALPHA & BLEND_ALPHA_TOP_MASK;
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
      PaletteFade_Interpolate(60);
      for (i = alpha; i != 0;) {
        REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
        i--;
        System_Wait(8);
      }
      for (i = 0; i != count;) {
        s32 slot = order[i];

        i++;
        MapGrid_CopyCellsTo(
            MIDDLE_LAYER_COLUMN, 0,
            FIELD_CELL(leader->x.fixed +
                       INTtoFIXED(gAnemosInnerSanctum_PlateOffsets[slot][0])) +
                MIDDLE_LAYER_COLUMN,
            FIELD_CELL(leader->z.fixed +
                       INTtoFIXED(gAnemosInnerSanctum_PlateOffsets[slot][1])),
            1, 1);
      }
      MapGrid_CopyCellsTo(MIDDLE_LAYER_COLUMN, 0,
                          MIDDLE_LAYER_COLUMN + column - 1, row - 3, 3, 5);
      BlendScript_Resume();
      Event_WaitTransitionFrames();
    } else {
      Event_SetFlag((s16)state->stepTrigger - TRIGGER_VENUS_SYMBOL +
                    FLAG_ANEMOS_VENUS_TORCH_LIT);
      Actor_StartScript(djinni, gAnemosInnerSanctum_DjinniSinksScript);
      FieldEvent_Wait(60);
      FieldCamera_SetSpeed(4);
      FieldCamera_MoveTo(536, CAMERA_KEEP, 120, 1);
      FieldCamera_WaitForArrival();
      Event_PlaySound(SE_SHIMMER);
      {
        FIXED ringX = INTtoFIXED(targetX);
        FIXED ringY = INTtoFIXED(targetY);

        AnemosInnerSanctum_SparkRing(ringX, ringY, palette,
                                     TRIG_TURN * 8 / 256);
        FieldEvent_Wait(10);
        AnemosInnerSanctum_SparkRing(ringX, ringY, palette,
                                     TRIG_TURN * 9 / 256);
        FieldEvent_Wait(10);
        AnemosInnerSanctum_SparkRing(ringX, ringY, palette,
                                     TRIG_TURN * 10 / 256);
      }
      FieldEvent_Wait(12);
      System_Wait(16);
      PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
      PaletteFade_Interpolate(1);
      System_Wait(1);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_Interpolate(32);
      Event_PlaySound(SE_FLASH);
      AnemosInnerSanctum_LightTorch(targetX, targetY, palette);
      FieldEvent_Wait(80);
      if (GameFlag_Get(FLAG_ANEMOS_VENUS_TORCH_LIT) &&
          GameFlag_Get(FLAG_ANEMOS_MERCURY_TORCH_LIT) &&
          GameFlag_Get(FLAG_ANEMOS_MARS_TORCH_LIT) &&
          GameFlag_Get(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
        // The north door opens.
        Event_SetFlag(FLAG_ANEMOS_NORTH_DOOR_OPEN);
        Event_PlaySound(SE_THUMP);
        MapGrid_CopyCellsTo(52, 7, 33, 7, 1, 2);
        System_Wait(5);
        MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
        MapGrid_SetCellHeight(toFIXED(536), toFIXED(136), 0, 0);
        System_Wait(60);
      }
    }

    Actor_Release(djinni);
    Event_SetFlag(FLAG_ON_SYMBOL);
    EventCamera_FollowActor(gMainRoutineState.leaderActorId, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(60);
    Event_End();
  }
}

/// Nothing visible happens as Felix steps off a symbol onto the floor
/// between them; the game lets the symbols be stepped onto again.
void AnemosInnerSanctum_StepOffSymbol(void) { GameFlag_Clear(FLAG_ON_SYMBOL); }

extern s16 gAnemosInnerSanctum_ElevatorBlocks[];
extern s16 gAnemosInnerSanctum_SwitchesBlocks[];
extern s16 gAnemosInnerSanctum_OrihalconChestBlocks[];

/// Nothing visible of its own happens as Felix starts to move a pillar with
/// Move; in the switches' and the Orihalcon chest's rooms, the game closes
/// the stretch of sand by the pillar's place while it is moving, so that a
/// mound of sand cannot pass under it, and opens a cell or two beside it.
void AnemosInnerSanctum_StartMovingBlock(void) {
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
    Actor_LiftTargetBlock((s32)gAnemosInnerSanctum_ElevatorBlocks);
  } else {
    if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
      Actor_LiftTargetBlock((s32)gAnemosInnerSanctum_SwitchesBlocks);
      Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72), toFIXED(440),
                              CELL_BEHAVIOR_WALL);
    } else {
      if (gMainRoutineState.next ==
          SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
        Actor_LiftTargetBlock((s32)gAnemosInnerSanctum_OrihalconChestBlocks);
        Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
                                toFIXED(344), CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
                                toFIXED(376), CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(296),
                                toFIXED(360), CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280),
                                toFIXED(360), CELL_BEHAVIOR_SAND);
        Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520),
                                toFIXED(632), CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520),
                                toFIXED(616), CELL_BEHAVIOR_SAND);
      }
    }
  }
}

/// Once Felix has moved a pillar with Move: a pillar left in the place it
/// belongs stays there on later visits, as a shortcut. In the switches' room,
/// the pillar moved onto the switch presses it with a click, and the door
/// west of it opens with a thump, for good.
///
/// The stretches of sand AnemosInnerSanctum_StartMovingBlock closed are
/// opened again. Each place is checked in half cells. In the switches' room
/// whichever pillar is set down on pillar B's place counts as pillar B, the
/// game trading the two actors' places while the third pillar is not on its
/// own.
void AnemosInnerSanctum_FinishMovingBlock(void) {
  struct Actor *block = gFieldState->movedBlock;
  s32 x;
  s32 z;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_PlaceMovedBlock();
  x = FIELD_HALF_CELL(block->x.fixed);
  z = FIELD_HALF_CELL(block->z.fixed);
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
    Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72), toFIXED(440),
                            CELL_BEHAVIOR_SAND);
    if (x != 8)
      Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(72), toFIXED(424),
                              CELL_BEHAVIOR_SAND);
  }
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
    Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280), toFIXED(344),
                            CELL_BEHAVIOR_NORMAL);
    Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(280), toFIXED(376),
                            CELL_BEHAVIOR_SAND);
    Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(296), toFIXED(360),
                            CELL_BEHAVIOR_NORMAL);
    Field_SetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, toFIXED(520), toFIXED(632),
                            CELL_BEHAVIOR_SAND);
  }
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
    if (x == 37 && z == 31) {
      GameFlag_Set(FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED);
      Actor_SetSpritePriority(ACTOR_ELEVATOR_FIRST_PILLAR, 2);
    }
    if (x == 55 && z == 95)
      GameFlag_Set(FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED);
  } else if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
    if (x == 7 && z == 53)
      GameFlag_Set(FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED);
    if (x == 67 && z == 45) {
      MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
      Actor_SetPosition(ACTOR_SWITCHES_SWITCH_MARKER, 0, 0);
      Event_PlaySound(SE_BUTTON_PRESS);
      FieldEvent_Wait(30);
      Event_PlaySound(SE_THUMP);
      MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
      FieldEvent_Wait(5);
      MapGrid_SetCellHeight(toFIXED(496), toFIXED(336), 0, 0);
      GameFlag_Set(FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED);
    }
    if (x == 93 && z == 57)
      GameFlag_Set(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED);
    if (x == 91 && z == 59) {
      if (!Event_GetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED)) {
        s32 fromX =
            INTtoFIXED(FIXEDtoINT(Actor_Get(ACTOR_SWITCHES_PILLAR_B)->x.fixed));

        Actor_SetPositionFX(ACTOR_SWITCHES_THIRD_PILLAR, fromX,
                            INTtoFIXED(FIXEDtoINT(
                                Actor_Get(ACTOR_SWITCHES_PILLAR_B)->z.fixed)));
        Actor_SetPosition(ACTOR_SWITCHES_PILLAR_B, 728, 472);
      }
      GameFlag_Set(FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED);
    }
  }
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST &&
      x == 33 && z == 45)
    GameFlag_Set(FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED);
  Event_End();
}

extern s32 gAnemosInnerSanctum_SwitchGlowScript[];

/// A round switch stepped on: it lights under the leader in a burst of blue
/// rings and its scene flag is set; once all seven are pressed, the room's
/// lift floats up with its arrow pointing up (FLAG_LIFT_READY), ready to
/// carry the party.
void AnemosInnerSanctum_PressRoundSwitch(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  struct Actor *actor;
  s32 all;
  s32 i;
  s32 id;

  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
    MapGrid_CopyCellsTo(17, 71, FIELD_CELL(leader->x.fixed),
                        FIELD_CELL(leader->z.fixed) + OVERHEAD_LAYER_ROW, 1, 1);
  else
    MapGrid_CopyCellsTo(54, 87, FIELD_CELL(leader->x.fixed),
                        FIELD_CELL(leader->z.fixed) + OVERHEAD_LAYER_ROW, 1, 1);
  Event_PlaySound(SE_DRIP);
  actor = FieldActor_Create(
      SPRITE_ENERGY_CHARGE,
      FIELD_CELL_ORIGIN(FIELD_CELL(leader->x.fixed)) + toFIXED(8), 0,
      FIELD_CELL_ORIGIN(FIELD_CELL(leader->z.fixed)) + toFIXED(8));
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = 0;
  FieldActor_SetSpriteColor(actor, SPRITE_COLOR_GBR);
  actor->scaleY = FIXED_ONE * 11 / 8;
  actor->scaleX = FIXED_ONE * 11 / 8;
  Scene_SetActorSpritePriority(actor, 2);
  Actor_SelectAnimation(actor, ENERGY_CHARGE_ANIMATION_BURST);
  Actor_SetSpriteFlags(actor, 0);
  Actor_StartScript(actor, gAnemosInnerSanctum_SwitchGlowScript);
  Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(state->stepTrigger -
                                          TRIGGER_FIRST_ROUND_SWITCH));
  all = Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(0));
  for (i = 1; i != ROUND_SWITCHES; i++)
    all &= Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(i));
  if (all) {
    Event_Start();
    FieldEvent_EndLastingPsynergy(0);
    FieldEvent_Wait(10);
    FieldCamera_SetSpeed(0.8);
    id = ACTOR_ELEVATOR_LIFT;
    if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
      id = ACTOR_ORIHALCON_CHEST_LIFT;
    FieldCamera_MoveToActorById(id, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    Event_PlaySound(SE_LEMURIAN_SHIP_LIFT_FAST);
    Actor_SetAnimation(id, ARROW_PANEL_ANIMATION_UP);
    Event_SetFlag(FLAG_LIFT_READY);
    Actor_Get(id)->motionFlags = ACTOR_FLOATS;
    FieldEvent_Wait(80);
    Event_End();
  }
}

/// Felix steps onto the lift once it floats ready: he walks to its middle,
/// turns to face south, and it carries the party up out of the room to the
/// room above.
void AnemosInnerSanctum_RideLiftUp(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  struct Actor *lift;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
    lift = Actor_Get(ACTOR_ORIHALCON_CHEST_LIFT);
    FieldEvent_SetExitRequest(
        ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST);
  } else {
    lift = Actor_Get(ACTOR_ELEVATOR_LIFT);
    FieldEvent_SetExitRequest(EXIT_UP_THE_LIFT);
  }
  {
    s32 x = (FIELD_CELL(leader->x.fixed) << 4) + 8;
    s32 z = (FIELD_CELL(leader->z.fixed) << 4) + 8;

    Actor_WalkToAndWait(gMainRoutineState.leaderActorId, x, z);
  }
  Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 10);
  FieldEvent_Wait(30);
  lift->motionFlags = 0;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  lift->speedLimit = toFIXED(2);
  lift->acceleration = toFIXED(1) / 32;
  Event_PlaySound(SE_KOLIMA_BRIDGE_POST);
  FieldActor_SetMoveTargetFX(lift, lift->x.fixed, lift->y.fixed + toFIXED(4),
                             lift->z.fixed);
  FieldActor_WaitForMove(lift);
  Scene_SetActorSpritePriority(leader, 1);
  Scene_SetActorSpritePriority(lift, 1);
  FieldActor_SetMoveTargetFX(lift, lift->x.fixed, lift->y.fixed + toFIXED(128),
                             lift->z.fixed);
  FieldEvent_Wait(80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Felix casts Sand standing on sand and sinks into it; as a mound of sand
/// he can then travel along the stretch of sand, where on foot he cannot
/// pass, until it wears off.
///
/// While it lasts the leader moves on the overhead layer, where the sand's
/// own cells are.
void AnemosInnerSanctum_SinkIntoSand(void) {
  struct Actor *actor;
  s32 behavior;

  actor = Actor_Get(gMainRoutineState.leaderActorId);
  behavior = FieldMapGrid_GetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD,
                                            actor->x.fixed, actor->z.fixed);
  if (behavior == CELL_BEHAVIOR_SAND) {
    actor->layer = MAPGRID_LAYER_OVERHEAD;
  }
}

/// Nothing visible of its own: as Sand wears off and Felix rises from the
/// sand, the game sets him walking on the ground's cells again.
void AnemosInnerSanctum_EndSand(void) {
  Actor_Get(gMainRoutineState.leaderActorId)->layer = MAPGRID_LAYER_GROUND;
}

/// Stepping onto the switch: the leader walks onto its middle, it sinks with a
/// click and the door west of it opens with a thump.
void AnemosInnerSanctum_PressSwitch(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_WalkToAndWait(gMainRoutineState.leaderActorId, 536, 360);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
  Actor_SetPosition(ACTOR_SWITCHES_SWITCH_MARKER, 0, 0);
  Event_PlaySound(SE_BUTTON_PRESS);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_SetFlag(FLAG_SWITCH_PRESSED);
  Event_End();
}

/// Stepping off the pressed switch to the east: it rises again with a click
/// and its door shuts with a thump.
void AnemosInnerSanctum_StepOffSwitch(void) {
  struct Actor *platform;

  platform = Actor_Get(ACTOR_SWITCHES_SWITCH_MARKER);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  platform->x.fixed = toFIXED(536);
  platform->z.fixed = toFIXED(360);
  platform->groundHeight = -toFIXED(14);
  platform->y.fixed = -toFIXED(14);
  MapGrid_CopyCellsTo(51, 22, 33, 22, 1, 1);
  Event_PlaySound(SE_BUTTON_PRESS);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(52, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_ClearFlag(FLAG_SWITCH_PRESSED);
  Event_End();
}

s32 CrackedTile_Step();

/// Defines a step trigger on a cracked floor: when a tile gives way under the
/// leader, the party leaves by exit `exit`, through the floor.
#define DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(name, exit)                        \
  void name(void) {                                                            \
    s32 fell;                                                                  \
                                                                               \
    fell = CrackedTile_Step();                                                 \
    if (fell != 0) {                                                           \
      FieldEvent_SetExitRequest(exit);                                         \
    }                                                                          \
  }

/// Stepping onto the first cracked floor: once a tile gives way, the party
/// falls through to the part of the room below.
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(AnemosInnerSanctum_FallThroughFirstCracks,
                                    THROUGH_FIRST_CRACKED_FLOOR)

/// As AnemosInnerSanctum_FallThroughFirstCracks, for the second floor.
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(AnemosInnerSanctum_FallThroughSecondCracks,
                                    THROUGH_SECOND_CRACKED_FLOOR)

extern s32 gAnemosInnerSanctum_DustScript[];

/// A mimic stone's update: each step the leader takes, it takes too, mirrored
/// east for west, scraping up dust, unless a wall stands in its way; on its
/// button or over a hole it stops and raises that trigger. Once the leader
/// goes back south of its row, it settles on its cell and may be started
/// again. sceneWord and acceleration hold the leader's last x and z.
void AnemosInnerSanctum_MimicLeader(struct Actor *self) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  FIXED next[2];
  FIXED ahead[3];
  struct Actor *dust;
  s32 behavior;

  if (FIXEDtoINT(Actor_Get(gMainRoutineState.leaderActorId)->z.fixed) >
      MIMIC_STONE_LINE_Z) {
    self->update = 0;
    Event_ClearFlag(FLAG_MIMIC_STONE_STARTED);
    self->x.fixed = (self->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    self->z.fixed = (self->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    return;
  }
  ahead[0] = next[0] = self->x.fixed - (leader->x.fixed - self->sceneWord);
  ahead[1] = leader->y.fixed;
  ahead[2] = next[1] = self->z.fixed + (leader->z.fixed - self->acceleration);
  if (self->sceneWord != leader->x.fixed ||
      self->acceleration != leader->z.fixed) {
    dust = (FieldActor_Create(SPRITE_PARTICLE, self->x.fixed, self->y.fixed,
                              self->z.fixed));
    dust->x.fixed += INTtoFIXED((Random16() & 15) - 8);
    dust->y.fixed += INTtoFIXED((Random16() & 31) - 4);
    dust->motionFlags = 0;
    FieldActor_SetSpriteColor(dust, self->sceneCounter);
    Scene_SetActorSpritePriority(dust, 2);
    Actor_SelectAnimation(dust, 1);
    Actor_SetSpriteFlags(dust, 0);
    Actor_StartScript(dust, gAnemosInnerSanctum_DustScript);
    self->sceneWord = leader->x.fixed;
    self->acceleration = leader->z.fixed;
    if ((gFrameCount & 3) == 0)
      Event_PlaySound(SE_DRIP);
  }
  if (FIXEDtoINT(ahead[2]) > MIMIC_STONE_LINE_Z)
    return;
  FieldVector_AddPolarOffset(7, DEGtoANG(180) - leader->facing, ahead);
  if (FieldMapGrid_GetCellBehaviorAt(self->layer, ahead[0], ahead[2]) ==
      CELL_BEHAVIOR_WALL)
    return;
  {
    FIXED x = next[0];

    self->x.fixed = x;
    self->z.fixed = next[1];
    behavior = FieldMapGrid_GetCellBehaviorAt(self->layer, x, next[1]);
  }
  if (behavior == TRIGGER_MIMIC_STONE_BUTTON) {
    gFieldState->raisedTrigger = TRIGGER_MIMIC_STONE_ON_BUTTON;
    self->update = 0;
  } else if (behavior == TRIGGER_MIMIC_STONE_HOLE) {
    gFieldState->raisedTrigger = TRIGGER_MIMIC_STONE_IN_HOLE;
    self->update = 0;
  }
}

#include "text/msg_ids.h"

void Actor_PlaceBlocks(void);

void Teleport_StartFastShimmer(void);

/// Stepping onto the floor before a mimic stone: the stone of the room the
/// party came into starts to mirror the leader's steps.
void AnemosInnerSanctum_StartMimicStone(void) {
  struct Actor *actor;

  if (gMainRoutineState.entrance == ENTRANCE_BY_MIMIC_STONE_A)
    actor = Actor_Get(ACTOR_MIMIC_STONE_A);
  else if (gMainRoutineState.entrance == ENTRANCE_BY_MIMIC_STONE_B)
    actor = Actor_Get(ACTOR_MIMIC_STONE_B);
  else
    actor = Actor_Get(ACTOR_MIMIC_STONE_C);
  if (Actor_Get(gMainRoutineState.leaderActorId)->z.part[0].integer <
      MIMIC_STONE_LINE_Z) {
    actor->motionFlags = 0;
    actor->update = AnemosInnerSanctum_MimicLeader;
    actor->sceneWord = Actor_Get(gMainRoutineState.leaderActorId)->x.fixed;
    actor->acceleration = Actor_Get(gMainRoutineState.leaderActorId)->z.fixed;
    Event_SetFlag(FLAG_MIMIC_STONE_STARTED);
  }
}

/// A mimic stone falls into a hole: it slides to the hole's middle and drops in
/// with a puff of dust, hidden below the hole's edge by a crate's silhouette
/// drawn as an object window, and is taken away.
void AnemosInnerSanctum_DropMimicStone(void) {
  struct Actor *stone;
  struct Actor *mask;
  struct FieldSprite *sprite;
  FIXED position[3];

  if (gMainRoutineState.entrance == ENTRANCE_BY_MIMIC_STONE_A)
    stone = Actor_Get(ACTOR_MIMIC_STONE_A);
  else if (gMainRoutineState.entrance == ENTRANCE_BY_MIMIC_STONE_B)
    stone = Actor_Get(ACTOR_MIMIC_STONE_B);
  else
    stone = Actor_Get(ACTOR_MIMIC_STONE_C);
  position[0] = (stone->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[2] = (stone->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  stone->acceleration = toFIXED(1);
  stone->speedLimit = toFIXED(1);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldActor_SetMoveTargetFX(stone, position[0], 0, position[2]);
  mask = Actor_CreateSilhouette(stone->x.fixed, 0, position[2] + toFIXED(59),
                                SPRITE_CRATE);
  mask->scaleX = FIXED_ONE * 3 / 2;
  mask->scaleY = FIXED_ONE * 3 / 2;
  sprite = mask->sprite;
  sprite->objectMode = OBJ_MODE_WINDOW;
  REG_WINOUT = WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
               WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_CLR;
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Event_PlaySound(SE_DUST_PUFF);
  stone->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_WaitUntilLanded(stone);
  stone->x.fixed = 0;
  stone->y.fixed = 0;
  stone->z.fixed = 0;
  // Stone A goes too, whichever stone fell; the room places it again when it
  // is next entered.
  Actor_SetPosition(ACTOR_MIMIC_STONE_A, 0, 0);
  Actor_Release(mask);
  Event_End();
}

/// A mimic stone reaches its button: it settles on the button's middle with a
/// thump, and its room's north door opens, its flag keeping both so.
void AnemosInnerSanctum_OpenMimicStoneDoor(void) {
  struct Actor *stone;
  s32 column;
  FIXED target[3];

  if (gMainRoutineState.entrance == ENTRANCE_BY_MIMIC_STONE_A) {
    stone = Actor_Get(ACTOR_MIMIC_STONE_A);
    Event_SetFlag(FLAG_MIMIC_STONE_A_ON_BUTTON);
    column = 11;
  } else if (gMainRoutineState.entrance == ENTRANCE_BY_MIMIC_STONE_B) {
    stone = Actor_Get(ACTOR_MIMIC_STONE_B);
    Event_SetFlag(FLAG_MIMIC_STONE_B_ON_BUTTON);
    column = 26;
  } else {
    stone = Actor_Get(ACTOR_MIMIC_STONE_C);
    Event_SetFlag(FLAG_MIMIC_STONE_C_ON_BUTTON);
    column = 53;
  }
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  target[0] = (stone->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  target[2] = (stone->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  stone->acceleration = toFIXED(1);
  stone->speedLimit = toFIXED(1);
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldActor_SetMoveTargetFX(stone, target[0], 0, target[2]);
  FieldActor_WaitForMove(stone);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(18, 4, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_CopyCellsTo(18, 7, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_SetCellHeight(FIELD_CELL_ORIGIN(column), toFIXED(88), 0, 0);
  Event_End();
}

/// The first time Felix comes up to Charon's slate, the room's colours warm
/// and the slate speaks: the power of the land to create a great darkness
/// is for whoever attacks it.
void AnemosInnerSanctum_ApproachCharonSlate(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(60);
  FieldEvent_Wait(60);
  // "We inherited the power of the land to create a great darkness. If you
  // crave this power, attack this slate!"
  FieldMessage_ShowCentered((s32)MsgSummonInheritedEarthDarkness, 0, 6);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(60);
  FieldEvent_Wait(60);
  Event_SetFlag(FLAG_ANEMOS_CHARON_SLATE_SPOKE);
  Event_End();
}

extern struct Trigger gAnemosInnerSanctum_Triggers[];
extern struct Trigger gAnemosInnerSanctum_DragonSkinChestTriggers[];
extern struct Trigger gAnemosInnerSanctum_ElevatorTriggers[];
extern struct Trigger gAnemosInnerSanctum_SwitchesTriggers[];
extern struct Trigger gAnemosInnerSanctum_OrihalconChestTriggers[];
extern struct Trigger gAnemosInnerSanctum_GuardianTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
    return gAnemosInnerSanctum_DragonSkinChestTriggers;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
    return gAnemosInnerSanctum_ElevatorTriggers;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
    return gAnemosInnerSanctum_SwitchesTriggers;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
    return gAnemosInnerSanctum_OrihalconChestTriggers;
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
    return gAnemosInnerSanctum_GuardianTriggers;
  return gAnemosInnerSanctum_Triggers;
}

/// Felix casts Teleport on the circle in the middle of the symbols' room:
/// sparks fly as the colours cycle, and the party teleports back to
/// Contigo.
void AnemosInnerSanctum_TeleportOut(void) {
  Teleport_StartShimmer();
  Teleport_PlaySparks();
  gMainRoutineState.teleportState = TELEPORT_ARRIVING;
  Event_SetDestination(SCENE_CONTIGO,
                       CONTIGO_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST);
  FieldEvent_Wait(40);
  Teleport_StopShimmer();
}

extern s16 gAnemosInnerSanctum_NorthDoorFrames[];
extern struct MapGridCellAnimation gAnemosInnerSanctum_MimicStoneDoors[];

/// Plays a door's map edit at x, y.
static __inline__ void AnemosInnerSanctum_AnimateDoor(const void *steps, s32 x,
                                                      s32 y) {
  MapGrid_PlayMapEdit(steps, x, y);
}

/// Unused: stills every placed actor, opens the north door or a mimic stone's
/// door by the step trigger's number, and walks the leader out by the exit of
/// that number.
UNUSED void AnemosInnerSanctum_EnterTheRoom(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 i;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  for (i = ACTOR_FIRST_PLACED; (u32)i < FIELD_TREASURE_ACTOR; i++) {
    struct Actor *actor = Actor_Get(i);

    if (actor != NULL)
      actor->motionFlags = 0;
  }
  Event_PlaySound(SE_DOOR_OPEN);
  if (gMainRoutineState.next == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
    MapGrid_AnimateCells(gAnemosInnerSanctum_NorthDoorFrames, 33, 7);
  } else {
    s32 index = state->stepTrigger / 2 - 1;

    AnemosInnerSanctum_AnimateDoor(
        gAnemosInnerSanctum_MimicStoneDoors[index].steps,
        gAnemosInnerSanctum_MimicStoneDoors[index].x,
        gAnemosInnerSanctum_MimicStoneDoors[index].y);
  }
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_SetDestinationOffset(gMainRoutineState.leaderActorId, 0, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(state->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The party arrives up the lift in the elevator room: it rises the last of
/// the way into the room with Felix on it and stays there.
void AnemosInnerSanctum_ArriveOnElevatorLift(void) {
  struct Actor *lift = Actor_Get(ACTOR_ELEVATOR_LIFT);

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetAnimation(ACTOR_ELEVATOR_LIFT, ARROW_PANEL_ANIMATION_UP);
  lift->y.fixed = -toFIXED(64);
  Actor_SetPosition(ACTOR_ELEVATOR_LIFT, 728, 120);
  Actor_SetPosition(gMainRoutineState.leaderActorId, 728, 120);
  Actor_Get(gMainRoutineState.leaderActorId)->y.fixed = -toFIXED(48);
  Event_StartScreenTransition();
  lift->acceleration = toFIXED(1);
  lift->speedLimit = toFIXED(1);
  FieldActor_SetMoveTargetFX(lift, lift->x.fixed, lift->y.fixed + toFIXED(48),
                             lift->z.fixed);
  FieldActor_WaitForMove(lift);
  Event_SetFlag(FLAG_ELEVATOR_LIFT_RISEN);
  Event_End();
}

/// As AnemosInnerSanctum_ArriveOnElevatorLift, on the guardian room's lift.
void AnemosInnerSanctum_ArriveOnGuardianLift(void) {
  struct Actor *lift = Actor_Get(ACTOR_GUARDIAN_LIFT);

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  lift->motionFlags = 0;
  lift->y.fixed = -toFIXED(64);
  Actor_Get(gMainRoutineState.leaderActorId)->y.fixed = -toFIXED(48);
  Event_StartScreenTransition();
  lift->acceleration = toFIXED(1);
  lift->speedLimit = toFIXED(1);
  FieldActor_SetMoveTargetFX(lift, lift->x.fixed, lift->y.fixed + toFIXED(48),
                             lift->z.fixed);
  FieldActor_WaitForMove(lift);
  Event_SetFlag(FLAG_GUARDIAN_LIFT_RISEN);
  Event_End();
}

s32 CrackedTile_Find();
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
extern u16 gTeleportCircle_PaletteSlots[];
extern u16 gTeleportCircle_Colors[];
extern u16 gTeleportCircle_ShimmerColors[];
extern u16 gTeleportCircle_ShimmerSlots[];

/// Sets up the room the party comes into: the Dragon Skin room's lit torches
/// and opened north door; the pillars left in their places; the lifts, below
/// or above as they last carried the party, and the round switches already
/// pressed; the cracked floors; and the mimic stones resting on their
/// buttons, their doors open.
s32 Scene_Initialize(void) {
  s32 scene;
  s32 entranceHalfword;
  s32 entrance;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  scene = gMainRoutineState.next;
  if (scene == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
    Palette_SetColorsFade(
        (s32)gTeleportCircle_PaletteSlots, (s32)gTeleportCircle_Colors,
        (s32)gTeleportCircle_ShimmerColors, (s32)gTeleportCircle_ShimmerSlots);
    if (Event_GetFlag(FLAG_ANEMOS_VENUS_TORCH_LIT)) {
      AnemosInnerSanctum_LightTorch(VENUS_TORCH_X, VENUS_TORCH_Y,
                                    SPRITE_COLOR_NORMAL);
      MapGrid_CopyCellsTo(85, 34, 85, 8, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_MERCURY_TORCH_LIT)) {
      AnemosInnerSanctum_LightTorch(MERCURY_TORCH_X, MERCURY_TORCH_Y,
                                    SPRITE_COLOR_BGR);
      MapGrid_CopyCellsTo(101, 34, 101, 8, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_MARS_TORCH_LIT)) {
      AnemosInnerSanctum_LightTorch(MARS_TORCH_X, MARS_TORCH_Y,
                                    SPRITE_COLOR_RED_TINT);
      MapGrid_CopyCellsTo(85, 46, 85, 20, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_JUPITER_TORCH_LIT)) {
      AnemosInnerSanctum_LightTorch(JUPITER_TORCH_X, JUPITER_TORCH_Y,
                                    SPRITE_COLOR_RBG);
      MapGrid_CopyCellsTo(101, 46, 101, 20, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_NORTH_DOOR_OPEN)) {
      MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
      MapGrid_SetCellHeight(toFIXED(536), toFIXED(136), 0, 0);
    }
  } else if (scene == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
    FieldActor_SetSpritePriority(ACTOR_ELEVATOR_FIRST_PILLAR, 1);
    if (Event_GetFlag(FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED)) {
      Actor_SetPosition(ACTOR_ELEVATOR_FIRST_PILLAR, 296, 248);
      FieldActor_SetSpritePriority(ACTOR_ELEVATOR_FIRST_PILLAR, 2);
    }
    if (Event_GetFlag(FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED))
      Actor_SetPosition(ACTOR_ELEVATOR_SECOND_PILLAR, 440, 760);
    Call1((void (*)())Actor_PlaceBlocks,
          (s32)gAnemosInnerSanctum_ElevatorBlocks);
    Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Actor_Get(ACTOR_ELEVATOR_LIFT)->motionFlags = 0;
    Actor_Get(ACTOR_ELEVATOR_LIFT)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
    if (gMainRoutineState.entrance ==
            WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST &&
        Event_GetFlag(FLAG_ELEVATOR_LIFT_RISEN)) {
      Actor_SetAnimation(ACTOR_ELEVATOR_LIFT, ARROW_PANEL_ANIMATION_UP);
      Actor_SetPosition(ACTOR_ELEVATOR_LIFT, 728, 120);
    }
    if (gMainRoutineState.entrance == ENTRANCE_UP_THE_LIFT &&
        GameFlag_Get(FLAG_ELEVATOR_LIFT_RISEN) == 0)
      AnemosInnerSanctum_ArriveOnElevatorLift();
    if (gMainRoutineState.entrance == WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST) {
      if (Event_GetFlag(FLAG_ELEVATOR_LIFT_RISEN))
        Actor_SetPosition(ACTOR_ELEVATOR_LIFT, 0, 0);
      if (Event_GetFlag(FLAG_LIFT_READY))
        Actor_Get(ACTOR_ELEVATOR_LIFT)->motionFlags = ACTOR_FLOATS;
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(0)))
        MapGrid_CopyCellsTo(54, 87, 44, 86, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(1)))
        MapGrid_CopyCellsTo(54, 87, 46, 86, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(2)))
        MapGrid_CopyCellsTo(54, 87, 43, 87, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(3)))
        MapGrid_CopyCellsTo(54, 87, 45, 87, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(4)))
        MapGrid_CopyCellsTo(54, 87, 47, 87, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(5)))
        MapGrid_CopyCellsTo(54, 87, 44, 88, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(6)))
        MapGrid_CopyCellsTo(54, 87, 46, 88, 1, 1);
      Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
    }
  } else if (scene == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
    struct Actor *marker = Actor_Get(ACTOR_SWITCHES_SWITCH_MARKER);

    marker->motionFlags = ACTOR_FALLS;
    marker->groundHeight = -toFIXED(14);
    marker->y.fixed = -toFIXED(14);
    marker->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
    Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Actor_Get(ACTOR_SWITCHES_BOULDER)->sceneCounter = 1;
    if (Event_GetFlag(FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED))
      Actor_SetPosition(ACTOR_SWITCHES_FIRST_PILLAR, 56, 424);
    if (Event_GetFlag(FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED)) {
      Actor_SetPosition(ACTOR_SWITCHES_SECOND_PILLAR, 536, 360);
      MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
      MapGrid_SetCellHeight(toFIXED(496), toFIXED(336), 0, 0);
    }
    if (Event_GetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED))
      Actor_SetPosition(ACTOR_SWITCHES_THIRD_PILLAR, 744, 456);
    if (Event_GetFlag(FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED)) {
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
          !Event_GetFlag(FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED))
        Actor_SetPositionFX(
            ACTOR_SWITCHES_THIRD_PILLAR,
            INTtoFIXED(Actor_Get(ACTOR_SWITCHES_PILLAR_B)->x.part[0].integer),
            INTtoFIXED(Actor_Get(ACTOR_SWITCHES_PILLAR_B)->z.part[0].integer));
      Actor_SetPosition(ACTOR_SWITCHES_PILLAR_B, 728, 472);
    }
    Call1((void (*)())Actor_PlaceBlocks,
          (s32)gAnemosInnerSanctum_SwitchesBlocks);
  } else if (scene == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
    if (Event_GetFlag(FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED))
      Actor_SetPosition(ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, 264, 360);
    Call1((void (*)())Actor_PlaceBlocks,
          (s32)gAnemosInnerSanctum_OrihalconChestBlocks);
    entranceHalfword = (u16)gMainRoutineState.entrance;
    entrance = gMainRoutineState.entrance;
    if (entrance ==
        ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST) {
      CrackedTile_Find(TRIGGER_FIRST_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
                       FLAG_AREA_TEMP_80, 18, 68);
      if (Event_GetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0) {
        Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
            ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
        Actor_Get(ACTOR_ORIHALCON_CHEST_LIFT)->motionFlags = 0;
        Actor_Get(ACTOR_ORIHALCON_CHEST_LIFT)->priorityFlags |=
            ACTOR_PRIORITY_PLATFORM;
      } else {
        Actor_SetPosition(ACTOR_ORIHALCON_CHEST_LIFT, 0, 0);
        Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(0));
        Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(1));
        Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(2));
        Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(3));
        Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(4));
        Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(5));
        Event_SetFlag(FLAG_ROUND_SWITCH_PRESSED(6));
      }
      if (Event_GetFlag(FLAG_LIFT_READY))
        Actor_Get(ACTOR_ORIHALCON_CHEST_LIFT)->motionFlags = ACTOR_FLOATS;
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(0)))
        MapGrid_CopyCellsTo(17, 71, 8, 69, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(5)))
        MapGrid_CopyCellsTo(17, 71, 6, 71, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(1)))
        MapGrid_CopyCellsTo(17, 71, 5, 75, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(2)))
        MapGrid_CopyCellsTo(17, 71, 8, 75, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(3)))
        MapGrid_CopyCellsTo(17, 71, 11, 75, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(4)))
        MapGrid_CopyCellsTo(17, 71, 8, 73, 1, 1);
      if (Event_GetFlag(FLAG_ROUND_SWITCH_PRESSED(6)))
        MapGrid_CopyCellsTo(17, 71, 10, 71, 1, 1);
    } else if (entrance == THROUGH_FIRST_CRACKED_FLOOR) {
      CrackedTile_Find(TRIGGER_FIRST_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
                       FLAG_AREA_TEMP_80, 18, 68);
      CrackedTile_PlaceLanding(
          22, 0,
          SCENE_TRANSITION(TRANSITION_WINDOW,
                           WINDOW_TRANSITION_INTERLACED_WIPE));
    } else if (entrance == THROUGH_SECOND_CRACKED_FLOOR) {
      CrackedTile_Find(TRIGGER_SECOND_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
                       FLAG_AREA_TEMP_80, 18, 68);
      if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
        CrackedTile_PlaceLanding(
            36, -36,
            SCENE_TRANSITION(TRANSITION_WINDOW,
                             WINDOW_TRANSITION_INTERLACED_WIPE));
    } else if ((u16)(entranceHalfword -
                     WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST) <= 1) {
      CrackedTile_Find(TRIGGER_SECOND_CRACKED_FLOOR, FLAG_SCENE_TEMP_80,
                       FLAG_AREA_TEMP_80, 18, 68);
    }
  } else if (scene == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN) {
    struct MapState *map = Ram_HeapSlots->mapGrid;

    switch (gMainRoutineState.entrance) {
    case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST:
      if (Event_GetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0)
        Actor_SetPosition(ACTOR_GUARDIAN_LIFT, 0, 0);
    case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST:
      Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
      Actor_SetAnimation(ACTOR_GUARDIAN_LIFT, ARROW_PANEL_ANIMATION_UP);
      Actor_Get(ACTOR_GUARDIAN_LIFT)->motionFlags = 0;
      Actor_Get(ACTOR_GUARDIAN_LIFT)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
      Actor_SetAnimation(ACTOR_GUARDIAN_LIFT, ARROW_PANEL_ANIMATION_UP);
      if (Event_GetFlag(FLAG_GUARDIAN_LIFT_RISEN) == 0 &&
          gMainRoutineState.entrance ==
              ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST)
        AnemosInnerSanctum_ArriveOnGuardianLift();
      else
        Actor_Get(ACTOR_GUARDIAN_LIFT)->y.fixed = -toFIXED(16);
      break;
    case ENTRANCE_BY_MIMIC_STONE_A:
    case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST:
      if (Event_GetFlag(FLAG_MIMIC_STONE_A_ON_BUTTON)) {
        Actor_SetPosition(ACTOR_MIMIC_STONE_A, 120, 120);
        Event_SetFlag(FLAG_MIMIC_STONE_STARTED);
      } else {
        MapGrid_CopyCellsTo(18, 1, 11, 4, 1, 2);
        MapGrid_SetCellHeight(toFIXED(184), toFIXED(88), 0, 2);
      }
      break;
    case ENTRANCE_BY_MIMIC_STONE_B:
    case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST:
      if (Event_GetFlag(FLAG_MIMIC_STONE_B_ON_BUTTON)) {
        Actor_SetPosition(ACTOR_MIMIC_STONE_B, 552, 120);
        Event_SetFlag(FLAG_MIMIC_STONE_STARTED);
      } else {
        MapGrid_CopyCellsTo(18, 1, 26, 4, 1, 2);
        MapGrid_SetCellHeight(toFIXED(424), toFIXED(88), 0, 2);
      }
      break;
    case ENTRANCE_BY_MIMIC_STONE_C:
    case ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST:
      if (Event_GetFlag(FLAG_MIMIC_STONE_C_ON_BUTTON)) {
        Actor_SetPosition(ACTOR_MIMIC_STONE_C, 792, 120);
        Event_SetFlag(FLAG_MIMIC_STONE_STARTED);
      } else {
        MapGrid_CopyCellsTo(18, 1, 53, 4, 1, 2);
        MapGrid_SetCellHeight(toFIXED(856), toFIXED(88), 0, 2);
      }
      map->minX += toFIXED(8);
      map->maxX += toFIXED(8);
      break;
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "../field/actor_set_sprite_priority_b.c"

/// An effect's update: moves it by its velocity, grows it by its scale rates
/// and spins its sprite.
void Effect_MoveLocal(union FieldObject *object) {
  s32 x;
  s32 velocityX;
  struct FieldSprite *sprite;

  x = object->effect.x;
  velocityX = object->effect.velocity.x;
  sprite = object->effect.sprite;

  object->effect.x = x + velocityX;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  sprite->rotation += object->effect.spin;
}

extern s32 gFieldEffect_DurationScripts[];
void Actor_SetSpritePriorityB(struct Actor *actor, s32 priority);
/// As FieldEffect_Spawn, the effect taking the leader's sprite priority and
/// moved by Effect_MoveLocal: the shared spawner's update and priority setter
/// are fixed, so this copy is written out too.
void FieldEffect_SpawnLocal(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                            s32 velocityZ, u32 flags,
                            const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineState.leaderActorId));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect =
        (struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = Effect_MoveLocal;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Actor_SetSpritePriorityB(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpritePriorityB(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    FIXED targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[EFFECT_SCRIPT_REPEAT_COUNT];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    } else {
      effect->scaleRateX = (targetX - FIXED_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepth
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF
#undef CELL_PUSH_SLOT_ENTRY
/// The leader lands from the cracked floor above in a ring of dust, as Mars
/// Lighthouse's lands from its cracks (MarsLighthouseEntrance_LandFromCrack).
void CrackedTile_DropIn(void) {
  struct Actor *leader = Actor_Get(gMainRoutineState.leaderActorId);
  u32 i;
  struct EffectOptions options;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 16 / 25;
  options.startScaleY = FIXED_ONE * 16 / 25;
  for (i = 0; i <= DUST_RING_PUFFS; i++) {
    s32 angle;
    s32 velocity[3];
    FIXED cosine;

    angle = i * (TRIG_TURN / DUST_RING_PUFFS);
    cosine = Trig_Cos(angle);
    velocity[0] = cosine;
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16() * (toFIXED(3) / 4) >> 16) - (toFIXED(3) / 8);
    velocity[2] += (Random16() * (toFIXED(3) / 8) >> 16) - (toFIXED(3) / 16);
    FieldEffect_SpawnLocal(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                           velocity[0], velocity[1], velocity[2],
                           EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                               EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
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
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_STAND);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

// clang-format off
// The same three lifetimes as gFieldEffect_ShortScript, MediumScript and
// LongScript (shared/field_effect_duration_scripts.c), under other names.
u32 gAnemosInnerSanctum_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gAnemosInnerSanctum_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gAnemosInnerSanctum_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctum_SwitchGlowScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctum_OrbScript[] = {
    SC_CALL(AnemosInnerSanctum_CircleOrb),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctum_DustScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 10),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_RELEASE,
    SC_END,
};

u32 gAnemosInnerSanctum_DjinniRisesScript[] = {
    SC_WAIT(50),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 10),
    SC_ADD(SW_Y, toFIXED(3)),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_MOTION, ACTOR_FLOATS),
    SC_END,
};

s32 gAnemosInnerSanctum_DjinniSinksScript[] = {
    SC_WAIT(30),
    SC_SET(SW_MOTION, 0),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 10),
    SC_ADD(SW_Y, -toFIXED(3)),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_END,
};

u32 gAnemosInnerSanctum_FadingSparkScript[] = {
    SC_WAIT(3),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctum_SparkScript[] = {
    SC_SET(SW_SCALE_X, FIXED_ONE * 2),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 2),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 10),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_CALL((s32)AnemosInnerSanctum_SpiralIn),
    SC_CALL((s32)AnemosInnerSanctum_LightPlate),
    SC_RELEASE,
    SC_END,
};

/// The blocks Move pushes in each room (Actor_LiftTargetBlock), ended by -1:
/// the elevator room's two pillars.
s16 gAnemosInnerSanctum_ElevatorBlocks[] = {
    ACTOR_ELEVATOR_FIRST_PILLAR, ACTOR_ELEVATOR_SECOND_PILLAR, -1,
};

/// The switches room's five pillars.
s16 gAnemosInnerSanctum_SwitchesBlocks[] = {
    ACTOR_SWITCHES_FIRST_PILLAR, ACTOR_SWITCHES_SECOND_PILLAR,
    ACTOR_SWITCHES_PILLAR_A,     ACTOR_SWITCHES_PILLAR_B,
    ACTOR_SWITCHES_THIRD_PILLAR, -1,
};

/// The Orihalcon chest room's two pillars.
s16 gAnemosInnerSanctum_OrihalconChestBlocks[] = {
    ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, -1,
};

/// Where the sparks fly to round the leader as a symbol lights, in pixels from
/// him in x and z.
s32 gAnemosInnerSanctum_PlateOffsets[][2] = {
    {-48, -32}, {48, -32}, {-64, -16}, {64, -16}, {-32, -16}, {32, -16},
    {-48, 0}, {48, 0}, {-32, 0}, {32, 0}, {-64, 16}, {64, 16}, {-32, 16},
    {32, 16}, {-48, 32}, {48, 32}, {-16, 32}, {16, 32},
};

#include "shared/field_effect_duration_scripts.c"

/// The field effects' scripts by how long they last, which FieldEffect_Spawn
/// picks by its flags (EFFECT_SCRIPT_MASK).
s32 gAnemosInnerSanctum_DurationScripts[] = {
    (s32)gAnemosInnerSanctum_ShortScript,
    (s32)gAnemosInnerSanctum_MediumScript,
    (s32)gAnemosInnerSanctum_LongScript,
};

#define TELEPORT_CIRCLE_FIRST_ENTRY 89
#include "shared/teleport_circle_palette.c"

s32 gAnemosInnerSanctum_NoShadowScript[] = {
    SC_CALL((s32)AnemosInnerSanctum_ClearScriptedActorSpriteFlags),
    SC_END,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(AnemosInnerSanctumDragonSkinChest),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_CONTIGO, Contigo, CONTIGO_FROM_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumElevator),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_NORTHWEST_TO_SOUTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHWEST_TO_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_NORTHWEST_TO_EAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_EAST_TO_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_NORTHWEST_TO_ABOVE_THE_LIFT, AnemosInnerSanctumElevator, WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_SWITCHES, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHWEST_TO_NORTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_NORTHWEST_TO_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHWEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHWEST_TO_EAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_EAST_TO_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_WEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHWEST_TO_SOUTHEAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHEAST_TO_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST, AnemosInnerSanctumElevator, WAY_ELEVATOR_EAST_TO_ROUND_SWITCHES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EXIT_UP_THE_LIFT, AnemosInnerSanctumElevator, ENTRANCE_UP_THE_LIFT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_EAST_TO_NORTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_NORTHWEST_TO_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_EAST_TO_SOUTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHWEST_TO_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_EAST_TO_ROUND_SWITCHES, AnemosInnerSanctumElevator, WAY_ELEVATOR_ROUND_SWITCHES_TO_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_SOUTHEAST_TO_SOUTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_SOUTHWEST_TO_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ELEVATOR_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ELEVATOR_ABOVE_THE_LIFT_TO_NORTHWEST, AnemosInnerSanctumElevator, WAY_ELEVATOR_NORTHWEST_TO_ABOVE_THE_LIFT, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumSwitches),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_NORTH_TO_MIDDLE, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHEAST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_NORTHEAST_TO_MIDDLE_WEST, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_NORTHEAST_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_NORTHEAST_TO_MIDDLE_EAST, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_NORTHEAST_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_WEST_TO_MIDDLE, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTH, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ELEVATOR, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_SWITCHES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHWEST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_WEST, AnemosInnerSanctumSwitches, WAY_SWITCHES_WEST_TO_MIDDLE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_SOUTH, AnemosInnerSanctumSwitches, WAY_SWITCHES_SOUTH_TO_MIDDLE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_NORTH, AnemosInnerSanctumSwitches, WAY_SWITCHES_NORTH_TO_MIDDLE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_NORTHEAST_WEST, AnemosInnerSanctumSwitches, WAY_SWITCHES_NORTHEAST_TO_MIDDLE_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_MIDDLE_TO_NORTHEAST_EAST, AnemosInnerSanctumSwitches, WAY_SWITCHES_NORTHEAST_TO_MIDDLE_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_SWITCHES_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_SWITCHES_SOUTH_TO_MIDDLE, AnemosInnerSanctumSwitches, WAY_SWITCHES_MIDDLE_TO_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumOrihalconChest),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTH, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_NORTHEAST_TO_SOUTHEAST, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_SOUTHEAST_TO_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_SOUTHEAST_TO_CRACKED_FLOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE, AnemosInnerSanctumEntrance, ANEMOS_INNER_SANCTUM_ENTRANCE_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHEAST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_SOUTHEAST_TO_CRACKED_FLOOR, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_CRACKED_FLOOR_TO_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_ORIHALCON_CHEST_SOUTHEAST_TO_NORTHEAST, AnemosInnerSanctumOrihalconChest, WAY_ORIHALCON_CHEST_NORTHEAST_TO_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(THROUGH_FIRST_CRACKED_FLOOR, AnemosInnerSanctumOrihalconChest, THROUGH_FIRST_CRACKED_FLOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(THROUGH_SECOND_CRACKED_FLOOR, AnemosInnerSanctumOrihalconChest, THROUGH_SECOND_CRACKED_FLOOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST, AnemosInnerSanctumGuardian, ANEMOS_INNER_SANCTUM_GUARDIAN_FROM_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumGuardian),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST, AnemosInnerSanctumOrihalconChest, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST, AnemosInnerSanctumSwitches, ANEMOS_INNER_SANCTUM_SWITCHES_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_GUARDIAN_SOUTH_TO_SOUTHEAST, AnemosInnerSanctumGuardian, WAY_GUARDIAN_SOUTHEAST_TO_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_EAST, AnemosInnerSanctumElevator, ANEMOS_INNER_SANCTUM_ELEVATOR_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST, AnemosInnerSanctumDragonSkinChest, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_FROM_ANEMOS_INNER_SANCTUM_GUARDIAN, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(WAY_GUARDIAN_SOUTHEAST_TO_SOUTH, AnemosInnerSanctumGuardian, WAY_GUARDIAN_SOUTH_TO_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAnemosInnerSanctum_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_DragonSkinChestPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_ElevatorPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_NoShadowScript, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_SwitchesPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 6, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 7, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_OrihalconChestPlacements[] = {
    {SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_NoShadowScript, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 5, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_GuardianPlacements[] = {
    {SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 1, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 2, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_MIMIC_STONE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 3, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_ARROW_PANEL, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_NoShadowScript, 4, 0, 0, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_NoShadowScript, 584, 0, 582, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_NoShadowScript, 584, 0, 630, FACING_WEST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_NoShadowScript, 680, 0, 582, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_COILED_DRAGON_STATUE, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_NoShadowScript, 680, 0, 630, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_DragonSkinChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN},
    {TRIGGER_STEP, TRIGGER_FLOOR_BETWEEN_SYMBOLS, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StepOffSymbol)},
    {TRIGGER_STEP, TRIGGER_VENUS_SYMBOL, FLAG_ON_SYMBOL, EVENT(AnemosInnerSanctum_LightSymbol)},
    {TRIGGER_STEP, TRIGGER_MERCURY_SYMBOL, FLAG_ON_SYMBOL, EVENT(AnemosInnerSanctum_LightSymbol)},
    {TRIGGER_STEP, TRIGGER_MARS_SYMBOL, FLAG_ON_SYMBOL, EVENT(AnemosInnerSanctum_LightSymbol)},
    {TRIGGER_STEP, TRIGGER_JUPITER_SYMBOL, FLAG_ON_SYMBOL, EVENT(AnemosInnerSanctum_LightSymbol)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), TRIGGER_TELEPORT_CIRCLE, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_TeleportOut)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_ElevatorTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_SWITCHES},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHWEST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_WEST},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ELEVATOR_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, TRIGGER_LIFT, IF_FLAG_SET(FLAG_LIFT_READY), EVENT(AnemosInnerSanctum_RideLiftUp)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH, FLAG_ROUND_SWITCH_PRESSED(0), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 1, FLAG_ROUND_SWITCH_PRESSED(1), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 2, FLAG_ROUND_SWITCH_PRESSED(2), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 3, FLAG_ROUND_SWITCH_PRESSED(3), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 4, FLAG_ROUND_SWITCH_PRESSED(4), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 5, FLAG_ROUND_SWITCH_PRESSED(5), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 6, FLAG_ROUND_SWITCH_PRESSED(6), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELEVATOR_FIRST_PILLAR, FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELEVATOR_FIRST_PILLAR, FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ELEVATOR_SECOND_PILLAR, FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ELEVATOR_SECOND_PILLAR, FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_SwitchesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_SOUTHEAST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_SOUTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ELEVATOR},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHWEST},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_SWITCHES_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_EndSand)},
    {TRIGGER_STEP, TRIGGER_SWITCH, FLAG_SWITCH_PRESSED, EVENT(AnemosInnerSanctum_PressSwitch)},
    {TRIGGER_STEP, TRIGGER_EAST_OF_SWITCH, IF_FLAG_SET(FLAG_SWITCH_PRESSED), EVENT(AnemosInnerSanctum_StepOffSwitch)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SWITCHES_BOULDER, CONDITION_ALWAYS, 0},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_FIRST_PILLAR, FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_FIRST_PILLAR, FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_SECOND_PILLAR, FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_SECOND_PILLAR, FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_PILLAR_A, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_PILLAR_A, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_PILLAR_B, FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_PILLAR_B, FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SWITCHES_THIRD_PILLAR, FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SWITCHES_THIRD_PILLAR, FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_OrihalconChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHWEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_ENTRANCE},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHEAST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_TO_ANEMOS_INNER_SANCTUM_GUARDIAN_NORTHEAST},
    {TRIGGER_STEP, TRIGGER_LIFT, IF_FLAG_SET(FLAG_LIFT_READY), EVENT(AnemosInnerSanctum_RideLiftUp)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH, FLAG_ROUND_SWITCH_PRESSED(0), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 1, FLAG_ROUND_SWITCH_PRESSED(1), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 2, FLAG_ROUND_SWITCH_PRESSED(2), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 3, FLAG_ROUND_SWITCH_PRESSED(3), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 4, FLAG_ROUND_SWITCH_PRESSED(4), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 5, FLAG_ROUND_SWITCH_PRESSED(5), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_ROUND_SWITCH + 6, FLAG_ROUND_SWITCH_PRESSED(6), EVENT(AnemosInnerSanctum_PressRoundSwitch)},
    {TRIGGER_STEP, TRIGGER_FIRST_CRACKED_FLOOR, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughFirstCracks)},
    {TRIGGER_STEP, TRIGGER_FIRST_CRACKED_FLOOR + 1, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughFirstCracks)},
    {TRIGGER_STEP, TRIGGER_SECOND_CRACKED_FLOOR, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughSecondCracks)},
    {TRIGGER_STEP, TRIGGER_SECOND_CRACKED_FLOOR + 1, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughSecondCracks)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_EndSand)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_GuardianTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_NORTHEAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_SOUTHWEST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_EAST},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, TRIGGER_MIMIC_STONE_START, FLAG_MIMIC_STONE_STARTED, EVENT(AnemosInnerSanctum_StartMimicStone)},
    {TRIGGER_STEP, TRIGGER_MIMIC_STONE_HOLE, FLAG_MIMIC_STONE_STARTED, EVENT(AnemosInnerSanctum_StartMimicStone)},
    {TRIGGER_STEP, TRIGGER_MIMIC_STONE_BUTTON, FLAG_MIMIC_STONE_STARTED, EVENT(AnemosInnerSanctum_StartMimicStone)},
    {TRIGGER_STEP, TRIGGER_BEFORE_CHARON_SLATE, FLAG_ANEMOS_CHARON_SLATE_SPOKE, EVENT(AnemosInnerSanctum_ApproachCharonSlate)},
    {TRIGGER_RAISED, TRIGGER_MIMIC_STONE_IN_HOLE, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_DropMimicStone)},
    {TRIGGER_RAISED, TRIGGER_MIMIC_STONE_ON_BUTTON, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_OpenMimicStoneDoor)},
    {TRIGGER_TABLE_END},
    // Unused: rows past the table's end.
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ELEVATOR_NORTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_EAST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_SWITCHES_NORTHEAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ANEMOS_INNER_SANCTUM_GUARDIAN_TO_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST_EAST},
    {TRIGGER_TABLE_END},
};

/// The north door's opening, as MAPGRID_ANIMATION_STEP rows: two steps of two
/// frames.
s16 gAnemosInnerSanctum_NorthDoorFrames[] = {
    52, 7, 1, 2, 2, 54, 7, 1,
    2, 2, -1,
};

/// A mimic stone room's door opening, the same.
s16 gAnemosInnerSanctum_MimicStoneDoorFrames[] = {
    18, 4, 1, 2, 2, 18, 7, 1,
    2, 2, -1,
};
// clang-format on

/// The three mimic stone rooms' doors, by their step trigger's number.
struct MapGridCellAnimation gAnemosInnerSanctum_MimicStoneDoors[] = {
    {gAnemosInnerSanctum_MimicStoneDoorFrames, 11, 4},
    {gAnemosInnerSanctum_MimicStoneDoorFrames, 26, 4},
    {gAnemosInnerSanctum_MimicStoneDoorFrames, 53, 4},
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

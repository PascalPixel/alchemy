/// @file
/// The Anemos Inner Sanctum, the rooms beneath Contigo reached by Teleport from
/// its entrance: blocks Moved onto switches, tiles that glow underfoot
/// (AnemosInnerSanctum_StepOnTile), pillars of light
/// (AnemosInnerSanctum_LightThePillar) and Sand, up to the guardian summoned at
/// the end (AnemosInnerSanctum_SummonTheGuardian).
#include "field/field_sequence.h"
#include "field/map_grid.h"
#include "graphics/output.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "types.h"
#include "system/task.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
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

/// An actor as the spiral mover keeps its spiral: its own place, the fall,
/// step, height, centre, radius, angle and spin in the actor's motion fields.
struct Mover {
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
  u8 riseEnabled;
  s16 angle;
  s16 spin;
};

struct AnemosInnerSanctumPicture {
  const void *data;
  u16 a;
  u16 b;
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

void AnemosInnerSanctum_TeleportIn(void);

s32 Actor_LiftTargetBlock();

void Actor_PlaceMovedBlock(void);

void AnemosInnerSanctum_StopPaletteCycle(void);

void Teleport_PlaySparks(void);

void Leader_CenterInCell(void);

struct CellTarget gCrackedTiles BSS_DATA;

void AnemosInnerSanctum_LightThePillar(void);
void AnemosInnerSanctum_StepOntoCenter(void);
void AnemosInnerSanctum_StartMovingBlock(void);
void AnemosInnerSanctum_FinishMovingBlock(void);
void AnemosInnerSanctum_StepOnTile(void);
void AnemosInnerSanctum_RaisePlatform(void);
void AnemosInnerSanctum_SinkIntoSand(void);
void Leader_SetLayerByte(void);
void AnemosInnerSanctum_PressFirstSwitch(void);
void AnemosInnerSanctum_PressSecondSwitch(void);
void AnemosInnerSanctum_FallThroughFirstCracks(void);
void AnemosInnerSanctum_FallThroughSecondCracks(void);

void AnemosInnerSanctum_StartFollower(void);
void AnemosInnerSanctum_RaiseDust(void);
void AnemosInnerSanctum_MarkEntrance(void);
void AnemosInnerSanctum_SummonTheGuardian(void);
void AnemosInnerSanctum_TeleportOut(void);

extern u32 gAnemosInnerSanctum_ShortScript[];
extern u32 gAnemosInnerSanctum_MediumScript[];
extern u32 gAnemosInnerSanctum_LongScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gAnemosInnerSanctum_StatueScript[];
extern s16 gAnemosInnerSanctum_OtherDoorFrames[];
extern s32 gAnemosInnerSanctum_OrbScript[];

/// Writes one background palette entry from its red, green and blue.
#define BG_PLTT_SET_RGB(index, r, g, b)                                        \
  do {                                                                         \
    int value = RGB(r, g, b);                                                  \
                                                                               \
    BG_PLTT[index] = value;                                                    \
  } while (0)

/// The actors of the inner rooms the set-up and the switches move, numbering
/// each room's placement table: stone pillars (SPRITE_STONE_PILLAR) Moved onto
/// the floor's marks, and the statues (sprite 432) that rise when the tiles or
/// the switches ask.
enum AnemosInnerSanctumElevatorActorId {
  ACTOR_ELEVATOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_ELEVATOR_STATUE,
  ACTOR_ELEVATOR_SECOND_PILLAR
};

enum AnemosInnerSanctumSwitchesActorId {
  ACTOR_SWITCHES_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_SWITCHES_BOULDER, ///< SPRITE_ROUND_BOULDER
  ACTOR_SWITCHES_SECOND_PILLAR,
  ACTOR_SWITCHES_PILLAR_A,
  ACTOR_SWITCHES_PILLAR_B,
  ACTOR_SWITCHES_THIRD_PILLAR,
  /// The first switch, sunk into the floor until it is pressed
  /// (AnemosInnerSanctum_PressFirstSwitch).
  ACTOR_SWITCHES_FIRST_SWITCH
};

enum AnemosInnerSanctumOrihalconChestActorId {
  ACTOR_ORIHALCON_CHEST_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_ORIHALCON_CHEST_FIRST_PILLAR,
  ACTOR_ORIHALCON_CHEST_SECOND_PILLAR
};

/// The guardian room's three mimic stones (sprite 431), one by each way in
/// from above: entered by entrance 1, 3 or 5, that one's stone follows the
/// leader's steps (AnemosInnerSanctum_StartFollower) until it drops into its
/// hole.
#define ACTOR_MIMIC_STONE_A ACTOR_FIRST_PLACED
#define ACTOR_MIMIC_STONE_B (ACTOR_FIRST_PLACED + 1)
#define ACTOR_MIMIC_STONE_C (ACTOR_FIRST_PLACED + 2)

/// The guardian room's ways in from above beside the first two mimic stones;
/// the third stone's is entrance 5.
enum { ENTRANCE_BY_MIMIC_STONE_A = 1, ENTRANCE_BY_MIMIC_STONE_B = 3 };

/// Set as each mimic stone drops into its hole
/// (AnemosInnerSanctum_MarkEntrance), so that the set-up leaves it there.
#define FLAG_MIMIC_STONE_A_DROPPED FLAG_AREA_TEMP_01
#define FLAG_MIMIC_STONE_B_DROPPED FLAG_AREA_TEMP_02
#define FLAG_MIMIC_STONE_C_DROPPED FLAG_AREA_TEMP_03

/// The step triggers of the Dragon Skin's room: its four element symbols, each
/// lit by the Djinn of its element the party has
/// (AnemosInnerSanctum_LightThePillar), and the centre between them.
enum AnemosInnerSanctumSymbolTrigger {
  TRIGGER_VENUS_SYMBOL = 50,
  TRIGGER_MERCURY_SYMBOL,
  TRIGGER_MARS_SYMBOL,
  TRIGGER_JUPITER_SYMBOL,
  TRIGGER_CENTER
};

/// The first of the seven glowing tiles' step triggers, each with its own
/// scene flag from FLAG_SCENE_TEMP_00 (AnemosInnerSanctum_StepOnTile).
enum { TRIGGER_FIRST_TILE = 50 };

/// The Djinn of each element the game places, before its two spares.
enum { DJINN_FOUND_PER_ELEMENT = 18 };

/// The sanctum's lasting flags: each pillar Moved onto its mark, each element
/// symbol lit, all four lit (the door opens), and the guardian's words heard.
#define FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED 0xa80
#define FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED 0xa81
#define FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED 0xa82
#define FLAG_ANEMOS_SWITCHES_SECOND_PILLAR_PLACED 0xa83
#define FLAG_ANEMOS_SWITCHES_THIRD_PILLAR_PLACED 0xa84
#define FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED 0xa85
#define FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED 0xa86
#define FLAG_ANEMOS_VENUS_SYMBOL_LIT 0xa87
#define FLAG_ANEMOS_MERCURY_SYMBOL_LIT 0xa88
#define FLAG_ANEMOS_MARS_SYMBOL_LIT 0xa89
#define FLAG_ANEMOS_JUPITER_SYMBOL_LIT 0xa8a
#define FLAG_ANEMOS_ALL_SYMBOLS_LIT 0xa8b
#define FLAG_ANEMOS_GUARDIAN_SPOKE 0xa8c

/// The guardian room's statue (sprite 432) that rises at its entrance 12
/// (AnemosInnerSanctum_RaiseSecondStatue).
#define ACTOR_GUARDIAN_ROOM_STATUE (ACTOR_FIRST_PLACED + 3)

/// Writes each listed background colour from its red, green and blue, keeps
/// the lists for the palette cycle, and saves the palettes as the fade's
/// source; arriving by Teleport, the party then teleports in.
void Palette_SetColors(u16 *slots, u16 *colours, u16 *cycleColors,
                       u16 *cycleSlots) {
  u8 *saved = Ram_HeapSlots->paletteFade;

  gPaletteSlots = slots;
  gPaletteColors = colours;
  gPaletteCycleColors = cycleColors;
  gPaletteCycleSlots = cycleSlots;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int index = *slots++;
      int red = *colours++;
      int green = *colours++;
      int blue = *colours++;

      BG_PLTT_SET_RGB(index, red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  if (gMainRoutineWork.teleportState == 2)
    AnemosInnerSanctum_TeleportIn();
}

/// Starts a palette cycle's task.
void PaletteCycle_Start(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

/// Stops the palette cycle, leaving the next transition a backdrop fade.
void AnemosInnerSanctum_StopPaletteCycle(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldTask_Remove((s32)PaletteCycle_Fade);
}

/// Starts the palette cycle at a step every two frames, as Teleport flashes.
void AnemosInnerSanctum_StartFastPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 2;
  gPaletteCycleTimer = 2;
  gPaletteCycleBright = 1;
  Task_AddFrame(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

#include "shared/list_count_entries.h"

#include "shared/palette_cycle_fade.h"

/// The party arrives by Teleport: the room fades in from grey while the
/// palettes cycle and the whirl plays.
void AnemosInnerSanctum_TeleportIn(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_OBJECTS);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  AnemosInnerSanctum_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  System_Wait(40);
  AnemosInnerSanctum_StopPaletteCycle();
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_Interpolate(16);
  System_Wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/actor_prepare.c"

#include "../field/field_effect_move.c"

#undef BG_PLTT_SET_RGB
extern s32 gAnemosInnerSanctum_DurationScripts[];

/// Spawns a field effect at x, y, z moving at the velocity given, its type,
/// scale, palette, priority, script, rotation, spin and update chosen by the
/// EFFECT_USE_ flags from the options.
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

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    // Read as its low byte: (u8)options->priority orders the loads apart.
    sprite->priority = *(u8 *)&options->priority;
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
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - FIXED_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
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
s32 AnemosInnerSanctum_CircleOrb(struct Actor *object) {
  FIXED radius;

  object->timer += object->userValue;
  SET_ACTOR_POSITION(object, object->bounce, object->gravity, toFIXED(152));
  FieldVector_AddPolarOffsetFX(object->riseBaseY, object->timer,
                               &object->x.fixed);
  radius = object->riseBaseY - toFIXED(1);
  object->riseBaseY = radius;
  if (radius)
    return 1;
  return 0;
}

extern u32 gAnemosInnerSanctum_MoteScript[];

/// Leaves a fading mote of the source's palette where the source is.
void AnemosInnerSanctum_SpawnMote(struct Actor *source) {
  struct Actor *object = (FieldActor_Create(SPRITE_PARTICLE, source->x.fixed,
                                            source->y.fixed, source->z.fixed));

  object->motionFlags = 0;
  FieldActor_SetSpriteColor(object, source->riseCounter);
  Scene_SetActorSpritePriority(object, 2);
  Actor_SelectAnimation(object, 1);
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gAnemosInnerSanctum_MoteScript);
  Actor_SetBlendMode(object, 1);
}

/// A spark's update: two steps a frame along its spiral, closing in and
/// falling, leaving a mote at each; 0 once it reaches the centre.
s32 AnemosInnerSanctum_SpiralMover(struct Mover *mover) {
  if (mover->radius < mover->step)
    return 0;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  FieldVector_AddPolarOffsetFX(mover->radius, mover->angle, &mover->x);
  AnemosInnerSanctum_SpawnMote(mover);
  mover->radius -= mover->step >> 1;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  FieldVector_AddPolarOffsetFX(mover->radius, mover->angle, &mover->x);
  AnemosInnerSanctum_SpawnMote(mover);
  mover->radius -= mover->step >> 1;
  mover->height -= mover->fall;
  return 1;
}

/// Puffs sparks of the object's palette where it is, and copies the cell
/// picture under its centre.
s32 AnemosInnerSanctum_SpawnOrbitSparks(struct Actor *object) {
  struct EffectOptions options;
  s32 x;
  s32 y;

  options.priority = 2;
  options.palette = object->riseCounter;
  FieldEffect_Spawn(object->x.fixed, object->y.fixed, object->z.fixed, 0,
                    toFIXED(1), 0, EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    &options);
  x = FIELD_CELL(object->bounce) + 64;
  y = FIELD_CELL(object->gravity);
  MapGrid_CopyCellsTo(x, y + 26, x, y, 1, 1);
  return 0;
}

#include "../field/actor_wait_until_idle.c"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gAnemosInnerSanctum_Placements[];
extern struct ActorPlacement gAnemosInnerSanctum_DragonSkinChestPlacements[];
extern struct ActorPlacement gAnemosInnerSanctum_ElevatorPlacements[];
extern struct ActorPlacement gAnemosInnerSanctum_SwitchesPlacements[];
extern s32 gAnemosInnerSanctum_OrihalconChestPlacements[];
extern struct ActorPlacement gAnemosInnerSanctum_GuardianPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
    return gAnemosInnerSanctum_DragonSkinChestPlacements;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
    return gAnemosInnerSanctum_ElevatorPlacements;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
    return gAnemosInnerSanctum_SwitchesPlacements;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
    return gAnemosInnerSanctum_OrihalconChestPlacements;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
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
  struct Mover *actor;
  s32 x, z, distance;

  actor = ((struct Mover *)FieldActor_Create(SPRITE_PARTICLE, from->x, from->y,
                                             from->z));
  x = (from->x - to->x) >> 16;
  z = (from->z - to->z) >> 16;
  actor->angle = Trig_Atan(from->z - to->z, from->x - to->x);
  actor->spin = (((Random16() & 2) - 1) * (Random16() & 15)) << 8;
  actor->palette = palette;
  actor->motionFlags = 0;
  actor->originX = to->x;
  actor->originZ = to->z;
  distance = INTtoFIXED(Iwram_Sqrt(x * x + z * z));
  actor->radius = distance;
  actor->step = distance / 16;
  actor->height = from->y;
  actor->fall = (from->y - to->y) / 16;
  Actor_SetBlendMode(actor, 1);
  FieldActor_SetSpriteColor(actor, (u16)palette);
  Scene_SetActorSpritePriority(actor, 2);
  Actor_SelectAnimation(actor, 7);
  Actor_SetSpriteFlags(actor, 0);
  FieldVector_AddPolarOffsetFX(actor->radius, actor->angle, &actor->x);
  Actor_StartScript(actor, gAnemosInnerSanctum_SparkScript);
  AnemosInnerSanctum_SpiralMover(actor);
}

/// Rings a point at x, y with eight orbs of the given palette that circle in
/// at the given speed.
void AnemosInnerSanctum_SparkRing(x, y, palette, speed) s32 x;
s32 y;
s16 palette;
s16 speed;
{
  s32 i;

  for (i = 0; i != 16;) {
    struct Actor *object =
        (FieldActor_Create(SPRITE_PARTICLE, x, y, toFIXED(152)));
    s32 angle = i << 12;

    FieldVector_AddPolarOffset(32, angle, &object->x.fixed);
    object->timer = angle;
    object->userValue = speed;
    object->riseCounter = i;
    object->riseEnabled = palette;
    object->bounce = x;
    object->riseBaseY = toFIXED(32);
    object->gravity = y;
    object->motionFlags = 0;
    Actor_SetBlendMode(object, 1);
    FieldActor_SetSpriteColor(object, (u16)palette);
    Scene_SetActorSpritePriority(object, 2);
    Actor_SelectAnimation(object, 7);
    Actor_SetSpriteFlags(object, 0);
    Actor_StartScript(object, gAnemosInnerSanctum_OrbScript);
    i += 2;
  }
}

/// Drops a spark of the given palette on the cell x, z.
void AnemosInnerSanctum_DropSpark(x, z, palette) s32 x;
s32 z;
s16 palette;
{
  struct Actor *object;

  object = FieldActor_Create(SPRITE_PARTICLE, INTtoFIXED(x), INTtoFIXED(z + 3),
                             toFIXED(152));
  object->motionFlags = ACTOR_FLOATS;
  FieldActor_SetSpriteColor(object, (u16)palette);
  Scene_SetActorSpritePriority(object, 2);
  Actor_SelectAnimation(object, 7);
  Actor_SetSpriteFlags(object, 0);
}

#include "text/msg_ids.h"

extern u32 gAnemosInnerSanctum_PillarRisesScript[];
extern s32 gAnemosInnerSanctum_PillarSparkScript[];
extern s32 gAnemosInnerSanctum_SparkOffsets[][2];

/// Stepping onto an element's symbol in the Dragon Skin's room: the leader
/// faces south and that element's Djinn the party has found fly out as
/// sparks; with all eighteen the symbol is lit and the sparks ring it, and
/// with all four lit the door opens. Already lit, or with none, nothing.
void AnemosInnerSanctum_LightThePillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  volatile u16 alpha = REG_BLDALPHA;
  s32 x, z;
  s32 targetX = 0;
  s32 targetZ = 0;
  s16 type = work->stepTrigger - TRIGGER_VENUS_SYMBOL + SPRITE_VENUS_DJINNI;
  s16 room = work->stepTrigger;
  s32 index = room - TRIGGER_VENUS_SYMBOL;
  s32 base;
  u8 count;
  s32 palette;
  s32 column, row, cellX, cellZ;
  s32 i;
  u8 last;
  struct Actor *actor;
  u8 found[20];
  u8 order[20];
  struct Vector3 from;
  struct Vector3 to;

  x = ((index & 1) << 8) + 408;
  base = 0;
  count = 0;
  palette = 0;
  z = (index & 2) * 3 * 32 + 168;
  switch (room) {
  case TRIGGER_VENUS_SYMBOL:
    base = FLAG_DJINNI(DJINNI_FLINT);
    targetX = 520;
    targetZ = 48;
    if (GameFlag_Get(FLAG_ANEMOS_VENUS_SYMBOL_LIT))
      goto seen;
    break;
  case TRIGGER_MERCURY_SYMBOL:
    palette = 3;
    base = FLAG_DJINNI(DJINNI_FIZZ);
    targetX = 552;
    targetZ = 48;
    if (GameFlag_Get(FLAG_ANEMOS_MERCURY_SYMBOL_LIT))
      goto seen;
    break;
  case TRIGGER_MARS_SYMBOL:
    palette = 10;
    base = FLAG_DJINNI(DJINNI_FORGE);
    targetX = 504;
    targetZ = 40;
    if (GameFlag_Get(FLAG_ANEMOS_MARS_SYMBOL_LIT))
      goto seen;
    break;
  case TRIGGER_JUPITER_SYMBOL:
    palette = 2;
    base = FLAG_DJINNI(DJINNI_GUST);
    targetX = 568;
    targetZ = 40;
    if (GameFlag_Get(FLAG_ANEMOS_JUPITER_SYMBOL_LIT)) {
    seen:
      GameFlag_Set(FLAG_SCENE_TEMP_00);
      return;
    }
    break;
  }

  i = (u16)base;
  if (i != (u16)base + DJINN_FOUND_PER_ELEMENT) {
  scan:
    if (GameFlag_Get(i)) {
      found[count] = i - base;
      count++;
    }
    i++;
    if (i != (u16)base + DJINN_FOUND_PER_ELEMENT)
      goto scan;
  }
  if (count == 0)
    return;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  FieldCamera_SetSpeed(0.8);
  Actor_WalkTo(gMainRoutineWork.leaderActorId, x, z);
  FieldCamera_MoveToFX(INTtoFIXED(x), -1, INTtoFIXED(z), 1);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_SOUTH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 28);
  actor = (FieldActor_Create((u16)type, leader->x.fixed,
                             leader->y.fixed + toFIXED(8), leader->z.fixed));
  actor->motionFlags = 0;
  actor->scaleY = FIXED_ONE / 10;
  actor->scaleX = FIXED_ONE / 10;
  Actor_SetSpriteFlags((s32)actor, 0);
  Actor_StartScript(actor, gAnemosInnerSanctum_PillarRisesScript);
  Actor_SetBlendMode(actor, 1);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(60);
  BlendScript_Suspend();
  alpha = alpha & 255;
  REG_BLDALPHA = BLEND_ALPHA(0, 16);
  cellX = x >> 4;
  cellZ = z >> 4;
  column = cellX;
  row = cellZ;
  MapGrid_CopyCellsTo(cellX + 63, cellZ + 23, cellX + 63, cellZ - 3, 3, 5);
  i = 0;
  if (alpha != 0) {
  fadeIn:
    REG_BLDALPHA = BLEND_ALPHA(i, BLEND_WEIGHT_MAX);
    System_Wait(8);
    i++;
    if (i != alpha)
      goto fadeIn;
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
    to.x =
        leader->x.fixed + INTtoFIXED(gAnemosInnerSanctum_SparkOffsets[slot][0]);
    to.y = leader->y.fixed;
    to.z =
        leader->z.fixed + INTtoFIXED(gAnemosInnerSanctum_SparkOffsets[slot][1]);
    AnemosInnerSanctum_SendSpark(&from, &to, palette);
    if (i & 1)
      Event_PlaySound(SE_SPARKS);
    i++;
    FieldEvent_Wait(8);
  }

  if (count != DJINN_FOUND_PER_ELEMENT) {
    FieldEvent_Wait(96);
    Actor_StartScript(actor, gAnemosInnerSanctum_PillarSparkScript);
    BlendScript_Suspend();
    alpha = REG_BLDALPHA & 255;
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
          64, 0,
          ((leader->x.fixed +
            INTtoFIXED(gAnemosInnerSanctum_SparkOffsets[slot][0])) >>
           20) +
              64,
          (leader->z.fixed +
           INTtoFIXED(gAnemosInnerSanctum_SparkOffsets[slot][1])) >>
              20,
          1, 1);
    }
    MapGrid_CopyCellsTo(64, 0, column + 63, row - 3, 3, 5);
    BlendScript_Resume();
    Event_WaitTransitionFrames();
  } else {
    Event_SetFlag((s16)work->stepTrigger - TRIGGER_VENUS_SYMBOL +
                  FLAG_ANEMOS_VENUS_SYMBOL_LIT);
    Actor_StartScript(actor, gAnemosInnerSanctum_PillarSparkScript);
    FieldEvent_Wait(60);
    FieldCamera_SetSpeed(4);
    FieldCamera_MoveTo(536, CAMERA_KEEP, 120, 1);
    FieldCamera_WaitForArrival();
    Event_PlaySound(SE_SHIMMER);
    {
      FIXED ringX = INTtoFIXED(targetX);
      FIXED ringY = INTtoFIXED(targetZ);

      AnemosInnerSanctum_SparkRing(ringX, ringY, palette, TRIG_TURN * 8 / 256);
      FieldEvent_Wait(10);
      AnemosInnerSanctum_SparkRing(ringX, ringY, palette, TRIG_TURN * 9 / 256);
      FieldEvent_Wait(10);
      AnemosInnerSanctum_SparkRing(ringX, ringY, palette, TRIG_TURN * 10 / 256);
    }
    FieldEvent_Wait(12);
    System_Wait(16);
    PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
    PaletteFade_Interpolate(1);
    System_Wait(1);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_Interpolate(32);
    Event_PlaySound(SE_FLASH);
    AnemosInnerSanctum_DropSpark(targetX, targetZ, palette);
    FieldEvent_Wait(80);
    if (GameFlag_Get(FLAG_ANEMOS_VENUS_SYMBOL_LIT) &&
        GameFlag_Get(FLAG_ANEMOS_MERCURY_SYMBOL_LIT) &&
        GameFlag_Get(FLAG_ANEMOS_MARS_SYMBOL_LIT) &&
        GameFlag_Get(FLAG_ANEMOS_JUPITER_SYMBOL_LIT)) {
      Event_SetFlag(FLAG_ANEMOS_ALL_SYMBOLS_LIT);
      Event_PlaySound(SE_THUMP);
      MapGrid_CopyCellsTo(52, 7, 33, 7, 1, 2);
      System_Wait(5);
      MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
      MapGrid_SetCellHeight(toFIXED(536), toFIXED(136), 0, 0);
      System_Wait(60);
    }
  }

  Actor_Release(actor);
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Event_End();
}

/// Stepping onto the centre between the symbols lets them light again.
void AnemosInnerSanctum_StepOntoCenter(void) {
  GameFlag_Clear(FLAG_SCENE_TEMP_00);
}

extern s16 gAnemosInnerSanctum_ElevatorBlocks[];
extern s16 gAnemosInnerSanctum_SwitchesBlocks[];
extern s16 gAnemosInnerSanctum_OrihalconChestBlocks[];

/// Move lifts a pillar: in the switches' and the Orihalcon chest's rooms the
/// cells around its marks are walled while it moves.
void AnemosInnerSanctum_StartMovingBlock(void) {
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
    Actor_LiftTargetBlock((s32)gAnemosInnerSanctum_ElevatorBlocks);
  } else {
    if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
      Actor_LiftTargetBlock((s32)gAnemosInnerSanctum_SwitchesBlocks);
      Field_SetCellBehaviorAt(2, toFIXED(72), toFIXED(440), CELL_BEHAVIOR_WALL);
    } else {
      if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
        Actor_LiftTargetBlock((s32)gAnemosInnerSanctum_OrihalconChestBlocks);
        Field_SetCellBehaviorAt(2, toFIXED(280), toFIXED(344),
                                CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(2, toFIXED(280), toFIXED(376),
                                CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(2, toFIXED(296), toFIXED(360),
                                CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(2, toFIXED(280), toFIXED(360), 232);
        Field_SetCellBehaviorAt(2, toFIXED(520), toFIXED(632),
                                CELL_BEHAVIOR_WALL);
        Field_SetCellBehaviorAt(2, toFIXED(520), toFIXED(616), 232);
      }
    }
  }
}

/// Move has set a pillar down: reads where it stands in 8-pixel cells, restores
/// the cells StartMovingBlock opened, and where a pillar has reached its mark
/// sets that pillar's flag. In the switches' room the second pillar on its mark
/// presses the first switch and opens the way; the pillar B set down short of
/// its mark trades places with the third pillar until that one is placed.
void AnemosInnerSanctum_FinishMovingBlock(void) {
  struct Actor *leader = gFieldWork->movedBlock;
  s32 x;
  s32 z;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  x = leader->x.fixed >> 19;
  z = leader->z.fixed >> 19;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
    Field_SetCellBehaviorAt(2, toFIXED(72), toFIXED(440), 232);
    if (x != 8)
      Field_SetCellBehaviorAt(2, toFIXED(72), toFIXED(424), 232);
  }
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
    Field_SetCellBehaviorAt(2, toFIXED(280), toFIXED(344), 0);
    Field_SetCellBehaviorAt(2, toFIXED(280), toFIXED(376), 232);
    Field_SetCellBehaviorAt(2, toFIXED(296), toFIXED(360), 0);
    Field_SetCellBehaviorAt(2, toFIXED(520), toFIXED(632), 232);
  }
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR) {
    if (x == 37 && z == 31) {
      GameFlag_Set(FLAG_ANEMOS_ELEVATOR_FIRST_PILLAR_PLACED);
      Actor_SetSpritePriority(ACTOR_ELEVATOR_FIRST_PILLAR, 2);
    }
    if (x == 55 && z == 95)
      GameFlag_Set(FLAG_ANEMOS_ELEVATOR_SECOND_PILLAR_PLACED);
  } else if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
    if (x == 7 && z == 53)
      GameFlag_Set(FLAG_ANEMOS_SWITCHES_FIRST_PILLAR_PLACED);
    if (x == 67 && z == 45) {
      MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
      Actor_SetPosition(ACTOR_SWITCHES_FIRST_SWITCH, 0, 0);
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

        Actor_SetPositionFX(
            ACTOR_SWITCHES_THIRD_PILLAR, fromX,
            FIXEDtoINT(Actor_Get(ACTOR_SWITCHES_PILLAR_B)->z.fixed) << 16);
        Actor_SetPosition(ACTOR_SWITCHES_PILLAR_B, 728, 472);
      }
      GameFlag_Set(FLAG_ANEMOS_SWITCHES_PILLAR_B_PLACED);
    }
  }
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST &&
      x == 33 && z == 45)
    GameFlag_Set(FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED);
  Event_End();
}

extern s32 gAnemosInnerSanctum_TileGlowScript[];

/// A glowing tile stepped on: lights the tile under the leader with a glow
/// and sets the tile's scene flag; once all seven are lit, the room's statue
/// floats up (FLAG_SCENE_TEMP_07), ready to raise the party.
void AnemosInnerSanctum_StepOnTile(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *object;
  s32 all;
  s32 i;
  s32 id;

  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
    MapGrid_CopyCellsTo(17, 71, FIELD_CELL(leader->x.fixed),
                        FIELD_CELL(leader->z.fixed) + 64, 1, 1);
  else
    MapGrid_CopyCellsTo(54, 87, FIELD_CELL(leader->x.fixed),
                        FIELD_CELL(leader->z.fixed) + 64, 1, 1);
  Event_PlaySound(SE_DRIP);
  object =
      FieldActor_Create(0x15d, (FIELD_CELL(leader->x.fixed) << 20) + toFIXED(8),
                        0, (FIELD_CELL(leader->z.fixed) << 20) + toFIXED(8));
  object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  object->motionFlags = 0;
  FieldActor_SetSpriteColor(object, 4);
  object->scaleY = FIXED_ONE * 11 / 8;
  object->scaleX = FIXED_ONE * 11 / 8;
  Scene_SetActorSpritePriority(object, 2);
  Actor_SelectAnimation(object, 3);
  Actor_SetSpriteFlags(object, 0);
  Actor_StartScript(object, gAnemosInnerSanctum_TileGlowScript);
  Event_SetFlag(work->stepTrigger - TRIGGER_FIRST_TILE + FLAG_SCENE_TEMP_00);
  all = Event_GetFlag(FLAG_SCENE_TEMP_00);
  for (i = 1; i != 7; i++)
    all &= Event_GetFlag(FLAG_SCENE_TEMP_00 + i);
  if (all) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldEvent_Wait(10);
    FieldCamera_SetSpeed(0.8);
    id = ACTOR_ELEVATOR_STATUE;
    if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
      id = ACTOR_ORIHALCON_CHEST_STATUE;
    FieldCamera_MoveToActorById(id, 1);
    FieldCamera_WaitForArrival();
    FieldEvent_Wait(10);
    Event_PlaySound(SE_582);
    Actor_SetAnimation(id, 2);
    Event_SetFlag(FLAG_SCENE_TEMP_07);
    Actor_Get(id)->motionFlags = ACTOR_FLOATS;
    FieldEvent_Wait(80);
    Event_End();
  }
}

/// Centres the leader on its cell and raises the room's statue with the party
/// on it, 4 pixels at once and then 128 more, as the room is left by its
/// upper exit.
void AnemosInnerSanctum_RaisePlatform(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *object;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST) {
    object = Actor_Get(ACTOR_ORIHALCON_CHEST_STATUE);
    FieldEvent_SetExitRequest(13);
  } else {
    object = Actor_Get(ACTOR_ELEVATOR_STATUE);
    FieldEvent_SetExitRequest(11);
  }
  {
    s32 x = (FIELD_CELL(leader->x.fixed) << 4) + 8;
    s32 z = (FIELD_CELL(leader->z.fixed) << 4) + 8;

    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, x, z);
  }
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 10);
  FieldEvent_Wait(30);
  object->motionFlags = 0;
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  object->speedLimit = toFIXED(2);
  object->acceleration = toFIXED(1) / 32;
  Event_PlaySound(SE_KOLIMA_BRIDGE_POST);
  FieldActor_SetMoveTargetFX(object, object->x.fixed,
                             object->y.fixed + toFIXED(4), object->z.fixed);
  FieldActor_WaitForMove(object);
  Scene_SetActorSpritePriority(leader, 1);
  Scene_SetActorSpritePriority(object, 1);
  FieldActor_SetMoveTargetFX(object, object->x.fixed,
                             object->y.fixed + toFIXED(128), object->z.fixed);
  FieldEvent_Wait(80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Sand on a cell of behavior 232: the leader sinks into the layer below.
void AnemosInnerSanctum_SinkIntoSand(void) {
  struct Actor *object;
  s32 behavior;

  object = Actor_Get(gMainRoutineWork.leaderActorId);
  behavior =
      FieldMapGrid_GetCellBehaviorAt(2, object->x.fixed, object->z.fixed);
  if (behavior == 232) {
    object->layer = 2;
  }
}

/// As Sand fades, the leader rises back to its own layer.
void Leader_SetLayerByte(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// Stepping onto the first switch: the leader walks onto it, it sinks with a
/// thump and the way opens.
void AnemosInnerSanctum_PressFirstSwitch(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 536, 360);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(50, 22, 33, 22, 1, 1);
  Actor_SetPosition(ACTOR_SWITCHES_FIRST_SWITCH, 0, 0);
  Event_PlaySound(SE_BUTTON_PRESS);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(51, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  Event_End();
}

/// Stepping onto the second switch once the first is pressed: the first
/// switch's actor sinks under it and the way opens.
void AnemosInnerSanctum_PressSecondSwitch(void) {
  struct Actor *object;

  object = Actor_Get(ACTOR_SWITCHES_FIRST_SWITCH);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->x.fixed = toFIXED(536);
  object->z.fixed = toFIXED(360);
  object->groundHeight = -toFIXED(14);
  object->y.fixed = -toFIXED(14);
  MapGrid_CopyCellsTo(51, 22, 33, 22, 1, 1);
  Event_PlaySound(SE_BUTTON_PRESS);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(50, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  MapGrid_CopyCellsTo(52, 20, 31, 20, 1, 2);
  FieldEvent_Wait(5);
  Event_ClearFlag(FLAG_SCENE_TEMP_00);
  Event_End();
}

s32 CrackedTiles_Step();

#define DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(name, exit)                        \
  void name(void) {                                                            \
    s32 fell;                                                                  \
                                                                               \
    fell = CrackedTiles_Step();                                                \
    if (fell != 0) {                                                           \
      FieldEvent_SetExitRequest(exit);                                         \
    }                                                                          \
  }

/// Stepping onto the first cracked floor: once it gives way the party falls
/// through to the room below, by exit 11.
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(AnemosInnerSanctum_FallThroughFirstCracks,
                                    11)

/// As AnemosInnerSanctum_FallThroughFirstCracks for the second, by exit 12.
DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF(AnemosInnerSanctum_FallThroughSecondCracks,
                                    12)

extern s32 gAnemosInnerSanctum_DustScript[];

/// The update of an object that moves with the leader: once the leader is past
/// z 248 it settles on its cell and clears FLAG_SCENE_TEMP_00; otherwise it
/// repeats the leader's step, raising dust on each step, and stops where the
/// cell kind under it is 52 or 51, raising trigger 201 or 200. riseBaseY and
/// acceleration hold the leader's last x and z.
void AnemosInnerSanctum_FollowLeader(struct Actor *self) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  FIXED next[2];
  FIXED ahead[3];
  struct Actor *dust;
  s32 kind;

  if (FIXEDtoINT(Actor_Get(gMainRoutineWork.leaderActorId)->z.fixed) > 248) {
    self->update = 0;
    Event_ClearFlag(FLAG_SCENE_TEMP_00);
    self->x.fixed = (self->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    self->z.fixed = (self->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    return;
  }
  ahead[0] = next[0] = self->x.fixed - (leader->x.fixed - self->riseBaseY);
  ahead[1] = leader->y.fixed;
  ahead[2] = next[1] = self->z.fixed + (leader->z.fixed - self->acceleration);
  if (self->riseBaseY != leader->x.fixed ||
      self->acceleration != leader->z.fixed) {
    dust = (FieldActor_Create(SPRITE_PARTICLE, self->x.fixed, self->y.fixed,
                              self->z.fixed));
    dust->x.fixed += INTtoFIXED((Random16() & 15) - 8);
    dust->y.fixed += INTtoFIXED((Random16() & 31) - 4);
    dust->motionFlags = 0;
    FieldActor_SetSpriteColor(dust, self->riseCounter);
    Scene_SetActorSpritePriority(dust, 2);
    Actor_SelectAnimation(dust, 1);
    Actor_SetSpriteFlags(dust, 0);
    Actor_StartScript(dust, gAnemosInnerSanctum_DustScript);
    self->riseBaseY = leader->x.fixed;
    self->acceleration = leader->z.fixed;
    if ((gFrameCount & 3) == 0)
      Event_PlaySound(SE_DRIP);
  }
  if (ahead[2] >> 16 > 248)
    return;
  FieldVector_AddPolarOffset(7, DEGtoANG(180) - leader->facing, ahead);
  if (FieldMapGrid_GetCellBehaviorAt(self->layer, ahead[0], ahead[2]) ==
      CELL_BEHAVIOR_WALL)
    return;
  {
    FIXED x = next[0];

    self->x.fixed = x;
    self->z.fixed = next[1];
    kind = FieldMapGrid_GetCellBehaviorAt(self->layer, x, next[1]);
  }
  if (kind == 52) {
    gFieldWork->raisedTrigger = 201;
    self->update = 0;
  } else if (kind == 51) {
    gFieldWork->raisedTrigger = 200;
    self->update = 0;
  }
}

#include "text/msg_ids.h"

void Actor_PlaceBlocks(void);

void AnemosInnerSanctum_StartFastPaletteCycle(void);

/// Stepping by a mimic stone in the guardian's room: the stone of the way
/// the party came in by starts to follow the leader's steps.
void AnemosInnerSanctum_StartFollower(void) {
  struct Actor *object;

  if (gMainRoutineWork.entrance == ENTRANCE_BY_MIMIC_STONE_A)
    object = Actor_Get(ACTOR_MIMIC_STONE_A);
  else if (gMainRoutineWork.entrance == ENTRANCE_BY_MIMIC_STONE_B)
    object = Actor_Get(ACTOR_MIMIC_STONE_B);
  else
    object = Actor_Get(ACTOR_MIMIC_STONE_C);
  if (Actor_Get(gMainRoutineWork.leaderActorId)->z.part[0].integer <= 247) {
    object->motionFlags = 0;
    object->update = AnemosInnerSanctum_FollowLeader;
    object->riseBaseY = Actor_Get(gMainRoutineWork.leaderActorId)->x.fixed;
    object->acceleration = Actor_Get(gMainRoutineWork.leaderActorId)->z.fixed;
    Event_SetFlag(FLAG_SCENE_TEMP_00);
  }
}

/// A mimic stone reaches its hole: it settles into the middle of its cell in
/// a cloud of dust and drops in.
void AnemosInnerSanctum_RaiseDust(void) {
  struct Actor *object;
  struct Actor *effect;
  struct FieldSprite *sprite;
  FIXED position[3];

  if (gMainRoutineWork.entrance == ENTRANCE_BY_MIMIC_STONE_A)
    object = Actor_Get(ACTOR_MIMIC_STONE_A);
  else if (gMainRoutineWork.entrance == ENTRANCE_BY_MIMIC_STONE_B)
    object = Actor_Get(ACTOR_MIMIC_STONE_B);
  else
    object = Actor_Get(ACTOR_MIMIC_STONE_C);
  position[0] = (object->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  position[2] = (object->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  object->acceleration = toFIXED(1);
  object->speedLimit = toFIXED(1);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTargetFX(object, position[0], 0, position[2]);
  effect = OverlayObject_PrepareObject(object->x.fixed, 0,
                                       position[2] + toFIXED(59), 286);
  effect->scaleX = FIXED_ONE * 3 / 2;
  effect->scaleY = FIXED_ONE * 3 / 2;
  sprite = effect->sprite;
  sprite->blendMode = OBJ_MODE_WINDOW;
  REG_WINOUT = WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
               WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_CLR;
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Event_PlaySound(SE_DUST_PUFF);
  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  OverlayObject_WaitUntilIdle(object);
  object->x.fixed = 0;
  object->y.fixed = 0;
  object->z.fixed = 0;
  Actor_SetPosition(ACTOR_MIMIC_STONE_A, 0, 0);
  Actor_Release(effect);
  Event_End();
}

/// By the entrance taken, sets that entrance's flag, moves its actor to the
/// middle of its cell and copies a cell picture column twice beside it.
void AnemosInnerSanctum_MarkEntrance(void) {
  struct Actor *object;
  s32 column;
  FIXED target[3];

  if (gMainRoutineWork.entrance == ENTRANCE_BY_MIMIC_STONE_A) {
    object = Actor_Get(ACTOR_MIMIC_STONE_A);
    Event_SetFlag(FLAG_MIMIC_STONE_A_DROPPED);
    column = 11;
  } else if (gMainRoutineWork.entrance == ENTRANCE_BY_MIMIC_STONE_B) {
    object = Actor_Get(ACTOR_MIMIC_STONE_B);
    Event_SetFlag(FLAG_MIMIC_STONE_B_DROPPED);
    column = 26;
  } else {
    object = Actor_Get(ACTOR_MIMIC_STONE_C);
    Event_SetFlag(FLAG_MIMIC_STONE_C_DROPPED);
    column = 53;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target[0] = (object->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  target[2] = (object->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  object->acceleration = toFIXED(1);
  object->speedLimit = toFIXED(1);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTargetFX(object, target[0], 0, target[2]);
  FieldActor_WaitForMove(object);
  Event_PlaySound(SE_THUMP);
  MapGrid_CopyCellsTo(18, 4, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_CopyCellsTo(18, 7, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_SetCellHeight(column << 20, toFIXED(88), 0, 0);
  Event_End();
}

/// At the end of the sanctum: the room darkens and the guardian's words are
/// shown, once.
void AnemosInnerSanctum_SummonTheGuardian(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
  PaletteFade_Interpolate(60);
  FieldEvent_Wait(60);
  FieldMessage_ShowCentered((s32)MsgSummonInheritedEarthDarkness, 0, 6);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  PaletteFade_Interpolate(60);
  FieldEvent_Wait(60);
  Event_SetFlag(FLAG_ANEMOS_GUARDIAN_SPOKE);
  Event_End();
}

extern struct Trigger gAnemosInnerSanctum_Triggers[];
extern struct Trigger gAnemosInnerSanctum_DragonSkinChestTriggers[];
extern struct Trigger gAnemosInnerSanctum_ElevatorTriggers[];
extern struct Trigger gAnemosInnerSanctum_SwitchesTriggers[];
extern struct Trigger gAnemosInnerSanctum_OrihalconChestTriggers[];
extern struct Trigger gAnemosInnerSanctum_GuardianTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST)
    return gAnemosInnerSanctum_DragonSkinChestTriggers;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ELEVATOR)
    return gAnemosInnerSanctum_ElevatorTriggers;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES)
    return gAnemosInnerSanctum_SwitchesTriggers;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_ORIHALCON_CHEST)
    return gAnemosInnerSanctum_OrihalconChestTriggers;
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN)
    return gAnemosInnerSanctum_GuardianTriggers;
  return gAnemosInnerSanctum_Triggers;
}

/// Teleport on the sanctum's pad: the party teleports back to Contigo.
void AnemosInnerSanctum_TeleportOut(void) {
  PaletteCycle_Start();
  Teleport_PlaySparks();
  gMainRoutineWork.teleportState = 2;
  Event_SetDestination(SCENE_CONTIGO, 99);
  FieldEvent_Wait(40);
  AnemosInnerSanctum_StopPaletteCycle();
}

extern s16 gAnemosInnerSanctum_DoorFrames[];
extern struct MapGridCellAnimation gAnemosInnerSanctum_Doors[];

/// Plays a door's map edit at x, y.
static __inline__ void AnemosInnerSanctum_PlayMapEdit(const void *steps, s32 x,
                                                      s32 y) {
  MapGrid_PlayMapEdit(steps, x, y);
}

/// The scene opening: clears the actors' motion flags, plays the entrance's
/// map edit and steps the leader in.
void AnemosInnerSanctum_EnterTheRoom(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  Event_PlaySound(SE_DOOR_OPEN);
  if (gMainRoutineWork.next == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
    MapGrid_AnimateCells(gAnemosInnerSanctum_DoorFrames, 33, 7);
  } else {
    s32 index = work->stepTrigger / 2 - 1;

    AnemosInnerSanctum_PlayMapEdit(gAnemosInnerSanctum_Doors[index].steps,
                                   gAnemosInnerSanctum_Doors[index].x,
                                   gAnemosInnerSanctum_Doors[index].y);
  }
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Entering the elevator room on its statue: the statue rises 48 pixels with
/// the leader on it, up to the floor.
void AnemosInnerSanctum_RaiseFirstStatue(void) {
  struct Actor *object = Actor_Get(ACTOR_ELEVATOR_STATUE);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(ACTOR_ELEVATOR_STATUE, 2);
  object->y.fixed = -toFIXED(64);
  Actor_SetPosition(ACTOR_ELEVATOR_STATUE, 728, 120);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 728, 120);
  Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed = -toFIXED(48);
  Event_StartScreenTransition();
  object->acceleration = toFIXED(1);
  object->speedLimit = toFIXED(1);
  FieldActor_SetMoveTargetFX(object, object->x.fixed,
                             object->y.fixed + toFIXED(48), object->z.fixed);
  FieldActor_WaitForMove(object);
  Event_SetFlag(FLAG_AREA_TEMP_00);
  Event_End();
}

/// As AnemosInnerSanctum_RaiseFirstStatue, on the guardian room's statue.
void AnemosInnerSanctum_RaiseSecondStatue(void) {
  struct Actor *object = Actor_Get(ACTOR_GUARDIAN_ROOM_STATUE);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  object->motionFlags = 0;
  object->y.fixed = -toFIXED(64);
  Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed = -toFIXED(48);
  Event_StartScreenTransition();
  object->acceleration = toFIXED(1);
  object->speedLimit = toFIXED(1);
  FieldActor_SetMoveTargetFX(object, object->x.fixed,
                             object->y.fixed + toFIXED(48), object->z.fixed);
  FieldActor_WaitForMove(object);
  Event_SetFlag(FLAG_AREA_TEMP_04);
  Event_End();
}

s32 CrackedTiles_Find();
extern s16 gAnemosInnerSanctum_PaletteSlots[];
extern s16 gAnemosInnerSanctum_PaletteColors[];
extern s16 gAnemosInnerSanctum_DimColors[];
extern u16 gAnemosInnerSanctum_CycleSlots[];

s32 Scene_Initialize(void) {
  s32 scene;
  s32 nearStairs;
  s32 entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_ANEMOS_INNER_SANCTUM_DRAGON_SKIN_CHEST) {
    Palette_SetColors((s32)gAnemosInnerSanctum_PaletteSlots,
                      (s32)gAnemosInnerSanctum_PaletteColors,
                      (s32)gAnemosInnerSanctum_DimColors,
                      (s32)gAnemosInnerSanctum_CycleSlots);
    if (Event_GetFlag(FLAG_ANEMOS_VENUS_SYMBOL_LIT)) {
      AnemosInnerSanctum_DropSpark(0x208, 48, 0);
      MapGrid_CopyCellsTo(85, 34, 85, 8, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_MERCURY_SYMBOL_LIT)) {
      AnemosInnerSanctum_DropSpark(0x228, 48, 3);
      MapGrid_CopyCellsTo(101, 34, 101, 8, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_MARS_SYMBOL_LIT)) {
      AnemosInnerSanctum_DropSpark(0x1f8, 40, 10);
      MapGrid_CopyCellsTo(85, 46, 85, 20, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_JUPITER_SYMBOL_LIT)) {
      AnemosInnerSanctum_DropSpark(0x238, 40, 2);
      MapGrid_CopyCellsTo(101, 46, 101, 20, 9, 8);
    }
    if (Event_GetFlag(FLAG_ANEMOS_ALL_SYMBOLS_LIT)) {
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
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    Actor_Get(ACTOR_ELEVATOR_STATUE)->motionFlags = 0;
    Actor_Get(ACTOR_ELEVATOR_STATUE)->priorityFlags |= 128;
    if (gMainRoutineWork.entrance == 17 && Event_GetFlag(FLAG_AREA_TEMP_00)) {
      Actor_SetAnimation(ACTOR_ELEVATOR_STATUE, 2);
      Actor_SetPosition(ACTOR_ELEVATOR_STATUE, 728, 120);
    }
    if (gMainRoutineWork.entrance == 18 && GameFlag_Get(FLAG_AREA_TEMP_00) == 0)
      AnemosInnerSanctum_RaiseFirstStatue();
    if (gMainRoutineWork.entrance == 10) {
      if (Event_GetFlag(FLAG_AREA_TEMP_00))
        Actor_SetPosition(ACTOR_ELEVATOR_STATUE, 0, 0);
      if (Event_GetFlag(FLAG_SCENE_TEMP_07))
        Actor_Get(ACTOR_ELEVATOR_STATUE)->motionFlags = ACTOR_FLOATS;
      if (Event_GetFlag(FLAG_SCENE_TEMP_00))
        MapGrid_CopyCellsTo(54, 87, 44, 86, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_01))
        MapGrid_CopyCellsTo(54, 87, 46, 86, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_02))
        MapGrid_CopyCellsTo(54, 87, 43, 87, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_03))
        MapGrid_CopyCellsTo(54, 87, 45, 87, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_04))
        MapGrid_CopyCellsTo(54, 87, 47, 87, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_05))
        MapGrid_CopyCellsTo(54, 87, 44, 88, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_06))
        MapGrid_CopyCellsTo(54, 87, 46, 88, 1, 1);
      Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
    }
  } else if (scene == SCENE_ANEMOS_INNER_SANCTUM_SWITCHES) {
    struct Actor *lift = Actor_Get(ACTOR_SWITCHES_FIRST_SWITCH);

    lift->motionFlags = ACTOR_FALLS;
    lift->groundHeight = -toFIXED(14);
    lift->y.fixed = -toFIXED(14);
    lift->priorityFlags |= 128;
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    Actor_Get(ACTOR_SWITCHES_BOULDER)->riseCounter = 1;
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
    nearStairs = *(u16 *)&gMainRoutineWork.entrance;
    entrance = gMainRoutineWork.entrance;
    if (entrance == 1) {
      CrackedTiles_Find(33, 0x280, 0x380, 18, 68);
      if (Event_GetFlag(FLAG_AREA_TEMP_04) == 0) {
        Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
        Actor_Get(ACTOR_ORIHALCON_CHEST_STATUE)->motionFlags = 0;
        Actor_Get(ACTOR_ORIHALCON_CHEST_STATUE)->priorityFlags |= 128;
      } else {
        Actor_SetPosition(ACTOR_ORIHALCON_CHEST_STATUE, 0, 0);
        Event_SetFlag(FLAG_SCENE_TEMP_00);
        Event_SetFlag(FLAG_SCENE_TEMP_01);
        Event_SetFlag(FLAG_SCENE_TEMP_02);
        Event_SetFlag(FLAG_SCENE_TEMP_03);
        Event_SetFlag(FLAG_SCENE_TEMP_04);
        Event_SetFlag(FLAG_SCENE_TEMP_05);
        Event_SetFlag(FLAG_SCENE_TEMP_06);
      }
      if (Event_GetFlag(FLAG_SCENE_TEMP_07))
        Actor_Get(ACTOR_ORIHALCON_CHEST_STATUE)->motionFlags = ACTOR_FLOATS;
      if (Event_GetFlag(FLAG_SCENE_TEMP_00))
        MapGrid_CopyCellsTo(17, 71, 8, 69, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_05))
        MapGrid_CopyCellsTo(17, 71, 6, 71, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_01))
        MapGrid_CopyCellsTo(17, 71, 5, 75, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_02))
        MapGrid_CopyCellsTo(17, 71, 8, 75, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_03))
        MapGrid_CopyCellsTo(17, 71, 11, 75, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_04))
        MapGrid_CopyCellsTo(17, 71, 8, 73, 1, 1);
      if (Event_GetFlag(FLAG_SCENE_TEMP_06))
        MapGrid_CopyCellsTo(17, 71, 10, 71, 1, 1);
    } else if (entrance == 11) {
      CrackedTiles_Find(33, 0x280, 0x380, 18, 68);
      CrackedTile_PlaceLanding(22, 0, FLAG_SCENE_TEMP_04);
    } else if (entrance == 12) {
      CrackedTiles_Find(35, 0x280, 0x380, 18, 68);
      if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
        CrackedTile_PlaceLanding(36, -36, FLAG_SCENE_TEMP_04);
    } else if ((u32)((nearStairs - 4) << 16) <= 0x10000) {
      CrackedTiles_Find(35, 0x280, 0x380, 18, 68);
    }
  } else if (scene == SCENE_ANEMOS_INNER_SANCTUM_GUARDIAN) {
    struct MapWork *map = Ram_HeapSlots->mapGrid;

    switch (gMainRoutineWork.entrance) {
    case 7:
      if (Event_GetFlag(FLAG_AREA_TEMP_04) == 0)
        Actor_SetPosition(ACTOR_GUARDIAN_ROOM_STATUE, 0, 0);
    case 12:
      Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
      Actor_SetAnimation(ACTOR_GUARDIAN_ROOM_STATUE, 2);
      Actor_Get(ACTOR_GUARDIAN_ROOM_STATUE)->motionFlags = 0;
      Actor_Get(ACTOR_GUARDIAN_ROOM_STATUE)->priorityFlags |= 128;
      Actor_SetAnimation(ACTOR_GUARDIAN_ROOM_STATUE, 2);
      if (Event_GetFlag(FLAG_AREA_TEMP_04) == 0 &&
          gMainRoutineWork.entrance == 12)
        AnemosInnerSanctum_RaiseSecondStatue();
      else
        Actor_Get(ACTOR_GUARDIAN_ROOM_STATUE)->y.fixed = -toFIXED(16);
      break;
    case 1:
    case 2:
      if (Event_GetFlag(FLAG_MIMIC_STONE_A_DROPPED)) {
        Actor_SetPosition(ACTOR_MIMIC_STONE_A, 120, 120);
        Event_SetFlag(FLAG_SCENE_TEMP_00);
      } else {
        MapGrid_CopyCellsTo(18, 1, 11, 4, 1, 2);
        MapGrid_SetCellHeight(toFIXED(184), toFIXED(88), 0, 2);
      }
      break;
    case 3:
    case 4:
      if (Event_GetFlag(FLAG_MIMIC_STONE_B_DROPPED)) {
        Actor_SetPosition(ACTOR_MIMIC_STONE_B, 552, 120);
        Event_SetFlag(FLAG_SCENE_TEMP_00);
      } else {
        MapGrid_CopyCellsTo(18, 1, 26, 4, 1, 2);
        MapGrid_SetCellHeight(toFIXED(424), toFIXED(88), 0, 2);
      }
      break;
    case 5:
    case 6:
      if (Event_GetFlag(FLAG_MIMIC_STONE_C_DROPPED)) {
        Actor_SetPosition(ACTOR_MIMIC_STONE_C, 792, 120);
        Event_SetFlag(FLAG_SCENE_TEMP_00);
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
void Actor_SetSpritePriorityB(struct Actor *object, s32 priority);
/// As FieldEffect_Spawn, the effect taking the leader's sprite priority and
/// moved by Effect_MoveLocal.
void FieldEffect_SpawnLocal(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                            s32 velocityZ, u32 flags,
                            const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.leaderActorId));
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

  if ((flags & 0xffff0000) == 0 || options == 0)
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
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - FIXED_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
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

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_ANEMOS_INNER_SANCTUM_EXIT_IF
#undef CELL_PUSH_SLOT_ENTRY
void CrackedTile_DropIn(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
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
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 16 / 25;
  options.startScaleY = FIXED_ONE * 16 / 25;
  for (i = 0; i <= 16; i++) {
    s32 angle;
    s32 velocity[3];
    FIXED cosine;

    angle = i << 12;
    cosine = Trig_Cos(angle);
    velocity[0] = cosine;
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16() * (toFIXED(3) / 4) >> 16) - (toFIXED(3) / 8);
    velocity[2] += (Random16() * (toFIXED(3) / 8) >> 16) - (toFIXED(3) / 16);
    FieldEffect_SpawnLocal(actor->x.fixed, actor->y.fixed, actor->z.fixed,
                           velocity[0], velocity[1], velocity[2], 0x1090001,
                           &options);
  }
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  actor->gravity = toFIXED(1);
  actor->bounce = FIXED_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Event_End();
}

void GameFlag_SetByte(s32 flag, s32 value);
#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

// clang-format off
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

s32 gAnemosInnerSanctum_TileGlowScript[] = {
    SC_WAIT(20),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctum_OrbScript[] = {                                                               
      46,                                                                      
      (s32)AnemosInnerSanctum_CircleOrb,                                                              
      38,                                                                      
      17,                                                                      
  };

s32 gAnemosInnerSanctum_DustScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -6553),
    SC_ADD(SW_SCALE_Y, -6553),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(1)),
    SC_RELEASE,
    SC_END,
};

u32 gAnemosInnerSanctum_PillarRisesScript[] = {
    SC_WAIT(50),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 6553),
    SC_ADD(SW_SCALE_Y, 6553),
    SC_ADD(SW_Y, toFIXED(3)),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_SET(SW_MOTION, 4),
    SC_END,
};

s32 gAnemosInnerSanctum_PillarSparkScript[] = {
    SC_WAIT(30),
    SC_SET(SW_MOTION, 0),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -6553),
    SC_ADD(SW_SCALE_Y, -6553),
    SC_ADD(SW_Y, -toFIXED(3)),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_END,
};

u32 gAnemosInnerSanctum_MoteScript[] = {
    SC_WAIT(3),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctum_SparkScript[] = {
    SC_SET(SW_SCALE_X, 0x20000),
    SC_SET(SW_SCALE_Y, 0x20000),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, -6553),
    SC_ADD(SW_SCALE_Y, -6553),
    SC_WAIT(2),
    SC_LOOP(10, SC_TO(1)),
    SC_CALL((s32)AnemosInnerSanctum_SpiralMover),
    SC_CALL((s32)AnemosInnerSanctum_SpawnOrbitSparks),
    SC_RELEASE,
    SC_END,
};

s16 gAnemosInnerSanctum_ElevatorBlocks[] = {
    8, 10, -1,
};

s16 gAnemosInnerSanctum_SwitchesBlocks[] = {
    8, 10, 11, 12, 13, -1,
};

s16 gAnemosInnerSanctum_OrihalconChestBlocks[] = {
    9, 10, -1,
};

s32 gAnemosInnerSanctum_SparkOffsets[][2] = {
    {-48, -32}, {48, -32}, {-64, -16}, {64, -16}, {-32, -16}, {32, -16},
    {-48, 0}, {48, 0}, {-32, 0}, {32, 0}, {-64, 16}, {64, 16}, {-32, 16},
    {32, 16}, {-48, 32}, {48, 32}, {-16, 32}, {16, 32},
};

u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s32 gAnemosInnerSanctum_DurationScripts[] = {
    (s32)gAnemosInnerSanctum_ShortScript,
    (s32)gAnemosInnerSanctum_MediumScript,
    (s32)gAnemosInnerSanctum_LongScript,
};

s16 gAnemosInnerSanctum_PaletteSlots[] = {
    89, 90, 91, 92, 93, 94, 95, -1,
};

s16 gAnemosInnerSanctum_PaletteColors[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gAnemosInnerSanctum_DimColors[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u16 gAnemosInnerSanctum_CycleSlots[] = {                                                               
      94, 89, 90, 91, 92, 93, 93, 94, 89, 90, 91, 92, 92, 93, 94, 89, 90, 91,  
      91, 92, 93, 94, 89, 90, 90, 91, 92, 93, 94, 89, 89, 90, 91, 92, 93, 94,  
  };

s32 gAnemosInnerSanctum_StatueScript[] = {
    SC_CALL((s32)AnemosInnerSanctum_ClearScriptedActorSpriteFlags),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AnemosInnerSanctumDragonSkinChest),
    SCENE_EXIT_UNLESS(1, AnemosInnerSanctumEntrance, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctumGuardian, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(99, Contigo, 99, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumElevator),
    SCENE_EXIT_UNLESS(1, AnemosInnerSanctumElevator, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctumElevator, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AnemosInnerSanctumElevator, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AnemosInnerSanctumSwitches, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AnemosInnerSanctumElevator, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AnemosInnerSanctumGuardian, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AnemosInnerSanctumElevator, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AnemosInnerSanctumGuardian, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AnemosInnerSanctumElevator, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AnemosInnerSanctumElevator, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AnemosInnerSanctumElevator, 18, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AnemosInnerSanctumElevator, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AnemosInnerSanctumElevator, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AnemosInnerSanctumElevator, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, AnemosInnerSanctumElevator, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, AnemosInnerSanctumGuardian, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, AnemosInnerSanctumElevator, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumSwitches),
    SCENE_EXIT_UNLESS(1, AnemosInnerSanctumSwitches, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctumOrihalconChest, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AnemosInnerSanctumSwitches, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AnemosInnerSanctumSwitches, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AnemosInnerSanctumSwitches, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AnemosInnerSanctumGuardian, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AnemosInnerSanctumOrihalconChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AnemosInnerSanctumElevator, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AnemosInnerSanctumOrihalconChest, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AnemosInnerSanctumSwitches, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AnemosInnerSanctumSwitches, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AnemosInnerSanctumGuardian, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AnemosInnerSanctumSwitches, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AnemosInnerSanctumSwitches, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, AnemosInnerSanctumSwitches, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, AnemosInnerSanctumGuardian, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, AnemosInnerSanctumSwitches, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumOrihalconChest),
    SCENE_EXIT_UNLESS(1, AnemosInnerSanctumSwitches, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctumSwitches, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AnemosInnerSanctumOrihalconChest, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AnemosInnerSanctumOrihalconChest, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AnemosInnerSanctumEntrance, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AnemosInnerSanctumGuardian, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AnemosInnerSanctumSwitches, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AnemosInnerSanctumOrihalconChest, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AnemosInnerSanctumOrihalconChest, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AnemosInnerSanctumGuardian, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AnemosInnerSanctumOrihalconChest, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AnemosInnerSanctumOrihalconChest, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AnemosInnerSanctumGuardian, 12, EXIT_FLAG_NONE),
    SCENE_EXITS(AnemosInnerSanctumGuardian),
    SCENE_EXIT_UNLESS(1, AnemosInnerSanctumElevator, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AnemosInnerSanctumElevator, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AnemosInnerSanctumSwitches, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AnemosInnerSanctumSwitches, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AnemosInnerSanctumOrihalconChest, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AnemosInnerSanctumOrihalconChest, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AnemosInnerSanctumSwitches, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AnemosInnerSanctumGuardian, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AnemosInnerSanctumElevator, 16, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AnemosInnerSanctumDragonSkinChest, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AnemosInnerSanctumGuardian, 8, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gAnemosInnerSanctum_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_DragonSkinChestPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_ElevatorPlacements[] = {
    {272, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_NORTH, 2, 2},
    {432, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_StatueScript, 0x2, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnemosInnerSanctum_SwitchesPlacements[] = {
    {272, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_NORTH, 2, 2},
    {329, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x4, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x5, 0, 0, FACING_NORTH, 2, 2},
    {272, CONDITION_ALWAYS, 7, 0x6, 0, 0, FACING_NORTH, 2, 2},
    {489, CONDITION_ALWAYS, 7, 0x7, 0, 0, FACING_NORTH, 2, 2},
    {SCENE_TABLE_END},
};

s32 gAnemosInnerSanctum_OrihalconChestPlacements[] = {                                                               
      (s32)0xffff01b0, (s32)gAnemosInnerSanctum_StatueScript, 3, 0, 0, 0x202c000, (s32)0xffff0110, 7, 4, 0, 0, 0x202c000,    
      (s32)0xffff0110, 7,         5, 0, 0, 0x202c000, 0xffff,          0,  0,  0, 0, 0,     
  };

struct ActorPlacement gAnemosInnerSanctum_GuardianPlacements[] = {
    {431, CONDITION_ALWAYS, 7, 0x1, 0, 0, FACING_NORTH, 2, 2},
    {431, CONDITION_ALWAYS, 7, 0x2, 0, 0, FACING_NORTH, 2, 2},
    {431, CONDITION_ALWAYS, 7, 0x3, 0, 0, FACING_NORTH, 2, 2},
    {432, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_StatueScript, 0x4, 0, 0, FACING_NORTH, 2, 2},
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_StatueScript, 584, 0, 582, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_StatueScript, 584, 0, 630, FACING_WEST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_StatueScript, 680, 0, 582, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(429, CONDITION_ALWAYS, (s32)gAnemosInnerSanctum_StatueScript, 680, 0, 630, FACING_EAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_DragonSkinChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, TRIGGER_CENTER, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StepOntoCenter)},
    {TRIGGER_STEP, TRIGGER_VENUS_SYMBOL, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_LightThePillar)},
    {TRIGGER_STEP, TRIGGER_MERCURY_SYMBOL, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_LightThePillar)},
    {TRIGGER_STEP, TRIGGER_MARS_SYMBOL, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_LightThePillar)},
    {TRIGGER_STEP, TRIGGER_JUPITER_SYMBOL, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_LightThePillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 30, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_TeleportOut)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_ElevatorTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, 11, IF_FLAG_SET(FLAG_SCENE_TEMP_07), EVENT(AnemosInnerSanctum_RaisePlatform)},
    {TRIGGER_STEP, 50, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 51, FLAG_SCENE_TEMP_01, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 52, FLAG_SCENE_TEMP_02, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 53, FLAG_SCENE_TEMP_03, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 54, FLAG_SCENE_TEMP_04, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 55, FLAG_SCENE_TEMP_05, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 56, FLAG_SCENE_TEMP_06, EVENT(AnemosInnerSanctum_StepOnTile)},
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
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {TRIGGER_STEP, 50, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_PressFirstSwitch)},
    {TRIGGER_STEP, 51, IF_FLAG_SET(FLAG_SCENE_TEMP_00), EVENT(AnemosInnerSanctum_PressSecondSwitch)},
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
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 11, IF_FLAG_SET(FLAG_SCENE_TEMP_07), EVENT(AnemosInnerSanctum_RaisePlatform)},
    {TRIGGER_STEP, 50, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 51, FLAG_SCENE_TEMP_01, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 52, FLAG_SCENE_TEMP_02, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 53, FLAG_SCENE_TEMP_03, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 54, FLAG_SCENE_TEMP_04, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 55, FLAG_SCENE_TEMP_05, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 56, FLAG_SCENE_TEMP_06, EVENT(AnemosInnerSanctum_StepOnTile)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughFirstCracks)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughFirstCracks)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughSecondCracks)},
    {TRIGGER_STEP, 36, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FallThroughSecondCracks)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Leader_SetLayerByte)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_FIRST_PILLAR, FLAG_ANEMOS_ORIHALCON_CHEST_FIRST_PILLAR_PLACED, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ORIHALCON_CHEST_SECOND_PILLAR, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_FinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnemosInnerSanctum_GuardianTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_STEP, 50, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_StartFollower)},
    {TRIGGER_STEP, 51, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_StartFollower)},
    {TRIGGER_STEP, 52, FLAG_SCENE_TEMP_00, EVENT(AnemosInnerSanctum_StartFollower)},
    {TRIGGER_STEP, 30, FLAG_ANEMOS_GUARDIAN_SPOKE, EVENT(AnemosInnerSanctum_SummonTheGuardian)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_RaiseDust)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(AnemosInnerSanctum_MarkEntrance)},
    {0xffffffff, 0, 0x0, 0},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TABLE_END},
};

s16 gAnemosInnerSanctum_DoorFrames[] = {
    52, 7, 1, 2, 2, 54, 7, 1,
    2, 2, -1,
};

s16 gAnemosInnerSanctum_OtherDoorFrames[] = {
    18, 4, 1, 2, 2, 18, 7, 1,
    2, 2, -1,
};
// clang-format on

struct MapGridCellAnimation gAnemosInnerSanctum_Doors[] = {
    {gAnemosInnerSanctum_OtherDoorFrames, 11, 4},
    {gAnemosInnerSanctum_OtherDoorFrames, 26, 4},
    {gAnemosInnerSanctum_OtherDoorFrames, 53, 4},
};

s32 gFieldEffect_DurationScripts[] = {
    (s32)gFieldEffect_ShortScript,
    (s32)gFieldEffect_MediumScript,
    (s32)gFieldEffect_LongScript,
};

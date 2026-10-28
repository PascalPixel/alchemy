/// @file
/// Altmiller Cave, where the party finds Babi while Tolbi searches for him
/// during Colosso (babi_palace.c). Its six scenes share this overlay and the
/// area name Altmiller Cave:
///
/// - AltmillerCaveEntrance (SceneId_AltmillerCaveEntrance): the dark passage in
///   from the World Map and the chamber beyond it.
/// - AltmillerCaveBabi (SceneId_AltmillerCaveBabi): the invisible Babi, who
/// asks
///   for his draught (AltmillerCave_MeetInvisibleBabi) and drinks it
///   (AltmillerCave_RestoreBabiAndInviteParty), and a chest that is a Mimic.
/// - AltmillerCaveVialChest (SceneId_AltmillerCaveVialChest) and
///   AltmillerCaveCookieChest (SceneId_AltmillerCaveCookieChest): a chest each.
/// - AltmillerCaveSquall (SceneId_AltmillerCaveSquall): Squall, a Jupiter
/// Djinni,
///   three fallen columns to push, a stone pillar for Move and a puddle for
///   Frost.
/// - AltmillerCaveDraught (SceneId_AltmillerCaveDraught): three more fallen
///   columns, the rock puzzle (AltmillerCave_TurnRock) and the chest that holds
///   the Mystic Draught behind the wall the puzzle opens.
///
/// The rooms share one entrance table and one exit table, and the other
/// getters choose by gMainRoutineWork.sceneId. AltmillerCave_Initialize
/// sends an entry from outside the cave (entrance 0) to the room's first
/// entrance, darkens the dark rooms and puts back what the party has done.
/// Saving Babi (FLAG_BABI_SAVED) is the story flag the rest of Tolbi waits on;
/// the overlay's own state is in scene temp flags and gSceneLocalState, so it
/// starts over on every ordinary arrival.
///
/// Every room but the two lit ones is dark (FLAG_ALTMILLER_CAVE_DARK): BG1 is
/// blended over the rest (ALTMILLER_CAVE_DARKNESS_BLEND), and the light around
/// the party is a display window, an ellipse around the leader inside which the
/// layers are drawn differently (AltmillerCave_InitializeDarkness). In an
/// evenly dark room BG1 covers everything outside it and is only blended inside
/// it. AltmillerCaveEntrance darkens by steps instead
/// (FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS): step triggers 60 to 65 each set a
/// level, kept in gSceneLocalState byte 4, from the lightest at the mouth to
/// the darkest by the inner chamber, and blends outside the light while BG1 is
/// left out inside it. AltmillerCave_ApplyFadeBlend gives BG1 no weight and the
/// layers under it 16 sixteenths at level 0, two fewer at each level after,
/// down to 6 at level 5. Where one level meets the next, a cell of each lies
/// side by side, so the level is that of the cell the leader last stepped on,
/// in either direction. Reveal lights a dark room while it lasts and shows the
/// rocks' colours (AltmillerCave_StartReveal, AltmillerCave_EndReveal).
///
/// The rock puzzle keeps the rounds matched in gSceneLocalState byte 0 and its
/// state and the light at the top in struct RockPuzzle after it. Turning the
/// leftmost rock and then the rightmost brings out the ring of five lights; the
/// ring spins (AltmillerCave_SpinLights, a task) and stops with one light at
/// the top, and turning that light's rock spins it on to a different light. A
/// wrong rock puts the lights away. The third match bursts the wall
/// (AltmillerCave_ReleaseWallBurst). Coming up to the rocks from the pillar
/// cave (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) starts the puzzle over.

#include "main_routine_work.h"
#include "version.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/altmiller_cave.h"
#include "module/field/field_sequence.h"
#include "module/system/main_routine.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/shop/shop.h"
#include "module/system/system.h"
#include "iowrite.h"

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "pushed_actor.h"
#include "edition.h"
#include "actor_builtin_scripts.h"
#include "scene_ids.h"
#include "dma.h"
#include "io_reg.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "call.h"
#include "display_transition.h"
#include "field_particle.h"
#include "item_ids.h"
#include "actor_script.h"
#include "frame.h"
#include "fixed_math.h"
#include "scene.h"
#include "fx_work.h"
#include "flags.h"
#include "sounds.h"
#include "sound_driver_symbols.h"

/// AltmillerCaveBabi's actors: Babi while invisible and once seen, and
/// the two men from Tolbi who come looking for him.
enum {
  /// Babi's second sheet, SPRITE_INVISIBLE_BABI: the old man with
  /// long pink hair and a cane, standing unseen in the cave.
  /// ★ Named from the game: "Wrapped in Psynergetic light... Who could it
  /// be?" (MsgAltmillerSee)
  ACTOR_INVISIBLE_BABI = 8,
  /// SPRITE_BABI, the same old man once seen.
  /// ★ Named from the game: "Babi!" (MsgAltmillerBabi)
  ACTOR_BABI = 9,
  /// Their sprite is a helmeted soldier carrying a torch
  /// (SPRITE_SOLDIER_WITH_TORCH).
  /// ★ Named from what the player sees: a soldier from Tolbi, sent to find
  /// Babi (its render)
  ACTOR_FIRST_SOLDIER = 10,
  /// ★ Named from what the player sees: a second soldier from Tolbi (its
  /// render)
  ACTOR_SECOND_SOLDIER = 11
};

/// AltmillerCaveSquall and AltmillerCaveDraught place first the three fallen
/// columns the party pushes; AltmillerCaveSquall then a puddle, a pillar for
/// Move and Squall, a Jupiter Djinni.
enum {
  /// ★ Named from what the player sees: white stone columns lying across the
  /// floor, in the way (its render)
  ACTOR_FIRST_OBSTACLE = 8,
  /// ★ Named from what the player sees: a small blue puddle in the middle of
  /// the floor (its render)
  ACTOR_SQUALL_ROOM_PUDDLE = 11,
  /// ★ Named from what the player sees: a brown stone pillar by the room's
  /// east wall (its render)
  ACTOR_SQUALL_ROOM_PILLAR = 12,
  /// ★ Named from the game: "Squall" (MsgDjinnName)
  ACTOR_SQUALL = 13
};

/// AltmillerCaveDraught: the five coloured lights and the five rocks, each in
/// Babi's order, the rocks left to right.
enum {
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_FIRST_LIGHT = 11,
  /// ★ Named from the game: "The blue light corresponds to the rock farthest
  /// to the left." (MsgAltmillerDontTrustAnyone)
  ACTOR_BLUE_LIGHT = ACTOR_FIRST_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_GREEN_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_WHITE_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_YELLOW_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_RED_LIGHT,
  /// ★ Named from the game: "Turn the five rocks in accordance with those
  /// colors." (MsgAltmillerDontTrustAnyone)
  ACTOR_FIRST_ROCK,
  /// ★ Named from the game: "The blue light corresponds to the rock farthest
  /// to the left." (MsgAltmillerDontTrustAnyone)
  ACTOR_BLUE_ROCK = ACTOR_FIRST_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_GREEN_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_WHITE_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_YELLOW_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  ACTOR_RED_ROCK
};

enum { LIGHT_COUNT = 5 };

/// The palette a light or rock is drawn in, as its child value: the five
/// colors in the same order, and none for a rock while Reveal is off.
enum {
  COLOR_BLUE = 1,
  COLOR_GREEN = 4,
  COLOR_WHITE = 11,
  COLOR_YELLOW = 2,
  COLOR_RED = 3,
  COLOR_NONE = 6
};

/// The scene temp flags the cave keeps.
enum {
  /// The cave is dark: set on entry everywhere but AltmillerCaveSquall and
  /// AltmillerCaveDraught.
  /// ★ Named from what the player sees: the cave dark but for a small circle of
  /// light around the party (its render)
  FLAG_ALTMILLER_CAVE_DARK = FLAG_SCENE_TEMP_00,
  /// AltmillerCaveEntrance darkens by steps as the party walks in.
  /// ★ Named from its area name: Altmiller Cave
  FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS = FLAG_SCENE_TEMP_01,
  /// Frost has frozen AltmillerCaveSquall's puddle.
  /// ★ Named from what the player sees: a small puddle of water in the middle
  /// of the room (its render)
  FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN = FLAG_SCENE_TEMP_11,
  /// AltmillerCaveSquall's pillar stands where Move opens the passage.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE = FLAG_SCENE_TEMP_12
};

/// The darkness: BG1 blended over every layer, at the weights
/// BLEND_ALPHA gives each level.
#define ALTMILLER_CAVE_DARKNESS_BLEND                                          \
  (BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL)
/// A dark room's weights, which AltmillerCaveEntrance's levels do not use.
#define ALTMILLER_CAVE_DARK_ALPHA BLEND_ALPHA(4, 12)

// The light around the party is a window, an ellipse a little taller than
// wide around the leader (WINDOW_TRANSITION_LEADER_LOW); the transition's
// progress sets its size, 5 pixels to each side of the leader for each step.
/// The light's progress: 25 pixels to each side of the leader.
#define ALTMILLER_CAVE_LIGHT_STEPS 5
/// The light's progress once Babi is found: wide enough to fill the screen.
#define ALTMILLER_CAVE_LIGHT_FULL 32
/// Inside the light where AltmillerCaveEntrance darkens by steps: every layer
/// but BG1, with no blend, so the party's surroundings show at full
/// brightness.
#define ALTMILLER_CAVE_STEPS_LIGHT_WININ                                       \
  (WININ_WIN0_BG0 | WININ_WIN0_BG2 | WININ_WIN0_BG3 | WININ_WIN0_OBJ |         \
   WININ_WIN1_BG0 | WININ_WIN1_BG2 | WININ_WIN1_BG3 | WININ_WIN1_OBJ)
/// Outside that light: everything, blended to the step's darkness.
#define ALTMILLER_CAVE_STEPS_LIGHT_WINOUT WINOUT_WIN01_ALL
/// Inside the light of an evenly dark room: everything, blended at
/// ALTMILLER_CAVE_DARK_ALPHA.
#define ALTMILLER_CAVE_DARK_LIGHT_WININ (WININ_WIN0_ALL | WININ_WIN1_ALL)
/// Outside it: every layer with no blend, so BG1 covers the room.
#define ALTMILLER_CAVE_DARK_LIGHT_WINOUT                                       \
  (WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 | \
   WINOUT_WIN01_OBJ)
/// Outside the light once Babi is found: everything, blended, so the whole
/// room brightens as BG1 fades.
#define ALTMILLER_CAVE_BRIGHTENING_WINOUT (WINOUT_WIN01_ALL | WINOUT_WINOBJ_ALL)

/// The puddle's animation once Frost has frozen it.
enum { PUDDLE_ANIMATION_FROZEN = 5 };

static void AltmillerCave_ReleaseWallBurst(void);

/// The ring of five lights above AltmillerCaveDraught's rocks as it spins to
/// its next light: angles are 16-bit turns, the light at the top is the chosen
/// one.
struct LightWheel {
  s16 phase;
  s16 frames;
  s16 unused; ///< nothing reads or writes it
  u16 angle;
  u16 speed;
  /// How far the ring has turned since its last click.
  u16 travel;
};

/// The ring speeds up, coasts half a second, slows down, then stops with the
/// chosen light at the top.
enum {
  WHEEL_SPEEDING_UP,
  WHEEL_COASTING,
  WHEEL_SLOWING,
  WHEEL_STOPPING,
  WHEEL_STOPPED = 99
};

/// AltmillerCaveDraught's rock puzzle, kept in gSceneLocalState: the rounds of
/// lights matched in byte 0, then the state and the light shown.
struct RockPuzzle {
  u8 state;
  s8 chosen;
};

#define ROCK_PUZZLE ((struct RockPuzzle *)&gSceneLocalState[1])

/// The puzzle's states: the leftmost rock turned, then the rightmost, which
/// shows the lights; after three lights matched the wall opens.
enum {
  PUZZLE_START,
  PUZZLE_LEFT_ROCK_TURNED,
  PUZZLE_LIGHTS_SHOWN,
  PUZZLE_SOLVED = 99
};

struct LightWheel gAltmillerCave_LightWheelState
    __attribute__((nocommon, aligned(4)));

#include "shared/staged_actor.h"

/// Talking to the Djinni in AltmillerCaveSquall starts the battle with Squall,
/// a Jupiter Djinni.
static void AltmillerCave_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_SQUALL, DJINNI_SQUALL);
}

/// The six rooms share one entrance table.
const struct SceneEntrance *AltmillerCave_GetEntrances(void) {
  return gAltmillerCave_Entrances;
}

const struct SceneExitRegion *AltmillerCave_GetRegions(void) { return NULL; }

const u32 *AltmillerCave_GetExits(void) { return gAltmillerCave_Exits; }

const struct ActorPlacement *AltmillerCave_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_AltmillerCaveBabi) {
    return gAltmillerCave_BabiActorPlacements;
  }
  if (sceneId == (s32)&SceneId_AltmillerCaveSquall) {
    return gAltmillerCave_SquallActorPlacements;
  }
  if (sceneId == (s32)&SceneId_AltmillerCaveDraught) {
    return gAltmillerCave_DraughtActorPlacements;
  }
  return gAltmillerCave_ActorPlacementsOther;
}

/// Darkens AltmillerCaveEntrance to its current level: the brighter blend at
/// level 0, each level one step darker.
static void AltmillerCave_ApplyFadeBlend(void) {
  s8 level = ((s8 *)gSceneLocalState)[4];

  Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
  if (level == 0) {
    s32 alpha = BLEND_ALPHA(0, 16);

    REG_BLDALPHA = alpha;
  } else if (level == 1) {
    s32 alpha = BLEND_ALPHA(0, 14);

    REG_BLDALPHA = alpha;
  } else if (level == 2) {
    s32 alpha = BLEND_ALPHA(0, 12);

    REG_BLDALPHA = alpha;
  } else if (level == 3) {
    s32 alpha = BLEND_ALPHA(0, 10);

    REG_BLDALPHA = alpha;
  } else if (level == 4) {
    s32 alpha = BLEND_ALPHA(0, 8);

    REG_BLDALPHA = alpha;
  } else {
    s32 alpha = BLEND_ALPHA(0, 6);

    REG_BLDALPHA = alpha;
  }
}

/// Keeps AltmillerCaveEntrance's darkness level, and shows it unless Reveal is
/// lighting the room.
static void AltmillerCave_SetFadeLevel(s32 level) {
  struct FieldWork *work;
  u8 *fade = &gSceneLocalState[4];

  work = gFieldWork;
  *fade = level;
  if (work->revealActive == 0) {
    AltmillerCave_ApplyFadeBlend();
  }
}

/// Crossing the first darkness boundary sets the cave's lightest level.
/// AltmillerCaveEntrance's boundaries are step triggers 60 to 65:
/// crossing one sets its level, 0 to 5, each a step darker than the last.
static void AltmillerCave_DarkenToLevel0(void) {
  AltmillerCave_SetFadeLevel(0);
}

/// Crossing the second darkness boundary (step trigger 61) sets darkness level
/// 1.
static void AltmillerCave_DarkenToLevel1(void) {
  AltmillerCave_SetFadeLevel(1);
}

/// Crossing the third darkness boundary (step trigger 62) sets darkness
/// level 2.
static void AltmillerCave_DarkenToLevel2(void) {
  AltmillerCave_SetFadeLevel(2);
}

/// Crossing the fourth darkness boundary (step trigger 63) sets darkness level
/// 3.
static void AltmillerCave_DarkenToLevel3(void) {
  AltmillerCave_SetFadeLevel(3);
}

/// Crossing the fifth darkness boundary (step trigger 64) sets darkness
/// level 4.
static void AltmillerCave_DarkenToLevel4(void) {
  AltmillerCave_SetFadeLevel(4);
}

/// Crossing the sixth darkness boundary (step trigger 65) sets darkness
/// level 5.
static void AltmillerCave_DarkenToLevel5(void) {
  AltmillerCave_SetFadeLevel(5);
}

/// Darkens the cave on entry: AltmillerCaveEntrance by its levels, the other
/// dark rooms evenly.
static void AltmillerCave_InitializeDarkness(void) {
  u8 **workCells = (u8 **)&gFieldWork;
  struct ScreenTransitionState *state;

  {
    struct FieldWork *work = (struct FieldWork *)*workCells;
    work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transitionFrames = 24;
  }
  wait(1);
  ScreenTransition_InitializeBattleEffectState(WINDOW_TRANSITION_LEADER_LOW);
  // UB: the fourth following pointer cell lies past scalar gFieldWork.
  // Its named load adds eight bytes in the native EN trial (0x98 to 0xa0).
#ifdef UBFIX
  state = gScreenTransitionWork;
#else
  state = (struct ScreenTransitionState *)workCells[4];
#endif
  {
    u16 *slot = &state->value;
    s32 value = ALTMILLER_CAVE_LIGHT_STEPS;
    *slot = value;
  }
  if (GameFlag_Get(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS) != 0) {
    {
      s16 *slot = &state->mask;
      s32 value = ALTMILLER_CAVE_STEPS_LIGHT_WININ;
      *slot = value;
    }
    {
      s16 *slot = &state->active;
      s32 value = ALTMILLER_CAVE_STEPS_LIGHT_WINOUT;
      *slot = value;
    }
    AltmillerCave_ApplyFadeBlend();
    return;
  }
  {
    s16 *slot = &state->mask;
    s32 value = ALTMILLER_CAVE_DARK_LIGHT_WININ;
    *slot = value;
  }
  {
    s16 *slot = &state->active;
    s32 value = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;
    *slot = value;
  }
  {
    s32 blendControl = ALTMILLER_CAVE_DARKNESS_BLEND;
    REG_BLDCNT = blendControl;
  }
  {
    s32 blendAlpha = ALTMILLER_CAVE_DARK_ALPHA;
    REG_BLDALPHA = blendAlpha;
  }
}

/// Reveal's light: AltmillerCaveEntrance is lit and AltmillerCaveDraught's
/// rocks show their colors.
static void AltmillerCave_StartReveal(void) {
  if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveEntrance) {
    s32 alpha = BLEND_ALPHA(0, 16);

    REG_BLDALPHA = alpha;
  }
  if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveDraught) {
    Actor_ConfigureSpriteColor(ACTOR_BLUE_ROCK, COLOR_BLUE);
    Actor_ConfigureSpriteColor(ACTOR_GREEN_ROCK, COLOR_GREEN);
    Actor_ConfigureSpriteColor(ACTOR_WHITE_ROCK, COLOR_WHITE);
    Actor_ConfigureSpriteColor(ACTOR_YELLOW_ROCK, COLOR_YELLOW);
    Actor_ConfigureSpriteColor(ACTOR_RED_ROCK, COLOR_RED);
  }
}

/// Reveal's light ends: the cave darkens again and AltmillerCaveDraught's rocks
/// lose their colors.
static void AltmillerCave_EndReveal(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;

  if (GameFlag_Get(FLAG_ALTMILLER_CAVE_DARK) != 0) {
    AltmillerCave_InitializeDarkness();
    effectWork->childOption = 1;
  }
  if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveDraught) {
    Actor_ConfigureSpriteColor(ACTOR_BLUE_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_GREEN_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_WHITE_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_YELLOW_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_RED_ROCK, COLOR_NONE);
  }
}

/// Retreat's first phase: the Psynergy's first actor faces south, and
/// it is copied into a new actor for the Psynergy to use.
static void AltmillerCave_StartRetreat(void) {
  struct FieldEffectWork *effectWork;
  struct Actor *original;
  struct Actor *replacement;

  effectWork = (struct FieldEffectWork *)gFieldEffectWork;
  original = effectWork->mainActor;
  Actor_ArmCallback(effectWork->firstActorId, FACING_SOUTH, 0);
  Actor_SetSpriteColorMode(original, 0);
  Event_Wait(20);
  replacement = FieldActor_Create(0, original->x.fixed, original->y.fixed,
                                  original->z.fixed);
  if (replacement != 0) {
    Dma_Set(original, replacement,
            (DMA_ENABLE | DMA_32BIT) << 16 | sizeof(struct Actor) / 4,
            REG_DMA3);
    original->update = 0;
    effectWork->mainActor = replacement;
    original->spriteStorageKind = 0;
  }
}

/// After Move on AltmillerCaveSquall's pillar: in its cell at (30, 20) it
/// settles and opens the way beside it (remembered in TBS_PATCH_1 and later).
static void AltmillerCave_UpdateMovedObstacle(void) {
  struct Actor *actor = Actor_GetById(ACTOR_SQUALL_ROOM_PILLAR);

  if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
    actor->motionFlags = 2;
    actor->groundHeight = 0;
    actor->priorityFlags = 2;
    MapGrid_CopyCellAttributes(30, 20, 1, 1, 32, 20);
#if TBS_PATCH_1
    GameFlag_Set(FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE);
#endif
  }
}

/// Walking east into AltmillerCaveSquall's pillar pushes it a cell.
static void AltmillerCave_AdvanceObstacleAndCheckPassage(void) {
  PushedActor_AdvancePair();
  AltmillerCave_UpdateMovedObstacle();
}

/// Frost's first phase on AltmillerCaveSquall's puddle: notes whether its place
/// is blocked, which keeps it from freezing.
static void AltmillerCave_CheckFrostTargetCollision(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;
  struct Actor *target = Actor_GetById(ACTOR_SQUALL_ROOM_PUDDLE);
  struct FieldPosition position;

  position.x = target->x.fixed;
  position.y = target->y.fixed;
  position.z = target->z.fixed;

  if (FieldActor_CheckMovementCollision(target, &position) > 0) {
    effectWork->childMode = 1;
  }
}

/// Frost on AltmillerCaveSquall's puddle, unless blocked: it freezes and its
/// cell takes the frozen attributes kept at (76, 16).
static void AltmillerCave_FreezePuddle(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;
  struct Actor *puddle = Actor_GetById(ACTOR_SQUALL_ROOM_PUDDLE);
  s32 blocked;

  blocked = effectWork->childMode;
  if (blocked == 0) {
    MapGrid_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    if (puddle != 0) {
      s32 motionFlags = 2;
      puddle->motionFlags = motionFlags;
      puddle->priorityFlags = blocked;
    }
    GameFlag_Set(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN);
  }
}

/// Walking into AltmillerCaveSquall's puddle while it is not frozen: the leader
/// hops 32 pixels over it, if the landing is clear.
static void AltmillerCave_HopSelectedActor(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 motionFlags = actor->motionFlags;
  union {
    struct FieldPosition position;
    s32 coordinates[3];
    union FieldCoordinate components[3];
  } destination;
  union FieldCoordinate *components = destination.components;

  components[0].fixed = actor->x.fixed;
  components[1].fixed = actor->y.fixed;
  components[2].fixed = actor->z.fixed;
  {
    s32 angle = actor->facing & 0xf000;

    Vector_AddPolarOffset(PIXELS(32), angle, destination.coordinates);
  }
  if (FieldActor_CheckMovementCollision(actor, &destination.position) == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 126;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, components[0].part.pixel,
                               components[2].part.pixel);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, 1);
    actor->motionFlags = motionFlags;
    Event_End();
  }
}

/// Walking into one of the three obstacles pushes it, if there is room.
static void AltmillerCave_MoveStagedActorToClearPosition(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
  }
  Event_End();
}

/// Update routine of the light at the top of the stopped ring: it pulses
/// through four sizes.
static void AltmillerCave_PulseLight(struct Actor *actor) {
  struct Actor *light = (struct Actor *)actor;
  s32 scale = gAltmillerCave_PulseScales[(light->timer >> 2) & 3];

  light->scaleX = scale;
  light->scaleY = scale;
  light->timer++;
  light->timer &= 15;
}

/// Puts a light on the ring, radius pixels from its center above the rocks, at
/// the given angle.
static void AltmillerCave_PlaceLight(s32 actorId, s32 radius, s32 angle) {
  s32 centreX = PIXELS(504);
  s32 centreHeight = PIXELS(24);
  s32 depth = PIXELS(144);
  struct Actor *light = Actor_GetById(actorId);
  s32 coordinates[3];
  s32 *position = coordinates;

  position[0] = centreX;
  position[2] = centreHeight;
  Vector_AddPolarOffset(radius, angle, position);
  light->x.fixed = position[0];
  light->y.fixed = position[2];
  light->z.fixed = depth;
}

/// Spaces the five lights evenly around the ring, the first at the given
/// angle.
static void AltmillerCave_PlaceLights(s32 angle) {
  s32 i = 0;

  do {
    AltmillerCave_PlaceLight(ACTOR_FIRST_LIGHT + i, PIXELS(24), angle);
    angle -= 0x3333; // One fifth of a full turn.
    i++;
  } while (i < LIGHT_COUNT);
}

/// Task that spins the ring of lights: it speeds up, coasts, slows
/// and stops with the chosen light at the top, which then pulses; a click
/// (SE_CLICK) sounds for each fifth of a turn.
static void AltmillerCave_SpinLights(void) {
  struct LightWheel *wheel = gAltmillerCave_LightWheel;
  s32 moving = 1;
  s32 phase = wheel->phase;

  if (phase == WHEEL_SPEEDING_UP) {
    wheel->speed += 16;
    if (wheel->speed > 0xbff) {
      wheel->phase++;
      wheel->frames = 0;
    }
  } else if (phase == WHEEL_COASTING) {
    if (wheel->frames == 30) {
      wheel->phase++;
    }
  } else if (phase == WHEEL_SLOWING) {
    wheel->speed -= 8;
    if (wheel->speed <= 0x2ff) {
      wheel->phase++;
    }
  } else if (phase == WHEEL_STOPPING) {
    s32 chosenLight = ((s8 *)gSceneLocalState)[2];
    s32 chosenAngle = Math_Divide(chosenLight * Q16_ONE, 5);
    if ((unsigned int)(((u32)(wheel->angle - chosenAngle) << 16) +
                       0xc2ff0000) <= 0x5fe0000) {
      wheel->angle = chosenAngle + 0x4000;
      wheel->phase = WHEEL_STOPPED;
      wheel->speed = 0;
      Actor_GetById(ACTOR_FIRST_LIGHT + chosenLight)->update =
          AltmillerCave_PulseLight;
    }
  } else if (phase == WHEEL_STOPPED) {
    moving = 0;
  }
  if (moving != 0) {
    gAltmillerCave_LightWheel->angle += gAltmillerCave_LightWheel->speed;
    AltmillerCave_PlaceLights(gAltmillerCave_LightWheel->angle);
    gAltmillerCave_LightWheel->travel += gAltmillerCave_LightWheel->speed;
    if (gAltmillerCave_LightWheel->travel > 0x3000) {
      gAltmillerCave_LightWheel->travel = 0;
      Sound_Play(SE_CLICK);
    }
  }
  gAltmillerCave_LightWheel->frames++;
}

/// Talking to one of AltmillerCaveDraught's rocks rotates it. "Isaac rotated
/// the rock." Turning the leftmost rock, then the rightmost, brings out the
/// ring of lights; then each time the rock matching the light at the top is
/// turned, the ring spins to another light, and a wrong rock puts the lights
/// away and starts over. After three lights matched the ring shrinks away, the
/// wall bursts and the way to Babi's draught opens.
static void AltmillerCave_TurnRock(s32 rock) {
  s32 correctRock = 0;
  s32 i;
  struct Actor *actor;
  s32 scale;
  s8 state;
  s32 chosenLight;
  u8 previousLight;
  s32 radius;
  u16 angle;
  u8 *puzzleState;

  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgAltmillerRotatedRock);
  Event_ShowMessage(ACTOR_FIRST_ROCK, 0);
  Event_End();
  for (i = 0; i < LIGHT_COUNT; i++) {
    actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
    actor->update = NULL;
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  {
    struct LightWheel *wheel;

    state = ROCK_PUZZLE->state;
    previousLight = ROCK_PUZZLE->chosen;
    wheel = gAltmillerCave_LightWheel;
    chosenLight = ROCK_PUZZLE->chosen;
    wheel->angle = (chosenLight * Q16_ONE) / 5 + 0x4000;
  }
  // The leftmost rock, then the rightmost, before the rounds of lights.
  if (state == PUZZLE_START) {
    if (rock == ACTOR_BLUE_ROCK) {
      state = PUZZLE_LEFT_ROCK_TURNED;
      Event_PlaySound(SE_CHIME);
    } else {
      Event_PlaySound(SE_MENU_ERROR);
    }
    gSceneLocalState[0] = 0;
  } else if (state == PUZZLE_LEFT_ROCK_TURNED) {
    if (rock == ACTOR_BLUE_ROCK) {
      Event_PlaySound(SE_CHIME);
    } else if (rock == ACTOR_RED_ROCK) {
      // The lights grow in one by one, each with SE_LIGHT_APPEAR.
      state = PUZZLE_LIGHTS_SHOWN;
      Event_PlaySound(SE_CHIME);
      wait(30);
      angle = gAltmillerCave_LightWheel->angle;
      for (i = 0; i < LIGHT_COUNT; i++) {
        AltmillerCave_PlaceLight(ACTOR_FIRST_LIGHT + i, PIXELS(24), angle);
        Event_PlaySound(SE_LIGHT_APPEAR);
        actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
        actor->scaleX = 0;
        scale = 2 * Q16_ONE / 5;
        do {
          actor->scaleY = scale;
          actor->scaleX = scale;
          wait(1);
          scale += 3 * Q16_ONE / 64;
        } while (actor->scaleX < Q16_ONE);
        angle -= 0x3333;
      }
      wait(30);
      correctRock = 1;
    } else {
      Event_PlaySound(SE_MENU_ERROR);
      state = PUZZLE_START;
    }
  } else if (state == PUZZLE_LIGHTS_SHOWN) {
    if (rock != ACTOR_FIRST_ROCK + chosenLight) {
      // The wrong rock: the lights shrink away.
      state = PUZZLE_START;
      Event_PlaySound(SE_MENU_ERROR);
      wait(30);
      for (i = 0; i < LIGHT_COUNT; i++) {
        actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
        Event_PlaySound(SE_LIGHT_APPEAR);
        for (scale = actor->scaleX; actor->scaleX > 2 * Q16_ONE / 5;
             scale -= 3 * Q16_ONE / 64) {
          actor->scaleY = scale;
          actor->scaleX = scale;
          wait(1);
        }
        Actor_SetPosition(ACTOR_FIRST_LIGHT + i, 0, 0);
      }
    } else {
      Event_PlaySound(SE_CHIME);
      correctRock = 1;
      wait(30);
    }
  }
  puzzleState = &gSceneLocalState[1];
  puzzleState[0] = state;
  // A light matched: choose a different light and spin the ring to it.
  if (correctRock) {
    u8 rounds = ++puzzleState[-1];

    puzzleState[1] =
        ((s8)(((u32)(Random_Next() << 2) >> 16) + previousLight + 1) + 5) % 5;
    gAltmillerCave_LightWheel->phase = WHEEL_SPEEDING_UP;
    gAltmillerCave_LightWheel->frames = 0;
    gAltmillerCave_LightWheel->speed = 0x200;
    gAltmillerCave_LightWheel->travel = 0x3000;
    Task_AddFrame(AltmillerCave_SpinLights, TASK_PASS_FRAME);
    if (rounds <= 2) {
      while (gAltmillerCave_LightWheel->phase != WHEEL_STOPPED) {
        wait(1);
      }
      wait(10);
      Event_PlaySound(SE_CHIME);
    } else {
      // The third light matched: the ground shakes, the ring spirals in to
      // nothing, and the wall bursts.
      puzzleState[0] = PUZZLE_SOLVED;
      while (gAltmillerCave_LightWheel->phase != WHEEL_SLOWING) {
        wait(1);
      }
      gAltmillerCave_LightWheel->phase = WHEEL_SLOWING;
      gAltmillerCave_LightWheel->frames = 0;
      MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
      Event_Wait(20);
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      gAltmillerCave_LightWheel->phase = WHEEL_STOPPED;
      Event_PlaySound(SE_WARP);
      radius = PIXELS(24);
      angle = gAltmillerCave_LightWheel->angle;
      do {
        for (i = 0; i < LIGHT_COUNT; i++) {
          actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
          actor->scaleX -= 16;
          actor->scaleY -= 16;
          AltmillerCave_PlaceLight(ACTOR_FIRST_LIGHT + i, radius, angle);
          angle -= 0x3333;
        }
        radius -= PIXELS(1) / 5;
        angle += 0xc00;
        wait(1);
      } while (radius > 0);
      for (i = 0; i < LIGHT_COUNT; i++) {
        Actor_SetPosition(ACTOR_FIRST_LIGHT + i, 0, 0);
      }
      AltmillerCave_ReleaseWallBurst();
      Event_PlaySound(MUS_PUZZLE_SOLVED);
    }
    Task_Remove(AltmillerCave_SpinLights);
  }
}

/// Divides the nonnegative random value for wall-fragment scaling.
s32 Math_DivideUnsigned(s32 value, s32 divisor);

/// The party turns north, toward Babi.
static __inline__ void AltmillerCave_FacePartyNorth(void) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
}

/// In AltmillerCaveBabi, talking to Babi or reading his mind once the Mystic
/// Draught is found: he drinks it and appears, startling the party. Ivan reads
/// his mind and finds him to be Babi, the ruler of Tolbi, who sought Lemurians
/// to escape growing old. Two soldiers from Tolbi find him and take him back,
/// and he invites the party to Colosso's finals; Garet asks whether they
/// should take part.
static void AltmillerCave_RestoreBabiAndInviteParty(void) {
  struct ScreenTransitionState *state;
  struct Actor *actor;
  struct Actor *leader;
  s32 i;
  s32 eva;
  s32 evb;

  // Give Babi the draught and bring the party beside him.
  GameFlag_Set(FLAG_BABI_SAVED);
  PartyInventory_Discard(ITEM_MYSTIC_DRAUGHT);
  Event_Start();
  Field_EndBusyAndReveal();
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgAltmillerCameBackPleaseDraughtQuickly);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(50);
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(20);
  actor = Actor_Get(ACTOR_INVISIBLE_BABI);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  actor->sprite->rotation = 0;
  Actor_Jump(ACTOR_INVISIBLE_BABI, 10, 70);
  // Flicker between sprite priorities before replacing invisible Babi.
  for (i = 0; i < 30; i++) {
    actor->sprite->image.priority = 0;
    Event_Wait(2);
    actor->sprite->image.priority = 2;
    Event_Wait(2);
  }
  Event_Wait(40);
  {
    s32 x;
    s32 z;

    x = Actor_Get(ACTOR_INVISIBLE_BABI)->x.part.pixel;
    z = Actor_Get(ACTOR_INVISIBLE_BABI)->z.part.pixel;
    Actor_SetPosition(ACTOR_INVISIBLE_BABI, 0, 0);
    Actor_SetPosition(ACTOR_BABI, PIXELS(x), PIXELS(z));
  }
  actor->sprite->flags = 0;
  Event_PlaySound(MUS_BABI);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_BABI, 32, 32);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, 16);
  Actor_WalkBy(ACTOR_GARET, 0, 16);
  Actor_WalkBy(ACTOR_MIA, 0, 16);
  Actor_WalkByAndWait(ACTOR_IVAN, 0, 16);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(ACTOR_IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  wait(1);
  AltmillerCave_FacePartyNorth();
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_Jump(ACTOR_GARET, 4, 13);
  Actor_Jump(ACTOR_GARET, 4, 30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_GARET, 30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_IVAN, 30);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Event_OpenMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessageId(MsgAltmillerWouldHaveRevealedMyselfSooner);
    Event_ShowMessage(ACTOR_BABI, 0);
  } else {
    Event_SetMessageId(MsgAltmillerMustTraveledIndeedIfDidnt);
    Event_ShowMessage(ACTOR_BABI, 0);
  }
  // Ask how Babi became invisible and hear about his Alchemy.
  Event_SetMessageId(MsgAltmillerHowDidBecomeParalyzedWhile);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_IVAN, 0, -48);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Event_Wait(40);
  // Ivan reads Babi's mind: he is Babi, the ruler of Tolbi, and hid by the
  // power of an ancient civilization.
  FieldPsynergy_LoadResources(ABILITY_MIND_READ, 1);
  FieldPsynergy_SetupActorPair(ACTOR_IVAN, ACTOR_BABI);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  Event_Wait(150);
  FieldPsynergy_RunHandler(2);
  FieldPsynergy_StopCasterUpdate();
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(40);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_BABI, 4, 13);
  Actor_Jump(ACTOR_BABI, 4, 30);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(50);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_IVAN, 0, 48);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Event_Wait(50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Event_Wait(50);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(MsgAltmillerThatsKradenSaidAlchemyCould);
    Event_ShowMessage(ACTOR_GARET, 0);
  } else {
    Event_SetMessageId(MsgAltmillerForgotKradenSaidAlchemyCould);
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_SetMessageId(MsgAltmillerWhatWasThat);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  Event_OpenMessage(ACTOR_BABI, 0);
  Event_SetMessageId(MsgAltmillerBabi);
  Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
  // The cave brightens as two men from Tolbi arrive: "Babi!" The light
  // widens to fill the screen and BG1 fades out.
  state = gScreenTransitionWork;
  {
    u16 *slot = &state->value;
    s32 value = ALTMILLER_CAVE_LIGHT_FULL;

    *slot = value;
  }
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = ALTMILLER_CAVE_DARKNESS_BLEND;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = BLEND_ALPHA(evb, eva);
    wait(7);
  }
  {
    u16 *slot = (u16 *)&state->active;
    s32 value = ALTMILLER_CAVE_BRIGHTENING_WINOUT;

    *slot = value;
  }
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, PIXELS(280), PIXELS(304));
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, PIXELS(296), PIXELS(304));
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Event_Wait(10);
  Event_Wait(10);
  FieldCamera_FollowActor(ACTOR_FIRST_SOLDIER, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(70);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_FIRST_SOLDIER);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_FIRST_SOLDIER);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_FIRST_SOLDIER);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_FIRST_SOLDIER);
  // The two men greet Babi; he invites the party to the finals.
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(7) / 5, PIXELS(7) / 10);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(7) / 5, PIXELS(7) / 10);
  Actor_SetScript(ACTOR_FIRST_SOLDIER, gAltmillerCave_GuardsArrivePath);
  Event_Wait(3);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, -16, 0);
  Actor_SetScript(ACTOR_SECOND_SOLDIER, gAltmillerCave_GuardsArrivePath);
  Actor_RefreshSelectorById(ACTOR_FIRST_SOLDIER);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 0, -16);
  Actor_RefreshSelectorById(ACTOR_SECOND_SOLDIER);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  AltmillerCave_FacePartyNorth();
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(216), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(50);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 16, 0);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_SECOND_SOLDIER, 4, 13);
  Actor_Jump(ACTOR_SECOND_SOLDIER, 4, 30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
  Event_Wait(70);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 70);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Event_Wait(30);
  Event_Wait(10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(40);
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 48);
  Actor_WalkBy(ACTOR_SECOND_SOLDIER, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 32);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_SECOND_SOLDIER, ANIM_STAND);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_GARET, ACTOR_BABI, 30);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(70);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  // Babi and the two men walk back toward the cave entrance.
  Actor_SetScript(ACTOR_SECOND_SOLDIER, gAltmillerCave_SecondGuardLeavePath);
  Actor_SetScript(ACTOR_FIRST_SOLDIER, gAltmillerCave_FirstGuardLeavePath);
  Actor_SetScript(ACTOR_BABI, gAltmillerCave_BabiLeavePath);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Actor_RefreshSelectorById(ACTOR_BABI);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(248), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Event_Wait(40);
  Actor_WalkBy(ACTOR_BABI, 32, 0);
  Actor_WalkBy(ACTOR_SECOND_SOLDIER, 0, 64);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 64);
  Actor_WaitForMoveById(ACTOR_BABI);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 64);
  Actor_SetPosition(ACTOR_BABI, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, 0, 0);
  Event_Wait(20);
  // With Babi gone the darkness returns: BG1 fades back in, then the light
  // shrinks around the party again.
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = ALTMILLER_CAVE_DARKNESS_BLEND;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = BLEND_ALPHA(eva, evb);
    wait(7);
  }
  {
    u16 *slot = &state->value;
    s32 value = ALTMILLER_CAVE_LIGHT_STEPS;

    *slot = value;
  }
  {
    u16 *slot = (u16 *)&state->active;
    s32 value = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;

    *slot = value;
  }
  wait(1);
  Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
  Io_Write16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
  Actor_PlaceCurrentWithinCameraBounds(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Event_OpenMessage(ACTOR_GARET, 0);
  Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Event_SetMessageId(MsgAltmillerSoWellDoIt);
    Event_ShowMessage(ACTOR_GARET, 0);
  } else {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Event_SetMessageId(MsgAltmillerDontWantEitherDo);
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  // Let Isaac answer the invitation and reunite the party.
  Event_SetMessageId(MsgAltmillerEitherWayWereStuckHere);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessageId(MsgAltmillerGreatItsDecidedJustHave);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(10);
    Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(20);
    Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_SetMessageId(MsgAltmillerDontLetMeDownLike);
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(10);
    Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Scene_PlayMusic();
  Event_End();
}

/// In AltmillerCaveBabi, talking to the invisible Babi or reading his mind
/// the first time: he can be heard but not seen, cannot move, and asks the
/// party to bring his draught from deep in the cave, telling the order of the
/// five rocks.
static void AltmillerCave_MeetInvisibleBabi(void) {
  struct Actor *leader;
  s32 line;

  // Gather the party around the invisible voice.
  GameFlag_Set(FLAG_MET_INVISIBLE_BABI);
  Sound_Play(MUS_CAVE);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgAltmillerSee);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(184), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_INVISIBLE_BABI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(20);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldEffect_SpawnAttached(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_OpenMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(MsgAltmillerKiddingHaventActually);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(2), PIXELS(1));
    Actor_WalkByAndWait(ACTOR_GARET, 0, -16);
    Event_Wait(10);
    Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
    Event_ShowMessageAndWait(ACTOR_GARET, 0);
  } else {
    Event_SetMessageId(MsgAltmillerDontTrustAnyone);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(ACTOR_GARET, 0, -16);
    Event_Wait(10);
    Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    Event_ShowMessageAndWait(ACTOR_GARET, 0);
  }

  // Question Babi's condition before agreeing to find his draught.
  line = MsgAltmillerSaidCouldntMove;
  Event_SetMessageId(line);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(2), PIXELS(1));
  Actor_WalkByAndWait(ACTOR_IVAN, 0, -16);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_MIA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_MIA, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_CommitPositionAndActivate(ACTOR_GARET, 0, 16);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_CommitPositionAndActivate(ACTOR_IVAN, 0, 16);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  // Explain the hidden draught and the order of the five rocks.
  line += 7;
  Event_SetMessageId(line);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_TurnToFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_GARET, ACTOR_MIA, 0);
  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_MIA, 0);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_ResetAndSetPosition(ACTOR_IVAN, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_ResetAndSetPosition(ACTOR_MIA, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Scene_PlayMusic();
  Event_End();
}

/// In AltmillerCaveBabi, talking to Babi again before the draught is found:
/// he asks whether the party forgot the order of the rock colors, and tells it
/// again.
static void AltmillerCave_RepeatRockOrder(void) {
  s32 question;

  Event_Start();
  question = MsgAltmillerForgetOrderRock;
  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(ACTOR_INVISIBLE_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Event_SetMessageId(question + 1);
    Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  } else {
    Event_Wait(20);
    Event_SetMessageId(question + 2);
    Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  }
  Event_End();
}

/// In AltmillerCaveBabi, walking away from Babi (step trigger 19) the first
/// time after meeting him: he calls the party back and says where the draught
/// is. "Wait... The draught is... on a table behind the door."
static void AltmillerCave_BabiCallsPartyBack(void) {
  if (GameFlag_Get(FLAG_MET_INVISIBLE_BABI) == 0)
    return;
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0)
    return;

  GameFlag_Set(FLAG_BABI_CALLED_PARTY_BACK);
  Event_Start();
  Event_SetMessageId(MsgAltmillerWait);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(30);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_INVISIBLE_BABI, 0);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Event_End();
}

const struct Trigger *AltmillerCave_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.sceneId;

  if (sceneId == (s32)&SceneId_AltmillerCaveBabi) {
    return gAltmillerCave_BabiTriggers;
  }
  if (sceneId == (s32)&SceneId_AltmillerCaveVialChest) {
    return gAltmillerCave_VialChestTriggers;
  }
  if (sceneId == (s32)&SceneId_AltmillerCaveSquall) {
    return gAltmillerCave_SquallTriggers;
  }
  if (sceneId == (s32)&SceneId_AltmillerCaveCookieChest) {
    return gAltmillerCave_CookieChestTriggers;
  }
  if (sceneId == (s32)&SceneId_AltmillerCaveDraught) {
    return gAltmillerCave_DraughtTriggers;
  }
  return gAltmillerCave_TriggersOther;
}

/// Update routine of AltmillerCaveDraught's obstacles: drawn on the leader's
/// sprite priority.
static void AltmillerCave_MatchLeaderPriority(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;
  if (actor != NULL) {
    actor->priorityFlags = 0;
    actor->sprite->image.priority =
        Actor_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
  }
}

/// Redraws the three obstacles' cells where they stand.
static __inline__ void AltmillerCave_RedrawObstacleFootprints(void) {
  Scene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE);
  Scene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE + 1);
  Scene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE + 2);
}

/// Sets up the room being entered: an entry from outside the cave takes the
/// room's first entrance; AltmillerCaveEntrance picks its darkness from
/// the entrance; AltmillerCaveBabi's Babi is invisible until rescued;
/// AltmillerCaveSquall's obstacles and pillar and AltmillerCaveDraught's
/// obstacles, lights and rocks are restored. Returns 0.
s32 AltmillerCave_Initialize(void) {
  s8 *state;
  struct Actor *actor;
  s32 i;

  if (gMainRoutineWork.entrance == 0) {
    if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveBabi) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE;
    }
    if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveVialChest) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI;
    }
    if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveSquall) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST;
    }
    if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveCookieChest) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL;
    }
    if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveDraught) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST;
    }
  }
  GameFlag_Set(FLAG_ALTMILLER_CAVE_DARK);
  Event_ClearFlag(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
  if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveEntrance) {
    if (gMainRoutineWork.entrance == ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gSceneLocalState + 4) = 0;
      }
      GameFlag_Set(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
    }
    if (gMainRoutineWork.entrance ==
        ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gSceneLocalState + 4) = 5;
      }
      GameFlag_Set(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
    }
  }
  if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveBabi) {
    if (GameFlag_Get(FLAG_BABI_SAVED)) {
      Import_ActorSetPosition(ACTOR_INVISIBLE_BABI, 0, 0);
    } else {
      actor = Actor_GetById(ACTOR_INVISIBLE_BABI);
      actor->sprite->image.priority = 1;
      actor->sprite->flags = 2;
      actor->sprite->rotation = 0x4000;
    }
  }
  // Restore AltmillerCaveSquall's moved obstacles and frozen puddle.
  if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveSquall) {
    GameFlag_Clear(FLAG_ALTMILLER_CAVE_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    if (GameFlag_Get(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN)) {
      Import_ActorSetAnimation(ACTOR_SQUALL_ROOM_PUDDLE,
                               PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    } else {
      Actor_GetById(ACTOR_SQUALL_ROOM_PUDDLE)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
    }
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SQUALL_ROOM_PUDDLE), 0);
#if TBS_PATCH_1
    if (GameFlag_Get(FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE)) {
      MapGrid_CopyCellAttributes(30, 20, 1, 1, 32, 20);
    }
#endif
  }
  // Rebuild the final rock puzzle from its saved phase and chosen light.
  if (gMainRoutineWork.sceneId == (s32)&SceneId_AltmillerCaveDraught) {
    GameFlag_Clear(FLAG_ALTMILLER_CAVE_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    // UB: the update routines are stored through an s32 view of update, which
    // keeps the entrance read below the last store (as ->update, one store
    // moves: 1 of 403 instructions differ in tbs-en).
#ifdef UBFIX
    Actor_GetById(ACTOR_FIRST_OBSTACLE)->update =
        AltmillerCave_MatchLeaderPriority;
    Actor_GetById(ACTOR_FIRST_OBSTACLE + 1)->update =
        AltmillerCave_MatchLeaderPriority;
    Actor_GetById(ACTOR_FIRST_OBSTACLE + 2)->update =
        AltmillerCave_MatchLeaderPriority;
#else
    *(s32 *)&Actor_GetById(ACTOR_FIRST_OBSTACLE)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Actor_GetById(ACTOR_FIRST_OBSTACLE + 1)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Actor_GetById(ACTOR_FIRST_OBSTACLE + 2)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
#endif
    // Coming up to the rocks (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE)
    // starts the puzzle over, with the red light first.
    if (gMainRoutineWork.entrance ==
        ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) {
      Dma_Fill32(0, gAltmillerCave_LightWheel, 12);
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        u8 *puzzleState = gSceneLocalState;

        puzzleState[0] = 0;
        puzzleState[1] = PUZZLE_START;
        puzzleState[2] = ACTOR_RED_LIGHT - ACTOR_FIRST_LIGHT;
      }
    }
    state = (s8 *)&gSceneLocalState[1];
    if (state[0] == PUZZLE_SOLVED) {
      MapGrid_CopyCellsLayered(41, 55, 3, 2, 30, 55);
      MapGrid_CopyCellAttributes(42, 8, 1, 1, 31, 8);
    }
    if (state[0] == PUZZLE_LIGHTS_SHOWN) {
      AltmillerCave_PlaceLights((state[1] * Q16_ONE) / 5 + 0x4000);
    }
    for (i = 0; i < LIGHT_COUNT; i++) {
      actor = Actor_GetById(ACTOR_FIRST_LIGHT + i);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      actor->scaleX = Q16_ONE;
      actor->scaleY = Q16_ONE;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_LIGHT + i), 0);
      Import_ActorSetAnimation(ACTOR_FIRST_LIGHT + i, i + 1);
    }
    Actor_ConfigureSpriteColor(ACTOR_BLUE_LIGHT, COLOR_BLUE);
    Actor_ConfigureSpriteColor(ACTOR_GREEN_LIGHT, COLOR_GREEN);
    Actor_ConfigureSpriteColor(ACTOR_WHITE_LIGHT, COLOR_WHITE);
    Actor_ConfigureSpriteColor(ACTOR_YELLOW_LIGHT, COLOR_YELLOW);
    Actor_ConfigureSpriteColor(ACTOR_RED_LIGHT, COLOR_RED);
    Actor_ConfigureSpriteColor(ACTOR_BLUE_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_GREEN_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_WHITE_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_YELLOW_ROCK, COLOR_NONE);
    Actor_ConfigureSpriteColor(ACTOR_RED_ROCK, COLOR_NONE);
    Actor_GetById(ACTOR_BLUE_ROCK)->sprite->image.priority = 3;
    Actor_GetById(ACTOR_RED_ROCK)->sprite->image.priority = 3;
    Actor_GetById(ACTOR_BLUE_ROCK)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_RED_ROCK)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_BLUE_ROCK), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_RED_ROCK), 0);
  }
  if (GameFlag_Get(FLAG_ALTMILLER_CAVE_DARK)) {
    AltmillerCave_InitializeDarkness();
  } else {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gFieldWork->transitionFrames = 24;
  }
  return 0;
}

/// Update routine of a piece of the burst wall: it flies off to a random
/// point 120 pixels away, with a crash every fourth frame as they start.
static void AltmillerCave_UpdateWallFragment(struct FieldParticle *fragment) {
  s32 position[3];
  s32 x;
  s32 z;
  u8 *phase;
  s32 currentPhase;

  phase = (u8 *)&fragment->state;
  currentPhase = *(s8 *)phase;
  if (currentPhase == 0) {
    z = fragment->originZ;
    x = fragment->originX;
    fragment->z = z;
    position[2] = z;
    fragment->x = x;
    position[0] = x;
    Vector_AddPolarOffset(PIXELS(120), Import_RandomNext(), position);
    fragment->targetX = position[0];
    fragment->targetZ = position[2];
    fragment->acceleration = PIXELS(5);
    fragment->speedMax = PIXELS(5);
    fragment->turnStepLimited = currentPhase;
    (*phase)++;
    if ((gVBlankCount & 3) == 0)
      Sound_Play(SE_HEAVY_IMPACT);
  } else if (currentPhase == 1) {
    if (FieldParticle_HasPendingTarget(fragment) == 0) {
      s32 nextPhase = *phase;
      nextPhase--;
      *phase = nextPhase;
    }
  } else if (currentPhase == 2) {
    if (FieldParticle_HasPendingTarget(fragment) == 0)
      FieldEffect_ClearOwnedSlot(fragment);
  }
}

/// The wall bursts: 24 pieces fly out from the ring's center, the wall's
/// cells open to the passage, and the shaking calms.
static void AltmillerCave_ReleaseWallBurst(void) {
  struct FieldEffectWork *effectWork;
  struct Vector3 position;
  struct FieldParticle *fragment;
  s32 fragmentIndex;

  FieldEffect_InitializeSlots();
  effectWork = (struct FieldEffectWork *)gFieldEffectWork;
  Shop_SaveAndTintFieldPalette(0x202108);
  position.x = PIXELS(504);
  position.y = PIXELS(24);
  position.z = PIXELS(144);
  Camera_WorldToScreen(&position);
  fragment = effectWork->particles;
  for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
    s32 scale;

    FieldParticle_Initialize(fragment, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(fragment, AltmillerCave_UpdateWallFragment);
    FieldParticle_SetActorMode(fragment, 7);
    Sprite_SetAnimationColorModes(fragment->actor,
                                  (u32)(Import_RandomNext() * 7) >> 16);
    scale = Math_DivideUnsigned(Import_RandomNext(), 3) + 3 * Q16_ONE / 2;
    fragment->scaleY = scale;
    fragment->scaleX = scale;
    wait(1);
    fragment++;
  }
  wait(80);
  MapGrid_CopyCellsLayered(41, 55, 3, 2, 30, 55);
  MapGrid_CopyCellAttributes(42, 8, 1, 1, 31, 8);
  wait(50);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  wait(30);
  {
    s32 mode = 2;
    // Walk the state bytes within the complete field effect sprite array.
    u8 *stateCursor =
        (u8 *)&effectWork->particles + offsetof(struct FieldParticle, state);

    for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
      if (((s8 *)stateCursor)[offsetof(struct FieldParticle, active) -
                              offsetof(struct FieldParticle, state)] != 0) {
        *stateCursor = mode;
      }
      stateCursor += sizeof(struct FieldParticle);
    }
  }
  MapGrid_WaitForShakeToSettle();
  Shop_RestoreFieldPalette();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

struct LightWheel *gAltmillerCave_LightWheel = &gAltmillerCave_LightWheelState;

struct SceneEntrance gAltmillerCave_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP, CONDITION_ALWAYS, 456, 0, 504,
     FACING_NORTH, 0, 0, 0, 544, 528, 0},
    {ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER, CONDITION_ALWAYS, 280, 0, 360,
     FACING_SOUTH, 0, 0, 0, 544, 528, 0},
    {ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE, CONDITION_ALWAYS, 120,
     0, 696, FACING_NORTH, 0, 0, 552, 240, 720, 0},
    {ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, 120, 0,
     600, FACING_SOUTH, 0, 0, 552, 240, 720, 0},
    {ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE, CONDITION_ALWAYS, 392, 0,
     504, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS, 136,
     0, 104, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI, CONDITION_ALWAYS, 232,
     0, 216, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS,
     328, 0, 264, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST, CONDITION_ALWAYS,
     200, 0, 424, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS,
     520, 0, 328, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL, CONDITION_ALWAYS,
     680, 0, 152, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT, CONDITION_ALWAYS,
     40, 0, 248, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST, CONDITION_ALWAYS,
     296, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS, CONDITION_ALWAYS, 424, 0, 392,
     FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE, CONDITION_ALWAYS, 504, 0,
     232, FACING_NORTH, 0, 384, 64, 624, 240, 0},
    {ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST, CONDITION_ALWAYS, 504, 0, 152,
     FACING_SOUTH, 0, 384, 64, 624, 240, 0},
    {ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS, CONDITION_ALWAYS, 184, 0, 216,
     FACING_NORTH, 0, 64, 64, 304, 240, 0},
    {SCENE_TABLE_END},
};

u32 gAltmillerCave_Exits[] = {
    SCENE_EXITS(AltmillerCaveEntrance),
    SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_ALTMILLER_CAVE_ENTRANCE),
    SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER, AltmillerCaveEntrance,
               ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE),
    SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE,
               AltmillerCaveEntrance,
               ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER),
    SCENE_EXIT(ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI,
               AltmillerCaveBabi,
               ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE),
    SCENE_EXITS(AltmillerCaveBabi),
    SCENE_EXIT(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE,
               AltmillerCaveEntrance,
               ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI),
    SCENE_EXIT(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST,
               AltmillerCaveVialChest,
               ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI),
    SCENE_EXITS(AltmillerCaveVialChest),
    SCENE_EXIT(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI,
               AltmillerCaveBabi,
               ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST),
    SCENE_EXIT(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL,
               AltmillerCaveSquall,
               ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST),
    SCENE_EXITS(AltmillerCaveSquall),
    SCENE_EXIT(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST,
               AltmillerCaveVialChest,
               ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
    SCENE_EXIT(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST,
               AltmillerCaveCookieChest,
               ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
    SCENE_EXITS(AltmillerCaveCookieChest),
    SCENE_EXIT(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL,
               AltmillerCaveSquall,
               ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
    SCENE_EXIT(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT,
               AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
    SCENE_EXITS(AltmillerCaveDraught),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST,
               AltmillerCaveCookieChest,
               ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_TO_ROCKS, AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE,
               AltmillerCaveDraught, ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST, AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS),
    SCENE_EXIT(ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS, AltmillerCaveDraught,
               ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST),
    SCENE_EXITS_END,
};

struct ActorPlacement gAltmillerCave_ActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltmillerCave_BabiActorPlacements[] = {
    {ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACING_PARTY, 0},
    {ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACING_PARTY, 0},
    {ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACING_PARTY, 0},
    {SPRITE_INVISIBLE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(232),
     0, PIXELS(160), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -PIXELS(64), 0,
     -PIXELS(64), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     -PIXELS(64), 0, -PIXELS(64), FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     -PIXELS(64), 0, -PIXELS(64), FACING_NORTH, TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltmillerCave_SquallActorPlacements[] = {
    {SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
     MOVEMENT_TYPE_RESET_ACTION, PIXELS(328), 0, PIXELS(256), FACING_SOUTH,
     TALK_FACING_KEEP, 0},
    {SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
     MOVEMENT_TYPE_RESET_ACTION, PIXELS(352), 0, PIXELS(296), FACING_SOUTH,
     TALK_FACING_KEEP, 0},
    {SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
     MOVEMENT_TYPE_RESET_ACTION, PIXELS(368), 0, PIXELS(200), FACING_SOUTH,
     TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(376), 0,
     PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(472), 0,
     PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {JUPITER_DJINNI, FLAG_DJINNI(DJINNI_SQUALL), MOVEMENT_TYPE_STAND,
     PIXELS(264), 0, PIXELS(200), FACING_SOUTHWEST, TALK_FACING_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltmillerCave_DraughtActorPlacements[] = {
    {SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
     MOVEMENT_TYPE_RESET_ACTION, PIXELS(392), 0, PIXELS(416), FACING_SOUTH,
     TALK_FACING_KEEP, 0},
    {SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
     MOVEMENT_TYPE_RESET_ACTION, PIXELS(416), 0, PIXELS(456), FACING_SOUTH,
     TALK_FACING_KEEP, 0},
    {SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
     MOVEMENT_TYPE_RESET_ACTION, PIXELS(456), 0, PIXELS(480), FACING_SOUTH,
     TALK_FACING_KEEP, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(488),
     PIXELS(8), PIXELS(1752), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(472),
     PIXELS(40), PIXELS(1752), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(504),
     PIXELS(56), PIXELS(1752), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(536),
     PIXELS(40), PIXELS(1752), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(520),
     PIXELS(8), PIXELS(1752), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(440), 0,
     PIXELS(190), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(472), 0,
     PIXELS(174), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(504), 0,
     PIXELS(174), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(536), 0,
     PIXELS(174), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(568), 0,
     PIXELS(190), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SCENE_TABLE_END},
};

/// The sizes the light at the top pulses through.
u32 gAltmillerCave_PulseScales[] = {
    Q16_ONE,
    11 * Q16_ONE / 10,
    6 * Q16_ONE / 5,
    11 * Q16_ONE / 10,
};

/// The two men from Tolbi walk in to Babi.
s32 gAltmillerCave_GuardsArrivePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, -PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(-PIXELS(64), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -PIXELS(64)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Babi and the two men walk out of the room, each by his own path.
s32 gAltmillerCave_SecondGuardLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(48), 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gAltmillerCave_FirstGuardLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gAltmillerCave_BabiLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

// clang-format off
struct Trigger gAltmillerCave_TriggersOther[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI},
    {TRIGGER_STEP, 60, CONDITION_ALWAYS, EVENT(AltmillerCave_DarkenToLevel0)},
    {TRIGGER_STEP, 61, CONDITION_ALWAYS, EVENT(AltmillerCave_DarkenToLevel1)},
    {TRIGGER_STEP, 62, CONDITION_ALWAYS, EVENT(AltmillerCave_DarkenToLevel2)},
    {TRIGGER_STEP, 63, CONDITION_ALWAYS, EVENT(AltmillerCave_DarkenToLevel3)},
    {TRIGGER_STEP, 64, CONDITION_ALWAYS, EVENT(AltmillerCave_DarkenToLevel4)},
    {TRIGGER_STEP, 65, CONDITION_ALWAYS, EVENT(AltmillerCave_DarkenToLevel5)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        EVENT(AltmillerCave_StartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(AltmillerCave_EndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(AltmillerCave_StartRetreat)},
    {TRIGGERS_END},
};

struct Trigger gAltmillerCave_BabiTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {TRIGGER_TALK, ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI, EVENT(AltmillerCave_MeetInvisibleBabi)},
    {TRIGGER_TALK, ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, EVENT(AltmillerCave_RepeatRockOrder)},
    {TRIGGER_TALK, ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED, EVENT(AltmillerCave_RestoreBabiAndInviteParty)},
    {TRIGGER_STEP, 19, FLAG_BABI_CALLED_PARTY_BACK, EVENT(AltmillerCave_BabiCallsPartyBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI,
        EVENT(AltmillerCave_MeetInvisibleBabi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED,
        EVENT(AltmillerCave_RestoreBabiAndInviteParty)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(AltmillerCave_EndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(AltmillerCave_StartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTMILLER_CAVE_MIMIC_BEATEN, FIND_MIMIC | 5},
    {TRIGGERS_END},
};

struct Trigger gAltmillerCave_VialChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 20, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 21, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(AltmillerCave_EndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(AltmillerCave_StartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTMILLER_CAVE_VIAL, FIND_ITEM(ITEM_VIAL)},
    {TRIGGERS_END},
};

struct Trigger gAltmillerCave_SquallTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 30, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 31, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 72, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 73, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 74, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 75, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 71, CONDITION_ALWAYS, EVENT(AltmillerCave_MoveStagedActorToClearPosition)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 77, CONDITION_ALWAYS, EVENT(AltmillerCave_AdvanceObstacleAndCheckPassage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SQUALL_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(AltmillerCave_UpdateMovedObstacle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_CheckFrostTargetCollision)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_FreezePuddle)},
    {TRIGGER_TALK, ACTOR_SQUALL, CONDITION_ALWAYS, EVENT(AltmillerCave_StartDjinnBattle)},
    {TRIGGERS_END},
};

struct Trigger gAltmillerCave_CookieChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 40, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 41, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(AltmillerCave_EndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(AltmillerCave_StartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTMILLER_CAVE_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gAltmillerCave_DraughtTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 50, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {TRIGGER_EXIT, 51, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_TO_ROCKS},
    {TRIGGER_EXIT, 52, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE},
    {TRIGGER_EXIT, 53, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST},
    {TRIGGER_EXIT, 54, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 70, CONDITION_ALWAYS,
     EVENT(AltmillerCave_MoveStagedActorToClearPosition)},
    {TRIGGER_TALK, ACTOR_BLUE_ROCK, CONDITION_ALWAYS,
     EVENT(AltmillerCave_TurnRock)},
    {TRIGGER_TALK, ACTOR_GREEN_ROCK, CONDITION_ALWAYS,
     EVENT(AltmillerCave_TurnRock)},
    {TRIGGER_TALK, ACTOR_WHITE_ROCK, CONDITION_ALWAYS,
     EVENT(AltmillerCave_TurnRock)},
    {TRIGGER_TALK, ACTOR_YELLOW_ROCK, CONDITION_ALWAYS,
     EVENT(AltmillerCave_TurnRock)},
    {TRIGGER_TALK, ACTOR_RED_ROCK, CONDITION_ALWAYS,
     EVENT(AltmillerCave_TurnRock)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT,
     FIND_ITEM(ITEM_MYSTIC_DRAUGHT)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
     CONDITION_ALWAYS, EVENT(AltmillerCave_StartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     CONDITION_ALWAYS, EVENT(AltmillerCave_EndReveal)},
    {TRIGGERS_END},
};

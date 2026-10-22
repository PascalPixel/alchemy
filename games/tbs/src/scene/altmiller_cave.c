/// @file
/// Altmiller Cave, where the party finds Babi while Tolbi searches for him
/// during Colosso (babi_palace.c). Its six scenes share this overlay and the
/// area name Altmiller Cave:
///
/// - AltmillerCaveEntrance (SceneId_AltmillerCaveEntrance): the dark passage
///   in from the world map and the chamber beyond it.
/// - AltmillerCaveBabi (SceneId_AltmillerCaveBabi): the invisible Babi, who
///   asks for his draught (AltmillerCave_MeetInvisibleBabi) and drinks it
///   (AltmillerCave_RestoreBabiAndInviteParty), and a chest that is a Mimic.
/// - AltmillerCaveVialChest (SceneId_AltmillerCaveVialChest) and
///   AltmillerCaveCookieChest (SceneId_AltmillerCaveCookieChest): a chest
///   each.
/// - AltmillerCaveSquall (SceneId_AltmillerCaveSquall): Squall, a Jupiter
///   Djinni, three fallen columns to push, a stone pillar for Move and a
///   puddle for Frost.
/// - AltmillerCaveDraught (SceneId_AltmillerCaveDraught): three more fallen
///   columns, the rock puzzle (AltmillerCave_TurnRock) and the chest that
///   holds the Mystic Draught behind the wall the puzzle opens.
///
/// The rooms share one entrance table and one exit table, and the other
/// getters choose by gGameState.scene. AltmillerCave_Initialize sends an entry
/// from outside the cave (entrance 0) to the room's first entrance, darkens the
/// dark rooms and puts back what the party has done. Saving Babi
/// (FLAG_BABI_SAVED) is the story flag the rest of Tolbi waits on; the
/// overlay's own state is in scene flags and gSceneState, so it starts over on
/// every ordinary entrance.
///
/// Every room but the two lit ones is dark (FLAG_DARK): BG1 is blended over
/// the rest (ALTMILLER_CAVE_DARKNESS_BLEND), and the light around the party is
/// a display window, an ellipse around the leader inside which the layers are
/// drawn differently (AltmillerCave_InitializeDarkness). In an evenly dark
/// room BG1 covers everything outside it and is only blended inside it.
/// AltmillerCaveEntrance darkens by steps instead
/// (FLAG_DARKNESS_BY_STEPS): touch cells 60 to 65 each set a level, kept in
/// gSceneState byte 4, from the lightest at the mouth to the darkest by the
/// inner chamber, and blends outside the light while BG1 is left out inside
/// it. AltmillerCave_ApplyFadeBlend gives BG1 no weight and the layers under
/// it 16 sixteenths at level 0, two fewer at each level after, down to 6 at
/// level 5. Where one level meets the next, a cell of each lies side by
/// side, so the level is that of the cell the leader last stepped on, in
/// either direction
/// (https://alchemy.pascalpixel.com/places/altmiller-cave-entrance/darkness-levels.png:
/// the passage's map with each cell labelled with the level it sets, over the
/// same moment drawn at each of the six levels). Reveal lights a dark room
/// while it lasts and shows the rocks' colours (AltmillerCave_StartReveal,
/// AltmillerCave_EndReveal).
///
/// The rock puzzle keeps the rounds matched in gSceneState byte 0 and its
/// state and the light at the top in struct RockPuzzle after it. Turning the
/// leftmost rock and then the rightmost brings out the ring of five lights; the
/// ring spins (AltmillerCave_SpinLights, a scheduler task) and stops with one
/// light at the top, and turning that light's rock spins it on to a different
/// light. A wrong rock puts the lights away. The third match bursts the wall
/// (AltmillerCave_ReleaseWallBurst). Coming up to the rocks from the pillar
/// cave (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) starts the puzzle over.

#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/altmiller_cave.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/shop/shop.h"
#include "unit/runtime/runtime.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "edition.h"
#include "object_dispatch.h"
#include "scene_ids.h"
#include "dma.h"
#include "io_reg.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "call.h"
#include "display_transition.h"
#include "effect_slot.h"
#include "item_ids.h"
#include "objcmd.h"
#include "frame.h"
#include "fixed_math.h"
#include "field_runtime.h"
#include "fx_scene.h"
#include "flags.h"
#include "songs.h"

/// AltmillerCaveBabi's actors: Babi while invisible and once seen, and the two
/// men from Tolbi who come looking for him.
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

/// The scene flags the cave keeps.
enum {
  /// The cave is dark: set on entry everywhere but AltmillerCaveSquall and
  /// AltmillerCaveDraught.
  /// ★ Named from what the player sees: the cave dark but for a small circle of
  /// light around the party (its render)
  FLAG_DARK = FLAG_SCENE_TEMP_00,
  /// AltmillerCaveEntrance darkens by steps as the party walks in.
  /// ★ Named from its area name: Altmiller Cave
  FLAG_DARKNESS_BY_STEPS = FLAG_SCENE_TEMP_01,
  /// Frost has frozen AltmillerCaveSquall's puddle.
  /// ★ Named from what the player sees: a small puddle of water in the middle
  /// of the room (its render)
  FLAG_PUDDLE_FROZEN = FLAG_SCENE_TEMP_11,
  /// AltmillerCaveSquall's pillar stands where Move opens the passage.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  FLAG_PILLAR_IN_PLACE = FLAG_SCENE_TEMP_12
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
enum { PUDDLE_ANIM_FROZEN = 5 };

static void AltmillerCave_ReleaseWallBurst(void);

/// The ring of five lights above AltmillerCaveDraught's rocks as it spins to
/// its next light: angles are 16-bit turns, the light at the top is the chosen
/// one.
struct LightWheel {
  s16 phase;
  s16 ticks;
  s16 unk04; ///< never read or written
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

/// AltmillerCaveDraught's rock puzzle, kept in gSceneState: the rounds of
/// lights matched in byte 0, then the state and the light shown.
struct RockPuzzle {
  u8 state;
  s8 chosen;
};

#define ROCK_PUZZLE ((struct RockPuzzle *)&gSceneState[1])

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

#include "common/object_staged_actor.h"

/// Talking to the Djinni in AltmillerCaveSquall starts the battle with Squall,
/// a Jupiter Djinni.
static void AltmillerCave_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_SQUALL, DJINNI_SQUALL);
}

/// The six rooms share one entrance table.
const struct SceneEntrance *AltmillerCave_GetEntrances(void) {
  return gAltmillerCave_Entrances;
}

const struct SceneRegion *AltmillerCave_GetRegions(void) { return NULL; }

const u32 *AltmillerCave_GetExits(void) { return gAltmillerCave_Exits; }

const struct ScenePlacement *AltmillerCave_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_AltmillerCaveBabi) {
    return gAltmillerCave_BabiPlacements;
  }
  if (scene == (s32)&SceneId_AltmillerCaveSquall) {
    return gAltmillerCave_SquallPlacements;
  }
  if (scene == (s32)&SceneId_AltmillerCaveDraught) {
    return gAltmillerCave_DraughtPlacements;
  }
  return gAltmillerCave_PlacementsOther;
}

/// Darkens AltmillerCaveEntrance to its current level: the brighter blend at
/// level 0, each level one step darker.
static void AltmillerCave_ApplyFadeBlend(void) {
  s8 level = ((s8 *)gSceneState)[4];

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
  u8 *fade = &gSceneState[4];

  work = (struct FieldWork *)gEventWork;
  *fade = level;
  if (work->revealActive == 0) {
    AltmillerCave_ApplyFadeBlend();
  }
}

/// Crossing the first darkness boundary sets the cave's lightest level.
/// AltmillerCaveEntrance's boundaries are touch cells 60 to 65:
/// crossing one sets its level, 0 to 5, each a step darker than the last.
static void AltmillerCave_DarkenToLevel0(void) {
  AltmillerCave_SetFadeLevel(0);
}

/// Crossing the second darkness boundary (touch cell 61) sets darkness level 1.
static void AltmillerCave_DarkenToLevel1(void) {
  AltmillerCave_SetFadeLevel(1);
}

/// Crossing the third darkness boundary (touch cell 62) sets darkness level 2.
static void AltmillerCave_DarkenToLevel2(void) {
  AltmillerCave_SetFadeLevel(2);
}

/// Crossing the fourth darkness boundary (touch cell 63) sets darkness level 3.
static void AltmillerCave_DarkenToLevel3(void) {
  AltmillerCave_SetFadeLevel(3);
}

/// Crossing the fifth darkness boundary (touch cell 64) sets darkness level 4.
static void AltmillerCave_DarkenToLevel4(void) {
  AltmillerCave_SetFadeLevel(4);
}

/// Crossing the sixth darkness boundary (touch cell 65) sets darkness level 5.
static void AltmillerCave_DarkenToLevel5(void) {
  AltmillerCave_SetFadeLevel(5);
}

/// Darkens the cave on entry: AltmillerCaveEntrance by its levels, the other
/// dark rooms evenly.
static void AltmillerCave_InitializeDarkness(void) {
  u8 **workCells = (u8 **)&gEventWork;
  struct DisplayTransitionState *transition;

  {
    struct EventWork *work = (struct EventWork *)*workCells;
    work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transitionFrames = 24;
  }
  Engine_TaskWait(1);
  DisplayTransition_InitializeBattleEffectState(WINDOW_TRANSITION_LEADER_LOW);
  // UB: the fourth following pointer cell lies past scalar gEventWork.
  // Its named load adds eight bytes in the native EN trial (0x98 to 0xa0).
#ifdef UBFIX
  transition = gDisplayTransition;
#else
  transition = (struct DisplayTransitionState *)workCells[4];
#endif
  {
    u16 *slot = &transition->value;
    s32 value = ALTMILLER_CAVE_LIGHT_STEPS;
    *slot = value;
  }
  if (Engine_GameFlagIsSet(FLAG_DARKNESS_BY_STEPS) != 0) {
    {
      s16 *slot = &transition->mask;
      s32 value = ALTMILLER_CAVE_STEPS_LIGHT_WININ;
      *slot = value;
    }
    {
      s16 *slot = &transition->active;
      s32 value = ALTMILLER_CAVE_STEPS_LIGHT_WINOUT;
      *slot = value;
    }
    AltmillerCave_ApplyFadeBlend();
    return;
  }
  {
    s16 *slot = &transition->mask;
    s32 value = ALTMILLER_CAVE_DARK_LIGHT_WININ;
    *slot = value;
  }
  {
    s16 *slot = &transition->active;
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
  if (gGameState.scene == (s32)&SceneId_AltmillerCaveEntrance) {
    s32 alpha = BLEND_ALPHA(0, 16);

    REG_BLDALPHA = alpha;
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCaveDraught) {
    Engine_ActorSetChildValue(ACTOR_BLUE_ROCK, COLOR_BLUE);
    Engine_ActorSetChildValue(ACTOR_GREEN_ROCK, COLOR_GREEN);
    Engine_ActorSetChildValue(ACTOR_WHITE_ROCK, COLOR_WHITE);
    Engine_ActorSetChildValue(ACTOR_YELLOW_ROCK, COLOR_YELLOW);
    Engine_ActorSetChildValue(ACTOR_RED_ROCK, COLOR_RED);
  }
}

/// Reveal's light ends: the cave darkens again and AltmillerCaveDraught's rocks
/// lose their colors.
static void AltmillerCave_EndReveal(void) {
  struct FieldFxScene *work = (struct FieldFxScene *)gEffectWork;

  if (Engine_GameFlagIsSet(FLAG_DARK) != 0) {
    AltmillerCave_InitializeDarkness();
    work->childOption = 1;
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCaveDraught) {
    Engine_ActorSetChildValue(ACTOR_BLUE_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_GREEN_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_WHITE_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_YELLOW_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_RED_ROCK, COLOR_NONE);
  }
}

/// Retreat's first phase: the Psynergy's first actor faces south, and its
/// object is copied into a new one for the effect to use.
static void AltmillerCave_BeginRetreat(void) {
  struct FieldFxScene *work;
  struct FieldActor *original;
  struct FieldActor *replacement;

  work = (struct FieldFxScene *)gEffectWork;
  original = work->mainObject;
  Engine_ActorFaceDirection(work->firstObjectId, FACING_SOUTH, 0);
  Animation_ApplyChildValues((struct DispatchObject *)original, 0);
  Engine_EventWait(20);
  replacement = Engine_ObjectCreate(0, original->x.fixed, original->y.fixed,
                                    original->z.fixed);
  if (replacement != 0) {
    Dma_Set(original, replacement,
            (DMA_ENABLE | DMA_32BIT) << 16 | sizeof(struct FieldActor) / 4,
            REG_DMA3);
    original->update = 0;
    work->mainObject = replacement;
    original->spriteStorageKind = 0;
  }
}

/// After Move on AltmillerCaveSquall's pillar: in its cell at (30, 20) it
/// settles and opens the way beside it (remembered in TBS_PATCH_1 and later).
static void AltmillerCave_UpdateMovedObstacle(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_SQUALL_ROOM_PILLAR);

  if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
    actor->motionFlags = 2;
    actor->terrainHeight = 0;
    actor->priorityFlags = 2;
    Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
#if TBS_PATCH_1
    Engine_GameFlagSet(FLAG_PILLAR_IN_PLACE);
#endif
  }
}

/// Walking east into AltmillerCaveSquall's pillar pushes it a cell.
static void AltmillerCave_AdvanceObstacleAndCheckPassage(void) {
  StagedActor_AdvancePair();
  AltmillerCave_UpdateMovedObstacle();
}

/// Frost's first phase on AltmillerCaveSquall's puddle: notes whether its place
/// is blocked, which keeps it from freezing.
static void AltmillerCave_CheckFrostTargetCollision(void) {
  struct FieldFxScene *effect = (struct FieldFxScene *)gEffectWork;
  struct FieldActor *target = Object_GetById(ACTOR_SQUALL_ROOM_PUDDLE);
  struct FieldPosition position;

  position.x = target->x.fixed;
  position.y = target->y.fixed;
  position.z = target->z.fixed;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    effect->childMode = 1;
  }
}

/// Frost on AltmillerCaveSquall's puddle, unless blocked: it freezes and its
/// cell takes the frozen attributes kept at (76, 16).
static void AltmillerCave_FreezePuddle(void) {
  struct FieldFxScene *effect = (struct FieldFxScene *)gEffectWork;
  struct FieldActor *puddle = Object_GetById(ACTOR_SQUALL_ROOM_PUDDLE);
  s32 blocked;

  blocked = effect->childMode;
  if (blocked == 0) {
    Map_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    if (puddle != 0) {
      s32 motionFlags = 2;
      puddle->motionFlags = motionFlags;
      puddle->priorityFlags = blocked;
    }
    Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
  }
}

/// Walking into AltmillerCaveSquall's puddle while it is not frozen: the leader
/// hops 32 pixels over it, if the landing is clear.
static void AltmillerCave_HopSelectedActor(void) {
  struct FieldActor *actor = Object_GetById(gGameState.selectedActor);
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
  if (FieldObject_CheckMovementCollision(actor, &destination.position) == 0) {
    Engine_EventBegin();
    Object_SetMode(actor, ANIM_LEADER_SPRING);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(actor, ANIM_LEADER_LEAP);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 126;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectMotionSetPositionAndCommit(
        ACTOR_PARTY_LEADER, components[0].part.pixel, components[2].part.pixel);
    Object_SetMode(actor, ANIM_LEADER_SPRING);
    Engine_ActorSetSpriteFlags(actor, 1);
    actor->motionFlags = motionFlags;
    Engine_EventEnd();
  }
}

/// Walking into one of the three obstacles pushes it, if there is room.
static void AltmillerCave_MoveStagedActorToClearPosition(void) {
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
  }
  Engine_EventEnd();
}

/// Update callback of the light at the top of the stopped ring: it pulses
/// through four sizes.
static void AltmillerCave_PulseLight(union FieldObject *fieldObject) {
  struct FieldActor *light = (struct FieldActor *)fieldObject;
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
  struct FieldActor *light = Object_GetById(actorId);
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

/// Scheduler task that spins the ring of lights: it speeds up, coasts, slows
/// and stops with the chosen light at the top, which then pulses; a click
/// (SE_135) sounds for each fifth of a turn.
static void AltmillerCave_SpinLights(void) {
  struct LightWheel *wheel = gAltmillerCave_LightWheel;
  s32 moving = 1;
  s32 phase = wheel->phase;

  if (phase == WHEEL_SPEEDING_UP) {
    wheel->speed += 16;
    if (wheel->speed > 0xbff) {
      wheel->phase++;
      wheel->ticks = 0;
    }
  } else if (phase == WHEEL_COASTING) {
    if (wheel->ticks == 30) {
      wheel->phase++;
    }
  } else if (phase == WHEEL_SLOWING) {
    wheel->speed -= 8;
    if (wheel->speed <= 0x2ff) {
      wheel->phase++;
    }
  } else if (phase == WHEEL_STOPPING) {
    s32 chosenLight = ((s8 *)gSceneState)[2];
    s32 chosenAngle = Math_Divide(chosenLight * Q16_ONE, 5);
    if ((unsigned int)(((u32)(wheel->angle - chosenAngle) << 16) +
                       0xc2ff0000) <= 0x5fe0000) {
      wheel->angle = chosenAngle + 0x4000;
      wheel->phase = WHEEL_STOPPED;
      wheel->speed = 0;
      Object_GetById(ACTOR_FIRST_LIGHT + chosenLight)->update =
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
      Engine_AudioPlayCue(SE_135);
    }
  }
  gAltmillerCave_LightWheel->ticks++;
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
  struct FieldActor *actor;
  s32 scale;
  s8 state;
  s32 chosenLight;
  u8 previousLight;
  s32 radius;
  u16 angle;
  u8 *puzzleState;

  Event_Begin();
  Field_EndBusyAndReveal();
  Event_SetMessage(MsgAltmillerRotatedRock);
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
      Event_PlayCue(SE_110);
    } else {
      Event_PlayCue(SE_MENU_ERROR);
    }
    gSceneState[0] = 0;
  } else if (state == PUZZLE_LEFT_ROCK_TURNED) {
    if (rock == ACTOR_BLUE_ROCK) {
      Event_PlayCue(SE_110);
    } else if (rock == ACTOR_RED_ROCK) {
      // The lights grow in one by one, each with SE_151.
      state = PUZZLE_LIGHTS_SHOWN;
      Event_PlayCue(SE_110);
      Task_Wait(30);
      angle = gAltmillerCave_LightWheel->angle;
      for (i = 0; i < LIGHT_COUNT; i++) {
        AltmillerCave_PlaceLight(ACTOR_FIRST_LIGHT + i, PIXELS(24), angle);
        Event_PlayCue(SE_151);
        actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
        actor->scaleX = 0;
        scale = 2 * Q16_ONE / 5;
        do {
          actor->scaleY = scale;
          actor->scaleX = scale;
          Task_Wait(1);
          scale += 3 * Q16_ONE / 64;
        } while (actor->scaleX < Q16_ONE);
        angle -= 0x3333;
      }
      Task_Wait(30);
      correctRock = 1;
    } else {
      Event_PlayCue(SE_MENU_ERROR);
      state = PUZZLE_START;
    }
  } else if (state == PUZZLE_LIGHTS_SHOWN) {
    if (rock != ACTOR_FIRST_ROCK + chosenLight) {
      // The wrong rock: the lights shrink away.
      state = PUZZLE_START;
      Event_PlayCue(SE_MENU_ERROR);
      Task_Wait(30);
      for (i = 0; i < LIGHT_COUNT; i++) {
        actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
        Event_PlayCue(SE_151);
        for (scale = actor->scaleX; actor->scaleX > 2 * Q16_ONE / 5;
             scale -= 3 * Q16_ONE / 64) {
          actor->scaleY = scale;
          actor->scaleX = scale;
          Task_Wait(1);
        }
        Actor_SetPosition(ACTOR_FIRST_LIGHT + i, 0, 0);
      }
    } else {
      Event_PlayCue(SE_110);
      correctRock = 1;
      Task_Wait(30);
    }
  }
  puzzleState = &gSceneState[1];
  puzzleState[0] = state;
  // A light matched: choose a different light and spin the ring to it.
  if (correctRock) {
    u8 rounds = ++puzzleState[-1];

    puzzleState[1] =
        ((s8)(((u32)(Random_Next() << 2) >> 16) + previousLight + 1) + 5) % 5;
    gAltmillerCave_LightWheel->phase = WHEEL_SPEEDING_UP;
    gAltmillerCave_LightWheel->ticks = 0;
    gAltmillerCave_LightWheel->speed = 0x200;
    gAltmillerCave_LightWheel->travel = 0x3000;
    Task_AddCallback(AltmillerCave_SpinLights, TASK_PRIORITY_SCENE);
    if (rounds <= 2) {
      while (gAltmillerCave_LightWheel->phase != WHEEL_STOPPED) {
        Task_Wait(1);
      }
      Task_Wait(10);
      Event_PlayCue(SE_110);
    } else {
      // The third light matched: the ground shakes, the ring spirals in to
      // nothing, and the wall bursts.
      puzzleState[0] = PUZZLE_SOLVED;
      while (gAltmillerCave_LightWheel->phase != WHEEL_SLOWING) {
        Task_Wait(1);
      }
      gAltmillerCave_LightWheel->phase = WHEEL_SLOWING;
      gAltmillerCave_LightWheel->ticks = 0;
      Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
      Engine_EventWait(20);
      Map_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      gAltmillerCave_LightWheel->phase = WHEEL_STOPPED;
      Event_PlayCue(SE_190);
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
        Task_Wait(1);
      } while (radius > 0);
      for (i = 0; i < LIGHT_COUNT; i++) {
        Actor_SetPosition(ACTOR_FIRST_LIGHT + i, 0, 0);
      }
      AltmillerCave_ReleaseWallBurst();
      Event_PlayCue(MUS_PUZZLE_SOLVED);
    }
    Engine_TaskRemoveCallback(AltmillerCave_SpinLights);
  }
}

/// Divides the nonnegative random value for wall-fragment scaling.
s32 Math_DivideUnsigned(s32 value, s32 divisor);

/// The party turns north, toward Babi.
static __inline__ void AltmillerCave_FacePartyNorth(void) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
}

/// In AltmillerCaveBabi, talking to Babi or reading his mind once the Mystic
/// Draught is found: he drinks it and appears, startling the party. Ivan reads
/// his mind and finds him to be Babi, the ruler of Tolbi, who sought Lemurians
/// to escape growing old. Two soldiers from Tolbi find him and take him back,
/// and he invites the party to Colosso's finals; Garet asks whether they
/// should take part.
static void AltmillerCave_RestoreBabiAndInviteParty(void) {
  struct DisplayTransitionState *display;
  struct FieldActor *actor;
  struct FieldActor *leader;
  s32 i;
  s32 eva;
  s32 evb;

  // Give Babi the draught and bring the party beside him.
  GameFlag_Set(FLAG_BABI_SAVED);
  Engine_PartyRemoveItem(ITEM_MYSTIC_DRAUGHT);
  Engine_EventBegin();
  Field_EndBusyAndReveal();
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgAltmillerCameBackPleaseDraughtQuickly);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Camera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(20);
  actor = Actor_Get(ACTOR_INVISIBLE_BABI);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(60);
  Event_PlayCue(MUS_017);
  actor->sprite->rotation = 0;
  Engine_ActorJump(ACTOR_INVISIBLE_BABI, 10, 70);
  // Flicker between sprite priorities before replacing invisible Babi.
  for (i = 0; i < 30; i++) {
    actor->sprite->image.priority = 0;
    Engine_EventWait(2);
    actor->sprite->image.priority = 2;
    Engine_EventWait(2);
  }
  Engine_EventWait(40);
  {
    s32 x;
    s32 z;

    x = Actor_Get(ACTOR_INVISIBLE_BABI)->x.part.pixel;
    z = Actor_Get(ACTOR_INVISIBLE_BABI)->z.part.pixel;
    Actor_SetPosition(ACTOR_INVISIBLE_BABI, 0, 0);
    Actor_SetPosition(ACTOR_BABI, PIXELS(x), PIXELS(z));
  }
  actor->sprite->flags = 0;
  Event_PlayCue(MUS_030);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkByAndWait(ACTOR_BABI, 32, 32);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(MIA)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_Get(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_PARTY_LEADER, 0, 16);
  Actor_WalkBy(GARET, 0, 16);
  Actor_WalkBy(MIA, 0, 16);
  Actor_WalkByAndWait(IVAN, 0, 16);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(MIA)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_Get(IVAN)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_TaskWait(1);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ActorJump(GARET, 4, 13);
  Engine_ActorJump(GARET, 4, 30);
  Event_ShowMessage(GARET, 0);
  Actor_FaceActor(IVAN, GARET, 30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_OpenMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerWouldHaveRevealedMyselfSooner);
    Event_ShowMessage(ACTOR_BABI, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerMustTraveledIndeedIfDidnt);
    Event_ShowMessage(ACTOR_BABI, 0);
  }
  // Ask how Babi became invisible and hear about his Alchemy.
  Engine_EventSetMessage(MsgAltmillerHowDidBecomeParalyzedWhile);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(IVAN, 0, -48);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(40);
  // Ivan reads Babi's mind: he is Babi, the ruler of Tolbi, and hid by the
  // power of an ancient civilization.
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(IVAN, ACTOR_BABI);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_EventWait(150);
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(40);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_BABI, 4, 13);
  Engine_ActorJump(ACTOR_BABI, 4, 30);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(50);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(10);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(20);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(IVAN, 0, 48);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Engine_EventWait(50);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerThatsKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerForgotKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  }
  Engine_EventSetMessage(MsgAltmillerWhatWasThat);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_OpenMessage(ACTOR_BABI, 0);
  Engine_EventSetMessage(MsgAltmillerBabi);
  Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0);
  // The cave brightens as two men from Tolbi arrive: "Babi!" The light
  // widens to fill the screen and BG1 fades out.
  display = gDisplayTransition;
  {
    u16 *slot = &display->value;
    s32 value = ALTMILLER_CAVE_LIGHT_FULL;

    *slot = value;
  }
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = ALTMILLER_CAVE_DARKNESS_BLEND;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = BLEND_ALPHA(evb, eva);
    Engine_TaskWait(7);
  }
  {
    u16 *slot = (u16 *)&display->active;
    s32 value = ALTMILLER_CAVE_BRIGHTENING_WINOUT;

    *slot = value;
  }
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, PIXELS(280), PIXELS(304));
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, PIXELS(296), PIXELS(304));
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Engine_EventWait(10);
  Engine_CameraFollowActor(ACTOR_FIRST_SOLDIER, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(70);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, ACTOR_FIRST_SOLDIER);
  Engine_ActorFollow(GARET, ACTOR_FIRST_SOLDIER);
  Engine_ActorFollow(MIA, ACTOR_FIRST_SOLDIER);
  Engine_ActorFollow(IVAN, ACTOR_FIRST_SOLDIER);
  // The two men greet Babi; he invites the party to the finals.
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, PIXELS(7) / 5, PIXELS(7) / 10);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, PIXELS(7) / 5, PIXELS(7) / 10);
  Actor_SetActionScript(ACTOR_FIRST_SOLDIER, gAltmillerCave_GuardsArrivePath);
  Engine_EventWait(3);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, -16, 0);
  Actor_SetActionScript(ACTOR_SECOND_SOLDIER, gAltmillerCave_GuardsArrivePath);
  Object_RefreshSelectorById(ACTOR_FIRST_SOLDIER);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 0, -16);
  Object_RefreshSelectorById(ACTOR_SECOND_SOLDIER);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(MIA);
  Engine_ActorStop(IVAN);
  AltmillerCave_FacePartyNorth();
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(216), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, 16, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_SECOND_SOLDIER, 4, 13);
  Engine_ActorJump(ACTOR_SECOND_SOLDIER, 4, 30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
  Engine_EventWait(70);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 70);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, ACTOR_BABI);
  Engine_ActorFollow(GARET, ACTOR_BABI);
  Engine_ActorFollow(MIA, ACTOR_BABI);
  Engine_ActorFollow(IVAN, ACTOR_BABI);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 48);
  Actor_WalkBy(ACTOR_SECOND_SOLDIER, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 32);
  Engine_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_SECOND_SOLDIER, ANIM_STAND);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(MIA);
  Engine_ActorStop(IVAN);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 30);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Actor_FaceActor(GARET, ACTOR_BABI, 30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(70);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  // Babi and the two men walk back toward the cave entrance.
  Actor_SetActionScript(ACTOR_SECOND_SOLDIER,
                        gAltmillerCave_SecondGuardLeavePath);
  Actor_SetActionScript(ACTOR_FIRST_SOLDIER,
                        gAltmillerCave_FirstGuardLeavePath);
  Actor_SetActionScript(ACTOR_BABI, gAltmillerCave_BabiLeavePath);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Object_RefreshSelectorById(ACTOR_BABI);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(248), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_SOLDIER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 60);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Engine_EventWait(40);
  Actor_WalkBy(ACTOR_BABI, 32, 0);
  Actor_WalkBy(ACTOR_SECOND_SOLDIER, 0, 64);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 16, 0);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 64);
  Engine_ActorWaitForMove(ACTOR_BABI);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 64);
  Actor_SetPosition(ACTOR_BABI, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, 0, 0);
  Engine_EventWait(20);
  // With Babi gone the darkness returns: BG1 fades back in, then the light
  // shrinks around the party again.
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = ALTMILLER_CAVE_DARKNESS_BLEND;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = BLEND_ALPHA(eva, evb);
    Engine_TaskWait(7);
  }
  {
    u16 *slot = &display->value;
    s32 value = ALTMILLER_CAVE_LIGHT_STEPS;

    *slot = value;
  }
  {
    u16 *slot = (u16 *)&display->active;
    s32 value = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;

    *slot = value;
  }
  Engine_TaskWait(1);
  Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
  Io_Write16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
  Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_OpenMessage(GARET, 0);
  Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgAltmillerSoWellDoIt);
    Event_ShowMessage(GARET, 0);
  } else {
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgAltmillerDontWantEitherDo);
    Event_ShowMessage(GARET, 0);
  }
  // Let Isaac answer the invitation and reunite the party.
  Engine_EventSetMessage(MsgAltmillerEitherWayWereStuckHere);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessage(GARET, 0);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerGreatItsDecidedJustHave);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(MIA, IVAN, 50);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(IVAN, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimation(MIA, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(30);
  } else {
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
    Engine_EventWait(30);
    Engine_EventSetMessage(MsgAltmillerDontLetMeDownLike);
    Event_ShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(MIA, IVAN, 50);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Event_ShowMessage(IVAN, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(30);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  }
  Event_PlayCue(MUS_017);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

/// In AltmillerCaveBabi, talking to the invisible Babi or reading his mind
/// the first time: he can be heard but not seen, cannot move, and asks the
/// party to bring his draught from deep in the cave, telling the order of the
/// five rocks.
static void AltmillerCave_MeetInvisibleBabi(void) {
  struct FieldActor *leader;
  s32 line;

  // Gather the party around the invisible voice.
  GameFlag_Set(FLAG_MET_INVISIBLE_BABI);
  Engine_AudioPlayCue(MUS_CAVE);
  Engine_EventBegin();
  Field_EndBusyAndReveal();
  Engine_EventSetMessage(MsgAltmillerSee);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Camera_MoveTo(PIXELS(248), -1, PIXELS(184), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_EventWait(40);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, IVAN, 50);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_SetAttachedEffect(ACTOR_INVISIBLE_BABI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(20);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_OpenMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerKiddingHaventActually);
    Engine_EventWait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_SetSpeed(GARET, PIXELS(2), PIXELS(1));
    Actor_WalkByAndWait(GARET, 0, -16);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 30);
    Engine_EventShowMessage(GARET, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerDontTrustAnyone);
    Engine_EventWait(10);
    Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkByAndWait(GARET, 0, -16);
    Engine_EventWait(10);
    Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 30);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(20);
    Engine_EventShowMessage(GARET, 0);
  }

  // Question Babi's condition before agreeing to find his draught.
  line = MsgAltmillerSaidCouldntMove;
  Engine_EventSetMessage(line);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_SetSpeed(IVAN, PIXELS(2), PIXELS(1));
  Actor_WalkByAndWait(IVAN, 0, -16);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 30);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, MIA, 0);
  Actor_FaceActor(GARET, MIA, 0);
  Engine_ActorFaceActor(IVAN, MIA, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkByAndWait(GARET, 0, 16);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(30);
  Engine_ActorFaceEachOther(MIA, IVAN, 30);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkByAndWait(IVAN, 0, 16);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  // Explain the hidden draught and the order of the five rocks.
  line += 7;
  Engine_EventSetMessage(line);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(MIA, IVAN, 50);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_FaceActor(ACTOR_PARTY_LEADER, MIA, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Actor_FaceActor(GARET, MIA, 0);
  Engine_ActorFaceActor(IVAN, MIA, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_AudioPlayCue(MUS_017);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

/// In AltmillerCaveBabi, talking to Babi again before the draught is found:
/// he asks whether the party forgot the order of the rock colors, and tells it
/// again.
static void AltmillerCave_RepeatRockOrder(void) {
  s32 question;

  Engine_EventBegin();
  question = MsgAltmillerForgetOrderRock;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(ACTOR_INVISIBLE_BABI, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(20);
    Engine_EventSetMessage(question + 1);
    Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  } else {
    Engine_EventWait(20);
    Engine_EventSetMessage(question + 2);
    Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  }
  Engine_EventEnd();
}

/// In AltmillerCaveBabi, walking away from Babi (touch cell 19) the first
/// time after meeting him: he calls the party back and says where the draught
/// is. "Wait... The draught is... on a table behind the door."
static void AltmillerCave_BabiCallsPartyBack(void) {
  if (Engine_GameFlagIsSet(FLAG_MET_INVISIBLE_BABI) == 0)
    return;
  if (Engine_GameFlagIsSet(FLAG_BABI_SAVED) != 0)
    return;

  Engine_GameFlagSet(FLAG_BABI_CALLED_PARTY_BACK);
  Engine_EventBegin();
  Engine_EventSetMessage(MsgAltmillerWait);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(30);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventEnd();
}

const struct SceneEvent *AltmillerCave_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_AltmillerCaveBabi) {
    return gAltmillerCave_BabiEvents;
  }
  if (scene == (s32)&SceneId_AltmillerCaveVialChest) {
    return gAltmillerCave_VialChestEvents;
  }
  if (scene == (s32)&SceneId_AltmillerCaveSquall) {
    return gAltmillerCave_SquallEvents;
  }
  if (scene == (s32)&SceneId_AltmillerCaveCookieChest) {
    return gAltmillerCave_CookieChestEvents;
  }
  if (scene == (s32)&SceneId_AltmillerCaveDraught) {
    return gAltmillerCave_DraughtEvents;
  }
  return gAltmillerCave_EventsOther;
}

/// Update callback of AltmillerCaveDraught's obstacles: drawn on the leader's
/// sprite priority.
static void AltmillerCave_MatchLeaderPriority(union FieldObject *fieldObject) {
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  if (actor != NULL) {
    actor->priorityFlags = 0;
    actor->sprite->image.priority =
        Object_GetById(ACTOR_PARTY_LEADER)->sprite->image.priority;
  }
}

/// Redraws the three obstacles' cells where they stand.
static __inline__ void AltmillerCave_RedrawObstacleFootprints(void) {
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE);
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE + 1);
  FieldScene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE + 2);
}

/// Sets up the room being entered: an entry from outside the cave takes the
/// room's first entrance; AltmillerCaveEntrance picks its darkness from the
/// entrance; AltmillerCaveBabi's Babi is invisible until rescued;
/// AltmillerCaveSquall's obstacles and pillar and AltmillerCaveDraught's
/// obstacles, lights and rocks are restored. Returns 0.
s32 AltmillerCave_Initialize(void) {
  s8 *state;
  struct FieldActor *actor;
  s32 i;

  if (gGameState.entrance == 0) {
    if (gGameState.scene == (s32)&SceneId_AltmillerCaveBabi) {
      gGameState.entrance = ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCaveVialChest) {
      gGameState.entrance = ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCaveSquall) {
      gGameState.entrance =
          ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCaveCookieChest) {
      gGameState.entrance =
          ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCaveDraught) {
      gGameState.entrance =
          ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST;
    }
  }
  Engine_GameFlagSet(FLAG_DARK);
  GameFlag_Clear(FLAG_DARKNESS_BY_STEPS);
  if (gGameState.scene == (s32)&SceneId_AltmillerCaveEntrance) {
    if (gGameState.entrance == ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP) {
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gSceneState + 4) = 0;
      }
      Engine_GameFlagSet(FLAG_DARKNESS_BY_STEPS);
    }
    if (gGameState.entrance == ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER) {
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gSceneState + 4) = 5;
      }
      Engine_GameFlagSet(FLAG_DARKNESS_BY_STEPS);
    }
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCaveBabi) {
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
      Engine_ActorSetPosition(ACTOR_INVISIBLE_BABI, 0, 0);
    } else {
      actor = Object_GetById(ACTOR_INVISIBLE_BABI);
      actor->sprite->image.priority = 1;
      actor->sprite->flags = 2;
      actor->sprite->rotation = 0x4000;
    }
  }
  // Restore AltmillerCaveSquall's moved obstacles and frozen puddle.
  if (gGameState.scene == (s32)&SceneId_AltmillerCaveSquall) {
    Engine_GameFlagClear(FLAG_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    if (Engine_GameFlagIsSet(FLAG_PUDDLE_FROZEN)) {
      Engine_ActorSetAnimation(ACTOR_SQUALL_ROOM_PUDDLE, PUDDLE_ANIM_FROZEN);
      Map_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    } else {
      Object_GetById(ACTOR_SQUALL_ROOM_PUDDLE)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SQUALL_ROOM_PUDDLE), 0);
#if TBS_PATCH_1
    if (Engine_GameFlagIsSet(FLAG_PILLAR_IN_PLACE)) {
      Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
    }
#endif
  }
  // Rebuild the final rock puzzle from its saved phase and chosen light.
  if (gGameState.scene == (s32)&SceneId_AltmillerCaveDraught) {
    Engine_GameFlagClear(FLAG_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    // UB: the callbacks are stored through an s32 view of update, which keeps
    // the entrance read below the last store.
#ifdef UBFIX
    Object_GetById(ACTOR_FIRST_OBSTACLE)->update =
        AltmillerCave_MatchLeaderPriority;
    Object_GetById(ACTOR_FIRST_OBSTACLE + 1)->update =
        AltmillerCave_MatchLeaderPriority;
    Object_GetById(ACTOR_FIRST_OBSTACLE + 2)->update =
        AltmillerCave_MatchLeaderPriority;
#else
    *(s32 *)&Object_GetById(ACTOR_FIRST_OBSTACLE)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Object_GetById(ACTOR_FIRST_OBSTACLE + 1)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&Object_GetById(ACTOR_FIRST_OBSTACLE + 2)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
#endif
    // Coming up to the rocks (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE)
    // starts the puzzle over, with the red light first.
    if (gGameState.entrance == ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) {
      Dma_Fill32(0, gAltmillerCave_LightWheel, 12);
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        u8 *puzzleState = gSceneState;

        puzzleState[0] = 0;
        puzzleState[1] = PUZZLE_START;
        puzzleState[2] = ACTOR_RED_LIGHT - ACTOR_FIRST_LIGHT;
      }
    }
    state = (s8 *)&gSceneState[1];
    if (state[0] == PUZZLE_SOLVED) {
      Map_CopyCellsLayered(41, 55, 3, 2, 30, 55);
      Map_CopyCellAttributes(42, 8, 1, 1, 31, 8);
    }
    if (state[0] == PUZZLE_LIGHTS_SHOWN) {
      AltmillerCave_PlaceLights((state[1] * Q16_ONE) / 5 + 0x4000);
    }
    for (i = 0; i < LIGHT_COUNT; i++) {
      actor = Object_GetById(ACTOR_FIRST_LIGHT + i);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      actor->scaleX = Q16_ONE;
      actor->scaleY = Q16_ONE;
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_LIGHT + i), 0);
      Engine_ActorSetAnimation(ACTOR_FIRST_LIGHT + i, i + 1);
    }
    Engine_ActorSetChildValue(ACTOR_BLUE_LIGHT, COLOR_BLUE);
    Engine_ActorSetChildValue(ACTOR_GREEN_LIGHT, COLOR_GREEN);
    Engine_ActorSetChildValue(ACTOR_WHITE_LIGHT, COLOR_WHITE);
    Engine_ActorSetChildValue(ACTOR_YELLOW_LIGHT, COLOR_YELLOW);
    Engine_ActorSetChildValue(ACTOR_RED_LIGHT, COLOR_RED);
    Engine_ActorSetChildValue(ACTOR_BLUE_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_GREEN_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_WHITE_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_YELLOW_ROCK, COLOR_NONE);
    Engine_ActorSetChildValue(ACTOR_RED_ROCK, COLOR_NONE);
    Object_GetById(ACTOR_BLUE_ROCK)->sprite->image.priority = 3;
    Object_GetById(ACTOR_RED_ROCK)->sprite->image.priority = 3;
    Object_GetById(ACTOR_BLUE_ROCK)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(ACTOR_RED_ROCK)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BLUE_ROCK), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_RED_ROCK), 0);
  }
  if (Engine_GameFlagIsSet(FLAG_DARK)) {
    AltmillerCave_InitializeDarkness();
  } else {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gEventWork->transitionFrames = 24;
  }
  return 0;
}

/// Update callback of a piece of the burst wall: it flies off to a random
/// point 120 pixels away, with a crash every fourth frame as they start.
static void AltmillerCave_UpdateWallFragment(struct EffectSlot *fragment) {
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
    Vector_AddPolarOffset(PIXELS(120), Engine_RandomNext(), position);
    fragment->targetX = position[0];
    fragment->targetZ = position[2];
    fragment->acceleration = PIXELS(5);
    fragment->maxSpeed = PIXELS(5);
    fragment->limitTurnStep = currentPhase;
    (*phase)++;
    if ((gFrameTick & 3) == 0)
      Engine_AudioPlayCue(SE_HEAVY_IMPACT);
  } else if (currentPhase == 1) {
    if (EffectSlot_HasPendingTarget(fragment) == 0) {
      s32 nextPhase = *phase;
      nextPhase--;
      *phase = nextPhase;
    }
  } else if (currentPhase == 2) {
    if (EffectSlot_HasPendingTarget(fragment) == 0)
      FieldFx_ClearOwnedSlot(fragment);
  }
}

/// The wall bursts: 24 pieces fly out from the ring's center, the wall's
/// cells open to the passage, and the shaking calms.
static void AltmillerCave_ReleaseWallBurst(void) {
  struct FieldFxScene *work;
  struct EffectVector position;
  struct EffectSlot *fragment;
  s32 fragmentIndex;

  FieldFx_InitializeSlots();
  work = (struct FieldFxScene *)gEffectWork;
  Shop_SaveAndTintFieldPalette(0x202108);
  position.x = PIXELS(504);
  position.y = PIXELS(24);
  position.z = PIXELS(144);
  Camera_WorldToScreen(&position);
  fragment = work->slots;
  for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
    s32 scale;

    EffectSlot_Initialize(fragment, SPRITE_PARTICLE, position.x, position.z);
    EffectSlot_SetCallback(fragment, AltmillerCave_UpdateWallFragment);
    EffectSlot_SetObjectMode(fragment, 7);
    ObjectGroup_SetChildValueUnlessFifteen(
        fragment->object, (u32)(Engine_RandomNext() * 7) >> 16);
    scale = Math_DivideUnsigned(Engine_RandomNext(), 3) + 3 * Q16_ONE / 2;
    fragment->scaleY = scale;
    fragment->scaleX = scale;
    Engine_TaskWait(1);
    fragment++;
  }
  Engine_TaskWait(80);
  Map_CopyCellsLayered(41, 55, 3, 2, 30, 55);
  Map_CopyCellAttributes(42, 8, 1, 1, 31, 8);
  Engine_TaskWait(50);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_TaskWait(30);
  {
    s32 mode = 2;
    // Walk the state bytes within the complete slot array representation.
    u8 *stateCursor = (u8 *)&work->slots + offsetof(struct EffectSlot, state);

    for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
      if (((s8 *)stateCursor)[offsetof(struct EffectSlot, active) -
                              offsetof(struct EffectSlot, state)] != 0) {
        *stateCursor = mode;
      }
      stateCursor += sizeof(struct EffectSlot);
    }
  }
  Engine_MapWaitWorkValuesBelow256();
  Shop_RestoreFieldPalette();
  FieldFx_ClearActiveSlotsAndScheduleUpdates();
}

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

u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
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

struct ScenePlacement gAltmillerCave_PlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCave_BabiPlacements[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {SPRITE_INVISIBLE_BABI, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(232), 0,
     PIXELS(160), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_BABI, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0, -PIXELS(64),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0,
     -PIXELS(64), FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0,
     -PIXELS(64), FACING_NORTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCave_SquallPlacements[] = {
    {SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
     PIXELS(328), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
     PIXELS(352), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
     PIXELS(368), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(280),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0,
     PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {JUPITER_DJINNI, FLAG_DJINNI_0 + DJINNI_SQUALL, ACTOR_STAND, PIXELS(264), 0,
     PIXELS(200), FACING_SOUTHWEST, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCave_DraughtPlacements[] = {
    {SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
     PIXELS(392), 0, PIXELS(416), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
     PIXELS(416), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
     PIXELS(456), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(488), PIXELS(8),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), PIXELS(56),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_LIGHT, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(520), PIXELS(8),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(440), 0,
     PIXELS(190), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0,
     PIXELS(174), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), 0,
     PIXELS(174), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0,
     PIXELS(174), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SPRITE_TURNING_ROCK, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(568), 0,
     PIXELS(190), FACING_SOUTH, TALK_FACE_PARTY, 0},
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
    SC_ARG(ANIM_WALK),
    SC_MOVE_BY(0, 0, -PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(-PIXELS(64), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -PIXELS(64)),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_END,
};

/// Babi and the two men walk out of the room, each by his own path.
s32 gAltmillerCave_SecondGuardLeavePath[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(48), 0, 0),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_END,
};

s32 gAltmillerCave_FirstGuardLeavePath[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_END,
};

s32 gAltmillerCave_BabiLeavePath[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_END,
};

// clang-format off
struct SceneEvent gAltmillerCave_EventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 4, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI},
    {EVENT_TOUCH, 60, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel0)},
    {EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel1)},
    {EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel2)},
    {EVENT_TOUCH, 63, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel3)},
    {EVENT_TOUCH, 64, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel4)},
    {EVENT_TOUCH, 65, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel5)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_StartReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_BabiEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 10, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 11, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {EVENT_TALK, ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI, EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {EVENT_TALK, ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, EVENT_SCRIPT(AltmillerCave_RepeatRockOrder)},
    {EVENT_TALK, ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED, EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {EVENT_TOUCH, 19, FLAG_BABI_CALLED_PARTY_BACK, EVENT_SCRIPT(AltmillerCave_BabiCallsPartyBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), EVENT_TRIGGER_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI,
        EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), EVENT_TRIGGER_OWN_EFFECT | ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED,
        EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTMILLER_CAVE_MIMIC_BEATEN, FIND_MIMIC | 5},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_VialChestEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 20, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 21, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTMILLER_CAVE_VIAL, FIND_ITEM(ITEM_VIAL)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_SquallEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 30, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 31, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 72, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_WEST), 73, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 74, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_EAST), 75, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 71, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_MoveStagedActorToClearPosition)},
    {TOUCH_WALKING_INTO(FACING_EAST), 77, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_AdvanceObstacleAndCheckPassage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SQUALL_ROOM_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_UpdateMovedObstacle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_CheckFrostTargetCollision)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_FreezePuddle)},
    {EVENT_TALK, ACTOR_SQUALL, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartDjinnBattle)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_CookieChestEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 40, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 41, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTMILLER_CAVE_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {SCENE_EVENTS_END},
};
// clang-format on

struct SceneEvent gAltmillerCave_DraughtEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 50, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {EVENT_EXIT, 51, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_TO_ROCKS},
    {EVENT_EXIT, 52, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE},
    {EVENT_EXIT, 53, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST},
    {EVENT_EXIT, 54, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_MoveStagedActorToClearPosition)},
    {EVENT_TALK, ACTOR_BLUE_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, ACTOR_GREEN_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, ACTOR_WHITE_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, ACTOR_YELLOW_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {EVENT_TALK, ACTOR_RED_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT,
     FIND_ITEM(ITEM_MYSTIC_DRAUGHT)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {SCENE_EVENTS_END},
};

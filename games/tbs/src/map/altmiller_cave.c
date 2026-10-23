/// @file
/// Altmiller Cave, where the party finds Babi while Tolbi searches for him
/// during Colosso (babi_palace.c). Its six maps share this overlay and the
/// area name Altmiller Cave:
///
/// - AltmillerCaveEntrance (MapId_AltmillerCaveEntrance): the dark passage
///   in from the world map and the chamber beyond it.
/// - AltmillerCaveBabi (MapId_AltmillerCaveBabi): the invisible Babi, who
///   asks for his draught (AltmillerCave_MeetInvisibleBabi) and drinks it
///   (AltmillerCave_RestoreBabiAndInviteParty), and a chest that is a Mimic.
/// - AltmillerCaveVialChest (MapId_AltmillerCaveVialChest) and
///   AltmillerCaveCookieChest (MapId_AltmillerCaveCookieChest): a chest
///   each.
/// - AltmillerCaveSquall (MapId_AltmillerCaveSquall): Squall, a Jupiter
///   Djinni, three fallen columns to push, a stone pillar for Move and a
///   puddle for Frost.
/// - AltmillerCaveDraught (MapId_AltmillerCaveDraught): three more fallen
///   columns, the rock puzzle (AltmillerCave_TurnRock) and the chest that
///   holds the Mystic Draught behind the wall the puzzle opens.
///
/// The rooms share one warp destination table and one warp table, and the
/// other getters choose by gSaveBlock.mapId. AltmillerCave_Initialize sends an
/// entry from outside the cave (warp destination 0) to the room's first warp
/// destination, darkens the dark rooms and puts back what the party has done.
/// Saving Babi (FLAG_BABI_SAVED) is the story flag the rest of Tolbi waits on;
/// the overlay's own state is in map temp flags and gMapLocalState, so it
/// starts over on every ordinary arrival.
///
/// Every room but the two lit ones is dark (FLAG_DARK): BG1 is blended over
/// the rest (ALTMILLER_CAVE_DARKNESS_BLEND), and the light around the party is
/// a display window, an ellipse around the leader inside which the layers are
/// drawn differently (AltmillerCave_InitializeDarkness). In an evenly dark
/// room BG1 covers everything outside it and is only blended inside it.
/// AltmillerCaveEntrance darkens by steps instead
/// (FLAG_DARKNESS_BY_STEPS): coord events 60 to 65 each set a level, kept in
/// gMapLocalState byte 4, from the lightest at the mouth to the darkest by the
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
/// The rock puzzle keeps the rounds matched in gMapLocalState byte 0 and its
/// state and the light at the top in struct RockPuzzle after it. Turning the
/// leftmost rock and then the rightmost brings out the ring of five lights; the
/// ring spins (AltmillerCave_SpinLights, a task) and stops with one
/// light at the top, and turning that light's rock spins it on to a different
/// light. A wrong rock puts the lights away. The third match bursts the wall
/// (AltmillerCave_ReleaseWallBurst). Coming up to the rocks from the pillar
/// cave (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) starts the puzzle over.
///
/// Open questions: none.

#include "save_block.h"
#include "version.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/map/altmiller_cave.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "pushed_object.h"
#include "edition.h"
#include "object_dispatch.h"
#include "map_ids.h"
#include "dma.h"
#include "io_reg.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "call.h"
#include "display_transition.h"
#include "field_effect_sprite.h"
#include "item_ids.h"
#include "objcmd.h"
#include "frame.h"
#include "fixed_math.h"
#include "map_header.h"
#include "fx_work.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"

/// AltmillerCaveBabi's object events: Babi while invisible and once seen, and
/// the two men from Tolbi who come looking for him.
enum {
  /// Babi's second sheet, SPRITE_INVISIBLE_BABI: the old man with
  /// long pink hair and a cane, standing unseen in the cave.
  /// ★ Named from the game: "Wrapped in Psynergetic light... Who could it
  /// be?" (MsgAltmillerSee)
  LOCALID_INVISIBLE_BABI = 8,
  /// SPRITE_BABI, the same old man once seen.
  /// ★ Named from the game: "Babi!" (MsgAltmillerBabi)
  LOCALID_BABI = 9,
  /// Their sprite is a helmeted soldier carrying a torch
  /// (SPRITE_SOLDIER_WITH_TORCH).
  /// ★ Named from what the player sees: a soldier from Tolbi, sent to find
  /// Babi (its render)
  LOCALID_FIRST_SOLDIER = 10,
  /// ★ Named from what the player sees: a second soldier from Tolbi (its
  /// render)
  LOCALID_SECOND_SOLDIER = 11
};

/// AltmillerCaveSquall and AltmillerCaveDraught place first the three fallen
/// columns the party pushes; AltmillerCaveSquall then a puddle, a pillar for
/// Move and Squall, a Jupiter Djinni.
enum {
  /// ★ Named from what the player sees: white stone columns lying across the
  /// floor, in the way (its render)
  LOCALID_FIRST_OBSTACLE = 8,
  /// ★ Named from what the player sees: a small blue puddle in the middle of
  /// the floor (its render)
  LOCALID_SQUALL_ROOM_PUDDLE = 11,
  /// ★ Named from what the player sees: a brown stone pillar by the room's
  /// east wall (its render)
  LOCALID_SQUALL_ROOM_PILLAR = 12,
  /// ★ Named from the game: "Squall" (MsgDjinnName)
  LOCALID_SQUALL = 13
};

/// AltmillerCaveDraught: the five coloured lights and the five rocks, each in
/// Babi's order, the rocks left to right.
enum {
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_FIRST_LIGHT = 11,
  /// ★ Named from the game: "The blue light corresponds to the rock farthest
  /// to the left." (MsgAltmillerDontTrustAnyone)
  LOCALID_BLUE_LIGHT = LOCALID_FIRST_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_GREEN_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_WHITE_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_YELLOW_LIGHT,
  /// ★ Named from the game: "There will be five colors: blue, green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_RED_LIGHT,
  /// ★ Named from the game: "Turn the five rocks in accordance with those
  /// colors." (MsgAltmillerDontTrustAnyone)
  LOCALID_FIRST_ROCK,
  /// ★ Named from the game: "The blue light corresponds to the rock farthest
  /// to the left." (MsgAltmillerDontTrustAnyone)
  LOCALID_BLUE_ROCK = LOCALID_FIRST_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_GREEN_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_WHITE_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_YELLOW_ROCK,
  /// ★ Named from the game: "In order, the other rocks are green, white,
  /// yellow, and red." (MsgAltmillerDontTrustAnyone)
  LOCALID_RED_ROCK
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

/// The map temp flags the cave keeps.
enum {
  /// The cave is dark: set on entry everywhere but AltmillerCaveSquall and
  /// AltmillerCaveDraught.
  /// ★ Named from what the player sees: the cave dark but for a small circle of
  /// light around the party (its render)
  FLAG_DARK = FLAG_MAP_TEMP_00,
  /// AltmillerCaveEntrance darkens by steps as the party walks in.
  /// ★ Named from its area name: Altmiller Cave
  FLAG_DARKNESS_BY_STEPS = FLAG_MAP_TEMP_01,
  /// Frost has frozen AltmillerCaveSquall's puddle.
  /// ★ Named from what the player sees: a small puddle of water in the middle
  /// of the room (its render)
  FLAG_PUDDLE_FROZEN = FLAG_MAP_TEMP_11,
  /// AltmillerCaveSquall's pillar stands where Move opens the passage.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  FLAG_PILLAR_IN_PLACE = FLAG_MAP_TEMP_12
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
  s16 ticks;
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

/// AltmillerCaveDraught's rock puzzle, kept in gMapLocalState: the rounds of
/// lights matched in byte 0, then the state and the light shown.
struct RockPuzzle {
  u8 state;
  s8 chosen;
};

#define ROCK_PUZZLE ((struct RockPuzzle *)&gMapLocalState[1])

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

#include "common/object_staged_object_event.h"

/// Talking to the Djinni in AltmillerCaveSquall starts the battle with Squall,
/// a Jupiter Djinni.
static void AltmillerCave_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(LOCALID_SQUALL, DJINNI_SQUALL);
}

/// The six rooms share one warp destination table.
const struct MapWarpDest *AltmillerCave_GetWarpDests(void) {
  return gAltmillerCave_WarpDests;
}

const struct MapWarpRegion *AltmillerCave_GetRegions(void) { return NULL; }

const u32 *AltmillerCave_GetWarps(void) { return gAltmillerCave_Warps; }

const struct ObjectEventTemplate *AltmillerCave_GetObjectEventTemplates(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_AltmillerCaveBabi) {
    return gAltmillerCave_BabiObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltmillerCaveSquall) {
    return gAltmillerCave_SquallObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltmillerCaveDraught) {
    return gAltmillerCave_DraughtObjectEventTemplates;
  }
  return gAltmillerCave_ObjectEventTemplatesOther;
}

/// Darkens AltmillerCaveEntrance to its current level: the brighter blend at
/// level 0, each level one step darker.
static void AltmillerCave_ApplyFadeBlend(void) {
  s8 level = ((s8 *)gMapLocalState)[4];

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
  u8 *fade = &gMapLocalState[4];

  work = gFieldWork;
  *fade = level;
  if (work->revealActive == 0) {
    AltmillerCave_ApplyFadeBlend();
  }
}

/// Crossing the first darkness boundary sets the cave's lightest level.
/// AltmillerCaveEntrance's boundaries are coord events 60 to 65:
/// crossing one sets its level, 0 to 5, each a step darker than the last.
static void AltmillerCave_DarkenToLevel0(void) {
  AltmillerCave_SetFadeLevel(0);
}

/// Crossing the second darkness boundary (coord event 61) sets darkness level
/// 1.
static void AltmillerCave_DarkenToLevel1(void) {
  AltmillerCave_SetFadeLevel(1);
}

/// Crossing the third darkness boundary (coord event 62) sets darkness level 2.
static void AltmillerCave_DarkenToLevel2(void) {
  AltmillerCave_SetFadeLevel(2);
}

/// Crossing the fourth darkness boundary (coord event 63) sets darkness level
/// 3.
static void AltmillerCave_DarkenToLevel3(void) {
  AltmillerCave_SetFadeLevel(3);
}

/// Crossing the fifth darkness boundary (coord event 64) sets darkness level 4.
static void AltmillerCave_DarkenToLevel4(void) {
  AltmillerCave_SetFadeLevel(4);
}

/// Crossing the sixth darkness boundary (coord event 65) sets darkness level 5.
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
    work->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transitionFrames = 24;
  }
  WaitFrames(1);
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
  if (GameFlag_Get(FLAG_DARKNESS_BY_STEPS) != 0) {
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
  if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveEntrance) {
    s32 alpha = BLEND_ALPHA(0, 16);

    REG_BLDALPHA = alpha;
  }
  if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveDraught) {
    ObjectEvent_ConfigureSpriteColor(LOCALID_BLUE_ROCK, COLOR_BLUE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_GREEN_ROCK, COLOR_GREEN);
    ObjectEvent_ConfigureSpriteColor(LOCALID_WHITE_ROCK, COLOR_WHITE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_YELLOW_ROCK, COLOR_YELLOW);
    ObjectEvent_ConfigureSpriteColor(LOCALID_RED_ROCK, COLOR_RED);
  }
}

/// Reveal's light ends: the cave darkens again and AltmillerCaveDraught's rocks
/// lose their colors.
static void AltmillerCave_EndReveal(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;

  if (GameFlag_Get(FLAG_DARK) != 0) {
    AltmillerCave_InitializeDarkness();
    effectWork->childOption = 1;
  }
  if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveDraught) {
    ObjectEvent_ConfigureSpriteColor(LOCALID_BLUE_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_GREEN_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_WHITE_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_YELLOW_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_RED_ROCK, COLOR_NONE);
  }
}

/// Retreat's first phase: the Psynergy's first object event faces south, and
/// its object is copied into a new one for the effect to use.
static void AltmillerCave_StartRetreat(void) {
  struct FieldEffectWork *effectWork;
  struct ObjectEvent *original;
  struct ObjectEvent *replacement;

  effectWork = (struct FieldEffectWork *)gFieldEffectWork;
  original = effectWork->mainObject;
  ObjectMotion_ArmCallback(effectWork->firstObjectId, FACING_SOUTH, 0);
  Anim_ApplyChildValues((struct Object *)original, 0);
  FieldScript_WaitFrames(20);
  replacement = ObjectEvent_Create(0, original->x.fixed, original->y.fixed,
                                   original->z.fixed);
  if (replacement != 0) {
    Dma_Set(original, replacement,
            (DMA_ENABLE | DMA_32BIT) << 16 | sizeof(struct ObjectEvent) / 4,
            REG_DMA3);
    original->update = 0;
    effectWork->mainObject = replacement;
    original->spriteStorageKind = 0;
  }
}

/// After Move on AltmillerCaveSquall's pillar: in its cell at (30, 20) it
/// settles and opens the way beside it (remembered in TBS_PATCH_1 and later).
static void AltmillerCave_UpdateMovedObstacle(void) {
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_SQUALL_ROOM_PILLAR);

  if (objectEvent->x.fixed >> 20 == 30 && objectEvent->z.fixed >> 20 == 20) {
    objectEvent->motionFlags = 2;
    objectEvent->groundHeight = 0;
    objectEvent->priorityFlags = 2;
    MapGrid_CopyCellAttributes(30, 20, 1, 1, 32, 20);
#if TBS_PATCH_1
    GameFlag_Set(FLAG_PILLAR_IN_PLACE);
#endif
  }
}

/// Walking east into AltmillerCaveSquall's pillar pushes it a cell.
static void AltmillerCave_AdvanceObstacleAndCheckPassage(void) {
  PushedObject_AdvancePair();
  AltmillerCave_UpdateMovedObstacle();
}

/// Frost's first phase on AltmillerCaveSquall's puddle: notes whether its place
/// is blocked, which keeps it from freezing.
static void AltmillerCave_CheckFrostTargetCollision(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;
  struct ObjectEvent *target = ObjectEvent_GetById(LOCALID_SQUALL_ROOM_PUDDLE);
  struct FieldPosition position;

  position.x = target->x.fixed;
  position.y = target->y.fixed;
  position.z = target->z.fixed;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    effectWork->childMode = 1;
  }
}

/// Frost on AltmillerCaveSquall's puddle, unless blocked: it freezes and its
/// cell takes the frozen attributes kept at (76, 16).
static void AltmillerCave_FreezePuddle(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;
  struct ObjectEvent *puddle = ObjectEvent_GetById(LOCALID_SQUALL_ROOM_PUDDLE);
  s32 blocked;

  blocked = effectWork->childMode;
  if (blocked == 0) {
    MapGrid_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    if (puddle != 0) {
      s32 motionFlags = 2;
      puddle->motionFlags = motionFlags;
      puddle->priorityFlags = blocked;
    }
    GameFlag_Set(FLAG_PUDDLE_FROZEN);
  }
}

/// Walking into AltmillerCaveSquall's puddle while it is not frozen: the leader
/// hops 32 pixels over it, if the landing is clear.
static void AltmillerCave_HopSelectedObjectEvent(void) {
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(gSaveBlock.leaderLocalId);
  s32 motionFlags = objectEvent->motionFlags;
  union {
    struct FieldPosition position;
    s32 coordinates[3];
    union FieldCoordinate components[3];
  } destination;
  union FieldCoordinate *components = destination.components;

  components[0].fixed = objectEvent->x.fixed;
  components[1].fixed = objectEvent->y.fixed;
  components[2].fixed = objectEvent->z.fixed;
  {
    s32 angle = objectEvent->facing & 0xf000;

    Vector_AddPolarOffset(PIXELS(32), angle, destination.coordinates);
  }
  if (FieldObject_CheckMovementCollision(objectEvent, &destination.position) ==
      0) {
    FieldScript_Start();
    Object_SelectAnimation(objectEvent, ANIM_LEADER_SPRING);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Object_SelectAnimation(objectEvent, ANIM_LEADER_LEAP);
    objectEvent->speed = PIXELS(3);
    objectEvent->acceleration = PIXELS(2);
    objectEvent->velocityY = PIXELS(4);
    objectEvent->motionFlags &= 126;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER,
                                      components[0].part.pixel,
                                      components[2].part.pixel);
    Object_SelectAnimation(objectEvent, ANIM_LEADER_SPRING);
    ObjectEvent_SetSpriteFlags(objectEvent, 1);
    objectEvent->motionFlags = motionFlags;
    FieldScript_End();
  }
}

/// Walking into one of the three obstacles pushes it, if there is room.
static void AltmillerCave_MoveStagedObjectEventToClearPosition(void) {
  struct PushedObjectProbe probe;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);
  }
  FieldScript_End();
}

/// Update callback of the light at the top of the stopped ring: it pulses
/// through four sizes.
static void AltmillerCave_PulseLight(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *light = (struct ObjectEvent *)fieldObject;
  s32 scale = gAltmillerCave_PulseScales[(light->timer >> 2) & 3];

  light->scaleX = scale;
  light->scaleY = scale;
  light->timer++;
  light->timer &= 15;
}

/// Puts a light on the ring, radius pixels from its center above the rocks, at
/// the given angle.
static void AltmillerCave_PlaceLight(s32 localId, s32 radius, s32 angle) {
  s32 centreX = PIXELS(504);
  s32 centreHeight = PIXELS(24);
  s32 depth = PIXELS(144);
  struct ObjectEvent *light = ObjectEvent_GetById(localId);
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
    AltmillerCave_PlaceLight(LOCALID_FIRST_LIGHT + i, PIXELS(24), angle);
    angle -= 0x3333; // One fifth of a full turn.
    i++;
  } while (i < LIGHT_COUNT);
}

/// Task that spins the ring of lights: it speeds up, coasts, slows
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
    s32 chosenLight = ((s8 *)gMapLocalState)[2];
    s32 chosenAngle = Math_Divide(chosenLight * Q16_ONE, 5);
    if ((unsigned int)(((u32)(wheel->angle - chosenAngle) << 16) +
                       0xc2ff0000) <= 0x5fe0000) {
      wheel->angle = chosenAngle + 0x4000;
      wheel->phase = WHEEL_STOPPED;
      wheel->speed = 0;
      ObjectEvent_GetById(LOCALID_FIRST_LIGHT + chosenLight)->update =
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
      Sound_PlaySong(SE_135);
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
  struct ObjectEvent *objectEvent;
  s32 scale;
  s8 state;
  s32 chosenLight;
  u8 previousLight;
  s32 radius;
  u16 angle;
  u8 *puzzleState;

  Script_Start();
  Field_EndBusyAndReveal();
  Script_SetMessage(MsgAltmillerRotatedRock);
  Script_ShowMessage(LOCALID_FIRST_ROCK, 0);
  Script_End();
  for (i = 0; i < LIGHT_COUNT; i++) {
    objectEvent = ObjectEvent_Get(LOCALID_FIRST_LIGHT + i);
    objectEvent->update = NULL;
    objectEvent->scaleX = Q16_ONE;
    objectEvent->scaleY = Q16_ONE;
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
    if (rock == LOCALID_BLUE_ROCK) {
      state = PUZZLE_LEFT_ROCK_TURNED;
      Script_PlaySong(SE_110);
    } else {
      Script_PlaySong(SE_MENU_ERROR);
    }
    gMapLocalState[0] = 0;
  } else if (state == PUZZLE_LEFT_ROCK_TURNED) {
    if (rock == LOCALID_BLUE_ROCK) {
      Script_PlaySong(SE_110);
    } else if (rock == LOCALID_RED_ROCK) {
      // The lights grow in one by one, each with SE_151.
      state = PUZZLE_LIGHTS_SHOWN;
      Script_PlaySong(SE_110);
      Task_Wait(30);
      angle = gAltmillerCave_LightWheel->angle;
      for (i = 0; i < LIGHT_COUNT; i++) {
        AltmillerCave_PlaceLight(LOCALID_FIRST_LIGHT + i, PIXELS(24), angle);
        Script_PlaySong(SE_151);
        objectEvent = ObjectEvent_Get(LOCALID_FIRST_LIGHT + i);
        objectEvent->scaleX = 0;
        scale = 2 * Q16_ONE / 5;
        do {
          objectEvent->scaleY = scale;
          objectEvent->scaleX = scale;
          Task_Wait(1);
          scale += 3 * Q16_ONE / 64;
        } while (objectEvent->scaleX < Q16_ONE);
        angle -= 0x3333;
      }
      Task_Wait(30);
      correctRock = 1;
    } else {
      Script_PlaySong(SE_MENU_ERROR);
      state = PUZZLE_START;
    }
  } else if (state == PUZZLE_LIGHTS_SHOWN) {
    if (rock != LOCALID_FIRST_ROCK + chosenLight) {
      // The wrong rock: the lights shrink away.
      state = PUZZLE_START;
      Script_PlaySong(SE_MENU_ERROR);
      Task_Wait(30);
      for (i = 0; i < LIGHT_COUNT; i++) {
        objectEvent = ObjectEvent_Get(LOCALID_FIRST_LIGHT + i);
        Script_PlaySong(SE_151);
        for (scale = objectEvent->scaleX; objectEvent->scaleX > 2 * Q16_ONE / 5;
             scale -= 3 * Q16_ONE / 64) {
          objectEvent->scaleY = scale;
          objectEvent->scaleX = scale;
          Task_Wait(1);
        }
        ObjectEvent_SetPosition(LOCALID_FIRST_LIGHT + i, 0, 0);
      }
    } else {
      Script_PlaySong(SE_110);
      correctRock = 1;
      Task_Wait(30);
    }
  }
  puzzleState = &gMapLocalState[1];
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
    Task_AddCallback(AltmillerCave_SpinLights, TASK_PRIORITY_MAP);
    if (rounds <= 2) {
      while (gAltmillerCave_LightWheel->phase != WHEEL_STOPPED) {
        Task_Wait(1);
      }
      Task_Wait(10);
      Script_PlaySong(SE_110);
    } else {
      // The third light matched: the ground shakes, the ring spirals in to
      // nothing, and the wall bursts.
      puzzleState[0] = PUZZLE_SOLVED;
      while (gAltmillerCave_LightWheel->phase != WHEEL_SLOWING) {
        Task_Wait(1);
      }
      gAltmillerCave_LightWheel->phase = WHEEL_SLOWING;
      gAltmillerCave_LightWheel->ticks = 0;
      MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
      FieldScript_WaitFrames(20);
      MapGrid_SetShake(PIXELS(4), PIXELS(4), Q16_ONE);
      gAltmillerCave_LightWheel->phase = WHEEL_STOPPED;
      Script_PlaySong(SE_190);
      radius = PIXELS(24);
      angle = gAltmillerCave_LightWheel->angle;
      do {
        for (i = 0; i < LIGHT_COUNT; i++) {
          objectEvent = ObjectEvent_Get(LOCALID_FIRST_LIGHT + i);
          objectEvent->scaleX -= 16;
          objectEvent->scaleY -= 16;
          AltmillerCave_PlaceLight(LOCALID_FIRST_LIGHT + i, radius, angle);
          angle -= 0x3333;
        }
        radius -= PIXELS(1) / 5;
        angle += 0xc00;
        Task_Wait(1);
      } while (radius > 0);
      for (i = 0; i < LIGHT_COUNT; i++) {
        ObjectEvent_SetPosition(LOCALID_FIRST_LIGHT + i, 0, 0);
      }
      AltmillerCave_ReleaseWallBurst();
      Script_PlaySong(MUS_PUZZLE_SOLVED);
    }
    Task_Remove(AltmillerCave_SpinLights);
  }
}

/// Divides the nonnegative random value for wall-fragment scaling.
s32 Math_DivideUnsigned(s32 value, s32 divisor);

/// The party turns north, toward Babi.
static __inline__ void AltmillerCave_FacePartyNorth(void) {
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
}

/// In AltmillerCaveBabi, talking to Babi or reading his mind once the Mystic
/// Draught is found: he drinks it and appears, startling the party. Ivan reads
/// his mind and finds him to be Babi, the ruler of Tolbi, who sought Lemurians
/// to escape growing old. Two soldiers from Tolbi find him and take him back,
/// and he invites the party to Colosso's finals; Garet asks whether they
/// should take part.
static void AltmillerCave_RestoreBabiAndInviteParty(void) {
  struct ScreenTransitionState *state;
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *leader;
  s32 i;
  s32 eva;
  s32 evb;

  // Give Babi the draught and bring the party beside him.
  GameFlag_Set(FLAG_BABI_SAVED);
  PartyInventory_Discard(ITEM_MYSTIC_DRAUGHT);
  FieldScript_Start();
  Field_EndBusyAndReveal();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MsgAltmillerCameBackPleaseDraughtQuickly);
  Script_ShowMessage(LOCALID_INVISIBLE_BABI, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 232, 160);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  FieldScript_WaitFrames(50);
  FieldScript_WaitFrames(10);
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(200), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 264, 208);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  FieldMotion_LaunchFromFocusedObject(LOCALID_GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(LOCALID_MIA, 0, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(LOCALID_IVAN, 16, 16, FACING_NORTH);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE,
                        40);
  Script_ShowMessage(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  objectEvent = ObjectEvent_Get(LOCALID_INVISIBLE_BABI);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(60);
  Script_PlaySong(SONG_FADE_OUT_BGM);
  objectEvent->sprite->rotation = 0;
  ObjectMotion_Launch(LOCALID_INVISIBLE_BABI, 10, 70);
  // Flicker between sprite priorities before replacing invisible Babi.
  for (i = 0; i < 30; i++) {
    objectEvent->sprite->image.priority = 0;
    FieldScript_WaitFrames(2);
    objectEvent->sprite->image.priority = 2;
    FieldScript_WaitFrames(2);
  }
  FieldScript_WaitFrames(40);
  {
    s32 x;
    s32 z;

    x = ObjectEvent_Get(LOCALID_INVISIBLE_BABI)->x.part.pixel;
    z = ObjectEvent_Get(LOCALID_INVISIBLE_BABI)->z.part.pixel;
    ObjectEvent_SetPosition(LOCALID_INVISIBLE_BABI, 0, 0);
    ObjectEvent_SetPosition(LOCALID_BABI, PIXELS(x), PIXELS(z));
  }
  objectEvent->sprite->flags = 0;
  Script_PlaySong(MUS_030);
  ObjectEvent_SetSpeed(LOCALID_BABI, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, 32, 32);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_MIA)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_IVAN)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_WalkBy(LOCALID_PARTY_LEADER, 0, 16);
  ObjectEvent_WalkBy(LOCALID_GARET, 0, 16);
  ObjectEvent_WalkBy(LOCALID_MIA, 0, 16);
  ObjectEvent_WalkByAndWait(LOCALID_IVAN, 0, 16);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_MIA)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  ObjectEvent_Get(LOCALID_IVAN)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  WaitFrames(1);
  AltmillerCave_FacePartyNorth();
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectMotion_Launch(LOCALID_GARET, 4, 13);
  ObjectMotion_Launch(LOCALID_GARET, 4, 30);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_GARET, 30);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_IVAN, 30);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  Script_OpenMessage(LOCALID_BABI, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
    FieldScript_WaitFrames(30);
    FieldScript_SetMessageId(MsgAltmillerWouldHaveRevealedMyselfSooner);
    Script_ShowMessage(LOCALID_BABI, 0);
  } else {
    FieldScript_SetMessageId(MsgAltmillerMustTraveledIndeedIfDidnt);
    Script_ShowMessage(LOCALID_BABI, 0);
  }
  // Ask how Babi became invisible and hear about his Alchemy.
  FieldScript_SetMessageId(MsgAltmillerHowDidBecomeParalyzedWhile);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
  AltmillerCave_FacePartyNorth();
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_IVAN, 0, -48);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  FieldScript_WaitFrames(40);
  // Ivan reads Babi's mind: he is Babi, the ruler of Tolbi, and hid by the
  // power of an ancient civilization.
  FieldEffect_LoadActionEffectResources(ABILITY_MIND_READ, 1);
  FieldEffect_SetupObjectPair(LOCALID_IVAN, LOCALID_BABI);
  FieldEffect_StartCast();
  Field_DispatchTypeHandler(1);
  FieldScript_WaitFrames(150);
  Field_DispatchTypeHandler(2);
  FieldEffect_StopCasterHook();
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_STAND);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(40);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_BABI, 4, 13);
  ObjectMotion_Launch(LOCALID_BABI, 4, 30);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_EAST, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(50);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_IVAN, 0, 48);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
  AltmillerCave_FacePartyNorth();
  FieldScript_WaitFrames(50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_NORTH, 0);
  FieldScript_WaitFrames(50);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_SetMessageId(MsgAltmillerThatsKradenSaidAlchemyCould);
    Script_ShowMessage(LOCALID_GARET, 0);
  } else {
    FieldScript_SetMessageId(MsgAltmillerForgotKradenSaidAlchemyCould);
    Script_ShowMessage(LOCALID_GARET, 0);
  }
  FieldScript_SetMessageId(MsgAltmillerWhatWasThat);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  FieldScript_WaitFrames(10);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_OpenMessage(LOCALID_BABI, 0);
  FieldScript_SetMessageId(MsgAltmillerBabi);
  FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0);
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
    WaitFrames(7);
  }
  {
    u16 *slot = (u16 *)&state->active;
    s32 value = ALTMILLER_CAVE_BRIGHTENING_WINOUT;

    *slot = value;
  }
  ObjectEvent_SetPosition(LOCALID_FIRST_SOLDIER, PIXELS(280), PIXELS(304));
  ObjectEvent_SetPosition(LOCALID_SECOND_SOLDIER, PIXELS(296), PIXELS(304));
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  FieldScript_WaitFrames(10);
  FieldScript_WaitFrames(10);
  FieldCamera_FollowObjectEvent(LOCALID_FIRST_SOLDIER, 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(70);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_PARTY_LEADER,
                                       LOCALID_FIRST_SOLDIER);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_GARET, LOCALID_FIRST_SOLDIER);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_MIA, LOCALID_FIRST_SOLDIER);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_IVAN, LOCALID_FIRST_SOLDIER);
  // The two men greet Babi; he invites the party to the finals.
  ObjectEvent_SetSpeed(LOCALID_FIRST_SOLDIER, PIXELS(7) / 5, PIXELS(7) / 10);
  ObjectEvent_SetSpeed(LOCALID_SECOND_SOLDIER, PIXELS(7) / 5, PIXELS(7) / 10);
  ObjectEvent_SetActionScript(LOCALID_FIRST_SOLDIER,
                              gAltmillerCave_GuardsArrivePath);
  FieldScript_WaitFrames(3);
  ObjectEvent_WalkByAndWait(LOCALID_SECOND_SOLDIER, -16, 0);
  ObjectEvent_SetActionScript(LOCALID_SECOND_SOLDIER,
                              gAltmillerCave_GuardsArrivePath);
  ObjectEvent_RefreshSelectorById(LOCALID_FIRST_SOLDIER);
  ObjectEvent_WalkByAndWait(LOCALID_FIRST_SOLDIER, 0, -16);
  ObjectEvent_RefreshSelectorById(LOCALID_SECOND_SOLDIER);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_EAST, 0);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_MIA);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_IVAN);
  AltmillerCave_FacePartyNorth();
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(216), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(50);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  FieldScript_WaitFrames(10);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  ObjectEvent_WalkByAndWait(LOCALID_SECOND_SOLDIER, 16, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  ObjectEvent_WalkByAndWait(LOCALID_FIRST_SOLDIER, 16, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_Launch(LOCALID_SECOND_SOLDIER, 4, 13);
  ObjectMotion_Launch(LOCALID_SECOND_SOLDIER, 4, 30);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(70);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_EAST, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_EAST, 0);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        40);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_WEST, 0);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_FIRST_SOLDIER, LOCALID_SECOND_SOLDIER, 70);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_SOUTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_SOUTHEAST, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_SECOND_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
  AltmillerCave_FacePartyNorth();
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_SMILE,
                        40);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_EAST, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_EAST, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_PARTY_LEADER, LOCALID_BABI);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_GARET, LOCALID_BABI);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_MIA, LOCALID_BABI);
  ObjectEvent_LinkObjectAndSetCallback(LOCALID_IVAN, LOCALID_BABI);
  ObjectEvent_SetSpeed(LOCALID_BABI, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, -16, 0);
  ObjectEvent_WalkBy(LOCALID_FIRST_SOLDIER, 0, 48);
  ObjectEvent_WalkBy(LOCALID_SECOND_SOLDIER, 0, 48);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, -16, 0);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, 0, 32);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_SOLDIER, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_SECOND_SOLDIER, ANIM_STAND);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_PARTY_LEADER);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_GARET);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_MIA);
  ObjectMotion_EnableActionAndResetMotion(LOCALID_IVAN);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 30);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_BABI, 30);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_BABI, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  FieldScript_WaitFrames(70);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  FieldScript_WaitFrames(30);
  // Babi and the two men walk back toward the cave entrance.
  ObjectEvent_SetActionScript(LOCALID_SECOND_SOLDIER,
                              gAltmillerCave_SecondGuardLeavePath);
  ObjectEvent_SetActionScript(LOCALID_FIRST_SOLDIER,
                              gAltmillerCave_FirstGuardLeavePath);
  ObjectEvent_SetActionScript(LOCALID_BABI, gAltmillerCave_BabiLeavePath);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 0);
  ObjectEvent_RefreshSelectorById(LOCALID_BABI);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(20);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(248), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FIRST_SOLDIER, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_WEST, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_WEST, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_FIRST_SOLDIER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_SECOND_SOLDIER, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_BABI, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceObjectEvent(LOCALID_FIRST_SOLDIER, LOCALID_SECOND_SOLDIER,
                              60);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_WEST, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_BABI, FACING_EAST, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_BABI, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_FIRST_SOLDIER, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_SECOND_SOLDIER, ANIM_NOD);
  FieldScript_WaitFrames(40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_SOLDIER, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_SECOND_SOLDIER, FACING_EAST, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_WalkBy(LOCALID_BABI, 32, 0);
  ObjectEvent_WalkBy(LOCALID_SECOND_SOLDIER, 0, 64);
  ObjectEvent_WalkByAndWait(LOCALID_FIRST_SOLDIER, 16, 0);
  ObjectEvent_WalkBy(LOCALID_FIRST_SOLDIER, 0, 64);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_BABI);
  ObjectEvent_WalkByAndWait(LOCALID_BABI, 0, 64);
  ObjectEvent_SetPosition(LOCALID_BABI, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_SOLDIER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_SECOND_SOLDIER, 0, 0);
  FieldScript_WaitFrames(20);
  // With Babi gone the darkness returns: BG1 fades back in, then the light
  // shrinks around the party again.
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = ALTMILLER_CAVE_DARKNESS_BLEND;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = BLEND_ALPHA(eva, evb);
    WaitFrames(7);
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
  WaitFrames(1);
  Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
  Io_Write16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
  ObjectEvent_PlaceCurrentWithinCameraBounds(LOCALID_PARTY_LEADER, 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  Script_OpenMessage(LOCALID_GARET, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_MIA, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgAltmillerSoWellDoIt);
    Script_ShowMessage(LOCALID_GARET, 0);
  } else {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgAltmillerDontWantEitherDo);
    Script_ShowMessage(LOCALID_GARET, 0);
  }
  // Let Isaac answer the invitation and reunite the party.
  FieldScript_SetMessageId(MsgAltmillerEitherWayWereStuckHere);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Script_ShowMessage(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_MIA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_ShowMessage(LOCALID_GARET, 0);
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    FieldScript_WaitFrames(30);
    FieldScript_SetMessageId(MsgAltmillerGreatItsDecidedJustHave);
    Script_ShowMessage(LOCALID_GARET, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
    FieldScript_WaitFrames(30);
    Script_ShowMessage(LOCALID_IVAN, 0);
    FieldScript_WaitFrames(20);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
    FieldScript_WaitFrames(30);
  } else {
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(30);
    FieldScript_SetMessageId(MsgAltmillerDontLetMeDownLike);
    Script_ShowMessage(LOCALID_GARET, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
    FieldScript_WaitFrames(30);
    Script_ShowMessage(LOCALID_IVAN, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
    FieldScript_WaitFrames(30);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
  }
  Script_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_MIA, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  Map_PlayMusic();
  FieldScript_End();
}

/// In AltmillerCaveBabi, talking to the invisible Babi or reading his mind
/// the first time: he can be heard but not seen, cannot move, and asks the
/// party to bring his draught from deep in the cave, telling the order of the
/// five rocks.
static void AltmillerCave_MeetInvisibleBabi(void) {
  struct ObjectEvent *leader;
  s32 line;

  // Gather the party around the invisible voice.
  GameFlag_Set(FLAG_MET_INVISIBLE_BABI);
  Sound_PlaySong(MUS_CAVE);
  FieldScript_Start();
  Field_EndBusyAndReveal();
  FieldScript_SetMessageId(MsgAltmillerSee);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(184), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 248, 192);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  FieldMotion_LaunchFromFocusedObject(LOCALID_GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(LOCALID_MIA, 0, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(LOCALID_IVAN, 16, 16, FACING_NORTH);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  FieldScript_WaitFrames(40);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetAttachedEffect(
      LOCALID_INVISIBLE_BABI, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        40);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 30);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_ShowMessage(LOCALID_GARET, 0);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessage(LOCALID_INVISIBLE_BABI, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI,
                        EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldEffect_SpawnLinked(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_INVISIBLE_BABI, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_OpenMessage(LOCALID_INVISIBLE_BABI, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_PARTY_LEADER, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_SetMessageId(MsgAltmillerKiddingHaventActually);
    FieldScript_WaitFrames(20);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(2), PIXELS(1));
    ObjectEvent_WalkByAndWait(LOCALID_GARET, 0, -16);
    FieldScript_WaitFrames(10);
    ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 30);
    FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  } else {
    FieldScript_SetMessageId(MsgAltmillerDontTrustAnyone);
    FieldScript_WaitFrames(10);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkByAndWait(LOCALID_GARET, 0, -16);
    FieldScript_WaitFrames(10);
    ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 30);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  }

  // Question Babi's condition before agreeing to find his draught.
  line = MsgAltmillerSaidCouldntMove;
  FieldScript_SetMessageId(line);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkByAndWait(LOCALID_IVAN, 0, -16);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_IVAN, LOCALID_PARTY_LEADER, 30);
  Script_ShowMessage(LOCALID_IVAN, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_MIA, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_MIA, 0);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_MIA, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectMotion_CommitPositionAndActivate(LOCALID_GARET, 0, 16);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 30);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectMotion_CommitPositionAndActivate(LOCALID_IVAN, 0, 16);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  // Explain the hidden draught and the order of the five rocks.
  line += 7;
  FieldScript_SetMessageId(line);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE,
                        40);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 50);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 50);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE,
                        50);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE,
                        40);
  Script_ShowMessage(LOCALID_INVISIBLE_BABI, 0);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INVISIBLE_BABI, 2);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_MIA, 40);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_MIA, 0);
  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_MIA, 0);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  Sound_PlaySong(SONG_FADE_OUT_BGM);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(LOCALID_GARET, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(LOCALID_IVAN, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  ObjectEvent_SetPosition(LOCALID_IVAN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(LOCALID_MIA, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  Map_PlayMusic();
  FieldScript_End();
}

/// In AltmillerCaveBabi, talking to Babi again before the draught is found:
/// he asks whether the party forgot the order of the rock colors, and tells it
/// again.
static void AltmillerCave_RepeatRockOrder(void) {
  s32 question;

  FieldScript_Start();
  question = MsgAltmillerForgetOrderRock;
  FieldScript_SetMessageId(question);
  FieldMessage_ShowAtObject(LOCALID_INVISIBLE_BABI, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(question + 1);
    FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  } else {
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(question + 2);
    FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  }
  FieldScript_End();
}

/// In AltmillerCaveBabi, walking away from Babi (coord event 19) the first
/// time after meeting him: he calls the party back and says where the draught
/// is. "Wait... The draught is... on a table behind the door."
static void AltmillerCave_BabiCallsPartyBack(void) {
  if (GameFlag_Get(FLAG_MET_INVISIBLE_BABI) == 0)
    return;
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0)
    return;

  GameFlag_Set(FLAG_BABI_CALLED_PARTY_BACK);
  FieldScript_Start();
  FieldScript_SetMessageId(MsgAltmillerWait);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_INVISIBLE_BABI, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_INVISIBLE_BABI, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_End();
}

const struct MapEvent *AltmillerCave_GetEvents(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_AltmillerCaveBabi) {
    return gAltmillerCave_BabiEvents;
  }
  if (mapId == (s32)&MapId_AltmillerCaveVialChest) {
    return gAltmillerCave_VialChestEvents;
  }
  if (mapId == (s32)&MapId_AltmillerCaveSquall) {
    return gAltmillerCave_SquallEvents;
  }
  if (mapId == (s32)&MapId_AltmillerCaveCookieChest) {
    return gAltmillerCave_CookieChestEvents;
  }
  if (mapId == (s32)&MapId_AltmillerCaveDraught) {
    return gAltmillerCave_DraughtEvents;
  }
  return gAltmillerCave_EventsOther;
}

/// Update callback of AltmillerCaveDraught's obstacles: drawn on the leader's
/// sprite priority.
static void
AltmillerCave_MatchLeaderPriority(union ObjectEventOrEffect *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  if (objectEvent != NULL) {
    objectEvent->priorityFlags = 0;
    objectEvent->sprite->image.priority =
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->sprite->image.priority;
  }
}

/// Redraws the three obstacles' cells where they stand.
static __inline__ void AltmillerCave_RedrawObstacleFootprints(void) {
  Map_RedrawObjectEventFootprint(LOCALID_FIRST_OBSTACLE);
  Map_RedrawObjectEventFootprint(LOCALID_FIRST_OBSTACLE + 1);
  Map_RedrawObjectEventFootprint(LOCALID_FIRST_OBSTACLE + 2);
}

/// Sets up the room being entered: an entry from outside the cave takes the
/// room's first warp destination; AltmillerCaveEntrance picks its darkness from
/// the warp destination; AltmillerCaveBabi's Babi is invisible until rescued;
/// AltmillerCaveSquall's obstacles and pillar and AltmillerCaveDraught's
/// obstacles, lights and rocks are restored. Returns 0.
s32 AltmillerCave_Initialize(void) {
  s8 *state;
  struct ObjectEvent *objectEvent;
  s32 i;

  if (gSaveBlock.warpDest == 0) {
    if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveBabi) {
      gSaveBlock.warpDest = ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE;
    }
    if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveVialChest) {
      gSaveBlock.warpDest = ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI;
    }
    if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveSquall) {
      gSaveBlock.warpDest =
          ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST;
    }
    if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveCookieChest) {
      gSaveBlock.warpDest =
          ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL;
    }
    if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveDraught) {
      gSaveBlock.warpDest =
          ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST;
    }
  }
  GameFlag_Set(FLAG_DARK);
  Script_ClearFlag(FLAG_DARKNESS_BY_STEPS);
  if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveEntrance) {
    if (gSaveBlock.warpDest == ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gMapLocalState + 4) = 0;
      }
      GameFlag_Set(FLAG_DARKNESS_BY_STEPS);
    }
    if (gSaveBlock.warpDest == ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gMapLocalState + 4) = 5;
      }
      GameFlag_Set(FLAG_DARKNESS_BY_STEPS);
    }
  }
  if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveBabi) {
    if (GameFlag_Get(FLAG_BABI_SAVED)) {
      Import_ObjectEventSetPosition(LOCALID_INVISIBLE_BABI, 0, 0);
    } else {
      objectEvent = ObjectEvent_GetById(LOCALID_INVISIBLE_BABI);
      objectEvent->sprite->image.priority = 1;
      objectEvent->sprite->flags = 2;
      objectEvent->sprite->rotation = 0x4000;
    }
  }
  // Restore AltmillerCaveSquall's moved obstacles and frozen puddle.
  if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveSquall) {
    GameFlag_Clear(FLAG_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    if (GameFlag_Get(FLAG_PUDDLE_FROZEN)) {
      Import_ObjectEventSetAnimation(LOCALID_SQUALL_ROOM_PUDDLE,
                                     PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    } else {
      ObjectEvent_GetById(LOCALID_SQUALL_ROOM_PUDDLE)->priorityFlags |=
          OBJ_EVENT_PRIORITY_UNDERFOOT;
    }
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_SQUALL_ROOM_PUDDLE),
                               0);
#if TBS_PATCH_1
    if (GameFlag_Get(FLAG_PILLAR_IN_PLACE)) {
      MapGrid_CopyCellAttributes(30, 20, 1, 1, 32, 20);
    }
#endif
  }
  // Rebuild the final rock puzzle from its saved phase and chosen light.
  if (gSaveBlock.mapId == (s32)&MapId_AltmillerCaveDraught) {
    GameFlag_Clear(FLAG_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    // UB: the callbacks are stored through an s32 view of update, which keeps
    // the warp destination read below the last store (as ->update, one store
    // moves: 1 of 403 instructions differ in tbs-en).
#ifdef UBFIX
    ObjectEvent_GetById(LOCALID_FIRST_OBSTACLE)->update =
        AltmillerCave_MatchLeaderPriority;
    ObjectEvent_GetById(LOCALID_FIRST_OBSTACLE + 1)->update =
        AltmillerCave_MatchLeaderPriority;
    ObjectEvent_GetById(LOCALID_FIRST_OBSTACLE + 2)->update =
        AltmillerCave_MatchLeaderPriority;
#else
    *(s32 *)&ObjectEvent_GetById(LOCALID_FIRST_OBSTACLE)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&ObjectEvent_GetById(LOCALID_FIRST_OBSTACLE + 1)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
    *(s32 *)&ObjectEvent_GetById(LOCALID_FIRST_OBSTACLE + 2)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
#endif
    // Coming up to the rocks (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE)
    // starts the puzzle over, with the red light first.
    if (gSaveBlock.warpDest == ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) {
      Dma_Fill32(0, gAltmillerCave_LightWheel, 12);
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        u8 *puzzleState = gMapLocalState;

        puzzleState[0] = 0;
        puzzleState[1] = PUZZLE_START;
        puzzleState[2] = LOCALID_RED_LIGHT - LOCALID_FIRST_LIGHT;
      }
    }
    state = (s8 *)&gMapLocalState[1];
    if (state[0] == PUZZLE_SOLVED) {
      MapGrid_CopyCellsLayered(41, 55, 3, 2, 30, 55);
      MapGrid_CopyCellAttributes(42, 8, 1, 1, 31, 8);
    }
    if (state[0] == PUZZLE_LIGHTS_SHOWN) {
      AltmillerCave_PlaceLights((state[1] * Q16_ONE) / 5 + 0x4000);
    }
    for (i = 0; i < LIGHT_COUNT; i++) {
      objectEvent = ObjectEvent_GetById(LOCALID_FIRST_LIGHT + i);
      objectEvent->motionFlags = 0;
      objectEvent->collisionFlags = 0;
      objectEvent->scaleX = Q16_ONE;
      objectEvent->scaleY = Q16_ONE;
      ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FIRST_LIGHT + i),
                                 0);
      Import_ObjectEventSetAnimation(LOCALID_FIRST_LIGHT + i, i + 1);
    }
    ObjectEvent_ConfigureSpriteColor(LOCALID_BLUE_LIGHT, COLOR_BLUE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_GREEN_LIGHT, COLOR_GREEN);
    ObjectEvent_ConfigureSpriteColor(LOCALID_WHITE_LIGHT, COLOR_WHITE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_YELLOW_LIGHT, COLOR_YELLOW);
    ObjectEvent_ConfigureSpriteColor(LOCALID_RED_LIGHT, COLOR_RED);
    ObjectEvent_ConfigureSpriteColor(LOCALID_BLUE_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_GREEN_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_WHITE_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_YELLOW_ROCK, COLOR_NONE);
    ObjectEvent_ConfigureSpriteColor(LOCALID_RED_ROCK, COLOR_NONE);
    ObjectEvent_GetById(LOCALID_BLUE_ROCK)->sprite->image.priority = 3;
    ObjectEvent_GetById(LOCALID_RED_ROCK)->sprite->image.priority = 3;
    ObjectEvent_GetById(LOCALID_BLUE_ROCK)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_GetById(LOCALID_RED_ROCK)->priorityFlags =
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_BLUE_ROCK), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_RED_ROCK), 0);
  }
  if (GameFlag_Get(FLAG_DARK)) {
    AltmillerCave_InitializeDarkness();
  } else {
    gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 4);
    gFieldWork->transitionFrames = 24;
  }
  return 0;
}

/// Update callback of a piece of the burst wall: it flies off to a random
/// point 120 pixels away, with a crash every fourth frame as they start.
static void
AltmillerCave_UpdateWallFragment(struct FieldEffectSprite *fragment) {
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
    if ((gFrameTick & 3) == 0)
      Sound_PlaySong(SE_HEAVY_IMPACT);
  } else if (currentPhase == 1) {
    if (FieldEffectSprite_HasPendingTarget(fragment) == 0) {
      s32 nextPhase = *phase;
      nextPhase--;
      *phase = nextPhase;
    }
  } else if (currentPhase == 2) {
    if (FieldEffectSprite_HasPendingTarget(fragment) == 0)
      FieldEffect_ClearOwnedSlot(fragment);
  }
}

/// The wall bursts: 24 pieces fly out from the ring's center, the wall's
/// cells open to the passage, and the shaking calms.
static void AltmillerCave_ReleaseWallBurst(void) {
  struct FieldEffectWork *effectWork;
  struct Vector3 position;
  struct FieldEffectSprite *fragment;
  s32 fragmentIndex;

  FieldEffect_InitializeSlots();
  effectWork = (struct FieldEffectWork *)gFieldEffectWork;
  Shop_SaveAndTintFieldPalette(0x202108);
  position.x = PIXELS(504);
  position.y = PIXELS(24);
  position.z = PIXELS(144);
  Camera_WorldToScreen(&position);
  fragment = effectWork->sprites;
  for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
    s32 scale;

    FieldEffectSprite_Initialize(fragment, SPRITE_PARTICLE, position.x,
                                 position.z);
    FieldEffectSprite_SetCallback(fragment, AltmillerCave_UpdateWallFragment);
    FieldEffectSprite_SetObjectMode(fragment, 7);
    Sprite_SetAnimColorModes(fragment->object,
                             (u32)(Import_RandomNext() * 7) >> 16);
    scale = Math_DivideUnsigned(Import_RandomNext(), 3) + 3 * Q16_ONE / 2;
    fragment->scaleY = scale;
    fragment->scaleX = scale;
    WaitFrames(1);
    fragment++;
  }
  WaitFrames(80);
  MapGrid_CopyCellsLayered(41, 55, 3, 2, 30, 55);
  MapGrid_CopyCellAttributes(42, 8, 1, 1, 31, 8);
  WaitFrames(50);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  WaitFrames(30);
  {
    s32 mode = 2;
    // Walk the state bytes within the complete field effect sprite array.
    u8 *stateCursor =
        (u8 *)&effectWork->sprites + offsetof(struct FieldEffectSprite, state);

    for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
      if (((s8 *)stateCursor)[offsetof(struct FieldEffectSprite, active) -
                              offsetof(struct FieldEffectSprite, state)] != 0) {
        *stateCursor = mode;
      }
      stateCursor += sizeof(struct FieldEffectSprite);
    }
  }
  MapGrid_WaitForShakeToSettle();
  Shop_RestoreFieldPalette();
  FieldEffect_ClearActiveSlotsAndScheduleUpdates();
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

struct LightWheel *gAltmillerCave_LightWheel = &gAltmillerCave_LightWheelState;

struct MapWarpDest gAltmillerCave_WarpDests[] = {
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
    {MAP_TABLE_END},
};

u32 gAltmillerCave_Warps[] = {
    MAP_WARPS(AltmillerCaveEntrance),
    MAP_WARP(ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP, WorldMap,
             WORLD_MAP_FROM_ALTMILLER_CAVE_ENTRANCE),
    MAP_WARP(ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER, AltmillerCaveEntrance,
             ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_FROM_PASSAGE),
    MAP_WARP(ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE,
             AltmillerCaveEntrance, ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER),
    MAP_WARP(ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI, AltmillerCaveBabi,
             ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE),
    MAP_WARPS(AltmillerCaveBabi),
    MAP_WARP(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE,
             AltmillerCaveEntrance,
             ALTMILLER_CAVE_ENTRANCE_FROM_ALTMILLER_CAVE_BABI),
    MAP_WARP(ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST,
             AltmillerCaveVialChest,
             ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI),
    MAP_WARPS(AltmillerCaveVialChest),
    MAP_WARP(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI,
             AltmillerCaveBabi,
             ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_VIAL_CHEST),
    MAP_WARP(ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL,
             AltmillerCaveSquall,
             ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST),
    MAP_WARPS(AltmillerCaveSquall),
    MAP_WARP(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST,
             AltmillerCaveVialChest,
             ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
    MAP_WARP(ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST,
             AltmillerCaveCookieChest,
             ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL),
    MAP_WARPS(AltmillerCaveCookieChest),
    MAP_WARP(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL,
             AltmillerCaveSquall,
             ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
    MAP_WARP(ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT,
             AltmillerCaveDraught,
             ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST),
    MAP_WARPS(AltmillerCaveDraught),
    MAP_WARP(ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST,
             AltmillerCaveCookieChest,
             ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_DRAUGHT),
    MAP_WARP(ALTMILLER_CAVE_DRAUGHT_TO_ROCKS, AltmillerCaveDraught,
             ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE),
    MAP_WARP(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE, AltmillerCaveDraught,
             ALTMILLER_CAVE_DRAUGHT_FROM_ROCKS),
    MAP_WARP(ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST, AltmillerCaveDraught,
             ALTMILLER_CAVE_DRAUGHT_CHEST_FROM_ROCKS),
    MAP_WARP(ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS, AltmillerCaveDraught,
             ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_CHEST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gAltmillerCave_ObjectEventTemplatesOther[] = {
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltmillerCave_BabiObjectEventTemplates[] = {
    {LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACING_PARTY, 0},
    {LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACING_PARTY, 0},
    {LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACING_PARTY, 0},
    {SPRITE_INVISIBLE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(232),
     0, PIXELS(160), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -PIXELS(64), 0,
     -PIXELS(64), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     -PIXELS(64), 0, -PIXELS(64), FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
     -PIXELS(64), 0, -PIXELS(64), FACING_NORTH, TALK_FACING_KEEP, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltmillerCave_SquallObjectEventTemplates[] = {
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
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltmillerCave_DraughtObjectEventTemplates[] = {
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
    {MAP_TABLE_END},
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
struct MapEvent gAltmillerCave_EventsOther[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_WORLD_MAP},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_TORCH_CHAMBER},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TORCH_CHAMBER_TO_PASSAGE},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, ALTMILLER_CAVE_ENTRANCE_TO_ALTMILLER_CAVE_BABI},
    {MAP_EVENT_COORD, 60, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel0)},
    {MAP_EVENT_COORD, 61, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel1)},
    {MAP_EVENT_COORD, 62, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel2)},
    {MAP_EVENT_COORD, 63, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel3)},
    {MAP_EVENT_COORD, 64, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel4)},
    {MAP_EVENT_COORD, 65, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel5)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_StartReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartRetreat)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltmillerCave_BabiEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 10, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_ENTRANCE},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, ALTMILLER_CAVE_BABI_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {MAP_EVENT_TALK, LOCALID_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI, EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {MAP_EVENT_TALK, LOCALID_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, EVENT_SCRIPT(AltmillerCave_RepeatRockOrder)},
    {MAP_EVENT_TALK, LOCALID_INVISIBLE_BABI, FLAG_BABI_SAVED, EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {MAP_EVENT_COORD, 19, FLAG_BABI_CALLED_PARTY_BACK, EVENT_SCRIPT(AltmillerCave_BabiCallsPartyBack)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), MAP_EVENT_TRIGGER_OWN_EFFECT | LOCALID_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI,
        EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), MAP_EVENT_TRIGGER_OWN_EFFECT | LOCALID_INVISIBLE_BABI, FLAG_BABI_SAVED,
        EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartRetreat)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTMILLER_CAVE_MIMIC_BEATEN, FIND_MIMIC | 5},
    {MAP_EVENTS_END},
};

struct MapEvent gAltmillerCave_VialChestEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 20, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 21, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartRetreat)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTMILLER_CAVE_VIAL, FIND_ITEM(ITEM_VIAL)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltmillerCave_SquallEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 30, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 31, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 72, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedObjectEvent)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 73, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedObjectEvent)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 74, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedObjectEvent)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 75, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedObjectEvent)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 71, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_MoveStagedObjectEventToClearPosition)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 77, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_AdvanceObstacleAndCheckPassage)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SQUALL_ROOM_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_UpdateMovedObstacle)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_SQUALL_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_CheckFrostTargetCollision)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SQUALL_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_FreezePuddle)},
    {MAP_EVENT_TALK, LOCALID_SQUALL, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartDjinnBattle)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltmillerCave_CookieChestEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 40, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 41, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartRetreat)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTMILLER_CAVE_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {MAP_EVENTS_END},
};
// clang-format on

struct MapEvent gAltmillerCave_DraughtEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 50, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {MAP_EVENT_WARP, 51, CONDITION_ALWAYS, ALTMILLER_CAVE_DRAUGHT_TO_ROCKS},
    {MAP_EVENT_WARP, 52, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_PILLAR_CAVE},
    {MAP_EVENT_WARP, 53, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_ROCKS_TO_CHEST},
    {MAP_EVENT_WARP, 54, CONDITION_ALWAYS,
     ALTMILLER_CAVE_DRAUGHT_CHEST_TO_ROCKS},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_MoveStagedObjectEventToClearPosition)},
    {MAP_EVENT_TALK, LOCALID_BLUE_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {MAP_EVENT_TALK, LOCALID_GREEN_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {MAP_EVENT_TALK, LOCALID_WHITE_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {MAP_EVENT_TALK, LOCALID_YELLOW_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {MAP_EVENT_TALK, LOCALID_RED_ROCK, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_TurnRock)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT,
     FIND_ITEM(ITEM_MYSTIC_DRAUGHT)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0,
     CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {MAP_EVENTS_END},
};

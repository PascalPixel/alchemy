/// @file
/// Altmiller Cave, where the party finds Babi while Tolbi searches for him
/// during Colosso (babi_palace.c). Its six scenes share this overlay and the
/// area name Altmiller Cave:
///
/// - AltmillerCaveEntrance (SceneId_AltmillerCaveEntrance): the dark passage in
///   from the World Map and the chamber beyond it.
/// - AltmillerCaveBabi (SceneId_AltmillerCaveBabi): the invisible Babi, who
///   asks for his draught (AltmillerCave_MeetInvisibleBabi) and drinks it
///   (AltmillerCave_RestoreBabiAndInviteParty), and a chest that is a Mimic.
/// - AltmillerCaveVialChest (SceneId_AltmillerCaveVialChest) and
///   AltmillerCaveCookieChest (SceneId_AltmillerCaveCookieChest): a chest each.
/// - AltmillerCaveSquall (SceneId_AltmillerCaveSquall): Squall, a Jupiter
///   Djinni, three fallen columns to push, a stone pillar for Move and a puddle
///   for Frost.
/// - AltmillerCaveDraught (SceneId_AltmillerCaveDraught): three more fallen
///   columns, the rock puzzle (AltmillerCave_TurnRock) and the chest that holds
///   the Mystic Draught behind the wall the puzzle opens.
///
/// The rooms share one entrance table and one exit table, and the other getters
/// choose by gMainRoutineWork.next. AltmillerCave_Initialize sends an entry
/// from outside the cave (entrance 0) to the room's first entrance, darkens the
/// dark rooms and puts back what the party has done.
///
/// Saving Babi (FLAG_BABI_SAVED) is the story flag the rest of Tolbi waits on.
/// The overlay's own state is in scene temp flags and gSceneLocalState, so it
/// starts over on every ordinary arrival.
///
/// Every room but the two lit ones is dark (FLAG_ALTMILLER_CAVE_DARK). BG1 is
/// blended over the rest (ALTMILLER_CAVE_DARKNESS_BLEND). The light around the
/// party is a display window, an ellipse around the leader inside which the
/// layers are drawn differently (AltmillerCave_InitializeDarkness). In an
/// evenly dark room BG1 covers everything outside it and is only blended inside
/// it.
///
/// AltmillerCaveEntrance darkens by steps instead
/// (FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS). Step triggers 60 to 65 each set a
/// level, kept in struct AltmillerCaveState, from the lightest at the mouth to
/// the darkest by the inner chamber. This room blends outside the light, while
/// BG1 is left out inside it.
///
/// AltmillerCave_ApplyFadeBlend gives BG1 no weight and the layers under it 16
/// sixteenths at level 0, two fewer at each level after, down to 6 at level 5.
/// Where one level meets the next, a cell of each lies side by side. So the
/// level is that of the cell the leader last stepped on, in either direction.
///
/// Reveal lights a dark room while it lasts and shows the rocks' colours
/// (AltmillerCave_StartReveal, AltmillerCave_EndReveal).
///
/// The rock puzzle keeps the rounds matched, its state and the light at the top
/// in struct AltmillerCaveState.
///
/// Turning the leftmost rock and then the rightmost brings out the ring of five
/// lights. The ring spins (AltmillerCave_SpinLights, a task) and stops with one
/// light at the top. Turning that light's rock spins it on to a different
/// light.
///
/// A wrong rock puts the lights away. The third match bursts the wall
/// (AltmillerCave_ReleaseWallBurst). Coming up to the rocks from the pillar
/// cave (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) starts the puzzle over.

#include "main_routine_work.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/altmiller_cave.h"
#include "shared/walk_into_leader.h"
#include "module/system/main_routine.h"
#include "io_write.h"

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "djinn_ids.h"
#include "pushed_actor.h"
#include "actor_builtin_scripts.h"
#include "dma.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "display_transition.h"
#include "item_ids.h"
#include "actor_script.h"
#include "field_psynergy_work.h"
#include "flags.h"
#include "sound_driver_symbols.h"

/// AltmillerCaveBabi's actors: Babi while invisible and once seen, and
/// the two men from Tolbi who come looking for him.
enum {
  /// Babi's second sheet, SPRITE_INVISIBLE_BABI. It is the old man with long
  /// pink hair and a cane, standing unseen in the cave.
  ACTOR_INVISIBLE_BABI = 8,
  /// SPRITE_BABI, the same old man once seen.
  ACTOR_BABI = 9,
  /// Their sprite is a helmeted soldier carrying a torch
  /// (SPRITE_SOLDIER_WITH_TORCH).
  ACTOR_FIRST_SOLDIER = 10,
  ACTOR_SECOND_SOLDIER = 11
};

/// AltmillerCaveSquall and AltmillerCaveDraught place first the three fallen
/// columns the party pushes. AltmillerCaveSquall then places a puddle, a pillar
/// for Move and Squall, a Jupiter Djinni.
enum {
  ACTOR_FIRST_OBSTACLE = 8,
  ACTOR_SQUALL_ROOM_PUDDLE = 11,
  ACTOR_SQUALL_ROOM_PILLAR = 12,
  ACTOR_SQUALL = 13
};

/// AltmillerCaveDraught: the five coloured lights and the five rocks, each in
/// Babi's order, the rocks left to right.
enum {
  ACTOR_FIRST_LIGHT = 11,
  ACTOR_BLUE_LIGHT = ACTOR_FIRST_LIGHT,
  ACTOR_GREEN_LIGHT,
  ACTOR_WHITE_LIGHT,
  ACTOR_YELLOW_LIGHT,
  ACTOR_RED_LIGHT,
  ACTOR_FIRST_ROCK,
  ACTOR_BLUE_ROCK = ACTOR_FIRST_ROCK,
  ACTOR_GREEN_ROCK,
  ACTOR_WHITE_ROCK,
  ACTOR_YELLOW_ROCK,
  ACTOR_RED_ROCK
};

enum { LIGHT_COUNT = 5 };

/// The palette a light or rock is drawn in, as its child value. They are the
/// five colors in the same order, and none for a rock while Reveal is off.
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
  /// The cave is dark. It is set on entry everywhere but AltmillerCaveSquall
  /// and AltmillerCaveDraught.
  FLAG_ALTMILLER_CAVE_DARK = FLAG_SCENE_TEMP_00,
  /// AltmillerCaveEntrance darkens by steps as the party walks in.
  FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS = FLAG_SCENE_TEMP_01,
  /// Frost has frozen AltmillerCaveSquall's puddle.
  FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN = FLAG_SCENE_TEMP_11,
  /// AltmillerCaveSquall's pillar stands where Move opens the passage.
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

static void AltmillerCave_ReleaseWallBurst(void);

/// The ring of five lights above AltmillerCaveDraught's rocks as it spins to
/// its next light. Angles are 16-bit turns, and the light at the top is the
/// chosen one.
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

/// The cave's own state, bytes of gSceneLocalState: AltmillerCaveDraught's
/// rock puzzle and AltmillerCaveEntrance's darkness.
enum {
  CAVE_ROUNDS_MATCHED = 0, ///< the rounds of lights matched, up to three
  CAVE_PUZZLE = 1,         ///< the rock puzzle's state
  CAVE_CHOSEN_LIGHT = 2,   ///< the light the ring stops with at the top, 0 to 4
  CAVE_DARKNESS = 4        ///< AltmillerCaveEntrance's darkness level, 0 to 5
};

/// A byte of the cave's state; the chosen light and the darkness are signed.
#define CAVE_STATE(byte) (((u8 *)gSceneLocalState)[byte])
#define CAVE_SIGNED_STATE(byte) (((s8 *)gSceneLocalState)[byte])

/// The rock puzzle's two bytes, from CAVE_PUZZLE.
struct RockPuzzle {
  u8 state;
  s8 chosenLight;
};

#define ROCK_PUZZLE ((struct RockPuzzle *)&CAVE_STATE(CAVE_PUZZLE))

/// The puzzle's states. The leftmost rock is turned, then the rightmost, which
/// shows the lights. After three lights matched the wall opens.
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
  s16 sceneId = gMainRoutineWork.next;

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

/// Darkens AltmillerCaveEntrance to its current level. The brighter blend is at
/// level 0, and each level is one step darker.
static void AltmillerCave_ApplyFadeBlend(void) {
  s8 level = CAVE_SIGNED_STATE(CAVE_DARKNESS);

  Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
  if (level == 0) {
    Io_Write16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
  } else if (level == 1) {
    Io_Write16(BLEND_ALPHA(0, 14), &REG_BLDALPHA);
  } else if (level == 2) {
    Io_Write16(BLEND_ALPHA(0, 12), &REG_BLDALPHA);
  } else if (level == 3) {
    Io_Write16(BLEND_ALPHA(0, 10), &REG_BLDALPHA);
  } else if (level == 4) {
    Io_Write16(BLEND_ALPHA(0, 8), &REG_BLDALPHA);
  } else {
    Io_Write16(BLEND_ALPHA(0, 6), &REG_BLDALPHA);
  }
}

/// Keeps AltmillerCaveEntrance's darkness level, and shows it unless Reveal is
/// lighting the room.
static void AltmillerCave_SetFadeLevel(s32 level) {
  struct FieldWork *work = gFieldWork;

  CAVE_STATE(CAVE_DARKNESS) = level;
  if (work->revealActive == 0) {
    AltmillerCave_ApplyFadeBlend();
  }
}

/// Crossing the first darkness boundary sets the cave's lightest level.
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

/// Darkens the cave on entry. AltmillerCaveEntrance darkens by its levels, the
/// other dark rooms evenly.
static void AltmillerCave_InitializeDarkness(void) {
  // The heap slots from the field's on.
  void **slots = (void **)&gFieldWork;
  struct FieldWork *work = slots[0];
  struct ScreenTransitionState *state;

  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 24;
  wait(1);
  ScreenTransition_InitializeBattleEffectState(WINDOW_TRANSITION_LEADER_LOW);
  // UB: the transition's slot is read past gFieldWork, a single pointer.
#ifdef UBFIX
  state = gScreenTransitionWork;
#else
  state = slots[HEAP_SLOT_SCREEN_TRANSITION - HEAP_SLOT_FIELD];
#endif
  state->value = ALTMILLER_CAVE_LIGHT_STEPS;
  if (GameFlag_Get(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS) != 0) {
    state->mask = ALTMILLER_CAVE_STEPS_LIGHT_WININ;
    state->active = ALTMILLER_CAVE_STEPS_LIGHT_WINOUT;
    AltmillerCave_ApplyFadeBlend();
    return;
  }
  state->mask = ALTMILLER_CAVE_DARK_LIGHT_WININ;
  state->active = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;
  Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
  Io_Write16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
}

/// Reveal's light starts. AltmillerCaveEntrance is lit and
/// AltmillerCaveDraught's rocks show their colors.
static void AltmillerCave_StartReveal(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveEntrance) {
    Io_Write16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
  }
  if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveDraught) {
    Actor_SetSpriteColorById(ACTOR_BLUE_ROCK, COLOR_BLUE);
    Actor_SetSpriteColorById(ACTOR_GREEN_ROCK, COLOR_GREEN);
    Actor_SetSpriteColorById(ACTOR_WHITE_ROCK, COLOR_WHITE);
    Actor_SetSpriteColorById(ACTOR_YELLOW_ROCK, COLOR_YELLOW);
    Actor_SetSpriteColorById(ACTOR_RED_ROCK, COLOR_RED);
  }
}

/// Reveal's light ends. The cave darkens again and AltmillerCaveDraught's rocks
/// lose their colors.
static void AltmillerCave_EndReveal(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;

  if (GameFlag_Get(FLAG_ALTMILLER_CAVE_DARK) != 0) {
    AltmillerCave_InitializeDarkness();
    effectWork->childOption = 1;
  }
  if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveDraught) {
    Actor_SetSpriteColorById(ACTOR_BLUE_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_GREEN_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_WHITE_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_YELLOW_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_RED_ROCK, COLOR_NONE);
  }
}

/// Retreat's first phase. The Psynergy's first actor faces south, and it is
/// copied into a new actor for the Psynergy to use.
static void AltmillerCave_StartRetreat(void) {
  struct FieldEffectWork *effectWork;
  struct Actor *original;
  struct Actor *replacement;

  effectWork = (struct FieldEffectWork *)gFieldEffectWork;
  original = effectWork->mainActor;
  Actor_FaceDirection(effectWork->firstActorId, FACING_SOUTH, 0);
  Actor_SetSpriteColorMode(original, 0);
  Event_Wait(20);
  replacement = FieldActor_Create(0, original->x.fixed, original->y.fixed,
                                  original->z.fixed);
  if (replacement != NULL) {
    Dma_Set(original, replacement,
            (DMA_ENABLE | DMA_32BIT) << 16 | sizeof(struct Actor) / 4,
            REG_DMA3);
    original->update = 0;
    effectWork->mainActor = replacement;
    original->spriteStorageKind = 0;
  }
}

/// Runs after Move on AltmillerCaveSquall's pillar.
static void AltmillerCave_UpdateMovedObstacle(void) {
  struct Actor *actor = Actor_Get(ACTOR_SQUALL_ROOM_PILLAR);

  if (FIELD_CELL(actor->x.fixed) == 30 && FIELD_CELL(actor->z.fixed) == 20) {
    actor->motionFlags = ACTOR_FALLS;
    actor->groundHeight = 0;
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
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

/// Frost's first phase on AltmillerCaveSquall's puddle notes whether its place
/// is blocked, which keeps it from freezing.
static void AltmillerCave_CheckFrostTargetCollision(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;
  struct Actor *target = Actor_Get(ACTOR_SQUALL_ROOM_PUDDLE);
  struct FieldPosition position;

  position.x = target->x.fixed;
  position.y = target->y.fixed;
  position.z = target->z.fixed;

  if (FieldActor_CheckMovementCollision(target, &position) > 0) {
    effectWork->childMode = 1;
  }
}

/// Frost on AltmillerCaveSquall's puddle, unless blocked, freezes it. Its cell
/// takes the frozen attributes kept at (76, 16).
static void AltmillerCave_FreezePuddle(void) {
  struct FieldEffectWork *effectWork =
      (struct FieldEffectWork *)gFieldEffectWork;
  struct Actor *puddle = Actor_Get(ACTOR_SQUALL_ROOM_PUDDLE);
  s32 blocked;

  blocked = effectWork->childMode;
  if (blocked == 0) {
    MapGrid_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    if (puddle != NULL) {
      s32 motionFlags = 2;
      puddle->motionFlags = motionFlags;
      puddle->priorityFlags = blocked;
    }
    GameFlag_Set(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN);
  }
}

/// Walking into AltmillerCaveSquall's puddle while it is not frozen makes the
/// leader hop 32 pixels over it, if the landing is clear.
static void AltmillerCave_HopSelectedActor(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
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

    Vector_AddPolarOffset(px(32), angle, destination.coordinates);
  }
  if (FieldActor_CheckMovementCollision(actor, &destination.position) == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);
    actor->speedLimit = px(3);
    actor->acceleration = px(2);
    actor->velocityY = px(4);
    actor->motionFlags &= 126;
    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER, components[0].part.pixel,
                        components[2].part.pixel);
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->motionFlags = motionFlags;
    Event_End();
  }
}

/// Walking into one of the three fallen columns pushes it, if there is room.
static void AltmillerCave_PushFallenColumn(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
  }
  Event_End();
}

/// Update routine of the light at the top of the stopped ring. It pulses
/// through four sizes.
static void AltmillerCave_PulseLight(struct Actor *light) {
  s32 scale = gAltmillerCave_PulseScales[(light->timer >> 2) & 3];

  light->scaleX = scale;
  light->scaleY = scale;
  light->timer++;
  light->timer &= 15;
}

/// Puts a light on the ring, radius pixels from its center above the rocks, at
/// the given angle.
static void AltmillerCave_PlaceLight(s32 actorId, s32 radius, s32 angle) {
  s32 centreX = px(504);
  s32 centreHeight = px(24);
  s32 depth = px(144);
  struct Actor *light = Actor_Get(actorId);
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
    AltmillerCave_PlaceLight(ACTOR_FIRST_LIGHT + i, px(24), angle);
    angle -= TRIG_TURN / 5;
    i++;
  } while (i < LIGHT_COUNT);
}

/// Task that spins the ring of lights.
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
    s32 chosenLight = CAVE_SIGNED_STATE(CAVE_CHOSEN_LIGHT);
    s32 chosenAngle = Math_Divide(chosenLight * Q16_ONE, 5);

    if ((u32)(((wheel->angle - chosenAngle) << 16) +
              ((767 - TRIG_TURN / 4) << 16)) <= (2 * 767) << 16) {
      wheel->angle = chosenAngle + TRIG_TURN / 4;
      wheel->phase = WHEEL_STOPPED;
      wheel->speed = 0;
      Actor_Get(ACTOR_FIRST_LIGHT + chosenLight)->update =
          AltmillerCave_PulseLight;
    }
  } else if (phase == WHEEL_STOPPED) {
    moving = 0;
  }
  if (moving != 0) {
    gAltmillerCave_LightWheel->angle += gAltmillerCave_LightWheel->speed;
    AltmillerCave_PlaceLights(gAltmillerCave_LightWheel->angle);
    gAltmillerCave_LightWheel->travel += gAltmillerCave_LightWheel->speed;
    if (gAltmillerCave_LightWheel->travel > TRIG_TURN * 3 / 16) {
      gAltmillerCave_LightWheel->travel = 0;
      Sound_Play(SE_CLICK);
    }
  }
  gAltmillerCave_LightWheel->frames++;
}

/// Talking to one of AltmillerCaveDraught's rocks rotates it. "Isaac rotated
/// the rock."
static void AltmillerCave_TurnRock(s32 rock) {
  u8 *puzzle;
  struct LightWheel *wheel;
  s32 correctRock = 0;
  s32 i;
  struct Actor *actor;
  s32 scale;
  s8 state;
  s32 chosenLight;
  u8 previousLight;
  s32 radius;
  u16 angle;

  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgAltmillerRotatedRock);
  // "Isaac rotated the rock."
  Event_ShowMessage(ACTOR_FIRST_ROCK, 0);
  Event_End();
  for (i = 0; i < LIGHT_COUNT; i++) {
    actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
    actor->update = NULL;
    actor->scaleX = Q16_ONE;
    actor->scaleY = Q16_ONE;
  }
  state = ROCK_PUZZLE->state;
  previousLight = ROCK_PUZZLE->chosenLight;
  wheel = gAltmillerCave_LightWheel;
  chosenLight = ROCK_PUZZLE->chosenLight;
  wheel->angle = (chosenLight * Q16_ONE) / 5 + TRIG_TURN / 4;
  // The leftmost rock, then the rightmost, before the rounds of lights.
  if (state == PUZZLE_START) {
    if (rock == ACTOR_BLUE_ROCK) {
      state = PUZZLE_LEFT_ROCK_TURNED;
      Sound_Play(SE_CHIME);
    } else {
      Sound_Play(SE_MENU_ERROR);
    }
    CAVE_STATE(CAVE_ROUNDS_MATCHED) = 0;
  } else if (state == PUZZLE_LEFT_ROCK_TURNED) {
    if (rock == ACTOR_BLUE_ROCK) {
      Sound_Play(SE_CHIME);
    } else if (rock == ACTOR_RED_ROCK) {
      // The lights grow in one by one, each with SE_LIGHT_APPEAR.
      state = PUZZLE_LIGHTS_SHOWN;
      Sound_Play(SE_CHIME);
      wait(30);
      angle = gAltmillerCave_LightWheel->angle;
      for (i = 0; i < LIGHT_COUNT; i++) {
        AltmillerCave_PlaceLight(ACTOR_FIRST_LIGHT + i, px(24), angle);
        Sound_Play(SE_LIGHT_APPEAR);
        actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
        actor->scaleX = 0;
        scale = 2 * Q16_ONE / 5;
        do {
          actor->scaleY = scale;
          actor->scaleX = scale;
          wait(1);
          scale += 3 * Q16_ONE / 64;
        } while (actor->scaleX < Q16_ONE);
        angle -= TRIG_TURN / 5;
      }
      wait(30);
      correctRock = 1;
    } else {
      Sound_Play(SE_MENU_ERROR);
      state = PUZZLE_START;
    }
  } else if (state == PUZZLE_LIGHTS_SHOWN) {
    if (rock != ACTOR_FIRST_ROCK + chosenLight) {
      // The wrong rock: the lights shrink away.
      state = PUZZLE_START;
      Sound_Play(SE_MENU_ERROR);
      wait(30);
      for (i = 0; i < LIGHT_COUNT; i++) {
        actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
        Sound_Play(SE_LIGHT_APPEAR);
        for (scale = actor->scaleX; actor->scaleX > 2 * Q16_ONE / 5;
             scale -= 3 * Q16_ONE / 64) {
          actor->scaleY = scale;
          actor->scaleX = scale;
          wait(1);
        }
        Actor_SetPosition(ACTOR_FIRST_LIGHT + i, 0, 0);
      }
    } else {
      Sound_Play(SE_CHIME);
      correctRock = 1;
      wait(30);
    }
  }
  puzzle = &CAVE_STATE(CAVE_PUZZLE);
  *puzzle = state;
  // A light matched: choose a different light and spin the ring to it.
  if (correctRock) {
    u8 rounds = ++puzzle[CAVE_ROUNDS_MATCHED - CAVE_PUZZLE];

    // One of the four others: 1 to 4 lights on from the last.
    puzzle[CAVE_CHOSEN_LIGHT - CAVE_PUZZLE] =
        ((s8)((Random_Below(4)) + previousLight + 1) + 5) % 5;
    gAltmillerCave_LightWheel->phase = WHEEL_SPEEDING_UP;
    gAltmillerCave_LightWheel->frames = 0;
    gAltmillerCave_LightWheel->speed = TRIG_TURN / 128;
    gAltmillerCave_LightWheel->travel = TRIG_TURN * 3 / 16;
    Task_AddFrame(AltmillerCave_SpinLights, TASK_PASS_FRAME);
    if (rounds <= 2) {
      while (gAltmillerCave_LightWheel->phase != WHEEL_STOPPED) {
        wait(1);
      }
      wait(10);
      Sound_Play(SE_CHIME);
    } else {
      // The third light matched: the ground shakes, the ring spirals in to
      // nothing, and the wall bursts.
      *puzzle = PUZZLE_SOLVED;
      while (gAltmillerCave_LightWheel->phase != WHEEL_SLOWING) {
        wait(1);
      }
      gAltmillerCave_LightWheel->phase = WHEEL_SLOWING;
      gAltmillerCave_LightWheel->frames = 0;
      MapGrid_SetShake(2, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(4, 4, 1);
      gAltmillerCave_LightWheel->phase = WHEEL_STOPPED;
      Sound_Play(SE_WARP);
      radius = px(24);
      angle = gAltmillerCave_LightWheel->angle;
      do {
        for (i = 0; i < LIGHT_COUNT; i++) {
          actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
          actor->scaleX -= 16;
          actor->scaleY -= 16;
          AltmillerCave_PlaceLight(ACTOR_FIRST_LIGHT + i, radius, angle);
          angle -= TRIG_TURN / 5;
        }
        radius -= px(1) / 5;
        angle += TRIG_TURN * 3 / 64;
        wait(1);
      } while (radius > 0);
      for (i = 0; i < LIGHT_COUNT; i++) {
        Actor_SetPosition(ACTOR_FIRST_LIGHT + i, 0, 0);
      }
      AltmillerCave_ReleaseWallBurst();
      Sound_Play(MUS_PUZZLE_SOLVED);
    }
    Task_Remove(AltmillerCave_SpinLights);
  }
}

/// The party turns north, toward Babi.
static inline void AltmillerCave_FacePartyNorth(void) {
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
}

/// Babi drinks the Mystic Draught and appears, startling the party.
static void AltmillerCave_RestoreBabiAndInviteParty(void) {
  struct ScreenTransitionState *state;
  struct Actor *actor;
  struct Actor *leader;
  s32 i;
  s32 eva;
  s32 evb;

  GameFlag_Set(FLAG_BABI_SAVED);
  PartyInventory_Discard(ITEM_MYSTIC_DRAUGHT);
  Event_Start();
  Field_EndBusyAndReveal();
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Event_SetMessageId(MsgAltmillerCameBackPleaseDraughtQuickly);
  // "You... You came back... Please, the draught- quickly."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 160);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(50);
  Event_Wait(10);
  FieldCamera_MoveTo(264, CAMERA_KEEP, 200, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 208);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(20);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Oh, thank you! You've saved my life. Squeak... Glug glug glug..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
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
  Sound_Play(SOUND_FADE_OUT_BGM);
  actor->sprite->rotation = 0;
  Actor_Jump(ACTOR_INVISIBLE_BABI, 10, 70);
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
    Actor_SetPosition(ACTOR_BABI, x, z);
  }
  actor->sprite->flags = 0;
  Sound_Play(MUS_BABI);
  Actor_SetSpeed(ACTOR_BABI, 0.8);
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
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
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
    // "I would have revealed myself sooner, but I could not even..."
    Event_ShowMessage(ACTOR_BABI, 0);
  } else {
    Event_SetMessageId(MsgAltmillerMustTraveledIndeedIfDidnt);
    // "You must be well traveled indeed if that didn't shock you."
    Event_ShowMessage(ACTOR_BABI, 0);
  }
  Event_SetMessageId(MsgAltmillerHowDidBecomeParalyzedWhile);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "How did you become paralyzed while you were invisible?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "And that ability to conceal yourself... Are you an Adept?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "An Adept? What are you talking about?"
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
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
  Actor_SetSpeed(ACTOR_IVAN, 1);
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
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
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
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_WalkByAndWait(ACTOR_IVAN, 0, 48);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
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
    // "That's right! Kraden said Alchemy could thwart death itself!"
    Event_ShowMessage(ACTOR_GARET, 0);
  } else {
    Event_SetMessageId(MsgAltmillerForgotKradenSaidAlchemyCould);
    // "You forgot? Kraden said Alchemy could be used to thwart death."
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_SetMessageId(MsgAltmillerWhatWasThat);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Event_Wait(10);
  // "What was that?"
  Event_ShowMessage(ACTOR_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Event_Wait(30);
  // "Did you say you know Kraden?"
  Event_OpenMessage(ACTOR_BABI, 0);
  Event_SetMessageId(MsgAltmillerBabi);
  Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
  // The cave brightens as two men from Tolbi arrive: "Babi!" The light
  // widens to fill the screen and BG1 fades out.
  state = gScreenTransitionWork;
  state->value = ALTMILLER_CAVE_LIGHT_FULL;
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
    REG_BLDALPHA = BLEND_ALPHA(evb, eva);
    wait(7);
  }
  {
    u16 *slot = (u16 *)&state->active;
    s32 value = ALTMILLER_CAVE_BRIGHTENING_WINOUT;

    *slot = value;
  }
  Actor_SetPosition(ACTOR_FIRST_SOLDIER, 280, 304);
  Actor_SetPosition(ACTOR_SECOND_SOLDIER, 296, 304);
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
  FieldCamera_WaitForMove();
  Event_Wait(70);
  Actor_ShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessage(ACTOR_SECOND_SOLDIER, 0);
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_FIRST_SOLDIER);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_FIRST_SOLDIER);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_FIRST_SOLDIER);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_FIRST_SOLDIER);
  // The two men greet Babi; he invites the party to the finals.
  Actor_SetSpeed(ACTOR_FIRST_SOLDIER, 1.4);
  Actor_SetSpeed(ACTOR_SECOND_SOLDIER, 1.4);
  Actor_SetScript(ACTOR_FIRST_SOLDIER, gAltmillerCave_GuardsArrivePath);
  Event_Wait(3);
  Actor_WalkByAndWait(ACTOR_SECOND_SOLDIER, -16, 0);
  Actor_SetScript(ACTOR_SECOND_SOLDIER, gAltmillerCave_GuardsArrivePath);
  Actor_WaitForScriptEndById(ACTOR_FIRST_SOLDIER);
  Actor_WalkByAndWait(ACTOR_FIRST_SOLDIER, 0, -16);
  Actor_WaitForScriptEndById(ACTOR_SECOND_SOLDIER);
  Actor_FaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_MIA);
  Actor_Stop(ACTOR_IVAN);
  AltmillerCave_FacePartyNorth();
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 216, 1);
  FieldCamera_WaitForMove();
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
  Actor_FaceEachOther(ACTOR_FIRST_SOLDIER, ACTOR_SECOND_SOLDIER, 70);
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
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  AltmillerCave_FacePartyNorth();
  Event_Wait(30);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
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
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Event_Wait(40);
  Actor_WatchAndSetScript(ACTOR_PARTY_LEADER, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_BABI);
  Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_BABI);
  Actor_SetSpeed(ACTOR_BABI, 1);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkBy(ACTOR_FIRST_SOLDIER, 0, 48);
  Actor_WalkBy(ACTOR_SECOND_SOLDIER, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 32);
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_STAND);
  Actor_SetAnimation(ACTOR_SECOND_SOLDIER, ANIM_STAND);
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
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
  Actor_WaitForScriptEndById(ACTOR_BABI);
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
  FieldCamera_MoveTo(248, CAMERA_KEEP, 248, 1);
  FieldCamera_WaitForMove();
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
  Actor_SetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
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
    Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
    REG_BLDALPHA = BLEND_ALPHA(eva, evb);
    wait(7);
  }
  state->value = ALTMILLER_CAVE_LIGHT_STEPS;
  {
    u16 *slot = (u16 *)&state->active;
    s32 value = ALTMILLER_CAVE_DARK_LIGHT_WINOUT;

    *slot = value;
  }
  wait(1);
  Io_Write16(ALTMILLER_CAVE_DARKNESS_BLEND, &REG_BLDCNT);
  Io_Write16(ALTMILLER_CAVE_DARK_ALPHA, &REG_BLDALPHA);
  FieldCamera_MoveToActorById(ACTOR_PARTY_LEADER, 1);
  FieldCamera_WaitForMove();
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
    // "So we'll do it?"
    Event_ShowMessage(ACTOR_GARET, 0);
  } else {
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_Wait(20);
    Event_SetMessageId(MsgAltmillerDontWantEitherDo);
    // "You don't want to either, do you?"
    Event_ShowMessage(ACTOR_GARET, 0);
  }
  Event_SetMessageId(MsgAltmillerEitherWayWereStuckHere);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  // "Either way, we're stuck here in Tolbi until Colosso is over."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  // "Babi said that he wanted to talk to us, didn't he?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "He seemed to know Kraden."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  // "What could be the connection between Lemuria and Adepts?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "I suspect he knows quite a bit about what's going on with us."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "I guess all we can do is wait around until Colosso is..."
  Event_ShowMessage(ACTOR_GARET, 0);
  // "Come on, let's enter Colosso! Can we do it?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(30);
    Event_SetMessageId(MsgAltmillerGreatItsDecidedJustHave);
    // "Great! It's decided. We'll just have to do our best!"
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    // "If you want to do it, Isaac, we'll go along with you."
    Event_ShowMessage(ACTOR_IVAN, 0);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
  } else {
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_Wait(30);
    Event_SetMessageId(MsgAltmillerDontLetMeDownLike);
    // "What? Don't let me down like this!"
    Event_ShowMessage(ACTOR_GARET, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Event_Wait(30);
    // "I guess we'll just wait until we reach Tolbi and let Isaac..."
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
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1.2);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Scene_WalkIntoLeader(ACTOR_GARET);
  Scene_WalkIntoLeader(ACTOR_IVAN);
  Scene_WalkIntoLeader(ACTOR_MIA);
  Scene_PlayMusic();
  Event_End();
}

/// The invisible Babi asks the party to bring his draught from deep in the
/// cave.
static void AltmillerCave_MeetInvisibleBabi(void) {
  struct Actor *leader;
  s32 line;

  GameFlag_Set(FLAG_MET_INVISIBLE_BABI);
  Sound_Play(MUS_CAVE);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgAltmillerSee);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  // "You... You can see me?"
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 184, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 192);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_Wait(40);
  // "Someone's there."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "Wrapped in Psynergetic light... Who could it be?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "You know about Psynergy? And you can see me?"
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  // "Come on, mister! Don't hide-show yourself!"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_INVISIBLE_BABI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(30);
  // "Even if I wanted to show myself... As you can see, I cannot..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(20);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  // "What do you mean, 'as you can see'? You're invisible!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "Oh... That's right."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "You say you can't move... What happened?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "Hmmm, yes. I ran out of my draught and collapsed here."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Draught?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(20);
  // "Exactly what kind of draught is it?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "You couldn't even begin to comprehend if I tried to tell..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "You seem to be in a lot of pain. Your life depends on that..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  // "It does. Could you do me a favor?"
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "A favor? What is it?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(20);
  // "I'd like you to get my draught for me."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Get it? Where is it?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  // "Deep in this cave..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(20);
  // "What, is there a pharmacy down there?"
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Witty, aren't we? I'll have to tell you how to find it. Can..."
  Event_OpenMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(MsgAltmillerKiddingHaventActually);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Actor_SetSpeed(ACTOR_GARET, 2);
    Actor_WalkByAndWait(ACTOR_GARET, 0, -16);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
    // "Are you kidding, Isaac? We haven't even actually seen this guy!"
    Event_ShowMessage(ACTOR_GARET, 0);
  } else {
    Event_SetMessageId(MsgAltmillerDontTrustAnyone);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_GARET, 1);
    Actor_WalkByAndWait(ACTOR_GARET, 0, -16);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 30);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(20);
    // "I'm with you, Isaac. I don't trust anyone who's afraid to..."
    Event_ShowMessage(ACTOR_GARET, 0);
  }

  // Question Babi's condition before agreeing to find his draught.
  line = MsgAltmillerSaidCouldntMove;
  Event_SetMessageId(line);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
  Actor_SetSpeed(ACTOR_IVAN, 2);
  Actor_WalkByAndWait(ACTOR_IVAN, 0, -16);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
  // "He said he couldn't move even if he wanted to, right?..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_MIA, 0);
  Event_Wait(20);
  // "I agree with Ivan- whoever he is, he needs our help."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_WalkByAndWait(ACTOR_GARET, 0, 16);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  // "Fine! Okay! We should help people in need. Is that better!?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(30);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_WalkByAndWait(ACTOR_IVAN, 0, 16);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  line += 7;
  Event_SetMessageId(line);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "He said he couldn't move even if he wanted to, right?..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  // "I agree with Ivan- whoever he is, he needs our help."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Fine! Okay! We should help people in need. Is that better!?"
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  // "I'm with you, Isaac. I don't trust anyone who's afraid to..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(30);
  // "He said he couldn't even move, didn't he? Someone is in..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  // "I agree with Ivan line of thinking."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "Fine! Whatever! We'll help everybody who needs help. Is that..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(30);
  // "Will you get it for me?!"
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Event_Wait(30);
  // "What do we have to do to get your draught?"
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 50);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Event_Wait(30);
  // "First, you'll have to go deep into the cave."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_Wait(20);
  // "You'll find five rocks sticking up out of the ground."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(30);
  // "Rotate the rocks on the left and right sides."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "When you turn the rocks in the right order, five colored..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  // "Colored lights!? This sounds weird."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  // "That's right. Lights will appear on the wall."
  Event_ShowMessage(ACTOR_GARET, 0);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "There will be five colors: blue, green, white, yellow, and red."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_RunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Event_Wait(20);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Turn the five rocks in accordance with those colors."
  Event_ShowMessage(ACTOR_MIA, 0);
  Event_Wait(10);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_MIA, 40);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_GARET, ACTOR_MIA, 0);
  Actor_FaceActor(ACTOR_IVAN, ACTOR_MIA, 0);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(30);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetSpeed(ACTOR_GARET, 1.2);
  Actor_SetSpeed(ACTOR_IVAN, 1.2);
  Actor_SetSpeed(ACTOR_MIA, 1.2);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_IVAN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Scene_PlayMusic();
  Event_End();
}

/// Babi asks whether the party forgot the order of the rock colors, and tells
/// it again.
static void AltmillerCave_RepeatRockOrder(void) {
  s32 question;

  Event_Start();
  question = MsgAltmillerForgetOrderRock;
  Event_SetMessageId(question);
  // "Did you forget the order of rock colors?"
  Event_OpenMessage(ACTOR_INVISIBLE_BABI, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Event_SetMessageId(question + 1);
    // "From the left, it's blue, green, white, yellow, and red...."
    Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  } else {
    Event_Wait(20);
    Event_SetMessageId(question + 2);
    // "Deep in the cave are five rocks. Turn the ones on the ends..."
    Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  }
  Event_End();
}

/// Babi calls the party back and says where the draught is. "Wait... The
/// draught is... on a table behind the door."
static void AltmillerCave_BabiCallsPartyBack(void) {
  if (GameFlag_Get(FLAG_MET_INVISIBLE_BABI) == 0)
    return;
  if (GameFlag_Get(FLAG_BABI_SAVED) != 0)
    return;

  GameFlag_Set(FLAG_BABI_CALLED_PARTY_BACK);
  Event_Start();
  Event_SetMessageId(MsgAltmillerWait);
  // "Wait..."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_Wait(30);
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_INVISIBLE_BABI, 0);
  Event_Wait(30);
  // "The draught is... on a table behind the door."
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Event_End();
}

const struct Trigger *AltmillerCave_GetTriggers(void) {
  s16 sceneId = gMainRoutineWork.next;

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

/// Update routine of AltmillerCaveDraught's obstacles. They are drawn on the
/// leader's sprite priority.
static void AltmillerCave_MatchLeaderPriority(struct Actor *actor) {
  if (actor != NULL) {
    actor->priorityFlags = 0;
    actor->sprite->image.priority =
        Actor_Get(ACTOR_PARTY_LEADER)->sprite->image.priority;
  }
}

/// Redraws the three obstacles' cells where they stand.
static inline void AltmillerCave_RedrawObstacleFootprints(void) {
  Scene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE);
  Scene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE + 1);
  Scene_RedrawActorFootprint(ACTOR_FIRST_OBSTACLE + 2);
}

/// Sets up the room being entered. Returns 0.
s32 AltmillerCave_Initialize(void) {
  struct Actor *actor;
  s32 i;

  if (gMainRoutineWork.entrance == 0) {
    if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveBabi) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_BABI_FROM_ALTMILLER_CAVE_ENTRANCE;
    }
    if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveVialChest) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_VIAL_CHEST_FROM_ALTMILLER_CAVE_BABI;
    }
    if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveSquall) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_SQUALL_FROM_ALTMILLER_CAVE_VIAL_CHEST;
    }
    if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveCookieChest) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_COOKIE_CHEST_FROM_ALTMILLER_CAVE_SQUALL;
    }
    if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveDraught) {
      gMainRoutineWork.entrance =
          ALTMILLER_CAVE_DRAUGHT_FROM_ALTMILLER_CAVE_COOKIE_CHEST;
    }
  }
  GameFlag_Set(FLAG_ALTMILLER_CAVE_DARK);
  Event_ClearFlag(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
  if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveEntrance) {
    if (gMainRoutineWork.entrance == ALTMILLER_CAVE_ENTRANCE_FROM_WORLD_MAP) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        CAVE_STATE(CAVE_DARKNESS) = 0;
      }
      GameFlag_Set(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
    }
    if (gMainRoutineWork.entrance ==
        ALTMILLER_CAVE_ENTRANCE_FROM_TORCH_CHAMBER) {
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        CAVE_STATE(CAVE_DARKNESS) = 5;
      }
      GameFlag_Set(FLAG_ALTMILLER_CAVE_DARKENS_BY_STEPS);
    }
  }
  if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveBabi) {
    if (GameFlag_Get(FLAG_BABI_SAVED)) {
      Actor_SetPosition(ACTOR_INVISIBLE_BABI, 0, 0);
    } else {
      actor = Actor_Get(ACTOR_INVISIBLE_BABI);
      actor->sprite->image.priority = 1;
      actor->sprite->flags = 2;
      actor->sprite->rotation = TRIG_TURN / 4;
    }
  }
  if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveSquall) {
    GameFlag_Clear(FLAG_ALTMILLER_CAVE_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    if (GameFlag_Get(FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN)) {
      Actor_SetAnimation(ACTOR_SQUALL_ROOM_PUDDLE, PUDDLE_ANIMATION_PILLAR);
      MapGrid_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    } else {
      Actor_Get(ACTOR_SQUALL_ROOM_PUDDLE)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
    }
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SQUALL_ROOM_PUDDLE), 0);
#if TBS_PATCH_1
    // The cells AltmillerCave_UpdateMovedObstacle copies as it sets the flag,
    // redrawn on arrival.
    if (GameFlag_Get(FLAG_ALTMILLER_CAVE_PILLAR_IN_PLACE)) {
      MapGrid_CopyCellAttributes(30, 20, 1, 1, 32, 20);
    }
#endif
  }
  if (gMainRoutineWork.next == (s32)&SceneId_AltmillerCaveDraught) {
    GameFlag_Clear(FLAG_ALTMILLER_CAVE_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    Actor_Get(ACTOR_FIRST_OBSTACLE)->update = AltmillerCave_MatchLeaderPriority;
    Actor_Get(ACTOR_FIRST_OBSTACLE + 1)->update =
        AltmillerCave_MatchLeaderPriority;
    // UB: the last is stored through a word view of the routine pointer.
#ifdef UBFIX
    Actor_Get(ACTOR_FIRST_OBSTACLE + 2)->update =
        AltmillerCave_MatchLeaderPriority;
#else
    *(s32 *)&Actor_Get(ACTOR_FIRST_OBSTACLE + 2)->update =
        (s32)AltmillerCave_MatchLeaderPriority;
#endif
    // Coming up to the rocks (ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE)
    // starts the puzzle over, with the red light first.
    if (gMainRoutineWork.entrance ==
        ALTMILLER_CAVE_DRAUGHT_ROCKS_FROM_PILLAR_CAVE) {
      Dma_Fill32(0, gAltmillerCave_LightWheel, 12);
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        CAVE_STATE(CAVE_ROUNDS_MATCHED) = 0;
        CAVE_STATE(CAVE_PUZZLE) = PUZZLE_START;
        CAVE_STATE(CAVE_CHOSEN_LIGHT) = ACTOR_RED_LIGHT - ACTOR_FIRST_LIGHT;
      }
    }
    if (CAVE_SIGNED_STATE(CAVE_PUZZLE) == PUZZLE_SOLVED) {
      MapGrid_CopyCellsLayered(41, 55, 3, 2, 30, 55);
      MapGrid_CopyCellAttributes(42, 8, 1, 1, 31, 8);
    }
    if (CAVE_SIGNED_STATE(CAVE_PUZZLE) == PUZZLE_LIGHTS_SHOWN) {
      AltmillerCave_PlaceLights(
          (CAVE_SIGNED_STATE(CAVE_CHOSEN_LIGHT) * Q16_ONE) / 5 + TRIG_TURN / 4);
    }
    for (i = 0; i < LIGHT_COUNT; i++) {
      actor = Actor_Get(ACTOR_FIRST_LIGHT + i);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      actor->scaleX = Q16_ONE;
      actor->scaleY = Q16_ONE;
      Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_LIGHT + i), 0);
      Actor_SetAnimation(ACTOR_FIRST_LIGHT + i, i + 1);
    }
    Actor_SetSpriteColorById(ACTOR_BLUE_LIGHT, COLOR_BLUE);
    Actor_SetSpriteColorById(ACTOR_GREEN_LIGHT, COLOR_GREEN);
    Actor_SetSpriteColorById(ACTOR_WHITE_LIGHT, COLOR_WHITE);
    Actor_SetSpriteColorById(ACTOR_YELLOW_LIGHT, COLOR_YELLOW);
    Actor_SetSpriteColorById(ACTOR_RED_LIGHT, COLOR_RED);
    Actor_SetSpriteColorById(ACTOR_BLUE_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_GREEN_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_WHITE_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_YELLOW_ROCK, COLOR_NONE);
    Actor_SetSpriteColorById(ACTOR_RED_ROCK, COLOR_NONE);
    Actor_Get(ACTOR_BLUE_ROCK)->sprite->image.priority = 3;
    Actor_Get(ACTOR_RED_ROCK)->sprite->image.priority = 3;
    Actor_Get(ACTOR_BLUE_ROCK)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_RED_ROCK)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteFlags(Actor_Get(ACTOR_BLUE_ROCK), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_RED_ROCK), 0);
  }
  if (GameFlag_Get(FLAG_ALTMILLER_CAVE_DARK)) {
    AltmillerCave_InitializeDarkness();
  } else {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    gFieldWork->transitionFrames = 24;
  }
  return 0;
}

/// Update routine of a piece of the burst wall.
static void AltmillerCave_UpdateWallFragment(struct FieldParticle *fragment) {
  s32 position[3];

  if (fragment->state == 0) {
    s32 startZ = fragment->originZ;
    s32 startX = fragment->originX;

    fragment->z = startZ;
    position[2] = startZ;
    fragment->x = startX;
    position[0] = startX;
    Vector_AddPolarOffset(px(120), Random_Next(), position);
    fragment->targetX = position[0];
    fragment->targetZ = position[2];
    fragment->acceleration = px(5);
    fragment->speedMax = px(5);
    fragment->turnStepLimited = 0;
    fragment->state++;
    if ((gVBlankCount & 3) == 0)
      Sound_Play(SE_HEAVY_IMPACT);
  } else if (fragment->state == 1) {
    if (FieldParticle_HasPendingTarget(fragment) == 0) {
      fragment->state--;
    }
  } else if (fragment->state == 2) {
    // Unused: no step leads here.
    if (FieldParticle_HasPendingTarget(fragment) == 0)
      FieldEffect_ClearOwnedSlot(fragment);
  }
}

/// The wall bursts. Its 24 pieces fly out from the ring's center, the wall's
/// cells open to the passage, and the shaking calms.
static void AltmillerCave_ReleaseWallBurst(void) {
  struct FieldEffectWork *effectWork;
  struct Vector3 position;
  struct FieldParticle *fragment;
  s32 fragmentIndex;

  FieldEffect_InitializeSlots();
  effectWork = (struct FieldEffectWork *)gFieldEffectWork;
  Shop_SaveAndTintFieldPalette(PALETTE_TINT_CUSTOM | RGB(8, 8, 8));
  position.x = px(504);
  position.y = px(24);
  position.z = px(144);
  Camera_WorldToScreen(&position);
  fragment = effectWork->particles;
  for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
    s32 scale;

    FieldParticle_Initialize(fragment, SPRITE_PARTICLE, position.x, position.z);
    FieldParticle_SetCallback(fragment, AltmillerCave_UpdateWallFragment);
    FieldParticle_SetActorMode(fragment, 7);
    Sprite_SetAnimationColorModes(fragment->actor, Random_Below(7));
    scale = Math_DivideUnsigned(Random_Next(), 3) + 3 * Q16_ONE / 2;
    fragment->scaleY = scale;
    fragment->scaleX = scale;
    wait(1);
    fragment++;
  }
  wait(80);
  MapGrid_CopyCellsLayered(41, 55, 3, 2, 30, 55);
  MapGrid_CopyCellAttributes(42, 8, 1, 1, 31, 8);
  wait(50);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  wait(30);
  {
    u8 *stateCursor;
    FIELD_PARTICLES_SET_ACTIVE_STATE(stateCursor, fragmentIndex, effectWork,
                                     particles, 2);
  }
  MapGrid_WaitForShakeToSettle();
  Shop_RestoreFieldPalette();
  FieldEffect_ReleaseSlots();
}

#include "shared/pushed_actor_tables.h"

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
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_INVISIBLE_BABI, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 232, 0, 160, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -64, 0,
                    -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, -64, 0, -64, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_WITH_TORCH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, -64, 0, -64, FACING_NORTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltmillerCave_SquallActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 328, 0, 256, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 352, 0, 296, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 368, 0, 200, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376,
                    0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_SQUALL),
                    MOVEMENT_TYPE_STAND, 264, 0, 200, FACING_SOUTHWEST,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltmillerCave_DraughtActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 392, 0, 416, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 416, 0, 456, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_RESET_ACTION, 456, 0, 480, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 8,
                    1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472,
                    40, 1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504,
                    56, 1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536,
                    40, 1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_LIGHT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 8,
                    1752, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    440, 0, 190, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    472, 0, 174, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    504, 0, 174, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    536, 0, 174, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    568, 0, 190, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_MOVE_BY(-64, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -64),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

/// Babi and the two men walk out of the room, each by his own path.
s32 gAltmillerCave_SecondGuardLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gAltmillerCave_FirstGuardLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, 16),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(32, 0, 0),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

s32 gAltmillerCave_BabiLeavePath[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 0),
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
    {TRIGGER_TABLE_END},
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
    {TRIGGER_TABLE_END},
};

struct Trigger gAltmillerCave_VialChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 20, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_BABI},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 21, CONDITION_ALWAYS, ALTMILLER_CAVE_VIAL_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(AltmillerCave_EndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(AltmillerCave_StartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTMILLER_CAVE_VIAL, FIND_ITEM(ITEM_VIAL)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltmillerCave_SquallTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 30, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_VIAL_CHEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 31, CONDITION_ALWAYS, ALTMILLER_CAVE_SQUALL_TO_ALTMILLER_CAVE_COOKIE_CHEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 72, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 73, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 74, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 75, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_HopSelectedActor)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 71, CONDITION_ALWAYS, EVENT(AltmillerCave_PushFallenColumn)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 77, CONDITION_ALWAYS, EVENT(AltmillerCave_AdvanceObstacleAndCheckPassage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SQUALL_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(AltmillerCave_UpdateMovedObstacle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_CheckFrostTargetCollision)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SQUALL_ROOM_PUDDLE, FLAG_ALTMILLER_CAVE_PUDDLE_FROZEN, EVENT(AltmillerCave_FreezePuddle)},
    {TRIGGER_TALK, ACTOR_SQUALL, CONDITION_ALWAYS, EVENT(AltmillerCave_StartDjinnBattle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltmillerCave_CookieChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 40, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_SQUALL},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 41, CONDITION_ALWAYS, ALTMILLER_CAVE_COOKIE_CHEST_TO_ALTMILLER_CAVE_DRAUGHT},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(AltmillerCave_EndReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(AltmillerCave_StartRetreat)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTMILLER_CAVE_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {TRIGGER_TABLE_END},
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
     EVENT(AltmillerCave_PushFallenColumn)},
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
    {TRIGGER_TABLE_END},
};

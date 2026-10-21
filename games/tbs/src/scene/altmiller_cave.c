/// @file
/// Altmiller Cave, six scenes (AltmillerCave1 to AltmillerCave6, scenes 146 to
/// 151) whose area name is Altmiller Cave: rooms one after another, each
/// numbered here as its scene id is (room 1 is AltmillerCave1), entered from
/// the world map at room 1. The rooms share one entrance table, and the
/// getters pick the rest of each room's tables by gGameState.scene;
/// AltmillerCave_Initialize sends an entry from outside the cave to the
/// room's first entrance, darkens the dark rooms and puts back what the party
/// has done.
///
/// The party comes here during Colosso's trials, while Babi, the ruler of
/// Tolbi, is missing: in his palace it has heard Iodem's soldiers report that
/// they searched this cave for him in vain, and Iodem say that he has seen
/// Babi vanish into it before (babi_palace.c). The cave is dark: room 1
/// darkens by steps as the party walks in (touches 60 to 65), the other dark
/// rooms all at once, and Reveal lights them while it lasts
/// (AltmillerCave_StartReveal, AltmillerCave_EndReveal; scene flag FLAG_DARK).
///
/// In room 2 the party meets someone it can hear but not see, wrapped in
/// Psynergetic light (ACTOR_INVISIBLE_BABI). Talking to him the first time
/// (AltmillerCave_MeetInvisibleBabi, FLAG_MET_INVISIBLE_BABI), he says he ran
/// out of his draught and cannot move, and asks the party to fetch it from
/// deep in the cave: five rocks there, turned the ends first, bring out
/// colored lights, and turning the rock of each light's color, `blue, green,
/// white, yellow, and red` from the left, opens a hidden door left by an
/// ancient civilization. The party argues over whether to help him. Walking
/// away the first time he calls the party back to say the draught is on a
/// table behind the door (AltmillerCave_BabiCallsPartyBack), and until it is
/// found he repeats the order (AltmillerCave_RepeatRockOrder).
///
/// Rooms 3 to 5 lead on, with a chest in rooms 3 and 5. Room 4 is lit; in it
/// Squall, a Jupiter Djinni, starts its battle when spoken to, three obstacles
/// move when pushed (as three do in room 6), a block that Move slides into
/// its cell opens the way beside it (which the editions from TBS_PATCH_1 on
/// also keep open after a battle, FLAG_BLOCK_IN_PLACE), and a puddle the
/// party hops over (AltmillerCave_HopSelectedActor) until Frost freezes it,
/// unless something stands on it (AltmillerCave_CheckFrostTargetCollision).
/// Room 6, also lit, holds the five rocks, whose colors show only while Reveal
/// shines. Talking to one turns it (AltmillerCave_TurnRock): the leftmost and
/// then the rightmost bring out a ring of five lights above them, which spins
/// and stops with one light on top; turning that light's rock spins it again, a
/// wrong rock puts the lights away and starts over, and after three matches
/// the wall bursts (AltmillerCave_ReleaseWallBurst) and the chest beyond
/// holds the Mystic Draught (FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT). The
/// puzzle's state is kept in the scene's work, and coming back up to the
/// rocks (entrance 52) starts it over.
///
/// Back in room 2, talking to him with the draught plays
/// AltmillerCave_RestoreBabiAndInviteParty (FLAG_BABI_SAVED): he drinks it
/// and appears, and Ivan reads his mind and finds him to be Babi, the ruler of
/// Tolbi, who hid himself with the power of an ancient civilization, not
/// Psynergy, and has been searching for Lemurians to escape growing old. Two
/// men from Tolbi, sent by Iodem, find him and take him back for the finals,
/// and Babi invites the party to fight in them. Stuck in Tolbi until Colosso
/// is over anyway, Garet asks whether they should enter: on yes it is decided,
/// and on no they will wait until Tolbi and let Isaac decide there, where the
/// story goes on (colosseum_entrance.c, babi_palace.c).

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

/// Room 2's actors: Babi while invisible and once seen, and the two men from
/// Tolbi who come looking for him.
enum {
  ACTOR_INVISIBLE_BABI = 8,
  ACTOR_BABI = 9,
  ACTOR_FIRST_GUARD = 10,
  ACTOR_SECOND_GUARD = 11
};

/// Rooms 4 and 6 place first the three obstacles the party pushes; room 4
/// then a puddle, a block for Move and Squall, a Jupiter Djinni.
enum {
  ACTOR_FIRST_OBSTACLE = 8,
  ACTOR_ROOM4_PUDDLE = 11,
  ACTOR_ROOM4_BLOCK = 12,
  ACTOR_DJINNI = 13
};

/// Room 6: the five colored lights and the five rocks, each in Babi's order,
/// the rocks left to right. The order is "blue, green, white, yellow, red".
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
  /// The cave is dark: set on entry everywhere but rooms 4 and 6.
  FLAG_DARK = FLAG_SCENE_TEMP_00,
  /// Room 1 darkens by steps as the party walks in.
  FLAG_GRADED_DARKNESS = FLAG_SCENE_TEMP_01,
  /// Frost has frozen room 4's puddle.
  FLAG_PUDDLE_FROZEN = FLAG_SCENE_TEMP_11,
  /// Room 4's block stands where Move opens the passage.
  FLAG_BLOCK_IN_PLACE = FLAG_SCENE_TEMP_12
};

static void AltmillerCave_ReleaseWallBurst(void);

/// The ring of five lights above room 6's rocks as it spins to its next
/// light: angles are 16-bit turns, the light at the top is the chosen one.
struct LightWheel {
  s16 phase;
  s16 ticks;
  s16 unk04;
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

/// Room 6's rock puzzle, kept in gSceneState: the rounds of lights matched in
/// byte 0, then the state and the light shown.
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

/// Talking to the Djinni in room 4 starts the battle with Squall, a Jupiter
/// Djinni.
static void AltmillerCave_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_DJINNI, DJINNI_SQUALL);
}

/// The six rooms share one entrance table.
const struct SceneEntrance *AltmillerCave_GetEntrances(void) {
  return gAltmillerCave_Entrances;
}

const struct SceneRegion *AltmillerCave_GetRegions(void) { return NULL; }

const u32 *AltmillerCave_GetExits(void) { return gAltmillerCave_Exits; }

const struct ScenePlacement *AltmillerCave_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_AltmillerCave2) {
    return gAltmillerCave_Placements2;
  }
  if (scene == (s32)&SceneId_AltmillerCave4) {
    return gAltmillerCave_Placements4;
  }
  if (scene == (s32)&SceneId_AltmillerCave6) {
    return gAltmillerCave_Placements6;
  }
  return gAltmillerCave_PlacementsOther;
}

/// Darkens room 1 to its current level: the brighter blend at level 0, each
/// level one step darker.
static void AltmillerCave_ApplyFadeBlend(void) {
  s8 level = ((s8 *)gSceneState)[4];

  Io_Write16(0x3f42, &REG_BLDCNT);
  if (level == 0) {
    s32 alpha = 0x1000;

    REG_BLDALPHA = alpha;
  } else if (level == 1) {
    s32 alpha = 0xe00;

    REG_BLDALPHA = alpha;
  } else if (level == 2) {
    s32 alpha = 0xc00;

    REG_BLDALPHA = alpha;
  } else if (level == 3) {
    s32 alpha = 0xa00;

    REG_BLDALPHA = alpha;
  } else if (level == 4) {
    s32 alpha = 0x800;

    REG_BLDALPHA = alpha;
  } else {
    s32 alpha = 0x600;

    REG_BLDALPHA = alpha;
  }
}

/// Keeps room 1's darkness level, and shows it unless Reveal is lighting the
/// room.
static void AltmillerCave_SetFadeLevel(s32 level) {
  struct FieldWork *work;
  u8 *fade = &gSceneState[4];

  work = (struct FieldWork *)gEventWork;
  *fade = level;
  if (work->revealActive == 0) {
    AltmillerCave_ApplyFadeBlend();
  }
}

/// Room 1's darkness boundaries: crossing one sets its level.
static void AltmillerCave_DarkenToLevel0(void) {
  AltmillerCave_SetFadeLevel(0);
}

static void AltmillerCave_DarkenToLevel1(void) {
  AltmillerCave_SetFadeLevel(1);
}

static void AltmillerCave_DarkenToLevel2(void) {
  AltmillerCave_SetFadeLevel(2);
}

static void AltmillerCave_DarkenToLevel3(void) {
  AltmillerCave_SetFadeLevel(3);
}

static void AltmillerCave_DarkenToLevel4(void) {
  AltmillerCave_SetFadeLevel(4);
}

static void AltmillerCave_DarkenToLevel5(void) {
  AltmillerCave_SetFadeLevel(5);
}

/// Darkens the cave on entry: room 1 by its levels, the other dark rooms
/// evenly.
static void AltmillerCave_InitializeDarkness(void) {
  u8 **workCells = (u8 **)&gEventWork;
  struct DisplayTransitionState *transition;

  {
    struct EventWork *work = (struct EventWork *)*workCells;
    work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    work->transitionFrames = 24;
  }
  Engine_TaskWait(1);
  DisplayTransition_InitializeBattleEffectState(0x4d);
  // UB: the fourth following pointer cell lies past scalar gEventWork.
  // Its named load adds eight bytes in the native EN trial (0x98 to 0xa0).
#ifdef UBFIX
  transition = gDisplayTransition;
#else
  transition = (struct DisplayTransitionState *)workCells[4];
#endif
  {
    u16 *slot = &transition->value;
    s32 value = 5;
    *slot = value;
  }
  if (Engine_GameFlagIsSet(FLAG_GRADED_DARKNESS) != 0) {
    {
      s16 *slot = &transition->mask;
      s32 value = 0x1d1d;
      *slot = value;
    }
    {
      s16 *slot = &transition->active;
      s32 value = 0x3f;
      *slot = value;
    }
    AltmillerCave_ApplyFadeBlend();
    return;
  }
  {
    s16 *slot = &transition->mask;
    s32 value = 0x3f3f;
    *slot = value;
  }
  {
    s16 *slot = &transition->active;
    s32 value = 31;
    *slot = value;
  }
  {
    s32 blendControl = 0x3f42;
    REG_BLDCNT = blendControl;
  }
  {
    s32 blendAlpha = 0xc04;
    REG_BLDALPHA = blendAlpha;
  }
}

/// Reveal's light: room 1 is lit and room 6's rocks show their colors.
static void AltmillerCave_StartReveal(void) {
  if (gGameState.scene == (s32)&SceneId_AltmillerCave1) {
    s32 alpha = 0x1000;

    REG_BLDALPHA = alpha;
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
    Engine_ActorSetChildValue(ACTOR_BLUE_ROCK, COLOR_BLUE);
    Engine_ActorSetChildValue(ACTOR_GREEN_ROCK, COLOR_GREEN);
    Engine_ActorSetChildValue(ACTOR_WHITE_ROCK, COLOR_WHITE);
    Engine_ActorSetChildValue(ACTOR_YELLOW_ROCK, COLOR_YELLOW);
    Engine_ActorSetChildValue(ACTOR_RED_ROCK, COLOR_RED);
  }
}

/// Reveal's light ends: the cave darkens again and room 6's rocks lose their
/// colors.
static void AltmillerCave_EndReveal(void) {
  struct BattleFxScene *work = (struct BattleFxScene *)gEffectWork;

  if (Engine_GameFlagIsSet(FLAG_DARK) != 0) {
    AltmillerCave_InitializeDarkness();
    work->childOption = 1;
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
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
  struct BattleFxScene *work;
  struct FieldActor *original;
  struct FieldActor *replacement;

  work = (struct BattleFxScene *)gEffectWork;
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

/// After Move on room 4's block: in its cell at (30, 20) it settles and opens
/// the way beside it (remembered in TBS_PATCH_1 and later).
static void AltmillerCave_UpdateMovedObstacle(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_ROOM4_BLOCK);

  if (actor->x.fixed >> 20 == 30 && actor->z.fixed >> 20 == 20) {
    actor->motionFlags = 2;
    actor->terrainHeight = 0;
    actor->priorityFlags = 2;
    Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
#if TBS_PATCH_1
    Engine_GameFlagSet(FLAG_BLOCK_IN_PLACE);
#endif
  }
}

/// Walking east into room 4's block pushes it a cell.
static void AltmillerCave_AdvanceObstacleAndCheckPassage(void) {
  StagedActor_AdvancePair();
  AltmillerCave_UpdateMovedObstacle();
}

/// Frost's first phase on room 4's puddle: notes whether its place is
/// blocked, which keeps it from freezing.
static void AltmillerCave_CheckFrostTargetCollision(void) {
  struct BattleFxScene *effect = (struct BattleFxScene *)gEffectWork;
  struct FieldActor *target = Object_GetById(ACTOR_ROOM4_PUDDLE);
  struct FieldPosition position;

  position.x = target->x.fixed;
  position.y = target->y.fixed;
  position.z = target->z.fixed;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    effect->childMode = 1;
  }
}

/// Frost on room 4's puddle, unless blocked: it freezes and its cell takes
/// the frozen attributes kept at (76, 16).
static void AltmillerCave_FreezePuddle(void) {
  struct BattleFxScene *effect = (struct BattleFxScene *)gEffectWork;
  struct FieldActor *puddle = Object_GetById(ACTOR_ROOM4_PUDDLE);
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

/// Walking into room 4's puddle while it is not frozen: the leader hops 32
/// pixels over it, if the landing is clear.
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
    Object_SetMode(actor, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(actor, 7);
    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 126;
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ObjectMotionSetPositionAndCommit(0, components[0].part.pixel,
                                            components[2].part.pixel);
    Object_SetMode(actor, 6);
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

/// Talking to one of room 6's rocks rotates it. "Isaac rotated the rock."
/// Turning the leftmost rock, then the rightmost, brings out the ring of
/// lights; then each time the rock matching the light at the top is turned,
/// the ring spins to another light, and a wrong rock puts the lights away and
/// starts over. After three lights matched the ring shrinks away, the wall
/// bursts and the way to Babi's draught opens.
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
  Battle_ResetEffectCounter();
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

/// Bringing Babi the Mystic Draught: he drinks it and appears, startling the
/// party; Ivan reads his mind and finds him to be Babi, the ruler of Tolbi,
/// who sought Lemurians to escape growing old. Two men from Tolbi find him and
/// take him back, and he invites the party to Colosso's finals; Garet asks
/// whether they should take part.
static void AltmillerCave_RestoreBabiAndInviteParty(void) {
  struct DisplayTransitionState *display;
  struct FieldActor *actor;
  struct FieldActor *leader;
  s32 i;
  s32 eva;
  s32 evb;

  // Give Babi the draught and bring the party beside him.
  GameFlag_Set(FLAG_BABI_SAVED);
  // Item 237 is the Mystic Draught.
  Engine_PartyRemoveItem(237);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
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
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 8, 40);
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
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Engine_ActorJump(GARET, 4, 13);
  Engine_ActorJump(GARET, 4, 30);
  Event_ShowMessage(GARET, 0);
  Actor_FaceActor(IVAN, GARET, 30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 40);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_OpenMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
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
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 1, 40);
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
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
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
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
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
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 50);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 1, 40);
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
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
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
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 80);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_NORTH, 0);
  Engine_EventWait(50);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerThatsKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  } else {
    Engine_EventSetMessage(MsgAltmillerForgotKradenSaidAlchemyCould);
    Event_ShowMessage(GARET, 0);
  }
  Engine_EventSetMessage(MsgAltmillerWhatWasThat);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
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
  Engine_EventChooseYesNo(0, 0);
  // The cave brightens as two men from Tolbi arrive: "Babi!"
  display = gDisplayTransition;
  {
    u16 *slot = &display->value;
    s32 value = 32;

    *slot = value;
  }
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = 0x3f42;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = (eva << 8) | evb;
    Engine_TaskWait(7);
  }
  {
    u16 *slot = (u16 *)&display->active;
    s32 value = 0x3f3f;

    *slot = value;
  }
  Actor_SetPosition(ACTOR_FIRST_GUARD, PIXELS(280), PIXELS(304));
  Actor_SetPosition(ACTOR_SECOND_GUARD, PIXELS(296), PIXELS(304));
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Engine_EventWait(10);
  Engine_EventWait(10);
  Engine_CameraFollowActor(ACTOR_FIRST_GUARD, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(70);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, ACTOR_FIRST_GUARD);
  Engine_ActorFollow(GARET, ACTOR_FIRST_GUARD);
  Engine_ActorFollow(MIA, ACTOR_FIRST_GUARD);
  Engine_ActorFollow(IVAN, ACTOR_FIRST_GUARD);
  // The two men greet Babi; he invites the party to the finals.
  Actor_SetSpeed(ACTOR_FIRST_GUARD, PIXELS(7) / 5, PIXELS(7) / 10);
  Actor_SetSpeed(ACTOR_SECOND_GUARD, PIXELS(7) / 5, PIXELS(7) / 10);
  Actor_SetActionScript(ACTOR_FIRST_GUARD, gAltmillerCave_GuardsArrivePath);
  Engine_EventWait(3);
  Actor_WalkByAndWait(ACTOR_SECOND_GUARD, -16, 0);
  Actor_SetActionScript(ACTOR_SECOND_GUARD, gAltmillerCave_GuardsArrivePath);
  Object_RefreshSelectorById(ACTOR_FIRST_GUARD);
  Actor_WalkByAndWait(ACTOR_FIRST_GUARD, 0, -16);
  Object_RefreshSelectorById(ACTOR_SECOND_GUARD);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
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
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_SECOND_GUARD, 16, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_WalkByAndWait(ACTOR_FIRST_GUARD, 16, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_SECOND_GUARD, 4, 13);
  Engine_ActorJump(ACTOR_SECOND_GUARD, 4, 30);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BABI, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(70);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_BABI, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 70);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_SECOND_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
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
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 8, 40);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
  Engine_EventWait(30);
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_BABI, ANIM_NOD);
  Engine_EventWait(40);
  Engine_ActorFollow(ACTOR_PARTY_LEADER, ACTOR_BABI);
  Engine_ActorFollow(GARET, ACTOR_BABI);
  Engine_ActorFollow(MIA, ACTOR_BABI);
  Engine_ActorFollow(IVAN, ACTOR_BABI);
  Actor_SetSpeed(ACTOR_BABI, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkBy(ACTOR_FIRST_GUARD, 0, 48);
  Actor_WalkBy(ACTOR_SECOND_GUARD, 0, 48);
  Actor_WalkByAndWait(ACTOR_BABI, -16, 0);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 32);
  Engine_ActorSetAnimation(ACTOR_FIRST_GUARD, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_SECOND_GUARD, ANIM_STAND);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(MIA);
  Engine_ActorStop(IVAN);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_NORTH, 0);
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
  Actor_SetActionScript(ACTOR_SECOND_GUARD,
                        gAltmillerCave_SecondGuardLeavePath);
  Actor_SetActionScript(ACTOR_FIRST_GUARD, gAltmillerCave_FirstGuardLeavePath);
  Actor_SetActionScript(ACTOR_BABI, gAltmillerCave_BabiLeavePath);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Object_RefreshSelectorById(ACTOR_BABI);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
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
  Engine_ActorRunRepeatedMotion(ACTOR_FIRST_GUARD, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_WEST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_FIRST_GUARD, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_SECOND_GUARD, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_BABI, EMOTE_IN_FRONT | 7, 40);
  Actor_FaceDirection(ACTOR_BABI, FACING_EAST, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FIRST_GUARD, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_SECOND_GUARD, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(30);
  Actor_FaceActor(ACTOR_FIRST_GUARD, ACTOR_SECOND_GUARD, 60);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_WEST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_FIRST_GUARD, ANIM_NOD);
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
  Engine_ActorSetAnimation(ACTOR_FIRST_GUARD, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_SECOND_GUARD, ANIM_NOD);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_FIRST_GUARD, FACING_EAST, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_SECOND_GUARD, FACING_EAST, 0);
  Engine_EventWait(40);
  Actor_WalkBy(ACTOR_BABI, 32, 0);
  Actor_WalkBy(ACTOR_SECOND_GUARD, 0, 64);
  Actor_WalkByAndWait(ACTOR_FIRST_GUARD, 16, 0);
  Actor_WalkBy(ACTOR_FIRST_GUARD, 0, 64);
  Engine_ActorWaitForMove(ACTOR_BABI);
  Actor_WalkByAndWait(ACTOR_BABI, 0, 64);
  Actor_SetPosition(ACTOR_BABI, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_GUARD, 0, 0);
  Actor_SetPosition(ACTOR_SECOND_GUARD, 0, 0);
  Engine_EventWait(20);
  for (eva = 0, evb = 16; eva <= 16; eva++, evb--) {
    {
      s32 control = 0x3f42;

      REG_BLDCNT = control;
    }
    REG_BLDALPHA = (evb << 8) | eva;
    Engine_TaskWait(7);
  }
  {
    u16 *slot = &display->value;
    s32 value = 5;

    *slot = value;
  }
  {
    u16 *slot = (u16 *)&display->active;
    s32 value = 31;

    *slot = value;
  }
  Engine_TaskWait(1);
  Io_Write16(0x3f42, &REG_BLDCNT);
  Io_Write16(0xc04, &REG_BLDALPHA);
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
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_OpenMessage(GARET, 0);
  Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
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
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
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
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
  Event_ShowMessage(GARET, 0);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
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
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 70);
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

/// Talking to the invisible Babi the first time: he can be heard but not
/// seen, cannot move, and asks the party to bring his draught from deep in
/// the cave, telling the order of the five rocks.
static void AltmillerCave_MeetInvisibleBabi(void) {
  struct FieldActor *leader;
  s32 line;

  // Gather the party around the invisible voice.
  GameFlag_Set(FLAG_MET_INVISIBLE_BABI);
  Engine_AudioPlayCue(MUS_024);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
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
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, IVAN, 50);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_SetAttachedEffect(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 2);
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
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 5, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 30);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Event_ShowMessage(GARET, 0);
  Engine_ActorFaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 6, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Engine_ActorShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_INVISIBLE_BABI, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 2, 40);
  Event_OpenMessage(ACTOR_INVISIBLE_BABI, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventSetMessage(MsgAltmillerKiddingHaventActually);
    Engine_EventWait(20);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 40);
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
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 3, 40);
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
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT, 40);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 8, 40);
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
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
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
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 8, 50);
  Engine_EventShowMessage(ACTOR_INVISIBLE_BABI, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_INVISIBLE_BABI, EMOTE_IN_FRONT | 2, 40);
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

/// Talking to Babi again before the draught is found: he asks whether the
/// party forgot the order of the rock colors, and tells it again.
static void AltmillerCave_RepeatRockOrder(void) {
  s32 question;

  Engine_EventBegin();
  question = MsgAltmillerForgetOrderRock;
  Engine_EventSetMessage(question);
  Engine_EventOpenMessage(ACTOR_INVISIBLE_BABI, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
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

/// Walking away from Babi once: the party is told where the draught is.
/// "Wait... The draught is... on a table behind the door."
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

  if (scene == (s32)&SceneId_AltmillerCave2) {
    return gAltmillerCave_Events2;
  }
  if (scene == (s32)&SceneId_AltmillerCave3) {
    return gAltmillerCave_Events3;
  }
  if (scene == (s32)&SceneId_AltmillerCave4) {
    return gAltmillerCave_Events4;
  }
  if (scene == (s32)&SceneId_AltmillerCave5) {
    return gAltmillerCave_Events5;
  }
  if (scene == (s32)&SceneId_AltmillerCave6) {
    return gAltmillerCave_Events6;
  }
  return gAltmillerCave_EventsOther;
}

/// Update callback of room 6's obstacles: drawn on the leader's sprite
/// priority.
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
/// room's first entrance; room 1 picks its darkness from the entrance; room
/// 2's Babi is invisible until rescued; room 4's obstacles and pillar and
/// room 6's obstacles, lights and rocks are restored. Returns 0.
s32 AltmillerCave_Initialize(void) {
  s8 *state;
  struct FieldActor *actor;
  s32 i;

  if (gGameState.entrance == 0) {
    if (gGameState.scene == (s32)&SceneId_AltmillerCave2) {
      gGameState.entrance = 10;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave3) {
      gGameState.entrance = 20;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave4) {
      gGameState.entrance = 30;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave5) {
      gGameState.entrance = 40;
    }
    if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
      gGameState.entrance = 50;
    }
  }
  Engine_GameFlagSet(FLAG_DARK);
  GameFlag_Clear(FLAG_GRADED_DARKNESS);
  if (gGameState.scene == (s32)&SceneId_AltmillerCave1) {
    if (gGameState.entrance == ALTMILLER_CAVE_1_FROM_WORLD_MAP) {
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gSceneState + 4) = 0;
      }
      Engine_GameFlagSet(FLAG_GRADED_DARKNESS);
    }
    if (gGameState.entrance == 2) {
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        *(u8 *)((u32)gSceneState + 4) = 5;
      }
      Engine_GameFlagSet(FLAG_GRADED_DARKNESS);
    }
  }
  if (gGameState.scene == (s32)&SceneId_AltmillerCave2) {
    if (Engine_GameFlagIsSet(FLAG_BABI_SAVED)) {
      Engine_ActorSetPosition(ACTOR_INVISIBLE_BABI, 0, 0);
    } else {
      actor = Object_GetById(ACTOR_INVISIBLE_BABI);
      actor->sprite->image.priority = 1;
      actor->sprite->flags = 2;
      actor->sprite->rotation = 0x4000;
    }
  }
  // Restore room 4's moved obstacles and frozen puddle.
  if (gGameState.scene == (s32)&SceneId_AltmillerCave4) {
    Engine_GameFlagClear(FLAG_DARK);
    AltmillerCave_RedrawObstacleFootprints();
    if (Engine_GameFlagIsSet(FLAG_PUDDLE_FROZEN)) {
      Engine_ActorSetAnimation(ACTOR_ROOM4_PUDDLE, 5);
      Map_CopyCellAttributes(76, 16, 1, 1, 73, 17);
    } else {
      Object_GetById(ACTOR_ROOM4_PUDDLE)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
    }
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ROOM4_PUDDLE), 0);
#if TBS_PATCH_1
    if (Engine_GameFlagIsSet(FLAG_BLOCK_IN_PLACE)) {
      Map_CopyCellAttributes(30, 20, 1, 1, 32, 20);
    }
#endif
  }
  // Rebuild the final rock puzzle from its saved phase and chosen light.
  if (gGameState.scene == (s32)&SceneId_AltmillerCave6) {
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
    // Coming up to the rocks (entrance 52) starts the puzzle over, with the
    // red light first.
    if (gGameState.entrance == 52) {
      Dma_Fill32(0, gAltmillerCave_LightWheel, 12);
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        u8 *puzzleState = gSceneState;

        puzzleState[0] = 0;
        puzzleState[1] = PUZZLE_START;
        puzzleState[2] = 4;
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
      BattleFx_ClearOwnedSlot(fragment);
  }
}

/// The wall bursts: 24 pieces fly out from the ring's center, the wall's
/// cells open to the passage, and the shaking calms.
static void AltmillerCave_ReleaseWallBurst(void) {
  struct BattleFxScene *work;
  struct EffectVector position;
  struct EffectSlot *fragment;
  s32 fragmentIndex;

  BattleFx_InitializeSlots();
  work = (struct BattleFxScene *)gEffectWork;
  Shop_SaveAndTintFieldPalette(0x202108);
  position.x = PIXELS(504);
  position.y = PIXELS(24);
  position.z = PIXELS(144);
  Camera_WorldToScreen(&position);
  fragment = work->slots;
  for (fragmentIndex = 23; fragmentIndex >= 0; fragmentIndex--) {
    s32 scale;

    EffectSlot_Initialize(fragment, 284, position.x, position.z);
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
  BattleFx_ClearActiveSlotsAndScheduleUpdates();
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
    {1, CONDITION_ALWAYS, 456, 0, 504, FACING_NORTH, 0, 0, 0, 544, 528, 0},
    {2, CONDITION_ALWAYS, 280, 0, 360, FACING_SOUTH, 0, 0, 0, 544, 528, 0},
    {3, CONDITION_ALWAYS, 120, 0, 696, FACING_NORTH, 0, 0, 552, 240, 720, 0},
    {4, CONDITION_ALWAYS, 120, 0, 600, FACING_SOUTH, 0, 0, 552, 240, 720, 0},
    {10, CONDITION_ALWAYS, 392, 0, 504, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {11, CONDITION_ALWAYS, 136, 0, 104, FACING_SOUTH, 0, 48, 16, 496, 672, 0},
    {20, CONDITION_ALWAYS, 232, 0, 216, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {21, CONDITION_ALWAYS, 328, 0, 264, FACING_SOUTH, 0, 64, 48, 592, 576, 0},
    {30, CONDITION_ALWAYS, 200, 0, 424, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {31, CONDITION_ALWAYS, 520, 0, 328, FACING_SOUTH, 0, 48, 80, 576, 480, 0},
    {40, CONDITION_ALWAYS, 680, 0, 152, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {41, CONDITION_ALWAYS, 40, 0, 248, FACING_SOUTH, 0, 0, 0, 768, 512, 0},
    {50, CONDITION_ALWAYS, 296, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {51, CONDITION_ALWAYS, 424, 0, 392, FACING_SOUTH, 0, 160, 304, 576, 704, 0},
    {52, CONDITION_ALWAYS, 504, 0, 232, FACING_NORTH, 0, 384, 64, 624, 240, 0},
    {53, CONDITION_ALWAYS, 504, 0, 152, FACING_SOUTH, 0, 384, 64, 624, 240, 0},
    {54, CONDITION_ALWAYS, 184, 0, 216, FACING_NORTH, 0, 64, 64, 304, 240, 0},
    {SCENE_TABLE_END},
};

u32 gAltmillerCave_Exits[] = {
    SCENE_EXITS(AltmillerCave1),
    SCENE_EXIT(ALTMILLER_CAVE_1_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_ALTMILLER_CAVE_1),
    SCENE_EXIT(2, AltmillerCave1, 3),
    SCENE_EXIT(3, AltmillerCave1, 2),
    SCENE_EXIT(ALTMILLER_CAVE_1_TO_ALTMILLER_CAVE_2, AltmillerCave2,
               ALTMILLER_CAVE_2_FROM_ALTMILLER_CAVE_1),
    SCENE_EXITS(AltmillerCave2),
    SCENE_EXIT(ALTMILLER_CAVE_2_TO_ALTMILLER_CAVE_1, AltmillerCave1,
               ALTMILLER_CAVE_1_FROM_ALTMILLER_CAVE_2),
    SCENE_EXIT(ALTMILLER_CAVE_2_TO_ALTMILLER_CAVE_3, AltmillerCave3,
               ALTMILLER_CAVE_3_FROM_ALTMILLER_CAVE_2),
    SCENE_EXITS(AltmillerCave3),
    SCENE_EXIT(ALTMILLER_CAVE_3_TO_ALTMILLER_CAVE_2, AltmillerCave2,
               ALTMILLER_CAVE_2_FROM_ALTMILLER_CAVE_3),
    SCENE_EXIT(ALTMILLER_CAVE_3_TO_ALTMILLER_CAVE_4, AltmillerCave4,
               ALTMILLER_CAVE_4_FROM_ALTMILLER_CAVE_3),
    SCENE_EXITS(AltmillerCave4),
    SCENE_EXIT(ALTMILLER_CAVE_4_TO_ALTMILLER_CAVE_3, AltmillerCave3,
               ALTMILLER_CAVE_3_FROM_ALTMILLER_CAVE_4),
    SCENE_EXIT(ALTMILLER_CAVE_4_TO_ALTMILLER_CAVE_5, AltmillerCave5,
               ALTMILLER_CAVE_5_FROM_ALTMILLER_CAVE_4),
    SCENE_EXITS(AltmillerCave5),
    SCENE_EXIT(ALTMILLER_CAVE_5_TO_ALTMILLER_CAVE_4, AltmillerCave4,
               ALTMILLER_CAVE_4_FROM_ALTMILLER_CAVE_5),
    SCENE_EXIT(ALTMILLER_CAVE_5_TO_ALTMILLER_CAVE_6, AltmillerCave6,
               ALTMILLER_CAVE_6_FROM_ALTMILLER_CAVE_5),
    SCENE_EXITS(AltmillerCave6),
    SCENE_EXIT(ALTMILLER_CAVE_6_TO_ALTMILLER_CAVE_5, AltmillerCave5,
               ALTMILLER_CAVE_5_FROM_ALTMILLER_CAVE_6),
    SCENE_EXIT(51, AltmillerCave6, 52),
    SCENE_EXIT(52, AltmillerCave6, 51),
    SCENE_EXIT(53, AltmillerCave6, 54),
    SCENE_EXIT(54, AltmillerCave6, 53),
    SCENE_EXITS_END,
};

struct ScenePlacement gAltmillerCave_PlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCave_Placements2[] = {
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST,
     TALK_FACE_PARTY, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY,
     0},
    {0x41, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(232), 0, PIXELS(160),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x34, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0, -PIXELS(64),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xa3, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0, -PIXELS(64),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {0xa3, CONDITION_ALWAYS, ACTOR_STAND, -PIXELS(64), 0, -PIXELS(64),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCave_Placements4[] = {
    {0x129, CONDITION_ALWAYS, 7, PIXELS(328), 0, PIXELS(256), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(352), 0, PIXELS(296), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(368), 0, PIXELS(200), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(376), 0, PIXELS(280),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0, PIXELS(328),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x5d, FLAG_DJINNI_0 + 65, ACTOR_STAND, PIXELS(264), 0, PIXELS(200),
     FACING_SOUTHWEST, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltmillerCave_Placements6[] = {
    {0x129, CONDITION_ALWAYS, 7, PIXELS(392), 0, PIXELS(416), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x12a, CONDITION_ALWAYS, 7, PIXELS(416), 0, PIXELS(456), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x129, CONDITION_ALWAYS, 7, PIXELS(456), 0, PIXELS(480), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(488), PIXELS(8), PIXELS(1752),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), PIXELS(56),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), PIXELS(40),
     PIXELS(1752), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x124, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(520), PIXELS(8), PIXELS(1752),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(440), 0, PIXELS(190),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(472), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(504), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(174),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
    {0x125, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(568), 0, PIXELS(190),
     FACING_SOUTH, TALK_FACE_PARTY, 0},
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
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {EVENT_TOUCH, 60, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel0)},
    {EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel1)},
    {EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel2)},
    {EVENT_TOUCH, 63, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel3)},
    {EVENT_TOUCH, 64, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel4)},
    {EVENT_TOUCH, 65, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_DarkenToLevel5)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_StartReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_Events2[] = {
    {0x21, 10, CONDITION_ALWAYS, 10},
    {0x31, 11, CONDITION_ALWAYS, 11},
    {EVENT_TALK, ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI, EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {EVENT_TALK, ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, EVENT_SCRIPT(AltmillerCave_RepeatRockOrder)},
    {EVENT_TALK, ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED, EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {EVENT_TOUCH, 19, FLAG_BABI_CALLED_PARTY_BACK, EVENT_SCRIPT(AltmillerCave_BabiCallsPartyBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_INVISIBLE_BABI, FLAG_MET_INVISIBLE_BABI,
        EVENT_SCRIPT(AltmillerCave_MeetInvisibleBabi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INVISIBLE_BABI, FLAG_ALTMILLER_CAVE_MYSTIC_DRAUGHT, MSGNO(MsgAltmillerNeedDraught)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_INVISIBLE_BABI, FLAG_BABI_SAVED,
        EVENT_SCRIPT(AltmillerCave_RestoreBabiAndInviteParty)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SEARCH(SEARCH_CHEST), 100, 0xef5, 0x500005},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_Events3[] = {
    {0x21, 20, CONDITION_ALWAYS, 20},
    {0x31, 21, CONDITION_ALWAYS, 21},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTMILLER_CAVE_VIAL, FIND_ITEM(ITEM_VIAL)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_Events4[] = {
    {0x21, 30, CONDITION_ALWAYS, 30},
    {0x31, 31, CONDITION_ALWAYS, 31},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 72, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_WEST), 73, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 74, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {TOUCH_WALKING_INTO(FACING_EAST), 75, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_HopSelectedActor)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 71, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_MoveStagedActorToClearPosition)},
    {TOUCH_WALKING_INTO(FACING_EAST), 77, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_AdvanceObstacleAndCheckPassage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROOM4_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_UpdateMovedObstacle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, 1), ACTOR_ROOM4_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_CheckFrostTargetCollision)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM4_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltmillerCave_FreezePuddle)},
    {EVENT_TALK, ACTOR_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_StartDjinnBattle)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltmillerCave_Events5[] = {
    {0x21, 40, CONDITION_ALWAYS, 40},
    {0x31, 41, CONDITION_ALWAYS, 41},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {PSYNERGY_IN_AREA(ABILITY_RETREAT, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(AltmillerCave_BeginRetreat)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTMILLER_CAVE_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {SCENE_EVENTS_END},
};
// clang-format on

struct SceneEvent gAltmillerCave_Events6[] = {
    {0x21, 50, CONDITION_ALWAYS, 50},
    {EVENT_EXIT, 51, CONDITION_ALWAYS, 51},
    {EVENT_EXIT, 52, CONDITION_ALWAYS, 52},
    {EVENT_EXIT, 53, CONDITION_ALWAYS, 53},
    {EVENT_EXIT, 54, CONDITION_ALWAYS, 54},
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
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_StartReveal)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltmillerCave_EndReveal)},
    {SCENE_EVENTS_END},
};

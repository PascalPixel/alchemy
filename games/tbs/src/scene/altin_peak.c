#include "script.h"
#include "version.h"
// Altin Mine, the eleven rooms under Altin where the hydros statues flood
// the village. The party rides rolling logs, freezes puddles into pillars
// with Frost, fights the water beasts, lowers the water three times and
// finds the ancient ruins at the bottom.

#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/altin_peak.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "field_event.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "altin_peak_motion.h"
#include "call.h"
#include "field_effect.h"
#include "iwram_call.h"
#include "dma.h"

#include "types.h"

#include "facing_object.h"
#include "field_event.h"
#include "field_scene.h"

#include "battle_effect_runtime.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "objcmd.h"
#include "irq.h"

static void FieldScene_RunSharedSetPiece(s32 a0);
static void AltinPeak_ApplyEntryState(void);
static void AltinPeak_RestoreAreaThree(void);
static void AltinPeak_RestoreAreaFour(void);
static void AltinPeak_RestoreAreaFive(void);
static void AltinPeak_RestoreAreaSix(void);
static void AltinPeak_RestoreAreaSeven(void);
static void AltinPeak_PlaceFlaggedActors(void);
static void AltinPeak_RestoreAreaTen(void);
static void AltinPeak_RestoreAreaEleven(void);
static void AltinPeak_StartLeafGust(s32 x, s32 y, s32 z, s32 facing);
static void FieldScene_RunScene3a4SequenceG(void);
static void SceneState_ForwardByRuntimeSelector(union FieldObject *object);
static void FieldScene_RunActorTenFourStepSequence(void);
static void FieldScene_RunOpeningAuxiliarySequence(void);
static void FieldScene_RunScene3a4SequenceF(void);
static void FieldScene_RunScene3a4SequenceE(void);
static void RunEventScript01(void);
static void SceneActor_SetActor12ModeByActorZeroHeight(void);
static void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no);
static void AltinPeak_RunRollingObject(s32 id, s32 heading);
static void AltinPeak_StartPaletteAnim(void);

#define SCENE_MINE_ROOM(n) ((s32)&SceneId_AltinPeak##n)

// Each room's water beast fight returns the party to this entrance.
enum { ENTRANCE_AFTER_BATTLE = 99 };

// The log the party talks to in rooms 3, 5, 6 and 7, and the puddle Frost
// freezes.
enum { ACTOR_LOG = 8, ACTOR_DJINNI = 14 };

enum { SE_JUMP = 152, SE_LAND_ON_LOG = 159 };

enum { SE_WATER_DRAIN = 230, SE_WATER_GONE = 0x121 };

// The leader's jump animations.
enum { ANIM_LEADER_JUMP = 22, ANIM_LEADER_LEAP = 7 };

#define ABS(x) ((x) < 0 ? -(x) : (x))

enum { ITEM_FROST_JEWEL = 202 };

extern s32 gAltinPeak_LeafMode;
extern s32 gAltinPeak_LeafOrigin[3];

// Talking to the Djinni in room 7 makes it hop and starts the battle for
// Djinni 23.
static void Mine_TalkToDjinni(void) {
  Field_RequestDjinnBattle(ACTOR_DJINNI, 23);
}

// Shared source, included where its bytes sit in the ROM.
#include "common/object_facing.h"

// Update callback that keeps an object on actor 8, 12 pixels up.
// UB: update hooks return void. Plain void removes the native zero result
// and changes its return sequence (three instructions in tbs-en).
#ifdef UBFIX
static void Object_FollowActor8(union FieldObject *object) {
#else
static s32 Object_FollowActor8(union FieldObject *object) {
#endif
  struct FieldActor *dst = (struct FieldActor *)object;
  struct FieldActor *src = Object_GetById(8);

  dst->x.fixed = src->x.fixed;
  dst->y.fixed = 0xFFF40000;
  dst->z.fixed = src->z.fixed;
#ifndef UBFIX
  return 0;
#endif
}

// Script callback for a flying spark: moves by its velocity, grows by its z
// velocity and falls under its gravity.
static s32 Spark_Fly(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s32 rise;
  s32 growth;

  actor->x.fixed += actor->velocityX;
  rise = actor->velocityY;
  actor->y.fixed += rise;
  growth = actor->velocityZ;
  actor->scaleX += growth;
  actor->scaleY += growth;
  actor->velocityY = rise - actor->gravity;
  return 0;
}

// Returns the entrance table of the room being entered.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_MINE_ROOM(1)) {
    return gAltinPeakEntrances1;
  }
  if (scene == SCENE_MINE_ROOM(2)) {
    return gAltinPeakEntrances2;
  }
  if (scene == SCENE_MINE_ROOM(3)) {
    return gAltinPeakEntrances3;
  }
  if (scene == SCENE_MINE_ROOM(4)) {
    return gAltinPeakEntrances4;
  }
  if (scene == SCENE_MINE_ROOM(5)) {
    return gAltinPeakEntrances5;
  }
  if (scene == SCENE_MINE_ROOM(6)) {
    return gAltinPeakEntrances6;
  }
  if (scene == SCENE_MINE_ROOM(7)) {
    return gAltinPeakEntrances7;
  }
  if (scene == SCENE_MINE_ROOM(8)) {
    return gAltinPeakEntrances8;
  }
  if (scene == SCENE_MINE_ROOM(9)) {
    return gAltinPeakEntrances9;
  }
  if (scene == SCENE_MINE_ROOM(10)) {
    return gAltinPeakEntrances10;
  }
  if (scene == SCENE_MINE_ROOM(11)) {
    return gAltinPeakEntrances11;
  }
  return gAltinPeakEntrancesOther;
}

// Rooms 9 and 10 have regions.
const struct SceneRegion *Scene_GetRegions(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_MINE_ROOM(9)) {
    return (const struct SceneRegion *)gAltinPeakRegions9;
  }
  if (scene == SCENE_MINE_ROOM(10)) {
    return (const struct SceneRegion *)gAltinPeakRegions10;
  }
  return 0;
}

// All the rooms share one exit table.
const u32 *Scene_GetExits(void) { return gAltinMine_Exits; }

// Returns the actor placements of the room being entered.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_MINE_ROOM(1)) {
    return gAltinPeakPlacements1;
  }
  if (scene == SCENE_MINE_ROOM(3)) {
    return gAltinPeakPlacements3;
  }
  if (scene == SCENE_MINE_ROOM(5)) {
    return gAltinPeakPlacements5;
  }
  if (scene == SCENE_MINE_ROOM(6)) {
    return gAltinPeakPlacements6;
  }
  if (scene == SCENE_MINE_ROOM(7)) {
    return gAltinPeakPlacements7;
  }
  if (scene == SCENE_MINE_ROOM(8)) {
    return gAltinPeakPlacements8;
  }
  if (scene == SCENE_MINE_ROOM(9)) {
    return gAltinPeakPlacements9;
  }
  if (scene == SCENE_MINE_ROOM(10)) {
    return gAltinPeakPlacements10;
  }
  if (scene == SCENE_MINE_ROOM(11)) {
    return gAltinPeakPlacements11;
  }
  return gAltinPeakPlacementsOther;
}

// Talking to a log: the party leader jumps onto it, and once the leader has
// landed on top the log rolls away with the leader riding it.
static void Mine_RideLog(s32 log) {
  struct FieldActor *logActor;
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  logActor = Object_GetById(ACTOR_LOG);
  Battle_ResetEffectCounter();
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_JUMP);
  Engine_EventWait(10);
  Engine_AudioPlayCue(SE_JUMP);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x33333, 0x19999);
  // Leap by the height between leader and log, in quarter pixels, plus 4.
  leader->velocityY =
      ((ABS(logActor->y.fixed - leader->y.fixed) >> 14) << 14) + 0x40000;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  Engine_ObjectSetPosition(leader, logActor->x.fixed, logActor->y.fixed,
                           logActor->z.fixed);
  Engine_TaskWait(10);
  leader->sprite->priority = 3;
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);

  // Wait until the leader has come down onto the log.
  for (;;) {
    if (!((logActor->y.fixed >> 14) < (leader->y.fixed >> 14)))
      break;
    Engine_TaskWait(1);
  }
  Engine_EventEnd();
  Engine_AudioPlayCue(SE_LAND_ON_LOG);
  AltinPeak_RunRollingObject(log, 0);
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
}

// Talking to (or reading the mind of) a room's water beast starts its
// battle; afterwards the party returns to the room's battle entrance.
static void Mine_FightRoom1Beast(void) {
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(1), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 2);
}

static void Mine_FightRoom3Beast(void) {
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(3), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 2);
}

static void Mine_FightRoom5Beast(void) {
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(5), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 2);
}

// Move on room 7's block: the cell it left becomes free again and the cell
// it now stands on (column 24 or 25 of row 26) takes the blocking
// attributes kept at (0, 0).
static void Mine_MoveRoom7Block(void) {
  struct FieldActor *block = Object_GetById(10);

  if (block != 0) {
    s32 column;

    Map_CopyCellAttributes(24, 27, 2, 1, 24, 26);
    column = block->x.fixed >> 20;
    if (column == 25) {
      Map_CopyCellAttributes(0, 0, 1, 1, column, 26);
    } else {
      Map_CopyCellAttributes(0, 0, 1, 1, 24, 26);
    }
    Engine_ActorSetSpriteFlags(block, 0);
    block->motionFlags = 0;
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
}

// Frost on a puddle freezes it into an ice pillar the party can stand on:
// the puddle's cell takes the pillar's attributes, kept elsewhere on the
// map, and the pillar is drawn 32 pixels lower, under the party. Each
// room's puddle has its own copy, setting its own scene flag.
static void Mine_FreezeRoom1Puddle(void) {

  struct FieldActor *puddle;

  puddle = Object_GetById(8);
  Map_CopyCellAttributes(7, 13, 1, 1, 9, 13);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

// Frost on room 1's wide puddle, three cells across: the cells take the
// ice's attributes and the ice is left to fall to the ground.
static void Mine_FreezeRoom1WidePuddle(void) {

  struct FieldActor *puddle;

  puddle = Object_GetById(9);
  Map_CopyCellAttributes(29, 1, 3, 1, 17, 13);
  if (puddle != 0) {
    puddle->motionFlags = ACTOR_FALLS;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
}

static void Mine_FreezeRoom7Puddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(9);
  Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

static void Mine_FreezeRoom5Puddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(9);
  Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

static void Mine_FreezeRoom6Puddle(void) {

  struct FieldActor *puddle;

  puddle = Object_GetById(9);
  Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
}

static void Mine_FreezeRoom7SecondPuddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(11);
  Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
}

static void Mine_FreezeRoom7ThirdPuddle(void) {

  struct FieldActor *puddle;

  puddle = Object_GetById(12);
  Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SCENE_TEMP_04);
}

// Interrupt handler while the water layer vanishes: each time, background 1
// (the water) stays on only when a random percentage reaches the timer, so
// as the timer climbs to 100 the water flickers away.
static void Water_FlickerOut(void) {
#ifdef MODERN
  // modern GCC holds one type for a name in a file
  extern u16 gAltinPeak_RiseTimer_modern __asm__("gAltinPeak_RiseTimer");
#define gAltinPeak_RiseTimer gAltinPeak_RiseTimer_modern
#else
  extern u16 gAltinPeak_RiseTimer;
#endif

  volatile u16 *reg = &REG_DISPCNT;
  s16 v = *reg & 0xfdff;

  if ((u32)(Engine_RandomNext() * 100) >> 16 >= gAltinPeak_RiseTimer) {
    s32 k = 0x200;

    v |= k;
  }
  {
    u32 t = (u16)v;

    *reg = t;
  }
}
#ifdef MODERN
#undef gAltinPeak_RiseTimer
#endif

// The first drain, after the Frost Jewel is found: the screen shakes, the
// water layer sinks 24 pixels, one step every 4 frames (a puddle growing
// from a tenth of its size partway down), then flickers away over 100
// frames. The way on opens and the drain is remembered.
static void Mine_DrainFirstWater(void) {
  extern u8 gAltinPeak_RiseTimer[];

  s32 step;
  struct FieldActor *puddle;
  u8 *mapWork;
  u8 *waterLayer;
  s32 timer;

  mapWork = gMapWork[0];
  Event_PlayCue(SE_WATER_DRAIN);
  Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
  Engine_EventWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = mapWork + 356;
  for (step = 0; step <= 23; step++) {
    *(s32 *)(waterLayer + 12) -= 0x10000;
    Engine_TaskWait(4);
    if (step == 8) {
      puddle = Object_GetById(8);
      puddle->scaleX = 0x1999;
      puddle = Actor_Get(8);
      puddle->scaleY = 0x1999;
      Actor_SetPosition(8, PIXELS(152), PIXELS(216));
      Actor_SetActionScript(8, gAltinPeak_PuddleGrowScript);
    }
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  *(u16 *)gAltinPeak_RiseTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = *(u16 *)gAltinPeak_RiseTimer + 1;
    *(u16 *)gAltinPeak_RiseTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Event_PlayCue(SE_WATER_GONE);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(30);
  Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
  GameFlag_Set(FLAG_ALTIN_MINE_FIRST_DRAIN);
}

// The second drain: the same sinking and flicker over the cells the water
// leaves behind, which then take their dry form.
static void Mine_DrainSecondWater(void) {
  extern u8 gAltinPeak_RiseTimer[];

  s32 step;
  u8 *mapWork;
  u8 *waterLayer;
  s32 timer;

  mapWork = gMapWork[0];
  Map_CopyCells(93, 41, 16, 4, 77, 28);
  Event_PlayCue(SE_WATER_DRAIN);
  Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
  Engine_EventWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = mapWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= 0x10000;
    Engine_TaskWait(4);
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  *(u16 *)gAltinPeak_RiseTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = *(u16 *)gAltinPeak_RiseTimer + 1;
    *(u16 *)gAltinPeak_RiseTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Event_PlayCue(SE_WATER_GONE);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(30);
  Map_CopyCells(77, 41, 16, 4, 77, 28);
  GameFlag_Set(FLAG_ALTIN_MINE_SECOND_DRAIN);
}

// The third drain, likewise.
static void Mine_DrainThirdWater(void) {
  extern u8 gAltinPeak_RiseTimer[];

  s32 step;
  u8 *mapWork;
  u8 *waterLayer;
  s32 timer;

  mapWork = gMapWork[0];
  Map_CopyCellsTo(113, 31, 103, 17, 1, 1);
  Map_CopyCellsTo(111, 32, 104, 18, 3, 2);
  Map_CopyCellsTo(64, 32, 103, 18, 1, 2);
  Event_PlayCue(SE_WATER_DRAIN);
  Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
  Engine_EventWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = mapWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= 0x10000;
    Engine_TaskWait(4);
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  *(u16 *)gAltinPeak_RiseTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = *(u16 *)gAltinPeak_RiseTimer + 1;
    *(u16 *)gAltinPeak_RiseTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Event_PlayCue(SE_WATER_GONE);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(30);
  Map_CopyCellsTo(103, 14, 103, 17, 4, 3);
  GameFlag_Set(FLAG_ALTIN_MINE_THIRD_DRAIN);
}

static void FieldScene_RunScene3a4SequenceB(void) {
  extern u8 gAltinPeak_RiseTimer[];

  if (GameFlag_IsSet(0x323) != 0) {
    Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
    GameFlag_Clear(0x323);
  } else {
    Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
    GameFlag_Set(0x323);
  }
}

static void EV83_SEARCH(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(125);
  FieldScene_RunScene3a4SequenceB();
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

static void FieldScene_RunScene3a4SequenceA(void) {
  extern u8 gAltinPeak_RiseTimer[];

  if (GameFlag_IsSet(0x325) != 0) {
    Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
    GameFlag_Clear(0x325);
  } else {
    Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
    GameFlag_Set(0x325);
  }
}

static void EV50_SEARCH_5(void) {
  extern u8 gAltinPeak_RiseTimer[];

  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(125);
  FieldScene_RunScene3a4SequenceA();
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

static void EV50_SEARCH_6(void) {
  extern u8 gAltinPeak_RiseTimer[];

  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Event_PlayCue(125);
  if (GameFlag_IsSet(0x326) != 0) {
    Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
    GameFlag_Clear(0x326);
  } else {
    Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
    GameFlag_Set(0x326);
  }
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

static void EV50_SEARCH_7(void) {
  extern u8 gAltinPeak_RiseTimer[];

  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Event_PlayCue(125);
  if (GameFlag_IsSet(0x327) != 0) {
    Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
    GameFlag_Clear(0x327);
  } else {
    Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
    GameFlag_Set(0x327);
  }
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

static void EV10_TOUCH_7(void) { Object_GetById(10)->priorityFlags = 3; }

static void EV11_TOUCH_7(void) { Object_GetById(10)->priorityFlags = 1; }

static void EV12_TOUCH(void) {
  extern u8 gAltinPeak_RiseTimer[];

  Engine_EventBegin();
  Map_CopyCellAttributes(24, 27, 2, 1, 24, 26);
  Event_PlayCue(185);
  Actor_SetSpeed(10, 0x3333, 0x1999);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
  Object_GetById(10)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 0x190, 0x1a8);
  Actor_SetDestination(10, 0x198, 0x1a8);
  Engine_ActorWaitForMove(10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Mine_MoveRoom7Block();
  Engine_EventEnd();
}

static void EV11_PSYNERGY_9(void) {
  struct FieldActor *actor;
  struct FieldSprite *sprite;
  s32 x;
  s32 y;
  s32 c;
  s32 s;
  u16 step;

  actor = Actor_Get(10);
  sprite = actor->sprite;
  x = actor->x.fixed;
  y = actor->y.fixed;
  Event_Begin();
  Event_PlayCue(141);
  Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
  Event_Wait(10);
  Event_PlayCue(0x121);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Event_Wait(20);

  step = 0;
  while (1) {
    step += 8;
    sprite->rotation += step;
    c = Math_Cos(sprite->rotation + 0x4000);
    actor->x.fixed = (c << 4) + x;
    if (sprite->rotation > 0x8fff) {
      break;
    }
    Task_Wait(1);
  }
  step = 0;
  while (1) {
    step += 8;
    sprite->rotation -= step;
    c = Math_Cos(sprite->rotation + 0x4000);
    actor->x.fixed = (c << 4) + x;
    if (sprite->rotation <= 0x7000) {
      break;
    }
    Task_Wait(1);
  }
  step = 8;
  while (1) {
    step += step >> 3;
    sprite->rotation += step;
    c = Math_Cos(sprite->rotation + 0x4000);
    s = Math_Sin(sprite->rotation + 0x8000);
    actor->x.fixed = (c << 4) + x;
    if (sprite->rotation > 0x8000) {
      actor->y.fixed = y - (s << 3);
    }
    if (sprite->rotation + step > 0xbfff) {
      break;
    }
    Task_Wait(1);
  }
  Task_Wait(1);
  sprite->rotation = 0xc000;
  Event_PlayCue(183);
  Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
  Event_Wait(20);
  Event_PlayCue(0x121);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  FieldScene_RunSharedSetPiece(5);
  Event_End();
}

union TimedActor {
  struct FieldActor actor;
  struct SceneMotion motion;
};

static void FieldScene_RunSharedSetPiece(s32 a0) {
  struct FieldActor *actor0;
  struct FieldActor *actor8;
  struct FieldActor *actor9;
  struct FieldActor *actor10;
  struct FieldActor *actor;

  actor0 = Actor_Get(ACTOR_PARTY_LEADER);
  actor8 = Actor_Get(8);
  actor9 = Actor_Get(9);
  actor10 = Actor_Get(10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Engine_EventWait(40);
  Camera_SetSpeed(0x10000, 0x2000);
  Camera_MoveTo(0x3100000, -1, 0x740000, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x318, 140);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 100);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
  Event_PlayCue(183);
  Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
  Engine_EventWait(20);
  actor9->scaleX = 0x13333;
  actor9->scaleY = 0x13333;
  actor9->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  actor9->update = Object_FollowActor8;
#else
  actor9->update = (void (*)(union FieldObject *))Object_FollowActor8;
#endif
  Engine_ActorSetAnimation(8, 4);
  actor8->bounce = 0x8000;
  actor8->x.fixed = 0x3120000;
  actor8->y.fixed = 0x200000;
  actor8->z.fixed = 0x5a0000;
  actor8->scaleX = 0x20000;
  actor8->scaleY = 0x20000;
  Engine_EventWait(10);
  Event_PlayCue(183);
  Work_SetValuesIfNonNegative(0x40000, 0x20000, 0x10000);
  Engine_EventWait(20);
  actor10->x.fixed += 0xe0000;
  actor10->y.fixed += -0x80000;
  actor10->sprite->rotation = 0xc000;
  Event_PlayCue(107);
  Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 80);
  Event_PlayCue(55);
  Work_SetValuesIfNonNegative(0x10000, 0x30000, 0x10000);
  Engine_ActorSetSpritePriority(8, 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
  actor0->timer = 0;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gAltinPeak_LeaderRideScript);
  if (GameFlag_IsSet(0x205) != 0) {
    Actor_SetPosition(GARET, 0x36e0000, 0x2100000);
    actor = Actor_Get(GARET);
    actor->facing = FACING_SOUTH + FACING_STEP;
  }
  Camera_SetSpeed(0x14000, 0x2800);
  Camera_MoveTo(0x3120000, -1, 0x22c0000, 1);
  Engine_EventWait(a0);
  Engine_ActorSetSpritePriority(8, 1);
  Actor_SetSpeed(8, 0x195c2, 0xcae1);
  actor8->timer = 0;
  Actor_SetActionScript(8, gAltinPeak_LogRideScript);
  do {
    Engine_TaskWait(1);
  } while (actor0->timer == 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
  do {
    Engine_TaskWait(1);
  } while (actor8->timer == 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_PlayCue(0x121);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  actor9->x.fixed = 0x3120000;
  actor9->update = NULL;
  actor9->z.fixed = 0x26a0000;
  actor9->y.fixed = -0x400000;
  Actor_SetSpeed(8, 0x19999, 0xcccc);
  actor8->bounce = 0x1999;
  actor8->gravity = 0x3333;
  actor8->velocityY = 0x40000;
  Actor_MoveToAndWait(8, 0x312, 0x25c);
  Actor_SetSpeed(8, 0x33333, 0x19999);
  Actor_SetDestination(8, 0x312, 0x284);
  Engine_EventWait(15);
  Work_SetValuesIfNonNegative(0x50000, 0x70000, 0x10000);
  Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
  Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
  Actor_SetPosition(8, 0, 0);
  Actor_SetPosition(9, 0, 0);
  FieldScheduler_AddCallback(FieldScene_RunScene3a4SequenceG, 0xc80);
  Engine_EventWait(80);
  Engine_TaskRemoveCallback(FieldScene_RunScene3a4SequenceG);
  Engine_EventWait(60);
  Event_PlayCue(17);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(120);
  if (GameFlag_IsSet(0x205) != 0) {
    Actor_SetSpeed(GARET, 0x10000, 0x8000);
    Actor_WalkTo(GARET, 0x338, 0x22e);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x356, 0x248);
  if (GameFlag_IsSet(0x205) != 0) {
    Engine_ActorSetAnimation(GARET, 1);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(GARET, 0x102, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
  Camera_MoveTo(0x3140000, -0x400000, 0x2620000, 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(148);
  Engine_EventWait(240);
  if (GameFlag_IsSet(0x205) != 0) {
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0x3560000, 0, 0x2480000, 1);
    Engine_CameraWaitForMove();
    Actor_WalkToAndWait(GARET, 0x348, 0x228);
    Actor_WalkToAndWait(GARET, 0x356, 0x232);
    Engine_ActorSetAnimation(GARET, 2);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
  }
  Audio_PlayCueFromEventWork();
  GameFlag_Set(0x908);
}

static void EV12_SEARCH(void) {
  struct FieldActor *record;
  s32 msg;

  Engine_EventBegin();
  msg = MsgAltinWatchFallingRocks;
  Engine_MessageShowCentered(msg, 1);
  if (GameFlag_IsSet(0x908) == 0 && GameFlag_IsSet(0xf14) == 0) {
    GameFlag_Set(0x205);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x316, 140);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x30c, 140);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
      Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(GARET, 0xcccc, 0x6666);
    Actor_WalkToAndWait(GARET, 0x320, 140);
    Actor_FaceDirection(GARET, 0xc000, 20);
    Engine_EventSetMessage(msg + 1);
    Engine_ActorSetAnimation(GARET, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GARET, 0, 10);
    Engine_ActorJump(GARET, 6, 0);
    Actor_SetSpeed(GARET, 0x19999, 0xcccc);
    Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(GARET, 0x318, 110);
    Engine_EventWait(1);
    Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_PlayCue(161);
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(GARET, 0x318, 120);
    Engine_EventWait(1);
    {
      struct FieldActor *record = Actor_Get(GARET);

      record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
    }
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(80);
    Event_PlayCue(141);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GARET, 0x101, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(GARET, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(GARET, 0x8000, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
    Actor_FaceDirection(GARET, 0, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GARET, 0xc000, 40);
    Actor_ShowEmote(GARET, 0x102, 60);
    Actor_FaceDirection(GARET, 0x4000, 20);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Event_ShowMessageAndWait(GARET, 0, 10);
    Actor_SetSpeed(GARET, 0x28000, 0x14000);
    Engine_ActorSetAnimation(GARET, 5);
    Actor_MoveToAndWait(GARET, 0x31c, 138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_MoveToAndWait(GARET, 0x324, 140);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_MoveToAndWait(GARET, 0x324, 166);
    Actor_MoveToAndWait(GARET, 0x2fc, 166);
    Actor_MoveToAndWait(GARET, 0x2fc, 198);
    Actor_MoveToAndWait(GARET, 0x312, 198);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_MoveToAndWait(GARET, 0x312, 246);
    Engine_ActorSetAnimation(GARET, 1);
    Actor_SetPosition(GARET, 0, 0);
    Engine_EventWait(40);
    FieldScene_RunSharedSetPiece(10);
  }
  Engine_EventEnd();
}

// Returns the event table of the room being entered; room 1 has its own
// once the first drain has lowered its water.
const struct SceneEvent *Scene_GetEvents(void) {
  const struct SceneEvent *table;

  if (gGameState.scene == SCENE_MINE_ROOM(1)) {
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
      table = gAltinPeakEvents1Drained;
    } else {
      table = gAltinPeakEvents1;
    }
  } else if (gGameState.scene == SCENE_MINE_ROOM(2)) {
    table = gAltinPeakEvents2;
  } else if (gGameState.scene == SCENE_MINE_ROOM(3)) {
    table = gAltinPeakEvents3;
  } else if (gGameState.scene == SCENE_MINE_ROOM(4)) {
    table = gAltinPeakEvents4;
  } else if (gGameState.scene == SCENE_MINE_ROOM(5)) {
    table = gAltinPeakEvents5;
  } else if (gGameState.scene == SCENE_MINE_ROOM(6)) {
    table = gAltinPeakEvents6;
  } else if (gGameState.scene == SCENE_MINE_ROOM(7)) {
    table = gAltinPeakEvents7;
  } else if (gGameState.scene == SCENE_MINE_ROOM(8)) {
    table = gAltinPeakEvents8;
  } else if (gGameState.scene == SCENE_MINE_ROOM(9)) {
    table = gAltinPeakEvents9;
  } else if (gGameState.scene == SCENE_MINE_ROOM(10)) {
    table = gAltinPeakEvents10;
  } else if (gGameState.scene == SCENE_MINE_ROOM(11)) {
    table = gAltinPeakEvents11;
  } else {
    table = gAltinPeakEventsOther;
  }
  return table;
}

static void SceneMotion_UpdateTimedActor(union FieldObject *object) {
  struct SceneMotion *work = (struct SceneMotion *)object;
  if (work->delay != 0) {
    if (--work->delay == 1)
      Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  }
  if (work->velocity == 0) {
    Object_SetMode(work, 1);
    work->y += -0x18000;
    if (work->y < work->ground) {
      if (work->active != 0) {
        Engine_AudioPlayCue(229);
        work->active = 0;
        work->delay = 4;
        Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
      }
      work->y = work->ground;
    }
    work->state = 1;
  } else {
    work->state = 0;
  }
  if (work->timer == 0) {
    Engine_AudioPlayCue(152);
    work->active = 1;
    Object_SetMode(work, 2);
    work->velocity = 0x30000;
  }
  if (++work->timer == 60)
    work->timer = 0;
}

static void EV10_TOUCH_1(void) {
  union TimedActor *rec3;
  struct FieldActor *rec7;
  struct FieldActor *record;
  s32 v6;

  rec3 = (union TimedActor *)Object_GetById(10);
  Engine_EventBegin();
  Engine_CameraSetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(0x12a0000, -1, 0x1510000, 1);
  Engine_CameraWaitForMove();
  Engine_AudioPlayCue(147);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(40);
  Engine_CameraSetSpeed(0xcccc, 0x1999);
  Engine_CameraMoveTo(0x1270000, 0x200000, 0xd40000, 1);
  rec3->motion.active = 0;
  rec3->motion.timer = 0;
  rec3->motion.delay = 0;
  rec3->actor.gravity = 0x6666;
  rec3->actor.update = SceneMotion_UpdateTimedActor;
  Actor_SetSpeed(10, 0x13333, 0x9999);
  Actor_MoveToAndWait(10, 0x134, 0x123);
  Engine_ActorMoveToAndWait(10, 0x137, 215);
  rec3->actor.update = 0;
  rec3->motion.state = 0;
  Engine_EventWait(16);
  Engine_ActorSetAnimation(10, 1);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(40);
  Actor_FaceDirection(10, 0xb000, 20);
  Actor_FaceDirection(10, 0x8000, 40);
  FieldScene_RunActorTenFourStepSequence();
  Engine_EventWait(40);
  Actor_FaceDirection(10, 0x5000, 20);
  Actor_FaceDirection(10, 0x3000, 40);
  Camera_MoveTo(0x14e0000, -1, 0xf40000, 1);
  rec3->motion.active = 0;
  rec3->motion.timer = 0;
  rec3->motion.delay = 0;
  rec3->actor.update = SceneMotion_UpdateTimedActor;
  Actor_MoveToAndWait(10, 0x140, 232);
  Actor_MoveToAndWait(10, 0x154, 0x106);
  Engine_ActorMoveToAndWait(10, 0x176, 0x106);
  rec3->actor.update = 0;
  Engine_EventWait(16);
  Engine_ActorSetAnimation(10, 1);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(20);
  Actor_FaceDirection(10, 0xf000, 20);
  Engine_ActorFaceDirection(10, 0xd000, 40);
  Engine_AudioPlayCue(153);
  record = Object_GetById(10);
  record->velocityY = 0x40000;
  Engine_ActorSetAnimation(10, 2);
  Engine_ActorMoveToAndWait(10, 0x17c, 248);
  Engine_EventWait(10);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(10, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(10, 0xb000, 20);
  Actor_FaceDirection(10, 0x8000, 40);
  Camera_MoveTo(0x1300000, -1, 0xd70000, 1);
  rec3->motion.active = 0;
  rec3->motion.timer = 0;
  rec3->motion.delay = 0;
  rec3->actor.update = SceneMotion_UpdateTimedActor;
  Actor_MoveToAndWait(10, 0x149, 219);
  rec3->actor.update = 0;
  Engine_ActorSetAnimation(10, 1);
  Engine_EventWait(16);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(10, 0x8000, 40);
  Object_GetById(9)->motionFlags = 0;
  Map_CopyCellAttributes(3, 0, 1, 1, 17, 13);
  Map_CopyCellAttributes(3, 0, 1, 1, 18, 13);
  Map_CopyCellAttributes(3, 0, 1, 1, 19, 13);
  Engine_ActorSetSpeed(10, 0x16666, 0xb333);
  record = Object_GetById(10);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_AudioPlayCue(153);
  v6 = 160;
  record = Object_GetById(10);
  record->velocityY = (v6 << 11);
  Engine_ActorSetAnimation(10, 3);
  Actor_MoveToAndWait(10, 0x127, 215);
  Engine_ActorSetAnimation(10, 1);
  record = Object_GetById(10);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(40);
  Engine_AudioPlayCue(153);
  record = Object_GetById(10);
  record->velocityY = (v6 << 11);
  Engine_ActorSetAnimation(10, 3);
  Actor_MoveToAndWait(10, 0x104, 215);
  Engine_ActorSetAnimation(10, 1);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(40);
  Actor_FaceDirection(10, 0x5000, 20);
  Engine_ActorFaceDirection(10, 0x3000, 20);
  Engine_AudioPlayCue(147);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(40);
  Map_CopyCellAttributes(4, 0, 1, 1, 17, 13);
  Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
  Map_CopyCellAttributes(4, 0, 1, 1, 19, 13);
  rec7 = Object_GetById(0);
  Engine_CameraSetSpeed(0x4cccc, 0x9999);
  Engine_CameraMoveTo(rec7->x.fixed, rec7->y.fixed, rec7->z.fixed, 1);
  Engine_CameraWaitForMove();
  FieldObject_SetTargetAndCallback(10, 0x10000, gAltinPeak_ActorScript);
  Engine_GameFlagSet(0x904);
  Engine_EventEnd();
}

static void EV11_TOUCH_1(void) {
  union TimedActor *actor;
  struct FieldActor *record;
  u32 n;

  actor = (union TimedActor *)Object_GetById(10);
  Engine_EventBegin();
  ObjectMotion_EnableActionAndResetMotion(10);
  Camera_SetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(0x1170000, 0x400000, 0xd80000, 1);
  Engine_CameraWaitForMove();
  Engine_AudioPlayCue(147);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(10, 0x3000, 20);
  Actor_FaceDirection(10, 0x5000, 20);
  Actor_FaceDirection(10, 0x8000, 40);
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(0x800000, 0x400000, 0xca0000, 1);
  actor->motion.active = 0;
  actor->motion.timer = 0;
  actor->motion.delay = 0;
  actor->actor.gravity = 0x6666;
  actor->actor.update = SceneMotion_UpdateTimedActor;
  Actor_SetSpeed(10, 0x13333, 0x9999);
  Actor_MoveToAndWait(10, 212, 200);
  Actor_MoveToAndWait(10, 103, 200);
  actor->actor.update = NULL;
  actor->motion.state = 0;
  Engine_EventWait(10);
  Engine_ActorSetAnimation(10, 1);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(20);
  Actor_FaceDirection(10, 0x5000, 40);
  Object_GetById(10)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(10, 0x13333, 0x9999);
  record = Object_GetById(10);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_AudioPlayCue(153);
  record = Object_GetById(10);
  record->velocityY = 0x40000;
  Actor_SetAnimation(10, 3);
  Engine_ActorMoveToAndWait(10, 86, 214);
  Engine_ActorSetAnimation(10, 1);
  record = Object_GetById(10);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_EventWait(10);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x20000, 0, 0x10000);
  Engine_EventWait(8);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(40);
  Object_GetById(10)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(10, 0x3000, 20);
  Engine_ActorFaceDirection(10, 0, 40);
  actor->motion.active = 0;
  actor->motion.timer = 0;
  actor->motion.delay = 0;
  actor->actor.update = SceneMotion_UpdateTimedActor;
  Actor_SetSpeed(10, 0x13333, 0x9999);
  Actor_MoveToAndWait(10, 120, 215);
  actor->actor.update = NULL;
  actor->motion.state = 0;
  Engine_ActorSetAnimation(10, 1);
  Engine_EventWait(16);
  Engine_AudioPlayCue(229);
  Work_SetValuesIfNonNegative(0x10000, 0, 0x10000);
  Engine_EventWait(4);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(10, 0, 10);
  Engine_AudioPlayCue(147);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(80);
  Engine_ActorSetAnimation(10, 3);
  AltinPeak_StartLeafGust(0x820000, 0, 0xa80000, 0);
  Engine_EventWait(60);
  for (n = 0; n < 60 && gKeysPressed == 0; n++) {
    Engine_EventWait(1);
  }
  actor = (union TimedActor *)Object_GetById(0);
  Camera_SetSpeed(0x4cccc, 0x9999);
  Engine_CameraMoveTo(actor->actor.x.fixed, actor->actor.y.fixed,
                      actor->actor.z.fixed, 1);
  Engine_CameraWaitForMove();
  GameFlag_Set(0x905);
  Engine_EventEnd();
}

// Sets up the room being entered: puzzles already solved and the water
// levels already lowered. Returns 0.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == SCENE_MINE_ROOM(1)) {
    AltinPeak_ApplyEntryState();
  } else if (gGameState.scene == SCENE_MINE_ROOM(3)) {
    AltinPeak_RestoreAreaThree();
  } else if (gGameState.scene == SCENE_MINE_ROOM(4)) {
    AltinPeak_RestoreAreaFour();
  } else if (gGameState.scene == SCENE_MINE_ROOM(5)) {
    AltinPeak_RestoreAreaFive();
  } else if (gGameState.scene == SCENE_MINE_ROOM(6)) {
    AltinPeak_RestoreAreaSix();
  } else if (gGameState.scene == SCENE_MINE_ROOM(7)) {
    AltinPeak_RestoreAreaSeven();
  } else if (gGameState.scene == SCENE_MINE_ROOM(9)) {
    AltinPeak_PlaceFlaggedActors();
  } else if (gGameState.scene == SCENE_MINE_ROOM(10)) {
    AltinPeak_RestoreAreaTen();
  } else if (gGameState.scene == SCENE_MINE_ROOM(11)) {
    AltinPeak_RestoreAreaEleven();
  }
  return 0;
}

static void AltinPeak_ApplyEntryState(void) {
  s32 opened;
  s32 flag;
  struct FieldActor *actor;
  u8 *map;

  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gGameState.entrance == 99) {
      FieldScene_RunOpeningAuxiliarySequence();
    }
  }
  AltinPeak_StartPaletteAnim();
  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
    Engine_ActorSetPosition(8, 0, 0);
  } else {
    actor = Object_GetById(8);
    if (actor != 0) {
      actor->priorityFlags = 2;
      Engine_ActorSetSpriteFlags(actor, 0);
    }
  }
  actor = Object_GetById(9);
  if (actor != 0) {
    actor->priorityFlags = 2;
    Engine_ActorSetSpriteFlags(actor, 0);
  }
  opened = Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN);
  if (opened == 0) {
    Engine_ActorSetChildValue(10, 2);
    flag = Engine_GameFlagIsSet(0x905);
    if (flag != 0) {
      Engine_ActorSetAnimation(9, 0);
      actor = Object_GetById(9);
      actor->update = SceneState_ForwardByRuntimeSelector;
      Object_GetById(9)->motionFlags = opened;
      actor = Object_GetById(9);
      actor->y.fixed = 0x200000;
      Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
      Engine_ActorSetPosition(10, 0x780000, 0xd70000);
      actor = Object_GetById(10);
      actor->facing = opened;
      Engine_ActorSetAnimation(10, 3);
      AltinPeak_StartLeafGust(0x820000, 0, 0xa80000, 0);
      return;
    }
    if (Engine_GameFlagIsSet(0x904) == 0) {
      return;
    }
    Engine_ActorSetAnimation(9, 0);
    actor = Object_GetById(9);
    actor->update = SceneState_ForwardByRuntimeSelector;
    Object_GetById(9)->motionFlags = flag;
    actor = Object_GetById(9);
    actor->y.fixed = 0x200000;
    Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
    Actor_SetPosition(10, 0x1040000, 0xd70000);
    FieldObject_SetTargetAndCallback(10, 0x10000, gAltinPeak_ActorScript);
  } else {
    map = gMapWork[0];
    Engine_ActorSetPosition(10, 0, 0);
    Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    *(u16 *)(map + 20) &= 0xfdff;
    SceneActor_ClearCollisionFlagAndPlaceMarker(8);
    if (Engine_GameFlagIsSet(0x200) != 0) {
      Engine_ActorSetAnimation(8, 5);
      Map_CopyCellAttributes(7, 13, 1, 1, 9, 13);
      actor = Object_GetById(8);
      actor->y.fixed = 0;
      actor->priorityFlags |= 2;
    }
    SceneActor_ClearCollisionFlagAndPlaceMarker(9);
    if (Engine_GameFlagIsSet(0x201) != 0) {
      Engine_ActorSetAnimation(9, 5);
      Map_CopyCellAttributes(29, 1, 3, 1, 17, 13);
      actor = Object_GetById(9);
      actor->y.fixed = 0x200000;
      actor->priorityFlags |= 2;
    }
  }
}

static void AltinPeak_RestoreAreaThree(void) {

  if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
    Actor_SetPosition(9, 0, 0);
  } else {
    AltinPeak_StartPaletteAnim();
    if (GameFlag_IsSet(0x109) == 0 && gGameState.entrance == 99) {
      FieldScene_RunScene3a4SequenceF();
    } else {
      Map_CopyCellAttributes(38, 24, 1, 2, 37, 24);
      Map_CopyCellAttributes(44, 23, 1, 2, 45, 23);
      if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) == 0) {
        Actor_SetChildValue(9, 2);
        Engine_ActorSetAnimation(9, 3);
        AltinPeak_StartLeafGust(0xee0000, 0, 0x1a20000, 0x8000);
      }
    }
  }
  if (GameFlag_IsSet(0x323) != 0) {
    Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
  } else {
    Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
  }
}

static void AltinPeak_RestoreAreaFour(void) {
  extern u8 gAltinPeak_RiseTimer[];

  if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
  } else {
    Map_CopyCellAttributes(52, 42, 1, 1, 53, 42);
  }
  {
    s32 index = 225;
    if ((u32)((((u16 *)&gGameState)[index] - 6) << 16) <= 0x10000) {
      GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    }
  }
}

static void AltinPeak_RestoreAreaFive(void) {
  extern u8 gAltinPeak_RiseTimer[];

  struct FieldActor *record;

  if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
    Actor_SetPosition(10, 0, 0);
  } else {
    if (GameFlag_IsSet(0x109) == 0) {
      if (gGameState.entrance == 99) {
        FieldScene_RunScene3a4SequenceE();
      }
    }
    AltinPeak_StartPaletteAnim();
    if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) == 0) {
      Actor_SetChildValue(10, 2);
      Engine_ActorSetAnimation(10, 3);
      AltinPeak_StartLeafGust(0x2ec0000, 0x80000, 0x1180000, 0x8000);
    }
  }
  SceneActor_ClearCollisionFlagAndPlaceMarker(9);
  if (GameFlag_IsSet(0x200) != 0) {
    Engine_ActorSetAnimation(9, 5);
    Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
    {
      struct FieldActor *record = Actor_Get(9);
      record->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | record->priorityFlags;
    }
  }
  if (GameFlag_IsSet(0x325) != 0) {
    Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
  } else {
    Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
  }
}

static void AltinPeak_RestoreAreaSix(void) {
  extern u8 gAltinPeak_RiseTimer[];

  struct FieldActor *rec7;

  if (gGameState.entrance == 2) {
    if (GameFlag_IsSet(0x109) == 0) {
      Actor_SetPosition(8, 0x1660000, 0x680000);
    }
  }
  SceneActor_ClearCollisionFlagAndPlaceMarker(9);
  if (GameFlag_IsSet(0x200) != 0) {
    rec7 = Actor_Get(9);
    Engine_ActorSetAnimation(9, 5);
    Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
    {
      rec7->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | rec7->priorityFlags;
    }
  }
  if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    *(u16 *)(((u8 *)gMapWork[0]) + 20) &= ~0x200;
  }
  if (GameFlag_IsSet(0x326) != 0) {
    Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
  } else {
    Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
  }
}

static void AltinPeak_RestoreAreaSeven(void) {
  extern u8 gAltinPeak_RiseTimer[];

  struct FieldActor *record;

  record = Actor_Get(9);
  Engine_ActorSetSpriteFlags((s32)record, 0);
  Mine_MoveRoom7Block();
  SceneActor_ClearCollisionFlagAndPlaceMarker(9);
  if (GameFlag_IsSet(0x200) != 0) {
    Engine_ActorSetAnimation(9, 5);
    Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
    {
      struct FieldActor *record = Actor_Get(9);
      record->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | record->priorityFlags;
    }
  }
  SceneActor_ClearCollisionFlagAndPlaceMarker(11);
  if (GameFlag_IsSet(0x201) != 0) {
    Engine_ActorSetAnimation(11, 5);
    Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
    {
      struct FieldActor *record = Actor_Get(11);
      record->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | record->priorityFlags;
    }
  }
  SceneActor_ClearCollisionFlagAndPlaceMarker(12);
  if (GameFlag_IsSet(0x204) != 0) {
    Engine_ActorSetAnimation(12, 5);
    Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
    {
      struct FieldActor *record = Actor_Get(12);
      record->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | record->priorityFlags;
    }
  }
  FieldScheduler_AddCallback(SceneActor_SetActor12ModeByActorZeroHeight, 0xc80);
  if (GameFlag_IsSet(0x327) != 0) {
    Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
  } else {
    Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
  }
}

static void AltinPeak_PlaceFlaggedActors(void) {
  struct FieldActor *actor;

  actor = Object_GetById(10);
  Engine_ActorSetPosition(8, 0, 0);
  Engine_ActorSetPosition(9, 0, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
  actor->motionFlags = 0;
  actor->scaleX = 0xe666;
  actor->scaleY = 0x9999;
  actor->sprite->rotation = 0x8000;
  Object_GetById(12)->motionFlags = 0;
  Object_GetById(12)->y.fixed = -0x1c0000;
  if (GameFlag_IsSet(0x908)) {
    actor->x.fixed += 0xe0000;
    actor->y.fixed += -0x80000;
    actor->sprite->rotation = 0xc000;
  }
  if (Engine_GameFlagIsSet(0x908)) {
    Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
    Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
  if (gGameState.entrance == 6 &&
      !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
    Engine_EventBegin();
    Object_GetById(0)->y.fixed = -0x580000;
    Engine_CameraMoveTo(0x3180000, -0x580000, 0x2410000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventEnd();
  }
}

static void AltinPeak_RestoreAreaTen(void) {
  extern u8 gAltinPeak_RiseTimer[];

  s32 rec7;
  struct FieldActor *record;
  s16 flag;

  rec7 = Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED);
  if (rec7 != 0) {
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
  } else {
    record = Object_GetById(8);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetSpritePriority(9, 3);
    record = Object_GetById(9);
    Engine_ActorSetSpriteFlags(record, 0);
    Object_GetById(9)->collisionFlags = rec7;
  }
  flag = gGameState.entrance;
  if (flag == 1 || flag == 98) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      rec7 = Object_GetById(ACTOR_PARTY_LEADER);
      Engine_EventBegin();
      ((struct FieldActor *)rec7)->y.fixed = 0x100000;
      Engine_EventEnd();
    }
  } else if (flag == 99) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      RunEventScript01();
    }
  }
}

static void AltinPeak_RestoreAreaEleven(void) {
  extern u8 gAltinPeak_RiseTimer[];

  struct FieldActor *record;

  record = Actor_Get(9);
  Engine_ActorSetSpriteFlags(record, 0);
  if (gGameState.entrance == 2) {
    Actor_SetPosition(9, 0xb80000, 0x1480000);
  }
}

static void AltinPeak_InitLeaf(struct FieldActor *leaf) {
  leaf->motionFlags = 0;
  leaf->sprite->priority = 1;
  ObjectGroup_SetChildValue(leaf, 3);
  Engine_ActorSetSpriteFlags(leaf, 0);
  leaf->scaleX = 0x4ccc;
  leaf->scaleY = 0x4ccc;
}

static void AltinPeak_UpdateDriftingLeaf(union FieldObject *object) {
  struct FieldActor *leaf = &object->actor;

  switch (gAltinPeak_LeafMode) {
  case 0:
    leaf->x.fixed +=
        (leaf->timer << 12) +
        ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
    break;
  case 0x8000:
    leaf->x.fixed -=
        (leaf->timer << 12) +
        ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
    break;
  }
  if (leaf->timer <= 3) {
    switch (gAltinPeak_LeafMode) {
    case 0:
      leaf->x.fixed += 0x8000;
      break;
    case 0x8000:
      leaf->x.fixed -= 0x8000;
      break;
    }
    leaf->scaleX += 0x1999;
    leaf->scaleY -= 0xccc;
  } else {
    leaf->z.fixed += 0x13333;
    leaf->scaleX += 0x7ae;
    leaf->scaleY += 0x7ae;
  }
  if ((u32)(leaf->timer * Engine_RandomNext()) >> 16 == 0)
    ObjectGroup_SetChildValue(leaf, 7);
  if (leaf->timer != 0)
    leaf->timer -= 2;
  else
    leaf->timer = ((u32)(Engine_RandomNext() * 5) >> 16) * 2 + 2;
  if (--object->effect.countdown == 0)
    Engine_ObjectDispatchRelease(leaf);
}

static void AltinPeak_SpawnLeaf(void) {
  union FieldObject *leaf;

  if ((gFrameCount & 3) != 0)
    return;
  leaf = (union FieldObject *)Engine_ObjectCreate(222, gAltinPeak_LeafOrigin[0],
                                                  gAltinPeak_LeafOrigin[1],
                                                  gAltinPeak_LeafOrigin[2]);
  if (leaf == NULL)
    return;
  leaf->actor.timer = 30;
  leaf->actor.work = 1;
  leaf->effect.countdown = 20;
  AltinPeak_InitLeaf(&leaf->actor);
  leaf->effect.update = AltinPeak_UpdateDriftingLeaf;
  Object_SetMode(&leaf->actor, 1);
}

static void AltinPeak_StartLeafGust(s32 x, s32 y, s32 z, s32 facing) {
  gAltinPeak_LeafOrigin[0] = x;
  gAltinPeak_LeafOrigin[1] = y;
  gAltinPeak_LeafOrigin[2] = z;
  gAltinPeak_LeafMode = facing;

  BattleFx_SetQueuedSoundAndPlay(170);
  FieldScheduler_AddCallback(AltinPeak_SpawnLeaf, 0xc80);
}

static void FieldScene_RunScene3a4SequenceG(void) {
  struct FieldActor *spark;
  s32 scale;

  if (gFrameCount % 3 == 0) {
    spark = Object_Create(
        200, (((u32)(Engine_RandomNext() * 48) >> 16) << 16) + 0x2fd0000,
        -0x400000, 0x2600000);
    if (spark != NULL) {
      if (gFrameCount % 9 == 0) {
        if ((u32)(Engine_RandomNext() << 1) >> 16 != 0)
          Engine_AudioPlayCue(145);
        else
          Engine_AudioPlayCue(144);
      }
      spark->motionFlags = 0;
      scale = ((u32)(Engine_RandomNext() << 15) >> 16) + 0x4ccc;
      spark->gravity = 0x6666;
      spark->scaleY = scale;
      spark->scaleX = scale;
      spark->unk5d[4] = 1;
      Engine_ActorSetSpriteFlags(spark, 0);
      spark->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      spark->sprite->priority = 1;
      Object_SetMode(spark, 1);
      Engine_ObjectSetScript(spark, gAltinPeak_SparkScript);
      spark->velocityX = (((u32)(Engine_RandomNext() * 6) >> 16) - 3) << 16;
      spark->velocityY = 0x80000;
      spark->velocityZ = ((u32)(Engine_RandomNext() * 0x600) >> 16) - 0x300;
    }
  }
}

static void SceneState_ForwardByRuntimeSelector(union FieldObject *object) {
  struct FieldActor *arg = (struct FieldActor *)object;

  s32 selection = gFrameCount & 7;

  if (selection == 0) {
    ObjectGroup_SetChildValue(arg, 2);
  } else if (selection == 2) {
    ObjectGroup_SetChildValue(arg, 0);
  }
}

static void FieldScene_RunActorTenFourStepSequence(void) {
  Engine_PsynergyBegin(24, 1);
  Engine_PsynergySetTarget(10, 9);
  Engine_PsynergyRaiseHands();
  Engine_ActorSetChildValue(10, 2);
  Engine_PsynergyPlayEffect(1);
  Engine_ActorSetChildValue(10, 2);
  Engine_PsynergyLowerHands();
  Engine_ActorSetChildValue(10, 2);
  BattleEffect_CleanupSceneObjects();
  Engine_AudioPlayCue(288);
  Engine_ActorSetChildValue(10, 2);
}

static void FieldScene_RunOpeningAuxiliarySequence(void) {
  extern u8 gAltinPeak_RiseTimer[];

  struct FieldActor *rec7;

  rec7 = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Actor_SetPosition(10, 0, 0);
  Actor_SetPosition(8, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ItemShowFound(ITEM_FROST_JEWEL, 3);
  Engine_PartyGiveItem(ITEM_FROST_JEWEL, 0);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(0x640000, 0, 0xf90000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Mine_DrainFirstWater();
  Camera_MoveTo(rec7->x.fixed, rec7->y.fixed, rec7->z.fixed, 1);
  Engine_CameraWaitForMove();
  Event_LoadAreaScript(gAltinPeakEvents1Drained);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventEnd();
}

static void FieldScene_RunScene3a4SequenceF(void) {
  extern u8 gAltinPeak_RiseTimer[];

  Engine_EventBegin();
  Engine_ActorSetPosition(9, 0, 0);
  Engine_ActorSetPosition(8, 0x1480000, 0x1a80000);
  Engine_GameFlagSet(0x323);
  FieldScene_RunScene3a4SequenceB();
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Mine_DrainSecondWater();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventEnd();
}

static void FieldScene_RunScene3a4SequenceE(void) {
  extern u8 gAltinPeak_RiseTimer[];

  Engine_EventBegin();
  Engine_ActorSetPosition(10, 0, 0);
  Engine_ActorSetPosition(8, 0x1e80000, 0x8a0000);
  Engine_GameFlagSet(0x325);
  FieldScene_RunScene3a4SequenceA();
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Mine_DrainThirdWater();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventEnd();
}

static void SceneAudio_PlayCue183EverySixtyTicks(void) {
  gAltinPeak_CueTicks++;
  if (gAltinPeak_CueTicks == 60) {
    Engine_AudioPlayCue(183);
    gAltinPeak_CueTicks = 0;
  }
}

static void EV10_TOUCH_10(void) {
  Engine_EventBegin();
  Camera_SetSpeed(0x9999, 0x1333);
  Camera_MoveTo(0x1480000, -1, 0x570000, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
  Event_PlayCue(148);
  FieldScheduler_AddCallback(SceneAudio_PlayCue183EverySixtyTicks, 3200);
  Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
  Actor_SetSpeed(8, 0x1999, 0xccc);
  Actor_SetSpeed(9, 0x1999, 0xccc);
  Engine_ActorSetAnimation(8, 2);
  Actor_SetDestination(8, 328, 104);
  Actor_SetDestination(9, 328, 108);
  Engine_EventWait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 256, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorWaitForMove(8);
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(10), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 3);
}

static void SceneActor_SetActor8ModeByCounterBit(void) {

  if (((gFrameCount >> 1) & 1) != 0) {
    Engine_ActorSetChildValue(8, 7);
  } else {
    Engine_ActorSetChildValue(8, 6);
  }
}

static void RunEventScript01(void) {
  struct FieldActor *record;
  struct EventWork *work;
  SchedulerCallback update;
  const s32 *celebrate;
  const s32 *party;

  Engine_EventBegin();
  Actor_SetPosition(8, 0x1480000, 0x580000);
  Actor_SetPosition(9, 0x1480000, 0x580000);
  Engine_ActorSetAnimation(8, 0);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(IVAN, record->x.fixed, record->z.fixed);
  }
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(MIA, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(GARET, 0x9999, 0x4ccc);
  Actor_SetSpeed(IVAN, 0x9999, 0x4ccc);
  Actor_SetSpeed(MIA, 0x9999, 0x4ccc);
  Actor_SetActionScript(1, gAltinPeak_GaretScript);
  Actor_SetActionScript(2, gAltinPeak_IvanScript);
  Object_SetActionCallbackAndRefreshById(3, gAltinPeak_MiaScript);
  Actor_FaceDirection(GARET, 0xc000, 0);
  Actor_FaceDirection(IVAN, 0xc000, 0);
  Actor_FaceDirection(MIA, 0xc000, 40);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Actor_FaceDirection(GARET, 0xe000, 10);
  Engine_EventSetMessage(MsgAltinWeDidIsaacWeBeat);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Actor_FaceDirection(IVAN, 0xa000, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
  Event_OpenMessage(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(IVAN, 3);
  } else {
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Event_SkipMessages(1);
  }
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventSetMessage(MsgAltinGuardianStatuesWereCreatedLong);
  Event_ShowMessage(MIA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 10);
  Actor_ShowEmote(GARET, 0x102, 60);
  Actor_FaceDirection(GARET, 0x2000, 40);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 40);
  Event_PlayCue(190);
  Actor_SetChildValue(8, 7);
  Engine_EventWait(10);
  Event_PlayCue(0x121);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
  Actor_FaceDirection(GARET, 0xc000, 0);
  Actor_FaceDirection(IVAN, 0xc000, 0);
  Actor_FaceDirection(MIA, 0xc000, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
  Actor_ShowEmote(GARET, 0x100, 0);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Actor_ShowEmote(MIA, 0x100, 40);
  Event_PlayCue(103);
  update = SceneActor_SetActor8ModeByCounterBit;
  Task_AddCallback(update, 0xc80);
  celebrate = gAltinPeak_CelebrateScript;
  Actor_SetActionScript(9, celebrate);
  Object_SetActionCallbackAndRefreshById(8, (s32)celebrate);
  Engine_TaskRemoveCallback(update);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceDirection(GARET, 0xe000, 0);
  Actor_FaceDirection(IVAN, 0xa000, 20);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(GARET, 0xe000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 40);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Engine_EventWait(20);
  party = gAltinPeak_PartyScript;
  Actor_SetActionScript(GARET, party);
  Actor_SetActionScript(2, party);
  Object_SetActionCallbackAndRefreshById(3, (s32)party);
  Engine_EventWait(20);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  gEventWork->transitionFrames = 16;
  GameFlag_Set(FLAG_HYDROS_STATUES_DEFEATED);
  Engine_EventEnd();
}

UNUSED static void AltinPeak_RequestPsynergy(void) {
  gEventWork->psynergyRequest = 0x1018;
}

static void SceneActor_SetActor12ModeByActorZeroHeight(void) {

  if (Object_GetById(0)->y.fixed > 0x00300000) {
    {
      u8 *flag = &Object_GetById(11)->priorityFlags;
      s32 bit = 2;

      bit |= *flag;
      *flag = bit;
    }
    Engine_ActorSetSpritePriority(12, 3);
  } else {
    Engine_ActorSetSpritePriority(12, 2);
  }
}

static void SceneActor_ClearCollisionFlagAndPlaceMarker(s32 no) {
  struct FieldActor *record;

  record = Object_GetById(no);
  record->collisionFlags &= 0xfe;

  SetMapCellCollision(0, record->x.fixed, record->z.fixed, 255);
}

static void AltinPeak_SettleAndMountLeader(struct FieldActor *object) {
  s32 x;
  s32 z;
  s32 centerZ;
  struct FieldSprite *sprite;
  struct FieldActor *leader;

  Engine_AudioPlayCue(288);
  Engine_AudioPlayCue(232);
  x = object->x.fixed & 0xfff00000;
  centerZ = 0x80000;
  z = object->z.fixed & 0xfff00000;
  object->acceleration = 0x20000;
  x += centerZ;
  centerZ += z;
  Engine_ObjectSetPosition(object, x, object->y.fixed, centerZ);
  Engine_ObjectCommitPosition(object);
  object->terrainId = 0;
  object->x.fixed = x;
  object->z.fixed = centerZ;
  object->velocityX = 0;
  object->velocityZ = 0;
  Object_SetMode(object, 2);
  Engine_TaskWait(15);
  Object_SetMode(object, 1);
  Engine_TaskWait(30);
  sprite = object->sprite;
  sprite->partCount = 1;
  ResourceMetadata_ClearRecord(*(s32 *)((u8 *)sprite + 44));
  *(s32 *)((u8 *)sprite + 44) = 0;
  ((u8 *)sprite)[37] = 1;
  leader = Engine_ActorLookup(gGameState.selectedActor);
  Engine_AudioPlayCue(152);
  leader->x.fixed = x;
  leader->velocityY = 0x60000;
  leader->gravity = 0x10000;
  leader->z.fixed = centerZ;
  leader->sprite->priority = 0;
  Object_SetMode(leader, 7);
  Engine_ObjectSetPosition(leader, x, leader->y.fixed, z + 0x180000);
  leader->facing = 0x4000;
  Engine_TaskWait(20);
  leader->sprite->priority = 2;
  Engine_AudioPlayCue(159);
}

UNUSED static void AltinPeak_ReservedNoOp(void) {}

static void AltinPeak_TurnRollingObjectA(struct FieldActor *object) {
  union FieldCoordinate *p;
  union FieldCoordinate pos[3];
  s32 angle;
  s32 x;
  s32 z;
  s32 n;

  angle = (object->facing + 0x4000) & 0xc000;
  p = pos;
  p[0].fixed = object->x.fixed;
  p[1].fixed = object->y.fixed;
  p[2].fixed = object->z.fixed;
  Vector_AddPolarOffset(0x180000, angle, p);
  x = (p[0].fixed + 0x80000) & 0xfff00000;
  z = (p[2].fixed + 0x80000) & 0xfff00000;
  angle += 0x8000;
  Object_SetMode(object, 5);
  n = 0;
  Engine_AudioPlayCue(184);
  while (n < 16) {
    angle += 0x400;
    p[0].fixed = x;
    p[2].fixed = z;
    Vector_AddPolarOffset(0x180000, angle, p);
    object->x.fixed = p[0].fixed;
    object->z.fixed = p[2].fixed;
    object->facing = angle + 0x4000;
    Engine_TaskWait(1);
    n++;
  }
  Engine_AudioPlayCue(233);
}

static void AltinPeak_TurnRollingObjectB(struct FieldActor *object) {
  union FieldCoordinate *p;
  union FieldCoordinate pos[3];
  s32 angle;
  s32 x;
  s32 z;
  s32 n;

  angle = (object->facing - 0x4000) & 0xc000;
  p = pos;
  p[0].fixed = object->x.fixed;
  p[1].fixed = object->y.fixed;
  p[2].fixed = object->z.fixed;
  Vector_AddPolarOffset(0x180000, angle, p);
  x = (p[0].fixed + 0x80000) & 0xfff00000;
  z = (p[2].fixed + 0x80000) & 0xfff00000;
  angle += 0x8000;
  Object_SetMode(object, 6);
  n = 0;
  Engine_AudioPlayCue(184);
  while (n < 16) {
    angle -= 0x400;
    p[0].fixed = x;
    p[2].fixed = z;
    Vector_AddPolarOffset(0x180000, angle, p);
    object->x.fixed = p[0].fixed;
    object->z.fixed = p[2].fixed;
    object->facing = angle - 0x4000;
    Engine_TaskWait(1);
    n++;
  }
  Engine_AudioPlayCue(233);
}

static void AltinPeak_SetRollingStateAndPlayCue(u16 *record) {
  record = (u16 *)((char *)record + 102);
  {
    s32 value = 0x21;

    *record = value;
  }
  Engine_AudioPlayCue(288);
}

static void AltinPeak_AdvanceRollingObject(struct FieldActor *object) {
  s32 angle = 0xc000;
  s32 radius;
  s16 *timer;
  union FieldCoordinate pos[3];
  union FieldCoordinate *p;
  s32 ahead;
  s32 behind;

  angle &= object->facing;
  radius = object->y.fixed / 0x10000;
  radius = gAltinPeak_RollRadii[object->timer - radius + 16];
  timer = (s16 *)&object->work;
  if (*timer != 0) {
    if (--*timer == 20) {
      Engine_AudioPlayCue(184);
    }
    if (*timer == 0) {
      Engine_AudioPlayCue(233);
    }
  }
  p = pos;
  p[0].fixed = object->x.fixed;
  p[1].fixed = object->y.fixed;
  p[2].fixed = object->z.fixed;
  Vector_AddPolarOffset(Iwram_MulQ16(radius, 0xc000), angle, p);
  object->x.fixed = p[0].fixed;
  object->z.fixed = p[2].fixed;
  ahead = Map_GetTerrainHeight(2, p[0].fixed, p[2].fixed);
  Vector_AddPolarOffset(-Iwram_MulQ16(radius, 0x18000), angle, p);
  behind = Map_GetTerrainHeight(2, p[0].fixed, p[2].fixed);
  if (*timer <= 20) {
    if (ahead == behind) {
      Object_SetMode(object, 2);
    } else if (ahead > behind) {
      Object_SetMode(object, 3);
    } else {
      Object_SetMode(object, 4);
    }
  }
}

static void AltinPeak_RunRollingObject(s32 id, s32 heading) {
  struct FieldActor *object = Object_GetById(id);
  struct FieldActor *leader = Engine_ActorLookup(gGameState.selectedActor);
  union FieldCoordinate pos[3];
  s32 n;

  Engine_EventBegin();
  if (heading == -1) {
    heading = object->facing;
  }
  for (n = 0; n <= 3; n++) {
    pos[0].fixed = object->x.fixed;
    pos[1].fixed = object->y.fixed;
    pos[2].fixed = object->z.fixed;
    Vector_AddPolarOffset(0x100000, heading, pos);
    if (Map_GetTerrainHeight(2, pos[0].fixed, pos[2].fixed) ==
        object->y.fixed) {
      break;
    }
    heading += 0x4000;
  }
  if (n == 4) {
    return;
  }
  object->terrainId = 2;
  leader->x.fixed = 0;
  leader->z.fixed = 0;
  ResourceMetadata_Register(object->sprite, 16);
  Engine_CameraFollowActor(id, 1);
  Engine_CameraWaitForMove();
  Camera_SetSpeed(0x100000, 0x20000);
  object->facing = heading;
  object->speed = 0x20000;
  object->acceleration = 0xccc;
  object->paused = 0;
  object->timer = object->y.fixed / 0x10000;
  object->work = 0;
  pos[0].fixed = object->x.fixed;
  pos[1].fixed = object->y.fixed;
  pos[2].fixed = object->z.fixed;
  Vector_AddPolarOffset(0x180000, heading, pos);
  Engine_ObjectSetPosition(object, pos[0].fixed, object->y.fixed, pos[2].fixed);
  Engine_ObjectCommitPosition(object);
  Engine_AudioPlayCue(233);
  for (;;) {
    switch (GetMapCellCollision(2, object->x.fixed, object->z.fixed)) {
    case 98:
      AltinPeak_TurnRollingObjectA(object);
      break;
    case 97:
      AltinPeak_TurnRollingObjectB(object);
      break;
    case 96:
      AltinPeak_SetRollingStateAndPlayCue(object);
      break;
    case 99:
      goto arrived;
    }
    AltinPeak_AdvanceRollingObject(object);
    Engine_TaskWait(1);
  }
arrived:
  AltinPeak_SettleAndMountLeader(object);
  Engine_EventEnd();
}

static void AltinPeak_StepPaletteAnim(void) {
  u16 *palette = gPaletteBufferWork;

  if (gAltinPeak_PaletteHold <= 0) {
    s32 frame;

  next:
    frame = gAltinPeak_PaletteScript[gAltinPeak_PaletteStep++];
    if (frame == -1) {
      gAltinPeak_PaletteStep = 0;
      goto next;
    }
    gAltinPeak_PaletteHold = gAltinPeak_PaletteScript[gAltinPeak_PaletteStep++];
    palette += frame;
    Dma_Set(palette, (void *)0x05000006, DMA_ENABLE32 | 9, REG_DMA3);
  }
  gAltinPeak_PaletteHold--;
}

static void AltinPeak_StartPaletteAnim(void) {
  gAltinPeak_PaletteStep = 0;
  gAltinPeak_PaletteHold = 0;
  FieldScheduler_AddCallback(AltinPeak_StepPaletteAnim, 0xc80);
}

// clang-format off
s8 gAltinPeak_PaletteScript[] = {
    3, 16,
    2, 32,
    1, 64,
    2, 32,
    -1,
};

s32 gAltinPeak_ActorScript[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// The new puddle grows to full size over 108 frames.
s32 gAltinPeak_PuddleGrowScript[] = {
    SC_ADD(SW_SCALE_X, 546),
    SC_ADD(SW_SCALE_Y, 546),
    SC_WAIT(1),
    SC_LOOP(108, SC_TOP),
    SC_END,
};

s32 gAltinPeak_LogRideScript[] = {
    SC_MOVE(PIXELS(786), 0, PIXELS(220)),
    SC_SYNC,
    SC_MOVE(PIXELS(786), 0, PIXELS(326)),
    SC_SYNC,
    SC_MOVE(PIXELS(786), 0, PIXELS(326)),
    SC_SYNC,
    SC_MOVE(PIXELS(786), 0, PIXELS(480)),
    SC_SYNC,
    SC_MOVE(PIXELS(786), 0, PIXELS(520)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_END,
};

s32 gAltinPeak_LeaderRideScript[] = {
    SC_ARG(5),
    SC_MOVE(PIXELS(804), 0, PIXELS(140)),
    SC_SYNC,
    SC_MOVE(PIXELS(804), 0, PIXELS(166)),
    SC_SYNC,
    SC_MOVE(PIXELS(764), 0, PIXELS(166)),
    SC_SYNC,
    SC_MOVE(PIXELS(764), 0, PIXELS(198)),
    SC_SYNC,
    SC_MOVE(PIXELS(786), 0, PIXELS(198)),
    SC_SYNC,
    SC_MOVE(PIXELS(786), 0, PIXELS(246)),
    SC_SYNC,
    SC_MOVE(PIXELS(764), 0, PIXELS(246)),
    SC_SYNC,
    SC_MOVE(PIXELS(764), 0, PIXELS(282)),
    SC_SYNC,
    SC_MOVE(PIXELS(780), 0, PIXELS(298)),
    SC_SYNC,
    SC_MOVE(PIXELS(804), 0, PIXELS(298)),
    SC_SYNC,
    SC_MOVE(PIXELS(804), 0, PIXELS(326)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(326)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(358)),
    SC_SYNC,
    SC_MOVE(PIXELS(804), 0, PIXELS(358)),
    SC_SYNC,
    SC_MOVE(PIXELS(804), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(786), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(762), 0, PIXELS(416)),
    SC_SYNC,
    SC_MOVE(PIXELS(762), 0, PIXELS(438)),
    SC_SYNC,
    SC_MOVE(PIXELS(804), 0, PIXELS(438)),
    SC_SYNC,
    SC_MOVE(PIXELS(804), 0, PIXELS(472)),
    SC_SYNC,
    SC_MOVE(PIXELS(794), 0, PIXELS(472)),
    SC_SYNC,
    SC_MOVE(PIXELS(792), 0, PIXELS(504)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(504)),
    SC_SYNC,
    SC_MOVE(PIXELS(808), 0, PIXELS(552)),
    SC_SYNC,
    SC_SET(0x23, 1),
    SC_MOVE(PIXELS(854), 0, PIXELS(552)),
    SC_SYNC,
    SC_ARG(1),
    SC_LABEL(1),
    SC_ADD(SW_FACING, 1024),
    SC_LOOP(24, SC_TO(1)),
    SC_END,
};

s32 gAltinPeak_SparkScript[] = {
    SC_CALL(Spark_Fly),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gAltinPeak_GaretScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(314), 0, PIXELS(126)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_IvanScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(346), 0, PIXELS(126)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_MiaScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(328), 0, PIXELS(138)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_CelebrateScript[] = {
    SC_ADD(SW_SCALE_X, 128),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gAltinPeak_PartyScript[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(328), 0, PIXELS(118)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gAltinPeakEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances1[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 456, 0, 520, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {98, CONDITION_ALWAYS, 456, 0, 488, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {99, CONDITION_ALWAYS, 104, 0, 232, FACING_SOUTH, 0, 0, 56, 506, 530, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances2[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 248, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 56, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances3[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 664, 0, 380, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 904, 0, 214, FACING_NORTH, 0, 842, 40, 1016, 248, 0},
    {98, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 320, 0, 472, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances4[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 248, 0, 498, FACING_NORTH, 0, 24, 0, 648, 560, 0},
    {2, CONDITION_ALWAYS, 119, 0, 108, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {3, CONDITION_ALWAYS, 567, 0, 105, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {4, CONDITION_ALWAYS, 887, 0, 798, FACING_NORTH, 0, 712, 568, 960, 860, 0},
    {5, CONDITION_ALWAYS, 791, 0, 679, FACING_SOUTH, 0, 712, 560, 960, 860, 0},
    {6, CONDITION_ALWAYS, 88, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {7, CONDITION_ALWAYS, 216, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances5[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {98, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {99, CONDITION_ALWAYS, 747, 0, 258, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances6[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 568, 0, 348, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {2, CONDITION_ALWAYS, 295, 0, 95, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {3, CONDITION_ALWAYS, 442, 0, 774, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {4, CONDITION_ALWAYS, 72, 0, 870, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {5, CONDITION_ALWAYS, 918, 0, 712, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {6, CONDITION_ALWAYS, 615, 0, 684, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances7[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 72, 0, 502, FACING_NORTH, 0, 0, 0, 820, 568, 0},
    {2, CONDITION_ALWAYS, 152, 0, 174, FACING_SOUTH, 0, 0, 0, 820, 568, 0},
    {3, CONDITION_ALWAYS, 264, 0, 890, FACING_NORTH, 0, 144, 720, 384, 908, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances8[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 392, 0, 116, FACING_SOUTH, 0, 0, 0, 830, 564, 0},
    {2, CONDITION_ALWAYS, 40, 0, 116, FACING_NORTH, 0, 0, 0, 830, 564, 0},
    {3, CONDITION_ALWAYS, 520, 0, 616, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {4, CONDITION_ALWAYS, 408, 0, 970, FACING_NORTH, 0, 244, 564, 1024, 1024, 0},
    {5, CONDITION_ALWAYS, 951, 0, 791, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances9[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 361, 0, 309, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {2, CONDITION_ALWAYS, 89, 0, 749, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {3, CONDITION_ALWAYS, 728, 0, 717, FACING_NORTH, 0, 618, 0, 990, 854, 0},
    {4, CONDITION_ALWAYS, 935, 0, 493, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {5, CONDITION_ALWAYS, 791, 0, 566, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {6, CONDITION_ALWAYS, 792, 16, 577, FACING_SOUTH, 0, 618, 0, 998, 854, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances10[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {98, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {99, CONDITION_ALWAYS, 328, 0, 118, FACING_NORTH, 0, 0, 0, 448, 524, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeakEntrances11[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 408, 0, 517, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {2, CONDITION_ALWAYS, 198, 0, 336, FACING_SOUTH, 0, 0, 0, 540, 590, 0},
    {3, CONDITION_ALWAYS, 568, 0, 136, FACING_NORTH, 0, 480, 15, 684, 180, 0},
    {4, CONDITION_ALWAYS, 73, 0, 190, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {SCENE_TABLE_END},
};

s16 gAltinPeakRegions9[] = {
#if !TBS_PATCH_3
    790,
    -106,
    575,
    794,
    -102,
    579,
    -1,
    6,
    788,
    -108,
    579,
    796,
    -100,
    587,
    -1,
    6,
#else
    784,
    -116,
    572,
    800,
    -100,
    588,
    -1,
    6,
#endif
    -1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

s16 gAltinPeakRegions10[] = {
#if !TBS_PATCH_3
    84,
    38,
    420,
    92,
    46,
    428,
    -1,
    1,
#else
    80,
    34,
    416,
    96,
    50,
    432,
    -1,
    1,
#endif
    -1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

u32 gAltinMine_Exits[] = {
    SCENE_EXITS(77),
    SCENE_EXIT(1, 75, 10),
    SCENE_EXIT(0, 78, 0),
    SCENE_EXIT(1, 80, 3),
    SCENE_EXIT(2, 79, 1),
    SCENE_EXIT(0, 79, 0),
    SCENE_EXIT(1, 78, 2),
    SCENE_EXIT(2, 79, 3),
    SCENE_EXIT(3, 79, 2),
    SCENE_EXIT(0, 80, 0),
    SCENE_EXIT(1, 75, 11),
    SCENE_EXIT(2, 80, 4),
    SCENE_EXIT(3, 78, 1),
    SCENE_EXIT(4, 80, 2),
    SCENE_EXIT(5, 82, 1),
    SCENE_EXIT(6, 74, 4),
    SCENE_EXIT(7, 74, 5),
    SCENE_EXIT(0, 81, 0),
    SCENE_EXIT(1, 82, 6),
    SCENE_EXIT(0, 82, 0),
    SCENE_EXIT(1, 80, 5),
    SCENE_EXIT(2, 82, 3),
    SCENE_EXIT(3, 82, 2),
    SCENE_EXIT(4, 82, 5),
    SCENE_EXIT(5, 82, 4),
    SCENE_EXIT(6, 81, 1),
    SCENE_EXIT(0, 83, 0),
    SCENE_EXIT(1, 84, 1),
    SCENE_EXIT(2, 85, 1),
    SCENE_EXIT(3, 75, 13),
    SCENE_EXIT(0, 84, 0),
    SCENE_EXIT(1, 83, 1),
    SCENE_EXIT(2, 85, 5),
    SCENE_EXIT(3, 87, 1),
    SCENE_EXIT(4, 75, 12),
    SCENE_EXIT(5, 85, 3),
    SCENE_EXIT(0, 85, 0),
    SCENE_EXIT(1, 83, 2),
    SCENE_EXIT(2, 85, 4),
    SCENE_EXIT(3, 84, 5),
    SCENE_EXIT(4, 85, 2),
    SCENE_EXIT(5, 84, 2),
    SCENE_EXIT(6, 86, 1),
    SCENE_EXIT(0, 86, 0),
    SCENE_EXIT(1, 85, 6),
    SCENE_EXIT(0, 87, 0),
    SCENE_EXIT(1, 84, 3),
    SCENE_EXIT(2, 87, 3),
    SCENE_EXIT(3, 87, 2),
    SCENE_EXIT(4, 2, 52),
    SCENE_EXITS_END,
};

struct ScenePlacement gAltinPeakPlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements1[] = {
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(152), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(296), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(296), 0, PIXELS(336), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements3[] = {
    {0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(216), 0, PIXELS(312), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(248), 0, PIXELS(464), FACING_WEST,
        TALK_KEEP_FACING, 0},
    {0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(888), 0, PIXELS(172), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements5[] = {
    {0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(296), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(759), 0, PIXELS(285), FACING_WEST,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements6[] = {
    {0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(712), 0, PIXELS(184), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(696), 0, PIXELS(664), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements7[] = {
    {0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(264), 0, PIXELS(408), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(424), 0, PIXELS(424), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0, PIXELS(424), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(280), 0, PIXELS(168), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(424), 0, PIXELS(248), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(520), 0, PIXELS(632), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0x5b, 0x47, ACTOR_STAND, PIXELS(56), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements8[] = {
    {0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(636), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements9[] = {
    {0xc8, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(784), 0, PIXELS(80), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0x120, CONDITION_ALWAYS, 7, PIXELS(784), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xcd, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(76), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x16, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x112, CONDITION_ALWAYS, 7, PIXELS(792), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements10[] = {
    {0xc7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(84), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0x120, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(88), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeakPlacements11[] = {
    {0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(280), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(200), 0, PIXELS(328), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gAltinPeakEventsOther[] = {
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents1[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_TOUCH, 10, 0x904, EVENT_SCRIPT(EV10_TOUCH_1)},
    {EVENT_TOUCH, 11, 0x905, EVENT_SCRIPT(EV11_TOUCH_1)},
    {EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_FightRoom1Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, CONDITION_ALWAYS,
        EVENT_SCRIPT(Mine_FightRoom1Beast)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents1Drained[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, 0x200,
        EVENT_SCRIPT(Mine_FreezeRoom1Puddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x201, EVENT_SCRIPT(Mine_FreezeRoom1WidePuddle)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents2[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_RideLog)},
    {EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_FightRoom3Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x409, CONDITION_ALWAYS,
        EVENT_SCRIPT(Mine_FightRoom3Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 10, CONDITION_ALWAYS, 0},
    {SEARCH(SEARCH_UNNAMED), 83, CONDITION_ALWAYS, EVENT_SCRIPT(EV83_SEARCH)},
    {SEARCH(SEARCH_CHEST), 100, 0xf76, FIND_ITEM(0xc0)},
    {SCENE_EVENTS_END},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {SEARCH(SEARCH_CHEST), 100, 0xf05, FIND_ITEM(0xb4)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents4[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents5[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x200, EVENT_SCRIPT(Mine_FreezeRoom5Puddle)},
    {EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_FightRoom5Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, CONDITION_ALWAYS,
        EVENT_SCRIPT(Mine_FightRoom5Beast)},
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_RideLog)},
    {SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SEARCH_5)},
    {0x2413, 100, 0xf77, FIND_ITEM(0x7b)},
    {0x413, 100, 0xf77, FIND_ITEM(0x7b)},
    {0xe413, 100, 0xf77, FIND_ITEM(0x7b)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents6[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x200, EVENT_SCRIPT(Mine_FreezeRoom6Puddle)},
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_RideLog)},
    {SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SEARCH_6)},
    {SEARCH(SEARCH_CHEST), 100, 0xef2, 0x500002},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents7[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {SEARCH(SEARCH_CHEST), 101, 0xf07, FIND_ITEM(0xba)},
    {EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_TalkToDjinni)},
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_RideLog)},
    {SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_SEARCH_7)},
    {EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH_7)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH_7)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Mine_MoveRoom7Block)},
    {TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
        EVENT_SCRIPT(EV12_TOUCH)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x200, EVENT_SCRIPT(Mine_FreezeRoom7Puddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x201,
        EVENT_SCRIPT(Mine_FreezeRoom7SecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x204,
        EVENT_SCRIPT(Mine_FreezeRoom7ThirdPuddle)},
    {SCENE_EVENTS_END},
};
// clang-format on

struct SceneEvent gAltinPeakEvents8[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {SEARCH(SEARCH_CHEST), 100, 0xf78, FIND_ITEM(ITEM_LUCKY_MEDAL)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 8, CONDITION_ALWAYS, 0},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents9[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {SEARCH(SEARCH_UNNAMED), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_SEARCH)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 11, 0x908,
     EVENT_SCRIPT(EV11_PSYNERGY_9)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents10[] = {
    {EVENT_TOUCH, 10, FLAG_HYDROS_STATUES_DEFEATED,
     EVENT_SCRIPT(EV10_TOUCH_10)},
    {SEARCH(SEARCH_CHEST), 100, 0xf08, FIND_ITEM(0xcb)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeakEvents11[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), 8, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, 0},
    {SEARCH(SEARCH_CHEST), 100, 0xf79, FIND_ITEM(ITEM_VIAL)},
    {SCENE_EVENTS_END},
};

s32 gAltinPeak_CueTicks = 0;

u32 gAltinPeak_RollRadii[] = {
    0x10000, 0x10b55, 0x1172b, 0x12387, 0x1306f, 0x13dea, 0x14bfd,
    0x15ab0, 0x16a09, 0x17a11, 0x18ace, 0x19c49, 0x1ae89, 0x1c199,
    0x1d581, 0x1ea4b, 0x20000, 0x216ab, 0x22e56, 0x2470f, 0x260df,
    0x27bd4, 0x297fb, 0x2b560, 0x2d413, 0x2f422, 0x3159c, 0x33892,
    0x35d13, 0x38333, 0x3ab03, 0x3d495, 0x40000,
};

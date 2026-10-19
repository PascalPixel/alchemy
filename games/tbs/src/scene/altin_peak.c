#include "script.h"
#include "version.h"
#include "macros.h"
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
#include "animspr.h"
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
#include "fixed_math.h"
#include "field_runtime.h"
#include "map_scroll.h"
#include "songs.h"

static void AltinPeak_RunBoulderChase(s32 boulderDelay);
static void AltinPeak_RestoreRoom1(void);
static void AltinPeak_RestoreRoom3(void);
static void AltinPeak_RestoreRoom4(void);
static void AltinPeak_RestoreRoom5(void);
static void AltinPeak_RestoreRoom6(void);
static void AltinPeak_RestoreRoom7(void);
static void AltinPeak_RestoreRoom9(void);
static void AltinPeak_RestoreRoom10(void);
static void AltinPeak_RestoreRoom11(void);
static void AltinPeak_StartSpitting(s32 x, s32 y, s32 z, s32 facing);
static void AltinPeak_SpawnDebris(void);
static void AltinPeak_SparkleIce(union FieldObject *object);
static void AltinPeak_BeastFreezesWidePuddle(void);
static void AltinPeak_AwardFrostJewelAndDrain(void);
static void AltinPeak_DrainAfterSecondBeast(void);
static void AltinPeak_DrainAfterThirdBeast(void);
static void AltinPeak_CelebrateHydrosVictory(void);
static void AltinPeak_UpdateFrostPillarPriority(void);
static void AltinPeak_MarkActorTerrainCell(s32 actorId);
static void AltinPeak_RollLog(s32 id, s32 heading);
static void AltinPeak_StartPaletteAnim(void);

#define SCENE_MINE_ROOM(n) ((s32)&SceneId_AltinPeak##n)

// Each room's water beast fight returns the party to this entrance.
enum { ENTRANCE_AFTER_BATTLE = 99 };

// The rooms' actors, from their placement tables. Rooms 3, 5, 6 and 7 place
// the log the party rides first.
enum { ACTOR_LOG = 8 };

// Room 1: a puddle that appears with the first drain, a wide puddle and the
// water beast.
enum {
  ACTOR_ROOM1_PUDDLE = 8,
  ACTOR_ROOM1_WIDE_PUDDLE = 9,
  ACTOR_ROOM1_BEAST = 10
};

enum { ACTOR_ROOM3_BEAST = 9, ACTOR_ROOM3_BOULDER = 10 };

enum { ACTOR_ROOM5_PUDDLE = 9, ACTOR_ROOM5_BEAST = 10 };

enum { ACTOR_ROOM6_PUDDLE = 9 };

enum {
  ACTOR_ROOM7_PUDDLE = 9,
  ACTOR_ROOM7_BLOCK = 10,
  ACTOR_ROOM7_SECOND_PUDDLE = 11,
  ACTOR_ROOM7_THIRD_PUDDLE = 12,
  ACTOR_DJINNI = 14
};

enum { ACTOR_ROOM8_BOULDER = 8 };

// Room 9: the boulder that falls and its shadow, the rock that topples as it
// comes down, and the actor Force is cast on.
enum {
  ACTOR_FALLING_BOULDER = 8,
  ACTOR_BOULDER_SHADOW = 9,
  ACTOR_TOPPLING_ROCK = 10,
  ACTOR_WALL = 11
};

// Room 10: the guardian statue and its shadow.
enum { ACTOR_STATUE = 8, ACTOR_STATUE_SHADOW = 9 };

enum { ACTOR_ROOM11_BOULDER = 8, ACTOR_ROOM11_BLOCK = 9 };

// The rooms' puzzles, kept while the party stays in the mine.
enum {
  // Frost has frozen the room's first puddle.
  FLAG_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  // Frost has frozen room 1's wide puddle or room 7's second.
  FLAG_SECOND_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
  // Frost has frozen room 7's third puddle.
  FLAG_THIRD_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
  // Garet has come out to look at room 9's falling-rock sign.
  FLAG_GARET_WATCHING_ROCKS = FLAG_SCENE_TEMP_05
};

// A chest searched while facing the given way.
#define SEARCH_CHEST_FACING(facing)                                            \
  (SEARCH(SEARCH_CHEST) | EVENT_TOUCH_FACING | (facing))

// The leader's jump animations.
enum { ANIM_LEADER_JUMP = 22, ANIM_LEADER_LEAP = 7 };

// The sprites the scene makes objects from: the water beast's spray (as in
// Altin) and room 9's boulder, whose pieces fly in the crash.
enum { SPRITE_BOULDER = 200, SPRITE_SPRAY = 222 };

// The spray's direction (0 east, 0x8000 west) and origin, and the water's
// flicker timer.
extern s32 gAltinPeak_LeafMode;
extern s32 gAltinPeak_LeafOrigin[3];
extern u16 gAltinPeak_RiseTimer;

// Talking to the Djinni in room 7 makes it hop and starts the battle for
// Djinni 23.
static void AltinPeak_TalkToDjinni(void) {
  Field_RequestDjinnBattle(ACTOR_DJINNI, 23);
}

// Shared source, included where its bytes sit in the ROM.
#include "common/object_facing.h"

// Update callback of room 9's boulder shadow: it stays under the boulder, on
// the ground 12 pixels down, while the boulder bounces.
// UB: update hooks return void. Plain void removes the native zero result
// and changes its return sequence (three instructions in tbs-en).
#ifdef UBFIX
static void AltinPeak_ShadowFollowBoulder(union FieldObject *object) {
#else
static s32 AltinPeak_ShadowFollowBoulder(union FieldObject *object) {
#endif
  struct FieldActor *follower = (struct FieldActor *)object;
  struct FieldActor *actor = Object_GetById(ACTOR_FALLING_BOULDER);

  follower->x.fixed = actor->x.fixed;
  follower->y.fixed = -PIXELS(12);
  follower->z.fixed = actor->z.fixed;
#ifndef UBFIX
  return 0;
#endif
}

// Script callback for a piece of flying rock debris: it moves by its
// velocity, grows by its z velocity and falls under its gravity.
static s32 AltinPeak_FlyDebris(struct ScriptInterpreter *interpreter) {
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
    return gAltinPeak_Entrances1;
  }
  if (scene == SCENE_MINE_ROOM(2)) {
    return gAltinPeak_Entrances2;
  }
  if (scene == SCENE_MINE_ROOM(3)) {
    return gAltinPeak_Entrances3;
  }
  if (scene == SCENE_MINE_ROOM(4)) {
    return gAltinPeak_Entrances4;
  }
  if (scene == SCENE_MINE_ROOM(5)) {
    return gAltinPeak_Entrances5;
  }
  if (scene == SCENE_MINE_ROOM(6)) {
    return gAltinPeak_Entrances6;
  }
  if (scene == SCENE_MINE_ROOM(7)) {
    return gAltinPeak_Entrances7;
  }
  if (scene == SCENE_MINE_ROOM(8)) {
    return gAltinPeak_Entrances8;
  }
  if (scene == SCENE_MINE_ROOM(9)) {
    return gAltinPeak_Entrances9;
  }
  if (scene == SCENE_MINE_ROOM(10)) {
    return gAltinPeak_Entrances10;
  }
  if (scene == SCENE_MINE_ROOM(11)) {
    return gAltinPeak_Entrances11;
  }
  return gAltinPeak_EntrancesOther;
}

// Rooms 9 and 10 have regions.
const struct SceneRegion *Scene_GetRegions(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_MINE_ROOM(9)) {
    return (const struct SceneRegion *)gAltinPeak_Regions9;
  }
  if (scene == SCENE_MINE_ROOM(10)) {
    return (const struct SceneRegion *)gAltinPeak_Regions10;
  }
  return 0;
}

// All the rooms share one exit table.
const u32 *Scene_GetExits(void) { return gAltinPeak_Exits; }

// Returns the actor placements of the room being entered.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == SCENE_MINE_ROOM(1)) {
    return gAltinPeak_Placements1;
  }
  if (scene == SCENE_MINE_ROOM(3)) {
    return gAltinPeak_Placements3;
  }
  if (scene == SCENE_MINE_ROOM(5)) {
    return gAltinPeak_Placements5;
  }
  if (scene == SCENE_MINE_ROOM(6)) {
    return gAltinPeak_Placements6;
  }
  if (scene == SCENE_MINE_ROOM(7)) {
    return gAltinPeak_Placements7;
  }
  if (scene == SCENE_MINE_ROOM(8)) {
    return gAltinPeak_Placements8;
  }
  if (scene == SCENE_MINE_ROOM(9)) {
    return gAltinPeak_Placements9;
  }
  if (scene == SCENE_MINE_ROOM(10)) {
    return gAltinPeak_Placements10;
  }
  if (scene == SCENE_MINE_ROOM(11)) {
    return gAltinPeak_Placements11;
  }
  return gAltinPeak_PlacementsOther;
}

// Talking to a log: the party leader jumps onto it, and once the leader has
// landed on top the log rolls away with the leader riding it.
static void AltinPeak_RideLog(s32 log) {
  struct FieldActor *logActor;
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  logActor = Object_GetById(ACTOR_LOG);
  Battle_ResetEffectCounter();
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_JUMP);
  Engine_EventWait(10);
  Engine_AudioPlayCue(SE_152);
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(16) / 5, PIXELS(8) / 5);
  // Leap by the height between leader and log, in quarter pixels, plus 4.
  leader->velocityY =
      ((ABS(logActor->y.fixed - leader->y.fixed) >> 14) << 14) + PIXELS(4);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  Engine_ObjectSetPosition(leader, logActor->x.fixed, logActor->y.fixed,
                           logActor->z.fixed);
  Engine_TaskWait(10);
  leader->sprite->image.priority = 3;
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);

  // Wait until the leader has come down onto the log.
  for (;;) {
    if (!((logActor->y.fixed >> 14) < (leader->y.fixed >> 14)))
      break;
    Engine_TaskWait(1);
  }
  Engine_EventEnd();
  Engine_AudioPlayCue(SE_159);
  AltinPeak_RollLog(log, 0);
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
}

// Talking to (or reading the mind of) a room's water beast starts its
// battle; afterwards the party returns to the room's battle entrance.
static void AltinPeak_FightRoom1Beast(void) {
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(1), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 2);
}

static void AltinPeak_FightRoom3Beast(void) {
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(3), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 2);
}

static void AltinPeak_FightRoom5Beast(void) {
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(5), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 2);
}

// Move on room 7's block: the cell it left becomes free again and the cell
// it now stands on (column 24 or 25 of row 26) takes the blocking
// attributes kept at (0, 0).
static void AltinPeak_SettleRoom7Block(void) {
  struct FieldActor *block = Object_GetById(ACTOR_ROOM7_BLOCK);

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
static void AltinPeak_FreezeRoom1Puddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_ROOM1_PUDDLE);
  Map_CopyCellAttributes(7, 13, 1, 1, 9, 13);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

// Frost on room 1's wide puddle, three cells across: the cells take the
// ice's attributes and the ice is left to fall to the ground.
static void AltinPeak_FreezeRoom1WidePuddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE);
  Map_CopyCellAttributes(29, 1, 3, 1, 17, 13);
  if (puddle != 0) {
    puddle->motionFlags = ACTOR_FALLS;
  }
  Engine_GameFlagSet(FLAG_SECOND_PUDDLE_FROZEN);
}

// Frost on room 7's first puddle.
static void AltinPeak_FreezeRoom7Puddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_ROOM7_PUDDLE);
  Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

// Frost on room 5's puddle.
static void AltinPeak_FreezeRoom5Puddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_ROOM5_PUDDLE);
  Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

// Frost on room 6's puddle.
static void AltinPeak_FreezeRoom6Puddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_ROOM6_PUDDLE);
  Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

// Frost on room 7's second puddle.
static void AltinPeak_FreezeRoom7SecondPuddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_ROOM7_SECOND_PUDDLE);
  Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SECOND_PUDDLE_FROZEN);
}

// Frost on room 7's third puddle.
static void AltinPeak_FreezeRoom7ThirdPuddle(void) {
  struct FieldActor *puddle;

  puddle = Object_GetById(ACTOR_ROOM7_THIRD_PUDDLE);
  Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
  if (puddle != 0) {
    Engine_ActorSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_THIRD_PUDDLE_FROZEN);
}

// Interrupt handler while the water layer vanishes: each time, background 1
// (the water) stays on only when a random percentage reaches the timer, so
// as the timer climbs to 100 the water flickers away.
static void Water_FlickerOut(void) {
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

// The first drain, after the Frost Jewel is found: the screen shakes, the
// water layer sinks 24 pixels, one step every 4 frames (a puddle growing
// from a tenth of its size partway down), then flickers away over 100
// frames. The way on opens and the drain is remembered.
static void AltinPeak_DrainFirstWater(void) {
  s32 step;
  struct FieldActor *puddle;
  u8 *mapWork;
  u8 *waterLayer;
  s32 timer;

  mapWork = gMapWork[0];
  Event_PlayCue(SE_230);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = mapWork + 356;
  for (step = 0; step <= 23; step++) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    Engine_TaskWait(4);
    if (step == 8) {
      puddle = Object_GetById(ACTOR_ROOM1_PUDDLE);
      puddle->scaleX = Q16_ONE / 10;
      puddle = Actor_Get(ACTOR_ROOM1_PUDDLE);
      puddle->scaleY = Q16_ONE / 10;
      Actor_SetPosition(ACTOR_ROOM1_PUDDLE, PIXELS(152), PIXELS(216));
      Actor_SetActionScript(ACTOR_ROOM1_PUDDLE, gAltinPeak_PuddleGrowScript);
    }
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  gAltinPeak_RiseTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = gAltinPeak_RiseTimer + 1;
    gAltinPeak_RiseTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(30);
  Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
  GameFlag_Set(FLAG_ALTIN_MINE_FIRST_DRAIN);
}

// The second drain: the same sinking and flicker over the cells the water
// leaves behind, which then take their dry form.
static void AltinPeak_DrainSecondWater(void) {
  s32 step;
  u8 *mapWork;
  u8 *waterLayer;
  s32 timer;

  mapWork = gMapWork[0];
  Map_CopyCells(93, 41, 16, 4, 77, 28);
  Event_PlayCue(SE_230);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = mapWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    Engine_TaskWait(4);
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  gAltinPeak_RiseTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = gAltinPeak_RiseTimer + 1;
    gAltinPeak_RiseTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(30);
  Map_CopyCells(77, 41, 16, 4, 77, 28);
  GameFlag_Set(FLAG_ALTIN_MINE_SECOND_DRAIN);
}

// The third drain, likewise.
static void AltinPeak_DrainThirdWater(void) {
  s32 step;
  u8 *mapWork;
  u8 *waterLayer;
  s32 timer;

  mapWork = gMapWork[0];
  Map_CopyCellsTo(113, 31, 103, 17, 1, 1);
  Map_CopyCellsTo(111, 32, 104, 18, 3, 2);
  Map_CopyCellsTo(64, 32, 103, 18, 1, 2);
  Event_PlayCue(SE_230);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = mapWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    Engine_TaskWait(4);
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  gAltinPeak_RiseTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = gAltinPeak_RiseTimer + 1;
    gAltinPeak_RiseTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(30);
  Map_CopyCellsTo(103, 14, 103, 17, 4, 3);
  GameFlag_Set(FLAG_ALTIN_MINE_THIRD_DRAIN);
}

// Flips room 3's switch to its other position, redrawing it and changing the
// cell its position opens or closes.
static void AltinPeak_ToggleRoom3SwitchCells(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_3_SWITCH) != 0) {
    Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_ROOM_3_SWITCH);
  } else {
    Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_ROOM_3_SWITCH);
  }
}

// Searching room 3's switch: "You flipped the switch." and it flips.
static void AltinPeak_FlipRoom3Switch(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(SE_125);
  AltinPeak_ToggleRoom3SwitchCells();
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

// Flips room 5's switch, the same way.
static void AltinPeak_ToggleRoom5SwitchCells(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_5_SWITCH) != 0) {
    Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_ROOM_5_SWITCH);
  } else {
    Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_ROOM_5_SWITCH);
  }
}

// Searching room 5's switch flips it.
static void AltinPeak_FlipRoom5Switch(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(SE_125);
  AltinPeak_ToggleRoom5SwitchCells();
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

// Searching room 6's switch flips it.
static void AltinPeak_FlipRoom6Switch(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Event_PlayCue(SE_125);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_6_SWITCH) != 0) {
    Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_ROOM_6_SWITCH);
  } else {
    Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_ROOM_6_SWITCH);
  }
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

// Searching room 7's switch flips it.
static void AltinPeak_FlipRoom7Switch(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Event_PlayCue(SE_125);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_7_SWITCH) != 0) {
    Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_ROOM_7_SWITCH);
  } else {
    Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_ROOM_7_SWITCH);
  }
  Engine_TaskWait(20);
  BattleFx_PlayQueuedSound();
  Engine_EventEnd();
}

// Stepping onto room 7's block: it is drawn under the party leader.
static void AltinPeak_PlaceBlockUnderLeader(void) {
  Object_GetById(ACTOR_ROOM7_BLOCK)->priorityFlags =
      ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
}

// Stepping off room 7's block: it sorts against the leader again.
static void AltinPeak_RestoreBlockAutomaticPriority(void) {
  Object_GetById(ACTOR_ROOM7_BLOCK)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
}

// Walking east into room 7's block pushes it one cell along with a scrape;
// the leader follows it and its cells are settled as after Move.
static void AltinPeak_PushRoom7Block(void) {
  Engine_EventBegin();
  Map_CopyCellAttributes(24, 27, 2, 1, 24, 26);
  Event_PlayCue(SE_185);
  Actor_SetSpeed(ACTOR_ROOM7_BLOCK, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  Object_GetById(ACTOR_ROOM7_BLOCK)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 400, 424);
  Actor_SetDestination(ACTOR_ROOM7_BLOCK, 408, 424);
  Engine_ActorWaitForMove(ACTOR_ROOM7_BLOCK);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  AltinPeak_SettleRoom7Block();
  Engine_EventEnd();
}

// Force on room 9's wall: it rumbles, the rock on it rocks one way and back,
// then topples with a crash, and the rockfall follows.
static void AltinPeak_ForceRockOver(void) {
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  s32 originX;
  s32 originHeight;
  s32 cosine;
  s32 sine;
  u16 rotationStep;

  actor = Actor_Get(ACTOR_TOPPLING_ROCK);
  sprite = actor->sprite;
  originX = actor->x.fixed;
  originHeight = actor->y.fixed;
  Event_Begin();
  Event_PlayCue(SE_141);
  Map_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  Event_Wait(10);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_Wait(20);

  // Rock the obstacle to each side before the accelerating fall.
  rotationStep = 0;
  while (1) {
    rotationStep += 8;
    sprite->rotation += rotationStep;
    cosine = Math_Cos(sprite->rotation + 0x4000);
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation > 0x8fff) {
      break;
    }
    Task_Wait(1);
  }
  rotationStep = 0;
  while (1) {
    rotationStep += 8;
    sprite->rotation -= rotationStep;
    cosine = Math_Cos(sprite->rotation + 0x4000);
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation <= 0x7000) {
      break;
    }
    Task_Wait(1);
  }
  rotationStep = 8;
  while (1) {
    rotationStep += rotationStep >> 3;
    sprite->rotation += rotationStep;
    cosine = Math_Cos(sprite->rotation + 0x4000);
    sine = Math_Sin(sprite->rotation + 0x8000);
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation > 0x8000) {
      actor->y.fixed = originHeight - (sine << 3);
    }
    if (sprite->rotation + rotationStep > 0xbfff) {
      break;
    }
    Task_Wait(1);
  }
  Task_Wait(1);
  sprite->rotation = 0xc000;
  Event_PlayCue(SE_183);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Event_Wait(20);
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  AltinPeak_RunBoulderChase(5);
  Event_End();
}

union AltinPeakHoppingActor {
  struct FieldActor actor;
  struct AltinPeakHopMotion motion;
};

// Room 9's rockfall. The party leader steps back and looks up as the slope
// rumbles; the rock lies toppled and a boulder drops onto the path. The
// leader flees down the winding path, and after boulderDelay frames the
// boulder rolls after; it bounces to a stop at the bottom and crashes into
// the wall, throwing up rock debris and leaving rubble. Garet, if he came out
// at the sign, rejoins the leader. Remembered by
// FLAG_ALTIN_PEAK_BOULDER_FALLEN.
static void AltinPeak_RunBoulderChase(s32 boulderDelay) {
  struct FieldActor *leader;
  struct FieldActor *boulder;
  struct FieldActor *shadow;
  struct FieldActor *rock;
  struct FieldActor *actor;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  boulder = Actor_Get(ACTOR_FALLING_BOULDER);
  shadow = Actor_Get(ACTOR_BOULDER_SHADOW);
  rock = Actor_Get(ACTOR_TOPPLING_ROCK);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(784), -1, PIXELS(116), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 792, 140);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 100);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 60);
  Event_PlayCue(SE_183);
  Map_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_EventWait(20);
  // Stage the rolling object and its follower for the escape route.
  shadow->scaleX = 6 * Q16_ONE / 5;
  shadow->scaleY = 6 * Q16_ONE / 5;
  shadow->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  shadow->update = AltinPeak_ShadowFollowBoulder;
#else
  shadow->update = (void (*)(union FieldObject *))AltinPeak_ShadowFollowBoulder;
#endif
  Engine_ActorSetAnimation(ACTOR_FALLING_BOULDER, 4);
  boulder->bounce = PIXELS(1) / 2;
  boulder->x.fixed = PIXELS(786);
  boulder->y.fixed = PIXELS(32);
  boulder->z.fixed = PIXELS(90);
  boulder->scaleX = 2 * Q16_ONE;
  boulder->scaleY = 2 * Q16_ONE;
  Engine_EventWait(10);
  Event_PlayCue(SE_183);
  Map_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  rock->x.fixed += PIXELS(14);
  rock->y.fixed += -PIXELS(8);
  rock->sprite->rotation = 0xc000;
  Event_PlayCue(SE_107);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 80);
  Event_PlayCue(MUS_055);
  Map_SetShake(PIXELS(1), PIXELS(3), Q16_ONE);
  Engine_ActorSetSpritePriority(ACTOR_FALLING_BOULDER, 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1);
  // Send the leader first, then release the rolling object after its delay.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  leader->timer = 0;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, gAltinPeak_LeaderFleePath);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    Actor_SetPosition(GARET, PIXELS(878), PIXELS(528));
    actor = Actor_Get(GARET);
    actor->facing = FACING_SOUTH + FACING_STEP;
  }
  Camera_SetSpeed(PIXELS(5) / 4, PIXELS(5) / 32);
  Camera_MoveTo(PIXELS(786), -1, PIXELS(556), 1);
  Engine_EventWait(boulderDelay);
  Engine_ActorSetSpritePriority(ACTOR_FALLING_BOULDER, 1);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, PIXELS(317) / 200, PIXELS(275) / 347);
  boulder->timer = 0;
  Actor_SetActionScript(ACTOR_FALLING_BOULDER, gAltinPeak_BoulderPath);
  do {
    Engine_TaskWait(1);
  } while (leader->timer == 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
  do {
    Engine_TaskWait(1);
  } while (boulder->timer == 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_PlayCue(SE_289);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  shadow->x.fixed = PIXELS(786);
  shadow->update = NULL;
  shadow->z.fixed = PIXELS(618);
  shadow->y.fixed = -PIXELS(64);
  // Let the rolling object bounce before the final collapse.
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, PIXELS(8) / 5, PIXELS(4) / 5);
  boulder->bounce = PIXELS(1) / 10;
  boulder->gravity = PIXELS(1) / 5;
  boulder->velocityY = PIXELS(4);
  Actor_MoveToAndWait(ACTOR_FALLING_BOULDER, 786, 604);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, PIXELS(16) / 5, PIXELS(8) / 5);
  Actor_SetDestination(ACTOR_FALLING_BOULDER, 786, 644);
  Engine_EventWait(15);
  Map_SetShake(PIXELS(5), PIXELS(7), Q16_ONE);
  Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
  Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
  Actor_SetPosition(ACTOR_FALLING_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  // Hide the rolling actors and animate the falling debris.
  FieldScheduler_AddCallback(AltinPeak_SpawnDebris, TASK_PRIORITY_SCENE);
  Engine_EventWait(80);
  Engine_TaskRemoveCallback(AltinPeak_SpawnDebris);
  Engine_EventWait(60);
  Event_PlayCue(MUS_017);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // Return Garet to the party if the warning sign brought him into the scene.
  Engine_EventWait(120);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkTo(GARET, 824, 558);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 854, 584);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
  Camera_MoveTo(PIXELS(788), -PIXELS(64), PIXELS(610), 1);
  Engine_CameraWaitForMove();
  Event_PlayCue(SE_148);
  Engine_EventWait(240);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
    Camera_MoveTo(PIXELS(854), 0, PIXELS(584), 1);
    Engine_CameraWaitForMove();
    Actor_WalkToAndWait(GARET, 840, 552);
    Actor_WalkToAndWait(GARET, 854, 562);
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Actor_SetPosition(GARET, 0, 0);
  }
  Audio_PlayCueFromEventWork();
  GameFlag_Set(FLAG_ALTIN_PEAK_BOULDER_FALLEN);
}

// Searching room 9's sign: "Watch for falling rocks! Do not strike the
// wall!" Before the rockfall, while the party has no Orb of Force, Garet
// steps out, grumbles at the dead end and strikes the wall; the ground
// shakes, he tells the leader to run and runs down the path, and the
// rockfall follows.
static void AltinPeak_ReadFallingRockSign(void) {
  struct FieldActor *actor;
  s32 message;

  Engine_EventBegin();
  message = MsgAltinWatchFallingRocks;
  Engine_MessageShowCentered(message, 1);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_BOULDER_FALLEN) == 0 &&
      GameFlag_IsSet(0xf14) == 0) {
    // 0xf14: the Orb of Force's chest in Fuchin Falls, not yet opened.
    // Bring Garet out to inspect the warning and demonstrate the danger.
    GameFlag_Set(FLAG_GARET_WATCHING_ROCKS);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 790, 140);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 780, 140);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(GARET, 800, 140);
    Actor_FaceDirection(GARET, FACING_NORTH, 20);
    Engine_EventSetMessage(message + 1);
    Engine_ActorSetAnimation(GARET, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GARET, 0, 10);
    Engine_ActorJump(GARET, 6, 0);
    Actor_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
    Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(GARET, 792, 110);
    Engine_EventWait(1);
    Object_GetById(GARET)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_PlayCue(SE_161);
    Map_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
    Object_GetById(GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(GARET, 792, 120);
    Engine_EventWait(1);
    {
      struct FieldActor *actor = Actor_Get(GARET);

      actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
    }
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Engine_EventWait(80);
    Event_PlayCue(SE_141);
    Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GARET, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GARET, 0, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 40);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 60);
    Actor_FaceDirection(GARET, FACING_SOUTH, 20);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Event_ShowMessageAndWait(GARET, 0, 10);
    Actor_SetSpeed(GARET, PIXELS(5) / 2, PIXELS(5) / 4);
    Engine_ActorSetAnimation(GARET, 5);
    Actor_MoveToAndWait(GARET, 796, 138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_MoveToAndWait(GARET, 804, 140);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_MoveToAndWait(GARET, 804, 166);
    Actor_MoveToAndWait(GARET, 764, 166);
    Actor_MoveToAndWait(GARET, 764, 198);
    Actor_MoveToAndWait(GARET, 786, 198);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_MoveToAndWait(GARET, 786, 246);
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
    Actor_SetPosition(GARET, 0, 0);
    Engine_EventWait(40);
    AltinPeak_RunBoulderChase(10);
  }
  Engine_EventEnd();
}

// Returns the event table of the room being entered; room 1 has its own
// once the first drain has lowered its water.
const struct SceneEvent *Scene_GetEvents(void) {
  const struct SceneEvent *table;

  if (gGameState.scene == SCENE_MINE_ROOM(1)) {
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
      table = gAltinPeak_Events1Drained;
    } else {
      table = gAltinPeak_Events1;
    }
  } else if (gGameState.scene == SCENE_MINE_ROOM(2)) {
    table = gAltinPeak_Events2;
  } else if (gGameState.scene == SCENE_MINE_ROOM(3)) {
    table = gAltinPeak_Events3;
  } else if (gGameState.scene == SCENE_MINE_ROOM(4)) {
    table = gAltinPeak_Events4;
  } else if (gGameState.scene == SCENE_MINE_ROOM(5)) {
    table = gAltinPeak_Events5;
  } else if (gGameState.scene == SCENE_MINE_ROOM(6)) {
    table = gAltinPeak_Events6;
  } else if (gGameState.scene == SCENE_MINE_ROOM(7)) {
    table = gAltinPeak_Events7;
  } else if (gGameState.scene == SCENE_MINE_ROOM(8)) {
    table = gAltinPeak_Events8;
  } else if (gGameState.scene == SCENE_MINE_ROOM(9)) {
    table = gAltinPeak_Events9;
  } else if (gGameState.scene == SCENE_MINE_ROOM(10)) {
    table = gAltinPeak_Events10;
  } else if (gGameState.scene == SCENE_MINE_ROOM(11)) {
    table = gAltinPeak_Events11;
  } else {
    table = gAltinPeak_EventsOther;
  }
  return table;
}

// Update callback of the hopping water beast: once a second it leaps up and
// lands with a splash (SE_229) that shakes the screen briefly.
static void AltinPeak_UpdateBeastHops(union FieldObject *object) {
  struct AltinPeakHopMotion *motion = (struct AltinPeakHopMotion *)object;
  if (motion->shakeDelay != 0) {
    if (--motion->shakeDelay == 1)
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  }
  if (motion->velocityY == 0) {
    Object_SetMode(&object->actor, 1);
    motion->height -= PIXELS(3) / 2;
    if (motion->height < motion->terrainHeight) {
      if (motion->landingCuePending != 0) {
        Engine_AudioPlayCue(SE_229);
        motion->landingCuePending = 0;
        motion->shakeDelay = 4;
        Map_SetShake(PIXELS(1), 0, Q16_ONE);
      }
      motion->height = motion->terrainHeight;
    }
    motion->verticalStopped = 1;
  } else {
    motion->verticalStopped = 0;
  }
  if (motion->hopTimer == 0) {
    Engine_AudioPlayCue(SE_152);
    motion->landingCuePending = 1;
    Object_SetMode(&object->actor, 2);
    motion->velocityY = PIXELS(3);
  }
  if (++motion->hopTimer == 60)
    motion->hopTimer = 0;
}

// The beast lands with a splash, shaking the screen for holdFrames.
static __inline__ void AltinPeak_PlayLandingShake(s32 amplitude,
                                                  s32 holdFrames) {
  Engine_AudioPlayCue(SE_229);
  Map_SetShake(amplitude, 0, Q16_ONE);
  Engine_EventWait(holdFrames);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
}

// Stepping near room 1's water beast the first time: the camera finds it, it
// cries out and hops away around the room, freezes the wide puddle with
// Frost and hops across it, then stops to watch the party leader.
static void AltinPeak_ShowFirstBeastRoute(void) {
  union AltinPeakHoppingActor *beast;
  struct FieldActor *leader;
  struct FieldActor *actor;
  s32 jumpSpeed;

  beast = (union AltinPeakHoppingActor *)Object_GetById(ACTOR_ROOM1_BEAST);
  Engine_EventBegin();
  Engine_CameraSetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(298), -1, PIXELS(337), 1);
  Engine_CameraWaitForMove();
  Engine_AudioPlayCue(SE_147);
  Engine_ActorRunRepeatedMotion(ACTOR_ROOM1_BEAST, 2);
  Engine_EventWait(40);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(295), PIXELS(32), PIXELS(212), 1);
  // Run the timed hopping callback while the beast moves along its route.
  beast->motion.landingCuePending = 0;
  beast->motion.hopTimer = 0;
  beast->motion.shakeDelay = 0;
  beast->actor.gravity = PIXELS(2) / 5;
  beast->actor.update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_ROOM1_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 308, 291);
  Engine_ActorMoveToAndWait(ACTOR_ROOM1_BEAST, 311, 215);
  beast->actor.update = 0;
  beast->motion.verticalStopped = 0;
  Engine_EventWait(16);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_WEST, 40);
  // Demonstrate Frost between the hopping sections.
  AltinPeak_BeastFreezesWidePuddle();
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTHEAST + FACING_STEP, 40);
  Camera_MoveTo(PIXELS(334), -1, PIXELS(244), 1);
  beast->motion.landingCuePending = 0;
  beast->motion.hopTimer = 0;
  beast->motion.shakeDelay = 0;
  beast->actor.update = AltinPeak_UpdateBeastHops;
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 320, 232);
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 340, 262);
  Engine_ActorMoveToAndWait(ACTOR_ROOM1_BEAST, 374, 262);
  beast->actor.update = 0;
  Engine_EventWait(16);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_NORTHEAST + FACING_STEP, 20);
  Engine_ActorFaceDirection(ACTOR_ROOM1_BEAST, FACING_NORTH + FACING_STEP, 40);
  Engine_AudioPlayCue(SE_153);
  actor = Object_GetById(ACTOR_ROOM1_BEAST);
  actor->velocityY = PIXELS(4);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 2);
  Engine_ActorMoveToAndWait(ACTOR_ROOM1_BEAST, 380, 248);
  Engine_EventWait(10);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_WEST, 40);
  Camera_MoveTo(PIXELS(304), -1, PIXELS(215), 1);
  beast->motion.landingCuePending = 0;
  beast->motion.hopTimer = 0;
  beast->motion.shakeDelay = 0;
  beast->actor.update = AltinPeak_UpdateBeastHops;
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 329, 219);
  beast->actor.update = 0;
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  Engine_EventWait(16);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(ACTOR_ROOM1_BEAST, FACING_WEST, 40);
  Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE)->motionFlags = 0;
  // Open the route for the final two beast jumps.
  Map_CopyCellAttributes(3, 0, 1, 1, 17, 13);
  Map_CopyCellAttributes(3, 0, 1, 1, 18, 13);
  Map_CopyCellAttributes(3, 0, 1, 1, 19, 13);
  Engine_ActorSetSpeed(ACTOR_ROOM1_BEAST, PIXELS(7) / 5, PIXELS(7) / 10);
  actor = Object_GetById(ACTOR_ROOM1_BEAST);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_AudioPlayCue(SE_153);
  jumpSpeed = PIXELS(5);
  actor = Object_GetById(ACTOR_ROOM1_BEAST);
  actor->velocityY = jumpSpeed;
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 3);
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 295, 215);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  actor = Object_GetById(ACTOR_ROOM1_BEAST);
  Engine_ActorSetSpriteFlags(actor, 1);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(40);
  Engine_AudioPlayCue(SE_153);
  actor = Object_GetById(ACTOR_ROOM1_BEAST);
  actor->velocityY = jumpSpeed;
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 3);
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 260, 215);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTH + FACING_STEP, 20);
  Engine_ActorFaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTHEAST + FACING_STEP,
                            20);
  Engine_AudioPlayCue(SE_147);
  Engine_ActorRunRepeatedMotion(ACTOR_ROOM1_BEAST, 2);
  Engine_EventWait(40);
  Map_CopyCellAttributes(4, 0, 1, 1, 17, 13);
  Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
  Map_CopyCellAttributes(4, 0, 1, 1, 19, 13);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_CameraSetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Engine_CameraMoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  Engine_CameraWaitForMove();
  FieldObject_SetTargetAndCallback(10,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gAltinPeak_FaceLeaderScript);
  Engine_GameFlagSet(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN);
  Engine_EventEnd();
}

// Stepping near the beast again: it hops back over the ice and settles down,
// spitting water.
static void AltinPeak_ShowSecondBeastRoute(void) {
  union AltinPeakHoppingActor *actor;
  struct FieldActor *movingActor;
  u32 waitedFrames;

  actor = (union AltinPeakHoppingActor *)Object_GetById(ACTOR_ROOM1_BEAST);
  Engine_EventBegin();
  ObjectMotion_EnableActionAndResetMotion(ACTOR_ROOM1_BEAST);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(279), PIXELS(64), PIXELS(216), 1);
  Engine_CameraWaitForMove();
  Engine_AudioPlayCue(SE_147);
  Engine_ActorRunRepeatedMotion(ACTOR_ROOM1_BEAST, 2);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_WEST, 40);
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(128), PIXELS(64), PIXELS(202), 1);
  actor->motion.landingCuePending = 0;
  actor->motion.hopTimer = 0;
  actor->motion.shakeDelay = 0;
  actor->actor.gravity = PIXELS(2) / 5;
  actor->actor.update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_ROOM1_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 212, 200);
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 103, 200);
  actor->actor.update = NULL;
  actor->motion.verticalStopped = 0;
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTH + FACING_STEP, 40);
  Object_GetById(ACTOR_ROOM1_BEAST)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_ROOM1_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  movingActor = Object_GetById(ACTOR_ROOM1_BEAST);
  Engine_ActorSetSpriteFlags(movingActor, 0);
  Engine_AudioPlayCue(SE_153);
  movingActor = Object_GetById(ACTOR_ROOM1_BEAST);
  movingActor->velocityY = PIXELS(4);
  Actor_SetAnimation(ACTOR_ROOM1_BEAST, 3);
  Engine_ActorMoveToAndWait(ACTOR_ROOM1_BEAST, 86, 214);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  movingActor = Object_GetById(ACTOR_ROOM1_BEAST);
  Engine_ActorSetSpriteFlags(movingActor, 1);
  Engine_EventWait(10);
  AltinPeak_PlayLandingShake(PIXELS(2), 8);
  Engine_EventWait(40);
  Object_GetById(ACTOR_ROOM1_BEAST)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_ROOM1_BEAST, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorFaceDirection(ACTOR_ROOM1_BEAST, 0, 40);
  actor->motion.landingCuePending = 0;
  actor->motion.hopTimer = 0;
  actor->motion.shakeDelay = 0;
  actor->actor.update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_ROOM1_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_MoveToAndWait(ACTOR_ROOM1_BEAST, 120, 215);
  actor->actor.update = NULL;
  actor->motion.verticalStopped = 0;
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 1);
  Engine_EventWait(16);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_EventWait(40);
  Engine_ActorFaceDirection(ACTOR_ROOM1_BEAST, 0, 10);
  Engine_AudioPlayCue(SE_147);
  Engine_ActorRunRepeatedMotion(ACTOR_ROOM1_BEAST, 2);
  Engine_EventWait(80);
  Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 3);
  // End the route with a spray gust and a short skippable wait.
  AltinPeak_StartSpitting(PIXELS(130), 0, PIXELS(168), 0);
  Engine_EventWait(60);
  for (waitedFrames = 0; waitedFrames < 60 && gKeysPressed == 0;
       waitedFrames++) {
    Engine_EventWait(1);
  }
  actor = (union AltinPeakHoppingActor *)Object_GetById(ACTOR_PARTY_LEADER);
  Camera_SetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Engine_CameraMoveTo(actor->actor.x.fixed, actor->actor.y.fixed,
                      actor->actor.z.fixed, 1);
  Engine_CameraWaitForMove();
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN);
  Engine_EventEnd();
}

// Sets up the room being entered: puzzles already solved and the water
// levels already lowered. Returns 0.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == SCENE_MINE_ROOM(1)) {
    AltinPeak_RestoreRoom1();
  } else if (gGameState.scene == SCENE_MINE_ROOM(3)) {
    AltinPeak_RestoreRoom3();
  } else if (gGameState.scene == SCENE_MINE_ROOM(4)) {
    AltinPeak_RestoreRoom4();
  } else if (gGameState.scene == SCENE_MINE_ROOM(5)) {
    AltinPeak_RestoreRoom5();
  } else if (gGameState.scene == SCENE_MINE_ROOM(6)) {
    AltinPeak_RestoreRoom6();
  } else if (gGameState.scene == SCENE_MINE_ROOM(7)) {
    AltinPeak_RestoreRoom7();
  } else if (gGameState.scene == SCENE_MINE_ROOM(9)) {
    AltinPeak_RestoreRoom9();
  } else if (gGameState.scene == SCENE_MINE_ROOM(10)) {
    AltinPeak_RestoreRoom10();
  } else if (gGameState.scene == SCENE_MINE_ROOM(11)) {
    AltinPeak_RestoreRoom11();
  }
  return 0;
}

// Room 1: after its beast's battle the Frost Jewel is found and the water
// drains; the beast stays where its routes left it while the water is up,
// and the frozen puddles are restored once it is down.
static void AltinPeak_RestoreRoom1(void) {
  s32 opened;
  s32 flag;
  struct FieldActor *actor;
  struct MapScrollWork *map;

  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gGameState.entrance == ENTRANCE_AFTER_BATTLE) {
      AltinPeak_AwardFrostJewelAndDrain();
    }
  }
  AltinPeak_StartPaletteAnim();
  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
    Engine_ActorSetPosition(ACTOR_ROOM1_PUDDLE, 0, 0);
  } else {
    actor = Object_GetById(ACTOR_ROOM1_PUDDLE);
    if (actor != 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Engine_ActorSetSpriteFlags(actor, 0);
    }
  }
  actor = Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE);
  if (actor != 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpriteFlags(actor, 0);
  }
  opened = Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN);
  if (opened == 0) {
    Engine_ActorSetChildValue(ACTOR_ROOM1_BEAST, 2);
    flag = Engine_GameFlagIsSet(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN);
    if (flag != 0) {
      Engine_ActorSetAnimation(ACTOR_ROOM1_WIDE_PUDDLE, 0);
      actor = Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE);
      actor->update = AltinPeak_SparkleIce;
      Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE)->motionFlags = opened;
      actor = Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE);
      actor->y.fixed = PIXELS(32);
      Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
      Engine_ActorSetPosition(ACTOR_ROOM1_BEAST, PIXELS(120), PIXELS(215));
      actor = Object_GetById(ACTOR_ROOM1_BEAST);
      actor->facing = opened;
      Engine_ActorSetAnimation(ACTOR_ROOM1_BEAST, 3);
      AltinPeak_StartSpitting(PIXELS(130), 0, PIXELS(168), 0);
      return;
    }
    if (Engine_GameFlagIsSet(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN) == 0) {
      return;
    }
    Engine_ActorSetAnimation(ACTOR_ROOM1_WIDE_PUDDLE, 0);
    actor = Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE);
    actor->update = AltinPeak_SparkleIce;
    Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE)->motionFlags = flag;
    actor = Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE);
    actor->y.fixed = PIXELS(32);
    Map_CopyCellAttributes(2, 0, 1, 1, 18, 13);
    Actor_SetPosition(ACTOR_ROOM1_BEAST, PIXELS(260), PIXELS(215));
    FieldObject_SetTargetAndCallback(
        10, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
        gAltinPeak_FaceLeaderScript);
  } else {
    map = gMapWork[0];
    Engine_ActorSetPosition(ACTOR_ROOM1_BEAST, 0, 0);
    Map_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    map->flags &= 0xfdff;
    AltinPeak_MarkActorTerrainCell(ACTOR_ROOM1_PUDDLE);
    if (Engine_GameFlagIsSet(FLAG_PUDDLE_FROZEN) != 0) {
      Engine_ActorSetAnimation(ACTOR_ROOM1_PUDDLE, 5);
      Map_CopyCellAttributes(7, 13, 1, 1, 9, 13);
      actor = Object_GetById(ACTOR_ROOM1_PUDDLE);
      actor->y.fixed = 0;
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
    AltinPeak_MarkActorTerrainCell(ACTOR_ROOM1_WIDE_PUDDLE);
    if (Engine_GameFlagIsSet(FLAG_SECOND_PUDDLE_FROZEN) != 0) {
      Engine_ActorSetAnimation(ACTOR_ROOM1_WIDE_PUDDLE, 5);
      Map_CopyCellAttributes(29, 1, 3, 1, 17, 13);
      actor = Object_GetById(ACTOR_ROOM1_WIDE_PUDDLE);
      actor->y.fixed = PIXELS(32);
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
  }
}

// Room 3: after its beast's battle the second drain runs; while the water is
// up the beast spits it; the switch is restored.
static void AltinPeak_RestoreRoom3(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapScrollWork *)gMapWork[0])->flags &= ~0x200;
    Actor_SetPosition(ACTOR_ROOM3_BEAST, 0, 0);
  } else {
    AltinPeak_StartPaletteAnim();
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0 &&
        gGameState.entrance == ENTRANCE_AFTER_BATTLE) {
      AltinPeak_DrainAfterSecondBeast();
    } else {
      Map_CopyCellAttributes(38, 24, 1, 2, 37, 24);
      Map_CopyCellAttributes(44, 23, 1, 2, 45, 23);
      if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) == 0) {
        Actor_SetChildValue(ACTOR_ROOM3_BEAST, 2);
        Engine_ActorSetAnimation(ACTOR_ROOM3_BEAST, 3);
        AltinPeak_StartSpitting(PIXELS(238), 0, PIXELS(418), 0x8000);
      }
    }
  }
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_3_SWITCH) != 0) {
    Map_CopyCellAttributes(0, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(0, 1, 24, 11, 1, 2);
  } else {
    Map_CopyCellAttributes(2, 0, 1, 1, 24, 80);
    Map_CopyCellsTo(2, 1, 24, 11, 1, 2);
  }
}

// Room 4: its water is up until the second drain; entrances 6 and 7 skip
// the arrival event.
static void AltinPeak_RestoreRoom4(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapScrollWork *)gMapWork[0])->flags &= ~0x200;
  } else {
    Map_CopyCellAttributes(52, 42, 1, 1, 53, 42);
  }
  {
    // Entrances 6 and 7 do not run the pending arrival event.
    if ((u32)(((u16)gGameState.entrance - 6) << 16) <= 0x10000) {
      GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    }
  }
}

// Room 5: after its beast's battle the third drain runs; while the water is
// up the beast spits it; the frozen puddle and the switch are restored.
static void AltinPeak_RestoreRoom5(void) {

  if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapScrollWork *)gMapWork[0])->flags &= ~0x200;
    Actor_SetPosition(ACTOR_ROOM5_BEAST, 0, 0);
  } else {
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gGameState.entrance == ENTRANCE_AFTER_BATTLE) {
        AltinPeak_DrainAfterThirdBeast();
      }
    }
    AltinPeak_StartPaletteAnim();
    if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) == 0) {
      Actor_SetChildValue(ACTOR_ROOM5_BEAST, 2);
      Engine_ActorSetAnimation(ACTOR_ROOM5_BEAST, 3);
      AltinPeak_StartSpitting(PIXELS(748), PIXELS(8), PIXELS(280), 0x8000);
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_ROOM5_PUDDLE);
  if (GameFlag_IsSet(FLAG_PUDDLE_FROZEN) != 0) {
    Engine_ActorSetAnimation(ACTOR_ROOM5_PUDDLE, 5);
    Map_CopyCellAttributes(23, 13, 1, 1, 25, 13);
    {
      struct FieldActor *actor = Actor_Get(ACTOR_ROOM5_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_5_SWITCH) != 0) {
    Map_CopyCellAttributes(10, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(49, 32, 11, 4, 1, 2);
  } else {
    Map_CopyCellAttributes(12, 72, 1, 1, 11, 73);
    Map_CopyCellsTo(48, 32, 11, 4, 1, 2);
  }
}

// Room 6: arriving at entrance 2 the log waits there; the frozen puddle, the
// water level and the switch are restored.
static void AltinPeak_RestoreRoom6(void) {
  struct FieldActor *pillar;

  if (gGameState.entrance == 2) {
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_LOG, PIXELS(358), PIXELS(104));
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_ROOM6_PUDDLE);
  if (GameFlag_IsSet(FLAG_PUDDLE_FROZEN) != 0) {
    pillar = Actor_Get(ACTOR_ROOM6_PUDDLE);
    Engine_ActorSetAnimation(ACTOR_ROOM6_PUDDLE, 5);
    Map_CopyCellAttributes(45, 41, 1, 1, 43, 41);
    {
      pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | pillar->priorityFlags;
    }
  }
  if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapScrollWork *)gMapWork[0])->flags &= ~0x200;
  }
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_6_SWITCH) != 0) {
    Map_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(46, 29, 16, 28, 1, 2);
  } else {
    Map_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    Map_CopyCellsTo(47, 29, 16, 28, 1, 2);
  }
}

// Room 7: the block, the three frozen puddles and the switch are restored.
static void AltinPeak_RestoreRoom7(void) {
  struct FieldActor *actor;

  actor = Actor_Get(ACTOR_ROOM7_PUDDLE);
  Engine_ActorSetSpriteFlags((s32)actor, 0);
  AltinPeak_SettleRoom7Block();
  AltinPeak_MarkActorTerrainCell(ACTOR_ROOM7_PUDDLE);
  if (GameFlag_IsSet(FLAG_PUDDLE_FROZEN) != 0) {
    Engine_ActorSetAnimation(ACTOR_ROOM7_PUDDLE, 5);
    Map_CopyCellAttributes(0, 0, 1, 1, 26, 26);
    {
      struct FieldActor *actor = Actor_Get(ACTOR_ROOM7_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_ROOM7_SECOND_PUDDLE);
  if (GameFlag_IsSet(FLAG_SECOND_PUDDLE_FROZEN) != 0) {
    Engine_ActorSetAnimation(ACTOR_ROOM7_SECOND_PUDDLE, 5);
    Map_CopyCellAttributes(1, 0, 1, 1, 17, 10);
    {
      struct FieldActor *actor = Actor_Get(ACTOR_ROOM7_SECOND_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_ROOM7_THIRD_PUDDLE);
  if (GameFlag_IsSet(FLAG_THIRD_PUDDLE_FROZEN) != 0) {
    Engine_ActorSetAnimation(ACTOR_ROOM7_THIRD_PUDDLE, 5);
    Map_CopyCellAttributes(1, 0, 1, 1, 26, 15);
    {
      struct FieldActor *actor = Actor_Get(ACTOR_ROOM7_THIRD_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  FieldScheduler_AddCallback(AltinPeak_UpdateFrostPillarPriority,
                             TASK_PRIORITY_SCENE);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_ROOM_7_SWITCH) != 0) {
    Map_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(46, 28, 29, 17, 1, 2);
  } else {
    Map_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    Map_CopyCellsTo(47, 28, 29, 17, 1, 2);
  }
}

// Room 9: the boulder is put away; once the rockfall is over the rock lies
// toppled and the rubble fills the bottom of the path. Arriving at entrance
// 6, the leader starts 88 pixels down.
static void AltinPeak_RestoreRoom9(void) {
  struct FieldActor *actor;

  actor = Object_GetById(ACTOR_TOPPLING_ROCK);
  Engine_ActorSetPosition(ACTOR_FALLING_BOULDER, 0, 0);
  Engine_ActorSetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_BOULDER_SHADOW), 0);
  actor->motionFlags = 0;
  actor->scaleX = 9 * Q16_ONE / 10;
  actor->scaleY = 3 * Q16_ONE / 5;
  actor->sprite->rotation = 0x8000;
  Object_GetById(12)->motionFlags = 0;
  Object_GetById(12)->y.fixed = -PIXELS(28);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    actor->x.fixed += PIXELS(14);
    actor->y.fixed += -PIXELS(8);
    actor->sprite->rotation = 0xc000;
  }
  if (Engine_GameFlagIsSet(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    Map_CopyCellsTo(25, 36, 43, 36, 11, 9);
    Map_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
  if (gGameState.entrance == 6 &&
      !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
    Engine_EventBegin();
    Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(88);
    Engine_CameraMoveTo(PIXELS(792), -PIXELS(88), PIXELS(577), 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventEnd();
  }
}

// Room 10: the guardian statue stands until the Hydros statues are beaten;
// after their battle the party talks it over.
static void AltinPeak_RestoreRoom10(void) {
  s32 statuesDefeated;
  struct FieldActor *leader;
  struct FieldActor *actor;
  s16 entrance;

  statuesDefeated = Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED);
  if (statuesDefeated != 0) {
    Engine_ActorSetPosition(ACTOR_STATUE, 0, 0);
    Engine_ActorSetPosition(ACTOR_STATUE_SHADOW, 0, 0);
  } else {
    actor = Object_GetById(ACTOR_STATUE);
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorSetSpritePriority(ACTOR_STATUE_SHADOW, 3);
    actor = Object_GetById(ACTOR_STATUE_SHADOW);
    Engine_ActorSetSpriteFlags(actor, 0);
    Object_GetById(ACTOR_STATUE_SHADOW)->collisionFlags = statuesDefeated;
  }
  entrance = gGameState.entrance;
  if (entrance == 1 || entrance == 98) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      Engine_EventBegin();
      leader->y.fixed = PIXELS(16);
      Engine_EventEnd();
    }
  } else if (entrance == ENTRANCE_AFTER_BATTLE) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      AltinPeak_CelebrateHydrosVictory();
    }
  }
}

// Room 11: arriving at entrance 2 the block stands at its pushed place.
static void AltinPeak_RestoreRoom11(void) {
  struct FieldActor *actor;

  actor = Actor_Get(ACTOR_ROOM11_BLOCK);
  Engine_ActorSetSpriteFlags(actor, 0);
  if (gGameState.entrance == 2) {
    Actor_SetPosition(ACTOR_ROOM11_BLOCK, PIXELS(184), PIXELS(328));
  }
}

// Gives a new spray particle its look: standing still, on sprite priority 1
// and at 0.3 scale.
static void AltinPeak_InitSpray(struct FieldActor *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  ObjectGroup_SetChildValue(spray, 3);
  Engine_ActorSetSpriteFlags(spray, 0);
  spray->scaleX = 3 * Q16_ONE / 10;
  spray->scaleY = 3 * Q16_ONE / 10;
}

// Update callback of a spray particle: it drifts away from the beast with a
// random wobble, rising and stretching for a few frames and then falling and
// growing, until its life runs out.
static void AltinPeak_DriftSpray(union FieldObject *object) {
  struct FieldActor *spray = &object->actor;

  switch (gAltinPeak_LeafMode) {
  case 0:
    spray->x.fixed +=
        (spray->timer << 12) +
        ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
    break;
  case 0x8000:
    spray->x.fixed -=
        (spray->timer << 12) +
        ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
    break;
  }
  if (spray->timer <= 3) {
    switch (gAltinPeak_LeafMode) {
    case 0:
      spray->x.fixed += PIXELS(1) / 2;
      break;
    case 0x8000:
      spray->x.fixed -= PIXELS(1) / 2;
      break;
    }
    spray->scaleX += Q16_ONE / 10;
    spray->scaleY -= Q16_ONE / 20;
  } else {
    spray->z.fixed += PIXELS(6) / 5;
    spray->scaleX += 3 * Q16_ONE / 100;
    spray->scaleY += 3 * Q16_ONE / 100;
  }
  if ((u32)(spray->timer * Engine_RandomNext()) >> 16 == 0)
    ObjectGroup_SetChildValue(spray, 7);
  if (spray->timer != 0)
    spray->timer -= 2;
  else
    spray->timer = ((u32)(Engine_RandomNext() * 5) >> 16) * 2 + 2;
  if (--object->effect.countdown == 0)
    Engine_ObjectDispatchRelease(spray);
}

// Scheduler task while a beast spits: every fourth frame a spray particle
// starts from its spout, living 20 frames.
static void AltinPeak_SpawnSpray(void) {
  union FieldObject *spray;

  if ((gFrameCount & 3) != 0)
    return;
  spray = (union FieldObject *)Engine_ObjectCreate(
      SPRITE_SPRAY, gAltinPeak_LeafOrigin[0], gAltinPeak_LeafOrigin[1],
      gAltinPeak_LeafOrigin[2]);
  if (spray == NULL)
    return;
  spray->actor.timer = 30;
  spray->actor.work = 1;
  spray->effect.countdown = 20;
  AltinPeak_InitSpray(&spray->actor);
  spray->effect.update = AltinPeak_DriftSpray;
  Object_SetMode(&spray->actor, 1);
}

// A water beast starts spitting water from (x, y, z) toward facing (east, 0,
// or west, 0x8000), with its sound (SE_170).
static void AltinPeak_StartSpitting(s32 x, s32 y, s32 z, s32 facing) {
  SET_VECTOR(gAltinPeak_LeafOrigin, x, y, z);
  gAltinPeak_LeafMode = facing;

  BattleFx_SetQueuedSoundAndPlay(SE_170);
  FieldScheduler_AddCallback(AltinPeak_SpawnSpray, TASK_PRIORITY_SCENE);
}

// Scheduler task while the boulder crashes: every third frame a piece of rock
// (the boulder's own sprite, small) flies out at random, with a clatter
// (SE_144 or SE_145) every ninth frame.
static void AltinPeak_SpawnDebris(void) {
  struct FieldActor *debris;
  s32 scale;

  if (gFrameCount % 3 == 0) {
    debris = Object_Create(SPRITE_BOULDER,
                           (((u32)(Engine_RandomNext() * 48) >> 16) << 16) +
                               PIXELS(765),
                           -PIXELS(64), PIXELS(608));
    if (debris != NULL) {
      if (gFrameCount % 9 == 0) {
        if ((u32)(Engine_RandomNext() << 1) >> 16 != 0)
          Engine_AudioPlayCue(SE_145);
        else
          Engine_AudioPlayCue(SE_144);
      }
      debris->motionFlags = 0;
      scale = ((u32)(Engine_RandomNext() << 15) >> 16) + 3 * Q16_ONE / 10;
      debris->gravity = PIXELS(2) / 5;
      debris->scaleY = scale;
      debris->scaleX = scale;
      debris->unk61 = 1;
      Engine_ActorSetSpriteFlags(debris, 0);
      debris->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      debris->sprite->image.priority = 1;
      Object_SetMode(debris, 1);
      Engine_ObjectSetScript(debris, gAltinPeak_DebrisScript);
      SET_VELOCITY(debris, (((u32)(Engine_RandomNext() * 6) >> 16) - 3) << 16,
                   PIXELS(8),
                   ((u32)(Engine_RandomNext() * 0x600) >> 16) - 0x300);
    }
  }
}

// Update callback of the frozen wide puddle: its ice glints every eight
// frames.
static void AltinPeak_SparkleIce(union FieldObject *object) {
  struct FieldActor *actor = (struct FieldActor *)object;

  s32 framePhase = gFrameCount & 7;

  if (framePhase == 0) {
    ObjectGroup_SetChildValue(actor, 2);
  } else if (framePhase == 2) {
    ObjectGroup_SetChildValue(actor, 0);
  }
}

// Room 1's water beast casts Frost on the wide puddle.
static void AltinPeak_BeastFreezesWidePuddle(void) {
  Engine_PsynergyBegin(ABILITY_FROST, 1);
  Engine_PsynergySetTarget(ACTOR_ROOM1_BEAST, ACTOR_ROOM1_WIDE_PUDDLE);
  Engine_PsynergyRaiseHands();
  Engine_ActorSetChildValue(ACTOR_ROOM1_BEAST, 2);
  Engine_PsynergyPlayEffect(1);
  Engine_ActorSetChildValue(ACTOR_ROOM1_BEAST, 2);
  Engine_PsynergyLowerHands();
  Engine_ActorSetChildValue(ACTOR_ROOM1_BEAST, 2);
  BattleEffect_CleanupSceneObjects();
  Engine_AudioPlayCue(SE_288);
  Engine_ActorSetChildValue(ACTOR_ROOM1_BEAST, 2);
}

// Back from room 1's beast battle: the beast is gone and the party finds the
// Frost Jewel; the camera pans to the water and the first drain runs.
static void AltinPeak_AwardFrostJewelAndDrain(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_ROOM1_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_ROOM1_PUDDLE, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Engine_ItemShowFound(ITEM_FROST_JEWEL, 3);
  Engine_PartyGiveItem(ITEM_FROST_JEWEL, 0);
  Camera_GetEventObject()->motionFlags = 0;
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(100), 0, PIXELS(249), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  AltinPeak_DrainFirstWater();
  Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  Engine_CameraWaitForMove();
  Event_LoadAreaScript(gAltinPeak_Events1Drained);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventEnd();
}

// Back from room 3's beast battle: the beast is gone, the log waits at
// (328, 424), the switch is back in its first position, and the second
// drain runs.
static void AltinPeak_DrainAfterSecondBeast(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_ROOM3_BEAST, 0, 0);
  Engine_ActorSetPosition(ACTOR_LOG, PIXELS(328), PIXELS(424));
  Engine_GameFlagSet(FLAG_ALTIN_PEAK_ROOM_3_SWITCH);
  AltinPeak_ToggleRoom3SwitchCells();
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  AltinPeak_DrainSecondWater();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventEnd();
}

// Back from room 5's beast battle, the same way, with the third drain.
static void AltinPeak_DrainAfterThirdBeast(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_ROOM5_BEAST, 0, 0);
  Engine_ActorSetPosition(ACTOR_LOG, PIXELS(488), PIXELS(138));
  Engine_GameFlagSet(FLAG_ALTIN_PEAK_ROOM_5_SWITCH);
  AltinPeak_ToggleRoom5SwitchCells();
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  AltinPeak_DrainThirdWater();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Engine_EventEnd();
}

// Scheduler task while the statue wakes: a rumble (SE_183) every second.
static void AltinPeak_PlayHydrosRumble(void) {
  gAltinPeak_RumbleTicks++;
  if (gAltinPeak_RumbleTicks == 60) {
    Engine_AudioPlayCue(SE_183);
    gAltinPeak_RumbleTicks = 0;
  }
}

// Stepping toward room 10's guardian statue: the leader walks up to it, it
// rumbles and slides forward, and the battle with the Hydros statues
// starts.
static void AltinPeak_StartHydrosBattle(void) {
  Engine_EventBegin();
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(328), -1, PIXELS(87), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
  Event_PlayCue(SE_148);
  FieldScheduler_AddCallback(AltinPeak_PlayHydrosRumble, TASK_PRIORITY_SCENE);
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_SetSpeed(ACTOR_STATUE, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_SetSpeed(ACTOR_STATUE_SHADOW, PIXELS(1) / 10, PIXELS(1) / 20);
  Engine_ActorSetAnimation(ACTOR_STATUE, 2);
  Actor_SetDestination(ACTOR_STATUE, 328, 104);
  Actor_SetDestination(ACTOR_STATUE_SHADOW, 328, 108);
  Engine_EventWait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorWaitForMove(ACTOR_STATUE);
  Battle_SetStart(3);
  Event_SetNextDestination(SCENE_MINE_ROOM(10), ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(53, 3);
}

// Scheduler task while the statue sinks: it flickers every other frame.
static void AltinPeak_FlickerStatue(void) {
  if (((gFrameCount >> 1) & 1) != 0) {
    Engine_ActorSetChildValue(ACTOR_STATUE, 7);
  } else {
    Engine_ActorSetChildValue(ACTOR_STATUE, 6);
  }
}

// Back from the Hydros statues' battle: Garet, Ivan and Mia step out. Garet
// cheers, Ivan asks whether the leader agrees, and Mia tells of the guardian
// statues; the statue shakes, sinks into the floor and shows a chest behind
// it, and they agree to open it before they go, then rejoin the leader.
static void AltinPeak_CelebrateHydrosVictory(void) {
  struct FieldActor *leader;
  struct EventWork *work;
  SchedulerCallback update;
  const s32 *celebrate;
  const s32 *party;

  Engine_EventBegin();
  Actor_SetPosition(ACTOR_STATUE, PIXELS(328), PIXELS(88));
  Actor_SetPosition(ACTOR_STATUE_SHADOW, PIXELS(328), PIXELS(88));
  Engine_ActorSetAnimation(ACTOR_STATUE, 0);
  work = gEventWork;
  work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  // Bring the party out for the victory conversation.
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetActionScript(GARET, gAltinPeak_GaretScript);
  Actor_SetActionScript(IVAN, gAltinPeak_IvanScript);
  Actor_RunActionScriptAndWait(MIA, gAltinPeak_MiaScript);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_EventSetMessage(MsgAltinWeDidIsaacWeBeat);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Event_OpenMessage(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  } else {
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
    Event_SkipMessages(1);
  }
  Event_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventSetMessage(MsgAltinGuardianStatuesWereCreatedLong);
  Event_ShowMessage(MIA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 60);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 40);
  Event_ShowMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 40);
  Event_PlayCue(SE_190);
  Actor_SetChildValue(ACTOR_STATUE, 7);
  Engine_EventWait(10);
  Event_PlayCue(SE_289);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 40);
  Event_PlayCue(SE_103);
  // Animate the statues while the party reacts.
  update = AltinPeak_FlickerStatue;
  Task_AddCallback(update, TASK_PRIORITY_SCENE);
  celebrate = gAltinPeak_StatueSinkScript;
  Actor_SetActionScript(ACTOR_STATUE_SHADOW, celebrate);
  Actor_RunActionScriptAndWait(ACTOR_STATUE, celebrate);
  Engine_TaskRemoveCallback(update);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  // Return the companions and save the completed Hydros battle.
  party = gAltinPeak_RejoinScript;
  Actor_SetActionScript(GARET, party);
  Actor_SetActionScript(IVAN, party);
  Actor_RunActionScriptAndWait(MIA, party);
  Engine_EventWait(20);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  gEventWork->transitionFrames = 16;
  GameFlag_Set(FLAG_HYDROS_STATUES_DEFEATED);
  Engine_EventEnd();
}

// Request the original unused Frost action.
UNUSED static void AltinPeak_RequestPsynergy(void) {
  gEventWork->psynergyRequest = 0x1018;
}

// Scheduler task in room 7: while the leader stands higher than 48 pixels
// the second pillar is drawn under the party and the third further back.
static void AltinPeak_UpdateFrostPillarPriority(void) {
  if (Object_GetById(ACTOR_PARTY_LEADER)->y.fixed > PIXELS(48)) {
    {
      u8 *flag = &Object_GetById(ACTOR_ROOM7_SECOND_PUDDLE)->priorityFlags;
      s32 bit = 2;

      bit |= *flag;
      *flag = bit;
    }
    Engine_ActorSetSpritePriority(ACTOR_ROOM7_THIRD_PUDDLE, 3);
  } else {
    Engine_ActorSetSpritePriority(ACTOR_ROOM7_THIRD_PUDDLE, 2);
  }
}

// Makes a puddle walkable to the actor checks and marks its map cell.
static void AltinPeak_MarkActorTerrainCell(s32 actorId) {
  struct FieldActor *actor;

  actor = Object_GetById(actorId);
  actor->collisionFlags &= 0xfe;

  SetMapCellCollision(0, actor->x.fixed, actor->z.fixed, 255);
}

// The log reaches the end of its run: it settles in the middle of its cell
// and the party leader jumps off it to the south.
static void AltinPeak_StopLogAndJumpOff(struct FieldActor *object) {
  s32 x;
  s32 z;
  s32 centerZ;
  struct AnimationObject *sprite;
  struct FieldActor *leader;

  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_232);
  x = object->x.fixed & 0xfff00000;
  centerZ = PIXELS(8);
  z = object->z.fixed & 0xfff00000;
  object->acceleration = PIXELS(2);
  x += centerZ;
  centerZ += z;
  Engine_ObjectSetPosition(object, x, object->y.fixed, centerZ);
  FieldObject_WaitForMove(object);
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
  sprite->count = 1;
  ResourceMetadata_ClearRecord(sprite->entries[1]);
  sprite->entries[1] = NULL;
  sprite->dirty = 1;
  leader = Engine_ActorLookup(gGameState.selectedActor);
  Engine_AudioPlayCue(SE_152);
  leader->x.fixed = x;
  leader->velocityY = PIXELS(6);
  leader->gravity = PIXELS(1);
  leader->z.fixed = centerZ;
  leader->sprite->image.priority = 0;
  Object_SetMode(leader, 7);
  Engine_ObjectSetPosition(leader, x, leader->y.fixed, z + PIXELS(24));
  leader->facing = FACING_SOUTH;
  Engine_TaskWait(20);
  leader->sprite->image.priority = 2;
  Engine_AudioPlayCue(SE_159);
}

// Retain the unused empty scene entry.
UNUSED static void AltinPeak_ReservedNoOp(void) {}

// The log turns a quarter turn right around the corner it reaches.
static void AltinPeak_TurnLogRight(struct FieldActor *object) {
  struct FieldPosition *point;
  struct FieldPosition position;
  s32 angle;
  s32 x;
  s32 z;
  s32 turnFrame;

  angle = (object->facing + 0x4000) & 0xc000;
  point = &position;
  SET_POSITION(point, object->x.fixed, object->y.fixed, object->z.fixed);
  FieldVector_AddPolarOffset(PIXELS(24), angle, point);
  x = (point->x + PIXELS(8)) & 0xfff00000;
  z = (point->z + PIXELS(8)) & 0xfff00000;
  angle += 0x8000;
  Object_SetMode(object, 5);
  turnFrame = 0;
  Engine_AudioPlayCue(SE_184);
  while (turnFrame < 16) {
    angle += 0x400;
    point->x = x;
    point->z = z;
    FieldVector_AddPolarOffset(PIXELS(24), angle, point);
    object->x.fixed = point->x;
    object->z.fixed = point->z;
    object->facing = angle + 0x4000;
    Engine_TaskWait(1);
    turnFrame++;
  }
  Engine_AudioPlayCue(SE_233);
}

// The log turns a quarter turn left.
static void AltinPeak_TurnLogLeft(struct FieldActor *object) {
  struct FieldPosition *point;
  struct FieldPosition position;
  s32 angle;
  s32 x;
  s32 z;
  s32 turnFrame;

  angle = (object->facing - 0x4000) & 0xc000;
  point = &position;
  SET_POSITION(point, object->x.fixed, object->y.fixed, object->z.fixed);
  FieldVector_AddPolarOffset(PIXELS(24), angle, point);
  x = (point->x + PIXELS(8)) & 0xfff00000;
  z = (point->z + PIXELS(8)) & 0xfff00000;
  angle += 0x8000;
  Object_SetMode(object, 6);
  turnFrame = 0;
  Engine_AudioPlayCue(SE_184);
  while (turnFrame < 16) {
    angle -= 0x400;
    point->x = x;
    point->z = z;
    FieldVector_AddPolarOffset(PIXELS(24), angle, point);
    object->x.fixed = point->x;
    object->z.fixed = point->z;
    object->facing = angle - 0x4000;
    Engine_TaskWait(1);
    turnFrame++;
  }
  Engine_AudioPlayCue(SE_233);
}

// The log rolls over a marked cell: SE_288 plays, and SE_184 and SE_233
// follow as its countdown runs out.
static void AltinPeak_StartLogCountdown(struct FieldActor *object) {
  object->work = 33;
  Engine_AudioPlayCue(SE_288);
}

// Rolls the log on by a frame and picks its animation from the terrain ahead
// of and behind it: flat, uphill or downhill.
static void AltinPeak_AdvanceLog(struct FieldActor *object) {
  s32 angle = 0xc000;
  s32 radius;
  s16 *timer;
  struct FieldPosition position;
  struct FieldPosition *point;
  s32 ahead;
  s32 behind;

  angle &= object->facing;
  radius = object->y.fixed / Q16_ONE;
  radius = gAltinPeak_RollRadii[object->timer - radius + 16];
  timer = (s16 *)&object->work;
  if (*timer != 0) {
    if (--*timer == 20) {
      Engine_AudioPlayCue(SE_184);
    }
    if (*timer == 0) {
      Engine_AudioPlayCue(SE_233);
    }
  }
  point = &position;
  SET_POSITION(point, object->x.fixed, object->y.fixed, object->z.fixed);
  FieldVector_AddPolarOffset(Iwram_MulQ16(radius, 3 * Q16_ONE / 4), angle,
                             point);
  object->x.fixed = point->x;
  object->z.fixed = point->z;
  ahead = Map_GetTerrainHeight(2, point->x, point->z);
  FieldVector_AddPolarOffset(-Iwram_MulQ16(radius, 3 * Q16_ONE / 2), angle,
                             point);
  behind = Map_GetTerrainHeight(2, point->x, point->z);
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

// Rolls the log the party leader rides: it sets off the first way it can
// roll, starting from heading (or its own facing for -1), follows the turns
// its route's cells mark until the end cell, and the leader jumps off.
static void AltinPeak_RollLog(s32 id, s32 heading) {
  struct FieldActor *object = Object_GetById(id);
  struct FieldActor *leader = Engine_ActorLookup(gGameState.selectedActor);
  struct FieldPosition position;
  s32 directionIndex;

  Engine_EventBegin();
  if (heading == -1) {
    heading = object->facing;
  }
  for (directionIndex = 0; directionIndex <= 3; directionIndex++) {
    position.x = object->x.fixed;
    position.y = object->y.fixed;
    position.z = object->z.fixed;
    FieldVector_AddPolarOffset(PIXELS(16), heading, &position);
    if (Map_GetTerrainHeight(2, position.x, position.z) == object->y.fixed) {
      break;
    }
    heading += 0x4000;
  }
  if (directionIndex == 4) {
    return;
  }
  object->terrainId = 2;
  leader->x.fixed = 0;
  leader->z.fixed = 0;
  ResourceMetadata_Register(object->sprite, 16);
  Engine_CameraFollowActor(id, 1);
  Engine_CameraWaitForMove();
  Camera_SetSpeed(PIXELS(16), PIXELS(2));
  object->facing = heading;
  object->speed = PIXELS(2);
  object->acceleration = PIXELS(1) / 20;
  object->paused = 0;
  object->timer = object->y.fixed / Q16_ONE;
  object->work = 0;
  position.x = object->x.fixed;
  position.y = object->y.fixed;
  position.z = object->z.fixed;
  FieldVector_AddPolarOffset(PIXELS(24), heading, &position);
  Engine_ObjectSetPosition(object, position.x, object->y.fixed, position.z);
  FieldObject_WaitForMove(object);
  Engine_AudioPlayCue(SE_233);
  // The route cells select turns, a countdown cue, and the destination.
  for (;;) {
    switch (GetMapCellCollision(2, object->x.fixed, object->z.fixed)) {
    case 98:
      AltinPeak_TurnLogRight(object);
      break;
    case 97:
      AltinPeak_TurnLogLeft(object);
      break;
    case 96:
      AltinPeak_StartLogCountdown(object);
      break;
    case 99:
      goto arrived;
    }
    AltinPeak_AdvanceLog(object);
    Engine_TaskWait(1);
  }
arrived:
  AltinPeak_StopLogAndJumpOff(object);
  Engine_EventEnd();
}

// Scheduler task that cycles the water's palette colors through
// gAltinPeak_PaletteScript.
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
    Dma_Set(palette, (void *)0x05000006, (DMA_ENABLE << 16) | 9, REG_DMA3);
  }
  gAltinPeak_PaletteHold--;
}

// Starts the water's palette cycle.
static void AltinPeak_StartPaletteAnim(void) {
  gAltinPeak_PaletteStep = 0;
  gAltinPeak_PaletteHold = 0;
  FieldScheduler_AddCallback(AltinPeak_StepPaletteAnim, TASK_PRIORITY_SCENE);
}

// clang-format off
// The water's palette cycle: pairs of a color offset and the frames it holds,
// repeating from the top at -1.
s8 gAltinPeak_PaletteScript[] = {
    3, 16,
    2, 32,
    1, 64,
    2, 32,
    -1,
};

// Room 1's beast keeps turning to face the party leader.
s32 gAltinPeak_FaceLeaderScript[] = {
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

// Room 9's boulder rolls straight down the path, and marks its arrival.
s32 gAltinPeak_BoulderPath[] = {
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

// The party leader flees down the winding path ahead of the boulder, marks
// the arrival, steps aside and turns round.
s32 gAltinPeak_LeaderFleePath[] = {
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
    SC_ARG(ANIM_STAND),
    SC_LABEL(1),
    SC_ADD(SW_FACING, 1024),
    SC_LOOP(24, SC_TO(1)),
    SC_END,
};

// A piece of debris flies for 60 frames and is freed.
s32 gAltinPeak_DebrisScript[] = {
    SC_CALL(AltinPeak_FlyDebris),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

// Garet, Ivan and Mia step out beside the leader after the Hydros battle.
s32 gAltinPeak_GaretScript[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(314), 0, PIXELS(126)),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_IvanScript[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(346), 0, PIXELS(126)),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_MiaScript[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(328), 0, PIXELS(138)),
    SC_SYNC,
    SC_ARG(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

// The guardian statue and its shadow flatten into the floor over 60 frames.
s32 gAltinPeak_StatueSinkScript[] = {
    SC_ADD(SW_SCALE_X, 128),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

// Garet, Ivan and Mia walk back to the leader and rejoin the party.
s32 gAltinPeak_RejoinScript[] = {
    SC_ARG(ANIM_WALK),
    SC_MOVE(PIXELS(328), 0, PIXELS(118)),
    SC_SYNC,
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gAltinPeak_EntrancesOther[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances1[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 456, 0, 520, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {98, CONDITION_ALWAYS, 456, 0, 488, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {99, CONDITION_ALWAYS, 104, 0, 232, FACING_SOUTH, 0, 0, 56, 506, 530, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances2[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 248, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 56, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances3[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 664, 0, 380, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 904, 0, 214, FACING_NORTH, 0, 842, 40, 1016, 248, 0},
    {98, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 320, 0, 472, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances4[] = {
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

struct SceneEntrance gAltinPeak_Entrances5[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {98, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {99, CONDITION_ALWAYS, 747, 0, 258, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances6[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 568, 0, 348, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {2, CONDITION_ALWAYS, 295, 0, 95, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {3, CONDITION_ALWAYS, 442, 0, 774, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {4, CONDITION_ALWAYS, 72, 0, 870, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {5, CONDITION_ALWAYS, 918, 0, 712, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {6, CONDITION_ALWAYS, 615, 0, 684, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances7[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 72, 0, 502, FACING_NORTH, 0, 0, 0, 820, 568, 0},
    {2, CONDITION_ALWAYS, 152, 0, 174, FACING_SOUTH, 0, 0, 0, 820, 568, 0},
    {3, CONDITION_ALWAYS, 264, 0, 890, FACING_NORTH, 0, 144, 720, 384, 908, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances8[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 392, 0, 116, FACING_SOUTH, 0, 0, 0, 830, 564, 0},
    {2, CONDITION_ALWAYS, 40, 0, 116, FACING_NORTH, 0, 0, 0, 830, 564, 0},
    {3, CONDITION_ALWAYS, 520, 0, 616, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {4, CONDITION_ALWAYS, 408, 0, 970, FACING_NORTH, 0, 244, 564, 1024, 1024, 0},
    {5, CONDITION_ALWAYS, 951, 0, 791, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances9[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 361, 0, 309, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {2, CONDITION_ALWAYS, 89, 0, 749, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {3, CONDITION_ALWAYS, 728, 0, 717, FACING_NORTH, 0, 618, 0, 990, 854, 0},
    {4, CONDITION_ALWAYS, 935, 0, 493, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {5, CONDITION_ALWAYS, 791, 0, 566, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {6, CONDITION_ALWAYS, 792, 16, 577, FACING_SOUTH, 0, 618, 0, 998, 854, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances10[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {98, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {99, CONDITION_ALWAYS, 328, 0, 118, FACING_NORTH, 0, 0, 0, 448, 524, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_Entrances11[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 408, 0, 517, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {2, CONDITION_ALWAYS, 198, 0, 336, FACING_SOUTH, 0, 0, 0, 540, 590, 0},
    {3, CONDITION_ALWAYS, 568, 0, 136, FACING_NORTH, 0, 480, 15, 684, 180, 0},
    {4, CONDITION_ALWAYS, 73, 0, 190, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {SCENE_TABLE_END},
};

s16 gAltinPeak_Regions9[] = {
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

s16 gAltinPeak_Regions10[] = {
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

u32 gAltinPeak_Exits[] = {
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

struct ScenePlacement gAltinPeak_PlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements1[] = {
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(152), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(296), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(296), 0, PIXELS(336), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements3[] = {
    {0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(216), 0, PIXELS(312), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(248), 0, PIXELS(464), FACING_WEST,
        TALK_KEEP_FACING, 0},
    {0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(888), 0, PIXELS(172), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements5[] = {
    {0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(296), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(408), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xbe, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(759), 0, PIXELS(285), FACING_WEST,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements6[] = {
    {0xe7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(712), 0, PIXELS(184), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {0xe3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(696), 0, PIXELS(664), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements7[] = {
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
    {0x5b, FLAG_DJINNI_0 + 23, ACTOR_STAND, PIXELS(56), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements8[] = {
    {0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(536), 0, PIXELS(636), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements9[] = {
    {0xc8, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(784), 0, PIXELS(80), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0x120, CONDITION_ALWAYS, 7, PIXELS(784), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0xcd, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(76), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x16, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {0x112, CONDITION_ALWAYS, 7, PIXELS(792), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements10[] = {
    {0xc7, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(84), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0x120, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(328), 0, PIXELS(88), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {GARET, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {IVAN, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {MIA, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gAltinPeak_Placements11[] = {
    {0x127, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(312), 0, PIXELS(280), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {0xd3, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(200), 0, PIXELS(328), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct SceneEvent gAltinPeak_EventsOther[] = {
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events1[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_TOUCH, 10, FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN, EVENT_SCRIPT(AltinPeak_ShowFirstBeastRoute)},
    {EVENT_TOUCH, 11, FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN, EVENT_SCRIPT(AltinPeak_ShowSecondBeastRoute)},
    {EVENT_TALK, ACTOR_ROOM1_BEAST, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FightRoom1Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_ROOM1_BEAST, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_FightRoom1Beast)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events1Drained[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM1_PUDDLE, FLAG_PUDDLE_FROZEN,
        EVENT_SCRIPT(AltinPeak_FreezeRoom1Puddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM1_WIDE_PUDDLE, FLAG_SECOND_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezeRoom1WidePuddle)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events2[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_TALK, ACTOR_LOG, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideLog)},
    {EVENT_TALK, ACTOR_ROOM3_BEAST, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FightRoom3Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_ROOM3_BEAST, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_FightRoom3Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_ROOM3_BOULDER, CONDITION_ALWAYS, 0},
    {SEARCH(SEARCH_UNNAMED), 83, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipRoom3Switch)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTIN_PEAK_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {SCENE_EVENTS_END},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {SEARCH(SEARCH_CHEST), 100, 0xf05, FIND_ITEM(ITEM_HERB)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events4[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events5[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM5_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezeRoom5Puddle)},
    {EVENT_TALK, ACTOR_ROOM5_BEAST, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FightRoom5Beast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_ROOM5_BEAST, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_FightRoom5Beast)},
    {EVENT_TALK, ACTOR_LOG, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideLog)},
    {SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipRoom5Switch)},
    // The Dragon Shield's chest opens only from its west side.
    {SEARCH_CHEST_FACING(FACING_SOUTHEAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {SEARCH_CHEST_FACING(FACING_EAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {SEARCH_CHEST_FACING(FACING_NORTHEAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events6[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM6_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezeRoom6Puddle)},
    {EVENT_TALK, ACTOR_LOG, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideLog)},
    {SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipRoom6Switch)},
    {SEARCH(SEARCH_CHEST), 100, 0xef2, 0x500002},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events7[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {SEARCH(SEARCH_CHEST), 101, FLAG_ALTIN_PEAK_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL)},
    {EVENT_TALK, ACTOR_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_TalkToDjinni)},
    {EVENT_TALK, ACTOR_LOG, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideLog)},
    {SEARCH(SEARCH_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipRoom7Switch)},
    {EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_PlaceBlockUnderLeader)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RestoreBlockAutomaticPriority)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROOM7_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_SettleRoom7Block)},
    {TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_PushRoom7Block)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM7_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezeRoom7Puddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM7_SECOND_PUDDLE, FLAG_SECOND_PUDDLE_FROZEN,
        EVENT_SCRIPT(AltinPeak_FreezeRoom7SecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_ROOM7_THIRD_PUDDLE, FLAG_THIRD_PUDDLE_FROZEN,
        EVENT_SCRIPT(AltinPeak_FreezeRoom7ThirdPuddle)},
    {SCENE_EVENTS_END},
};
// clang-format on

struct SceneEvent gAltinPeak_Events8[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTIN_PEAK_LUCKY_MEDAL,
     FIND_ITEM(ITEM_LUCKY_MEDAL)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_ROOM8_BOULDER, CONDITION_ALWAYS, 0},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events9[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {SEARCH(SEARCH_UNNAMED), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltinPeak_ReadFallingRockSign)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_WALL,
     FLAG_ALTIN_PEAK_BOULDER_FALLEN, EVENT_SCRIPT(AltinPeak_ForceRockOver)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events10[] = {
    {EVENT_TOUCH, 10, FLAG_HYDROS_STATUES_DEFEATED,
     EVENT_SCRIPT(AltinPeak_StartHydrosBattle)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTIN_PEAK_LIFTING_GEM,
     FIND_ITEM(ITEM_LIFTING_GEM)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gAltinPeak_Events11[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_ROOM11_BOULDER, CONDITION_ALWAYS,
     0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROOM11_BLOCK, CONDITION_ALWAYS, 0},
    {SEARCH(SEARCH_CHEST), 100, FLAG_ALTIN_PEAK_VIAL, FIND_ITEM(ITEM_VIAL)},
    {SCENE_EVENTS_END},
};

// Frames since the statue's last rumble.
s32 gAltinPeak_RumbleTicks = 0;

// The log's speed factor by its height against where it set off: 2 to the
// power i / 16 for i from 0 to 32, in 16.16.
u32 gAltinPeak_RollRadii[] = {
    0x10000, 0x10b55, 0x1172b, 0x12387, 0x1306f, 0x13dea, 0x14bfd,
    0x15ab0, 0x16a09, 0x17a11, 0x18ace, 0x19c49, 0x1ae89, 0x1c199,
    0x1d581, 0x1ea4b, 0x20000, 0x216ab, 0x22e56, 0x2470f, 0x260df,
    0x27bd4, 0x297fb, 0x2b560, 0x2d413, 0x2f422, 0x3159c, 0x33892,
    0x35d13, 0x38333, 0x3ab03, 0x3d495, 0x40000,
};

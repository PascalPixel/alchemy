/// @file
/// Altin Peak, the mine below Altin that its villagers and the Alpine
/// Crossing's signpost call Altin Mine.
///
/// It is eleven scenes, SceneId_AltinPeakFirstWaterBeast to
/// SceneId_AltinPeakWestExit, whose area name is Altin Peak. There is one
/// exception. The international editions call AltinPeakSouthEntrance the Alpine
/// Crossing when the party comes in from there
/// (ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST and
/// ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST).
///
/// Each scene is a room of the mine. The getters pick each room's tables by
/// gMainRoutineWork.next, and all the rooms share one exit table.
/// Scene_Initialize calls the setup of the room being entered
/// (AltinPeak_RestoreFirstWaterBeastRoom and so on), which puts back what the
/// party has already done there.
///
/// Altin's doorways lead down into AltinPeakFirstWaterBeast,
/// AltinPeakSouthEntrance, AltinPeakSpritz and AltinPeakJunction.
///
/// The mine is flooded, and a water beast waits in AltinPeakFirstWaterBeast,
/// AltinPeakSecondWaterBeast and AltinPeakThirdWaterBeast. It is
/// SPRITE_WATER_BEAST, drawn with its green and blue swapped
/// (SPRITE_COLOR_RBG).
///
/// In AltinPeakFirstWaterBeast the first one shows the way. When the party
/// steps near it (step trigger 10, AltinPeak_ShowFirstBeastRoute) it cries out,
/// hops around the room, freezes the second puddle with Frost and hops across.
/// When the party steps near it again (step trigger 11) it hops back and
/// settles down to spit.
///
/// Talking to a beast, or reading its mind, starts its battle. The party comes
/// back to the room by ENTRANCE_AFTER_BATTLE, where the room's setup runs a
/// drain. The water layer sinks and flickers away (AltinPeak_DrainFirstWater
/// and the two after it). FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN remember
/// the three drains, and Altin's code reads them to lower the water in the
/// village too.
///
/// Back from the first battle, in AltinPeakFirstWaterBeast, the party finds the
/// Frost Jewel before the water drains. From then on Frost freezes the mine's
/// puddles into ice pillars to stand on. Scene temp flags keep them frozen only
/// within one visit.
///
/// AltinPeakSouthEntrance's water stands until the second drain and
/// AltinPeakPool's until the third.
///
/// The rooms between are built from a few mechanisms:
///
/// - Mine carts the party rides by talking to one (AltinPeak_RollCart). They
///   roll along the turns their route's cells mark.
/// - Floor switches in AltinPeakSecondWaterBeast, AltinPeakThirdWaterBeast,
///   AltinPeakPool and AltinPeakSpritz. Each is remembered by an area temp flag
///   (FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH and the three after it)
///   and redrawn on entry.
/// - A stone pillar in AltinPeakSpritz that Move or a push slides along.
///
/// Spritz, a Mercury Djinni, waits in AltinPeakSpritz too, placed until it has
/// joined. Talking to it starts its battle.
///
/// In AltinPeakFallingRocks a sign warns `Watch for falling rocks! Do not
/// strike the wall! Rocks may fall!` at what looks like a dead end. While the
/// party has no Orb of Force, Garet steps out and strikes the wall anyway. With
/// the Orb, Force does the same (AltinPeak_ForceLogOver). Either way a boulder
/// rolls after the leader down the path and crashes into the wall at the
/// bottom, leaving rubble (FLAG_ALTIN_PEAK_BOULDER_FALLEN).
///
/// AltinPeakHydrosStatue, at the bottom, is a tiled room of carved stone, part
/// of ruins buried in the mine. Stepping toward its guardian statue
/// (AltinPeak_StartHydrosBattle) wakes it and starts the battle with the Hydros
/// statues.
///
/// Back from it (FLAG_HYDROS_STATUES_DEFEATED, which Altin's villagers read),
/// Garet, Ivan and Mia step out and talk. The statue sinks into the floor, and
/// a chest behind it holds the Lifting Gem.
///
/// The gem's Lift frees Hsu at the Alpine Crossing
/// (alpine_crossing_lama_temple.c). In the mine the boulders of
/// AltinPeakSecondWaterBeast, AltinPeakJunction and AltinPeakWestExit have Lift
/// rows that run nothing of their own.
///
/// AltinPeakSouthEntrance opens onto the Alpine Crossing
/// (ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST and
/// ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST) and AltinPeakWestExit
/// onto the World Map (ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP).
///
/// The falling boulder is SPRITE_ROCK, and its shadow is a SPRITE_BLANK_LARGE
/// actor. Its pieces in the crash are SPRITE_ROCK again, scaled down
/// (AltinPeak_SpawnDebris). The beasts' spray is SPRITE_DUST_PUFF recoloured,
/// as in Altin.

#include "macros.h"
#include "scene_ids.h"
#include "types.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "scene/import.h"
#include "system/main_routine_work.h"

extern s8 gAltinPeak_PaletteScript[];
extern s32 gAltinPeak_FaceLeaderScript[];
extern s32 gAltinPeak_PuddleGrowScript[];
extern s32 gAltinPeak_BoulderPath[];
extern s32 gAltinPeak_LeaderFleePath[];
extern s32 gAltinPeak_DebrisScript[];
extern s32 gAltinPeak_GaretScript[];
extern s32 gAltinPeak_IvanScript[];
extern s32 gAltinPeak_MiaScript[];
extern s32 gAltinPeak_StatueSinkScript[];
extern s32 gAltinPeak_RejoinScript[];
extern struct SceneEntrance gAltinPeak_EntrancesOther[];
extern struct SceneEntrance gAltinPeak_FirstWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_RailTunnelEntrances[];
extern struct SceneEntrance gAltinPeak_SecondWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_SouthEntranceEntrances[];
extern struct SceneEntrance gAltinPeak_ThirdWaterBeastEntrances[];
extern struct SceneEntrance gAltinPeak_PoolEntrances[];
extern struct SceneEntrance gAltinPeak_SpritzEntrances[];
extern struct SceneEntrance gAltinPeak_JunctionEntrances[];
extern struct SceneEntrance gAltinPeak_FallingRocksEntrances[];
extern struct SceneEntrance gAltinPeak_HydrosStatueEntrances[];
extern struct SceneEntrance gAltinPeak_WestExitEntrances[];
extern struct SceneExitRegion gAltinPeak_FallingRocksRegions[];
extern struct SceneExitRegion gAltinPeak_HydrosStatueRegions[];
extern u32 gAltinPeak_Exits[];
extern struct ActorPlacement gAltinPeak_ActorPlacementsOther[];
extern struct ActorPlacement gAltinPeak_FirstWaterBeastActorPlacements[];
extern struct ActorPlacement gAltinPeak_SecondWaterBeastActorPlacements[];
extern struct ActorPlacement gAltinPeak_ThirdWaterBeastActorPlacements[];
extern struct ActorPlacement gAltinPeak_PoolActorPlacements[];
extern struct ActorPlacement gAltinPeak_SpritzActorPlacements[];
extern struct ActorPlacement gAltinPeak_JunctionActorPlacements[];
extern struct ActorPlacement gAltinPeak_FallingRocksActorPlacements[];
extern struct ActorPlacement gAltinPeak_HydrosStatueActorPlacements[];
extern struct ActorPlacement gAltinPeak_WestExitActorPlacements[];
extern struct Trigger gAltinPeak_TriggersOther[];
extern struct Trigger gAltinPeak_FirstWaterBeastTriggers[];
extern struct Trigger gAltinPeak_FirstWaterBeastTriggersDrained[];
extern struct Trigger gAltinPeak_RailTunnelTriggers[];
extern struct Trigger gAltinPeak_SecondWaterBeastTriggers[];
extern struct Trigger gAltinPeak_SouthEntranceTriggers[];
extern struct Trigger gAltinPeak_ThirdWaterBeastTriggers[];
extern struct Trigger gAltinPeak_PoolTriggers[];
extern struct Trigger gAltinPeak_SpritzTriggers[];
extern struct Trigger gAltinPeak_JunctionTriggers[];
extern struct Trigger gAltinPeak_FallingRocksTriggers[];
extern struct Trigger gAltinPeak_HydrosStatueTriggers[];
extern struct Trigger gAltinPeak_WestExitTriggers[];
extern s32 gAltinPeak_RumbleFrames;
extern u32 gAltinPeak_RollRadii[];
// The spray's direction (FACING_EAST or FACING_WEST) and origin, and the
// water's flicker timer.
extern s32 gAltinPeak_SprayDirection;
extern s32 gAltinPeak_SprayOrigin[3];
extern u16 gAltinPeak_FlickerTimer;

#include "field/field_sequence.h"
#include "system/main_routine.h"
#include "field/event_commands.h"
#include "system/system.h"
#include "battle/battle.h"
#include "system/input.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "field/event.h"
#include "system/io_reg.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"
#include "field/field_effect.h"
#include "system/iwram_call.h"
#include "system/dma.h"

#include "djinn_ids.h"

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "field/map_grid_scroll.h"
#include "sound/sound_driver_symbols.h"
#include "scene/frost_glint.h"

// The rooms' floor switches, each set once flipped from its first position;
// they are area temp flags, kept until the party leaves the mine.

#define FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH FLAG_AREA_TEMP_23
#define FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH FLAG_AREA_TEMP_25
#define FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH FLAG_AREA_TEMP_26
#define FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH FLAG_AREA_TEMP_27

static void AltinPeak_RunBoulderChase(s32 boulderDelay);
static void AltinPeak_RestoreFirstWaterBeastRoom(void);
static void AltinPeak_RestoreSecondWaterBeastRoom(void);
static void AltinPeak_RestoreSouthEntranceRoom(void);
static void AltinPeak_RestoreThirdWaterBeastRoom(void);
static void AltinPeak_RestorePoolRoom(void);
static void AltinPeak_RestoreSpritzRoom(void);
static void AltinPeak_RestoreFallingRocksRoom(void);
static void AltinPeak_RestoreHydrosStatueRoom(void);
static void AltinPeak_RestoreWestExitRoom(void);
static void AltinPeak_StartSpittingFixed(s32 x, s32 y, s32 z, s32 facing);
/// AltinPeak_StartSpittingFixed from a point in whole pixels.
#define AltinPeak_StartSpitting(x, y, z, facing)                               \
  AltinPeak_StartSpittingFixed(Q16(x), Q16(y), Q16(z), facing)
static void AltinPeak_SpawnDebris(void);
static void FieldEffect_UpdateFrostGlint(struct Actor *actor);
static void AltinPeak_BeastFreezesSecondPuddle(void);
static void AltinPeak_AwardFrostJewelAndDrain(void);
static void AltinPeak_DrainAfterSecondBeast(void);
static void AltinPeak_DrainAfterThirdBeast(void);
static void AltinPeak_CelebrateHydrosVictory(void);
static void AltinPeak_UpdateFrostPillarPriority(void);
static void AltinPeak_MarkActorTerrainCell(s32 actorId);
static void AltinPeak_RollCart(s32 id, s32 heading);
static void AltinPeak_StartPaletteAnimation(void);

/// Each room's water beast fight returns the party to this entrance.
enum { ENTRANCE_AFTER_BATTLE = 99 };

/// The rooms' actors, from their actor placements.
///
/// AltinPeakSecondWaterBeast, AltinPeakThirdWaterBeast, AltinPeakPool and
/// AltinPeakSpritz place the mine cart the party rides first.
enum { ACTOR_CART = ACTOR_FIRST_PLACED };

/// AltinPeakFirstWaterBeast: a puddle that appears with the first drain, a
/// second puddle and the water beast.
enum {
  ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE = ACTOR_FIRST_PLACED,
  ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
  /// Its battle names it a Living Statue.
  ACTOR_FIRST_WATER_BEAST
};

enum {
  ACTOR_SECOND_WATER_BEAST = ACTOR_CART + 1,
  ACTOR_SECOND_WATER_BEAST_ROOM_BOULDER
};

enum {
  ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE = ACTOR_CART + 1,
  ACTOR_THIRD_WATER_BEAST
};

enum { ACTOR_POOL_ROOM_PUDDLE = ACTOR_CART + 1 };

enum {
  ACTOR_SPRITZ_ROOM_PUDDLE = ACTOR_CART + 1,
  ACTOR_SPRITZ_ROOM_PILLAR,
  ACTOR_SPRITZ_ROOM_SECOND_PUDDLE,
  ACTOR_SPRITZ_ROOM_THIRD_PUDDLE,
  /// Unused: a round boulder at (520, 632), outside both of the room's camera
  /// boxes, so the player never sees it, and no trigger names it.
  ///
  /// The boxes are gAltinPeak_SpritzEntrances'. One ends at z 568, the one
  /// below it at x 384. The boulder stands almost where AltinPeakJunction
  /// places its own boulder, (536, 636).
  ACTOR_UNUSED_SPRITZ_ROOM_BOULDER,
  /// The Mercury Djinni, placed until it has joined.
  ACTOR_SPRITZ = 14
};

enum { ACTOR_JUNCTION_ROOM_BOULDER = ACTOR_FIRST_PLACED };

/// AltinPeakFallingRocks: the boulder that falls and its shadow, the log that
/// topples as it comes down, the actor Force is cast on, and the sign.
enum {
  ACTOR_FALLING_BOULDER = ACTOR_FIRST_PLACED,
  ACTOR_BOULDER_SHADOW,
  /// SPRITE_LOG_NORTH_SOUTH.
  ACTOR_STANDING_LOG,
  /// SPRITE_BLANK, at the dead end's wall.
  ACTOR_WALL,
  /// The falling-rock sign, drawn with the sprite of Lalivero's sign and the
  /// Kolima Forest floodgate's (SPRITE_SIGNPOST). It is read by searching cell
  /// behavior 12 (AltinPeak_ReadFallingRockSign).
  ACTOR_FALLING_ROCK_SIGN
};

/// AltinPeakHydrosStatue: the guardian statue and its shadow.
enum { ACTOR_STATUE = ACTOR_FIRST_PLACED, ACTOR_STATUE_SHADOW };

enum {
  ACTOR_WEST_EXIT_ROOM_BOULDER = ACTOR_FIRST_PLACED,
  ACTOR_WEST_EXIT_ROOM_PILLAR
};

/// The rooms' frozen puddles and Garet at the sign. They are scene temp flags,
/// kept within one visit to a room, across its battles.
enum {
  /// Frost has frozen the room's first puddle.
  FLAG_ALTIN_PEAK_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  /// Frost has frozen AltinPeakFirstWaterBeast's second puddle or
  /// AltinPeakSpritz's second.
  FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
  /// Frost has frozen AltinPeakSpritz's third puddle.
  FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
  /// Garet has come out to look at AltinPeakFallingRocks's falling-rock sign.
  FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS = FLAG_SCENE_TEMP_05
};

/// A chest searched while facing the given way.
#define EXAMINE_CHEST_FACING(facing)                                           \
  (EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | (facing))

/// Talking to the Djinni in AltinPeakSpritz starts the battle with Spritz, a
/// Mercury Djinni.
static void AltinPeak_TalkToDjinni(void) {
  Field_RequestDjinnBattle(ACTOR_SPRITZ, DJINNI_SPRITZ);
}

#include "shared/actor_facing.c"

/// Update routine of AltinPeakFallingRocks's boulder shadow: it stays under the
/// boulder, on the ground 12 pixels down, while the boulder bounces.
static UNREAD_RESULT AltinPeak_ShadowFollowBoulder(struct Actor *follower) {
  struct Actor *actor = Actor_Get(ACTOR_FALLING_BOULDER);

  follower->x.fixed = actor->x.fixed;
  follower->y.fixed = -px(12);
  follower->z.fixed = actor->z.fixed;
#ifndef UBFIX
  return 0;
#endif
}

/// Actor script routine for a piece of flying rock debris. It moves by its
/// velocity, grows by its z velocity and falls under its gravity.
static s32 AltinPeak_FlyDebris(struct Actor *actor) {
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

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
    return gAltinPeak_FirstWaterBeastEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_RAIL_TUNNEL) {
    return gAltinPeak_RailTunnelEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
    return gAltinPeak_SecondWaterBeastEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
    return gAltinPeak_SouthEntranceEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
    return gAltinPeak_ThirdWaterBeastEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_POOL) {
    return gAltinPeak_PoolEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_SPRITZ) {
    return gAltinPeak_SpritzEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_JUNCTION) {
    return gAltinPeak_JunctionEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
    return gAltinPeak_FallingRocksEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
    return gAltinPeak_HydrosStatueEntrances;
  }
  if (sceneId == SCENE_ALTIN_PEAK_WEST_EXIT) {
    return gAltinPeak_WestExitEntrances;
  }
  return gAltinPeak_EntrancesOther;
}

/// AltinPeakFallingRocks and AltinPeakHydrosStatue have regions.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
    return gAltinPeak_FallingRocksRegions;
  }
  if (sceneId == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
    return gAltinPeak_HydrosStatueRegions;
  }
  return 0;
}

/// All the rooms share one exit table.
const u32 *Scene_GetExits(void) { return gAltinPeak_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
    return gAltinPeak_FirstWaterBeastActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
    return gAltinPeak_SecondWaterBeastActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
    return gAltinPeak_ThirdWaterBeastActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_POOL) {
    return gAltinPeak_PoolActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_SPRITZ) {
    return gAltinPeak_SpritzActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_JUNCTION) {
    return gAltinPeak_JunctionActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
    return gAltinPeak_FallingRocksActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
    return gAltinPeak_HydrosStatueActorPlacements;
  }
  if (sceneId == SCENE_ALTIN_PEAK_WEST_EXIT) {
    return gAltinPeak_WestExitActorPlacements;
  }
  return gAltinPeak_ActorPlacementsOther;
}

/// Talking to a mine cart makes the party leader jump into it.
static void AltinPeak_RideCart(s32 log) {
  struct Actor *logActor;
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  logActor = Actor_Get(ACTOR_CART);
  Field_EndBusyAndReveal();
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Event_Wait(10);
  Sound_Play(SE_JUMP);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3.2);
  // Leap by the height between leader and cart, in quarter pixels, plus 4.
  leader->velocityY =
      ((ABS(logActor->y.fixed - leader->y.fixed) >> 14) << 14) + px(4);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  FieldActor_SetMoveTargetFixed(leader, logActor->x.fixed, logActor->y.fixed,
                                logActor->z.fixed);
  wait(10);
  leader->sprite->image.priority = 3;
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);

  for (;;) {
    if (!((logActor->y.fixed >> 14) < (leader->y.fixed >> 14)))
      break;
    wait(1);
  }
  Event_End();
  Sound_Play(SE_DOOR_CLOSE);
  AltinPeak_RollCart(log, 0);
  wait(20);
  Scene_PlayQueuedMusic();
}

/// The party fights the first water beast.
static void AltinPeak_FightFirstWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_ALTIN_PEAK_FIRST_WATER_BEAST,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// The party fights the second water beast. It starts and ends as the first
/// one's does, from its own room.
static void AltinPeak_FightSecondWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_ALTIN_PEAK_SECOND_WATER_BEAST,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// The party fights the third water beast. It starts and ends as the first
/// one's does, from its own room.
static void AltinPeak_FightThirdWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_ALTIN_PEAK_THIRD_WATER_BEAST,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// Move on AltinPeakSpritz's pillar settles the cells under it.
static void AltinPeak_SettleSpritzRoomPillar(void) {
  struct Actor *pillar = Actor_Get(ACTOR_SPRITZ_ROOM_PILLAR);

  if (pillar != NULL) {
    s32 column;

    MapGrid_CopyCellAttributes(24, 27, 2, 1, 24, 26);
    column = FIELD_CELL(pillar->x.fixed);
    if (column == 25) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, column, 26);
    } else {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 26);
    }
    Actor_SetSpriteFlags(pillar, 0);
    pillar->motionFlags = 0;
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
}

/// Frost on a puddle freezes it into an ice pillar the party can stand on.
static void AltinPeak_FreezeFirstWaterBeastRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(7, 13, 1, 1, 9, 13);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= px(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakFirstWaterBeast's second puddle, three cells across. The
/// cells take the ice's attributes and the ice is left to fall to the ground.
static void AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(29, 1, 3, 1, 17, 13);
  if (puddle != NULL) {
    puddle->motionFlags = ACTOR_FALLS;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's first puddle.
static void AltinPeak_FreezeSpritzRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_SPRITZ_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 26, 26);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= px(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakThirdWaterBeast's puddle.
static void AltinPeak_FreezeThirdWaterBeastRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(23, 13, 1, 1, 25, 13);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= px(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakPool's puddle.
static void AltinPeak_FreezePoolRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_POOL_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(45, 41, 1, 1, 43, 41);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= px(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's second puddle. Its cell takes the map's cell
/// (1, 0), a jump spot too, standing higher than the first puddle's.
static void AltinPeak_FreezeSpritzRoomSecondPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 17, 10);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= px(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's third puddle.
static void AltinPeak_FreezeSpritzRoomThirdPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 26, 15);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= px(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN);
}

/// Interrupt handler while the water layer vanishes.
static void AltinPeak_FlickerWaterOut(void) {
  s16 control = REG_DISPCNT & ~DISPCNT_BG1_ON;
  s32 bg1 = DISPCNT_BG1_ON;
  /* FAKEMATCH(temporary): the new control widened into a word before its store.
   */
  u32 value;

  if (Random_Below(100) >= gAltinPeak_FlickerTimer)
    control |= bg1;
  value = (u16)control;
  REG_DISPCNT = value;
}

/// Flickers the water away while the timer counts to 100, then lets the
/// screen settle; every drain ends this way.
static inline void AltinPeak_FlickerWaterAway(void) {
  Irq_SetHandler(1, 0, AltinPeak_FlickerWaterOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    wait(1);
  } while (++gAltinPeak_FlickerTimer <= 100);
  wait(1);
  Irq_SetHandler(1, 0, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(30);
}

/// The first drain, after the Frost Jewel is found.
static void AltinPeak_DrainFirstWater(void) {
  struct MapGridScrollWork *map = gMapGridWork[0];
  struct MapGridLayerScroll *water;
  struct Actor *puddle;
  s32 step;

  Sound_Play(SE_WATER_DRAIN);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  water = &map->layers[MAPGRID_LAYER_OVERHEAD];
  for (step = 0; step < 24; step++) {
    water->offsetY -= px(1);
    wait(4);
    if (step == 8) {
      puddle = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
      puddle->scaleX = Q16_ONE / 10;
      puddle = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
      puddle->scaleY = Q16_ONE / 10;
      Actor_SetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 152, 216);
      Actor_SetScript(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE,
                      gAltinPeak_PuddleGrowScript);
    }
  }
  AltinPeak_FlickerWaterAway();
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 3, 14);
  GameFlag_Set(FLAG_ALTIN_MINE_FIRST_DRAIN);
}

/// The second drain is the same sinking and flicker over the cells the water
/// leaves behind, which then take their dry form.
static void AltinPeak_DrainSecondWater(void) {
  struct MapGridScrollWork *map = gMapGridWork[0];
  struct MapGridLayerScroll *water;
  s32 step;

  MapGrid_CopyCells(93, 41, 16, 4, 77, 28);
  Sound_Play(SE_WATER_DRAIN);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  water = &map->layers[MAPGRID_LAYER_OVERHEAD];
  for (step = 0; step < 24; step++) {
    water->offsetY -= px(1);
    wait(4);
  }
  AltinPeak_FlickerWaterAway();
  MapGrid_CopyCells(77, 41, 16, 4, 77, 28);
  GameFlag_Set(FLAG_ALTIN_MINE_SECOND_DRAIN);
}

/// The third drain, likewise.
static void AltinPeak_DrainThirdWater(void) {
  struct MapGridScrollWork *map = gMapGridWork[0];
  struct MapGridLayerScroll *water;
  s32 step;

  MapGrid_CopyCellsTo(113, 31, 103, 17, 1, 1);
  MapGrid_CopyCellsTo(111, 32, 104, 18, 3, 2);
  MapGrid_CopyCellsTo(64, 32, 103, 18, 1, 2);
  Sound_Play(SE_WATER_DRAIN);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  water = &map->layers[MAPGRID_LAYER_OVERHEAD];
  for (step = 0; step < 24; step++) {
    water->offsetY -= px(1);
    wait(4);
  }
  AltinPeak_FlickerWaterAway();
  MapGrid_CopyCellsTo(103, 14, 103, 17, 4, 3);
  GameFlag_Set(FLAG_ALTIN_MINE_THIRD_DRAIN);
}

/// Flips AltinPeakSecondWaterBeast's switch to its other position, redrawing it
/// and changing the cell its position opens or closes.
static void AltinPeak_ToggleSecondWaterBeastRoomSwitchCells(void) {
  if (Event_GetFlag(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 24, 80);
    MapGrid_CopyCellsTo(2, 1, 24, 11, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 80);
    MapGrid_CopyCellsTo(0, 1, 24, 11, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
  }
}

/// Searching AltinPeakSecondWaterBeast's switch flips it. "You flipped the
/// switch."
static void AltinPeak_FlipSecondWaterBeastRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_Play(SE_CHEST_CLOSE);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  wait(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Flips AltinPeakThirdWaterBeast's switch, the same way.
static void AltinPeak_ToggleThirdWaterBeastRoomSwitchCells(void) {
  if (Event_GetFlag(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(12, 72, 1, 1, 11, 73);
    MapGrid_CopyCellsTo(48, 32, 11, 4, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(10, 72, 1, 1, 11, 73);
    MapGrid_CopyCellsTo(49, 32, 11, 4, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
  }
}

/// Searching AltinPeakThirdWaterBeast's switch flips it.
static void AltinPeak_FlipThirdWaterBeastRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_Play(SE_CHEST_CLOSE);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  wait(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Searching AltinPeakPool's switch flips it.
static void AltinPeak_FlipPoolRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_Play(SE_CHEST_CLOSE);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(47, 29, 16, 28, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(46, 29, 16, 28, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
  }
  wait(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Searching AltinPeakSpritz's switch flips it.
static void AltinPeak_FlipSpritzRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_Play(SE_CHEST_CLOSE);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(47, 28, 29, 17, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(46, 28, 29, 17, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
  }
  wait(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Stepping onto AltinPeakSpritz's pillar draws it under the party leader.
static void AltinPeak_PlacePillarUnderLeader(void) {
  Actor_Get(ACTOR_SPRITZ_ROOM_PILLAR)->priorityFlags =
      ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
}

/// Stepping off AltinPeakSpritz's pillar lets it sort against the leader again.
static void AltinPeak_RestorePillarAutomaticPriority(void) {
  Actor_Get(ACTOR_SPRITZ_ROOM_PILLAR)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
}

/// Walking east into AltinPeakSpritz's pillar pushes it one cell along with a
/// scrape. The leader follows it and its cells are settled as after Move.
static void AltinPeak_PushSpritzRoomPillar(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(24, 27, 2, 1, 24, 26);
  Sound_Play(SE_PUSH);
  Actor_SetSpeed(ACTOR_SPRITZ_ROOM_PILLAR, 0.2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.2);
  Actor_Get(ACTOR_SPRITZ_ROOM_PILLAR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 400, 424);
  Actor_SetDestination(ACTOR_SPRITZ_ROOM_PILLAR, 408, 424);
  Actor_WaitForMoveById(ACTOR_SPRITZ_ROOM_PILLAR);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  AltinPeak_SettleSpritzRoomPillar();
  Event_End();
}

/// Force on AltinPeakFallingRocks's wall topples the log by it.
static void AltinPeak_ForceLogOver(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  s32 originX;
  s32 originHeight;
  s32 cosine;
  s32 sine;
  u16 rotationStep;

  actor = Actor_Get(ACTOR_STANDING_LOG);
  sprite = actor->sprite;
  originX = actor->x.fixed;
  originHeight = actor->y.fixed;
  Event_Start();
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(2, 1, 1);
  Event_Wait(10);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(20);

  // Rock the obstacle to each side before the accelerating fall.
  rotationStep = 0;
  while (1) {
    rotationStep += 8;
    sprite->rotation += rotationStep;
    cosine = Trig_Cos(sprite->rotation + TRIG_TURN / 4);
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation > 0x8fff) {
      break;
    }
    wait(1);
  }
  rotationStep = 0;
  while (1) {
    rotationStep += 8;
    sprite->rotation -= rotationStep;
    cosine = Trig_Cos(sprite->rotation + TRIG_TURN / 4);
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation <= TRIG_TURN * 7 / 16) {
      break;
    }
    wait(1);
  }
  rotationStep = 8;
  while (1) {
    rotationStep += rotationStep >> 3;
    sprite->rotation += rotationStep;
    cosine = Trig_Cos(sprite->rotation + TRIG_TURN / 4);
    sine = Trig_Sin(sprite->rotation + TRIG_TURN / 2);
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation > TRIG_TURN / 2) {
      actor->y.fixed = originHeight - (sine << 3);
    }
    if (sprite->rotation + rotationStep > 0xbfff) {
      break;
    }
    wait(1);
  }
  wait(1);
  sprite->rotation = TRIG_TURN * 3 / 4;
  Sound_Play(SE_STONE_RUMBLE);
  MapGrid_SetShake(3, 3, 1);
  Event_Wait(20);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  AltinPeak_RunBoulderChase(5);
  Event_End();
}

/// AltinPeakFallingRocks's rockfall.
static void AltinPeak_RunBoulderChase(s32 boulderDelay) {
  struct Actor *leader;
  struct Actor *boulder;
  struct Actor *shadow;
  struct Actor *rock;
  struct Actor *actor;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  boulder = Actor_Get(ACTOR_FALLING_BOULDER);
  shadow = Actor_Get(ACTOR_BOULDER_SHADOW);
  rock = Actor_Get(ACTOR_STANDING_LOG);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(784, CAMERA_KEEP, 116, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 792, 140);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 100);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Sound_Play(SE_STONE_RUMBLE);
  MapGrid_SetShake(3, 3, 1);
  Event_Wait(20);
  shadow->scaleX = 6 * Q16_ONE / 5;
  shadow->scaleY = 6 * Q16_ONE / 5;
  shadow->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  shadow->update = AltinPeak_ShadowFollowBoulder;
#else
  shadow->update = (void (*)(struct Actor *))AltinPeak_ShadowFollowBoulder;
#endif
  Actor_SetAnimation(ACTOR_FALLING_BOULDER, 4);
  boulder->bounce = Q16_ONE / 2;
  boulder->x.fixed = px(786);
  boulder->y.fixed = px(32);
  boulder->z.fixed = px(90);
  boulder->scaleX = 2 * Q16_ONE;
  boulder->scaleY = 2 * Q16_ONE;
  Event_Wait(10);
  Sound_Play(SE_STONE_RUMBLE);
  MapGrid_SetShake(4, 2, 1);
  Event_Wait(20);
  rock->x.fixed += px(14);
  rock->y.fixed -= px(8);
  rock->sprite->rotation = TRIG_TURN * 3 / 4;
  Sound_Play(SE_TREMOR);
  MapGrid_SetShake(1, 1, 1);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Sound_Play(MUS_BOULDER_CHASE);
  MapGrid_SetShake(1, 3, 1);
  Actor_SetSpritePriority(ACTOR_FALLING_BOULDER, 0);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2.5);
  leader->timer = 0;
  Actor_SetScript(ACTOR_PARTY_LEADER, gAltinPeak_LeaderFleePath);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    Actor_SetPosition(ACTOR_GARET, 878, 528);
    actor = Actor_Get(ACTOR_GARET);
    actor->facing = FACING_SOUTH_SOUTHWEST;
  }
  FieldCamera_SetSpeed(1.25);
  FieldCamera_MoveTo(786, CAMERA_KEEP, 556, 1);
  Event_Wait(boulderDelay);
  Actor_SetSpritePriority(ACTOR_FALLING_BOULDER, 1);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, 1.585);
  boulder->timer = 0;
  Actor_SetScript(ACTOR_FALLING_BOULDER, gAltinPeak_BoulderPath);
  do {
    wait(1);
  } while (leader->timer == 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
  do {
    wait(1);
  } while (boulder->timer == 0);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  shadow->x.fixed = px(786);
  shadow->update = NULL;
  shadow->z.fixed = px(618);
  shadow->y.fixed = -px(64);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, 1.6);
  boulder->bounce = Q16_ONE / 10;
  boulder->gravity = px(1) / 5;
  boulder->velocityY = px(4);
  Actor_MoveToAndWait(ACTOR_FALLING_BOULDER, 786, 604);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, 3.2);
  Actor_SetDestination(ACTOR_FALLING_BOULDER, 786, 644);
  Event_Wait(15);
  MapGrid_SetShake(5, 7, 1);
  MapGrid_CopyCellsTo(25, 36, 43, 36, 11, 9);
  MapGrid_CopyCellAttributes(25, 35, 10, 5, 43, 35);
  Actor_SetPosition(ACTOR_FALLING_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  Task_AddFrame(AltinPeak_SpawnDebris, TASK_PASS_FRAME);
  Event_Wait(80);
  Task_Remove(AltinPeak_SpawnDebris);
  Event_Wait(60);
  Sound_Play(SOUND_FADE_OUT_BGM);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(120);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    Actor_SetSpeed(ACTOR_GARET, 1);
    Actor_WalkTo(ACTOR_GARET, 824, 558);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 854, 584);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldCamera_MoveTo(788, -64, 610, 1);
  FieldCamera_WaitForMove();
  Sound_Play(SE_BOSS_ROAR);
  Event_Wait(240);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    FieldCamera_SetSpeed(4);
    FieldCamera_MoveTo(854, 0, 584, 1);
    FieldCamera_WaitForMove();
    Actor_WalkToAndWait(ACTOR_GARET, 840, 552);
    Actor_WalkToAndWait(ACTOR_GARET, 854, 562);
    Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel,
                           actor->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
  }
  Scene_PlayMusic();
  GameFlag_Set(FLAG_ALTIN_PEAK_BOULDER_FALLEN);
}

/// Searching AltinPeakFallingRocks's sign, which can bring the rockfall. `Watch
/// for falling rocks! Do not strike the wall! Rocks may fall!`
static void AltinPeak_ReadFallingRockSign(void) {
  struct Actor *actor;
  s32 message;

  Event_Start();
  message = MsgAltinWatchFallingRocks;
  FieldMessage_Show(message, 1);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_BOULDER_FALLEN) == 0 &&
      GameFlag_Get(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) == 0) {
    GameFlag_Set(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 790, 140);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 780, 140);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPositionFixed(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GARET, 0.8);
    Actor_WalkToAndWait(ACTOR_GARET, 800, 140);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
    Event_SetMessageId(message + 1);
    Actor_SetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "Tch! We came this deep into the mine just to find a dead end!"
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
    Actor_Jump(ACTOR_GARET, 6, 0);
    Actor_SetSpeed(ACTOR_GARET, 1.6);
    Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_GARET, 792, 110);
    Event_Wait(1);
    Actor_Get(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
    Sound_Play(SE_RATTLE);
    MapGrid_SetShake(2, 1, 1);
    Actor_Get(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_GARET, 792, 120);
    Event_Wait(1);
    {
      struct Actor *actor = Actor_Get(ACTOR_GARET);

      actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
    }
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    Event_Wait(80);
    Sound_Play(SE_RUMBLE);
    MapGrid_SetShake(1, 1, 1);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 40);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 20);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
    Actor_SetSpeed(ACTOR_GARET, 2.5);
    Actor_SetAnimation(ACTOR_GARET, 5);
    Actor_MoveToAndWait(ACTOR_GARET, 796, 138);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_MoveToAndWait(ACTOR_GARET, 804, 140);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_MoveToAndWait(ACTOR_GARET, 804, 166);
    Actor_MoveToAndWait(ACTOR_GARET, 764, 166);
    Actor_MoveToAndWait(ACTOR_GARET, 764, 198);
    Actor_MoveToAndWait(ACTOR_GARET, 786, 198);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_MoveToAndWait(ACTOR_GARET, 786, 246);
    Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Event_Wait(40);
    AltinPeak_RunBoulderChase(10);
  }
  Event_End();
}

/// AltinPeakFirstWaterBeast has its own triggers once the first drain has
/// lowered its water.
const struct Trigger *Scene_GetTriggers(void) {
  const struct Trigger *table;

  if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
    if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
      table = gAltinPeak_FirstWaterBeastTriggersDrained;
    } else {
      table = gAltinPeak_FirstWaterBeastTriggers;
    }
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_RAIL_TUNNEL) {
    table = gAltinPeak_RailTunnelTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
    table = gAltinPeak_SecondWaterBeastTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
    table = gAltinPeak_SouthEntranceTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
    table = gAltinPeak_ThirdWaterBeastTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_POOL) {
    table = gAltinPeak_PoolTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_SPRITZ) {
    table = gAltinPeak_SpritzTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_JUNCTION) {
    table = gAltinPeak_JunctionTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
    table = gAltinPeak_FallingRocksTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
    table = gAltinPeak_HydrosStatueTriggers;
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_WEST_EXIT) {
    table = gAltinPeak_WestExitTriggers;
  } else {
    table = gAltinPeak_TriggersOther;
  }
  return table;
}

/// Update routine of the hopping water beast. Once a second it leaps up and
/// lands with a splash (SE_SPLASH) that shakes the screen briefly.
static void AltinPeak_UpdateBeastHops(struct Actor *updated) {
  struct Actor *beast = updated;
  s16 *shakeDelay = (s16 *)&beast->userValue;

  // The route owns timer (hop phase), work (shake delay) and the effect's
  // countdown (landing-sound latch) until it removes this update routine.
  if (*shakeDelay != 0) {
    if (--*shakeDelay == 1)
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  }
  if (beast->velocityY == 0) {
    Actor_SelectAnimation(updated, 1);
    beast->y.fixed -= px(3) / 2;
    if (beast->y.fixed < beast->groundHeight) {
      if (((struct FieldEffect *)updated)->countdown != 0) {
        Sound_Play(SE_SPLASH);
        ((struct FieldEffect *)updated)->countdown = 0;
        beast->userValue = 4;
        MapGrid_SetShake(1, 0, 1);
      }
      beast->y.fixed = beast->groundHeight;
    }
    beast->paused = 1;
  } else {
    beast->paused = 0;
  }
  if (beast->timer == 0) {
    Sound_Play(SE_JUMP);
    ((struct FieldEffect *)updated)->countdown = 1;
    Actor_SelectAnimation(updated, 2);
    beast->velocityY = px(3);
  }
  if (++beast->timer == 60)
    beast->timer = 0;
}

/// The beast lands with a splash, shaking the screen by amplitude pixels for
/// holdFrames.
static inline void AltinPeak_PlayLandingShake(s32 amplitude, s32 holdFrames) {
  Sound_Play(SE_SPLASH);
  MapGrid_SetShakeFixed(px(amplitude), 0, Q16_ONE);
  Event_Wait(holdFrames);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/// Stepping near AltinPeakFirstWaterBeast's water beast the first time makes it
/// show the way.
static void AltinPeak_ShowFirstBeastRoute(void) {
  struct Actor *beast;
  struct Actor *leader;
  s32 jumpSpeed;

  beast = Actor_Get(ACTOR_FIRST_WATER_BEAST);
  Event_Start();
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(298, CAMERA_KEEP, 337, 1);
  FieldCamera_WaitForMove();
  Sound_Play(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_Wait(40);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(295, 32, 212, 1);
  ((struct FieldEffect *)beast)->countdown = 0;
  beast->timer = 0;
  beast->userValue = 0;
  beast->gravity = px(2) / 5;
  beast->update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 308, 291);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 311, 215);
  beast->update = 0;
  beast->paused = 0;
  Event_Wait(16);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  // Demonstrate Frost between the hopping sections.
  AltinPeak_BeastFreezesSecondPuddle();
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 40);
  FieldCamera_MoveTo(334, CAMERA_KEEP, 244, 1);
  ((struct FieldEffect *)beast)->countdown = 0;
  beast->timer = 0;
  beast->userValue = 0;
  beast->update = AltinPeak_UpdateBeastHops;
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 320, 232);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 340, 262);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 374, 262);
  beast->update = 0;
  Event_Wait(16);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_EAST_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTH_NORTHEAST, 40);
  Sound_Play(SE_HIGH_JUMP);
  Actor_Get(ACTOR_FIRST_WATER_BEAST)->velocityY = px(4);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_LEAP);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 380, 248);
  Event_Wait(10);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(6);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTH_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  FieldCamera_MoveTo(304, CAMERA_KEEP, 215, 1);
  ((struct FieldEffect *)beast)->countdown = 0;
  beast->timer = 0;
  beast->userValue = 0;
  beast->update = AltinPeak_UpdateBeastHops;
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 329, 219);
  beast->update = 0;
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Event_Wait(16);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags = 0;
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 17, 13);
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 18, 13);
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 19, 13);
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, 1.4);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WATER_BEAST), 0);
  Sound_Play(SE_HIGH_JUMP);
  jumpSpeed = px(5);
  Actor_Get(ACTOR_FIRST_WATER_BEAST)->velocityY = jumpSpeed;
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 295, 215);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WATER_BEAST), SPRITE_FLAG_SHADOW);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(40);
  Sound_Play(SE_HIGH_JUMP);
  Actor_Get(ACTOR_FIRST_WATER_BEAST)->velocityY = jumpSpeed;
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 260, 215);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 20);
  Sound_Play(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_Wait(40);
  MapGrid_CopyCellAttributes(4, 0, 1, 1, 17, 13);
  MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
  MapGrid_CopyCellAttributes(4, 0, 1, 1, 19, 13);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveWithinMapBounds(leader->x.fixed, leader->y.fixed,
                                  leader->z.fixed, 1);
  FieldCamera_WaitForMove();
  FieldActor_WatchAndStartScript(ACTOR_FIRST_WATER_BEAST,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                 gAltinPeak_FaceLeaderScript);
  GameFlag_Set(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN);
  Event_End();
}

/// When the party steps near the beast again, it hops back over the ice and
/// settles down, spitting water.
static void AltinPeak_ShowSecondBeastRoute(void) {
  struct Actor *actor;
  struct Actor *movingActor;
  u32 waitedFrames;

  actor = Actor_Get(ACTOR_FIRST_WATER_BEAST);
  Event_Start();
  Actor_Stop(ACTOR_FIRST_WATER_BEAST);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(279, 64, 216, 1);
  FieldCamera_WaitForMove();
  Sound_Play(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(128, 64, 202, 1);
  ((struct FieldEffect *)actor)->countdown = 0;
  actor->timer = 0;
  actor->userValue = 0;
  actor->gravity = px(2) / 5;
  actor->update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 212, 200);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 103, 200);
  actor->update = NULL;
  actor->paused = 0;
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 40);
  Actor_Get(ACTOR_FIRST_WATER_BEAST)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
  movingActor = Actor_Get(ACTOR_FIRST_WATER_BEAST);
  Actor_SetSpriteFlags(movingActor, 0);
  Sound_Play(SE_HIGH_JUMP);
  movingActor = Actor_Get(ACTOR_FIRST_WATER_BEAST);
  movingActor->velocityY = px(4);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 86, 214);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  movingActor = Actor_Get(ACTOR_FIRST_WATER_BEAST);
  Actor_SetSpriteFlags(movingActor, SPRITE_FLAG_SHADOW);
  Event_Wait(10);
  AltinPeak_PlayLandingShake(2, 8);
  Event_Wait(40);
  Actor_Get(ACTOR_FIRST_WATER_BEAST)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_EAST, 40);
  ((struct FieldEffect *)actor)->countdown = 0;
  actor->timer = 0;
  actor->userValue = 0;
  actor->update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 120, 215);
  actor->update = NULL;
  actor->paused = 0;
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Event_Wait(16);
  AltinPeak_PlayLandingShake(1, 4);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_EAST, 10);
  Sound_Play(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_Wait(80);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  AltinPeak_StartSpitting(130, 0, 168, FACING_EAST);
  Event_Wait(60);
  for (waitedFrames = 0; waitedFrames < 60 && gKeysPressed == 0;
       waitedFrames++) {
    Event_Wait(1);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveWithinMapBounds(actor->x.fixed, actor->y.fixed,
                                  actor->z.fixed, 1);
  FieldCamera_WaitForMove();
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN);
  Event_End();
}

/// Sets up the room being entered, with puzzles already solved and the water
/// levels already lowered. Returns 0.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
    AltinPeak_RestoreFirstWaterBeastRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
    AltinPeak_RestoreSecondWaterBeastRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
    AltinPeak_RestoreSouthEntranceRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
    AltinPeak_RestoreThirdWaterBeastRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_POOL) {
    AltinPeak_RestorePoolRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_SPRITZ) {
    AltinPeak_RestoreSpritzRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
    AltinPeak_RestoreFallingRocksRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
    AltinPeak_RestoreHydrosStatueRoom();
  } else if (gMainRoutineWork.next == SCENE_ALTIN_PEAK_WEST_EXIT) {
    AltinPeak_RestoreWestExitRoom();
  }
  return 0;
}

/// Sets up AltinPeakFirstWaterBeast.
static void AltinPeak_RestoreFirstWaterBeastRoom(void) {
  struct Actor *actor;
  struct MapGridScrollWork *grid;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
      AltinPeak_AwardFrostJewelAndDrain();
    }
  }
  AltinPeak_StartPaletteAnimation();
  if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
    Actor_SetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
  } else {
    actor = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
    if (actor != NULL) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetSpriteFlags(actor, 0);
    }
  }
  actor = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  if (actor != NULL) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteFlags(actor, 0);
  }
  if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
    Actor_SetSpriteColorById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
    if (GameFlag_Get(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN) != 0) {
      Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, 0);
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->update =
          FieldEffect_UpdateFrostGlint;
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->y.fixed = px(32);
      MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
      Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 120, 215);
      Actor_Get(ACTOR_FIRST_WATER_BEAST)->facing = FACING_EAST;
      Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
      AltinPeak_StartSpitting(130, 0, 168, FACING_EAST);
      return;
    }
    if (GameFlag_Get(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN) == 0) {
      return;
    }
    Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, 0);
    Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->update =
        FieldEffect_UpdateFrostGlint;
    Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags = 0;
    Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->y.fixed = px(32);
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
    Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 260, 215);
    FieldActor_WatchAndStartScript(10,
                                   ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gAltinPeak_FaceLeaderScript);
  } else {
    grid = gMapGridWork[0];
    Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    grid->flags &= ~DISPCNT_BG1_ON;
    AltinPeak_MarkActorTerrainCell(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
    if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
      Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE,
                         PUDDLE_ANIMATION_PILLAR);
      MapGrid_CopyCellAttributes(7, 13, 1, 1, 9, 13);
      actor = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
      actor->y.fixed = 0;
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
    AltinPeak_MarkActorTerrainCell(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
    if (GameFlag_Get(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN) != 0) {
      Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
                         PUDDLE_ANIMATION_PILLAR);
      MapGrid_CopyCellAttributes(29, 1, 3, 1, 17, 13);
      actor = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
      actor->y.fixed = px(32);
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
  }
}

/// Sets up AltinPeakSecondWaterBeast.
static void AltinPeak_RestoreSecondWaterBeastRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~DISPCNT_BG1_ON;
    Actor_SetPosition(ACTOR_SECOND_WATER_BEAST, 0, 0);
  } else {
    AltinPeak_StartPaletteAnimation();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
        gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
      AltinPeak_DrainAfterSecondBeast();
    } else {
      MapGrid_CopyCellAttributes(38, 24, 1, 2, 37, 24);
      MapGrid_CopyCellAttributes(44, 23, 1, 2, 45, 23);
      if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) == 0) {
        Actor_SetSpriteColorById(ACTOR_SECOND_WATER_BEAST, SPRITE_COLOR_RBG);
        Actor_SetAnimation(ACTOR_SECOND_WATER_BEAST,
                           WATER_BEAST_ANIMATION_SPIT);
        AltinPeak_StartSpitting(238, 0, 418, FACING_WEST);
      }
    }
  }
  if (GameFlag_Get(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 80);
    MapGrid_CopyCellsTo(0, 1, 24, 11, 1, 2);
  } else {
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 24, 80);
    MapGrid_CopyCellsTo(2, 1, 24, 11, 1, 2);
  }
}

/// Sets up AltinPeakSouthEntrance.
static void AltinPeak_RestoreSouthEntranceRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~DISPCNT_BG1_ON;
  } else {
    MapGrid_CopyCellAttributes(52, 42, 1, 1, 53, 42);
  }
  {
    // Either entrance from the Alpine Crossing: _EAST is the number
    // after ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST.
    if ((u32)(((u16)gMainRoutineWork.entrance -
               ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST)
              << 16) <= 1 << 16) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    }
  }
}

/// Sets up AltinPeakThirdWaterBeast.
static void AltinPeak_RestoreThirdWaterBeastRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~DISPCNT_BG1_ON;
    Actor_SetPosition(ACTOR_THIRD_WATER_BEAST, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
        AltinPeak_DrainAfterThirdBeast();
      }
    }
    AltinPeak_StartPaletteAnimation();
    if (GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) == 0) {
      Actor_SetSpriteColorById(ACTOR_THIRD_WATER_BEAST, SPRITE_COLOR_RBG);
      Actor_SetAnimation(ACTOR_THIRD_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
      AltinPeak_StartSpitting(748, 8, 280, FACING_WEST);
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
    Actor_SetAnimation(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE,
                       PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(23, 13, 1, 1, 25, 13);
    {
      struct Actor *actor = Actor_Get(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  if (GameFlag_Get(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(10, 72, 1, 1, 11, 73);
    MapGrid_CopyCellsTo(49, 32, 11, 4, 1, 2);
  } else {
    MapGrid_CopyCellAttributes(12, 72, 1, 1, 11, 73);
    MapGrid_CopyCellsTo(48, 32, 11, 4, 1, 2);
  }
}

/// Sets up AltinPeakPool.
static void AltinPeak_RestorePoolRoom(void) {
  struct Actor *pillar;

  if (gMainRoutineWork.entrance == ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_CART, 358, 104);
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_POOL_ROOM_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
    pillar = Actor_Get(ACTOR_POOL_ROOM_PUDDLE);
    Actor_SetAnimation(ACTOR_POOL_ROOM_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(45, 41, 1, 1, 43, 41);
    {
      pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | pillar->priorityFlags;
    }
  }
  if (GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~DISPCNT_BG1_ON;
  }
  // The cells AltinPeak_FlipPoolRoomSwitch copies as it sets the flag, redrawn
  // on arrival.
  if (GameFlag_Get(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(46, 29, 16, 28, 1, 2);
  } else {
    MapGrid_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(47, 29, 16, 28, 1, 2);
  }
}

/// Sets up AltinPeakSpritz. The pillar, the three frozen puddles and the switch
/// are restored.
static void AltinPeak_RestoreSpritzRoom(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_SPRITZ_ROOM_PUDDLE);
  Actor_SetSpriteFlags(actor, 0);
  AltinPeak_SettleSpritzRoomPillar();
  AltinPeak_MarkActorTerrainCell(ACTOR_SPRITZ_ROOM_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
    Actor_SetAnimation(ACTOR_SPRITZ_ROOM_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 26, 26);
    {
      struct Actor *actor = Actor_Get(ACTOR_SPRITZ_ROOM_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN) != 0) {
    Actor_SetAnimation(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE,
                       PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 17, 10);
    {
      struct Actor *actor = Actor_Get(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN) != 0) {
    Actor_SetAnimation(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 26, 15);
    {
      struct Actor *actor = Actor_Get(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  Task_AddFrame(AltinPeak_UpdateFrostPillarPriority, TASK_PASS_FRAME);
  // The cells AltinPeak_FlipSpritzRoomSwitch copies as it sets the flag,
  // redrawn on arrival.
  if (GameFlag_Get(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(46, 28, 29, 17, 1, 2);
  } else {
    MapGrid_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(47, 28, 29, 17, 1, 2);
  }
}

/// Sets up AltinPeakFallingRocks.
static void AltinPeak_RestoreFallingRocksRoom(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_STANDING_LOG);
  Actor_SetPosition(ACTOR_FALLING_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BOULDER_SHADOW), 0);
  actor->motionFlags = 0;
  actor->scaleX = 9 * Q16_ONE / 10;
  actor->scaleY = 3 * Q16_ONE / 5;
  actor->sprite->rotation = TRIG_TURN / 2;
  Actor_Get(ACTOR_FALLING_ROCK_SIGN)->motionFlags = 0;
  Actor_Get(ACTOR_FALLING_ROCK_SIGN)->y.fixed = -px(28);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    actor->x.fixed += px(14);
    actor->y.fixed -= px(8);
    actor->sprite->rotation = TRIG_TURN * 3 / 4;
  }
  // The cells AltinPeak_RunBoulderChase copies as it sets the flag, redrawn on
  // arrival.
  if (GameFlag_Get(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    MapGrid_CopyCellsTo(25, 36, 43, 36, 11, 9);
    MapGrid_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  if (gMainRoutineWork.entrance ==
          ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    Event_Start();
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = -px(88);
    FieldCamera_MoveTo(792, -88, 577, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    Event_End();
  }
}

/// Sets up AltinPeakHydrosStatue. The guardian statue stands until the Hydros
/// statues are beaten. After their battle the party talks it over.
static void AltinPeak_RestoreHydrosStatueRoom(void) {
  s32 statuesDefeated;
  struct Actor *leader;
  s16 entrance;

  statuesDefeated = GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED);
  if (statuesDefeated != 0) {
    Actor_SetPosition(ACTOR_STATUE, 0, 0);
    Actor_SetPosition(ACTOR_STATUE_SHADOW, 0, 0);
  } else {
    Actor_SetSpriteFlags(Actor_Get(ACTOR_STATUE), 0);
    Actor_SetSpritePriority(ACTOR_STATUE_SHADOW, 3);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_STATUE_SHADOW), 0);
    Actor_Get(ACTOR_STATUE_SHADOW)->collisionFlags = statuesDefeated;
  }
  entrance = gMainRoutineWork.entrance;
  if (entrance == ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS ||
      entrance == 98) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      Event_Start();
      leader->y.fixed = px(16);
      Event_End();
    }
  } else if (entrance == ENTRANCE_AFTER_BATTLE) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      AltinPeak_CelebrateHydrosVictory();
    }
  }
}

/// Sets up AltinPeakWestExit.
static void AltinPeak_RestoreWestExitRoom(void) {
  Actor_SetSpriteFlags(Actor_Get(ACTOR_WEST_EXIT_ROOM_PILLAR), 0);
  if (gMainRoutineWork.entrance ==
      ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM) {
    Actor_SetPosition(ACTOR_WEST_EXIT_ROOM_PILLAR, 184, 328);
  }
}

/// Gives a new spray particle its look. It stands still, on sprite priority 1
/// and at 0.3 scale.
static void AltinPeak_InitializeSpray(struct Actor *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  Actor_SetSpriteColor(spray, SPRITE_COLOR_BGR);
  Actor_SetSpriteFlags(spray, 0);
  spray->scaleX = 3 * Q16_ONE / 10;
  spray->scaleY = 3 * Q16_ONE / 10;
}

/// Update routine of a spray particle.
static void AltinPeak_DriftSpray(struct Actor *spray) {
  switch (gAltinPeak_SprayDirection) {
  case FACING_EAST:
    spray->x.fixed +=
        (spray->timer << 12) + ((s32)(((Random_Below(2)) - 1) << 16) >> 1);
    break;
  case FACING_WEST:
    spray->x.fixed -=
        (spray->timer << 12) + ((s32)(((Random_Below(2)) - 1) << 16) >> 1);
    break;
  }
  if (spray->timer <= 3) {
    switch (gAltinPeak_SprayDirection) {
    case FACING_EAST:
      spray->x.fixed += px(1) / 2;
      break;
    case FACING_WEST:
      spray->x.fixed -= px(1) / 2;
      break;
    }
    spray->scaleX += Q16_ONE / 10;
    spray->scaleY -= Q16_ONE / 20;
  } else {
    spray->z.fixed += px(6) / 5;
    spray->scaleX += 3 * Q16_ONE / 100;
    spray->scaleY += 3 * Q16_ONE / 100;
  }
  if (Random_Below(spray->timer) == 0)
    Actor_SetSpriteColor(spray, SPRITE_COLOR_WHITE);
  if (spray->timer != 0)
    spray->timer -= 2;
  else
    spray->timer = (Random_Below(5)) * 2 + 2;
  if (--((struct FieldEffect *)spray)->countdown == 0)
    Actor_Release(spray);
}

/// Task while a beast spits. Every fourth frame a spray particle starts from
/// its spout, living 20 frames.
static void AltinPeak_SpawnSpray(void) {
  struct Actor *spray;

  if ((gFrameCount & 3) != 0)
    return;
  spray = (struct Actor *)Actor_Create(
      SPRITE_DUST_PUFF, gAltinPeak_SprayOrigin[0], gAltinPeak_SprayOrigin[1],
      gAltinPeak_SprayOrigin[2]);
  if (spray == NULL)
    return;
  spray->timer = 30;
  spray->userValue = 1;
  ((struct FieldEffect *)spray)->countdown = 20;
  AltinPeak_InitializeSpray(spray);
  ((struct FieldEffect *)spray)->update = AltinPeak_DriftSpray;
  Actor_SelectAnimation(spray, 1);
}

/// A water beast starts spitting water from (x, y, z), in 16.16 fixed point,
/// toward facing
/// (FACING_EAST or FACING_WEST), with its sound (SE_AMBIENT_WATER).
static void AltinPeak_StartSpittingFixed(s32 x, s32 y, s32 z, s32 facing) {
  SET_VECTOR(gAltinPeak_SprayOrigin, x, y, z);
  gAltinPeak_SprayDirection = facing;

  Scene_QueueMusic(SE_AMBIENT_WATER);
  Task_AddFrame(AltinPeak_SpawnSpray, TASK_PASS_FRAME);
}

/// Task while the boulder crashes.
static void AltinPeak_SpawnDebris(void) {
  struct Actor *debris;
  s32 scale;

  if (gFrameCount % 3 == 0) {
    debris = FieldActor_Create(
        SPRITE_ROCK, ((Random_Below(48)) << 16) + px(765), -px(64), px(608));
    if (debris != NULL) {
      if (gFrameCount % 9 == 0) {
        if (Random_Below(2) != 0)
          Sound_Play(SE_CRASH);
        else
          Sound_Play(SE_CLATTER);
      }
      debris->motionFlags = 0;
      scale = (Random_Below(32768)) + 3 * Q16_ONE / 10;
      debris->gravity = px(2) / 5;
      debris->scaleY = scale;
      debris->scaleX = scale;
      debris->frozen = 1;
      Actor_SetSpriteFlags(debris, 0);
      debris->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      debris->sprite->image.priority = 1;
      Actor_SelectAnimation(debris, 1);
      Actor_StartScript(debris, gAltinPeak_DebrisScript);
      SET_VELOCITY(debris, ((Random_Below(6)) - 3) << 16, px(8),
                   (Random_Below(1536)) - 768);
    }
  }
}

/// The frozen second puddle's glint.
DEFINE_FROST_GLINT(static, Actor_SetSpriteColor)

/// AltinPeakFirstWaterBeast's water beast casts Frost on the second puddle.
static void AltinPeak_BeastFreezesSecondPuddle(void) {
  FieldPsynergy_LoadResources(ABILITY_FROST, 1);
  FieldPsynergy_SetupActorPair(ACTOR_FIRST_WATER_BEAST,
                               ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  FieldPsynergy_StartCast();
  Actor_SetSpriteColorById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  FieldPsynergy_RunHandler(1);
  Actor_SetSpriteColorById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  FieldPsynergy_StopCasterUpdate();
  Actor_SetSpriteColorById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  FieldPsynergy_End();
  Sound_Play(SE_STOP_ACTION_SE);
  Actor_SetSpriteColorById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
}

/// Back from AltinPeakFirstWaterBeast's beast battle, the beast is gone and the
/// party finds the Frost Jewel.
static void AltinPeak_AwardFrostJewelAndDrain(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Item_ShowFound(ITEM_FROST_JEWEL, 3);
  Party_GiveItem(ITEM_FROST_JEWEL, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(100, 0, 249, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  AltinPeak_DrainFirstWater();
  FieldCamera_MoveToFixed(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  FieldCamera_WaitForMove();
  Event_SetTriggers(gAltinPeak_FirstWaterBeastTriggersDrained);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Back from AltinPeakSecondWaterBeast's beast battle, the beast is gone and
/// the mine cart waits at (328, 424).
static void AltinPeak_DrainAfterSecondBeast(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_SECOND_WATER_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_CART, 328, 424);
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  AltinPeak_DrainSecondWater();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Back from AltinPeakThirdWaterBeast's beast battle, the same way, with the
/// third drain.
static void AltinPeak_DrainAfterThirdBeast(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_THIRD_WATER_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_CART, 488, 138);
  GameFlag_Set(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  AltinPeak_DrainThirdWater();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Task while the statue wakes. A rumble (SE_STONE_RUMBLE) plays every second.
static void AltinPeak_PlayHydrosRumble(void) {
  gAltinPeak_RumbleFrames++;
  if (gAltinPeak_RumbleFrames == 60) {
    Sound_Play(SE_STONE_RUMBLE);
    gAltinPeak_RumbleFrames = 0;
  }
}

/// Stepping toward AltinPeakHydrosStatue's guardian statue wakes it.
static void AltinPeak_StartHydrosBattle(void) {
  Event_Start();
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 87, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
  Sound_Play(SE_BOSS_ROAR);
  Task_AddFrame(AltinPeak_PlayHydrosRumble, TASK_PASS_FRAME);
  MapGrid_SetShake(1, 1, 1);
  Actor_SetSpeed(ACTOR_STATUE, 0.1);
  Actor_SetSpeed(ACTOR_STATUE_SHADOW, 0.1);
  Actor_SetAnimation(ACTOR_STATUE, 2);
  Actor_SetDestination(ACTOR_STATUE, 328, 104);
  Actor_SetDestination(ACTOR_STATUE_SHADOW, 328, 108);
  Event_Wait(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_WaitForMoveById(ACTOR_STATUE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_ALTIN_PEAK_HYDROS_STATUE,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 3);
}

/// Task while the statue sinks. It flickers every other frame.
static void AltinPeak_FlickerStatue(void) {
  if (((gFrameCount >> 1) & 1) != 0) {
    Actor_SetSpriteColorById(ACTOR_STATUE, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorById(ACTOR_STATUE, SPRITE_COLOR_GREY);
  }
}

/// Back from the Hydros statues' battle, Garet, Ivan and Mia step out.
static void AltinPeak_CelebrateHydrosVictory(void) {
  struct Actor *leader;
  struct FieldWork *work;
  TaskFunc update;
  const s32 *celebrate;
  const s32 *party;

  Event_Start();
  Actor_SetPosition(ACTOR_STATUE, 328, 88);
  Actor_SetPosition(ACTOR_STATUE_SHADOW, 328, 88);
  Actor_SetAnimation(ACTOR_STATUE, 0);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFixed(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, 0.6);
  Actor_SetSpeed(ACTOR_IVAN, 0.6);
  Actor_SetSpeed(ACTOR_MIA, 0.6);
  Actor_SetScript(ACTOR_GARET, gAltinPeak_GaretScript);
  Actor_SetScript(ACTOR_IVAN, gAltinPeak_IvanScript);
  Actor_RunScriptAndWait(ACTOR_MIA, gAltinPeak_MiaScript);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Event_SetMessageId(MsgAltinWeDidIsaacWeBeat);
  // "We did it, Isaac! We beat the last water beast!"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  // "I wonder why Altin's guardian statues turned into monsters...."
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  } else {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_SkipMessages(1);
  }
  Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_SetMessageId(MsgAltinGuardianStatuesWereCreatedLong);
  // "The guardian statues were created long, long ago."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 40);
  // "What are they for?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Sound_Play(SE_WARP);
  Actor_SetSpriteColorById(ACTOR_STATUE, SPRITE_COLOR_WHITE);
  Event_Wait(10);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Sound_Play(SE_CURSE);
  update = AltinPeak_FlickerStatue;
  Task_AddFrame(update, TASK_PASS_FRAME);
  celebrate = gAltinPeak_StatueSinkScript;
  Actor_SetScript(ACTOR_STATUE_SHADOW, celebrate);
  Actor_RunScriptAndWait(ACTOR_STATUE, celebrate);
  Task_Remove(update);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "There's a treasure chest behind the guardian statue!"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  // "It must all be related to these ruins, buried deep in the..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  // "I guess so... Hey, Isaac, we should check that chest before..."
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  party = gAltinPeak_RejoinScript;
  Actor_SetScript(ACTOR_GARET, party);
  Actor_SetScript(ACTOR_IVAN, party);
  Actor_RunScriptAndWait(ACTOR_MIA, party);
  Event_Wait(20);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  gFieldWork->transitionFrames = 16;
  GameFlag_Set(FLAG_HYDROS_STATUES_DEFEATED);
  Event_End();
}

/// Unused: asks the field to have Felix use Frost. Nothing calls it.
UNUSED static void AltinPeak_RequestPsynergy(void) {
  // XXX (no evidence): why the user is Felix, who never joins the party in
  // this game; nothing calls this, so no caller shows what it was for.
  gFieldWork->psynergy = (FELIX << 10) | ABILITY_FROST;
}

/// Task in AltinPeakSpritz. While the leader stands higher than 48 pixels the
/// second pillar is drawn under the party and the third further back.
static void AltinPeak_UpdateFrostPillarPriority(void) {
  if (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed > px(48)) {
    Actor_Get(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 3);
  } else {
    Actor_SetSpritePriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 2);
  }
}

/// Makes a puddle walkable to the actor checks and marks its map cell.
static void AltinPeak_MarkActorTerrainCell(s32 actorId) {
  struct Actor *actor;

  actor = Actor_Get(actorId);
  actor->collisionFlags &= (u8)~ACTOR_SOLID;

  MapGrid_SetCellBehaviorAt(0, actor->x.fixed, actor->z.fixed,
                            CELL_BEHAVIOR_WALL);
}

/// The mine cart reaches the end of its run. It settles in the middle of its
/// cell and the party leader jumps off it to the south.
static void AltinPeak_StopCartAndJumpOff(struct Actor *actor) {
  s32 x;
  s32 z;
  s32 centerZ;
  struct Sprite *sprite;
  struct Actor *leader;

  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_CRUNCH);
  x = actor->x.fixed & FIELD_CELL_MASK;
  centerZ = px(8);
  z = actor->z.fixed & FIELD_CELL_MASK;
  actor->acceleration = px(2);
  x += centerZ;
  centerZ += z;
  FieldActor_SetMoveTargetFixed(actor, x, actor->y.fixed, centerZ);
  Actor_WaitForMove(actor);
  actor->layer = 0;
  actor->x.fixed = x;
  actor->z.fixed = centerZ;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  Actor_SelectAnimation(actor, 2);
  wait(15);
  Actor_SelectAnimation(actor, 1);
  wait(30);
  sprite = actor->sprite;
  sprite->count = 1;
  SpriteAnimation_Clear(sprite->entries[1]);
  sprite->entries[1] = NULL;
  sprite->dirty = 1;
  leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  Sound_Play(SE_JUMP);
  leader->x.fixed = x;
  leader->velocityY = px(6);
  leader->gravity = px(1);
  leader->z.fixed = centerZ;
  leader->sprite->image.priority = 0;
  Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
  FieldActor_SetMoveTargetFixed(leader, x, leader->y.fixed, z + px(24));
  leader->facing = FACING_SOUTH;
  wait(20);
  leader->sprite->image.priority = 2;
  Sound_Play(SE_DOOR_CLOSE);
}

UNUSED static void AltinPeak_ReservedNoOp(void) {}

/// The cart turns a quarter turn right around the corner it reaches.
static void AltinPeak_TurnCartRight(struct Actor *actor) {
  struct FieldPosition position;
  s32 angle;
  s32 x;
  s32 z;
  s32 turnFrame;

  angle = (actor->facing + TRIG_TURN / 4) & 0xc000;
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffset(24, angle, &position);
  x = (position.x + px(8)) & FIELD_CELL_MASK;
  z = (position.z + px(8)) & FIELD_CELL_MASK;
  angle += TRIG_TURN / 2;
  Actor_SelectAnimation(actor, 5);
  turnFrame = 0;
  Sound_Play(SE_CART_TURN);
  while (turnFrame < 16) {
    angle += TRIG_TURN / 64;
    position.x = x;
    position.z = z;
    FieldVector_AddPolarOffset(24, angle, &position);
    actor->x.fixed = position.x;
    actor->z.fixed = position.z;
    actor->facing = angle + TRIG_TURN / 4;
    wait(1);
    turnFrame++;
  }
  Sound_Play(SE_CART_ROLL);
}

/// The cart turns a quarter turn left.
static void AltinPeak_TurnCartLeft(struct Actor *actor) {
  struct FieldPosition position;
  s32 angle;
  s32 x;
  s32 z;
  s32 turnFrame;

  angle = (actor->facing - TRIG_TURN / 4) & 0xc000;
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffset(24, angle, &position);
  x = (position.x + px(8)) & FIELD_CELL_MASK;
  z = (position.z + px(8)) & FIELD_CELL_MASK;
  angle += TRIG_TURN / 2;
  Actor_SelectAnimation(actor, 6);
  turnFrame = 0;
  Sound_Play(SE_CART_TURN);
  while (turnFrame < 16) {
    angle -= TRIG_TURN / 64;
    position.x = x;
    position.z = z;
    FieldVector_AddPolarOffset(24, angle, &position);
    actor->x.fixed = position.x;
    actor->z.fixed = position.z;
    actor->facing = angle - TRIG_TURN / 4;
    wait(1);
    turnFrame++;
  }
  Sound_Play(SE_CART_ROLL);
}

/// The cart rolls over a marked cell. SE_STOP_ACTION_SE plays, and SE_CART_TURN
/// and SE_CART_ROLL follow as its countdown runs out.
static void AltinPeak_StartCartCountdown(struct Actor *actor) {
  actor->userValue = 33;
  Sound_Play(SE_STOP_ACTION_SE);
}

/// Rolls the cart on by a frame and picks its animation from the terrain ahead
/// of and behind it: flat, uphill or downhill.
static void AltinPeak_AdvanceCart(struct Actor *actor) {
  s32 angle = TRIG_TURN * 3 / 4;
  s32 radius;
  s16 *timer;
  struct FieldPosition position;
  s32 ahead;
  s32 behind;

  angle &= actor->facing;
  radius = actor->y.fixed / Q16_ONE;
  radius = gAltinPeak_RollRadii[actor->timer - radius + 16];
  timer = (s16 *)&actor->userValue;
  if (*timer != 0) {
    if (--*timer == 20) {
      Sound_Play(SE_CART_TURN);
    }
    if (*timer == 0) {
      Sound_Play(SE_CART_ROLL);
    }
  }
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffsetFixed(Iwram_MulQ16(radius, 3 * Q16_ONE / 4), angle,
                                  &position);
  actor->x.fixed = position.x;
  actor->z.fixed = position.z;
  ahead = MapGrid_GetTerrainHeight(2, position.x, position.z);
  FieldVector_AddPolarOffsetFixed(-Iwram_MulQ16(radius, 3 * Q16_ONE / 2), angle,
                                  &position);
  behind = MapGrid_GetTerrainHeight(2, position.x, position.z);
  if (*timer <= 20) {
    if (ahead == behind) {
      Actor_SelectAnimation(actor, 2);
    } else if (ahead > behind) {
      Actor_SelectAnimation(actor, 3);
    } else {
      Actor_SelectAnimation(actor, 4);
    }
  }
}

/// The cell behaviors that mark the mine cart's route, on map layer 2.
enum AltinPeakCellBehavior {
  CELL_BEHAVIOR_ALTIN_PEAK_CART_COUNTDOWN =
      96, ///< AltinPeak_StartCartCountdown
  CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_LEFT = 97,
  CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_RIGHT = 98,
  /// The end of the route, where the leader jumps off.
  CELL_BEHAVIOR_ALTIN_PEAK_CART_STOP = 99
};

/// Rolls the mine cart the party leader rides.
static void AltinPeak_RollCart(s32 id, s32 heading) {
  struct Actor *actor = Actor_Get(id);
  struct Actor *leader = Actor_Lookup(gMainRoutineWork.leaderActorId);
  struct FieldPosition position;
  s32 directionIndex;

  Event_Start();
  if (heading == -1) {
    heading = actor->facing;
  }
  for (directionIndex = 0; directionIndex <= 3; directionIndex++) {
    position.x = actor->x.fixed;
    position.y = actor->y.fixed;
    position.z = actor->z.fixed;
    FieldVector_AddPolarOffset(16, heading, &position);
    if (MapGrid_GetTerrainHeight(2, position.x, position.z) == actor->y.fixed) {
      break;
    }
    heading += TRIG_TURN / 4;
  }
  if (directionIndex == 4) {
    return;
  }
  actor->layer = 2;
  leader->x.fixed = 0;
  leader->z.fixed = 0;
  Sprite_AddAnimation(actor->sprite, SPRITE_CART_RIDER);
  FieldCamera_FollowActor(id, 1);
  FieldCamera_WaitForMove();
  FieldCamera_SetSpeed(16);
  actor->facing = heading;
  actor->speedLimit = px(2);
  actor->acceleration = px(1) / 20;
  actor->paused = 0;
  actor->timer = actor->y.fixed / Q16_ONE;
  actor->userValue = 0;
  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed;
  FieldVector_AddPolarOffset(24, heading, &position);
  FieldActor_SetMoveTargetFixed(actor, position.x, actor->y.fixed, position.z);
  Actor_WaitForMove(actor);
  Sound_Play(SE_CART_ROLL);
  // The route cells select turns, a countdown sound, and the destination.
  for (;;) {
    switch (MapGrid_GetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed)) {
    case CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_RIGHT:
      AltinPeak_TurnCartRight(actor);
      break;
    case CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_LEFT:
      AltinPeak_TurnCartLeft(actor);
      break;
    case CELL_BEHAVIOR_ALTIN_PEAK_CART_COUNTDOWN:
      AltinPeak_StartCartCountdown(actor);
      break;
    case CELL_BEHAVIOR_ALTIN_PEAK_CART_STOP:
      goto arrived;
    }
    AltinPeak_AdvanceCart(actor);
    wait(1);
  }
arrived:
  AltinPeak_StopCartAndJumpOff(actor);
  Event_End();
}

/// Task that cycles the water's palette colors through
/// gAltinPeak_PaletteScript.
static void AltinPeak_StepPaletteAnimation(void) {
  // The palette cycle borrows RGB555 colours from the palette fade's source
  // palette; each step replaces BG colours 3 to 11.
  u16 *palette = gPaletteFadeWork->palette;

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
    Dma_Set(palette, (void *)&BG_PLTT_COLOR(0, 3), (DMA_ENABLE << 16) | 9,
            REG_DMA3);
  }
  gAltinPeak_PaletteHold--;
}

/// Starts the water's palette cycle.
static void AltinPeak_StartPaletteAnimation(void) {
  gAltinPeak_PaletteStep = 0;
  gAltinPeak_PaletteHold = 0;
  Task_AddFrame(AltinPeak_StepPaletteAnimation, TASK_PASS_FRAME);
}

// clang-format off
/// The water's palette cycle. It is pairs of a color offset and the frames it
/// holds, repeating from the top at -1.
s8 gAltinPeak_PaletteScript[] = {
    3, 16,
    2, 32,
    1, 64,
    2, 32,
    -1,
};

/// AltinPeakFirstWaterBeast's beast keeps turning to face the party leader.
s32 gAltinPeak_FaceLeaderScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The new puddle grows to full size over 108 frames.
s32 gAltinPeak_PuddleGrowScript[] = {
    SC_ADD(SW_SCALE_X, 546),
    SC_ADD(SW_SCALE_Y, 546),
    SC_WAIT(1),
    SC_LOOP(108, SC_TOP),
    SC_END,
};

/// AltinPeakFallingRocks's boulder rolls straight down the path, and marks its
/// arrival.
s32 gAltinPeak_BoulderPath[] = {
    SC_MOVE(786, 0, 220),
    SC_SYNC,
    SC_MOVE(786, 0, 326),
    SC_SYNC,
    SC_MOVE(786, 0, 326),
    SC_SYNC,
    SC_MOVE(786, 0, 480),
    SC_SYNC,
    SC_MOVE(786, 0, 520),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_END,
};

/// The party leader flees down the winding path ahead of the boulder, marks
/// the arrival, steps aside and turns round.
s32 gAltinPeak_LeaderFleePath[] = {
    SC_ANIMATION(5),
    SC_MOVE(804, 0, 140),
    SC_SYNC,
    SC_MOVE(804, 0, 166),
    SC_SYNC,
    SC_MOVE(764, 0, 166),
    SC_SYNC,
    SC_MOVE(764, 0, 198),
    SC_SYNC,
    SC_MOVE(786, 0, 198),
    SC_SYNC,
    SC_MOVE(786, 0, 246),
    SC_SYNC,
    SC_MOVE(764, 0, 246),
    SC_SYNC,
    SC_MOVE(764, 0, 282),
    SC_SYNC,
    SC_MOVE(780, 0, 298),
    SC_SYNC,
    SC_MOVE(804, 0, 298),
    SC_SYNC,
    SC_MOVE(804, 0, 326),
    SC_SYNC,
    SC_MOVE(776, 0, 326),
    SC_SYNC,
    SC_MOVE(776, 0, 358),
    SC_SYNC,
    SC_MOVE(804, 0, 358),
    SC_SYNC,
    SC_MOVE(804, 0, 392),
    SC_SYNC,
    SC_MOVE(786, 0, 392),
    SC_SYNC,
    SC_MOVE(762, 0, 416),
    SC_SYNC,
    SC_MOVE(762, 0, 438),
    SC_SYNC,
    SC_MOVE(804, 0, 438),
    SC_SYNC,
    SC_MOVE(804, 0, 472),
    SC_SYNC,
    SC_MOVE(794, 0, 472),
    SC_SYNC,
    SC_MOVE(792, 0, 504),
    SC_SYNC,
    SC_MOVE(808, 0, 504),
    SC_SYNC,
    SC_MOVE(808, 0, 552),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(854, 0, 552),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_LABEL(1),
    SC_ADD(SW_FACING, 1024),
    SC_LOOP(24, SC_TO(1)),
    SC_END,
};

/// A piece of debris flies for 60 frames and is released.
s32 gAltinPeak_DebrisScript[] = {
    SC_CALL(AltinPeak_FlyDebris),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

/// Garet, Ivan and Mia step out beside the leader after the Hydros battle.
s32 gAltinPeak_GaretScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(314, 0, 126),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_IvanScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(346, 0, 126),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_MiaScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(328, 0, 138),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

/// The guardian statue and its shadow flatten into the floor over 60 frames.
s32 gAltinPeak_StatueSinkScript[] = {
    SC_ADD(SW_SCALE_X, 128),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/// Garet, Ivan and Mia walk back to the leader and rejoin the party.
s32 gAltinPeak_RejoinScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(328, 0, 118),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gAltinPeak_EntrancesOther[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_FirstWaterBeastEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN, CONDITION_ALWAYS, 456, 0, 520, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {98, CONDITION_ALWAYS, 456, 0, 488, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 104, 0, 232, FACING_SOUTH, 0, 0, 56, 506, 530, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_RailTunnelEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 248, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SECOND_WATER_BEAST, CONDITION_ALWAYS, 56, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_SecondWaterBeastEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_FROM_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_FROM_CHEST_ROOM, CONDITION_ALWAYS, 664, 0, 380, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_FROM_LAKE, CONDITION_ALWAYS, 904, 0, 214, FACING_NORTH, 0, 842, 40, 1016, 248, 0},
    {98, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 320, 0, 472, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_SouthEntranceEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN, CONDITION_ALWAYS, 248, 0, 498, FACING_NORTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_FROM_FLOODED_CHAMBER, CONDITION_ALWAYS, 119, 0, 108, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, 567, 0, 105, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_FROM_TUNNEL, CONDITION_ALWAYS, 887, 0, 798, FACING_NORTH, 0, 712, 568, 960, 860, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_POOL, CONDITION_ALWAYS, 791, 0, 679, FACING_SOUTH, 0, 712, 560, 960, 860, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, 88, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, 216, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_ThirdWaterBeastEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_THIRD_WATER_BEAST_FROM_ALTIN_PEAK_POOL, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {98, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 747, 0, 258, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_PoolEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 568, 0, 348, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST, CONDITION_ALWAYS, 295, 0, 95, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {ALTIN_PEAK_POOL_SOUTHWEST_FROM_NORTH, CONDITION_ALWAYS, 442, 0, 774, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {ALTIN_PEAK_POOL_SOUTHWEST_FROM_SOUTHEAST, CONDITION_ALWAYS, 72, 0, 870, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {ALTIN_PEAK_POOL_SOUTHEAST_FROM_SOUTHWEST, CONDITION_ALWAYS, 918, 0, 712, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_THIRD_WATER_BEAST, CONDITION_ALWAYS, 615, 0, 684, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_SpritzEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 72, 0, 502, FACING_NORTH, 0, 0, 0, 820, 568, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, 152, 0, 174, FACING_SOUTH, 0, 0, 0, 820, 568, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN, CONDITION_ALWAYS, 264, 0, 890, FACING_NORTH, 0, 144, 720, 384, 908, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_JunctionEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 392, 0, 116, FACING_SOUTH, 0, 0, 0, 830, 564, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_NORTH, CONDITION_ALWAYS, 40, 0, 116, FACING_NORTH, 0, 0, 0, 830, 564, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_WEST_EXIT, CONDITION_ALWAYS, 520, 0, 616, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN, CONDITION_ALWAYS, 408, 0, 970, FACING_NORTH, 0, 244, 564, 1024, 1024, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_SOUTH, CONDITION_ALWAYS, 951, 0, 791, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_FallingRocksEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 361, 0, 309, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {ALTIN_PEAK_FALLING_ROCKS_WEST_FROM_EAST, CONDITION_ALWAYS, 89, 0, 749, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_SOUTH, CONDITION_ALWAYS, 728, 0, 717, FACING_NORTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_EAST_FROM_WEST, CONDITION_ALWAYS, 935, 0, 493, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_NORTH, CONDITION_ALWAYS, 791, 0, 566, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE, CONDITION_ALWAYS, 792, 16, 577, FACING_SOUTH, 0, 618, 0, 998, 854, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_HydrosStatueEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {98, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 328, 0, 118, FACING_NORTH, 0, 0, 0, 448, 524, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gAltinPeak_WestExitEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_WEST_EXIT_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 408, 0, 517, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM, CONDITION_ALWAYS, 198, 0, 336, FACING_SOUTH, 0, 0, 0, 540, 590, 0},
    {ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_FROM_TUNNEL, CONDITION_ALWAYS, 568, 0, 136, FACING_NORTH, 0, 480, 15, 684, 180, 0},
    {ALTIN_PEAK_WEST_EXIT_FROM_WORLD_MAP, CONDITION_ALWAYS, 73, 0, 190, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {SCENE_TABLE_END},
};

struct SceneExitRegion gAltinPeak_FallingRocksRegions[] = {
#if !TBS_PATCH_3
    {790, -106, 575, 794, -102, 579, CONDITION_ALWAYS, 6},
    {788, -108, 579, 796, -100, 587, CONDITION_ALWAYS, 6},
#else
    {784, -116, 572, 800, -100, 588, CONDITION_ALWAYS, 6},
#endif
    {SCENE_TABLE_END},
};

struct SceneExitRegion gAltinPeak_HydrosStatueRegions[] = {
#if !TBS_PATCH_3
    {84, 38, 420, 92, 46, 428, CONDITION_ALWAYS, 1},
#else
    {80, 34, 416, 96, 50, 432, CONDITION_ALWAYS, 1},
#endif
    {SCENE_TABLE_END},
};

u32 gAltinPeak_Exits[] = {
    SCENE_EXITS(AltinPeakFirstWaterBeast),
    SCENE_EXIT(ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_FIRST_WATER_BEAST),
    SCENE_EXITS(AltinPeakRailTunnel),
    SCENE_EXIT(ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_RAIL_TUNNEL),
    SCENE_EXIT(ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_FROM_ALTIN_PEAK_RAIL_TUNNEL),
    SCENE_EXITS(AltinPeakSecondWaterBeast),
    SCENE_EXIT(ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL, AltinPeakRailTunnel, ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SECOND_WATER_BEAST),
    SCENE_EXIT(ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_FROM_LAKE),
    SCENE_EXIT(ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_FROM_CHEST_ROOM),
    SCENE_EXITS(AltinPeakSouthEntrance),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_FROM_TUNNEL),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL, AltinPeakRailTunnel, ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_FROM_FLOODED_CHAMBER),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL, AltinPeakPool, ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST, AlpineCrossing, ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST, AlpineCrossing, ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST),
    SCENE_EXITS(AltinPeakThirdWaterBeast),
    SCENE_EXIT(ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL, AltinPeakPool, ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_THIRD_WATER_BEAST),
    SCENE_EXITS(AltinPeakPool),
    SCENE_EXIT(ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_POOL),
    SCENE_EXIT(ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHWEST_FROM_NORTH),
    SCENE_EXIT(ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH, AltinPeakPool, ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST),
    SCENE_EXIT(ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHEAST_FROM_SOUTHWEST),
    SCENE_EXIT(ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHWEST_FROM_SOUTHEAST),
    SCENE_EXIT(ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST, AltinPeakThirdWaterBeast, ALTIN_PEAK_THIRD_WATER_BEAST_FROM_ALTIN_PEAK_POOL),
    SCENE_EXITS(AltinPeakSpritz),
    SCENE_EXIT(ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_SPRITZ),
    SCENE_EXIT(ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_SPRITZ),
    SCENE_EXIT(ALTIN_PEAK_SPRITZ_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_SPRITZ),
    SCENE_EXITS(AltinPeakJunction),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz, ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_JUNCTION),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_NORTH),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_FROM_ALTIN_PEAK_JUNCTION),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_JUNCTION),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_SOUTH),
    SCENE_EXITS(AltinPeakFallingRocks),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz, ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_FALLING_ROCKS),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_EAST_FROM_WEST),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_SOUTH),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_WEST_FROM_EAST),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_NORTH),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_HYDROS_STATUE, AltinPeakHydrosStatue, ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS),
    SCENE_EXITS(AltinPeakHydrosStatue),
    SCENE_EXIT(ALTIN_PEAK_HYDROS_STATUE_TO_ALTIN_PEAK_FALLING_ROCKS, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE),
    SCENE_EXITS(AltinPeakWestExit),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_WEST_EXIT),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_FROM_TUNNEL),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTIN_PEAK_WEST_EXIT),
    SCENE_EXITS_END,
};

struct ActorPlacement gAltinPeak_ActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_FirstWaterBeastActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 216, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 336, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_SecondWaterBeastActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 312, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 464, FACING_WEST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 172, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_ThirdWaterBeastActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 296, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 216, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 759, 0, 285, FACING_WEST,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_PoolActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 184, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 664, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_SpritzActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 408, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 424, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 424, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 248, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SPRITZ), MOVEMENT_TYPE_STAND, 56, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_JunctionActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 636, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_FallingRocksActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 784, 0, 80, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK_LARGE, CONDITION_ALWAYS, 7, 784, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 774, 0, 76, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, 7, 774, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, 7, 792, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_HydrosStatueActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HYDROS_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 84, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 88, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAltinPeak_WestExitActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 328, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gAltinPeak_TriggersOther[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_FirstWaterBeastTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {TRIGGER_STEP, 10, FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN, EVENT(AltinPeak_ShowFirstBeastRoute)},
    {TRIGGER_STEP, 11, FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN, EVENT(AltinPeak_ShowSecondBeastRoute)},
    {TRIGGER_TALK, ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS, EVENT(AltinPeak_FightFirstWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(AltinPeak_FightFirstWaterBeast)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_FirstWaterBeastTriggersDrained[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN,
        EVENT(AltinPeak_FreezeFirstWaterBeastRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN, EVENT(AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_RailTunnelTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_SecondWaterBeastTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {TRIGGER_TALK, ACTOR_SECOND_WATER_BEAST, CONDITION_ALWAYS, EVENT(AltinPeak_FightSecondWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_SECOND_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(AltinPeak_FightSecondWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SECOND_WATER_BEAST_ROOM_BOULDER, CONDITION_ALWAYS, 0},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 83, CONDITION_ALWAYS, EVENT(AltinPeak_FlipSecondWaterBeastRoomSwitch)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {TRIGGER_TABLE_END},
    // Unused: two rows past the end marker, which no lookup reaches: a second
    // ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE exit and a chest on
    // trigger 100 holding a Herb under FLAG_ALTIN_PEAK_UNUSED_HERB, the spot
    // where the Cookie chest above already stands.
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_UNUSED_HERB, FIND_ITEM(ITEM_HERB)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_SouthEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_ThirdWaterBeastTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(AltinPeak_FreezeThirdWaterBeastRoomPuddle)},
    {TRIGGER_TALK, ACTOR_THIRD_WATER_BEAST, CONDITION_ALWAYS, EVENT(AltinPeak_FightThirdWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_THIRD_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(AltinPeak_FightThirdWaterBeast)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 50, CONDITION_ALWAYS, EVENT(AltinPeak_FlipThirdWaterBeastRoomSwitch)},
    // The Dragon Shield's chest opens only from its west side.
    {EXAMINE_CHEST_FACING(FACING_SOUTHEAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {EXAMINE_CHEST_FACING(FACING_EAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {EXAMINE_CHEST_FACING(FACING_NORTHEAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_PoolTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_POOL_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(AltinPeak_FreezePoolRoomPuddle)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 50, CONDITION_ALWAYS, EVENT(AltinPeak_FlipPoolRoomSwitch)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_POOL_MIMIC_BEATEN, FIND_MIMIC | 2},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_SpritzTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_ALTIN_PEAK_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL)},
    {TRIGGER_TALK, ACTOR_SPRITZ, CONDITION_ALWAYS, EVENT(AltinPeak_TalkToDjinni)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 50, CONDITION_ALWAYS, EVENT(AltinPeak_FlipSpritzRoomSwitch)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(AltinPeak_PlacePillarUnderLeader)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(AltinPeak_RestorePillarAutomaticPriority)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SPRITZ_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(AltinPeak_SettleSpritzRoomPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
        EVENT(AltinPeak_PushSpritzRoomPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPRITZ_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(AltinPeak_FreezeSpritzRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPRITZ_ROOM_SECOND_PUDDLE, FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN,
        EVENT(AltinPeak_FreezeSpritzRoomSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN,
        EVENT(AltinPeak_FreezeSpritzRoomThirdPuddle)},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gAltinPeak_JunctionTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_LUCKY_MEDAL,
     FIND_ITEM(ITEM_LUCKY_MEDAL)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_JUNCTION_ROOM_BOULDER,
     CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_FallingRocksTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 12, CONDITION_ALWAYS,
     EVENT(AltinPeak_ReadFallingRockSign)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_WALL,
     FLAG_ALTIN_PEAK_BOULDER_FALLEN, EVENT(AltinPeak_ForceLogOver)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_HydrosStatueTriggers[] = {
    {TRIGGER_STEP, 10, FLAG_HYDROS_STATUES_DEFEATED,
     EVENT(AltinPeak_StartHydrosBattle)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_LIFTING_GEM,
     FIND_ITEM(ITEM_LIFTING_GEM)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_WestExitTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_WEST_EXIT_ROOM_BOULDER,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_EXIT_ROOM_PILLAR,
     CONDITION_ALWAYS, 0},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_VIAL,
     FIND_ITEM(ITEM_VIAL)},
    {TRIGGER_TABLE_END},
};

/// Frames since the statue's last rumble.
s32 gAltinPeak_RumbleFrames = 0;

/// The cart's speed factor by its height against where it set off. It is 2 to
/// the power i / 16 for i from 0 to 32, in 16.16.
u32 gAltinPeak_RollRadii[] = {
    0x10000, 0x10b55, 0x1172b, 0x12387, 0x1306f, 0x13dea, 0x14bfd,
    0x15ab0, 0x16a09, 0x17a11, 0x18ace, 0x19c49, 0x1ae89, 0x1c199,
    0x1d581, 0x1ea4b, 0x20000, 0x216ab, 0x22e56, 0x2470f, 0x260df,
    0x27bd4, 0x297fb, 0x2b560, 0x2d413, 0x2f422, 0x3159c, 0x33892,
    0x35d13, 0x38333, 0x3ab03, 0x3d495, 0x40000,
};

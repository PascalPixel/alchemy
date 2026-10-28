/// @file
/// Altin Peak, the mine below Altin that its villagers and the Alpine
/// Crossing's signpost call Altin Mine: eleven scenes,
/// SceneId_AltinPeakFirstWaterBeast to SceneId_AltinPeakWestExit, whose area
/// name is Altin Peak, except that the international editions call
/// AltinPeakSouthEntrance the Alpine Crossing when the party comes in from
/// there (ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST and
/// ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST). Each scene is a room
/// of the mine, and the getters pick each room's tables by
/// gAdventure.sceneId, all the rooms share one exit table, and
/// Scene_Initialize calls the setup of the room being entered
/// (AltinPeak_RestoreFirstWaterBeastRoom and so on), which puts back what the
/// party has already done there.
///
/// Altin's doorways lead down into AltinPeakFirstWaterBeast,
/// AltinPeakSouthEntrance, AltinPeakSpritz and AltinPeakJunction. The mine is
/// flooded, and a water beast (SPRITE_WATER_BEAST, drawn with its green and
/// blue swapped, SPRITE_COLOR_RBG) waits in AltinPeakFirstWaterBeast,
/// AltinPeakSecondWaterBeast and AltinPeakThirdWaterBeast. In
/// AltinPeakFirstWaterBeast the first one shows the way: stepping near it
/// (step trigger 10, AltinPeak_ShowFirstBeastRoute) it cries out, hops around
/// the room, freezes the second puddle with Frost and hops across, and stepping
/// near it again (step trigger 11) it hops back and settles down to spit.
/// Talking to a beast, or reading its mind, starts its battle, and the party
/// comes back to the room by ENTRANCE_AFTER_BATTLE, where the room's setup
/// runs a drain: the water layer sinks and flickers away
/// (AltinPeak_DrainFirstWater and the two after it), and
/// FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN remember the three drains, which
/// Altin's code reads to lower the water in the village too. Back from the
/// first battle, in AltinPeakFirstWaterBeast, the party finds the Frost Jewel
/// before the water drains, and from then on Frost freezes the mine's puddles
/// into ice pillars to stand on (scene temp flags keep them frozen only within
/// one visit). AltinPeakSouthEntrance's water stands until the second drain and
/// AltinPeakPool's until the third.
///
/// The rooms between are built from a few mechanisms: mine carts the party
/// rides (talking to one, AltinPeak_RollCart), which roll along the turns their
/// route's cells mark; floor switches in AltinPeakSecondWaterBeast,
/// AltinPeakThirdWaterBeast, AltinPeakPool and AltinPeakSpritz, each remembered
/// by an area temp flag (FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH
/// and the three after it) and redrawn on entry; and a stone pillar in
/// AltinPeakSpritz that Move or a push slides along. Spritz, a Mercury Djinni,
/// waits there too, placed until it has joined, and talking to it starts its
/// battle. In AltinPeakFallingRocks a sign warns `Watch for falling rocks! Do
/// not strike the wall! Rocks may fall!` at what looks like a dead end: while
/// the party has no Orb of Force, Garet steps out and strikes the wall anyway,
/// and with it Force does the same (AltinPeak_ForceLogOver); either way a
/// boulder rolls after the leader down the path and crashes into the wall at
/// the bottom, leaving rubble (FLAG_ALTIN_PEAK_BOULDER_FALLEN).
///
/// AltinPeakHydrosStatue, at the bottom, is a tiled room of carved stone, part
/// of ruins buried in the mine. Stepping toward its guardian statue
/// (AltinPeak_StartHydrosBattle) wakes it and starts the battle with the
/// Hydros statues. Back from it (FLAG_HYDROS_STATUES_DEFEATED, which Altin's
/// villagers read), Garet, Ivan and Mia step out and talk, the statue sinks
/// into the floor, and a chest behind it holds the Lifting Gem. Its Lift frees
/// Hsu at
/// the Alpine Crossing (alpine_crossing_lama_temple.c), and in the mine the
/// boulders of AltinPeakSecondWaterBeast, AltinPeakJunction and
/// AltinPeakWestExit have Lift rows that run nothing of their own.
/// AltinPeakSouthEntrance opens onto the Alpine Crossing
/// (ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST and
/// ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST) and AltinPeakWestExit
/// onto the World Map (ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP).
///
/// The falling boulder is SPRITE_ROCK, its shadow a SPRITE_BLANK_LARGE
/// actor, and its pieces in the crash SPRITE_ROCK again, scaled down
/// (AltinPeak_SpawnDebris); the beasts' spray is SPRITE_DUST_PUFF recoloured,
/// as in Altin.

#include "script_operands.h"
#include "version.h"
#include "macros.h"
#include "adventure.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/altin_peak.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "heap_slots.h"
#include "types.h"
#include "animspr.h"
#include "event_layer.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "call.h"
#include "field_effect.h"
#include "iwram_call.h"
#include "dma.h"

#include "types.h"

#include "event_layer.h"
#include "scene_tables.h"
#include "djinn_ids.h"

#include "speaker.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "actor_script.h"
#include "irq.h"
#include "fixed_math.h"
#include "scene.h"
#include "map_grid_scroll.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// The rooms' floor switches, each set once flipped from its first position;
// they are area temp flags, kept until the party leaves the mine.

/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH FLAG_AREA_TEMP_23
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH FLAG_AREA_TEMP_25
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH FLAG_AREA_TEMP_26
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
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
static void AltinPeak_StartSpitting(s32 x, s32 y, s32 z, s32 facing);
static void AltinPeak_SpawnDebris(void);
static void AltinPeak_SparkleIce(struct Actor *actor);
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

/// The rooms' actors, from their actor templates.
/// AltinPeakSecondWaterBeast, AltinPeakThirdWaterBeast, AltinPeakPool and
/// AltinPeakSpritz place the mine cart the party rides first.
enum {
  /// ★ Named from what the player sees: a wooden mine cart on the rails (its
  /// render)
  ACTOR_CART = ACTOR_FIRST_PLACED
};

/// AltinPeakFirstWaterBeast: a puddle that appears with the first drain, a
/// second puddle and the water beast.
enum {
  /// ★ Named from what the player sees: a small blue puddle on the floor west
  /// of the beast's ledge, once the water has drained (its render)
  ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a small blue puddle on the ledge north
  /// of the beast (its render)
  ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
  /// Its battle names it a Living Statue.
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  ACTOR_FIRST_WATER_BEAST
};

enum {
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  ACTOR_SECOND_WATER_BEAST = ACTOR_CART + 1,
  /// ★ Named from what the player sees: a round brown boulder by the chest in
  /// the room's east end (its render)
  ACTOR_SECOND_WATER_BEAST_ROOM_BOULDER
};

enum {
  /// ★ Named from what the player sees: a small blue puddle on the floor (its
  /// render)
  ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE = ACTOR_CART + 1,
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  ACTOR_THIRD_WATER_BEAST
};

enum {
  /// ★ Named from what the player sees: a small blue puddle on the floor (its
  /// render)
  ACTOR_POOL_ROOM_PUDDLE = ACTOR_CART + 1
};

enum {
  /// ★ Named from what the player sees: a small blue puddle beside the stone
  /// pillar (its render)
  ACTOR_SPRITZ_ROOM_PUDDLE = ACTOR_CART + 1,
  /// ★ Named from what the player sees: a stone pillar standing in the room's
  /// south (its render)
  ACTOR_SPRITZ_ROOM_PILLAR,
  /// ★ Named from what the player sees: a small blue puddle in the room's
  /// north (its render)
  ACTOR_SPRITZ_ROOM_SECOND_PUDDLE,
  /// ★ Named from what the player sees: a small blue puddle by the rails (its
  /// render)
  ACTOR_SPRITZ_ROOM_THIRD_PUDDLE,
  /// Unused: a round boulder at (520, 632), outside both of the room's camera
  /// boxes (gAltinPeak_SpritzEntrances': one ends at z 568, the one below it at
  /// x 384), so the player never sees it, and no trigger names it. It stands
  /// almost where AltinPeakJunction places its own boulder, (536, 636).
  /// ★ Named from the game: "Mother, look!! A boulder blocking the road!!"
  /// (MsgGomaNoUsePsynergy)
  ACTOR_UNUSED_SPRITZ_ROOM_BOULDER,
  /// The Mercury Djinni, placed until it has joined.
  /// ★ Named from the game: "Spritz" (MsgDjinnName)
  ACTOR_SPRITZ = 14
};

enum {
  /// ★ Named from what the player sees: a round brown boulder (its render)
  ACTOR_JUNCTION_ROOM_BOULDER = ACTOR_FIRST_PLACED
};

/// AltinPeakFallingRocks: the boulder that falls and its shadow, the log that
/// topples as it comes down, the actor Force is cast on, and the sign.
enum {
  /// ★ Named from what the player sees: its sprite, a grey boulder (its
  /// render)
  ACTOR_FALLING_BOULDER = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: its sprite, a dark shadow (its render)
  ACTOR_BOULDER_SHADOW,
  /// SPRITE_LOG_NORTH_SOUTH.
  /// ★ Named from what the player sees: a log standing on end beside the sign
  /// at the dead end, lying on its side after the rockfall (its render)
  ACTOR_STANDING_LOG,
  /// SPRITE_BLANK, at the dead end's wall.
  /// ★ Named from the game: "Do not strike the wall!"
  /// (MsgAltinWatchFallingRocks)
  ACTOR_WALL,
  /// The falling-rock sign, drawn with the sprite of Lalivero's sign and the
  /// Kolima Forest floodgate's (SPRITE_SIGNPOST); it is read by searching
  /// metatile behavior 12 (AltinPeak_ReadFallingRockSign).
  /// ★ Named from the game: "Watch for falling rocks!"
  /// (MsgAltinWatchFallingRocks)
  ACTOR_FALLING_ROCK_SIGN
};

/// AltinPeakHydrosStatue: the guardian statue and its shadow.
enum {
  /// ★ Named from the game: "Could the guardian statues have turned into the
  /// monsters?" (MsgAltinTheWaterBeastsLookedExactlyLike)
  ACTOR_STATUE = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: its sprite, a dark shadow (its render)
  ACTOR_STATUE_SHADOW
};

enum {
  /// ★ Named from what the player sees: a round brown boulder by the mine's
  /// west exit (its render)
  ACTOR_WEST_EXIT_ROOM_BOULDER = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a stone pillar south of the boulder
  /// (its render)
  ACTOR_WEST_EXIT_ROOM_PILLAR
};

/// The rooms' frozen puddles and Garet at the sign: scene temp flags, kept
/// within one visit to a room, across its battles.
enum {
  /// Frost has frozen the room's first puddle.
  /// ★ Named from what the player sees: a small puddle of water on the mine's
  /// floor (its render)
  FLAG_ALTIN_PEAK_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  /// Frost has frozen AltinPeakFirstWaterBeast's second puddle or
  /// AltinPeakSpritz's second.
  /// ★ Named from what the player sees: a small puddle of water on the mine's
  /// floor (its render)
  FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
  /// Frost has frozen AltinPeakSpritz's third puddle.
  /// ★ Named from what the player sees: a small puddle of water on the mine's
  /// floor (its render)
  FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
  /// Garet has come out to look at AltinPeakFallingRocks's falling-rock sign.
  /// ★ Named from the game: "Watch for falling rocks!"
  /// (MsgAltinWatchFallingRocks)
  FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS = FLAG_SCENE_TEMP_05
};

/// A chest searched while facing the given way.
#define EXAMINE_CHEST_FACING(facing)                                           \
  (EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | (facing))

/// SPRITE_PUDDLE's animation once Frost has frozen it into an ice pillar.
enum { PUDDLE_ANIMATION_FROZEN = 5 };

// The spray's direction (FACING_EAST or FACING_WEST) and origin, and the
// water's flicker timer.
extern s32 gAltinPeak_SprayDirection;
extern s32 gAltinPeak_SprayOrigin[3];
extern u16 gAltinPeak_FlickerTimer;

/// Talking to the Djinni in AltinPeakSpritz starts the battle with Spritz, a
/// Mercury Djinni.
static void AltinPeak_TalkToDjinni(void) {
  Field_RequestDjinnBattle(ACTOR_SPRITZ, DJINNI_SPRITZ);
}

// Shared source, included where its bytes sit in the ROM.
#include "shared/actor_facing.h"

// Update routine of AltinPeakFallingRocks's boulder shadow: it stays under the
// boulder, on the ground 12 pixels down, while the boulder bounces. UB: update
// routines return void. Plain void removes the native zero result and changes
// its return (three instructions in tbs-en).
#ifdef UBFIX
static void AltinPeak_ShadowFollowBoulder(struct Actor *updated) {
#else
static s32 AltinPeak_ShadowFollowBoulder(struct Actor *updated) {
#endif
  struct Actor *follower = (struct Actor *)updated;
  struct Actor *actor = Actor_GetById(ACTOR_FALLING_BOULDER);

  follower->x.fixed = actor->x.fixed;
  follower->y.fixed = -PIXELS(12);
  follower->z.fixed = actor->z.fixed;
#ifndef UBFIX
  return 0;
#endif
}

/// Actor script routine for a piece of flying rock debris: it moves by its
/// velocity, grows by its z velocity and falls under its gravity.
static s32 AltinPeak_FlyDebris(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
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
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_AltinPeakFirstWaterBeast) {
    return gAltinPeak_FirstWaterBeastEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakRailTunnel) {
    return gAltinPeak_RailTunnelEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakSecondWaterBeast) {
    return gAltinPeak_SecondWaterBeastEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakSouthEntrance) {
    return gAltinPeak_SouthEntranceEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakThirdWaterBeast) {
    return gAltinPeak_ThirdWaterBeastEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakPool) {
    return gAltinPeak_PoolEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakSpritz) {
    return gAltinPeak_SpritzEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakJunction) {
    return gAltinPeak_JunctionEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakFallingRocks) {
    return gAltinPeak_FallingRocksEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakHydrosStatue) {
    return gAltinPeak_HydrosStatueEntrances;
  }
  if (sceneId == (s32)&SceneId_AltinPeakWestExit) {
    return gAltinPeak_WestExitEntrances;
  }
  return gAltinPeak_EntrancesOther;
}

/// AltinPeakFallingRocks and AltinPeakHydrosStatue have regions.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_AltinPeakFallingRocks) {
    return gAltinPeak_FallingRocksRegions;
  }
  if (sceneId == (s32)&SceneId_AltinPeakHydrosStatue) {
    return gAltinPeak_HydrosStatueRegions;
  }
  return 0;
}

/// All the rooms share one exit table.
const u32 *Scene_GetExits(void) { return gAltinPeak_Exits; }

const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_AltinPeakFirstWaterBeast) {
    return gAltinPeak_FirstWaterBeastActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakSecondWaterBeast) {
    return gAltinPeak_SecondWaterBeastActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakThirdWaterBeast) {
    return gAltinPeak_ThirdWaterBeastActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakPool) {
    return gAltinPeak_PoolActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakSpritz) {
    return gAltinPeak_SpritzActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakJunction) {
    return gAltinPeak_JunctionActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakFallingRocks) {
    return gAltinPeak_FallingRocksActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakHydrosStatue) {
    return gAltinPeak_HydrosStatueActorTemplates;
  }
  if (sceneId == (s32)&SceneId_AltinPeakWestExit) {
    return gAltinPeak_WestExitActorTemplates;
  }
  return gAltinPeak_ActorTemplatesOther;
}

/// Talking to a mine cart: the party leader jumps into it, and once the leader
/// has landed in it the cart rolls away along its rails with the leader riding
/// it.
static void AltinPeak_RideCart(s32 log) {
  struct Actor *logActor;
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  logActor = Actor_GetById(ACTOR_CART);
  Field_EndBusyAndReveal();
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Event_WaitFrames(10);
  Sound_PlaySong(SE_JUMP);
  Actor_SetSpeedParameters(ACTOR_PARTY_LEADER, PIXELS(16) / 5, PIXELS(8) / 5);
  // Leap by the height between leader and cart, in quarter pixels, plus 4.
  leader->velocityY =
      ((ABS(logActor->y.fixed - leader->y.fixed) >> 14) << 14) + PIXELS(4);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  FieldActor_SetMoveTarget(leader, logActor->x.fixed, logActor->y.fixed,
                           logActor->z.fixed);
  WaitFrames(10);
  leader->sprite->image.priority = 3;
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);

  // Wait until the leader has come down into the cart.
  for (;;) {
    if (!((logActor->y.fixed >> 14) < (leader->y.fixed >> 14)))
      break;
    WaitFrames(1);
  }
  Event_End();
  Sound_PlaySong(SE_DOOR_CLOSE);
  AltinPeak_RollCart(log, 0);
  WaitFrames(20);
  Scene_PlayQueuedMusic();
}

/// The party fights the first water beast. Talking to (or reading the mind of)
/// a room's water beast starts its battle; afterwards the party returns to the
/// room's entrance after battle.
static void AltinPeak_FightFirstWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_AltinPeakFirstWaterBeast,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// The party fights the second water beast. It starts and ends as the first
/// one's does, from its own room.
static void AltinPeak_FightSecondWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_AltinPeakSecondWaterBeast,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// The party fights the third water beast. It starts and ends as the first
/// one's does, from its own room.
static void AltinPeak_FightThirdWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_AltinPeakThirdWaterBeast,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// Move on AltinPeakSpritz's pillar: the cell it left becomes free again and
/// the cell it now stands on (column 24 or 25 of row 26) takes the blocking
/// attributes kept at (0, 0).
static void AltinPeak_SettleSpritzRoomPillar(void) {
  struct Actor *pillar = Actor_GetById(ACTOR_SPRITZ_ROOM_PILLAR);

  if (pillar != 0) {
    s32 column;

    MapGrid_CopyCellAttributes(24, 27, 2, 1, 24, 26);
    column = pillar->x.fixed >> 20;
    if (column == 25) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, column, 26);
    } else {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 26);
    }
    Actor_SetSpriteFlags(pillar, 0);
    pillar->motionFlags = 0;
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
  }
}

/// Frost on a puddle freezes it into an ice pillar the party can stand on:
/// the puddle's cell takes the pillar's attributes, kept elsewhere on the
/// map, and the pillar is drawn 32 pixels lower, under the party. Each
/// room's puddle has its own copy, setting its own scene temp flag.
static void AltinPeak_FreezeFirstWaterBeastRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(7, 13, 1, 1, 9, 13);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakFirstWaterBeast's second puddle, three cells across: the
/// cells take the ice's attributes and the ice is left to fall to the ground.
static void AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(29, 1, 3, 1, 17, 13);
  if (puddle != 0) {
    puddle->motionFlags = ACTOR_FALLS;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's first puddle.
static void AltinPeak_FreezeSpritzRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_SPRITZ_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 26, 26);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakThirdWaterBeast's puddle.
static void AltinPeak_FreezeThirdWaterBeastRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(23, 13, 1, 1, 25, 13);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakPool's puddle.
static void AltinPeak_FreezePoolRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_POOL_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(45, 41, 1, 1, 43, 41);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's second puddle.
static void AltinPeak_FreezeSpritzRoomSecondPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 17, 10);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's third puddle.
static void AltinPeak_FreezeSpritzRoomThirdPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_GetById(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 26, 15);
  if (puddle != 0) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN);
}

/// Interrupt handler while the water layer vanishes: each time, background 1
/// (the water) stays on only when a random percentage reaches the timer, so
/// as the timer climbs to 100 the water flickers away.
static void Water_FlickerOut(void) {
  volatile u16 *reg = &REG_DISPCNT;
  s16 v = *reg & 0xfdff;

  if ((u32)(Import_RandomNext() * 100) >> 16 >= gAltinPeak_FlickerTimer) {
    s32 k = 0x200;

    v |= k;
  }
  {
    u32 t = (u16)v;

    *reg = t;
  }
}

/// The first drain, after the Frost Jewel is found: the screen shakes, the
/// water layer sinks 24 pixels, one step every 4 frames (a puddle growing
/// from a tenth of its size partway down), then flickers away over 100
/// frames. The way on opens and the drain is remembered.
static void AltinPeak_DrainFirstWater(void) {
  s32 step;
  struct Actor *puddle;
  u8 *gridWork;
  u8 *waterLayer;
  s32 timer;

  gridWork = gMapGridWork[0];
  Event_PlaySong(SE_WATER_DRAIN);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_WaitFrames(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = gridWork + 356;
  for (step = 0; step <= 23; step++) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    WaitFrames(4);
    if (step == 8) {
      puddle = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
      puddle->scaleX = Q16_ONE / 10;
      puddle = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
      puddle->scaleY = Q16_ONE / 10;
      Actor_SetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, PIXELS(152),
                        PIXELS(216));
      Actor_SetScript(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE,
                      gAltinPeak_PuddleGrowScript);
    }
  }
  // Flicker the water away while the timer counts to 100.
  Irq_SetHandler(1, 0, Water_FlickerOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    WaitFrames(1);
    timer = gAltinPeak_FlickerTimer + 1;
    gAltinPeak_FlickerTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  WaitFrames(1);
  Irq_SetHandler(1, 0, 0);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_WaitFrames(30);
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 3, 14);
  GameFlag_Set(FLAG_ALTIN_MINE_FIRST_DRAIN);
}

/// The second drain: the same sinking and flicker over the cells the water
/// leaves behind, which then take their dry form.
static void AltinPeak_DrainSecondWater(void) {
  s32 step;
  u8 *gridWork;
  u8 *waterLayer;
  s32 timer;

  gridWork = gMapGridWork[0];
  MapGrid_CopyCells(93, 41, 16, 4, 77, 28);
  Event_PlaySong(SE_WATER_DRAIN);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_WaitFrames(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = gridWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    WaitFrames(4);
  }
  // Flicker the water away while the timer counts to 100.
  Irq_SetHandler(1, 0, Water_FlickerOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    WaitFrames(1);
    timer = gAltinPeak_FlickerTimer + 1;
    gAltinPeak_FlickerTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  WaitFrames(1);
  Irq_SetHandler(1, 0, 0);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_WaitFrames(30);
  MapGrid_CopyCells(77, 41, 16, 4, 77, 28);
  GameFlag_Set(FLAG_ALTIN_MINE_SECOND_DRAIN);
}

/// The third drain, likewise.
static void AltinPeak_DrainThirdWater(void) {
  s32 step;
  u8 *gridWork;
  u8 *waterLayer;
  s32 timer;

  gridWork = gMapGridWork[0];
  MapGrid_CopyCellsTo(113, 31, 103, 17, 1, 1);
  MapGrid_CopyCellsTo(111, 32, 104, 18, 3, 2);
  MapGrid_CopyCellsTo(64, 32, 103, 18, 1, 2);
  Event_PlaySong(SE_WATER_DRAIN);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_WaitFrames(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = gridWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    WaitFrames(4);
  }
  // Flicker the water away while the timer counts to 100.
  Irq_SetHandler(1, 0, Water_FlickerOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    WaitFrames(1);
    timer = gAltinPeak_FlickerTimer + 1;
    gAltinPeak_FlickerTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  WaitFrames(1);
  Irq_SetHandler(1, 0, 0);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_WaitFrames(30);
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
  Sound_PlaySong(SE_CHEST_CLOSE);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  WaitFrames(20);
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
  Sound_PlaySong(SE_CHEST_CLOSE);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  WaitFrames(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Searching AltinPeakPool's switch flips it.
static void AltinPeak_FlipPoolRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Event_PlaySong(SE_CHEST_CLOSE);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(47, 29, 16, 28, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(46, 29, 16, 28, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
  }
  WaitFrames(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Searching AltinPeakSpritz's switch flips it.
static void AltinPeak_FlipSpritzRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Event_PlaySong(SE_CHEST_CLOSE);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(47, 28, 29, 17, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(46, 28, 29, 17, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
  }
  WaitFrames(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Stepping onto AltinPeakSpritz's pillar: it is drawn under the party leader.
static void AltinPeak_PlacePillarUnderLeader(void) {
  Actor_GetById(ACTOR_SPRITZ_ROOM_PILLAR)->priorityFlags =
      ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
}

/// Stepping off AltinPeakSpritz's pillar: it sorts against the leader again.
static void AltinPeak_RestorePillarAutomaticPriority(void) {
  Actor_GetById(ACTOR_SPRITZ_ROOM_PILLAR)->priorityFlags =
      ACTOR_PRIORITY_AUTOMATIC;
}

/// Walking east into AltinPeakSpritz's pillar pushes it one cell along with a
/// scrape; the leader follows it and its cells are settled as after Move.
static void AltinPeak_PushSpritzRoomPillar(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(24, 27, 2, 1, 24, 26);
  Event_PlaySong(SE_PUSH);
  Actor_SetSpeed(ACTOR_SPRITZ_ROOM_PILLAR, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  Actor_GetById(ACTOR_SPRITZ_ROOM_PILLAR)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 400, 424);
  Actor_SetDestination(ACTOR_SPRITZ_ROOM_PILLAR, 408, 424);
  Actor_WaitForMoveById(ACTOR_SPRITZ_ROOM_PILLAR);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  AltinPeak_SettleSpritzRoomPillar();
  Event_End();
}

/// Force on AltinPeakFallingRocks's wall: it rumbles, the log by it rocks one
/// way and back, then topples with a crash, and the rockfall follows.
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
  Event_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  Event_WaitFrames(10);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_WaitFrames(20);

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
    WaitFrames(1);
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
    WaitFrames(1);
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
    WaitFrames(1);
  }
  WaitFrames(1);
  sprite->rotation = 0xc000;
  Event_PlaySong(SE_STONE_RUMBLE);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Event_WaitFrames(20);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  AltinPeak_RunBoulderChase(5);
  Event_End();
}

/// AltinPeakFallingRocks's rockfall. The party leader steps back and looks up
/// as the slope rumbles; the log lies toppled and a boulder drops onto the
/// path. The leader flees down the winding path, and after boulderDelay frames
/// the boulder rolls after; it bounces to a stop at the bottom and crashes into
/// the wall, throwing up rock debris and leaving rubble. Garet, if he came out
/// at the sign, rejoins the leader. Remembered by
/// FLAG_ALTIN_PEAK_BOULDER_FALLEN.
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
  Event_WaitFrames(40);
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(784), -1, PIXELS(116), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 792, 140);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 100);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Event_PlaySong(SE_STONE_RUMBLE);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Event_WaitFrames(20);
  // Stage the rolling actor and its follower for the escape route.
  shadow->scaleX = 6 * Q16_ONE / 5;
  shadow->scaleY = 6 * Q16_ONE / 5;
  shadow->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  shadow->update = AltinPeak_ShadowFollowBoulder;
#else
  shadow->update = (void (*)(struct Actor *))AltinPeak_ShadowFollowBoulder;
#endif
  Import_ActorSetAnimation(ACTOR_FALLING_BOULDER, 4);
  boulder->bounce = PIXELS(1) / 2;
  boulder->x.fixed = PIXELS(786);
  boulder->y.fixed = PIXELS(32);
  boulder->z.fixed = PIXELS(90);
  boulder->scaleX = 2 * Q16_ONE;
  boulder->scaleY = 2 * Q16_ONE;
  Event_WaitFrames(10);
  Event_PlaySong(SE_STONE_RUMBLE);
  MapGrid_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
  Event_WaitFrames(20);
  rock->x.fixed += PIXELS(14);
  rock->y.fixed += -PIXELS(8);
  rock->sprite->rotation = 0xc000;
  Event_PlaySong(SE_TREMOR);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Event_PlaySong(MUS_BOULDER_CHASE);
  MapGrid_SetShake(PIXELS(1), PIXELS(3), Q16_ONE);
  Actor_SetSpritePriority(ACTOR_FALLING_BOULDER, 0);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // Send the leader first, then release the rolling actor after its delay.
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  leader->timer = 0;
  Actor_SetScript(ACTOR_PARTY_LEADER, gAltinPeak_LeaderFleePath);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    Actor_SetPosition(ACTOR_GARET, PIXELS(878), PIXELS(528));
    actor = Actor_Get(ACTOR_GARET);
    actor->facing = FACING_SOUTH + FACING_STEP;
  }
  FieldCamera_SetSpeed(PIXELS(5) / 4, PIXELS(5) / 32);
  FieldCamera_MoveTo(PIXELS(786), -1, PIXELS(556), 1);
  Event_WaitFrames(boulderDelay);
  Actor_SetSpritePriority(ACTOR_FALLING_BOULDER, 1);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, PIXELS(317) / 200, PIXELS(275) / 347);
  boulder->timer = 0;
  Actor_SetScript(ACTOR_FALLING_BOULDER, gAltinPeak_BoulderPath);
  do {
    WaitFrames(1);
  } while (leader->timer == 0);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0);
  do {
    WaitFrames(1);
  } while (boulder->timer == 0);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  shadow->x.fixed = PIXELS(786);
  shadow->update = NULL;
  shadow->z.fixed = PIXELS(618);
  shadow->y.fixed = -PIXELS(64);
  // Let the rolling actor bounce before the final collapse.
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, PIXELS(8) / 5, PIXELS(4) / 5);
  boulder->bounce = PIXELS(1) / 10;
  boulder->gravity = PIXELS(1) / 5;
  boulder->velocityY = PIXELS(4);
  Actor_MoveToAndWait(ACTOR_FALLING_BOULDER, 786, 604);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, PIXELS(16) / 5, PIXELS(8) / 5);
  Actor_SetDestination(ACTOR_FALLING_BOULDER, 786, 644);
  Event_WaitFrames(15);
  MapGrid_SetShake(PIXELS(5), PIXELS(7), Q16_ONE);
  MapGrid_CopyCellsTo(25, 36, 43, 36, 11, 9);
  MapGrid_CopyCellAttributes(25, 35, 10, 5, 43, 35);
  Actor_SetPosition(ACTOR_FALLING_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  // Hide the rolling actors and animate the falling debris.
  Task_AddFrame(AltinPeak_SpawnDebris, TASK_PASS_FRAME);
  Event_WaitFrames(80);
  Task_Remove(AltinPeak_SpawnDebris);
  Event_WaitFrames(60);
  Event_PlaySong(SONG_FADE_OUT_BGM);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // Return Garet to the party if the warning sign brought him into the event.
  Event_WaitFrames(120);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    Actor_SetSpeed(ACTOR_GARET, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkTo(ACTOR_GARET, 824, 558);
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 854, 584);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldCamera_MoveTo(PIXELS(788), -PIXELS(64), PIXELS(610), 1);
  FieldCamera_WaitForArrival();
  Event_PlaySong(SE_BOSS_ROAR);
  Event_WaitFrames(240);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
    FieldCamera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
    FieldCamera_MoveTo(PIXELS(854), 0, PIXELS(584), 1);
    FieldCamera_WaitForArrival();
    Actor_WalkToAndWait(ACTOR_GARET, 840, 552);
    Actor_WalkToAndWait(ACTOR_GARET, 854, 562);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
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
/// for falling rocks! Do not strike the wall! Rocks may fall!` Before the
/// rockfall, while the party has no Orb of Force, Garet steps out, grumbles at
/// the dead end and strikes the wall; the ground shakes, he tells the leader to
/// run and runs down the path, and the rockfall follows.
static void AltinPeak_ReadFallingRockSign(void) {
  struct Actor *actor;
  s32 message;

  Event_Start();
  message = MsgAltinWatchFallingRocks;
  FieldMessage_Show(message, 1);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_BOULDER_FALLEN) == 0 &&
      GameFlag_Get(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) == 0) {
    // Bring Garet out to inspect the warning and demonstrate the danger.
    GameFlag_Set(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 790, 140);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 780, 140);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetSpeed(ACTOR_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(ACTOR_GARET, 800, 140);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
    Event_SetMessageId(message + 1);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
    Actor_Jump(ACTOR_GARET, 6, 0);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(8) / 5, PIXELS(4) / 5);
    Actor_GetById(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_GARET, 792, 110);
    Event_WaitFrames(1);
    Actor_GetById(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
    Event_PlaySong(SE_RATTLE);
    MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
    Actor_GetById(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_GARET, 792, 120);
    Event_WaitFrames(1);
    {
      struct Actor *actor = Actor_Get(ACTOR_GARET);

      actor->steeringFlags = ACTOR_FACES_MOTION | actor->steeringFlags;
    }
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Event_WaitFrames(80);
    Event_PlaySong(SE_RUMBLE);
    MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    Event_WaitFrames(40);
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
    FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
    Actor_SetSpeed(ACTOR_GARET, PIXELS(5) / 2, PIXELS(5) / 4);
    Import_ActorSetAnimation(ACTOR_GARET, 5);
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
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Event_WaitFrames(40);
    AltinPeak_RunBoulderChase(10);
  }
  Event_End();
}

/// AltinPeakFirstWaterBeast has its own triggers once the first drain has
/// lowered its water.
const struct Trigger *Scene_GetTriggers(void) {
  const struct Trigger *table;

  if (gAdventure.sceneId == (s32)&SceneId_AltinPeakFirstWaterBeast) {
    if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
      table = gAltinPeak_FirstWaterBeastTriggersDrained;
    } else {
      table = gAltinPeak_FirstWaterBeastTriggers;
    }
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakRailTunnel) {
    table = gAltinPeak_RailTunnelTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakSecondWaterBeast) {
    table = gAltinPeak_SecondWaterBeastTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakSouthEntrance) {
    table = gAltinPeak_SouthEntranceTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakThirdWaterBeast) {
    table = gAltinPeak_ThirdWaterBeastTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakPool) {
    table = gAltinPeak_PoolTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakSpritz) {
    table = gAltinPeak_SpritzTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakJunction) {
    table = gAltinPeak_JunctionTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakFallingRocks) {
    table = gAltinPeak_FallingRocksTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakHydrosStatue) {
    table = gAltinPeak_HydrosStatueTriggers;
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakWestExit) {
    table = gAltinPeak_WestExitTriggers;
  } else {
    table = gAltinPeak_TriggersOther;
  }
  return table;
}

/// Update routine of the hopping water beast: once a second it leaps up and
/// lands with a splash (SE_SPLASH) that shakes the screen briefly.
static void AltinPeak_UpdateBeastHops(struct Actor *updated) {
  struct Actor *beast = updated;
  s16 *shakeDelay = (s16 *)&beast->userValue;

  // The route owns timer (hop phase), work (shake delay) and the effect's
  // countdown (landing-sound latch) until it removes this update routine.
  if (*shakeDelay != 0) {
    if (--*shakeDelay == 1)
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  }
  if (beast->velocityY == 0) {
    Actor_SelectAnimation(updated, 1);
    beast->y.fixed -= PIXELS(3) / 2;
    if (beast->y.fixed < beast->groundHeight) {
      if (((struct FieldEffect *)updated)->countdown != 0) {
        Sound_PlaySong(SE_SPLASH);
        ((struct FieldEffect *)updated)->countdown = 0;
        beast->userValue = 4;
        MapGrid_SetShake(PIXELS(1), 0, Q16_ONE);
      }
      beast->y.fixed = beast->groundHeight;
    }
    beast->paused = 1;
  } else {
    beast->paused = 0;
  }
  if (beast->timer == 0) {
    Sound_PlaySong(SE_JUMP);
    ((struct FieldEffect *)updated)->countdown = 1;
    Actor_SelectAnimation(updated, 2);
    beast->velocityY = PIXELS(3);
  }
  if (++beast->timer == 60)
    beast->timer = 0;
}

/// The beast lands with a splash, shaking the screen for holdFrames.
static __inline__ void AltinPeak_PlayLandingShake(s32 amplitude,
                                                  s32 holdFrames) {
  Sound_PlaySong(SE_SPLASH);
  MapGrid_SetShake(amplitude, 0, Q16_ONE);
  Event_WaitFrames(holdFrames);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
}

/// Stepping near AltinPeakFirstWaterBeast's water beast the first time: the
/// camera finds it, it cries out and hops away around the room, freezes the
/// second puddle with Frost and hops across it, then stops to watch the party
/// leader.
static void AltinPeak_ShowFirstBeastRoute(void) {
  struct Actor *beast;
  struct Actor *leader;
  struct Actor *actor;
  s32 jumpSpeed;

  beast = (struct Actor *)Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  Event_Start();
  FieldCamera_SetSpeedParameters(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(298), -1, PIXELS(337), 1);
  FieldCamera_WaitForArrival();
  Sound_PlaySong(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_WaitFrames(40);
  FieldCamera_SetSpeedParameters(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveWithinMapBounds(PIXELS(295), PIXELS(32), PIXELS(212), 1);
  // Run the timed hopping update routine while the beast moves along its route.
  ((struct FieldEffect *)beast)->countdown = 0;
  beast->timer = 0;
  beast->userValue = 0;
  beast->gravity = PIXELS(2) / 5;
  beast->update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 308, 291);
  Actor_SetPositionAndCommit(ACTOR_FIRST_WATER_BEAST, 311, 215);
  beast->update = 0;
  beast->paused = 0;
  Event_WaitFrames(16);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTHWEST + FACING_STEP,
                      20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  // Demonstrate Frost between the hopping sections.
  AltinPeak_BeastFreezesSecondPuddle();
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTHEAST + FACING_STEP,
                      40);
  FieldCamera_MoveTo(PIXELS(334), -1, PIXELS(244), 1);
  ((struct FieldEffect *)beast)->countdown = 0;
  beast->timer = 0;
  beast->userValue = 0;
  beast->update = AltinPeak_UpdateBeastHops;
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 320, 232);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 340, 262);
  Actor_SetPositionAndCommit(ACTOR_FIRST_WATER_BEAST, 374, 262);
  beast->update = 0;
  Event_WaitFrames(16);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTHEAST + FACING_STEP,
                      20);
  Actor_ArmCallback(ACTOR_FIRST_WATER_BEAST, FACING_NORTH + FACING_STEP, 40);
  Sound_PlaySong(SE_HIGH_JUMP);
  actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  actor->velocityY = PIXELS(4);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_LEAP);
  Actor_SetPositionAndCommit(ACTOR_FIRST_WATER_BEAST, 380, 248);
  Event_WaitFrames(10);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(6);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTHWEST + FACING_STEP,
                      20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  FieldCamera_MoveTo(PIXELS(304), -1, PIXELS(215), 1);
  ((struct FieldEffect *)beast)->countdown = 0;
  beast->timer = 0;
  beast->userValue = 0;
  beast->update = AltinPeak_UpdateBeastHops;
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 329, 219);
  beast->update = 0;
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Event_WaitFrames(16);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(40);
  Actor_ArmCallback(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags = 0;
  // Open the route for the final two beast jumps.
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 17, 13);
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 18, 13);
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 19, 13);
  Actor_SetSpeedParameters(ACTOR_FIRST_WATER_BEAST, PIXELS(7) / 5,
                           PIXELS(7) / 10);
  actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  Actor_SetSpriteFlags(actor, 0);
  Sound_PlaySong(SE_HIGH_JUMP);
  jumpSpeed = PIXELS(5);
  actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  actor->velocityY = jumpSpeed;
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 295, 215);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  Actor_SetSpriteFlags(actor, 1);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(40);
  Sound_PlaySong(SE_HIGH_JUMP);
  actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  actor->velocityY = jumpSpeed;
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 260, 215);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH + FACING_STEP, 20);
  Actor_ArmCallback(ACTOR_FIRST_WATER_BEAST, FACING_SOUTHEAST + FACING_STEP,
                    20);
  Sound_PlaySong(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_WaitFrames(40);
  MapGrid_CopyCellAttributes(4, 0, 1, 1, 17, 13);
  MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
  MapGrid_CopyCellAttributes(4, 0, 1, 1, 19, 13);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  FieldCamera_SetSpeedParameters(PIXELS(24) / 5, PIXELS(3) / 5);
  FieldCamera_MoveWithinMapBounds(leader->x.fixed, leader->y.fixed,
                                  leader->z.fixed, 1);
  FieldCamera_WaitForArrival();
  FieldActor_SetTargetAndCallback(ACTOR_FIRST_WATER_BEAST,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                  gAltinPeak_FaceLeaderScript);
  GameFlag_Set(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN);
  Event_End();
}

/// Stepping near the beast again: it hops back over the ice and settles down,
/// spitting water.
static void AltinPeak_ShowSecondBeastRoute(void) {
  struct Actor *actor;
  struct Actor *movingActor;
  u32 waitedFrames;

  actor = (struct Actor *)Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  Event_Start();
  Actor_Stop(ACTOR_FIRST_WATER_BEAST);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(279), PIXELS(64), PIXELS(216), 1);
  FieldCamera_WaitForArrival();
  Sound_PlaySong(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(128), PIXELS(64), PIXELS(202), 1);
  ((struct FieldEffect *)actor)->countdown = 0;
  actor->timer = 0;
  actor->userValue = 0;
  actor->gravity = PIXELS(2) / 5;
  actor->update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 212, 200);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 103, 200);
  actor->update = NULL;
  actor->paused = 0;
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH + FACING_STEP, 40);
  Actor_GetById(ACTOR_FIRST_WATER_BEAST)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  movingActor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  Actor_SetSpriteFlags(movingActor, 0);
  Sound_PlaySong(SE_HIGH_JUMP);
  movingActor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  movingActor->velocityY = PIXELS(4);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  Actor_SetPositionAndCommit(ACTOR_FIRST_WATER_BEAST, 86, 214);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  movingActor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
  Actor_SetSpriteFlags(movingActor, 1);
  Event_WaitFrames(10);
  AltinPeak_PlayLandingShake(PIXELS(2), 8);
  Event_WaitFrames(40);
  Actor_GetById(ACTOR_FIRST_WATER_BEAST)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTHEAST + FACING_STEP,
                      20);
  Actor_ArmCallback(ACTOR_FIRST_WATER_BEAST, 0, 40);
  ((struct FieldEffect *)actor)->countdown = 0;
  actor->timer = 0;
  actor->userValue = 0;
  actor->update = AltinPeak_UpdateBeastHops;
  Actor_SetSpeed(ACTOR_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 120, 215);
  actor->update = NULL;
  actor->paused = 0;
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Event_WaitFrames(16);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Event_WaitFrames(40);
  Actor_ArmCallback(ACTOR_FIRST_WATER_BEAST, 0, 10);
  Sound_PlaySong(SE_BEAST_CRY);
  Actor_RunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
  Event_WaitFrames(80);
  Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  // End the route with a spray gust and a short skippable wait.
  AltinPeak_StartSpitting(PIXELS(130), 0, PIXELS(168), FACING_EAST);
  Event_WaitFrames(60);
  for (waitedFrames = 0; waitedFrames < 60 && gKeysPressed == 0;
       waitedFrames++) {
    Event_WaitFrames(1);
  }
  actor = (struct Actor *)Actor_GetById(ACTOR_PARTY_LEADER);
  FieldCamera_SetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  FieldCamera_MoveWithinMapBounds(actor->x.fixed, actor->y.fixed,
                                  actor->z.fixed, 1);
  FieldCamera_WaitForArrival();
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN);
  Event_End();
}

/// Sets up the room being entered: puzzles already solved and the water
/// levels already lowered. Returns 0.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gAdventure.sceneId == (s32)&SceneId_AltinPeakFirstWaterBeast) {
    AltinPeak_RestoreFirstWaterBeastRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakSecondWaterBeast) {
    AltinPeak_RestoreSecondWaterBeastRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakSouthEntrance) {
    AltinPeak_RestoreSouthEntranceRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakThirdWaterBeast) {
    AltinPeak_RestoreThirdWaterBeastRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakPool) {
    AltinPeak_RestorePoolRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakSpritz) {
    AltinPeak_RestoreSpritzRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakFallingRocks) {
    AltinPeak_RestoreFallingRocksRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakHydrosStatue) {
    AltinPeak_RestoreHydrosStatueRoom();
  } else if (gAdventure.sceneId == (s32)&SceneId_AltinPeakWestExit) {
    AltinPeak_RestoreWestExitRoom();
  }
  return 0;
}

/// AltinPeakFirstWaterBeast: after its beast's battle the Frost Jewel is found
/// and the water drains; the beast stays where its routes left it while the
/// water is up, and the frozen puddles are restored once it is down.
static void AltinPeak_RestoreFirstWaterBeastRoom(void) {
  s32 opened;
  s32 flag;
  struct Actor *actor;
  struct MapGridScrollWork *grid;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gAdventure.entrance == ENTRANCE_AFTER_BATTLE) {
      AltinPeak_AwardFrostJewelAndDrain();
    }
  }
  AltinPeak_StartPaletteAnimation();
  if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
    Import_ActorSetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
  } else {
    actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
    if (actor != 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetSpriteFlags(actor, 0);
    }
  }
  actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  if (actor != 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpriteFlags(actor, 0);
  }
  opened = GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN);
  if (opened == 0) {
    Actor_ConfigureSpriteColor(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
    flag = GameFlag_Get(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN);
    if (flag != 0) {
      Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, 0);
      actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
      actor->update = AltinPeak_SparkleIce;
      Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags =
          opened;
      actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
      actor->y.fixed = PIXELS(32);
      MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
      Import_ActorSetPosition(ACTOR_FIRST_WATER_BEAST, PIXELS(120),
                              PIXELS(215));
      actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST);
      actor->facing = opened;
      Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST,
                               WATER_BEAST_ANIMATION_SPIT);
      AltinPeak_StartSpitting(PIXELS(130), 0, PIXELS(168), FACING_EAST);
      return;
    }
    if (GameFlag_Get(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN) == 0) {
      return;
    }
    Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, 0);
    actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
    actor->update = AltinPeak_SparkleIce;
    Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags =
        flag;
    actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
    actor->y.fixed = PIXELS(32);
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
    Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, PIXELS(260), PIXELS(215));
    FieldActor_SetTargetAndCallback(10,
                                    ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                    gAltinPeak_FaceLeaderScript);
  } else {
    grid = gMapGridWork[0];
    Import_ActorSetPosition(ACTOR_FIRST_WATER_BEAST, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    grid->flags &= 0xfdff;
    AltinPeak_MarkActorTerrainCell(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
    if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
      Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE,
                               PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(7, 13, 1, 1, 9, 13);
      actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
      actor->y.fixed = 0;
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
    AltinPeak_MarkActorTerrainCell(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
    if (GameFlag_Get(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN) != 0) {
      Import_ActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
                               PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(29, 1, 3, 1, 17, 13);
      actor = Actor_GetById(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
      actor->y.fixed = PIXELS(32);
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
  }
}

/// AltinPeakSecondWaterBeast: after its beast's battle the second drain runs;
/// while the water is up the beast spits it; the switch is restored.
static void AltinPeak_RestoreSecondWaterBeastRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
    Actor_SetPosition(ACTOR_SECOND_WATER_BEAST, 0, 0);
  } else {
    AltinPeak_StartPaletteAnimation();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
        gAdventure.entrance == ENTRANCE_AFTER_BATTLE) {
      AltinPeak_DrainAfterSecondBeast();
    } else {
      MapGrid_CopyCellAttributes(38, 24, 1, 2, 37, 24);
      MapGrid_CopyCellAttributes(44, 23, 1, 2, 45, 23);
      if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) == 0) {
        Actor_SetSpriteColorById(ACTOR_SECOND_WATER_BEAST, SPRITE_COLOR_RBG);
        Import_ActorSetAnimation(ACTOR_SECOND_WATER_BEAST,
                                 WATER_BEAST_ANIMATION_SPIT);
        AltinPeak_StartSpitting(PIXELS(238), 0, PIXELS(418), FACING_WEST);
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

/// AltinPeakSouthEntrance: its water is up until the second drain. Arriving
/// from the Alpine Crossing, at
/// ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST or _EAST, shows no area
/// name.
static void AltinPeak_RestoreSouthEntranceRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
  } else {
    MapGrid_CopyCellAttributes(52, 42, 1, 1, 53, 42);
  }
  {
    // Either entrance from the Alpine Crossing: _EAST is the number
    // after ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST.
    if ((u32)(((u16)gAdventure.entrance -
               ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST)
              << 16) <= 0x10000) {
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    }
  }
}

/// AltinPeakThirdWaterBeast: after its beast's battle the third drain runs;
/// while the water is up the beast spits it; the frozen puddle and the switch
/// are restored.
static void AltinPeak_RestoreThirdWaterBeastRoom(void) {

  if (GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
    Actor_SetPosition(ACTOR_THIRD_WATER_BEAST, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gAdventure.entrance == ENTRANCE_AFTER_BATTLE) {
        AltinPeak_DrainAfterThirdBeast();
      }
    }
    AltinPeak_StartPaletteAnimation();
    if (GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) == 0) {
      Actor_SetSpriteColorById(ACTOR_THIRD_WATER_BEAST, SPRITE_COLOR_RBG);
      Import_ActorSetAnimation(ACTOR_THIRD_WATER_BEAST,
                               WATER_BEAST_ANIMATION_SPIT);
      AltinPeak_StartSpitting(PIXELS(748), PIXELS(8), PIXELS(280), FACING_WEST);
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
    Import_ActorSetAnimation(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE,
                             PUDDLE_ANIMATION_FROZEN);
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

/// AltinPeakPool: arriving by ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST the mine
/// cart waits there; the frozen puddle, the water level and the switch are
/// restored.
static void AltinPeak_RestorePoolRoom(void) {
  struct Actor *pillar;

  if (gAdventure.entrance == ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_SetPosition(ACTOR_CART, PIXELS(358), PIXELS(104));
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_POOL_ROOM_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
    pillar = Actor_Get(ACTOR_POOL_ROOM_PUDDLE);
    Import_ActorSetAnimation(ACTOR_POOL_ROOM_PUDDLE, PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(45, 41, 1, 1, 43, 41);
    {
      pillar->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | pillar->priorityFlags;
    }
  }
  if (GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
  }
  if (GameFlag_Get(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(46, 29, 16, 28, 1, 2);
  } else {
    MapGrid_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(47, 29, 16, 28, 1, 2);
  }
}

/// AltinPeakSpritz: the pillar, the three frozen puddles and the switch are
/// restored.
static void AltinPeak_RestoreSpritzRoom(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_SPRITZ_ROOM_PUDDLE);
  Actor_SetSpriteFlags(actor, 0);
  AltinPeak_SettleSpritzRoomPillar();
  AltinPeak_MarkActorTerrainCell(ACTOR_SPRITZ_ROOM_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
    Import_ActorSetAnimation(ACTOR_SPRITZ_ROOM_PUDDLE, PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 26, 26);
    {
      struct Actor *actor = Actor_Get(ACTOR_SPRITZ_ROOM_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN) != 0) {
    Import_ActorSetAnimation(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE,
                             PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 17, 10);
    {
      struct Actor *actor = Actor_Get(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  AltinPeak_MarkActorTerrainCell(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN) != 0) {
    Import_ActorSetAnimation(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE,
                             PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 26, 15);
    {
      struct Actor *actor = Actor_Get(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT | actor->priorityFlags;
    }
  }
  Task_AddFrame(AltinPeak_UpdateFrostPillarPriority, TASK_PASS_FRAME);
  if (GameFlag_Get(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(46, 28, 29, 17, 1, 2);
  } else {
    MapGrid_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(47, 28, 29, 17, 1, 2);
  }
}

/// AltinPeakFallingRocks: the boulder is put away; once the rockfall is over
/// the log lies toppled and the rubble fills the bottom of the path. Arriving
/// from AltinPeakHydrosStatue
/// (ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE), the leader starts
/// 88 pixels down.
static void AltinPeak_RestoreFallingRocksRoom(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_STANDING_LOG);
  Import_ActorSetPosition(ACTOR_FALLING_BOULDER, 0, 0);
  Import_ActorSetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_BOULDER_SHADOW), 0);
  actor->motionFlags = 0;
  actor->scaleX = 9 * Q16_ONE / 10;
  actor->scaleY = 3 * Q16_ONE / 5;
  actor->sprite->rotation = 0x8000;
  Actor_GetById(ACTOR_FALLING_ROCK_SIGN)->motionFlags = 0;
  Actor_GetById(ACTOR_FALLING_ROCK_SIGN)->y.fixed = -PIXELS(28);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    actor->x.fixed += PIXELS(14);
    actor->y.fixed += -PIXELS(8);
    actor->sprite->rotation = 0xc000;
  }
  if (GameFlag_Get(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    MapGrid_CopyCellsTo(25, 36, 43, 36, 11, 9);
    MapGrid_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
  }
  if (gAdventure.entrance ==
          ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    Event_Start();
    Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed = -PIXELS(88);
    FieldCamera_MoveWithinMapBounds(PIXELS(792), -PIXELS(88), PIXELS(577), 0);
    MapGrid_CenterOnFollowedPoint();
    WaitFrames(1);
    Event_End();
  }
}

/// AltinPeakHydrosStatue: the guardian statue stands until the Hydros statues
/// are beaten; after their battle the party talks it over.
static void AltinPeak_RestoreHydrosStatueRoom(void) {
  s32 statuesDefeated;
  struct Actor *leader;
  struct Actor *actor;
  s16 entrance;

  statuesDefeated = GameFlag_Get(FLAG_HYDROS_STATUES_DEFEATED);
  if (statuesDefeated != 0) {
    Import_ActorSetPosition(ACTOR_STATUE, 0, 0);
    Import_ActorSetPosition(ACTOR_STATUE_SHADOW, 0, 0);
  } else {
    actor = Actor_GetById(ACTOR_STATUE);
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpritePriority(ACTOR_STATUE_SHADOW, 3);
    actor = Actor_GetById(ACTOR_STATUE_SHADOW);
    Actor_SetSpriteFlags(actor, 0);
    Actor_GetById(ACTOR_STATUE_SHADOW)->collisionFlags = statuesDefeated;
  }
  entrance = gAdventure.entrance;
  if (entrance == ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS ||
      entrance == 98) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Actor_GetById(ACTOR_PARTY_LEADER);
      Event_Start();
      leader->y.fixed = PIXELS(16);
      Event_End();
    }
  } else if (entrance == ENTRANCE_AFTER_BATTLE) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      AltinPeak_CelebrateHydrosVictory();
    }
  }
}

/// AltinPeakWestExit: arriving by ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM
/// the pillar stands at its pushed place.
static void AltinPeak_RestoreWestExitRoom(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_WEST_EXIT_ROOM_PILLAR);
  Actor_SetSpriteFlags(actor, 0);
  if (gAdventure.entrance == ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM) {
    Actor_SetPosition(ACTOR_WEST_EXIT_ROOM_PILLAR, PIXELS(184), PIXELS(328));
  }
}

/// Gives a new spray particle its look: standing still, on sprite priority 1
/// and at 0.3 scale.
static void AltinPeak_InitializeSpray(struct Actor *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  Actor_SetSpriteColor(spray, SPRITE_COLOR_BGR);
  Actor_SetSpriteFlags(spray, 0);
  spray->scaleX = 3 * Q16_ONE / 10;
  spray->scaleY = 3 * Q16_ONE / 10;
}

/// Update routine of a spray particle: it grows while drifting away from the
/// beast, then curls and flattens when its movement timer runs low. The timer
/// can start another curl; the separate lifetime still ends after 20 updates.
static void AltinPeak_DriftSpray(struct Actor *updated) {
  struct Actor *spray = updated;

  switch (gAltinPeak_SprayDirection) {
  case FACING_EAST:
    spray->x.fixed +=
        (spray->timer << 12) +
        ((s32)((((u32)(Import_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
    break;
  case FACING_WEST:
    spray->x.fixed -=
        (spray->timer << 12) +
        ((s32)((((u32)(Import_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
    break;
  }
  if (spray->timer <= 3) {
    switch (gAltinPeak_SprayDirection) {
    case FACING_EAST:
      spray->x.fixed += PIXELS(1) / 2;
      break;
    case FACING_WEST:
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
  if ((u32)(spray->timer * Import_RandomNext()) >> 16 == 0)
    Actor_SetSpriteColor(spray, SPRITE_COLOR_WHITE);
  if (spray->timer != 0)
    spray->timer -= 2;
  else
    spray->timer = ((u32)(Import_RandomNext() * 5) >> 16) * 2 + 2;
  if (--((struct FieldEffect *)updated)->countdown == 0)
    Actor_Release(spray);
}

/// Task while a beast spits: every fourth frame a spray particle
/// starts from its spout, living 20 frames.
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

/// A water beast starts spitting water from (x, y, z) toward facing
/// (FACING_EAST or FACING_WEST), with its sound (SE_AMBIENT_WATER).
static void AltinPeak_StartSpitting(s32 x, s32 y, s32 z, s32 facing) {
  SET_VECTOR(gAltinPeak_SprayOrigin, x, y, z);
  gAltinPeak_SprayDirection = facing;

  Scene_QueueMusic(SE_AMBIENT_WATER);
  Task_AddFrame(AltinPeak_SpawnSpray, TASK_PASS_FRAME);
}

/// Task while the boulder crashes: every third frame a piece of rock
/// (the boulder's own sprite, small) flies out at random, with a clatter
/// (SE_CLATTER or SE_CRASH) every ninth frame.
static void AltinPeak_SpawnDebris(void) {
  struct Actor *debris;
  s32 scale;

  if (gFrameCount % 3 == 0) {
    debris = FieldActor_Create(SPRITE_ROCK,
                               (((u32)(Import_RandomNext() * 48) >> 16) << 16) +
                                   PIXELS(765),
                               -PIXELS(64), PIXELS(608));
    if (debris != NULL) {
      if (gFrameCount % 9 == 0) {
        if ((u32)(Import_RandomNext() << 1) >> 16 != 0)
          Sound_PlaySong(SE_CRASH);
        else
          Sound_PlaySong(SE_CLATTER);
      }
      debris->motionFlags = 0;
      scale = ((u32)(Import_RandomNext() << 15) >> 16) + 3 * Q16_ONE / 10;
      debris->gravity = PIXELS(2) / 5;
      debris->scaleY = scale;
      debris->scaleX = scale;
      debris->frozen = 1;
      Actor_SetSpriteFlags(debris, 0);
      debris->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      debris->sprite->image.priority = 1;
      Actor_SelectAnimation(debris, 1);
      Actor_StartScript(debris, gAltinPeak_DebrisScript);
      SET_VELOCITY(debris, (((u32)(Import_RandomNext() * 6) >> 16) - 3) << 16,
                   PIXELS(8),
                   ((u32)(Import_RandomNext() * 0x600) >> 16) - 0x300);
    }
  }
}

/// Update routine of the frozen second puddle: its ice glints every eight
/// frames.
static void AltinPeak_SparkleIce(struct Actor *updated) {
  struct Actor *actor = (struct Actor *)updated;

  s32 framePhase = gFrameCount & 7;

  if (framePhase == 0) {
    Actor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
  } else if (framePhase == 2) {
    Actor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
  }
}

/// AltinPeakFirstWaterBeast's water beast casts Frost on the second puddle.
static void AltinPeak_BeastFreezesSecondPuddle(void) {
  FieldPsynergy_LoadResources(ABILITY_FROST, 1);
  FieldPsynergy_SetupActorPair(ACTOR_FIRST_WATER_BEAST,
                               ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  FieldPsynergy_StartCast();
  Actor_ConfigureSpriteColor(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  FieldPsynergy_RunHandler(1);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  FieldPsynergy_StopCasterUpdate();
  Actor_ConfigureSpriteColor(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  FieldPsynergy_End();
  Sound_PlaySong(SE_STOP_ACTION_SE);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
}

/// Back from AltinPeakFirstWaterBeast's beast battle: the beast is gone and the
/// party finds the Frost Jewel; the camera pans to the water and the first
/// drain runs.
static void AltinPeak_AwardFrostJewelAndDrain(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  Event_Start();
  Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  ItemDisplay_ShowItem(ITEM_FROST_JEWEL, 3);
  PartyInventory_GiveItem(ITEM_FROST_JEWEL, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(100), 0, PIXELS(249), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(20);
  AltinPeak_DrainFirstWater();
  FieldCamera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  FieldCamera_WaitForArrival();
  Event_SetTriggers(gAltinPeak_FirstWaterBeastTriggersDrained);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Back from AltinPeakSecondWaterBeast's beast battle: the beast is gone, the
/// mine cart waits at (328, 424), the switch is back in its first position, and
/// the second drain runs.
static void AltinPeak_DrainAfterSecondBeast(void) {
  Event_Start();
  Import_ActorSetPosition(ACTOR_SECOND_WATER_BEAST, 0, 0);
  Import_ActorSetPosition(ACTOR_CART, PIXELS(328), PIXELS(424));
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
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
  Import_ActorSetPosition(ACTOR_THIRD_WATER_BEAST, 0, 0);
  Import_ActorSetPosition(ACTOR_CART, PIXELS(488), PIXELS(138));
  GameFlag_Set(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  AltinPeak_DrainThirdWater();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Task while the statue wakes: a rumble (SE_STONE_RUMBLE) every second.
static void AltinPeak_PlayHydrosRumble(void) {
  gAltinPeak_RumbleFrames++;
  if (gAltinPeak_RumbleFrames == 60) {
    Sound_PlaySong(SE_STONE_RUMBLE);
    gAltinPeak_RumbleFrames = 0;
  }
}

/// Stepping toward AltinPeakHydrosStatue's guardian statue: the leader walks up
/// to it, it rumbles and slides forward, and the battle with the Hydros statues
/// starts.
static void AltinPeak_StartHydrosBattle(void) {
  Event_Start();
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(87), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
  Event_PlaySong(SE_BOSS_ROAR);
  Task_AddFrame(AltinPeak_PlayHydrosRumble, TASK_PASS_FRAME);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Actor_SetSpeed(ACTOR_STATUE, PIXELS(1) / 10, PIXELS(1) / 20);
  Actor_SetSpeed(ACTOR_STATUE_SHADOW, PIXELS(1) / 10, PIXELS(1) / 20);
  Import_ActorSetAnimation(ACTOR_STATUE, 2);
  Actor_SetDestination(ACTOR_STATUE, 328, 104);
  Actor_SetDestination(ACTOR_STATUE_SHADOW, 328, 108);
  Event_WaitFrames(60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_WaitForMoveById(ACTOR_STATUE);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_AltinPeakHydrosStatue,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 3);
}

/// Task while the statue sinks: it flickers every other frame.
static void AltinPeak_FlickerStatue(void) {
  if (((gFrameCount >> 1) & 1) != 0) {
    Actor_ConfigureSpriteColor(ACTOR_STATUE, SPRITE_COLOR_WHITE);
  } else {
    Actor_ConfigureSpriteColor(ACTOR_STATUE, SPRITE_COLOR_GREY);
  }
}

/// Back from the Hydros statues' battle: Garet, Ivan and Mia step out. Garet
/// cheers, Ivan asks whether the leader agrees, and Mia tells of the guardian
/// statues; the statue shakes, sinks into the floor and shows a chest behind
/// it, and they agree to open it before they go, then rejoin the leader.
static void AltinPeak_CelebrateHydrosVictory(void) {
  struct Actor *leader;
  struct FieldWork *work;
  TaskFunc update;
  const s32 *celebrate;
  const s32 *party;

  Event_Start();
  Actor_SetPosition(ACTOR_STATUE, PIXELS(328), PIXELS(88));
  Actor_SetPosition(ACTOR_STATUE_SHADOW, PIXELS(328), PIXELS(88));
  Import_ActorSetAnimation(ACTOR_STATUE, 0);
  work = gFieldWork;
  work->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(20);
  // Bring the party out for the victory conversation.
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetScript(ACTOR_GARET, gAltinPeak_GaretScript);
  Actor_SetScript(ACTOR_IVAN, gAltinPeak_IvanScript);
  Actor_RunScriptAndWait(ACTOR_MIA, gAltinPeak_MiaScript);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Event_SetMessageId(MsgAltinWeDidIsaacWeBeat);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Event_OpenMessage(ACTOR_IVAN, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  } else {
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    Event_SkipMessages(1);
  }
  FieldEvent_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_SetMessageId(MsgAltinGuardianStatuesWereCreatedLong);
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 40);
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_PlaySong(SE_WARP);
  Actor_SetSpriteColorById(ACTOR_STATUE, SPRITE_COLOR_WHITE);
  Event_WaitFrames(10);
  Event_PlaySong(SONG_FADE_OUT_ACTION_SE);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_PlaySong(SE_CURSE);
  // Animate the statues while the party reacts.
  update = AltinPeak_FlickerStatue;
  Task_AddFrame(update, TASK_PASS_FRAME);
  celebrate = gAltinPeak_StatueSinkScript;
  Actor_SetScript(ACTOR_STATUE_SHADOW, celebrate);
  Actor_RunScriptAndWait(ACTOR_STATUE, celebrate);
  Task_Remove(update);
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  FieldEvent_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  // Return the companions and save the completed Hydros battle.
  party = gAltinPeak_RejoinScript;
  Actor_SetScript(ACTOR_GARET, party);
  Actor_SetScript(ACTOR_IVAN, party);
  Actor_RunScriptAndWait(ACTOR_MIA, party);
  Event_WaitFrames(20);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  gFieldWork->transitionFrames = 16;
  GameFlag_Set(FLAG_HYDROS_STATUES_DEFEATED);
  Event_End();
}

/// Unused: asks the field to use Frost (psynergy 0x1018, party member 4 and
/// ABILITY_FROST); nothing calls it.
UNUSED static void AltinPeak_RequestPsynergy(void) {
  gFieldWork->psynergy = 0x1018;
}

/// Task in AltinPeakSpritz: while the leader stands higher than 48
/// pixels the second pillar is drawn under the party and the third further
/// back.
static void AltinPeak_UpdateFrostPillarPriority(void) {
  if (Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed > PIXELS(48)) {
    {
      u8 *flag = &Actor_GetById(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE)->priorityFlags;
      s32 bit = 2;

      bit |= *flag;
      *flag = bit;
    }
    Actor_SetSpritePriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 3);
  } else {
    Actor_SetSpritePriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 2);
  }
}

/// Makes a puddle walkable to the actor checks and marks its map cell.
static void AltinPeak_MarkActorTerrainCell(s32 actorId) {
  struct Actor *actor;

  actor = Actor_GetById(actorId);
  actor->collisionFlags &= (u8)~ACTOR_SOLID;

  MapGrid_SetMetatileBehaviorAt(0, actor->x.fixed, actor->z.fixed, MB_WALL);
}

/// The mine cart reaches the end of its run: it settles in the middle of its
/// cell and the party leader jumps off it to the south.
static void AltinPeak_StopCartAndJumpOff(struct Actor *actor) {
  s32 x;
  s32 z;
  s32 centerZ;
  struct Sprite *sprite;
  struct Actor *leader;

  Sound_PlaySong(SE_STOP_ACTION_SE);
  Sound_PlaySong(SE_CRUNCH);
  x = actor->x.fixed & 0xfff00000;
  centerZ = PIXELS(8);
  z = actor->z.fixed & 0xfff00000;
  actor->acceleration = PIXELS(2);
  x += centerZ;
  centerZ += z;
  FieldActor_SetMoveTarget(actor, x, actor->y.fixed, centerZ);
  Actor_WaitForMove(actor);
  actor->layer = 0;
  actor->x.fixed = x;
  actor->z.fixed = centerZ;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  Actor_SelectAnimation(actor, 2);
  WaitFrames(15);
  Actor_SelectAnimation(actor, 1);
  WaitFrames(30);
  sprite = actor->sprite;
  sprite->count = 1;
  SpriteAnim_Clear(sprite->entries[1]);
  sprite->entries[1] = NULL;
  sprite->dirty = 1;
  leader = Actor_Lookup(gAdventure.leaderActorId);
  Sound_PlaySong(SE_JUMP);
  leader->x.fixed = x;
  leader->velocityY = PIXELS(6);
  leader->gravity = PIXELS(1);
  leader->z.fixed = centerZ;
  leader->sprite->image.priority = 0;
  Actor_SelectAnimation(leader, 7);
  FieldActor_SetMoveTarget(leader, x, leader->y.fixed, z + PIXELS(24));
  leader->facing = FACING_SOUTH;
  WaitFrames(20);
  leader->sprite->image.priority = 2;
  Sound_PlaySong(SE_DOOR_CLOSE);
}

UNUSED static void AltinPeak_ReservedNoOp(void) {}

/// The cart turns a quarter turn right around the corner it reaches.
static void AltinPeak_TurnCartRight(struct Actor *actor) {
  struct FieldPosition *point;
  struct FieldPosition position;
  s32 angle;
  s32 x;
  s32 z;
  s32 turnFrame;

  angle = (actor->facing + 0x4000) & 0xc000;
  point = &position;
  SET_POSITION(point, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffset(PIXELS(24), angle, point);
  x = (point->x + PIXELS(8)) & 0xfff00000;
  z = (point->z + PIXELS(8)) & 0xfff00000;
  angle += 0x8000;
  Actor_SelectAnimation(actor, 5);
  turnFrame = 0;
  Sound_PlaySong(SE_CART_TURN);
  while (turnFrame < 16) {
    angle += 0x400;
    point->x = x;
    point->z = z;
    FieldVector_AddPolarOffset(PIXELS(24), angle, point);
    actor->x.fixed = point->x;
    actor->z.fixed = point->z;
    actor->facing = angle + 0x4000;
    WaitFrames(1);
    turnFrame++;
  }
  Sound_PlaySong(SE_CART_ROLL);
}

/// The cart turns a quarter turn left.
static void AltinPeak_TurnCartLeft(struct Actor *actor) {
  struct FieldPosition *point;
  struct FieldPosition position;
  s32 angle;
  s32 x;
  s32 z;
  s32 turnFrame;

  angle = (actor->facing - 0x4000) & 0xc000;
  point = &position;
  SET_POSITION(point, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffset(PIXELS(24), angle, point);
  x = (point->x + PIXELS(8)) & 0xfff00000;
  z = (point->z + PIXELS(8)) & 0xfff00000;
  angle += 0x8000;
  Actor_SelectAnimation(actor, 6);
  turnFrame = 0;
  Sound_PlaySong(SE_CART_TURN);
  while (turnFrame < 16) {
    angle -= 0x400;
    point->x = x;
    point->z = z;
    FieldVector_AddPolarOffset(PIXELS(24), angle, point);
    actor->x.fixed = point->x;
    actor->z.fixed = point->z;
    actor->facing = angle - 0x4000;
    WaitFrames(1);
    turnFrame++;
  }
  Sound_PlaySong(SE_CART_ROLL);
}

/// The cart rolls over a marked cell: SE_STOP_ACTION_SE plays, and SE_CART_TURN
/// and SE_CART_ROLL follow as its countdown runs out.
static void AltinPeak_StartCartCountdown(struct Actor *actor) {
  actor->userValue = 33;
  Sound_PlaySong(SE_STOP_ACTION_SE);
}

/// Rolls the cart on by a frame and picks its animation from the terrain ahead
/// of and behind it: flat, uphill or downhill.
static void AltinPeak_AdvanceCart(struct Actor *actor) {
  s32 angle = 0xc000;
  s32 radius;
  s16 *timer;
  struct FieldPosition position;
  struct FieldPosition *point;
  s32 ahead;
  s32 behind;

  angle &= actor->facing;
  radius = actor->y.fixed / Q16_ONE;
  radius = gAltinPeak_RollRadii[actor->timer - radius + 16];
  timer = (s16 *)&actor->userValue;
  if (*timer != 0) {
    if (--*timer == 20) {
      Sound_PlaySong(SE_CART_TURN);
    }
    if (*timer == 0) {
      Sound_PlaySong(SE_CART_ROLL);
    }
  }
  point = &position;
  SET_POSITION(point, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffset(Iwram_MulQ16(radius, 3 * Q16_ONE / 4), angle,
                             point);
  actor->x.fixed = point->x;
  actor->z.fixed = point->z;
  ahead = MapGrid_GetTerrainHeight(2, point->x, point->z);
  FieldVector_AddPolarOffset(-Iwram_MulQ16(radius, 3 * Q16_ONE / 2), angle,
                             point);
  behind = MapGrid_GetTerrainHeight(2, point->x, point->z);
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

/// The metatile behaviors that mark the mine cart's route, on map layer 2.
enum AltinPeakMetatileBehavior {
  MB_ALTIN_PEAK_CART_COUNTDOWN = 96, ///< AltinPeak_StartCartCountdown
  MB_ALTIN_PEAK_CART_TURN_LEFT = 97,
  MB_ALTIN_PEAK_CART_TURN_RIGHT = 98,
  /// The end of the route, where the leader jumps off.
  MB_ALTIN_PEAK_CART_STOP = 99
};

/// Rolls the mine cart the party leader rides: it sets off the first way it can
/// roll, starting from heading (or its own facing for -1), follows the turns
/// its route's cells mark until the end cell, and the leader jumps off.
static void AltinPeak_RollCart(s32 id, s32 heading) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *leader = Actor_Lookup(gAdventure.leaderActorId);
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
    FieldVector_AddPolarOffset(PIXELS(16), heading, &position);
    if (MapGrid_GetTerrainHeight(2, position.x, position.z) == actor->y.fixed) {
      break;
    }
    heading += 0x4000;
  }
  if (directionIndex == 4) {
    return;
  }
  actor->layer = 2;
  leader->x.fixed = 0;
  leader->z.fixed = 0;
  Sprite_AddAnim(actor->sprite, 16);
  FieldCamera_FollowActor(id, 1);
  FieldCamera_WaitForArrival();
  FieldCamera_SetSpeed(PIXELS(16), PIXELS(2));
  actor->facing = heading;
  actor->speedLimit = PIXELS(2);
  actor->acceleration = PIXELS(1) / 20;
  actor->paused = 0;
  actor->timer = actor->y.fixed / Q16_ONE;
  actor->userValue = 0;
  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed;
  FieldVector_AddPolarOffset(PIXELS(24), heading, &position);
  FieldActor_SetMoveTarget(actor, position.x, actor->y.fixed, position.z);
  Actor_WaitForMove(actor);
  Sound_PlaySong(SE_CART_ROLL);
  // The route cells select turns, a countdown sound, and the destination.
  for (;;) {
    switch (MapGrid_GetMetatileBehaviorAt(2, actor->x.fixed, actor->z.fixed)) {
    case MB_ALTIN_PEAK_CART_TURN_RIGHT:
      AltinPeak_TurnCartRight(actor);
      break;
    case MB_ALTIN_PEAK_CART_TURN_LEFT:
      AltinPeak_TurnCartLeft(actor);
      break;
    case MB_ALTIN_PEAK_CART_COUNTDOWN:
      AltinPeak_StartCartCountdown(actor);
      break;
    case MB_ALTIN_PEAK_CART_STOP:
      goto arrived;
    }
    AltinPeak_AdvanceCart(actor);
    WaitFrames(1);
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
/// The water's palette cycle: pairs of a color offset and the frames it holds,
/// repeating from the top at -1.
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
    SC_SET(SW_TIMER, 1),
    SC_END,
};

/// The party leader flees down the winding path ahead of the boulder, marks
/// the arrival, steps aside and turns round.
s32 gAltinPeak_LeaderFleePath[] = {
    SC_ANIMATION(5),
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
    SC_SET(SW_TIMER, 1),
    SC_MOVE(PIXELS(854), 0, PIXELS(552)),
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
    SC_MOVE(PIXELS(314), 0, PIXELS(126)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_IvanScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(346), 0, PIXELS(126)),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_MiaScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(PIXELS(328), 0, PIXELS(138)),
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
    SC_MOVE(PIXELS(328), 0, PIXELS(118)),
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

struct ActorTemplate gAltinPeak_ActorTemplatesOther[] = {
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_FirstWaterBeastActorTemplates[] = {
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(152), 0, PIXELS(216), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(296), 0, PIXELS(216), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(296), 0, PIXELS(336), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_SecondWaterBeastActorTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(216), 0, PIXELS(312), FACING_EAST,
        TALK_FACING_KEEP, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(248), 0, PIXELS(464), FACING_WEST,
        TALK_FACING_KEEP, 0},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(888), 0, PIXELS(172), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_ThirdWaterBeastActorTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(536), 0, PIXELS(296), FACING_EAST,
        TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(408), 0, PIXELS(216), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(759), 0, PIXELS(285), FACING_WEST,
        TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_PoolActorTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(712), 0, PIXELS(184), FACING_EAST,
        TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(696), 0, PIXELS(664), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_SpritzActorTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(264), 0, PIXELS(408), FACING_EAST,
        TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(424), 0, PIXELS(424), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(408), 0, PIXELS(424), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(280), 0, PIXELS(168), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(424), 0, PIXELS(248), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(632), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SPRITZ), MOVEMENT_TYPE_STAND, PIXELS(56), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_JunctionActorTemplates[] = {
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(536), 0, PIXELS(636), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_FallingRocksActorTemplates[] = {
    {SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(784), 0, PIXELS(80), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_BLANK_LARGE, CONDITION_ALWAYS, 7, PIXELS(784), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(76), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_BLANK, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_SIGNPOST, CONDITION_ALWAYS, 7, PIXELS(792), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_HydrosStatueActorTemplates[] = {
    {0xc7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(84), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(88), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0},
    {ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0},
    {ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gAltinPeak_WestExitActorTemplates[] = {
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(312), 0, PIXELS(280), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(200), 0, PIXELS(328), FACING_SOUTH,
        TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct Trigger gAltinPeak_TriggersOther[] = {
    {TRIGGERS_END},
};

struct Trigger gAltinPeak_FirstWaterBeastTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {TRIGGER_STEP, 10, FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN, EVENT(AltinPeak_ShowFirstBeastRoute)},
    {TRIGGER_STEP, 11, FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN, EVENT(AltinPeak_ShowSecondBeastRoute)},
    {TRIGGER_TALK, ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS, EVENT(AltinPeak_FightFirstWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(AltinPeak_FightFirstWaterBeast)},
    {TRIGGERS_END},
};

struct Trigger gAltinPeak_FirstWaterBeastTriggersDrained[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN,
        EVENT(AltinPeak_FreezeFirstWaterBeastRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN, EVENT(AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle)},
    {TRIGGERS_END},
};

struct Trigger gAltinPeak_RailTunnelTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST},
    {TRIGGERS_END},
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
    {TRIGGERS_END},
    // Unused: two rows past the end marker, which no lookup reaches: a second
    // ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE exit and a chest on
    // trigger 100 holding a Herb under FLAG_ALTIN_PEAK_UNUSED_HERB, the spot
    // where the Cookie chest above already stands.
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_UNUSED_HERB, FIND_ITEM(ITEM_HERB)},
    {TRIGGERS_END},
};

struct Trigger gAltinPeak_SouthEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST},
    {TRIGGERS_END},
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
    {TRIGGERS_END},
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
    {TRIGGERS_END},
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
    {TRIGGERS_END},
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
    {TRIGGERS_END},
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
    {TRIGGERS_END},
};

struct Trigger gAltinPeak_HydrosStatueTriggers[] = {
    {TRIGGER_STEP, 10, FLAG_HYDROS_STATUES_DEFEATED,
     EVENT(AltinPeak_StartHydrosBattle)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_ALTIN_PEAK_LIFTING_GEM,
     FIND_ITEM(ITEM_LIFTING_GEM)},
    {TRIGGERS_END},
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
    {TRIGGERS_END},
};

/// Frames since the statue's last rumble.
s32 gAltinPeak_RumbleFrames = 0;

/// The cart's speed factor by its height against where it set off: 2 to the
/// power i / 16 for i from 0 to 32, in 16.16.
u32 gAltinPeak_RollRadii[] = {
    0x10000, 0x10b55, 0x1172b, 0x12387, 0x1306f, 0x13dea, 0x14bfd,
    0x15ab0, 0x16a09, 0x17a11, 0x18ace, 0x19c49, 0x1ae89, 0x1c199,
    0x1d581, 0x1ea4b, 0x20000, 0x216ab, 0x22e56, 0x2470f, 0x260df,
    0x27bd4, 0x297fb, 0x2b560, 0x2d413, 0x2f422, 0x3159c, 0x33892,
    0x35d13, 0x38333, 0x3ab03, 0x3d495, 0x40000,
};

/// @file
/// Altin Peak, the mine below Altin that its villagers and the Alpine
/// Crossing's signpost call Altin Mine: eleven scenes,
/// MapId_AltinPeakFirstWaterBeast to MapId_AltinPeakWestExit, whose area
/// name is Altin Peak, except that the international editions call
/// AltinPeakSouthEntrance the Alpine Crossing when the party comes in from
/// there (ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST and
/// ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST). Each scene is a room
/// of the mine, and the getters pick each room's tables by gGameState.scene,
/// all the rooms share one exit table, and Map_Initialize calls the setup of
/// the room being entered (AltinPeak_RestoreFirstWaterBeastRoom and so on),
/// which puts back what the party has already done there.
///
/// Altin's doorways lead down into AltinPeakFirstWaterBeast,
/// AltinPeakSouthEntrance, AltinPeakSpritz and AltinPeakJunction. The mine is
/// flooded, and a water beast (SPRITE_WATER_BEAST, drawn with its green and
/// blue swapped, SPRITE_COLOR_RBG) waits in AltinPeakFirstWaterBeast,
/// AltinPeakSecondWaterBeast and AltinPeakThirdWaterBeast. In
/// AltinPeakFirstWaterBeast the first one shows the way: stepping near it
/// (touch 10, AltinPeak_ShowFirstBeastRoute) it cries out, hops around the
/// room, freezes the second puddle with Frost and hops across, and stepping
/// near it again (touch 11) it hops back and settles down to spit. Talking to a
/// beast, or reading its mind, starts its battle, and the party comes back to
/// the room by WARP_DEST_AFTER_BATTLE, where the room's setup runs a drain: the
/// water layer sinks and flickers away (AltinPeak_DrainFirstWater and the two
/// after it), and FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN remember the
/// three drains, which Altin's code reads to lower the water in the village
/// too. Back from the first battle, in AltinPeakFirstWaterBeast, the party
/// finds the Frost Jewel before the water drains, and from then on Frost
/// freezes the mine's puddles into ice pillars to stand on (scene flags keep
/// them frozen only within one visit). AltinPeakSouthEntrance's water stands
/// until the second drain and AltinPeakPool's until the third.
///
/// The rooms between are built from a few mechanisms: mine carts the party
/// rides (talking to one, AltinPeak_RollCart), which roll along the turns their
/// route's cells mark; floor switches in AltinPeakSecondWaterBeast,
/// AltinPeakThirdWaterBeast, AltinPeakPool and AltinPeakSpritz, each remembered
/// by a place flag (FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH and the
/// three after it) and redrawn on entry; and a
/// stone pillar in AltinPeakSpritz that Move or a push slides along. Spritz, a
/// Mercury Djinni, waits there too, placed until it has joined, and talking to
/// it starts its battle. In AltinPeakFallingRocks a sign warns `Watch for
/// falling rocks! Do not strike the wall! Rocks may fall!` at what looks like a
/// dead end: while the party has no Orb of Force, Garet steps out and strikes
/// the wall anyway, and with it Force does the same (AltinPeak_ForceLogOver);
/// either way a boulder rolls after the leader down the path and crashes into
/// the wall at the bottom, leaving rubble (FLAG_ALTIN_PEAK_BOULDER_FALLEN).
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
/// onto the world map (ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP).
///
/// The falling boulder is SPRITE_ROCK, its shadow a SPRITE_BLANK_LARGE actor,
/// and its pieces in the crash SPRITE_ROCK again, scaled down
/// (AltinPeak_SpawnDebris); the beasts' spray is SPRITE_DUST_PUFF recoloured,
/// as in Altin.
///
/// Open questions: none.

#include "action_script.h"
#include "version.h"
#include "macros.h"
#include "game_state.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/altin_peak.h"
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
#include "script_layer.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "call.h"
#include "field_effect.h"
#include "iwram_call.h"
#include "dma.h"

#include "types.h"

#include "facing_object.h"
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"

#include "battle_effect_runtime.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "objcmd.h"
#include "irq.h"
#include "fixed_math.h"
#include "field_runtime.h"
#include "map_grid_scroll.h"
#include "songs.h"
#include "audio_engine_symbols.h"

// The rooms' floor switches, each set once flipped from its first position;
// they are place flags, kept until the party leaves the mine.

/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH FLAG_MAPSEC_TEMP_23
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH FLAG_MAPSEC_TEMP_25
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH FLAG_MAPSEC_TEMP_26
/// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
#define FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH FLAG_MAPSEC_TEMP_27

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
static void AltinPeak_SparkleIce(union FieldObject *fieldObject);
static void AltinPeak_BeastFreezesSecondPuddle(void);
static void AltinPeak_AwardFrostJewelAndDrain(void);
static void AltinPeak_DrainAfterSecondBeast(void);
static void AltinPeak_DrainAfterThirdBeast(void);
static void AltinPeak_CelebrateHydrosVictory(void);
static void AltinPeak_UpdateFrostPillarPriority(void);
static void AltinPeak_MarkObjectEventTerrainCell(s32 objectEventId);
static void AltinPeak_RollCart(s32 id, s32 heading);
static void AltinPeak_StartPaletteAnimation(void);

/// Each room's water beast fight returns the party to this entrance.
enum { WARP_DEST_AFTER_BATTLE = 99 };

/// The rooms' actors, from their placement tables. AltinPeakSecondWaterBeast,
/// AltinPeakThirdWaterBeast, AltinPeakPool and AltinPeakSpritz place the mine
/// cart the party rides first.
enum {
  /// ★ Named from what the player sees: a wooden mine cart on the rails (its
  /// render)
  LOCALID_CART = LOCALID_FIRST_PLACED
};

/// AltinPeakFirstWaterBeast: a puddle that appears with the first drain, a
/// second puddle and the water beast.
enum {
  /// ★ Named from what the player sees: a small blue puddle on the floor west
  /// of the beast's ledge, once the water has drained (its render)
  LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a small blue puddle on the ledge north
  /// of the beast (its render)
  LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
  /// Its battle names it a Living Statue.
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  LOCALID_FIRST_WATER_BEAST
};

enum {
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  LOCALID_SECOND_WATER_BEAST = LOCALID_CART + 1,
  /// ★ Named from what the player sees: a round brown boulder by the chest in
  /// the room's east end (its render)
  LOCALID_SECOND_WATER_BEAST_ROOM_BOULDER
};

enum {
  /// ★ Named from what the player sees: a small blue puddle on the floor (its
  /// render)
  LOCALID_THIRD_WATER_BEAST_ROOM_PUDDLE = LOCALID_CART + 1,
  /// ★ Named from the game: "Are you the ones who defeated the water beasts?"
  /// (MsgAltinOnesWhoDefeatedWaterBeasts)
  LOCALID_THIRD_WATER_BEAST
};

enum {
  /// ★ Named from what the player sees: a small blue puddle on the floor (its
  /// render)
  LOCALID_POOL_ROOM_PUDDLE = LOCALID_CART + 1
};

enum {
  /// ★ Named from what the player sees: a small blue puddle beside the stone
  /// pillar (its render)
  LOCALID_SPRITZ_ROOM_PUDDLE = LOCALID_CART + 1,
  /// ★ Named from what the player sees: a stone pillar standing in the room's
  /// south (its render)
  LOCALID_SPRITZ_ROOM_PILLAR,
  /// ★ Named from what the player sees: a small blue puddle in the room's
  /// north (its render)
  LOCALID_SPRITZ_ROOM_SECOND_PUDDLE,
  /// ★ Named from what the player sees: a small blue puddle by the rails (its
  /// render)
  LOCALID_SPRITZ_ROOM_THIRD_PUDDLE,
  /// Unused: a round boulder at (520, 632), outside both of the room's camera
  /// boxes (gAltinPeak_SpritzWarpDests': one ends at z 568, the one below it
  /// at x 384), so the player never sees it, and no event names it. It stands
  /// almost where AltinPeakJunction places its own boulder, (536, 636).
  /// ★ Named from the game: "Mother, look!! A boulder blocking the road!!"
  /// (MsgGomaNoUsePsynergy)
  LOCALID_UNUSED_SPRITZ_ROOM_BOULDER,
  /// The Mercury Djinni, placed until it has joined.
  /// ★ Named from the game: "Spritz" (MsgDjinnName)
  LOCALID_SPRITZ = 14
};

enum {
  /// ★ Named from what the player sees: a round brown boulder (its render)
  LOCALID_JUNCTION_ROOM_BOULDER = LOCALID_FIRST_PLACED
};

/// AltinPeakFallingRocks: the boulder that falls and its shadow, the log that
/// topples as it comes down, the actor Force is cast on, and the sign.
enum {
  /// ★ Named from what the player sees: its sprite, a grey boulder (its
  /// render)
  LOCALID_FALLING_BOULDER = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: its sprite, a dark shadow (its render)
  LOCALID_BOULDER_SHADOW,
  /// SPRITE_LOG_NORTH_SOUTH.
  /// ★ Named from what the player sees: a log standing on end beside the sign
  /// at the dead end, lying on its side after the rockfall (its render)
  LOCALID_STANDING_LOG,
  /// SPRITE_BLANK, at the dead end's wall.
  /// ★ Named from the game: "Do not strike the wall!"
  /// (MsgAltinWatchFallingRocks)
  LOCALID_WALL,
  /// The falling-rock sign, drawn with the sprite of Lalivero's sign and the
  /// Kolima Forest floodgate's (SPRITE_SIGNPOST); it is read by searching cell
  /// code 12
  /// (AltinPeak_ReadFallingRockSign).
  /// ★ Named from the game: "Watch for falling rocks!"
  /// (MsgAltinWatchFallingRocks)
  LOCALID_FALLING_ROCK_SIGN
};

/// AltinPeakHydrosStatue: the guardian statue and its shadow.
enum {
  /// ★ Named from the game: "Could the guardian statues have turned into the
  /// monsters?" (MsgAltinTheWaterBeastsLookedExactlyLike)
  LOCALID_STATUE = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: its sprite, a dark shadow (its render)
  LOCALID_STATUE_SHADOW
};

enum {
  /// ★ Named from what the player sees: a round brown boulder by the mine's
  /// west exit (its render)
  LOCALID_WEST_EXIT_ROOM_BOULDER = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a stone pillar south of the boulder
  /// (its render)
  LOCALID_WEST_EXIT_ROOM_PILLAR
};

/// The rooms' frozen puddles and Garet at the sign: scene flags, kept within
/// one visit to a room, across its battles.
enum {
  /// Frost has frozen the room's first puddle.
  /// ★ Named from what the player sees: a small puddle of water on the mine's
  /// floor (its render)
  FLAG_PUDDLE_FROZEN = FLAG_MAP_TEMP_00,
  /// Frost has frozen AltinPeakFirstWaterBeast's second puddle or
  /// AltinPeakSpritz's second.
  /// ★ Named from what the player sees: a small puddle of water on the mine's
  /// floor (its render)
  FLAG_SECOND_PUDDLE_FROZEN = FLAG_MAP_TEMP_01,
  /// Frost has frozen AltinPeakSpritz's third puddle.
  /// ★ Named from what the player sees: a small puddle of water on the mine's
  /// floor (its render)
  FLAG_THIRD_PUDDLE_FROZEN = FLAG_MAP_TEMP_04,
  /// Garet has come out to look at AltinPeakFallingRocks's falling-rock sign.
  /// ★ Named from the game: "Watch for falling rocks!"
  /// (MsgAltinWatchFallingRocks)
  FLAG_GARET_WATCHING_ROCKS = FLAG_MAP_TEMP_05
};

/// A chest searched while facing the given way.
#define BG_EVENT_CHEST_FACING(facing)                                          \
  (BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | (facing))

/// SPRITE_PUDDLE's animation once Frost has frozen it into an ice pillar.
enum { PUDDLE_ANIMATION_FROZEN = 5 };

// The spray's direction (0 east, 0x8000 west) and origin, and the water's
// flicker timer.
extern s32 gAltinPeak_SprayDirection;
extern s32 gAltinPeak_SprayOrigin[3];
extern u16 gAltinPeak_FlickerTimer;

/// Talking to the Djinni in AltinPeakSpritz starts the battle with Spritz, a
/// Mercury Djinni.
static void AltinPeak_TalkToDjinni(void) {
  Field_RequestDjinnBattle(LOCALID_SPRITZ, DJINNI_SPRITZ);
}

// Shared source, included where its bytes sit in the ROM.
#include "common/object_facing.h"

// Update callback of AltinPeakFallingRocks's boulder shadow: it stays under the
// boulder, on the ground 12 pixels down, while the boulder bounces. UB: update
// hooks return void. Plain void removes the native zero result and changes its
// return sequence (three instructions in tbs-en).
#ifdef UBFIX
static void AltinPeak_ShadowFollowBoulder(union FieldObject *fieldObject) {
#else
static s32 AltinPeak_ShadowFollowBoulder(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *follower = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_FALLING_BOULDER);

  follower->x.fixed = objectEvent->x.fixed;
  follower->y.fixed = -PIXELS(12);
  follower->z.fixed = objectEvent->z.fixed;
#ifndef UBFIX
  return 0;
#endif
}

/// Script callback for a piece of flying rock debris: it moves by its
/// velocity, grows by its z velocity and falls under its gravity.
static s32 AltinPeak_FlyDebris(struct ScriptInterpreter *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  s32 rise;
  s32 growth;

  objectEvent->x.fixed += objectEvent->velocityX;
  rise = objectEvent->velocityY;
  objectEvent->y.fixed += rise;
  growth = objectEvent->velocityZ;
  objectEvent->scaleX += growth;
  objectEvent->scaleY += growth;
  objectEvent->velocityY = rise - objectEvent->gravity;
  return 0;
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_AltinPeakFirstWaterBeast) {
    return gAltinPeak_FirstWaterBeastWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakRailTunnel) {
    return gAltinPeak_RailTunnelWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakSecondWaterBeast) {
    return gAltinPeak_SecondWaterBeastWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakSouthEntrance) {
    return gAltinPeak_SouthEntranceWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakThirdWaterBeast) {
    return gAltinPeak_ThirdWaterBeastWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakPool) {
    return gAltinPeak_PoolWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakSpritz) {
    return gAltinPeak_SpritzWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakJunction) {
    return gAltinPeak_JunctionWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakFallingRocks) {
    return gAltinPeak_FallingRocksWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakHydrosStatue) {
    return gAltinPeak_HydrosStatueWarpDests;
  }
  if (mapId == (s32)&MapId_AltinPeakWestExit) {
    return gAltinPeak_WestExitWarpDests;
  }
  return gAltinPeak_WarpDestsOther;
}

/// AltinPeakFallingRocks and AltinPeakHydrosStatue have regions.
const struct MapWarpRegion *Map_GetWarpRegions(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_AltinPeakFallingRocks) {
    return gAltinPeak_FallingRocksRegions;
  }
  if (mapId == (s32)&MapId_AltinPeakHydrosStatue) {
    return gAltinPeak_HydrosStatueRegions;
  }
  return 0;
}

/// All the rooms share one exit table.
const u32 *Map_GetWarps(void) { return gAltinPeak_Warps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_AltinPeakFirstWaterBeast) {
    return gAltinPeak_FirstWaterBeastObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakSecondWaterBeast) {
    return gAltinPeak_SecondWaterBeastObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakThirdWaterBeast) {
    return gAltinPeak_ThirdWaterBeastObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakPool) {
    return gAltinPeak_PoolObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakSpritz) {
    return gAltinPeak_SpritzObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakJunction) {
    return gAltinPeak_JunctionObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakFallingRocks) {
    return gAltinPeak_FallingRocksObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakHydrosStatue) {
    return gAltinPeak_HydrosStatueObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_AltinPeakWestExit) {
    return gAltinPeak_WestExitObjectEventTemplates;
  }
  return gAltinPeak_ObjectEventTemplatesOther;
}

/// Talking to a mine cart: the party leader jumps into it, and once the leader
/// has landed in it the cart rolls away along its rails with the leader riding
/// it.
static void AltinPeak_RideCart(s32 log) {
  struct ObjectEvent *logObjectEvent;
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  logObjectEvent = ObjectEvent_GetById(LOCALID_CART);
  Field_EndBusyAndReveal();
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                 ANIMATION_LEADER_LEAN_BACK);
  Engine_ScriptWait(10);
  Engine_AudioPlayCue(SE_JUMP);
  Engine_ObjectEventSetSpeed(LOCALID_PARTY_LEADER, PIXELS(16) / 5,
                             PIXELS(8) / 5);
  // Leap by the height between leader and cart, in quarter pixels, plus 4.
  leader->velocityY =
      ((ABS(logObjectEvent->y.fixed - leader->y.fixed) >> 14) << 14) +
      PIXELS(4);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_LEAP);
  Engine_ObjectSetPosition(leader, logObjectEvent->x.fixed,
                           logObjectEvent->y.fixed, logObjectEvent->z.fixed);
  Engine_TaskWait(10);
  leader->sprite->image.priority = 3;
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);

  // Wait until the leader has come down into the cart.
  for (;;) {
    if (!((logObjectEvent->y.fixed >> 14) < (leader->y.fixed >> 14)))
      break;
    Engine_TaskWait(1);
  }
  Engine_ScriptEnd();
  Engine_AudioPlayCue(SE_159);
  AltinPeak_RollCart(log, 0);
  Engine_TaskWait(20);
  FieldEffect_PlayQueuedSound();
}

/// The party fights the first water beast. Talking to (or reading the mind of)
/// a room's water beast starts its battle; afterwards the party returns to the
/// room's battle entrance.
static void AltinPeak_FightFirstWaterBeast(void) {
  Battle_SetStart(BATTLE_START_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_AltinPeakFirstWaterBeast,
                                 WARP_DEST_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// The party fights the second water beast. It starts and ends as the first
/// one's does, from its own room.
static void AltinPeak_FightSecondWaterBeast(void) {
  Battle_SetStart(BATTLE_START_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_AltinPeakSecondWaterBeast,
                                 WARP_DEST_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// The party fights the third water beast. It starts and ends as the first
/// one's does, from its own room.
static void AltinPeak_FightThirdWaterBeast(void) {
  Battle_SetStart(BATTLE_START_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_AltinPeakThirdWaterBeast,
                                 WARP_DEST_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// Move on AltinPeakSpritz's pillar: the cell it left becomes free again and
/// the cell it now stands on (column 24 or 25 of row 26) takes the blocking
/// attributes kept at (0, 0).
static void AltinPeak_SettleSpritzRoomPillar(void) {
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_PILLAR);

  if (pillar != 0) {
    s32 column;

    MapGrid_CopyCellAttributes(24, 27, 2, 1, 24, 26);
    column = pillar->x.fixed >> 20;
    if (column == 25) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, column, 26);
    } else {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 26);
    }
    Engine_ObjectEventSetSpriteFlags(pillar, 0);
    pillar->motionFlags = 0;
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
  }
}

/// Frost on a puddle freezes it into an ice pillar the party can stand on:
/// the puddle's cell takes the pillar's attributes, kept elsewhere on the
/// map, and the pillar is drawn 32 pixels lower, under the party. Each
/// room's puddle has its own copy, setting its own scene flag.
static void AltinPeak_FreezeFirstWaterBeastRoomPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(7, 13, 1, 1, 9, 13);
  if (puddle != 0) {
    Engine_ObjectEventSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

/// Frost on AltinPeakFirstWaterBeast's second puddle, three cells across: the
/// cells take the ice's attributes and the ice is left to fall to the ground.
static void AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(29, 1, 3, 1, 17, 13);
  if (puddle != 0) {
    puddle->motionFlags = OBJ_EVENT_FALLS;
  }
  Engine_GameFlagSet(FLAG_SECOND_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's first puddle.
static void AltinPeak_FreezeSpritzRoomPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 26, 26);
  if (puddle != 0) {
    Engine_ObjectEventSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

/// Frost on AltinPeakThirdWaterBeast's puddle.
static void AltinPeak_FreezeThirdWaterBeastRoomPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_THIRD_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(23, 13, 1, 1, 25, 13);
  if (puddle != 0) {
    Engine_ObjectEventSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

/// Frost on AltinPeakPool's puddle.
static void AltinPeak_FreezePoolRoomPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_POOL_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(45, 41, 1, 1, 43, 41);
  if (puddle != 0) {
    Engine_ObjectEventSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's second puddle.
static void AltinPeak_FreezeSpritzRoomSecondPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 17, 10);
  if (puddle != 0) {
    Engine_ObjectEventSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_SECOND_PUDDLE_FROZEN);
}

/// Frost on AltinPeakSpritz's third puddle.
static void AltinPeak_FreezeSpritzRoomThirdPuddle(void) {
  struct ObjectEvent *puddle;

  puddle = ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_THIRD_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 26, 15);
  if (puddle != 0) {
    Engine_ObjectEventSetSpriteFlags(puddle, 0);
    puddle->y.fixed -= PIXELS(32);
    puddle->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
  }
  Engine_GameFlagSet(FLAG_THIRD_PUDDLE_FROZEN);
}

/// Interrupt handler while the water layer vanishes: each time, background 1
/// (the water) stays on only when a random percentage reaches the timer, so
/// as the timer climbs to 100 the water flickers away.
static void Water_FlickerOut(void) {
  volatile u16 *reg = &REG_DISPCNT;
  s16 v = *reg & 0xfdff;

  if ((u32)(Engine_RandomNext() * 100) >> 16 >= gAltinPeak_FlickerTimer) {
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
  struct ObjectEvent *puddle;
  u8 *gridWork;
  u8 *waterLayer;
  s32 timer;

  gridWork = gMapGridWork[0];
  Script_PlayCue(SE_230);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = gridWork + 356;
  for (step = 0; step <= 23; step++) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    Engine_TaskWait(4);
    if (step == 8) {
      puddle = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE);
      puddle->scaleX = Q16_ONE / 10;
      puddle = ObjectEvent_Get(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE);
      puddle->scaleY = Q16_ONE / 10;
      ObjectEvent_SetPosition(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE,
                              PIXELS(152), PIXELS(216));
      ObjectEvent_SetActionScript(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE,
                                  gAltinPeak_PuddleGrowScript);
    }
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = gAltinPeak_FlickerTimer + 1;
    gAltinPeak_FlickerTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_ScriptWait(30);
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
  Script_PlayCue(SE_230);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = gridWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    Engine_TaskWait(4);
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = gAltinPeak_FlickerTimer + 1;
    gAltinPeak_FlickerTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_ScriptWait(30);
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
  Script_PlayCue(SE_230);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(10);
  // Sink the water layer one pixel every 4 frames, 24 in all.
  waterLayer = gridWork + 356;
  for (step = 23; step >= 0; step--) {
    *(s32 *)(waterLayer + 12) -= PIXELS(1);
    Engine_TaskWait(4);
  }
  // Flicker the water away while the timer counts to 100.
  Runtime_SetIrqHandler(1, 0, Water_FlickerOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    Engine_TaskWait(1);
    timer = gAltinPeak_FlickerTimer + 1;
    gAltinPeak_FlickerTimer = timer;
  } while ((u32)(timer << 16) <= 0x640000);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_ScriptWait(30);
  MapGrid_CopyCellsTo(103, 14, 103, 17, 4, 3);
  GameFlag_Set(FLAG_ALTIN_MINE_THIRD_DRAIN);
}

/// Flips AltinPeakSecondWaterBeast's switch to its other position, redrawing it
/// and changing the cell its position opens or closes.
static void AltinPeak_ToggleSecondWaterBeastRoomSwitchCells(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH) != 0) {
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
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(SE_125);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  Engine_TaskWait(20);
  FieldEffect_PlayQueuedSound();
  Engine_ScriptEnd();
}

/// Flips AltinPeakThirdWaterBeast's switch, the same way.
static void AltinPeak_ToggleThirdWaterBeastRoomSwitchCells(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH) != 0) {
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
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(SE_125);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  Engine_TaskWait(20);
  FieldEffect_PlayQueuedSound();
  Engine_ScriptEnd();
}

/// Searching AltinPeakPool's switch flips it.
static void AltinPeak_FlipPoolRoomSwitch(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Script_PlayCue(SE_125);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(15, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(47, 29, 16, 28, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(17, 93, 1, 1, 16, 92);
    MapGrid_CopyCellsTo(46, 29, 16, 28, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
  }
  Engine_TaskWait(20);
  FieldEffect_PlayQueuedSound();
  Engine_ScriptEnd();
}

/// Searching AltinPeakSpritz's switch flips it.
static void AltinPeak_FlipSpritzRoomSwitch(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Script_PlayCue(SE_125);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(28, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(47, 28, 29, 17, 1, 2);
    GameFlag_Clear(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
  } else {
    MapGrid_CopyCellAttributes(30, 82, 1, 1, 29, 81);
    MapGrid_CopyCellsTo(46, 28, 29, 17, 1, 2);
    GameFlag_Set(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
  }
  Engine_TaskWait(20);
  FieldEffect_PlayQueuedSound();
  Engine_ScriptEnd();
}

/// Stepping onto AltinPeakSpritz's pillar: it is drawn under the party leader.
static void AltinPeak_PlacePillarUnderLeader(void) {
  ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_PILLAR)->priorityFlags =
      OBJ_EVENT_PRIORITY_AUTOMATIC | OBJ_EVENT_PRIORITY_UNDERFOOT;
}

/// Stepping off AltinPeakSpritz's pillar: it sorts against the leader again.
static void AltinPeak_RestorePillarAutomaticPriority(void) {
  ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_PILLAR)->priorityFlags =
      OBJ_EVENT_PRIORITY_AUTOMATIC;
}

/// Walking east into AltinPeakSpritz's pillar pushes it one cell along with a
/// scrape; the leader follows it and its cells are settled as after Move.
static void AltinPeak_PushSpritzRoomPillar(void) {
  Engine_ScriptStart();
  MapGrid_CopyCellAttributes(24, 27, 2, 1, 24, 26);
  Script_PlayCue(SE_PUSH);
  ObjectEvent_SetSpeed(LOCALID_SPRITZ_ROOM_PILLAR, PIXELS(1) / 5,
                       PIXELS(1) / 10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 5, PIXELS(1) / 10);
  ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_PILLAR)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_PUSH);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 400, 424);
  ObjectEvent_SetDestination(LOCALID_SPRITZ_ROOM_PILLAR, 408, 424);
  Engine_ObjectEventWaitForMove(LOCALID_SPRITZ_ROOM_PILLAR);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  AltinPeak_SettleSpritzRoomPillar();
  Engine_ScriptEnd();
}

/// Force on AltinPeakFallingRocks's wall: it rumbles, the log by it rocks one
/// way and back, then topples with a crash, and the rockfall follows.
static void AltinPeak_ForceLogOver(void) {
  struct ObjectEvent *objectEvent;
  struct AnimationObject *sprite;
  s32 originX;
  s32 originHeight;
  s32 cosine;
  s32 sine;
  u16 rotationStep;

  objectEvent = ObjectEvent_Get(LOCALID_STANDING_LOG);
  sprite = objectEvent->sprite;
  originX = objectEvent->x.fixed;
  originHeight = objectEvent->y.fixed;
  Script_Start();
  Script_PlayCue(SE_141);
  MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
  Script_Wait(10);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Script_Wait(20);

  // Rock the obstacle to each side before the accelerating fall.
  rotationStep = 0;
  while (1) {
    rotationStep += 8;
    sprite->rotation += rotationStep;
    cosine = Math_Cos(sprite->rotation + 0x4000);
    objectEvent->x.fixed = (cosine << 4) + originX;
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
    objectEvent->x.fixed = (cosine << 4) + originX;
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
    objectEvent->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation > 0x8000) {
      objectEvent->y.fixed = originHeight - (sine << 3);
    }
    if (sprite->rotation + rotationStep > 0xbfff) {
      break;
    }
    Task_Wait(1);
  }
  Task_Wait(1);
  sprite->rotation = 0xc000;
  Script_PlayCue(SE_183);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Script_Wait(20);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  AltinPeak_RunBoulderChase(5);
  Script_End();
}

/// AltinPeakFallingRocks's rockfall. The party leader steps back and looks up
/// as the slope rumbles; the log lies toppled and a boulder drops onto the
/// path. The leader flees down the winding path, and after boulderDelay frames
/// the boulder rolls after; it bounces to a stop at the bottom and crashes into
/// the wall, throwing up rock debris and leaving rubble. Garet, if he came out
/// at the sign, rejoins the leader. Remembered by
/// FLAG_ALTIN_PEAK_BOULDER_FALLEN.
static void AltinPeak_RunBoulderChase(s32 boulderDelay) {
  struct ObjectEvent *leader;
  struct ObjectEvent *boulder;
  struct ObjectEvent *shadow;
  struct ObjectEvent *rock;
  struct ObjectEvent *objectEvent;

  leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  boulder = ObjectEvent_Get(LOCALID_FALLING_BOULDER);
  shadow = ObjectEvent_Get(LOCALID_BOULDER_SHADOW);
  rock = ObjectEvent_Get(LOCALID_STANDING_LOG);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(784), -1, PIXELS(116), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_SPRING);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 792, 140);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 100);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  Script_PlayCue(SE_183);
  MapGrid_SetShake(PIXELS(3), PIXELS(3), Q16_ONE);
  Engine_ScriptWait(20);
  // Stage the rolling object and its follower for the escape route.
  shadow->scaleX = 6 * Q16_ONE / 5;
  shadow->scaleY = 6 * Q16_ONE / 5;
  shadow->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  shadow->update = AltinPeak_ShadowFollowBoulder;
#else
  shadow->update = (void (*)(union FieldObject *))AltinPeak_ShadowFollowBoulder;
#endif
  Engine_ObjectEventSetAnimation(LOCALID_FALLING_BOULDER, 4);
  boulder->bounce = PIXELS(1) / 2;
  boulder->x.fixed = PIXELS(786);
  boulder->y.fixed = PIXELS(32);
  boulder->z.fixed = PIXELS(90);
  boulder->scaleX = 2 * Q16_ONE;
  boulder->scaleY = 2 * Q16_ONE;
  Engine_ScriptWait(10);
  Script_PlayCue(SE_183);
  MapGrid_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
  Engine_ScriptWait(20);
  rock->x.fixed += PIXELS(14);
  rock->y.fixed += -PIXELS(8);
  rock->sprite->rotation = 0xc000;
  Script_PlayCue(SE_107);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Script_PlayCue(MUS_055);
  MapGrid_SetShake(PIXELS(1), PIXELS(3), Q16_ONE);
  Engine_ObjectEventSetSpritePriority(LOCALID_FALLING_BOULDER, 0);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 0);
  ObjectEvent_SetAttachedEffect(
      LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  // Send the leader first, then release the rolling object after its delay.
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  leader->timer = 0;
  ObjectEvent_SetActionScript(LOCALID_PARTY_LEADER, gAltinPeak_LeaderFleePath);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    ObjectEvent_SetPosition(GARET, PIXELS(878), PIXELS(528));
    objectEvent = ObjectEvent_Get(GARET);
    objectEvent->facing = FACING_SOUTH + FACING_STEP;
  }
  Camera_SetSpeed(PIXELS(5) / 4, PIXELS(5) / 32);
  Camera_MoveTo(PIXELS(786), -1, PIXELS(556), 1);
  Engine_ScriptWait(boulderDelay);
  Engine_ObjectEventSetSpritePriority(LOCALID_FALLING_BOULDER, 1);
  ObjectEvent_SetSpeed(LOCALID_FALLING_BOULDER, PIXELS(317) / 200,
                       PIXELS(275) / 347);
  boulder->timer = 0;
  ObjectEvent_SetActionScript(LOCALID_FALLING_BOULDER, gAltinPeak_BoulderPath);
  do {
    Engine_TaskWait(1);
  } while (leader->timer == 0);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, 0);
  do {
    Engine_TaskWait(1);
  } while (boulder->timer == 0);
  Engine_ObjectEventSetSpritePriority(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  shadow->x.fixed = PIXELS(786);
  shadow->update = NULL;
  shadow->z.fixed = PIXELS(618);
  shadow->y.fixed = -PIXELS(64);
  // Let the rolling object bounce before the final collapse.
  ObjectEvent_SetSpeed(LOCALID_FALLING_BOULDER, PIXELS(8) / 5, PIXELS(4) / 5);
  boulder->bounce = PIXELS(1) / 10;
  boulder->gravity = PIXELS(1) / 5;
  boulder->velocityY = PIXELS(4);
  ObjectEvent_MoveToAndWait(LOCALID_FALLING_BOULDER, 786, 604);
  ObjectEvent_SetSpeed(LOCALID_FALLING_BOULDER, PIXELS(16) / 5, PIXELS(8) / 5);
  ObjectEvent_SetDestination(LOCALID_FALLING_BOULDER, 786, 644);
  Engine_ScriptWait(15);
  MapGrid_SetShake(PIXELS(5), PIXELS(7), Q16_ONE);
  MapGrid_CopyCellsTo(25, 36, 43, 36, 11, 9);
  MapGrid_CopyCellAttributes(25, 35, 10, 5, 43, 35);
  ObjectEvent_SetPosition(LOCALID_FALLING_BOULDER, 0, 0);
  ObjectEvent_SetPosition(LOCALID_BOULDER_SHADOW, 0, 0);
  // Hide the rolling actors and animate the falling debris.
  FieldScheduler_AddCallback(AltinPeak_SpawnDebris, TASK_PRIORITY_MAP);
  Engine_ScriptWait(80);
  Engine_TaskRemoveCallback(AltinPeak_SpawnDebris);
  Engine_ScriptWait(60);
  Script_PlayCue(AUDIO_CUE_FADE_BGM);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // Return Garet to the party if the warning sign brought him into the scene.
  Engine_ScriptWait(120);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkTo(GARET, 824, 558);
  }
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 854, 584);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 0);
  }
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 40);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Camera_MoveTo(PIXELS(788), -PIXELS(64), PIXELS(610), 1);
  Engine_CameraWaitForMove();
  Script_PlayCue(SE_148);
  Engine_ScriptWait(240);
  if (GameFlag_IsSet(FLAG_GARET_WATCHING_ROCKS) != 0) {
    Camera_SetSpeed(PIXELS(4), PIXELS(1) / 2);
    Camera_MoveTo(PIXELS(854), 0, PIXELS(584), 1);
    Engine_CameraWaitForMove();
    ObjectEvent_WalkToAndWait(GARET, 840, 552);
    ObjectEvent_WalkToAndWait(GARET, 854, 562);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
    objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (objectEvent != NULL) {
      ObjectEvent_SetDestination(GARET, objectEvent->x.part.pixel,
                                 objectEvent->z.part.pixel);
    }
    Engine_ObjectEventWaitForMove(GARET);
    ObjectEvent_SetPosition(GARET, 0, 0);
  }
  Audio_PlayCueFromEventWork();
  GameFlag_Set(FLAG_ALTIN_PEAK_BOULDER_FALLEN);
}

/// Searching AltinPeakFallingRocks's sign, which can bring the rockfall. `Watch
/// for falling rocks! Do not strike the wall! Rocks may fall!` Before the
/// rockfall, while the party has no Orb of Force, Garet steps out, grumbles at
/// the dead end and strikes the wall; the ground shakes, he tells the leader to
/// run and runs down the path, and the rockfall follows.
static void AltinPeak_ReadFallingRockSign(void) {
  struct ObjectEvent *objectEvent;
  s32 message;

  Engine_ScriptStart();
  message = MsgAltinWatchFallingRocks;
  Engine_MessageShowCentered(message, 1);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_BOULDER_FALLEN) == 0 &&
      GameFlag_IsSet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) == 0) {
    // Bring Garet out to inspect the warning and demonstrate the danger.
    GameFlag_Set(FLAG_GARET_WATCHING_ROCKS);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 790, 140);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 780, 140);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      ObjectEvent_SetPosition(GARET, objectEvent->x.fixed,
                              objectEvent->z.fixed);
    }
    ObjectEvent_SetSpeed(GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkToAndWait(GARET, 800, 140);
    ObjectEvent_FaceDirection(GARET, FACING_NORTH, 20);
    Engine_ScriptSetMessage(message + 1);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_SHAKE_HEAD);
    Engine_ScriptWait(20);
    Script_ShowMessageAndWait(GARET, 0, 10);
    Engine_ObjectEventJump(GARET, 6, 0);
    ObjectEvent_SetSpeed(GARET, PIXELS(8) / 5, PIXELS(4) / 5);
    ObjectEvent_GetById(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_WalkToAndWait(GARET, 792, 110);
    Engine_ScriptWait(1);
    ObjectEvent_GetById(GARET)->steeringFlags |= OBJ_EVENT_FACES_MOTION;
    Script_PlayCue(SE_161);
    MapGrid_SetShake(PIXELS(2), PIXELS(1), Q16_ONE);
    ObjectEvent_GetById(GARET)->steeringFlags &= ~OBJ_EVENT_FACES_MOTION;
    ObjectEvent_WalkToAndWait(GARET, 792, 120);
    Engine_ScriptWait(1);
    {
      struct ObjectEvent *objectEvent = ObjectEvent_Get(GARET);

      objectEvent->steeringFlags =
          OBJ_EVENT_FACES_MOTION | objectEvent->steeringFlags;
    }
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Engine_ScriptWait(80);
    Script_PlayCue(SE_141);
    MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    Engine_ScriptWait(40);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                          0);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
    ObjectEvent_FaceDirection(GARET, 0, 20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, 0, 0);
    ObjectEvent_FaceDirection(GARET, FACING_WEST, 40);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
    ObjectEvent_FaceDirection(GARET, 0, 40);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(GARET, FACING_NORTH, 40);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    ObjectEvent_FaceDirection(GARET, FACING_SOUTH, 20);
    Engine_ObjectEventStartRepeatedMotion(GARET, 2);
    Script_ShowMessageAndWait(GARET, 0, 10);
    ObjectEvent_SetSpeed(GARET, PIXELS(5) / 2, PIXELS(5) / 4);
    Engine_ObjectEventSetAnimation(GARET, 5);
    ObjectEvent_MoveToAndWait(GARET, 796, 138);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, 0, 0);
    ObjectEvent_MoveToAndWait(GARET, 804, 140);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectEvent_MoveToAndWait(GARET, 804, 166);
    ObjectEvent_MoveToAndWait(GARET, 764, 166);
    ObjectEvent_MoveToAndWait(GARET, 764, 198);
    ObjectEvent_MoveToAndWait(GARET, 786, 198);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_MoveToAndWait(GARET, 786, 246);
    Engine_ObjectEventSetAnimation(GARET, ANIMATION_STAND);
    ObjectEvent_SetPosition(GARET, 0, 0);
    Engine_ScriptWait(40);
    AltinPeak_RunBoulderChase(10);
  }
  Engine_ScriptEnd();
}

/// AltinPeakFirstWaterBeast has its own events once the first drain has lowered
/// its water.
const struct MapEvent *Map_GetEvents(void) {
  const struct MapEvent *table;

  if (gGameState.mapId == (s32)&MapId_AltinPeakFirstWaterBeast) {
    if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
      table = gAltinPeak_FirstWaterBeastEventsDrained;
    } else {
      table = gAltinPeak_FirstWaterBeastEvents;
    }
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakRailTunnel) {
    table = gAltinPeak_RailTunnelEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakSecondWaterBeast) {
    table = gAltinPeak_SecondWaterBeastEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakSouthEntrance) {
    table = gAltinPeak_SouthEntranceEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakThirdWaterBeast) {
    table = gAltinPeak_ThirdWaterBeastEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakPool) {
    table = gAltinPeak_PoolEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakSpritz) {
    table = gAltinPeak_SpritzEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakJunction) {
    table = gAltinPeak_JunctionEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakFallingRocks) {
    table = gAltinPeak_FallingRocksEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakHydrosStatue) {
    table = gAltinPeak_HydrosStatueEvents;
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakWestExit) {
    table = gAltinPeak_WestExitEvents;
  } else {
    table = gAltinPeak_EventsOther;
  }
  return table;
}

/// Update callback of the hopping water beast: once a second it leaps up and
/// lands with a splash (SE_SPLASH) that shakes the screen briefly.
static void AltinPeak_UpdateBeastHops(union FieldObject *fieldObject) {
  struct ObjectEvent *beast = &fieldObject->objectEvent;
  s16 *shakeDelay = (s16 *)&beast->work;

  // The route owns timer (hop phase), work (shake delay) and the effect's
  // countdown (landing-sound latch) until it removes this hook.
  if (*shakeDelay != 0) {
    if (--*shakeDelay == 1)
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  }
  if (beast->velocityY == 0) {
    ObjectEvent_SetMode(&fieldObject->objectEvent, 1);
    beast->y.fixed -= PIXELS(3) / 2;
    if (beast->y.fixed < beast->terrainHeight) {
      if (fieldObject->effect.countdown != 0) {
        Engine_AudioPlayCue(SE_SPLASH);
        fieldObject->effect.countdown = 0;
        beast->work = 4;
        MapGrid_SetShake(PIXELS(1), 0, Q16_ONE);
      }
      beast->y.fixed = beast->terrainHeight;
    }
    beast->paused = 1;
  } else {
    beast->paused = 0;
  }
  if (beast->timer == 0) {
    Engine_AudioPlayCue(SE_JUMP);
    fieldObject->effect.countdown = 1;
    ObjectEvent_SetMode(&fieldObject->objectEvent, 2);
    beast->velocityY = PIXELS(3);
  }
  if (++beast->timer == 60)
    beast->timer = 0;
}

/// The beast lands with a splash, shaking the screen for holdFrames.
static __inline__ void AltinPeak_PlayLandingShake(s32 amplitude,
                                                  s32 holdFrames) {
  Engine_AudioPlayCue(SE_SPLASH);
  MapGrid_SetShake(amplitude, 0, Q16_ONE);
  Engine_ScriptWait(holdFrames);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
}

/// Stepping near AltinPeakFirstWaterBeast's water beast the first time: the
/// camera finds it, it cries out and hops away around the room, freezes the
/// second puddle with Frost and hops across it, then stops to watch the party
/// leader.
static void AltinPeak_ShowFirstBeastRoute(void) {
  union FieldObject *beast;
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  s32 jumpSpeed;

  beast = (union FieldObject *)ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  Engine_ScriptStart();
  Engine_CameraSetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(298), -1, PIXELS(337), 1);
  Engine_CameraWaitForMove();
  Engine_AudioPlayCue(SE_147);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_WATER_BEAST, 2);
  Engine_ScriptWait(40);
  Engine_CameraSetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Engine_CameraMoveTo(PIXELS(295), PIXELS(32), PIXELS(212), 1);
  // Run the timed hopping callback while the beast moves along its route.
  beast->effect.countdown = 0;
  beast->objectEvent.timer = 0;
  beast->objectEvent.work = 0;
  beast->objectEvent.gravity = PIXELS(2) / 5;
  beast->objectEvent.update = AltinPeak_UpdateBeastHops;
  ObjectEvent_SetSpeed(LOCALID_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 308, 291);
  Engine_ObjectEventMoveToAndWait(LOCALID_FIRST_WATER_BEAST, 311, 215);
  beast->objectEvent.update = 0;
  beast->objectEvent.paused = 0;
  Engine_ScriptWait(16);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_NORTHWEST + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST, FACING_WEST, 40);
  // Demonstrate Frost between the hopping sections.
  AltinPeak_BeastFreezesSecondPuddle();
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_SOUTHEAST + FACING_STEP, 40);
  Camera_MoveTo(PIXELS(334), -1, PIXELS(244), 1);
  beast->effect.countdown = 0;
  beast->objectEvent.timer = 0;
  beast->objectEvent.work = 0;
  beast->objectEvent.update = AltinPeak_UpdateBeastHops;
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 320, 232);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 340, 262);
  Engine_ObjectEventMoveToAndWait(LOCALID_FIRST_WATER_BEAST, 374, 262);
  beast->objectEvent.update = 0;
  Engine_ScriptWait(16);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_NORTHEAST + FACING_STEP, 20);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_WATER_BEAST,
                                  FACING_NORTH + FACING_STEP, 40);
  Engine_AudioPlayCue(SE_HIGH_JUMP);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  objectEvent->velocityY = PIXELS(4);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST,
                                 WATER_BEAST_ANIMATION_LEAP);
  Engine_ObjectEventMoveToAndWait(LOCALID_FIRST_WATER_BEAST, 380, 248);
  Engine_ScriptWait(10);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(6);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_NORTHWEST + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST, FACING_WEST, 40);
  Camera_MoveTo(PIXELS(304), -1, PIXELS(215), 1);
  beast->effect.countdown = 0;
  beast->objectEvent.timer = 0;
  beast->objectEvent.work = 0;
  beast->objectEvent.update = AltinPeak_UpdateBeastHops;
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 329, 219);
  beast->objectEvent.update = 0;
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  Engine_ScriptWait(16);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(40);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_WATER_BEAST, FACING_WEST, 40);
  ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)
      ->motionFlags = 0;
  // Open the route for the final two beast jumps.
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 17, 13);
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 18, 13);
  MapGrid_CopyCellAttributes(3, 0, 1, 1, 19, 13);
  Engine_ObjectEventSetSpeed(LOCALID_FIRST_WATER_BEAST, PIXELS(7) / 5,
                             PIXELS(7) / 10);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  Engine_AudioPlayCue(SE_HIGH_JUMP);
  jumpSpeed = PIXELS(5);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  objectEvent->velocityY = jumpSpeed;
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST,
                                 WATER_BEAST_ANIMATION_SPIT);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 295, 215);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 1);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(40);
  Engine_AudioPlayCue(SE_HIGH_JUMP);
  objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  objectEvent->velocityY = jumpSpeed;
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST,
                                 WATER_BEAST_ANIMATION_SPIT);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 260, 215);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_SOUTH + FACING_STEP, 20);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_WATER_BEAST,
                                  FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_AudioPlayCue(SE_147);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_WATER_BEAST, 2);
  Engine_ScriptWait(40);
  MapGrid_CopyCellAttributes(4, 0, 1, 1, 17, 13);
  MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
  MapGrid_CopyCellAttributes(4, 0, 1, 1, 19, 13);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_CameraSetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Engine_CameraMoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  Engine_CameraWaitForMove();
  FieldObject_SetTargetAndCallback(LOCALID_FIRST_WATER_BEAST,
                                   OBJ_EVENT_LINK_KEEP_MOTION |
                                       LOCALID_PARTY_LEADER,
                                   gAltinPeak_FaceLeaderScript);
  Engine_GameFlagSet(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN);
  Engine_ScriptEnd();
}

/// Stepping near the beast again: it hops back over the ice and settles down,
/// spitting water.
static void AltinPeak_ShowSecondBeastRoute(void) {
  union FieldObject *objectEvent;
  struct ObjectEvent *movingObjectEvent;
  u32 waitedFrames;

  objectEvent =
      (union FieldObject *)ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  Engine_ScriptStart();
  ObjectMotion_EnableActionAndResetMotion(LOCALID_FIRST_WATER_BEAST);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(279), PIXELS(64), PIXELS(216), 1);
  Engine_CameraWaitForMove();
  Engine_AudioPlayCue(SE_147);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_WATER_BEAST, 2);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_SOUTH + FACING_STEP, 20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST, FACING_WEST, 40);
  Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  Camera_MoveTo(PIXELS(128), PIXELS(64), PIXELS(202), 1);
  objectEvent->effect.countdown = 0;
  objectEvent->objectEvent.timer = 0;
  objectEvent->objectEvent.work = 0;
  objectEvent->objectEvent.gravity = PIXELS(2) / 5;
  objectEvent->objectEvent.update = AltinPeak_UpdateBeastHops;
  ObjectEvent_SetSpeed(LOCALID_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 212, 200);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 103, 200);
  objectEvent->objectEvent.update = NULL;
  objectEvent->objectEvent.paused = 0;
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_SOUTH + FACING_STEP, 40);
  ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  ObjectEvent_SetSpeed(LOCALID_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  movingObjectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  Engine_ObjectEventSetSpriteFlags(movingObjectEvent, 0);
  Engine_AudioPlayCue(SE_HIGH_JUMP);
  movingObjectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  movingObjectEvent->velocityY = PIXELS(4);
  ObjectEvent_SetAnimation(LOCALID_FIRST_WATER_BEAST,
                           WATER_BEAST_ANIMATION_SPIT);
  Engine_ObjectEventMoveToAndWait(LOCALID_FIRST_WATER_BEAST, 86, 214);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  movingObjectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
  Engine_ObjectEventSetSpriteFlags(movingObjectEvent, 1);
  Engine_ScriptWait(10);
  AltinPeak_PlayLandingShake(PIXELS(2), 8);
  Engine_ScriptWait(40);
  ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  ObjectEvent_FaceDirection(LOCALID_FIRST_WATER_BEAST,
                            FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_WATER_BEAST, 0, 40);
  objectEvent->effect.countdown = 0;
  objectEvent->objectEvent.timer = 0;
  objectEvent->objectEvent.work = 0;
  objectEvent->objectEvent.update = AltinPeak_UpdateBeastHops;
  ObjectEvent_SetSpeed(LOCALID_FIRST_WATER_BEAST, PIXELS(6) / 5, PIXELS(3) / 5);
  ObjectEvent_MoveToAndWait(LOCALID_FIRST_WATER_BEAST, 120, 215);
  objectEvent->objectEvent.update = NULL;
  objectEvent->objectEvent.paused = 0;
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST, ANIMATION_STAND);
  Engine_ScriptWait(16);
  AltinPeak_PlayLandingShake(PIXELS(1), 4);
  Engine_ScriptWait(40);
  Engine_ObjectEventFaceDirection(LOCALID_FIRST_WATER_BEAST, 0, 10);
  Engine_AudioPlayCue(SE_147);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_FIRST_WATER_BEAST, 2);
  Engine_ScriptWait(80);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST,
                                 WATER_BEAST_ANIMATION_SPIT);
  // End the route with a spray gust and a short skippable wait.
  AltinPeak_StartSpitting(PIXELS(130), 0, PIXELS(168), 0);
  Engine_ScriptWait(60);
  for (waitedFrames = 0; waitedFrames < 60 && gKeysPressed == 0;
       waitedFrames++) {
    Engine_ScriptWait(1);
  }
  objectEvent = (union FieldObject *)ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Camera_SetSpeed(PIXELS(24) / 5, PIXELS(3) / 5);
  Engine_CameraMoveTo(objectEvent->objectEvent.x.fixed,
                      objectEvent->objectEvent.y.fixed,
                      objectEvent->objectEvent.z.fixed, 1);
  Engine_CameraWaitForMove();
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN);
  Engine_ScriptEnd();
}

/// Sets up the room being entered: puzzles already solved and the water
/// levels already lowered. Returns 0.
s32 Map_Initialize(void) {
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.mapId == (s32)&MapId_AltinPeakFirstWaterBeast) {
    AltinPeak_RestoreFirstWaterBeastRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakSecondWaterBeast) {
    AltinPeak_RestoreSecondWaterBeastRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakSouthEntrance) {
    AltinPeak_RestoreSouthEntranceRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakThirdWaterBeast) {
    AltinPeak_RestoreThirdWaterBeastRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakPool) {
    AltinPeak_RestorePoolRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakSpritz) {
    AltinPeak_RestoreSpritzRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakFallingRocks) {
    AltinPeak_RestoreFallingRocksRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakHydrosStatue) {
    AltinPeak_RestoreHydrosStatueRoom();
  } else if (gGameState.mapId == (s32)&MapId_AltinPeakWestExit) {
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
  struct ObjectEvent *objectEvent;
  struct MapGridScrollWork *grid;

  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gGameState.warpDest == WARP_DEST_AFTER_BATTLE) {
      AltinPeak_AwardFrostJewelAndDrain();
    }
  }
  AltinPeak_StartPaletteAnimation();
  if (Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
    Engine_ObjectEventSetPosition(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
  } else {
    objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE);
    if (objectEvent != 0) {
      objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
      Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    }
  }
  objectEvent =
      ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  if (objectEvent != 0) {
    objectEvent->priorityFlags = OBJ_EVENT_PRIORITY_UNDERFOOT;
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  }
  opened = Engine_GameFlagIsSet(FLAG_ALTIN_MINE_FIRST_DRAIN);
  if (opened == 0) {
    Engine_ObjectEventSetChildValue(LOCALID_FIRST_WATER_BEAST,
                                    SPRITE_COLOR_RBG);
    flag = Engine_GameFlagIsSet(FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN);
    if (flag != 0) {
      Engine_ObjectEventSetAnimation(
          LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, 0);
      objectEvent =
          ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
      objectEvent->update = AltinPeak_SparkleIce;
      ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)
          ->motionFlags = opened;
      objectEvent =
          ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
      objectEvent->y.fixed = PIXELS(32);
      MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
      Engine_ObjectEventSetPosition(LOCALID_FIRST_WATER_BEAST, PIXELS(120),
                                    PIXELS(215));
      objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST);
      objectEvent->facing = opened;
      Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST,
                                     WATER_BEAST_ANIMATION_SPIT);
      AltinPeak_StartSpitting(PIXELS(130), 0, PIXELS(168), 0);
      return;
    }
    if (Engine_GameFlagIsSet(FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN) == 0) {
      return;
    }
    Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
                                   0);
    objectEvent =
        ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
    objectEvent->update = AltinPeak_SparkleIce;
    ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)
        ->motionFlags = flag;
    objectEvent =
        ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
    objectEvent->y.fixed = PIXELS(32);
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
    ObjectEvent_SetPosition(LOCALID_FIRST_WATER_BEAST, PIXELS(260),
                            PIXELS(215));
    FieldObject_SetTargetAndCallback(
        10, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
        gAltinPeak_FaceLeaderScript);
  } else {
    grid = gMapGridWork[0];
    Engine_ObjectEventSetPosition(LOCALID_FIRST_WATER_BEAST, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 2, 3, 14);
    grid->flags &= 0xfdff;
    AltinPeak_MarkObjectEventTerrainCell(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE);
    if (Engine_GameFlagIsSet(FLAG_PUDDLE_FROZEN) != 0) {
      Engine_ObjectEventSetAnimation(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE,
                                     PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(7, 13, 1, 1, 9, 13);
      objectEvent = ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE);
      objectEvent->y.fixed = 0;
      objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    }
    AltinPeak_MarkObjectEventTerrainCell(
        LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
    if (Engine_GameFlagIsSet(FLAG_SECOND_PUDDLE_FROZEN) != 0) {
      Engine_ObjectEventSetAnimation(
          LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
          PUDDLE_ANIMATION_FROZEN);
      MapGrid_CopyCellAttributes(29, 1, 3, 1, 17, 13);
      objectEvent =
          ObjectEvent_GetById(LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
      objectEvent->y.fixed = PIXELS(32);
      objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    }
  }
}

/// AltinPeakSecondWaterBeast: after its beast's battle the second drain runs;
/// while the water is up the beast spits it; the switch is restored.
static void AltinPeak_RestoreSecondWaterBeastRoom(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
    ObjectEvent_SetPosition(LOCALID_SECOND_WATER_BEAST, 0, 0);
  } else {
    AltinPeak_StartPaletteAnimation();
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0 &&
        gGameState.warpDest == WARP_DEST_AFTER_BATTLE) {
      AltinPeak_DrainAfterSecondBeast();
    } else {
      MapGrid_CopyCellAttributes(38, 24, 1, 2, 37, 24);
      MapGrid_CopyCellAttributes(44, 23, 1, 2, 45, 23);
      if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) == 0) {
        ObjectEvent_SetChildValue(LOCALID_SECOND_WATER_BEAST, SPRITE_COLOR_RBG);
        Engine_ObjectEventSetAnimation(LOCALID_SECOND_WATER_BEAST,
                                       WATER_BEAST_ANIMATION_SPIT);
        AltinPeak_StartSpitting(PIXELS(238), 0, PIXELS(418), 0x8000);
      }
    }
  }
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH) != 0) {
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 24, 80);
    MapGrid_CopyCellsTo(0, 1, 24, 11, 1, 2);
  } else {
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 24, 80);
    MapGrid_CopyCellsTo(2, 1, 24, 11, 1, 2);
  }
}

/// AltinPeakSouthEntrance: its water is up until the second drain. Arriving
/// from the Alpine Crossing, at
/// ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST or _EAST, shows no place
/// name.
static void AltinPeak_RestoreSouthEntranceRoom(void) {
  if (GameFlag_IsSet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
  } else {
    MapGrid_CopyCellAttributes(52, 42, 1, 1, 53, 42);
  }
  {
    // Either entrance from the Alpine Crossing: _EAST is the number after
    // ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST.
    if ((u32)(((u16)gGameState.warpDest -
               ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST)
              << 16) <= 0x10000) {
      GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    }
  }
}

/// AltinPeakThirdWaterBeast: after its beast's battle the third drain runs;
/// while the water is up the beast spits it; the frozen puddle and the switch
/// are restored.
static void AltinPeak_RestoreThirdWaterBeastRoom(void) {

  if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
    ObjectEvent_SetPosition(LOCALID_THIRD_WATER_BEAST, 0, 0);
  } else {
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gGameState.warpDest == WARP_DEST_AFTER_BATTLE) {
        AltinPeak_DrainAfterThirdBeast();
      }
    }
    AltinPeak_StartPaletteAnimation();
    if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) == 0) {
      ObjectEvent_SetChildValue(LOCALID_THIRD_WATER_BEAST, SPRITE_COLOR_RBG);
      Engine_ObjectEventSetAnimation(LOCALID_THIRD_WATER_BEAST,
                                     WATER_BEAST_ANIMATION_SPIT);
      AltinPeak_StartSpitting(PIXELS(748), PIXELS(8), PIXELS(280), 0x8000);
    }
  }
  AltinPeak_MarkObjectEventTerrainCell(LOCALID_THIRD_WATER_BEAST_ROOM_PUDDLE);
  if (GameFlag_IsSet(FLAG_PUDDLE_FROZEN) != 0) {
    Engine_ObjectEventSetAnimation(LOCALID_THIRD_WATER_BEAST_ROOM_PUDDLE,
                                   PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(23, 13, 1, 1, 25, 13);
    {
      struct ObjectEvent *objectEvent =
          ObjectEvent_Get(LOCALID_THIRD_WATER_BEAST_ROOM_PUDDLE);
      objectEvent->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT | objectEvent->priorityFlags;
    }
  }
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH) != 0) {
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
  struct ObjectEvent *pillar;

  if (gGameState.warpDest == ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST) {
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      ObjectEvent_SetPosition(LOCALID_CART, PIXELS(358), PIXELS(104));
    }
  }
  AltinPeak_MarkObjectEventTerrainCell(LOCALID_POOL_ROOM_PUDDLE);
  if (GameFlag_IsSet(FLAG_PUDDLE_FROZEN) != 0) {
    pillar = ObjectEvent_Get(LOCALID_POOL_ROOM_PUDDLE);
    Engine_ObjectEventSetAnimation(LOCALID_POOL_ROOM_PUDDLE,
                                   PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(45, 41, 1, 1, 43, 41);
    {
      pillar->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT | pillar->priorityFlags;
    }
  }
  if (GameFlag_IsSet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapGridScrollWork *)gMapGridWork[0])->flags &= ~0x200;
  }
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
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
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_Get(LOCALID_SPRITZ_ROOM_PUDDLE);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  AltinPeak_SettleSpritzRoomPillar();
  AltinPeak_MarkObjectEventTerrainCell(LOCALID_SPRITZ_ROOM_PUDDLE);
  if (GameFlag_IsSet(FLAG_PUDDLE_FROZEN) != 0) {
    Engine_ObjectEventSetAnimation(LOCALID_SPRITZ_ROOM_PUDDLE,
                                   PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 26, 26);
    {
      struct ObjectEvent *objectEvent =
          ObjectEvent_Get(LOCALID_SPRITZ_ROOM_PUDDLE);
      objectEvent->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT | objectEvent->priorityFlags;
    }
  }
  AltinPeak_MarkObjectEventTerrainCell(LOCALID_SPRITZ_ROOM_SECOND_PUDDLE);
  if (GameFlag_IsSet(FLAG_SECOND_PUDDLE_FROZEN) != 0) {
    Engine_ObjectEventSetAnimation(LOCALID_SPRITZ_ROOM_SECOND_PUDDLE,
                                   PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 17, 10);
    {
      struct ObjectEvent *objectEvent =
          ObjectEvent_Get(LOCALID_SPRITZ_ROOM_SECOND_PUDDLE);
      objectEvent->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT | objectEvent->priorityFlags;
    }
  }
  AltinPeak_MarkObjectEventTerrainCell(LOCALID_SPRITZ_ROOM_THIRD_PUDDLE);
  if (GameFlag_IsSet(FLAG_THIRD_PUDDLE_FROZEN) != 0) {
    Engine_ObjectEventSetAnimation(LOCALID_SPRITZ_ROOM_THIRD_PUDDLE,
                                   PUDDLE_ANIMATION_FROZEN);
    MapGrid_CopyCellAttributes(1, 0, 1, 1, 26, 15);
    {
      struct ObjectEvent *objectEvent =
          ObjectEvent_Get(LOCALID_SPRITZ_ROOM_THIRD_PUDDLE);
      objectEvent->priorityFlags =
          OBJ_EVENT_PRIORITY_UNDERFOOT | objectEvent->priorityFlags;
    }
  }
  FieldScheduler_AddCallback(AltinPeak_UpdateFrostPillarPriority,
                             TASK_PRIORITY_MAP);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
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
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(LOCALID_STANDING_LOG);
  Engine_ObjectEventSetPosition(LOCALID_FALLING_BOULDER, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_BOULDER_SHADOW, 0, 0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_BOULDER_SHADOW),
                                   0);
  objectEvent->motionFlags = 0;
  objectEvent->scaleX = 9 * Q16_ONE / 10;
  objectEvent->scaleY = 3 * Q16_ONE / 5;
  objectEvent->sprite->rotation = 0x8000;
  ObjectEvent_GetById(LOCALID_FALLING_ROCK_SIGN)->motionFlags = 0;
  ObjectEvent_GetById(LOCALID_FALLING_ROCK_SIGN)->y.fixed = -PIXELS(28);
  if (GameFlag_IsSet(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    objectEvent->x.fixed += PIXELS(14);
    objectEvent->y.fixed += -PIXELS(8);
    objectEvent->sprite->rotation = 0xc000;
  }
  if (Engine_GameFlagIsSet(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    MapGrid_CopyCellsTo(25, 36, 43, 36, 11, 9);
    MapGrid_CopyCellAttributes(25, 35, 10, 5, 43, 35);
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
  }
  if (gGameState.warpDest ==
          ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE &&
      !Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
    Engine_ScriptStart();
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed = -PIXELS(88);
    Engine_CameraMoveTo(PIXELS(792), -PIXELS(88), PIXELS(577), 0);
    Engine_MapGridRedraw();
    Engine_TaskWait(1);
    Engine_ScriptEnd();
  }
}

/// AltinPeakHydrosStatue: the guardian statue stands until the Hydros statues
/// are beaten; after their battle the party talks it over.
static void AltinPeak_RestoreHydrosStatueRoom(void) {
  s32 statuesDefeated;
  struct ObjectEvent *leader;
  struct ObjectEvent *objectEvent;
  s16 warpDest;

  statuesDefeated = Engine_GameFlagIsSet(FLAG_HYDROS_STATUES_DEFEATED);
  if (statuesDefeated != 0) {
    Engine_ObjectEventSetPosition(LOCALID_STATUE, 0, 0);
    Engine_ObjectEventSetPosition(LOCALID_STATUE_SHADOW, 0, 0);
  } else {
    objectEvent = ObjectEvent_GetById(LOCALID_STATUE);
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    Engine_ObjectEventSetSpritePriority(LOCALID_STATUE_SHADOW, 3);
    objectEvent = ObjectEvent_GetById(LOCALID_STATUE_SHADOW);
    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    ObjectEvent_GetById(LOCALID_STATUE_SHADOW)->collisionFlags =
        statuesDefeated;
  }
  warpDest = gGameState.warpDest;
  if (warpDest == ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS ||
      warpDest == 98) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      Engine_ScriptStart();
      leader->y.fixed = PIXELS(16);
      Engine_ScriptEnd();
    }
  } else if (warpDest == WARP_DEST_AFTER_BATTLE) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      AltinPeak_CelebrateHydrosVictory();
    }
  }
}

/// AltinPeakWestExit: arriving by ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM
/// the pillar stands at its pushed place.
static void AltinPeak_RestoreWestExitRoom(void) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_Get(LOCALID_WEST_EXIT_ROOM_PILLAR);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  if (gGameState.warpDest == ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM) {
    ObjectEvent_SetPosition(LOCALID_WEST_EXIT_ROOM_PILLAR, PIXELS(184),
                            PIXELS(328));
  }
}

/// Gives a new spray particle its look: standing still, on sprite priority 1
/// and at 0.3 scale.
static void AltinPeak_InitializeSpray(struct ObjectEvent *spray) {
  spray->motionFlags = 0;
  spray->sprite->image.priority = 1;
  ObjectGroup_SetChildValue(spray, SPRITE_COLOR_BGR);
  Engine_ObjectEventSetSpriteFlags(spray, 0);
  spray->scaleX = 3 * Q16_ONE / 10;
  spray->scaleY = 3 * Q16_ONE / 10;
}

/// Update callback of a spray particle: it grows while drifting away from the
/// beast, then curls and flattens when its movement timer runs low. The timer
/// can start another curl; the separate lifetime still ends after 20 updates.
static void AltinPeak_DriftSpray(union FieldObject *fieldObject) {
  struct ObjectEvent *spray = &fieldObject->objectEvent;

  switch (gAltinPeak_SprayDirection) {
  case FACING_EAST:
    spray->x.fixed +=
        (spray->timer << 12) +
        ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
    break;
  case FACING_WEST:
    spray->x.fixed -=
        (spray->timer << 12) +
        ((s32)((((u32)(Engine_RandomNext() << 1) >> 16) - 1) << 16) >> 1);
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
  if ((u32)(spray->timer * Engine_RandomNext()) >> 16 == 0)
    ObjectGroup_SetChildValue(spray, SPRITE_COLOR_WHITE);
  if (spray->timer != 0)
    spray->timer -= 2;
  else
    spray->timer = ((u32)(Engine_RandomNext() * 5) >> 16) * 2 + 2;
  if (--fieldObject->effect.countdown == 0)
    Engine_ObjectDispatchRelease(spray);
}

/// Scheduler task while a beast spits: every fourth frame a spray particle
/// starts from its spout, living 20 frames.
static void AltinPeak_SpawnSpray(void) {
  union FieldObject *spray;

  if ((gFrameCount & 3) != 0)
    return;
  spray = (union FieldObject *)Engine_ObjectCreate(
      SPRITE_DUST_PUFF, gAltinPeak_SprayOrigin[0], gAltinPeak_SprayOrigin[1],
      gAltinPeak_SprayOrigin[2]);
  if (spray == NULL)
    return;
  spray->objectEvent.timer = 30;
  spray->objectEvent.work = 1;
  spray->effect.countdown = 20;
  AltinPeak_InitializeSpray(&spray->objectEvent);
  spray->effect.update = AltinPeak_DriftSpray;
  ObjectEvent_SetMode(&spray->objectEvent, 1);
}

/// A water beast starts spitting water from (x, y, z) toward facing (east, 0,
/// or west, 0x8000), with its sound (SE_170).
static void AltinPeak_StartSpitting(s32 x, s32 y, s32 z, s32 facing) {
  SET_VECTOR(gAltinPeak_SprayOrigin, x, y, z);
  gAltinPeak_SprayDirection = facing;

  FieldEffect_SetQueuedSoundAndPlay(SE_170);
  FieldScheduler_AddCallback(AltinPeak_SpawnSpray, TASK_PRIORITY_MAP);
}

/// Scheduler task while the boulder crashes: every third frame a piece of rock
/// (the boulder's own sprite, small) flies out at random, with a clatter
/// (SE_144 or SE_145) every ninth frame.
static void AltinPeak_SpawnDebris(void) {
  struct ObjectEvent *debris;
  s32 scale;

  if (gFrameCount % 3 == 0) {
    debris = ObjectEvent_Create(
        SPRITE_ROCK,
        (((u32)(Engine_RandomNext() * 48) >> 16) << 16) + PIXELS(765),
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
      debris->frozen = 1;
      Engine_ObjectEventSetSpriteFlags(debris, 0);
      debris->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
      debris->sprite->image.priority = 1;
      ObjectEvent_SetMode(debris, 1);
      Engine_ObjectSetScript(debris, gAltinPeak_DebrisScript);
      SET_VELOCITY(debris, (((u32)(Engine_RandomNext() * 6) >> 16) - 3) << 16,
                   PIXELS(8),
                   ((u32)(Engine_RandomNext() * 0x600) >> 16) - 0x300);
    }
  }
}

/// Update callback of the frozen second puddle: its ice glints every eight
/// frames.
static void AltinPeak_SparkleIce(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;

  s32 framePhase = gFrameCount & 7;

  if (framePhase == 0) {
    ObjectGroup_SetChildValue(objectEvent, SPRITE_COLOR_RBG);
  } else if (framePhase == 2) {
    ObjectGroup_SetChildValue(objectEvent, SPRITE_COLOR_NORMAL);
  }
}

/// AltinPeakFirstWaterBeast's water beast casts Frost on the second puddle.
static void AltinPeak_BeastFreezesSecondPuddle(void) {
  Engine_PsynergyStart(ABILITY_FROST, 1);
  Engine_PsynergySetTarget(LOCALID_FIRST_WATER_BEAST,
                           LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  Engine_PsynergyRaiseHands();
  Engine_ObjectEventSetChildValue(LOCALID_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  Engine_PsynergyPlayEffect(1);
  Engine_ObjectEventSetChildValue(LOCALID_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  Engine_PsynergyLowerHands();
  Engine_ObjectEventSetChildValue(LOCALID_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
  FieldEffect_End();
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_ObjectEventSetChildValue(LOCALID_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
}

/// Back from AltinPeakFirstWaterBeast's beast battle: the beast is gone and the
/// party finds the Frost Jewel; the camera pans to the water and the first
/// drain runs.
static void AltinPeak_AwardFrostJewelAndDrain(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_FIRST_WATER_BEAST, 0, 0);
  ObjectEvent_SetPosition(LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  Engine_ItemShowFound(ITEM_FROST_JEWEL, 3);
  Engine_PartyGiveItem(ITEM_FROST_JEWEL, 0);
  Camera_GetScriptObject()->motionFlags = 0;
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(100), 0, PIXELS(249), 1);
  Engine_CameraWaitForMove();
  Engine_ScriptWait(20);
  AltinPeak_DrainFirstWater();
  Camera_MoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  Engine_CameraWaitForMove();
  FieldScript_LoadAreaScript(gAltinPeak_FirstWaterBeastEventsDrained);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Engine_ScriptEnd();
}

/// Back from AltinPeakSecondWaterBeast's beast battle: the beast is gone, the
/// mine cart waits at (328, 424), the switch is back in its first position, and
/// the second drain runs.
static void AltinPeak_DrainAfterSecondBeast(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetPosition(LOCALID_SECOND_WATER_BEAST, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_CART, PIXELS(328), PIXELS(424));
  Engine_GameFlagSet(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  AltinPeak_DrainSecondWater();
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Engine_ScriptEnd();
}

/// Back from AltinPeakThirdWaterBeast's beast battle, the same way, with the
/// third drain.
static void AltinPeak_DrainAfterThirdBeast(void) {
  Engine_ScriptStart();
  Engine_ObjectEventSetPosition(LOCALID_THIRD_WATER_BEAST, 0, 0);
  Engine_ObjectEventSetPosition(LOCALID_CART, PIXELS(488), PIXELS(138));
  Engine_GameFlagSet(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  AltinPeak_DrainThirdWater();
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Engine_ScriptEnd();
}

/// Scheduler task while the statue wakes: a rumble (SE_183) every second.
static void AltinPeak_PlayHydrosRumble(void) {
  gAltinPeak_RumbleTicks++;
  if (gAltinPeak_RumbleTicks == 60) {
    Engine_AudioPlayCue(SE_183);
    gAltinPeak_RumbleTicks = 0;
  }
}

/// Stepping toward AltinPeakHydrosStatue's guardian statue: the leader walks up
/// to it, it rumbles and slides forward, and the battle with the Hydros statues
/// starts.
static void AltinPeak_StartHydrosBattle(void) {
  Engine_ScriptStart();
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(328), -1, PIXELS(87), 1);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 328, 116);
  Script_PlayCue(SE_148);
  FieldScheduler_AddCallback(AltinPeak_PlayHydrosRumble, TASK_PRIORITY_MAP);
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  ObjectEvent_SetSpeed(LOCALID_STATUE, PIXELS(1) / 10, PIXELS(1) / 20);
  ObjectEvent_SetSpeed(LOCALID_STATUE_SHADOW, PIXELS(1) / 10, PIXELS(1) / 20);
  Engine_ObjectEventSetAnimation(LOCALID_STATUE, 2);
  ObjectEvent_SetDestination(LOCALID_STATUE, 328, 104);
  ObjectEvent_SetDestination(LOCALID_STATUE_SHADOW, 328, 108);
  Engine_ScriptWait(60);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Engine_ObjectEventStartRepeatedMotion(LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventWaitForMove(LOCALID_STATUE);
  Battle_SetStart(BATTLE_START_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_AltinPeakHydrosStatue,
                                 WARP_DEST_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 3);
}

/// Scheduler task while the statue sinks: it flickers every other frame.
static void AltinPeak_FlickerStatue(void) {
  if (((gFrameCount >> 1) & 1) != 0) {
    Engine_ObjectEventSetChildValue(LOCALID_STATUE, SPRITE_COLOR_WHITE);
  } else {
    Engine_ObjectEventSetChildValue(LOCALID_STATUE, SPRITE_COLOR_GREY);
  }
}

/// Back from the Hydros statues' battle: Garet, Ivan and Mia step out. Garet
/// cheers, Ivan asks whether the leader agrees, and Mia tells of the guardian
/// statues; the statue shakes, sinks into the floor and shows a chest behind
/// it, and they agree to open it before they go, then rejoin the leader.
static void AltinPeak_CelebrateHydrosVictory(void) {
  struct ObjectEvent *leader;
  struct EventWork *work;
  SchedulerCallback update;
  const s32 *celebrate;
  const s32 *party;

  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_STATUE, PIXELS(328), PIXELS(88));
  ObjectEvent_SetPosition(LOCALID_STATUE_SHADOW, PIXELS(328), PIXELS(88));
  Engine_ObjectEventSetAnimation(LOCALID_STATUE, 0);
  work = gEventWork;
  work->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  work->transitionFrames = 40;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(20);
  // Bring the party out for the victory conversation.
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectEvent_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetActionScript(GARET, gAltinPeak_GaretScript);
  ObjectEvent_SetActionScript(IVAN, gAltinPeak_IvanScript);
  ObjectEvent_RunActionScriptAndWait(MIA, gAltinPeak_MiaScript);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 40);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_ScriptSetMessage(MsgAltinWeDidIsaacWeBeat);
  Script_ShowMessageAndWait(GARET, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 1);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Script_OpenMessage(IVAN, 0);
  if (Engine_ScriptChooseYesNo(0, 0) == 0) {
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_NOD);
  } else {
    Engine_ObjectEventSetAnimationAndWait(IVAN, ANIMATION_SHAKE_HEAD);
    Script_SkipMessages(1);
  }
  Script_ShowMessageAndWait(IVAN, 0, 20);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(MIA, 2);
  Engine_ScriptSetMessage(MsgAltinGuardianStatuesWereCreatedLong);
  Script_ShowMessage(MIA, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_FaceDirection(GARET, FACING_SOUTHEAST, 40);
  Script_ShowMessage(GARET, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Script_PlayCue(SE_190);
  ObjectEvent_SetChildValue(LOCALID_STATUE, SPRITE_COLOR_WHITE);
  Engine_ScriptWait(10);
  Script_PlayCue(AUDIO_CUE_FADE_SLOT3);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Script_PlayCue(SE_103);
  // Animate the statues while the party reacts.
  update = AltinPeak_FlickerStatue;
  Task_AddCallback(update, TASK_PRIORITY_MAP);
  celebrate = gAltinPeak_StatueSinkScript;
  ObjectEvent_SetActionScript(LOCALID_STATUE_SHADOW, celebrate);
  ObjectEvent_RunActionScriptAndWait(LOCALID_STATUE, celebrate);
  Engine_TaskRemoveCallback(update);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessage(IVAN, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 20);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(MIA, 1);
  Script_ShowMessageAndWait(MIA, 0, 20);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 40);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIMATION_NOD);
  Script_ShowMessageAndWait(GARET, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIMATION_NOD);
  Engine_ScriptWait(20);
  // Return the companions and save the completed Hydros battle.
  party = gAltinPeak_RejoinScript;
  ObjectEvent_SetActionScript(GARET, party);
  ObjectEvent_SetActionScript(IVAN, party);
  ObjectEvent_RunActionScriptAndWait(MIA, party);
  Engine_ScriptWait(20);
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  gEventWork->transitionFrames = 16;
  GameFlag_Set(FLAG_HYDROS_STATUES_DEFEATED);
  Engine_ScriptEnd();
}

/// Unused: asks the field to use Frost (psynergyRequest 0x1018, owner 4 and
/// ABILITY_FROST); nothing calls it.
UNUSED static void AltinPeak_RequestPsynergy(void) {
  gEventWork->psynergyRequest = 0x1018;
}

/// Scheduler task in AltinPeakSpritz: while the leader stands higher than 48
/// pixels the second pillar is drawn under the party and the third further
/// back.
static void AltinPeak_UpdateFrostPillarPriority(void) {
  if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed > PIXELS(48)) {
    {
      u8 *flag = &ObjectEvent_GetById(LOCALID_SPRITZ_ROOM_SECOND_PUDDLE)
                      ->priorityFlags;
      s32 bit = 2;

      bit |= *flag;
      *flag = bit;
    }
    Engine_ObjectEventSetSpritePriority(LOCALID_SPRITZ_ROOM_THIRD_PUDDLE, 3);
  } else {
    Engine_ObjectEventSetSpritePriority(LOCALID_SPRITZ_ROOM_THIRD_PUDDLE, 2);
  }
}

/// Makes a puddle walkable to the actor checks and marks its map cell.
static void AltinPeak_MarkObjectEventTerrainCell(s32 objectEventId) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(objectEventId);
  objectEvent->collisionFlags &= (u8)~OBJ_EVENT_SOLID;

  MapGrid_SetMetatileBehaviorAt(0, objectEvent->x.fixed, objectEvent->z.fixed,
                                255);
}

/// The mine cart reaches the end of its run: it settles in the middle of its
/// cell and the party leader jumps off it to the south.
static void AltinPeak_StopCartAndJumpOff(struct ObjectEvent *object) {
  s32 x;
  s32 z;
  s32 centerZ;
  struct AnimationObject *sprite;
  struct ObjectEvent *leader;

  Engine_AudioPlayCue(SE_STOP_SLOT3);
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
  ObjectEvent_SetMode(object, 2);
  Engine_TaskWait(15);
  ObjectEvent_SetMode(object, 1);
  Engine_TaskWait(30);
  sprite = object->sprite;
  sprite->count = 1;
  ResourceMetadata_ClearRecord(sprite->entries[1]);
  sprite->entries[1] = NULL;
  sprite->dirty = 1;
  leader = Engine_ObjectEventLookup(gGameState.selectedObjectEvent);
  Engine_AudioPlayCue(SE_JUMP);
  leader->x.fixed = x;
  leader->velocityY = PIXELS(6);
  leader->gravity = PIXELS(1);
  leader->z.fixed = centerZ;
  leader->sprite->image.priority = 0;
  ObjectEvent_SetMode(leader, 7);
  Engine_ObjectSetPosition(leader, x, leader->y.fixed, z + PIXELS(24));
  leader->facing = FACING_SOUTH;
  Engine_TaskWait(20);
  leader->sprite->image.priority = 2;
  Engine_AudioPlayCue(SE_159);
}

/// Unused: an empty function nothing calls.
UNUSED static void AltinPeak_ReservedNoOp(void) {}

/// The cart turns a quarter turn right around the corner it reaches.
static void AltinPeak_TurnCartRight(struct ObjectEvent *object) {
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
  ObjectEvent_SetMode(object, 5);
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

/// The cart turns a quarter turn left.
static void AltinPeak_TurnCartLeft(struct ObjectEvent *object) {
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
  ObjectEvent_SetMode(object, 6);
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

/// The cart rolls over a marked cell: SE_STOP_SLOT3 plays, and SE_184 and
/// SE_233 follow as its countdown runs out.
static void AltinPeak_StartCartCountdown(struct ObjectEvent *object) {
  object->work = 33;
  Engine_AudioPlayCue(SE_STOP_SLOT3);
}

/// Rolls the cart on by a frame and picks its animation from the terrain ahead
/// of and behind it: flat, uphill or downhill.
static void AltinPeak_AdvanceCart(struct ObjectEvent *object) {
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
  ahead = MapGrid_GetTerrainHeight(2, point->x, point->z);
  FieldVector_AddPolarOffset(-Iwram_MulQ16(radius, 3 * Q16_ONE / 2), angle,
                             point);
  behind = MapGrid_GetTerrainHeight(2, point->x, point->z);
  if (*timer <= 20) {
    if (ahead == behind) {
      ObjectEvent_SetMode(object, 2);
    } else if (ahead > behind) {
      ObjectEvent_SetMode(object, 3);
    } else {
      ObjectEvent_SetMode(object, 4);
    }
  }
}

/// The collision codes that mark the mine cart's route, on map layer 2.
enum AltinPeakCartCell {
  ALTIN_PEAK_CART_COUNTDOWN = 96, ///< AltinPeak_StartCartCountdown
  ALTIN_PEAK_CART_TURN_LEFT = 97,
  ALTIN_PEAK_CART_TURN_RIGHT = 98,
  /// The end of the route, where the leader jumps off.
  ALTIN_PEAK_CART_STOP = 99
};

/// Rolls the mine cart the party leader rides: it sets off the first way it can
/// roll, starting from heading (or its own facing for -1), follows the turns
/// its route's cells mark until the end cell, and the leader jumps off.
static void AltinPeak_RollCart(s32 id, s32 heading) {
  struct ObjectEvent *object = ObjectEvent_GetById(id);
  struct ObjectEvent *leader =
      Engine_ObjectEventLookup(gGameState.selectedObjectEvent);
  struct FieldPosition position;
  s32 directionIndex;

  Engine_ScriptStart();
  if (heading == -1) {
    heading = object->facing;
  }
  for (directionIndex = 0; directionIndex <= 3; directionIndex++) {
    position.x = object->x.fixed;
    position.y = object->y.fixed;
    position.z = object->z.fixed;
    FieldVector_AddPolarOffset(PIXELS(16), heading, &position);
    if (MapGrid_GetTerrainHeight(2, position.x, position.z) ==
        object->y.fixed) {
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
  Engine_CameraFollowObjectEvent(id, 1);
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
    switch (
        MapGrid_GetMetatileBehaviorAt(2, object->x.fixed, object->z.fixed)) {
    case ALTIN_PEAK_CART_TURN_RIGHT:
      AltinPeak_TurnCartRight(object);
      break;
    case ALTIN_PEAK_CART_TURN_LEFT:
      AltinPeak_TurnCartLeft(object);
      break;
    case ALTIN_PEAK_CART_COUNTDOWN:
      AltinPeak_StartCartCountdown(object);
      break;
    case ALTIN_PEAK_CART_STOP:
      goto arrived;
    }
    AltinPeak_AdvanceCart(object);
    Engine_TaskWait(1);
  }
arrived:
  AltinPeak_StopCartAndJumpOff(object);
  Engine_ScriptEnd();
}

/// Scheduler task that cycles the water's palette colors through
/// gAltinPeak_PaletteScript.
static void AltinPeak_StepPaletteAnimation(void) {
  // The palette cycle borrows RGB555 colours from the fade work's source
  // palette; each step replaces BG colours 3 to 11.
  u16 *palette = gPaletteBufferWork->palette;

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

/// Starts the water's palette cycle.
static void AltinPeak_StartPaletteAnimation(void) {
  gAltinPeak_PaletteStep = 0;
  gAltinPeak_PaletteHold = 0;
  FieldScheduler_AddCallback(AltinPeak_StepPaletteAnimation, TASK_PRIORITY_MAP);
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
    SC_CALL(Map_TurnTowardTarget),
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

/// AltinPeakFallingRocks's boulder rolls straight down the path, and marks its arrival.
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
    SC_ANIMATION(ANIMATION_STAND),
    SC_LABEL(1),
    SC_ADD(SW_FACING, 1024),
    SC_LOOP(24, SC_TO(1)),
    SC_END,
};

/// A piece of debris flies for 60 frames and is freed.
s32 gAltinPeak_DebrisScript[] = {
    SC_CALL(AltinPeak_FlyDebris),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

/// Garet, Ivan and Mia step out beside the leader after the Hydros battle.
s32 gAltinPeak_GaretScript[] = {
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(314), 0, PIXELS(126)),
    SC_SYNC,
    SC_ANIMATION(ANIMATION_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_IvanScript[] = {
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(346), 0, PIXELS(126)),
    SC_SYNC,
    SC_ANIMATION(ANIMATION_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 gAltinPeak_MiaScript[] = {
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(328), 0, PIXELS(138)),
    SC_SYNC,
    SC_ANIMATION(ANIMATION_STAND),
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
    SC_ANIMATION(ANIMATION_WALK),
    SC_MOVE(PIXELS(328), 0, PIXELS(118)),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

struct MapWarpDest gAltinPeak_WarpDestsOther[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_FirstWaterBeastWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN, CONDITION_ALWAYS, 456, 0, 520, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {98, CONDITION_ALWAYS, 456, 0, 488, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {WARP_DEST_AFTER_BATTLE, CONDITION_ALWAYS, 104, 0, 232, FACING_SOUTH, 0, 0, 56, 506, 530, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_RailTunnelWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 248, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SECOND_WATER_BEAST, CONDITION_ALWAYS, 56, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_SecondWaterBeastWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_FROM_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_FROM_CHEST_ROOM, CONDITION_ALWAYS, 664, 0, 380, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_FROM_LAKE, CONDITION_ALWAYS, 904, 0, 214, FACING_NORTH, 0, 842, 40, 1016, 248, 0},
    {98, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {WARP_DEST_AFTER_BATTLE, CONDITION_ALWAYS, 320, 0, 472, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_SouthEntranceWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN, CONDITION_ALWAYS, 248, 0, 498, FACING_NORTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_FROM_FLOODED_CHAMBER, CONDITION_ALWAYS, 119, 0, 108, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, 567, 0, 105, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_FROM_TUNNEL, CONDITION_ALWAYS, 887, 0, 798, FACING_NORTH, 0, 712, 568, 960, 860, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_POOL, CONDITION_ALWAYS, 791, 0, 679, FACING_SOUTH, 0, 712, 560, 960, 860, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, 88, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, 216, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_ThirdWaterBeastWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_THIRD_WATER_BEAST_FROM_ALTIN_PEAK_POOL, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {98, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {WARP_DEST_AFTER_BATTLE, CONDITION_ALWAYS, 747, 0, 258, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_PoolWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 568, 0, 348, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST, CONDITION_ALWAYS, 295, 0, 95, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {ALTIN_PEAK_POOL_SOUTHWEST_FROM_NORTH, CONDITION_ALWAYS, 442, 0, 774, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {ALTIN_PEAK_POOL_SOUTHWEST_FROM_SOUTHEAST, CONDITION_ALWAYS, 72, 0, 870, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {ALTIN_PEAK_POOL_SOUTHEAST_FROM_SOUTHWEST, CONDITION_ALWAYS, 918, 0, 712, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_THIRD_WATER_BEAST, CONDITION_ALWAYS, 615, 0, 684, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_SpritzWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 72, 0, 502, FACING_NORTH, 0, 0, 0, 820, 568, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, 152, 0, 174, FACING_SOUTH, 0, 0, 0, 820, 568, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN, CONDITION_ALWAYS, 264, 0, 890, FACING_NORTH, 0, 144, 720, 384, 908, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_JunctionWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 392, 0, 116, FACING_SOUTH, 0, 0, 0, 830, 564, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_NORTH, CONDITION_ALWAYS, 40, 0, 116, FACING_NORTH, 0, 0, 0, 830, 564, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_WEST_EXIT, CONDITION_ALWAYS, 520, 0, 616, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN, CONDITION_ALWAYS, 408, 0, 970, FACING_NORTH, 0, 244, 564, 1024, 1024, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_SOUTH, CONDITION_ALWAYS, 951, 0, 791, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_FallingRocksWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 361, 0, 309, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {ALTIN_PEAK_FALLING_ROCKS_WEST_FROM_EAST, CONDITION_ALWAYS, 89, 0, 749, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_SOUTH, CONDITION_ALWAYS, 728, 0, 717, FACING_NORTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_EAST_FROM_WEST, CONDITION_ALWAYS, 935, 0, 493, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_NORTH, CONDITION_ALWAYS, 791, 0, 566, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE, CONDITION_ALWAYS, 792, 16, 577, FACING_SOUTH, 0, 618, 0, 998, 854, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_HydrosStatueWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {98, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {WARP_DEST_AFTER_BATTLE, CONDITION_ALWAYS, 328, 0, 118, FACING_NORTH, 0, 0, 0, 448, 524, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gAltinPeak_WestExitWarpDests[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_WEST_EXIT_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 408, 0, 517, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM, CONDITION_ALWAYS, 198, 0, 336, FACING_SOUTH, 0, 0, 0, 540, 590, 0},
    {ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_FROM_TUNNEL, CONDITION_ALWAYS, 568, 0, 136, FACING_NORTH, 0, 480, 15, 684, 180, 0},
    {ALTIN_PEAK_WEST_EXIT_FROM_WORLD_MAP, CONDITION_ALWAYS, 73, 0, 190, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {MAP_TABLE_END},
};

struct MapWarpRegion gAltinPeak_FallingRocksRegions[] = {
#if !TBS_PATCH_3
    {790, -106, 575, 794, -102, 579, CONDITION_ALWAYS, 6},
    {788, -108, 579, 796, -100, 587, CONDITION_ALWAYS, 6},
#else
    {784, -116, 572, 800, -100, 588, CONDITION_ALWAYS, 6},
#endif
    {MAP_TABLE_END},
};

struct MapWarpRegion gAltinPeak_HydrosStatueRegions[] = {
#if !TBS_PATCH_3
    {84, 38, 420, 92, 46, 428, CONDITION_ALWAYS, 1},
#else
    {80, 34, 416, 96, 50, 432, CONDITION_ALWAYS, 1},
#endif
    {MAP_TABLE_END},
};

u32 gAltinPeak_Warps[] = {
    MAP_WARPS(AltinPeakFirstWaterBeast),
    MAP_WARP(ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_FIRST_WATER_BEAST),
    MAP_WARPS(AltinPeakRailTunnel),
    MAP_WARP(ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_RAIL_TUNNEL),
    MAP_WARP(ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_FROM_ALTIN_PEAK_RAIL_TUNNEL),
    MAP_WARPS(AltinPeakSecondWaterBeast),
    MAP_WARP(ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL, AltinPeakRailTunnel, ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SECOND_WATER_BEAST),
    MAP_WARP(ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_FROM_LAKE),
    MAP_WARP(ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_FROM_CHEST_ROOM),
    MAP_WARPS(AltinPeakSouthEntrance),
    MAP_WARP(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    MAP_WARP(ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_FROM_TUNNEL),
    MAP_WARP(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL, AltinPeakRailTunnel, ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    MAP_WARP(ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_FROM_FLOODED_CHAMBER),
    MAP_WARP(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL, AltinPeakPool, ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    MAP_WARP(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST, AlpineCrossing, ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST),
    MAP_WARP(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST, AlpineCrossing, ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST),
    MAP_WARPS(AltinPeakThirdWaterBeast),
    MAP_WARP(ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL, AltinPeakPool, ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_THIRD_WATER_BEAST),
    MAP_WARPS(AltinPeakPool),
    MAP_WARP(ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_POOL),
    MAP_WARP(ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHWEST_FROM_NORTH),
    MAP_WARP(ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH, AltinPeakPool, ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST),
    MAP_WARP(ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHEAST_FROM_SOUTHWEST),
    MAP_WARP(ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHWEST_FROM_SOUTHEAST),
    MAP_WARP(ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST, AltinPeakThirdWaterBeast, ALTIN_PEAK_THIRD_WATER_BEAST_FROM_ALTIN_PEAK_POOL),
    MAP_WARPS(AltinPeakSpritz),
    MAP_WARP(ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_SPRITZ),
    MAP_WARP(ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_SPRITZ),
    MAP_WARP(ALTIN_PEAK_SPRITZ_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_SPRITZ),
    MAP_WARPS(AltinPeakJunction),
    MAP_WARP(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz, ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_JUNCTION),
    MAP_WARP(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_NORTH),
    MAP_WARP(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_FROM_ALTIN_PEAK_JUNCTION),
    MAP_WARP(ALTIN_PEAK_JUNCTION_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_JUNCTION),
    MAP_WARP(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_SOUTH),
    MAP_WARPS(AltinPeakFallingRocks),
    MAP_WARP(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz, ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_FALLING_ROCKS),
    MAP_WARP(ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_EAST_FROM_WEST),
    MAP_WARP(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_SOUTH),
    MAP_WARP(ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_WEST_FROM_EAST),
    MAP_WARP(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_NORTH),
    MAP_WARP(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_HYDROS_STATUE, AltinPeakHydrosStatue, ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS),
    MAP_WARPS(AltinPeakHydrosStatue),
    MAP_WARP(ALTIN_PEAK_HYDROS_STATUE_TO_ALTIN_PEAK_FALLING_ROCKS, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE),
    MAP_WARPS(AltinPeakWestExit),
    MAP_WARP(ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_WEST_EXIT),
    MAP_WARP(ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_FROM_TUNNEL),
    MAP_WARP(ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM),
    MAP_WARP(ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTIN_PEAK_WEST_EXIT),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gAltinPeak_ObjectEventTemplatesOther[] = {
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_FirstWaterBeastObjectEventTemplates[] = {
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(152), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(296), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(296), 0, PIXELS(336), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_SecondWaterBeastObjectEventTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(216), 0, PIXELS(312), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(248), 0, PIXELS(464), FACING_WEST,
        TALK_KEEP_FACING, 0},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(888), 0, PIXELS(172), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_ThirdWaterBeastObjectEventTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(536), 0, PIXELS(296), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(408), 0, PIXELS(216), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(759), 0, PIXELS(285), FACING_WEST,
        TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_PoolObjectEventTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(712), 0, PIXELS(184), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(696), 0, PIXELS(664), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_SpritzObjectEventTemplates[] = {
    {SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(264), 0, PIXELS(408), FACING_EAST,
        TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(424), 0, PIXELS(424), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(408), 0, PIXELS(424), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(280), 0, PIXELS(168), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(424), 0, PIXELS(248), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(632), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {MERCURY_DJINNI, FLAG_DJINNI_0 + DJINNI_SPRITZ, MOVEMENT_TYPE_STAND, PIXELS(56), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_JunctionObjectEventTemplates[] = {
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(536), 0, PIXELS(636), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_FallingRocksObjectEventTemplates[] = {
    {SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(784), 0, PIXELS(80), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_BLANK_LARGE, CONDITION_ALWAYS, 7, PIXELS(784), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(76), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_BLANK, CONDITION_ALWAYS, 7, PIXELS(774), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_SIGNPOST, CONDITION_ALWAYS, 7, PIXELS(792), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_HydrosStatueObjectEventTemplates[] = {
    {0xc7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(84), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(328), 0, PIXELS(88), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gAltinPeak_WestExitObjectEventTemplates[] = {
    {SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(312), 0, PIXELS(280), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, PIXELS(200), 0, PIXELS(328), FACING_SOUTH,
        TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct MapEvent gAltinPeak_EventsOther[] = {
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_FirstWaterBeastEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {MAP_EVENT_COORD, 10, FLAG_ALTIN_PEAK_FIRST_BEAST_ROUTE_SEEN, EVENT_SCRIPT(AltinPeak_ShowFirstBeastRoute)},
    {MAP_EVENT_COORD, 11, FLAG_ALTIN_PEAK_SECOND_BEAST_ROUTE_SEEN, EVENT_SCRIPT(AltinPeak_ShowSecondBeastRoute)},
    {MAP_EVENT_TALK, LOCALID_FIRST_WATER_BEAST, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FightFirstWaterBeast)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | LOCALID_FIRST_WATER_BEAST, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_FightFirstWaterBeast)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_FirstWaterBeastEventsDrained[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_FIRST_WATER_BEAST_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN,
        EVENT_SCRIPT(AltinPeak_FreezeFirstWaterBeastRoomPuddle)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, FLAG_SECOND_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_RailTunnelEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_SecondWaterBeastEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {MAP_EVENT_TALK, LOCALID_CART, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideCart)},
    {MAP_EVENT_TALK, LOCALID_SECOND_WATER_BEAST, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FightSecondWaterBeast)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | LOCALID_SECOND_WATER_BEAST, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_FightSecondWaterBeast)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_SECOND_WATER_BEAST_ROOM_BOULDER, CONDITION_ALWAYS, 0},
    {BG_EVENT(BG_EVENT_UNNAMED), 83, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipSecondWaterBeastRoomSwitch)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTIN_PEAK_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {MAP_EVENTS_END},
    // Unused: two rows past the end marker, which no lookup reaches: a second
    // ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE exit and a chest on code
    // 100 holding a Herb under FLAG_ALTIN_PEAK_UNUSED_HERB, the spot where the
    // Cookie chest above already stands.
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTIN_PEAK_UNUSED_HERB, FIND_ITEM(ITEM_HERB)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_SouthEntranceEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST},
    {MAP_EVENT_WARP, 7, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_ThirdWaterBeastEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_THIRD_WATER_BEAST_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezeThirdWaterBeastRoomPuddle)},
    {MAP_EVENT_TALK, LOCALID_THIRD_WATER_BEAST, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FightThirdWaterBeast)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_EFFECT | LOCALID_THIRD_WATER_BEAST, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_FightThirdWaterBeast)},
    {MAP_EVENT_TALK, LOCALID_CART, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideCart)},
    {BG_EVENT(BG_EVENT_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipThirdWaterBeastRoomSwitch)},
    // The Dragon Shield's chest opens only from its west side.
    {BG_EVENT_CHEST_FACING(FACING_SOUTHEAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {BG_EVENT_CHEST_FACING(FACING_EAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {BG_EVENT_CHEST_FACING(FACING_NORTHEAST), 100, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_PoolEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_POOL_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezePoolRoomPuddle)},
    {MAP_EVENT_TALK, LOCALID_CART, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideCart)},
    {BG_EVENT(BG_EVENT_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipPoolRoomSwitch)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTIN_PEAK_POOL_MIMIC_BEATEN, FIND_MIMIC | 2},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_SpritzEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN},
    {BG_EVENT(BG_EVENT_CHEST), 101, FLAG_ALTIN_PEAK_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL)},
    {MAP_EVENT_TALK, LOCALID_SPRITZ, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_TalkToDjinni)},
    {MAP_EVENT_TALK, LOCALID_CART, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RideCart)},
    {BG_EVENT(BG_EVENT_UNNAMED), 50, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_FlipSpritzRoomSwitch)},
    {MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_PlacePillarUnderLeader)},
    {MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_RestorePillarAutomaticPriority)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SPRITZ_ROOM_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(AltinPeak_SettleSpritzRoomPillar)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
        EVENT_SCRIPT(AltinPeak_PushSpritzRoomPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SPRITZ_ROOM_PUDDLE, FLAG_PUDDLE_FROZEN, EVENT_SCRIPT(AltinPeak_FreezeSpritzRoomPuddle)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SPRITZ_ROOM_SECOND_PUDDLE, FLAG_SECOND_PUDDLE_FROZEN,
        EVENT_SCRIPT(AltinPeak_FreezeSpritzRoomSecondPuddle)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SPRITZ_ROOM_THIRD_PUDDLE, FLAG_THIRD_PUDDLE_FROZEN,
        EVENT_SCRIPT(AltinPeak_FreezeSpritzRoomThirdPuddle)},
    {MAP_EVENTS_END},
};
// clang-format on

struct MapEvent gAltinPeak_JunctionEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTIN_PEAK_LUCKY_MEDAL,
     FIND_ITEM(ITEM_LUCKY_MEDAL)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_JUNCTION_ROOM_BOULDER,
     CONDITION_ALWAYS, 0},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_FallingRocksEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH},
    {BG_EVENT(BG_EVENT_UNNAMED), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(AltinPeak_ReadFallingRockSign)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_WALL,
     FLAG_ALTIN_PEAK_BOULDER_FALLEN, EVENT_SCRIPT(AltinPeak_ForceLogOver)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_HydrosStatueEvents[] = {
    {MAP_EVENT_COORD, 10, FLAG_HYDROS_STATUES_DEFEATED,
     EVENT_SCRIPT(AltinPeak_StartHydrosBattle)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTIN_PEAK_LIFTING_GEM,
     FIND_ITEM(ITEM_LIFTING_GEM)},
    {MAP_EVENTS_END},
};

struct MapEvent gAltinPeak_WestExitEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_LIFT), LOCALID_WEST_EXIT_ROOM_BOULDER,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_WEST_EXIT_ROOM_PILLAR,
     CONDITION_ALWAYS, 0},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_ALTIN_PEAK_VIAL, FIND_ITEM(ITEM_VIAL)},
    {MAP_EVENTS_END},
};

/// Frames since the statue's last rumble.
s32 gAltinPeak_RumbleTicks = 0;

/// The cart's speed factor by its height against where it set off: 2 to the
/// power i / 16 for i from 0 to 32, in 16.16.
u32 gAltinPeak_RollRadii[] = {
    0x10000, 0x10b55, 0x1172b, 0x12387, 0x1306f, 0x13dea, 0x14bfd,
    0x15ab0, 0x16a09, 0x17a11, 0x18ace, 0x19c49, 0x1ae89, 0x1c199,
    0x1d581, 0x1ea4b, 0x20000, 0x216ab, 0x22e56, 0x2470f, 0x260df,
    0x27bd4, 0x297fb, 0x2b560, 0x2d413, 0x2f422, 0x3159c, 0x33892,
    0x35d13, 0x38333, 0x3ab03, 0x3d495, 0x40000,
};

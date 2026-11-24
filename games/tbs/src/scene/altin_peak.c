/// @file
/// Altin Peak, the mine under Altin that the villagers and the Alpine
/// Crossing's signpost call Altin Mine. Its rooms are eleven scenes, all
/// named Altin Peak, except that the international editions name the south
/// entrance the Alpine Crossing when the party comes in from there.
///
/// Altin is flooded because water beasts are spitting water in the mine. They
/// look just like Altin's guardian statues, and their battles call each one a
/// Living Statue. Each beast Isaac beats lowers the water a step, in the mine
/// and in the village (FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN). The party
/// goes down three times, each time by another doorway of Altin. Before any of
/// that, the passage Move opens behind Altin's first guardian statue leads to
/// a small cave with the Psy Crystal's chest, a corner of Spritz's room's map.
///
/// First, through the doorway in Altin's west cliff, the party follows the
/// beast it saw spitting by the village into its cave. When the party walks up
/// the passage to it, it hops into the cave, freezes the puddle in the gap
/// between two ledges with Frost and hops across the ice. When Isaac climbs
/// the ladder onto the east ledge, it hops off west and settles there to spit.
/// Talking to it, or reading its mind, starts the battle. Back from it, Isaac
/// gets the Frost Jewel and the water drains away; from then on Frost freezes
/// the mine's puddles into ice pillars to hop across. A pillar stays frozen
/// only until the party leaves its room.
///
/// Second, through the rail tunnel in the north cliff, the rails lead to the
/// second beast's lake. Isaac flips the switch so its arrow points east and
/// rides the mine cart to the beast. After it the water drains from the south
/// entrance's flooded chamber, which leads on to the pool cave: a switch, a
/// Mimic in a chest, and a puddle to freeze on the way to the third beast's
/// room. There a frozen puddle and a cart ride reach the Dragon
/// Shield, and the switch and another ride lead to the beast. The third drain
/// empties the pool and the rest of Altin.
///
/// Last, through the doorway in the middle of the village, the party passes
/// the Lucky Medal's chest and comes to the big room where Spritz, a Mercury
/// Djinni, waits. Isaac moves the stone pillar there and freezes three puddles
/// into a path, and a switch and the mine cart take him to Spritz. The way
/// out of that room leads past many small rocks to a narrow passage that ends
/// at a sign: "Watch for falling rocks! Do not strike the wall! Rocks may
/// fall!" Garet strikes the wall anyway, or Isaac uses Force on it, and a
/// boulder chases Isaac down the winding path and crashes into the old doorway
/// at its foot. A ladder there now goes down to a room of ruins buried in the
/// mine, where the Hydros Statue wakes and fights the party. When it is beaten,
/// the statue sinks into the floor and the chest behind it holds the Lifting
/// Gem, whose Lift frees Hsu at the Alpine Crossing
/// (alpine_crossing_lama_temple.c). In the mine, Lift raises the round boulders
/// of the lake room's side chamber (by the Cookie's chest), the junction and
/// the west exit; the field does that alone, and their rows run no event of
/// this file's.
///
/// The south entrance also opens onto the Alpine Crossing, and the west exit
/// onto the World Map.
///
/// Every room's tables are picked by gMainRoutineState.next,
/// all the rooms share one exit table, and Scene_Initialize calls the setup of
/// the room being entered (AltinPeak_RestoreFirstWaterBeastRoom and so on),
/// which puts back what the party has already done there. A beast is
/// SPRITE_WATER_BEAST drawn with its green and blue swapped (SPRITE_COLOR_RBG),
/// and its spit is SPRITE_DUST_PUFF recoloured, as in Altin. The falling
/// boulder is SPRITE_ROCK with a SPRITE_BLANK_LARGE actor for its shadow, and
/// its pieces in the crash are SPRITE_ROCK again, scaled down
/// (AltinPeak_SpawnDebris).

#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_sequence.h"
#include "field/map_grid_scroll.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/frost_glint.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/dma.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/main_routine.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// How far a drain's flicker has gone, 0 to 100: the chance in a hundred that a
/// screen line no longer shows the water (AltinPeak_FlickerWaterAway).
u16 gAltinPeak_FlickerTimer BSS_DATA;
/// The spit's direction, FACING_EAST or FACING_WEST.
s32 gAltinPeak_SpitDirection BSS_DATA;
/// Where a beast's spit starts, its spout: x, y and z in 16.16 pixels.
s32 gAltinPeak_SpitOrigin[3] BSS_DATA;
/// Unused: nothing reads or writes these bytes.
u8 gAltinPeak_UnusedAfterSpitOrigin[16] BSS_DATA;
/// Frames left on the water palette's current step (gAltinPeak_PaletteScript).
s16 gAltinPeak_PaletteHold BSS_DATA;
/// The next byte of gAltinPeak_PaletteScript; 0 again at its -1.
s16 gAltinPeak_PaletteStep BSS_DATA ALIGNED(4);

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
extern FIXED gAltinPeak_CartSpeeds[];

/// The rail switches by the forks, each set once flipped from its first
/// position; they are area temp flags, kept until the party leaves the mine.
#define FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH FLAG_AREA_TEMP_23
#define FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH FLAG_AREA_TEMP_25
#define FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH FLAG_AREA_TEMP_26
#define FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH FLAG_AREA_TEMP_27

/// The cell behaviours the rooms' trigger tables answer, besides their exits
/// and chests. Each is a cell of the map named here.
enum AltinPeakTriggerCell {
  /// The first water beast's cave: the passage just south of the beast, cells
  /// (17, 24) to (19, 24).
  CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_FIRST_BEAST = 10,
  /// The first water beast's cave: the east ledge at the edge of the gap,
  /// cells (21, 13) and (21, 14).
  CELL_BEHAVIOR_ALTIN_PEAK_EAST_LEDGE_BY_GAP = 11,
  /// The lake room: the switch by the fork below the torch, cell (24, 13).
  CELL_BEHAVIOR_ALTIN_PEAK_LAKE_SWITCH = 83,
  /// The switch of the third water beast's room (cell (11, 7)), the pool cave
  /// (cell (16, 31)) and Spritz's room (cell (29, 20)).
  CELL_BEHAVIOR_ALTIN_PEAK_SWITCH = 50,
  /// Spritz's room: the top of the ladder down to the stone pillar, cells
  /// (25, 18) to (27, 19).
  CELL_BEHAVIOR_ALTIN_PEAK_LADDER_TOP = 10,
  /// Spritz's room: the foot of that ladder, cells (25, 20) to (27, 21).
  CELL_BEHAVIOR_ALTIN_PEAK_LADDER_FOOT = 11,
  /// Spritz's room: west of the stone pillar's first place, cell (23, 26).
  CELL_BEHAVIOR_ALTIN_PEAK_WEST_OF_PILLAR = 12,
  /// The falling-rock sign, cell (49, 6).
  CELL_BEHAVIOR_ALTIN_PEAK_FALLING_ROCK_SIGN = 12,
  /// The Hydros statue's room: the top of the steps before the statue, cells
  /// (19, 7) to (21, 7).
  CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_HYDROS_STATUE = 10,
  /// A room's chest.
  CELL_BEHAVIOR_ALTIN_PEAK_CHEST = 100,
  /// The Psy Crystal's chest, in the small cave behind Altin's first guardian
  /// statue that is a corner of Spritz's room's map.
  CELL_BEHAVIOR_ALTIN_PEAK_PSY_CRYSTAL_CHEST = 101
};

/// SPRITE_DUST_PUFF's quick puff, the spit's drops: its eight pictures once,
/// two to four frames each.
enum { DUST_PUFF_ANIMATION_QUICK = 1 };

/// SPRITE_ROCK rolling fast, the falling boulder.
enum { ROCK_ANIMATION_ROLL_FAST = 4 };

/// SPRITE_HYDROS_STATUE's animations (hydros_statue.tsv).
enum HydrosStatueAnimation {
  /// Grey stone, held: the statue once beaten.
  HYDROS_STATUE_ANIMATION_STONE = ANIM_STILL,
  /// Awake and moving, as it comes at Isaac.
  HYDROS_STATUE_ANIMATION_ADVANCE = 2
};

/// SPRITE_MINE_CART's animations (mine_cart.tsv), as the ride picks them.
enum MineCartAnimation {
  MINE_CART_ANIMATION_ROLL = 2,     ///< rolling on the flat
  MINE_CART_ANIMATION_UPHILL = 3,   ///< the ground ahead higher
  MINE_CART_ANIMATION_DOWNHILL = 4, ///< the ground ahead lower
  MINE_CART_ANIMATION_TURN_RIGHT = 5,
  MINE_CART_ANIMATION_TURN_LEFT = 6
};

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
static void AltinPeak_StartSpittingFX(FIXED x, FIXED y, FIXED z, s32 facing);
/// AltinPeak_StartSpittingFX from a point in whole pixels.
#define AltinPeak_StartSpitting(x, y, z, facing)                               \
  AltinPeak_StartSpittingFX(toFIXED(x), toFIXED(y), toFIXED(z), facing)
static void AltinPeak_SpawnDebris(void);
/// Makes the frozen puddle glint (frost_glint.h).
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

/// Each water beast's battle and the Hydros Statue's return the party to its
/// room by ENTRANCE_AFTER_BATTLE, where the room's setup plays what follows.
///
/// ENTRANCE_ROOM_START stands at the way in of each water beast's room and the
/// Hydros statue's, and the statue room's setup lands the party there as it
/// does from the falling-rocks passage. Unused: no exit or event leads to it.
enum { ENTRANCE_ROOM_START = 98, ENTRANCE_AFTER_BATTLE = 99 };

/// The rooms' actors, from their actor placements.
///
/// The lake room, the third water beast's room, the pool cave and Spritz's
/// room place their mine cart first.
enum { ACTOR_CART = ACTOR_FIRST_PLACED };

/// The first water beast's cave: the puddle that appears by the west ladder
/// when the water drains, the puddle in the gap between the ledges, and the
/// beast.
enum {
  ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE = ACTOR_FIRST_PLACED,
  ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
  /// Its battle names it a Living Statue.
  ACTOR_FIRST_WATER_BEAST
};

/// The lake room, the second water beast's.
enum {
  ACTOR_SECOND_WATER_BEAST = ACTOR_CART + 1,
  ACTOR_SECOND_WATER_BEAST_ROOM_BOULDER
};

/// The third water beast's room: its puddle and the beast.
enum {
  ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE = ACTOR_CART + 1,
  ACTOR_THIRD_WATER_BEAST
};

/// The pool room's puddle.
enum { ACTOR_POOL_ROOM_PUDDLE = ACTOR_CART + 1 };

/// Spritz's room: its puddles and pillar, an unused boulder, and the Djinni.
enum {
  ACTOR_SPRITZ_ROOM_PUDDLE = ACTOR_CART + 1,
  ACTOR_SPRITZ_ROOM_PILLAR,
  ACTOR_SPRITZ_ROOM_SECOND_PUDDLE,
  ACTOR_SPRITZ_ROOM_THIRD_PUDDLE,
  /// Unused: a round boulder at (520, 632), outside both of the room's camera
  /// boxes, so the player never sees it, and no trigger names it.
  ///
  /// The boxes are gAltinPeak_SpritzEntrances'. One ends at z 568, the one
  /// below it at x 384. The boulder stands almost where the junction places
  /// its own boulder, (536, 636).
  ACTOR_UNUSED_SPRITZ_ROOM_BOULDER,
  /// The Mercury Djinni, placed until it has joined.
  ACTOR_SPRITZ = 14
};

/// The junction's boulder.
enum { ACTOR_JUNCTION_ROOM_BOULDER = ACTOR_FIRST_PLACED };

/// The falling-rocks passage: the boulder that falls and its shadow, the log
/// standing upright at the dead end, which topples as the boulder comes down,
/// the wall Force is cast on, and the sign.
enum {
  ACTOR_FALLING_BOULDER = ACTOR_FIRST_PLACED,
  ACTOR_BOULDER_SHADOW,
  /// SPRITE_LOG_NORTH_SOUTH.
  ACTOR_STANDING_LOG,
  /// SPRITE_BLANK, at the dead end's wall.
  ACTOR_WALL,
  /// The falling-rock sign, drawn with the sprite of Lalivero's sign and the
  /// Kolima Forest floodgate's (SPRITE_SIGNPOST). It is read by searching its
  /// cell, CELL_BEHAVIOR_ALTIN_PEAK_FALLING_ROCK_SIGN
  /// (AltinPeak_ReadFallingRockSign).
  ACTOR_FALLING_ROCK_SIGN
};

/// The Hydros statue's room: the statue on its dais and its shadow.
enum { ACTOR_STATUE = ACTOR_FIRST_PLACED, ACTOR_STATUE_SHADOW };

/// The west exit's room: its boulder and pillar.
enum {
  ACTOR_WEST_EXIT_ROOM_BOULDER = ACTOR_FIRST_PLACED,
  ACTOR_WEST_EXIT_ROOM_PILLAR
};

/// The rooms' frozen puddles and Garet at the sign. They are scene temp flags,
/// kept within one visit to a room, across its battles.
enum {
  /// Frost has frozen the room's first puddle.
  FLAG_ALTIN_PEAK_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
  /// Frost has frozen the puddle in the gap of the first water beast's cave,
  /// or the second puddle of Spritz's room.
  FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
  /// Frost has frozen the third puddle of Spritz's room.
  FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
  /// Garet has come out at the falling-rock sign and struck the wall.
  FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS = FLAG_SCENE_TEMP_05
};

/// Talking to Spritz, the Mercury Djinni waiting in its big room of the mine,
/// starts its battle; beaten, it joins the party.
static void AltinPeak_TalkToDjinni(void) {
  Field_RequestDjinnBattle(ACTOR_SPRITZ, DJINNI_SPRITZ);
}

#include "shared/actor_facing.c"

/// Update routine of the falling boulder's shadow: it stays under the
/// boulder, on the ground 12 pixels down, while the boulder bounces.
static UNREAD_RESULT AltinPeak_ShadowFollowBoulder(struct Actor *follower) {
  struct Actor *actor = Actor_Get(ACTOR_FALLING_BOULDER);

  follower->x.fixed = actor->x.fixed;
  follower->y.fixed = -toFIXED(12);
  follower->z.fixed = actor->z.fixed;
#ifndef UBFIX
  return 0;
#endif
}

/// Actor script routine for a piece of flying rock debris. It moves by its
/// velocity, grows by its z velocity and falls under its gravity.
static s32 AltinPeak_FlyDebris(struct Actor *actor) {
  FIXED rise;
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
  s16 sceneId = gMainRoutineState.next;

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

/// The falling-rocks passage and the Hydros statue's room have exit regions,
/// round the ladder between them.
const struct SceneExitRegion *Scene_GetExitRegions(void) {
  s16 sceneId = gMainRoutineState.next;

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
  s16 sceneId = gMainRoutineState.next;

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

/// A 16.16 height shifted down to whole quarter pixels.
enum { QUARTER_PIXEL_SHIFT = 14 };

/// Isaac jumps into a mine cart when he talks to it, and it rolls him along
/// the rails until the end of its line, where he jumps out to the south.
///
/// The ride itself is AltinPeak_RollCart; its turns are cells of the map, so
/// a switch changes where it goes.
static void AltinPeak_RideCart(s32 cartId) {
  struct Actor *cart;
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  cart = Actor_Get(ACTOR_CART);
  Field_EndBusyAndReveal();
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
  Event_Wait(10);
  Sound_Play(SE_JUMP);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3.2);
  // Leap by the height between leader and cart, in quarter pixels, plus 4.
  leader->velocityY =
      ((ABS(cart->y.fixed - leader->y.fixed) >> QUARTER_PIXEL_SHIFT)
       << QUARTER_PIXEL_SHIFT) +
      toFIXED(4);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  FieldActor_SetMoveTargetFX(leader, cart->x.fixed, cart->y.fixed,
                             cart->z.fixed);
  wait(10);
  leader->sprite->image.priority = 3;
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);

  for (;;) {
    if (!((cart->y.fixed >> QUARTER_PIXEL_SHIFT) <
          (leader->y.fixed >> QUARTER_PIXEL_SHIFT)))
      break;
    wait(1);
  }
  Event_End();
  Sound_Play(SE_DOOR_CLOSE);
  AltinPeak_RollCart(cartId, 0);
  wait(20);
  Scene_PlayQueuedMusic();
}

/// Talking to the first water beast, or reading its mind, starts its battle
/// (a Living Statue), wherever it stands in its cave.
static void AltinPeak_FightFirstWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_ALTIN_PEAK_FIRST_WATER_BEAST,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// Talking to the second water beast at the lake, or reading its mind, starts
/// its battle, as the first one's does.
static void AltinPeak_FightSecondWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_ALTIN_PEAK_SECOND_WATER_BEAST,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// Talking to the third water beast, or reading its mind, starts its battle,
/// as the first one's does.
static void AltinPeak_FightThirdWaterBeast(void) {
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_ALTIN_PEAK_THIRD_WATER_BEAST,
                                  ENTRANCE_AFTER_BATTLE);
  Field_RequestBattleFromEncounterTable(
      ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/// Move on the stone pillar in Spritz's room, in the channel below the ladder
/// (first at cell (25, 26)), slides it a cell, and its top becomes a square
/// Isaac can hop onto, a step of the path he makes there with the frozen
/// puddle beside it.
///
/// The field's Move slides the pillar; this event runs after it, and the
/// room's setup and AltinPeak_PushSpritzRoomPillar call it too. It puts back
/// the floor of both cells the pillar can stand on, then gives the one it
/// stands on the hop square kept at cell (0, 0).
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

/// Frost on the puddle by the west ladder of the first water beast's cave
/// (cell (9, 13)), which appears when the water drains, freezes it into an
/// ice pillar against the ledge, whose top Isaac can hop onto.
///
/// The puddle's cell takes the hop square kept at cell (7, 13), and the
/// pillar is raised 32 pixels and drawn under the party.
static void AltinPeak_FreezeFirstWaterBeastRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(7, 13, 1, 1, 9, 13);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= toFIXED(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on the puddle in the gap between the two ledges of the first water
/// beast's cave (cells (17, 13) to (19, 13)) freezes it into an ice pillar in
/// the gap again, after the drain has taken the beast's own ice away.
///
/// The three cells of the gap take the ones kept at cells (29, 1) to (31, 1),
/// a hop square between two plain cells, and the ice is left to fall to the
/// gap's floor.
static void AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(29, 1, 3, 1, 17, 13);
  if (puddle != NULL) {
    puddle->motionFlags = ACTOR_FALLS;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/// Frost on the puddle beside the stone pillar in Spritz's room (cell (26,
/// 26)) freezes it into an ice pillar whose top Isaac can hop onto, the rest
/// of the path across the channel below the ladder.
static void AltinPeak_FreezeSpritzRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_SPRITZ_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(0, 0, 1, 1, 26, 26);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= toFIXED(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on the puddle in the middle of the third water beast's room (cell
/// (25, 13)), in the low channel between the ledges, freezes it into an ice
/// pillar whose top Isaac can hop onto.
static void AltinPeak_FreezeThirdWaterBeastRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(23, 13, 1, 1, 25, 13);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= toFIXED(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on the puddle on the way to the third water beast's room, in the
/// pool cave's south-east part (cell (43, 41)), freezes it into an ice pillar
/// whose top Isaac can hop onto.
static void AltinPeak_FreezePoolRoomPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_POOL_ROOM_PUDDLE);
  MapGrid_CopyCellAttributes(45, 41, 1, 1, 43, 41);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= toFIXED(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/// Frost on the puddle between the two torches in the north-west of Spritz's
/// room (cell (17, 10)) freezes it into an ice pillar whose top Isaac can hop
/// onto, on the way out of the room.
///
/// Its cell takes the hop square kept at cell (1, 0), of another ground shape
/// than the one the first puddle takes at cell (0, 0).
static void AltinPeak_FreezeSpritzRoomSecondPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 17, 10);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= toFIXED(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/// Frost on the puddle north of the rails at the top of the ladder in
/// Spritz's room (cell (26, 15)) freezes it into an ice pillar whose top
/// Isaac can hop onto.
static void AltinPeak_FreezeSpritzRoomThirdPuddle(void) {
  struct Actor *puddle;

  puddle = Actor_Get(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 26, 15);
  if (puddle != NULL) {
    Actor_SetSpriteFlags(puddle, 0);
    puddle->y.fixed -= toFIXED(32);
    puddle->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  GameFlag_Set(FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN);
}

/// HBlank handler while the water layer vanishes: a line shows the water while
/// a random number below 100 reaches the timer, so ever fewer lines show it.
static void AltinPeak_FlickerWaterOut(void) {
  u16 control = REG_DISPCNT & ~DISPCNT_BG1_ON;

  if (Random_Below(100) >= gAltinPeak_FlickerTimer) {
    control |= DISPCNT_BG1_ON;
  }
  Io_Write16(control, &REG_DISPCNT);
}

/// Flickers the water away while the timer counts to 100, then lets the
/// screen settle; every drain ends this way.
static inline void AltinPeak_FlickerWaterAway(void) {
  Irq_SetHandler(IRQ_HBLANK, 0, AltinPeak_FlickerWaterOut);
  gAltinPeak_FlickerTimer = 0;
  do {
    wait(1);
  } while (++gAltinPeak_FlickerTimer <= 100);
  wait(1);
  Irq_SetHandler(IRQ_HBLANK, 0, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(30);
}

/// The pixels the water layer sinks in a drain, one every fourth frame.
enum { DRAIN_SINK_PIXELS = 24 };

/// The first drain, after the Frost Jewel is found: with a rumble the water of
/// the first water beast's cave sinks, flickers and is gone, and a new puddle
/// grows by the west ladder as it goes.
///
/// The water is the overhead map layer, scrolled down a pixel at a time; the
/// two cells under the west ladder (3, 14) and (3, 15) then take their dry
/// form from cell (0, 0).
static void AltinPeak_DrainFirstWater(void) {
  struct MapGridScrollState *grid = gMapGridState[0];
  struct MapGridLayerScroll *water;
  s32 step;

  Sound_Play(SE_WATER_DRAIN);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  water = &grid->layers[MAPGRID_LAYER_OVERHEAD];
  for (step = 0; step < DRAIN_SINK_PIXELS; step++) {
    water->offsetY -= toFIXED(1);
    wait(4);
    if (step == 8) {
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE)->scaleX = FIXED_ONE / 10;
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE)->scaleY = FIXED_ONE / 10;
      Actor_SetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 152, 216);
      Actor_SetScript(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE,
                      gAltinPeak_PuddleGrowScript);
    }
  }
  AltinPeak_FlickerWaterAway();
  MapGrid_CopyCellAttributes(0, 0, 1, 2, 3, 14);
  GameFlag_Set(FLAG_ALTIN_MINE_FIRST_DRAIN);
}

/// The second drain, after the beast at the lake: the water sinks and flickers
/// away in the same way.
///
/// The cells (77, 28) to (92, 31) take the pictures kept at (93, 41) while the
/// water goes, and their dry form, kept at (77, 41), once it has gone.
static void AltinPeak_DrainSecondWater(void) {
  struct MapGridScrollState *grid = gMapGridState[0];
  struct MapGridLayerScroll *water;
  s32 step;

  MapGrid_CopyCells(93, 41, 16, 4, 77, 28);
  Sound_Play(SE_WATER_DRAIN);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  water = &grid->layers[MAPGRID_LAYER_OVERHEAD];
  for (step = 0; step < DRAIN_SINK_PIXELS; step++) {
    water->offsetY -= toFIXED(1);
    wait(4);
  }
  AltinPeak_FlickerWaterAway();
  MapGrid_CopyCells(77, 41, 16, 4, 77, 28);
  GameFlag_Set(FLAG_ALTIN_MINE_SECOND_DRAIN);
}

/// The third drain, after the third beast: the same sinking and flicker, which
/// empties the last of the mine's water.
///
/// The cells round (103, 17) take the pictures kept at (111, 31) and nearby
/// while the water goes, and their dry form, kept at (103, 14), once it has
/// gone.
static void AltinPeak_DrainThirdWater(void) {
  struct MapGridScrollState *grid = gMapGridState[0];
  struct MapGridLayerScroll *water;
  s32 step;

  MapGrid_CopyCellsTo(113, 31, 103, 17, 1, 1);
  MapGrid_CopyCellsTo(111, 32, 104, 18, 3, 2);
  MapGrid_CopyCellsTo(64, 32, 103, 18, 1, 2);
  Sound_Play(SE_WATER_DRAIN);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(10);
  water = &grid->layers[MAPGRID_LAYER_OVERHEAD];
  for (step = 0; step < DRAIN_SINK_PIXELS; step++) {
    water->offsetY -= toFIXED(1);
    wait(4);
  }
  AltinPeak_FlickerWaterAway();
  MapGrid_CopyCellsTo(103, 14, 103, 17, 4, 3);
  GameFlag_Set(FLAG_ALTIN_MINE_THIRD_DRAIN);
}

/// Turns the arrow of the lake room's switch to point the other way and sends
/// the mine cart down the other branch of the fork below it.
///
/// The arrow is the pair of cells (24, 11) and (24, 12), redrawn from cells
/// (0, 1) or (2, 1). The fork's turn is the cart route's cell (24, 16) on the
/// overhead layer (row 80 of the grid), set to turn right or left from cell
/// (0, 0) or (2, 0) (CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_RIGHT or _LEFT).
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

/// Searching the switch by the torch in the lake room (cell (24, 13)) flips
/// it: "Isaac flipped the switch." Its red arrow, first pointing west over
/// the fork below it, turns east, and the mine cart takes the other branch.
///
/// The walkthrough's way to the second beast is to make the arrow point east
/// and then ride the cart.
static void AltinPeak_FlipSecondWaterBeastRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_Play(SE_CHEST_CLOSE);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  wait(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Turns the arrow of the third water beast's room's switch and the fork's
/// turn, as AltinPeak_ToggleSecondWaterBeastRoomSwitchCells does.
///
/// The arrow is cells (11, 4) and (11, 5), redrawn from (48, 32) or (49, 32),
/// and the turn is the route's cell (11, 9) (row 73), from (12, 72) or (10,
/// 72).
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

/// Searching the switch on the small ledge by the torch at the top of the
/// third water beast's room (cell (11, 7)) flips it: "Isaac flipped the
/// switch." Its red arrow over the fork below turns from west to east, or
/// back, and the mine cart takes the other branch.
static void AltinPeak_FlipThirdWaterBeastRoomSwitch(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_Play(SE_CHEST_CLOSE);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  wait(20);
  Scene_PlayQueuedMusic();
  Event_End();
}

/// Searching the switch below the fork in the north of the pool cave (cell
/// (16, 31)) flips it: "Isaac flipped the switch." Its red arrow turns from
/// east to west, or back, and the mine cart takes the other branch.
///
/// The arrow is cells (16, 28) and (16, 29), redrawn from (46, 29) or (47,
/// 29), and the turn is the route's cell (16, 28) (row 92), from (17, 93) or
/// (15, 93).
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

/// Searching the switch east of the ladder's top in Spritz's room (cell (29,
/// 20)) flips it: "Isaac flipped the switch." Its red arrow by the fork turns
/// from east to west, or back, and the mine cart takes the other branch.
///
/// The arrow is cells (29, 17) and (29, 18), redrawn from (46, 28) or (47,
/// 28), and the turn is the route's cell (29, 17) (row 81), from (30, 82) or
/// (28, 82).
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

/// Nothing visible happens when Isaac steps onto the top of the ladder down to
/// the stone pillar in Spritz's room (cells (25, 18) to (27, 19)); the game
/// only has the pillar drawn beneath him while he is up there.
///
/// It sets the pillar's ACTOR_PRIORITY_UNDERFOOT;
/// AltinPeak_RestorePillarAutomaticPriority clears it at the ladder's foot.
static void AltinPeak_PlacePillarUnderLeader(void) {
  Actor_Get(ACTOR_SPRITZ_ROOM_PILLAR)->priorityFlags =
      ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
}

/// Nothing visible happens when Isaac steps onto the foot of the ladder down
/// to the stone pillar in Spritz's room (cells (25, 20) to (27, 21)); the
/// game draws the pillar in front of or behind him by where each stands
/// again, as before he climbed up.
static void AltinPeak_RestorePillarAutomaticPriority(void) {
  Actor_Get(ACTOR_SPRITZ_ROOM_PILLAR)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
}

/// Walking east into the stone pillar in Spritz's room from cell (23, 26),
/// after Move has slid it a cell west, pushes it back with a scrape to where
/// it first stood, Isaac following behind it.
///
/// Its cells are then set as after Move (AltinPeak_SettleSpritzRoomPillar).
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

/// Force on the wall at the dead end of the falling-rocks passage, past the
/// sign, sends the upright log standing there rocking and toppling over, and a
/// boulder drops and chases Isaac down the winding path.
///
/// The Force row is on the blank actor at the wall, and needs the boulder not
/// to have fallen yet. The log is rocked to each side and then falls with its
/// sprite turning, before AltinPeak_RunBoulderChase.
static void AltinPeak_ForceLogOver(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  FIXED originX;
  FIXED originHeight;
  FIXED cosine;
  FIXED sine;
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

  // Rock the log to each side before the accelerating fall.
  rotationStep = 0;
  while (1) {
    rotationStep += 8;
    sprite->rotation += rotationStep;
    cosine = Trig_Cos(sprite->rotation + DEGtoANG(90));
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation > TRIG_TURN * 9 / 16 - 1) {
      break;
    }
    wait(1);
  }
  rotationStep = 0;
  while (1) {
    rotationStep += 8;
    sprite->rotation -= rotationStep;
    cosine = Trig_Cos(sprite->rotation + DEGtoANG(90));
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
    cosine = Trig_Cos(sprite->rotation + DEGtoANG(90));
    sine = Trig_Sin(sprite->rotation + DEGtoANG(180));
    actor->x.fixed = (cosine << 4) + originX;
    if (sprite->rotation > DEGtoANG(180)) {
      actor->y.fixed = originHeight - (sine << 3);
    }
    if (sprite->rotation + rotationStep > TRIG_TURN * 3 / 4 - 1) {
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

/// The rockfall in the falling-rocks passage: Isaac looks up at the rumble, a
/// boulder drops beside the toppled log, and he runs down the winding path
/// ahead of it to the clearing at the bottom, where it crashes into the old
/// doorway in a shower of rocks and opens the way down. If Garet struck the
/// wall, he waits at the bottom and comes back to rejoin Isaac.
///
/// boulderDelay is the frames Isaac's start is given over the boulder's. The
/// doorway's cells from (43, 35) take the broken ones kept from (25, 35), and
/// FLAG_ALTIN_PEAK_BOULDER_FALLEN keeps them on later visits.
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
  shadow->scaleX = 6 * FIXED_ONE / 5;
  shadow->scaleY = 6 * FIXED_ONE / 5;
  shadow->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
  shadow->update = AltinPeak_ShadowFollowBoulder;
#else
  shadow->update = (void (*)(struct Actor *))AltinPeak_ShadowFollowBoulder;
#endif
  Actor_SetAnimation(ACTOR_FALLING_BOULDER, ROCK_ANIMATION_ROLL_FAST);
  boulder->bounce = FIXED_ONE / 2;
  boulder->x.fixed = toFIXED(786);
  boulder->y.fixed = toFIXED(32);
  boulder->z.fixed = toFIXED(90);
  boulder->scaleX = 2 * FIXED_ONE;
  boulder->scaleY = 2 * FIXED_ONE;
  Event_Wait(10);
  Sound_Play(SE_STONE_RUMBLE);
  MapGrid_SetShake(4, 2, 1);
  Event_Wait(20);
  rock->x.fixed += toFIXED(14);
  rock->y.fixed -= toFIXED(8);
  rock->sprite->rotation = TRIG_TURN * 3 / 4;
  Sound_Play(SE_TREMOR);
  MapGrid_SetShake(1, 1, 1);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Sound_Play(MUS_TEST_YOUR_SKILLS);
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
    Actor_Get(ACTOR_GARET)->facing = FACING_SOUTH_SOUTHWEST;
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
  shadow->x.fixed = toFIXED(786);
  shadow->update = NULL;
  shadow->z.fixed = toFIXED(618);
  shadow->y.fixed = -toFIXED(64);
  Actor_SetSpeed(ACTOR_FALLING_BOULDER, 1.6);
  boulder->bounce = FIXED_ONE / 10;
  boulder->gravity = toFIXED(1) / 5;
  boulder->velocityY = toFIXED(4);
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
      Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
  }
  Scene_PlayMusic();
  GameFlag_Set(FLAG_ALTIN_PEAK_BOULDER_FALLEN);
}

/// Reading the sign at the dead end of the falling-rocks passage (cell (49,
/// 6)) says "Watch for falling rocks! Do not strike the wall! Rocks may
/// fall!". Without the Orb of Force, Garet then steps out: "Tch! We came this
/// deep into the mine just to find a dead end!" He strikes the wall, the cave
/// rumbles, and he shouts "...I have a bad feeling about this. Run, Isaac!
/// Now!" and runs off down the path before the boulder comes.
///
/// Once the boulder has fallen, or with the Orb of Force, the sign is only
/// read, and Force on the wall brings the rockfall (AltinPeak_ForceLogOver).
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
      Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
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
    Actor_SetAnimation(ACTOR_GARET, ANIM_LEADER_RUN);
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

/// The first water beast's cave changes its triggers once the first drain has
/// lowered its water: the beast is gone and its two puddles can be frozen.
const struct Trigger *Scene_GetTriggers(void) {
  const struct Trigger *table;

  if (gMainRoutineState.next == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
    if (GameFlag_Get(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
      table = gAltinPeak_FirstWaterBeastTriggersDrained;
    } else {
      table = gAltinPeak_FirstWaterBeastTriggers;
    }
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_RAIL_TUNNEL) {
    table = gAltinPeak_RailTunnelTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
    table = gAltinPeak_SecondWaterBeastTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
    table = gAltinPeak_SouthEntranceTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
    table = gAltinPeak_ThirdWaterBeastTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_POOL) {
    table = gAltinPeak_PoolTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_SPRITZ) {
    table = gAltinPeak_SpritzTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_JUNCTION) {
    table = gAltinPeak_JunctionTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
    table = gAltinPeak_FallingRocksTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
    table = gAltinPeak_HydrosStatueTriggers;
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_WEST_EXIT) {
    table = gAltinPeak_WestExitTriggers;
  } else {
    table = gAltinPeak_TriggersOther;
  }
  return table;
}

/// The frames from one of a water beast's hops to the next.
#define WATER_BEAST_HOP_PERIOD 60

/// A water beast's update routine while it hops along a route: it leaps every
/// WATER_BEAST_HOP_PERIOD frames with SE_JUMP, and lands with a splash
/// (SE_SPLASH) that shakes the screen for a few frames.
/// Altin's Altin_BeastHop is the same hop in that scene's overlay, its landing
/// shaking the screen up and down where this one shakes it sideways.
static void AltinPeak_UpdateBeastHops(struct Actor *updated) {
  struct Actor *beast = updated;
  // The frames left of the landing's shake.
  s16 *shakeFrames = (s16 *)&beast->userValue;

  // timer counts the frames of the hop, and the effect's countdown is set
  // while the beast is in the air, so that it lands with one splash.
  if (*shakeFrames != 0) {
    if (--*shakeFrames == 1)
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  }
  if (beast->velocityY == 0) {
    Actor_SelectAnimation(updated, ANIM_STAND);
    beast->y.fixed -= toFIXED(3) / 2;
    if (beast->y.fixed < beast->groundHeight) {
      if (((struct FieldEffect *)updated)->countdown != 0) {
        Sound_Play(SE_SPLASH);
        ((struct FieldEffect *)updated)->countdown = 0;
        // shakeFrames, set through the field itself: four frames of shake.
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
    Actor_SelectAnimation(updated, WATER_BEAST_ANIMATION_LEAP);
    beast->velocityY = toFIXED(3);
  }
  if (++beast->timer == WATER_BEAST_HOP_PERIOD)
    beast->timer = 0;
}

/// The beast lands with a splash, shaking the screen by amplitude pixels for
/// holdFrames.
static inline void AltinPeak_PlayLandingShake(s32 amplitude, s32 holdFrames) {
  Sound_Play(SE_SPLASH);
  MapGrid_SetShakeFX(toFIXED(amplitude), 0, FIXED_ONE);
  Event_Wait(holdFrames);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/// Walking up the passage to the water beast in its cave (cells (17, 24) to
/// (19, 24)) makes it show the way: it cries out, hops north into the cave to
/// the gap between the two ledges, freezes the puddle in the gap with Frost,
/// hops round by the east ledge and across the ice to the west ledge, and
/// cries again.
///
/// It happens once (FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP). The beast then
/// keeps turning to face Isaac. The ice stays a square to hop on, at cell (18,
/// 13), so Isaac can follow it across.
static void AltinPeak_ShowFirstBeastRoute(void) {
  struct Actor *beast;
  struct Actor *leader;
  FIXED jumpSpeed;

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
  beast->gravity = toFIXED(2) / 5;
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
  // It freezes the puddle in the gap before hopping across it.
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
  Actor_Get(ACTOR_FIRST_WATER_BEAST)->velocityY = toFIXED(4);
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
  jumpSpeed = toFIXED(5);
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
  GameFlag_Set(FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP);
  Event_End();
}

/// When Isaac reaches the east ledge at the edge of the gap (cells (21, 13)
/// and (21, 14)), the water beast across the ice cries out, hops away to the
/// west end of its ledge and settles there to spit water east.
///
/// It happens once (FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY), and the beast
/// spits there until its battle. The view stays on it for one to two
/// seconds, less if a button is pressed, before going back to Isaac.
static void AltinPeak_FirstBeastHopsAway(void) {
  struct Actor *actor;
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
  actor->gravity = toFIXED(2) / 5;
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
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WATER_BEAST), 0);
  Sound_Play(SE_HIGH_JUMP);
  Actor_Get(ACTOR_FIRST_WATER_BEAST)->velocityY = toFIXED(4);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
  Actor_MoveToAndWait(ACTOR_FIRST_WATER_BEAST, 86, 214);
  Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_WATER_BEAST), SPRITE_FLAG_SHADOW);
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
  GameFlag_Set(FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY);
  Event_End();
}

/// Sets up the room being entered, with what the party has done there put
/// back and its water as low as the drains have left it. Returns 0.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.next == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
    AltinPeak_RestoreFirstWaterBeastRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
    AltinPeak_RestoreSecondWaterBeastRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
    AltinPeak_RestoreSouthEntranceRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
    AltinPeak_RestoreThirdWaterBeastRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_POOL) {
    AltinPeak_RestorePoolRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_SPRITZ) {
    AltinPeak_RestoreSpritzRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
    AltinPeak_RestoreFallingRocksRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
    AltinPeak_RestoreHydrosStatueRoom();
  } else if (gMainRoutineState.next == SCENE_ALTIN_PEAK_WEST_EXIT) {
    AltinPeak_RestoreWestExitRoom();
  }
  return 0;
}

/// Sets up the first water beast's cave: back from its battle, the Frost
/// Jewel and the drain; before the drain, the beast where it last went; after
/// it, the two puddles, frozen if Frost has frozen them on this visit.
static void AltinPeak_RestoreFirstWaterBeastRoom(void) {
  struct Actor *actor;
  struct MapGridScrollState *grid;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineState.entrance == ENTRANCE_AFTER_BATTLE) {
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
    if (GameFlag_Get(FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY) != 0) {
      // SPRITE_PUDDLE's first animation holds the ice pillar.
      Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
                         ANIM_STILL);
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->update =
          FieldEffect_UpdateFrostGlint;
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->y.fixed =
          toFIXED(32);
      MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
      Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 120, 215);
      Actor_Get(ACTOR_FIRST_WATER_BEAST)->facing = FACING_EAST;
      Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
      AltinPeak_StartSpitting(130, 0, 168, FACING_EAST);
      return;
    }
    if (GameFlag_Get(FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP) == 0) {
      return;
    }
    Actor_SetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, ANIM_STILL);
    Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->update =
        FieldEffect_UpdateFrostGlint;
    Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlags = 0;
    Actor_Get(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->y.fixed =
        toFIXED(32);
    MapGrid_CopyCellAttributes(2, 0, 1, 1, 18, 13);
    Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 260, 215);
    FieldActor_WatchAndStartScript(ACTOR_FIRST_WATER_BEAST,
                                   ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gAltinPeak_FaceLeaderScript);
  } else {
    grid = gMapGridState[0];
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
      actor->y.fixed = toFIXED(32);
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
  }
}

/// Sets up the lake room: back from its battle, the drain; before it, the
/// beast spitting water at the lake; and the switch's arrow and the fork as
/// last flipped.
static void AltinPeak_RestoreSecondWaterBeastRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollState *)gMapGridState[0])->flags &= ~DISPCNT_BG1_ON;
    Actor_SetPosition(ACTOR_SECOND_WATER_BEAST, 0, 0);
  } else {
    AltinPeak_StartPaletteAnimation();
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
        gMainRoutineState.entrance == ENTRANCE_AFTER_BATTLE) {
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

/// Sets up the south entrance: its flooded chamber dries with the second
/// drain, and coming in from the Alpine Crossing shows no area name.
static void AltinPeak_RestoreSouthEntranceRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
    ((struct MapGridScrollState *)gMapGridState[0])->flags &= ~DISPCNT_BG1_ON;
  } else {
    MapGrid_CopyCellAttributes(52, 42, 1, 1, 53, 42);
  }
  // Either entrance from the Alpine Crossing.
  if (gMainRoutineState.entrance ==
          ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST ||
      gMainRoutineState.entrance ==
          ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
}

/// Sets up the third water beast's room: back from its battle, the drain;
/// before it, the beast spitting water; the frozen puddle; and the switch.
static void AltinPeak_RestoreThirdWaterBeastRoom(void) {
  if (GameFlag_Get(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
    ((struct MapGridScrollState *)gMapGridState[0])->flags &= ~DISPCNT_BG1_ON;
    Actor_SetPosition(ACTOR_THIRD_WATER_BEAST, 0, 0);
  } else {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      if (gMainRoutineState.entrance == ENTRANCE_AFTER_BATTLE) {
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

/// Sets up the pool cave: the mine cart waiting at (358, 104) when the party
/// comes back up from the south-west part, the frozen puddle, the pool gone
/// after the third drain, and the switch.
static void AltinPeak_RestorePoolRoom(void) {
  struct Actor *pillar;

  if (gMainRoutineState.entrance == ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST) {
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
    ((struct MapGridScrollState *)gMapGridState[0])->flags &= ~DISPCNT_BG1_ON;
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

/// Sets up Spritz's room: the stone pillar where it stands, the three puddles
/// frozen on this visit, and the switch.
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

/// Sets up the falling-rocks passage: the upright log and the sign, and once
/// the boulder has fallen, the log lying toppled and the doorway broken open.
/// Climbing up the ladder from the Hydros statue's room, Isaac comes up out
/// of the hole.
static void AltinPeak_RestoreFallingRocksRoom(void) {
  struct Actor *actor;

  actor = Actor_Get(ACTOR_STANDING_LOG);
  Actor_SetPosition(ACTOR_FALLING_BOULDER, 0, 0);
  Actor_SetPosition(ACTOR_BOULDER_SHADOW, 0, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BOULDER_SHADOW), 0);
  actor->motionFlags = 0;
  actor->scaleX = 9 * FIXED_ONE / 10;
  actor->scaleY = 3 * FIXED_ONE / 5;
  actor->sprite->rotation = DEGtoANG(180);
  Actor_Get(ACTOR_FALLING_ROCK_SIGN)->motionFlags = 0;
  Actor_Get(ACTOR_FALLING_ROCK_SIGN)->y.fixed = -toFIXED(28);
  if (Event_GetFlag(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
    actor->x.fixed += toFIXED(14);
    actor->y.fixed -= toFIXED(8);
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
  if (gMainRoutineState.entrance ==
          ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE &&
      !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    Event_Start();
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(88);
    FieldCamera_MoveTo(792, -88, 577, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
    Event_End();
  }
}

/// Sets up the Hydros statue's room: the statue stands on its dais until it is
/// beaten, and back from its battle the party talks it over.
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
    // statuesDefeated is 0 here: nothing about the shadow is solid.
    Actor_Get(ACTOR_STATUE_SHADOW)->collisionFlags = statuesDefeated;
  }
  entrance = gMainRoutineState.entrance;
  if (entrance == ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS ||
      entrance == ENTRANCE_ROOM_START) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      Event_Start();
      leader->y.fixed = toFIXED(16);
      Event_End();
    }
  } else if (entrance == ENTRANCE_AFTER_BATTLE) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      AltinPeak_CelebrateHydrosVictory();
    }
  }
}

/// Sets up the west exit: coming back out of the Vial's chest room, the
/// stone pillar stands a cell west of its first place, at (184, 328).
static void AltinPeak_RestoreWestExitRoom(void) {
  Actor_SetSpriteFlags(Actor_Get(ACTOR_WEST_EXIT_ROOM_PILLAR), 0);
  if (gMainRoutineState.entrance ==
      ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM) {
    Actor_SetPosition(ACTOR_WEST_EXIT_ROOM_PILLAR, 184, 328);
  }
}

/// Gives a new drop of spit its look. It stands still, on sprite priority 1
/// and at 0.3 scale.
static void AltinPeak_InitializeSpit(struct Actor *spit) {
  spit->motionFlags = 0;
  spit->sprite->image.priority = 1;
  Actor_SetSpriteColor(spit, SPRITE_COLOR_BGR);
  Actor_SetSpriteFlags(spit, 0);
  spit->scaleX = 3 * FIXED_ONE / 10;
  spit->scaleY = 3 * FIXED_ONE / 10;
}

/// A drop of spit's update routine: it drifts the way the beast spits,
/// slowing as it goes, with a random wobble, and moves down the screen as it
/// grows; in its last few frames it spreads flat and shoots on half a pixel a
/// frame. It flashes white the more often the nearer its end, and is
/// released after its countdown.
static void AltinPeak_DriftSpit(struct Actor *spit) {
  switch (gAltinPeak_SpitDirection) {
  case FACING_EAST:
    spit->x.fixed += spit->timer * (toFIXED(1) / 16) +
                     ((s32)toFIXED(Random_Below(2) - 1) >> 1);
    break;
  case FACING_WEST:
    spit->x.fixed -= spit->timer * (toFIXED(1) / 16) +
                     ((s32)toFIXED(Random_Below(2) - 1) >> 1);
    break;
  }
  if (spit->timer <= 3) {
    switch (gAltinPeak_SpitDirection) {
    case FACING_EAST:
      spit->x.fixed += toFIXED(1) / 2;
      break;
    case FACING_WEST:
      spit->x.fixed -= toFIXED(1) / 2;
      break;
    }
    spit->scaleX += FIXED_ONE / 10;
    spit->scaleY -= FIXED_ONE / 20;
  } else {
    spit->z.fixed += toFIXED(6) / 5;
    spit->scaleX += 3 * FIXED_ONE / 100;
    spit->scaleY += 3 * FIXED_ONE / 100;
  }
  if (Random_Below(spit->timer) == 0)
    Actor_SetSpriteColor(spit, SPRITE_COLOR_WHITE);
  if (spit->timer != 0)
    spit->timer -= 2;
  else
    spit->timer = (Random_Below(5)) * 2 + 2;
  if (--((struct FieldEffect *)spit)->countdown == 0)
    Actor_Release(spit);
}

/// Task while a beast spits: every fourth frame a drop of spit starts from its
/// spout, living 20 frames.
static void AltinPeak_SpawnSpit(void) {
  struct Actor *spit;

  if ((gFrameCount & 3) != 0)
    return;
  spit = (struct Actor *)Actor_Create(
      SPRITE_DUST_PUFF, gAltinPeak_SpitOrigin[0], gAltinPeak_SpitOrigin[1],
      gAltinPeak_SpitOrigin[2]);
  if (spit == NULL)
    return;
  spit->timer = 30;
  spit->userValue = 1;
  ((struct FieldEffect *)spit)->countdown = 20;
  AltinPeak_InitializeSpit(spit);
  ((struct FieldEffect *)spit)->update = AltinPeak_DriftSpit;
  Actor_SelectAnimation(spit, DUST_PUFF_ANIMATION_QUICK);
}

/// A water beast starts spitting water from (x, y, z), in 16.16 fixed point,
/// toward facing (FACING_EAST or FACING_WEST), with its sound
/// (SE_AMBIENT_WATER).
static void AltinPeak_StartSpittingFX(FIXED x, FIXED y, FIXED z, s32 facing) {
  SET_VECTOR(gAltinPeak_SpitOrigin, x, y, z);
  gAltinPeak_SpitDirection = facing;

  Scene_QueueMusic(SE_AMBIENT_WATER);
  Task_AddFrame(AltinPeak_SpawnSpit, TASK_PASS_FRAME);
}

/// Task while the boulder crashes into the doorway: every third frame a piece
/// of rock of random size flies up from the crash and falls, and every ninth a
/// crash or a clatter sounds.
static void AltinPeak_SpawnDebris(void) {
  struct Actor *debris;
  FIXED scale;

  if (gFrameCount % 3 == 0) {
    debris =
        FieldActor_Create(SPRITE_ROCK, toFIXED(Random_Below(48)) + toFIXED(765),
                          -toFIXED(64), toFIXED(608));
    if (debris != NULL) {
      if (gFrameCount % 9 == 0) {
        if (Random_Below(2) != 0)
          Sound_Play(SE_CRASH);
        else
          Sound_Play(SE_CLATTER);
      }
      debris->motionFlags = 0;
      scale = (Random_Below(32768)) + 3 * FIXED_ONE / 10;
      debris->gravity = toFIXED(2) / 5;
      debris->scaleY = scale;
      debris->scaleX = scale;
      debris->frozen = 1;
      Actor_SetSpriteFlags(debris, 0);
      debris->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      debris->sprite->image.priority = 1;
      Actor_SelectAnimation(debris, ANIM_STAND);
      Actor_StartScript(debris, gAltinPeak_DebrisScript);
      SET_VELOCITY(debris, toFIXED(Random_Below(6) - 3), toFIXED(8),
                   (Random_Below(1536)) - 768);
    }
  }
}

/// The glint of the ice the first beast leaves in the gap of its cave.
DEFINE_FROST_GLINT(static, Actor_SetSpriteColor)

/// The first water beast casts Frost on the puddle in the gap of its cave, the
/// screen turning red as when the party casts it, and the puddle freezes into
/// an ice pillar.
///
/// The beast's colours are set back to its green and blue swapped after each
/// step of the cast.
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

/// Back from the first water beast's battle, on the west ledge where it last
/// stood: the beast is gone, "Isaac got the Frost Jewel.", and the view moves
/// over the cave as the water drains away (AltinPeak_DrainFirstWater).
static void AltinPeak_AwardFrostJewelAndDrain(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  Event_Start();
  Actor_SetPosition(ACTOR_FIRST_WATER_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
  gFieldState->transition =
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
  FieldCamera_MoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
  FieldCamera_WaitForMove();
  Event_SetTriggers(gAltinPeak_FirstWaterBeastTriggersDrained);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Back from the second water beast's battle: the beast is gone, the mine
/// cart waits where its ride ended, at (328, 424), the switch is back in its
/// first position, and the water drains (AltinPeak_DrainSecondWater).
static void AltinPeak_DrainAfterSecondBeast(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_SECOND_WATER_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_CART, 328, 424);
  GameFlag_Set(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleSecondWaterBeastRoomSwitchCells();
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  AltinPeak_DrainSecondWater();
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Back from the third water beast's battle, the same way: the beast is gone,
/// the cart waits at (488, 138), the switch is back in its first position, and
/// the last water drains (AltinPeak_DrainThirdWater).
static void AltinPeak_DrainAfterThirdBeast(void) {
  Event_Start();
  Actor_SetPosition(ACTOR_THIRD_WATER_BEAST, 0, 0);
  Actor_SetPosition(ACTOR_CART, 488, 138);
  GameFlag_Set(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
  AltinPeak_ToggleThirdWaterBeastRoomSwitchCells();
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  AltinPeak_DrainThirdWater();
  gFieldState->transition =
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

/// Climbing the steps onto the dais before the guardian statue in the ruins'
/// room (cells (19, 7) to (21, 7)) wakes the Hydros Statue: Isaac walks up to
/// it, it roars and slides toward him with the floor rumbling, and its battle
/// starts.
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
  Actor_SetAnimation(ACTOR_STATUE, HYDROS_STATUE_ANIMATION_ADVANCE);
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

/// Task while the statue sinks. It flickers white and grey, every other
/// frame.
static void AltinPeak_FlickerStatue(void) {
  if (((gFrameCount >> 1) & 1) != 0) {
    Actor_SetSpriteColorById(ACTOR_STATUE, SPRITE_COLOR_WHITE);
  } else {
    Actor_SetSpriteColorById(ACTOR_STATUE, SPRITE_COLOR_GREY);
  }
}

/// Back from the Hydros Statue's battle, Garet, Ivan and Mia step out beside
/// Isaac on the dais. "We did it, Isaac! We beat the last water beast!" Ivan
/// wonders why Altin's guardian statues turned into monsters, and Mia says
/// they were made long, long ago. The statue flashes white and sinks into the
/// floor, Ivan spots a treasure chest behind it, and Garet says they should
/// check it.
static void AltinPeak_CelebrateHydrosVictory(void) {
  struct Actor *leader;
  struct FieldState *state;
  TaskFunc update;
  const s32 *celebrate;
  const s32 *party;

  Event_Start();
  Actor_SetPosition(ACTOR_STATUE, 328, 88);
  Actor_SetPosition(ACTOR_STATUE_SHADOW, 328, 88);
  Actor_SetAnimation(ACTOR_STATUE, HYDROS_STATUE_ANIMATION_STONE);
  state = gFieldState;
  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  state->transitionFrames = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPositionFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
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
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  gFieldState->transitionFrames = MAP_TRANSITION_FRAMES;
  GameFlag_Set(FLAG_HYDROS_STATUES_DEFEATED);
  Event_End();
}

/// Unused: asks the field to have Felix use Frost. Nothing calls it.
UNUSED static void AltinPeak_RequestPsynergy(void) {
  // XXX (no evidence): why the user is Felix, who never joins the party in
  // this game; nothing calls this, so no caller shows what it was for.
  gFieldState->psynergy = FIELD_PSYNERGY(FELIX, ABILITY_FROST);
}

/// Task in Spritz's room: while Isaac stands higher than 48 pixels, the ice
/// pillar of the north-west puddle is drawn under the party and the one north
/// of the rails further back.
static void AltinPeak_UpdateFrostPillarPriority(void) {
  if (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed > toFIXED(48)) {
    Actor_Get(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetSpritePriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 3);
  } else {
    Actor_SetSpritePriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 2);
  }
}

/// Lets the party pass through a puddle's actor and makes its map cell a wall
/// instead, until Frost gives the cell a hop square.
static void AltinPeak_MarkActorTerrainCell(s32 actorId) {
  struct Actor *actor;

  actor = Actor_Get(actorId);
  actor->collisionFlags &= (u8)~ACTOR_SOLID;

  MapGrid_SetCellBehaviorAt(0, actor->x.fixed, actor->z.fixed,
                            CELL_BEHAVIOR_WALL);
}

/// The mine cart reaches the end of its line with a crunch: it settles in the
/// middle of its cell and Isaac jumps out of it to the south.
static void AltinPeak_StopCartAndJumpOff(struct Actor *actor) {
  s32 x;
  s32 z;
  FIXED centerZ;
  struct Sprite *sprite;
  struct Actor *leader;

  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_CRUNCH);
  x = actor->x.fixed & FIELD_CELL_MASK;
  centerZ = toFIXED(8);
  z = actor->z.fixed & FIELD_CELL_MASK;
  actor->acceleration = toFIXED(2);
  x += centerZ;
  centerZ += z;
  FieldActor_SetMoveTargetFX(actor, x, actor->y.fixed, centerZ);
  Actor_WaitForMove(actor);
  actor->layer = 0;
  actor->x.fixed = x;
  actor->z.fixed = centerZ;
  actor->velocityX = 0;
  actor->velocityZ = 0;
  Actor_SelectAnimation(actor, MINE_CART_ANIMATION_ROLL);
  wait(15);
  Actor_SelectAnimation(actor, ANIM_STAND);
  wait(30);
  sprite = actor->sprite;
  sprite->count = 1;
  SpriteAnimation_Clear(sprite->entries[1]);
  sprite->entries[1] = NULL;
  sprite->dirty = 1;
  leader = Actor_Lookup(gMainRoutineState.leaderActorId);
  Sound_Play(SE_JUMP);
  leader->x.fixed = x;
  leader->velocityY = toFIXED(6);
  leader->gravity = toFIXED(1);
  leader->z.fixed = centerZ;
  leader->sprite->image.priority = 0;
  Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
  FieldActor_SetMoveTargetFX(leader, x, leader->y.fixed, z + toFIXED(24));
  leader->facing = FACING_SOUTH;
  wait(20);
  leader->sprite->image.priority = 2;
  Sound_Play(SE_DOOR_CLOSE);
}

/// Unused: does nothing, and nothing calls it.
UNUSED static void AltinPeak_ReservedNoOp(void) {}

/// The cart turns a quarter turn right around the corner it reaches.
static void AltinPeak_TurnCartRight(struct Actor *actor) {
  struct FieldPosition position;
  s32 angle;
  s32 x;
  s32 z;
  s32 turnFrame;

  angle = FACING_QUARTER(actor->facing + DEGtoANG(90));
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffset(24, angle, &position);
  x = (position.x + toFIXED(8)) & FIELD_CELL_MASK;
  z = (position.z + toFIXED(8)) & FIELD_CELL_MASK;
  angle += DEGtoANG(180);
  Actor_SelectAnimation(actor, MINE_CART_ANIMATION_TURN_RIGHT);
  Sound_Play(SE_CART_TURN);
  for (turnFrame = 0; turnFrame < 16; turnFrame++) {
    angle += TRIG_TURN / 64;
    position.x = x;
    position.z = z;
    FieldVector_AddPolarOffset(24, angle, &position);
    actor->x.fixed = position.x;
    actor->z.fixed = position.z;
    actor->facing = angle + DEGtoANG(90);
    wait(1);
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

  angle = FACING_QUARTER(actor->facing - DEGtoANG(90));
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffset(24, angle, &position);
  x = (position.x + toFIXED(8)) & FIELD_CELL_MASK;
  z = (position.z + toFIXED(8)) & FIELD_CELL_MASK;
  angle += DEGtoANG(180);
  Actor_SelectAnimation(actor, MINE_CART_ANIMATION_TURN_LEFT);
  Sound_Play(SE_CART_TURN);
  for (turnFrame = 0; turnFrame < 16; turnFrame++) {
    angle -= TRIG_TURN / 64;
    position.x = x;
    position.z = z;
    FieldVector_AddPolarOffset(24, angle, &position);
    actor->x.fixed = position.x;
    actor->z.fixed = position.z;
    actor->facing = angle - DEGtoANG(90);
    wait(1);
  }
  Sound_Play(SE_CART_ROLL);
}

/// A countdown cell's frames: SE_CART_TURN plays with CART_COUNTDOWN_TURN
/// frames left, from when the cart's slope animation is picked again, and
/// SE_CART_ROLL at the end.
enum { CART_COUNTDOWN_FRAMES = 33, CART_COUNTDOWN_TURN = 20 };

/// The cart rolls over a countdown cell. SE_STOP_ACTION_SE plays, and
/// SE_CART_TURN and SE_CART_ROLL follow as its countdown runs out.
static void AltinPeak_StartCartCountdown(struct Actor *actor) {
  actor->userValue = CART_COUNTDOWN_FRAMES;
  Sound_Play(SE_STOP_ACTION_SE);
}

/// Rolls the cart on by a frame and picks its animation from the terrain ahead
/// of and behind it: flat, uphill or downhill.
static void AltinPeak_AdvanceCart(struct Actor *actor) {
  s32 angle = TRIG_TURN * 3 / 4;
  FIXED speed;
  s16 *timer;
  struct FieldPosition position;
  FIXED ahead;
  FIXED behind;

  angle &= actor->facing;
  speed = actor->y.fixed / FIXED_ONE;
  speed = gAltinPeak_CartSpeeds[actor->timer - speed + 16];
  timer = (s16 *)&actor->userValue;
  if (*timer != 0) {
    if (--*timer == CART_COUNTDOWN_TURN) {
      Sound_Play(SE_CART_TURN);
    }
    if (*timer == 0) {
      Sound_Play(SE_CART_ROLL);
    }
  }
  SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  FieldVector_AddPolarOffsetFX(Iwram_MulFX(speed, 3 * FIXED_ONE / 4), angle,
                               &position);
  actor->x.fixed = position.x;
  actor->z.fixed = position.z;
  ahead =
      MapGrid_GetTerrainHeight(MAPGRID_LAYER_OVERHEAD, position.x, position.z);
  FieldVector_AddPolarOffsetFX(-Iwram_MulFX(speed, 3 * FIXED_ONE / 2), angle,
                               &position);
  behind =
      MapGrid_GetTerrainHeight(MAPGRID_LAYER_OVERHEAD, position.x, position.z);
  if (*timer <= CART_COUNTDOWN_TURN) {
    if (ahead == behind) {
      Actor_SelectAnimation(actor, MINE_CART_ANIMATION_ROLL);
    } else if (ahead > behind) {
      Actor_SelectAnimation(actor, MINE_CART_ANIMATION_UPHILL);
    } else {
      Actor_SelectAnimation(actor, MINE_CART_ANIMATION_DOWNHILL);
    }
  }
}

/// The cell behaviours that mark the mine cart's route, on the overhead layer
/// (map rows 64 and up): its turns, a countdown before a turn, and its stop.
enum AltinPeakCellBehavior {
  CELL_BEHAVIOR_ALTIN_PEAK_CART_COUNTDOWN =
      96, ///< AltinPeak_StartCartCountdown
  CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_LEFT = 97,
  CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_RIGHT = 98,
  /// The end of the route, where the leader jumps off.
  CELL_BEHAVIOR_ALTIN_PEAK_CART_STOP = 99
};

/// Rolls the mine cart Isaac rides, the view following it, from where it
/// stands along the rails until its stop.
///
/// heading is the way it sets off, or -1 for the way it faces; if no rail
/// leads that way, the next quarter turns are tried. Each frame it reads the
/// route's cell under it (AltinPeakCellBehavior) and rolls faster downhill
/// (gAltinPeak_CartSpeeds).
static void AltinPeak_RollCart(s32 id, s32 heading) {
  struct Actor *actor = Actor_Get(id);
  struct Actor *leader = Actor_Lookup(gMainRoutineState.leaderActorId);
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
    if (MapGrid_GetTerrainHeight(MAPGRID_LAYER_OVERHEAD, position.x,
                                 position.z) == actor->y.fixed) {
      break;
    }
    heading += DEGtoANG(90);
  }
  if (directionIndex == 4) {
    return;
  }
  actor->layer = MAPGRID_LAYER_OVERHEAD;
  leader->x.fixed = 0;
  leader->z.fixed = 0;
  Sprite_AddAnimation(actor->sprite, SPRITE_CART_RIDER);
  FieldCamera_FollowActor(id, 1);
  FieldCamera_WaitForMove();
  FieldCamera_SetSpeed(16);
  actor->facing = heading;
  actor->speedLimit = toFIXED(2);
  actor->acceleration = toFIXED(1) / 20;
  actor->paused = 0;
  actor->timer = actor->y.fixed / FIXED_ONE;
  actor->userValue = 0;
  position.x = actor->x.fixed;
  position.y = actor->y.fixed;
  position.z = actor->z.fixed;
  FieldVector_AddPolarOffset(24, heading, &position);
  FieldActor_SetMoveTargetFX(actor, position.x, actor->y.fixed, position.z);
  Actor_WaitForMove(actor);
  Sound_Play(SE_CART_ROLL);
  // The route cells select turns, a countdown sound, and the destination.
  for (;;) {
    switch (MapGrid_GetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD, actor->x.fixed,
                                      actor->z.fixed)) {
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
  u16 *palette = gPaletteFadeState->palette;

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
    Dma_Set(palette, (void *)&BG_PLTT_COLOR(0, 3), DMA_COPY_HALFWORDS(9),
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

/// The first water beast keeps turning to face Isaac.
s32 gAltinPeak_FaceLeaderScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The puddle that appears by the west ladder as the first drain lowers the
/// water grows to full size over 108 frames.
s32 gAltinPeak_PuddleGrowScript[] = {
    SC_ADD(SW_SCALE_X, 546),
    SC_ADD(SW_SCALE_Y, 546),
    SC_WAIT(1),
    SC_LOOP(108, SC_TOP),
    SC_END,
};

/// The falling boulder rolls straight down the passage, over the bends Isaac
/// runs round, and marks its arrival.
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
    SC_ANIMATION(ANIM_LEADER_RUN),
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

/// The Hydros statue and its shadow flatten into the floor over 60 frames.
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
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 456, 0, 488, FACING_NORTH, 0, 0, 56, 506, 530, 0},
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
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
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
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
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
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
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
    ACTOR_PLACEMENT(SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 784, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 774, 0, 76, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 774, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 792, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {TRIGGER_EXIT, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_FIRST_BEAST, FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP, EVENT(AltinPeak_ShowFirstBeastRoute)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_EAST_LEDGE_BY_GAP, FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY, EVENT(AltinPeak_FirstBeastHopsAway)},
    {TRIGGER_TALK, ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS, EVENT(AltinPeak_FightFirstWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(AltinPeak_FightFirstWaterBeast)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_FirstWaterBeastTriggersDrained[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN,
        EVENT(AltinPeak_FreezeFirstWaterBeastRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN, EVENT(AltinPeak_FreezeFirstWaterBeastRoomSecondPuddle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_RailTunnelTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {TRIGGER_EXIT, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_SecondWaterBeastTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM},
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {TRIGGER_TALK, ACTOR_SECOND_WATER_BEAST, CONDITION_ALWAYS, EVENT(AltinPeak_FightSecondWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_SECOND_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(AltinPeak_FightSecondWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SECOND_WATER_BEAST_ROOM_BOULDER, CONDITION_ALWAYS, 0},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_LAKE_SWITCH, CONDITION_ALWAYS, EVENT(AltinPeak_FlipSecondWaterBeastRoomSwitch)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {TRIGGER_TABLE_END},
    // Unused: two rows past the end marker, which no lookup reaches: a second
    // ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE exit and a chest
    // holding a Herb under FLAG_ALTIN_PEAK_UNUSED_HERB, at the spot where the
    // Cookie's chest above already stands.
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_UNUSED_HERB, FIND_ITEM(ITEM_HERB)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_SouthEntranceTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_ThirdWaterBeastTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL, CONDITION_ALWAYS, ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(AltinPeak_FreezeThirdWaterBeastRoomPuddle)},
    {TRIGGER_TALK, ACTOR_THIRD_WATER_BEAST, CONDITION_ALWAYS, EVENT(AltinPeak_FightThirdWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_THIRD_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(AltinPeak_FightThirdWaterBeast)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_SWITCH, CONDITION_ALWAYS, EVENT(AltinPeak_FlipThirdWaterBeastRoomSwitch)},
    // The Dragon Shield's chest opens only from its west side.
    {EXAMINE_CHEST_FACING(FACING_SOUTHEAST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {EXAMINE_CHEST_FACING(FACING_EAST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {EXAMINE_CHEST_FACING(FACING_NORTHEAST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_PoolTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_POOL_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(AltinPeak_FreezePoolRoomPuddle)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_SWITCH, CONDITION_ALWAYS, EVENT(AltinPeak_FlipPoolRoomSwitch)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_POOL_MIMIC_BEATEN, FIND_MIMIC | 2},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_SpritzTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION},
    {TRIGGER_EXIT, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS},
    {TRIGGER_EXIT, ALTIN_PEAK_SPRITZ_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_PSY_CRYSTAL_CHEST, FLAG_ALTIN_PEAK_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL)},
    {TRIGGER_TALK, ACTOR_SPRITZ, CONDITION_ALWAYS, EVENT(AltinPeak_TalkToDjinni)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(AltinPeak_RideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_SWITCH, CONDITION_ALWAYS, EVENT(AltinPeak_FlipSpritzRoomSwitch)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_LADDER_TOP, CONDITION_ALWAYS, EVENT(AltinPeak_PlacePillarUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_LADDER_FOOT, CONDITION_ALWAYS, EVENT(AltinPeak_RestorePillarAutomaticPriority)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SPRITZ_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(AltinPeak_SettleSpritzRoomPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ALTIN_PEAK_WEST_OF_PILLAR, CONDITION_ALWAYS,
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
    {TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ},
    {TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH,
     CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH},
    {TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT,
     CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT},
    {TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN, CONDITION_ALWAYS,
     ALTIN_PEAK_JUNCTION_TO_ALTIN},
    {TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH,
     CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST,
     FLAG_ALTIN_PEAK_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_JUNCTION_ROOM_BOULDER,
     CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_FallingRocksTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ,
     CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ},
    {TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST},
    {TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH,
     CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH},
    {TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST, CONDITION_ALWAYS,
     ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST},
    {TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH,
     CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH},
    {EXAMINE_TRIGGER(EXAMINE_SILENT),
     CELL_BEHAVIOR_ALTIN_PEAK_FALLING_ROCK_SIGN, CONDITION_ALWAYS,
     EVENT(AltinPeak_ReadFallingRockSign)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_WALL,
     FLAG_ALTIN_PEAK_BOULDER_FALLEN, EVENT(AltinPeak_ForceLogOver)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_HydrosStatueTriggers[] = {
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_HYDROS_STATUE,
     FLAG_HYDROS_STATUES_DEFEATED, EVENT(AltinPeak_StartHydrosBattle)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST,
     FLAG_ALTIN_PEAK_LIFTING_GEM, FIND_ITEM(ITEM_LIFTING_GEM)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAltinPeak_WestExitTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION,
     CONDITION_ALWAYS, ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION},
    {TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM},
    {TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL},
    {TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP, CONDITION_ALWAYS,
     ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_WEST_EXIT_ROOM_BOULDER,
     CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_EXIT_ROOM_PILLAR,
     CONDITION_ALWAYS, 0},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST,
     FLAG_ALTIN_PEAK_VIAL, FIND_ITEM(ITEM_VIAL)},
    {TRIGGER_TABLE_END},
};

/// Frames since the statue's last rumble.
s32 gAltinPeak_RumbleFrames = 0;

/// The cart's speed by its height against where it set off, of which it rolls
/// three quarters a frame: 2 at that height, doubling for each 16 pixels lower
/// and halving for each 16 higher. Entry i is about 2 to the power i / 16.
FIXED gAltinPeak_CartSpeeds[] = {
    toFIXED(1),       toFIXED(1.04427), toFIXED(1.0905),  toFIXED(1.13878),
    toFIXED(1.1892),  toFIXED(1.24186), toFIXED(1.29683), toFIXED(1.35425),
    toFIXED(1.4142),  toFIXED(1.47683), toFIXED(1.54221), toFIXED(1.6105),
    toFIXED(1.68178), toFIXED(1.75625), toFIXED(1.834),   toFIXED(1.91521),
    toFIXED(2),       toFIXED(2.08855), toFIXED(2.181),   toFIXED(2.27758),
    toFIXED(2.37841), toFIXED(2.48371), toFIXED(2.59368), toFIXED(2.7085),
    toFIXED(2.82842), toFIXED(2.95365), toFIXED(3.08442), toFIXED(3.22098),
    toFIXED(3.36358), toFIXED(3.5125),  toFIXED(3.66802), toFIXED(3.8304),
    toFIXED(4),
};

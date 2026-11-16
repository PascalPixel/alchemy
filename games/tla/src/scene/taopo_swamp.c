/// @file
/// Taopo Swamp, north of Yallam: mud that swallows the party back to firm
/// ground, vents whose gusts throw it about, puddles Douse fills and Frost
/// freezes, lava rocks to cool and push, drops falling in the dark rooms, the
/// Mad Plant in its bush and Flower, the Venus Djinni.
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "types.h"
#include "version.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "field/field_sprite.h"
#include "field/scene_tables.h"
#include "graphics/graphics.h"
#include "scene/lemurian_ship.h"
#include "scene/scene.h"
#include "system/frame.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "system/vector3.h"

void Rope_Initialize(void);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);

/// The mud room's actors (SceneId_TaopoSwampMud).
enum TaopoSwampMudActorId {
  /// The five ivy bushes Whirlwind blows away (SPRITE_IVY), each kept gone by
  /// its flag, FLAG_TAOPO_SWAMP_BUSH_BLOWN plus its id.
  ACTOR_FIRST_BUSH = ACTOR_FIRST_PLACED,
  ACTOR_LAST_BUSH = ACTOR_FIRST_BUSH + 4,
  /// The sprout Growth grows into a vine (SPRITE_SPROUT).
  ACTOR_MUD_SPROUT,
  /// The stump Move pushes into the mud (SPRITE_STUMP).
  ACTOR_MUD_STUMP
};

/// Flower, the Venus Djinni: on its ledge among the vents, then, blown off it,
/// in its own room (SceneId_TaopoSwampFlower).
#define ACTOR_FLOWER ACTOR_FIRST_PLACED

/// The vents room's pillars Move pushes over a vent (SceneId_TaopoSwampVents).
enum TaopoSwampVentsActorId {
  ACTOR_WEST_VENT_PILLAR = ACTOR_FLOWER + 1,
  ACTOR_EAST_VENT_PILLAR
};

/// The pillar gap's actors (SceneId_TaopoSwampPillarGap).
enum TaopoSwampPillarGapActorId {
  /// The round spot over a vent that Douse fills and Frost freezes
  /// (SPRITE_PUDDLE).
  ACTOR_GAP_PUDDLE = ACTOR_FIRST_PLACED,
  /// The pillar Move drops into the gap.
  ACTOR_GAP_PILLAR
};

/// The drips room's puddle under the drops (SceneId_TaopoSwampDrips,
/// SPRITE_PUDDLE).
#define ACTOR_DRIP_PUDDLE (ACTOR_FIRST_PLACED + 2)

/// The lava rocks' room's two rocks (SceneId_TaopoSwampLavaRocks): by the chest
/// and by the exit.
enum TaopoSwampLavaRocksActorId {
  ACTOR_CHEST_ROCK = ACTOR_FIRST_PLACED,
  ACTOR_EXIT_ROCK
};

/// The Tremor room's actors (SceneId_TaopoSwampTremor).
enum TaopoSwampTremorActorId {
  ACTOR_TREMOR_LAVA_ROCK = ACTOR_FIRST_PLACED + 1,
  ACTOR_TREMOR_PILLAR_ROCK,
  /// The pillar Tremor shakes, and the boulder on it that rolls off.
  ACTOR_TREMOR_PILLAR,
  ACTOR_TREMOR_BOULDER = ACTOR_FIRST_PLACED + 11
};

/// The Star Dust's room's glowing rocks (SceneId_TaopoSwampStarDust).
enum TaopoSwampStarDustActorId {
  ACTOR_DARK_ROCK = ACTOR_FIRST_PLACED,
  ACTOR_LAST_ROCK
};

/// Whirlwind's flags for the mud room's bushes, one per actor id from
/// ACTOR_FIRST_BUSH.
#define FLAG_TAOPO_SWAMP_BUSH_BLOWN 0x8d8
/// Set once the mud room's stump has sunk.
#define FLAG_TAOPO_SWAMP_STUMP_SUNK 0x8e5
/// Set once Move has left the vents room's west or east pillar over its vent.
#define FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT 0xa6d
#define FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT 0xa6e
/// Set once the pillar has dropped into the gap.
#define FLAG_TAOPO_SWAMP_PILLAR_IN_GAP 0xa6f
/// Set once Scoop has dug up the Star Dust.
#define FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP FLAG_AREA_TEMP_48
/// Set once the Mad Plant hiding in the bush has been beaten.
#define FLAG_TAOPO_SWAMP_MAD_PLANT_BEATEN 0x9b6
/// Set once Growth has grown the mud room's sprout into a vine.
#define FLAG_TAOPO_SWAMP_VINE_GROWN FLAG_SCENE_TEMP_20
/// The round spot over a vent (ACTOR_GAP_PUDDLE, ACTOR_DRIP_PUDDLE): set while
/// it holds water, a puddle or an ice pillar.
#define FLAG_TAOPO_SWAMP_SPOT_WET FLAG_SCENE_TEMP_20
/// Set while Frost has turned the spot's water into an ice pillar.
#define FLAG_TAOPO_SWAMP_SPOT_FROZEN FLAG_SCENE_TEMP_21
/// Set while the spot holds a puddle Frost can freeze.
#define FLAG_TAOPO_SWAMP_SPOT_PUDDLE FLAG_SCENE_TEMP_22
/// The drops the drip room's spot has caught, up to four: a byte of flags.
#define FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT FLAG_SCENE_TEMP_30
/// Set once Scoop has dug up the item under treasure actor n (0 or 1) of the
/// tear stones' room.
#define FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(n) (FLAG_AREA_TEMP_00 + (n))
/// Set once Flower has been blown off its ledge into its own room.
#define FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE FLAG_AREA_TEMP_10
/// Set once Tremor has opened the passage and the pool has drained.
#define FLAG_TAOPO_SWAMP_PASSAGE_OPEN FLAG_AREA_TEMP_30
/// Each red rock's flags: _COOLED once Douse has cooled it, _UNMOVABLE while
/// Move cannot push it (until it is cooled, and once it has sunk), _SUNK once
/// it has sunk into the lava as a stepping stone.
#define FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED FLAG_AREA_TEMP_31
#define FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE FLAG_AREA_TEMP_32
#define FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK FLAG_AREA_TEMP_33
#define FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED FLAG_AREA_TEMP_34
#define FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE FLAG_AREA_TEMP_35
#define FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK FLAG_AREA_TEMP_36
#define FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED FLAG_AREA_TEMP_38
#define FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE FLAG_AREA_TEMP_39
#define FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK FLAG_AREA_TEMP_3A
#define FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED FLAG_AREA_TEMP_3B
#define FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE FLAG_AREA_TEMP_3C
#define FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK FLAG_AREA_TEMP_3D
#define FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED FLAG_AREA_TEMP_40
#define FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE FLAG_AREA_TEMP_41
#define FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED FLAG_AREA_TEMP_42
#define FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE FLAG_AREA_TEMP_43

void GameFlag_SetByte(s32 flag, s32 value);
void Vent_SetGust(s32 id, s32 a);
void Vents_SetId(s32 a, s32 b);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void Actor_StartScript(struct Actor *object, const void *data);

extern s32 gTaopoSwamp_SteamPuffs;
extern s32 gTaopoSwamp_PassageRows;
extern s32 gTaopoSwamp_PoolFrames;
extern u32 gTaopoSwamp_PuffSteamScript[];
void Field_TryJumpForward(void);
void TaopoSwamp_DarkenRoom(s32 a);
void TaopoSwamp_ShowMessage(s32 message);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 a);
s32 Vents_GetCounter(void);
#ifdef MODERN
void Vent_SetActive(s32 index, s32 active);
#endif
s32 Vents_AdvanceCounter(s32 a);

/// The mud room's state: whether the leader is in the mud, how deep they have
/// sunk (256 to a row of the sprite) and the last firm ground they stood on.
struct SceneState {
  s16 mode;
  s16 timer;
  s32 x;
  s32 z;
};

extern struct SceneState gSceneState;

typedef volatile u16 vu16;

/// A 16-bit I/O register.
struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
/// One vent: whether it blows, its place in the cycle, its position and its
/// gust's speed, 16.16 pixels.
struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

/// The vents of the room: their frame counter, the room's hook, up to sixteen
/// vents and the ids of up to four.
struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

/// MapWork as far as groundEffects, read through its bits, the footprints and
/// the kind of wading, as Shrine of the Sea God reads it.
struct MapWorkGroundEffects {
  u8 before[offsetof(struct MapWork, groundEffects)];
  u8 footprints : 2;
  u8 wading : 2;
  u8 unused : 4;
};

/// The mud room's kind of wading: the leader sinks into the mud.
enum { MAP_WADING_MUD = 2 };

extern u8 gInventorySnapshot[];
#define VENT_WORK ((struct VentWork *)(gInventorySnapshot + 0x188))
#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

void Vent_Bounce(struct Actor *object);
extern u32 gTaopoSwamp_PuffRises[];
extern s32 gTaopoSwamp_SparkleFrame;
void BlendScript_Suspend(void);
void TaopoSwamp_TwinkleStarDust(void);
void TaopoSwamp_FocusMapOn(s32 x, s32 z);
void TaopoSwamp_SaveFirmGround(void);
void TaopoSwamp_FillDripSpot(s32 a, s32 id);
extern s32 gTaopoSwamp_DripFromCeilingScript[];
extern s32 gTaopoSwamp_DropFalls[];
void TaopoSwamp_ShatterIcePillar(s32 id);
void TaopoSwamp_BlowAwayPuddle(s32 id);
extern u32 gTaopoSwamp_SparkFades[];
extern s32 gTaopoSwamp_DripAtRandomScript[];
extern u8 MsgIThoughtI;
void TaopoSwamp_PuffSteam(void);
void TaopoSwamp_OpenPassage(void);
void TaopoSwamp_FlickerPool(void);

/// DEFINE_TAOPO_SWAMP_FREEZE_SPOT below calls it by this name.
static __inline__ void TaopoSwamp_CopyCellAttributes(s32 a, s32 b, s32 c, s32 d,
                                                     s32 e, s32 f) {
  MapGrid_CopyCellAttributes(a, b, c, d, e, f);
}

static __inline__ void TaopoSwamp_JumpDownLedge(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

static __inline__ void TileCopies_CopyTouchedCells_(s32 a, s32 b, s32 c, s32 d,
                                                    s32 e, s32 f, s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ struct Actor *TaopoSwamp_CreateActor(s32 type, s32 x, s32 y,
                                                       s32 z) {
  return Import_ActorCreate(type, x, y, z);
}

static __inline__ void MadPlant_LeapAtLeader_(s32 a, s32 b, s32 c) {
  MadPlant_LeapAtLeader(a, b, c);
}

s32 Vent_IsActive(s32 hole);

void TaopoSwamp_FocusMapMiddle();
void TaopoSwamp_JumpForward(void);

void TaopoSwamp_GrowVine(void);

void TaopoSwamp_DigHoleUnderfoot(void);

void TaopoSwamp_StartMadPlantBattle(void);

void TaopoSwamp_TremorPillar(void);

void TaopoSwamp_SinkInMud(void);

void TaopoSwamp_RiseOnBubble(void);

void TaopoSwamp_StepOnFirmGround(void);

s32 MapGrid_CheckCollision(struct Actor *object, struct Vector3 *position);
void FieldEffect_StartBlankOverlay(s32 a);
void Rope_Place(s32 a, s32 flag, s32 b, s32 c);

/// Talking to Flower, the Venus Djinni, starts its battle.
void TaopoSwamp_FightFlower(void) {
  Field_RequestDjinnBattle(ACTOR_FLOWER, DJINNI_FLOWER);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gTaopoSwamp_Placements[];
extern struct ActorPlacement gTaopoSwampMudActorPlacements[];
extern struct ActorPlacement gTaopoSwampFlowerActorPlacements[];
extern struct ActorPlacement gTaopoSwampVentsActorPlacements[];
extern struct ActorPlacement gTaopoSwampPillarGapActorPlacements[];
extern struct ActorPlacement gTaopoSwampDripsActorPlacements[];
extern struct ActorPlacement gTaopoSwampTremorActorPlacements[];
extern struct ActorPlacement gTaopoSwampLavaRocksActorPlacements[];
extern struct ActorPlacement gTaopoSwampStarDustActorPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_TAOPO_SWAMP_MUD)
    return gTaopoSwampMudActorPlacements;
  if (scene == SCENE_TAOPO_SWAMP_FLOWER)
    return gTaopoSwampFlowerActorPlacements;
  if (scene == SCENE_TAOPO_SWAMP_VENTS)
    return gTaopoSwampVentsActorPlacements;
  if (scene == SCENE_TAOPO_SWAMP_PILLAR_GAP)
    return gTaopoSwampPillarGapActorPlacements;
  if (scene == SCENE_TAOPO_SWAMP_DRIPS)
    return gTaopoSwampDripsActorPlacements;
  if (scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS)
    return gTaopoSwampLavaRocksActorPlacements;
  if (scene == SCENE_TAOPO_SWAMP_TREMOR)
    return gTaopoSwampTremorActorPlacements;
  if (scene == SCENE_TAOPO_SWAMP_STAR_DUST)
    return gTaopoSwampStarDustActorPlacements;
  return gTaopoSwamp_Placements;
}

/// Whirlwind on one of the bushes 8 to 12: its flag
/// (FLAG_TAOPO_SWAMP_BUSH_BLOWN plus its id) keeps it gone.
void TaopoSwamp_BlowAwayBush(s32 a, s32 b) {
  GameFlag_Set(FLAG_TAOPO_SWAMP_BUSH_BLOWN + b);
}

/// Growth on the sprout, actor 13: the vine's cells become climbable.
void TaopoSwamp_GrowVine(void) {
  MapGrid_CopyCellAttributes(50, 16, 1, 2, 47, 16);
  Event_SetFlag(FLAG_TAOPO_SWAMP_VINE_GROWN);
}

struct ChildGroup;

extern u32 gTaopoSwamp_SplashFades[];
s32 Sprite_StartAnimation(struct Sprite *state, s32 flags);
void Sprite_SetAnimationColorModes(struct FieldSprite *record, u32 value);
void Sprite_SetAnimationSpeed(struct ChildGroup *group, s32 value);

// When actor id stands at x 12, inside an event: the actor drops in place, then
// for 25 frames throws splashes, then two sparks, and sets
// FLAG_TAOPO_SWAMP_STUMP_SUNK.
/// Move on the small pillar, actor 14, once it stands at cell x 12: it sinks
/// into the mud with splashes and two sparks, and FLAG_TAOPO_SWAMP_STUMP_SUNK
/// keeps it gone.
void TaopoSwamp_SinkPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *splash, *spark;
  struct FieldSprite *sprite, *splashSprite, *sparkSprite;
  s32 i;
  FIXED x, z;
  s32 k, zero;

  if (FIELD_CELL(actor->x.fixed) != 12)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  System_Wait(5);
  actor->motionFlags = 0;
  actor->velocityY = 0;
  actor->y.fixed = toFIXED(4);
  sprite = actor->sprite;
  i = 0;
  do {
    ((struct Sprite *)sprite)->sunkRows = i;
    sprite->flags |= 8;
    sprite->flags &= ~1;
    k = i;
    sprite->dirty = 1;
    k &= 3;
    if (k == 0) {
      x = actor->x.fixed + Random16() * 2 - Random16() * 2;
      z = actor->z.fixed + Random16() * 2 - Random16() * 2;
      splash = (FieldActor_Create(14, x, actor->y.fixed, z));
      if (splash != 0) {
        splashSprite = splash->sprite;
        splash->groundHeight = actor->groundHeight;
        Actor_StartScript(splash, gTaopoSwamp_SplashFades);
        splash->motionFlags = 0;
        if (splashSprite != 0) {
          ((s32 (*)())Sprite_StartAnimation)(splashSprite, 1);
          splashSprite->flags = k;
          splashSprite->priority = sprite->priority;
          Sprite_SetAnimationColorModes(splashSprite, 13);
          Sprite_SetAnimationSpeed(splashSprite, 8);
        }
      }
    }
    System_Wait(3);
    i++;
  } while (i <= 24);

  actor->spriteStorageKind = 0;
  actor->y.fixed = 0;
  for (i = 1; i >= 0; i--) {
    spark = (FieldActor_Create(269, actor->x.fixed, actor->y.fixed,
                               actor->z.fixed));
    if (spark != 0) {
      sparkSprite = spark->sprite;
      Actor_StartScript(spark, gTaopoSwamp_SplashFades);
      {
        u8 *flags = &spark->motionFlags;

        zero = 0;
        *flags = zero;
      }
      spark->layer = 1;
      if (sparkSprite != 0) {
        Sprite_StartAnimation(sparkSprite, 2);
        sparkSprite->flags = zero;
        sparkSprite->priority = 2;
      }
    }
    System_Wait(15);
  }
  FieldEvent_Wait(30);
  actor->spriteStorageKind = 1;
  Actor_SetPosition(id, 0, 0);
  Event_End();
  Event_SetFlag(FLAG_TAOPO_SWAMP_STUMP_SUNK);
}

/// Scoop on the spot under treasure actor 1 digs up its item.
void TaopoSwamp_ScoopHiddenItem(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR + 1,
                      FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1));
}

/// Scoop on the spot under treasure actor 0 digs up its item.
void TaopoSwamp_ScoopOtherHiddenItem(s32 a) {
  Event_DigUpTreasure(a, FIELD_TREASURE_ACTOR,
                      FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0));
}

/// Scoop digs into the mud at one of the two spots of the lowest level.
void TaopoSwamp_DigHole(void) {
  TileCopies_CopyTouchedCells_(76, 70, 14, 8, 14, 37, 0);
}

/// As TaopoSwamp_DigHole, and the first time (until flag 0xf99) the buried item
/// comes up, throwing the leader standing on it back a step.
void TaopoSwamp_DigHoleUnderfoot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  TileCopies_CopyTouchedCells_(76, 70, 14, 8, 14, 37, 0);
  if (Event_GetFlag(0xf99) == 0) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 264, 664);
    if (FIELD_CELL(leader->x.fixed) == 16 &&
        FIELD_CELL(leader->z.fixed) == 41) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -16);
      leader->velocityY = toFIXED(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Until FLAG_TAOPO_SWAMP_MAD_PLANT_BEATEN is set, the bush hides the Mad
/// Plant, which the party fights from encounter table 106.
void TaopoSwamp_StartMadPlantBattle(void) {
  TileCopies_CopyTouchedCells_(76, 70, 14, 8, 14, 37, 0);
  if (!Event_GetFlag(FLAG_TAOPO_SWAMP_MAD_PLANT_BEATEN)) {
    MadPlant_LeapAtLeader_(SPRITE_MAD_PLANT, 344, 648);
    gMainRoutineWork.pendingDjinni =
        IF_FLAG_SET(FLAG_TAOPO_SWAMP_MAD_PLANT_BEATEN);
    Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_MAD_PLANTS, 1);
  }
}

/// The leader drops through a hole in the floor to the room below.
void TaopoSwamp_FallThroughHole(s32 a) {
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
  TaopoSwamp_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0x1000);
  Event_PlaySound(SE_FALL);
  Event_SetFlag(FLAG_ARRIVE_FALLING);
  FieldEvent_SetExitRequest(a);
}

/// Move on the vents room's west or east pillar to the cell over its vent: the
/// vent's gust is redirected, and the pillar's flag keeps it there.
void TaopoSwamp_PushPillarOverVent(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == ACTOR_WEST_VENT_PILLAR) {
    if (FIELD_CELL(object->x.fixed) == 17) {
      object->z.fixed += toFIXED(1);
      Vent_SetGust(2, toFIXED(8));
      Event_SetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT);
    }
  } else if (id == ACTOR_EAST_VENT_PILLAR) {
    if (FIELD_CELL(object->x.fixed) == 29) {
      object->z.fixed += toFIXED(1);
      Vent_SetGust(8, toFIXED(8));
      Event_SetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT);
    }
  }
}

extern u8 gTaopoSwampVentsHoleVents[];
extern u8 gTaopoSwampPillarGapHoleVents[];

/// Stepping on a working vent: the gust pushes the leader back until a wall
/// stops them, or down the hole.
void TaopoSwamp_StepOnVent(s32 hole) {
  struct Actor *leader;
  struct Vector3 position;
  s32 found, result, slip;

  if (gMainRoutineWork.next == SCENE_TAOPO_SWAMP_VENTS) {
    found = Vent_IsActive(gTaopoSwampVentsHoleVents[hole - 10]);
    if (hole == 12 && Event_GetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT))
      return;
    if (hole == 18 && Event_GetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT))
      return;
  }
  if (gMainRoutineWork.next == SCENE_TAOPO_SWAMP_PILLAR_GAP) {
    u8 *table = gTaopoSwampPillarGapHoleVents + 1;
    s32 at = hole - 10;
    found = Vent_IsActive(table[at]);
    if (hole == 10 && Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
      return;
    if (hole == 12 && Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
      return;
  }
  if (gMainRoutineWork.next == SCENE_TAOPO_SWAMP_DRIPS) {
    found = Vent_IsActive(hole - 10);
    if (hole == 10 && Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
      return;
  }
  if (gMainRoutineWork.next == SCENE_TAOPO_SWAMP_LAVA_ROCKS)
    found = Vent_IsActive(hole - 10);
  if (found == 0)
    return;
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  goto test;
sink:
  if (result < 0) {
    leader->facing = FACING_SOUTH;
    TaopoSwamp_FallThroughHole(hole);
    return;
  }
  leader->z.fixed += toFIXED(2);
  slip = (leader->x.fixed & 0xfffff) + 0xfff80000;
  {
    s32 limit = 0x10001;

    if (slip >= limit)
      slip = toFIXED(1);
    limit = -0x10001;
    if (slip <= limit)
      slip = -toFIXED(1);
  }
  leader->x.fixed -= slip;
  leader->facing += TRIG_TURN / 16;
  System_Wait(1);
test:
  position.x = leader->x.fixed;
  position.y = leader->y.fixed;
  position.z = leader->z.fixed + toFIXED(8);
  result = MapGrid_CheckCollision(leader, &position);
  if (result <= 0)
    goto sink;
}

/// The gust from the covered vent knocks Flower, the Venus Djinni, down one
/// level in three hops (FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE).
void TaopoSwamp_BlowFlowerOffLedge(void) {
  struct Actor *object = Actor_Get(ACTOR_FLOWER);
  FIXED speed;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(gMainRoutineWork.leaderActorId, 8, 0);
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldCamera_MoveTo(408, CAMERA_KEEP, 312, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAttachedEffect(ACTOR_FLOWER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  speed = toFIXED(1) / 2;
  i = 2;
  do {
    object->velocityY = toFIXED(2);
    Event_PlaySound(SE_JUMP);
    object->speedLimit = speed;
    object->acceleration = speed / 2;
    Actor_WalkByAndWait(ACTOR_FLOWER, 0, 8);
    speed += 0x4000;
  } while (--i >= 0);
  Event_PlaySound(SE_FALL);
  object->velocityY = toFIXED(4);
  Actor_WalkByAndWait(ACTOR_FLOWER, 0, 8);
  FieldEvent_Wait(60);
  Event_SetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE);
  Event_End();
}

#include "shared/actor_wait_to_land.h"

/// Moved to column 16, the pillar drops into the gap there
/// (FLAG_TAOPO_SWAMP_PILLAR_IN_GAP).
void TaopoSwamp_DropPillarIntoGap(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == ACTOR_GAP_PILLAR && FIELD_CELL(object->x.fixed) == 16) {
    Actor_WaitToLand(ACTOR_GAP_PILLAR);
    MapGrid_CopyCellAttributes(15, 26, 1, 1, 16, 26);
    Event_SetFlag(FLAG_TAOPO_SWAMP_PILLAR_IN_GAP);
  }
}

/// Douse on the circular spot, actor 8: a puddle the next gust blows away
/// unless Frost turns it to an ice pillar first.
void TaopoSwamp_DouseVentSpot(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetAnimation(id, 1);
  Event_SetFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
  Event_SetFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
}

/// Ice on a spot: the two copies differ only in their cells.
#define DEFINE_TAOPO_SWAMP_FREEZE_SPOT(name, n1, n2, n3)                       \
  void name(s32 a, s32 id) {                                                   \
    TaopoSwamp_CopyCellAttributes(n1, n2, 1, 1, n3, n2);                       \
    FieldActor_SetSpritePriority(id, 2);                                       \
    Actor_Get(id)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;                 \
    Vents_SetId(0, id);                                                        \
    Event_SetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);                               \
    Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);                             \
  }

DEFINE_TAOPO_SWAMP_FREEZE_SPOT(TaopoSwamp_FreezeVentSpot, 28, 8, 26)

/// The gust breaks the ice pillar on the vent spot, and a leader standing on it
/// hops off.
void TaopoSwamp_ShatterIcePillar(s32 id) {
  struct Actor *leader;
  struct Actor *object;
  s32 zero = 0;

  Actor_SetAnimation(id, 8);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(id)->update = (void (*)(void))zero;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  object = Actor_Get(id);
  if (FIELD_CELL(object->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(object->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_WalkByAndWait(gMainRoutineWork.leaderActorId, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  MapGrid_CopyCellAttributes(22, 8, 1, 1, 26, 8);
}

/// The gust blows the puddle off the vent spot.
void TaopoSwamp_BlowAwayPuddle(s32 id) {
  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(id)->update = 0;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
}

/// A drop or Douse on the spot under the drip: the puddle is frozen solid by
/// Frost once it is full (Douse, or more than three drops).
void TaopoSwamp_FillDripSpot(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetAnimation(id, 1);
  if (a == 5 || FieldGameFlag_GetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT) > 3) {
    Event_SetFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
    Event_SetFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
  }
}

DEFINE_TAOPO_SWAMP_FREEZE_SPOT(TaopoSwamp_FreezeDripSpot, 23, 27, 25)

/// As TaopoSwamp_ShatterIcePillar, for the spot the ceiling drips onto, whose
/// puddle size (FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT) starts again.
void TaopoSwamp_ShatterDripIcePillar(s32 id) {
  struct Actor *leader;
  struct Actor *object;

  Actor_SetAnimation(id, 8);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(id)->update = 0;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_PUDDLE);
  GameFlag_SetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT, 0);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  object = Actor_Get(id);
  if (FIELD_CELL(object->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(object->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_WalkByAndWait(gMainRoutineWork.leaderActorId, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  MapGrid_CopyCellAttributes(1, 1, 1, 1, 25, 27);
}

/// As TaopoSwamp_BlowAwayPuddle, for the spot the ceiling drips onto.
void TaopoSwamp_BlowAwayDripPuddle(s32 id) {
  s32 zero = 0;

  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_Get(id)->update = (void (*)(void))zero;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_SPOT_WET);
  GameFlag_SetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT, 0);
}

/// Douse on the red glowing rock by the lava, actor 9: it cools, and Move can
/// push it.
void TaopoSwamp_CoolLavaRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
}

/// Move on the cooled rock at cell x 7: it sinks into the lava as a stepping
/// stone.
void TaopoSwamp_PushLavaRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (FIELD_CELL(object->x.fixed) == 7) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Event_SetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
    Event_SetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK);
    MapGrid_CopyCellAttributes(7, 17, 1, 1, 7, 15);
  }
}

/// Douse on the red rock Tremor knocked out of the pillar, actor 10.
void TaopoSwamp_CoolPillarRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
  MapGrid_CopyCellAttributes(27, 14, 1, 1, 27, 13);
}

/// Move on the cooled pillar rock at cell z 16: it slides two cells and sinks
/// into the lava.
void TaopoSwamp_PushPillarRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (FIELD_CELL(object->z.fixed) == 16) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Event_SetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
    Event_SetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK);
    Actor_SetSpeed(id, 0.5);
    Actor_SetDestinationOffset(id, -32, 0);
    FieldActor_WaitForMoveById(id);
    Actor_SetAnimation(id, 3);
    MapGrid_CopyCellAttributes(25, 18, 1, 1, 25, 16);
  }
}

/// The update of the boulder Tremor knocks off the pillar: it turns back a
/// thirty-second of a turn each frame as it rolls.
void TaopoSwamp_SpinBoulder(struct Actor *object) {
  ((struct FieldSprite *)object->sprite)->rotation -= TRIG_TURN / 32;
}

/// One puff of steam from the cooling lava, until the count runs out.
void TaopoSwamp_PuffSteam(void) {
  s32 a;
  s32 x;
  s32 z;
  struct Actor *object;

  gTaopoSwamp_SteamPuffs--;
  a = Random16() * 32;
  x = Random16() * 16 + a + toFIXED(300);
  z = Random16() * 16 + a + toFIXED(260);
  object = FieldActor_Create(SPRITE_DUST_CLOUD, x, 0, z);
  Actor_StartScript(object, gTaopoSwamp_PuffSteamScript);
  Actor_SetSpriteFlags(object, 0);
  Actor_SetBlendMode(object, 1);
  if (gTaopoSwamp_SteamPuffs == 0)
    FieldTask_Remove(TaopoSwamp_PuffSteam);
}

/// The passage behind the pillar opens, a row of cells at a time.
void TaopoSwamp_OpenPassage(void) {
  s32 timer = --gTaopoSwamp_PassageRows;
  s32 step = timer / 8;

  MapGrid_CopyCells(47 - (timer & 1) * 2, 64 - step, 2, step, 29, 15 - step);
  if (gTaopoSwamp_PassageRows == 0)
    FieldTask_Remove(TaopoSwamp_OpenPassage);
}

/// The drained pool's two frames in turn, until the count runs out.
void TaopoSwamp_FlickerPool(void) {
  s32 n = --gTaopoSwamp_PoolFrames;

  MapGrid_CopyCells(50, (n & 1) * 5 + 59, 13, 5, 18, 15);
  if (gTaopoSwamp_PoolFrames == 0)
    FieldTask_Remove(TaopoSwamp_FlickerPool);
}

extern u32 gTaopoSwamp_TremorPillarScript[];

/// Tremor on the pillar with the huge rock on top, actor 19: the rock rolls off
/// and lands, a red rock pops out, clods burst, the pool drains and the passage
/// opens (flag 0x330).
void TaopoSwamp_TremorPillar(void) {
  struct Actor *rock = Actor_Get(ACTOR_TREMOR_BOULDER);
  struct Actor *clod;
  s32 i, row;
  FIXED cosine;
  s32 (*mul)(s32, s32);
  u32 angle, speed, scale;
  s32 turn = -256;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (i = 9; i >= 0; i--) {
    System_Wait(1);
    ((struct FieldSprite *)rock->sprite)->rotation += turn;
    rock->x.fixed -=
        Trig_Cos(((struct FieldSprite *)rock->sprite)->rotation) / 2;
    rock->targetX = 0x80000000;
  }
  rock->update = TaopoSwamp_SpinBoulder;
  Actor_SetSpeed(ACTOR_TREMOR_BOULDER, 2);
  Actor_SetDestination(ACTOR_TREMOR_BOULDER, 492, 246);
  rock->gravity = toFIXED(4) / 5;
  rock->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  rock->layer = 0;
  FieldActor_WaitForMoveById(ACTOR_TREMOR_BOULDER);
  rock->velocityY = 0;
  rock->y.fixed = rock->groundHeight;
  Actor_SetPosition(ACTOR_TREMOR_BOULDER, 480, 248);
  ((struct FieldSprite *)rock->sprite)->rotation = 0;
  Event_PlaySound(SE_THUMP);
  MapGrid_SetShake(5, 5, 1);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_SetPosition(ACTOR_TREMOR_PILLAR_ROCK, 480, 248);
  Actor_SetDestination(ACTOR_TREMOR_PILLAR_ROCK, 440, 232);
  Actor_Get(ACTOR_TREMOR_PILLAR_ROCK)->velocityY = toFIXED(10);
  rock->update = 0;
  i = 15;
  do {
    clod = (FieldActor_Create(SPRITE_ROCK, rock->x.fixed,
                              rock->y.fixed + toFIXED(16), rock->z.fixed));
    if (clod != 0) {

      angle = Random16();
      speed = Random16();
      scale = (Random16() >> 2) + 0x1000;
      Actor_StartScript(clod, gTaopoSwamp_TremorPillarScript);
      Actor_SetSpriteFlags(clod, 0);
      clod->velocityY = (angle << 2) + toFIXED(2);
      cosine = Trig_Cos(angle);
      mul = Iwram_MulFX;
      speed <<= 3;
      clod->velocityZ = mul(speed, cosine);
      clod->velocityX = mul(speed, Math_Sin(angle));
      clod->acceleration = 0;
      clod->gravity = 0x547a;
      clod->scaleX = scale;
      clod->scaleY = scale;
      clod->bounce = FIXED_ONE / 2;
    }
  } while (--i >= 0);
  FieldMapGrid_WaitForShakeToSettle();
  Task_AddFrame(TaopoSwamp_OpenPassage, TASK_PASS_VBLANK);
  System_Wait(40);
  FieldCamera_SetSpeed(0.9);
  FieldCamera_MoveTo(336, CAMERA_KEEP, 280, 1);
  i = 11;
  row = 114;
  do {
    MapGrid_CopyCells(50, row, 13, 5, 18, 15);
    System_Wait(8);
    row -= 5;
  } while (--i > 0);
  Task_AddFrame(TaopoSwamp_PuffSteam, TASK_PASS_VBLANK);
  System_Wait(60);
  Task_AddFrame(TaopoSwamp_FlickerPool, TASK_PASS_VBLANK);
  System_Wait(24);
  FieldEvent_Wait(120);
  Event_End();
  Event_SetFlag(FLAG_TAOPO_SWAMP_PASSAGE_OPEN);
}

/// Douse on the red rock in the lower left corner, actor 8, on the way to the
/// Vial's chest.
void TaopoSwamp_CoolChestRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
}

/// Move on the cooled rock at cell z 21: it sinks into the lava.
void TaopoSwamp_PushChestRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (FIELD_CELL(object->z.fixed) == 21) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Event_SetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
    Event_SetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK);
    MapGrid_CopyCellAttributes(11, 19, 1, 1, 11, 21);
  }
}

/// Douse on the red rock in the upper right corner, actor 9, by the other way
/// out.
void TaopoSwamp_CoolExitRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
}

/// Move on the cooled rock at cell x 31: it slides two cells and sinks into the
/// lava.
void TaopoSwamp_PushExitRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (FIELD_CELL(object->x.fixed) == 31) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Event_SetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
    Event_SetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK);
    Actor_SetSpeed(id, 0.5);
    Actor_SetDestinationOffset(id, 0, -32);
    FieldActor_WaitForMoveById(id);
    Actor_SetAnimation(id, 3);
    MapGrid_CopyCellAttributes(30, 13, 1, 1, 31, 11);
  }
}

#if TLA_PATCH_1 /* Absent from the Japanese edition. */
void TaopoSwamp_JumpForward(void) { Field_TryJumpForward(); }
#endif

/// Douse on the glowing rock that lights the dark room, actor 8: the lights go
/// out and Move can push it aside.
void TaopoSwamp_CoolDarkRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE);
  MapGrid_CopyCellAttributes(7, 39, 1, 1, 7, 38);
  TaopoSwamp_DarkenRoom(1);
}

/// Douse on the glowing rock of the last chamber, actor 9: the lights go out
/// and the Star Dust's sparkle shows in the dark.
void TaopoSwamp_CoolLastRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED);
  Event_ClearFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE);
  TaopoSwamp_DarkenRoom(1);
}

/// Scoop on the sparkle in the dark chamber digs up the Star Dust
/// (FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP).
void TaopoSwamp_ScoopStarDust(s32 a) {
  Event_DigUpTreasure(a, 64, FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP);
}

/// Searching the Star Dust's sparkle before it is dug up: "I thought I saw
/// something sparkling in the ground!"
void TaopoSwamp_ShowSparkleMessage(void) {
  TaopoSwamp_ShowMessage((s32)&MsgIThoughtI);
}

extern struct Trigger gTaopoSwamp_Triggers[];
extern struct Trigger gTaopoSwampEntranceTriggers[];
extern struct Trigger gTaopoSwampMudTriggers[];
extern struct Trigger gTaopoSwampTearStonesTriggers[];
extern struct Trigger gTaopoSwampFlowerTriggers[];
extern struct Trigger gTaopoSwampVentsTriggers[];
extern struct Trigger gTaopoSwampPoolTriggers[];
extern struct Trigger gTaopoSwampPillarGapTriggers[];
extern struct Trigger gTaopoSwampDripsTriggers[];
extern struct Trigger gTaopoSwampTremorTriggers[];
extern struct Trigger gTaopoSwampLavaRocksTriggers[];
extern struct Trigger gTaopoSwampStarDustTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_TAOPO_SWAMP_ENTRANCE)
    return (const struct Trigger *)gTaopoSwampEntranceTriggers;
  if (scene == SCENE_TAOPO_SWAMP_MUD)
    return (const struct Trigger *)gTaopoSwampMudTriggers;
  if (scene == SCENE_TAOPO_SWAMP_TEAR_STONES)
    return (const struct Trigger *)gTaopoSwampTearStonesTriggers;
  if (scene == SCENE_TAOPO_SWAMP_FLOWER)
    return (const struct Trigger *)gTaopoSwampFlowerTriggers;
  if (scene == SCENE_TAOPO_SWAMP_VENTS)
    return (const struct Trigger *)gTaopoSwampVentsTriggers;
  if (scene == SCENE_TAOPO_SWAMP_POOL)
    return (const struct Trigger *)gTaopoSwampPoolTriggers;
  if (scene == SCENE_TAOPO_SWAMP_PILLAR_GAP)
    return (const struct Trigger *)gTaopoSwampPillarGapTriggers;
  if (scene == SCENE_TAOPO_SWAMP_DRIPS)
    return (const struct Trigger *)gTaopoSwampDripsTriggers;
  if (scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS)
    return gTaopoSwampLavaRocksTriggers;
  if (scene == SCENE_TAOPO_SWAMP_TREMOR)
    return (const struct Trigger *)gTaopoSwampTremorTriggers;
  if (scene == SCENE_TAOPO_SWAMP_STAR_DUST)
    return (const struct Trigger *)gTaopoSwampStarDustTriggers;
  return (const struct Trigger *)gTaopoSwamp_Triggers;
}

/// A falling drop splashes where it lands.
s32 TaopoSwamp_LandDrop(struct Actor *object) {
  FIXED y = object->y.fixed;
  struct Actor *spark;
  FIXED scale;

  if (y + object->velocityY < object->groundHeight) {
    spark = TaopoSwamp_CreateActor(269, object->x.fixed, y, object->z.fixed);
    scale = FIXED_ONE * 55 / 100;
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Actor_StartScript(spark, gTaopoSwamp_SparkFades);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 2);
    }
    Event_PlaySound(SE_DRIP);
    object->y.fixed = object->groundHeight;
    object->velocityY = 0;
    return 0;
  }
  return 1;
}

/// Now and then a drop falls at a random place along the swamp's edge.
void TaopoSwamp_DripAtRandom(void) {
  if ((gFrameCount & 7) == 0 && (u32)Random16() < 0x4000) {
    s32 x = (Random16() << 5) + 0x2080000;
    s32 z = 0xb00000 - (Random16() << 3);
    FIXED y = toFIXED(160);
    struct Actor *object = FieldActor_Create(SPRITE_DROPLET, x, y, z);
    FIXED scale = FIXED_ONE * 33 / 100;

    if (object != NULL) {
      object->scaleX = scale;
      object->scaleY = scale;
      object->gravity = 0x7333;
      Actor_StartScript(object, gTaopoSwamp_DripAtRandomScript);
      Actor_SetSpriteFlags(object, 0);
      Actor_SetSpritePriorityDirect(object, 3);
    }
  }
}

/// The vent room's cycle of 480 frames: two sets of vents blow in turn, and
/// while the second blows over the covered vent the Djinni is knocked down.
void TaopoSwamp_CycleDjinniVents(void) {
  s32 timer = Vents_GetCounter();

  switch (timer) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Vent_SetActive(4, 1);
    Vent_SetActive(5, 1);
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case 180:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Vent_SetActive(4, 0);
    Vent_SetActive(5, 0);
    Scene_QueueMusic(-1);
    break;
  case 240:
    Vent_SetActive(2, 1);
    Vent_SetActive(3, 1);
    Vent_SetActive(6, 1);
    Vent_SetActive(7, 1);
    Vent_SetActive(8, 1);
    Vent_SetActive(9, 1);
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case 420:
    Vent_SetActive(2, 0);
    Vent_SetActive(3, 0);
    Vent_SetActive(6, 0);
    Vent_SetActive(7, 0);
    Vent_SetActive(8, 0);
    Vent_SetActive(9, 0);
    Scene_QueueMusic(-1);
    break;
  }
  if ((u32)(timer - 251) <= 178 &&
      Event_GetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT) &&
      !Event_GetFlag(59) && !Event_GetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
    gFieldWork->raisedTrigger = 100;
  Vents_AdvanceCounter(480);
}

/// The cycle of the vent under the circular spot: 120 frames of gust in 480
/// that break its ice pillar or blow its puddle away.
void TaopoSwamp_CycleSpotVent(void) {
  s32 timer = Vents_GetCounter();

  switch (timer) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case 120:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  if ((u32)(timer - 11) <= 118) {
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
      TaopoSwamp_ShatterIcePillar(8);
    else if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
      TaopoSwamp_BlowAwayPuddle(8);
  }
  Vents_AdvanceCounter(480);
}

/// As TaopoSwamp_CycleSpotVent, for the spot the ceiling drips onto.
void TaopoSwamp_CycleDripVent(void) {
  s32 timer = Vents_GetCounter();

  switch (timer) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case 120:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  if ((u32)(timer - 11) <= 118) {
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN))
      TaopoSwamp_ShatterDripIcePillar(ACTOR_DRIP_PUDDLE);
    else if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
      TaopoSwamp_BlowAwayDripPuddle(ACTOR_DRIP_PUDDLE);
  }
  Vents_AdvanceCounter(480);
}

/// The cycle of the two vents in the room of the cooled rocks: 180 frames of
/// gust in 240.
void TaopoSwamp_CycleRockRoomVents(void) {
  switch (Vents_GetCounter()) {
  case 0:
    Vent_SetActive(0, 1);
    Vent_SetActive(1, 1);
    Scene_QueueMusic(SE_AMBIENT_WATER);
    break;
  case 180:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  Vents_AdvanceCounter(240);
}

/// A drop from the ceiling lands: it splashes and grows the puddle below by
/// one, up to four.
s32 TaopoSwamp_LandDrip(struct Actor *object) {
  s32 flag;
  s32 count;
  struct Actor *spark;
  FIXED scale;

  if (object->y.fixed + object->velocityY < object->groundHeight) {
    flag = Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN);
    if (flag) {
      Event_PlaySound(SE_DRIP);
      return 0;
    }
    spark = TaopoSwamp_CreateActor(269, object->x.fixed, object->y.fixed,
                                   object->z.fixed);
    scale = FIXED_ONE * 55 / 100;
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Actor_StartScript(spark, gTaopoSwamp_SparkFades);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 2);
    }
    Event_PlaySound(SE_DRIP);
    object->y.fixed = object->groundHeight;
    object->velocityY = 0;
    if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      count = FieldGameFlag_GetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT);
      if (count <= 3) {
        count++;
        GameFlag_SetByte(FLAG_BYTE_TAOPO_SWAMP_DROPS_CAUGHT, count);
      }
      if (count != 0)
        TaopoSwamp_FillDripSpot(0, ACTOR_DRIP_PUDDLE);
      object = Actor_Get(ACTOR_DRIP_PUDDLE);
      object->scaleX = count << 14;
      object->scaleY = count << 14;
    }
    return 0;
  }
  return 1;
}

/// Every 64 frames a drop falls from the ceiling onto the vent spot.
void TaopoSwamp_DripFromCeiling(void) {
  if ((gFrameCount & 63) == 0) {
    FIXED x = toFIXED(408);
    FIXED z = toFIXED(440);
    FIXED y = toFIXED(160);
    struct Actor *object = FieldActor_Create(SPRITE_DROPLET, x, y, z);
    FIXED scale = FIXED_ONE * 33 / 100;

    if (object != NULL) {
      object->scaleX = scale;
      object->scaleY = scale;
      object->gravity = 0x7333;
      Actor_StartScript(object, gTaopoSwamp_DripFromCeilingScript);
      Actor_SetSpriteFlags(object, 0);
    }
  }
}

/// A drop's script test: whether it is still above the floor.
s32 TaopoSwamp_IsDropFalling(struct Actor *object) {
  if (object->y.fixed + object->velocityY < -toFIXED(48))
    return 0;
  return 1;
}

/// A task: now and then a drop falls from the ceiling near (n1, n2).
#define DEFINE_DRIP_AT_RANDOM(name, n1, n2)                                    \
  void name(void) {                                                            \
    if ((gFrameCount & 7) == 0 && (u32)Random16() < 0x4000) {                  \
      s32 offset = Random16() << 5;                                            \
      s32 x = offset + n1;                                                     \
      s32 z = n2 - offset;                                                     \
      s32 y = toFIXED(160);                                                    \
      struct Actor *object = FieldActor_Create(SPRITE_DROPLET, x, y, z);       \
      s32 scale = FIXED_ONE * 33 / 100;                                        \
                                                                               \
      if (object != NULL) {                                                    \
        object->scaleX = scale;                                                \
        object->scaleY = scale;                                                \
        object->motionFlags = ACTOR_FALLS;                                     \
        object->groundHeight = -toFIXED(64);                                   \
        object->gravity = 0x7333;                                              \
        Actor_StartScript(object, gTaopoSwamp_DropFalls);                      \
        Actor_SetSpriteFlags(object, 0);                                       \
      }                                                                        \
    }                                                                          \
  }

DEFINE_DRIP_AT_RANDOM(TaopoSwamp_DripInPillarGap, 0x1800000, 0x1900000)

DEFINE_DRIP_AT_RANDOM(TaopoSwamp_DripInDripRoom, 0x1a00000, 0x1600000)

extern u16 gTaopoSwampVentsVents[];
extern u16 gTaopoSwampVentsVentPair[];
extern u16 gTaopoSwampPillarGapVents[];
extern u16 gTaopoSwampDripsVents[];
extern u16 gTaopoSwamp_RockRoomVents[];
void TaopoSwamp_ResetMud(void);
void Actor_SetWideSprite(s32 id, s32 wide);
void TaopoSwamp_StartVents(u16 *points, u16 *values, s32 arg);

#if !TLA_PATCH_1
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object, *dummy, *piece;
  s32 id, entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  if (scene == SCENE_TAOPO_SWAMP_MUD) {
    gMainRoutineWork.retreatScene = SCENE_TAOPO_SWAMP_ENTRANCE;
    gMainRoutineWork.retreatEntrance = 1;
    TaopoSwamp_ResetMud();
    for (id = ACTOR_FIRST_BUSH; id <= ACTOR_LAST_BUSH; id++) {
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_BUSH_BLOWN + id))
        Actor_SetPosition(id, 0, 0);
      else
        Actor_GetById(id)->scaleY = FIXED_ONE * 5 / 4;
    }
    Actor_SetSpritePriority(ACTOR_MUD_SPROUT, 3);
    Actor_GetById(ACTOR_MUD_SPROUT)->collisionFlags = 0;
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_VINE_GROWN))
      MapGrid_CopyCellAttributes(50, 16, 1, 2, 47, 16);
    dummy = Actor_GetById(ACTOR_MUD_STUMP);
    if (dummy != 0) {
      dummy->scaleX = FIXED_ONE * 3 / 4;
      dummy->scaleY = FIXED_ONE * 3 / 4;
      dummy->motionFlags = 0;
    }
    Task_Add(TaopoSwamp_DripAtRandom, TASK_PASS_VBLANK);
    ((struct MapWorkGroundEffects *)map)->wading = MAP_WADING_MUD;
    gDecodeFillByte = 93;
  } else if (scene == SCENE_TAOPO_SWAMP_TEAR_STONES) {
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0)))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR, 1);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1)))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 1, 1);
  } else if (scene == SCENE_TAOPO_SWAMP_FLOWER) {
    FieldEffect_StartBlankOverlay(0);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
      Actor_SetPosition(ACTOR_FLOWER, 0, 0);
  } else if (scene == SCENE_TAOPO_SWAMP_VENTS) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
      Actor_SetPosition(ACTOR_FLOWER, 0, 0);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT))
      Actor_SetPosition(ACTOR_WEST_VENT_PILLAR, 280, 232);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT))
      Actor_SetPosition(ACTOR_EAST_VENT_PILLAR, 472, 248);
    TaopoSwamp_StartVents(gTaopoSwampVentsVents, gTaopoSwampVentsVentPair,
                          TaopoSwamp_CycleDjinniVents);
  } else if (scene == SCENE_TAOPO_SWAMP_PILLAR_GAP) {
    FieldEffect_StartBlankOverlay(0);
    Task_Add(TaopoSwamp_DripInPillarGap, TASK_PASS_VBLANK);
    if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5)
      TaopoSwamp_StartVents(gTaopoSwampPillarGapVents, 0,
                            TaopoSwamp_CycleSpotVent);
    piece = Actor_GetById(ACTOR_GAP_PUDDLE);
    piece->motionFlags = 0;
    Actor_SetSpriteFlags(piece, 0);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
      MapGrid_CopyCellAttributes(28, 8, 1, 1, 26, 8);
      Actor_SelectAnimation(piece, 0);
    } else {
      MapGrid_CopyCellAttributes(22, 8, 1, 1, 26, 8);
      Actor_SetSpritePriority(ACTOR_GAP_PUDDLE, 3);
      piece->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      if (!Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
        Actor_SelectAnimation(piece, 7);
    }
    piece = Actor_GetById(ACTOR_GAP_PILLAR);
    piece->motionFlags = 0;
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_PILLAR_IN_GAP)) {
      Actor_SetPosition(ACTOR_GAP_PILLAR, 264, 424);
      piece->groundHeight = 0;
      piece->y.fixed = 0;
      MapGrid_CopyCellAttributes(15, 26, 1, 1, 16, 26);
    }
  } else if (scene == SCENE_TAOPO_SWAMP_DRIPS) {
    if (gMainRoutineWork.entrance != 2 && gMainRoutineWork.entrance != 4 &&
        gMainRoutineWork.entrance != 9) {
      Rope_Initialize();
      Call4(Rope_Place, 0, 776, 8, 9);
    } else {
      Value3((s32 (*)())TaopoSwamp_StartVents, (s32)gTaopoSwampDripsVents, 0,
             (s32)TaopoSwamp_CycleDripVent);
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      FieldTask_AddFrame(TaopoSwamp_DripInDripRoom, TASK_PASS_VBLANK);
      FieldTask_AddFrame(TaopoSwamp_DripFromCeiling, TASK_PASS_VBLANK);
      object = Actor_GetById(ACTOR_DRIP_PUDDLE);
      object->motionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
        Actor_SelectAnimation(object, 0);
      } else {
        Actor_SetSpritePriority(ACTOR_DRIP_PUDDLE, 3);
        object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        if (!Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
          Actor_SelectAnimation(object, 7);
      }
      MapGrid_CopyCells(50, 36, 13, 13, 60, 5);
    }
  } else if (scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS) {
    TaopoSwamp_StartVents(gTaopoSwamp_RockRoomVents, 0,
                          TaopoSwamp_CycleRockRoomVents);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
    } else if (Event_GetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK)) {
      Actor_SetAnimation(ACTOR_CHEST_ROCK, 3);
      Actor_SetPosition(ACTOR_CHEST_ROCK, 184, 344);
      Actor_GetById(ACTOR_CHEST_ROCK)->collisionFlags = 0;
      Actor_GetById(ACTOR_CHEST_ROCK)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetSpritePriority(ACTOR_CHEST_ROCK, 3);
      MapGrid_CopyCellAttributes(11, 19, 1, 1, 11, 21);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
    } else if (Event_GetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK)) {
      Actor_SetAnimation(ACTOR_EXIT_ROCK, 3);
      Actor_SetPosition(ACTOR_EXIT_ROCK, 504, 184);
      Actor_GetById(ACTOR_EXIT_ROCK)->collisionFlags = 0;
      Actor_GetById(ACTOR_EXIT_ROCK)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(30, 13, 1, 1, 31, 11);
    }
  } else if (scene == SCENE_TAOPO_SWAMP_TREMOR) {
    piece = Actor_GetById(ACTOR_TREMOR_BOULDER);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_PASSAGE_OPEN)) {
      Actor_SetPosition(ACTOR_TREMOR_BOULDER, 480, 248);
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        Actor_SetPosition(ACTOR_TREMOR_PILLAR_ROCK, 440, 232);
      MapGrid_CopyCells(47, 50, 2, 14, 29, 1);
      MapGrid_CopyCells(50, 59, 13, 5, 18, 15);
    } else {
      piece->motionFlags = 0;
      piece->y.fixed = toFIXED(32);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
    } else if (Event_GetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK)) {
      Actor_SetAnimation(ACTOR_TREMOR_LAVA_ROCK, 3);
      Actor_SetPosition(ACTOR_TREMOR_LAVA_ROCK, 120, 248);
      Actor_GetById(ACTOR_TREMOR_LAVA_ROCK)->collisionFlags = 0;
      Actor_GetById(ACTOR_TREMOR_LAVA_ROCK)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(7, 17, 1, 1, 7, 15);
    } else {
      Actor_SetAnimation(ACTOR_TREMOR_LAVA_ROCK, 0);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
    } else {
      MapGrid_CopyCellAttributes(27, 14, 1, 1, 27, 13);
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK)) {
        Actor_SetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 3);
        Actor_SetPosition(ACTOR_TREMOR_PILLAR_ROCK, 408, 264);
        Actor_GetById(ACTOR_TREMOR_PILLAR_ROCK)->collisionFlags = 0;
        Actor_GetById(ACTOR_TREMOR_PILLAR_ROCK)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(25, 18, 1, 1, 25, 16);
      } else {
        Actor_SetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 0);
      }
    }
  } else if (scene == SCENE_TAOPO_SWAMP_STAR_DUST) {
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE);
    } else {
      Actor_SetAnimation(ACTOR_DARK_ROCK, 0);
      MapGrid_CopyCellAttributes(7, 39, 1, 1, 7, 38);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
      Event_SetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE);
    else
      Actor_SetAnimation(ACTOR_LAST_ROCK, 0);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR, 0);
    entrance = gMainRoutineWork.entrance;
    if ((u32)(entrance - 2) <= 1) {
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED))
        TaopoSwamp_DarkenRoom(0);
      else
        TaopoSwamp_FocusMapMiddle(0);
    }
    if (entrance == 6) {
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
        TaopoSwamp_DarkenRoom(0);
      else
        TaopoSwamp_FocusMapMiddle(0);
    }
    if ((u32)(entrance - 4) <= 1)
      BlendScript_Suspend();
  }
  return 0;
}
#else
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.next;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *effect = &gFieldWork->transition;
  struct Actor *object, *dummy, *piece;
  s32 id, entrance;

  *effect = 0;
  if (scene == SCENE_TAOPO_SWAMP_MUD) {
    gMainRoutineWork.retreatScene = SCENE_TAOPO_SWAMP_ENTRANCE;
    gMainRoutineWork.retreatEntrance = 1;
    TaopoSwamp_ResetMud();
    for (id = ACTOR_FIRST_BUSH; id <= ACTOR_LAST_BUSH; id++) {
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_BUSH_BLOWN + id))
        Actor_SetPosition(id, 0, 0);
      else
        Actor_GetById(id)->scaleY = FIXED_ONE * 5 / 4;
    }
    Actor_SetSpritePriority(ACTOR_MUD_SPROUT, 3);
    Actor_GetById(ACTOR_MUD_SPROUT)->collisionFlags = 0;
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
      MapGrid_CopyCellAttributes(50, 16, 1, 2, 47, 16);
    dummy = Actor_GetById(ACTOR_MUD_STUMP);
    if (dummy != 0) {
      dummy->scaleX = FIXED_ONE * 3 / 4;
      dummy->scaleY = FIXED_ONE * 3 / 4;
      dummy->motionFlags = 0;
    }
    Task_Add(TaopoSwamp_DripAtRandom, TASK_PASS_VBLANK);
    ((struct MapWorkGroundEffects *)map)->wading = MAP_WADING_MUD;
    gDecodeFillByte = 93;
  } else if (scene == SCENE_TAOPO_SWAMP_TEAR_STONES) {
    *effect = 516;
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0)))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR, 1);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1)))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR + 1, 1);
  } else if (scene == SCENE_TAOPO_SWAMP_FLOWER) {
    *effect = 516;
    FieldEffect_StartBlankOverlay(0);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
      Actor_SetPosition(ACTOR_FLOWER, 0, 0);
  } else if (scene == SCENE_TAOPO_SWAMP_VENTS) {
    *effect = 516;
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE))
      Actor_SetPosition(ACTOR_FLOWER, 0, 0);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT))
      Actor_SetPosition(ACTOR_WEST_VENT_PILLAR, 280, 232);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT))
      Actor_SetPosition(ACTOR_EAST_VENT_PILLAR, 472, 248);
    TaopoSwamp_StartVents(gTaopoSwampVentsVents, gTaopoSwampVentsVentPair,
                          TaopoSwamp_CycleDjinniVents);
  } else if (scene == SCENE_TAOPO_SWAMP_POOL) {
    *effect = 516;
  } else if (scene == SCENE_TAOPO_SWAMP_PILLAR_GAP) {
    *effect = 516;
    FieldEffect_StartBlankOverlay(0);
    Task_Add(TaopoSwamp_DripInPillarGap, TASK_PASS_VBLANK);
    if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5)
      TaopoSwamp_StartVents(gTaopoSwampPillarGapVents, 0,
                            TaopoSwamp_CycleSpotVent);
    piece = Actor_GetById(ACTOR_GAP_PUDDLE);
    piece->motionFlags = 0;
    Actor_SetSpriteFlags(piece, 0);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
      MapGrid_CopyCellAttributes(28, 8, 1, 1, 26, 8);
      Actor_SelectAnimation(piece, 0);
    } else {
      MapGrid_CopyCellAttributes(22, 8, 1, 1, 26, 8);
      Actor_SetSpritePriority(ACTOR_GAP_PUDDLE, 3);
      piece->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      if (!Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
        Actor_SelectAnimation(piece, 7);
    }
    piece = Actor_GetById(ACTOR_GAP_PILLAR);
    piece->motionFlags = 0;
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_PILLAR_IN_GAP)) {
      Actor_SetPosition(ACTOR_GAP_PILLAR, 264, 424);
      piece->groundHeight = 0;
      piece->y.fixed = 0;
      MapGrid_CopyCellAttributes(15, 26, 1, 1, 16, 26);
    }
  } else if (scene == SCENE_TAOPO_SWAMP_DRIPS) {
    *effect = 516;
    if (gMainRoutineWork.entrance != 2 && gMainRoutineWork.entrance != 4 &&
        gMainRoutineWork.entrance != 9) {
      Rope_Initialize();
      Call4(Rope_Place, 0, 776, 8, 9);
    } else {
      Value3((s32 (*)())TaopoSwamp_StartVents, (s32)gTaopoSwampDripsVents, 0,
             (s32)TaopoSwamp_CycleDripVent);
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_IGNORES_RISES;
      FieldTask_AddFrame(TaopoSwamp_DripInDripRoom, TASK_PASS_VBLANK);
      FieldTask_AddFrame(TaopoSwamp_DripFromCeiling, TASK_PASS_VBLANK);
      object = Actor_GetById(ACTOR_DRIP_PUDDLE);
      object->motionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_FROZEN)) {
        Actor_SelectAnimation(object, 0);
      } else {
        Actor_SetSpritePriority(ACTOR_DRIP_PUDDLE, 3);
        object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        if (!Event_GetFlag(FLAG_TAOPO_SWAMP_SPOT_WET))
          Actor_SelectAnimation(object, 7);
      }
      MapGrid_CopyCells(50, 36, 13, 13, 60, 5);
    }
  } else if (scene == SCENE_TAOPO_SWAMP_LAVA_ROCKS) {
    *effect = 516;
    TaopoSwamp_StartVents(gTaopoSwamp_RockRoomVents, 0,
                          TaopoSwamp_CycleRockRoomVents);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE);
    } else if (Event_GetFlag(FLAG_TAOPO_SWAMP_CHEST_ROCK_SUNK)) {
      Actor_SetAnimation(ACTOR_CHEST_ROCK, 3);
      Actor_SetPosition(ACTOR_CHEST_ROCK, 184, 344);
      Actor_GetById(ACTOR_CHEST_ROCK)->collisionFlags = 0;
      Actor_GetById(ACTOR_CHEST_ROCK)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetSpritePriority(ACTOR_CHEST_ROCK, 3);
      MapGrid_CopyCellAttributes(11, 19, 1, 1, 11, 21);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE);
    } else if (Event_GetFlag(FLAG_TAOPO_SWAMP_EXIT_ROCK_SUNK)) {
      Actor_SetAnimation(ACTOR_EXIT_ROCK, 3);
      Actor_SetPosition(ACTOR_EXIT_ROCK, 504, 184);
      Actor_GetById(ACTOR_EXIT_ROCK)->collisionFlags = 0;
      Actor_GetById(ACTOR_EXIT_ROCK)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(30, 13, 1, 1, 31, 11);
    }
  } else if (scene == SCENE_TAOPO_SWAMP_TREMOR) {
    *effect = 516;
    piece = Actor_GetById(ACTOR_TREMOR_BOULDER);
    if (Event_GetFlag(FLAG_TAOPO_SWAMP_PASSAGE_OPEN)) {
      Actor_SetPosition(ACTOR_TREMOR_BOULDER, 480, 248);
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        Actor_SetPosition(ACTOR_TREMOR_PILLAR_ROCK, 440, 232);
      MapGrid_CopyCells(47, 50, 2, 14, 29, 1);
      MapGrid_CopyCells(50, 59, 13, 5, 18, 15);
    } else {
      piece->motionFlags = 0;
      piece->y.fixed = toFIXED(32);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE);
    } else if (Event_GetFlag(FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_SUNK)) {
      Actor_SetAnimation(ACTOR_TREMOR_LAVA_ROCK, 3);
      Actor_SetPosition(ACTOR_TREMOR_LAVA_ROCK, 120, 248);
      Actor_GetById(ACTOR_TREMOR_LAVA_ROCK)->collisionFlags = 0;
      Actor_GetById(ACTOR_TREMOR_LAVA_ROCK)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      MapGrid_CopyCellAttributes(7, 17, 1, 1, 7, 15);
    } else {
      Actor_SetAnimation(ACTOR_TREMOR_LAVA_ROCK, 0);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE);
    } else {
      MapGrid_CopyCellAttributes(27, 14, 1, 1, 27, 13);
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_PILLAR_ROCK_SUNK)) {
        Actor_SetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 3);
        Actor_SetPosition(ACTOR_TREMOR_PILLAR_ROCK, 408, 264);
        Actor_GetById(ACTOR_TREMOR_PILLAR_ROCK)->collisionFlags = 0;
        Actor_GetById(ACTOR_TREMOR_PILLAR_ROCK)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        MapGrid_CopyCellAttributes(25, 18, 1, 1, 25, 16);
      } else {
        Actor_SetAnimation(ACTOR_TREMOR_PILLAR_ROCK, 0);
      }
    }
  } else if (scene == SCENE_TAOPO_SWAMP_STAR_DUST) {
    *effect = 516;
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED)) {
      Event_SetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE);
    } else {
      Actor_SetAnimation(ACTOR_DARK_ROCK, 0);
      MapGrid_CopyCellAttributes(7, 39, 1, 1, 7, 38);
    }
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
      Event_SetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE);
    else
      Actor_SetAnimation(ACTOR_LAST_ROCK, 0);
    if (!Event_GetFlag(FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP))
      Actor_SetWideSprite(FIELD_TREASURE_ACTOR, 0);
    entrance = gMainRoutineWork.entrance;
    if ((u32)(entrance - 2) <= 1) {
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED))
        TaopoSwamp_DarkenRoom(0);
      else
        TaopoSwamp_FocusMapMiddle(0);
    }
    if (entrance == 6) {
      if (Event_GetFlag(FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED))
        TaopoSwamp_DarkenRoom(0);
      else
        TaopoSwamp_FocusMapMiddle(0);
    }
    if ((u32)(entrance - 4) <= 1)
      BlendScript_Suspend();
  }
  return 0;
}
#endif

s32 Scene_PrepareMap(void) { return 0; }

/// The Star Dust's sparkle in the dark, until it is dug up
/// (FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP).
void TaopoSwamp_TwinkleStarDust(void) {
  s32 frame = -1;

  if (Event_GetFlag(FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP)) {
    frame = 62;
  } else {
    switch (gTaopoSwamp_SparkleFrame) {
    case 0:
      frame = 59;
      break;
    case 10:
    case 50:
      frame = 56;
      break;
    case 20:
    case 40:
      frame = 53;
      break;
    case 30:
      frame = 50;
      break;
    }
    if (++gTaopoSwamp_SparkleFrame > 59)
      gTaopoSwamp_SparkleFrame = 0;
  }
  if (frame != -1)
    MapGrid_CopyCellsLayered(45, frame, 3, 3, 76, 61);
}

extern u8 gMapGridCellBuffer[];

/// Points the map's followed point at cell (x, z), and the screen follows.
void TaopoSwamp_FocusMapOn(s32 x, s32 z) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapLayer *layer;
  s32 index, fx = x << 19;

  layer = &map->layers[2];
  layer->offsetX = fx;
  layer->offsetY = z << 19;
  layer->maskX = 127;
  layer->maskY = 0x3f80;
  layer->originY = z << 7;
  layer->originX = x;
  layer->speedX = 0;
  layer->speedY = 0;
  layer->phaseX = 0;
  layer->phaseY = 0;
  index = (z / 2 << 7) + x / 2;
  layer->scaleY = layer->scaleX = FIXED_ONE;
  layer->cells = (struct MapCell *)(gMapGridCellBuffer + index * 4);
  layer->terrainIds = gMapShapeGrid + index;
  layer->x = Iwram_MulFX(map->viewX, FIXED_ONE) + layer->offsetX;
  layer->y = Iwram_MulFX(map->viewZ, layer->scaleY) + layer->offsetY;
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
}

/// The lights go out (fading over 50 frames when asked) and the sparkle starts
/// in the last chamber.
void TaopoSwamp_DarkenRoom(s32 fade) {
  BlendScript_Suspend();
  if (fade) {
    Io_Write16(BLEND_ALPHA(6, 14), &REG_BLDALPHA->value);
    System_Wait(10);
    Io_Write16(BLEND_ALPHA(4, 13), &REG_BLDALPHA->value);
    System_Wait(10);
    Io_Write16(BLEND_ALPHA(2, 13), &REG_BLDALPHA->value);
    System_Wait(10);
    Io_Write16(BLEND_ALPHA(1, 12), &REG_BLDALPHA->value);
    System_Wait(10);
    Io_Write16(BLEND_ALPHA(0, 12), &REG_BLDALPHA->value);
    System_Wait(10);
  }
  TaopoSwamp_FocusMapOn(100, 50);
  Io_Write16(BLEND_ALPHA(7, 12), &REG_BLDALPHA->value);
  if (gMainRoutineWork.entrance == 6)
    FieldTask_AddFrame(TaopoSwamp_TwinkleStarDust, TASK_PASS_VBLANK);
}

/// Points the map's followed point at cell (100, 100).
void TaopoSwamp_FocusMapMiddle(void) { TaopoSwamp_FocusMapOn(100, 100); }

/// Entering the mud room afresh (not returning from a menu or battle), the
/// leader starts out of the mud.
void TaopoSwamp_ResetMud(void) {
  struct SceneState *state = &gSceneState;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    Iwram_ClearWordsEntry(state, sizeof(struct SceneState));
}

/// volatile: the VBlank handler sets the held keys, as gKeysHeld in The Broken
/// Seal.
extern volatile u32 gInput;

/// Each step on the mud sinks the leader deeper; sunk to the top, they are
/// pulled out to the last firm cell.
void TaopoSwamp_SinkInMud(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  struct Sprite *sprite = actor->sprite;
  struct Actor *spark;
  struct FieldSprite *sparkSprite;
  s32 charge, step, i;

  if (sprite->entries[1] != 0)
    sprite->entries[1]->priority = 1;
  sprite->flags |= 8;
  sprite->flags &= ~SPRITE_FLAG_SHADOW;
  Io_Write8(1, &sprite->dirty);
  if (gInput & 0xf0) {
    step = 0x1400;
    if (gInput & 2)
      step = step * 3 >> 1;
    state->timer += step / 96 + 1;
  }
  sprite->sunkRows = state->timer / 256;
  state->mode = 1;
  if (state->timer < 0x1400)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->spriteStorageKind = 0;
  for (i = 1; i >= 0; i--) {
    spark = (FieldActor_Create(269, actor->x.fixed, actor->y.fixed,
                               actor->z.fixed));
    if (spark != 0) {
      sparkSprite = spark->sprite;
      Actor_StartScript(spark, gTaopoSwamp_SplashFades);
      spark->motionFlags = 0;
      spark->layer = 1;
      if (sparkSprite != 0) {
        Sprite_StartAnimation(sparkSprite, 2);
        sparkSprite->flags = 0;
        sparkSprite->priority = 2;
      }
    }
    System_Wait(15);
  }
  FieldEvent_Wait(30);
  actor->spriteStorageKind = 1;
  sprite->sunkRows = 0;
  sprite->flags = 0;
  Event_End();
  actor->x.fixed = state->x;
  actor->z.fixed = state->z;
  state->mode = 0;
  state->timer = 0;
}

/// A bubble in the mud lifts the leader back to full height.
void TaopoSwamp_RiseOnBubble(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *object;
  struct Sprite *sprite;

  if (state->mode != 2) {
    object = Actor_Get(FieldParty_GetLeaderActor());
    sprite = object->sprite;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_SetMoveTargetFX(
        object, (object->x.fixed & -toFIXED(16)) + toFIXED(8), object->y.fixed,
        (object->z.fixed & -toFIXED(16)) + toFIXED(8));
    FieldActor_WaitForMove(object);
    while (sprite->sunkRows != 0) {
      sprite->sunkRows--;
      sprite->dirty = 1;
      System_Wait(1);
    }
    Event_End();
    sprite->flags |= 8;
    sprite->flags &= ~SPRITE_FLAG_SHADOW;
    state->mode = 2;
    state->timer = 0;
  }
}

/// Off the mud: the leader stands at full height again.
void TaopoSwamp_StepOnFirmGround(void) {
  struct SceneState *state = &gSceneState;
  struct Sprite *sprite;

  TaopoSwamp_SaveFirmGround();
  if (state->mode != 0) {
    sprite = Actor_Get(FieldParty_GetLeaderActor())->sprite;
    if (sprite->entries[1])
      sprite->entries[1]->priority = 9;
    sprite->flags = (sprite->flags & 0xf7) | SPRITE_FLAG_SHADOW;
    sprite->sunkRows = 0;
    sprite->dirty = 1;
    state->mode = 0;
    state->timer = 0;
  }
}

/// Keeps the cell the leader stands on as the one the mud returns them to.
void TaopoSwamp_SaveFirmGround(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());

  state->x = (object->x.fixed & -toFIXED(16)) + toFIXED(8);
  state->z = (object->z.fixed & -toFIXED(16)) + toFIXED(8);
}

#define VENT_TOUCHING Vent_IsTouching
#define VENT_BOUNCE Vent_Bounce
#include "../field/actor_vent_bounce.c"

#include "shared/actor_start_scripted.h"

/// A puff of dust from a vent at position, as its gust blows.
void Vent_Puff(s32 *position, s32 base) {
  struct Actor *object = TaopoSwamp_CreateActor(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));

  if (object != 0) {
    object->bounce = Trig_Cos((Random_Below(0x3000)) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() << 1) + base;
    // A spin of up to half a turn a frame, either way.
    object->timer = Random_Below(TRIG_TURN) - DEGtoANG(180);
    Actor_StartScripted(object, 3);
    object->update = (void (*)(void))Vent_Bounce;
    Actor_SetBlendMode(object, 1);
  }
}

#include "shared/vents_update.h"

// The registers by io_reg.h's names from here on: local.h names the blend
// register its own way for TaopoSwamp_DarkenRoom above.
#undef REG_BLDALPHA
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

/// Places a room's vents from its tables and starts their cycle.
void TaopoSwamp_StartVents(u16 *points, u16 *values, s32 arg) {
  struct VentWork *work = VENT_WORK;
  struct VentEntry *point = work->entries;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 x;
    s32 i;

    Iwram_ClearWordsEntry(work, sizeof(struct VentWork));
    x = *points++;
    i = 0;
    while (x > 0) {
      s32 z = points[0];
      s32 w = points[1];

      point->active = 0;
      point->x = toFIXED(x);
      point->z = toFIXED(z);
      point->y = Map_GetTerrainHeight(0, toFIXED(x), toFIXED(z) + toFIXED(16));
      point->value = toFIXED(w);
      point->timer = i++;
      points += 2;
      point++;
      if (i > 15)
        break;
      x = *points++;
    }
    if (values != 0) {
      x = *values++;
      i = 0;
      if (x > 0) {
        do {
          work->ids[i++] = x;
          if (i > 3)
            break;
          x = *values++;
        } while (x > 0);
      }
    }
  }
  work->hook = (s32 (*)(void))arg;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = BLEND_ALPHA(8, 12);
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_AddFrame(Vents_Update, TASK_PASS_VBLANK);
}

/// The accessors step the work pointer by whole entries and then take entry 0:
/// the bytes add the base to index * 20 before the field offset.
#define VENT_AT(work, index)                                                   \
  ((struct VentWork *)((u8 *)(work) + (index) * sizeof(struct VentEntry)))

/// Whether vent index is blowing now.
s32 Vent_IsActive(s32 index) {
  struct VentWork *work;

  work = VENT_WORK;
  return VENT_AT(work, index)->entries[0].active;
}

/// Starts or stops vent index's gust.
void Vent_SetActive(s32 index, s32 active) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].active = active;
}

/// The vents' frame counter, which their cycles time their gusts by.
s32 Vents_GetCounter(void) { return VENT_WORK->counter; }

#include "shared/vents_advance_counter.h"

/// The accessors step the work pointer by whole entries and then take entry
/// 0: the bytes add the base to index * 20 before the field offset.

/// Sets the speed of vent index's gust, in 16.16 pixels a frame.
void Vent_SetGust(s32 index, s32 value) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].value = value;
}

#include "shared/vents_set_id.h"
#undef VENT_WORK
#undef DIFF
#undef DEFINE_TAOPO_SWAMP_FREEZE_SPOT
#undef DEFINE_DRIP_AT_RANDOM
#undef VENT_TOUCHING
#undef VENT_BOUNCE
#undef VENT_AT

void TaopoSwamp_FightFlower(void);
void TaopoSwamp_DigHole(void);
void TaopoSwamp_BlowFlowerOffLedge(void);
void TaopoSwamp_ShowSparkleMessage(void);

u8 gTaopoSwampVentsHoleVents[] = {
    0, 2, 9, 6, 4, 6, 5, 1,
};

u8 gTaopoSwampPillarGapHoleVents[] = {
    3, 0, 1, 0, 1, 0, 0, 0,
};

// clang-format off
u32 gTaopoSwamp_PuffSteamScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ANIMATION(2),
    SC_LABEL(1),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_WAIT(1),
    SC_LOOP(30, SC_TO(1)),
    SC_RELEASE,
};

u32 gTaopoSwamp_TremorPillarScript[] = {
    SC_WAIT(40),
    SC_LABEL(1),
    SC_SET(SW_SPRITE_STORAGE_KIND, 0),
    SC_WAIT(1),
    SC_SET(SW_SPRITE_STORAGE_KIND, 1),
    SC_WAIT(1),
    SC_LOOP(10, SC_TO(1)),
    SC_RELEASE,
};

u32 gTaopoSwamp_SparkFades[] = {
    SC_WAIT(30),
    SC_RELEASE,
};

s32 gTaopoSwamp_DripAtRandomScript[] = {
    SC_CALL((s32)TaopoSwamp_LandDrop),
    SC_RELEASE,
};

s32 gTaopoSwamp_DripFromCeilingScript[] = {
    SC_CALL((s32)TaopoSwamp_LandDrip),
    SC_RELEASE,
};

s32 gTaopoSwamp_DropFalls[] = {
    SC_CALL(TaopoSwamp_IsDropFalling),
    SC_RELEASE,
};

/// Each room's vents for TaopoSwamp_StartVents: a vent's x and z in pixels and
/// the speed of its gust, ended by 0.
u16 gTaopoSwampVentsVents[] = {
    200, 200, 4,
    264, 200, 4,
    232, 216, 4,
    280, 216, 4,
    568, 200, 4,
    632, 200, 4,
    536, 216, 4,
    600, 216, 4,
    408, 232, 4,
    472, 232, 4,
    0,
};

/// The ids TaopoSwamp_StartVents keeps for the vents room, ended by 0.
u16 gTaopoSwampVentsVentPair[] = {9, 10, 0};

u16 gTaopoSwampPillarGapVents[] = {
    424, 104, 4,
    536, 136, 4,
    0,
};

u16 gTaopoSwampDripsVents[] = {
    408, 408, 4,
    472, 360, 4,
    0,
};

u16 gTaopoSwamp_RockRoomVents[] = {
    216, 184, 4,
    312, 136, 4,
    0, 0,
};

u32 gTaopoSwamp_SplashFades[] = {
    SC_WAIT(32),
    SC_RELEASE,
};

u32 gTaopoSwamp_PuffRises[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(TaopoSwampEntrance),
    SCENE_EXIT(TAOPO_SWAMP_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TAOPO_SWAMP_ENTRANCE),
    SCENE_EXIT(TAOPO_SWAMP_ENTRANCE_TO_TAOPO_SWAMP_MUD_NORTH, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_ENTRANCE_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_ENTRANCE_TO_TAOPO_SWAMP_MUD_SOUTH, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_ENTRANCE_SOUTH),
    SCENE_EXITS(TaopoSwampMud),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_ENTRANCE_NORTH, TaopoSwampEntrance, TAOPO_SWAMP_ENTRANCE_FROM_TAOPO_SWAMP_MUD_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_ENTRANCE_SOUTH, TaopoSwampEntrance, TAOPO_SWAMP_ENTRANCE_FROM_TAOPO_SWAMP_MUD_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_TEAR_STONES_WEST, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_MUD_WEST),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_POOL, TaopoSwampPool, TAOPO_SWAMP_POOL_FROM_TAOPO_SWAMP_MUD),
    SCENE_EXIT(TAOPO_SWAMP_MUD_TO_TAOPO_SWAMP_TEAR_STONES_EAST, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_MUD_EAST),
    SCENE_EXITS(TaopoSwampTearStones),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_MUD_WEST, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_TEAR_STONES_WEST),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_MUD_EAST, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_TEAR_STONES_EAST),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_VENTS_NORTH, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_TEAR_STONES_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_TEAR_STONES_TO_TAOPO_SWAMP_VENTS_SOUTH, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_TEAR_STONES_SOUTH),
    SCENE_EXIT(8, TaopoSwampTearStones, 9),
    SCENE_EXIT(9, TaopoSwampTearStones, 8),
    SCENE_EXITS(TaopoSwampFlower),
    SCENE_EXIT(TAOPO_SWAMP_FLOWER_TO_TAOPO_SWAMP_VENTS_WEST_A, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_FLOWER_WEST_A),
    SCENE_EXIT(TAOPO_SWAMP_FLOWER_TO_TAOPO_SWAMP_VENTS_EAST_A, TaopoSwampVents, TAOPO_SWAMP_VENTS_FROM_TAOPO_SWAMP_FLOWER_EAST_A),
    SCENE_EXITS(TaopoSwampVents),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_WEST_A, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_WEST_A),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_TEAR_STONES_NORTH, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_VENTS_NORTH),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_TEAR_STONES_SOUTH, TaopoSwampTearStones, TAOPO_SWAMP_TEAR_STONES_FROM_TAOPO_SWAMP_VENTS_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_A, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_A),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_WEST_B, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_WEST_B),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_WEST_C, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_WEST_C),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_SOUTHEAST, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_SOUTHEAST),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_B, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_B),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_C, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_C),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_D, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_D),
    SCENE_EXIT(TAOPO_SWAMP_VENTS_TO_TAOPO_SWAMP_FLOWER_EAST_E, TaopoSwampFlower, TAOPO_SWAMP_FLOWER_FROM_TAOPO_SWAMP_VENTS_EAST_E),
    SCENE_EXITS(TaopoSwampPool),
    SCENE_EXIT(TAOPO_SWAMP_POOL_TO_TAOPO_SWAMP_PILLAR_GAP, TaopoSwampPillarGap, TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_POOL),
    SCENE_EXIT(TAOPO_SWAMP_POOL_TO_TAOPO_SWAMP_MUD, TaopoSwampMud, TAOPO_SWAMP_MUD_FROM_TAOPO_SWAMP_POOL),
    SCENE_EXITS(TaopoSwampPillarGap),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_POOL, TaopoSwampPool, TAOPO_SWAMP_POOL_FROM_TAOPO_SWAMP_PILLAR_GAP),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_SOUTH, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_NORTHWEST, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_NORTHWEST),
    SCENE_EXIT(4, TaopoSwampPillarGap, 5),
    SCENE_EXIT(5, TaopoSwampPillarGap, 4),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_EAST_A, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_EAST),
    SCENE_EXIT(TAOPO_SWAMP_PILLAR_GAP_TO_TAOPO_SWAMP_DRIPS_EAST_B, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_PILLAR_GAP_EAST),
    SCENE_EXITS(TaopoSwampDrips),
    SCENE_EXIT(TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_TREMOR, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_DRIPS),
    SCENE_EXIT(TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_PILLAR_GAP_SOUTH, TaopoSwampPillarGap, TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_DRIPS_SOUTH),
    SCENE_EXIT(TAOPO_SWAMP_DRIPS_TO_TAOPO_SWAMP_PILLAR_GAP_NORTHWEST, TaopoSwampPillarGap, TAOPO_SWAMP_PILLAR_GAP_FROM_TAOPO_SWAMP_DRIPS_NORTHWEST),
    SCENE_EXIT(4, TaopoSwampDrips, 5),
    SCENE_EXIT(5, TaopoSwampDrips, 4),
    SCENE_EXITS(TaopoSwampTremor),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_DRIPS, TaopoSwampDrips, TAOPO_SWAMP_DRIPS_FROM_TAOPO_SWAMP_TREMOR),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_STAR_DUST, TaopoSwampStarDust, TAOPO_SWAMP_STAR_DUST_FROM_TAOPO_SWAMP_TREMOR),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_LAVA_ROCKS_EAST, TaopoSwampLavaRocks, TAOPO_SWAMP_LAVA_ROCKS_FROM_TAOPO_SWAMP_TREMOR_EAST),
    SCENE_EXIT(TAOPO_SWAMP_TREMOR_TO_TAOPO_SWAMP_LAVA_ROCKS_WEST, TaopoSwampLavaRocks, TAOPO_SWAMP_LAVA_ROCKS_FROM_TAOPO_SWAMP_TREMOR_WEST),
    SCENE_EXITS(TaopoSwampLavaRocks),
    SCENE_EXIT(TAOPO_SWAMP_LAVA_ROCKS_TO_TAOPO_SWAMP_TREMOR_EAST, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_LAVA_ROCKS_EAST),
    SCENE_EXIT(TAOPO_SWAMP_LAVA_ROCKS_TO_TAOPO_SWAMP_TREMOR_WEST, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_LAVA_ROCKS_WEST),
    SCENE_EXITS(TaopoSwampStarDust),
    SCENE_EXIT(TAOPO_SWAMP_STAR_DUST_TO_TAOPO_SWAMP_TREMOR, TaopoSwampTremor, TAOPO_SWAMP_TREMOR_FROM_TAOPO_SWAMP_STAR_DUST),
    SCENE_EXIT(3, TaopoSwampStarDust, 4),
    SCENE_EXIT(4, TaopoSwampStarDust, 3),
    SCENE_EXIT(5, TaopoSwampStarDust, 6),
    SCENE_EXIT(6, TaopoSwampStarDust, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gTaopoSwamp_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampMudActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 128, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 112, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 112, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 176, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 256, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 272, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, FLAG_TAOPO_SWAMP_STUMP_SUNK, MOVEMENT_TYPE_STAND, 216, 0, 264, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampFlowerActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_FLOWER), MOVEMENT_TYPE_STAND, 440, 0, 632, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampVentsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_FLOWER), MOVEMENT_TYPE_STAND, 408, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampPillarGapActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampDripsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampTremorActorPlacements[] = {
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 296, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 360, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 376, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 336, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 352, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampLavaRocksActorPlacements[] = {
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 360, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwampStarDustActorPlacements[] = {
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 632, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gTaopoSwamp_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampEntranceTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampMudTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 3, IF_FLAG_SET(0x8e0), 3},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 4, IF_FLAG_SET(0x8e3), 4},
    {TRIGGER_EXIT | STEP_TRIGGER_FLAG_WALKING | STEP_TRIGGER_FLAG_QUICK | TRIGGER_FLAG_FACING | FACING_NORTH, 5, IF_FLAG_SET(0x8e4), 5},
    {TRIGGER_STEP, 60, CONDITION_ALWAYS, EVENT(TaopoSwamp_SinkInMud)},
    {TRIGGER_STEP, 61, CONDITION_ALWAYS, EVENT(TaopoSwamp_RiseOnBubble)},
    {TRIGGER_STEP, 62, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnFirmGround)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_BlowAwayBush)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 13, CONDITION_ALWAYS, EVENT(TaopoSwamp_GrowVine)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(TaopoSwamp_SinkPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampTearStonesTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 51, FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(1), EVENT(TaopoSwamp_ScoopHiddenItem)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50, FLAG_TAOPO_SWAMP_TREASURE_DUG_UP(0), EVENT(TaopoSwamp_ScoopOtherHiddenItem)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampFlowerTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(TaopoSwamp_FightFlower)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(TaopoSwamp_DigHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(TaopoSwamp_DigHoleUnderfoot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(TaopoSwamp_StartMadPlantBattle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampVentsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 14, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 16, CONDITION_ALWAYS, EVENT(TaopoSwamp_FallThroughHole)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 17, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 18, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_VENT_PILLAR, FLAG_TAOPO_SWAMP_WEST_PILLAR_OVER_VENT, EVENT(TaopoSwamp_PushPillarOverVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_VENT_PILLAR, FLAG_TAOPO_SWAMP_EAST_PILLAR_OVER_VENT, EVENT(TaopoSwamp_PushPillarOverVent)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TaopoSwamp_PushPillarOverVent)},
    {TRIGGER_RAISED, 100, FLAG_TAOPO_SWAMP_FLOWER_OFF_LEDGE, EVENT(TaopoSwamp_BlowFlowerOffLedge)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampPoolTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gTaopoSwampPillarGapTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT(TaopoSwamp_FallThroughHole)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GAP_PILLAR,
     FLAG_TAOPO_SWAMP_PILLAR_IN_GAP, EVENT(TaopoSwamp_DropPillarIntoGap)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TaopoSwamp_DropPillarIntoGap)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS),
     ACTOR_GAP_PUDDLE, FLAG_TAOPO_SWAMP_SPOT_WET,
     EVENT(TaopoSwamp_DouseVentSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_GAP_PUDDLE,
     IF_FLAG_SET(FLAG_TAOPO_SWAMP_SPOT_PUDDLE),
     EVENT(TaopoSwamp_FreezeVentSpot)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampDripsTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x308, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS),
     ACTOR_DRIP_PUDDLE, FLAG_TAOPO_SWAMP_SPOT_WET,
     EVENT(TaopoSwamp_FillDripSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_DRIP_PUDDLE,
     IF_FLAG_SET(FLAG_TAOPO_SWAMP_SPOT_PUDDLE),
     EVENT(TaopoSwamp_FreezeDripSpot)},
    {TRIGGER_TABLE_END},
};

s32 gTaopoSwamp_SteamPuffs = 112;

s32 gTaopoSwamp_PassageRows = 120;

s32 gTaopoSwamp_PoolFrames = 24;

struct Trigger gTaopoSwampTremorTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TREMOR_LAVA_ROCK,
     FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_COOLED, EVENT(TaopoSwamp_CoolLavaRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_TREMOR_PILLAR_ROCK,
     FLAG_TAOPO_SWAMP_PILLAR_ROCK_COOLED, EVENT(TaopoSwamp_CoolPillarRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREMOR_LAVA_ROCK,
     FLAG_TAOPO_SWAMP_TREMOR_LAVA_ROCK_UNMOVABLE,
     EVENT(TaopoSwamp_PushLavaRockIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREMOR_PILLAR_ROCK,
     FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE,
     EVENT(TaopoSwamp_PushPillarRockIn)},
    {TRIGGER_PUSHED, 0, FLAG_TAOPO_SWAMP_PILLAR_ROCK_UNMOVABLE,
     EVENT(TaopoSwamp_PushPillarRockIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), ACTOR_TREMOR_PILLAR,
     FLAG_TAOPO_SWAMP_PASSAGE_OPEN, EVENT(TaopoSwamp_TremorPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampLavaRocksTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
#if TLA_PATCH_1
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 64, CONDITION_ALWAYS,
     EVENT(TaopoSwamp_JumpForward)},
#endif
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_CHEST_ROCK,
     FLAG_TAOPO_SWAMP_CHEST_ROCK_COOLED, EVENT(TaopoSwamp_CoolChestRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_EXIT_ROCK,
     FLAG_TAOPO_SWAMP_EXIT_ROCK_COOLED, EVENT(TaopoSwamp_CoolExitRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CHEST_ROCK,
     FLAG_TAOPO_SWAMP_CHEST_ROCK_UNMOVABLE, EVENT(TaopoSwamp_PushChestRockIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EXIT_ROCK,
     FLAG_TAOPO_SWAMP_EXIT_ROCK_UNMOVABLE, EVENT(TaopoSwamp_PushExitRockIn)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwampStarDustTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), 50, FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP,
     EVENT(TaopoSwamp_ShowSparkleMessage)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_DARK_ROCK,
     FLAG_TAOPO_SWAMP_DARK_ROCK_COOLED, EVENT(TaopoSwamp_CoolDarkRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_LAST_ROCK,
     FLAG_TAOPO_SWAMP_LAST_ROCK_COOLED, EVENT(TaopoSwamp_CoolLastRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DARK_ROCK,
     FLAG_TAOPO_SWAMP_DARK_ROCK_UNMOVABLE, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_ROCK,
     FLAG_TAOPO_SWAMP_LAST_ROCK_UNMOVABLE, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50,
     FLAG_TAOPO_SWAMP_STAR_DUST_DUG_UP, EVENT(TaopoSwamp_ScoopStarDust)},
    {TRIGGER_TABLE_END},
};

s32 gTaopoSwamp_SparkleFrame = 0;

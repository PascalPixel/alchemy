/// @file
/// Taopo Swamp, north of Yallam: mud that swallows the party back to firm
/// ground, vents whose gusts throw it about, puddles Douse fills and Frost
/// freezes, lava rocks to cool and push, drops falling in the dark rooms, the
/// Mad Plant in its bush and Flower, the Venus Djinni.

#include "flags.h"
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "system/iwram_call.h"
#include "field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "system/vector3.h"
#include "version.h"
#include "event_work.h"
#include "call.h"
#include "field/field.h"
#include "graphics/graphics.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "module/scene/lemurian_ship.h"

extern u32 gSceneExits[];

extern char SceneId_TaopoSwamp1;
extern char SceneId_TaopoSwamp2;
extern char SceneId_TaopoSwamp3;
extern char SceneId_TaopoSwamp4;
extern char SceneId_TaopoSwamp5;
extern char SceneId_TaopoSwamp6;
extern char SceneId_TaopoSwamp7;
extern char SceneId_TaopoSwamp8;
extern char SceneId_TaopoSwamp9;
extern char SceneId_TaopoSwamp10;
extern char SceneId_TaopoSwamp11;

void GameFlag_SetByte(s32 flag, s32 value);
void Vent_SetGust(s32 id, s32 a);
void Vents_SetId(s32 a, s32 b);
void Event_End(void);
void Actor_JumpDownLedge(s32 id, s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Actor_ReturnHome(s32 a, s32 b, s32 c);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void FieldItem_JumpToLeader(s32 a, s32 b, s32 c);
void Actor_StartScript(struct Actor *object, const void *data);
void Actor_SetBlendMode(struct Actor *object, u32 value);

extern s32 gTaopoSwamp_SteamPuffs;
extern s32 gTaopoSwamp_PassageRows;
extern s32 gTaopoSwamp_PoolFrames;
extern u32 gTaopoSwamp_PuffSteamScript[];
void Field_TryJumpForward(void);
void TaopoSwamp_DarkenRoom(s32 a);
void Func_020029a0(s32 message);
void Actor_SelectAnimation(struct Actor *object, s32 a);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 a);
s32 Vents_GetCounter(void);
#ifdef MODERN
void Vent_SetActive(s32 index, s32 active);
#endif
s32 Vents_AdvanceCounter(s32 a);
void Scene_QueueMusic(s32 a);

struct SpriteWork {
  u8 unk00[17];
  u8 layer : 2;
  u8 level : 6;
  u8 unk12[7];
  u8 dirty;
  u8 mode;
  u8 unk1b[17];
  u8 *child;
};

struct SceneState {
  s16 mode;
  s16 timer;
  s32 x;
  s32 z;
};

extern struct SceneState gSceneState;

typedef volatile u16 vu16;

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
struct VentEntry {
  s16 active;
  s16 timer;
  s32 x;
  s32 y;
  s32 z;
  s32 value;
};

struct VentWork {
  s32 counter;
  s32 (*hook)(void);
  struct VentEntry entries[16];
  u16 ids[4];
};

extern u8 gInventorySnapshot[];
#define VENT_WORK ((struct VentWork *)(gInventorySnapshot + 0x188))
#define DIFF(a, b) ((a) - (b) >= 0 ? (a) - (b) : (b) - (a))

s32 Trig_Cos(s32 angle);
void Vent_Bounce(struct Actor *object);
extern u32 gTaopoSwamp_PuffRises[];
extern s32 gTaopoSwamp_SparkleFrame;
extern u8 gMapShapeGrid[];
void Map_CenterOnFollowedPoint(void);
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

// DEFINE_TAOPO_SWAMP_FREEZE_SPOT below calls it by this name.
static __inline__ void TaopoSwamp_CopyCellAttributes(s32 a, s32 b, s32 c, s32 d,
                                                     s32 e, s32 f) {
  MapGrid_CopyCellAttributes(a, b, c, d, e, f);
}

static __inline__ void TaopoSwamp_JumpDownLedge(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

static __inline__ void TaopoSwamp_ReturnHome(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

static __inline__ void Func_02002990_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                      s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

static __inline__ struct Actor *TaopoSwamp_CreateActor(s32 type, s32 x, s32 y,
                                                       s32 z) {
  return Import_ActorCreate(type, x, y, z);
}

static __inline__ void Func_020029a8_(s32 a, s32 b, s32 c) {
  FieldItem_JumpToLeader(a, b, c);
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
void TaopoSwamp_FightFlower(void) { Field_RequestDjinnBattle(8, 11); }

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gTaopoSwamp_Placements[];
extern struct ActorPlacement gTaopoSwamp_Room2Placements[];
extern struct ActorPlacement gTaopoSwamp_Room4Placements[];
extern struct ActorPlacement gTaopoSwamp_Room5Placements[];
extern struct ActorPlacement gTaopoSwamp_Room7Placements[];
extern struct ActorPlacement gTaopoSwamp_Room8Placements[];
extern struct ActorPlacement gTaopoSwamp_Room10Placements[];
extern struct ActorPlacement gTaopoSwamp_Room9Placements[];
extern struct ActorPlacement gTaopoSwamp_Room11Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_TaopoSwamp2)
    return gTaopoSwamp_Room2Placements;
  if (scene == (s32)&SceneId_TaopoSwamp4)
    return gTaopoSwamp_Room4Placements;
  if (scene == (s32)&SceneId_TaopoSwamp5)
    return gTaopoSwamp_Room5Placements;
  if (scene == (s32)&SceneId_TaopoSwamp7)
    return gTaopoSwamp_Room7Placements;
  if (scene == (s32)&SceneId_TaopoSwamp8)
    return gTaopoSwamp_Room8Placements;
  if (scene == (s32)&SceneId_TaopoSwamp9)
    return gTaopoSwamp_Room9Placements;
  if (scene == (s32)&SceneId_TaopoSwamp10)
    return gTaopoSwamp_Room10Placements;
  if (scene == (s32)&SceneId_TaopoSwamp11)
    return gTaopoSwamp_Room11Placements;
  return gTaopoSwamp_Placements;
}

/// Whirlwind on one of the bushes 8 to 12: its flag (0x8d8 plus its id) keeps
/// it gone.
void TaopoSwamp_BlowAwayBush(s32 a, s32 b) { GameFlag_Set(b + 0x8d8); }

/// Growth on the sprout, actor 13: the vine's cells become climbable.
void TaopoSwamp_GrowVine(void) {
  MapGrid_CopyCellAttributes(50, 16, 1, 2, 47, 16);
  Event_SetFlag(FLAG_SCENE_TEMP_20);
}

struct AnimationGroup;
struct ActorSpriteView;
struct ChildGroup;

extern u32 gTaopoSwamp_SplashFades[];
s32 Animation_ApplyChildArgument(struct AnimationGroup *state, s32 flags);
void Animation_ApplyChildValuesToRecord(struct ActorSpriteView *record,
                                        u32 value);
void Animation_ApplyChildValue(struct ChildGroup *group, s32 value);

// When actor id stands at x 12, inside an event: the actor drops in place, then
// for 25 frames throws splashes, then two sparks, and sets flag 0x8e5.
/// Move on the small pillar, actor 14, once it stands at cell x 12: it sinks
/// into the mud with splashes and two sparks, and flag 0x8e5 keeps it gone.
void TaopoSwamp_SinkPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  struct Actor *splash, *spark;
  struct FieldSprite *sprite, *splashSprite, *sparkSprite;
  s32 i, x, z, k, zero;

  if (actor->x >> 20 != 12)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->motionFlags = 3;
  System_Wait(5);
  actor->motionFlags = 0;
  actor->velocityY = 0;
  actor->y = px(4);
  sprite = actor->sprite;
  i = 0;
  do {
    ((struct Charge *)sprite)->level = i;
    sprite->nextFrame |= 8;
    sprite->nextFrame &= ~1;
    k = i;
    sprite->unk19 = 1;
    k &= 3;
    if (k == 0) {
      x = actor->x + Random16() * 2 - Random16() * 2;
      z = actor->z + Random16() * 2 - Random16() * 2;
      splash = (FieldActor_Create(14, x, actor->y, z));
      if (splash != 0) {
        splashSprite = splash->sprite;
        splash->terrainHeight = actor->terrainHeight;
        Actor_StartScript(splash, gTaopoSwamp_SplashFades);
        splash->motionFlags = 0;
        if (splashSprite != 0) {
          ((s32 (*)())Animation_ApplyChildArgument)(splashSprite, 1);
          splashSprite->nextFrame = k;
          splashSprite->priority = sprite->priority;
          Animation_ApplyChildValuesToRecord(splashSprite, 13);
          Animation_ApplyChildValue(splashSprite, 8);
        }
      }
    }
    System_Wait(3);
    i++;
  } while (i <= 24);

  actor->animationKind = 0;
  actor->y = 0;
  for (i = 1; i >= 0; i--) {
    spark = (FieldActor_Create(269, actor->x, actor->y, actor->z));
    if (spark != 0) {
      sparkSprite = spark->sprite;
      Actor_StartScript(spark, gTaopoSwamp_SplashFades);
      {
        u8 *flags = &spark->motionFlags;

        zero = 0;
        *flags = zero;
      }
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite->nextFrame = zero;
        sparkSprite->priority = 2;
      }
    }
    System_Wait(15);
  }
  FieldEvent_Wait(30);
  actor->animationKind = 1;
  Actor_SetPosition(id, 0, 0);
  Event_End();
  Event_SetFlag(0x8e5);
}

/// Scoop on the spot under actor 65 digs up its item (flag 0x301).
void TaopoSwamp_ScoopHiddenItem(s32 a) { TaopoSwamp_ReturnHome(a, 65, 0x301); }

/// Scoop on the spot under actor 64 digs up its item (flag 0x300).
void TaopoSwamp_ScoopOtherHiddenItem(s32 a) {
  TaopoSwamp_ReturnHome(a, 64, 0x300);
}

/// Scoop digs into the mud at one of the two spots of the lowest level.
void TaopoSwamp_DigHole(void) { Func_02002990_(76, 70, 14, 8, 14, 37, 0); }

/// As TaopoSwamp_DigHole, and the first time (until flag 0xf99) the buried item
/// comes up, throwing the leader standing on it back a step.
void TaopoSwamp_DigHoleUnderfoot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Func_02002990_(76, 70, 14, 8, 14, 37, 0);
  if (Event_GetFlag(0xf99) == 0) {
    Actor_SetPosition(64, 264, 664);
    if (leader->x >> 20 == 16 && leader->z >> 20 == 41) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -16);
      leader->velocityY = px(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

struct MainRoutineWork254 {
  u8 unk000[0x254];
  u16 value;
};

/// Until flag 0x9b6 is set, the bush hides the Mad Plant, which the party
/// fights from encounter table 106.
void TaopoSwamp_StartMadPlantBattle(void) {
  Func_02002990_(76, 70, 14, 8, 14, 37, 0);
  if (!Event_GetFlag(0x9b6)) {
    Func_020029a8_(249, 344, 648);
    ((struct MainRoutineWork254 *)&gMainRoutineWork)->value = 0x19b6;
    gMainRoutineWork.battleRule24b = 2;
    Field_RequestBattleFromEncounterTable(106, 1);
  }
}

/// The leader drops through a hole in the floor to the room below.
void TaopoSwamp_FallThroughHole(s32 a) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  TaopoSwamp_JumpDownLedge(gMainRoutineWork.currentOwner, 6, 0x1000);
  Event_PlaySound(SE_310);
  Event_SetFlag(0x122);
  FieldEvent_SetExitRequest(a);
}

/// Move on pillar 9 or 10 to the cell over its vent: the vent's gust is
/// redirected, and flag 0xa6d or 0xa6e keeps the pillar there.
void TaopoSwamp_PushPillarOverVent(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 9) {
    if (object->x >> 20 == 17) {
      object->z += px(1);
      Vent_SetGust(2, 0x80000);
      Event_SetFlag(0xa6d);
    }
  } else if (id == 10) {
    if (object->x >> 20 == 29) {
      object->z += px(1);
      Vent_SetGust(8, 0x80000);
      Event_SetFlag(0xa6e);
    }
  }
}

extern u8 gTaopoSwamp_Room5HoleVents[];
extern u8 gTaopoSwamp_Room7HoleVents[];

/// Stepping on a working vent: the gust pushes the leader back until a wall
/// stops them, or down the hole.
void TaopoSwamp_StepOnVent(s32 hole) {
  struct Actor *leader;
  struct Vector3 position;
  s32 found, result, slip;

  if (gMainRoutineWork.scene == (s32)&SceneId_TaopoSwamp5) {
    found = Vent_IsActive(gTaopoSwamp_Room5HoleVents[hole - 10]);
    if (hole == 12 && Event_GetFlag(0xa6e))
      return;
    if (hole == 18 && Event_GetFlag(0xa6d))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_TaopoSwamp7) {
    u8 *table = gTaopoSwamp_Room7HoleVents + 1;
    s32 at = hole - 10;
    found = Vent_IsActive(table[at]);
    if (hole == 10 && Event_GetFlag(FLAG_SCENE_TEMP_21))
      return;
    if (hole == 12 && Event_GetFlag(FLAG_SCENE_TEMP_21))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_TaopoSwamp8) {
    found = Vent_IsActive(hole - 10);
    if (hole == 10 && Event_GetFlag(FLAG_SCENE_TEMP_21))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_TaopoSwamp9)
    found = Vent_IsActive(hole - 10);
  if (found == 0)
    return;
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  goto test;
sink:
  if (result < 0) {
    leader->facing = 0x4000;
    TaopoSwamp_FallThroughHole(hole);
    return;
  }
  leader->z += px(2);
  slip = (leader->x & 0xfffff) + 0xfff80000;
  {
    s32 limit = 0x10001;

    if (slip >= limit)
      slip = 0x10000;
    limit = -0x10001;
    if (slip <= limit)
      slip = -0x10000;
  }
  leader->x -= slip;
  leader->facing += 0x1000;
  System_Wait(1);
test:
  position.x = leader->x;
  position.y = leader->y;
  position.z = leader->z + px(8);
  result = MapGrid_CheckCollision(leader, &position);
  if (result <= 0)
    goto sink;
}

/// The gust from the covered vent knocks the Venus Djinni, actor 8, down one
/// level in three hops (flag 0x310).
void TaopoSwamp_BlowFlowerOffLedge(void) {
  struct Actor *object = Actor_Get(8);
  s32 speed;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceActor(gMainRoutineWork.currentOwner, 8, 0);
  FieldActor_ShowEmote(gMainRoutineWork.currentOwner,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  FieldCamera_MoveTo(408, CAMERA_KEEP, 312, 1);
  FieldCamera_WaitForArrival();
  Actor_SetAttachedEffect(8, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  speed = 0x8000;
  i = 2;
  do {
    object->velocityY = px(2);
    Event_PlaySound(SE_JUMP);
    object->speedLimit = speed;
    object->acceleration = speed / 2;
    Actor_WalkByAndWait(8, 0, 8);
    speed += 0x4000;
  } while (--i >= 0);
  Event_PlaySound(SE_310);
  object->velocityY = px(4);
  Actor_WalkByAndWait(8, 0, 8);
  FieldEvent_Wait(60);
  Event_SetFlag(FLAG_AREA_TEMP_10);
  Event_End();
}

#include "shared/actor_wait_to_land.h"

/// Moved to column 16, the pillar drops into the gap there (flag 0xa6f).
void TaopoSwamp_DropPillarIntoGap(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 9 && object->x >> 20 == 16) {
    Actor_WaitToLand(9);
    MapGrid_CopyCellAttributes(15, 26, 1, 1, 16, 26);
    Event_SetFlag(0xa6f);
  }
}

/// Douse on the circular spot, actor 8: a puddle the next gust blows away
/// unless Frost turns it to an ice pillar first.
void TaopoSwamp_DouseVentSpot(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_SetAnimation(id, 1);
  Event_SetFlag(FLAG_SCENE_TEMP_20);
  Event_SetFlag(FLAG_SCENE_TEMP_22);
}

/// Ice on a spot: the two copies differ only in their cells.
#define DEFINE_TAOPO_SWAMP_FREEZE_SPOT(name, n1, n2, n3)                       \
  void name(s32 a, s32 id) {                                                   \
    TaopoSwamp_CopyCellAttributes(n1, n2, 1, 1, n3, n2);                       \
    FieldActor_SetSpritePriority(id, 2);                                       \
    Actor_Get(id)->priorityFlags &= 0xfd;                                      \
    Vents_SetId(0, id);                                                        \
    Event_SetFlag(FLAG_SCENE_TEMP_21);                                         \
    Event_ClearFlag(FLAG_SCENE_TEMP_22);                                       \
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
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = (void (*)(void))zero;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_20);
  Event_ClearFlag(FLAG_SCENE_TEMP_21);
  Event_ClearFlag(FLAG_SCENE_TEMP_22);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  MapGrid_CopyCellAttributes(22, 8, 1, 1, 26, 8);
}

/// The gust blows the puddle off the vent spot.
void TaopoSwamp_BlowAwayPuddle(s32 id) {
  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = 0;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_20);
  Event_ClearFlag(FLAG_SCENE_TEMP_22);
}

/// A drop or Douse on the spot under the drip: the puddle is frozen solid by
/// Frost once it is full (Douse, or more than three drops).
void TaopoSwamp_FillDripSpot(s32 a, s32 id) {
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_SetAnimation(id, 1);
  if (a == 5 || FieldGameFlag_GetByte(0x230) > 3) {
    Event_SetFlag(FLAG_SCENE_TEMP_20);
    Event_SetFlag(FLAG_SCENE_TEMP_22);
  }
}

DEFINE_TAOPO_SWAMP_FREEZE_SPOT(TaopoSwamp_FreezeDripSpot, 23, 27, 25)

/// As TaopoSwamp_ShatterIcePillar, for the spot the ceiling drips onto, whose
/// puddle size (byte flag 0x230) starts again.
void TaopoSwamp_ShatterDripIcePillar(s32 id) {
  struct Actor *leader;
  struct Actor *object;

  Actor_SetAnimation(id, 8);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = 0;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_20);
  Event_ClearFlag(FLAG_SCENE_TEMP_21);
  Event_ClearFlag(FLAG_SCENE_TEMP_22);
  GameFlag_SetByte(0x230, 0);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  object = Actor_Get(id);
  if (object->x >> 20 == leader->x >> 20 &&
      object->z >> 20 == leader->z >> 20) {
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_WalkByAndWait(gMainRoutineWork.currentOwner, 0, 16);
    FieldEvent_Wait(2);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
  }
  MapGrid_CopyCellAttributes(1, 1, 1, 1, 25, 27);
}

/// As TaopoSwamp_BlowAwayPuddle, for the spot the ceiling drips onto.
void TaopoSwamp_BlowAwayDripPuddle(s32 id) {
  s32 zero = 0;

  Actor_SetAnimation(id, 7);
  FieldActor_SetSpritePriority(id, 3);
  Actor_Get(id)->priorityFlags |= 2;
  Actor_Get(id)->update = (void (*)(void))zero;
  Vents_SetId(0, 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_20);
  GameFlag_SetByte(0x230, 0);
}

/// Douse on the red glowing rock by the lava, actor 9: it cools, and Move can
/// push it.
void TaopoSwamp_CoolLavaRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_AREA_TEMP_31);
  Event_ClearFlag(FLAG_AREA_TEMP_32);
}

/// Move on the cooled rock at cell x 7: it sinks into the lava as a stepping
/// stone.
void TaopoSwamp_PushLavaRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 7) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(FLAG_AREA_TEMP_32);
    Event_SetFlag(FLAG_AREA_TEMP_33);
    MapGrid_CopyCellAttributes(7, 17, 1, 1, 7, 15);
  }
}

/// Douse on the red rock Tremor knocked out of the pillar, actor 10.
void TaopoSwamp_CoolPillarRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_AREA_TEMP_34);
  Event_ClearFlag(FLAG_AREA_TEMP_35);
  MapGrid_CopyCellAttributes(27, 14, 1, 1, 27, 13);
}

/// Move on the cooled pillar rock at cell z 16: it slides two cells and sinks
/// into the lava.
void TaopoSwamp_PushPillarRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->z >> 20 == 16) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(FLAG_AREA_TEMP_35);
    Event_SetFlag(FLAG_AREA_TEMP_36);
    Actor_SetSpeed(id, 0.5);
    Actor_SetDestinationOffset(id, -32, 0);
    FieldActor_WaitForMoveById(id);
    Actor_SetAnimation(id, 3);
    MapGrid_CopyCellAttributes(25, 18, 1, 1, 25, 16);
  }
}

void TaopoSwamp_SpinBoulder(struct Actor *object) {
  *(s16 *)((u8 *)object->sprite + 18) -= 0x800;
}

/// One puff of steam from the cooling lava, until the count runs out.
void TaopoSwamp_PuffSteam(void) {
  s32 a;
  s32 x;
  s32 z;
  struct Actor *object;

  gTaopoSwamp_SteamPuffs--;
  a = Random16() * 32;
  x = Random16() * 16 + a + 0x12c0000;
  z = Random16() * 16 + a + 0x1040000;
  object = FieldActor_Create(0x11d, x, 0, z);
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
  struct Actor *rock = Actor_Get(19);
  struct Actor *clod;
  s32 i, row, cosine;
  s32 (*mul)(s32, s32);
  u32 angle, speed, scale;
  s32 turn = -256;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (i = 9; i >= 0; i--) {
    System_Wait(1);
    ((struct FieldSprite *)rock->sprite)->rotation += turn;
    rock->x -= Trig_Cos(((struct FieldSprite *)rock->sprite)->rotation) / 2;
    rock->targetX = 0x80000000;
  }
  rock->update = TaopoSwamp_SpinBoulder;
  Actor_SetSpeed(19, 2);
  Actor_SetDestination(19, 492, 246);
  rock->gravity = 0xcccc;
  rock->motionFlags = 3;
  rock->terrainId = 0;
  FieldActor_WaitForMoveById(19);
  rock->velocityY = 0;
  rock->y = rock->terrainHeight;
  Actor_SetPosition(19, 480, 248);
  ((struct FieldSprite *)rock->sprite)->rotation = 0;
  Event_PlaySound(SE_THUMP);
  MapGrid_SetShake(5, 5, 1);
  Event_PlaySound(SE_RUMBLE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Actor_SetPosition(10, 480, 248);
  Actor_SetDestination(10, 440, 232);
  Actor_Get(10)->velocityY = px(10);
  rock->update = 0;
  i = 15;
burst:
  clod = (FieldActor_Create(255, rock->x, rock->y + px(16), rock->z));
  if (clod == 0)
    goto next;
  angle = Random16();
  speed = Random16();
  scale = (Random16() >> 2) + 0x1000;
  Actor_StartScript(clod, gTaopoSwamp_TremorPillarScript);
  Actor_SetSpriteFlags(clod, 0);
  clod->velocityY = (angle << 2) + 0x20000;
  cosine = Trig_Cos(angle);
  mul = Iwram_MulQ16;
  speed <<= 3;
  clod->velocityZ = mul(speed, cosine);
  clod->velocityX = mul(speed, Math_Sin(angle));
  clod->acceleration = 0;
  clod->gravity = 0x547a;
  clod->scaleX = scale;
  clod->scaleY = scale;
  clod->bounce = Q16_ONE / 2;
next:
  i--;
  if (i >= 0)
    goto burst;
  FieldMapGrid_WaitForShakeToSettle();
  Task_AddFrame(TaopoSwamp_OpenPassage, 0x480);
  System_Wait(40);
  FieldCamera_SetSpeed(0.9);
  FieldCamera_MoveTo(336, CAMERA_KEEP, 280, 1);
  i = 11;
  row = 114;
drain:
  MapGrid_CopyCells(50, row, 13, 5, 18, 15);
  System_Wait(8);
  row -= 5;
  i--;
  if (i > 0)
    goto drain;
  Task_AddFrame(TaopoSwamp_PuffSteam, 0x480);
  System_Wait(60);
  Task_AddFrame(TaopoSwamp_FlickerPool, 0x480);
  System_Wait(24);
  FieldEvent_Wait(120);
  Event_End();
  Event_SetFlag(FLAG_AREA_TEMP_30);
}

/// Douse on the red rock in the lower left corner, actor 8, on the way to the
/// Vial's chest.
void TaopoSwamp_CoolChestRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_AREA_TEMP_38);
  Event_ClearFlag(FLAG_AREA_TEMP_39);
}

/// Move on the cooled rock at cell z 21: it sinks into the lava.
void TaopoSwamp_PushChestRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->z >> 20 == 21) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(FLAG_AREA_TEMP_39);
    Event_SetFlag(FLAG_AREA_TEMP_3A);
    MapGrid_CopyCellAttributes(11, 19, 1, 1, 11, 21);
  }
}

/// Douse on the red rock in the upper right corner, actor 9, by the other way
/// out.
void TaopoSwamp_CoolExitRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_AREA_TEMP_3B);
  Event_ClearFlag(FLAG_AREA_TEMP_3C);
}

/// Move on the cooled rock at cell x 31: it slides two cells and sinks into the
/// lava.
void TaopoSwamp_PushExitRockIn(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 31) {
    Actor_SetAnimation(id, 3);
    object->collisionFlags = 0;
    object->priorityFlags |= 2;
    Event_SetFlag(FLAG_AREA_TEMP_3C);
    Event_SetFlag(FLAG_AREA_TEMP_3D);
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
  Event_SetFlag(FLAG_AREA_TEMP_40);
  Event_ClearFlag(FLAG_AREA_TEMP_41);
  MapGrid_CopyCellAttributes(7, 39, 1, 1, 7, 38);
  TaopoSwamp_DarkenRoom(1);
}

/// Douse on the glowing rock of the last chamber, actor 9: the lights go out
/// and the Star Dust's sparkle shows in the dark.
void TaopoSwamp_CoolLastRock(s32 a, s32 id) {
  Actor_SetAnimation(id, 2);
  Event_SetFlag(FLAG_AREA_TEMP_42);
  Event_ClearFlag(FLAG_AREA_TEMP_43);
  TaopoSwamp_DarkenRoom(1);
}

/// Scoop on the sparkle in the dark chamber digs up the Star Dust (flag 0x348).
void TaopoSwamp_ScoopStarDust(s32 a) { TaopoSwamp_ReturnHome(a, 64, 0x348); }

void TaopoSwamp_ShowSparkleMessage(void) { Func_020029a0((s32)&MsgIThoughtI); }

extern struct Trigger gTaopoSwamp_Triggers[];
extern struct Trigger gTaopoSwamp_Room1Triggers[];
extern struct Trigger gTaopoSwamp_Room2Triggers[];
extern struct Trigger gTaopoSwamp_Room3Triggers[];
extern struct Trigger gTaopoSwamp_Room4Triggers[];
extern struct Trigger gTaopoSwamp_Room5Triggers[];
extern struct Trigger gTaopoSwamp_Room6Triggers[];
extern struct Trigger gTaopoSwamp_Room7Triggers[];
extern struct Trigger gTaopoSwamp_Room8Triggers[];
extern struct Trigger gTaopoSwamp_Room10Triggers[];
extern s32 gTaopoSwamp_Room9Triggers[];
extern struct Trigger gTaopoSwamp_Room11Triggers[];

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_TaopoSwamp1)
    return (const struct Trigger *)gTaopoSwamp_Room1Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp2)
    return (const struct Trigger *)gTaopoSwamp_Room2Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp3)
    return (const struct Trigger *)gTaopoSwamp_Room3Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp4)
    return (const struct Trigger *)gTaopoSwamp_Room4Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp5)
    return (const struct Trigger *)gTaopoSwamp_Room5Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp6)
    return (const struct Trigger *)gTaopoSwamp_Room6Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp7)
    return (const struct Trigger *)gTaopoSwamp_Room7Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp8)
    return (const struct Trigger *)gTaopoSwamp_Room8Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp9)
    return (const struct Trigger *)gTaopoSwamp_Room9Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp10)
    return (const struct Trigger *)gTaopoSwamp_Room10Triggers;
  if (scene == (s32)&SceneId_TaopoSwamp11)
    return (const struct Trigger *)gTaopoSwamp_Room11Triggers;
  return (const struct Trigger *)gTaopoSwamp_Triggers;
}

/// A falling drop splashes where it lands.
s32 TaopoSwamp_LandDrop(struct Actor *object) {
  s32 y = object->y;
  struct Actor *spark;
  s32 scale;

  if (y + object->velocityY < object->terrainHeight) {
    spark = TaopoSwamp_CreateActor(0x10d, object->x, y, object->z);
    scale = 0x8ccc;
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Actor_StartScript(spark, gTaopoSwamp_SparkFades);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 2);
    }
    Event_PlaySound(SE_DRIP);
    object->y = object->terrainHeight;
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
    s32 y = 0xa00000;
    struct Actor *object = FieldActor_Create(0x12d, x, y, z);
    s32 scale = 0x547a;

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
    Scene_QueueMusic(SE_170);
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
    Scene_QueueMusic(SE_170);
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
  if ((u32)(timer - 251) <= 178 && Event_GetFlag(0xa6e) && !Event_GetFlag(59) &&
      !Event_GetFlag(FLAG_AREA_TEMP_10))
    ((struct EventWork *)Ram_HeapSlots->eventWork)->unk16a = 100;
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
    Scene_QueueMusic(SE_170);
    break;
  case 120:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  if ((u32)(timer - 11) <= 118) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_21))
      TaopoSwamp_ShatterIcePillar(8);
    else if (Event_GetFlag(FLAG_SCENE_TEMP_20))
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
    Scene_QueueMusic(SE_170);
    break;
  case 120:
    Vent_SetActive(0, 0);
    Vent_SetActive(1, 0);
    Scene_QueueMusic(-1);
    break;
  }
  if ((u32)(timer - 11) <= 118) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_21))
      TaopoSwamp_ShatterDripIcePillar(10);
    else if (Event_GetFlag(FLAG_SCENE_TEMP_20))
      TaopoSwamp_BlowAwayDripPuddle(10);
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
    Scene_QueueMusic(SE_170);
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
  s32 scale;

  if (object->y + object->velocityY < object->terrainHeight) {
    flag = Event_GetFlag(FLAG_SCENE_TEMP_21);
    if (flag) {
      Event_PlaySound(SE_DRIP);
      return 0;
    }
    spark = TaopoSwamp_CreateActor(0x10d, object->x, object->y, object->z);
    scale = 0x8ccc;
    if (spark) {
      spark->scaleX = scale;
      spark->scaleY = scale;
      Actor_StartScript(spark, gTaopoSwamp_SparkFades);
      Actor_SetSpriteFlags(spark, 0);
      Actor_SelectAnimation(spark, 2);
    }
    Event_PlaySound(SE_DRIP);
    object->y = object->terrainHeight;
    object->velocityY = 0;
    if (Event_GetFlag(0x104) == 0) {
      count = FieldGameFlag_GetByte(0x230);
      if (count <= 3) {
        count++;
        GameFlag_SetByte(0x230, count);
      }
      if (count != 0)
        TaopoSwamp_FillDripSpot(0, 10);
      object = Actor_Get(10);
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
    s32 x = 0x1980000;
    s32 z = 0x1b80000;
    s32 y = 0xa00000;
    struct Actor *object = FieldActor_Create(0x12d, x, y, z);
    s32 scale = 0x547a;

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
  if (object->y + object->velocityY < -px(48))
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
      s32 y = 0xa00000;                                                        \
      struct Actor *object = FieldActor_Create(0x12d, x, y, z);                \
      s32 scale = 0x547a;                                                      \
                                                                               \
      if (object != NULL) {                                                    \
        object->scaleX = scale;                                                \
        object->scaleY = scale;                                                \
        object->motionFlags = 2;                                               \
        object->terrainHeight = 0xffc00000;                                    \
        object->gravity = 0x7333;                                              \
        Actor_StartScript(object, gTaopoSwamp_DropFalls);                      \
        Actor_SetSpriteFlags(object, 0);                                       \
      }                                                                        \
    }                                                                          \
  }

DEFINE_DRIP_AT_RANDOM(TaopoSwamp_DripInRoom7, 0x1800000, 0x1900000)

DEFINE_DRIP_AT_RANDOM(TaopoSwamp_DripInRoom8, 0x1a00000, 0x1600000)

extern u8 gTaopoSwamp_Room5Vents[];
extern u8 gTaopoSwamp_Room5VentPair[];
extern u8 gTaopoSwamp_Room7Vents[];
extern u8 gTaopoSwamp_Room8Vents[];
extern u8 gTaopoSwamp_RockRoomVents[];
void TaopoSwamp_ResetMud(void);
void Actor_SetWideSprite(s32 id, s32 wide);
void TaopoSwamp_StartVents(u16 *points, u16 *values, s32 arg);

#if !TLA_PATCH_1
s32 Scene_Initialize(void) {
  s32 scene = gMainRoutineWork.scene;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *object, *dummy, *piece;
  s32 id, entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 513;
  if (scene == (s32)&SceneId_TaopoSwamp2) {
    gMainRoutineWork.retreatScene = (s32)&SceneId_TaopoSwamp1;
    gMainRoutineWork.retreatEntrance = 1;
    TaopoSwamp_ResetMud();
    for (id = 8; id <= 12; id++) {
      if (Event_GetFlag(id + 0x8d8))
        Actor_SetPosition(id, 0, 0);
      else
        Actor_GetById(id)->scaleY = Q16_ONE * 5 / 4;
    }
    Actor_SetSpritePriority(13, 3);
    Actor_GetById(13)->collisionFlags = 0;
    if (Event_GetFlag(FLAG_SCENE_TEMP_20))
      Call6(Map_CopyCellAttributeRect, 50, 16, 1, 2, 47, 16);
    dummy = Actor_GetById(14);
    if (dummy != 0) {
      dummy->scaleX = Q16_ONE * 3 / 4;
      dummy->scaleY = Q16_ONE * 3 / 4;
      dummy->motionFlags = 0;
    }
    Task_Add(TaopoSwamp_DripAtRandom, 0x480);
    ((s8 *)map)[23] = (((s8 *)map)[23] & ~12) | 8;
    gDecodeFillByte = 93;
  } else if (scene == (s32)&SceneId_TaopoSwamp3) {
    if (!Event_GetFlag(FLAG_AREA_TEMP_00))
      Actor_SetWideSprite(64, 1);
    if (!Event_GetFlag(FLAG_AREA_TEMP_01))
      Actor_SetWideSprite(65, 1);
  } else if (scene == (s32)&SceneId_TaopoSwamp4) {
    FieldEffect_StartBlankOverlay(0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_10))
      Actor_SetPosition(8, 0, 0);
  } else if (scene == (s32)&SceneId_TaopoSwamp5) {
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(FLAG_AREA_TEMP_10))
      Actor_SetPosition(8, 0, 0);
    if (Event_GetFlag(0xa6d))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(280),
            PIXELS(232));
    if (Event_GetFlag(0xa6e))
      Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(472),
            PIXELS(248));
    TaopoSwamp_StartVents(gTaopoSwamp_Room5Vents, gTaopoSwamp_Room5VentPair,
                          TaopoSwamp_CycleDjinniVents);
  } else if (scene == (s32)&SceneId_TaopoSwamp7) {
    FieldEffect_StartBlankOverlay(0);
    Task_Add(TaopoSwamp_DripInRoom7, 0x480);
    if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5)
      TaopoSwamp_StartVents(gTaopoSwamp_Room7Vents, 0,
                            TaopoSwamp_CycleSpotVent);
    piece = Actor_GetById(8);
    piece->motionFlags = 0;
    Actor_SetSpriteFlags(piece, 0);
    if (Event_GetFlag(FLAG_SCENE_TEMP_21)) {
      Call6(Map_CopyCellAttributeRect, 28, 8, 1, 1, 26, 8);
      Actor_SelectAnimation(piece, 0);
    } else {
      Call6(Map_CopyCellAttributeRect, 22, 8, 1, 1, 26, 8);
      Actor_SetSpritePriority(8, 3);
      piece->priorityFlags |= 2;
      if (!Event_GetFlag(FLAG_SCENE_TEMP_20))
        Actor_SelectAnimation(piece, 7);
    }
    piece = Actor_GetById(9);
    piece->motionFlags = 0;
    if (Event_GetFlag(0xa6f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(264),
            PIXELS(424));
      piece->terrainHeight = 0;
      piece->y = 0;
      Call6(Map_CopyCellAttributeRect, 15, 26, 1, 1, 16, 26);
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp8) {
    if (gMainRoutineWork.entrance != 2 && gMainRoutineWork.entrance != 4 &&
        gMainRoutineWork.entrance != 9) {
      Rope_Initialize();
      Call4(Rope_Place, 0, 776, 8, 9);
    } else {
      Value3((s32 (*)())TaopoSwamp_StartVents, (s32)gTaopoSwamp_Room8Vents, 0,
             (s32)TaopoSwamp_CycleDripVent);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Call2(Task_Add, TaopoSwamp_DripInRoom8, 0x480);
      Call2(Task_Add, TaopoSwamp_DripFromCeiling, 0x480);
      object = Actor_GetById(10);
      object->motionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      if (Event_GetFlag(FLAG_SCENE_TEMP_21)) {
        Actor_SelectAnimation(object, 0);
      } else {
        Actor_SetSpritePriority(10, 3);
        object->priorityFlags |= 2;
        if (!Event_GetFlag(FLAG_SCENE_TEMP_20))
          Actor_SelectAnimation(object, 7);
      }
      Call6(Map_CopyCellsRect, 50, 36, 13, 13, 60, 5);
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp9) {
    TaopoSwamp_StartVents(gTaopoSwamp_RockRoomVents, 0,
                          TaopoSwamp_CycleRockRoomVents);
    if (!Event_GetFlag(FLAG_AREA_TEMP_38)) {
      Event_SetFlag(FLAG_AREA_TEMP_39);
    } else if (Event_GetFlag(FLAG_AREA_TEMP_3A)) {
      Actor_SetAnimation(8, 3);
      Call3(Actor_SetHorizontalPositionWithTerrain, 8, PIXELS(184),
            PIXELS(344));
      Actor_GetById(8)->collisionFlags = 0;
      Actor_GetById(8)->priorityFlags |= 2;
      Actor_SetSpritePriority(8, 3);
      Call6(Map_CopyCellAttributeRect, 11, 19, 1, 1, 11, 21);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_3B)) {
      Event_SetFlag(FLAG_AREA_TEMP_3C);
    } else if (Event_GetFlag(FLAG_AREA_TEMP_3D)) {
      Actor_SetAnimation(9, 3);
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(504),
            PIXELS(184));
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 30, 13, 1, 1, 31, 11);
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp10) {
    piece = Actor_GetById(19);
    if (Event_GetFlag(FLAG_AREA_TEMP_30)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 19, PIXELS(480),
            PIXELS(248));
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(440),
              PIXELS(232));
      Call6(Map_CopyCellsRect, 47, 50, 2, 14, 29, 1);
      Call6(Map_CopyCellsRect, 50, 59, 13, 5, 18, 15);
    } else {
      piece->motionFlags = 0;
      piece->y = px(32);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_31)) {
      Event_SetFlag(FLAG_AREA_TEMP_32);
    } else if (Event_GetFlag(FLAG_AREA_TEMP_33)) {
      Actor_SetAnimation(9, 3);
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(120),
            PIXELS(248));
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 7, 17, 1, 1, 7, 15);
    } else {
      Actor_SetAnimation(9, 0);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_34)) {
      Event_SetFlag(FLAG_AREA_TEMP_35);
    } else {
      Call6(Map_CopyCellAttributeRect, 27, 14, 1, 1, 27, 13);
      if (Event_GetFlag(FLAG_AREA_TEMP_36)) {
        Actor_SetAnimation(10, 3);
        Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(408),
              PIXELS(264));
        Actor_GetById(10)->collisionFlags = 0;
        Actor_GetById(10)->priorityFlags |= 2;
        Call6(Map_CopyCellAttributeRect, 25, 18, 1, 1, 25, 16);
      } else {
        Actor_SetAnimation(10, 0);
      }
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp11) {
    if (!Event_GetFlag(FLAG_AREA_TEMP_40)) {
      Event_SetFlag(FLAG_AREA_TEMP_41);
    } else {
      Actor_SetAnimation(8, 0);
      Call6(Map_CopyCellAttributeRect, 7, 39, 1, 1, 7, 38);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_42))
      Event_SetFlag(FLAG_AREA_TEMP_43);
    else
      Actor_SetAnimation(9, 0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_48))
      Actor_SetWideSprite(64, 0);
    entrance = gMainRoutineWork.entrance;
    if ((u32)(entrance - 2) <= 1) {
      if (Event_GetFlag(FLAG_AREA_TEMP_40))
        TaopoSwamp_DarkenRoom(0);
      else
        TaopoSwamp_FocusMapMiddle(0);
    }
    if (entrance == 6) {
      if (Event_GetFlag(FLAG_AREA_TEMP_42))
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
  s32 scene = gMainRoutineWork.scene;
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *effect = (s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac);
  struct Actor *object, *dummy, *piece;
  s32 id, entrance;

  *effect = 0;
  if (scene == (s32)&SceneId_TaopoSwamp2) {
    gMainRoutineWork.retreatScene = (s32)&SceneId_TaopoSwamp1;
    gMainRoutineWork.retreatEntrance = 1;
    TaopoSwamp_ResetMud();
    for (id = 8; id <= 12; id++) {
      if (Event_GetFlag(id + 0x8d8))
        Actor_SetPosition(id, 0, 0);
      else
        Actor_GetById(id)->scaleY = Q16_ONE * 5 / 4;
    }
    Actor_SetSpritePriority(13, 3);
    Actor_GetById(13)->collisionFlags = 0;
    if (Event_GetFlag(FLAG_SCENE_TEMP_20))
      Call6(Map_CopyCellAttributeRect, 50, 16, 1, 2, 47, 16);
    dummy = Actor_GetById(14);
    if (dummy != 0) {
      dummy->scaleX = Q16_ONE * 3 / 4;
      dummy->scaleY = Q16_ONE * 3 / 4;
      dummy->motionFlags = 0;
    }
    Task_Add(TaopoSwamp_DripAtRandom, 0x480);
    ((s8 *)map)[23] = (((s8 *)map)[23] & ~12) | 8;
    gDecodeFillByte = 93;
  } else if (scene == (s32)&SceneId_TaopoSwamp3) {
    *effect = 516;
    if (!Event_GetFlag(FLAG_AREA_TEMP_00))
      Actor_SetWideSprite(64, 1);
    if (!Event_GetFlag(FLAG_AREA_TEMP_01))
      Actor_SetWideSprite(65, 1);
  } else if (scene == (s32)&SceneId_TaopoSwamp4) {
    *effect = 516;
    FieldEffect_StartBlankOverlay(0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_10))
      Actor_SetPosition(8, 0, 0);
  } else if (scene == (s32)&SceneId_TaopoSwamp5) {
    *effect = 516;
    FieldEffect_StartBlankOverlay(0);
    if (Event_GetFlag(FLAG_AREA_TEMP_10))
      Actor_SetPosition(8, 0, 0);
    if (Event_GetFlag(0xa6d))
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(280),
            PIXELS(232));
    if (Event_GetFlag(0xa6e))
      Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(472),
            PIXELS(248));
    TaopoSwamp_StartVents(gTaopoSwamp_Room5Vents, gTaopoSwamp_Room5VentPair,
                          TaopoSwamp_CycleDjinniVents);
  } else if (scene == (s32)&SceneId_TaopoSwamp6) {
    *effect = 516;
  } else if (scene == (s32)&SceneId_TaopoSwamp7) {
    *effect = 516;
    FieldEffect_StartBlankOverlay(0);
    Task_Add(TaopoSwamp_DripInRoom7, 0x480);
    if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 5)
      TaopoSwamp_StartVents(gTaopoSwamp_Room7Vents, 0,
                            TaopoSwamp_CycleSpotVent);
    piece = Actor_GetById(8);
    piece->motionFlags = 0;
    Actor_SetSpriteFlags(piece, 0);
    if (Event_GetFlag(FLAG_SCENE_TEMP_21)) {
      Call6(Map_CopyCellAttributeRect, 28, 8, 1, 1, 26, 8);
      Actor_SelectAnimation(piece, 0);
    } else {
      Call6(Map_CopyCellAttributeRect, 22, 8, 1, 1, 26, 8);
      Actor_SetSpritePriority(8, 3);
      piece->priorityFlags |= 2;
      if (!Event_GetFlag(FLAG_SCENE_TEMP_20))
        Actor_SelectAnimation(piece, 7);
    }
    piece = Actor_GetById(9);
    piece->motionFlags = 0;
    if (Event_GetFlag(0xa6f)) {
      Call3(Actor_SetHorizontalPositionWithTerrain, 9, PIXELS(264),
            PIXELS(424));
      piece->terrainHeight = 0;
      piece->y = 0;
      Call6(Map_CopyCellAttributeRect, 15, 26, 1, 1, 16, 26);
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp8) {
    *effect = 516;
    if (gMainRoutineWork.entrance != 2 && gMainRoutineWork.entrance != 4 &&
        gMainRoutineWork.entrance != 9) {
      Rope_Initialize();
      Call4(Rope_Place, 0, 776, 8, 9);
    } else {
      Value3((s32 (*)())TaopoSwamp_StartVents, (s32)gTaopoSwamp_Room8Vents, 0,
             (s32)TaopoSwamp_CycleDripVent);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
      Call2(Task_Add, TaopoSwamp_DripInRoom8, 0x480);
      Call2(Task_Add, TaopoSwamp_DripFromCeiling, 0x480);
      object = Actor_GetById(10);
      object->motionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      if (Event_GetFlag(FLAG_SCENE_TEMP_21)) {
        Actor_SelectAnimation(object, 0);
      } else {
        Actor_SetSpritePriority(10, 3);
        object->priorityFlags |= 2;
        if (!Event_GetFlag(FLAG_SCENE_TEMP_20))
          Actor_SelectAnimation(object, 7);
      }
      Call6(Map_CopyCellsRect, 50, 36, 13, 13, 60, 5);
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp9) {
    *effect = 516;
    TaopoSwamp_StartVents(gTaopoSwamp_RockRoomVents, 0,
                          TaopoSwamp_CycleRockRoomVents);
    if (!Event_GetFlag(FLAG_AREA_TEMP_38)) {
      Event_SetFlag(FLAG_AREA_TEMP_39);
    } else if (Event_GetFlag(FLAG_AREA_TEMP_3A)) {
      Actor_SetAnimation(8, 3);
      Actor_SetPosition(8, 184, 344);
      Actor_GetById(8)->collisionFlags = 0;
      Actor_GetById(8)->priorityFlags |= 2;
      Actor_SetSpritePriority(8, 3);
      Call6(Map_CopyCellAttributeRect, 11, 19, 1, 1, 11, 21);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_3B)) {
      Event_SetFlag(FLAG_AREA_TEMP_3C);
    } else if (Event_GetFlag(FLAG_AREA_TEMP_3D)) {
      Actor_SetAnimation(9, 3);
      Actor_SetPosition(9, 504, 184);
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 30, 13, 1, 1, 31, 11);
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp10) {
    *effect = 516;
    piece = Actor_GetById(19);
    if (Event_GetFlag(FLAG_AREA_TEMP_30)) {
      Actor_SetPosition(19, 480, 248);
      if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
        Call3(Actor_SetHorizontalPositionWithTerrain, 10, PIXELS(440),
              PIXELS(232));
      Call6(Map_CopyCellsRect, 47, 50, 2, 14, 29, 1);
      Call6(Map_CopyCellsRect, 50, 59, 13, 5, 18, 15);
    } else {
      piece->motionFlags = 0;
      piece->y = px(32);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_31)) {
      Event_SetFlag(FLAG_AREA_TEMP_32);
    } else if (Event_GetFlag(FLAG_AREA_TEMP_33)) {
      Actor_SetAnimation(9, 3);
      Actor_SetPosition(9, 120, 248);
      Actor_GetById(9)->collisionFlags = 0;
      Actor_GetById(9)->priorityFlags |= 2;
      Call6(Map_CopyCellAttributeRect, 7, 17, 1, 1, 7, 15);
    } else {
      Actor_SetAnimation(9, 0);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_34)) {
      Event_SetFlag(FLAG_AREA_TEMP_35);
    } else {
      Call6(Map_CopyCellAttributeRect, 27, 14, 1, 1, 27, 13);
      if (Event_GetFlag(FLAG_AREA_TEMP_36)) {
        Actor_SetAnimation(10, 3);
        Actor_SetPosition(10, 408, 264);
        Actor_GetById(10)->collisionFlags = 0;
        Actor_GetById(10)->priorityFlags |= 2;
        Call6(Map_CopyCellAttributeRect, 25, 18, 1, 1, 25, 16);
      } else {
        Actor_SetAnimation(10, 0);
      }
    }
  } else if (scene == (s32)&SceneId_TaopoSwamp11) {
    *effect = 516;
    if (!Event_GetFlag(FLAG_AREA_TEMP_40)) {
      Event_SetFlag(FLAG_AREA_TEMP_41);
    } else {
      Actor_SetAnimation(8, 0);
      Call6(Map_CopyCellAttributeRect, 7, 39, 1, 1, 7, 38);
    }
    if (!Event_GetFlag(FLAG_AREA_TEMP_42))
      Event_SetFlag(FLAG_AREA_TEMP_43);
    else
      Actor_SetAnimation(9, 0);
    if (!Event_GetFlag(FLAG_AREA_TEMP_48))
      Actor_SetWideSprite(64, 0);
    entrance = gMainRoutineWork.entrance;
    if ((u32)(entrance - 2) <= 1) {
      if (Event_GetFlag(FLAG_AREA_TEMP_40))
        TaopoSwamp_DarkenRoom(0);
      else
        TaopoSwamp_FocusMapMiddle(0);
    }
    if (entrance == 6) {
      if (Event_GetFlag(FLAG_AREA_TEMP_42))
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

/// The Star Dust's sparkle in the dark, until it is dug up (flag 0x348).
void TaopoSwamp_TwinkleStarDust(void) {
  s32 frame = -1;

  if (Event_GetFlag(FLAG_AREA_TEMP_48)) {
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

extern u8 gMapCellBuffer[];

/// Points the map's followed point at cell (x, z), and the screen follows.
void TaopoSwamp_FocusMapOn(s32 x, s32 z) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct MapFocus *focus;
  s32 index, fx = x << 19;

  focus = (struct MapFocus *)(map + 0x178);
  focus->x = fx;
  focus->z = z << 19;
  focus->unk28 = 127;
  focus->unk2a = 0x3f80;
  focus->row = z << 7;
  focus->cellX = x;
  focus->unk18[0] = 0;
  focus->unk18[1] = 0;
  focus->unk18[2] = 0;
  focus->unk18[3] = 0;
  index = (z / 2 << 7) + x / 2;
  focus->scaleZ = focus->scaleX = Q16_ONE;
  focus->cell = gMapCellBuffer + index * 4;
  focus->shape = gMapShapeGrid + index;
  focus->scrolledX = Iwram_MulQ16(*(s32 *)(map + 0xe4), 0x10000) + focus->x;
  focus->scrolledZ =
      Iwram_MulQ16(*(s32 *)(map + 0xe8), focus->scaleZ) + focus->z;
  Map_CenterOnFollowedPoint();
  System_Wait(1);
}

static __inline__ void Blend_SetAlpha(s32 value) {
  REG_BLDALPHA->value = value;
}

/// The lights go out (fading over 50 frames when asked) and the sparkle starts
/// in the last chamber.
void TaopoSwamp_DarkenRoom(s32 fade) {
  BlendScript_Suspend();
  if (fade) {
    Blend_SetAlpha(0xe06);
    /* FAKEMATCH(dead): the empty once-only block finishes the blend value
     * before the shared register address. */
    do {
    } while (0);
    System_Wait(10);
    Blend_SetAlpha(0xd04);
    System_Wait(10);
    Blend_SetAlpha(0xd02);
    System_Wait(10);
    Blend_SetAlpha(0xc01);
    System_Wait(10);
    Blend_SetAlpha(0xc00);
    System_Wait(10);
  }
  TaopoSwamp_FocusMapOn(100, 50);
  Blend_SetAlpha(0xc07);
  if (gMainRoutineWork.entrance == 6)
    FieldTask_AddFrame(TaopoSwamp_TwinkleStarDust, 0x480);
}

/// Points the map's followed point at cell (100, 100).
void TaopoSwamp_FocusMapMiddle(void) { TaopoSwamp_FocusMapOn(100, 100); }

void TaopoSwamp_ResetMud(void) {
  struct SceneState *state = &gSceneState;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    Iwram_ClearWordsEntry(state, 12);
}

// volatile: the VBlank handler sets the held keys, as gKeysHeld in The Broken
// Seal.
extern volatile u32 gInput;

/// Each step on the mud sinks the leader deeper; sunk to the top, they are
/// pulled out to the last firm cell.
void TaopoSwamp_SinkInMud(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *actor = Actor_Get(FieldParty_GetLeaderActor());
  struct SpriteWork *sprite = actor->sprite;
  struct Actor *spark;
  struct FieldSprite *sparkSprite;
  s32 charge, step, i;

  if (sprite->child != 0)
    sprite->child[6] = 1;
  sprite->mode |= 8;
  sprite->mode &= 0xfe;
  /* FAKEMATCH(dead): the once-only block orders this statement. */
  do {
    sprite->dirty = 1;
  } while (0);
  if (gInput & 0xf0) {
    step = 0x1400;
    if (gInput & 2)
      step = step * 3 >> 1;
    state->timer += step / 96 + 1;
  }
  sprite->level = state->timer / 256;
  state->mode = 1;
  if (state->timer < 0x1400)
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  actor->animationKind = 0;
  for (i = 1; i >= 0; i--) {
    spark = (FieldActor_Create(269, actor->x, actor->y, actor->z));
    if (spark != 0) {
      sparkSprite = spark->sprite;
      Actor_StartScript(spark, gTaopoSwamp_SplashFades);
      spark->motionFlags = 0;
      spark->terrainId = 1;
      if (sparkSprite != 0) {
        Animation_ApplyChildArgument(sparkSprite, 2);
        sparkSprite->nextFrame = 0;
        sparkSprite->priority = 2;
      }
    }
    System_Wait(15);
  }
  FieldEvent_Wait(30);
  actor->animationKind = 1;
  sprite->level = 0;
  sprite->mode = 0;
  Event_End();
  actor->x = state->x;
  actor->z = state->z;
  state->mode = 0;
  state->timer = 0;
}

/// A bubble in the mud lifts the leader back to full height.
void TaopoSwamp_RiseOnBubble(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *object;
  struct SpriteWork *sprite;

  if (state->mode != 2) {
    object = Actor_Get(FieldParty_GetLeaderActor());
    sprite = object->sprite;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_SetMoveTargetFixed(object, (object->x & 0xfff00000) + 0x80000,
                                  object->y,
                                  (object->z & 0xfff00000) + 0x80000);
    FieldActor_WaitForMove(object);
    while (sprite->level != 0) {
      sprite->level--;
      sprite->dirty = 1;
      System_Wait(1);
    }
    Event_End();
    sprite->mode |= 8;
    sprite->mode &= 0xfe;
    state->mode = 2;
    state->timer = 0;
  }
}

/// Off the mud: the leader stands at full height again.
void TaopoSwamp_StepOnFirmGround(void) {
  struct SceneState *state = &gSceneState;
  struct SpriteWork *sprite;

  TaopoSwamp_SaveFirmGround();
  if (state->mode != 0) {
    sprite = Actor_Get(FieldParty_GetLeaderActor())->sprite;
    if (sprite->child)
      sprite->child[6] = 9;
    sprite->mode = (sprite->mode & 0xf7) | 1;
    sprite->level = 0;
    sprite->dirty = 1;
    state->mode = 0;
    state->timer = 0;
  }
}

/// Keeps the cell the leader stands on as the one the mud returns them to.
void TaopoSwamp_SaveFirmGround(void) {
  struct SceneState *state = &gSceneState;
  struct Actor *object = Actor_Get(FieldParty_GetLeaderActor());

  state->x = (object->x & 0xfff00000) + 0x80000;
  state->z = (object->z & 0xfff00000) + 0x80000;
}

#define VENT_TOUCHING Vent_IsTouching
#define VENT_BOUNCE Vent_Bounce
#include "../field/object/vent_bounce.c"

#include "shared/actor_start_scripted.h"

void Vent_Puff(s32 *position, s32 base) {
  struct Actor *object = TaopoSwamp_CreateActor(0x11d, position[0], position[1],
                                                position[2] - 0x20000);
  s32 angle;

  if (object != 0) {
    object->bounce = Trig_Cos(((u32)(Random16() * 0x3000) >> 16) + 0x3000);
    object->gravity = (u16)Random16();
    object->unk4c = (u16)(Random16() << 1) + base;
    /* FAKEMATCH(temporary): an s32 local keeps the zero extension and the pool
     * word 0xffff8000. */
    angle = (u16)Random16() - 0x8000;
    object->timer = angle;
    Actor_StartScripted(object, 3);
    object->update = (void (*)(void))Vent_Bounce;
    Actor_SetBlendMode(object, 1);
  }
}

#include "shared/vents_update.h"

// The registers by io_reg.h's names from here on: local.h names the blend
// register its own way for TaopoSwamp_DarkenRoom above.
#undef REG_BLDALPHA
#include "system/io_reg.h"

/// Places a room's vents from its tables and starts their cycle.
void TaopoSwamp_StartVents(u16 *points, u16 *values, s32 arg) {
  struct SwingWork *work = (struct SwingWork *)(gInventorySnapshot + 0x188);
  struct SwingPoint *point = work->points;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
    s32 x;
    s32 i;

    Iwram_ClearWordsEntry(work, 0x150);
    x = *points++;
    i = 0;
    while (x > 0) {
      s32 z = points[0];
      s32 w = points[1];

      point->unk00 = 0;
      point->x = x << 16;
      point->z = z << 16;
      point->y = Map_GetTerrainHeight(0, x << 16, (z << 16) + 0x100000);
      point->w = w << 16;
      point->index = i++;
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
      loop2:
        work->values[i++] = x;
        if (i > 3)
          goto done2;
        x = *values++;
        if (x > 0)
          goto loop2;
      }
    }
  done2:;
  }
  work->arg = arg;
  if (REG_BLDCNT == 0) {
    {
      s32 alpha = 0xc08;
      REG_BLDALPHA = alpha;
    }
    {
      s32 cnt = 0x3f10;
      REG_BLDCNT = cnt;
    }
  }
  Task_AddFrame(Vents_Update, 0x480);
}

/// The accessors step the work pointer by whole entries and then take entry 0:
/// the bytes add the base to index * 20 before the field offset.
#define VENT_AT(work, index)                                                   \
  ((struct VentWork *)((u8 *)(work) + (index) * sizeof(struct VentEntry)))

s32 Vent_IsActive(s32 index) {
  struct VentWork *work;

  work = VENT_WORK;
  return VENT_AT(work, index)->entries[0].active;
}

void Vent_SetActive(s32 index, s32 active) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].active = active;
}

/// The vents' frame counter, which their cycles time their gusts by.
s32 Vents_GetCounter(void) { return VENT_WORK->counter; }

#include "shared/vents_advance_counter.h"

/// The accessors step the work pointer by whole entries and then take entry 0:
/// the bytes add the base to index * 20 before the field offset.

void Vent_SetGust(s32 index, s32 value) {
  struct VentWork *work;

  work = VENT_WORK;
  VENT_AT(work, index)->entries[0].value = value;
}

#include "shared/vents_set_id.h"

#undef REG_BLDALPHA
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
#undef VENT_WORK
#undef DIFF
#undef DEFINE_TAOPO_SWAMP_FREEZE_SPOT
#undef DEFINE_DRIP_AT_RANDOM
#undef VENT_TOUCHING
#undef VENT_BOUNCE
#undef VENT_AT
#undef gTaopoSwamp_DripFromCeilingScript

void TaopoSwamp_FightFlower(void);
void TaopoSwamp_DigHole(void);
void TaopoSwamp_BlowFlowerOffLedge(void);
void TaopoSwamp_ShowSparkleMessage(void);

u8 gTaopoSwamp_Room5HoleVents[] = {
    0, 2, 9, 6, 4, 6, 5, 1,
};

u8 gTaopoSwamp_Room7HoleVents[] = {
    3, 0, 1, 0, 1, 0, 0, 0,
};

// clang-format off
u32 gTaopoSwamp_PuffSteamScript[] = {
    22, 26, 0, 39, 2, 0x80010000,
    23, 6, 0x10000, 0, 1, 13,
    30, 0xc0010000, 38,
};

u32 gTaopoSwamp_TremorPillarScript[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gTaopoSwamp_SparkFades[] = {
    0, 30, 38,
};

s32 gTaopoSwamp_DripAtRandomScript[] = {
    46,
    (s32)TaopoSwamp_LandDrop,
    38,
};

s32 gTaopoSwamp_DripFromCeilingScript[] = {
    46,
    (s32)TaopoSwamp_LandDrip,
    38,
};

s32 gTaopoSwamp_DropFalls[] = {
    46,
    (s32)TaopoSwamp_IsDropFalling,
    38,
};

u8 gTaopoSwamp_Room5Vents[] = {200, 0, 200, 0, 4, 0, 8, 1, 200, 0, 4, 0, 232, 0, 216, 0, 4, 0, 24, 1, 216, 0, 4, 0, 56, 2, 200, 0, 4, 0, 120, 2, 200, 0, 4, 0, 24, 2, 216, 0, 4, 0, 88, 2, 216, 0, 4, 0, 152, 1, 232, 0, 4, 0, 216, 1, 232, 0, 4, 0, 0, 0};

u8 gTaopoSwamp_Room5VentPair[] = {9, 0, 10, 0, 0, 0};

u8 gTaopoSwamp_Room7Vents[] = {168, 1, 104, 0, 4, 0, 24, 2, 136, 0, 4, 0, 0, 0};

u8 gTaopoSwamp_Room8Vents[] = {152, 1, 152, 1, 4, 0, 216, 1, 104, 1, 4, 0, 0, 0};

u8 gTaopoSwamp_RockRoomVents[] = {216, 0, 184, 0, 4, 0, 56, 1, 136, 0, 4, 0, 0, 0, 0, 0};

u32 gTaopoSwamp_SplashFades[] = {
    0, 32, 38,
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
    SCENE_EXITS(TaopoSwamp1),
    SCENE_EXIT(1, WorldMap, 35),
    SCENE_EXIT(2, TaopoSwamp2, 1),
    SCENE_EXIT(3, TaopoSwamp2, 2),
    SCENE_EXITS(TaopoSwamp2),
    SCENE_EXIT(1, TaopoSwamp1, 2),
    SCENE_EXIT(2, TaopoSwamp1, 3),
    SCENE_EXIT(3, TaopoSwamp3, 3),
    SCENE_EXIT(4, TaopoSwamp6, 4),
    SCENE_EXIT(5, TaopoSwamp3, 5),
    SCENE_EXITS(TaopoSwamp3),
    SCENE_EXIT(3, TaopoSwamp2, 3),
    SCENE_EXIT(5, TaopoSwamp2, 5),
    SCENE_EXIT(6, TaopoSwamp5, 2),
    SCENE_EXIT(7, TaopoSwamp5, 3),
    SCENE_EXIT(8, TaopoSwamp3, 9),
    SCENE_EXIT(9, TaopoSwamp3, 8),
    SCENE_EXITS(TaopoSwamp4),
    SCENE_EXIT(1, TaopoSwamp5, 1),
    SCENE_EXIT(2, TaopoSwamp5, 4),
    SCENE_EXITS(TaopoSwamp5),
    SCENE_EXIT(1, TaopoSwamp4, 1),
    SCENE_EXIT(2, TaopoSwamp3, 6),
    SCENE_EXIT(3, TaopoSwamp3, 7),
    SCENE_EXIT(4, TaopoSwamp4, 2),
    SCENE_EXIT(10, TaopoSwamp4, 10),
    SCENE_EXIT(11, TaopoSwamp4, 11),
    SCENE_EXIT(12, TaopoSwamp4, 12),
    SCENE_EXIT(13, TaopoSwamp4, 13),
    SCENE_EXIT(14, TaopoSwamp4, 14),
    SCENE_EXIT(15, TaopoSwamp4, 15),
    SCENE_EXIT(16, TaopoSwamp4, 16),
    SCENE_EXITS(TaopoSwamp6),
    SCENE_EXIT(1, TaopoSwamp7, 1),
    SCENE_EXIT(4, TaopoSwamp2, 4),
    SCENE_EXITS(TaopoSwamp7),
    SCENE_EXIT(1, TaopoSwamp6, 1),
    SCENE_EXIT(2, TaopoSwamp8, 2),
    SCENE_EXIT(3, TaopoSwamp8, 3),
    SCENE_EXIT(4, TaopoSwamp7, 5),
    SCENE_EXIT(5, TaopoSwamp7, 4),
    SCENE_EXIT(10, TaopoSwamp8, 9),
    SCENE_EXIT(12, TaopoSwamp8, 9),
    SCENE_EXITS(TaopoSwamp8),
    SCENE_EXIT(1, TaopoSwamp10, 1),
    SCENE_EXIT(2, TaopoSwamp7, 2),
    SCENE_EXIT(3, TaopoSwamp7, 3),
    SCENE_EXIT(4, TaopoSwamp8, 5),
    SCENE_EXIT(5, TaopoSwamp8, 4),
    SCENE_EXITS(TaopoSwamp10),
    SCENE_EXIT(1, TaopoSwamp8, 1),
    SCENE_EXIT(2, TaopoSwamp11, 2),
    SCENE_EXIT(3, TaopoSwamp9, 3),
    SCENE_EXIT(4, TaopoSwamp9, 4),
    SCENE_EXITS(TaopoSwamp9),
    SCENE_EXIT(3, TaopoSwamp10, 3),
    SCENE_EXIT(4, TaopoSwamp10, 4),
    SCENE_EXITS(TaopoSwamp11),
    SCENE_EXIT(2, TaopoSwamp10, 2),
    SCENE_EXIT(3, TaopoSwamp11, 4),
    SCENE_EXIT(4, TaopoSwamp11, 3),
    SCENE_EXIT(5, TaopoSwamp11, 6),
    SCENE_EXIT(6, TaopoSwamp11, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gTaopoSwamp_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room2Placements[] = {
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 128, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 112, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 112, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 176, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 256, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(299, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 272, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(273, 0x8e5, MOVEMENT_TYPE_STAND, 216, 0, 264, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room4Placements[] = {
    ACTOR_PLACEMENT(243, 0x3b, MOVEMENT_TYPE_STAND, 440, 0, 632, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room5Placements[] = {
    ACTOR_PLACEMENT(243, 0x3b, MOVEMENT_TYPE_STAND, 408, 0, 312, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 232, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room7Placements[] = {
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 424, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room8Placements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(290, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 440, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room10Placements[] = {
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 296, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 360, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 376, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 320, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 416, 0, 336, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 352, FACING_NORTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(328, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room9Placements[] = {
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 360, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTaopoSwamp_Room11Placements[] = {
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 632, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(292, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gTaopoSwamp_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room2Triggers[] = {
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

struct Trigger gTaopoSwamp_Room3Triggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 51, 0x301, EVENT(TaopoSwamp_ScoopHiddenItem)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50, 0x300, EVENT(TaopoSwamp_ScoopOtherHiddenItem)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room4Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(TaopoSwamp_FightFlower)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(TaopoSwamp_DigHole)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(TaopoSwamp_DigHoleUnderfoot)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(TaopoSwamp_StartMadPlantBattle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room5Triggers[] = {
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0xa6d, EVENT(TaopoSwamp_PushPillarOverVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0xa6e, EVENT(TaopoSwamp_PushPillarOverVent)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TaopoSwamp_PushPillarOverVent)},
    {TRIGGER_RAISED, 100, 0x310, EVENT(TaopoSwamp_BlowFlowerOffLedge)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room6Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gTaopoSwamp_Room7Triggers[] = {
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0xa6f,
     EVENT(TaopoSwamp_DropPillarIntoGap)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(TaopoSwamp_DropPillarIntoGap)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS), 8, 0x220,
     EVENT(TaopoSwamp_DouseVentSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 8, IF_FLAG_SET(0x222),
     EVENT(TaopoSwamp_FreezeVentSpot)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTaopoSwamp_Room8Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TaopoSwamp_StepOnVent)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x308, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_ACTS), 10, 0x220,
     EVENT(TaopoSwamp_FillDripSpot)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 10, IF_FLAG_SET(0x222),
     EVENT(TaopoSwamp_FreezeDripSpot)},
    {TRIGGER_TABLE_END},
};

s32 gTaopoSwamp_SteamPuffs = 112;

s32 gTaopoSwamp_PassageRows = 120;

s32 gTaopoSwamp_PoolFrames = 24;

struct Trigger gTaopoSwamp_Room10Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x331,
     EVENT(TaopoSwamp_CoolLavaRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 10, 0x334,
     EVENT(TaopoSwamp_CoolPillarRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x332,
     EVENT(TaopoSwamp_PushLavaRockIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x335,
     EVENT(TaopoSwamp_PushPillarRockIn)},
    {TRIGGER_PUSHED, 0, 0x335, EVENT(TaopoSwamp_PushPillarRockIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 11, 0x330,
     EVENT(TaopoSwamp_TremorPillar)},
    {TRIGGER_TABLE_END},
};

s32 gTaopoSwamp_Room9Triggers[] = {
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    2,
    (s32)0xffff000a,
    (s32)TaopoSwamp_StepOnVent,
    2,
    (s32)0xffff000b,
    (s32)TaopoSwamp_StepOnVent,
#if TLA_PATCH_1 /* Absent from the Japanese edition. */
    0x602,
    (s32)0xffff0040,
    (s32)TaopoSwamp_JumpForward,
#endif
    0x2115,
    0x3380008,
    (s32)TaopoSwamp_CoolChestRock,
    0x2115,
    0x33b0009,
    (s32)TaopoSwamp_CoolExitRock,
    0x8c15,
    0x3390008,
    (s32)TaopoSwamp_PushChestRockIn,
    0x8c15,
    0x33c0009,
    (s32)TaopoSwamp_PushExitRockIn,
    -1,
    0,
    0,
};

struct Trigger gTaopoSwamp_Room11Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {EXAMINE_TRIGGER(EXAMINE_GROUND), 50, 0x348,
     EVENT(TaopoSwamp_ShowSparkleMessage)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 8, 0x340,
     EVENT(TaopoSwamp_CoolDarkRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), 9, 0x342,
     EVENT(TaopoSwamp_CoolLastRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x341, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x343, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50, 0x348,
     EVENT(TaopoSwamp_ScoopStarDust)},
    {TRIGGER_TABLE_END},
};

s32 gTaopoSwamp_SparkleFrame = 0;

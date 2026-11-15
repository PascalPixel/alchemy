/// @file
/// Four small places the ship reaches, sharing one overlay: the Gondowan
/// Settlement (rooms 1 to 3), whose bush Cyclone cuts to a stair; the Hesperia
/// Settlement (4 to 6), with its vine, crate and Tinder; Kalt Island (7 and 8),
/// with its apple, frozen river and Gel; and SW Atteka Islet (9 and 10), whose
/// pillar and crate go over the falls, and its boulder and Jupiter Djinni.
#include "sprite_ids.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "djinn_ids.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field/field_event.h"
#include "field/field.h"
#include "graphics/palette_snapshots.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/io_reg.h"
#include "version.h"
#include "system/io_write_queue.h"
#include "system/irq_lock.h"
#include "field/map_grid.h"
#include "system/io_write.h"
#include "system/field_effect.h"
#include "field/field_effect.h"
#include "scene/scene.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "system/input.h"
#include "text/msg_ids.h"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#undef EFFECT_SPAWN

/// The apple, taken.
#define FLAG_APPLE_TAKEN 0xf5d
/// Kalt Island's rope, tied by Lash, for this visit.
#define FLAG_KALT_ROPE_TIED FLAG_SCENE_TEMP_03

/// The bush on the Gondowan Settlement's stairs is cut.
#define FLAG_STAIRS_BUSH_CUT 0xa60

/// The Gondowan Settlement, by the rows of
/// gSettlements_GondowanSettlementPlacements.
enum GondowanSettlementActorId {
  ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK = ACTOR_FIRST_PLACED,
  ACTOR_GONDOWAN_BOY
};

/// The Gondowan Settlement's houses.
enum GondowanSettlementHousesActorId {
  /// "I hear the Kibombo has finally..."
  ACTOR_WOMAN_ON_THE_KIBOMBO = ACTOR_FIRST_PLACED
};

/// The Hesperia Settlement, by the rows of
/// gSettlements_HesperiaSettlementPlacements.
enum HesperiaSettlementActorId {
  ACTOR_HESPERIA_MAN = ACTOR_FIRST_PLACED,
  ACTOR_SPROUT, ///< grown into a vine (Settlements_GrowVine)
  ACTOR_WALL_CRATE,
  ACTOR_TINDER
};

/// The Hesperia Settlement's houses.
enum HesperiaSettlementHousesActorId {
  /// "There's a place called Shaman Village..."
  ACTOR_WOMAN_ON_SHAMAN_VILLAGE = ACTOR_FIRST_PLACED,
  /// "The people of Shaman Village..."
  ACTOR_BOY_ON_THE_SHAMANS
};

/// Kalt Island, by the rows of gSettlements_KaltIslandPlacements.
enum KaltIslandActorId {
  /// The two posts of the rope Lash ties.
  ACTOR_ROPE_POST_A = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_POST_B,
  ACTOR_GEL,
  ACTOR_APPLE ///< Settlements_CatchApple
};

/// Kalt Island's houses.
enum KaltIslandHousesActorId {
  /// "If you want to cross the glaciers of the Northern Sea..."
  ACTOR_OLD_MAN_ON_THE_GLACIERS = ACTOR_FIRST_PLACED,
  /// "A long time ago, we used to trade with a village named Prox..."
  ACTOR_OLD_WOMAN_ON_PROX_TRADE
};

/// SW Atteka Islet, by the rows of gSettlements_AttekaIsletPlacements.
enum AttekaIsletActorId {
  ACTOR_ISLANDER = ACTOR_FIRST_PLACED,
  ACTOR_PILLAR,      ///< moved over the falls (Settlements_MovePillar)
  ACTOR_FALLS_CRATE, ///< moved over the falls (Settlements_MoveCrate)
  /// The eleven stones of the river, alike, bobbing in the current.
  ACTOR_RIVER_STONE_A,
  ACTOR_RIVER_STONE_B,
  ACTOR_RIVER_STONE_C,
  ACTOR_RIVER_STONE_D,
  ACTOR_RIVER_STONE_E,
  ACTOR_RIVER_STONE_F,
  ACTOR_RIVER_STONE_G,
  ACTOR_RIVER_STONE_H,
  ACTOR_RIVER_STONE_I,
  ACTOR_RIVER_STONE_J,
  ACTOR_RIVER_STONE_K,
  ACTOR_BOULDER, ///< raised by Lift
  /// The Djinni the game's text calls Wind19 (FLAG_DJINNI(DJINNI_WIND19)),
  /// met in Settlements_MeetLostJupiterDjinni.
  ACTOR_JUPITER_DJINNI
};

/// SW Atteka Islet's houses.
enum AttekaIsletHousesActorId {
  /// "There's nothing to the west..."
  ACTOR_ISLANDER_ON_THE_WEST = ACTOR_FIRST_PLACED
};

s32 Trig_Cos(s32 angle);

extern u8 MsgSettlementsDontTellMeYoureHere[];

extern s16 gSettlements_FallsObjects[];

void Settlements_SpawnSmoke(s32 x, s32 y, s32 z);
void Settlements_CycleWaterColors(void);
void Scroll_StartHBlank(void);
void Settlements_SinkPillar(void);
void Settlements_SlideCrateDownFalls(void);
void Settlements_RestoreWallCells(void);
void Settlements_DropCrate(void);
void Leader_SlideOnIce(void);
void Leader_StopMoving(void);
void Event_ResolveAllPendingActions(void);
void Scene_QueueMusic(s32 a);
#ifdef MODERN
void Rope_Lash(s32 a, s32 b, s32 c);
#endif
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);
void Settlements_ClimbDownLadder(void);
void Settlements_DriftSmoke(struct Actor *object);
void Actor_Release(struct Actor *object);
extern s32 gSettlements_WaterfallLine;
extern u16 gSettlements_WaterfallScrollAbove;
extern u16 gSettlements_WaterfallScrollBelow;
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Irq_SetHandler(s32 a, s32 b, void (*callback)(void));
void Event_End(void);
void ItemDisplay_ShowItem(s32 a, s32 b);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);

extern u32 gSettlements_HesperiaSettlementHousesRegions[];
extern u32 gSettlements_GondowanSettlementCaveRegions[];

static __inline__ void Window(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}
#include "shared/tables.h"

extern s16 gSlideOnIce_AnglesByKey[];

void Settlements_StepOntoWall(void);

void Leader_StopIfPending(void);

void Settlements_MeetTinder(void);

void Settlements_CatchApple(void);
void Settlements_CutLeftBush(void);

void Settlements_CutStairsBush(void);

void Settlements_StepOntoLadderFromEast(void);

void Settlements_TurnAndClimbDownLadder(void);

void Settlements_StepOntoIce(void);

void Settlements_StepOffIce(void);
void Settlements_StepOnIceSlide(void);
void Settlements_LashRope(void);

void Settlements_UnblockFallsObjects(void);

void Settlements_BlockFallsObject(void);

void Settlements_MoveCrate(void);

void Settlements_StepOffWall(void);

void Settlements_UnblockCrate(void);

void Settlements_BlockCrate(void);

void Settlements_LiftBoulder(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern s32 gSettlements_BobScript[];
extern s32 gSettlements_CrateBobScript[];

/// The apple's sprite as Kalt Island sets it up: the high bytes of its OAM
/// attributes 0 and 2, its VRAM block and its state, laid out as struct
/// FieldSprite has them.
struct OrbitSprite {
  u8 beforeShape[5];
  u8 affine : 2;
  u8 blendMode : 2;
  u8 mosaic : 1;
  u8 fullColor : 1;
  u8 shape : 2;
  u8 beforePriority[3];
  u8 tileHigh : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 beforeVramBlock[6];
  u8 vramBlock;
  u8 beforeState[10];
  u8 state;
};

struct OrbitObject {
  u8 beforeX[8];
  s32 x;
  s32 y;
  s32 z;
  u8 beforePriorityFlags[15];
  u8 priorityFlags;
  u8 beforeOrbitAngle[12];
  s32 orbitAngle;
  u8 beforeOrbitCenter[4];
  s32 orbitCenterX;
  s32 orbitCenterY;
  s32 orbitCenterZ;
  u8 beforeSprite[12];
  struct OrbitSprite *sprite;
  u8 beforeMode;
  u8 mode;
  u8 state;
  u8 beforeActive[5];
  u8 active;
  u8 beforeVisible[4];
  u8 visible;
  u8 beforeCallback[10];
  u32 callback;
};

struct SettlementsVector {
  s32 x;
  s32 y;
  s32 z;
};

void Rope_Initialize(void);

void Settlements_StepOntoWall(void);

void FieldEffect_StartBlankOverlay(s32 a);

void Actor_PlaceBlocks(const void *data);

extern u16 gSettlements_WaterColors[];
extern u16 gSettlements_WaterColorsSaved[];

void Settlements_SpawnSplashRing(s32 objectId);

void Settlements_MovePillar(void);

void Settlements_MeetGel(void) {
  Djinn_JoinParty(10, DJINNI_GEL / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_GEL));
}

void Settlements_MeetTinder(void) {
  Djinn_JoinParty(11, DJINNI_TINDER / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_TINDER));
}

void Settlements_MeetLostJupiterDjinni(void) {
  Djinn_JoinParty(23, DJINNI_WIND19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_WIND19));
}

s32 Settlements_ClearSpriteFlags(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

#include "shared/actor_bob.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

u32 *Scene_GetExitRegions(void) {
  s32 scene = gMainRoutineWork.next;
  u32 *placements;

  if (scene == SCENE_HESPERIA_SETTLEMENT_HOUSES)
    return gSettlements_HesperiaSettlementHousesRegions;
  placements = 0;
  if (scene == SCENE_GONDOWAN_SETTLEMENT_CAVE)
    placements = gSettlements_GondowanSettlementCaveRegions;
  return placements;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gSettlements_Placements[];
extern struct ActorPlacement gSettlements_GondowanSettlementPlacements[];
extern struct ActorPlacement gSettlements_GondowanSettlementHousesPlacements[];
extern struct ActorPlacement gSettlements_HesperiaSettlementPlacements[];
extern struct ActorPlacement gSettlements_HesperiaSettlementHousesPlacements[];
extern struct ActorPlacement gSettlements_AttekaIsletPlacements[];
extern struct ActorPlacement gSettlements_AttekaIsletHousesPlacements[];
extern struct ActorPlacement gSettlements_KaltIslandPlacements[];
extern struct ActorPlacement gSettlements_KaltIslandHousesPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GONDOWAN_SETTLEMENT)
    return gSettlements_GondowanSettlementPlacements;
  if (scene == SCENE_GONDOWAN_SETTLEMENT_HOUSES)
    return gSettlements_GondowanSettlementHousesPlacements;
  if (scene == SCENE_HESPERIA_SETTLEMENT)
    return gSettlements_HesperiaSettlementPlacements;
  if (scene == SCENE_HESPERIA_SETTLEMENT_HOUSES)
    return gSettlements_HesperiaSettlementHousesPlacements;
  if (scene == SCENE_KALT_ISLAND)
    return gSettlements_KaltIslandPlacements;
  if (scene == SCENE_KALT_ISLAND_HOUSES)
    return gSettlements_KaltIslandHousesPlacements;
  if (scene == SCENE_SW_ATTEKA_ISLET)
    return gSettlements_AttekaIsletPlacements;
  if (scene == SCENE_SW_ATTEKA_ISLET_HOUSES)
    return gSettlements_AttekaIsletHousesPlacements;
  return gSettlements_Placements;
}

void Settlements_CatchApple(void) {
  s32 mode;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(ACTOR_APPLE, 0, 0);
  Event_SetFlag(FLAG_APPLE_TAKEN);
  ItemDisplay_ShowItem(ITEM_APPLE, 3);
  mode = 0;
  Party_GiveItem(ITEM_APPLE, mode);
  Event_End();
}

void Settlements_WarnOfRiddleOfRock(s32 actor) {
  s32 msg = (s32)MsgSettlementsDontTellMeYoureHere;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Settlements_CutLeftBush(void) { Window(5, 70, 9, 7, 5, 43, 1); }

void Settlements_CutStairsBush(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);

  if (x == 5 && z == 17) {
    FieldActor_Jump(gMainRoutineWork.leaderActorId, 2, 0);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 88, 296);
  }
  TileCopies_CopyTouchedCells(5, 70, 9, 7, 5, 43, 1);
  MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
  MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
  Event_SetFlag(FLAG_STAIRS_BUSH_CUT);
}

void Settlements_ClimbDownLadder(void) {
  s32 next = gFieldWork->stepTrigger;

  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 4);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FELIX), 0);
  Actor_SetAnimation(ACTOR_FELIX, 13);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 16);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_FELIX, 10);
  FieldEvent_Wait(10);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(next);
  Event_End();
}

void Settlements_StepOntoLadderFromWest(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Settlements_ClimbDownLadder();
}

void Settlements_StepOntoLadderFromEast(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, -2, -6);
  Actor_WalkByAndWait(ACTOR_FELIX, -14, -8);
  Settlements_ClimbDownLadder();
}

void Settlements_TurnAndClimbDownLadder(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Settlements_ClimbDownLadder();
}

void Settlements_GrowVine(void) {
  MapGrid_CopyCellAttributes(0, 3, 1, 1, 3, 15);
  Event_SetFlag(FLAG_SCENE_TEMP_30);
}

extern struct Trigger gSettlements_Triggers[];
extern struct Trigger gSettlements_GondowanSettlementTriggers[];
extern struct Trigger gSettlements_GondowanSettlementHousesTriggers[];
extern struct Trigger gSettlements_HesperiaSettlementTriggers[];
extern struct Trigger gSettlements_HesperiaSettlementHousesTriggers[];
extern struct Trigger gSettlements_KaltIslandTriggers[];
extern struct Trigger gSettlements_KaltIslandHousesTriggers[];
extern struct Trigger gSettlements_AttekaIsletTriggers[];
extern struct Trigger gSettlements_AttekaIsletHousesTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_GONDOWAN_SETTLEMENT)
    return gSettlements_GondowanSettlementTriggers;
  if (scene == SCENE_GONDOWAN_SETTLEMENT_HOUSES)
    return gSettlements_GondowanSettlementHousesTriggers;
  if (scene == SCENE_HESPERIA_SETTLEMENT)
    return gSettlements_HesperiaSettlementTriggers;
  if (scene == SCENE_HESPERIA_SETTLEMENT_HOUSES)
    return gSettlements_HesperiaSettlementHousesTriggers;
  if (scene == SCENE_KALT_ISLAND)
    return gSettlements_KaltIslandTriggers;
  if (scene == SCENE_KALT_ISLAND_HOUSES)
    return gSettlements_KaltIslandHousesTriggers;
  if (scene == SCENE_SW_ATTEKA_ISLET)
    return gSettlements_AttekaIsletTriggers;
  if (scene == SCENE_SW_ATTEKA_ISLET_HOUSES)
    return gSettlements_AttekaIsletHousesTriggers;
  return gSettlements_Triggers;
}

void Settlements_DriftSmoke(struct Actor *object) {
  s32 step;

  object->x.fixed += object->timer << 8;

  object->y.fixed += px(1) / 2;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->timer += 2;
  if (--*(s32 *)&object->watchedActor == 0)
    Actor_Release(object);
}

void Settlements_SpawnSmoke(s32 x, s32 y, s32 z) {
  struct Actor *object = FieldActor_Create(SPRITE_DUST_CLOUD, x, y, z);

  if (object != NULL) {
    s8 *animation;

    object->retainSpriteCache = 2;
    object->motionFlags = 0;
    object->timer = 0;
    object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    FieldActor_SetSpriteColor(object, 7);
    animation = object->sprite;
    animation[9] = (animation[9] & -13) | 4;
    Actor_SetSpriteFlags(object, 0);
    object->scaleX = Q16_ONE / 2;
    object->scaleY = Q16_ONE / 2;
    *(s32 *)&object->watchedActor = 60;
    object->update = (void (*)(void))Settlements_DriftSmoke;
    Actor_SelectAnimation(object, 5);
    Actor_SetBlendMode(object, 1);
  }
}

void Settlements_PuffChimneySmoke(void) {
  if ((gFrameCount & 63) == 0)
    Settlements_SpawnSmoke(0xf80000, 0x300000, 0x1000000);
}

/// Moves the object around its orbit's centre (words 14 and 15) and steps the
/// angle by a random amount.
// As ☀️'s Scene_UpdateFloatingNut does.
s32 Settlements_CircleCentre(s32 *p) {
  s16 *q = (s16 *)p[20];
  s32 a, b;
  s32 d = Math_Sin(p[12]) * 2;

  if (d > 0)
    d = -d;
  p[2] = p[14] + Trig_Cos(p[12]) * 2;
  p[3] = p[15] + d;
  q[9] = Trig_Cos(p[12] + 0x8000) / 8;
  a = Random16();
  b = Random16();
  {
    u32 x;
    s32 angle;

    x = (u32)a << 9;
    angle = p[12];
    p[12] = angle + ((x >> 16) + (((u32)b << 9) >> 16)) + 0x400;
  }
  return 0;
}

void Rope_Place(s32 index, s32 flag, s32 post, s32 otherPost);

/// Rope_Place, from an event.
static inline void FieldRope_Place(s32 index, s32 flag, s32 post,
                                   s32 otherPost) {
  Rope_Place(index, flag, post, otherPost);
}

void Settlements_SetUpKaltIsland(void) {
  struct OrbitObject *actor;
  struct OrbitSprite *sprite;
  u8 *transfer;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    FieldTask_AddFrame(Settlements_PuffChimneySmoke, priority);
  }
  Rope_Initialize();
  Rope_Initialize();
  FieldRope_Place(1, FLAG_KALT_ROPE_TIED, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  if (Event_GetFlag(FLAG_APPLE_TAKEN)) {
    Actor_SetPosition(ACTOR_APPLE, 0, 0);
    return;
  }
  actor = (struct OrbitObject *)Actor_Get(ACTOR_APPLE);
  sprite = actor->sprite;
  sprite->priority = 1;
  sprite->fullColor = 0;
  sprite->palette = 0;
  sprite->state = 0;
  Actor_SetSpriteFlags((struct Actor *)actor, 0);
  actor->active = 0;
  actor->mode = 0;
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    actor->y += px(16);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->visible = 1;
  transfer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
  Item_LoadIcon(ITEM_APPLE);
  transfer += 0x400;
  Vram_Load(sprite->vramBlock, 128, transfer);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  actor->orbitCenterX = actor->x;
  actor->orbitAngle = 0;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Settlements_CircleCentre;
  actor->state = 0;
}

extern s16 gSettlements_WallCrate[];

void Settlements_StartWaterfall(void);

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gMainRoutineWork.next == SCENE_KALT_ISLAND)
    Settlements_SetUpKaltIsland();
  if (gMainRoutineWork.next == SCENE_SW_ATTEKA_ISLET) {
    FieldEffect_StartBlankOverlay(0);
    Actor_PlaceBlocks(gSettlements_FallsObjects);
    if (Event_GetFlag(FLAG_SCENE_TEMP_06))
      Actor_SetPosition(ACTOR_FALLS_CRATE, 0, 0);
    Actor_GetById(ACTOR_FALLS_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_RIVER_STONE_H, 2);
    Actor_SetAnimation(ACTOR_RIVER_STONE_I, 2);
    Actor_SetAnimation(ACTOR_RIVER_STONE_K, 2);
    if (Event_GetFlag(FLAG_SCENE_TEMP_05))
      Actor_SetAnimation(ACTOR_FALLS_CRATE, 2);
    Settlements_StartWaterfall();
#if TLA_PATCH_1
    if (Event_GetFlag(FLAG_SCENE_TEMP_04)) {
      Actor_SetPosition(ACTOR_PILLAR, 0, 0);
      Actor_SetPosition(ACTOR_RIVER_STONE_K, 184, 184);
      Actor_GetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.next == SCENE_HESPERIA_SETTLEMENT) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_30)) {
      MapGrid_CopyCellAttributes(0, 3, 1, 1, 3, 15);
      Actor_SetAnimation(ACTOR_SPROUT, 4);
    }
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        Event_GetFlag(FLAG_SCENE_TEMP_20)) {
      Event_ClearFlag(FLAG_SCENE_TEMP_20);
      Settlements_StepOntoWall();
    }
    Actor_PlaceBlocks(gSettlements_WallCrate);
  }
  if (gMainRoutineWork.next == SCENE_GONDOWAN_SETTLEMENT_CAVE)
    FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_HESPERIA_SETTLEMENT_CAVE)
    FieldEffect_StartBlankOverlay(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.next == SCENE_SW_ATTEKA_ISLET) {
    if (Event_GetFlag(FLAG_SCENE_TEMP_05)) {
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
    }
#if !TLA_PATCH_1
    if (Event_GetFlag(FLAG_SCENE_TEMP_05))
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
#else
    if (Event_GetFlag(FLAG_SCENE_TEMP_04)) {
      Actor_SetPosition(ACTOR_PILLAR, 0, 0);
      Actor_SetPosition(ACTOR_RIVER_STONE_K, 184, 184);
      Actor_GetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.next == SCENE_GONDOWAN_SETTLEMENT) {
    if (Event_GetFlag(FLAG_STAIRS_BUSH_CUT)) {
      MapGrid_CopyCellsTo(5, 46, 5, 47, 1, 1);
      MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
      MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
    }
  }
  return 0;
}

void Settlements_StepOntoIce(void) { GameFlag_Set(FLAG_SCENE_TEMP_01); }

void Settlements_StepOffIce(void) { GameFlag_Clear(FLAG_SCENE_TEMP_01); }

void Settlements_StepOnIceSlide(void) { Leader_SlideOnIce(); }

void Settlements_EnterCave(void) {
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = ACTOR_FIRST_PLACED;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != 0)
      object->motionFlags = 0;
  }
  i++;
  if (i < FIELD_TREASURE_ACTOR)
    goto loop;
  if (gMainRoutineWork.next == SCENE_HESPERIA_SETTLEMENT)
    MapGrid_CopyCellsTo(33, 30, 23, 14, 1, 2);
  else
    MapGrid_CopyCellsTo(27, 29, 40, 16, 1, 2);
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_CenterAndWalk(gMainRoutineWork.leaderActorId, 2, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Settlements_LashRope(void) {
  Rope_Lash(1, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  Event_SetFlag(FLAG_KALT_ROPE_TIED);
}

extern s32 gSettlements_WaterColorFrame;

void Settlements_CycleWaterColors(void) {
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  s32 *frame;
  s32 phase, i;

  if (work->psynergy != 0 || work->revealActive != 0)
    return;
  frame = &gSettlements_WaterColorFrame;
  if (*frame == -1) {
    u16 *colors = gSettlements_WaterColors;
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy(colors, (void *)&BG_PLTT_COLOR(3, 0), 32);
    copy(gSettlements_WaterColorsSaved, colors, 32);
    *frame = 0;
  }
  phase = (*frame / 8) % 4;
  for (i = 0; i <= 3; i++)
    gSettlements_WaterColorsSaved[12 + (phase + i) % 4] =
        gSettlements_WaterColors[12 + i];
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)gSettlements_WaterColorsSaved;
      q->count = count + 1;
      *entry++ = 0x5000060;
      *entry = 0x84000008;
    }
    IRQ_UNLOCK(saved);
  }
  gSettlements_WaterColorFrame++;
}

void Settlements_StartWaterfall(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  FieldTask_AddFrame(Settlements_CycleWaterColors, TASK_PASS_VBLANK);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Scroll_StartHBlank();
}

// The hblank split: above the line Scroll_StartHBlank sets, BG1 scrolls by one
// value and blending is off; below it, by the other, with BG1 blended.

/// The hblank split: above the line Scroll_StartHBlank sets, BG1 scrolls by one
/// value and blending is off; below it, by the other, with BG1 blended.
void Scroll_HBlank(void) {
  if (REG_VCOUNT < gSettlements_WaterfallLine) {
    Io_Write16(gSettlements_WaterfallScrollAbove, &REG_BG1HOFS);
    REG_BLDCNT = 0;
  } else {
    Io_Write16(gSettlements_WaterfallScrollBelow, &REG_BG1HOFS);
    REG_BLDALPHA = BLEND_ALPHA(12, 16);
    REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  }
}

/// Follows the third map layer's scroll.
void Layer_TrackScroll(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[2];
  s16 y = layer->y >> 16;

  gSettlements_WaterfallLine = 1448 - y;
  gSettlements_WaterfallScrollAbove = layer->x >> 16;
  gSettlements_WaterfallScrollBelow =
      (s16)(layer->x >> 16) - (gFrameCount >> 2);
}

#include "shared/scroll_start_hblank.h"

void Settlements_UnblockFallsObjects(void) {
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

void Settlements_BlockFallsObject(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

void Settlements_SplashCrate(s32 id);

void Settlements_SinkPillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_PILLAR);
  s32 column = FIELD_CELL(object->x.fixed);
  s32 row = FIELD_CELL(object->z.fixed);
  s32 i;

  if (!Event_GetFlag(FLAG_SCENE_TEMP_04) && column == 11 && row == 12) {
    object->motionFlags = 0;
    for (i = 10; i > 0; i--) {
      object->y.fixed -= px(2);
      Event_Wait(1);
    }
    Settlements_SplashCrate(9);
    Actor_SetPosition(ACTOR_PILLAR, 0, 0);
    Actor_SetPosition(ACTOR_RIVER_STONE_K, 184, 184);
    Actor_GetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
    Actor_GetById(ACTOR_RIVER_STONE_K)->y.fixed -= px(64);
    MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
  }
}

void Settlements_SlideCrateDownFalls(void) {
  struct Actor *object = Actor_GetById(ACTOR_FALLS_CRATE);
  s32 column = FIELD_CELL(object->x.fixed);
  s32 row = FIELD_CELL(object->z.fixed);
  s32 i;
  u8 *flags;

  if (column == 11 && row == 9) {
    object->motionFlags = 0;
    i = 8;
  sink:
    FieldEvent_Wait(1);
    object->y.fixed -= px(2);
    i--;
    if (i > 0)
      goto sink;
    Settlements_SplashCrate(10);
    FieldEvent_Wait(20);
    FieldCamera_MoveTo(192, CAMERA_KEEP, 224, 1);
    Actor_SetSpeed(ACTOR_FALLS_CRATE, 0.8);
    Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 8);
    FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
    FieldEvent_Wait(10);
    Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Actor_SetSpeed(ACTOR_FALLS_CRATE, 2.4);
    Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
    FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
    Settlements_SplashCrate(10);
    if (!Event_GetFlag(FLAG_SCENE_TEMP_04)) {
      FieldEvent_Wait(10);
      Actor_SetSpeed(ACTOR_FALLS_CRATE, 0.8);
      Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
      FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
      Actor_SetSpeed(ACTOR_FALLS_CRATE, 2.5);
      Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
      FieldEvent_Wait(5);
      Actor_SetPosition(ACTOR_FALLS_CRATE, 0, 0);
      Event_SetFlag(FLAG_SCENE_TEMP_06);
    } else {
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
      Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 12);
      FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
      FieldEvent_Wait(20);
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
      Actor_SetAnimation(ACTOR_FALLS_CRATE, 2);
      FieldEvent_Wait(30);
      Event_SetFlag(FLAG_SCENE_TEMP_05);
    }
    Event_End();
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_07) && column == 16 && row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &object->motionFlags;
    *flags = 3;
    FieldEvent_Wait(30);
    *flags = 0;
    Event_SetFlag(FLAG_SCENE_TEMP_07);
    Event_End();
  }
}

void Settlements_MovePillar(void) {
  Actor_PlaceMovedBlock();
  Settlements_SinkPillar();
}

void Settlements_MoveCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_SlideCrateDownFalls();
}

void Settlements_PushIntoFalls(void) {
  Actor_PlaceMovedBlock();
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags += 254;
  Settlements_SinkPillar();
  Settlements_SlideCrateDownFalls();
}

void Settlements_SaveWallCells(void) {
  MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 0);
  MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 10);
}

void Settlements_RestoreWallCells(void) {
  MapGrid_CopyCellAttributes(64, 0, 10, 10, 11, 73);
  MapGrid_CopyCellAttributes(64, 10, 10, 10, 42, 73);
}

void Settlements_StepOntoWall(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_21) &&
      !Event_GetFlag(FLAG_SCENE_TEMP_20)) {
    MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 50);
    MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 60);
    MapGrid_CopyCellAttributes(42, 11, 10, 4, 11, 75);
    Event_SetFlag(FLAG_SCENE_TEMP_20);
  }
}

void Settlements_StepOffWall(void) {
  if (Event_GetFlag(FLAG_SCENE_TEMP_20)) {
    MapGrid_CopyCellAttributes(64, 50, 10, 10, 11, 73);
    MapGrid_CopyCellAttributes(64, 60, 10, 10, 42, 73);
    Event_ClearFlag(FLAG_SCENE_TEMP_20);
  }
}

void Settlements_UnblockCrate(void) {
  struct Actor *object = Actor_Get(ACTOR_WALL_CRATE);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);

  Settlements_SaveWallCells();
  Actor_LiftTargetBlock(gSettlements_WallCrate);
  if (x == 12 && z == 15) {
    MapGrid_CopyCellAttributes(42, 3, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else if (x == 13 && z == 15) {
    MapGrid_CopyCellAttributes(42, 0, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else if (x == 16 && z == 15) {
    MapGrid_CopyCellAttributes(42, 0, 7, 3, 42, 14);
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  } else {
    MapGrid_CopyCellAttributes(42, 9, 10, 10, 11, 73);
  }
}

extern s32 gSettlements_CrateDropScript[];

#define SINK(frames)                                                           \
  i = (frames);                                                                \
  do {                                                                         \
    FieldEvent_Wait(1);                                                        \
    object->y.fixed -= px(2);                                                  \
    i--;                                                                       \
  } while (i > 0)

void Settlements_DropCrate(void) {
  struct Actor *object;
  s32 column;
  s32 row;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  EventCamera_FollowActor(ACTOR_WALL_CRATE, 1);
  object = Actor_Get(ACTOR_WALL_CRATE);
  column = FIELD_CELL(object->x.fixed);
  row = FIELD_CELL(object->z.fixed);
  if (column == 12 && row == 15) {
    Event_SetFlag(FLAG_SCENE_TEMP_21);
    object->motionFlags = 0;
    SINK(32);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_05) && column == 16 && row == 15) {
    object->motionFlags = 0;
    Event_SetFlag(FLAG_SCENE_TEMP_05);
    Event_SetFlag(FLAG_SCENE_TEMP_21);
    SINK(48);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(30);
  } else if (column == 19 && row == 15) {
    MapGrid_CopyCellAttributes(18, 16, 1, 1, column, row);
    object->motionFlags = 0;
    Event_SetFlag(FLAG_SCENE_TEMP_21);
    SINK(32);
    Settlements_SpawnSplashRing(10);
    if (!Event_GetFlag(102)) {
      Actor_SetPosition(ACTOR_TINDER, 0, 0);
      Actor_SetScript(ACTOR_WALL_CRATE, gSettlements_CrateDropScript);
      FieldEvent_Wait(20);
    }
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_04) && column == 13 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
    object->motionFlags = 0;
    SINK(16);
    Settlements_SpawnSplashRing(10);
    FieldEvent_Wait(20);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
  } else if (column == 14 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
  }
  Event_End();
}

void Settlements_BlockCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_RestoreWallCells();
  Settlements_DropCrate();
}

extern s32 gSettlements_SmokeScript[];

void Settlements_SplashCrate(s32 id) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 68 bytes it reserves. */
  UNUSED struct SettlementsVector velocity;
  struct EffectOptions spawn;
  struct Actor *object = Actor_Get(id);

  spawn.type = SPRITE_WATER_SPRAY;
  spawn.startScaleX = Q16_ONE * 4 / 5;
  spawn.startScaleY = Q16_ONE * 4 / 5;
  spawn.targetScaleX = Q16_ONE;
  spawn.targetScaleY = Q16_ONE;
  spawn.script = gSettlements_SmokeScript;
  Event_PlaySound(SE_CAST);
  FieldEffect_Spawn(object->x.fixed, object->y.fixed, object->z.fixed, 0, 0, 0,
                    EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                        EFFECT_SCALE_TO_TARGET,
                    &spawn);
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef SINK
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void Settlements_SpawnSplashRing(s32 objectId) {
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_Get(objectId);
  params.update = FieldEffect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    vec.x = Trig_Cos(angle);
    vec.y = 0;
    z = Math_Sin(angle);
    x = vec.x;
    vec.z = z;
    x += x / 3;
    vec.x = x;
    FieldEffect_Spawn(object->x.fixed, object->y.fixed, object->z.fixed, x,
                      vec.y, z, EFFECT_USE_UPDATE | 1, &params);
  }
}

void Settlements_LiftBoulder(void) { GameFlag_Set(FLAG_SCENE_TEMP_08); }

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern s16 gSlideOnIce_AnglesByKey[];
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

#include "shared/leader_slide_on_ice.h"

extern volatile s16 gSlideOnIce_Stopped;

extern s16 gSlideOnIce_AnglesByKey[];
void Actor_SteerByPad(struct Actor *object);
void Leader_StopIfPending(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Actor_HasArrived(struct Actor *object);
s32 Field_CheckSpheresOverlap(s32 *a, s32 aRadius, s32 *b, s32 bRadius);

#include "shared/slide_on_ice_from_edge.c"

void Settlements_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

void Settlements_MeetGel(void);
void Settlements_MeetLostJupiterDjinni(void);
void Settlements_StepOntoLadderFromWest(void);
void Settlements_GrowVine(void);
void Settlements_EnterCave(void);
void Settlements_PushIntoFalls(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

s16 gSettlements_FallsObjects[] = {
    9, 10, -1,
};

s16 gSettlements_WallCrate[] = {
    10, -1,
};

s16 gSlideOnIce_SteerAngles[] = {                                                               
      -1,     0,      0x8000, -1,     0xc000, 0xe000, 0xa000, 0xc000,          
      0x4000, 0x2000, 0x6000, 0x4000, -1,     0,      0x8000, -1,              
      -1,     0,      0x8000, -1,     0xc000, -1,     -1,     0xc000,          
      0x4000, -1,     -1,     0x4000, -1,     0,      0x8000, -1,              
  };

s16 gSlideOnIce_AnglesByKey[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSettlements_BobScript[] = {
    SC_ANIMATION(3),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, -px(16)),
    SC_CALL((s32)Settlements_ClearSpriteFlags),
    SC_END,
};

s32 gSettlements_SmokeScript[] = {
    SC_ANIMATION(2),
    SC_END,
};

s32 gSettlements_CrateBobScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

s32 gSettlements_CrateDropScript[] = {
    SC_CALL((s32)Actor_Bob),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 216, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 320, 0, 830, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSettlements_HesperiaSettlementHousesRegions[] = {
    0x2e02a0, 0x2b00190, 0x1a0003e, 0xeffff, 0xffff, 0,
    0, 0,
};

u32 gSettlements_GondowanSettlementCaveRegions[] = {
    0xc00d4, 0xdc0084, 0x8c0014, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(GondowanSettlement),
    SCENE_EXIT_UNLESS(1, WorldMap, 48, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, GondowanSettlementHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, GondowanSettlementCave, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, GondowanSettlementCave, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanSettlementHouses),
    SCENE_EXIT_UNLESS(1, GondowanSettlement, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanSettlementCave),
    SCENE_EXIT_UNLESS(1, GondowanSettlement, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, HesperiaSettlement, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(HesperiaSettlement),
    SCENE_EXIT_UNLESS(1, WorldMap, 49, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, HesperiaSettlementHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, HesperiaSettlementHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, HesperiaSettlementHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, HesperiaSettlementHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, HesperiaSettlementCave, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(HesperiaSettlementCave),
    SCENE_EXIT_UNLESS(2, HesperiaSettlement, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(HesperiaSettlementHouses),
    SCENE_EXIT_UNLESS(11, HesperiaSettlement, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, HesperiaSettlementHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, HesperiaSettlementHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, HesperiaSettlement, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(KaltIsland),
    SCENE_EXIT_UNLESS(1, WorldMap, 50, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, KaltIslandHouses, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(KaltIslandHouses),
    SCENE_EXIT_UNLESS(1, KaltIsland, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(SwAttekaIslet),
    SCENE_EXIT_UNLESS(1, WorldMap, 47, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, SwAttekaIsletHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(SwAttekaIsletHouses),
    SCENE_EXIT_UNLESS(1, SwAttekaIslet, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gSettlements_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 808, FACING_NORTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_GondowanSettlementPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 192, 0, 240, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(106, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 336, 0, 240, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_GondowanSettlementHousesPlacements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 632, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_HesperiaSettlementPlacements[] = {
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 280, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, 7, 56, 0, 240, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(287, CONDITION_ALWAYS, gSettlements_CrateBobScript, 280, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, 0x66, MOVEMENT_TYPE_STAND, 312, 0, 248, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_HesperiaSettlementHousesPlacements[] = {
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(106, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 696, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_AttekaIsletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 176, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 200, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 72, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 72, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 72, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 104, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 104, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 152, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(274, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 92, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, 0x7f, MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_AttekaIsletHousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 816, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_KaltIslandPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, 0x52, MOVEMENT_TYPE_STAND, 312, 0, 296, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, 7, 88, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_KaltIslandHousesPlacements[] = {
    ACTOR_PLACEMENT(75, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 216, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(76, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSettlements_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 9},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gSettlements_GondowanSettlementTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, CONDITION_ALWAYS,
     EVENT(Settlements_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 4, CONDITION_ALWAYS,
     EVENT(Settlements_StepOntoLadderFromWest)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK, CONDITION_ALWAYS,
     EVENT(Settlements_WarnOfRiddleOfRock)},
    {TRIGGER_TALK, ACTOR_GONDOWAN_BOY, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsThereWasGiantExplosion)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsMountainIsCalledMagmaRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GONDOWAN_BOY, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsExplosionsWithinMagmaRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10,
     CONDITION_ALWAYS, EVENT(Settlements_CutLeftBush)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11,
     CONDITION_ALWAYS, EVENT(Settlements_CutStairsBush)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_GondowanSettlementHousesTriggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_KIBOMBO, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsIHearKibomboHasFinally)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_KIBOMBO,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsWereLuckyRiverFloodsOver)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_HesperiaSettlementTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Settlements_EnterCave)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 3, CONDITION_ALWAYS,
     EVENT(Settlements_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 4, CONDITION_ALWAYS,
     EVENT(Settlements_StepOntoLadderFromWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 5, CONDITION_ALWAYS,
     EVENT(Settlements_StepOntoLadderFromEast)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 34, 0x220, EVENT(Settlements_StepOntoWall)},
    {TRIGGER_STEP, 35, 0x1220, EVENT(Settlements_StepOffWall)},
    {TRIGGER_TALK, ACTOR_HESPERIA_MAN, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsYouMustBeVeryBrave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HESPERIA_MAN, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsForContinentAsDesolate)},
    {TRIGGER_TALK, ACTOR_TINDER, CONDITION_ALWAYS,
     EVENT(Settlements_MeetTinder)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     CONDITION_ALWAYS, EVENT(Settlements_UnblockCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_CRATE, CONDITION_ALWAYS,
     EVENT(Settlements_BlockCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Settlements_UnblockCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Settlements_BlockCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, 0x230,
     EVENT(Settlements_GrowVine)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_HesperiaSettlementHousesTriggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_SHAMAN_VILLAGE, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsTheresPlaceCalledShamanVillage)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_SHAMANS, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsPeopleOfShamanVillage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_SHAMAN_VILLAGE,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsIThinkTheresCaveDown)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_SHAMANS,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsLongAgoShamanWasRavaged)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_KaltIslandTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS,
     EVENT(Settlements_EnterCave)},
    {TRIGGER_STEP, ACTOR_GEL, CONDITION_ALWAYS, EVENT(Settlements_StepOntoIce)},
    {TRIGGER_STEP, ACTOR_APPLE, CONDITION_ALWAYS,
     EVENT(Settlements_StepOffIce)},
    {TRIGGER_STEP, 20, 0x201, EVENT(Settlements_StepOnIceSlide)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A, FLAG_KALT_ROPE_TIED,
     EVENT(Settlements_LashRope)},
    {TRIGGER_TALK, ACTOR_GEL, CONDITION_ALWAYS, EVENT(Settlements_MeetGel)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_APPLE, FLAG_APPLE_TAKEN,
     EVENT(Settlements_CatchApple)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_KaltIslandHousesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_THE_GLACIERS, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsIfYouWantToCross)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_PROX_TRADE, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsLongTimeAgoWeUsed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_GLACIERS,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsPeopleOfLohoHaveLegendary)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_PROX_TRADE,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsNorthernSeaIsThick)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgSettlementsTheresKalticStewMade))},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_AttekaIsletTriggers[] = {
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TALK, ACTOR_ISLANDER, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsSeaWestOfHereSpills)},
    {TRIGGER_TALK, ACTOR_JUPITER_DJINNI, CONDITION_ALWAYS,
     EVENT(Settlements_MeetLostJupiterDjinni)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ISLANDER, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsMyBuddyThinksImNuts)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9,
     0x204, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, 0x204,
     EVENT(Settlements_MovePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     0x205, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FALLS_CRATE, 0x205,
     EVENT(Settlements_MoveCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Settlements_BlockFallsObject)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Settlements_PushIntoFalls)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, 0x208,
     EVENT(Settlements_LiftBoulder)},
    {TRIGGER_TABLE_END},
};

struct Trigger gSettlements_AttekaIsletHousesTriggers[] = {
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 1},
    {TRIGGER_TALK, ACTOR_ISLANDER_ON_THE_WEST, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsTheresNothingToWest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ISLANDER_ON_THE_WEST,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsCurrentsAreSoStrongNear)},
    {TRIGGER_TABLE_END},
};

s32 gSettlements_WaterColorFrame = -1;

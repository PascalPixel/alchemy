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
#include "macros.h"
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
#include "scene/event_layer.h"
#include "graphics/glyph.h"
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
/// Kalt Island: the leader is on the ice, where each step slides
/// (Leader_SlideOnIce).
#define FLAG_ON_ICE FLAG_SCENE_TEMP_01
/// SW Atteka Islet: the pillar has sunk into the river at (11, 12), and river
/// stone K stands in its place.
#define FLAG_PILLAR_SUNK FLAG_SCENE_TEMP_04
/// SW Atteka Islet: the crate has gone down the falls onto the sunk pillar and
/// rests there.
#define FLAG_FALLS_CRATE_AT_REST FLAG_SCENE_TEMP_05
/// SW Atteka Islet: the crate has gone down the falls with no pillar to stop
/// it, and is lost.
#define FLAG_FALLS_CRATE_LOST FLAG_SCENE_TEMP_06
/// SW Atteka Islet: the crate has bobbed in the water at (16, 12).
#define FLAG_FALLS_CRATE_BOBBED FLAG_SCENE_TEMP_07
/// SW Atteka Islet: Lift has raised the boulder.
#define FLAG_BOULDER_LIFTED FLAG_SCENE_TEMP_08
/// Hesperia Settlement: the party walks along the top of the wall.
#define FLAG_ON_WALL FLAG_SCENE_TEMP_20
/// Hesperia Settlement: the crate has dropped off the wall into the water.
#define FLAG_WALL_CRATE_DROPPED FLAG_SCENE_TEMP_21
/// Hesperia Settlement: Growth has made the sprout a vine.
#define FLAG_VINE_GROWN FLAG_SCENE_TEMP_30

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

FIXED Trig_Cos(s32 angle);

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
void Settlements_DriftSmoke(struct Actor *actor);
void Actor_Release(struct Actor *actor);
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Irq_SetHandler(s32 a, s32 b, void (*callback)(void));
void Event_End(void);
void ItemDisplay_ShowItem(s32 a, s32 b);

extern u32 gSettlements_HesperiaSettlementHousesRegions[];
extern u32 gSettlements_GondowanSettlementCaveRegions[];

#include "shared/tables.h"
#include "shared/event_check_spheres_overlap.h"

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

/// The apple as Kalt Island floats it: an actor whose fields hold the centre
/// and angle of its sway, as The Broken Seal's OrbitingMapActor.
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

/// Three 16.16 fixed-point components.
struct SettlementsVector {
  s32 x;
  s32 y;
  s32 z;
};

void Rope_Initialize(void);

void Settlements_StepOntoWall(void);

void FieldEffect_StartBlankOverlay(s32 a);

void Actor_PlaceBlocks(const void *data);

u16 gSettlements_WaterColors[16] BSS_DATA;
u16 gSettlements_WaterColorsSaved[16] BSS_DATA;
u8 gSettlements_UnusedAfterWaterColorsSaved[64] BSS_DATA;
s32 gSettlements_WaterfallLine BSS_DATA;
u16 gSettlements_WaterfallScrollAbove BSS_DATA;
u16 gSettlements_WaterfallScrollBelow BSS_DATA;

void Settlements_SpawnSplashRing(s32 objectId);

void Settlements_MovePillar(void);

/// Gel, the Mercury Djinni on Kalt Island, joins the party.
void Settlements_MeetGel(void) {
  Djinn_JoinParty(ACTOR_GEL, DJINNI_GEL / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_GEL));
}

/// Tinder, the Mars Djinni in Hesperia Settlement, joins the party.
void Settlements_MeetTinder(void) {
  Djinn_JoinParty(ACTOR_TINDER, DJINNI_TINDER / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_TINDER));
}

/// The Jupiter Djinni the game's text calls Wind19, on SW Atteka Islet, joins
/// the party.
void Settlements_MeetLostJupiterDjinni(void) {
  Djinn_JoinParty(ACTOR_JUPITER_DJINNI, DJINNI_WIND19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_WIND19));
}

/// A script call: draws the actor without its sprite flags.
s32 Settlements_ClearSpriteFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
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

/// The apple floating on Kalt Island is taken: it is put away and the party
/// receives an Apple.
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

/// Asks whether the party has come about the riddle of the rock, and answers
/// either way.
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

/// Cyclone clears the bush on the left of Gondowan Settlement.
void Settlements_CutLeftBush(void) {
  FieldTileCopies_CopyTouchedCells(5, 70, 9, 7, 5, 43, 1);
}

/// Cyclone clears the bush on the stairs of Gondowan Settlement: the leader
/// standing on it hops off, and the stairs open.
void Settlements_CutStairsBush(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (x == 5 && z == 17) {
    FieldActor_Jump(gMainRoutineWork.leaderActorId, 2, 0);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 88, 296);
  }
  TileCopies_CopyTouchedCells(5, 70, 9, 7, 5, 43, 1);
  MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
  MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
  Event_SetFlag(FLAG_STAIRS_BUSH_CUT);
}

/// The leader climbs down the ladder: steps onto its top, turns and climbs down
/// out of the scene.
void Settlements_ClimbDownLadder(void) {
  s32 next = gFieldWork->stepTrigger;

  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_Get(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 4);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FELIX), 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 16);
  FieldActor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
  FieldEvent_Wait(10);
  Event_PlaySound(SE_MAP_EXIT);
  FieldEvent_SetExitRequest(next);
  Event_End();
}

/// The leader steps onto the ladder from the west and climbs down.
void Settlements_StepOntoLadderFromWest(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Settlements_ClimbDownLadder();
}

/// The leader steps onto the ladder from the east and climbs down.
void Settlements_StepOntoLadderFromEast(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_WalkByAndWait(ACTOR_FELIX, -2, -6);
  Actor_WalkByAndWait(ACTOR_FELIX, -14, -8);
  Settlements_ClimbDownLadder();
}

/// The leader turns north on the ladder and climbs down.
void Settlements_TurnAndClimbDownLadder(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Settlements_ClimbDownLadder();
}

/// Growth makes Hesperia Settlement's sprout a vine the party can climb.
void Settlements_GrowVine(void) {
  MapGrid_CopyCellAttributes(0, 3, 1, 1, 3, 15);
  Event_SetFlag(FLAG_VINE_GROWN);
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

/// Drifts a chimney's puff sideways ever faster, raises and widens it, and
/// releases it when its frames run out.
void Settlements_DriftSmoke(struct Actor *actor) {
  s32 step;

  actor->x.fixed += actor->timer << 8;

  actor->y.fixed += toFIXED(1) / 2;

  step = FIXED_ONE / 50;
  actor->scaleX += step;
  actor->scaleY += step;
  actor->timer += 2;
  if (--*(s32 *)&actor->watchedActor == 0)
    Actor_Release(actor);
}

/// Adds one puff of chimney smoke, as Prox's roofs do.
void Settlements_SpawnSmoke(s32 x, s32 y, s32 z) {
  struct Actor *actor = FieldActor_Create(SPRITE_DUST_CLOUD, x, y, z);

  if (actor != NULL) {
    actor->retainSpriteCache = 2;
    actor->motionFlags = 0;
    actor->timer = 0;
    actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_WHITE);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
    Actor_SetSpriteFlags(actor, 0);
    actor->scaleX = FIXED_ONE / 2;
    actor->scaleY = FIXED_ONE / 2;
    *(s32 *)&actor->watchedActor = 60;
    actor->update = (void (*)(void))Settlements_DriftSmoke;
    Actor_SelectAnimation(actor, 5);
    Actor_SetBlendMode(actor, 1);
  }
}

/// A task on Kalt Island: a puff from the chimney every 64 frames.
void Settlements_PuffChimneySmoke(void) {
  if ((gFrameCount & 63) == 0)
    Settlements_SpawnSmoke(toFIXED(248), toFIXED(48), toFIXED(256));
}

/// The apple's callback, each frame: it sways up to two pixels either side of
/// its spot and dips up to two pixels below it, at a random pace, as The Broken
/// Seal's Nut floats.
s32 Settlements_FloatApple(struct OrbitObject *apple) {
  struct FieldSprite *sprite = (struct FieldSprite *)apple->sprite;
  FIXED dip = Math_Sin(apple->orbitAngle) * 2;

  if (dip > 0)
    dip = -dip;
  apple->x = apple->orbitCenterX + Trig_Cos(apple->orbitAngle) * 2;
  apple->y = apple->orbitCenterY + dip;
  sprite->rotation = Trig_Cos(apple->orbitAngle + DEGtoANG(180)) / 8;
  // The pace: a sixty-fourth of a turn and two random steps under 1/128 each.
  apple->orbitAngle += Random_Below(512) + Random_Below(512) + TRIG_TURN / 64;
  return 0;
}

void Rope_Place(s32 index, s32 flag, s32 post, s32 otherPost);

#include "shared/field_rope_place.c"

/// Sets up Kalt Island: the blend, the chimney smoke, the rope between its
/// posts, and the apple floating, its icon drawn, until it is taken.
void Settlements_SetUpKaltIsland(void) {
  struct OrbitObject *actor;
  struct OrbitSprite *sprite;
  GlyphTransfer *glyph;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = TASK_PASS_VBLANK;

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
    actor->y += toFIXED(16);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->visible = 1;
  glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
  Item_LoadIcon(ITEM_APPLE);
  Vram_Load(sprite->vramBlock, ICON_TILE_BYTES, glyph->tiles);
  HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  actor->orbitCenterX = actor->x;
  actor->orbitAngle = 0;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Settlements_FloatApple;
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
    if (Event_GetFlag(FLAG_FALLS_CRATE_LOST))
      Actor_SetPosition(ACTOR_FALLS_CRATE, 0, 0);
    Actor_GetById(ACTOR_FALLS_CRATE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_RIVER_STONE_H, 2);
    Actor_SetAnimation(ACTOR_RIVER_STONE_I, 2);
    Actor_SetAnimation(ACTOR_RIVER_STONE_K, 2);
    if (Event_GetFlag(FLAG_FALLS_CRATE_AT_REST))
      Actor_SetAnimation(ACTOR_FALLS_CRATE, 2);
    Settlements_StartWaterfall();
#if TLA_PATCH_1
    if (Event_GetFlag(FLAG_PILLAR_SUNK)) {
      Actor_SetPosition(ACTOR_PILLAR, 0, 0);
      Actor_SetPosition(ACTOR_RIVER_STONE_K, 184, 184);
      Actor_GetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.next == SCENE_HESPERIA_SETTLEMENT) {
    if (Event_GetFlag(FLAG_VINE_GROWN)) {
      MapGrid_CopyCellAttributes(0, 3, 1, 1, 3, 15);
      Actor_SetAnimation(ACTOR_SPROUT, 4);
    }
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
        Event_GetFlag(FLAG_ON_WALL)) {
      Event_ClearFlag(FLAG_ON_WALL);
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
    if (Event_GetFlag(FLAG_FALLS_CRATE_AT_REST)) {
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
    }
#if !TLA_PATCH_1
    if (Event_GetFlag(FLAG_FALLS_CRATE_AT_REST))
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
#else
    if (Event_GetFlag(FLAG_PILLAR_SUNK)) {
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

/// On Kalt Island's ice: the slide starts.
void Settlements_StepOntoIce(void) { GameFlag_Set(FLAG_ON_ICE); }

/// Off Kalt Island's ice.
void Settlements_StepOffIce(void) { GameFlag_Clear(FLAG_ON_ICE); }

/// Steps the leader's slide on Kalt Island's ice.
void Settlements_StepOnIceSlide(void) { Leader_SlideOnIce(); }

/// Goes into a settlement's cave: the placed actors stop, the door opens and
/// the leader walks in.
void Settlements_EnterCave(void) {
  u32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct Actor *actor = Actor_Get(i);

    if (actor != 0)
      actor->motionFlags = 0;
  }
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

/// Lash ties the rope between Kalt Island's two posts.
void Settlements_LashRope(void) {
  Rope_Lash(1, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  Event_SetFlag(FLAG_KALT_ROPE_TIED);
}

extern s32 gSettlements_WaterColorFrame;

/// A task on SW Atteka Islet: rotates the four colours of the falls' water in
/// BG palette 3 every 8 frames.
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

    copy(colors, (void *)&BG_PLTT_COLOR(3, 0),
         sizeof(gSettlements_WaterColors));
    copy(gSettlements_WaterColorsSaved, colors,
         sizeof(gSettlements_WaterColors));
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
      *entry++ = (u32)&BG_PLTT_COLOR(3, 0);
      *entry = DMA_COPY_WORDS(8);
    }
    IRQ_UNLOCK(saved);
  }
  gSettlements_WaterColorFrame++;
}

/// Starts SW Atteka Islet's falls: the water's colours, its sound and the
/// hblank split of its scroll.
void Settlements_StartWaterfall(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  FieldTask_AddFrame(Settlements_CycleWaterColors, TASK_PASS_VBLANK);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Scroll_StartHBlank();
}

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
  s16 y = FIXEDtoINT(layer->y);

  gSettlements_WaterfallLine = 1448 - y;
  gSettlements_WaterfallScrollAbove = FIXEDtoINT(layer->x);
  gSettlements_WaterfallScrollBelow =
      (s16)FIXEDtoINT(layer->x) - (gFrameCount >> 2);
}

#include "shared/scroll_start_hblank.h"

/// Lifts the block a pushed pillar or crate leaves on SW Atteka Islet.
void Settlements_UnblockFallsObjects(void) {
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

/// Pushing a pillar or crate on SW Atteka Islet: the leader is drawn above it
/// and its block is lifted.
void Settlements_BlockFallsObject(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

void Settlements_SplashCrate(s32 id);

/// A pillar pushed into the river at (11, 12) sinks, and river stone K takes
/// its place in the stream.
void Settlements_SinkPillar(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PILLAR);
  s32 column = FIELD_CELL(actor->x.fixed);
  s32 row = FIELD_CELL(actor->z.fixed);
  s32 i;

  if (!Event_GetFlag(FLAG_PILLAR_SUNK) && column == 11 && row == 12) {
    actor->motionFlags = 0;
    for (i = 10; i > 0; i--) {
      actor->y.fixed -= toFIXED(2);
      Event_Wait(1);
    }
    Settlements_SplashCrate(ACTOR_PILLAR);
    Actor_SetPosition(ACTOR_PILLAR, 0, 0);
    Actor_SetPosition(ACTOR_RIVER_STONE_K, 184, 184);
    Actor_GetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
    Actor_GetById(ACTOR_RIVER_STONE_K)->y.fixed -= toFIXED(64);
    MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    Event_SetFlag(FLAG_PILLAR_SUNK);
  }
}

/// Sinks object 2 pixels a frame for the given frames, counting in i.
#define SINK(frames)                                                           \
  i = (frames);                                                                \
  do {                                                                         \
    FieldEvent_Wait(1);                                                        \
    actor->y.fixed -= toFIXED(2);                                              \
    i--;                                                                       \
  } while (i > 0)

/// A crate pushed into the falls at (11, 9) sinks, slides down the falls and is
/// lost, or once the pillar has sunk comes to rest below; at (16, 12) it bobs a
/// moment.
void Settlements_SlideCrateDownFalls(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FALLS_CRATE);
  s32 column = FIELD_CELL(actor->x.fixed);
  s32 row = FIELD_CELL(actor->z.fixed);
  s32 i;
  u8 *flags;

  if (column == 11 && row == 9) {
    actor->motionFlags = 0;
    SINK(8);
    Settlements_SplashCrate(ACTOR_FALLS_CRATE);
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
    Settlements_SplashCrate(ACTOR_FALLS_CRATE);
    if (!Event_GetFlag(FLAG_PILLAR_SUNK)) {
      FieldEvent_Wait(10);
      Actor_SetSpeed(ACTOR_FALLS_CRATE, 0.8);
      Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
      FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
      Actor_SetSpeed(ACTOR_FALLS_CRATE, 2.5);
      Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
      FieldEvent_Wait(5);
      Actor_SetPosition(ACTOR_FALLS_CRATE, 0, 0);
      Event_SetFlag(FLAG_FALLS_CRATE_LOST);
    } else {
      MapGrid_CopyCellsTo(12, 32, 11, 15, 1, 1);
      MapGrid_CopyCellsTo(77, 15, 75, 15, 1, 1);
      Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 12);
      FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
      FieldEvent_Wait(20);
      MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 11);
      Actor_SetAnimation(ACTOR_FALLS_CRATE, 2);
      FieldEvent_Wait(30);
      Event_SetFlag(FLAG_FALLS_CRATE_AT_REST);
    }
    Event_End();
  } else if (!Event_GetFlag(FLAG_FALLS_CRATE_BOBBED) && column == 16 &&
             row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &actor->motionFlags;
    *flags = 3;
    FieldEvent_Wait(30);
    *flags = 0;
    Event_SetFlag(FLAG_FALLS_CRATE_BOBBED);
    Event_End();
  }
}

/// A pillar on SW Atteka Islet has been pushed: placed, and sunk if in the
/// river.
void Settlements_MovePillar(void) {
  Actor_PlaceMovedBlock();
  Settlements_SinkPillar();
}

/// A crate on SW Atteka Islet has been pushed: placed, and down the falls if
/// there.
void Settlements_MoveCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_SlideCrateDownFalls();
}

/// Something has been pushed on SW Atteka Islet: placed, then sunk or sent down
/// the falls by where it stands.
void Settlements_PushIntoFalls(void) {
  Actor_PlaceMovedBlock();
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags += 254;
  Settlements_SinkPillar();
  Settlements_SlideCrateDownFalls();
}

/// Keeps the collision of the wall's two 10 by 10 stretches before the crate
/// moves.
void Settlements_SaveWallCells(void) {
  MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 0);
  MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 10);
}

/// Gives the wall's two stretches their kept collision back.
void Settlements_RestoreWallCells(void) {
  MapGrid_CopyCellAttributes(64, 0, 10, 10, 11, 73);
  MapGrid_CopyCellAttributes(64, 10, 10, 10, 42, 73);
}

/// Onto Hesperia Settlement's wall: its collision is switched for walking along
/// the top until the party steps off.
void Settlements_StepOntoWall(void) {
  if (!Event_GetFlag(FLAG_WALL_CRATE_DROPPED) && !Event_GetFlag(FLAG_ON_WALL)) {
    MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 50);
    MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 60);
    MapGrid_CopyCellAttributes(42, 11, 10, 4, 11, 75);
    Event_SetFlag(FLAG_ON_WALL);
  }
}

/// Off Hesperia Settlement's wall: its stretches take their own collision back.
void Settlements_StepOffWall(void) {
  if (Event_GetFlag(FLAG_ON_WALL)) {
    MapGrid_CopyCellAttributes(64, 50, 10, 10, 11, 73);
    MapGrid_CopyCellAttributes(64, 60, 10, 10, 42, 73);
    Event_ClearFlag(FLAG_ON_WALL);
  }
}

/// Pushing Hesperia Settlement's crate: its cells open by where it stands.
void Settlements_UnblockCrate(void) {
  struct Actor *actor = Actor_Get(ACTOR_WALL_CRATE);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

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

/// Hesperia Settlement's crate pushed off the wall: where it lands it sinks
/// into the water, and at (19, 15), while Tinder is not yet met, it runs the
/// crate's drop script.
void Settlements_DropCrate(void) {
  struct Actor *actor;
  s32 column;
  s32 row;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  EventCamera_FollowActor(ACTOR_WALL_CRATE, 1);
  actor = Actor_Get(ACTOR_WALL_CRATE);
  column = FIELD_CELL(actor->x.fixed);
  row = FIELD_CELL(actor->z.fixed);
  // XXX: the two sinkings with flags of their own, at (16, 15) and (13, 15)
  // (FLAG_SCENE_TEMP_05 and 04), are named once the map shows what lies there.
  if (column == 12 && row == 15) {
    Event_SetFlag(FLAG_WALL_CRATE_DROPPED);
    actor->motionFlags = 0;
    SINK(32);
    Settlements_SpawnSplashRing(ACTOR_WALL_CRATE);
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_05) && column == 16 && row == 15) {
    actor->motionFlags = 0;
    Event_SetFlag(FLAG_SCENE_TEMP_05);
    Event_SetFlag(FLAG_WALL_CRATE_DROPPED);
    SINK(48);
    Settlements_SpawnSplashRing(ACTOR_WALL_CRATE);
    FieldEvent_Wait(30);
  } else if (column == 19 && row == 15) {
    MapGrid_CopyCellAttributes(18, 16, 1, 1, column, row);
    actor->motionFlags = 0;
    Event_SetFlag(FLAG_WALL_CRATE_DROPPED);
    SINK(32);
    Settlements_SpawnSplashRing(ACTOR_WALL_CRATE);
    if (!Event_GetFlag(FLAG_DJINNI(DJINNI_TINDER))) {
      Actor_SetPosition(ACTOR_TINDER, 0, 0);
      Actor_SetScript(ACTOR_WALL_CRATE, gSettlements_CrateDropScript);
      FieldEvent_Wait(20);
    }
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_SCENE_TEMP_04) && column == 13 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
    actor->motionFlags = 0;
    SINK(16);
    Settlements_SpawnSplashRing(ACTOR_WALL_CRATE);
    FieldEvent_Wait(20);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
  } else if (column == 14 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
  }
  Event_End();
}

/// Hesperia Settlement's crate has been pushed: placed, the wall's collision
/// restored, and dropped if it went over the edge.
void Settlements_BlockCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_RestoreWallCells();
  Settlements_DropCrate();
}

extern s32 gSettlements_SmokeScript[];

/// Water sprays up where an actor goes into the water.
void Settlements_SplashCrate(s32 id) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: of the 68 bytes it reserves. */
  UNUSED struct SettlementsVector velocity;
  struct EffectOptions spawn;
  struct Actor *actor = Actor_Get(id);

  spawn.type = SPRITE_WATER_SPRAY;
  spawn.startScaleX = FIXED_ONE * 4 / 5;
  spawn.startScaleY = FIXED_ONE * 4 / 5;
  spawn.targetScaleX = FIXED_ONE;
  spawn.targetScaleY = FIXED_ONE;
  spawn.script = gSettlements_SmokeScript;
  Event_PlaySound(SE_CAST);
  FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, 0, 0,
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

/// A ring of nine splashes thrown out around an actor.
void Settlements_SpawnSplashRing(s32 objectId) {
  struct Actor *actor;
  struct Vector3 vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  FIXED z;

  actor = Actor_Get(objectId);
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
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, x, vec.y,
                      z, EFFECT_USE_UPDATE | 1, &params);
  }
}

/// Lift raises SW Atteka Islet's boulder.
void Settlements_LiftBoulder(void) { GameFlag_Set(FLAG_BOULDER_LIFTED); }

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern s16 gSlideOnIce_AnglesByKey[];
s32 Field_CheckSpheresOverlap(FIXED *a, s32 aRadius, FIXED *b, s32 bRadius);

#include "shared/leader_slide_on_ice.h"

volatile s16 gSlideOnIce_Stopped BSS_DATA;

extern s16 gSlideOnIce_AnglesByKey[];
void Actor_SteerByPad(struct Actor *actor);
void Leader_StopIfPending(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Actor_HasArrived(struct Actor *actor);
s32 Field_CheckSpheresOverlap(FIXED *a, s32 aRadius, FIXED *b, s32 bRadius);

#include "shared/slide_on_ice_from_edge.c"

/// Stops or restarts the leader's slide on the ice (Leader_SlideOnIce).
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
#include "shared/field_effect_duration_scripts.c"

s16 gSettlements_FallsObjects[] = {
    9, 10, -1,
};

s16 gSettlements_WallCrate[] = {
    10, -1,
};

#include "shared/slide_on_ice_angles.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gSettlements_BobScript[] = {
    SC_ANIMATION(3),
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, -toFIXED(16)),
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
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 336, 0, 240, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_GondowanSettlementHousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 120, 0, 632, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_HesperiaSettlementPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 392, 0, 280, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, 7, 56, 0, 240, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, gSettlements_CrateBobScript, 280, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_TINDER), MOVEMENT_TYPE_STAND, 312, 0, 248, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_HesperiaSettlementHousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_SPIKY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 696, 0, 456, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_AttekaIsletPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 176, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 200, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 72, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 72, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 88, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 104, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 72, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 104, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 104, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 120, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 152, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 152, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_RIVER_STONE, CONDITION_ALWAYS, (s32)gSettlements_BobScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 92, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_WIND19), MOVEMENT_TYPE_STAND, 72, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_AttekaIsletHousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_RUFFIAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 816, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_KaltIslandPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 200, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_GEL), MOVEMENT_TYPE_STAND, 312, 0, 296, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, 7, 88, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gSettlements_KaltIslandHousesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 216, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
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
    {TRIGGER_STEP, 34, FLAG_ON_WALL, EVENT(Settlements_StepOntoWall)},
    {TRIGGER_STEP, 35, IF_FLAG_SET(FLAG_ON_WALL),
     EVENT(Settlements_StepOffWall)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_VINE_GROWN,
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
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Settlements_StepOntoIce)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(Settlements_StepOffIce)},
    {TRIGGER_STEP, 20, FLAG_ON_ICE, EVENT(Settlements_StepOnIceSlide)},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_PILLAR_SUNK,
     EVENT(Settlements_MovePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10,
     0x205, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FALLS_CRATE,
     FLAG_FALLS_CRATE_AT_REST, EVENT(Settlements_MoveCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(Settlements_BlockFallsObject)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(Settlements_PushIntoFalls)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, FLAG_BOULDER_LIFTED,
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

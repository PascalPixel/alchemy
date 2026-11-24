/// @file
/// Four small places the ship can put in at, sharing one overlay, each with its
/// houses. In Gondowan Settlement, below Magma Rock, Cyclone blows away the
/// leftmost bush to uncover a ladder down to a cave with Star Dust. Hesperia
/// Settlement is built into a cliff: Growth turns its sprout into a vine, the
/// crate on top of the wall is pushed down to make a step up to Tinder, and a
/// hole beside a palm on the wall top leads down into a room below. On Kalt
/// Island Catch takes the apple out of the snowy trees, the frozen river slides
/// Felix along, Lash ties a rope across it, and Gel waits by the house. On SW
/// Atteka Islet, by the falls at the edge of the world, the pillar pushed into
/// the river stops the crate sent down the falls, and once the boulder is
/// lifted a Jupiter Djinni the party lacks from The Broken Seal waits among the
/// posts in the river.
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
#include "system/main_routine_state.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "field/field_event.h"
#include "field/field.h"
#include "graphics/palette_cycles.h"
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
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "scene/event_layer.h"
#include "graphics/glyph.h"
#include "system/input.h"
#include "text/msg_ids.h"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

/// SW Atteka Islet's background palette whose water colours run down the falls.
#define FALLS_PALETTE 3

/// Kalt Island's rope across the frozen river, tied by Lash, for this visit.
#define FLAG_KALT_ROPE_TIED FLAG_SCENE_TEMP_03
/// Kalt Island: set by a step onto cell (5, 12), the top of the climb on the
/// frozen river's west bank, and cleared by one onto cell (5, 13) below it;
/// while it is set the river's ice slides no one (Leader_SlideOnIce).
#define FLAG_ICE_SLIDES_STOPPED FLAG_SCENE_TEMP_01
/// SW Atteka Islet: the pillar has been pushed into the river at cell (11, 12),
/// and its top stands in the water like the river's other posts.
#define FLAG_PILLAR_SUNK FLAG_SCENE_TEMP_04
/// SW Atteka Islet: the crate has gone down the falls and stuck against the
/// sunk pillar, making the path to the Dragon Skin's chest.
#define FLAG_FALLS_CRATE_AT_REST FLAG_SCENE_TEMP_05
/// SW Atteka Islet: the crate has gone down the falls with no pillar to stop
/// it, and is lost.
#define FLAG_FALLS_CRATE_LOST FLAG_SCENE_TEMP_06
/// SW Atteka Islet: the crate, moved onto cell (16, 12) at the grass's south
/// edge, has dropped down the cliff there.
#define FLAG_FALLS_CRATE_DROPPED FLAG_SCENE_TEMP_07
/// SW Atteka Islet: Lift has raised the boulder.
#define FLAG_BOULDER_LIFTED FLAG_SCENE_TEMP_08
/// Hesperia Settlement: Felix has stepped on cell (13, 11) and walks by the
/// wall top's cells (Settlements_StepOntoWall).
#define FLAG_ON_WALL FLAG_SCENE_TEMP_20
/// Hesperia Settlement: the crate has been pushed down off the wall.
#define FLAG_WALL_CRATE_DROPPED FLAG_SCENE_TEMP_21
/// Hesperia Settlement: the crate has been pushed off the wall at cell
/// (16, 15), the wall's whole height, to the ground below.
#define FLAG_WALL_CRATE_ON_GROUND FLAG_SCENE_TEMP_05
/// Hesperia Settlement: the crate, pushed down onto the step at cell (12, 15),
/// has been pushed east off it to the ground at cell (13, 15).
#define FLAG_WALL_CRATE_OFF_STEP FLAG_SCENE_TEMP_04
/// Hesperia Settlement: Growth has made the sprout below the west cliff a
/// vine.
#define FLAG_VINE_GROWN FLAG_SCENE_TEMP_30

/// The bush over the ladder hole in the west of Gondowan Settlement is blown
/// away.
#define FLAG_LADDER_BUSH_CUT 0xa60

/// Gondowan Settlement's cells that run an event. The two beside the ladder
/// hole are the numbers of the exits they lead down by.
enum GondowanSettlementCell {
  /// Cell (5, 16), north of the hole.
  GONDOWAN_LADDER_HOLE_NORTH = 3,
  /// Cell (4, 17), west of the hole.
  GONDOWAN_LADDER_HOLE_WEST = 4,
  /// The leafy bushes west of the village, such as cell (7, 15).
  GONDOWAN_BUSH = 10,
  /// The leftmost bush, at cell (5, 17), over the ladder hole.
  GONDOWAN_LADDER_BUSH = 11
};

/// Hesperia Settlement's cells that run an event. The door and the three
/// sides of the ladder hole are the numbers of the exits they lead by.
enum HesperiaSettlementCell {
  /// Cell (23, 16), before the door in the cliff east of Tinder's ledge.
  HESPERIA_HOUSE_DOOR = 2,
  /// Cell (23, 12), north of the hole beside the palm on the wall top.
  HESPERIA_LADDER_HOLE_NORTH = 3,
  /// Cell (22, 13), west of the hole.
  HESPERIA_LADDER_HOLE_WEST = 4,
  /// Cell (24, 13), east of the hole.
  HESPERIA_LADDER_HOLE_EAST = 5,
  /// Cell (13, 11), on the stairs in the cliff west of the wall, where Felix
  /// comes onto the wall top's cells.
  HESPERIA_ONTO_WALL = 34,
  /// Cell (10, 9), further along the stairs, where he leaves them.
  HESPERIA_OFF_WALL = 35
};

/// Kalt Island's cells that run an event.
enum KaltIslandCell {
  /// Cell (14, 18), before the house's door (its exit's number).
  KALT_HOUSE_DOOR = 2,
  /// Cell (5, 12), the top of the climb on the frozen river's west bank.
  KALT_ICE_SLIDES_OFF = 10,
  /// Cell (5, 13), below it.
  KALT_ICE_SLIDES_ON = 11,
  /// The frozen river's ice.
  KALT_ICE = 20
};

/// The people of Gondowan Settlement, by the rows of
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
  ACTOR_SPROUT, ///< below the west cliff, grown into a vine by Growth
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
  /// The two posts on either bank of the frozen river, tied by Lash.
  ACTOR_ROPE_POST_A = ACTOR_FIRST_PLACED,
  ACTOR_ROPE_POST_B,
  ACTOR_GEL,
  ACTOR_APPLE ///< in the snowy trees west of the river, taken by Catch
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
  ACTOR_PILLAR,      ///< on the grass by the river, pushed into it
  ACTOR_FALLS_CRATE, ///< by the tent, pushed down the falls
  /// The stone posts standing in the river, alike, bobbing in the current; the
  /// last is kept out of sight until the pillar sinks in its place.
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
  ACTOR_BOULDER, ///< on the grass above the river's posts, raised by Lift
  /// The Jupiter Djinni among the posts in the river, standing for whichever
  /// of The Broken Seal's Jupiter Djinn the party lacks.
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

void Smoke_Spawn(s32 x, s32 y, s32 z);
void Settlements_CycleWaterColors(void);
void Scroll_StartHBlank(void);
void Settlements_SinkPillar(void);
void Settlements_SlideCrateDownFalls(void);
void Settlements_RestoreWallCells(void);
void Settlements_DropCrate(void);
void Leader_SlideOnIce(void);
void Leader_StopMoving(void);
void Event_EndAllLastingPsynergy(void);
void Scene_QueueMusic(s32 soundId);
#ifdef MODERN
void Rope_Lash(s32 index, s32 fromId, s32 toId);
#endif
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);
void Settlements_ClimbDownLadder(void);
void Smoke_Drift(struct Actor *actor);
void Actor_Release(struct Actor *actor);
void Scroll_HBlank(void);
void Layer_TrackScroll(void);
void Irq_SetHandler(s32 irq, s32 vcount, void (*handler)(void));
void Event_End(void);
void ItemDisplay_ShowItem(s32 item, s32 flags);

extern struct SceneRegion gSettlements_HesperiaSettlementHousesRegions[];
extern struct SceneRegion gSettlements_GondowanSettlementCaveRegions[];

#include "shared/tables.h"
#include "shared/event_check_spheres_overlap.h"

extern s16 gSlideOnIce_AnglesByKey[];

void Settlements_StepOntoWall(void);

void Leader_StopIfPending(void);

void Settlements_MeetTinder(void);

void Settlements_CatchApple(void);
void Settlements_CutBushes(void);

void Settlements_CutLadderBush(void);

void Settlements_StepOntoLadderFromEast(void);

void Settlements_TurnAndClimbDownLadder(void);

void Settlements_StopIceSlides(void);

void Settlements_ResumeIceSlides(void);
void Settlements_StepOnIceSlide(void);
void Settlements_LashRope(void);

void Settlements_UnblockFallsObjects(void);

void Settlements_StartPushingFallsObject(void);

void Settlements_MoveCrate(void);

void Settlements_StepOffWall(void);

void Settlements_UnblockCrate(void);

void Settlements_BlockCrate(void);

void Settlements_LiftBoulder(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern s32 gSettlements_BobScript[];
extern s32 gSettlements_CrateBobScript[];

/// The apple's sprite as Kalt Island sets it up: the parts of struct
/// FieldSprite it changes.
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

void FieldEffect_StartBlankOverlay(s32 actor);

void Actor_PlaceBlocks(const void *data);

/// The falls' palette as the map loads it, and as it is shown with its water
/// colours turned.
u16 gSettlements_LoadedWaterColors[16] BSS_DATA;
u16 gSettlements_ShownWaterColors[16] BSS_DATA;
u8 gSettlements_UnusedAfterShownWaterColors[64] BSS_DATA;
s32 gSettlements_WaterfallLine BSS_DATA;
u16 gSettlements_WaterfallScrollAbove BSS_DATA;
u16 gSettlements_WaterfallScrollBelow BSS_DATA;

void Settlements_SpawnDustRing(s32 actorId);

void Settlements_MovePillar(void);

/// Felix talks to Gel, the Mercury Djinni in the snow east of Kalt Island's
/// house, and Gel joins the party: "Felix found the Mercury Djinni Gel!"
void Settlements_MeetGel(void) {
  Djinn_JoinParty(ACTOR_GEL, DJINNI_GEL / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_GEL));
}

/// Felix talks to Tinder, the Mars Djinni on the ledge below Hesperia
/// Settlement's wall, and Tinder joins the party: "Felix found the Mars Djinni
/// Tinder!"
void Settlements_MeetTinder(void) {
  Djinn_JoinParty(ACTOR_TINDER, DJINNI_TINDER / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_TINDER));
}

/// Felix talks to the Jupiter Djinni among the posts in SW Atteka Islet's
/// river, west of the boulder Lift moves, and it joins the party. It is
/// whichever of The Broken Seal's seven Jupiter Djinn the party did not bring;
/// with none missing, it is not there at all.
void Settlements_MeetLostJupiterDjinni(void) {
  Djinn_JoinParty(ACTOR_JUPITER_DJINNI, DJINNI_WIND19 / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_WIND19));
}

/// A script call: clears the actor's sprite flags.
s32 Settlements_ClearSpriteFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

#include "shared/actor_bob.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  s32 scene = gMainRoutineState.next;
  struct SceneRegion *placements;

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
  s16 scene = gMainRoutineState.next;

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

/// Felix casts Catch on the apple floating in the snowy trees west of Kalt
/// Island's frozen river; the hand brings it to him, he holds it up, and
/// "Felix got an Apple."
void Settlements_CatchApple(void) {
  s32 mode;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_SetPosition(ACTOR_APPLE, 0, 0);
  Event_SetFlag(FLAG_KALT_ISLAND_APPLE_TAKEN);
  ItemDisplay_ShowItem(ITEM_APPLE, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
  mode = 0;
  Party_GiveItem(ITEM_APPLE, mode);
  Event_End();
}

/// Talking to the man wandering in front of the houses of Gondowan Settlement:
/// "Don't tell me, you're here to solve the riddle of the rock!" Yes, and he
/// warns that even the warriors who make it back barely survive; no, and he
/// says to steer clear of the mountain.
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

/// Felix casts Cyclone on the leafy bushes west of Gondowan Settlement's
/// houses (cell (7, 15) and the rest), and the ones the whirlwind reaches are
/// blown away and stay gone.
void Settlements_CutBushes(void) {
  FieldTileCopies_CopyTouchedCells(5, 70, 9, 7, 5, 43, 1);
}

/// Felix casts Cyclone on the leftmost bush west of Gondowan Settlement (cell
/// (5, 17)), and blowing it away uncovers a hole with a ladder down to a cave.
/// Had he been standing on the bush, he hops off it to the south first.
void Settlements_CutLadderBush(void) {
  struct Actor *actor = Actor_Get(gMainRoutineState.leaderActorId);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (x == 5 && z == 17) {
    FieldActor_Jump(gMainRoutineState.leaderActorId, 2, 0);
    Actor_MoveToAndWait(gMainRoutineState.leaderActorId, 88, 296);
  }
  TileCopies_CopyTouchedCells(5, 70, 9, 7, 5, 43, 1);
  MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
  MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
  Event_SetFlag(FLAG_LADDER_BUSH_CUT);
}

/// Felix steps onto the ladder's top, faces north and climbs down it, and the
/// scene changes to the place below.
void Settlements_ClimbDownLadder(void) {
  s32 next = gFieldState->stepTrigger;

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

/// Felix walks east onto the ladder hole on Hesperia Settlement's wall top,
/// beside the palm (cell (22, 13)), or onto the hole Cyclone uncovered in
/// Gondowan Settlement (cell (4, 17)), and climbs down: in Hesperia into a
/// room with beds below, in Gondowan into the cave with the Star Dust.
void Settlements_StepOntoLadderFromWest(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Settlements_ClimbDownLadder();
}

/// Felix walks west onto the ladder hole beside the palm on Hesperia
/// Settlement's wall top (cell (24, 13)) and climbs down into the room with
/// beds below.
void Settlements_StepOntoLadderFromEast(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_WalkByAndWait(ACTOR_FELIX, -2, -6);
  Actor_WalkByAndWait(ACTOR_FELIX, -14, -8);
  Settlements_ClimbDownLadder();
}

/// Felix walks south onto the ladder hole beside the palm on Hesperia
/// Settlement's wall top (cell (23, 12)), or onto the hole Cyclone uncovered
/// in Gondowan Settlement (cell (5, 16)), turns to face north and climbs down.
void Settlements_TurnAndClimbDownLadder(void) {
  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Settlements_ClimbDownLadder();
}

/// Felix casts Growth on the sprout below Hesperia Settlement's west cliff,
/// and it grows into a vine he can climb (its cell, (3, 15), becomes
/// climbable).
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
  s16 scene = gMainRoutineState.next;

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

#include "shared/smoke_drift.h"

#include "shared/smoke_spawn.h"

/// A task on Kalt Island: a puff from the house's chimney every 64 frames.
void Settlements_PuffChimneySmoke(void) {
  if ((gFrameCount & 63) == 0)
    Smoke_Spawn(toFIXED(248), toFIXED(48), toFIXED(256));
}

/// The apple each frame: it sways up to two pixels either side of its spot and
/// dips up to two pixels below it, at a random pace, as The Broken Seal's Nut
/// floats.
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

/// Sets up Kalt Island: the see-through sprites, the chimney smoke, the rope
/// if it is tied, and, until it is taken, the apple floating in the trees.
void Settlements_SetUpKaltIsland(void) {
  struct OrbitObject *actor;
  struct OrbitSprite *sprite;
  GlyphTransfer *glyph;

  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = TASK_PASS_FRAME;

    FieldTask_AddFrame(Settlements_PuffChimneySmoke, priority);
  }
  Rope_Initialize();
  Rope_Initialize();
  FieldRope_Place(1, FLAG_KALT_ROPE_TIED, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  if (Event_GetFlag(FLAG_KALT_ISLAND_APPLE_TAKEN)) {
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
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gMainRoutineState.next == SCENE_KALT_ISLAND)
    Settlements_SetUpKaltIsland();
  if (gMainRoutineState.next == SCENE_SW_ATTEKA_ISLET) {
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
  if (gMainRoutineState.next == SCENE_HESPERIA_SETTLEMENT) {
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
  if (gMainRoutineState.next == SCENE_GONDOWAN_SETTLEMENT_CAVE)
    FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineState.next == SCENE_HESPERIA_SETTLEMENT_CAVE)
    FieldEffect_StartBlankOverlay(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineState.next == SCENE_SW_ATTEKA_ISLET) {
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
  if (gMainRoutineState.next == SCENE_GONDOWAN_SETTLEMENT) {
    if (Event_GetFlag(FLAG_LADDER_BUSH_CUT)) {
      MapGrid_CopyCellsTo(5, 46, 5, 47, 1, 1);
      MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
      MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
    }
  }
  return 0;
}

/// Nothing visible happens when Felix climbs the ladder up the low cliff on
/// Kalt Island's frozen river's west bank onto cell (5, 12), the ledge at its
/// top; from then on the ice no longer slides him, until he comes back down
/// past cell (5, 13). The ledge is the one the rope's west post stands on, at
/// the river's edge: walking east from the ladder's top Felix stops beside
/// the rope lying coiled on its post.
void Settlements_StopIceSlides(void) { GameFlag_Set(FLAG_ICE_SLIDES_STOPPED); }

/// Nothing visible happens when Felix climbs down onto cell (5, 13) on Kalt
/// Island's frozen river's west bank; the ice slides him again.
void Settlements_ResumeIceSlides(void) {
  GameFlag_Clear(FLAG_ICE_SLIDES_STOPPED);
}

/// Felix steps onto Kalt Island's frozen river and slides along the ice in the
/// way he was walking until he stops against a bank.
void Settlements_StepOnIceSlide(void) { Leader_SlideOnIce(); }

/// Felix walks north into a house's door: the door in the cliff east of
/// Tinder's ledge in Hesperia Settlement (cell (23, 16)), or the door of Kalt
/// Island's house (cell (14, 18)). The people outside stop, the door opens,
/// and he walks in as the screen fades.
void Settlements_EnterHouse(void) {
  u32 i;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    struct Actor *actor = Actor_Get(i);

    if (actor != 0)
      actor->motionFlags = 0;
  }
  if (gMainRoutineState.next == SCENE_HESPERIA_SETTLEMENT)
    MapGrid_CopyCellsTo(33, 30, 23, 14, 1, 2);
  else
    MapGrid_CopyCellsTo(27, 29, 40, 16, 1, 2);
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_CenterAndWalk(gMainRoutineState.leaderActorId, 2, -4);
  FieldEvent_Wait(6);
  // Both houses' doors are their scene's exit 2.
  FieldEvent_SetExitRequest(HESPERIA_HOUSE_DOOR);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Felix casts Lash on the post at the west end of Kalt Island's rope, and
/// the rope is tied across the frozen river to the post on its east bank.
void Settlements_LashRope(void) {
  Rope_Lash(1, ACTOR_ROPE_POST_A, ACTOR_ROPE_POST_B);
  Event_SetFlag(FLAG_KALT_ROPE_TIED);
}

extern s32 gSettlements_WaterColorFrame;

/// A task on SW Atteka Islet: the falls' water runs, its four colours moving
/// along by one every 8 frames.
void Settlements_CycleWaterColors(void) {
  struct FieldState *state = (struct FieldState *)Ram_HeapSlots->fieldState;
  s32 *frame;
  s32 phase, i;

  if (state->psynergy != 0 || state->revealActive != 0)
    return;
  frame = &gSettlements_WaterColorFrame;
  if (*frame == -1) {
    u16 *colors = gSettlements_LoadedWaterColors;
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy(colors, (void *)&BG_PLTT_COLOR(FALLS_PALETTE, 0),
         sizeof(gSettlements_LoadedWaterColors));
    copy(gSettlements_ShownWaterColors, colors,
         sizeof(gSettlements_LoadedWaterColors));
    *frame = 0;
  }
  phase = (*frame / 8) % 4;
  for (i = 0; i < WATER_COLORS; i++)
    gSettlements_ShownWaterColors[FIRST_WATER_COLOR +
                                  (phase + i) % WATER_COLORS] =
        gSettlements_LoadedWaterColors[FIRST_WATER_COLOR + i];
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count < IO_WRITE_QUEUE_SIZE) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)gSettlements_ShownWaterColors;
      q->count = count + 1;
      *entry++ = (u32)&BG_PLTT_COLOR(FALLS_PALETTE, 0);
      *entry = DMA_COPY_WORDS(PALETTE_COLORS / 2);
    }
    IRQ_UNLOCK(saved);
  }
  gSettlements_WaterColorFrame++;
}

/// Starts SW Atteka Islet's falls: the running colours of the water, the sound
/// of water, and the split of the screen that Scroll_HBlank draws.
void Settlements_StartWaterfall(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  FieldTask_AddFrame(Settlements_CycleWaterColors, TASK_PASS_FRAME);
  Scene_QueueMusic(SE_AMBIENT_WATER);
  Scroll_StartHBlank();
}

/// Each line of the screen as it is drawn: above the line Layer_TrackScroll
/// sets, background layer BG1 keeps to the map and is drawn solid; below it,
/// the layer drifts sideways with time and is drawn see-through.
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

/// Each frame: where Scroll_HBlank's line falls on screen, from the third map
/// layer's scroll, and how far BG1 is scrolled above and below it.
void Layer_TrackScroll(void) {
  struct MapLayer *layer =
      &((struct MapState *)Ram_HeapSlots->mapGrid)->layers[2];
  s16 y = FIXEDtoINT(layer->y);

  gSettlements_WaterfallLine = 1448 - y;
  gSettlements_WaterfallScrollAbove = FIXEDtoINT(layer->x);
  gSettlements_WaterfallScrollBelow =
      (s16)FIXEDtoINT(layer->x) - (gFrameCount >> 2);
}

#include "shared/scroll_start_hblank.h"

/// Nothing visible happens as Felix casts Move on SW Atteka Islet's pillar or
/// crate; the game frees the cell it stands on so it can move.
void Settlements_UnblockFallsObjects(void) {
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

/// Nothing visible happens as Felix starts to push SW Atteka Islet's pillar
/// or crate; the game frees the cell it stands on and keeps Felix drawn in
/// front of it.
void Settlements_StartPushingFallsObject(void) {
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_UNDERFOOT;
  Actor_LiftTargetBlock(gSettlements_FallsObjects);
}

void Settlements_SplashIntoRiver(s32 id);

/// The pillar pushed off the grass into SW Atteka Islet's river (cell
/// (11, 12)) sinks with a splash until only its top shows, a post like the
/// others in the water, which the crate can later stop against.
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
    Settlements_SplashIntoRiver(ACTOR_PILLAR);
    Actor_SetPosition(ACTOR_PILLAR, 0, 0);
    Actor_SetPosition(ACTOR_RIVER_STONE_K, 184, 184);
    Actor_GetById(ACTOR_RIVER_STONE_K)->motionFlags = 0;
    Actor_GetById(ACTOR_RIVER_STONE_K)->y.fixed -= toFIXED(64);
    MapGrid_CopyCellAttributes(9, 11, 1, 1, 11, 12);
    Event_SetFlag(FLAG_PILLAR_SUNK);
  }
}

/// Sinks actor 2 pixels a frame for the given frames, counting in i.
#define SINK(frames)                                                           \
  i = (frames);                                                                \
  do {                                                                         \
    FieldEvent_Wait(1);                                                        \
    actor->y.fixed -= toFIXED(2);                                              \
    i--;                                                                       \
  } while (i > 0)

/// The crate pushed into the river above SW Atteka Islet's falls (cell
/// (11, 9)) sinks in with a splash, drifts to the edge and goes down the falls
/// with the camera watching. With the pillar sunk below, it sticks against it
/// and makes the path to the Dragon Skin's chest; without, it is swept away
/// and lost for this visit. Pushed off the grass's south edge at cell (16, 12)
/// instead, it drops down the cliff.
void Settlements_SlideCrateDownFalls(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FALLS_CRATE);
  s32 column = FIELD_CELL(actor->x.fixed);
  s32 row = FIELD_CELL(actor->z.fixed);
  s32 i;
  u8 *flags;

  if (column == 11 && row == 9) {
    actor->motionFlags = 0;
    SINK(8);
    Settlements_SplashIntoRiver(ACTOR_FALLS_CRATE);
    FieldEvent_Wait(20);
    FieldCamera_MoveTo(192, CAMERA_KEEP, 224, 1);
    Actor_SetSpeed(ACTOR_FALLS_CRATE, 0.8);
    Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 8);
    FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
    FieldEvent_Wait(10);
    Actor_FaceDirection(gMainRoutineState.leaderActorId, FACING_SOUTH, 0);
    Actor_SetSpeed(ACTOR_FALLS_CRATE, 2.4);
    Actor_SetDestinationOffset(ACTOR_FALLS_CRATE, 0, 80);
    FieldActor_WaitForMoveById(ACTOR_FALLS_CRATE);
    Settlements_SplashIntoRiver(ACTOR_FALLS_CRATE);
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
  } else if (!Event_GetFlag(FLAG_FALLS_CRATE_DROPPED) && column == 16 &&
             row == 12) {
    MapGrid_CopyCellAttributes(17, 12, 1, 1, column, row);
    flags = &actor->motionFlags;
    *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    FieldEvent_Wait(30);
    *flags = 0;
    Event_SetFlag(FLAG_FALLS_CRATE_DROPPED);
    Event_End();
  }
}

/// Felix moves SW Atteka Islet's pillar with Move; moved into the river, it
/// sinks.
void Settlements_MovePillar(void) {
  Actor_PlaceMovedBlock();
  Settlements_SinkPillar();
}

/// Felix moves SW Atteka Islet's crate with Move; moved into the river above
/// the falls, it goes down them.
void Settlements_MoveCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_SlideCrateDownFalls();
}

/// Felix pushes SW Atteka Islet's pillar or crate one cell: the pillar pushed
/// into the river sinks, and the crate pushed into the river above the falls
/// goes down them.
void Settlements_PushIntoFalls(void) {
  Actor_PlaceMovedBlock();
  Actor_Get(gMainRoutineState.leaderActorId)->priorityFlags -=
      ACTOR_PRIORITY_UNDERFOOT;
  Settlements_SinkPillar();
  Settlements_SlideCrateDownFalls();
}

/// Keeps the cells of Hesperia Settlement's wall before its crate moves.
void Settlements_SaveWallCells(void) {
  MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 0);
  MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 10);
}

/// Gives Hesperia Settlement's wall its kept cells back.
void Settlements_RestoreWallCells(void) {
  MapGrid_CopyCellAttributes(64, 0, 10, 10, 11, 73);
  MapGrid_CopyCellAttributes(64, 10, 10, 10, 42, 73);
}

/// Nothing visible happens when Felix steps on cell (13, 11), on the stairs in
/// the cliff west of Hesperia Settlement's wall; the game swaps in the wall
/// top's cells until he steps on cell (10, 9). Until the crate is pushed off
/// the wall it is done again when he comes back after a battle there.
// The cells swapped in are the overhead layer's (map.txt's rows 73 on are its
// rows 9 on): its closed block over the wall, cells (11, 11) to (20, 14),
// becomes open ground 96 pixels up at (12, 12) to (19, 14), ringed by closed
// cells, as the probe of the room with FLAG_ON_WALL set shows.
// What they let him do shows nowhere on screen: with the swap made (the room
// entered after a battle with FLAG_ON_WALL set, which makes it at entry) and
// without it, Move cast from cell (17, 13) at the crate two cells north of
// him, with South held, pulls it one cell south, and Felix then walks west
// and north past its old place along its west side, alike. A walk from cell
// (12, 9) down the stairs onto (13, 11) and on into the fenced square did not
// run this.
void Settlements_StepOntoWall(void) {
  if (!Event_GetFlag(FLAG_WALL_CRATE_DROPPED) && !Event_GetFlag(FLAG_ON_WALL)) {
    MapGrid_CopyCellAttributes(11, 73, 10, 10, 64, 50);
    MapGrid_CopyCellAttributes(42, 73, 10, 10, 64, 60);
    MapGrid_CopyCellAttributes(42, 11, 10, 4, 11, 75);
    Event_SetFlag(FLAG_ON_WALL);
  }
}

/// Nothing visible happens when Felix steps on cell (10, 9), at the other end
/// of the stairs west of Hesperia Settlement's wall; the wall's own cells come
/// back.
void Settlements_StepOffWall(void) {
  if (Event_GetFlag(FLAG_ON_WALL)) {
    MapGrid_CopyCellAttributes(64, 50, 10, 10, 11, 73);
    MapGrid_CopyCellAttributes(64, 60, 10, 10, 42, 73);
    Event_ClearFlag(FLAG_ON_WALL);
  }
}

/// Nothing visible happens as Felix starts to move or push the crate on
/// Hesperia Settlement's wall; the game frees the cells where it may go, by
/// where it stands.
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

/// Hesperia Settlement's crate pushed off the south edge of the wall drops
/// with the camera following it and lands in a ring of dust: through the
/// leftmost gap onto the step at cell (12, 15), from there east to the ground
/// at cell (13, 15) (the way to Tinder, leaving a step up the wall), at cell
/// (16, 15) the wall's whole height to the ground, or onto Tinder's ledge at
/// cell (19, 15), where Tinder, not yet met, is gone for this visit.
void Settlements_DropCrate(void) {
  struct Actor *actor;
  s32 column;
  s32 row;
  s32 i;

  Event_Start();
  FieldEvent_EndLastingPsynergy(0);
  EventCamera_FollowActor(ACTOR_WALL_CRATE, 1);
  actor = Actor_Get(ACTOR_WALL_CRATE);
  column = FIELD_CELL(actor->x.fixed);
  row = FIELD_CELL(actor->z.fixed);
  if (column == 12 && row == 15) {
    Event_SetFlag(FLAG_WALL_CRATE_DROPPED);
    actor->motionFlags = 0;
    SINK(32);
    Settlements_SpawnDustRing(ACTOR_WALL_CRATE);
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_WALL_CRATE_ON_GROUND) && column == 16 &&
             row == 15) {
    actor->motionFlags = 0;
    Event_SetFlag(FLAG_WALL_CRATE_ON_GROUND);
    Event_SetFlag(FLAG_WALL_CRATE_DROPPED);
    SINK(48);
    Settlements_SpawnDustRing(ACTOR_WALL_CRATE);
    FieldEvent_Wait(30);
  } else if (column == 19 && row == 15) {
    MapGrid_CopyCellAttributes(18, 16, 1, 1, column, row);
    actor->motionFlags = 0;
    Event_SetFlag(FLAG_WALL_CRATE_DROPPED);
    SINK(32);
    Settlements_SpawnDustRing(ACTOR_WALL_CRATE);
    if (!Event_GetFlag(FLAG_DJINNI(DJINNI_TINDER))) {
      Actor_SetPosition(ACTOR_TINDER, 0, 0);
      Actor_SetScript(ACTOR_WALL_CRATE, gSettlements_CrateDropScript);
      FieldEvent_Wait(20);
    }
    FieldEvent_Wait(20);
  } else if (!Event_GetFlag(FLAG_WALL_CRATE_OFF_STEP) && column == 13 &&
             row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
    actor->motionFlags = 0;
    SINK(16);
    Settlements_SpawnDustRing(ACTOR_WALL_CRATE);
    FieldEvent_Wait(20);
    Event_SetFlag(FLAG_WALL_CRATE_OFF_STEP);
  } else if (column == 14 && row == 15) {
    MapGrid_CopyCellAttributes(1, 3, 1, 1, 12, row);
    MapGrid_CopyCellAttributes(1, 3, 1, 1, column, row);
  }
  Event_End();
}

/// Felix has moved or pushed the crate on Hesperia Settlement's wall; pushed
/// over the edge, it drops.
void Settlements_BlockCrate(void) {
  Actor_PlaceMovedBlock();
  Settlements_RestoreWallCells();
  Settlements_DropCrate();
}

extern s32 gSettlements_SmokeScript[];

/// Water splashes up where the pillar or crate goes into the river.
void Settlements_SplashIntoRiver(s32 id) {
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
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

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef SINK
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

/// Puffs a ring of dust out from the crate as it lands.
void Settlements_SpawnDustRing(s32 actorId) {
  struct Actor *actor;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 angleStep;
  s32 angle;
  s32 velocityX;
  FIXED velocityZ;

  actor = Actor_Get(actorId);
  options.update = FieldEffect_AdvanceMotion;
  for (angleStep = 0; angleStep <= TRIG_TURN / FACING_STEP; angleStep += 2) {
    angle = angleStep * FACING_STEP;
    velocity.x = Trig_Cos(angle);
    velocity.y = 0;
    velocityZ = Math_Sin(angle);
    velocityX = velocity.x;
    velocity.z = velocityZ;
    velocityX += velocityX / 3;
    velocity.x = velocityX;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, velocityX,
                      velocity.y, velocityZ,
                      EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
  }
}

/// Felix casts Lift on the round boulder on SW Atteka Islet's grass above the
/// river, and the hands raise it up out of the way, opening the way to the
/// posts in the river and the Jupiter Djinni.
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

/// Stops or restarts Felix's slide on the ice (Leader_SlideOnIce).
void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"

void Settlements_MeetGel(void);
void Settlements_MeetLostJupiterDjinni(void);
void Settlements_StepOntoLadderFromWest(void);
void Settlements_GrowVine(void);
void Settlements_EnterHouse(void);
void Settlements_PushIntoFalls(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The blocks Move pushes, ended by ACTOR_LIST_END: by the falls, a pillar and
/// a crate; by the wall, a crate.
s16 gSettlements_FallsObjects[] = {
    ACTOR_PILLAR, ACTOR_FALLS_CRATE, ACTOR_LIST_END,
};
s16 gSettlements_WallCrate[] = {
    ACTOR_WALL_CRATE, ACTOR_LIST_END,
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

struct SceneRegion gSettlements_HesperiaSettlementHousesRegions[] = {
    {672, 46, 400, 688, 62, 416, CONDITION_ALWAYS, 14},
    {SCENE_TABLE_END},
};

struct SceneRegion gSettlements_GondowanSettlementCaveRegions[] = {
    {212, 12, 132, 220, 20, 140, CONDITION_ALWAYS, 1},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(GondowanSettlement),
    SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GONDOWAN_SETTLEMENT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_GONDOWAN_SETTLEMENT_HOUSES_NORTH, GondowanSettlementHouses, GONDOWAN_SETTLEMENT_HOUSES_FROM_GONDOWAN_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_GONDOWAN_SETTLEMENT_CAVE_A, GondowanSettlementCave, GONDOWAN_SETTLEMENT_CAVE_FROM_GONDOWAN_SETTLEMENT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_TO_GONDOWAN_SETTLEMENT_CAVE_B, GondowanSettlementCave, GONDOWAN_SETTLEMENT_CAVE_FROM_GONDOWAN_SETTLEMENT, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanSettlementHouses),
    SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_HOUSES_TO_GONDOWAN_SETTLEMENT_NORTH, GondowanSettlement, GONDOWAN_SETTLEMENT_FROM_GONDOWAN_SETTLEMENT_HOUSES_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS(GondowanSettlementCave),
    SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_CAVE_TO_GONDOWAN_SETTLEMENT, GondowanSettlement, GONDOWAN_SETTLEMENT_FROM_GONDOWAN_SETTLEMENT_CAVE_A, EXIT_FLAG_NONE),
    // Hesperia Settlement's entrance 4 is where the caves below come up.
    SCENE_EXIT_UNLESS(GONDOWAN_SETTLEMENT_CAVE_TO_HESPERIA_SETTLEMENT, HesperiaSettlement, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(HesperiaSettlement),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_HESPERIA_SETTLEMENT, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_NORTH, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_SOUTH_A, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_SOUTH_B, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_HOUSES_SOUTH_C, HesperiaSettlementHouses, HESPERIA_SETTLEMENT_HOUSES_FROM_HESPERIA_SETTLEMENT_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_TO_HESPERIA_SETTLEMENT_CAVE, HesperiaSettlementCave, HESPERIA_SETTLEMENT_CAVE_FROM_HESPERIA_SETTLEMENT, EXIT_FLAG_NONE),
    SCENE_EXITS(HesperiaSettlementCave),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_CAVE_TO_HESPERIA_SETTLEMENT, HesperiaSettlement, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(HesperiaSettlementHouses),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_HOUSES_TO_HESPERIA_SETTLEMENT_NORTH, HesperiaSettlement, HESPERIA_SETTLEMENT_FROM_HESPERIA_SETTLEMENT_HOUSES_NORTH, EXIT_FLAG_NONE),
    // The stairs between the room with beds and the floor above it.
    SCENE_EXIT_UNLESS(12, HesperiaSettlementHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, HesperiaSettlementHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(HESPERIA_SETTLEMENT_HOUSES_TO_HESPERIA_SETTLEMENT_SOUTH, HesperiaSettlement, HESPERIA_SETTLEMENT_FROM_HESPERIA_SETTLEMENT_HOUSES_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXITS(KaltIsland),
    SCENE_EXIT_UNLESS(KALT_ISLAND_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KALT_ISLAND, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(KALT_ISLAND_TO_KALT_ISLAND_HOUSES, KaltIslandHouses, KALT_ISLAND_HOUSES_FROM_KALT_ISLAND, EXIT_FLAG_NONE),
    SCENE_EXITS(KaltIslandHouses),
    SCENE_EXIT_UNLESS(KALT_ISLAND_HOUSES_TO_KALT_ISLAND, KaltIsland, KALT_ISLAND_FROM_KALT_ISLAND_HOUSES, EXIT_FLAG_NONE),
    SCENE_EXITS(SwAttekaIslet),
    SCENE_EXIT_UNLESS(SW_ATTEKA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SW_ATTEKA_ISLET, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SW_ATTEKA_ISLET_TO_SW_ATTEKA_ISLET_HOUSES_NORTH, SwAttekaIsletHouses, SW_ATTEKA_ISLET_HOUSES_FROM_SW_ATTEKA_ISLET_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(SwAttekaIsletHouses),
    SCENE_EXIT_UNLESS(SW_ATTEKA_ISLET_HOUSES_TO_SW_ATTEKA_ISLET_NORTH, SwAttekaIslet, SW_ATTEKA_ISLET_FROM_SW_ATTEKA_ISLET_HOUSES_NORTH, EXIT_FLAG_NONE),
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
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 56, 0, 240, FACING_NORTH, TALK_FACING_KEEP, 0),
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
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 88, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), GONDOWAN_LADDER_HOLE_NORTH,
     CONDITION_ALWAYS, EVENT(Settlements_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), GONDOWAN_LADDER_HOLE_WEST,
     CONDITION_ALWAYS, EVENT(Settlements_StepOntoLadderFromWest)},
    {TRIGGER_TALK, ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK, CONDITION_ALWAYS,
     EVENT(Settlements_WarnOfRiddleOfRock)},
    {TRIGGER_TALK, ACTOR_GONDOWAN_BOY, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsThereWasGiantExplosion)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_RIDDLE_OF_THE_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgSettlementsMountainIsCalledMagmaRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GONDOWAN_BOY, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsExplosionsWithinMagmaRock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     GONDOWAN_BUSH, CONDITION_ALWAYS, EVENT(Settlements_CutBushes)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     GONDOWAN_LADDER_BUSH, CONDITION_ALWAYS, EVENT(Settlements_CutLadderBush)},
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
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), HESPERIA_HOUSE_DOOR,
     CONDITION_ALWAYS, EVENT(Settlements_EnterHouse)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), HESPERIA_LADDER_HOLE_NORTH,
     CONDITION_ALWAYS, EVENT(Settlements_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), HESPERIA_LADDER_HOLE_WEST,
     CONDITION_ALWAYS, EVENT(Settlements_StepOntoLadderFromWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), HESPERIA_LADDER_HOLE_EAST,
     CONDITION_ALWAYS, EVENT(Settlements_StepOntoLadderFromEast)},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_NORTH, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, HESPERIA_ONTO_WALL, FLAG_ON_WALL,
     EVENT(Settlements_StepOntoWall)},
    {TRIGGER_STEP, HESPERIA_OFF_WALL, IF_FLAG_SET(FLAG_ON_WALL),
     EVENT(Settlements_StepOffWall)},
    {TRIGGER_TALK, ACTOR_HESPERIA_MAN, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsYouMustBeVeryBrave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HESPERIA_MAN, CONDITION_ALWAYS,
     MSGNO(MsgSettlementsForContinentAsDesolate)},
    {TRIGGER_TALK, ACTOR_TINDER, CONDITION_ALWAYS,
     EVENT(Settlements_MeetTinder)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WALL_CRATE, CONDITION_ALWAYS, EVENT(Settlements_UnblockCrate)},
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
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), KALT_HOUSE_DOOR, CONDITION_ALWAYS,
     EVENT(Settlements_EnterHouse)},
    {TRIGGER_STEP, KALT_ICE_SLIDES_OFF, CONDITION_ALWAYS,
     EVENT(Settlements_StopIceSlides)},
    {TRIGGER_STEP, KALT_ICE_SLIDES_ON, CONDITION_ALWAYS,
     EVENT(Settlements_ResumeIceSlides)},
    {TRIGGER_STEP, KALT_ICE, FLAG_ICE_SLIDES_STOPPED,
     EVENT(Settlements_StepOnIceSlide)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST_A, FLAG_KALT_ROPE_TIED,
     EVENT(Settlements_LashRope)},
    {TRIGGER_TALK, ACTOR_GEL, CONDITION_ALWAYS, EVENT(Settlements_MeetGel)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_APPLE,
     FLAG_KALT_ISLAND_APPLE_TAKEN, EVENT(Settlements_CatchApple)},
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_PILLAR, FLAG_PILLAR_SUNK, EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_PILLAR_SUNK,
     EVENT(Settlements_MovePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FALLS_CRATE, FLAG_FALLS_CRATE_AT_REST,
     EVENT(Settlements_UnblockFallsObjects)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FALLS_CRATE,
     FLAG_FALLS_CRATE_AT_REST, EVENT(Settlements_MoveCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(Settlements_StartPushingFallsObject)},
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

/// The frames the falls' water colours have run; -1 before they start.
s32 gSettlements_WaterColorFrame = -1;

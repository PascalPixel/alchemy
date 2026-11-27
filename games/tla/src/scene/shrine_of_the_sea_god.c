// The Shrine of the Sea God east of Daila, in eleven rooms that share this overlay
// (SceneIdShrineOfTheSeaGodRope to SceneIdShrineOfTheSeaGodRightProng).
//
// On the first visit Tavi is stuck on a ledge in the entrance (the rope room) and Riki cannot throw
// him the rope; Riki goes off to the side room by the plank bridge, and Lash ties the rope so Tavi
// climbs down. Past the crossroads, where the Djinni Breath is first seen, its footprints cross the
// room below and it is chased round a ledge (Breath's room) until the torch pushed against the
// north wall traps it. On the second visit, with Frost on the puddles, Move on the pillar over the
// hole, Lash, and the broken bridges whose planks drop the party into the stream, the party hops
// across the stones in the water, Reveal showing the seventh in the middle of the hexagon of rocks,
// to the island. The Sea God's Tear used on its crystal statue raises the water to the statue room,
// and three ladders more lead up to the Right Prong.
#include "item_ids.h"
#include "macros.h"
#include "field/actor_script.h"
#include "field/call.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "scene/ground_effects.h"
#include "scene/scene.h"
#include "scene/shrine_of_the_sea_god.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"
#define EFFECT_SET_MODE   fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define FLAG_CROSSROADS_PILLAR_MOVED     0x84c // Move pushed the crossroads' pillar
#define FLAG_RIKI_GAVE_UP                0x872 // Riki missed with the rope, went off
// the Djinni ran east in the crossroads
#define FLAG_DJINNI_FLED_EAST            0x874
#define FLAG_DJINNI_FLED_SOUTH           0x875 // and ran on, out of it
#define FLAG_DJINNI_RAN_PAST             0x876 // its footprints are gone
#define FLAG_DJINNI_BY_LEDGE             0x877 // it waits by the ledge
#define FLAG_DJINNI_ON_LEDGE             0x87c // it hopped up onto the ledge
// Frost froze the puddle at (8, 18).
#define FLAG_SHRINE_FIRST_PUDDLE_FROZEN  FLAG_SCENE_TEMP_00
// Frost froze the puddle at (10, 18).
#define FLAG_SHRINE_SECOND_PUDDLE_FROZEN FLAG_SCENE_TEMP_01
// Frost froze the puddle at (10, 17).
#define FLAG_SHRINE_THIRD_PUDDLE_FROZEN  FLAG_SCENE_TEMP_02
// Lash tied the rope room's rope.
#define FLAG_SHRINE_ROPE_TIED            FLAG_SCENE_TEMP_03
// Lash tied the north rope.
#define FLAG_SHRINE_SECOND_ROPE_TIED     FLAG_SCENE_TEMP_04
// Breath is caught, or its battle begun.
#define FLAG_DJINNI_CHASE_OVER           FLAG_SCENE_TEMP_05
// The torch has stopped Breath on the north side.
#define FLAG_DJINNI_TRAPPED              FLAG_SCENE_TEMP_06
// The torch stands where it traps Breath.
#define FLAG_TORCH_AT_NORTH_WALL         FLAG_SCENE_TEMP_07
// The hole room's unseen blocks are drawn.
#define FLAG_BLOCKS_SHOWN                FLAG_SCENE_TEMP_08
// Felix is drawn in front of the crossroads' bridges.
#define FLAG_LEADER_IN_FRONT             FLAG_SCENE_TEMP_0A
// A broken bridge's middle has fallen into the stream below.
#define FLAG_CROSSROADS_BRIDGE_BROKEN    FLAG_SCENE_TEMP_30
#define FLAG_FIRST_BRIDGE_BROKEN         FLAG_SCENE_TEMP_31
#define FLAG_SECOND_BRIDGE_BROKEN        FLAG_SCENE_TEMP_32
#define FLAG_PILLAR_IN_HOLE              FLAG_AREA_TEMP_01
// Set as the battle with Breath starts, with where it was trapped, so the scene puts it back there
// once it is beaten.
#define FLAG_FIGHTING_BREATH             FLAG_AREA_TEMP_40
#define FLAG_BREATH_TRAPPED_EAST         FLAG_AREA_TEMP_41
#define FLAG_BREATH_TRAPPED_WEST         FLAG_AREA_TEMP_42

void scShrineOfTheSeaGodSetBlocksShown(Sint32 show);
void scShrineOfTheSeaGodKeepWithLeader(ACTOR *actor);

// The rope room's actors (SceneIdShrineOfTheSeaGodRope), numbering the rows of
// ShrineOfTheSeaGodRopeActorPlacements.
typedef enum SHRINE_OF_THE_SEA_GOD_ROPE_ROOM_ACTOR_ID
{
    // Two logs floating on the room's water.
    ACTOR_ROPE_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
    ACTOR_ROPE_ROOM_SECOND_LOG,
    // The posts of the rope Riki cannot throw up to Tavi: his own, the rope wound round it, which
    // Lash ties, and the one on Tavi's ledge.
    ACTOR_ROPE_ROOM_EAST_POST,
    ACTOR_ROPE_ROOM_WEST_POST,
    ACTOR_RIKI,
    // Tavi, stuck on the ledge.
    ACTOR_TAVI,
    ACTOR_ROPE_ROOM_KRADEN
} SHRINE_OF_THE_SEA_GOD_ROPE_ROOM_ACTOR_ID;

// The torch Move pushes, the first row of the side room (SceneIdShrineOfTheSeaGodPlankBridge) and
// of the ledge room (SceneIdShrineOfTheSeaGodBreath): a pedestal whose flame is its own animation.
// The second row is an unseen actor kept where the flame burns
// (scShrineOfTheSeaGodKeepFlameOnTorch).
typedef enum SHRINE_OF_THE_SEA_GOD_TORCH_ACTOR_ID
{
    ACTOR_TORCH = ACTOR_FIRST_PLACED,
    ACTOR_TORCH_FLAME_MARKER
} SHRINE_OF_THE_SEA_GOD_TORCH_ACTOR_ID;

// The rest of the side room's actors.
typedef enum SHRINE_OF_THE_SEA_GOD_SIDE_ROOM_ACTOR_ID
{
    // The plank bridge over the stream.
    ACTOR_SIDE_ROOM_BRIDGE = ACTOR_TORCH_FLAME_MARKER + 1,
    // Riki, who has come here looking for something to help Tavi with.
    ACTOR_SIDE_ROOM_RIKI,
    // The actor after Riki, which the room disables though no row places it.
    ACTOR_SIDE_ROOM_DISABLED
} SHRINE_OF_THE_SEA_GOD_SIDE_ROOM_ACTOR_ID;

// The crossroads' actors (SceneIdShrineOfTheSeaGodCrossroads), numbering the rows of
// ShrineOfTheSeaGodCrossroadsActorPlacements.
typedef enum SHRINE_OF_THE_SEA_GOD_CROSSROADS_ACTOR_ID
{
    // The stone pillar Move pushes aside.
    ACTOR_CROSSROADS_PILLAR = ACTOR_FIRST_PLACED,
    // The room's bridges, the first two logs side by side, drawn in front of or behind the leader
    // by the level it stands on (scShrineOfTheSeaGodLayerCrossroadsBridges).
    ACTOR_CROSSROADS_LOG_BRIDGE,
    ACTOR_CROSSROADS_FIRST_BRIDGE,
    ACTOR_CROSSROADS_SECOND_BRIDGE,
    ACTOR_CROSSROADS_THIRD_BRIDGE,
    // A broken bridge: its two ends on their legs, and its middle, which drops into the water below
    // under the leader.
    ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS,
    ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE,
    // Breath, the Jupiter Djinni, first seen here.
    ACTOR_CROSSROADS_BREATH
} SHRINE_OF_THE_SEA_GOD_CROSSROADS_ACTOR_ID;

// The room below's actors (SceneIdShrineOfTheSeaGodFootprints): Breath's footprints, sixteen of
// them, left and right feet in turn.
typedef enum SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_ACTOR_ID
{
    ACTOR_FIRST_FOOTPRINT = ACTOR_FIRST_PLACED,
    ACTOR_LAST_FOOTPRINT  = ACTOR_FIRST_FOOTPRINT + 15
} SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_ACTOR_ID;

// The rest of the ledge room's actors (SceneIdShrineOfTheSeaGodBreath).
typedef enum SHRINE_OF_THE_SEA_GOD_LEDGE_ROOM_ACTOR_ID
{
    // The puddles Frost freezes into pillars of ice.
    ACTOR_FIRST_PUDDLE = ACTOR_TORCH_FLAME_MARKER + 1,
    ACTOR_SECOND_PUDDLE,
    ACTOR_THIRD_PUDDLE,
    // The plank bridge over the gap south of the ledge.
    ACTOR_LEDGE_ROOM_BRIDGE,
    // Breath, chased round the ledge until it is trapped.
    ACTOR_BREATH
} SHRINE_OF_THE_SEA_GOD_LEDGE_ROOM_ACTOR_ID;

// The hole room's actors (SceneIdShrineOfTheSeaGodPillarHole).
typedef enum SHRINE_OF_THE_SEA_GOD_HOLE_ROOM_ACTOR_ID
{
    // The pillar Move pushes into the hole.
    ACTOR_HOLE_ROOM_PILLAR = ACTOR_FIRST_PLACED,
    // Three unseen blocks the room draws or not together (scShrineOfTheSeaGodSetBlocksShown).
    ACTOR_HOLE_ROOM_FIRST_BLOCK,
    ACTOR_HOLE_ROOM_SECOND_BLOCK,
    ACTOR_HOLE_ROOM_THIRD_BLOCK
} SHRINE_OF_THE_SEA_GOD_HOLE_ROOM_ACTOR_ID;

// The pillar room's actors (SceneIdShrineOfTheSeaGodPillars), numbering the rows of
// ShrineOfTheSeaGodPillarsActorPlacements.
typedef enum SHRINE_OF_THE_SEA_GOD_PILLAR_ROOM_ACTOR_ID
{
    // The plank bridges between the rock pillars, and a bridge of two logs.
    ACTOR_PILLAR_ROOM_FIRST_BRIDGE = ACTOR_FIRST_PLACED,
    ACTOR_PILLAR_ROOM_SECOND_BRIDGE,
    ACTOR_PILLAR_ROOM_THIRD_BRIDGE,
    ACTOR_PILLAR_ROOM_FOURTH_BRIDGE,
    ACTOR_PILLAR_ROOM_LOG_BRIDGE,
    // The two broken bridges' ends, then their middles, which drop into the stream under the leader
    // and carry him down it.
    ACTOR_FIRST_BROKEN_BRIDGE_ENDS,
    ACTOR_SECOND_BROKEN_BRIDGE_ENDS,
    ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE,
    ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE,
    // The posts of the room's two ropes; Lash ties each by the post its rope is wound round, the
    // south rope's east one and the north rope's west one.
    ACTOR_SOUTH_ROPE_EAST_POST,
    ACTOR_SOUTH_ROPE_WEST_POST,
    ACTOR_NORTH_ROPE_WEST_POST,
    ACTOR_NORTH_ROPE_EAST_POST,
    // The splashes each bridge's middle throws up as it lands, two apiece.
    ACTOR_FIRST_BRIDGE_SPLASH,
    ACTOR_FIRST_BRIDGE_OTHER_SPLASH,
    ACTOR_SECOND_BRIDGE_SPLASH,
    ACTOR_SECOND_BRIDGE_OTHER_SPLASH,
    // In a render of the room with the leader on its ladder, under the chest, taking each marker
    // away changes nothing on screen. Unseen markers behind the chest at the top of the room's
    // ladder; talking to one opens the field menu, as talking to nothing does.
    ACTOR_CHEST_NORTHWEST_MARKER,
    ACTOR_CHEST_NORTHEAST_MARKER,
    ACTOR_CHEST_NORTH_MARKER
} SHRINE_OF_THE_SEA_GOD_PILLAR_ROOM_ACTOR_ID;

// The island room's actors (SceneIdShrineOfTheSeaGodSeaGodsTear): three logs floating on the water
// the Sea God's Tear raises.
typedef enum SHRINE_OF_THE_SEA_GOD_ISLAND_ACTOR_ID
{
    ACTOR_ISLAND_FIRST_LOG = ACTOR_FIRST_PLACED,
    ACTOR_ISLAND_SECOND_LOG,
    ACTOR_ISLAND_THIRD_LOG
} SHRINE_OF_THE_SEA_GOD_ISLAND_ACTOR_ID;

// Where Breath stands on the ledge, kept in its ACTOR.sceneCount: at a corner, or stopped beside
// the torch on the north side, facing it.
typedef enum SHRINE_OF_THE_SEA_GOD_BREATH_PLACE
{
    BREATH_AT_SOUTHEAST_CORNER,
    BREATH_AT_NORTHEAST_CORNER,
    BREATH_AT_NORTHWEST_CORNER,
    BREATH_AT_SOUTHWEST_CORNER,
    BREATH_STOPPED_EAST_OF_TORCH,
    BREATH_STOPPED_WEST_OF_TORCH
} SHRINE_OF_THE_SEA_GOD_BREATH_PLACE;

// The ledge room's step cues round the ledge, each sending Breath on
// (scShrineOfTheSeaGodChaseDjinni): the ways up at its south-east and south-west corners, and two
// places on each of its other sides.
typedef enum SHRINE_OF_THE_SEA_GOD_LEDGE_STEP
{
    STEP_LEDGE_SOUTHEAST_WAY_UP = 13,
    STEP_LEDGE_EAST_SIDE_SOUTH,
    STEP_LEDGE_EAST_SIDE_NORTH,
    STEP_LEDGE_NORTH_SIDE_EAST,
    STEP_LEDGE_NORTH_SIDE_WEST,
    STEP_LEDGE_WEST_SIDE_NORTH,
    STEP_LEDGE_WEST_SIDE_SOUTH,
    STEP_LEDGE_SOUTHWEST_WAY_UP
} SHRINE_OF_THE_SEA_GOD_LEDGE_STEP;

// The metatiles that run the other rooms' scripts, each room numbering its own after the numbers of
// its exits.
typedef enum SHRINE_OF_THE_SEA_GOD_METATILE
{
    // The rope room's metatiles (14, 20) to (18, 20) and (18, 21), where Riki tries the rope again.
    RIKI_THROWS_ROPE_METATILE              = 10,
    // The crossroads' metatiles at the ends of its bridges and by its north doorways: (15, 12),
    // (14, 16) to (16, 16), (23, 13), (23, 20), (34, 13) to (34, 15) and (35, 20) to (35, 22).
    CROSSROADS_BRIDGE_END_METATILE         = 10,
    // The foot of the crossroads' ladder below the bridges, metatile (29, 22).
    CROSSROADS_LADDER_FOOT_METATILE        = 11,
    // Where Breath, seen again east of the bridges, flees south.
    CROSSROADS_BREATH_FLEES_SOUTH_METATILE = 12,
    // The crossroads' broken bridge.
    CROSSROADS_BROKEN_BRIDGE_METATILE      = 15,
    // The ledge room's stairs up from the south, where Breath runs north under the bridge.
    LEDGE_ROOM_SOUTH_STAIRS_METATILE       = 10,
    // South of the ledge, metatiles (19, 34) to (21, 34), where Breath runs west past the leader.
    LEDGE_ROOM_SOUTH_PATH_METATILE         = 11,
    // The path up toward the ledge's south-west corner, metatiles (15, 20) to (18, 23), where
    // Breath hops onto the ledge.
    LEDGE_ROOM_WAY_UP_METATILE             = 12,
    // The pillar room's first broken bridge, metatile (20, 13).
    FIRST_BROKEN_BRIDGE_METATILE           = 5,
    // Its second broken bridge, metatile (25, 22).
    SECOND_BROKEN_BRIDGE_METATILE          = 6,
    // Before the crystal statue on the island, metatile (15, 9).
    ISLAND_STATUE_METATILE                 = 10,
    // Before the crystal statue in the room above the island, metatile (21, 15).
    STATUE_ROOM_STATUE_METATILE            = 1
} SHRINE_OF_THE_SEA_GOD_METATILE;

// Talking to Breath from up on the ledge, once the torch has stopped it, starts its battle.
void scShrineOfTheSeaGodFightBreath(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    Uint8 *place  = &fdActorGetById(ACTOR_BREATH)->sceneCount;
    FIXED  y;

    if ((Uint8)(*place - BREATH_STOPPED_EAST_OF_TORCH)
        > BREATH_STOPPED_WEST_OF_TORCH - BREATH_STOPPED_EAST_OF_TORCH)
        return;
    y = leader->y.fixed;
    if (y > toFIXED(12))
    {
        syGameFlagSet(FLAG_FIGHTING_BREATH);
        if (*place == BREATH_STOPPED_EAST_OF_TORCH)
            syGameFlagSet(FLAG_BREATH_TRAPPED_EAST);
        else if (*place == BREATH_STOPPED_WEST_OF_TORCH)
            syGameFlagSet(FLAG_BREATH_TRAPPED_WEST);
        fdRequestDjinnBattle(ACTOR_BREATH, DJINNI_BREATH);
    }
}

extern SCENE_REGION   ShrineOfTheSeaGodStatueExitRegions[];
extern SCENE_REGION   ShrineOfTheSeaGodLaddersExitRegions[];
extern SCENE_REGION   ShrineOfTheSeaGodRightProngExitRegions[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SceneEntrances;
}

// The regions of the rooms whose exits are rectangles: the statue room, the ladders and the Right
// Prong's room.
const SCENE_REGION *scGetExitRegions(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_STATUE)
        return ShrineOfTheSeaGodStatueExitRegions;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_LADDERS)
        return ShrineOfTheSeaGodLaddersExitRegions;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG)
        return ShrineOfTheSeaGodRightProngExitRegions;
    return 0;
}

extern ACTOR_PLACE ShrineOfTheSeaGodPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodRopeActorPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodPlankBridgeActorPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodCrossroadsActorPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodFootprintsActorPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodBreathActorPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodPillarHoleActorPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodPillarsActorPlacements[];
extern ACTOR_PLACE ShrineOfTheSeaGodSeaGodsTearActorPlacements[];
extern CUE         ShrineOfTheSeaGodCues[];
extern CUE         ShrineOfTheSeaGodRopeCues[];
extern CUE         ShrineOfTheSeaGodPlankBridgeCues[];
extern CUE         ShrineOfTheSeaGodCrossroadsCues[];
extern CUE         ShrineOfTheSeaGodFootprintsCues[];
extern CUE         ShrineOfTheSeaGodBreathCues[];
extern CUE         ShrineOfTheSeaGodPillarHoleCues[];
extern CUE         ShrineOfTheSeaGodPillarsCues[];
extern CUE         ShrineOfTheSeaGodSeaGodsTearCues[];
extern CUE         ShrineOfTheSeaGodStatueCues[];

const Uint32      *scGetExits(void)
{
    return SceneExits;
}

// The current room's actors.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_ROPE)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodRopeActorPlacements;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodPlankBridgeActorPlacements;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodCrossroadsActorPlacements;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodFootprintsActorPlacements;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_BREATH)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodBreathActorPlacements;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodPillarHoleActorPlacements;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodPillarsActorPlacements;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR)
        return (const ACTOR_PLACE *)ShrineOfTheSeaGodSeaGodsTearActorPlacements;
    return (const ACTOR_PLACE *)ShrineOfTheSeaGodPlacements;
}

// The current room's cues.
const CUE *scGetCues(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_ROPE)
        return ShrineOfTheSeaGodRopeCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE)
        return ShrineOfTheSeaGodPlankBridgeCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS)
        return ShrineOfTheSeaGodCrossroadsCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS)
        return ShrineOfTheSeaGodFootprintsCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_BREATH)
        return ShrineOfTheSeaGodBreathCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
        return ShrineOfTheSeaGodPillarHoleCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS)
        return ShrineOfTheSeaGodPillarsCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR)
        return ShrineOfTheSeaGodSeaGodsTearCues;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_STATUE)
        return ShrineOfTheSeaGodStatueCues;
    return ShrineOfTheSeaGodCues;
}

extern Uint32 ShrineOfTheSeaGodRikiLooksAround[];

// Examining the crystal statue, on the island or in the room above it: it looks like the one in
// Daila's Sanctum.
void          scShrineOfTheSeaGodExamineStatue(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    // "This statue looks a lot like the one in Daila's Sanctum."
    mnFieldMessageShow((Sint32)MsgFieldStatueLikeDailaShrine, 1);
    state->fieldMenu = 1;
    fdScriptEnd();
}

// Talking to Riki from behind in the side room startles him; he is busy trying to help Tavi.
void scShrineOfTheSeaGodStartleRiki(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorStop(ACTOR_SIDE_ROOM_RIKI);
    scFieldActorShowEmote(ACTOR_SIDE_ROOM_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    fdActorFaceActorById(ACTOR_SIDE_ROOM_RIKI, MainRoutineStateWork.leaderActorId, 0);
    fdScriptSetMessageId((Sint32)MsgFieldStartledCompanion);
    // "Wah! You scared me!"
    fdScriptShowMessageAndWait(ACTOR_SIDE_ROOM_RIKI, 0);
    // "I thought you were one of the grown-ups from Daila... So, who are you?"
    fdScriptShowMessageAndWait(ACTOR_SIDE_ROOM_RIKI, 0);
    fdActorSetAnimationById(ACTOR_SIDE_ROOM_RIKI, 4);
    // "Look, I'm kinda busy trying to help my pal Tavi right now, OK?"
    fdScriptShowMessageThenWait(ACTOR_SIDE_ROOM_RIKI, 0, 20);
    fdActorSetMovementTypeById(ACTOR_SIDE_ROOM_RIKI, ShrineOfTheSeaGodRikiLooksAround);
    fdScriptEnd();
}

// Talking to an unseen marker behind the pillar room's chest opens the field menu, as talking to
// nothing does.
void scShrineOfTheSeaGodOpenFieldMenu(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;

    state->fieldMenu   = 1;
}

// The island room's water rises a step, lifting Felix, the camera and the floating logs with it.
void scShrineOfTheSeaGodLiftWithWater(void)
{
    MAP_STATE *map        = RamHeapSlots->mapGrid;
    MAP_LAYER *water      = &map->layers[0];
    MAP_LAYER *ground     = &map->layers[1];
    MAP_LAYER *groundTop  = &map->layers[2];
    ACTOR     *leader     = fdActorGetById(MainRoutineStateWork.leaderActorId);
    ACTOR     *camera     = fdCameraGetActor();
    ACTOR     *firstLog   = fdActorGetById(ACTOR_ISLAND_FIRST_LOG);
    ACTOR     *secondLog  = fdActorGetById(ACTOR_ISLAND_SECOND_LOG);
    ACTOR     *thirdLog   = fdActorGetById(ACTOR_ISLAND_THIRD_LOG);

    water->offsetY       -= toFIXED(1) / 2;
    ground->offsetY      += toFIXED(1) / 2;
    groundTop->offsetY   += toFIXED(1) / 2;
    leader->z.fixed      -= toFIXED(1) / 2;
    camera->y.fixed      -= toFIXED(1) / 2;
    firstLog->z.fixed    -= toFIXED(1) / 2;
    secondLog->z.fixed   -= toFIXED(1) / 2;
    thirdLog->z.fixed    -= toFIXED(1) / 2;
    sySynch(4);
}

// The water's layer moves a pixel as the water rises, then waits four ticks.
void scShrineOfTheSeaGodRaiseWaterLayer(void)
{
    MAP_STATE *map    = RamHeapSlots->mapGrid;
    MAP_LAYER *layer  = &map->layers[0];

    layer->offsetY   += -toFIXED(1);
    sySynch(4);
}

// Copies the next band of the risen water's metatiles, 6 by 56 from (x, y) to (18, 6), then moves
// the water and ground layers two pixels over 16 ticks.
void scShrineOfTheSeaGodRaiseWaterBand(Sint32 x, Sint32 y)
{
    MAP_STATE *map    = RamHeapSlots->mapGrid;
    MAP_LAYER *water  = &map->layers[0];
    MAP_LAYER *ground = &map->layers[1];
    Uint8      i;

    fdMapGridCopyMetatilesTo(x, y, 6, 56, 18, 6);
    for (i = 0; i < 16; i++)
    {
        water->offsetY  -= toFIXED(1) / 8;
        ground->offsetY -= toFIXED(1) / 8;
        sySynch(1);
    }
}

// After its bands, the island room's water rises a pixel a step for this many steps, then lifts the
// party half a pixel a step for as many again.
#define WATER_RISE_STEPS 24

// Using the Sea God's Tear on the crystal statue on the island: the room rumbles, the water rises
// round the island over the stepping stones, and it carries Felix up to the room above
// (scShrineOfTheSeaGodRiseOnWater).
void scShrineOfTheSeaGodUseSeaGodsTear(void)
{
    Uint32 i;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdBlendScriptSuspend();
    fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
    fdCameraMoveWithinMapBounds(toFIXED(248), -1, toFIXED(168), 1);
    fdCameraWaitForArrival();
    sySoundPlay(SE_SANDSTORM);
    syIOWrite16(BLEND_ALPHA(1, 16), &REG_BLDALPHA);
    sySynch(10);
    syIOWrite16(BLEND_ALPHA(2, 16), &REG_BLDALPHA);
    sySynch(10);
    syIOWrite16(BLEND_ALPHA(3, 16), &REG_BLDALPHA);
    sySynch(10);
    syIOWrite16(BLEND_ALPHA(4, 16), &REG_BLDALPHA);
    sySynch(10);
    fdMapGridSetShakeFX(0, toFIXED(1), toFIXED(1));
    fdMapGridCopyMetatilesTo(59, 124, 9, 12, 12, 3);
    fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlags = 0;
    fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 1);
    fdActorSetSpritePriority(ACTOR_ISLAND_FIRST_LOG, 1);
    fdActorSetSpritePriority(ACTOR_ISLAND_SECOND_LOG, 1);
    fdActorSetSpritePriority(ACTOR_ISLAND_THIRD_LOG, 1);
    fdMapGridCopyMetatilesTo(46, 51, 6, 56, 18, 25);
    sySynch(10);
    fdMapGridSetShakeFX(toFIXED(1), toFIXED(1), toFIXED(1));
    scShrineOfTheSeaGodRaiseWaterBand(65, 51);
    scShrineOfTheSeaGodRaiseWaterBand(65, 58);
    syIOWrite16(BLEND_ALPHA(5, 15), &REG_BLDALPHA);
    scShrineOfTheSeaGodRaiseWaterBand(65, 65);
    scShrineOfTheSeaGodRaiseWaterBand(65, 72);
    syIOWrite16(BLEND_ALPHA(6, 14), &REG_BLDALPHA);
    scShrineOfTheSeaGodRaiseWaterBand(65, 79);
    scShrineOfTheSeaGodRaiseWaterBand(65, 86);
    syIOWrite16(BLEND_ALPHA(7, 13), &REG_BLDALPHA);
    scShrineOfTheSeaGodRaiseWaterBand(65, 93);
    scShrineOfTheSeaGodRaiseWaterBand(65, 100);
    syIOWrite16(BLEND_ALPHA(8, 12), &REG_BLDALPHA);
    scShrineOfTheSeaGodRaiseWaterBand(84, 51);
    scShrineOfTheSeaGodRaiseWaterBand(84, 58);
    syIOWrite16(BLEND_ALPHA(9, 11), &REG_BLDALPHA);
    scShrineOfTheSeaGodRaiseWaterBand(84, 65);
    scShrineOfTheSeaGodRaiseWaterBand(84, 72);
    syIOWrite16(BLEND_ALPHA(10, 10), &REG_BLDALPHA);
    scShrineOfTheSeaGodRaiseWaterBand(84, 79);
    scShrineOfTheSeaGodRaiseWaterBand(84, 86);
    syIOWrite16(BLEND_ALPHA(11, 9), &REG_BLDALPHA);
    scShrineOfTheSeaGodRaiseWaterBand(84, 93);
    scShrineOfTheSeaGodRaiseWaterBand(84, 100);
    syIOWrite16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
    for (i = 0; i < WATER_RISE_STEPS; i++)
        scShrineOfTheSeaGodRaiseWaterLayer();
    for (i = 0; i < WATER_RISE_STEPS; i++)
        scShrineOfTheSeaGodLiftWithWater();
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptSetExitRequest(SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_STATUE);
}

// The island room's water falls a step, lowering Felix, the camera and the floating logs with it.
void scShrineOfTheSeaGodLowerWithWater(void)
{
    MAP_STATE *map        = RamHeapSlots->mapGrid;
    MAP_LAYER *water      = &map->layers[0];
    MAP_LAYER *ground     = &map->layers[1];
    MAP_LAYER *groundTop  = &map->layers[2];
    ACTOR     *leader     = fdActorGetById(MainRoutineStateWork.leaderActorId);
    ACTOR     *camera     = fdCameraGetActor();
    ACTOR     *firstLog   = fdActorGetById(ACTOR_ISLAND_FIRST_LOG);
    ACTOR     *secondLog  = fdActorGetById(ACTOR_ISLAND_SECOND_LOG);
    ACTOR     *thirdLog   = fdActorGetById(ACTOR_ISLAND_THIRD_LOG);

    water->offsetY       += toFIXED(1) / 2;
    ground->offsetY      -= toFIXED(1) / 2;
    groundTop->offsetY   -= toFIXED(1) / 2;
    leader->z.fixed      += toFIXED(1) / 2;
    camera->y.fixed      += toFIXED(1) / 2;
    firstLog->z.fixed    += toFIXED(1) / 2;
    secondLog->z.fixed   += toFIXED(1) / 2;
    thirdLog->z.fixed    += toFIXED(1) / 2;
    sySynch(4);
}

// The water's layer moves a pixel as the water falls, then waits four ticks.
void scShrineOfTheSeaGodLowerWaterLayer(void)
{
    MAP_STATE *map    = RamHeapSlots->mapGrid;
    MAP_LAYER *layer  = &map->layers[0];

    layer->offsetY   += toFIXED(1);
    sySynch(4);
}

// Copies the next band of the falling water's metatiles, 6 by 56 from (x, y) to (18, 6), then moves
// the water and ground layers back two pixels over 16 ticks.
void scShrineOfTheSeaGodLowerWaterBand(Sint32 x, Sint32 y)
{
    MAP_STATE *map    = RamHeapSlots->mapGrid;
    MAP_LAYER *water  = &map->layers[0];
    MAP_LAYER *ground = &map->layers[1];
    Uint8      i;

    fdMapGridCopyMetatilesTo(x, y, 6, 56, 18, 6);
    for (i = 0; i < 16; i++)
    {
        water->offsetY  += toFIXED(1) / 8;
        ground->offsetY += toFIXED(1) / 8;
        sySynch(1);
    }
}

// Back on the island after using the Tear in the room above, the water drains away round Felix
// until the stepping stones show again.
void scShrineOfTheSeaGodLowerWater(void)
{
    MAP_STATE *map       = RamHeapSlots->mapGrid;
    MAP_LAYER *water     = &map->layers[0];
    MAP_LAYER *ground    = &map->layers[1];
    MAP_LAYER *groundTop = &map->layers[2];
    ACTOR     *leader    = fdActorGetById(MainRoutineStateWork.leaderActorId);
    ACTOR     *camera    = fdCameraGetActor();
    ACTOR     *firstLog  = fdActorGetById(ACTOR_ISLAND_FIRST_LOG);
    ACTOR     *secondLog = fdActorGetById(ACTOR_ISLAND_SECOND_LOG);
    ACTOR     *thirdLog  = fdActorGetById(ACTOR_ISLAND_THIRD_LOG);
    Uint32     i;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlags = 0;
    fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 1);
    fdActorSetSpritePriority(ACTOR_ISLAND_FIRST_LOG, 1);
    fdActorSetSpritePriority(ACTOR_ISLAND_SECOND_LOG, 1);
    fdActorSetSpritePriority(ACTOR_ISLAND_THIRD_LOG, 1);
    fdBlendScriptSuspend();
    sySynch(1);
    fdMapGridCopyMetatilesTo(46, 51, 6, 56, 18, 25);
    fdMapGridCopyMetatilesTo(84, 100, 6, 56, 18, 6);
    fdMapGridCopyMetatilesTo(59, 124, 9, 12, 12, 3);
    syIOWrite16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
    sySynch(1);
    water->offsetY     += -toFIXED(68);
    ground->offsetY    += -toFIXED(20);
    groundTop->offsetY += toFIXED(12);
    leader->z.fixed    += -toFIXED(12);
    camera->y.fixed    += -toFIXED(12);
    firstLog->z.fixed  += -toFIXED(12);
    secondLog->z.fixed += -toFIXED(12);
    thirdLog->z.fixed  += -toFIXED(12);
    fdMapGridCenterOnFollowedPoint();
    sySynch(4);
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdMapGridSetShakeFX(toFIXED(1), toFIXED(1), toFIXED(1));
    sySoundPlay(SE_AMBIENT_STREAM);
    // One step more of each than the rise took: the water's layer ends a pixel and a half from
    // where it began, the ground's half a pixel.
    i = 0;
    do
    {
        i++;
        scShrineOfTheSeaGodLowerWithWater();
    } while (i <= WATER_RISE_STEPS);
    i = 0;
    do
    {
        i++;
        scShrineOfTheSeaGodLowerWaterLayer();
    } while (i <= WATER_RISE_STEPS);
    scShrineOfTheSeaGodLowerWaterBand(84, 93);
    scShrineOfTheSeaGodLowerWaterBand(84, 86);
    syIOWrite16(BLEND_ALPHA(11, 9), &REG_BLDALPHA);
    scShrineOfTheSeaGodLowerWaterBand(84, 79);
    scShrineOfTheSeaGodLowerWaterBand(84, 72);
    syIOWrite16(BLEND_ALPHA(10, 10), &REG_BLDALPHA);
    scShrineOfTheSeaGodLowerWaterBand(84, 65);
    scShrineOfTheSeaGodLowerWaterBand(84, 58);
    syIOWrite16(BLEND_ALPHA(9, 11), &REG_BLDALPHA);
    scShrineOfTheSeaGodLowerWaterBand(84, 51);
    scShrineOfTheSeaGodLowerWaterBand(65, 100);
    syIOWrite16(BLEND_ALPHA(8, 12), &REG_BLDALPHA);
    scShrineOfTheSeaGodLowerWaterBand(65, 93);
    scShrineOfTheSeaGodLowerWaterBand(65, 86);
    syIOWrite16(BLEND_ALPHA(7, 13), &REG_BLDALPHA);
    scShrineOfTheSeaGodLowerWaterBand(65, 79);
    scShrineOfTheSeaGodLowerWaterBand(65, 72);
    syIOWrite16(BLEND_ALPHA(6, 14), &REG_BLDALPHA);
    scShrineOfTheSeaGodLowerWaterBand(65, 65);
    scShrineOfTheSeaGodLowerWaterBand(65, 58);
    syIOWrite16(BLEND_ALPHA(5, 15), &REG_BLDALPHA);
    scShrineOfTheSeaGodLowerWaterBand(65, 51);
    fdMapGridSetShakeFX(0, toFIXED(1), toFIXED(1));
    syIOWrite16(BLEND_ALPHA(4, 16), &REG_BLDALPHA);
    sySynch(10);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdMapGridSetShakeFX(-1, -1, toFIXED(9) / 10);
    syIOWrite16(BLEND_ALPHA(3, 16), &REG_BLDALPHA);
    sySynch(10);
    syIOWrite16(BLEND_ALPHA(2, 16), &REG_BLDALPHA);
    sySynch(10);
    syIOWrite16(BLEND_ALPHA(1, 16), &REG_BLDALPHA);
    sySynch(10);
    syIOWrite16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
    fdMapGridCopyMetatilesTo(103, 51, 6, 56, 18, 25);
    fdMapGridCopyMetatilesTo(46, 108, 9, 12, 12, 3);
    sySynch(10);
    fdMapGridWaitForShakeToSettle();
    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ISLAND_FIRST_LOG);
    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ISLAND_SECOND_LOG);
    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ISLAND_THIRD_LOG);
    fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    leader->motionFlags  = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    leader->y.fixed      = 0;
    leader->groundHeight = 0;
    fdScriptEnd();
}

extern Sint16 ShrineOfTheSeaGodPushedActors[];

// How far the water carries the leader between the island and the statue room, in pixels, half a
// pixel every two ticks.
#define STATUE_ROOM_RISE        50
// Sinking back to the island, the steps of half a pixel left when the screen starts to close.
#define STATUE_ROOM_CLOSE_STEPS 10

// Arriving in the room above the island, Felix rises up through the floor on the crystal statue's
// platform as the screen opens. The platform is the ground layer, which starts STATUE_ROOM_RISE
// pixels down with him.
void scShrineOfTheSeaGodRiseOnWater(void)
{
    MAP_STATE *map = RamHeapSlots->mapGrid;
    MAP_LAYER *layer;
    ACTOR     *leader;
    Uint32     i;

    layer  = &map->layers[1];
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    sySoundPlay(SE_SANDSTORM);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdCameraMoveWithinMapBounds(toFIXED(344), -1, toFIXED(248), 0);
    fdMapGridCopyMetatilesTo(62, 77, 62, 18, 19, 6);
    sySynch(1);
    fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
    leader->motionFlags   = 0;
    leader->y.fixed       = -toFIXED(STATUE_ROOM_RISE);
    leader->groundHeight  = -toFIXED(STATUE_ROOM_RISE);
    layer->offsetY       += -toFIXED(STATUE_ROOM_RISE);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
    fdScriptStartScreenTransition();
    fdMapGridSetShakeFX(toFIXED(1), toFIXED(1), toFIXED(1));
    for (i = 0; i < STATUE_ROOM_RISE * 2; i++)
    {
        leader->y.fixed      += toFIXED(1) / 2;
        leader->groundHeight += toFIXED(1) / 2;
        layer->offsetY       += toFIXED(1) / 2;
        sySynch(2);
    }
    sySoundPlay(SE_STOP_ACTION_SE);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdMapGridSetScreenShake(0, 0, 0);
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    fdMapGridCopyMetatilesTo(62, 69, 62, 18, 19, 6);
    sySynch(1);
    fdScriptEnd();
}

// Using the Sea God's Tear on the crystal statue in the room above the island takes Felix back
// down: he turns to face south, the statue's platform sinks into the floor with him, the screen
// closes round him, and he is back on the island, where the water drains away
// (scShrineOfTheSeaGodLowerWater). The platform is the ground layer, lowered STATUE_ROOM_RISE
// pixels with him.
void scShrineOfTheSeaGodSinkWithWater(void)
{
    MAP_STATE *map = RamHeapSlots->mapGrid;
    MAP_LAYER *layer;
    ACTOR     *actor;
    Uint32     i;

    layer = &map->layers[1];
    actor = fdActorGetById(MainRoutineStateWork.leaderActorId);

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdCameraMoveWithinMapBounds(toFIXED(344), -1, toFIXED(248), 1);
    fdMapGridCopyMetatilesTo(62, 77, 62, 18, 19, 6);
    fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 20);
    fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
    actor->motionFlags = 0;
    sySoundPlay(SE_AMBIENT_STREAM);
    fdMapGridSetShakeFX(toFIXED(1), toFIXED(1), toFIXED(1));
    for (i = 0; i < STATUE_ROOM_RISE * 2; i++)
    {
        actor->y.fixed      -= toFIXED(1) / 2;
        actor->groundHeight -= toFIXED(1) / 2;
        layer->offsetY      -= toFIXED(1) / 2;
        sySynch(2);
        if (i == STATUE_ROOM_RISE * 2 - STATUE_ROOM_CLOSE_STEPS)
        {
            FieldStateWork->transition =
                SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
            fdScriptFinishScreenTransition();
        }
    }
    fdScriptSetExitRequest(SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR);
}

// Sets up one of the ledge room's puddles, its ice pillar not shown until Frost freezes it.
void scShrineOfTheSeaGodSetUpPuddle(Sint32 actor)
{
    fdActorDisableById(actor);
    fdActorGetById(actor)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
}

// Frost on the westernmost of the three puddles west of Breath's ledge, metatile (8, 18), freezes
// it into a pillar of ice. Felix hops along the three ice pillars to the way out at the room's
// north-west, metatile (7, 12), to the room with the pillar over the hole; each frozen metatile is
// given the hop of the spare metatile in the map's corner, (0, 0).
void scShrineOfTheSeaGodFreezeFirstPuddle(void)
{
    fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 8, 18);
    syGameFlagSet(FLAG_SHRINE_FIRST_PUDDLE_FROZEN);
}

// Frost on the puddle two metatiles east of the first, metatile (10, 18), the south one of the pair
// beside it, freezes it into a pillar of ice to hop along.
void scShrineOfTheSeaGodFreezeSecondPuddle(void)
{
    fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 10, 18);
    syGameFlagSet(FLAG_SHRINE_SECOND_PUDDLE_FROZEN);
}

// Frost on the north puddle of that pair, metatile (10, 17), freezes it into a pillar of ice to hop
// along.
void scShrineOfTheSeaGodFreezeThirdPuddle(void)
{
    fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 10, 17);
    syGameFlagSet(FLAG_SHRINE_THIRD_PUDDLE_FROZEN);
}

// Move lifts the room's torch, or in the crossroads its stone pillar.
void scShrineOfTheSeaGodLiftBlock(void)
{
    fdActorLiftTargetBlock(ShrineOfTheSeaGodPushedActors);
}

// Sets the torch down, noting whether it stands against the north wall where it traps Breath.
void scShrineOfTheSeaGodSetDownTorch(void)
{
    ACTOR *actor;

    fdActorPlaceMovedBlock();
    actor = fdActorGetById(ACTOR_TORCH);
    if (FIELD_METATILE(actor->x.fixed) == 21 && FIELD_METATILE(actor->z.fixed) == 13)
        syGameFlagSet(FLAG_TORCH_AT_NORTH_WALL);
    else
        syGameFlagClear(FLAG_TORCH_AT_NORTH_WALL);
}

// Sets the crossroads' stone pillar down; pushed all the way west, it stays there.
void scShrineOfTheSeaGodMovePillar(void)
{
    ACTOR *actor = fdActorGetById(ACTOR_CROSSROADS_PILLAR);

    fdActorPlaceMovedBlock();
    if (FIELD_METATILE(actor->x.fixed) == 12)
        syGameFlagSet(FLAG_CROSSROADS_PILLAR_MOVED);
}

// Lets a pillar fall to the ground with a thump.
void scShrineOfTheSeaGodDropPillar(ACTOR *actor)
{
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    do
    {
        sySynch(1);
    } while (actor->velocityY != 0);
    sySoundPlay(SE_THUMP);
    sySynch(10);
    actor->motionFlags = 0;
}

// Pushed onto the hole, the pillar drops in and fills it.
void scShrineOfTheSeaGodPushPillarIntoHole(void)
{
    ACTOR *actor = fdActorGetById(ACTOR_HOLE_ROOM_PILLAR);
    Sint32 z     = FIELD_METATILE(actor->z.fixed);

    if (z == 16)
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        scShrineOfTheSeaGodDropPillar(actor);
        fdMapCopyMetatileAttributeRect(0, 0, 1, 1, 15, z);
        fdActorSetSpritePriority(ACTOR_HOLE_ROOM_PILLAR, 3);
        fdScriptEnd();
        syGameFlagSet(FLAG_PILLAR_IN_HOLE);
    }
}

// The crossroads' event: its bridges are drawn under the leader when he is up on them, in front of
// him below.
void scShrineOfTheSeaGodLayerCrossroadsBridges(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    Uint32 id;

    for (id = ACTOR_CROSSROADS_LOG_BRIDGE; id <= ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS; id++)
    {
        ACTOR *actor = fdActorGetById(id);

        if (leader->y.fixed > toFIXED(16))
        {
            fdActorSetSpritePriority(id, 2);
            actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        }
        else
        {
            fdActorSetSpritePriority(id, 1);
            actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
        }
    }
}

// Notes whether the leader stands on the upper level of the room.
void scShrineOfTheSeaGodTrackUpperLevel(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

    fdActorGetMetatileIndex(leader);
    if (leader->y.fixed < toFIXED(16))
        MainRoutineStateWork.onUpperLevel = 0;
    else
        MainRoutineStateWork.onUpperLevel = 1;
}

// The ledge room's event: the puddles are drawn behind or in front of the leader by where it
// stands.
void scShrineOfTheSeaGodLayerPuddles(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    FIXED  y;
    FIXED  z;

    y = leader->y.fixed;
    if (y < toFIXED(16))
    {
        z = leader->z.fixed;
        if (z < toFIXED(296))
        {
            fdActorSetSpritePriority(ACTOR_FIRST_PUDDLE, 1);
            fdActorSetSpritePriority(ACTOR_SECOND_PUDDLE, 1);
            fdActorSetSpritePriority(ACTOR_THIRD_PUDDLE, 2);
        }
        else
        {
            fdActorSetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
            fdActorSetSpritePriority(ACTOR_SECOND_PUDDLE, 2);
            fdActorSetSpritePriority(ACTOR_THIRD_PUDDLE, 3);
        }
    }
    else
    {
        fdActorSetSpritePriority(ACTOR_FIRST_PUDDLE, 2);
        fdActorSetSpritePriority(ACTOR_SECOND_PUDDLE, 2);
        fdActorSetSpritePriority(ACTOR_THIRD_PUDDLE, 3);
    }
}

// Keeps the marker on the torch's flame, 20 pixels up, as Move carries the torch.
void scShrineOfTheSeaGodKeepFlameOnTorch(void)
{
    ACTOR *torch  = fdActorGetById(ACTOR_TORCH);
    ACTOR *marker = fdActorGetById(ACTOR_TORCH_FLAME_MARKER);

    SET_ACTOR_POSITION(marker, torch->x.fixed, toFIXED(20), torch->z.fixed);
}

// field_actors.c's priority update answers what ACTOR_PRIORITY_IGNORES_RISES does: with it, an
// actor takes only the priority of the metatiles under its feet, and ground rising a metatile or
// two in front of it never puts it further back. Unused: lets the leader be put behind rising
// ground again, undoing what the hole room sets on arrival (ACTOR_PRIORITY_IGNORES_RISES).
UNUSED void scShrineOfTheSeaGodClearLeaderIgnoresRises(void)
{
    fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags &=
        ~ACTOR_PRIORITY_IGNORES_RISES;
}

// Unused: keeps the leader in front of rising ground, as the hole room does.
UNUSED void scShrineOfTheSeaGodSetLeaderIgnoresRises(void)
{
    fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
}

void scActorsSetDrawn(Sint32 show, Sint32 first, Uint32 count);

// Draws (show 1) or stops drawing the hole room's three unseen blocks.
void scShrineOfTheSeaGodSetBlocksShown(Sint32 show)
{
    if (MainRoutineStateWork.next == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
        scActorsSetDrawn(show,
                         ACTOR_HOLE_ROOM_FIRST_BLOCK,
                         ACTOR_HOLE_ROOM_THIRD_BLOCK - ACTOR_HOLE_ROOM_FIRST_BLOCK + 1);
}

// Unused: draws the hole room's unseen blocks and notes them shown.
UNUSED void scShrineOfTheSeaGodShowBlocks(void)
{
    scShrineOfTheSeaGodSetBlocksShown(1);
    syGameFlagSet(FLAG_BLOCKS_SHOWN);
}

// Stops drawing the hole room's unseen blocks.
void scShrineOfTheSeaGodHideBlocks(void)
{
    scShrineOfTheSeaGodSetBlocksShown(0);
    syGameFlagClear(FLAG_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

// Nothing visible happens when Felix steps onto a metatile at the end of one of the crossroads'
// bridges or by its north doorways. The game sets him to be drawn in front of the bridges from then
// on, so that he shows on top of the one he walks across, until he comes down the ladder below
// them.
void scShrineOfTheSeaGodDrawLeaderInFront(void)
{
    fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 1);
    syGameFlagSet(FLAG_LEADER_IN_FRONT);
}

// Nothing visible happens when Felix comes down to the foot of the crossroads' ladder below the
// bridges, metatile (29, 22). The game lets the ground under him decide again how he is drawn, so
// that he passes behind the bridges overhead.
void scShrineOfTheSeaGodDrawLeaderBehind(void)
{
    fdActorSetSpritePriority(MainRoutineStateWork.leaderActorId, 2);
    fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    syGameFlagClear(FLAG_LEADER_IN_FRONT);
}

#include "../field/field_effect_advance_motion.c"

#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

// The broken bridge's middle less the leader's place, 16.16 pixels, as he rocks on it.
FIXED ShrineOfTheSeaGodBridgeOffset[3] BSS_DATA;

// On a broken bridge's middle the leader sways twice, the middle dipping under him, and loses his
// shadow; the middle's place against him is kept for the fall (scShrineOfTheSeaGodKeepWithLeader).
void  scShrineOfTheSeaGodRockOnBridge(ACTOR *leader, ACTOR *middle)
{
    FIELD_SPR *sprite = leader->sprite;
    Uint32     n;
    Uint32     i;

    fdEffectSetAttached(MainRoutineStateWork.leaderActorId,
                        ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    sySoundPlay(SE_HIDDEN_PASSAGE_OPEN);
    leader->motionFlags = 0;
    for (n = 0; n <= 1; n++)
    {
        middle->y.fixed -= toFIXED(1);
        for (i = 0; i <= 3; i++)
        {
            sprite->rotation += TRIG_TURN / 512;
            sySynch(1);
        }
        middle->y.fixed += toFIXED(1);
        for (i = 0; i <= 7; i++)
        {
            sprite->rotation -= TRIG_TURN / 512;
            sySynch(1);
        }
        middle->y.fixed -= toFIXED(1);
        for (i = 0; i <= 3; i++)
        {
            sprite->rotation += TRIG_TURN / 512;
            sySynch(1);
        }
    }
    middle->y.fixed += toFIXED(1);
    sySynch(20);
    ShrineOfTheSeaGodBridgeOffset[0] = middle->x.fixed - leader->x.fixed;
    ShrineOfTheSeaGodBridgeOffset[1] = middle->y.fixed - leader->y.fixed;
    ShrineOfTheSeaGodBridgeOffset[2] = middle->z.fixed - leader->z.fixed;
    fdActorSetSpriteFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), 0);
}

// Keeps a broken bridge's middle where it lies against the leader as he falls.
void scShrineOfTheSeaGodKeepWithLeader(ACTOR *actor)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    FIXED *offset = ShrineOfTheSeaGodBridgeOffset;

    SET_ACTOR_POSITION(actor,
                       leader->x.fixed + offset[0],
                       leader->y.fixed + offset[1],
                       leader->z.fixed + offset[2]);
}

// The event that carries the crossroads' broken bridge's middle along with the leader.
void scShrineOfTheSeaGodKeepCrossroadsBridgeWithLeader(void)
{
    scShrineOfTheSeaGodKeepWithLeader(fdActorGetById(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE));
}

void scFieldEffectSpawn(Sint32                x,
                        Sint32                y,
                        Sint32                z,
                        Sint32                velocityX,
                        Sint32                velocityY,
                        Sint32                velocityZ,
                        Uint32                flags,
                        const EFFECT_OPTIONS *options);

// The spray the crossroads' bridge throws up as it hits the water: a drop every sixteenth of a
// turn, the first direction thrown twice.
#define SPRAY_DIRECTIONS 16

// On the crossroads' broken bridge the leader sways until its middle drops with him into the water
// below, throwing up spray, and tips there.
void scShrineOfTheSeaGodBreakCrossroadsBridge(void)
{
    ACTOR         *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    ACTOR         *middle = fdActorGetById(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE);
    FIELD_SPR     *sprite = middle->sprite;
    EFFECT_OPTIONS options;
    Uint32         i;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 408, 328);
    scShrineOfTheSeaGodRockOnBridge(leader, middle);
    fdMapGridCopyMetatileAttributes(0, 50, 3, 1, 24, 70);
    syGameFlagSet(FLAG_CROSSROADS_BRIDGE_BROKEN);
    sySynch(1);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    fdActorSetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 3);
    middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    syFieldSetTickEvent(scShrineOfTheSeaGodKeepCrossroadsBridgeWithLeader, EVENT_PASS_TICK);
    do
        sySynch(1);
    while (leader->velocityY != 0);
    sySoundPlay(SE_TOPPLE);
    fdMapGridSetShakeFX(toFIXED(1), toFIXED(2), toFIXED(1));
    options.palette     = SPRITE_COLOR_WHITE;
    options.update      = fdEffectAdvanceMotion;
    options.startScaleX = toFIXED(64) / 100;
    options.startScaleY = toFIXED(64) / 100;
    for (i = 0; i <= SPRAY_DIRECTIONS;)
    {
        FIXED velocity[3];

        velocity[0]  = syCos(i * (TRIG_TURN / SPRAY_DIRECTIONS));
        velocity[1]  = 0;
        velocity[2]  = sySin(i * (TRIG_TURN / SPRAY_DIRECTIONS));
        velocity[0] += velocity[0] >> 1;
        velocity[0] += syRandomBelow(toFIXED(3) / 4);
        i++;
        velocity[0] += -(toFIXED(3) / 8);
        velocity[2] += syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
        scFieldEffectSpawn(leader->x.fixed,
                           leader->y.fixed - toFIXED(4),
                           leader->z.fixed,
                           velocity[0],
                           velocity[1],
                           velocity[2],
                           EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE
                               | EFFECT_SCRIPT_MEDIUM,
                           &options);
    }
    leader->velocityY = toFIXED(4);
    sySynch(2);
    fdMapGridSetShakeFX(-1, -1, toFIXED(9) / 10);
    syCloseEvent(scShrineOfTheSeaGodKeepCrossroadsBridgeWithLeader);
    for (i = 0; i < 8; i++)
    {
        sprite->rotation += TRIG_TURN / 128;
        middle->z.fixed  += toFIXED(1);
        sySynch(1);
    }
    fdMapGridWaitForShakeToSettle();
    fdActorSetSpriteFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), SPRITE_FLAG_SHADOW);
    leader->layer = 0;
    sySynch(10);
    fdScriptEnd();
}

// Waits for the leader, falling with a broken bridge's middle, to land in the stream, then throws
// up the bridge's two splashes there; bridge counts the splashes from ACTOR_FIRST_BRIDGE_SPLASH.
void scShrineOfTheSeaGodLandBrokenBridge(ACTOR *actor, Sint32 bridge)
{
    do
    {
        sySynch(1);
    } while (actor->velocityY != 0);
    sySoundPlay(SE_LOG_FALL);
    fdActorSetSpritePriority(ACTOR_FIRST_BRIDGE_SPLASH + bridge, 3);
    fdActorSetSpritePriority(ACTOR_FIRST_BRIDGE_OTHER_SPLASH + bridge, 3);
    fdActorSetPosAndResetMotion(
        fdActorGetById(ACTOR_FIRST_BRIDGE_SPLASH + bridge), actor->x.fixed, 0, actor->z.fixed);
    fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH + bridge),
                                actor->x.fixed,
                                0,
                                actor->z.fixed - toFIXED(16));
    fdActorSetAnimationById(ACTOR_FIRST_BRIDGE_SPLASH + bridge, 1);
    fdActorSetAnimationById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH + bridge, 1);
    sySynch(20);
}

// The event that carries the pillar room's first broken bridge's middle along with the leader.
void scShrineOfTheSeaGodKeepFirstBridgeWithLeader(void)
{
    scShrineOfTheSeaGodKeepWithLeader(fdActorGetById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE));
}

// On the pillar room's first broken bridge the leader sways until its middle drops with him into
// the stream, which carries him south on it.
void scShrineOfTheSeaGodBreakFirstBridge(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    ACTOR *middle = fdActorGetById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE);

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorWalkToAndWaitById(MainRoutineStateWork.leaderActorId, 328, 216);
    scShrineOfTheSeaGodRockOnBridge(leader, middle);
    fdMapCopyMetatileAttributeRect(0, 0, 3, 1, 19, 13);
    fdMapCopyMetatileAttributeRect(40, 0, 2, 1, 19, 33);
    sySynch(1);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    fdActorSetSpritePriority(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE, 3);
    middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    syFieldSetTickEvent(scShrineOfTheSeaGodKeepFirstBridgeWithLeader, EVENT_PASS_TICK);
    scShrineOfTheSeaGodLandBrokenBridge(leader, 0);
    // Down in the stream, the bridges it passes under are drawn over him.
    fdActorSetSpritePriority(ACTOR_PILLAR_ROOM_SECOND_BRIDGE, 1);
    fdActorGetById(ACTOR_PILLAR_ROOM_SECOND_BRIDGE)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    fdActorSetSpritePriority(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE, 1);
    fdActorGetById(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    leader->motionFlags                                             = 0;
    fdActorSetSpriteFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), SPRITE_FLAG_SHADOW);
    fdActorSetSpeedAndAcceleration(MainRoutineStateWork.leaderActorId, 0.6, 0.1);
    fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, 328, 224);
    fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 321, 256);
    fdActorSetHorizontalPosWithTerrain(ACTOR_FIRST_BRIDGE_SPLASH, 0, 0);
    fdActorSetHorizontalPosWithTerrain(ACTOR_FIRST_BRIDGE_OTHER_SPLASH, 0, 0);
    fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 321, 320);
    fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 321, 400);
    fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 321, 480);
    fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, 321, 536);
    sySoundPlay(SE_RATTLE);
    fdActorSetSpritePriority(ACTOR_PILLAR_ROOM_SECOND_BRIDGE, 2);
    fdActorGetById(ACTOR_PILLAR_ROOM_SECOND_BRIDGE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    fdActorSetSpritePriority(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE, 2);
    fdActorGetById(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    syCloseEvent(scShrineOfTheSeaGodKeepFirstBridgeWithLeader);
    sySynch(1);
    fdScriptEnd();
    syGameFlagSet(FLAG_FIRST_BRIDGE_BROKEN);
}

// The event that carries the pillar room's second broken bridge's middle along with the leader.
void scShrineOfTheSeaGodKeepSecondBridgeWithLeader(void)
{
    scShrineOfTheSeaGodKeepWithLeader(fdActorGetById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE));
}

// On the pillar room's second broken bridge the leader sways until its middle drops with him into
// the stream, which carries him a little way south.
void scShrineOfTheSeaGodBreakSecondBridge(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    ACTOR *middle = fdActorGetById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE);

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 408, 360);
    scShrineOfTheSeaGodRockOnBridge(leader, middle);
    fdMapCopyMetatileAttributeRect(0, 0, 3, 1, 24, 22);
    fdMapCopyMetatileAttributeRect(40, 0, 2, 1, 24, 24);
    sySynch(1);
    leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    fdActorSetSpritePriority(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE, 3);
    middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    syFieldSetTickEvent(scShrineOfTheSeaGodKeepSecondBridgeWithLeader, EVENT_PASS_TICK);
    scShrineOfTheSeaGodLandBrokenBridge(leader,
                                        ACTOR_SECOND_BRIDGE_SPLASH - ACTOR_FIRST_BRIDGE_SPLASH);
    fdActorSetSpriteFlags(fdActorGetById(MainRoutineStateWork.leaderActorId), SPRITE_FLAG_SHADOW);
    fdActorSetSpeedAndAcceleration(MainRoutineStateWork.leaderActorId, 0.6, 0.1);
    fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, 406, 360);
    fdActorMoveToAndWaitById(MainRoutineStateWork.leaderActorId, 400, 392);
    sySoundPlay(SE_RATTLE);
    fdActorSetHorizontalPosWithTerrain(ACTOR_SECOND_BRIDGE_SPLASH, 0, 0);
    fdActorSetHorizontalPosWithTerrain(ACTOR_SECOND_BRIDGE_OTHER_SPLASH, 0, 0);
    syCloseEvent(scShrineOfTheSeaGodKeepSecondBridgeWithLeader);
    sySynch(1);
    fdScriptEnd();
    syGameFlagSet(FLAG_SECOND_BRIDGE_BROKEN);
}

// Reveal shows the seventh stepping stone in the hexagon of rocks.
void scShrineOfTheSeaGodRevealStone(void)
{
    fdMapGridCopyMetatilesTo(42, 1, 15, 14, 1, 2);
    fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 15, 14);
}

// As Reveal fades, the seventh stone is hidden again.
void scShrineOfTheSeaGodHideStone(void)
{
    fdMapGridCopyMetatilesTo(15, 9, 15, 14, 1, 2);
    fdMapGridCopyMetatileAttributes(14, 14, 1, 1, 15, 14);
}

// Back in the rope room, Riki tries to throw the rope up to Tavi again, misses and goes off; Kraden
// joins the leader.
void scShrineOfTheSeaGodThrowRopeAgain(void)
{
    ACTOR *leader;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdScriptSetMessageId((Sint32)MsgFieldRopeThrownAgain);
    // "All right! I'm gonna throw the rope again!"
    fdScriptShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_RIKI, 0);
    fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
    fdActorSetSpeed(ACTOR_ROPE_ROOM_KRADEN, 0.8);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 272, 336);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
    fdActorSetHorizontalPosWithTerrain(ACTOR_ROPE_ROOM_KRADEN, toFIXED(272), toFIXED(336));
    sySynch(1);
    fdActorWalkToAndWait(ACTOR_ROPE_ROOM_KRADEN, 256, 336);
    fdActorFaceDirectionById(ACTOR_ROPE_ROOM_KRADEN, FACING_EAST, 0);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 40);
    fdActorFaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdCameraSetSpeed(4.8);
    fdCameraMoveTo(288, CAMERA_KEEP, 126, 1);
    fdCameraWaitForArrival();
    fdActorJump(ACTOR_TAVI, 4, 20);
    // "Just don't miss this time!"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
    fdActorSetAnimationAndWait(ACTOR_RIKI, 3);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_RIKI, 2);
    fdActorJump(ACTOR_RIKI, 6, 80);
    // "Aww, no! You missed again!"
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
    fdActorSetAnimationById(ACTOR_RIKI, 4);
    // "Sorry, Tavi... I can't do it!"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
    fdActorFaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 0);
    scFieldActorShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
    // "I thought I got lucky when that wave carried me up here..."
    fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
    // "What are you, crazy? It knocked you stupid for a good ten minutes!"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
    fdActorFaceDirectionById(ACTOR_TAVI, FACING_EAST, 0);
    fdEffectSetAttached(ACTOR_TAVI, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(20);
    // "Yeah, yeah! Just find some way to get me down from here!"
    fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_TAVI, 0);
    fdActorSetAnimationAndWait(ACTOR_RIKI, 3);
    // "Fine! Just don't jump down or do anything dumb, OK?"
    fdScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
    fdCameraSetSpeed(3.2);
    fdCameraMoveTo(420, CAMERA_KEEP, 200, 1);
    fdActorSetSpeed(ACTOR_RIKI, 1.6);
    fdActorWalkToAndWait(ACTOR_RIKI, 346, 186);
    fdActorWalkToAndWait(ACTOR_RIKI, 374, 192);
    fdActorWalkToAndWait(ACTOR_RIKI, 392, 224);
    fdActorWalkToAndWait(ACTOR_RIKI, 394, 246);
    fdActorWalkToAndWait(ACTOR_RIKI, 420, 268);
    fdActorWalkToAndWait(ACTOR_RIKI, 456, 268);
    fdActorWalkToAndWait(ACTOR_RIKI, 488, 240);
    fdActorWalkToAndWait(ACTOR_RIKI, 488, 226);
    fdActorSetHorizontalPosWithTerrain(ACTOR_RIKI, 0, 0);
    fdCameraSetSpeed(4.8);
    fdCameraMoveWithinMapBounds(toFIXED(264), -1, toFIXED(320), 1);
    fdCameraWaitForArrival();
    fdActorFaceDirectionById(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
    fdActorFaceDirectionById(ACTOR_ROPE_ROOM_KRADEN, FACING_EAST, 20);
    fdActorSetAnimationById(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_ROPE_ROOM_KRADEN, 3);
    fdActorSetAnimationById(ACTOR_ROPE_ROOM_KRADEN, 2);
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    if (leader != NULL)
        fdActorSetDestinationById(
            ACTOR_ROPE_ROOM_KRADEN, leader->x.part[0].integer, leader->z.part[0].integer);
    fdActorWaitForMoveById(ACTOR_ROPE_ROOM_KRADEN);
    fdActorSetHorizontalPosWithTerrain(ACTOR_ROPE_ROOM_KRADEN, 0, 0);
    fdActorSetHorizontalPosWithTerrain(ACTOR_TAVI, 0, 0);
    syGameFlagSet(FLAG_RIKI_GAVE_UP);
    fdScriptEnd();
}

extern Uint32 ShrineOfTheSeaGodBoysGoHome[];

// Lash ties the boys' rope, Tavi climbs down and both meet the party.
void          scShrineOfTheSeaGodLashRope(void)
{
    ACTOR      *tavi;
    ACTOR      *leader;
    FIELD_SPR  *sprite;
    const void *callback;

    if (fdScriptGetFlag(FLAG_TAVI_RESCUED))
        return;
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdCameraGetActor()->motionFlags = 0;
    fdCameraMoveTo(340, CAMERA_KEEP, 176, 1);
    fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
    fdActorSetSpeed(ACTOR_ROPE_ROOM_KRADEN, 0.8);
    fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 344, 184);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
    fdActorSetPos(ACTOR_ROPE_ROOM_KRADEN, 344, 184);
    sySynch(1);
    fdActorWalkToAndWait(ACTOR_ROPE_ROOM_KRADEN, 356, 196);
    fdActorFaceDirectionAndWait(ACTOR_ROPE_ROOM_KRADEN, FACING_NORTH_NORTHWEST);
    fdScriptSetMessageId((Sint32)MsgFieldPraiseRopeWork);
    // "Excellent work, Felix!"
    fdScriptShowMessage(ACTOR_ROPE_ROOM_KRADEN, 0);
    fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
    // "Hey, the rope!"
    fdScriptShowMessage(ACTOR_TAVI, 0);
    fdActorSetPos(ACTOR_TAVI, 184, 168);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_WEST, 0);
    fdCameraSetSpeed(4.8);
    fdCameraMoveTo(232, 64, 176, 1);
    fdActorSetSpeed(ACTOR_TAVI, 1.6);
    fdActorWalkToAndWait(ACTOR_TAVI, 232, 168);
    fdScriptWait(20);
    // "Awesome, Riki! Way to go!"
    fdScriptShowMessage(ACTOR_TAVI, 0);
    fdCameraSetSpeed(1.2);
    fdCameraMoveTo(348, CAMERA_KEEP, 216, 1);
    tavi   = fdActorGetById(ACTOR_TAVI);
    sprite = tavi->sprite;
    fdActorSetSpritePriority(ACTOR_TAVI, 1);
    fdActorSetSpeed(ACTOR_TAVI, 1.6);
    fdActorJump(ACTOR_TAVI, 4, 0);
    fdActorMoveToAndWait(ACTOR_TAVI, 248, 168);
    fdActorSetAnimationById(ACTOR_TAVI, 6);
    tavi->scaleX       = -toFIXED(1);
    sprite->rotation   = TRIG_TURN * 13 / 16;
    tavi->motionFlags  = 0;
    tavi->y.fixed     += toFIXED(6);
    fdActorSetSpriteFlags(fdActorGetById(ACTOR_TAVI), 0);
    fdActorSetSpeed(ACTOR_TAVI, 0.4);
    scFieldActorSetMoveTarget(tavi, 328, 38, 168);
    fdActorWaitForMoveById(ACTOR_TAVI);
    tavi->motionFlags  = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    tavi->groundHeight = toFIXED(32);
    tavi->scaleX       = toFIXED(1);
    sprite->rotation   = 0;
    fdActorSetAnimationById(ACTOR_TAVI, 1);
    fdActorSetSpriteFlags(fdActorGetById(ACTOR_TAVI), SPRITE_FLAG_SHADOW);
    fdActorSetSpeed(ACTOR_TAVI, 1.6);
    fdActorJump(ACTOR_TAVI, 4, 0);
    fdActorMoveToAndWait(ACTOR_TAVI, 328, 184);
    fdActorFaceDirection(ACTOR_TAVI, FACING_EAST, 20);
    scFieldActorShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorSetSpritePriority(ACTOR_TAVI, 2);
    // "Who are you?"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    // "We're just passing through."
    fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ROPE_ROOM_KRADEN, 0);
    scFieldActorShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    // "How come you threw me the rope?"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    fdActorFaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_NORTH_NORTHWEST, 40);
    fdActorFaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_WEST, 0);
    // "We'd heard that you needed some assistance."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ROPE_ROOM_KRADEN, 0, 60);
    // "Well, whatever. Thanks anyway."
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    fdActorSetAnimationById(ACTOR_TAVI, 3);
    fdActorSetPos(ACTOR_RIKI, 394, 286);
    sySynch(1);
    fdActorSetSpeed(ACTOR_RIKI, 1.6);
    fdActorWalkToAndWait(ACTOR_RIKI, 394, 248);
    fdActorFaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
    fdActorJump(ACTOR_RIKI, 4, 20);
    // "Hey, Tavi!"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_RIKI, 0);
    scFieldActorShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorFaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
    fdCameraSetSpeed(4.8);
    fdCameraMoveTo(376, CAMERA_KEEP, 264, 1);
    fdCameraWaitForArrival();
    // "How'd ya get down from there?"
    fdScriptShowMessage(ACTOR_RIKI, 0);
    fdCameraSetSpeed(2.4);
    fdCameraMoveTo(348, CAMERA_KEEP, 216, 1);
    fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_RIKI);
    fdActorWatchAndSetScript(ACTOR_ROPE_ROOM_KRADEN, ACTOR_RIKI);
    fdActorWatchAndSetScript(ACTOR_TAVI, ACTOR_RIKI);
    fdActorWalkToAndWait(ACTOR_RIKI, 394, 220);
    fdActorWalkToAndWait(ACTOR_RIKI, 370, 198);
    fdActorSetSpeed(ACTOR_ROPE_ROOM_KRADEN, 1.6);
    fdActorMoveToAndWait(ACTOR_ROPE_ROOM_KRADEN, 368, 180);
    fdActorWatchAndSetScript(ACTOR_ROPE_ROOM_KRADEN, ACTOR_RIKI);
    fdActorWalkToAndWait(ACTOR_RIKI, 336, 198);
    fdActorFaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
    fdActorStop(ACTOR_TAVI);
    // "These guys threw me the rope..."
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    fdActorFaceDirectionById(ACTOR_RIKI, FACING_NORTH_NORTHEAST, 0);
    // "Really? These grown-ups?"
    fdScriptShowMessage(ACTOR_RIKI, 0);
    fdActorFaceDirectionAndWait(ACTOR_RIKI, FACING_NORTH_NORTHWEST);
    fdActorJump(ACTOR_RIKI, 4, 20);
    // "Well, whatever. Now we can go nab that critter we saw!"
    fdScriptShowMessageThenWait(ACTOR_RIKI, 0, 20);
    scFieldActorShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    scFieldActorShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "What? What is it?"
    fdScriptShowMessage(ACTOR_RIKI, 0);
    fdActorSetAnimationById(ACTOR_TAVI, 4);
    // "That little guy's so fast, I can't catch him!"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    scFieldActorShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    // "Do you think we could catch him together?"
    fdScriptShowMessage(ACTOR_RIKI, 0);
    fdActorFaceDirection(ACTOR_TAVI, FACING_WEST, 20);
    // "Well, yeah, except that every time I'm about to grab him, he flies off!"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    fdActorFaceDirection(ACTOR_RIKI, FACING_WEST, 0);
    // "So I guess it wouldn't matter if we worked together or not."
    fdScriptShowMessage(ACTOR_RIKI, 0);
    fdActorSetAnimationAndWait(ACTOR_TAVI, 4);
    // "Yeah, I guess not."
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    scFieldActorShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    // "Well, that's too bad."
    fdScriptShowMessage(ACTOR_RIKI, 0);
    scFieldActorShowEmote(ACTOR_RIKI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    scFieldActorShowEmote(ACTOR_TAVI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorFaceDirection(ACTOR_RIKI, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_TAVI, FACING_SOUTH_SOUTHEAST, 40);
    fdActorJump(ACTOR_TAVI, 4, 40);
    // "Hey, I'm really hungry! I haven't eaten since before I got stuck!"
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    fdActorSetAnimationById(ACTOR_RIKI, 3);
    // "Yeah, same here... My stomach's growling up a storm!"
    fdScriptShowMessage(ACTOR_RIKI, 0);
    fdActorSetAnimationAndWait(ACTOR_TAVI, 3);
    // "Let's go home, Riki."
    fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_TAVI, 0);
    // "Sounds good, Tavi."
    fdScriptShowMessage(ACTOR_RIKI, 0);
    fdActorSetAnimationById(ACTOR_RIKI, 3);
    fdActorSetAnimationAndWait(ACTOR_TAVI, 3);
    fdActorSetSpeed(ACTOR_RIKI, 1.2);
    fdActorSetSpeed(ACTOR_TAVI, 1.2);
    callback = ShrineOfTheSeaGodBoysGoHome;
    fdActorSetMovementTypeById(ACTOR_RIKI, callback);
    fdScriptWait(10);
    fdActorSetMovementTypeAndWaitById(ACTOR_TAVI, callback);
    fdScriptWait(40);
    fdActorStop(MainRoutineStateWork.leaderActorId);
    fdActorStop(ACTOR_ROPE_ROOM_KRADEN);
    sySynch(1);
    // "They're gone..."
    fdScriptShowMessageThenWait(ACTOR_ROPE_ROOM_KRADEN, 0, 40);
    fdActorSetPosFX(ACTOR_TAVI, 0, 0);
    fdActorSetPosFX(ACTOR_RIKI, 0, 0);
    // "Well, it was good of you to help them."
    fdScriptShowMessage(ACTOR_ROPE_ROOM_KRADEN, 0);
    fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_ROPE_ROOM_KRADEN, FACING_WEST, 20);
    // "We should be going, too."
    fdScriptShowMessage(ACTOR_ROPE_ROOM_KRADEN, 0);
    fdActorSetAnimationById(ACTOR_ROPE_ROOM_KRADEN, 2);
    leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    if (leader != NULL)
        fdActorSetDestinationById(
            ACTOR_ROPE_ROOM_KRADEN, FIXEDtoINT(leader->x.fixed), FIXEDtoINT(leader->z.fixed));
    fdActorWaitForMoveById(ACTOR_ROPE_ROOM_KRADEN);
    fdActorSetPosFX(ACTOR_ROPE_ROOM_KRADEN, 0, 0);
    syGameFlagSet(FLAG_TAVI_RESCUED);
    fdScriptEnd();
}

// Breath, first seen in the crossroads, startles and runs east.
void scShrineOfTheSeaGodDjinniFleesEast(void)
{
    Uint8 off = 0;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdCameraGetActor()->motionFlags = off;
    fdCameraMoveWithinMapBounds(toFIXED(480), toFIXED(32), toFIXED(340), 1);
    fdCameraWaitForArrival();
    fdActorFaceActorById(ACTOR_CROSSROADS_BREATH, MainRoutineStateWork.leaderActorId, 0);
    fdActorShowEmoteById(ACTOR_CROSSROADS_BREATH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_BREATH, 1);
    fdActorGetById(ACTOR_CROSSROADS_BREATH)->motionFlags = off;
    fdActorSetSpeed(ACTOR_CROSSROADS_BREATH, 1.6);
    fdActorWalkToAndWait(ACTOR_CROSSROADS_BREATH, 500, 344);
    fdActorWalkToAndWait(ACTOR_CROSSROADS_BREATH, 580, 344);
    fdActorWatchAndSetScript(ACTOR_CROSSROADS_BREATH, MainRoutineStateWork.leaderActorId);
    fdScriptWait(20);
    syGameFlagSet(FLAG_DJINNI_FLED_EAST);
    fdScriptEnd();
}

// In the crossroads, Breath flees south out of the room.
void scShrineOfTheSeaGodDjinniFleesSouth(void)
{
    Uint8 off = 0;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorStop(ACTOR_CROSSROADS_BREATH);
    fdCameraGetActor()->motionFlags = off;
    fdCameraMoveWithinMapBounds(toFIXED(564), toFIXED(32), toFIXED(344), 1);
    fdCameraWaitForArrival();
    fdActorFaceActorById(ACTOR_CROSSROADS_BREATH, MainRoutineStateWork.leaderActorId, 0);
    fdActorShowEmoteById(ACTOR_CROSSROADS_BREATH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_BREATH, 1);
    fdActorGetById(ACTOR_CROSSROADS_BREATH)->motionFlags = off;
    fdActorSetSpeed(ACTOR_CROSSROADS_BREATH, 1.6);
    fdActorWalkToAndWait(ACTOR_CROSSROADS_BREATH, 616, 376);
    fdActorWalkToAndWait(ACTOR_CROSSROADS_BREATH, 616, 460);
    fdActorSetHorizontalPosWithTerrain(ACTOR_CROSSROADS_BREATH, 0, 0);
    fdScriptWait(20);
    syGameFlagSet(FLAG_DJINNI_FLED_SOUTH);
    fdScriptEnd();
}

// The Djinni runs up from the south stairs, under the bridge, and waits by the ledge.
void scShrineOfTheSeaGodDjinniRunsNorth(void)
{
    ACTOR *bridge = fdActorGetById(ACTOR_LEDGE_ROOM_BRIDGE);
    Sint32 angle;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorShowEmoteById(MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdCameraGetActor()->motionFlags = 0;
    fdCameraMoveTo(328, 32, 552, 1);
    fdCameraWaitForArrival();
    fdActorSetPosAndFacing(ACTOR_BREATH, toFIXED(328), toFIXED(624), FACING_NORTH);
    sySynch(1);
    fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_BREATH);
    bridge->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    fdActorSetSpritePriority(ACTOR_BREATH, 3);
    fdActorSetSpeed(ACTOR_BREATH, 1.6);
    fdActorWalkToAndWait(ACTOR_BREATH, 328, 568);
    fdActorWalkToAndWait(ACTOR_BREATH, 328, 496);
    fdActorWalkToAndWaitById(ACTOR_BREATH, 280, 432);
    fdActorStop(MainRoutineStateWork.leaderActorId);
    sySynch(1);
    angle = FACING_SOUTH;
    fdActorSetPosAndFacing(ACTOR_BREATH, toFIXED(280), toFIXED(328), angle);
    fdActorSetSpritePriority(ACTOR_BREATH, 2);
    fdActorGetById(ACTOR_BREATH)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    bridge->priorityFlags                       |= ACTOR_PRIORITY_UNDERFOOT;
    sySynch(1);
    syGameFlagSet(FLAG_DJINNI_RAN_PAST);
    syGameFlagSet(FLAG_DJINNI_BY_LEDGE);
    fdScriptEnd();
}

// In the ledge room, Breath runs west past the leader.
void scShrineOfTheSeaGodDjinniRunsWest(void)
{
    Sint32 done = fdScriptGetFlag(FLAG_DJINNI_RAN_PAST);

    if (done == 0)
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        fdActorShowEmoteById(
            MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdCameraGetActor()->motionFlags = done;
        fdCameraMoveWithinMapBounds(toFIXED(328), toFIXED(48), toFIXED(552), 1);
        fdCameraWaitForArrival();
        done = FACING_SOUTH;
        fdActorSetPosAndFacing(ACTOR_BREATH, toFIXED(424), toFIXED(520), done);
        sySynch(1);
        fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_BREATH);
        fdActorSetSpritePriority(ACTOR_BREATH, 1);
        fdActorSetSpeed(ACTOR_BREATH, 1.6);
        fdActorWalkToAndWait(ACTOR_BREATH, 380, 552);
        fdActorWalkToAndWait(ACTOR_BREATH, 264, 552);
        fdActorWalkToAndWaitById(ACTOR_BREATH, 264, 540);
        fdActorStop(MainRoutineStateWork.leaderActorId);
        sySynch(1);
        fdActorSetHorizontalPosWithTerrain(ACTOR_BREATH, 0, 0);
        fdActorSetSpritePriority(ACTOR_BREATH, 2);
        fdActorGetById(ACTOR_BREATH)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
        sySynch(1);
        syGameFlagSet(FLAG_DJINNI_RAN_PAST);
        fdScriptEnd();
    }
}

// Breath hops up onto the ledge, where the chase round it begins.
void scShrineOfTheSeaGodDjinniHopsOntoLedge(void)
{
    Uint8 off;

    if (syGameFlagGet(FLAG_DJINNI_BY_LEDGE))
    {
        fdScriptStart();
        fdScriptEndLastingPsynergy(0);
        off                             = 0;
        fdCameraGetActor()->motionFlags = off;
        fdCameraMoveWithinMapBounds(toFIXED(320), -1, toFIXED(300), 1);
        scFieldActorShowEmote(ACTOR_BREATH, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
        fdActorSetSpeed(ACTOR_BREATH, 1.2);
        fdActorWalkToAndWait(ACTOR_BREATH, 345, 296);
        fdActorWalkToAndWait(ACTOR_BREATH, 392, 296);
        fdActorWatchAndSetScript(ACTOR_BREATH, MainRoutineStateWork.leaderActorId);
        fdActorGetById(ACTOR_BREATH)->sceneCount = off;
        sySynch(10);
        syGameFlagSet(FLAG_DJINNI_ON_LEDGE);
        fdScriptEnd();
    }
}

// Starts one of Breath's dashes: the camera and Breath stop, and Breath picks up speed.
void scShrineOfTheSeaGodStartDjinniDash(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdCameraGetActor()->motionFlags = 0;
    fdActorStop(ACTOR_BREATH);
    sySynch(1);
    fdActorSetSpeed(ACTOR_BREATH, 2.4);
}

// Ends one of Breath's dashes: Breath watches the leader again.
void scShrineOfTheSeaGodEndDjinniDash(void)
{
    fdActorWatchAndSetScript(ACTOR_BREATH, MainRoutineStateWork.leaderActorId);
    fdScriptWait(5);
    fdScriptEnd();
}

// A dash of Breath's along the ledge to (toX, toZ), where it stands at place.
#define DEFINE_DJINNI_DASH(name, toX, toZ, place)                                                  \
    void name(void)                                                                                \
    {                                                                                              \
        scShrineOfTheSeaGodStartDjinniDash();                                                      \
        fdCameraMoveWithinMapBounds(toFIXED(toX), toFIXED(16), toFIXED(toZ), 1);                   \
        fdActorWalkToAndWaitById(ACTOR_BREATH, toX, toZ);                                          \
        fdActorGetById(ACTOR_BREATH)->sceneCount = place;                                          \
        scShrineOfTheSeaGodEndDjinniDash();                                                        \
    }

// With Breath at the ledge's south-east corner, the leader comes up the south- east way: Breath
// dashes north to the north-east corner.
DEFINE_DJINNI_DASH(scShrineOfTheSeaGodDjinniSouthEastToNorthEast,
                   392,
                   216,
                   BREATH_AT_NORTHEAST_CORNER)

// Breath runs along the ledge's north side to stopX, beside the torch's place at the north wall.
// With the torch there, it turns to face it and stops (stoppedPlace), trapped; otherwise it leaps
// over to landX and runs on to the corner at cornerX (cornerPlace).
#define DEFINE_DJINNI_WALK_AND_JUMP(                                                               \
    name, stopX, turnTo, stoppedPlace, cornerX, landX, cornerPlace)                                \
    void name(void)                                                                                \
    {                                                                                              \
        ACTOR *djinni = fdActorGetById(ACTOR_BREATH);                                              \
        Uint8 *place;                                                                              \
        Sint32 flag;                                                                               \
                                                                                                   \
        scShrineOfTheSeaGodStartDjinniDash();                                                      \
        fdCameraMoveToFX(toFIXED(stopX), toFIXED(16), toFIXED(216), 1);                            \
        fdActorWalkToAndWait(ACTOR_BREATH, stopX, 216);                                            \
        flag  = syGameFlagGet(FLAG_TORCH_AT_NORTH_WALL);                                           \
        place = &djinni->sceneCount;                                                               \
        if (flag)                                                                                  \
        {                                                                                          \
            djinni->facing = turnTo;                                                               \
            fdActorSetAttachedEffect(ACTOR_BREATH,                                                 \
                                     ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);             \
            fdScriptWait(20);                                                                      \
            *place = stoppedPlace;                                                                 \
            syGameFlagSet(FLAG_DJINNI_TRAPPED);                                                    \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            fdScriptWait(5);                                                                       \
            fdCameraMoveWithinMapBounds(toFIXED(cornerX), toFIXED(16), toFIXED(216), 1);           \
            sySoundPlay(SE_HIGH_JUMP);                                                             \
            fdActorSetSpriteFlags(fdActorGetById(ACTOR_BREATH), 0);                                \
            fdActorSetSpeed(ACTOR_BREATH, 4.8);                                                    \
            djinni->velocityY = toFIXED(8);                                                        \
            fdActorMoveToAndWaitById(ACTOR_BREATH, landX, 216);                                    \
            fdActorSetSpriteFlags(fdActorGetById(ACTOR_BREATH), SPRITE_FLAG_SHADOW);               \
            fdScriptWait(10);                                                                      \
            fdActorSetSpeed(ACTOR_BREATH, 1.6);                                                    \
            fdActorWalkToAndWait(ACTOR_BREATH, cornerX, 216);                                      \
            *place = cornerPlace;                                                                  \
        }                                                                                          \
        scShrineOfTheSeaGodEndDjinniDash();                                                        \
    }

// With Breath at the north-east corner, the leader steps onto the east side's north end: Breath
// runs west along the north side and leaps the torch's place to the north-west corner, unless the
// torch stands there.
DEFINE_DJINNI_WALK_AND_JUMP(scShrineOfTheSeaGodDjinniNorthEastJumpsWest,
                            360,
                            FACING_WEST,
                            BREATH_STOPPED_EAST_OF_TORCH,
                            296,
                            328,
                            BREATH_AT_NORTHWEST_CORNER)

// A dash of Breath's from the corner at fromX round the ledge's side, by its middle row, to (toX,
// toZ), where it stands at place.
#define DEFINE_DJINNI_DASH_AROUND(name, toX, toZ, fromX, place)                                    \
    void name(void)                                                                                \
    {                                                                                              \
        scShrineOfTheSeaGodStartDjinniDash();                                                      \
        fdCameraMoveWithinMapBounds(toFIXED(toX), toFIXED(16), toFIXED(toZ), 1);                   \
        fdActorWalkToAndWait(ACTOR_BREATH, fromX, 248);                                            \
        fdActorWalkToAndWait(ACTOR_BREATH, toX, 248);                                              \
        fdActorWalkToAndWaitById(ACTOR_BREATH, toX, toZ);                                          \
        fdActorGetById(ACTOR_BREATH)->sceneCount = place;                                          \
        scShrineOfTheSeaGodEndDjinniDash();                                                        \
    }

// With Breath at the north-west corner, the leader steps onto the north side's west end: Breath
// dashes round to the south-west corner.
DEFINE_DJINNI_DASH_AROUND(
    scShrineOfTheSeaGodDjinniNorthWestToSouthWest, 280, 296, 296, BREATH_AT_SOUTHWEST_CORNER)

// With Breath at the south-west corner, the leader steps onto the west side's south end: Breath
// dashes east to the south-east corner.
DEFINE_DJINNI_DASH(scShrineOfTheSeaGodDjinniSouthWestToSouthEast,
                   392,
                   296,
                   BREATH_AT_SOUTHEAST_CORNER)

// With Breath at the south-east corner, the leader steps onto the east side's south end: Breath
// dashes west to the south-west corner.
DEFINE_DJINNI_DASH(scShrineOfTheSeaGodDjinniSouthEastToSouthWest,
                   280,
                   296,
                   BREATH_AT_SOUTHWEST_CORNER)

// With Breath at the south-west corner, the leader comes up the south-west way: Breath dashes round
// to the north-west corner.
DEFINE_DJINNI_DASH_AROUND(
    scShrineOfTheSeaGodDjinniSouthWestToNorthWest, 296, 216, 280, BREATH_AT_NORTHWEST_CORNER)

// With Breath at the north-west corner, the leader steps onto the west side's north end: Breath
// runs east along the north side and leaps the torch's place to the north-east corner, unless the
// torch stands there.
DEFINE_DJINNI_WALK_AND_JUMP(scShrineOfTheSeaGodDjinniNorthWestJumpsEast,
                            328,
                            FACING_EAST,
                            BREATH_STOPPED_WEST_OF_TORCH,
                            392,
                            360,
                            BREATH_AT_NORTHEAST_CORNER)

// With Breath at the north-east corner, the leader steps onto the north side's east end: Breath
// dashes south to the south-east corner.
DEFINE_DJINNI_DASH(scShrineOfTheSeaGodDjinniNorthEastToSouthEast,
                   392,
                   296,
                   BREATH_AT_SOUTHEAST_CORNER)

// As DEFINE_DJINNI_WALK_AND_JUMP for Breath already beside the torch's place.
#define DEFINE_DJINNI_JUMP(name, turnTo, stoppedPlace, cornerX, landX, cornerPlace)                \
    void name(void)                                                                                \
    {                                                                                              \
        ACTOR *djinni = fdActorGetById(ACTOR_BREATH);                                              \
        Uint8 *place;                                                                              \
        Sint32 flag;                                                                               \
                                                                                                   \
        scShrineOfTheSeaGodStartDjinniDash();                                                      \
        flag  = syGameFlagGet(FLAG_TORCH_AT_NORTH_WALL);                                           \
        place = &djinni->sceneCount;                                                               \
        if (flag)                                                                                  \
        {                                                                                          \
            djinni->facing = turnTo;                                                               \
            fdActorSetAttachedEffect(ACTOR_BREATH,                                                 \
                                     ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);             \
            fdScriptWait(20);                                                                      \
            *place = stoppedPlace;                                                                 \
            syGameFlagSet(FLAG_DJINNI_TRAPPED);                                                    \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            fdCameraMoveWithinMapBounds(toFIXED(cornerX), toFIXED(16), toFIXED(216), 1);           \
            sySoundPlay(SE_HIGH_JUMP);                                                             \
            fdActorSetSpriteFlags(fdActorGetById(ACTOR_BREATH), 0);                                \
            fdActorSetSpeed(ACTOR_BREATH, 4.8);                                                    \
            djinni->velocityY = toFIXED(8);                                                        \
            fdActorMoveToAndWaitById(ACTOR_BREATH, landX, 216);                                    \
            fdActorSetSpriteFlags(fdActorGetById(ACTOR_BREATH), SPRITE_FLAG_SHADOW);               \
            fdScriptWait(10);                                                                      \
            fdActorSetSpeed(ACTOR_BREATH, 1.6);                                                    \
            fdActorWalkToAndWait(ACTOR_BREATH, cornerX, 216);                                      \
            *place = cornerPlace;                                                                  \
        }                                                                                          \
        scShrineOfTheSeaGodEndDjinniDash();                                                        \
    }

// Breath, stopped east of the torch's place, tries the leap west again as the leader steps onto the
// east side's north end.
DEFINE_DJINNI_JUMP(scShrineOfTheSeaGodTrappedDjinniJumpsWest,
                   FACING_WEST,
                   BREATH_STOPPED_EAST_OF_TORCH,
                   296,
                   328,
                   BREATH_AT_NORTHWEST_CORNER)

// Breath, stopped east of the torch, dashes back to the north-east corner as the leader steps onto
// the north side's west end.
DEFINE_DJINNI_DASH(scShrineOfTheSeaGodTrappedDjinniToNorthEast,
                   392,
                   216,
                   BREATH_AT_NORTHEAST_CORNER)

// Breath, stopped west of the torch, dashes back to the north-west corner as the leader steps onto
// the north side's east end.
DEFINE_DJINNI_DASH(scShrineOfTheSeaGodTrappedDjinniToNorthWest,
                   296,
                   216,
                   BREATH_AT_NORTHWEST_CORNER)

// Breath, stopped west of the torch's place, tries the leap east again as the leader steps onto the
// west side's north end.
DEFINE_DJINNI_JUMP(scShrineOfTheSeaGodTrappedDjinniJumpsEast,
                   FACING_EAST,
                   BREATH_STOPPED_WEST_OF_TORCH,
                   392,
                   360,
                   BREATH_AT_NORTHEAST_CORNER)

// Each step onto or round the ledge sends Breath away from the leader to another corner, leaping
// the north side where the torch's place is, unless the torch stands there and stops it.
void scShrineOfTheSeaGodChaseDjinni(void)
{
    FIELD_STATE *state;
    ACTOR       *actor;

    if (!syGameFlagGet(FLAG_DJINNI_ON_LEDGE))
        return;
    if (syGameFlagGet(FLAG_DJINNI(DJINNI_BREATH)))
        return;
    state = RamHeapSlots->fieldState;
    actor = fdActorGetById(ACTOR_BREATH);
    switch (state->stepCue)
    {
        case STEP_LEDGE_SOUTHEAST_WAY_UP:
            if (actor->sceneCount == BREATH_AT_SOUTHEAST_CORNER)
                scShrineOfTheSeaGodDjinniSouthEastToNorthEast();
            syGameFlagClear(FLAG_DJINNI_TRAPPED);
            break;
        case STEP_LEDGE_EAST_SIDE_SOUTH:
            if (actor->sceneCount == BREATH_AT_SOUTHEAST_CORNER)
                scShrineOfTheSeaGodDjinniSouthEastToSouthWest();
            break;
        case STEP_LEDGE_EAST_SIDE_NORTH:
        {
            Uint8 current = actor->sceneCount;

            if (current == BREATH_AT_NORTHEAST_CORNER)
                scShrineOfTheSeaGodDjinniNorthEastJumpsWest();
            else if (current == BREATH_STOPPED_EAST_OF_TORCH && !syGameFlagGet(FLAG_DJINNI_TRAPPED))
                scShrineOfTheSeaGodTrappedDjinniJumpsWest();
        }
        break;
        case STEP_LEDGE_NORTH_SIDE_EAST:
        {
            Uint8 current = actor->sceneCount;

            if (current == BREATH_AT_NORTHEAST_CORNER)
                scShrineOfTheSeaGodDjinniNorthEastToSouthEast();
            else if (current == BREATH_STOPPED_WEST_OF_TORCH)
                scShrineOfTheSeaGodTrappedDjinniToNorthWest();
        }
        break;
        case STEP_LEDGE_NORTH_SIDE_WEST:
        {
            Uint8 current = actor->sceneCount;

            if (current == BREATH_AT_NORTHWEST_CORNER)
                scShrineOfTheSeaGodDjinniNorthWestToSouthWest();
            else if (current == BREATH_STOPPED_EAST_OF_TORCH)
                scShrineOfTheSeaGodTrappedDjinniToNorthEast();
        }
        break;
        case STEP_LEDGE_WEST_SIDE_NORTH:
        {
            Uint8 current = actor->sceneCount;

            if (current == BREATH_AT_NORTHWEST_CORNER)
                scShrineOfTheSeaGodDjinniNorthWestJumpsEast();
            else if (current == BREATH_STOPPED_WEST_OF_TORCH && !syGameFlagGet(FLAG_DJINNI_TRAPPED))
                scShrineOfTheSeaGodTrappedDjinniJumpsEast();
        }
        break;
        case STEP_LEDGE_WEST_SIDE_SOUTH:
            if (actor->sceneCount == BREATH_AT_SOUTHWEST_CORNER)
                scShrineOfTheSeaGodDjinniSouthWestToSouthEast();
            break;
        case STEP_LEDGE_SOUTHWEST_WAY_UP:
            if (actor->sceneCount == BREATH_AT_SOUTHWEST_CORNER)
                scShrineOfTheSeaGodDjinniSouthWestToNorthWest();
            syGameFlagClear(FLAG_DJINNI_TRAPPED);
            break;
    }
}

extern SHRINE_OF_THE_SEA_GOD_SPOT ShrineOfTheSeaGodFootprintsTable[];

// Lights the torch: the pedestal in its burning animation, and the marker on its flame in its empty
// picture, disabled.
void                              scShrineOfTheSeaGodLightTorch(void)
{
    fdActorSetAnimationById(ACTOR_TORCH, 2);
    fdActorSetAnimationById(ACTOR_TORCH_FLAME_MARKER, 0);
    fdActorDisableById(ACTOR_TORCH_FLAME_MARKER);
}

// Casting Sand in the rope room turns Felix to sand as anywhere, and here also moves him onto the
// map's overhead layer. That layer's metatiles here (behaviour 232, ringed by metatiles 16 pixels
// down) mark out a winding way under the ridge of rock round the south ladder, metatiles (13, 21)
// to (25, 23) and (21, 18) to (20, 27), reaching the floor west of the ridge, the corridor south of
// it and the ledge east of it. Cast on the floor west of the ridge, at metatile (17, 21), Felix
// sinks into a mound of sand; steered east, then north, then east, the mound slides in under the
// ridge's rocky rim, comes out on its east side and goes on east along the floor to the room's east
// wall below the east ladder, still sand. Steered north off the way onto the floor by the water, he
// rises out of the sand. Cast off the way, as by the south ladder, he sinks and rises again in
// place a second later, whatever key is held (fdPsynergySand).
void scShrineOfTheSeaGodCastSand(void)
{
    fdActorGetById(MainRoutineStateWork.leaderActorId)->layer = 2;
}

// As Sand wears off, Felix rises out of the sand and walks on the ground layer again.
void scShrineOfTheSeaGodEndSand(void)
{
    fdActorGetById(MainRoutineStateWork.leaderActorId)->layer = 0;
}

// Sets up the room being entered.
Sint32 scInit(void)
{
    Sint16 scene;

    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    scene = MainRoutineStateWork.next;
    if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_ROPE)
        scShrineOfTheSeaGodInitRope();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE)
        scShrineOfTheSeaGodInitPlankBridge();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_CROSSROADS)
        scShrineOfTheSeaGodInitCrossroads();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS)
        scShrineOfTheSeaGodInitFootprints();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_BREATH)
        scShrineOfTheSeaGodInitBreath();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE)
        scShrineOfTheSeaGodInitPillarHole();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_PILLARS)
        scShrineOfTheSeaGodInitPillars();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR)
        scShrineOfTheSeaGodInitSeaGodsTear();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_STATUE)
        scShrineOfTheSeaGodInitStatue();
    else if (scene == SCENE_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG)
        scShrineOfTheSeaGodInitRightProng();
    return 0;
}

// Sets up a bridge: twelve pixels up, drawn under the leader, disabled.
ACTOR *scShrineOfTheSeaGodSetUpBridge(Sint32 id)
{
    ACTOR *actor          = fdActorGetById(id);

    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->y.fixed        = toFIXED(12);
    fdActorDisableById(id);
    return actor;
}

#include "shared/actor_hide.h"

// Back in the hole room with the party kept where it was, stops drawing its unseen blocks unless
// they were drawn; on a fresh arrival, notes them drawn.
void scRunUnlessFlag(void)
{
    if (syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
    {
        if (syGameFlagGet(FLAG_BLOCKS_SHOWN) == 0)
            scShrineOfTheSeaGodHideBlocks();
    }
    else
    {
        syGameFlagSet(FLAG_BLOCKS_SHOWN);
    }
}

// Hides the hole room's three unseen blocks behind the room.
void scShrineOfTheSeaGodSetUpBlocks(void)
{
    Uint32 i;

    for (i = 0; i <= ACTOR_HOLE_ROOM_THIRD_BLOCK - ACTOR_HOLE_ROOM_FIRST_BLOCK; i++)
        scActorHide(ACTOR_HOLE_ROOM_FIRST_BLOCK + i);
    scRunUnlessFlag();
}

// Lays the Djinni's footprints across the room below the crossroads, drawn dark on the sand.
void scShrineOfTheSeaGodPlaceFootprints(void)
{
    const SHRINE_OF_THE_SEA_GOD_SPOT *spot = ShrineOfTheSeaGodFootprintsTable;
    Uint32                            i;

    for (i = 0; i <= ACTOR_LAST_FOOTPRINT - ACTOR_FIRST_FOOTPRINT; i++)
    {
        Sint32     id    = i + ACTOR_FIRST_FOOTPRINT;
        ACTOR     *actor = fdActorGetById(id);
        FIELD_SPR *sprite;

        fdActorDisableById(id);
        fdActorSetAnimationById(id, 0);
        fdActorSetSpritePriority(id, 3);
        actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        fdActorSetSpriteColor(fdActorGetById(id), SPRITE_COLOR_DARK);
        sprite           = actor->sprite;
        actor->x.fixed   = spot->x;
        actor->z.fixed   = spot->z;
        actor->scaleX    = spot->scaleX;
        sprite->rotation = spot->rotation;
        spot++;
    }
}

// Sets up the rope room: its floating logs, the rope, and Riki and Tavi gone once Riki has given
// up.
void scShrineOfTheSeaGodInitRope(void)
{
    MAP_STATE *map = RamHeapSlots->mapGrid;

    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ROPE_ROOM_FIRST_LOG);
    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ROPE_ROOM_SECOND_LOG);
    fdMapSetUpperLevelWading(map);
    syFieldSetTickEvent(scShrineOfTheSeaGodTrackUpperLevel, EVENT_PASS_TICK);
    fdRopeInit();
    fdRopePlace(0, FLAG_SHRINE_ROPE_TIED, ACTOR_ROPE_ROOM_EAST_POST, ACTOR_ROPE_ROOM_WEST_POST);
    if (syGameFlagGet(FLAG_RIKI_GAVE_UP))
    {
        fdActorSetHorizontalPosWithTerrain(ACTOR_RIKI, 0, 0);
        fdActorSetHorizontalPosWithTerrain(ACTOR_TAVI, 0, 0);
    }
}

// In renders of the room once Riki has given up on the rope, A pressed beside him from the west
// opens the field menu, and from the east, behind him, he says "Wah! You scared me!". The side
// room's event: Riki, looking west, can be talked to and read only from behind; while he stands
// east of the leader he is no target.
void scShrineOfTheSeaGodUpdateRikiTarget(void)
{
    ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
    ACTOR *riki   = fdActorGetById(ACTOR_SIDE_ROOM_RIKI);

    if (riki->x.fixed > leader->x.fixed)
        riki->collisionFlags |= ACTOR_NOT_TARGETED;
    else
        riki->collisionFlags &= ~ACTOR_NOT_TARGETED;
}

// Sets up the side room: the plank bridge down over the stream, the torch, and Riki looking around
// once he has given up on the rope.
void scShrineOfTheSeaGodInitPlankBridge(void)
{
    scShrineOfTheSeaGodSetUpBridge(ACTOR_SIDE_ROOM_BRIDGE)->y.fixed = -toFIXED(16);
    fdActorSetSpritePriority(ACTOR_SIDE_ROOM_BRIDGE, 3);
    fdActorPlaceBlocks(ShrineOfTheSeaGodPushedActors);
    scShrineOfTheSeaGodLightTorch();
    syFieldSetTickEvent(scShrineOfTheSeaGodKeepFlameOnTorch, EVENT_PASS_TICK);
    fdActorDisableById(ACTOR_SIDE_ROOM_RIKI);
    // UB: the room places no ACTOR_SIDE_ROOM_DISABLED, so fdActorGetById returns NULL and these
    // stores go through it.
#if !FIX_NULL_DEREFERENCE
    fdActorDisableById(ACTOR_SIDE_ROOM_DISABLED);
    fdActorGetById(ACTOR_SIDE_ROOM_DISABLED)->motionFlags = 0;
#endif
    fdMapGridCopyMetatileAttributes(3, 15, 1, 1, 6, 15);
    if (syGameFlagGet(FLAG_TAVI_RESCUED))
    {
        fdActorSetHorizontalPosWithTerrain(ACTOR_SIDE_ROOM_RIKI, 0, 0);
    }
    else if (syGameFlagGet(FLAG_RIKI_GAVE_UP))
    {
        fdActorSetPosAndFacing(ACTOR_SIDE_ROOM_RIKI, toFIXED(328), toFIXED(200), FACING_WEST);
        sySynch(1);
        fdActorSetMovementTypeById(ACTOR_SIDE_ROOM_RIKI, ShrineOfTheSeaGodRikiLooksAround);
        fdActorSetSpriteFlags(fdActorGetById(ACTOR_SIDE_ROOM_RIKI), SPRITE_FLAG_SHADOW);
        fdActorGetById(ACTOR_SIDE_ROOM_RIKI)->collisionFlags = ACTOR_SOLID;
    }
    syFieldSetTickEvent(scShrineOfTheSeaGodUpdateRikiTarget, EVENT_PASS_TICK);
}

// Sets up the crossroads: its bridges, the broken one's middle down in the water once it has
// fallen, the stone pillar where Move left it, and Breath where it fled.
void scShrineOfTheSeaGodInitCrossroads(void)
{
    ACTOR *middle;

    scShrineOfTheSeaGodSetUpBridge(ACTOR_CROSSROADS_LOG_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_CROSSROADS_FIRST_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_CROSSROADS_SECOND_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_CROSSROADS_THIRD_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS);
    middle = scShrineOfTheSeaGodSetUpBridge(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE);
    fdActorSetAnimationById(ACTOR_CROSSROADS_LOG_BRIDGE, 0);
    fdActorSetAnimationById(ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS, 0);
    fdActorSetAnimationById(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 2);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_LOG_BRIDGE, 1);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_FIRST_BRIDGE, 1);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_SECOND_BRIDGE, 1);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_THIRD_BRIDGE, 1);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_ENDS, 1);
    fdActorSetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 1);
    syFieldSetTickEvent(scShrineOfTheSeaGodLayerCrossroadsBridges, EVENT_PASS_TICK);
    if (syGameFlagGet(FLAG_CROSSROADS_BRIDGE_BROKEN))
    {
        scFieldActorSetPosAndResetMotion(middle, toFIXED(408), -toFIXED(13), toFIXED(336));
        ((FIELD_SPR *)middle->sprite)->rotation = TRIG_TURN / 16;
        fdActorSetSpritePriority(ACTOR_CROSSROADS_BROKEN_BRIDGE_MIDDLE, 3);
        middle->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        fdMapGridCopyMetatileAttributes(0, 50, 3, 1, 24, 70);
    }
    if (syGameFlagGet(FLAG_CROSSROADS_PILLAR_MOVED))
        scFieldActorSetPosAndResetMotion(
            fdActorGetById(ACTOR_CROSSROADS_PILLAR), toFIXED(200), 0, toFIXED(296));
    fdActorPlaceBlocks(ShrineOfTheSeaGodPushedActors);
    if (syGameFlagGet(FLAG_DJINNI_FLED_SOUTH))
        fdActorSetHorizontalPosWithTerrain(ACTOR_CROSSROADS_BREATH, 0, 0);
    else if (syGameFlagGet(FLAG_DJINNI_FLED_EAST))
    {
        fdActorSetPosAndFacing(ACTOR_CROSSROADS_BREATH, toFIXED(580), toFIXED(344), FACING_SOUTH);
    }
    fdEffectStartBlankOverlay(0);
    sySynch(1);
}

// Sets up the room below: Breath's footprints until it has run past.
void scShrineOfTheSeaGodInitFootprints(void)
{
    MAP_STATE *map = RamHeapSlots->mapGrid;

    if (syGameFlagGet(FLAG_DJINNI_RAN_PAST) == 0)
        scShrineOfTheSeaGodPlaceFootprints();
    fdMapSetUpperLevelWading(map);
    syFieldSetTickEvent(scShrineOfTheSeaGodTrackUpperLevel, EVENT_PASS_TICK);
}

// Sets up the ledge room: the bridge, under the leader when he comes up the north-east or west
// stairs and over him otherwise, the puddles, frozen or not, the torch, and Breath where the chase
// left it.
void scShrineOfTheSeaGodInitBreath(void)
{
    ACTOR *bridge = scShrineOfTheSeaGodSetUpBridge(ACTOR_LEDGE_ROOM_BRIDGE);

    if (MainRoutineStateWork.entrance
            >= SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST
        && MainRoutineStateWork.entrance
               <= SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST)
    {
        fdActorSetSpritePriority(ACTOR_LEDGE_ROOM_BRIDGE, 3);
        bridge->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    }
    else
    {
        fdActorSetSpritePriority(ACTOR_LEDGE_ROOM_BRIDGE, 1);
        bridge->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    }
    scShrineOfTheSeaGodSetUpPuddle(ACTOR_FIRST_PUDDLE);
    scShrineOfTheSeaGodSetUpPuddle(ACTOR_SECOND_PUDDLE);
    scShrineOfTheSeaGodSetUpPuddle(ACTOR_THIRD_PUDDLE);
    if (syGameFlagGet(FLAG_SHRINE_FIRST_PUDDLE_FROZEN))
        fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 8, 18);
    if (syGameFlagGet(FLAG_SHRINE_SECOND_PUDDLE_FROZEN))
        fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 10, 18);
    if (syGameFlagGet(FLAG_SHRINE_THIRD_PUDDLE_FROZEN))
        fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 10, 17);
    syFieldSetTickEvent(scShrineOfTheSeaGodLayerPuddles, EVENT_PASS_TICK);
    fdActorPlaceBlocks(ShrineOfTheSeaGodPushedActors);
    scShrineOfTheSeaGodLightTorch();
    syFieldSetTickEvent(scShrineOfTheSeaGodKeepFlameOnTorch, EVENT_PASS_TICK);
    if (syGameFlagGet(FLAG_DJINNI(DJINNI_BREATH)) || syGameFlagGet(FLAG_FIGHTING_BREATH))
    {
        syGameFlagSet(FLAG_DJINNI_CHASE_OVER);
    }
    else if (syGameFlagGet(FLAG_DJINNI_ON_LEDGE))
    {
        ACTOR *djinni = fdActorGetById(ACTOR_BREATH);
        Sint32 moved  = syGameFlagGet(FLAG_KEEP_PARTY_POSITION);

        // A fresh arrival finds Breath back at the south-east corner.
        if (moved == 0)
        {
            djinni->sceneCount = moved;
            fdActorSetPos(ACTOR_BREATH, 392, 296);
        }
        fdActorWatchAndSetScript(ACTOR_BREATH, MainRoutineStateWork.leaderActorId);
    }
    else if (syGameFlagGet(FLAG_DJINNI_BY_LEDGE))
    {
        fdActorSetPosAndFacing(ACTOR_BREATH, toFIXED(280), toFIXED(328), FACING_SOUTH);
    }
}

// Sets up the hole room: the pillar, in its hole once pushed there, and the unseen blocks hidden;
// the leader stays in front of rising ground.
void scShrineOfTheSeaGodInitPillarHole(void)
{
    Sint32 open;

    fdActorGetById(MainRoutineStateWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    fdActorSetSpriteFlags(fdActorGetById(ACTOR_HOLE_ROOM_PILLAR), 0);
    open = syGameFlagGet(FLAG_PILLAR_IN_HOLE);
    if (open)
    {
        fdActorSetPos(ACTOR_HOLE_ROOM_PILLAR, 248, 264);
        fdActorSetSpritePriority(ACTOR_HOLE_ROOM_PILLAR, 3);
        fdActorGetById(ACTOR_HOLE_ROOM_PILLAR)->motionFlags = 0;
        fdMapGridCopyMetatileAttributes(0, 0, 1, 1, 15, 16);
        fdActorDisableById(ACTOR_HOLE_ROOM_PILLAR);
    }
    else
    {
        fdActorSetSpritePriority(ACTOR_HOLE_ROOM_PILLAR, 1);
        fdActorGetById(ACTOR_HOLE_ROOM_PILLAR)->motionFlags = open;
    }
    scShrineOfTheSeaGodSetUpBlocks();
}

// Sets up the pillar room: its bridges, the broken ones' middles down in the stream once fallen,
// the room's two ropes, and the splashes, out of sight.
void scShrineOfTheSeaGodInitPillars(void)
{
    ACTOR *firstSplash;
    ACTOR *secondSplash;

    fdMapSetUpperLevelWading((MAP_STATE *)RamHeapSlots->mapGrid);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_PILLAR_ROOM_FIRST_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_PILLAR_ROOM_SECOND_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_PILLAR_ROOM_THIRD_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_PILLAR_ROOM_FOURTH_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_PILLAR_ROOM_LOG_BRIDGE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_FIRST_BROKEN_BRIDGE_ENDS);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_SECOND_BROKEN_BRIDGE_ENDS);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE);
    scShrineOfTheSeaGodSetUpBridge(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE);
    fdActorSetAnimationById(ACTOR_FIRST_BROKEN_BRIDGE_ENDS, 0);
    fdActorSetAnimationById(ACTOR_SECOND_BROKEN_BRIDGE_ENDS, 0);
    fdActorSetAnimationById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE, 2);
    fdActorSetAnimationById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE, 2);
    if (syGameFlagGet(FLAG_FIRST_BRIDGE_BROKEN))
    {
        fdMapCopyMetatileAttributeRect(0, 0, 3, 1, 19, 13);
        fdMapCopyMetatileAttributeRect(40, 0, 2, 1, 19, 33);
        fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE),
                                    toFIXED(320),
                                    -toFIXED(21),
                                    toFIXED(534));
        fdActorSetSpritePriority(ACTOR_FIRST_BROKEN_BRIDGE_MIDDLE, 3);
    }
    if (syGameFlagGet(FLAG_SECOND_BRIDGE_BROKEN))
    {
        fdMapCopyMetatileAttributeRect(0, 0, 3, 1, 24, 22);
        fdMapCopyMetatileAttributeRect(40, 0, 2, 1, 24, 24);
        fdActorSetPosAndResetMotion(fdActorGetById(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE),
                                    toFIXED(400),
                                    -toFIXED(21),
                                    toFIXED(390));
        fdActorSetSpritePriority(ACTOR_SECOND_BROKEN_BRIDGE_MIDDLE, 3);
    }
    fdRopeInit();
    Call4((void (*)())fdRopePlace,
          0,
          FLAG_SHRINE_ROPE_TIED,
          ACTOR_SOUTH_ROPE_EAST_POST,
          ACTOR_SOUTH_ROPE_WEST_POST);
    fdRopePlace(
        1, FLAG_SHRINE_SECOND_ROPE_TIED, ACTOR_NORTH_ROPE_WEST_POST, ACTOR_NORTH_ROPE_EAST_POST);
    fdActorDisableById(ACTOR_FIRST_BRIDGE_SPLASH);
    fdActorGetById(ACTOR_FIRST_BRIDGE_SPLASH)->motionFlags = 0;
    firstSplash = fdActorGetById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH);
    fdActorDisableById(ACTOR_FIRST_BRIDGE_OTHER_SPLASH);
    firstSplash->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    firstSplash->motionFlags    = 0;
    fdActorDisableById(ACTOR_SECOND_BRIDGE_SPLASH);
    fdActorGetById(ACTOR_SECOND_BRIDGE_SPLASH)->motionFlags = 0;
    secondSplash = fdActorGetById(ACTOR_SECOND_BRIDGE_OTHER_SPLASH);
    fdActorDisableById(ACTOR_SECOND_BRIDGE_OTHER_SPLASH);
    secondSplash->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    secondSplash->motionFlags    = 0;
}

// Sets up the island room: its floating logs, and the water sinking back when the party comes down
// from the statue room.
void scShrineOfTheSeaGodInitSeaGodsTear(void)
{
    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ISLAND_FIRST_LOG);
    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ISLAND_SECOND_LOG);
    scShrineOfTheSeaGodSetUpFloatingLog(ACTOR_ISLAND_THIRD_LOG);
    if (MainRoutineStateWork.entrance
            == SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_STATUE
        && syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
        scShrineOfTheSeaGodLowerWater();
}

// Sets up the statue room: the party rises on the water when it comes up from the island.
void scShrineOfTheSeaGodInitStatue(void)
{
    if (MainRoutineStateWork.entrance
            == SHRINE_OF_THE_SEA_GOD_STATUE_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR
        && syGameFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
        scShrineOfTheSeaGodRiseOnWater();
}

#include "shared/prong_sparkle.h"

// Sets up the Right Prong's room: the Prong floats above its place, shedding sparkles.
void scShrineOfTheSeaGodInitRightProng(void)
{
    ACTOR     *actor  = fdActorGetById(FIELD_TREASURE_ACTOR);
    MAP_STATE *map    = RamHeapSlots->mapGrid;
    MAP_LAYER *layer  = &map->layers[2];

    layer->offsetX   += toFIXED(128);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    if (actor != NULL)
    {
        actor->motionFlags = 0;
        actor->y.fixed     = toFIXED(66);
        actor->update      = scProngSparkle;
    }
}

// Sets up a floating log: flat on the water, behind the leader, disabled.
void scShrineOfTheSeaGodSetUpFloatingLog(Sint32 id)
{
    ACTOR *actor = fdActorGetById(id);

    fdActorSetSpritePriority(id, 3);
    fdActorDisableById(id);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags    = 0;
    actor->scaleX         = toFIXED(8) / 10;
    actor->scaleY         = toFIXED(9) / 10;
}

// Back from the battle with Breath: once it is beaten, the one enemy defeated, it is put back where
// the torch trapped it, facing away.
Sint32 scPrepareMap(void)
{
    if (syGameFlagGet(FLAG_FIGHTING_BREATH))
    {
        if (MainRoutineStateWork.modeResult == 1)
        {
            if (syGameFlagGet(FLAG_BREATH_TRAPPED_EAST))
                fdActorSetPosAndFacing(ACTOR_BREATH, toFIXED(360), toFIXED(216), FACING_EAST);
            else if (syGameFlagGet(FLAG_BREATH_TRAPPED_WEST))
            {
                fdActorSetPosAndFacing(ACTOR_BREATH, toFIXED(328), toFIXED(216), FACING_WEST);
            }
        }
        syGameFlagClear(FLAG_FIGHTING_BREATH);
        syGameFlagClear(FLAG_BREATH_TRAPPED_EAST);
        syGameFlagClear(FLAG_BREATH_TRAPPED_WEST);
    }
    return 0;
}

#undef DEFINE_DJINNI_DASH
#undef DEFINE_DJINNI_WALK_AND_JUMP
#undef DEFINE_DJINNI_DASH_AROUND
#undef DEFINE_DJINNI_JUMP

extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectLongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

// Riki looks about the room, north-north-west, south-south-west and west, for ever.
Uint32 ShrineOfTheSeaGodRikiLooksAround[] = {
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(120),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// The boys walk home, out by the room's south way.
Uint32 ShrineOfTheSeaGodBoysGoHome[] = {
    SC_ANIMATION(2),
    SC_MOVE(344, 0, 198),
    SC_SYNC,
    SC_MOVE(378, 0, 198),
    SC_SYNC,
    SC_MOVE(392, 0, 220),
    SC_SYNC,
    SC_MOVE(392, 0, 284),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_END,
};

// What a push or Move lifts and sets down: each room's first placed actor, the torch, or in the crossroads its stone pillar.
Sint16 ShrineOfTheSeaGodPushedActors[] = {
    ACTOR_TORCH, -1,
};

// The Djinni's footprints, laid by scShrineOfTheSeaGodPlaceFootprints: where each lies, mirrored (a scale of -1) or not, and its turn.
SHRINE_OF_THE_SEA_GOD_SPOT ShrineOfTheSeaGodFootprintsTable[] = {
    {toFIXED(316), toFIXED(136), -toFIXED(1), TRIG_TURN * 16 / 32},
    {toFIXED(318), toFIXED(142), toFIXED(1), TRIG_TURN * 14 / 32},
    {toFIXED(324), toFIXED(154), -toFIXED(1), TRIG_TURN * 12 / 32},
    {toFIXED(330), toFIXED(158), toFIXED(1), TRIG_TURN * 10 / 32},
    {toFIXED(338), toFIXED(166), -toFIXED(1), TRIG_TURN * 9 / 32},
    {toFIXED(344), toFIXED(166), toFIXED(1), TRIG_TURN * 9 / 32},
    {toFIXED(288), toFIXED(240), toFIXED(1), TRIG_TURN * 27 / 32},
    {toFIXED(284), toFIXED(236), -toFIXED(1), TRIG_TURN * 27 / 32},
    {toFIXED(272), toFIXED(232), toFIXED(1), TRIG_TURN * 25 / 32},
    {toFIXED(266), toFIXED(230), -toFIXED(1), TRIG_TURN * 25 / 32},
    {toFIXED(256), toFIXED(238), toFIXED(1), TRIG_TURN * 22 / 32},
    {toFIXED(248), toFIXED(238), -toFIXED(1), TRIG_TURN * 23 / 32},
    {toFIXED(240), toFIXED(250), toFIXED(1), TRIG_TURN * 21 / 32},
    {toFIXED(234), toFIXED(252), -toFIXED(1), TRIG_TURN * 22 / 32},
    {toFIXED(224), toFIXED(262), toFIXED(1), TRIG_TURN * 21 / 32},
    {toFIXED(220), toFIXED(262), -toFIXED(1), TRIG_TURN * 21 / 32},
};

// Unused: the halfwords after the last footprint.
Sint16 ShrineOfTheSeaGodFootprintsEnd[] = {-1, 0};

#include "shared/scene_entrances_default.c"

SCENE_REGION ShrineOfTheSeaGodStatueExitRegions[] = {
    {256, 32, 192, 272, 48, 208, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_SOUTH},
    {-1},
};

SCENE_REGION ShrineOfTheSeaGodLaddersExitRegions[] = {
    {720, -38, 238, 736, -22, 254, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_STATUE_SOUTH},
    {864, 32, 208, 880, 48, 224, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_WEST},
    {-1},
};

SCENE_REGION ShrineOfTheSeaGodRightProngExitRegions[] = {
    {784, -38, 160, 800, -22, 176, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_EAST},
    {-1},
};

Uint32 SceneExits[] = {
    SCENE_EXITS(ShrineOfTheSeaGodRope),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_ROPE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE, ShrineOfTheSeaGodPlankBridge, SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_FROM_SHRINE_OF_THE_SEA_GOD_ROPE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_ROPE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SHRINE_OF_THE_SEA_GOD_ROPE),
    SCENE_EXITS(ShrineOfTheSeaGodPlankBridge),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_TO_SHRINE_OF_THE_SEA_GOD_ROPE, ShrineOfTheSeaGodRope, SHRINE_OF_THE_SEA_GOD_ROPE_FROM_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE),
    SCENE_EXITS(ShrineOfTheSeaGodCrossroads),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_PILLARS, ShrineOfTheSeaGodPillars, SHRINE_OF_THE_SEA_GOD_PILLARS_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST, ShrineOfTheSeaGodSeaGodsTear, SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST, ShrineOfTheSeaGodSeaGodsTear, SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_ROPE, ShrineOfTheSeaGodRope, SHRINE_OF_THE_SEA_GOD_ROPE_FROM_SHRINE_OF_THE_SEA_GOD_CROSSROADS),
    SCENE_EXITS(ShrineOfTheSeaGodFootprints),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_WEST, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST),
    SCENE_EXITS(ShrineOfTheSeaGodBreath),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE, ShrineOfTheSeaGodPillarHole, SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_FROM_SHRINE_OF_THE_SEA_GOD_BREATH),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_BREATH_WEST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST, ShrineOfTheSeaGodFootprints, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_FROM_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST),
    SCENE_EXITS(ShrineOfTheSeaGodPillarHole),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_BREATH, ShrineOfTheSeaGodBreath, SHRINE_OF_THE_SEA_GOD_BREATH_FROM_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_PILLARS, ShrineOfTheSeaGodPillars, SHRINE_OF_THE_SEA_GOD_PILLARS_FROM_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE),
    SCENE_EXITS(ShrineOfTheSeaGodPillars),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE, ShrineOfTheSeaGodPillarHole, SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_FROM_SHRINE_OF_THE_SEA_GOD_PILLARS),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_PILLARS),
    SCENE_EXITS(ShrineOfTheSeaGodSeaGodsTear),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST, ShrineOfTheSeaGodCrossroads, SHRINE_OF_THE_SEA_GOD_CROSSROADS_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_STATUE, ShrineOfTheSeaGodStatue, SHRINE_OF_THE_SEA_GOD_STATUE_FROM_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR),
    SCENE_EXITS(ShrineOfTheSeaGodStatue),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR, ShrineOfTheSeaGodSeaGodsTear, SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_FROM_SHRINE_OF_THE_SEA_GOD_STATUE),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_STATUE_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_SOUTH, ShrineOfTheSeaGodLadders, SHRINE_OF_THE_SEA_GOD_LADDERS_FROM_SHRINE_OF_THE_SEA_GOD_STATUE_NORTH),
    SCENE_EXITS(ShrineOfTheSeaGodLadders),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_STATUE_SOUTH, ShrineOfTheSeaGodStatue, SHRINE_OF_THE_SEA_GOD_STATUE_FROM_SHRINE_OF_THE_SEA_GOD_LADDERS_SOUTH),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_LADDERS_TO_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_WEST, ShrineOfTheSeaGodRightProng, SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_FROM_SHRINE_OF_THE_SEA_GOD_LADDERS_WEST),
    SCENE_EXITS(ShrineOfTheSeaGodRightProng),
    SCENE_EXIT(SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_TO_SHRINE_OF_THE_SEA_GOD_LADDERS_EAST, ShrineOfTheSeaGodLadders, SHRINE_OF_THE_SEA_GOD_LADDERS_FROM_SHRINE_OF_THE_SEA_GOD_RIGHT_PRONG_EAST),
    SCENE_EXITS_END,
};

ACTOR_PLACE ShrineOfTheSeaGodPlacements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodRopeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 224, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 340, 0, 168, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TAVI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodPlankBridgeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_RIKI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodCrossroadsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_PAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodFootprintsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FOOTPRINTS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodBreathActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLAME_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_EXCLAMATION_BUBBLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_BREATH), MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodPillarHoleActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodPillarsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LOG_PAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PLANK_BRIDGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 448, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 464, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 456, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE ShrineOfTheSeaGodSeaGodsTearActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 202, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 298, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 218, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

CUE ShrineOfTheSeaGodCues[] = {
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodRopeCues[] = {
    {CUE_EXIT, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS},
    {CUE_EXIT, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_ROPE_TO_SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE},
    {CUE_EXIT, SHRINE_OF_THE_SEA_GOD_ROPE_TO_WORLD_MAP, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_ROPE_TO_WORLD_MAP},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_ROOM_EAST_POST, FLAG_SHRINE_ROPE_TIED, SCRIPT_REF(scShrineOfTheSeaGodLashRope)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scShrineOfTheSeaGodCastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scShrineOfTheSeaGodEndSand)},
    {CUE_STEP, RIKI_THROWS_ROPE_METATILE, FLAG_RIKI_GAVE_UP, SCRIPT_REF(scShrineOfTheSeaGodThrowRopeAgain)},
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodPlankBridgeCues[] = {
    {CUE_EXIT, SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_TO_SHRINE_OF_THE_SEA_GOD_ROPE, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_PLANK_BRIDGE_TO_SHRINE_OF_THE_SEA_GOD_ROPE},
    {TALK_CUE_FACING(FACING_WEST), ACTOR_SIDE_ROOM_RIKI, CONDITION_ALWAYS, SCRIPT_REF(scShrineOfTheSeaGodStartleRiki)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIDE_ROOM_RIKI, CONDITION_ALWAYS, MSGNO(MsgFieldICantFindAnythingAroundHere)},
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodCrossroadsCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_PILLARS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_PILLARS},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_EAST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS},
    {CUE_EXIT, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_ROPE, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_CROSSROADS_TO_SHRINE_OF_THE_SEA_GOD_ROPE},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CROSSROADS_PILLAR, FLAG_CROSSROADS_PILLAR_MOVED, SCRIPT_REF(scShrineOfTheSeaGodLiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CROSSROADS_PILLAR, FLAG_CROSSROADS_PILLAR_MOVED, SCRIPT_REF(scShrineOfTheSeaGodMovePillar)},
    {CUE_STEP, CROSSROADS_BRIDGE_END_METATILE, FLAG_LEADER_IN_FRONT, SCRIPT_REF(scShrineOfTheSeaGodDrawLeaderInFront)},
    {CUE_STEP, CROSSROADS_LADDER_FOOT_METATILE, IF_FLAG_SET(FLAG_LEADER_IN_FRONT), SCRIPT_REF(scShrineOfTheSeaGodDrawLeaderBehind)},
    {CUE_STEP, CROSSROADS_BROKEN_BRIDGE_METATILE, FLAG_CROSSROADS_BRIDGE_BROKEN, SCRIPT_REF(scShrineOfTheSeaGodBreakCrossroadsBridge)},
    {CUE_STEP, CROSSROADS_LADDER_FOOT_METATILE, FLAG_DJINNI_FLED_EAST, SCRIPT_REF(scShrineOfTheSeaGodDjinniFleesEast)},
    {CUE_STEP, CROSSROADS_BREATH_FLEES_SOUTH_METATILE, FLAG_DJINNI_FLED_SOUTH, SCRIPT_REF(scShrineOfTheSeaGodDjinniFleesSouth)},
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodFootprintsCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_NORTHEAST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_SOUTHEAST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_WEST, CONDITION_ALWAYS, SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_TO_SHRINE_OF_THE_SEA_GOD_BREATH_WEST},
    {CUE_TABLE_END},
};
// clang-format on

CUE           ShrineOfTheSeaGodBreathCues[] = {
    {CUE_EXIT,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_NORTHEAST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_BREATH_TO_SHRINE_OF_THE_SEA_GOD_FOOTPRINTS_SOUTHEAST},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_FIRST_PUDDLE, FLAG_SHRINE_FIRST_PUDDLE_FROZEN,
     SCRIPT_REF(scShrineOfTheSeaGodFreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_SECOND_PUDDLE, FLAG_SHRINE_SECOND_PUDDLE_FROZEN,
     SCRIPT_REF(scShrineOfTheSeaGodFreezeSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST),
     ACTOR_THIRD_PUDDLE, FLAG_SHRINE_THIRD_PUDDLE_FROZEN,
     SCRIPT_REF(scShrineOfTheSeaGodFreezeThirdPuddle)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scShrineOfTheSeaGodLiftBlock)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scShrineOfTheSeaGodSetDownTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_TORCH, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodLiftBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_TORCH, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodSetDownTorch)},
    {CUE_STEP,
     LEDGE_ROOM_SOUTH_STAIRS_METATILE, FLAG_DJINNI_BY_LEDGE,
     SCRIPT_REF(scShrineOfTheSeaGodDjinniRunsNorth)},
    {CUE_STEP,
     LEDGE_ROOM_SOUTH_PATH_METATILE, FLAG_DJINNI_BY_LEDGE,
     SCRIPT_REF(scShrineOfTheSeaGodDjinniRunsWest)},
    {CUE_STEP,
     LEDGE_ROOM_WAY_UP_METATILE, FLAG_DJINNI_ON_LEDGE,
     SCRIPT_REF(scShrineOfTheSeaGodDjinniHopsOntoLedge)},
    {CUE_STEP,
     STEP_LEDGE_SOUTHEAST_WAY_UP, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_STEP,
     STEP_LEDGE_EAST_SIDE_SOUTH, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_STEP,
     STEP_LEDGE_EAST_SIDE_NORTH, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_STEP,
     STEP_LEDGE_NORTH_SIDE_EAST, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_STEP,
     STEP_LEDGE_NORTH_SIDE_WEST, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_STEP,
     STEP_LEDGE_WEST_SIDE_NORTH, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_STEP,
     STEP_LEDGE_WEST_SIDE_SOUTH, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_STEP,
     STEP_LEDGE_SOUTHWEST_WAY_UP, FLAG_DJINNI_CHASE_OVER,
     SCRIPT_REF(scShrineOfTheSeaGodChaseDjinni)},
    {CUE_TALK, ACTOR_BREATH, CONDITION_ALWAYS, SCRIPT_REF(scShrineOfTheSeaGodFightBreath)},
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodPillarHoleCues[] = {
    {CUE_EXIT,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_BREATH, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_BREATH},
    {CUE_EXIT | EXIT_STYLE_STAIRS_DOWN,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_PILLARS, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE_TO_SHRINE_OF_THE_SEA_GOD_PILLARS},
    {CUE_PUSHED, 0, FLAG_PILLAR_IN_HOLE, SCRIPT_REF(scShrineOfTheSeaGodPushPillarIntoHole)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE),
     ACTOR_HOLE_ROOM_PILLAR, FLAG_PILLAR_IN_HOLE,
     SCRIPT_REF(scShrineOfTheSeaGodPushPillarIntoHole)},
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodPillarsCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_PILLAR_HOLE},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_PILLARS_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SOUTH_ROPE_EAST_POST, FLAG_SHRINE_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_NORTH_ROPE_WEST_POST, FLAG_SHRINE_SECOND_ROPE_TIED, 0},
    {CUE_STEP,
     FIRST_BROKEN_BRIDGE_METATILE, FLAG_FIRST_BRIDGE_BROKEN,
     SCRIPT_REF(scShrineOfTheSeaGodBreakFirstBridge)},
    {CUE_STEP,
     SECOND_BROKEN_BRIDGE_METATILE, FLAG_SECOND_BRIDGE_BROKEN,
     SCRIPT_REF(scShrineOfTheSeaGodBreakSecondBridge)},
    {CUE_TALK,
     ACTOR_CHEST_NORTHWEST_MARKER, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodOpenFieldMenu)},
    {CUE_TALK,
     ACTOR_CHEST_NORTHEAST_MARKER, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodOpenFieldMenu)},
    {CUE_TALK,
     ACTOR_CHEST_NORTH_MARKER, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodOpenFieldMenu)},
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodSeaGodsTearCues[] = {
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_WEST},
    {CUE_EXIT | EXIT_STYLE_STAIRS_UP,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST, CONDITION_ALWAYS,
     SHRINE_OF_THE_SEA_GOD_SEA_GODS_TEAR_TO_SHRINE_OF_THE_SEA_GOD_CROSSROADS_EAST},
    {CUE_EXAMINE,
     ISLAND_STATUE_METATILE, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR),
     ISLAND_STATUE_METATILE, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodUseSeaGodsTear)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodRevealStone)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodHideStone)},
    {CUE_TABLE_END},
};

CUE ShrineOfTheSeaGodStatueCues[] = {
    {CUE_EXAMINE,
     STATUE_ROOM_STATUE_METATILE, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodExamineStatue)},
    {USE_ITEM(ITEM_SEA_GODS_TEAR),
     STATUE_ROOM_STATUE_METATILE, CONDITION_ALWAYS,
     SCRIPT_REF(scShrineOfTheSeaGodSinkWithWater)},
    {CUE_TABLE_END},
};

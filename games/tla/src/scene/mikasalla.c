// Mikasalla, the village in the far south of Osenia, and the tunnels under it, in two scenes that
// share this overlay (`SceneIdMikasalla`, the village, and `SceneIdMikasallaNorth`, the tunnels).
//
// The inn is the northeast door, the general store the northwest one, and the Sanctum the wide door
// in the west cliff. Once the party has crossed the desert, Scoop in the sheep pen in the southwest
// corner digs up a ladder down to the tunnels. Parch dries up the tunnels' water, and their far end
// comes out in the cliff behind the inn, where Spark the Mars Djinni joins the party without a
// fight.

#include "field/field.h"
#include "game/game.h"
#include "field/field_mode_script.h"
#include "field/script_inline.h"
#include "field/actor_script.h"
#include "field/map_grid.h"
#include "text/msg_ids.h"

// Set once Scoop has dug up the ladder down to the tunnels by the sheep; the village redraws it on
// every visit.
#define FLAG_MIKASALLA_LADDER_DUG_UP   0x85b
// Set once Parch has dried up the water in the tunnels, for this visit.
#define FLAG_MIKASALLA_TUNNELS_PARCHED FLAG_SCENE_TEMP_00

// The ways between the tunnels' cave and the chamber above it, by the way out behind the inn, each
// an exit and the entrance that lands there: the top of the cave's ladder, and the hole in the
// chamber's floor.
enum
{
    MIKASALLA_NORTH_CAVE_LADDER  = 2,
    MIKASALLA_NORTH_CHAMBER_HOLE = 3
};

// The metatiles round a ladder top that climb Felix down it, by the way he walks in. The dug ladder
// in the sheep pen has the first three, round its top at metatile (5, 40); the hole in the tunnels'
// chamber floor has the last two, at metatiles (25, 7) to (27, 7).
enum
{
    METATILE_LADDER_TOP_FROM_NORTH = 30,
    METATILE_LADDER_TOP_FROM_WEST,
    METATILE_LADDER_TOP_FROM_EAST,
    METATILE_LADDER_TOP_FROM_SOUTH,
    METATILE_LADDER_TOP_FROM_SOUTH_BESIDE
};

// The patch of ground in the sheep pen that Scoop digs, metatile (5, 41).
#define METATILE_BURIED_LADDER 50
// The tunnels' water, which Parch dries.
#define METATILE_TUNNEL_WATER  60

extern SCENE_REGION MikasallaTunnelRegions[];
extern ACTOR_PLACE  MikasallaVillagePlacements[];
extern ACTOR_PLACE  MikasallaTunnelPlacements[];

extern CUE          MikasallaTunnelCues[];

extern CUE          MikasallaVillageCues[];
extern Uint16       MikasallaDoorOpening[];
extern Uint16       MikasallaThirdDoorOpening[];
extern Uint16       MikasallaWideDoorOpening[];
extern Sint32       MikasallaChickenPaces[];
extern Sint32       MikasallaSheepPaces[];

// The village's actors, numbering the rows of `MikasallaVillagePlacements`.
typedef enum MIKASALLA_ACTOR_ID
{
    ACTOR_VILLAGER_NO_VISITORS = ACTOR_FIRST_PLACED,
    // scMikasallaAskLookingForSomeone's villager.
    ACTOR_VILLAGER_LOOKING_FOR_SOMEONE,
    ACTOR_VILLAGER_AFTER_EARTHQUAKE,
    ACTOR_VILLAGER_DEEPER_INTO_OSENIA,
    ACTOR_VILLAGER_AIRS_ROCK,
    ACTOR_VILLAGER_TWO_WAYS,
    ACTOR_VILLAGER_TUNDARIA,
    ACTOR_VILLAGER_GAROH,
    ACTOR_CHICKEN,
    ACTOR_SHEEP,
    ACTOR_SPARK
} MIKASALLA_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_MIKASALLA_NORTH)
        return MikasallaTunnelRegions;
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_MIKASALLA)
        return MikasallaVillagePlacements;
    return MikasallaTunnelPlacements;
}

void                               scMikasallaClimbDownLadder(void);

extern Uint8                       MsgFieldAreYouLookingForSomeone[];

extern MAP_GRID_METATILE_ANIMATION MikasallaDoors[];

// Felix walks north into one of the village's doors: the inn's, the northeast door at metatile (20,
// 32); the general store's, to the northwest at (13, 36); the house of the old man who climbs Air's
// Rock, to the east at (22, 40); the explorer's family's, to the southwest at (11, 43); or the
// Sanctum's wide door in the west cliff at (5, 31). Everyone stops, the door opens, and he walks
// in.
//
// Each door's metatile carries the number of the exit it leaves by, and `MikasallaDoors` holds its
// opening, drawn over its metatiles. The Sanctum's door is entered without the walk.
void                               scMikasallaEnterDoor(void)
{
    FIELD_STATE *state = RamHeapSlots->fieldState;
    Uint32       id;
    Sint16      *exit;
    Sint32       index;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    for (id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++)
    {
        ACTOR *actor = fdActorGetById(id);

        if (actor != NULL)
            actor->motionFlags = 0;
    }
    exit  = &state->stepCue;
    index = *exit;
    fdScriptPlaySound(SE_DOOR_OPEN);
    index--;
    fdMapGridAnimateMetatiles(
        MikasallaDoors[index].steps, MikasallaDoors[index].x, MikasallaDoors[index].y);
    fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
    fdActorGetById(MainRoutineStateWork.leaderActorId)->motionFlags = 0;

    fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
    if (index != MIKASALLA_TO_MIKASALLA_ROOMS_SANCTUM - 1)
    {
        fdActorCenterAndWalk(MainRoutineStateWork.leaderActorId, 2, -8);
        fdScriptWait(10);
    }
    syImportScriptSetExitRequest(*exit);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptEnd();
}

// Felix talks to Spark, the Mars Djinni on the path behind the inn, and Spark joins the party
// without a fight.
void scMikasallaMeetSpark(void)
{
    fdDjinnJoinParty(ACTOR_SPARK, DJINNI_SPARK / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_SPARK));
}

const CUE *scGetCues(void)
{
    Sint16 scene = MainRoutineStateWork.next;

    if (scene == SCENE_MIKASALLA_NORTH)
        return MikasallaTunnelCues;
    return MikasallaVillageCues;
}

// The woman in a white dress south of the inn asks Felix whether he is looking for someone; either
// way, she says strangers are rare here.
void scMikasallaAskLookingForSomeone(void)
{
    Sint32 msg;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    msg = (Sint32)&MsgFieldAreYouLookingForSomeone;
    fdScriptSetMessageId(msg);
    // "Are you looking for someone?" Yes: "Well, if you're looking for someone... I'd say the best
    // place to start would be... somewhere else." No: "Didn't think so... We don't get a lot of
    // strangers coming here."
    fdScriptOpenMessage(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
    if (fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0)
    {
        fdScriptWait(10);
        fdScriptSetMessageId(msg + 1);
    }
    else
    {
        fdScriptWait(20);
        fdScriptSetMessageId(msg + 2);
    }
    fdScriptShowMessage(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
    fdScriptEnd();
}

// Felix casts Scoop in the sheep pen in the village's southwest corner, metatile (5, 41), and digs
// up a ladder down to the tunnels under the village. It stays dug on later visits.
void scMikasallaDigUpLadder(void)
{
    fdMapGridCopyMetatilesTo(5, 49, 5, 41, 1, 1);
    fdMapGridCopyMetatileAttributes(4, 48, 3, 2, 4, 40);
    fdScriptSetFlag(FLAG_MIKASALLA_LADDER_DUG_UP);
}

// Dries up the water in the tunnels: its tile animations stop and dry ground is drawn over its
// pools.
static inline void scMikasallaDryTunnels(void)
{
    fdTilesetAnimationPause(0);
    fdTilesetAnimationPause(1);
    fdTilesetAnimationPause(2);
    fdMapGridCopyMetatilesTo(56, 46, 30, 46, 16, 16);
    fdMapGridCopyMetatilesTo(56, 174, 30, 174, 16, 16);
    fdMapGridCopyMetatiles(28, 23, 16, 16, 15, 23);
    fdMapGridCopyMetatiles(28, 90, 2, 2, 21, 88);
    fdMapGridCopyMetatiles(28, 90, 4, 4, 17, 89);
    fdMapGridCopyMetatiles(28, 90, 2, 1, 17, 91);
    fdMapGridCopyMetatiles(28, 90, 2, 2, 19, 91);
    fdMapGridCopyMetatiles(28, 92, 2, 2, 15, 92);
    fdMapGridCopyMetatiles(30, 92, 2, 2, 17, 92);
}

// Felix casts Parch on the water in the tunnels under the village: the screen flashes, and the
// water is gone, leaving bare ground to walk across. It stays dry until the party leaves, and is
// dried again on a return.
void scMikasallaCastParch(void)
{
    if (fdScriptGetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED) == 0)
    {
        scMikasallaDryTunnels();
        fdScriptSetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED);
    }
}

Sint32 scInit(void)
{
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    return 0;
}

Sint32 scPrepareMap(void)
{
    MAIN_ROUTINE_STATE *party;

    fdActorGetById(ACTOR_CHICKEN)->priorityFlags        &= ~ACTOR_PRIORITY_AUTOMATIC;
    party                                                = &MainRoutineStateWork;
    fdActorGetById(party->leaderActorId)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    if (party->next == SCENE_MIKASALLA_NORTH)
    {
        if (fdScriptGetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED))
            scMikasallaDryTunnels();
    }
    else if (fdScriptGetFlag(FLAG_MIKASALLA_LADDER_DUG_UP))
    {
        scMikasallaDigUpLadder();
    }
    return 0;
}

// Felix, at a ladder top, turns north, steps onto the rungs and climbs down out of sight: down the
// dug ladder from the sheep pen to the tunnels, or down the hole in the chamber's floor to the cave
// below.
void scMikasallaClimbDownLadder(void)
{
    Sint32 exit;
    Sint32 drop;

    if (MainRoutineStateWork.next == SCENE_MIKASALLA)
    {
        exit = MIKASALLA_TO_MIKASALLA_NORTH_SOUTH;
        drop = 17;
    }
    else
    {
        exit = MIKASALLA_NORTH_CHAMBER_HOLE;
        drop = 9;
    }
    fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    fdScriptWait(10);
    fdActorGetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
    // XXX: what do the two metatiles copied to (72, 14) show, in both the village and the tunnels?
    // A recording of each climb down, with the grid before and after, would settle it.
    fdMapGridCopyMetatilesTo(96, 14, 72, 14, 2, 1);
    fdActorSetDestinationOffset(ACTOR_FELIX, 0, 4);
    fdActorWaitForMoveById(ACTOR_FELIX);
    fdActorSetSpriteFlags(fdActorGetById(ACTOR_FELIX), 0);
    fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
    fdActorSetDestinationOffset(ACTOR_FELIX, 0, drop);
    fdActorWaitForMoveById(ACTOR_FELIX);
    fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
    fdScriptWait(10);
    fdScriptPlaySound(SE_MAP_EXIT);
    syImportScriptSetExitRequest(exit);
    fdScriptEnd();
}

// Felix walks east into the dug ladder in the sheep pen from its west side, metatile (4, 41): he
// steps up and across onto the rungs and climbs down into the tunnels.
void scMikasallaStepOntoLadderFacingEast(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorWalkByAndWait(ACTOR_FELIX, 0, -14);
    fdActorWalkByAndWait(ACTOR_FELIX, 16, 0);
    scMikasallaClimbDownLadder();
}

// Felix walks west into the dug ladder in the sheep pen from its east side, metatile (6, 41): he
// steps up and across onto the rungs and climbs down into the tunnels.
void scMikasallaStepOntoLadderFacingWest(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorWalkByAndWait(ACTOR_FELIX, -2, -6);
    fdActorWalkByAndWait(ACTOR_FELIX, -14, -8);
    scMikasallaClimbDownLadder();
}

// Felix walks south onto the top of the dug ladder in the sheep pen, metatile (5, 40): he turns
// round and climbs down into the tunnels.
void scMikasallaTurnAndClimbDownLadder(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
    scMikasallaClimbDownLadder();
}

// Felix walks north into the hole in the floor of the small chamber at the tunnels' north end,
// metatile (26, 7): he steps onto its ladder from whichever side he stands and climbs down into the
// cave below.
void scMikasallaStepOntoLadderFacingNorth(void)
{
    Sint32 x = fdActorGetById(ACTOR_FELIX)->x.part[0].integer;

    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    if (x % 16 <= 7)
    {
        fdActorWalkByAndWait(ACTOR_FELIX, -4, -4);
        fdActorWalkByAndWait(ACTOR_FELIX, -4, -8);
    }
    else
    {
        fdActorWalkByAndWait(ACTOR_FELIX, 4, -4);
        fdActorWalkByAndWait(ACTOR_FELIX, 4, -8);
    }
    fdActorWalkToAndWait(ACTOR_FELIX, 424, 99);
    scMikasallaClimbDownLadder();
}

// Felix walks north into the hole in the floor of the small chamber at the tunnels' north end,
// beside its ladder, metatile (25, 7) or (27, 7): he steps onto the ladder and climbs down into the
// cave below.
void scMikasallaStepOntoLadderAhead(void)
{
    fdScriptStart();
    fdScriptEndLastingPsynergy(0);
    fdActorWalkByAndWait(ACTOR_FELIX, 0, -4);
    fdActorWalkToAndWait(ACTOR_FELIX, 424, 99);
    scMikasallaClimbDownLadder();
}

// The doors' openings: most doors, the east house's door with metatiles of its own, and the
// Sanctum's wide door, two metatiles across.
Uint16 MikasallaDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(81, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(80, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
Uint16 MikasallaThirdDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(79, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(78, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
Uint16 MikasallaWideDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(78, 52, 2, 2, 6),
    MAPGRID_ANIMATION_STEP(78, 53, 2, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

// The village's doors, by exit number from the first (scMikasallaEnterDoor): each opening and the
// metatile it is drawn at.
MAP_GRID_METATILE_ANIMATION MikasallaDoors[] = {
    {MikasallaDoorOpening,      84, 29},
    {MikasallaDoorOpening,      77, 34},
    {MikasallaThirdDoorOpening, 86, 38},
    {MikasallaDoorOpening,      75, 41},
    {MikasallaWideDoorOpening,  69, 27},
};

// clang-format off
SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 584, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// The tunnels' ways out at the tops of their ladders: up to the sheep pen, and up from the cave to the chamber.
SCENE_REGION MikasallaTunnelRegions[] = {
    {68, 28, 404, 76, 36, 412, CONDITION_ALWAYS, MIKASALLA_NORTH_TO_MIKASALLA_SOUTH},
    {244, 28, 100, 252, 36, 108, CONDITION_ALWAYS, MIKASALLA_NORTH_CAVE_LADDER},
    {SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
    SCENE_EXITS(Mikasalla),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHEAST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHWEST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_EAST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_SOUTHWEST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_SANCTUM, MikasallaRoomsSanctum, MIKASALLA_ROOMS_SANCTUM_FROM_MIKASALLA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MIKASALLA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Mikasalla, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Mikasalla, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Mikasalla, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Mikasalla, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_NORTH_NORTH, MikasallaNorth, MIKASALLA_NORTH_FROM_MIKASALLA_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_NORTH_SOUTH, MikasallaNorth, MIKASALLA_NORTH_FROM_MIKASALLA_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(MikasallaNorth),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_TO_MIKASALLA_SOUTH, Mikasalla, MIKASALLA_FROM_MIKASALLA_NORTH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_CAVE_LADDER, MikasallaNorth, MIKASALLA_NORTH_CHAMBER_HOLE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_CHAMBER_HOLE, MikasallaNorth, MIKASALLA_NORTH_CAVE_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_TO_MIKASALLA_NORTH, Mikasalla, MIKASALLA_FROM_MIKASALLA_NORTH_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};
// clang-format on

// One squash of an animal in the pen as it turns: it flattens to three quarters of its width and
// springs back over ten ticks.
#define PEN_SQUASH                                                                                 \
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 4), SC_ADD(SW_SCALE_Y, toFIXED(1) / 8), SC_WAIT(2),           \
        SC_ADD(SW_SCALE_X, toFIXED(1) / 8), SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16), SC_WAIT(2),      \
        SC_ADD(SW_SCALE_X, toFIXED(1) / 16), SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32), SC_WAIT(2),     \
        SC_ADD(SW_SCALE_X, toFIXED(1) / 16), SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32), SC_WAIT(4)

// An animal at one end of its pace stands a moment, turns round and squashes three times.
#define PEN_TURN_ROUND                                                                             \
    SC_ANIMATION(ANIM_STAND), SC_WAIT(20), SC_ADD(SW_FACING, -DEGtoANG(180)), PEN_SQUASH,          \
        PEN_SQUASH, PEN_SQUASH

// The chicken paces between x 72 and 120 on its row, turning at each end.
Sint32 MikasallaChickenPaces[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(120, 0, 648),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(72, 0, 648),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

// The sheep paces its pen more slowly, between x 88 and 120.
Sint32 MikasallaSheepPaces[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(88, 0, 680),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(120, 0, 680),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
// clang-format off

// The village (`MIKASALLA_ACTOR_ID`).
ACTOR_PLACE MikasallaVillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 552, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 264, 0, 680, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 664, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 680, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 632, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 616, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHICKEN, CONDITION_ALWAYS, (Sint32)MikasallaChickenPaces, 72, 0, 648, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEEP, CONDITION_ALWAYS, (Sint32)MikasallaSheepPaces, 120, 0, 680, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_SPARK), MOVEMENT_TYPE_STAND, 312, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

// The tunnels under the village.
ACTOR_PLACE MikasallaTunnelPlacements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
// The village's doors, talk, Mind Read, the dug ladder, and Spark.
CUE MikasallaVillageCues[] = {
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaEnterDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaEnterDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_EAST, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaEnterDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaEnterDoor)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaEnterDoor)},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT, 8, CONDITION_ALWAYS, 8},
    {CUE_EXIT, 9, CONDITION_ALWAYS, 9},
    {CUE_EXIT, 10, CONDITION_ALWAYS, 10},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_LADDER_TOP_FROM_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaTurnAndClimbDownLadder)},
    {STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_LADDER_TOP_FROM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaStepOntoLadderFacingEast)},
    {STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_LADDER_TOP_FROM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaStepOntoLadderFacingWest)},
    {CUE_TALK, ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldItSBeenALong)},
    {CUE_TALK, ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaAskLookingForSomeone)},
    {CUE_TALK, ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldIWentOutToInvestigate)},
    {CUE_TALK, ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouReGoingDeeper)},
    {CUE_TALK, ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldInTheHeartOfOsenia)},
    {CUE_TALK, ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWantToGo)},
    {CUE_TALK, ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouLookAcrossThe)},
    {CUE_TALK, ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldATownCalledGarohLies)},
    {CUE_TALK, ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldBukBukBuk)},
    {CUE_TALK, ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldBahBaaaaaaa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldWhyWouldAnyoneTravelAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, MSGNO(MsgFieldILikeThisTimeOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldOseniaWasBigAlreadyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldNoWayIDLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldAirSRockHasStood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldItSWeirdThatYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldItSBitterColdIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldThePeopleInGarohAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldTheGroundHereIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldOhYeahMmmIJust)},
    {CUE_TALK, ACTOR_SPARK, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaMeetSpark)},
    {EXAMINE_CUE(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldItSCompletelyFullOf))},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), METATILE_BURIED_LADDER, CONDITION_ALWAYS, SCRIPT_REF(scMikasallaDigUpLadder)},
    {CUE_TABLE_END},
};
// clang-format on

// The tunnels' exits, the hole in the chamber's floor, and the water Parch dries. The rows for a
// ladder top entered from the north, west or east answer no metatile of the tunnels' map: only the
// village's dug ladder has those.
CUE MikasallaTunnelCues[] = {
    {CUE_EXIT, MIKASALLA_NORTH_CHAMBER_HOLE, CONDITION_ALWAYS, MIKASALLA_NORTH_CHAMBER_HOLE},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_CUE_WALKING_INTO(FACING_SOUTH),
     METATILE_LADDER_TOP_FROM_NORTH, CONDITION_ALWAYS,
     SCRIPT_REF(scMikasallaTurnAndClimbDownLadder)},
    {STEP_CUE_WALKING_INTO(FACING_EAST),
     METATILE_LADDER_TOP_FROM_WEST, CONDITION_ALWAYS,
     SCRIPT_REF(scMikasallaStepOntoLadderFacingEast)},
    {STEP_CUE_WALKING_INTO(FACING_WEST),
     METATILE_LADDER_TOP_FROM_EAST, CONDITION_ALWAYS,
     SCRIPT_REF(scMikasallaStepOntoLadderFacingWest)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_LADDER_TOP_FROM_SOUTH, CONDITION_ALWAYS,
     SCRIPT_REF(scMikasallaStepOntoLadderFacingNorth)},
    {STEP_CUE_WALKING_INTO(FACING_NORTH),
     METATILE_LADDER_TOP_FROM_SOUTH_BESIDE, CONDITION_ALWAYS,
     SCRIPT_REF(scMikasallaStepOntoLadderAhead)},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
     METATILE_TUNNEL_WATER, FLAG_MIKASALLA_TUNNELS_PARCHED,
     SCRIPT_REF(scMikasallaCastParch)},
    {CUE_TABLE_END},
};

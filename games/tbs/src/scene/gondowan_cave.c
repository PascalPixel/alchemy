// Gondowan Passage and Gondowan Cave, south and south-east of Tolbi.
//
// Gondowan Passage (SceneIdGondowanPassage) is the bridge from Tolbi into Gondowan: a gatehouse at
// its north end, three soldiers in spiked helmets at the gate, then a drawbridge and a long stone
// bridge running south over the water. The soldiers swear nobody gets through the cave to the east
// to Kalay. Once Babi has sent the party after Colosso (FLAG_BABI_SPOKE_AFTER_COLOSSO), Iodem waits
// at the north end of the drawbridge and comes along to Babi Lighthouse. While he travels with the
// party (FLAG_IODEM_FOLLOWING_PARTY) his actor is kept out of sight, and heading back north through
// the gate with him makes him step out and ask why. Retreat does not work on the bridge
// (FLAG_RETREAT_BLOCKED).
//
// Gondowan Cave (SceneIdGondowanCave) is the way the soldiers say is impossible: a cave from the
// world map east of the bridge through to the Kalay side. Its west mouth opens on a passage that
// forks. The right fork, the short way to the east cave, is blocked by a boulder that only Lift
// clears. The left fork leads to the river room (SceneIdGondowanCaveRiver), where Isaac hops along
// a line of posts in the water to the chest on its islet, and from there into the east cave. In the
// east cave Move sends a stump off its ledge into the water, where it floats as one more post to
// hop on, and the tall pillar by the narrow crevice is moved into the crevice to clear the way to
// it. Isaac jumps the crevice and walks out of the cave's east mouth.
//
// One exit table serves every scene. The other getters choose by MainRoutineStateWork.next, and
// Gondowan Passage's actor placements and cues also by FLAG_BABI_SPOKE_AFTER_COLOSSO. An empty set
// answers any other scene. Every scene's cue table carries the river room's two chests.
//
// The stump and the pillar are kept in story flags, so the cave stays solved and scInit puts them
// back: FLAG_GONDOWAN_CAVE_STUMP_MOVED once the stump has leapt in,
// FLAG_GONDOWAN_CAVE_PILLAR_DROPPED once the pillar has dropped into the crevice, shown from then
// on by ACTOR_DROPPED_PILLAR.
//
// Dropping the pillar also copies map metatile (108, 17) over (107, 17), the crevice's metatile
// (43, 17) as the second layer of the map holds it, 64 columns over, and every arrival by the east
// mouth makes the same copy, solved or not. Nothing on screen depends on it: the crevice looks the
// same, Isaac jumps it from either side, and the pillar settles at the same height wherever it
// drops.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE GondowanPassageEntrances[];
extern SCENE_ENTRANCE GondowanCaveRiverEntrances[];
extern SCENE_ENTRANCE GondowanCaveEntrances[];
extern Uint32         GondowanCaveExits[];
extern ACTOR_PLACE    GondowanCaveActorPlacementsOther[];
extern ACTOR_PLACE    GondowanPassageActorPlacements[];
extern ACTOR_PLACE    GondowanPassageActorPlacementsWithIodem[];
extern ACTOR_PLACE    GondowanCaveRiverActorPlacements[];
extern ACTOR_PLACE    GondowanCaveActorPlacements[];
extern CUE            GondowanCaveCuesOther[];
extern CUE            GondowanPassageCues[];
extern CUE            GondowanPassageCuesWithIodem[];
extern CUE            GondowanCaveRiverCues[];
extern CUE            GondowanCaveCues[];

// The metatiles of the bridge and the cave that run scripts. The cave's way out east is a step cue
// whose metatile is its exit's number (GONDOWAN_CAVE_TO_WORLD_MAP_EAST).
enum
{
    // Gondowan Passage: the gate through the gatehouse, at its north end.
    METATILE_BEHAVIOR_GONDOWAN_PASSAGE_GATE           = 10,
    // Gondowan Passage: the north end of the drawbridge, where Iodem waits.
    METATILE_BEHAVIOR_GONDOWAN_PASSAGE_IODEM          = 11,
    // Gondowan Cave: the west edge of the crevice beside the tall pillar.
    METATILE_BEHAVIOR_GONDOWAN_CAVE_CREVICE_WEST_EDGE = 10,
    // Gondowan Cave: the east edge of the crevice.
    METATILE_BEHAVIOR_GONDOWAN_CAVE_CREVICE_EAST_EDGE = 11,
    // Gondowan Cave: the tall pillar's first metatile, west of the crevice.
    METATILE_BEHAVIOR_GONDOWAN_CAVE_PILLAR            = 50
};

// Gondowan Passage's placed actors, numbered in the order of GondowanPassageActorPlacements and
// GondowanPassageActorPlacementsWithIodem from ACTOR_FIRST_PLACED.
//
// They are the soldiers who guard the bridge, and in the second table Iodem after them.
typedef enum GONDOWAN_PASSAGE_ACTOR_ID
{
    // On the road south of the gatehouse; asks whether the party will cross into Gondowan.
    ACTOR_FIRST_SOLDIER = ACTOR_FIRST_PLACED,
    // West of the gate.
    ACTOR_SECOND_SOLDIER,
    // East of the gate; says nobody gets through the cave to the east to Kalay.
    ACTOR_THIRD_SOLDIER,
    // Waits at the north end of the drawbridge once Babi has sent the party after Colosso.
    ACTOR_IODEM
} GONDOWAN_PASSAGE_ACTOR_ID;

// The river room's placed actor, the one row of GondowanCaveRiverActorPlacements that a cue names.
typedef enum GONDOWAN_CAVE_RIVER_ACTOR_ID
{
    // The boulder in the passage south of the river, raised by Lift.
    ACTOR_RIVER_BOULDER = ACTOR_FIRST_PLACED
} GONDOWAN_CAVE_RIVER_ACTOR_ID;

// Gondowan Cave's placed actors, numbered in the order of GondowanCaveActorPlacements from
// ACTOR_FIRST_PLACED.
typedef enum GONDOWAN_CAVE_ACTOR_ID
{
    // The squat stump on the ledge in the middle of the east cave, beside the torch; Move sends it
    // into the water west of the ledge. It is drawn in the sprite of the Goma Cave stump.
    ACTOR_STUMP = ACTOR_FIRST_PLACED,
    // The tall pillar on the west side of the crevice, pushed or moved with Move, in the sprite the
    // stone pillars of other scenes use (crossbone_isle_stepping_stones.c).
    ACTOR_PILLAR,
    // Stands in for the pillar once it has dropped in.
    //
    // It is out of sight at first. It is drawn in the stump's sprite, splashing in
    // (STUMP_ANIMATION_SPLASH) and on later visits afloat (STUMP_ANIMATION_SINK).
    ACTOR_DROPPED_PILLAR,
    // The boulder in the right fork north of the west mouth, raised by Lift.
    ACTOR_BOULDER,
    // An invisible actor (SPRITE_BLANK, its shadow turned off) 16 pixels west of the boulder.
    //
    // It fills the gap between the boulder and the passage's west wall, so the boulder blocks the
    // fork until Lift takes both away
    // (https://alchemy.pascalpixel.com/places/gondowan-cave/map-4.png shows the gap).
    ACTOR_BOULDER_GAP
} GONDOWAN_CAVE_ACTOR_ID;

// The stump's column at the edge of its ledge (its placement, x 496). Move west takes it off this
// column, and it leaps into the water.
#define STUMP_COLUMN       31

// The crevice's column, two east of the pillar's first metatile (x 696, where ACTOR_DROPPED_PILLAR
// stands). Pushed or moved this far, the pillar drops in.
#define PILLAR_DROP_COLUMN 43

// Move on the tall pillar beside the crevice in the east cave, and each push of it: once it reaches
// the crevice (metatile (43, 17)) it drops into the water at its foot with a splash and stays
// there, clearing the way to the edge.
static void scGondowanCaveMovePillar(void);
// The actor spins in place, leaps off the ledge and lands with a shake and a ring of dust, then
// hops on into the water with a splash and floats there.
static void scGondowanCaveLeapStump(Sint32 actorId);

#include "shared/field_effect_create.c"

#include "shared/field_effect_spawn.c"
#include "shared/pushed_actor.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_GONDOWAN_PASSAGE)
    {
        return GondowanPassageEntrances;
    }
    if (sceneId == SCENE_GONDOWAN_CAVE_RIVER)
    {
        return GondowanCaveRiverEntrances;
    }
    if (sceneId == SCENE_GONDOWAN_CAVE)
    {
        return GondowanCaveEntrances;
    }
    return SceneFallbackEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

// Every scene of the overlay shares one exit table.
const Uint32 *scGetExits(void)
{
    return GondowanCaveExits;
}

// Iodem waits on Gondowan Passage once Babi has sent the party after Colosso.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_GONDOWAN_PASSAGE)
    {
        if (gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0)
        {
            return GondowanPassageActorPlacementsWithIodem;
        }
        return GondowanPassageActorPlacements;
    }
    if (sceneId == SCENE_GONDOWAN_CAVE_RIVER)
    {
        return GondowanCaveRiverActorPlacements;
    }
    if (sceneId == SCENE_GONDOWAN_CAVE)
    {
        return GondowanCaveActorPlacements;
    }
    return GondowanCaveActorPlacementsOther;
}

// The cues of each scene, with Iodem's in Gondowan Passage when he is there.
const CUE *scGetCues(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_GONDOWAN_PASSAGE)
    {
        if (gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0)
        {
            return GondowanPassageCuesWithIodem;
        }
        return GondowanPassageCues;
    }
    if (sceneId == SCENE_GONDOWAN_CAVE_RIVER)
    {
        return GondowanCaveRiverCues;
    }
    if (sceneId == SCENE_GONDOWAN_CAVE)
    {
        return GondowanCaveCues;
    }
    return GondowanCaveCuesOther;
}

// Isaac heads back north through the gatehouse's gate on Gondowan Passage (metatiles (8, 7) to (10,
// 7)) with Iodem in the party, before Iodem has reached Babi Lighthouse: Iodem steps out beside him
// and asks whether he needs something he left behind.
//
// Answered yes, Iodem walks back to the north end of the drawbridge to wait there, and a defeat
// from then on returns the party to the Tolbi Sanctum. Answered no, he says they should go on to
// the lighthouse and falls in again, and Isaac steps back south off the gate.
static void scGondowanCaveIodemAsksWhyTurningBack(void)
{
    ACTOR *leader;

    if (gmFlagGet(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0
        && fdScriptGetFlag(FLAG_IODEM_FOLLOWING_PARTY) != 0)
    {
        fdScriptStart();
        fdActorSetSpeed(ACTOR_IODEM, 1);
        leader = fdActorGet(ACTOR_PARTY_LEADER);
        if (leader != NULL)
        {
            fdActorSetPosFX(ACTOR_IODEM, leader->x.fixed, leader->z.fixed);
        }
        fdActorSetDestinationOffset(ACTOR_IODEM, -8, 16);
        fdActorWaitForMoveById(ACTOR_IODEM);
        fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH_NORTHEAST, 0);
        fdScriptWait(10);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
        fdScriptSetMessageId(MsgKaragolWhyGoingBackIsaacDo);
        // "Why are you going back, Isaac?" "Do you need to get something you left behind?"
        fdScriptOpenMessage(ACTOR_IODEM, 0);
        if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
        {
            // "I see... In that case, I shall wait here for you to return."
            fdScriptShowMessage(ACTOR_IODEM, 0);
            fdActorWalkTo(ACTOR_IODEM, 152, 232);
            fdScriptClearFlag(FLAG_IODEM_FOLLOWING_PARTY);
            fdActorWaitForMoveById(ACTOR_IODEM);
            fdActorSetAnimation(ACTOR_IODEM, ANIM_STAND);
            // A defeat now returns the party to the Tolbi Sanctum.
            MainRoutineStateWork.restartScene    = SCENE_STUCCO_ROOMS_B;
            MainRoutineStateWork.restartEntrance = STUCCO_ROOMS_B_SANCTUM_NORTH;
        }
        else
        {
            FieldStateWork->msg += 1;
            // "Then we should continue on our way to the lighthouse."
            fdScriptShowMessage(ACTOR_IODEM, 0);
            fdActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
            leader = fdActorGet(ACTOR_PARTY_LEADER);
            if (leader != NULL)
            {
                fdActorSetDestination(
                    ACTOR_IODEM, leader->x.part[0].integer, leader->z.part[0].integer);
            }
            fdActorWaitForMoveById(ACTOR_IODEM);
            fdActorSetPos(ACTOR_IODEM, 0, 0);
            fdScriptWait(30);
            // Isaac steps back south off the gate.
            fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
            fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
            fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
            fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
        }
        fdScriptEnd();
    }
}

// Isaac talks to Iodem, waiting at the north end of the drawbridge on Gondowan Passage, or walks up
// to him (metatiles (8, 14) to (10, 14)): Iodem says he has been waiting and they should go to Babi
// Lighthouse, walks into Isaac and travels with the party from then on.
static void scGondowanCaveIodemComesAlong(void)
{
    fdScriptStart();
    fdActorFaceActor(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdScriptWait(10);
    fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
    fdScriptSetMessageId(MsgKaragolIveBeenWaitingForIsaac);
    // "I've been waiting for you, Isaac! We should go to Babi Lighthouse soon."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
    {
        ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

        if (leader != NULL)
            fdActorSetDestination(
                ACTOR_IODEM, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_IODEM);
    fdActorSetPos(ACTOR_IODEM, 0, 0);
    fdScriptWait(20);
    gmFlagSet(FLAG_IODEM_FOLLOWING_PARTY);
    fdScriptEnd();
}

// Isaac talks to the soldier on the road south of the gatehouse on Gondowan Passage, before Babi
// has sent the party on: he asks whether they wish to cross into Gondowan.
//
// Answered yes, he won't stop them but would sooner watch Colosso in Tolbi, or, once Colosso is
// over, simply wouldn't go without business there. Answered no, he agrees: they wouldn't be let
// into Babi Lighthouse anyway.
static void scGondowanCaveSoldierAsksAboutCrossing(void)
{
    Sint32 firstMsg;

    firstMsg = MsgKaragolDoYouWishCrossInto;
    fdScriptSetMessageId(firstMsg);
    // "Do you wish to cross into Gondowan?"
    fdScriptOpenMessage(ACTOR_FIRST_SOLDIER, 0);
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
    {
        // Yes: "I won't stop you, but I wouldn't go unless I had business there." "I'd go to Tolbi
        // and watch Colosso. That's what I'd do."
        if (gmFlagGet(FLAG_COLOSSO_OVER) != 0)
        {
            if (gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
            {
                // Once Colosso is over, the shorter answer eight messages on: "I won't stop you,
                // but I wouldn't go unless I had business there!"
                fdScriptSetMessageId(firstMsg + 8);
            }
        }
        fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    }
    else
    {
        FieldStateWork->msg++;
        // No: "That's a good idea. They wouldn't let you into Babi Lighthouse, anyway."
        fdScriptShowMessage(ACTOR_FIRST_SOLDIER, 0);
    }
}

// Mind Read on the soldier west of the gate on Gondowan Passage, before Babi has sent the party on:
// he grumbles about being stuck on guard duty, while the warriors fight or, once it is over,
// through all of Colosso.
static void scGondowanCaveReadSoldierMind(void)
{
    if (gmFlagGet(FLAG_COLOSSO_OVER) != 0 && gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) == 0)
        fdScriptSetMessageId(MsgKaragolWeMissedColossoBecauseWe);
    else
        fdScriptSetMessageId(MsgKaragolWarriorsHaveBeenFightingWhile);
    fdScriptShowMessage(ACTOR_SECOND_SOLDIER, 0);
}

// Isaac walks out of Gondowan Cave by its east mouth, at the foot of the path in the south-east
// corner of the east cave (metatile (49, 29)), and comes out on the world map.
static void scGondowanCaveLeaveToWorldMapEast(void)
{
    FIELD_STATE *state;

    state = FieldStateWork;
    fdScriptStart();
    // XXX (no evidence): what story flags 0x9a3, 0x9a4 and 0x9a6 hold, among
    // FLAG_GONDOWAN_CAVE_STUMP_MOVED and FLAG_GONDOWAN_CAVE_PILLAR_DROPPED; only this block touches
    // them, nothing in either game reads them, and nothing sets FLAG_SCENE_TEMP_04, so no play or
    // render shows them.
    if (gmFlagGet(FLAG_SCENE_TEMP_04) != 0)
    {
        gmFlagClear(0x9a3);
        fdScriptClearFlag(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
        fdScriptClearFlag(0x9a4);
        gmFlagClear(0x9a6);
        gmFlagSet(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
        gmFlagSet(0x9a4);
    }
    syImportScriptSetExitRequest(state->stepCue);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptEnd();
}

// Isaac walks to the edge of the narrow crevice in the east cave (metatile (42,
// 17) on its west side, (44, 17) on its east) and keeps walking: he jumps
// across it.
//
// fdTryJumpForward carries him two metatiles on where the map allows.
static void scGondowanCaveJumpCrevice(void)
{
    fdTryJumpForward();
}

// Move on the squat stump on the ledge in the middle of the east cave, steered west: it spins, hops
// off the ledge onto the ground below in a ring of dust, and on into the water with a splash, where
// it floats (metatile (27, 19)) for Isaac to hop on.
//
// Its metatile takes the attributes of metatile (29, 19), a square Isaac hops from, so the water
// there becomes one too. Once it has leapt Move on it no longer runs this.
static void scGondowanCaveMoveStump(void)
{
    // DEAD: the stump looked up and dropped; the ROM keeps its call.
    fdActorGet(ACTOR_STUMP);
    fdScriptStart();
    if (FIELD_METATILE(fdActorGet(ACTOR_STUMP)->x.fixed) < STUMP_COLUMN)
    {
        scGondowanCaveLeapStump(ACTOR_STUMP);
        fdMapGridCopyMetatileAttributes(29, 19, 1, 1, 27, 19);
        gmFlagSet(FLAG_GONDOWAN_CAVE_STUMP_MOVED);
    }
    fdScriptEnd();
}

// Isaac, standing where the tall pillar first stood beside the crevice (metatile (41, 17)), walks
// into it: he pushes it a metatile east, and at the crevice it drops in.
static void scGondowanCavePushPillar(void)
{
    scPushedActorAdvancePair();
    scGondowanCaveMovePillar();
}

// Hides the pillar and shows the actor that stands in for it in its place.
static inline void scGondowanCavePlaceDroppedPillar(Sint32 animation)
{
    fdActorSetPos(ACTOR_PILLAR, 0, 0);
    fdActorSetPos(ACTOR_DROPPED_PILLAR, 696, 288);
    fdActorSetAnimation(ACTOR_DROPPED_PILLAR, animation);
}

static void scGondowanCaveMovePillar(void)
{
    fdScriptStart();
    if (FIELD_METATILE(fdActorGet(ACTOR_PILLAR)->x.fixed) >= PILLAR_DROP_COLUMN)
    {
        fdMapGridCopyMetatileAttributes(108, 17, 1, 1, 107, 17);
        fdScriptWait(8);
        scGondowanCavePlaceDroppedPillar(STUMP_ANIMATION_SPLASH);
        sySoundPlay(SE_CAST);
        gmFlagSet(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED);
    }
    fdScriptEnd();
}

// Unused: an empty function nothing refers to.
static UNUSED void scGondowanCaveUnusedNoOperationA(void)
{
}

// Spins the leaping stump a sixty-fourth of a turn each tick.
static void scGondowanCaveSpinStump(ACTOR *actor)
{
    actor->sprite->rotation -= TRIG_TURN / 64;
}

// Waits until the actor has come down to height (16.16), forty ticks at most:
// scActorWaitBelowHeight with a shorter limit.
static void scGondowanCaveWaitBelowHeight(ACTOR *actor, Sint32 height)
{
    Sint32 ticksRemaining = 40;

    while (ticksRemaining != 0)
    {
        sySynch(1);
        ticksRemaining--;
        if (actor->y.fixed <= height)
        {
            break;
        }
    }
}

// The stump tips over the ledge, leaps spinning off it with a whoosh, lands with a shake and a ring
// of dust, and slides on into the water, where it splashes and settles to float.
//
// Goma Cave's stumps leap the same way (shared/stump_leap.c); this one spins and leaps slower,
// lands with no thump, slides on without a bounce and throws no spray, so it is written again here.
static void scGondowanCaveLeapStump(Sint32 actorId)
{
    ACTOR  *actor;
    Uint32  stepIndex;
    VECTOR3 particleVelocity;
    Sint32  angle;

    actor              = fdActorGet(actorId);
    // It winds up its spin in place.
    actor->motionFlags = 0;
    for (stepIndex = 0; stepIndex < 9; stepIndex++)
    {
        sySynch(1);
        actor->sprite->rotation -= TRIG_TURN / 256;
        actor->x.fixed          -= syCos(actor->sprite->rotation) / 2;
        actor->targetX           = ACTOR_NO_TARGET;
    }
    actor->update = scGondowanCaveSpinStump;
    sySoundPlay(SE_WHOOSH);
    fdActorSetSpeed(actorId, 2);
    fdActorSetDestination(actorId, 472, 288);
    actor->gravity     = toFIXED(4) / 5;
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    actor->layer       = 0;
    fdActorWaitForMoveById(actorId);
    scGondowanCaveWaitBelowHeight(actor, toFIXED(32));
    fdMapGridSetShake(5, 5, 1);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    for (stepIndex = 0; stepIndex <= TRIG_TURN / FACING_STEP; stepIndex++)
    {
        angle               = stepIndex * FACING_STEP;
        particleVelocity.x  = syCos(angle);
        particleVelocity.y  = 0;
        particleVelocity.z  = sySin(angle);
        particleVelocity.x -= particleVelocity.x / 4;
        particleVelocity.z -= particleVelocity.z / 2;
        fdEffectSpawn(actor->x.fixed,
                      actor->y.fixed,
                      actor->z.fixed,
                      particleVelocity.x,
                      particleVelocity.y,
                      particleVelocity.z,
                      0,
                      NULL);
    }
    fdActorSetDestination(actorId, 440, 308);
    fdActorWaitForMoveById(actorId);
    scGondowanCaveWaitBelowHeight(actor, toFIXED(32));
    actor->update           = NULL;
    actor->sprite->rotation = TRIG_TURN / 16;
    sySoundPlay(SE_CAST);
    fdActorSetAnimation(actorId, STUMP_ANIMATION_SPLASH);
    fdMapGridWaitForShakeToSettle();
    fdScriptWait(10);
    fdActorSetAnimation(actorId, STUMP_ANIMATION_SINK);
}

// Unused: an empty function nothing refers to.
static UNUSED void scGondowanCaveUnusedNoOperationB(void)
{
}

// Lift on the boulder in the right fork north of the cave's west mouth raises it away, and clears
// the short way north to the east cave.
//
// Lift removes the boulder itself; the script takes away ACTOR_BOULDER_GAP, which stood in the gap
// beside it.
static void scGondowanCaveLiftBoulder(void)
{
    fdActorSetPos(ACTOR_BOULDER_GAP, 0, 0);
}

// Sets up the scene as the party enters.
Sint32 scInit(void)
{
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_PASSAGE)
    {
        gmFlagSet(FLAG_RETREAT_BLOCKED);
        if (gmFlagGet(FLAG_IODEM_FOLLOWING_PARTY) != 0)
        {
            fdActorSetPos(ACTOR_IODEM, 0, 0);
        }
    }
    if (MainRoutineStateWork.next == SCENE_GONDOWAN_CAVE)
    {
        // XXX (no evidence): why this arrival changes the drop's metatile; the change has no effect
        // that play shows.
        if (MainRoutineStateWork.entrance == GONDOWAN_CAVE_FROM_WORLD_MAP_EAST)
        {
            fdMapGridCopyMetatileAttributes(108, 17, 1, 1, 107, 17);
        }
        if (gmFlagGet(FLAG_GONDOWAN_CAVE_STUMP_MOVED) != 0)
        {
            fdActorSetPos(ACTOR_STUMP, 440, 308);
            fdActorSetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
            fdMapGridCopyMetatileAttributes(29, 19, 1, 1, 27, 19);
        }
        if (gmFlagGet(FLAG_GONDOWAN_CAVE_PILLAR_DROPPED) != 0)
        {
            scGondowanCavePlaceDroppedPillar(STUMP_ANIMATION_SINK);
        }
        fdActorSetSpriteFlags(fdActorGet(ACTOR_BOULDER_GAP), 0);
    }
    return 0;
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

#include "shared/fallback_entrances.c"

SCENE_ENTRANCE GondowanPassageEntrances[] = {
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 152, 0, 120, FACING_SOUTH, 0, 32, 32, 272, 480, 0 },
    { GONDOWAN_PASSAGE_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 152, 0, 440, FACING_NORTH, 0, 32, 32, 272, 480, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE GondowanCaveRiverEntrances[] = {
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH, CONDITION_ALWAYS, 728, 0, 312, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE, CONDITION_ALWAYS, 168, 0, 408, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
    { GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER, CONDITION_ALWAYS, 168, 0, 520, FACING_SOUTH, 0, 112, 464, 576, 704, 0 },
    { GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH, CONDITION_ALWAYS, 504, 0, 664, FACING_NORTH, 0, 112, 464, 576, 704, 0 },
    { SCENE_TABLE_END },
};

SCENE_ENTRANCE GondowanCaveEntrances[] = {
    { GONDOWAN_CAVE_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 792, 0, 456, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH, CONDITION_ALWAYS, 360, 0, 152, FACING_SOUTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_FROM_WEST_PASSAGE, CONDITION_ALWAYS, 344, 0, 440, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
    { GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH, CONDITION_ALWAYS, 136, 0, 504, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
    { GONDOWAN_CAVE_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 216, 0, 888, FACING_NORTH, 0, 48, 432, 408, 896, 0 },
    { SCENE_TABLE_END },
};

Uint32 GondowanCaveExits[] = {
    SCENE_EXITS(GondowanPassage),
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_NORTH),
    SCENE_EXIT(GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_GONDOWAN_PASSAGE_SOUTH),
    SCENE_EXITS(GondowanCaveRiver),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_FROM_RIVER),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_BOULDER_PASSAGE),
    SCENE_EXIT(GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH, GondowanCave, GONDOWAN_CAVE_FROM_GONDOWAN_CAVE_RIVER_SOUTH),
    SCENE_EXITS(GondowanCave),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_NORTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WEST_PASSAGE, GondowanCave, GONDOWAN_CAVE_WEST_PASSAGE_FROM_EAST_CAVE),
    SCENE_EXIT(GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE, GondowanCave, GONDOWAN_CAVE_FROM_WEST_PASSAGE),
    SCENE_EXIT(GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH, GondowanCaveRiver, GONDOWAN_CAVE_RIVER_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXIT(GONDOWAN_CAVE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_GONDOWAN_CAVE_SOUTH),
    SCENE_EXITS_END,
};

ACTOR_PLACE GondowanCaveActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

ACTOR_PLACE GondowanPassageActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        144, 0, 168, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE GondowanPassageActorPlacementsWithIodem[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 136, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 232, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE GondowanCaveRiverActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        504, 0, 624, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE GondowanCaveActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        496, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 636, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 640, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

CUE GondowanCaveCuesOther[] = {
    { CUE_TABLE_END },
};

CUE GondowanPassageCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { CUE_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { CUE_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveSoldierAsksAboutCrossing) },
    { CUE_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveUsWereTheSoldiersWhoGuard) },
    { CUE_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouHeadEastYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveThoseWhoGoToGondowanRarely) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveReadSoldierMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveYoudThinkYouCouldGetThrough) },
    { EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_CUE(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { CUE_TABLE_END },
};

CUE GondowanPassageCuesWithIodem[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_NORTH },
    { CUE_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_PASSAGE_TO_WORLD_MAP_SOUTH },
    { CUE_STEP, METATILE_BEHAVIOR_GONDOWAN_PASSAGE_GATE, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveIodemAsksWhyTurningBack) },
    { CUE_STEP, METATILE_BEHAVIOR_GONDOWAN_PASSAGE_IODEM, FLAG_IODEM_FOLLOWING_PARTY, SCRIPT_REF(scGondowanCaveIodemComesAlong) },
    { CUE_TALK, ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWhyWouldIodemChooseToWait) },
    { CUE_TALK, ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaventSeenAnybodyUnusualCrossing) },
    { CUE_TALK, ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEvenIfYouGoEastFrom) },
    { CUE_TALK, ACTOR_IODEM, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveIodemComesAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveIodemIsGoingToBabiLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveItsALotOfStressBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveEveryoneComesDownThisWayWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgGondowanCaveWeHaveToHeadForThe) },
    { EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_CUE(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { CUE_TABLE_END },
};

CUE GondowanCaveRiverCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_NORTH },
    { CUE_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_BOULDER_PASSAGE },
    { CUE_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_BOULDER_PASSAGE_TO_RIVER },
    { CUE_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_RIVER_TO_GONDOWAN_CAVE_SOUTH },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_RIVER_BOULDER, CONDITION_ALWAYS, 0 },
    { EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_CUE(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { CUE_TABLE_END },
};

// Move on the tall pillar runs while FLAG_SCENE_TEMP_05 is clear, which is always, since nothing sets it; once the pillar has dropped it stands at (0, 0), out of reach.
CUE GondowanCaveCues[] = {
    { CUE_STEP, GONDOWAN_CAVE_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveLeaveToWorldMapEast) },
    { CUE_EXIT, 2, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_NORTH },
    { CUE_EXIT, 3, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WEST_PASSAGE },
    { CUE_EXIT, 4, CONDITION_ALWAYS, GONDOWAN_CAVE_WEST_PASSAGE_TO_EAST_CAVE },
    { CUE_EXIT, 5, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_GONDOWAN_CAVE_RIVER_SOUTH },
    { CUE_EXIT, 6, CONDITION_ALWAYS, GONDOWAN_CAVE_TO_WORLD_MAP_WEST },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveLiftBoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GONDOWAN_CAVE_STUMP_MOVED, SCRIPT_REF(scGondowanCaveMoveStump) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, FLAG_SCENE_TEMP_05, SCRIPT_REF(scGondowanCaveMovePillar) },
    { STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_GONDOWAN_CAVE_CREVICE_EAST_EDGE, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveJumpCrevice) },
    { STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_GONDOWAN_CAVE_CREVICE_WEST_EDGE, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCaveJumpCrevice) },
    { CUE_STEP | STEP_CUE_FLAG_WALKING, METATILE_BEHAVIOR_GONDOWAN_CAVE_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scGondowanCavePushPillar) },
    { EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_GONDOWAN_CAVE_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EXAMINE_CUE(EXAMINE_CHEST), 101, FLAG_GONDOWAN_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { CUE_TABLE_END },
};
// clang-format on

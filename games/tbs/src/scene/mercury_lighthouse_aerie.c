// Lighthouse Aerie (SceneIdMercuryLighthouseAerie), the top of Mercury Lighthouse, where its beacon
// burns.
//
// Isaac and Garet, with Ivan and Mia, have climbed the lighthouse after Saturos and Menardi, who
// hold Jenna and Kraden and carry the Elemental Stars. Ply on the statue below the rainbow turns
// the last waterfall to stream upward, and the party comes out at the top. They come too late to
// stop the beacon being lit.
//
// The aerie is three tower tops in the sky. The party lands on the east one, where a Psynergy Stone
// lies. The beacon stands on the middle one, at the head of a wide flight of stairs, with two
// statues either side of it. The west one is a lift, a tower top with a statue on it that sinks to
// the ground and rises again. Three floating platforms bridge each gap, and the leader jumps from
// one to the next.
//
// - The arrival (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
//   scMercuryLighthouseAeriePartyArrives). The Adepts of the lit element
//   regain their PP, the aerie shakes, and in a white flash the party lies
// on the east tower top. They get up and gather round Isaac.
// - The beacon (scMercuryLighthouseAerieConfrontSaturos). Climbing the stairs
//   to it, Garet sees `It's too late! The lighthouse has already been lit!`
//   Mia learns that the party came from Vale to recover the Elemental
// Stars, and that Vale protected Sol Sanctum as her clan protects the lighthouse. Jenna calls out
// from the west tower top, where Kraden, Felix and Menardi hold her. Saturos sends Menardi on to
// the next lighthouse with the others, and Menardi leaps onto the lift, which sinks with them.
// Saturos fights the party alone (ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2), the light of
// Mercury weakening his Psynergy.
// - Back from the battle (MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE,
//   scMercuryLighthouseAerieAlexAppears): `Isaac, we got him!` Alex, the
//   apprentice Mia lost, appears. He explains that Saturos, aligned with
// Mars, the clan of fire, had his power bound by the lighthouse's water, which
//       lent Mia, of the Mercury Clan like Alex, endless Psynergy. Alex was
// only stalling until Saturos recovered. Saturos rises, Alex asks whether the
//       party still holds the Mars Star, and the two vanish in sparkles,
//       reappear on the lift and ride it down. Mia means to go with the party,
//       since with the lighthouse lit the fountain will run over with healing
//       water again (FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED, and
//       FLAG_SATUROS_BEATEN_AT_MERCURY, which Imil, Bilibin and the lighthouse
//       test).
// - Leaving. The stairs on the west side of the beacon's tower bring the
//   lift back up if it is down, and stepping onto it from the
//   westmost platform rides it down to the ground before Mercury
// Lighthouse, by the Fountain of Healing (SceneIdMercuryLighthouseExterior,
//       mercury_lighthouse.c). Coming up that way
//       (MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR) leaves the
//       lift up.
//
// Every arrival sets FLAG_IN_MERCURY_LIGHTHOUSE, under which Mercury Adepts regain PP faster.
//
// On the aerie Retreat is blocked, and the beacon flickers and sheds sparks
// (scMercuryLighthouseAerieFlickerBeacon).
//
// Jenna, Kraden, Felix and Menardi are placed only on the first arrival. Every other entrance sets
// FLAG_HOSTAGES_GONE before the actor placements are read.
//
// Unused: SceneIdUnusedMercuryLighthouseAerieCopy, which the area names call Mercury Lighthouse and
// the debug scene list labels `MTO07NG`, loads this overlay and the same map. The setup does
// nothing for it, and no exit, table or code leads there. Only the debug scene selector, which
// reaches every scene, can.

#include "ability_ids.h"
#include "flags.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/sparkle_warp.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

// The metatile behaviors the aerie's cues answer.
typedef enum MERCURY_LIGHTHOUSE_AERIE_METATILE_BEHAVIOR
{
    // The stairs up to the beacon, metatiles (20, 14) and (21, 14).
    METATILE_BEHAVIOR_BEACON_STAIRS  = 5,
    // The stairs on the west side of the beacon's tower, metatile (12, 9), from which the lift is
    // called up.
    METATILE_BEHAVIOR_LIFT_LANDING   = 6,
    // The six floating platforms.
    METATILE_BEHAVIOR_PLATFORM       = 10,
    // The ground beside the platforms: the foot of the stairs either side of the beacon's tower and
    // the east tower top.
    METATILE_BEHAVIOR_PLATFORM_EDGE  = 11,
    // The lift's floor, metatile (6, 10), stepped onto from the westmost platform.
    METATILE_BEHAVIOR_LIFT           = 12,
    // Where the Psynergy Stone lies on the east tower top, metatile (31, 8).
    METATILE_BEHAVIOR_PSYNERGY_STONE = 100
} MERCURY_LIGHTHOUSE_AERIE_METATILE_BEHAVIOR;

extern SCENE_ENTRANCE MercuryLighthouseAerieEntrances[];
extern Uint32         MercuryLighthouseAerieExits[];
extern ACTOR_PLACE    MercuryLighthouseAerieActorPlacements[];
extern CUE            MercuryLighthouseAerieCues[];
extern Sint32         MercuryLighthouseAerieLandedPlatform;
extern Sint32         BeaconSparkScript[];

// The aerie's actors.
//
// The party and Jenna stand in their own slots. The speakers are known from the text: Kraden ("It's
// all right, Jenna."), Felix ("Jenna, please... Just do as she says"), Saturos (Menardi: "Oh,
// Saturos..."), Menardi (Saturos: "Menardi... Wait!") and Alex (Mia: "Alex!"). Saturos is fought as
// ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
typedef enum MERCURY_LIGHTHOUSE_AERIE_ACTOR_ID
{
    // Flickers between two sizes and sheds sparks while in view
    // (scMercuryLighthouseAerieFlickerBeacon).
    ACTOR_BEACON = ACTOR_FIRST_PLACED,
    // The statue on the lift, the west tower top. It rides up and down with it.
    ACTOR_LIFT_STATUE,
    // Four more statues flank the beacon, two on each side of it. The two east of it,
    // ACTOR_FIRST_BEACON_STATUE + 2 and + 3, are drawn mirrored.
    ACTOR_FIRST_BEACON_STATUE,
    // The six floating platforms, three in each gap between the towers.
    ACTOR_FIRST_PLATFORM      = ACTOR_FIRST_PLACED + 6,
    // The two platforms that stand on the lift and go up and down with it.
    ACTOR_FIRST_LIFT_PLATFORM = ACTOR_FIRST_PLATFORM + 3,
    ACTOR_SECOND_LIFT_PLATFORM,
    ACTOR_KRADEN = ACTOR_FIRST_PLACED + 12,
    ACTOR_FELIX,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    ACTOR_ALEX
} MERCURY_LIGHTHOUSE_AERIE_ACTOR_ID;

// Animations of the aerie's sprites beyond ACTOR_ANIMATION, as their animation tables and ticks
// under graphics/sprite draw them.
typedef enum MERCURY_LIGHTHOUSE_AERIE_ANIMATION
{
    // Mia seen side-on, her head bowed and her hands clasped at her chest (mia.png).
    ANIM_MIA_HEAD_BOWED = 16
} MERCURY_LIGHTHOUSE_AERIE_ANIMATION;

// Scene flags, kept while the party stays on the aerie.
enum
{
    // The leader stands on a platform, which has dipped under the weight.
    FLAG_ON_PLATFORM   = FLAG_SCENE_TEMP_50,
    FLAG_LIFT_UP       = FLAG_SCENE_TEMP_51,
    // Set on every arrival but the first, because Jenna, Kraden, Felix and Menardi are placed only
    // before it.
    FLAG_HOSTAGES_GONE = FLAG_SCENE_TEMP_53
};

// The aerie's Psynergy Stone. It is an area temp flag, so it refills once the party has been to
// another area.
enum
{
    FLAG_PSYNERGY_STONE = FLAG_AREA_TEMP_50
};

static void scMercuryLighthouseAeriePartyArrives(void);
static void scMercuryLighthouseAerieFloatPlatforms(void);
static void scMercuryLighthouseAerieAlexAppears(void);
static void scMercuryLighthouseAerieSaturosAndAlexWarpOut(void);
static void scMercuryLighthouseAerieSaturosAndAlexWarpIn(void);
static void scMercuryLighthouseAerieLowerLift(void);
static void scMercuryLighthouseAerieRaiseLift(void);
static void scMercuryLighthouseAerieFlickerBeacon(void);

#include "shared/field_effect_create.c"

#include "shared/field_effect_spawn.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return MercuryLighthouseAerieEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return 0;
}

// Returns the one exit, the lift down to the ground before Mercury Lighthouse, by the Fountain of
// Healing (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE).
const Uint32 *scGetExits(void)
{
    return MercuryLighthouseAerieExits;
}

// Returns the actors, with Jenna, Kraden, Felix and Menardi placed only on the first arrival
// (MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT).
const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.entrance != MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT)
    {
        gmFlagSet(FLAG_HOSTAGES_GONE);
    }
    return MercuryLighthouseAerieActorPlacements;
}

const CUE *scGetCues(void)
{
    return MercuryLighthouseAerieCues;
}

// Sets up the Lighthouse Aerie, then plays the script of the entrance taken.
Sint32 scInit(void)
{
    Uint32 i;
    Sint32 actorId;
    ACTOR *actor;

    gmFlagSet(FLAG_IN_MERCURY_LIGHTHOUSE);
    FieldStateWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
    if (MainRoutineStateWork.next == SCENE_MERCURY_LIGHTHOUSE_AERIE)
    {
        gmFlagSet(FLAG_RETREAT_BLOCKED);
        sySetTickEvent(scMercuryLighthouseAerieFlickerBeacon, EVENT_PASS_TICK);
        fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
        fdActorSetSpritePriority(ACTOR_GARET, 1);
        fdActorSetSpritePriority(ACTOR_IVAN, 1);
        fdActorSetSpritePriority(ACTOR_MIA, 1);
        fdActorSetSpritePriority(ACTOR_JENNA, 1);
        fdActorSetSpritePriority(ACTOR_KRADEN, 1);
        fdActorSetSpritePriority(ACTOR_FELIX, 1);
        fdActorSetSpritePriority(ACTOR_SATUROS, 1);
        fdActorSetSpritePriority(ACTOR_MENARDI, 1);
        fdActorSetSpritePriority(ACTOR_ALEX, 1);
        fdActorSetSpritePriority(ACTOR_BEACON, 1);
        fdActorSetSpritePriority(ACTOR_LIFT_STATUE, 1);
        fdActorSetSpritePriority(ACTOR_FIRST_BEACON_STATUE, 1);
        fdActorSetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 1, 1);
        fdActorSetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 2, 1);
        fdActorSetSpritePriority(ACTOR_FIRST_BEACON_STATUE + 3, 1);
        for (i = ACTOR_FIRST_PLATFORM; i < ACTOR_FIRST_PLATFORM + 6; i++)
        {
            fdActorSetSpritePriority(i, 1);
            fdActorGet(i)->motionFlags    = ACTOR_FLOATS;
            fdActorGet(i)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
            fdActorGet(i)->y.fixed        = -(toFIXED(50) + toFIXED(1) / 2);
        }
        if (gmFlagGet(FLAG_KEEP_PARTY_POSITION)
            && (actorId = scActorFindNearestFloatingPlatform()) != 0
            && (actor = fdActorGet(actorId)) != NULL)
        {
            actor->motionFlags = 0;
        }
        fdActorSetSpriteFlags(fdActorGet(ACTOR_LIFT_STATUE), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_BEACON_STATUE), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_BEACON_STATUE + 1), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_BEACON_STATUE + 2), 0);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_BEACON_STATUE + 3), 0);
        fdActorGet(ACTOR_FIRST_BEACON_STATUE + 2)->scaleX = -toFIXED(1);
        fdActorGet(ACTOR_FIRST_BEACON_STATUE + 3)->scaleX = -toFIXED(1);
        if (MainRoutineStateWork.entrance == MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT)
        {
            if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
            {
                scMercuryLighthouseAeriePartyArrives();
            }
        }
        else if (MainRoutineStateWork.entrance == MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE)
        {
            if (!gmFlagGet(FLAG_LIFT_UP))
            {
                {
                    MAP_GRID_LAYER_SCROLL *lift =
                        &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];

                    lift->offsetY = toFIXED(1024);
                }
                fdMapGridCenterOnFollowedPoint();
                sySynch(1);
                fdMapGridCopyMetatilesTo(4, 70, 4, 74, 5, 4);
                fdActorSetPos(ACTOR_LIFT_STATUE, 0, 0);
                if (!gmFlagGet(FLAG_KEEP_PARTY_POSITION))
                {
                    scMercuryLighthouseAerieAlexAppears();
                }
            }
        }
        else if (MainRoutineStateWork.entrance
                 == MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR)
        {
            gmFlagSet(FLAG_LIFT_UP);
        }
    }
    return 0;
}

// The party arrives at the top: the aerie shakes, a white flash fills the screen, and Isaac, Garet,
// Ivan and Mia lie on the east tower top, then get up and gather round Isaac.
//
// It plays at MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT, the entrance the world map sends the party
// to after the waterfall below turns to stream upward, but not on a return from a battle there.
static void scMercuryLighthouseAeriePartyArrives(void)
{
    ACTOR *leader;

    fdScriptStart();
    gmPartyRestorePpInLighthouse();
    fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA), 0);
    fdCameraMoveTo(304, CAMERA_KEEP, 120, 0);
    sySynch(1);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    sySoundPlay(SE_RUMBLE);
    fdMapGridSetShake(5, 5, 1);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdMapGridWaitForShakeToSettle();
    fdScriptWait(30);
    fdCameraGetActor()->motionFlags = 0;
    fdCameraSetSpeed(0.8);
    fdCameraMoveTo(512, -24, 160, 1);
    fdCameraWaitForArrival();
    fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(50);
    fdScriptWait(50);
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(30);
    fdScriptWait(30);
    fdActorSetPos(ACTOR_PARTY_LEADER, 504, 168);
    fdActorSetPos(ACTOR_GARET, 528, 144);
    fdActorSetPos(ACTOR_IVAN, 488, 144);
    fdActorSetPos(ACTOR_MIA, 512, 152);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_COLLAPSE);
    fdActorSetAnimation(ACTOR_GARET, ANIM_COLLAPSE);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_COLLAPSE);
    fdActorSetAnimation(ACTOR_MIA, ANIM_COLLAPSE);
    fdScriptWait(10);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(30);
    fdScriptWait(30);
    fdScriptWait(80);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdScriptWait(60);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_GARET), SPRITE_FLAG_SHADOW);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_IVAN), SPRITE_FLAG_SHADOW);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    fdScriptWait(40);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_MIA), SPRITE_FLAG_SHADOW);
    fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_GARET, 0.8);
    fdActorSetSpeed(ACTOR_IVAN, 0.8);
    fdActorSetSpeed(ACTOR_MIA, 0.8);
    fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
        fdActorSetDestination(ACTOR_GARET, leader->x.part[0].integer, leader->z.part[0].integer);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
        fdActorSetDestination(ACTOR_IVAN, leader->x.part[0].integer, leader->z.part[0].integer);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
        fdActorSetDestination(ACTOR_MIA, leader->x.part[0].integer, leader->z.part[0].integer);
    fdActorWaitForMoveById(ACTOR_MIA);
    fdActorSetPos(ACTOR_MIA, 0, 0);
    fdActorWaitForMoveById(ACTOR_IVAN);
    fdActorSetPos(ACTOR_IVAN, 0, 0);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorSetPos(ACTOR_GARET, 0, 0);
    fdScriptEnd();
}

#include "shared/actor_distance.c"
#include "shared/floating_platform.c"

// Walking on from a platform, or from the tower edge beside one, makes Isaac crouch and leap two
// metatiles to the next platform or tower top, if there is ground there to land on.
//
// The metatiles are the platforms in the gaps west and east of the beacon's tower, metatiles (6,
// 12), (8, 12), (10, 12), (27, 12), (29, 12) and (31, 12), and the ground beside them: the foot of
// the stairs either side of the beacon's tower, (12, 12) and (25, 12), and the east tower top, (31,
// 10). Before the leap every platform is set floating again.
static void scMercuryLighthouseAerieJumpToNextPlatform(void)
{
    ACTOR    *actor;
    FIELD_POS pos;
    Sint32    angle;
    Uint8     flags;

    actor = fdActorGet(ACTOR_PARTY_LEADER);
    angle = FACING_NEAREST_EIGHTH(actor->facing);
    flags = actor->motionFlags;
    SET_POSITION(&pos,
                 (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8),
                 actor->y.fixed,
                 (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8));
    syVectorAddPolarOffset(toFIXED(32), angle, (FIXED *)&pos);
    if (syFieldActorCheckMovementCollision(actor, &pos) == 0)
    {
        gmFlagClear(FLAG_ON_PLATFORM);
        scMercuryLighthouseAerieFloatPlatforms();
        fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
        sySynch(6);
        sySoundPlay(SE_JUMP);
        fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
        actor->speedLimit    = toFIXED(3);
        actor->acceleration  = toFIXED(2);
        actor->velocityY     = toFIXED(4);
        actor->motionFlags  &= ACTOR_JUMP_KEEPS_MOTION;
        fdActorSetSpriteFlags(actor, 0);
        fdActorMoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(pos.x), FIXEDtoINT(pos.z));
        fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
        fdActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
        actor->motionFlags = flags;
    }
}

// Landing on one of the floating platforms makes it dip under Isaac and spring back up.
//
// The platforms are metatiles (6, 12), (8, 12) and (10, 12) west of the beacon's tower and (27,
// 12), (29, 12) and (31, 12) east of it. The one under him stops floating and holds still while he
// stands on it (FLAG_ON_PLATFORM).
static void scMercuryLighthouseAerieLandOnPlatform(void)
{
    ACTOR *leader;
    ACTOR *platform;
    Sint32 platformId;

    leader = fdActorGet(ACTOR_PARTY_LEADER);
    fdScriptStart();
    platformId                           = scActorFindNearestFloatingPlatform();
    MercuryLighthouseAerieLandedPlatform = platformId;
    if (platformId != 0)
    {
        gmFlagSet(FLAG_ON_PLATFORM);
        platform               = fdActorGet(MercuryLighthouseAerieLandedPlatform);
        platform->motionFlags  = 0;
        leader->motionFlags   &= ~ACTOR_FOLLOWS_TERRAIN;
        platform->y.fixed     -= toFIXED(3);
        leader->y.fixed       -= toFIXED(3);
        leader->groundHeight  -= toFIXED(3);
        sySynch(2);
        platform->y.fixed    -= toFIXED(2);
        leader->y.fixed      -= toFIXED(2);
        leader->groundHeight -= toFIXED(2);
        sySynch(10);
        platform->y.fixed    += toFIXED(2);
        leader->y.fixed      += toFIXED(2);
        leader->groundHeight += toFIXED(2);
        sySynch(4);
        platform->y.fixed    += toFIXED(2);
        leader->y.fixed      += toFIXED(2);
        leader->groundHeight += toFIXED(2);
        sySynch(4);
        platform->y.fixed    += toFIXED(1);
        leader->y.fixed      += toFIXED(1);
        leader->groundHeight += toFIXED(1);
    }
    fdScriptEnd();
}

// Nothing visible happens when Isaac steps off the platforms onto the foot of the stairs either
// side of the beacon's tower, metatiles (12, 12) and (25, 12), or onto the east tower top at
// metatile (31, 10). The game sets every platform floating again, the one he left included, which
// stopped under him when he landed.
//
// The leap from one platform to the next calls it too.
static void scMercuryLighthouseAerieFloatPlatforms(void)
{
    fdActorGet(ACTOR_PARTY_LEADER)->motionFlags         = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    fdActorGet(ACTOR_FIRST_PLATFORM)->motionFlags       = ACTOR_FLOATS;
    fdActorGet(ACTOR_FIRST_PLATFORM + 1)->motionFlags   = ACTOR_FLOATS;
    fdActorGet(ACTOR_FIRST_PLATFORM + 2)->motionFlags   = ACTOR_FLOATS;
    fdActorGet(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags  = ACTOR_FLOATS;
    fdActorGet(ACTOR_SECOND_LIFT_PLATFORM)->motionFlags = ACTOR_FLOATS;
    fdActorGet(ACTOR_FIRST_PLATFORM + 5)->motionFlags   = ACTOR_FLOATS;
}

// Climbing the wide stairs up to the beacon, at metatiles (20, 14) and (21, 14), the party finds it
// already lit, Menardi rides the lift down with Jenna, Kraden and Felix, and Saturos fights the
// party alone.
//
// Garet sees `It's too late! The lighthouse has already been lit!` The party gathers on the stairs
// and talks it over with Mia before Jenna calls out from the west tower top. It plays until Saturos
// is beaten (FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED).
static void scMercuryLighthouseAerieConfrontSaturos(void)
{
    ACTOR *actor;

    fdScriptStart();
    fdActorGet(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags &= ~(ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
    fdActorSetSpeed(ACTOR_GARET, 0.8);
    fdActorSetSpeed(ACTOR_IVAN, 0.8);
    fdActorSetSpeed(ACTOR_MIA, 0.8);
    actor = fdActorGet(ACTOR_PARTY_LEADER);
    if (actor != NULL)
    {
        fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
    }
    actor = fdActorGet(ACTOR_PARTY_LEADER);
    if (actor != NULL)
    {
        fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = fdActorGet(ACTOR_PARTY_LEADER);
    if (actor != NULL)
    {
        fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    sySynch(1);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 344, 232);
    fdActorWalkTo(ACTOR_GARET, 328, 232);
    fdActorWalkTo(ACTOR_IVAN, 344, 248);
    fdActorWalkTo(ACTOR_MIA, 328, 248);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorWaitForMoveById(ACTOR_IVAN);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdActorWaitForMoveById(ACTOR_MIA);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdScriptWait(50);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdCameraSetSpeed(1.5);
    fdCameraMoveTo(328, 40, 176, 1);
    fdCameraGetActor()->motionFlags = 0;
    fdActorSetSpeed(ACTOR_GARET, 1.5);
    fdActorWalkToAndWait(ACTOR_GARET, 328, 216);
    fdCameraWaitForArrival();
    fdActorSetSpeed(ACTOR_GARET, 0.8);
    fdScriptWait(60);
    fdCameraMoveTo(344, 24, 232, 1);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
    fdCameraWaitForArrival();
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptSetMessageId(MsgMercuryLighthouseAlreadyLit);
    // "It's too late! The lighthouse has already been lit!"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    fdActorSetSpeed(ACTOR_MIA, 1.5);
    fdActorWalkToAndWait(ACTOR_MIA, 328, 232);
    fdScriptWait(10);
    // "It... It can't be!"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "The beacon cannot be lit without the Mercury Star..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 30);
    fdActorTurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(60);
    fdActorWalkToAndWait(ACTOR_IVAN, 336, 248);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdScriptWait(30);
    // "...The Elemental Stars were stolen from Sol Sanctum, on Mt...."
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 30);
    // "Mt. Aleph... Sol Sanctum... Are you healers from Sol Sanctum?"
    fdScriptOpenMessage(ACTOR_MIA, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
        fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
        // "We came from a village called Vale to recover the three..."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
        fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "But, unlike you, we are not healers."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
        FieldStateWork->msg += 2;
    }
    else
    {
        FieldStateWork->msg += 2;
        fdScriptWait(20);
        fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
        fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 10);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
        // "We came from Mt. Aleph, but we're not healers."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
        fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(20);
        // "We came from a village called Vale to recover the three..."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    }
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Did you say Vale?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(20);
    // "Our village was there to protect Sol Sanctum, but the Stars..."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    // "That sounds just like my clan..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 10);
    fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_HEAD_BOWED);
    // "I have failed in the one duty placed upon me... This is..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 60);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdActorSetPos(ACTOR_JENNA, 216, 200);
    // "Isaac! Garet!"
    fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
    fdActorSetPos(ACTOR_JENNA, 120, 160);
    fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
    fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
    fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 5);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 5);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 5);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdCameraSetSpeed(3);
    fdCameraMoveTo(120, -24, 168, 1);
    fdCameraWaitForArrival();
    fdScriptWait(40);
    fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
    fdScriptWait(20);
    sySoundPlay(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);
    // "So it's you, Isaac!"
    fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    fdScriptWait(20);
    // "Heh heh! Those kids are still alive?"
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH, 60);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
    // "You came all this way to save Jenna and Kraden?"
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorSetPos(ACTOR_GARET, 264, 288);
    // "That's not all... We're after the Elemental Stars!"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorSetPos(ACTOR_GARET, 328, 216);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "In other words, they've come to stop us."
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdScriptWait(20);
    // "Then we have no choice... Let's take care of them now,..."
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTHWEST, 10);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
    fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
    fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 30);
    fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 10);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdActorSetSpeed(ACTOR_FELIX, 0.8);
    fdActorWalkToAndWait(ACTOR_FELIX, 104, 168);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 10);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
    fdActorSetPos(ACTOR_SATUROS, 264, 288);
    // "Menardi... Wait!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorSetPos(ACTOR_SATUROS, 296, 120);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
    fdCameraSetSpeed(1.5);
    fdCameraMoveTo(232, 40, 152, 1);
    fdActorSetSpeed(ACTOR_SATUROS, 0.8);
    fdActorWalkToAndWait(ACTOR_SATUROS, 272, 128);
    fdActorWalkToAndWait(ACTOR_SATUROS, 264, 152);
    fdActorWalkToAndWait(ACTOR_SATUROS, 280, 168);
    fdCameraWaitForArrival();
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
    fdScriptWait(20);
    // "Oh, Saturos... They want to..."
    fdScriptShowMessage(ACTOR_MENARDI, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    fdScriptWait(20);
    // "I heard everything."
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    fdCameraMoveTo(296, 40, 216, 1);
    fdActorWalkToAndWait(ACTOR_SATUROS, 304, 176);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
    fdCameraWaitForArrival();
    fdScriptWait(20);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    fdActorStartRepeatedMotion(ACTOR_GARET, 1);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdScriptWait(20);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    fdScriptWait(20);
    // "They were lucky to survive the eruption of Mt. Aleph."
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 70);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    fdScriptWait(30);
    // "You have endured so much, and now you want to throw your..."
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    fdScriptWait(20);
    // "Fine. If that is your wish, prepare to have it granted."
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    fdScriptWait(20);
    // "Saturos... Are you going to fight them alone?"
    fdScriptShowMessage(ACTOR_MENARDI, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(40);
    // "Menardi, I can't expect you to leave our hostages alone and..."
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    fdScriptWait(30);
    // "Umm... That's true..."
    fdScriptShowMessage(ACTOR_MENARDI, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(30);
    // "Besides, I want to see how much their powers have developed."
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    fdScriptWait(20);
    fdActorGet(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkTo(ACTOR_GARET, 328, 224);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 344, 224);
    fdActorWalkTo(ACTOR_IVAN, 344, 232);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorGet(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    fdActorWaitForMoveById(ACTOR_IVAN);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(30);
    fdActorSetPos(ACTOR_MENARDI, 168, 200);
    // "Are you sure about this, Saturos?"
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorSetPos(ACTOR_MENARDI, 104, 200);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    fdScriptWait(20);
    // "Leave me be! Head for the next lighthouse-now!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdCameraSetSpeed(3);
    fdCameraMoveTo(120, -24, 168, 1);
    fdCameraWaitForArrival();
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 20);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdScriptWait(20);
    // "Come here, child..."
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 70);
    fdActorTurnToFaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 0);
    fdScriptWait(50);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    // "Do you mean to defy me?"
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 20);
    fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdScriptWait(20);
    // "I should teach you a lesson, child, but..."
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Felix, I'd get your sister to obey me, if you hope to keep..."
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorSetAttachedEffect(ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Jenna, please... Just do as she says for now."
    fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
    fdActorSetAttachedEffect(ACTOR_JENNA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    // "Felix..."
    fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 40);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    fdScriptWait(20);
    // "It's all right, Jenna."
    fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 30);
    fdActorSetSpeed(ACTOR_JENNA, 0.7);
    fdActorSetSpeed(ACTOR_KRADEN, 0.7);
    fdActorWalkTo(ACTOR_JENNA, 128, 144);
    fdActorWalkToAndWait(ACTOR_KRADEN, 120, 136);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    fdActorWaitForMoveById(ACTOR_JENNA);
    fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 20);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
    fdActorGet(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkToAndWait(ACTOR_FELIX, 88, 152);
    fdActorGet(ACTOR_FELIX)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
    fdActorSetAnimation(ACTOR_MENARDI, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    fdScriptWait(40);
    fdActorSetSpeedAndAcceleration(ACTOR_MENARDI, 3, 2);
    fdActorGet(ACTOR_MENARDI)->velocityY = toFIXED(4);
    sySoundPlay(SE_JUMP);
    fdActorGet(ACTOR_MENARDI)->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
    fdActorSetSpriteFlags(fdActorGet(ACTOR_MENARDI), 0);
    fdActorGet(ACTOR_FIRST_LIFT_PLATFORM)->motionFlags = ACTOR_FLOATS;
    fdActorMoveToAndWait(ACTOR_MENARDI, 104, 168);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_MENARDI), SPRITE_FLAG_SHADOW);
    fdActorGet(ACTOR_MENARDI)->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 30);
    fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 10);
    // "Isaac and Garet shouldn't have come after us..."
    fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "I'm sorry, Isaac, Garet..."
    fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
    // "I'm counting on you!"
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
    fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
    scMercuryLighthouseAerieLowerLift();
    fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
    fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
    fdCameraMoveTo(304, 32, 216, 1);
    fdCameraWaitForArrival();
    fdActorSetPos(ACTOR_KRADEN, 272, 280);
    // "Isaac! Garet! Don't die!"
    fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
    fdActorSetPos(ACTOR_KRADEN, 0, 0);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(20);
    // "Jenna... Kraden!"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorSetSpeed(ACTOR_GARET, 1.5);
    fdActorWalkToAndWait(ACTOR_GARET, 312, 216);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 10);
    // "If you want to save them, you'll have to beat me, first!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorWalkToAndWait(ACTOR_SATUROS, 312, 184);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHEAST, 20);
    // "No! Get back!"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    fdActorSetSpeed(ACTOR_GARET, 2);
    fdActorGet(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkToAndWait(ACTOR_GARET, 328, 224);
    fdScriptWait(1);
    fdActorGet(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    fdActorStartRepeatedMotion(ACTOR_GARET, 1);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    fdScriptWait(10);
    // "What's the matter!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptWait(10);
    // "Saturos is extremely powerful. I could sense it when I faced him."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    // "You have to be very careful when fighting him... We have to match..."
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Match me? Pah! You think you can match the great Saturos?"
    fdScriptOpenMessage(ACTOR_SATUROS, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "You overestimate your chances."
        fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
        FieldStateWork->msg++;
    }
    else
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        FieldStateWork->msg++;
        // "And yet you still want to fight? Ship of fools..."
        fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    }
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptWait(20);
    // "Fine then... I'll just teach you all a lesson!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorWalkToAndWait(ACTOR_SATUROS, 328, 200);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
    fdActorGet(ACTOR_SATUROS)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkToAndWait(ACTOR_SATUROS, 336, 208);
    fdScriptWait(1);
    fdActorGet(ACTOR_SATUROS)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "What happened? Saturos is moving strangely."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    // "What's this!? The light of Mercury... It's weakening my Psynergy!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptWait(20);
    // "I must finish this quickly... Come on, fools!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorWalkToAndWait(ACTOR_SATUROS, 336, 216);
    fdScriptSetAfterBattleDestination(SCENE_MERCURY_LIGHTHOUSE_AERIE,
                                      MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE);
    SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
    // Saturos: ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, slot 2.
    fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_TRET_MERCURY, 2);
    fdScriptEnd();
}

// Back from the battle, Saturos is beaten, but Alex appears, stalls until Saturos recovers and
// leaves with him on the lift.
//
// It plays at MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE, unless the party returns there after
// another battle. If the lift is down it is raised for them first.
static void scMercuryLighthouseAerieAlexAppears(void)
{
    fdScriptStart();
    fdActorGet(ACTOR_LIFT_STATUE)->motionFlags = 0;
    fdActorSetPos(ACTOR_PARTY_LEADER, 344, 224);
    fdActorSetPos(ACTOR_GARET, 328, 224);
    fdActorSetPos(ACTOR_IVAN, 344, 232);
    fdActorSetPos(ACTOR_MIA, 328, 232);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorSetPos(ACTOR_SATUROS, 336, 176);
    fdActorSetAnimation(ACTOR_SATUROS, SATUROS_ANIMATION_LIE);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_SATUROS), 0);
    fdCameraMoveTo(336, CAMERA_KEEP, 208, 0);
    sySynch(1);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);

    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(60);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
    fdScriptSetMessageId(MsgMercuryGot);
    // "Isaac, we got him!"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptWait(20);
    // "Who would have thought that Mercury Lighthouse held such..."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 20);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Saturos... You said you were weakened. What did you mean?"
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptWait(20);
    // "I... I meant nothing..."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 30);
    fdActorSetPos(ACTOR_ALEX, 312, 112);
    sySoundPlay(SE_STOP_ACTION_SE);
    // "I shall tell you what he means..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    // "That voice! Could it be?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 30);
    sySoundPlay(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
    fdCameraSetSpeed(0.8);
    fdCameraGetActor()->motionFlags = 0;
    fdCameraMoveTo(336, CAMERA_KEEP, 168, 1);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_ALEX, 0.8);
    fdActorWalkToAndWait(ACTOR_ALEX, 344, 136);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHWEST, 20);
    fdCameraWaitForArrival();
    // "Alex!"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 30);
    fdCameraMoveTo(336, CAMERA_KEEP, 184, 1);
    fdActorWalkToAndWait(ACTOR_ALEX, 344, 160);
    fdActorWalkToAndWait(ACTOR_ALEX, 328, 168);
    fdActorWalkToAndWait(ACTOR_ALEX, 312, 176);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
    fdCameraWaitForArrival();
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    fdScriptWait(20);
    // "It's been a while, Mia."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "Alex, do you realize what you've done?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Ha! Of course... I have freed a great power that has long..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 60);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
    // "Mercury, the lighthouse of Water... What a mighty force."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Alex, you... Are you mad!?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorStartRepeatedMotion(ACTOR_ALEX, 2);
    fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
    // "No, Mia... Don't you understand?"
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "The lighthouse granted you great power during your battle..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "It's true... I could use my power without ever depleting it."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    fdScriptWait(20);
    // "Don't you see? The Mercury Lighthouse supplied you with..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdScriptWait(20);
    // "Psynergy? Are you telling me my powers come from Psynergy?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorWalkToAndWait(ACTOR_ALEX, 312, 184);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
    // "You and I are both members of Mercury Clan... Masters of..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
    // "Mia is an Adept too, isn't she?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        // "Geez, Isaac, you knew all along, and you didn't tell us?"
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
        FieldStateWork->msg++;
    }
    else
    {
        FieldStateWork->msg++;
        fdActorRunRepeatedMotion(ACTOR_GARET, 1);
        // "I know it's hard to believe, but it looks like Mia's one of us."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    }
    // "Tell me, Alex... Why wasn't Saturos able to use his full power?"
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 20);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    fdScriptWait(20);
    // "Ah, yes... I shall explain it all."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 20);
    // "Saturos is aligned with Mars, the clan of fire."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Fire and water are opposing elements... His full power was..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Didn't Saturos know that?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Of course he knew."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 20);
    // "However, he failed to appreciate the power of this lighthouse."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
    fdScriptWait(30);
    // "As did I..."
    fdScriptShowMessage(ACTOR_ALEX, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(30);
    // "Is that why we were able to defeat Saturos?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "No... There is more to it."
    fdScriptShowMessage(ACTOR_ALEX, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
    fdScriptWait(30);
    // "I stood by, watching your battle from afar..."
    fdScriptShowMessage(ACTOR_ALEX, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    // "You were watching us?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    fdScriptWait(30);
    // "Yes. I did not help Saturos, because I was certain he would win."
    fdScriptShowMessage(ACTOR_ALEX, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
    fdScriptWait(30);
    // "But I was wrong... You have become such great fighters in a..."
    fdScriptShowMessage(ACTOR_ALEX, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_ALEX, FACING_EAST_SOUTHEAST, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_SATUROS, 8);
    fdScriptWait(45);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_STAND);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
    fdScriptWait(40);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    // "Saturos is up again!? But we beat him!"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTHEAST, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    // "Me? Beaten? Never! I lost to this lighthouse, not to you!"
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
    fdScriptWait(30);
    // "I was merely stalling you until Saturos regained his strength."
    fdScriptShowMessage(ACTOR_ALEX, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(30);
    // "You were just buying time! That's not fair, Alex!"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorTurnToFaceEachOther(ACTOR_ALEX, ACTOR_SATUROS, 0);
    fdScriptWait(35);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    fdScriptWait(20);
    fdActorWalkToAndWait(ACTOR_ALEX, 328, 176);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
    fdActorSetAnimation(ACTOR_ALEX, 5);
    fdActorSetAnimation(ACTOR_SATUROS, 7);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(20);
    // "I won't let Saturos escape!"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "And just what are you going to do? Will you finish him off?"
    fdScriptOpenMessage(ACTOR_ALEX, 0);
    fdScriptWait(30);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 0);
    fdScriptWait(20);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(30);
        // "If that's the case, you will have to fight me, as well. I've..."
        fdScriptShowMessage(ACTOR_ALEX, 0);
        FieldStateWork->msg++;
    }
    else
    {
        fdScriptWait(30);

        FieldStateWork->msg++;
        // "Yes, Isaac. You seem like a smart warrior. You can't beat..."
        fdScriptShowMessage(ACTOR_ALEX, 0);
    }
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdScriptWait(20);
    // "And I doubt you are the type to 'finish' anyone off!"
    fdScriptShowMessage(ACTOR_ALEX, 0);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    fdScriptWait(20);
    // "Of course, you are welcome to believe whatever you want."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    scMercuryLighthouseAerieSaturosAndAlexWarpOut();
    fdScriptWait(20);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 5);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 5);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 5);
    fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 5);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 5);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 5);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 5);
    fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 5);
    fdCameraGetActor()->motionFlags = 0;
    fdCameraMoveTo(280, CAMERA_KEEP, 232, 1);
    fdCameraWaitForArrival();
    fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
    fdActorSetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
    fdActorSetPos(ACTOR_SATUROS, 240, 208);
    fdActorSetPos(ACTOR_ALEX, 232, 208);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
    scMercuryLighthouseAerieSaturosAndAlexWarpIn();
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 15);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 328, 208);
    fdActorWalkTo(ACTOR_IVAN, 336, 224);
    fdActorWalkToAndWait(ACTOR_GARET, 312, 216);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
    fdActorWaitForMoveById(ACTOR_IVAN);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
    // "He warped again!"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "Again? He's done that before?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 20);
    // "Yeah, in Sol Sanctum..."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
    // "That Psynergy...the warp. Does that mean Mia can use it?"
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "No, it's impossible. Even Alex didn't have this kind of power before."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    // "Ah, well... I can't stay the same Alex you knew forever..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdCameraSetSpeed(3);
    {
        MAP_GRID_LAYER_SCROLL *lift;
        fdCameraMoveTo(152, CAMERA_KEEP, 216, 1);
        lift          = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
        lift->offsetY = toFIXED(896);
        fdMapGridCenterOnFollowedPoint();
        sySynch(1);
        fdActorGet(ACTOR_LIFT_STATUE)->motionFlags = 0;
        fdActorSetPos(ACTOR_LIFT_STATUE, 104, 264);
        fdActorGet(ACTOR_LIFT_STATUE)->y.fixed = -toFIXED(32);
        fdActorGet(ACTOR_LIFT_STATUE)->targetY = -toFIXED(32);
        fdMapGridCopyMetatilesTo(29, 74, 4, 74, 5, 4);
        fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
        fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
        scMercuryLighthouseAerieRaiseLift();
        fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
        fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
        fdCameraWaitForArrival();
        fdScriptWait(30);
        fdActorFaceDirection(ACTOR_ALEX, FACING_WEST, 20);
        // "Well, it seems that my ride has returned while we were speaking."
        fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
        fdCameraMoveTo(280, CAMERA_KEEP, 216, 1);
        fdCameraWaitForArrival();
        fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 20);
        fdActorSetAnimation(ACTOR_ALEX, 5);
        // "And so, I bid you adieu."
        fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
        fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
        // "Isaac, Saturos still has the Elemental Stars!"
        fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
        fdActorRunRepeatedMotion(ACTOR_GARET, 1);
        fdScriptWait(20);
        // "That's right! We can't let them leave now!"
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
        fdActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
        // "Oh? And you want to take the Elemental Stars from Saturos?"
        fdScriptOpenMessage(ACTOR_ALEX, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptWait(20);
            fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
            fdScriptWait(20);
            // "Ha! Too bad! Menardi has them. You'll have to ask her!"
            fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
            FieldStateWork->msg++;
        }
        else
        {
            FieldStateWork->msg++;
            fdScriptWait(20);
            // "Ha! You're right... Menardi has the Elemental Stars!"
            fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
        }
    }
    fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
    fdScriptWait(20);
    // "That reminds me..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Do you still have the Mars Star?"
    fdScriptOpenMessage(ACTOR_ALEX, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
        fdScriptWait(20);
        // "I appreciate your honesty."
        fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
        FieldStateWork->msg++;
    }
    else
    {
        FieldStateWork->msg++;
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_ALEX, 1);
        fdScriptWait(20);
        // "Are you telling the truth? Well, I shall believe you for now."
        fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    }
    fdScriptWait(20);
    scMercuryLighthouseAerieSaturosAndAlexWarpOut();
    fdScriptWait(20);
    fdCameraMoveTo(128, CAMERA_KEEP, 200, 1);
    fdCameraWaitForArrival();
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_STAND);
    fdActorSetAnimation(ACTOR_ALEX, ANIM_STAND);
    fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_HIDDEN);
    fdActorSetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_HIDDEN);
    fdActorSetPos(ACTOR_SATUROS, 120, 152);
    fdActorSetPos(ACTOR_ALEX, 112, 160);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH_SOUTHEAST, 0);
    scMercuryLighthouseAerieSaturosAndAlexWarpIn();
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 20);
    // "I suppose you'll be willing to pursue us for the Elemental Stars?"
    fdScriptOpenMessage(ACTOR_ALEX, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
        fdScriptWait(20);
        // "I look forward to it. I shall see you soon, then."
        fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
        FieldStateWork->msg++;
    }
    else
    {
        FieldStateWork->msg++;
        fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "That's too bad... I shall look forward to seeing you again."
        fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    }
    // "Good-bye."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 1);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 20);
    // "I look forward to our next battle."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 20);
    fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
    fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
    scMercuryLighthouseAerieLowerLift();
    fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
    fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdCameraMoveTo(336, CAMERA_KEEP, 216, 1);
    fdCameraWaitForArrival();
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    gmPlayMusic();
    // "They're gone..."
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 22);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(10);
    // "Shoot... We couldn't even save Jenna..."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    // "I don't deserve to be a healer..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
    // "We couldn't stop them from lighting the beacon."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 20);
    fdActorSetAnimation(ACTOR_MIA, ANIM_MIA_HEAD_BOWED);
    fdActorGet(ACTOR_MIA)->scaleX = -toFIXED(1);
    fdScriptWait(20);
    // "I've failed my clan, failed in my duty..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 40);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "That's not true! We can still beat them, right, Isaac?"
    fdScriptOpenMessage(ACTOR_IVAN, 0);
    fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    fdActorGet(ACTOR_MIA)->scaleX = toFIXED(1);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        fdScriptWait(20);
        // "That's right, Isaac! All we have to do is stop them from lighting..."
        fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
        FieldStateWork->msg++;
    }
    else
    {
        FieldStateWork->msg++;
        fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
        fdScriptWait(20);
        // "You're not helping here! We're fine as long as the lighthouses..."
        fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    }
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 20);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    // "The beacon of Mercury Lighthouse has been lit, but we'll stop them..."
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 30);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(10);
    // "That's right. We can save Jenna at the next lighthouse."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    // "Yes. We won't let Alex get away with this..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    // "Let's go after them now!"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    fdActorSetSpeed(ACTOR_MIA, 0.8);
    fdActorSetSpeed(ACTOR_GARET, 0.8);
    fdActorWalkToAndWait(ACTOR_MIA, 328, 216);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdActorGet(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkTo(ACTOR_GARET, 312, 200);
    fdActorWalkTo(ACTOR_MIA, 280, 216);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    fdActorWaitForMoveById(ACTOR_MIA);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 20);
    // "What's the matter? We should hurry!"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(20);
    // "We? Mia, are you coming along with us?"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    // "But the people of Imil need you, Mia."
    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "It's okay."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
    // "'The fountain shall be filled as long as light fills the..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorFaceDirection(ACTOR_MIA, FACING_EAST, 20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "I have mixed feelings about leaving, but Imil will be fine as long..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 30);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 40);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    // "We should hurry if we're going to follow them."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 312, 216);
    fdActorWalkTo(ACTOR_GARET, 312, 216);
    fdActorWalkTo(ACTOR_IVAN, 312, 216);
    fdActorWalkTo(ACTOR_MIA, 312, 216);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorSetPos(ACTOR_GARET, 0, 0);
    fdActorWaitForMoveById(ACTOR_IVAN);
    fdActorSetPos(ACTOR_IVAN, 0, 0);
    fdActorWaitForMoveById(ACTOR_MIA);
    fdActorSetPos(ACTOR_MIA, 0, 0);
    {
        MAP_GRID_LAYER_SCROLL *lift = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];

        lift->offsetY               = toFIXED(1024);
    }
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    fdMapGridCopyMetatilesTo(4, 70, 4, 74, 5, 4);
    gmFlagSet(FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED);
    gmFlagSet(FLAG_SATUROS_BEATEN_AT_MERCURY);
    fdScriptEnd();
}

// Walking down the stairs on the west side of the beacon's tower, at metatile (12, 9), brings the
// lift back up while it is down.
//
// The camera pans west to the empty space, the lift rises into it with its statue and the two
// platforms it carries, and the camera comes back to Isaac. It stays up (FLAG_LIFT_UP).
static void scMercuryLighthouseAerieCallLift(void)
{
    MAP_GRID_LAYER_SCROLL *lift;

    lift = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
    fdScriptStart();
    lift->offsetY = toFIXED(896);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    fdActorGet(ACTOR_LIFT_STATUE)->motionFlags = 0;
    fdActorSetPos(ACTOR_LIFT_STATUE, 104, 264);
    fdActorGet(ACTOR_LIFT_STATUE)->y.fixed = -toFIXED(32);
    fdActorGet(ACTOR_LIFT_STATUE)->targetY = -toFIXED(32);
    fdCameraGetActor()->motionFlags        = 0;
    fdCameraSetSpeed(0.8);
    fdCameraMoveTo(128, CAMERA_KEEP, 184, 1);
    fdCameraWaitForArrival();
    fdScriptWait(30);
    fdMapGridCopyMetatilesTo(29, 74, 4, 74, 5, 4);
    fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
    fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
    scMercuryLighthouseAerieRaiseLift();
    fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 1);
    fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 1);
    fdScriptWait(20);
    gmFlagSet(FLAG_LIFT_UP);
    fdScriptEnd();
}

// Stepping onto the lift from the westmost platform, at metatile (6, 10), rides it down to the
// ground before Mercury Lighthouse, by the Fountain of Healing.
//
// Isaac walks to the middle of the lift and turns to face south, and the lift sinks with him before
// the party leaves by MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR.
static void scMercuryLighthouseAerieRideLiftDown(void)
{
    fdScriptStart();
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 104, 152);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 60);
    fdActorSetSpritePriority(ACTOR_FIRST_LIFT_PLATFORM, 0);
    fdActorSetSpritePriority(ACTOR_SECOND_LIFT_PLATFORM, 0);
    scMercuryLighthouseAerieLowerLift();
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    syImportScriptSetExitRequest(MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR);
    fdScriptEnd();
}

// Saturos and Alex vanish in a column of sparkles.
static void scMercuryLighthouseAerieSaturosAndAlexWarpOut(void)
{
    EFFECT_OPTIONS options;
    ACTOR         *left;
    ACTOR         *right;
    Uint32         frame;
    FIXED          x;
    FIXED          r;

    left  = fdActorGet(ACTOR_SATUROS);
    right = fdActorGet(ACTOR_ALEX);
    sySoundPlay(SE_WARP);
    fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
    fdActorSetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_SATUROS), 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_ALEX), 0);
    options.priority    = 1;
    options.palette     = SPRITE_COLOR_BRG;
    options.type        = SPRITE_PARTICLE;
    options.startScaleX = toFIXED(2) / 5;
    options.startScaleY = toFIXED(3);
    for (frame = 0; frame < WARP_TICKS; frame++)
    {
        fdScriptWait(1);
        if (frame & 1)
        {
            r  = toFIXED(syRandomBelow(24));
            x  = left->x.fixed;
            x += r;
            x -= toFIXED(12);
            fdEffectSpawn(x,
                          left->y.fixed + toFIXED(syRandomBelow(32)) - toFIXED(16),
                          left->z.fixed,
                          0,
                          toFIXED(4),
                          0,
                          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY
                              | EFFECT_USE_PALETTE,
                          &options);
        }
        else
        {
            r  = toFIXED(syRandomBelow(24));
            x  = right->x.fixed;
            x += r;
            x -= toFIXED(12);
            fdEffectSpawn(x,
                          right->y.fixed + toFIXED(syRandomBelow(32)) - toFIXED(16),
                          right->z.fixed,
                          0,
                          toFIXED(4),
                          0,
                          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY
                              | EFFECT_USE_PALETTE,
                          &options);
        }
    }
    fdActorSetPos(ACTOR_SATUROS, 0, 0);
    fdActorSetPos(ACTOR_ALEX, 0, 0);
}

// Saturos and Alex appear in a column of sparkles where they have been placed.
static void scMercuryLighthouseAerieSaturosAndAlexWarpIn(void)
{
    EFFECT_OPTIONS options;
    ACTOR         *left;
    ACTOR         *right;
    Uint32         frame;
    FIXED          x;
    FIXED          r;

    left  = fdActorGet(ACTOR_SATUROS);
    right = fdActorGet(ACTOR_ALEX);
    sySoundPlay(SE_WARP);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_SATUROS), 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_ALEX), 0);
    options.priority    = 1;
    options.palette     = SPRITE_COLOR_BRG;
    options.type        = SPRITE_PARTICLE;
    options.startScaleX = toFIXED(2) / 5;
    options.startScaleY = toFIXED(3);
    for (frame = 0; frame < WARP_TICKS; frame++)
    {
        fdScriptWait(1);
        if (frame & 1)
        {
            r  = toFIXED(syRandomBelow(24));
            x  = left->x.fixed;
            x += r;
            x -= toFIXED(12);
            fdEffectSpawn(x,
                          left->y.fixed + toFIXED(syRandomBelow(32)) + toFIXED(32),
                          left->z.fixed,
                          0,
                          -toFIXED(4),
                          0,
                          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY
                              | EFFECT_USE_PALETTE,
                          &options);
        }
        else
        {
            r  = toFIXED(syRandomBelow(24));
            x  = right->x.fixed;
            x += r;
            x -= toFIXED(12);
            fdEffectSpawn(x,
                          right->y.fixed + toFIXED(syRandomBelow(32)) + toFIXED(32),
                          right->z.fixed,
                          0,
                          -toFIXED(4),
                          0,
                          EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY
                              | EFFECT_USE_PALETTE,
                          &options);
        }
        if (frame == WARP_IN_BLINK_TICK)
        {
            fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_BLINK);
            fdActorSetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_BLINK);
        }
    }
    fdActorSetSpriteColorById(ACTOR_SATUROS, SPRITE_COLOR_NORMAL);
    fdActorSetSpriteColorById(ACTOR_ALEX, SPRITE_COLOR_NORMAL);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_SATUROS), SPRITE_FLAG_SHADOW);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_ALEX), SPRITE_FLAG_SHADOW);
}

// Writes to out the ids of the actors standing on the lift, columns 4 to 8 and three rows from its
// top, while its map layer is scrolled y pixels down.
static void scMercuryLighthouseAerieFindActorsOnLift(Sint32 *out, Sint32 y)
{
    FIELD_STATE *state;
    Uint32       slot;

    state = FieldStateWork;
    y     = 64 - FIELD_METATILE(y);
    for (slot = 0; slot < sizeof(state->actors) / sizeof(state->actors[0]); slot++)
    {
        ACTOR *actor = state->actors[slot];

        if (actor != NULL)
        {
            Sint32 metatile = FIELD_METATILE(actor->x.fixed);
            Sint32 row      = FIELD_METATILE(actor->z.fixed);

            if (metatile >= 4 && metatile <= 8 && y + 8 <= row && row < y + 11)
                *out++ = slot;
        }
    }
}

// How long the lift takes to sink, speeding up every fourth tick.
#define LIFT_DOWN_TICKS    228

// How long the lift takes to rise, and the last ticks of it in which it slows every fourth tick.
#define LIFT_UP_TICKS      86
#define LIFT_SLOWING_TICKS 10

// The lift sinks with everything standing on it, speeding up.
static void scMercuryLighthouseAerieLowerLift(void)
{
    Sint32                 ids[5];
    MAP_GRID_LAYER_SCROLL *lift;
    Uint32                 i;
    Uint32                 j;
    Uint32                 n;
    FIXED                  speed;

    lift  = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
    speed = toFIXED(1) / 10;
    n     = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = ACTOR_ID_COUNT;
    scMercuryLighthouseAerieFindActorsOnLift(ids, lift->offsetY);
    for (i = 0; i <= 4; i++)
    {
        if (ids[i] == ACTOR_ID_COUNT)
            break;
        fdActorGet(ids[i])->motionFlags = 0;
        n++;
    }
    sySoundPlay(SE_LIFT);
    for (i = 0; i < LIFT_DOWN_TICKS; i++)
    {
        lift->offsetY -= speed;
        for (j = 0; j < n; j++)
        {
            fdActorGet(ids[j])->z.fixed += speed;
            fdActorGet(ids[j])->targetZ  = fdActorGet(ids[j])->z.fixed;
        }
        if ((i & 3) == 3)
            speed += toFIXED(1) / 10;
        if (speed >= toFIXED(3) / 2)
            speed = toFIXED(3) / 2;
        sySynch(1);
    }
    for (i = 0; i < n; i++)
        fdActorGet(ids[i])->motionFlags = 0;
}

// The lift rises with everything standing on it, slowing at the top.
static void scMercuryLighthouseAerieRaiseLift(void)
{
    Sint32                 ids[5];
    MAP_GRID_LAYER_SCROLL *lift;
    Uint32                 i;
    Uint32                 j;
    Uint32                 n;
    FIXED                  speed;

    lift  = &((MAP_GRID_SCROLL_STATE *)MapGridState[0])->layers[2];
    speed = toFIXED(3) / 2;
    n     = 0;
    for (i = 0; i <= 4; i++)
        ids[i] = ACTOR_ID_COUNT;
    scMercuryLighthouseAerieFindActorsOnLift(ids, lift->offsetY);
    for (i = 0; i <= 4; i++)
    {
        if (ids[i] == ACTOR_ID_COUNT)
            break;
        fdActorGet(ids[i])->motionFlags = 0;
        n++;
    }
    sySoundPlay(SE_LIFT);
    for (i = 0; i < LIFT_UP_TICKS; i++)
    {
        lift->offsetY += speed;
        for (j = 0; j < n; j++)
        {
            fdActorGet(ids[j])->z.fixed -= speed;
            fdActorGet(ids[j])->targetZ  = fdActorGet(ids[j])->z.fixed;
        }
        if ((i & 3) == 3 && i >= LIFT_UP_TICKS - LIFT_SLOWING_TICKS)
            speed -= toFIXED(1) / 5;
        if (speed < toFIXED(1) / 20)
            speed = toFIXED(1) / 20;
        sySynch(1);
    }
    lift->offsetY = toFIXED(1024);
    fdMapGridCenterOnFollowedPoint();
    sySynch(2);
}

// Unused: Alex casts Ply on the lift's statue.
//
// Nothing calls it. In the game, Alex and Saturos vanish, appear on the lift and ride it down
// without it (scMercuryLighthouseAerieAlexAppears).
UNUSED static void scMercuryLighthouseAerieUnusedAlexCastsPly(void)
{
    // XXX (no evidence): what this was for; no script plays it.
    fdPsynergyLoadResources(ABILITY_PLY, 1);
    fdPsynergySetupActorPair(ACTOR_ALEX, ACTOR_LIFT_STATUE);
    fdPsynergyStartCast();
    fdPsynergyRunHandler(1);
    fdPsynergyStopCasterUpdate();
    fdPsynergyEnd();
}

// A beacon spark circles the beacon this many pixels out, and up to SPARK_ORBIT_BULGE more the
// nearer it rose from the middle of the beacon's height, so the sparks outline a ball.
#define SPARK_ORBIT_RADIUS    28
#define SPARK_ORBIT_BULGE     24

// The beacon is in sight, and drawn, while the view's top is at most this far down the map.
#define BEACON_SHOWN_VIEW_TOP 129

// A beacon spark's update, which swings it round the beacon it watches.
static void scMercuryLighthouseAerieUpdateBeaconSpark(ACTOR *spark)
{
    ACTOR *beacon;
    Uint16 angle;

    beacon          = (ACTOR *)spark->watchedActor;
    angle           = spark->timer;
    spark->x.fixed  = beacon->x.fixed + syCos(angle) * (spark->speedLimit + SPARK_ORBIT_RADIUS);
    spark->z.fixed  = (sySin(angle) << 4) + toFIXED(144);
    spark->targetX  = spark->x.fixed;
    spark->targetZ  = spark->z.fixed;
    spark->timer   -= 512;
}

// Flickers the beacon and sheds its sparks, every tick.
static void scMercuryLighthouseAerieFlickerBeacon(void)
{
    ACTOR                 *beacon;
    ACTOR                 *spark;
    SPR                   *sprite;
    MAP_GRID_SCROLL_STATE *grid;
    Sint32                 height;
    FIXED                  scale;
    Sint32                 cycleTick;
    Uint32                 angle;

    beacon = fdActorGet(ACTOR_BEACON);
    grid   = MapGridState[0];
    height = toFIXED(syRandomBelow(48));
    if ((Sint16)FIXEDtoINT(grid->view[1]) <= BEACON_SHOWN_VIEW_TOP)
    {
        if (TickCount & 1)
        {
            syImportActorSetPos(ACTOR_BEACON, toFIXED(304), toFIXED(144));
            scale                            = toFIXED(1);
            fdActorGet(ACTOR_BEACON)->scaleX = scale;
        }
        else
        {
            syImportActorSetPos(ACTOR_BEACON, toFIXED(304), toFIXED(151));
            scale                            = toFIXED(13) / 10;
            fdActorGet(ACTOR_BEACON)->scaleX = scale;
        }
        fdActorGet(ACTOR_BEACON)->scaleY = scale;
    }
    else
    {
        fdActorSetPos(ACTOR_BEACON, 8, 8);
    }
    if (beacon == NULL)
    {
        return;
    }
    cycleTick = TickCount & 15;
    if (cycleTick != 0)
    {
        return;
    }
    spark    = syFieldActorCreate(SPRITE_PARTICLE,
                               beacon->x.fixed + toFIXED(8),
                               beacon->y.fixed + height + toFIXED(8),
                               beacon->z.fixed);
    height   = height / toFIXED(6);
    height <<= 16;
    if (spark == NULL)
    {
        return;
    }
    sprite = spark->sprite;
    fdActorStartScript(spark, BeaconSparkScript);
    fdActorSetSpriteColor(spark, SPRITE_COLOR_BGR);
    spark->motionFlags     = cycleTick;
    angle                  = syRandomNext() & 0x0ffff000;
    spark->timer           = angle;
    spark->userValue       = cycleTick;
    spark->watchedActor    = (ACTOR_OR_EFFECT *)beacon;
    spark->update          = scMercuryLighthouseAerieUpdateBeaconSpark;
    spark->speedLimit      = FIXEDtoINT(sySin((height & 0xfffff) >> 4) * SPARK_ORBIT_BULGE);
    sprite->flags          = 0;
    sprite->image.priority = beacon->sprite->image.priority;
}

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

SCENE_ENTRANCE MercuryLighthouseAerieEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_BEACON_LIT,
     CONDITION_ALWAYS, 8,
     0, 8,
     FACING_SOUTH, 0,
     0, 0,
     608, 424,
     0},
    {MERCURY_LIGHTHOUSE_AERIE_AFTER_SATUROS_BATTLE,
     CONDITION_ALWAYS, 8,
     0, 8,
     FACING_SOUTH, 0,
     0, 0,
     608, 424,
     0},
    {MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR,
     CONDITION_ALWAYS, 504,
     0, 152,
     FACING_SOUTH, 0,
     0, 0,
     608, 424,
     0},
    {51, CONDITION_ALWAYS, 504, 0, 168, FACING_SOUTH, 0, 0, 0, 608, 424, 0},
    {SCENE_TABLE_END},
};

Uint32 MercuryLighthouseAerieExits[] = {
    SCENE_EXITS(MercuryLighthouseAerie),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_AERIE_TO_MERCURY_LIGHTHOUSE_EXTERIOR,
               MercuryLighthouseExterior,
               MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE MercuryLighthouseAerieActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        120, 0, 160, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        304, 40, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        440, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        472, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        504, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        104, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_KRADEN, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        104, 0, 152, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        88, 0, 160, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, FLAG_HOSTAGES_GONE, MOVEMENT_TYPE_STAND,
        104, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_EAST, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};
// clang-format on

CUE MercuryLighthouseAerieCues[] = {
    {CUE_STEP,
     METATILE_BEHAVIOR_BEACON_STAIRS, FLAG_MERCURY_LIGHTHOUSE_AERIE_ALEX_APPEARED,
     SCRIPT_REF(scMercuryLighthouseAerieConfrontSaturos)},
    {CUE_STEP,
     METATILE_BEHAVIOR_LIFT_LANDING, FLAG_LIFT_UP,
     SCRIPT_REF(scMercuryLighthouseAerieCallLift)},
    {STEP_CUE_WALKING,
     METATILE_BEHAVIOR_PLATFORM_EDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scMercuryLighthouseAerieJumpToNextPlatform)},
    {CUE_STEP,
     METATILE_BEHAVIOR_PLATFORM_EDGE, CONDITION_ALWAYS,
     SCRIPT_REF(scMercuryLighthouseAerieFloatPlatforms)},
    {STEP_CUE_WALKING,
     METATILE_BEHAVIOR_PLATFORM, CONDITION_ALWAYS,
     SCRIPT_REF(scMercuryLighthouseAerieJumpToNextPlatform)},
    {CUE_STEP,
     METATILE_BEHAVIOR_PLATFORM, FLAG_ON_PLATFORM,
     SCRIPT_REF(scMercuryLighthouseAerieLandOnPlatform)},
    {EXAMINE_CUE(EXAMINE_SILENT),
     METATILE_BEHAVIOR_PSYNERGY_STONE, FLAG_PSYNERGY_STONE,
     FIND_PSYNERGY_STONE},
    {CUE_STEP,
     METATILE_BEHAVIOR_LIFT, CONDITION_ALWAYS,
     SCRIPT_REF(scMercuryLighthouseAerieRideLiftDown)},
    {CUE_TABLE_END},
};

// The floating platform the leader has landed on, 0 when none.
Sint32 MercuryLighthouseAerieLandedPlatform = 0;

#include "shared/beacon_spark_script.c"

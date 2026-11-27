// The outside of Gaia Rock, the great rock near Izumo where the Serpent lives, in three areas of
// this overlay.
//
// The path from the world map runs north under stone arches to the cliffs, where a dark cave mouth
// leads into the rock (gaia_rock_inside.c); the way up is outside. On the first cliffs
// (SceneIdGaiaRockSusa) Felix pushes a stone pillar west along a ledge until it drops into a gap
// and bridges it (scGaiaRockFinishMovingPillar), climbs walls of handholds and rides slides, and
// sees a stone statue blow whirlwinds at a hanging vine to set it swinging. Whirlwind on another
// vine, over a gap, swings him across, and a second pillar Moved east off the ledge beyond drops
// into a notch below. On the next cliffs (SceneIdGaiaRockStatueSpots) more statues blow at vines,
// and on a great wall of handholds three stone faces lie in wait: when Felix climbs onto one it
// bursts out under him and knocks him off the holds (scGaiaRockClimbOntoLowestStoneFace). Beyond, a
// pillar slides in a slot, and on the mesa past it Reveal shows the Dancing Idol on an altar ringed
// by pillars (scGaiaRockRevealIdol), for Uzume in Izumo. Once the party has fought the Serpent
// inside, they come out to find Susa lying worn out by the cave mouth, and he asks whether they
// will tell Kushinada they slew it (scGaiaRockSusaAsksAboutTheSerpent).

#include "scene/script_layer.h"
#include "shared/gaia_rock.h"
#include "field/field_mode.h"
#include "game/game.h"
// Set once Susa has said he only needs rest ("I'm not dying... I'm just so tired...").
#define FLAG_SUSA_RESTED 0xa61
#include "edition.h"
#include "field/script_inline.h"
#include "field/call.h"
#include "field/field_mode_script.h"
#include "field/hanging_vine.h"
#include "field/map_grid.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

// The first cliffs, from the world map's path to the cave mouth (SceneIdGaiaRockSusa): the eight
// hanging vines, the two pillars Moved into the gaps, the statue that blows whirlwinds, a
// whirlwind's row parked off the map (the whirlwinds the statue blows are spawned), and Susa, who
// lies by the cave mouth after the Serpent ("Kushinada's safe... She's safe! Nothing else
// matters.").
enum
{
    ACTOR_FIRST_VINE   = ACTOR_FIRST_PLACED,
    ACTOR_FIRST_PILLAR = ACTOR_FIRST_VINE + 8,
    ACTOR_WIND_STATUE,
    ACTOR_WHIRLWIND,
    ACTOR_SECOND_PILLAR,
    ACTOR_SUSA
};

// The next cliffs, with the great wall of handholds (SceneIdGaiaRockStatueSpots): seven hanging
// vines, two statues that blow wind, and the three stone faces that burst out of the wall under
// Felix.
enum
{
    ACTOR_FIRST_RISING_STATUE = ACTOR_FIRST_VINE + 9,
    ACTOR_SECOND_RISING_STATUE,
    ACTOR_THIRD_RISING_STATUE
};

// The top, with the altar's mesa (SceneIdGaiaRockDancingIdol): the pillar that slides in its slot,
// the altar and the Dancing Idol on it.
enum
{
    ACTOR_PUSHED_PILLAR = ACTOR_FIRST_PLACED,
    ACTOR_ALTAR         = ACTOR_FIRST_PLACED + 7,
    ACTOR_DANCING_IDOL
};

// Set once each stone face has burst out of the great wall, so that it stays out until the party
// enters another area.
#define FLAG_FIRST_STATUE_RISEN    FLAG_AREA_TEMP_00
#define FLAG_SECOND_STATUE_RISEN   FLAG_AREA_TEMP_01
#define FLAG_THIRD_STATUE_RISEN    FLAG_AREA_TEMP_02

// The two pillars Moved into the gaps: each stands sunk into its gap, its top level with the ledge
// it was Moved along, for good.
#define FLAG_FIRST_PILLAR_DROPPED  0x920
#define FLAG_SECOND_PILLAR_DROPPED 0x921

// The metatile behaviors of the cue tables.
typedef enum GAIA_ROCK_METATILE_BEHAVIOR
{
    // The tufts of weeds below the cliff east of the cave mouth, which Cyclone blows away.
    METATILE_BEHAVIOR_CAVE_MOUTH_WEEDS  = 20,
    // The great wall's three trap holds, lowest first, in its overhead layer
    // (scGaiaRockCueWallMetatile reads them as Felix climbs).
    METATILE_BEHAVIOR_LOWEST_STONE_FACE = 20,
    METATILE_BEHAVIOR_MIDDLE_STONE_FACE,
    METATILE_BEHAVIOR_HIGHEST_STONE_FACE,
    // The path just south of where Susa lies by the cave mouth.
    METATILE_BEHAVIOR_PAST_SUSA = 42
} GAIA_ROCK_METATILE_BEHAVIOR;

// The passages within the first two areas. Each of their maps holds two parts, each with a camera
// of its own: on the first cliffs, the path to the cave mouth in the south and the cliffs above it
// in the north; on the next cliffs, the cliffs of vines in the west and the great wall under the
// two wind statues in the east. A passage's exit and the entrance it leads to.
typedef enum GAIA_ROCK_PASSAGE
{
    GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH = 2,
    GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS,
    GAIA_ROCK_SUSA_CLIFFS_FROM_CAVE_MOUTH = 2,
    GAIA_ROCK_SUSA_CAVE_MOUTH_FROM_CLIFFS,
    GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL = 2,
    GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES,
    GAIA_ROCK_STATUE_SPOTS_VINES_FROM_WALL = 2,
    GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES
} GAIA_ROCK_PASSAGE;

// Whether the party arrives on the next cliffs' great wall side: by its passage, or down from the
// mesa (the entrance after it).
#define ARRIVES_AT_GREAT_WALL()                                                                    \
    (MainRoutineStateWork.entrance >= GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES                       \
     && MainRoutineStateWork.entrance <= GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES + 1)

// The whirlwind's circling leaves.
enum
{
    WHIRLWIND_LEAVES = 16
};

struct ACTOR;

extern SCENE_REGION   GaiaRockSusaExitRegions[];
extern SCENE_REGION   GaiaRockStatueSpotsExitRegions[];

void                  scStoneHeadBurstOut(Sint32 id, Sint32 flag);

extern ACTOR_PLACE    GaiaRockPlacements[];
extern ACTOR_PLACE    GaiaRockSusaActorPlacements[];

extern ACTOR_PLACE    GaiaRockStatueSpotsActorPlacements[];
extern ACTOR_PLACE    GaiaRockDancingIdolActorPlacements[];

extern CUE            GaiaRockSusaCues[];
extern CUE            GaiaRockStatueSpotsCues[];
extern CUE            GaiaRockCues[];
extern Uint32         GaiaRockLeafScript[];

extern Sint32         GaiaRockWindScript[];

void                  scGaiaRockStartWallCues(void);
void                  scStoneHeadPlace(Sint32 id, Sint32 x, Sint32 y, Sint32 z);

extern Sint32         StoneHeadRiseScript[];

extern Uint32         DebrisScript[];
extern Uint32         GaiaRockPillarScript[];

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA)
        return GaiaRockSusaExitRegions;
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
        return GaiaRockStatueSpotsExitRegions;
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA)
        return GaiaRockSusaActorPlacements;
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
        return GaiaRockStatueSpotsActorPlacements;
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_DANCING_IDOL)
        return GaiaRockDancingIdolActorPlacements;
    return GaiaRockPlacements;
}

// Climbing the great wall of handholds, Felix reaches the lowest of its three trap holds, near the
// middle of the wall: a stone face bursts out of the rock under him in a shower of stones, and he
// flails and loses his hold on the wall: he is left standing on the face, and when the player
// climbs on he drops off it, facing out, down the wall to its foot. The hold is metatile (111, 50)
// of the wall's overhead layer, drawn at (47, 50); the face stays out, and the hold is solid rock,
// until the party enters another area.
void scGaiaRockClimbOntoLowestStoneFace(void)
{
    scStoneHeadBurstOut(ACTOR_FIRST_RISING_STATUE, FLAG_FIRST_STATUE_RISEN);
}

// As scGaiaRockClimbOntoLowestStoneFace, for the middle trap hold, a little higher and to the east:
// metatile (113, 42) of the overhead layer, drawn at (49, 42).
void scGaiaRockClimbOntoMiddleStoneFace(void)
{
    scStoneHeadBurstOut(ACTOR_SECOND_RISING_STATUE, FLAG_SECOND_STATUE_RISEN);
}

// As scGaiaRockClimbOntoLowestStoneFace, for the highest trap hold, a little west of the middle
// one: metatile (112, 33) of the overhead layer, drawn at (48, 33).
void scGaiaRockClimbOntoHighestStoneFace(void)
{
    scStoneHeadBurstOut(ACTOR_THIRD_RISING_STATUE, FLAG_THIRD_STATUE_RISEN);
}

// The Japanese edition lets a pillar land with no sound, through the function of Taopo Swamp and
// the Kibombo Mountains, which tests for landing before it counts the tick down; the later editions
// take the Ankohl Ruins' function and play SE_BOOM as it lands.
#if !GAIA_ROCK_PILLAR_LANDS_WITH_BOOM
#    include "shared/actor_wait_to_land.h"
#else
#    include "shared/actor_wait_to_land_with_snd.h"
#endif

// Nothing visible happens as Felix's Move takes hold of the stone pillar on the west ledge of the
// first cliffs, standing at metatile (7, 27); the game changes one metatile of the map's lower half
// beside it so that it can slide. scGaiaRockFinishMovingPillar changes it back.
void scGaiaRockStartMovingPillar(void)
{
    fdMapGridCopyMetatileAttributes(7, 90, 1, 1, 8, 91);
}

#if !GAIA_ROCK_PILLAR_LANDS_WITH_BOOM
#    define DROP_PILLAR(id) scActorWaitToLand(id)
#else
#    define DROP_PILLAR(id) scActorWaitToLandWithSound(id, SE_BOOM)
#endif

// A pillar of the first cliffs stops after a push or a Move. The pillar on the west ledge, once
// pushed or Moved west over the gap at metatile (5, 27), drops in, its top level with the ledge, so
// that Felix can walk across. The pillar on the ledge past the swinging vine, at metatile (37, 46),
// once Moved east off the ledge's end drops onto the ledge below, and one Move more sets it for
// good in the notch where a grey block shows, metatile (40, 46), its top a square Felix hops
// across. (The walkthrough says to Move it to the left; the game takes it east.)
void scGaiaRockFinishMovingPillar(Sint32 unused, Sint32 id)
{
    ACTOR *actor = fdActorGet(id);

    fdMapGridCopyMetatileAttributes(8, 90, 1, 1, 8, 91);
    if (id == ACTOR_FIRST_PILLAR && fdScriptGetFlag(FLAG_FIRST_PILLAR_DROPPED) == 0
        && FIELD_HALF_METATILE(actor->x.fixed) <= 11)
    {
        DROP_PILLAR(ACTOR_FIRST_PILLAR);
        fdScriptSetFlag(FLAG_FIRST_PILLAR_DROPPED);
        fdMapGridCopyMetatileAttributes(6, 27, 1, 1, 5, 27);
    }
    if (id == ACTOR_SECOND_PILLAR && fdScriptGetFlag(FLAG_SECOND_PILLAR_DROPPED) == 0)
    {
        if (FIELD_HALF_METATILE(actor->x.fixed) == 77)
            fdMapGridCopyMetatileAttributes(33, 102, 1, 2, 37, 102);
        if (FIELD_HALF_METATILE(actor->x.fixed) == 79 && FIELD_HALF_METATILE(actor->y.fixed) == 12)
        {
            DROP_PILLAR(ACTOR_SECOND_PILLAR);
            fdMapGridCopyMetatileAttributes(38, 46, 1, 1, 39, 46);
        }
        if (FIELD_HALF_METATILE(actor->x.fixed) == 81)
        {
            fdScriptSetFlag(FLAG_SECOND_PILLAR_DROPPED);
            fdMapGridCopyMetatileAttributes(41, 46, 1, 1, 39, 46);
            fdMapGridCopyMetatileAttributes(27, 44, 1, 1, 40, 46);
        }
    }
}

// Nothing visible happens as Felix starts to push or Move the stone pillar at the east edge of the
// summit where he arrives, which slides north and south in its slot (metatiles (13, 14) to (13,
// 18)); the game frees the slot's metatile under it so that it can slide.
void scGaiaRockStartPushingPillar(void)
{
    fdMapGridCopyMetatileAttributes(
        13, 18, 1, 1, 13, FIELD_METATILE(fdActorGet(ACTOR_PUSHED_PILLAR)->z.fixed));
}

// Nothing visible happens when the pillar in its slot at the east edge of the first summit stops;
// the game makes its metatile a square Felix hops across, so that he can jump over the pillar's
// top. Moved to the slot's south end, metatile (13, 18), it opens the way on to the altar.
void scGaiaRockFinishPushingPillar(void)
{
    fdMapGridCopyMetatileAttributes(
        11, 14, 1, 1, 13, FIELD_METATILE(fdActorGet(ACTOR_PUSHED_PILLAR)->z.fixed));
}

// Felix casts Cyclone among the tufts of weeds below the cliff east of the cave mouth (metatiles
// (49, 48), (51, 48), (50, 49), (58, 48), (59, 47) and a few more), and its whirl of leaves blows
// away the tufts within its reach: cast from metatile (50, 50), the two either side of the sapling
// below the cave mouth; from metatile (57, 49), the one just east of Felix. The thick bushes south
// of them stay as they were. The game takes each tuft's picture from the overhead layer's block at
// metatiles (110, 64) to (123, 75) of map.txt into its place, in the block from (110, 47) that the
// overhead layer shows here.
void scGaiaRockCycloneBlowsAwayWeeds(void)
{
    syFieldTileCopiesCopyTouchedMetatiles(110, 64, 14, 12, 110, 47, 1);
}

// Casting Reveal at the top of the cliffs shows the Dancing Idol standing on the altar, on the mesa
// ringed by six pillars, until the party has taken it.
void scGaiaRockRevealIdol(void)
{
    ACTOR *actor = fdActorGet(FIELD_TREASURE_ACTOR);
    Sint32 done;

    if (actor == NULL)
        return;
    done = fdScriptGetFlag(FLAG_DANCING_IDOL_TAKEN);
    if (done == 0)
    {
        fdActorSetPos(FIELD_TREASURE_ACTOR, 657, 246);
        actor->motionFlags = done;
        actor->y.fixed     = toFIXED(80);
    }
}

// As Reveal wears off, the Dancing Idol vanishes from the altar again.
void scGaiaRockHideIdol(void)
{
    if (fdActorGet(FIELD_TREASURE_ACTOR) != NULL)
        fdActorSetPos(FIELD_TREASURE_ACTOR, 0, 0);
}

// Felix examines the altar on the mesa ringed by six pillars: "Felix checked the altar." With the
// Dancing Idol revealed on it, he takes it. Before that he says "I'm...sensing something..." and
// the field menu opens, a hint to cast Reveal; once it is taken, "I can't feel anything...".
void scGaiaRockCheckAltar(void)
{
    ACTOR *actor = fdActorGet(FIELD_TREASURE_ACTOR);

    mnMessageShowCentered(MsgCheckedTheAltar, 1);
    if (actor != NULL && actor->x.fixed != 0)
    {
        fdTalkToActor(FIELD_TALK_SILENT | FIELD_TREASURE_ACTOR);
    }
    else if (fdScriptGetFlag(FLAG_DANCING_IDOL_TAKEN) == 0)
    {
        FIELD_STATE *state = RamHeapSlots->fieldState;

        mnMessageShowCentered(MsgIMSensing, 1);
        state->fieldMenu = 1;
    }
    else
    {
        mnMessageShowCentered(MsgICanT, 1);
    }
}

// Once the Serpent is slain, Felix walks past Susa, who lies by the cave mouth (on the metatiles
// just south of him, (45, 52) to (50, 52)), and Susa asks: "You're going to tell Kushinada that you
// defeated the serpent, aren't you?" Yes: "Ha ha ha! Of course!" No: he says they should accept the
// credit. It plays once.
void scGaiaRockSusaAsksAboutTheSerpent(void)
{
    if (fdScriptGetFlag(FLAG_SERPENT_SLAIN))
    {
        fdScriptStart();
        syFieldScriptEndLastingPsynergy(0);
        syFieldScriptSetMessageId(MsgYouReGoing);
        scFieldActorStartRepeatedMotion(ACTOR_SUSA, 1);
        syFieldScriptWait(15);
        fdScriptAskYesNo(ACTOR_SUSA, 0);
        fdScriptSetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE);
        fdScriptEnd();
    }
}

// Talking to Susa where he lies by the cave mouth: before he has asked about the serpent, "If
// you're going to Izumo, tell my sister that I'm still alive."; after, he asks again.
void scGaiaRockTalkToSusa(void)
{
    if (fdScriptGetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE))
    {
        syFieldScriptSetMessageId(MsgYouReGoing);
        fdScriptAskYesNo(ACTOR_SUSA, 0);
    }
    else
    {
        syFieldScriptSetMessageId(MsgIfYouRe);
        // "If you're going to Izumo, tell my sister that I'm still alive."
        fdScriptShowMessage(ACTOR_SUSA, 0);
    }
}

// Mind Read on Susa where he lies by the cave mouth: "I really got worked over, but as long as
// Kushinada's safe, it was worth it." before he has asked about the serpent, "Kushinada's safe...
// She's safe!" after.
void scGaiaRockMindReadSusa(void)
{
    if (fdScriptGetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE))
    {
        syFieldScriptSetMessageId(MsgKushinadaSSafe);
        // "Kushinada's safe... She's safe! Nothing else matters."
        fdScriptShowMessage(ACTOR_SUSA, 0);
    }
    else
    {
        syFieldScriptSetMessageId(MsgIReallyGot);
        // "I really got worked over, but as long as Kushinada's safe, it was..."
        fdScriptShowMessage(ACTOR_SUSA, 0);
    }
}

const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA)
        return GaiaRockSusaCues;
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
        return GaiaRockStatueSpotsCues;
    return GaiaRockCues;
}

// A circling leaf as its update reads it: ACTOR's fields under a type of its own, the timer
// unsigned and the update taking the leaf.
typedef struct ORBIT_EFFECT
{
    Uint8                unused[6]; // the script and its cursor, not read here
    Uint16               facing;
    Sint32               x;
    Sint32               y;
    Sint32               z;
    Uint8                unusedAfterZ[15];
    Uint8                priorityFlags;
    Uint8                unusedAfterPriority[4];
    Sint32               velocityY;
    Uint8                unusedAfterVelocityY[28];
    Sint32               gravity;
    Uint8                unusedAfterGravity[24];
    Uint16               timer;
    Sint16               pause; // ACTOR.userValue
    struct ORBIT_EFFECT *watchedActor;
    void                 (*update)(struct ORBIT_EFFECT *);
} ORBIT_EFFECT;

// Each tick, carries a leaf round a wobbling circle, now and then turning faster for a moment, and
// releases it at its 71st tick.
void scGaiaRockCircleLeaf(ORBIT_EFFECT *effect)
{
    Sint32 radius;
    Sint32 angle;
    FIXED  dx;
    FIXED  dz;

    radius          = syRandom() + toFIXED(2);
    angle           = effect->facing;
    dx              = syMulFX(radius, syCos(angle));
    dz              = syMulFX(radius, syMathSin(angle));
    effect->x      += dx;
    effect->z      += dz;
    effect->facing -= TRIG_TURN / 4096;
    if (effect->pause != 0)
    {
        effect->pause--;
        effect->facing += TRIG_TURN / 32;
    }
    else if (syRandomBelow(32) == 0)
    {
        effect->pause = syRandomBelow(16) + 8;
    }
    if (++effect->timer == 71)
        fdActorStartScript((ACTOR *)effect, GaiaRockLeafScript);
}

// The whirlwind's 16 leaves, spawned in a column around it, each circling from its own start.
Sint32 scGaiaRockSpawnCirclingLeaves(ORBIT_EFFECT *src)
{
    Sint32 i;
    Sint32 timer;

    i     = 0;
    timer = 20;
    for (; i < WHIRLWIND_LEAVES; i++)
    {
        {
            ORBIT_EFFECT *effect;
            Sint32        radius;
            FIXED         pos[3];

            pos[0] = src->x;
            pos[1] = src->y + i * (toFIXED(4) / 5) + toFIXED(4);
            pos[2] = src->z;
            radius = syRandom();
            syFieldVectorAddPolarOffsetFX(radius * 5 + toFIXED(3), syRandom(), pos);
            effect = (ORBIT_EFFECT *)Value4(
                (Sint32 (*)())fdActorSpawn, SPRITE_LEAF, pos[0], pos[1], pos[2]);
            if (effect != NULL)
            {
                effect->update        = scGaiaRockCircleLeaf;
                effect->timer         = timer;
                effect->pause         = 0;
                effect->gravity       = toFIXED(1) / 10;
                effect->velocityY     = toFIXED(3);
                effect->facing        = syRandom();
                effect->priorityFlags = 0;
                fdActorSetSpritePriorityDirect((ACTOR *)effect, 2);
            }
        }
        timer -= 2;
    }
    return 0;
}

// The hanging vine of actor id, or NULL: this overlay's copy of fdHangingVineFindSlot.
static inline HANGING_VINE_SLOT *FindSlot(Sint32 id)
{
    HANGING_VINE_WORK *state = HANGING_VINE_STATE;
    HANGING_VINE_SLOT *found = NULL;
    Sint32             i     = 0;

    for (i = 0; i < ROPE_SLOT_COUNT; i++)
    {
        if (state->slots[i].id == id)
        {
            found = &state->slots[i];
            break;
        }
    }
    return found;
}

// The statue's whirlwind reaches the vine beside it and starts it swinging.
void scGaiaRockWhirlwindBlowsVine(void)
{
    HANGING_VINE_SLOT *slot = FindSlot(ACTOR_FIRST_VINE);

    slot->phase             = DEGtoANG(90);
    slot->swing             = TRIG_TURN / 1024;
}

// The statue's whirlwind swings the vine beside it wider.
void scGaiaRockWhirlwindBlowsVineHarder(void)
{
    HANGING_VINE_SLOT *slot  = FindSlot(ACTOR_FIRST_VINE);

    slot->phase              = DEGtoANG(90);
    slot->swing             += TRIG_TURN / 512;
}

void fdActorPlaySoundInView(ACTOR *actor, Sint32 sound);

// Every 256 ticks the stone statue on the upper ledge of the first cliffs blows a whirlwind west at
// the hanging vine beside it, setting it swinging.
void scGaiaRockStatueBlowsWind(void)
{
    ACTOR *actor = fdActorGet(ACTOR_WIND_STATUE);
    Uint8  phase = TickCount;

    if (phase == 0)
    {
        fdActorPlaySoundInView(actor, SE_FLAME_FLARE);
        actor = fdActorSpawn(SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed, actor->z.fixed);
        if (actor != NULL)
        {
            actor->acceleration  = toFIXED(2);
            actor->speedLimit    = toFIXED(2);
            actor->scaleX        = toFIXED(1) / 2;
            actor->scaleY        = toFIXED(1) / 10;
            actor->priorityFlags = phase;
            fdActorSelectAnimation(actor, 2);
            scFieldActorSetMoveTargetFX(
                actor, actor->x.fixed - toFIXED(32), actor->y.fixed, actor->z.fixed);
            fdActorStartScript(actor, GaiaRockWindScript);
            fdActorSetSpritePriorityDirect(actor, 2);
        }
    }
}

// The transition Gaia Rock opens with: the window's shape in the Japanese edition, an interlaced
// wipe from it.
#if !REVISED_ENTRANCE_TRANSITIONS
#    define SCREEN_EFFECT SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#    define SCREEN_EFFECT SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

Sint32 scInit(void)
{
    ACTOR       *actor;
    FIELD_STATE *state;

    FieldStateWork->transition                              = SCREEN_EFFECT;
    MainRoutineStateWork.retreatScene                       = SCENE_GAIA_ROCK_SUSA;
    MainRoutineStateWork.retreatEntrance                    = GAIA_ROCK_SUSA_FROM_WORLD_MAP;
    fdActorGetById(fdPartyGetLeaderActor())->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA)
    {
        sySetEvent((Sint32)scGaiaRockStatueBlowsWind, EVENT_PASS_TICK);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 1, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 2, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 3, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 4, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 5, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 6, 1);
        fdHangingVinePlace(ACTOR_FIRST_VINE + 7, 1);
        if (syGameFlagGet(FLAG_FIRST_PILLAR_DROPPED))
        {
            actor              = fdActorGetById(ACTOR_FIRST_PILLAR);
            actor->motionFlags = 0;
            actor->x.fixed     = toFIXED(88);
            actor->y.fixed     = toFIXED(128);
            fdActorSetSpriteFlags(actor, 0);
            fdMapGridCopyMetatileAttributes(6, 27, 1, 1, 5, 27);
        }
        actor                = fdActorGetById(ACTOR_SECOND_PILLAR);
        actor->priorityFlags = 0;
        if (syGameFlagGet(FLAG_SECOND_PILLAR_DROPPED))
        {
            actor->motionFlags = 0;
            actor->x.fixed     = toFIXED(648);
            actor->y.fixed     = toFIXED(64);
            fdActorSetSpriteFlags(actor, 0);
            fdMapGridCopyMetatileAttributes(33, 102, 1, 2, 37, 102);
            fdMapGridCopyMetatileAttributes(27, 44, 1, 1, 40, 46);
        }
        else if (syGameFlagGet(FLAG_KEEP_PARTY_POSITION))
        {
            if (FIELD_HALF_METATILE(actor->x.fixed) != 75)
            {
                fdMapGridCopyMetatileAttributes(33, 102, 1, 2, 37, 102);
            }
            if (FIELD_HALF_METATILE(actor->x.fixed) == 79)
            {
                actor->motionFlags = 0;
                fdMapGridCopyMetatileAttributes(38, 46, 1, 1, 39, 46);
            }
        }
        if (syGameFlagGet(FLAG_SERPENT_SLAIN) && !syGameFlagGet(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE))
        {
            fdActorSetPos(ACTOR_SUSA, 798, 800);
            // He lies across metatiles (49, 49) and (50, 49), in front of the sapling by the cave
            // mouth; both take the wall of metatile (53, 52), so that no one walks over him.
            fdMapCopyMetatileAttributeRect(53, 52, 1, 1, 49, 49);
            fdMapCopyMetatileAttributeRect(53, 52, 1, 1, 50, 49);
            fdActorSetAnimationById(ACTOR_SUSA, 5);
            if (!fdScriptGetFlag(FLAG_SUSA_RESTED))
            {
                fdScriptStartScreenTransition();
                fdScriptWaitTransitionTicks();
                scFieldActorStartRepeatedMotion(ACTOR_SUSA, 3);
                fdCameraMoveToActorById(ACTOR_SUSA, 1);
                syFieldScriptWait(30);
                syFieldScriptSetMessageId((Sint32)MsgIMNot);
                // "I'm not dying... I'm just so tired... I'm going to lie down for a..."
                fdScriptShowMessage(ACTOR_SUSA, 0);
                scFieldActorStartRepeatedMotion(ACTOR_SUSA, 2);
                syFieldScriptWait(30);
                fdScriptSetFlag(FLAG_SUSA_RESTED);
                fdScriptEnd();
            }
        }
    }
    else if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
    {
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 1, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 2, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 3, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 4, 1);
        Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 5, 1);
        fdHangingVinePlace(ACTOR_FIRST_VINE + 6, 1);
        if (ARRIVES_AT_GREAT_WALL())
        {
            scGaiaRockStartWallCues();
        }
        if (syGameFlagGet(FLAG_FIRST_STATUE_RISEN))
        {
            scStoneHeadPlace(ACTOR_FIRST_RISING_STATUE, toFIXED(764), -toFIXED(391), toFIXED(424));
        }
        if (syGameFlagGet(FLAG_SECOND_STATUE_RISEN))
        {
            scStoneHeadPlace(ACTOR_SECOND_RISING_STATUE, toFIXED(789), -toFIXED(248), toFIXED(424));
        }
        if (syGameFlagGet(FLAG_THIRD_STATUE_RISEN))
        {
            scStoneHeadPlace(ACTOR_THIRD_RISING_STATUE, toFIXED(778), -toFIXED(119), toFIXED(424));
        }
    }
    else if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_DANCING_IDOL)
    {
        actor = fdActorGetById(FIELD_TREASURE_ACTOR);
        if (actor != 0)
        {
            fdActorSetPos(FIELD_TREASURE_ACTOR, 0, 0);
        }
        state                               = RamHeapSlots->fieldState;
        actor                               = state->actors[ACTOR_DANCING_IDOL];
        state->actors[FIELD_TREASURE_ACTOR] = actor;
        actor                               = fdActorGetById(ACTOR_PUSHED_PILLAR);
        actor->motionFlags                  = 0;
        fdMapGridCopyMetatileAttributes(11, 14, 1, 1, 13, FIELD_METATILE(actor->z.fixed));
    }
    return 0;
}

Sint32 scPrepareMap(void)
{
    if (MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS && ARRIVES_AT_GREAT_WALL())
    {
        MAP_LAYER *layer = fdMapGridGetLayer(1);

        layer->offsetX   = -toFIXED(128);
        layer->offsetY   = toFIXED(1096);
    }
    return 0;
}

#include "shared/gaia_rock_cue_wall_metatile.c"

#include "shared/stone_head_place.h"

#include "shared/stone_head_burst_out.h"

// Starts scGaiaRockCueWallMetatile each tick.
void scGaiaRockStartWallCues(void)
{
    sySetTickEvent(scGaiaRockCueWallMetatile, EVENT_PASS_TICK);
}

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
SCENE_REGION GaiaRockSusaExitRegions[] = {
    {771, -28, 484, 779, -20, 492, CONDITION_ALWAYS, GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH},
    {771, 180, 707, 779, 188, 715, CONDITION_ALWAYS, GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS},
    {SCENE_TABLE_END},
};

SCENE_REGION GaiaRockStatueSpotsExitRegions[] = {
    {195, 296, 323, 203, 304, 331, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL},
    {707, -554, 452, 715, -546, 460, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES},
    {SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
    SCENE_EXITS(GaiaRockSusa),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH, GaiaRockSusa, GAIA_ROCK_SUSA_CAVE_MOUTH_FROM_CLIFFS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS, GaiaRockSusa, GAIA_ROCK_SUSA_CLIFFS_FROM_CAVE_MOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockStatueSpots),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA, GaiaRockSusa, GAIA_ROCK_SUSA_FROM_GAIA_ROCK_STATUE_SPOTS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_VINES_FROM_WALL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL, GaiaRockDancingIdol, GAIA_ROCK_DANCING_IDOL_FROM_GAIA_ROCK_STATUE_SPOTS, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockDancingIdol),
    SCENE_EXIT_UNLESS(GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_FROM_GAIA_ROCK_DANCING_IDOL, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

Uint32 GaiaRockPillarScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

ACTOR_PLACE GaiaRockPlacements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRockSusaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1000, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 936, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (Sint32)GaiaRockPillarScript, 120, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 295, 0, 442, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (Sint32)GaiaRockPillarScript, 600, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRockStatueSpotsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRockDancingIdolActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 656, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// Unused: an empty cue table nothing returns.
CUE GaiaRockUnusedCues[] = {
    {CUE_TABLE_END},
};

CUE GaiaRockSusaCues[] = {
    {CUE_EXIT, GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS},
    {CUE_EXIT, GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR},
    {CUE_EXIT, GAIA_ROCK_SUSA_TO_WORLD_MAP, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_WORLD_MAP},
    {CUE_STEP, METATILE_BEHAVIOR_PAST_SUSA, FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE, SCRIPT_REF(scGaiaRockSusaAsksAboutTheSerpent)},
    {CUE_TALK, ACTOR_SUSA, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockTalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockMindReadSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 5, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 6, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 7, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_CAVE_MOUTH_WEEDS, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockCycloneBlowsAwayWeeds)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_DROPPED, SCRIPT_REF(scGaiaRockStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_DROPPED, SCRIPT_REF(scGaiaRockFinishMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_DROPPED, SCRIPT_REF(scGaiaRockFinishMovingPillar)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockFinishMovingPillar)},
    {CUE_TABLE_END},
};

CUE GaiaRockStatueSpotsCues[] = {
    {CUE_EXIT, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA},
    {CUE_EXIT, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL},
    {CUE_STEP, METATILE_BEHAVIOR_LOWEST_STONE_FACE, FLAG_FIRST_STATUE_RISEN, SCRIPT_REF(scGaiaRockClimbOntoLowestStoneFace)},
    {CUE_STEP, METATILE_BEHAVIOR_MIDDLE_STONE_FACE, FLAG_SECOND_STATUE_RISEN, SCRIPT_REF(scGaiaRockClimbOntoMiddleStoneFace)},
    {CUE_STEP, METATILE_BEHAVIOR_HIGHEST_STONE_FACE, FLAG_THIRD_STATUE_RISEN, SCRIPT_REF(scGaiaRockClimbOntoHighestStoneFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 5, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 6, CONDITION_ALWAYS, 0},
    {CUE_TABLE_END},
};

CUE GaiaRockCues[] = {
    {CUE_EXIT, GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS, CONDITION_ALWAYS, GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS},
    {CUE_TALK, ACTOR_ALTAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockCheckAltar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockFinishPushingPillar)},
    {CUE_PUSH, 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockStartPushingPillar)},
    {CUE_PUSHED, 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockFinishPushingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockRevealIdol)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scGaiaRockHideIdol)},
    {CUE_TABLE_END},
};

Uint32 GaiaRockLeafScript[] = {
    SC_RELEASE,
};

Sint32 GaiaRockWindScript[] = {
    SC_ADD(SW_SCALE_X, toFIXED(1) / 10),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 10),
    SC_WAIT(1),
    SC_LOOP(12, SC_TOP),
    SC_SET(SW_SCALE_X, toFIXED(17) / 10),
    SC_SET(SW_SCALE_Y, toFIXED(13) / 10),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(scGaiaRockWhirlwindBlowsVine),
    SC_WAIT(2),
    SC_LABEL(1),
    SC_CALL(scGaiaRockWhirlwindBlowsVineHarder),
    SC_WAIT(2),
    SC_LOOP(3, SC_TO(1)),
    SC_CALL(scGaiaRockSpawnCirclingLeaves),
    SC_LABEL(2),
    SC_CALL(scGaiaRockWhirlwindBlowsVineHarder),
    SC_WAIT(2),
    SC_LOOP(14, SC_TO(2)),
    SC_CALL(scGaiaRockWhirlwindBlowsVineHarder),
    SC_WAIT(10),
    SC_RELEASE,
};

#include "shared/debris_script.c"

#include "shared/stone_head_rise_script.c"
// clang-format on

// The Osenia Cliffs, across the Madra Drawbridge, where the wreck of a Champa ship lies on the
// shore and three people from Madra look it over until Piers has carried Gabomba's message.
//
// The party hops across the wreckage to the exit on the east side. A rope already stretches between
// two posts near the villagers; in the northeast, two more posts stand bare until Felix ties a rope
// between them with Lash, on the way to the Pirate's Sword.

// This unit declares its own form of fdRopeLash, below.
#define OWN_fdRopeLash
// This unit declares its own form of fdRopeTie, below.
#define OWN_fdRopeTie

#include "field/field_state.h"
#include "field/field.h"
#include "game/game.h"
#include "field/script_inline.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

// The ROM's callers here are compiled with no declaration in scope, as against this form: no
// parameter list and an Sint32 result, where the definition takes the rope and two posts and
// returns nothing.
Sint32        fdRopeTie();

// The ROM's callers here are compiled with no declaration in scope, as against this form: no
// parameter list and an Sint32 result, where the definition takes the rope and two posts and
// returns nothing.
Sint32        fdRopeLash();

extern Uint32 OseniaCliffsPaletteCycleStep[];

#include "shared/field_rope_place.c"

void scOseniaCliffsCyclePalette(void);

// Set once the party has lashed the second rope, which stays tied for the visit.
#define FLAG_OSENIA_CLIFFS_ROPE_LASHED FLAG_SCENE_TEMP_03

// The actors, numbering the rows of `ActorPlacements`.
typedef enum OSENIA_CLIFFS_ACTOR_ID
{
    ACTOR_VILLAGER_CHAMPA_BOAT = ACTOR_FIRST_PLACED,
    ACTOR_VILLAGER_NOT_CHAMPA,
    ACTOR_VILLAGER_WAVE_DAMAGE,
    // The posts of the rope near the villagers, tied from the start.
    ACTOR_FIRST_ROPE_POST,
    ACTOR_SECOND_ROPE_POST,
    // The posts in the northeast, low and high, that Lash ties a rope between.
    ACTOR_THIRD_ROPE_POST,
    ACTOR_FOURTH_ROPE_POST
} OSENIA_CLIFFS_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
    return 0;
}

const Uint32 *scGetExits(void)
{
    return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    return ActorPlacements;
}

// Felix casts Lash at the lower of the two bare rope posts in the northeast of the Osenia Cliffs,
// the one on the wooden landing in the water, and a rope flies from it up to the post on the
// clifftop to the north-east and ties itself taut between them. Felix walks up it as a tightrope
// onto the clifftop, on the way to the Pirate's Sword. It stays tied until the party leaves.
void scOseniaCliffsLashRope(void)
{
    fdRopeLash(1, ACTOR_THIRD_ROPE_POST, ACTOR_FOURTH_ROPE_POST);
    fdScriptSetFlag(FLAG_OSENIA_CLIFFS_ROPE_LASHED);
}

const CUE *scGetCues(void)
{
    return SceneCues;
}

Sint32 scInit(void)
{
    MAP_BITS *map                   = (MAP_BITS *)RamHeapSlots->mapGrid;

    map->wading                     = 1;
    OseniaCliffsPaletteCycleStep[0] = 0;
    FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    sySetEvent(scOseniaCliffsCyclePalette, EVENT_PASS_TICK);
    {
        ACTOR *leader         = fdActorGetById(MainRoutineStateWork.leaderActorId);

        leader->priorityFlags = ACTOR_PRIORITY_IGNORES_RISES | leader->priorityFlags;
    }
    fdRopeInit();
    fdRopeTie(0, ACTOR_SECOND_ROPE_POST, ACTOR_FIRST_ROPE_POST);
    fdActorSetAnimation(ACTOR_SECOND_ROPE_POST, 4);
    fdActorSetAnimation(ACTOR_FIRST_ROPE_POST, 4);
    scFieldRopePlace(
        1, FLAG_OSENIA_CLIFFS_ROPE_LASHED, ACTOR_THIRD_ROPE_POST, ACTOR_FOURTH_ROPE_POST);
    return 0;
}

// The palette cycle: the eight colours of one background palette, and the four of another that show
// four of them in turn: the blues of the line where the sea meets the foot of the cliffs, and of
// glints on the sea.
#define CYCLE_SOURCE_PALETTE 12
#define CYCLE_SOURCE_COLORS  8
#define SHORELINE_PALETTE    6

// The sea shimmers where it meets the foot of the cliffs: every 16 ticks, four of
// `CYCLE_SOURCE_PALETTE`'s eight colours, starting one further each time, are copied to colours 1
// to 4 of `SHORELINE_PALETTE`.
void scOseniaCliffsCyclePalette(void)
{
    if ((TickCount & 15) == 0)
    {
        volatile Uint16 *colors;
        Sint32           i;

        if (OseniaCliffsPaletteCycleStep[0] == CYCLE_SOURCE_COLORS)
            OseniaCliffsPaletteCycleStep[0] = 0;
        i                                   = OseniaCliffsPaletteCycleStep[0]++;
        colors                              = (Uint16 *)&BG_PLTT_COLOR(CYCLE_SOURCE_PALETTE, 0);
        BG_PLTT_COLOR(SHORELINE_PALETTE, 1) = colors[i];
        BG_PLTT_COLOR(SHORELINE_PALETTE, 2) = colors[(i + 1) & (CYCLE_SOURCE_COLORS - 1)];
        BG_PLTT_COLOR(SHORELINE_PALETTE, 3) = colors[(i + 2) & (CYCLE_SOURCE_COLORS - 1)];
        BG_PLTT_COLOR(SHORELINE_PALETTE, 4) = colors[(i + 3) & (CYCLE_SOURCE_COLORS - 1)];
    }
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// The sea's shimmer, its step through the source colours.
Uint32 OseniaCliffsPaletteCycleStep[] = {
    0,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
    SCENE_EXITS(OseniaCliffs),
    SCENE_EXIT_UNLESS(1, WorldMap, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, WorldMap, 13, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR,
                    FLAG_GABOMBA_MESSAGE_CARRIED,
                    MOVEMENT_TYPE_STAND,
                    200,
                    0,
                    226,
                    FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK,
                    0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN,
                    FLAG_GABOMBA_MESSAGE_CARRIED,
                    MOVEMENT_TYPE_STAND,
                    259,
                    0,
                    312,
                    FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK,
                    0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR,
                    FLAG_GABOMBA_MESSAGE_CARRIED,
                    MOVEMENT_TYPE_STAND,
                    242,
                    0,
                    313,
                    FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK,
                    0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    240,
                    0,
                    184,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    200,
                    0,
                    184,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    424,
                    0,
                    104,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    472,
                    0,
                    104,
                    FACING_NORTH,
                    TALK_FACING_KEEP,
                    0),
    {SCENE_TABLE_END},
};

CUE SceneCues[] = {
    {CUE_EXIT, 1, CONDITION_ALWAYS, 1},
    {CUE_EXIT, 2, CONDITION_ALWAYS, 2},
    {CUE_TALK, ACTOR_VILLAGER_CHAMPA_BOAT, CONDITION_ALWAYS, MSGNO(MsgMadraChampaBoat)},
    {CUE_TALK, ACTOR_VILLAGER_NOT_CHAMPA, CONDITION_ALWAYS, MSGNO(MsgMadraSailorNotChampa)},
    {CUE_TALK, ACTOR_VILLAGER_WAVE_DAMAGE, CONDITION_ALWAYS, MSGNO(MsgMadraWaveDamage)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_CHAMPA_BOAT, CONDITION_ALWAYS,
     MSGNO(MsgMadraCannotEscapeBySea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_NOT_CHAMPA, CONDITION_ALWAYS,
     MSGNO(MsgMadraBriggsNewShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_VILLAGER_WAVE_DAMAGE, CONDITION_ALWAYS,
     MSGNO(MsgMadraPirateAttackParties)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH),
     ACTOR_THIRD_ROPE_POST, FLAG_OSENIA_CLIFFS_ROPE_LASHED,
     SCRIPT_REF(scOseniaCliffsLashRope)},
    {CUE_TABLE_END},
};

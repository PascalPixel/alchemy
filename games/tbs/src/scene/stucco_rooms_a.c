// Inside Kalay are the houses and the inn, and the Kalay Sanctum.
//
// - Stucco rooms A (SceneIdStuccoRoomsA), a rooms map drawn in stucco,
//       all of them Kalay's, whose area name is Kalay. It holds every house and
//       the inn, each entered from its own door in the town (kalay.c) at an
//       entrance of its own, and left by the matching exit. The inn's
// entrances, STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE to
//       STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST and
//       STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE, take the inn's
//       actor placements and cues. Every other entrance takes the houses'.
//       The inn is full of tourists on their way to see Colosso in Tolbi. The
//       innkeeper offers a room when spoken to across the counter. From
//       elsewhere he asks `Are you going to Tolbi also?`, and the tour's guide
// in the dining room says `Please finish eating if you are taking the tour!`
//       Either sets FLAG_KALAY_HEARD_OF_COLOSSO_TOUR, after which leaving the
// town plays the tour group's departure. Once the tour has left
//       (FLAG_KALAY_COLOSSO_TOUR_LEFT), its guests are gone from the inn. Two
//       people are gone from the houses, one of whom was about to join it, and
//       those who stay say so. Searching the ovens shows what the cooks are
//       making, except that the oven with roasted eggplant gives a Smoke Bomb.
// Its find comes first in the table under the same metatile behavior, and a
// named examine cue ignores its condition, so it answers every time and  the
// eggplant line is unused. Barrels hold 11 coins and an Elixir. The  inn's
// table and the houses' both carry these examine cues. The  Smoke Bomb's
// oven is in the easternmost house (metatile (34, 6)), the 11  coins' barrel
// in the house in the town's lower right corner (metatile  (12, 27)), and
// the Elixir's barrel in the inn's basement (metatile (24, 40)).
//
//       Stairs lead down from the inn to its basement: a passage with two
// wooden doors in its north wall and two doorways in its south wall, each into a guest room. The
// rooms behind the south doorways have wooden doors of their own back out, in their north walls.
// One holds a young man and a woman about to join the tour, the other two men who fled Lalivero;
// the rooms north of the passage hold Ouranos and Sean, and an empty one.
// - Kalay Sanctum (SceneIdKalaySanctum), whose healer offers the
//       Sanctum's services when spoken to across the counter. Its exit leads
//       back into the town (KALAY_FROM_KALAY_SANCTUM).

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE              StuccoRoomsAHouseEntrances[];
extern SCENE_ENTRANCE              StuccoRoomsASanctumEntrances[];
extern Uint32                      StuccoRoomsAExits[];
extern ACTOR_PLACE                 StuccoRoomsANoActorPlacements[];
extern ACTOR_PLACE                 StuccoRoomsAHouseActorPlacements[];
extern ACTOR_PLACE                 StuccoRoomsAInnActorPlacements[];
extern ACTOR_PLACE                 StuccoRoomsASanctumActorPlacements[];
extern CUE                         StuccoRoomsANoCues[];
extern CUE                         StuccoRoomsAHouseCues[];
extern CUE                         StuccoRoomsAInnCues[];
extern CUE                         StuccoRoomsASanctumCues[];
extern MAP_GRID_METATILE_ANIMATION StuccoRoomsADoorMetatiles[];

// Each table numbers its own rows from ACTOR_FIRST_PLACED.
//
// The tables are the houses' (StuccoRoomsAHouseActorPlacements), the inn's, whose rooms are entered
// through the inn's entrances (StuccoRoomsAInnActorPlacements), and the Kalay Sanctum's
// (StuccoRoomsASanctumActorPlacements).
//
// Unless a note says otherwise, it quotes what its actor says before the Colosso tour leaves.
typedef enum STUCCO_ROOMS_A_ACTOR_ID
{
    // Kalay's houses A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
    ACTOR_YOUNG_MAN_ON_LUNPA_GONE_BAD = ACTOR_FIRST_PLACED,
    // A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
    ACTOR_WOMAN_SAYING_LUNPA_WAS_KIND,
    // A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
    ACTOR_MAN_ON_THE_NORTH,
    // A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), who says the soldiers took the
    // merchant to Lord Hammet's Palace.
    ACTOR_WOMAN_ON_THE_VAULT_MERCHANT,
    // The worried father's son, a bald man in red (SPRITE_BALD_MAN_IN_RED_KALAY), who may not go to
    // Vault.
    ACTOR_WORRIED_FATHERS_SON,
    // An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
    ACTOR_WORRIED_FATHER,
    // A young man in green (SPRITE_YOUNG_MAN_IN_GREEN_KALAY), whose mother's thoughts call him a
    // merchant.
    ACTOR_MERCHANT,
    // The merchant's mother, an old woman (SPRITE_OLD_WOMAN).
    ACTOR_MERCHANTS_MOTHER,
    // A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), gone once the tour has left.
    ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH,
    // A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), about to join the tour, and gone with
    // it.
    ACTOR_WOMAN_BOUND_FOR_THE_TOUR,
    // Two men in white turbans (SPRITE_MAN_IN_WHITE_TURBAN) who fled Lalivero, in Gondowan.
    ACTOR_MAN_FROM_LALIVERO,
    ACTOR_SECOND_MAN_FROM_LALIVERO,
    // Kalay's inn: all but the innkeeper, his wife and ACTOR_INN_GUEST are gone once the tour has
    // left. A man with dark hair (SPRITE_MAN_WITH_DARK_HAIR).
    ACTOR_INNKEEPER = ACTOR_FIRST_PLACED,
    // The innkeeper's wife, a woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON), who says once the
    // tour has left that even their chef went with it.
    ACTOR_INNKEEPERS_WIFE,
    // A chef (SPRITE_CHEF).
    ACTOR_FIRST_TOURIST,
    // A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN), who talks of the Colosso gladiators.
    //
    // Like the other guests the tour takes, she is named by what Kalay's people say of the inn's
    // guests.
    ACTOR_SECOND_TOURIST,
    // The tour's guides, men carrying packs (SPRITE_MAN_CARRYING_A_PACK), in the inn's dining room.
    ACTOR_TOUR_GUIDE,
    ACTOR_SECOND_TOUR_GUIDE,
    // A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN), who expects Lord Hammet in Tolbi.
    ACTOR_THIRD_TOURIST,
    // An old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who would have entered Colosso if he were
    // younger.
    ACTOR_FOURTH_TOURIST,
    // An old woman (SPRITE_OLD_WOMAN), who stays once the tour has left.
    ACTOR_INN_GUEST,
    // A woman with a sword (SPRITE_PERSON_WITH_SWORD).
    ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO,
    // The warriors bound for Colosso, Ouranos and Sean.
    //
    // They are young men with red and with blue hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR and
    // SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, as in ship_rooms.c), named by what each says of the other.
    ACTOR_OURANOS,
    ACTOR_SEAN,
    // The Kalay Sanctum A man in a tall hat (SPRITE_MAN_IN_TALL_HAT).
    ACTOR_HEALER = ACTOR_FIRST_PLACED
} STUCCO_ROOMS_A_ACTOR_ID;

// Kalay's inn, as mnInnCheckIn numbers the inns.
enum
{
    KALAY_INN = 7
};

static void           scStuccoRoomsAApplyRoomState(void);

// Selects the entrance table of Kalay's houses and inn, or of the Kalay Sanctum.
const SCENE_ENTRANCE *scGetEntrances(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_STUCCO_ROOMS_A)
    {
        return StuccoRoomsAHouseEntrances;
    }
    if (sceneId == SCENE_KALAY_SANCTUM)
    {
        return StuccoRoomsASanctumEntrances;
    }
    return KalayFallbackEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return StuccoRoomsAExits;
}

// The inn's rooms share one actor placement table and the houses another.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    Sint32 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_STUCCO_ROOMS_A)
    {
        Sint32       entrance = MainRoutineStateWork.entrance;
        ACTOR_PLACE *table;

        switch (entrance)
        {
            case STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE:
            case STUCCO_ROOMS_A_INN:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
            case STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
                table = StuccoRoomsAInnActorPlacements;
                break;
            default: table = StuccoRoomsAHouseActorPlacements; break;
        }
        fdAssignViewFlags(table);
        return table;
    }
    if (sceneId == SCENE_KALAY_SANCTUM)
    {
        return StuccoRoomsASanctumActorPlacements;
    }
    return StuccoRoomsANoActorPlacements;
}

// Selects room dialogue with the same entrance grouping as the actor placements.
const CUE *scGetCues(void)
{
    Sint32 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_STUCCO_ROOMS_A)
    {
        Sint32 entrance = MainRoutineStateWork.entrance;

        switch (entrance)
        {
            case STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE:
            case STUCCO_ROOMS_A_INN:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
            case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
            case STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE: return StuccoRoomsAInnCues;
            default                                                  : return StuccoRoomsAHouseCues;
        }
    }
    if (sceneId == SCENE_KALAY_SANCTUM)
    {
        return StuccoRoomsASanctumCues;
    }
    return StuccoRoomsANoCues;
}

// Isaac talks to the tour guide with a pack in the dining room of Kalay's inn: "Please finish
// eating if you are taking the tour! We'll be leaving soon!" From then on, leaving the town plays
// the tour group's departure.
static void scStuccoRoomsATalkToTourGuide(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgKalayPleaseFinishEatingIfTaking);
    // "Please finish eating if you are taking the tour! We'll be..."
    fdScriptShowMessage(ACTOR_TOUR_GUIDE, 0);
    gmFlagSet(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR);
    fdScriptEnd();
}

// Isaac talks to the young man with brown hair in a guest room of the inn's basement, the one
// behind the passage's south-east doorway, and he asks whether Isaac knows the continent south of
// Tolbi. Yes: the guest next door is dressed like he comes from Gondowan. No: he tells of Gondowan,
// very large and very warm.
//
// He has left with the Colosso tour once it has gone.
static void scStuccoRoomsAAskAboutSouthernContinent(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgKalayDoKnowAboutContinentSouth);
    // "Do you know about the continent south of Tolbi?" Yes: "The guest next door is dressed like
    // he comes from Gondowan." No: "That continent is called Gondowan. It is very large. They..."
    fdScriptAskYesNo(ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH, 0);
    fdScriptEnd();
}

// Isaac talks to Kalay's innkeeper across the counter, facing west, and is offered a room. From
// anywhere else the innkeeper asks, "Are you going to Tolbi also?", or, once the tour has gone,
// says the inn feels empty.
static void scStuccoRoomsATalkToInnkeeper(void)
{
    ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

    if (leader->facing > FACING_SOUTHWEST && leader->facing < FACING_NORTHWEST)
    {
        mnInnCheckIn(KALAY_INN, ACTOR_INNKEEPER);
    }
    else
    {
        fdScriptStart();

        if (gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0)
        {
            fdScriptSetMessageId(MsgKalayOurInnFeelsEmptyNow);
            // "Our inn feels so empty, now that the Colosso tour group has..."
            fdScriptShowMessage(ACTOR_INNKEEPER, 0);
        }
        else
        {
            fdScriptSetMessageId(MsgKalayGoingTolbiAlso);
            // "Are you going to Tolbi also?" Yes: "Oh, so you must be taking the Colosso tour,
            // too... The..." No: "Merchants are always coming up with new ideas. A Colosso..."
            fdScriptAskYesNo(ACTOR_INNKEEPER, 0);
            gmFlagSet(FLAG_KALAY_HEARD_OF_COLOSSO_TOUR);
        }

        fdScriptEnd();
    }
}

// Isaac talks to the healer in a tall hat across the Kalay Sanctum's counter and is offered the
// Sanctum's services. From the side the healer only says that one can live in peace in Kalay.
static void scStuccoRoomsATalkToHealer(void)
{
    ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

    if (leader->facing > FACING_NORTHWEST && leader->facing < FACING_NORTHEAST)
    {
        mnSanctumRun(ACTOR_HEALER);
    }
    else
    {
        fdScriptStart();
        fdScriptSetMessageId(MsgKalayCanLiveInPeaceIn);
        // "You can live in peace in Kalay, as long as you practice good..."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptEnd();
    }
}

// Isaac walks north into one of the four wooden doors of the inn's basement, and it opens as he
// walks through: the two in the north wall of the passage (metatiles (34, 40) and (39, 40)), into
// the guest rooms north of it, or the door in the north wall of either guest room behind the
// passage's south doorways (metatiles (50, 38) and (52, 49)), back out to the passage.
//
// Each door's step cue is the exit it leaves by, and StuccoRoomsADoorMetatiles holds the metatiles
// its opening is drawn on: the two metatiles over the door's step metatile on map layer 2, whose
// metatiles begin at the map's row 60, so the door at (34, 40) opens at the map's (34, 98) and (34,
// 99).
static void scStuccoRoomsAOpenDoorAndExit(void)
{
    FIELD_STATE *state = FieldStateWork;
    Uint32       slot;
    Sint32       doorIndex;

    fdScriptStart();

    // Prevent idle movement while the doorway metatiles are animated.
    for (slot = ACTOR_FIRST_PLACED; slot <= ACTOR_LAST_ID; slot++)
    {
        ACTOR *actor = fdActorGet(slot);

        if (actor != NULL)
        {
            actor->motionFlags = 0;
        }
    }

    switch (state->stepCue)
    {
        case STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST: doorIndex = 0; break;
        case STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST: doorIndex = 1; break;
        case STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE: doorIndex = 2; break;
        case STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE: doorIndex = 3; break;
        default                                                : return;
    }

    sySoundPlay(SE_DOOR_OPEN);

    {
        Uint32 x = StuccoRoomsADoorMetatiles[doorIndex].x;
        Uint32 y = StuccoRoomsADoorMetatiles[doorIndex].y;

        fdMapGridAnimateMetatiles(StuccoRoomsADoorMetatiles[doorIndex].steps, x, y);
    }

    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);

    fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = 0;

    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -8);
    fdScriptWait(10);

    syImportScriptSetExitRequest(state->stepCue);
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptEnd();
}

// Sets the opening transition and restores room-specific actors and tiles.
Sint32 scInit(void)
{
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    if (MainRoutineStateWork.next == SCENE_STUCCO_ROOMS_A)
    {
        scStuccoRoomsAApplyRoomState();
    }
    return 0;
}

// Once the Colosso tour has left, its guests are gone from Kalay's inn, and two people from the
// houses. In the north-east house's lower room the weapon shop's top right corner, metatiles (30,
// 16) to (33, 17), would show at the bottom left of the view, so the empty metatiles north of it
// cover it.
static void scStuccoRoomsAApplyRoomState(void)
{
    Sint16 entrance = MainRoutineStateWork.entrance;

    switch (entrance)
    {
        case STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER:
        {
            fdMapGridCopyMetatilesTo(30, 14, 30, 16, 4, 2);
            return;
        }
        case STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE:
        case STUCCO_ROOMS_A_INN:
        case STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN:
        case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST:
        case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST:
        case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST:
        case STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST:
        case STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE:
            if (gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0)
            {
                fdActorReleaseById(ACTOR_FIRST_TOURIST);
                fdActorReleaseById(ACTOR_SECOND_TOURIST);
                fdActorReleaseById(ACTOR_TOUR_GUIDE);
                fdActorReleaseById(ACTOR_SECOND_TOUR_GUIDE);
                fdActorReleaseById(ACTOR_THIRD_TOURIST);
                fdActorReleaseById(ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO);
                fdActorReleaseById(ACTOR_OURANOS);
                fdActorReleaseById(ACTOR_SEAN);
                fdActorReleaseById(ACTOR_FOURTH_TOURIST);
            }
            else
            {
                fdActorSetSpriteColorById(ACTOR_SECOND_TOUR_GUIDE, SPRITE_COLOR_RBG);
            }
            return;
        default:
            if (gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0)
            {
                fdActorReleaseById(ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH);
                fdActorReleaseById(ACTOR_WOMAN_BOUND_FOR_THE_TOUR);
            }
            break;
    }
}

#include "shared/kalay_fallback_entrances.c"

SCENE_ENTRANCE StuccoRoomsAHouseEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {STUCCO_ROOMS_A_HOUSE_NORTHWEST,
     CONDITION_ALWAYS, 144,
     0, 192,
     FACING_NORTH, 0,
     5, 45,
     245, 230,
     0},
    {STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER,
     CONDITION_ALWAYS, 400,
     0, 192,
     FACING_NORTH, 0,
     255, 45,
     485, 230,
     0},
    {STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER,
     CONDITION_ALWAYS, 624,
     0, 240,
     FACING_NORTH, 0,
     500, 45,
     735, 275,
     0},
    {STUCCO_ROOMS_A_HOUSE_SOUTHEAST,
     CONDITION_ALWAYS, 128,
     0, 432,
     FACING_NORTH, 0,
     35, 290,
     275, 468,
     0},
    {STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER,
     CONDITION_ALWAYS, 384,
     0, 384,
     FACING_NORTH, 0,
     305, 255,
     545, 485,
     0},
    {STUCCO_ROOMS_A_WEAPON_SHOP,
     CONDITION_ALWAYS, 464,
     0, 448,
     FACING_NORTH, 0,
     305, 255,
     545, 485,
     0},
    {STUCCO_ROOMS_A_ARMOR_SHOP,
     CONDITION_ALWAYS, 720,
     0, 496,
     FACING_NORTH, 0,
     630, 305,
     875, 535,
     0},
    {STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER,
     CONDITION_ALWAYS, 800,
     0, 440,
     FACING_NORTH, 0,
     630, 305,
     875, 535,
     0},
    {STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE,
     CONDITION_ALWAYS, 48,
     0, 640,
     FACING_NORTH, 0,
     0, 495,
     305, 775,
     0},
    {STUCCO_ROOMS_A_INN, CONDITION_ALWAYS, 224, 0, 736, FACING_NORTH, 0, 0, 495, 305, 775, 0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN,
     CONDITION_ALWAYS, 408,
     0, 616,
     FACING_SOUTH, 0,
     355, 525,
     690, 710,
     0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST,
     CONDITION_ALWAYS, 552,
     0, 648,
     FACING_SOUTH, 0,
     355, 525,
     690, 710,
     0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST,
     CONDITION_ALWAYS, 632,
     0, 648,
     FACING_SOUTH, 0,
     355, 525,
     690, 710,
     0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST,
     CONDITION_ALWAYS, 552,
     0, 680,
     FACING_NORTH, 0,
     355, 525,
     690, 710,
     0},
    {STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST,
     CONDITION_ALWAYS, 632,
     0, 680,
     FACING_NORTH, 0,
     355, 525,
     690, 710,
     0},
    {STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE,
     CONDITION_ALWAYS, 808,
     0, 616,
     FACING_SOUTH, 0,
     745, 570,
     935, 720,
     0},
    {STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE,
     CONDITION_ALWAYS, 440,
     0, 872,
     FACING_NORTH, 0,
     340, 740,
     520, 900,
     0},
    {STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE,
     CONDITION_ALWAYS, 584,
     0, 872,
     FACING_NORTH, 0,
     540, 740,
     730, 900,
     0},
    {STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE,
     CONDITION_ALWAYS, 840,
     0, 792,
     FACING_SOUTH, 0,
     710, 740,
     940, 900,
     0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE StuccoRoomsASanctumEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {KALAY_SANCTUM_FROM_KALAY,
     CONDITION_ALWAYS, 248,
     0, 304,
     FACING_NORTH, 0,
     110, 35,
     390, 345,
     0},
    {KALAY_SANCTUM_AFTER_DEFEAT,
     CONDITION_ALWAYS, 248,
     0, 128,
     FACING_SOUTH, 0,
     110, 35,
     390, 345,
     0},
    {SCENE_TABLE_END},
};

Uint32 StuccoRoomsAExits[] = {
    SCENE_EXITS(StuccoRoomsA),
    SCENE_EXIT(STUCCO_ROOMS_A_HOUSE_NORTHWEST_TO_KALAY, Kalay, KALAY_FROM_HOUSE_NORTHWEST),
    SCENE_EXIT(
        STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER_TO_KALAY, Kalay, KALAY_FROM_HOUSE_NORTHEAST_UPPER),
    SCENE_EXIT(
        STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER_TO_KALAY, Kalay, KALAY_FROM_HOUSE_NORTHEAST_LOWER),
    SCENE_EXIT(STUCCO_ROOMS_A_HOUSE_SOUTHEAST_TO_KALAY, Kalay, KALAY_FROM_HOUSE_SOUTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER_TO_KALAY, Kalay, KALAY_FROM_WEAPON_SHOP_COUNTER),
    SCENE_EXIT(STUCCO_ROOMS_A_WEAPON_SHOP_TO_KALAY, Kalay, KALAY_FROM_WEAPON_SHOP),
    SCENE_EXIT(STUCCO_ROOMS_A_ARMOR_SHOP_TO_KALAY, Kalay, KALAY_FROM_ARMOR_SHOP),
    SCENE_EXIT(STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER_TO_KALAY, Kalay, KALAY_FROM_ARMOR_SHOP_COUNTER),
    SCENE_EXIT(
        STUCCO_ROOMS_A_INN_TO_INN_PASSAGE, StuccoRoomsA, STUCCO_ROOMS_A_INN_PASSAGE_FROM_INN),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_TO_KALAY, Kalay, KALAY_FROM_INN),
    SCENE_EXIT(
        STUCCO_ROOMS_A_INN_PASSAGE_TO_INN, StuccoRoomsA, STUCCO_ROOMS_A_INN_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST,
               StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST,
               StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST,
               StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST,
               StuccoRoomsA,
               STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE,
               StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE,
               StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE,
               StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST),
    SCENE_EXIT(STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE,
               StuccoRoomsA,
               STUCCO_ROOMS_A_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST),
    SCENE_EXITS(KalaySanctum),
    SCENE_EXIT(KALAY_SANCTUM_TO_KALAY, Kalay, KALAY_FROM_KALAY_SANCTUM),
    SCENE_EXITS_END,
};

ACTOR_PLACE StuccoRoomsANoActorPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
ACTOR_PLACE StuccoRoomsAHouseActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        101, 0, 181, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        167, 0, 109, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        383, 0, 108, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        346, 0, 109, FACING_EAST_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED_KALAY, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        573, 0, 163, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        670, 0, 154, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN_KALAY, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        183, 0, 416, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        120, 0, 369, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        856, 0, 628, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        780, 0, 633, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        795, 0, 806, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        857, 0, 841, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE StuccoRoomsAInnActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        120, 0, 627, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        505, 0, 665, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        66, 0, 622, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        196, 0, 624, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        232, 0, 586, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        168, 0, 570, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        207, 0, 672, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        168, 0, 643, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        247, 0, 736, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        263, 0, 627, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        417, 0, 810, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        459, 0, 820, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE StuccoRoomsASanctumActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 106, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

CUE StuccoRoomsANoCues[] = {
    { CUE_TABLE_END },
};

// A bare flag as a condition holds while that flag is clear.
//
// So these lines are spoken before the Colosso tour leaves, the next ones after.
CUE StuccoRoomsAHouseCues[] = {
    { CUE_EXIT, STUCCO_ROOMS_A_HOUSE_NORTHWEST_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_NORTHWEST_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_NORTHEAST_UPPER_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_NORTHEAST_LOWER_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_HOUSE_SOUTHEAST_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_HOUSE_SOUTHEAST_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_WEAPON_SHOP_COUNTER_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_WEAPON_SHOP_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_WEAPON_SHOP_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_ARMOR_SHOP_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_ARMOR_SHOP_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_ARMOR_SHOP_COUNTER_TO_KALAY },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_A_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsAOpenDoorAndExit) },
    { CUE_EXIT, STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_A_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_A_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsAOpenDoorAndExit) },
    { CUE_TALK, ACTOR_YOUNG_MAN_ON_LUNPA_GONE_BAD, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImShockedToHearTheRighteous) },
    { CUE_TALK, ACTOR_WOMAN_SAYING_LUNPA_WAS_KIND, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpaWasAKindThiefHis) },
    { CUE_TALK, ACTOR_MAN_ON_THE_NORTH, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSomehowTheNorthHasBecomeThe) },
    { CUE_TALK, ACTOR_WOMAN_ON_THE_VAULT_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsAMerchantFromVaultHadBeen) },
    { CUE_TALK, ACTOR_WORRIED_FATHERS_SON, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsNoOneIsAllowedToGo) },
    { CUE_TALK, ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDoNotWantMySon) },
    { CUE_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantSkillForA) },
    { CUE_TALK, ACTOR_MERCHANTS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonReceivedHisKnackFor) },
    { CUE_TALK, ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsAAskAboutSouthernContinent) },
    { CUE_TALK, ACTOR_WOMAN_BOUND_FOR_THE_TOUR, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIBetterGetReadyTheColosso) },
    { CUE_TALK, ACTOR_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsWeFledFromGondowanToThe) },
    { CUE_TALK, ACTOR_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeCameHereToEscapeThe) },
    { CUE_TALK, ACTOR_SECOND_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsLaliveroWhereImFromIsThe) },
    { CUE_TALK, ACTOR_SECOND_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeEscapedFromTheNortheasternRegion) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ON_LUNPA_GONE_BAD, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheRegionNorthOfVaultWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SAYING_LUNPA_WAS_KIND, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpasReputationIsVeryBadNowadays) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_NORTH, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsThreeYearsAgoThievesRaidedMt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_VAULT_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheySayOtherNorthernPartsAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_FATHERS_SON, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImAnAdultNowIDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDontCareHowMySon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantThingIsNot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANTS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonIsVeryTightWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_ABOUT_THE_SOUTH, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsPeopleFromTheContinentSouthOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_BOUND_FOR_THE_TOUR, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImPrettyExcitedBecauseThisIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsIFledAllThisWayNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhatColossoIsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MAN_FROM_LALIVERO, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsItLooksLikeColossoWillSoon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_MAN_FROM_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoScaredComingTo) },
    { EXAMINE_CUE(EXAMINE_BARREL), 100, FLAG_STUCCO_ROOMS_A_COINS_BARREL, FIND_COINS(11) },
    { EXAMINE_CUE(EXAMINE_OVEN), 101, FLAG_STUCCO_ROOMS_A_SMOKE_BOMB_OVEN, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_CUE(EXAMINE_BARREL), 102, FLAG_STUCCO_ROOMS_A_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    // Unused: this oven's item row answers first (see the maps README).
    { EXAMINE_CUE(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { CUE_TABLE_END },
};

CUE StuccoRoomsAInnCues[] = {
    { CUE_EXIT, STUCCO_ROOMS_A_INN_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_TO_INN_PASSAGE },
    { CUE_EXIT, STUCCO_ROOMS_A_INN_TO_KALAY, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_TO_KALAY },
    { CUE_EXIT, STUCCO_ROOMS_A_INN_PASSAGE_TO_INN, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_PASSAGE_TO_INN },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsAOpenDoorAndExit) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsAOpenDoorAndExit) },
    { CUE_EXIT, STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
    { CUE_EXIT, STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS, STUCCO_ROOMS_A_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
    { CUE_EXIT, STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_A_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
    { CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsATalkToInnkeeper) },
    { CUE_TALK, ACTOR_INNKEEPERS_WIFE, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsEveryoneIsGoingToTolbiTo) },
    { CUE_TALK, ACTOR_INNKEEPERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenOurChefLeftSoWe) },
    { CUE_TALK, ACTOR_FIRST_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIveSignedOnForATour) },
    { CUE_TALK, ACTOR_SECOND_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheColossoGladiatorsAreAllStrong) },
    { CUE_TALK, ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsATalkToTourGuide) },
    { CUE_TALK, ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEveryoneLooksForwardToGoingTo) },
    { CUE_TALK, ACTOR_THIRD_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSurelyLordHammetWillPopUp) },
    { CUE_TALK, ACTOR_FOURTH_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWouldHaveEnteredColossoIf) },
    { CUE_TALK, ACTOR_INN_GUEST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsManyOfTheGuestsHereSeem) },
    { CUE_TALK, ACTOR_INN_GUEST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsUhOhHesStillHereEven) },
    { CUE_TALK, ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsArentYouGoingToColossoYou) },
    { CUE_TALK, ACTOR_OURANOS, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIfYouAreEnteringColossoYou) },
    { CUE_TALK, ACTOR_SEAN, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWereSoStrongMakingTheFinals) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsHesAShrewdMerchantIfHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIdHaveGoneOnTheColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPERS_WIFE, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsEveryYearEveryoneGoesToTolbi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenIfMyHusbandWantedTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsJoiningATourGroupIsThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhichWarriorWillWin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsItsBusinessSoYouCantHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImExhaustedFromPreparingThisTour) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoExcitedICouldnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FOURTH_TOURIST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMyAnimalInstinctsStartWakingUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_GUEST, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayInteriorsColossoIsSoCommonWhatsSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_GUEST, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICantSeemToRelaxNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_SIGNING_UP_FOR_COLOSSO, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImGoingToSignUpFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OURANOS, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIHavePowerAndSeanHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SEAN, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeAreTheToughestOfAll) },
    { EXAMINE_CUE(EXAMINE_BARREL), 100, FLAG_STUCCO_ROOMS_A_COINS_BARREL, FIND_COINS(11) },
    { EXAMINE_CUE(EXAMINE_OVEN), 101, FLAG_STUCCO_ROOMS_A_SMOKE_BOMB_OVEN, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_CUE(EXAMINE_BARREL), 102, FLAG_STUCCO_ROOMS_A_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
    // Unused: this oven's item row answers first (see the maps README).
    { EXAMINE_CUE(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
    { CUE_TABLE_END },
};

CUE StuccoRoomsASanctumCues[] = {
    { CUE_EXIT, KALAY_SANCTUM_TO_KALAY, CONDITION_ALWAYS, KALAY_SANCTUM_TO_KALAY },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsATalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICameHereToSpreadGood) },
    { CUE_TABLE_END },
};
// clang-format on

// A door of the rooms opening, and where each door is: its two metatiles on map layer 2, 58 rows
// below the door's step metatile in the map (60 rows down to the layer, then the two rows over the
// step metatile).
Uint16 StuccoRoomsADoorMetatileSteps[] = {
    MAPGRID_ANIMATION_STEP(7, 125, 1, 2, 5),
    MAPGRID_ANIMATION_STEP(8, 125, 1, 2, 5),
    MAPGRID_ANIMATION_STEPS_END,
};
MAP_GRID_METATILE_ANIMATION StuccoRoomsADoorMetatiles[] = {
    {StuccoRoomsADoorMetatileSteps, 34, 98 },
    {StuccoRoomsADoorMetatileSteps, 39, 98 },
    {StuccoRoomsADoorMetatileSteps, 50, 96 },
    {StuccoRoomsADoorMetatileSteps, 52, 107},
};

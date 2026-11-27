// Inside Madra's buildings: the inn, the round sanctum, the houses, the mayor's house with the
// Black Crystal on its mantel, the jail where Piers is held, and, as a second scene, the left
// cavern home in the cliff. Walking into the jail the first time, Felix sees Shin accuse Piers of
// being one of the Champa and Piers, angered, lift him on a shaft of ice. Later the old couple who
// love mushrooms trade Char, the Mars Djinni, for the Healing Fungus from the Gondowan Cliffs, and
// when Felix brings Piers home the mayor's household welcomes him.

#include "item_ids.h"
#include "field/field.h"
#include "game/game.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "edition.h"
#include "field/field_sprite.h"
#include "field/field_mode_script.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

// Set once the old man of the mushroom house has been given the Healing Fungus and has traded Char
// for it.
#define FLAG_MADRA_HEALING_FUNGUS_GIVEN    0x98a
// Set as the Healing Fungus is given, for this visit: the couple look forward to dinner until the
// party leaves the area.
#define FLAG_MADRA_FUNGUS_FOR_DINNER       FLAG_AREA_TEMP_10
// Set once the old man has been offered the Laughing Fungus on this visit.
#define FLAG_MADRA_LAUGHING_FUNGUS_OFFERED FLAG_SCENE_TEMP_10
// Set once the man focusing on making ice has groaned on this visit.
#define FLAG_MADRA_FOCUSING_MAN_GROANED    FLAG_SCENE_TEMP_11

// Madra's houses, numbering the rows MadraRoomsPlacements and MadraRoomsPlacementsOnReturn share.
typedef enum MADRA_ROOMS_ACTOR_ID
{
    // A couple who fear what lies south in Gondowan, and then the Kibombo.
    ACTOR_OLD_MAN_WARY_OF_GONDOWAN = ACTOR_FIRST_PLACED,
    ACTOR_WOMAN_WARY_OF_GONDOWAN,
    // The old man who loves the rare mushrooms of the Gondowan Cliffs, in the house south of the
    // market.
    ACTOR_MUSHROOM_LOVER,
    // His wife, who cooks them.
    ACTOR_MUSHROOM_COOK,
    // Two men curious about the strange ship grounded in the cove to the east.
    ACTOR_MAN_ON_THE_STRANGE_SHIP,
    ACTOR_MAN_ON_THE_GROUNDED_SHIP,
    // The elder's household: who keeps the jewel on his mantel with Milady, his daughter, the old
    // woman of the house, and his two grandchildren.
    ACTOR_JEWEL_KEEPER,
    ACTOR_ELDERS_DAUGHTER,
    ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE,
    ACTOR_ELDERS_GRANDSON,
    ACTOR_ELDERS_GRANDDAUGHTER,
    // The sanctum's healer and the two women who pray there.
    ACTOR_HEALER,
    ACTOR_WOMAN_OF_THE_SANCTUM,
    ACTOR_OLD_WOMAN_OF_THE_SANCTUM,
    ACTOR_INNKEEPER,
    // Who helps at the inn, sure it is safe from the Champa.
    ACTOR_INN_HELPER,
    // A girl staying at the inn, who hid under her bed in the raid.
    ACTOR_GIRL_STAYING_AT_THE_INN,
    // Char, the Mars Djinni the mushroom couple keep.
    ACTOR_CHAR,
    // A blank actor on the elder's mantel that shows the Black Crystal
    // (scMadraRoomsPlaceBlackCrystal).
    ACTOR_BLACK_CRYSTAL
} MADRA_ROOMS_ACTOR_ID;

// The rest of MadraRoomsPlacements.
typedef enum MADRA_ROOMS_TRAVELERS_ACTOR_ID
{
    // Two travelers at the inn, the first slept through the pirate raid; his partner wears the same
    // sprite in other colours.
    ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID = ACTOR_BLACK_CRYSTAL + 1,
    ACTOR_TRAVELERS_PARTNER,
    // The two warriors on their way to Colosso in Tolbi, there once FLAG_TRANSFERRED_WON_COLOSSO is
    // set: "I'm Morgan, and this here's Dekka."
    ACTOR_MORGAN,
    ACTOR_DEKKA
} MADRA_ROOMS_TRAVELERS_ACTOR_ID;

// The rest of MadraRoomsPlacementsOnReturn, when the party brings Piers home. The rows for Piers,
// Jenna and Sheba go to their party slots.
typedef enum MADRA_ROOMS_RETURN_ACTOR_ID
{
    // The elder, who held Piers' jewel, and Madra's mayor.
    ACTOR_ELDER = ACTOR_BLACK_CRYSTAL + 1,
    ACTOR_MAYOR,
    // Unused: two more rows with Kraden's sprite, out of sight.
    ACTOR_UNUSED_FIRST_KRADEN,
    ACTOR_UNUSED_SECOND_KRADEN,
    ACTOR_KRADEN,
    ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN,
    ACTOR_TRAVELERS_PARTNER_ON_RETURN
} MADRA_ROOMS_RETURN_ACTOR_ID;

// The jail's actors, by the rows of MadraRoomsJailPlacements; Piers' row goes to his party slot.
typedef enum MADRA_ROOMS_JAIL_ACTOR_ID
{
    // The puddle Piers freezes into a shaft of ice.
    ACTOR_JAIL_PUDDLE = ACTOR_FIRST_PLACED,
    // Shin, who threatens Piers, and his friend, who accuses him of being one of the Champa.
    ACTOR_SHIN,
    ACTOR_SHINS_FRIEND,
    // Kraden, who jumps out from behind Felix.
    ACTOR_JAIL_KRADEN,
    ACTOR_SECOND_JAIL_PUDDLE,
    // Once FLAG_GABOMBA_MESSAGE_CARRIED is set: who saw Isaac's party hear of Felix, and a man who
    // has been focusing for days to make ice as Piers did.
    ACTOR_WITNESS_ON_ISAACS_PARTY,
    ACTOR_MAN_FOCUSING_ON_ICE
} MADRA_ROOMS_JAIL_ACTOR_ID;

// The left cavern home (MadraRoomsCavernHomes), by the rows of MadraRoomsSecondRoomsPlacements.
typedef enum MADRA_ROOMS_CAVERN_HOME_ACTOR_ID
{
    // A couple who talk of Alhafra, and later of a plan to make money.
    ACTOR_MAN_ON_ALHAFRA = ACTOR_FIRST_PLACED,
    ACTOR_WOMAN_ON_ALHAFRA,
    // A woman lying ill in bed until Briggs is caught.
    ACTOR_SISTER_IN_BED,
    // Her sister, glad she is better.
    ACTOR_HER_SISTER
} MADRA_ROOMS_CAVERN_HOME_ACTOR_ID;

// The metatiles, (44, 5) and (44, 6) a few steps in from the mayor's door, whose step cue runs
// scMadraRoomsWelcomePiersHome.
enum
{
    METATILE_BEHAVIOR_MADRA_ROOMS_WELCOME = 30
};

// Madra's inn, as mnInnCheckIn numbers the inns.
enum
{
    MADRA_INN = 1
};

extern Sint32         MadraRoomsShinLifted[];
extern Sint32         MadraRoomsShinFalls[];
void                  scMadraRoomsSayFarewellAgain(void);

void                  scMadraRoomsAskToStayLonger(void);

extern Sint32         MadraRoomsOldManWalksToBackRoom[];
extern Sint32         MadraRoomsWifeComesBack[];
extern Sint32         MadraRoomsOldManComesBack[];

extern Sint32         MadraRoomsCharFollows[];

// Counts one more weird answer Felix gives (fdScriptCountAnswer, through the overlay's stub).
void                  scMadraRoomsCountAnswer();

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

extern ACTOR_PLACE MadraRoomsPlacements[];
extern ACTOR_PLACE MadraRoomsPlacementsOnReturn[];
extern ACTOR_PLACE MadraRoomsJailPlacements[];
extern ACTOR_PLACE MadraRoomsSecondRoomsPlacements[];
extern CUE         MadraRoomsCuesBeforeRaid[];
extern CUE         MadraRoomsJailCues[];
extern CUE         MadraRoomsSecondRoomsCues[];
extern CUE         MadraRoomsCuesAfterRaid[];
extern CUE         MadraRoomsCuesOnReturn[];

void               scMadraRoomsShinAngersPiers(void);

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_MADRA_ROOMS_CAVERN_HOMES)
        return MadraRoomsSecondRoomsPlacements;
    if (MainRoutineStateWork.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
        return MadraRoomsJailPlacements;
    if (fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
        return MadraRoomsPlacementsOnReturn;
    return MadraRoomsPlacements;
}

// Talking to the auburn-haired woman in the house by the town's south-west corner: "Indra is in
// direct contact with southern Gondowan... Will we be safe?" Told yes, she says Felix is only
// trying to make her feel better; told no, she tells him to stop getting folks worked up.
void scMadraRoomsAskWillWeBeSafe(void)
{
    Sint32 msg;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    msg = (Sint32)MsgMadraRoomsIndraIsInDirectContact;
    syFieldScriptSetMessageId(msg);
    // "Indra is in direct contact with southern Gondowan... Will we be safe?"
    fdScriptOpenMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
    if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
    {
        syFieldScriptWait(10);
        syFieldScriptSetMessageId(msg + 1);
    }
    else
    {
        syFieldScriptWait(20);
        syFieldScriptSetMessageId(msg + 2);
    }
    fdScriptShowMessage(ACTOR_WOMAN_WARY_OF_GONDOWAN, 0);
    fdScriptEnd();
}

// Talking to Madra's innkeeper from across the counter on the inn's ground floor lets the party
// stay the night. From any other side he talks of the week's troubles, then of the Kibombo's wicked
// magic, and when Felix brings Piers home of the dead fish on the shore, which the later editions
// ask as a question.
void scMadraRoomsTalkToInnkeeper(void)
{
    Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

    if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
    {
        mnInnCheckIn(MADRA_INN, ACTOR_INNKEEPER);
    }
    else
    {
#if !MADRA_INNKEEPER_YES_NO_QUESTION
        // In the Japanese edition there is no choice prompt.
        if (fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsAreYouWorriedAboutAll);
        else if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsIveHeardThatKibomboWield);
        else
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsEarthquakesPirateAttacksWhatWeek);
        // "Earthquakes, pirate attacks... What a week!"
        fdScriptShowMessage(ACTOR_INNKEEPER, 0);
#else
        if (fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
        {
            Sint32 msg = (Sint32)MsgMadraRoomsAreYouWorriedAboutAll;

            syFieldScriptSetMessageId(msg);
            fdScriptOpenMessage(ACTOR_INNKEEPER, 0);
            if (syFieldScriptAskYesNoWithActorAnimation(syFieldPartyGetLeaderActor(), 0) == 0)
            {
                syFieldScriptWait(10);
                syFieldScriptSetMessageId(msg + 1);
            }
            else
            {
                syFieldScriptWait(20);
                syFieldScriptSetMessageId(msg + 2);
            }
            fdScriptShowMessage(ACTOR_INNKEEPER, 0);
        }
        else
        {
            if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
                syFieldScriptSetMessageId((Sint32)MsgMadraRoomsIveHeardThatKibomboWield);
            else
                syFieldScriptSetMessageId((Sint32)MsgMadraRoomsEarthquakesPirateAttacksWhatWeek);
            // "Earthquakes, pirate attacks... What a week!"
            fdScriptShowMessage(ACTOR_INNKEEPER, 0);
        }
#endif
    }
}

// Talking to the healer in the round sanctum from across his counter opens the sanctum's services;
// from any other side he talks of the town's trials, then of the Kibombo, and later of the warming
// sea.
void scMadraRoomsTalkToHealer(Sint32 actor)
{
    Uint16 facing = fdActorGet(MainRoutineStateWork.leaderActorId)->facing;

    if (FACING_NEAREST_CARDINAL(facing) == FACING_NORTH)
    {
        mnSanctumRun(actor);
    }
    else
    {
        if (fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsIveNoticedLatelyThatTemperature);
        else if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsKibomboAreTribeFromGondowan);
        else
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsSurelyThisTownIsEnduring);
        // "Surely this town is enduring some sort of test... We must not fail..."
        fdScriptShowMessage(actor, 0);
    }
}

// Felix goes to touch the Black Crystal on the mantel in the mayor's house: the auburn-haired woman
// by the table hops up, turns to him and cries "Don't touch that gem! The elder gave us strict
// orders to protect it!"
void scMadraRoomsGuardElderGem(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorFaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
    scFieldActorJump(ACTOR_JEWEL_KEEPER, 6, 20);
    fdActorFaceActor(ACTOR_FELIX, ACTOR_JEWEL_KEEPER, 0);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsDontTouchThatGemElder);
    // "Don't touch that gem! The elder gave us strict orders to protect it!"
    fdScriptShowMessage(ACTOR_JEWEL_KEEPER, 0);
    fdActorFaceDirection(ACTOR_JEWEL_KEEPER, FACING_SOUTHEAST, 0);
    fdScriptEnd();
}

// Talking to the old man who loves the rare mushrooms of the Gondowan Cliffs, in the house south of
// the market: he wonders how to climb the cliffs to get one, and once he has the Healing Fungus, he
// looks forward to dinner, then says he ate too much.
void scMadraRoomsTalkToMushroomLover(void)
{
    Sint32 msg;

    if (fdScriptGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN))
    {
        if (fdScriptGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER))
            msg = (Sint32)MsgMadraRoomsICantWaitForDinner;
        else
            msg = (Sint32)MsgMadraRoomsThatMushroomWasGreat;
        syFieldScriptSetMessageId(msg);
        // "That mushroom was great, but I ate way too much. I don't ever want..."
        fdScriptShowMessage(ACTOR_MUSHROOM_LOVER, 0);
    }
    else
    {
        if (fdScriptGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED))
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsThanksForBringing);
            // "Thanks for bringing this by, but it's just not the type of..."
            fdScriptShowMessage(ACTOR_MUSHROOM_LOVER, 0);
        }
        else
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsHowAreWeToClimb);
            // "How are we to climb the cliffs between here and Gondowan? If..."
            fdScriptShowMessage(ACTOR_MUSHROOM_LOVER, 0);
        }
        // The field menu opens as he finishes: the player's chance to use a fungus on him from the
        // items (the USE_ITEM_ON_ACTOR cues).
        FieldStateWork->fieldMenu = 1;
    }
}

// Talking to the mushroom lover's wife, in the same house: she tells of her husband's love of the
// rare mushrooms of the Gondowan Cliffs, begs the party to find the right one once he has turned
// down the Laughing Fungus, and after the Healing Fungus talks of cooking it.
void scMadraRoomsTalkToMushroomCook(void)
{
    if (fdScriptGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN))
    {
        if (fdScriptGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER))
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsThisHeresEnoughOfMushroom);
            // "This here's enough of a mushroom that we'll be eating it for days!"
            fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
        }
        else
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsActuallyIDidntCookMushrooms);
            // "Actually, I didn't cook the mushrooms properly that night. But..."
            fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
        }
    }
    else if (fdScriptGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED))
    {
        syFieldScriptSetMessageId((Sint32)MsgMadraRoomsPleaseWontYouPleaseTry);
        // "Please... Won't you please try to find the mushrooms my husband..."
        fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
    else
    {
        syFieldScriptSetMessageId((Sint32)MsgMadraRoomsMyHusbandRelishesThoseRare);
        // "My husband relishes those rare mushrooms growing in the Gondowan..."
        fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
    }
}

// Casting Mind Read on the mushroom lover: he thinks of the mushrooms he longs for, and once he has
// the Healing Fungus, of dinner.
void scMadraRoomsMindReadMushroomLover(void)
{
    if (fdScriptGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN))
    {
        if (fdScriptGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER))
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsMoreIThinkAboutDinner);
            // "The more I think about dinner tonight, the more I drool..."
            fdScriptShowMessage(ACTOR_MUSHROOM_LOVER, 0);
        }
        else
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsIDontRememberMushroomsLike);
            // "I don't remember mushrooms like that ever tasting bad..."
            fdScriptShowMessage(ACTOR_MUSHROOM_LOVER, 0);
        }
    }
    else
    {
        Sint32 flag = fdScriptGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
        Sint32 msg  = (Sint32)MsgMadraRoomsImDyingForSome;

        if (flag)
        {
            syFieldScriptSetMessageId(msg);
            fdScriptShowMessage(ACTOR_MUSHROOM_LOVER, 0);
        }
        else
        {
            syFieldScriptSetMessageId(msg);
            fdScriptShowMessage(ACTOR_MUSHROOM_LOVER, 0);
        }
    }
}

// Casting Mind Read on the mushroom lover's wife: she thinks of her husband growing old, and once
// he has the Healing Fungus, of cooking it.
void scMadraRoomsMindReadMushroomCook(void)
{
    if (fdScriptGetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN))
    {
        if (fdScriptGetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER))
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsTheseMushroomsWillMakeAny);
            // "These mushrooms will make any meal taste amazing!"
            fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
        }
        else
        {
            syFieldScriptSetMessageId((Sint32)MsgMadraRoomsIfMyHusbandFindsOut);
            // "If my husband finds out I messed up the recipe, he'll be so sad..."
            fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
        }
    }
    else
    {
        Sint32 flag = fdScriptGetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
        Sint32 msg  = (Sint32)MsgMadraRoomsMyPoorHusbandsGettingOld;

        if (flag)
        {
            syFieldScriptSetMessageId(msg);
            fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
        }
        else
        {
            syFieldScriptSetMessageId(msg);
            fdScriptShowMessage(ACTOR_MUSHROOM_COOK, 0);
        }
    }
}

// Talking to the man in the jail who has been trying for days to make ice as Piers did, once Felix
// brings Piers home: the first time on a visit he strains and groans "Uhhhnnn... UHHHNNN!", and
// afterwards he wonders how anyone makes ice just by thinking.
void scMadraRoomsTalkToManFocusingOnIce(void)
{
    if (!fdScriptGetFlag(FLAG_MADRA_FOCUSING_MAN_GROANED))
    {
        fdScriptSetFlag(FLAG_MADRA_FOCUSING_MAN_GROANED);
        syFieldScriptSetMessageId((Sint32)MsgMadraUhhhnnnUhhhnnn);
        fdActorWalkByAndWait(ACTOR_MAN_FOCUSING_ON_ICE, -12, 0);
        syFieldScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_MAN_FOCUSING_ON_ICE, 2);
        syFieldScriptWait(20);
        // "Uhhhnnn... UHHHNNN!"
        fdScriptShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
        scFieldActorShowEmote(ACTOR_MAN_FOCUSING_ON_ICE, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
        syFieldScriptWait(20);
        fdActorWalkByAndWait(ACTOR_MAN_FOCUSING_ON_ICE, 12, 0);
        fdActorFaceActor(ACTOR_MAN_FOCUSING_ON_ICE, ACTOR_FELIX, 0);
        // "They told me that you make Psynergy by focusing your mind... I've..."
        fdScriptShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
    }
    else
    {
        syFieldScriptSetMessageId((Sint32)MsgMadraRoomsHowCanAnyoneJustCreate);
        fdActorFaceActor(ACTOR_MAN_FOCUSING_ON_ICE, ACTOR_FELIX, 0);
        // "How can anyone just create ice by thinking about it? I mean, imagine..."
        fdScriptShowMessage(ACTOR_MAN_FOCUSING_ON_ICE, 0);
    }
}

const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_MADRA_ROOMS_CAVERN_HOMES)
        return MadraRoomsSecondRoomsCues;
    if (MainRoutineStateWork.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
        return MadraRoomsJailCues;
    if (fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
        return MadraRoomsCuesOnReturn;
    if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
        return MadraRoomsCuesAfterRaid;
    return MadraRoomsCuesBeforeRaid;
}

// The Black Crystal sits on the mantel in the mayor's house until the Kibombo steal it in their
// raid (FLAG_BRIGGS_CAUGHT), after which it is gone.
void scMadraRoomsPlaceBlackCrystal(void)
{
    if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
    {
        fdActorSetPos(ACTOR_BLACK_CRYSTAL, 888, 8);
    }
    else
    {
        fdActorSetItemIcon(fdActorGet(ACTOR_BLACK_CRYSTAL), ITEM_BLACK_CRYSTAL);
        fdActorSetSpriteFlags(fdActorGet(ACTOR_BLACK_CRYSTAL), 0);
    }
}

// Places an actor facing an angle, the angle taken as the Uint16 an actor's facing is.
static inline void scMadraRoomsPlaceFacing(Sint32 actorId, Sint32 x, Sint32 z, Uint16 facing)
{
    fdActorSetPosAndFacing(actorId, x, z, facing);
}

// Sets up the jail: the first time Felix walks in, Shin angers Piers (scMadraRoomsShinAngersPiers);
// afterwards Shin and his friend are gone until the Kibombo's raid, when they are back in the jail.
void scMadraRoomsSetUpJail(void)
{
    if (!fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
    {
        FieldStateWork->psynergyDrawFlags =
            PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
        if (!fdScriptGetFlag(FLAG_PIERS_INTERROGATED))
        {
            scMadraRoomsShinAngersPiers();
        }
        else
        {
            fdActorSetPos(ACTOR_SHIN, 0, 0);
            fdActorSetPos(ACTOR_SHINS_FRIEND, 0, 0);
        }
    }
    else if (fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
    {
        scMadraRoomsPlaceFacing(ACTOR_SHIN, toFIXED(515), toFIXED(841), FACING_NORTHEAST);
        scMadraRoomsPlaceFacing(ACTOR_SHINS_FRIEND, toFIXED(472), toFIXED(877), FACING_NORTH);
        fdActorSetPos(ACTOR_JAIL_PUDDLE, 0, 0);
    }
    else
    {
        scMadraRoomsPlaceFacing(ACTOR_SHIN, toFIXED(515), toFIXED(841), FACING_NORTHWEST);
        scMadraRoomsPlaceFacing(ACTOR_SHINS_FRIEND, toFIXED(472), toFIXED(877), FACING_NORTH);
        fdActorSetPos(ACTOR_JAIL_PUDDLE, 0, 0);
    }
}

extern Sint32 MadraRoomsShinAndFriendRunAway[];

// Sets up Madra's buildings as the party walks in: Morgan and Dekka at the inn only for a party
// that won Colosso in The Broken Seal, the jail, the sister lying ill in bed in the left cavern
// home until Briggs is caught, and the Black Crystal.
Sint32        scInit(void)
{
    FIELD_SPR   *sprite;
    FIELD_STATE *event;

    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    fdActorGet(MainRoutineStateWork.leaderActorId)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
    fdEffectStartBlankOverlay(0);
    if (MainRoutineStateWork.next == SCENE_MADRA_ROOMS_HOUSES)
    {
        event                    = RamHeapSlots->fieldState;
        event->psynergyDrawFlags = PSYNERGY_DRAW_NO_SCENE_SPRITE | PSYNERGY_DRAW_FIXED_PRIORITY;
        if (!fdScriptGetFlag(FLAG_TRANSFERRED_WON_COLOSSO))
        {
            fdActorSetPos(ACTOR_MORGAN, 0, 0);
            fdActorSetPos(ACTOR_DEKKA, 0, 0);
        }
        else
        {
            scFieldActorSetSpriteColor(fdActorGet(ACTOR_MORGAN), SPRITE_COLOR_BGR);
            scFieldActorSetSpriteColor(fdActorGet(ACTOR_DEKKA), SPRITE_COLOR_BRG);
        }
        if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
        {
            fdMapGridCopyMetatilesLayered(2, 110, 16, 10, 16, 64);
            fdMapGridCopyMetatileAttributes(2, 46, 16, 10, 16, 0);
        }
    }
    if (MainRoutineStateWork.entrance == MADRA_ROOMS_HOUSES_FROM_MADRA_WEST)
        scMadraRoomsSetUpJail();
    if (MainRoutineStateWork.next == SCENE_MADRA_ROOMS_CAVERN_HOMES)
    {
        if (fdScriptGetFlag(FLAG_BRIGGS_CAUGHT))
        {
            fdActorSetPos(ACTOR_SISTER_IN_BED, 0, 0);
        }
        else
        {
            fdActorSetSpriteRotationById(ACTOR_SISTER_IN_BED, TRIG_TURN * 3 / 4);
            fdActorSetSpriteFlags(fdActorGet(ACTOR_SISTER_IN_BED), 0);
            // Lying under the covers: her sprite turned, its bottom 8 rows sunk.
            ((FIELD_SPR *)fdActorGet(ACTOR_SISTER_IN_BED)->sprite)->flags |= SPRITE_FLAG_SUNK;
            sprite           = fdActorGet(ACTOR_SISTER_IN_BED)->sprite;
            sprite->sunkRows = 8;
            ((FIELD_SPR *)fdActorGet(ACTOR_SISTER_IN_BED)->sprite)->dirty = 1;
        }
    }
    scMadraRoomsPlaceBlackCrystal();
    if (fdScriptGetFlag(FLAG_GABOMBA_MESSAGE_CARRIED))
        scFieldActorSetSpriteColor(fdActorGet(ACTOR_TRAVELERS_PARTNER_ON_RETURN), SPRITE_COLOR_RBG);
    else
        scFieldActorSetSpriteColor(fdActorGet(ACTOR_TRAVELERS_PARTNER), SPRITE_COLOR_RBG);
    return 0;
}

Sint32 scPrepareMap(void)
{
    return 0;
}

// Turns an actor's sprite a thirty-second of a turn, a tick of Shin falling over
// (MadraRoomsShinFalls).
Sint32 scMadraRoomsRotateSprite(ACTOR *actor)
{
    FIELD_SPR *sprite;

    sprite = actor->sprite;
    if (actor != NULL && sprite != NULL)
        sprite->rotation -= TRIG_TURN / 32;
    return 0;
}

// Unused: stands an actor's sprite upright again; no script calls it.
UNUSED Sint32 scMadraRoomsResetSpriteRotation(ACTOR *actor)
{
    FIELD_SPR *sprite;

    sprite = actor->sprite;
    if (actor != NULL && sprite != NULL)
        sprite->rotation = 0;
    return 0;
}

// The party enters the jail as Shin and his friend accuse Piers of being one of the Champa, and
// sees Piers, angered, lift Shin on a shaft of ice.
void scMadraRoomsShinAngersPiers(void)
{
    Sint32 i;
    ACTOR *felix;

    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorSetAnimation(ACTOR_JAIL_PUDDLE, PUDDLE_ANIMATION_PUDDLE);
    fdActorSetPos(ACTOR_FELIX, 400, 888);
    fdCameraMoveTo(408, CAMERA_KEEP, 848, 1);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdActorSetSpeed(ACTOR_PIERS, 0.8);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsAdmitItYoureOne);
    // "Admit it! You're one of the Champa, aren't you, Piers?"
    syFieldScriptShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
    fdActorWalkToAndWait(ACTOR_FELIX, 396, 869);
    scFieldActorLaunchFromLeader(ACTOR_JAIL_KRADEN, 25, 19, FACING_NORTH);
    scFieldActorWaitForScriptEndById(ACTOR_JAIL_KRADEN);
    fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_JAIL_KRADEN, FACING_WEST, 0);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_JAIL_KRADEN, FACING_NORTHEAST, 0);
    syFieldScriptWait(10);
    fdCameraMoveTo(488, CAMERA_KEEP, 820, 1);
    fdCameraWaitForArrival();
    syFieldScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    // "Speak! Or let your silence condemn you!"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    syFieldScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    // "I told you already... I'm not a Champa."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_SHIN, 2);
    // "Then where did you come from?"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTH);
    fdActorWalkByAndWait(ACTOR_PIERS, 0, -16);
    syFieldScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    // "The heart of the Eastern Sea... If I told you where, you'd never..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    scFieldActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    fdActorGet(ACTOR_SHIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
    for (i = 0; i < 3; i++)
    {
        fdActorWalkByAndWait(ACTOR_SHIN, 0, -8);
        fdScriptPlaySound(SE_HEAVY_IMPACT);
        fdActorWalkByAndWait(ACTOR_SHIN, 0, 8);
    }
    fdActorGet(ACTOR_SHIN)->steeringFlags |= ACTOR_FACES_MOTION;
    syFieldScriptWait(30);
    fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Look at me when I'm talking to you! Are you trying to mock me!?"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
    fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    // "I implore you, do not anger me."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Oh, do you? So what's going to happen if I make you angry?"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    fdActorFaceActor(ACTOR_SHINS_FRIEND, ACTOR_SHIN, 0);
    syFieldScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
    // "Hey, Shin... Maybe you should go easy on him..."
    syFieldScriptShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
    fdActorFaceActor(ACTOR_SHIN, ACTOR_SHINS_FRIEND, 0);
    syFieldScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_SHIN, 2);
    // "Hey, it wasn't your girlfriend who got hurt!"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    syFieldScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    // "I'm sorry she was injured, but I cannot°"
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    scFieldActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    fdActorFaceDirectionAndWait(ACTOR_SHIN, FACING_NORTH);
    // "I don't want your sympathy, freak! I want you to get angry for me!"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
    // "Stop, please... Even my patience has its limits."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorFaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    // "Hey, don't you want to see what he'll do?"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    fdActorShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "I think we should just stop..."
    syFieldScriptShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
    // "If my words will not cool your temper, then..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    syFieldScriptWait(20);
    fdActorWalkByAndWait(ACTOR_PIERS, 0, 16);
    fdPsynergyLoadResources(ABILITY_FROST, 1);
    fdPsynergySetupActorPair(ACTOR_PIERS, ACTOR_JAIL_PUDDLE);
    fdPsynergyStartCast();
    fdActorShowEmote(ACTOR_SHIN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    fdActorFaceActor(ACTOR_SHIN, ACTOR_PIERS, 0);
    fdActorFaceActor(ACTOR_SHINS_FRIEND, ACTOR_PIERS, 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_SHIN), 0);
    fdActorSetScript(ACTOR_SHIN, MadraRoomsShinLifted);
    fdPsynergyRunHandler(1);
    fdBattleEffectStopMainActor();
    fdPsynergyEnd();
    // "Oh! That was Psynergy!"
    syFieldScriptShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
    scFieldActorStop(ACTOR_SHIN);
    fdActorGet(ACTOR_SHIN)->scaleX = toFIXED(1);
    fdActorGet(ACTOR_SHIN)->scaleY = toFIXED(1);
    syFieldScriptWait(4);
    fdActorGet(ACTOR_SHIN)->motionFlags    = ACTOR_FALLS;
    fdActorGet(ACTOR_SHIN)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorSetDestinationOffset(ACTOR_SHIN, -8, 0);
    fdActorSetScript(ACTOR_SHIN, MadraRoomsShinFalls);
    scFieldActorWaitForScriptEndById(ACTOR_SHIN);
    fdScriptPlaySound(SE_LANDING_THUD);
    syFieldScriptWait(60);
    fdActorGet(ACTOR_SHIN)->steeringFlags |= ACTOR_FACES_MOTION;
    scFieldActorJump(ACTOR_SHIN, 6, 0);
    syFieldScriptWait(3);
    fdActorSetSpriteRotationById(ACTOR_SHIN, 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_SHIN), 1);
    fdActorFaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_SHINS_FRIEND, FACING_EAST, 0);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_SHINS_FRIEND, FACING_NORTH, 0);
    syFieldScriptWait(20);
    scFieldActorShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    fdActorRunRepeatedMotion(ACTOR_SHIN, 2);
    syFieldScriptWait(20);
    syFieldScriptWait(10);
    scFieldActorJump(ACTOR_SHIN, 6, 20);
    syFieldScriptWait(20);
    fdActorFaceActor(ACTOR_SHIN, ACTOR_PIERS, 0);
    syFieldScriptWait(20);
    fdActorGet(ACTOR_SHINS_FRIEND)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_SHIN)->steeringFlags         &= ~ACTOR_FACES_MOTION;
    fdActorWalkByAndWait(ACTOR_SHIN, -16, 16);
    fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_SHIN, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    syFieldScriptWait(10);
    scFieldActorJump(ACTOR_SHIN, 6, 20);
    syFieldScriptWait(20);
    fdActorGet(ACTOR_SHIN)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorSetScript(ACTOR_SHIN, MadraRoomsShinAndFriendRunAway);
    scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_SHIN);
    scFieldActorWatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_SHIN);
    // "Waaaaaaaaaahh!"
    syFieldScriptShowMessageThenWait(ACTOR_SHIN, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
    syFieldScriptWait(20);
    fdActorFaceActor(ACTOR_SHINS_FRIEND, ACTOR_PIERS, 0);
    scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_SHINS_FRIEND);
    scFieldActorWatchAndSetScript(ACTOR_JAIL_KRADEN, ACTOR_SHINS_FRIEND);
    // "Did you do that?"
    syFieldScriptShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
    fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
    // "What did your friend expect?"
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorShowEmote(ACTOR_SHINS_FRIEND, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    fdActorWalkByAndWait(ACTOR_SHINS_FRIEND, -16, 16);
    // "You monster!"
    syFieldScriptShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 1);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    // "I... I am no monster."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    syFieldScriptWait(20);
    fdActorWalkByAndWait(ACTOR_SHINS_FRIEND, -16, 8);
    syFieldScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_SHINS_FRIEND, 2);
    syFieldScriptWait(20);
    // "Help!!!"
    syFieldScriptShowMessageThenWait(ACTOR_SHINS_FRIEND, 0, 10);
    fdActorGet(ACTOR_SHINS_FRIEND)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorSetScript(ACTOR_SHINS_FRIEND, MadraRoomsShinAndFriendRunAway);
    syFieldScriptWait(40);
    scFieldActorStop(ACTOR_FELIX);
    scFieldActorStop(ACTOR_JAIL_KRADEN);
    scFieldActorWaitForScriptEndById(ACTOR_SHINS_FRIEND);
    fdActorSetPos(ACTOR_SHIN, 0, 0);
    fdActorSetPos(ACTOR_SHINS_FRIEND, 0, 0);
    fdActorSetAnimation(ACTOR_JAIL_PUDDLE, 6);
    fdActorSetSpriteColorById(ACTOR_JAIL_PUDDLE, SPRITE_COLOR_NORMAL);
    fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    fdActorWaitForAnimationChange(8);
    syScriptCameraFollowActor(ACTOR_FELIX, 1);
    fdCameraWaitForArrival();
    fdActorFaceEachOther(ACTOR_JAIL_KRADEN, ACTOR_FELIX, 0);
    fdActorRunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
    syFieldScriptWait(20);
    // "This man... He must be an Adept!" Yes: "Of course! That was clearly Psynergy he just
    // used..." No: "You're right... We only know what we..."
    fdScriptOpenMessage(ACTOR_JAIL_KRADEN, 0);
    if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0)
    {
        fdActorRunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
        // "Of course! That was clearly Psynergy he just used..."
        syFieldScriptShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
        FieldStateWork->msg += 1;
    }
    else
    {
        fdActorSetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_SHAKE_HEAD);
        FieldStateWork->msg += 1;
        // "You're right... We only know what we just saw."
        syFieldScriptShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
    }
    fdActorRunRepeatedMotion(ACTOR_JAIL_KRADEN, 2);
    syFieldScriptWait(10);
    // "Perhaps we should try to learn a little more about him."
    syFieldScriptShowMessageThenWait(ACTOR_JAIL_KRADEN, 0, 10);
    fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_JAIL_KRADEN, ANIM_NOD);
    syFieldScriptWait(10);
    fdActorSetAnimation(ACTOR_JAIL_KRADEN, ANIM_WALK);
    felix = fdActorGet(ACTOR_FELIX);
    if (felix != NULL)
        fdActorSetDestination(
            ACTOR_JAIL_KRADEN, felix->x.part[0].integer, felix->z.part[0].integer);
    scFieldActorWaitForMoveById(ACTOR_JAIL_KRADEN);
    fdActorSetPos(ACTOR_JAIL_KRADEN, 0, 0);
    fdScriptSetFlag(FLAG_PIERS_INTERROGATED);
    fdScriptEnd();
}

// Talking to Piers in his metatile in Madra's jail: "Ah... When will that kindly elder return and
// end my imprisonment? I have no time to wait, but I don't want to have to hurt anyone..."
void scMadraRoomsTalkToPiers(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsAhWhenWillThatKindly);
    fdActorFaceDirection(ACTOR_JAIL_KRADEN, FACING_SOUTH, 0);
    // "Ah... When will that kindly elder return and end my imprisonment? I..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdScriptEnd();
}

// Casting Mind Read on Piers in his metatile: he regrets using his Psynergy in anger, then senses
// someone using Psynergy and looks about for them, and wonders when he will be freed.
void scMadraRoomsMindReadPiers(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsWhyDidIUse);
    // "Why did I use my Psynergy in anger? I must maintain control... My..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_PIERS, 2);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    // "I...sense someone... using Psynergy..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    syFieldScriptWait(10);
    fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
    syFieldScriptWait(10);
    fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
    syFieldScriptWait(10);
    fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
    syFieldScriptWait(10);
    fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    // "That person... Could he be°no, he is not of my people..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
    syFieldScriptWait(20);
    // "When will I be freed?"
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdScriptEnd();
}

// Unused: Piers' Frost at the jail's puddle alone, as scMadraRoomsShinAngersPiers plays it; nothing
// calls it.
UNUSED void scMadraRoomsPiersUsesPsynergy(void)
{
    fdPsynergyLoadResources(ABILITY_FROST, 1);
    fdPsynergySetupActorPair(ACTOR_PIERS, ACTOR_JAIL_PUDDLE);
    fdPsynergyStartCast();
    fdPsynergyRunHandler(1);
    fdBattleEffectStopMainActor();
    fdPsynergyEnd();
}

extern Sint32 MadraRoomsWifeWalksToBackRoom[];
extern Sint32 MadraRoomsLeaderStepsAside[];

// Felix gives the old mushroom lover the Healing Fungus from the Gondowan Cliffs: "Oh, my... Is...
// Is that..." He and his wife go to their back room and come back with Char, the Mars Djinni, who
// joins the party.
void          scMadraRoomsGiveHealingFungus(void)
{
    ACTOR *actor;

    fdScriptSetFlag(FLAG_MADRA_HEALING_FUNGUS_GIVEN);
    fdScriptSetFlag(FLAG_MADRA_FUNGUS_FOR_DINNER);
    gmPartyInventoryRemove(ITEM_HEALING_FUNGUS);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    fdActorSetSpeed(ACTOR_MUSHROOM_COOK, 0.4);
    fdActorWalkTo(ACTOR_MUSHROOM_COOK, 410, 109);
    actor = fdActorGet(ACTOR_FELIX);
    if (actor->z.fixed > toFIXED(95))
        fdActorWalkToAndWait(ACTOR_FELIX, actor->x.fixed / toFIXED(1), 95);
    fdActorWalkToAndWait(ACTOR_FELIX, 362, 95);
    fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
    scFieldActorWaitForMoveById(ACTOR_MUSHROOM_COOK);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsOhMy);
    fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Oh, my... Is... Is that..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
    fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 3);
    fdActorWalkToAndWait(ACTOR_MUSHROOM_COOK, 378, 109);
    fdActorFaceDirection(ACTOR_MUSHROOM_COOK, FACING_NORTHWEST, 0);
    syFieldScriptWait(20);
    // "Is that one of his most favorite mushrooms!?"
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
    fdActorFaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
    syFieldScriptWait(20);
    fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "Did you climb all the way up those cliffs just to get that mushroom..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "That's the nicest thing anyone has ever done for me. I have to thank..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
    fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
    syFieldScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
    syFieldScriptWait(30);
    fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "But what could I have that a warrior like you would want?"
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
    syFieldScriptWait(20);
    // "That's a tricky one..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_SOUTH);
    syFieldScriptWait(10);
    fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_NORTH);
    syFieldScriptWait(10);
    fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_COOK, FACING_WEST);
    syFieldScriptWait(10);
    fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
    scFieldActorStop(ACTOR_CHAR);
    fdActorSetSpeed(ACTOR_MUSHROOM_LOVER, 0.5);
    fdActorSetSpeed(ACTOR_MUSHROOM_COOK, 0.5);
    fdActorSetSpeed(ACTOR_CHAR, 0.5);
    fdActorSetScript(ACTOR_MUSHROOM_COOK, MadraRoomsWifeWalksToBackRoom);
    fdActorSetScript(ACTOR_FELIX, MadraRoomsLeaderStepsAside);
    syFieldScriptWait(60);
    fdActorSetScript(ACTOR_MUSHROOM_LOVER, MadraRoomsOldManWalksToBackRoom);
    scFieldActorWaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
    syFieldScriptWait(120);
    fdActorSetScript(ACTOR_MUSHROOM_COOK, MadraRoomsWifeComesBack);
    scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_MUSHROOM_COOK);
    syFieldScriptWait(60);
    fdActorSetScript(ACTOR_MUSHROOM_LOVER, MadraRoomsOldManComesBack);
    fdActorSetScript(ACTOR_CHAR, MadraRoomsCharFollows);
    scFieldActorWaitForScriptEndById(ACTOR_MUSHROOM_LOVER);
    scFieldActorWaitForScriptEndById(ACTOR_MUSHROOM_COOK);
    scFieldActorWaitForScriptEndById(ACTOR_CHAR);
    fdActorRunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
    syFieldScriptWait(20);
    // "I don't know if you want a pet, but this is all we can offer. Take..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
    fdDjinnJoinParty(ACTOR_CHAR, DJINNI_CHAR / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_CHAR));
    FieldStateWork->msg += 1;
    fdActorSetAnimation(ACTOR_FELIX, ANIM_STAND);
    fdActorFaceDirection(ACTOR_MUSHROOM_LOVER, FACING_SOUTH, 0);
    syFieldScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MUSHROOM_LOVER, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
    // "I can't wait for dinner tonight. Batter-fried mushrooms! Mmm!"
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_MUSHROOM_COOK, ANIM_NOD);
    syFieldScriptWait(20);
    // "This here's enough of a mushroom that we'll be eating it for days!"
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 10);
    fdScriptEnd();
}

// Felix gives the old mushroom lover the Laughing Fungus: he tells his wife "No, dear... They look
// similar, but these aren't quite right." and turns it down.
void scMadraRoomsGiveLaughingFungus(void)
{
    ACTOR *felix;

    fdScriptSetFlag(FLAG_MADRA_LAUGHING_FUNGUS_OFFERED);
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    felix = fdActorGet(ACTOR_FELIX);
    if (felix->z.fixed > toFIXED(95))
        fdActorWalkToAndWait(ACTOR_FELIX, felix->x.fixed / toFIXED(1), 95);
    fdActorWalkToAndWait(ACTOR_FELIX, 362, 95);
    fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsOhMy);
    fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
    syFieldScriptWait(30);
    fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "Oh, my... Is... Is that..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
    fdActorShowEmote(ACTOR_MUSHROOM_COOK, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 3);
    fdActorFaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
    syFieldScriptWait(20);
    // "Is that one of his most favorite mushrooms!?"
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsNoDearTheyLookSimilar);
    fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    fdActorFaceDirectionAndWait(ACTOR_MUSHROOM_LOVER, FACING_EAST);
    syFieldScriptWait(10);
    // "No, dear... They look similar, but these aren't quite right."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
    fdActorFaceEachOther(ACTOR_MUSHROOM_LOVER, ACTOR_MUSHROOM_COOK, 0);
    syFieldScriptWait(20);
    fdActorFaceActor(ACTOR_MUSHROOM_LOVER, ACTOR_FELIX, 0);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_MUSHROOM_LOVER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    syFieldScriptWait(20);
    // "Thanks for bringing this by, but it's just not the type of mushroom..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_LOVER, 0, 5);
    fdActorFaceActor(ACTOR_MUSHROOM_COOK, ACTOR_FELIX, 0);
    syFieldScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_MUSHROOM_COOK, 2);
    syFieldScriptWait(5);
    // "Please... Won't you please try to find the mushrooms my husband..."
    syFieldScriptShowMessageThenWait(ACTOR_MUSHROOM_COOK, 0, 5);
    fdScriptEnd();
}

// A party member walks into Felix and vanishes, as the party gathers up to leave the mayor's house.
static inline void scMadraRoomsWalkIntoLeader(Sint32 actorId)
{
    ACTOR *leader;

    fdActorSetSpeed(actorId, 1.2);
    fdActorSetAnimation(actorId, ANIM_WALK);
    leader = fdActorGet(ACTOR_FELIX);
    if (leader != NULL)
        fdActorSetDestination(actorId, leader->x.part[0].integer, leader->z.part[0].integer);
    scFieldActorWaitForMoveById(actorId);
    fdActorSetPos(actorId, 0, 0);
}

// Walking into the mayor's house when Felix brings Piers home (metatiles (44,
// 5) and (44, 6), a few steps in from the door), the mayor cries "Felix! You're
// back!" and his household welcomes Piers home, asking after his jewel and apologising for his
// imprisonment.
void scMadraRoomsWelcomePiersHome(void)
{
    fdScriptStart();
    syFieldScriptEndLastingPsynergy(0);
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsFelixYoureBack);
    fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    fdActorFaceActor(ACTOR_MAYOR, ACTOR_FELIX, 0);
    // "Felix! You're back!"
    syFieldScriptShowMessageThenWait(ACTOR_MAYOR, 0, 10);
    fdActorFaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
    fdActorFaceDirection(ACTOR_ELDERS_GRANDSON, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceActor(ACTOR_ELDERS_GRANDDAUGHTER, ACTOR_FELIX, 0);
    fdActorFaceActor(ACTOR_JEWEL_KEEPER, ACTOR_FELIX, 0);
    fdActorFaceActor(ACTOR_ELDERS_DAUGHTER, ACTOR_FELIX, 0);
    fdActorFaceActor(ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, ACTOR_FELIX, 0);
    syFieldScriptWait(20);
    fdActorWalkToAndWait(ACTOR_FELIX, 701, 93);
    fdActorWalkBy(ACTOR_FELIX, -32, 0);
    scFieldActorLaunchFromLeader(ACTOR_JENNA, -8, 32, FACING_WEST);
    scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_SOUTH_SOUTHWEST);
    scFieldActorLaunchFromLeader(ACTOR_SHEBA, -8, 16, FACING_SOUTHWEST);
    scFieldActorLaunchFromLeader(ACTOR_PIERS, -24, 16, FACING_SOUTHWEST);
    syFieldScriptWait(30);
    fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "And you've brought Piers with you!"
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
    fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    syFieldScriptWait(20);
    fdActorFaceDirection(ACTOR_ELDER, FACING_EAST, 0);
    syFieldScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_ELDER, 2);
    syFieldScriptWait(20);
    // "Oh! Piers!"
    syFieldScriptShowMessageThenWait(ACTOR_ELDER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Piers, I must apologize for the trouble we've caused you. I'm sorry..."
    syFieldScriptShowMessageThenWait(ACTOR_ELDER, 0, 10);
    fdActorFaceEachOther(ACTOR_PIERS, ACTOR_FELIX, 0);
    syFieldScriptWait(30);
    fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
    syFieldScriptWait(20);
    // "Don't be. Everything worked out for the best."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    scFieldActorJump(ACTOR_ELDERS_GRANDDAUGHTER, 6, 20);
    syFieldScriptWait(20);
    // "Piers, you said you were going to go to Gondowan. Did you make it?"
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH_SOUTHEAST);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    syFieldScriptWait(20);
    // "Yes. In fact, we have just returned."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "And your jewel..." Yes: "You actually got it back from the Kibombo? Incredible..." No: "You
    // didn't recover it from the Kibombo?"
    fdScriptOpenMessage(ACTOR_MAYOR, 0);
    if (syFieldScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0)
    {
        fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
        // "You actually got it back from the Kibombo? Incredible..."
        syFieldScriptShowMessageThenWait(ACTOR_ELDER, 0, 10);
        fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
        fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
        syFieldScriptWait(20);
        // "I was able to recover it with the help of Felix and his friends."
        syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
        fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
        // "That's wonderful! I'm so relieved! I was worried that you might..."
        syFieldScriptShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
        FieldStateWork->msg += 3;
    }
    else
    {
        fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
        scMadraRoomsCountAnswer();
        FieldStateWork->msg += 3;
        // "You didn't recover it from the Kibombo?"
        syFieldScriptShowMessageThenWait(ACTOR_ELDER, 0, 10);
        fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
        fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
        syFieldScriptWait(20);
        // "That's not true, sir. Felix and his companions helped me recover it."
        syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
        scFieldActorStartRepeatedMotion(ACTOR_ELDERS_DAUGHTER, 3);
        fdActorSetAttachedEffect(ACTOR_ELDERS_DAUGHTER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        syFieldScriptWait(50);
        // "Please! Don't startle me like that! I was afraid you might never..."
        syFieldScriptShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
    }
    fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(20);
    // "That's why we came back... To let you know that we've recovered it..."
    syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    if (fdScriptGetFlag(FLAG_PIERS_SETTING_SAIL) != 0)
        scMadraRoomsSayFarewellAgain();
    else
        scMadraRoomsAskToStayLonger();
    fdActorFaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
    fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_SOUTH);
    syFieldScriptWait(20);
    fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    syFieldScriptWait(40);
    fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
    syFieldScriptWait(40);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
    fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    syFieldScriptWait(20);
    scMadraRoomsWalkIntoLeader(ACTOR_KRADEN);
    scMadraRoomsWalkIntoLeader(ACTOR_SHEBA);
    scMadraRoomsWalkIntoLeader(ACTOR_PIERS);
    scMadraRoomsWalkIntoLeader(ACTOR_JENNA);
    fdScriptSetFlag(FLAG_MADRA_FAREWELL_SAID);
    fdScriptEnd();
}

// The welcome's end when Piers has already agreed to sail for Lemuria: the household asks "Ah, so
// you'll be leaving again, will you?"
void scMadraRoomsSayFarewellAgain(void)
{
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsAhSoYoullBeLeaving);
    // "Ah, so you'll be leaving again, will you?"
    syFieldScriptShowMessageThenWait(ACTOR_MAYOR, 0, 10);
    fdActorFaceEachOther(ACTOR_FELIX, ACTOR_KRADEN, 0);
    syFieldScriptWait(40);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    syFieldScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "Are you sure you must leave so soon, Piers?"
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    syFieldScriptWait(20);
    // "I may yet return here again, but I'm afraid I must be going soon."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "You seem to be quite busy."
    syFieldScriptShowMessageThenWait(ACTOR_ELDER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(20);
    // "It is their youth that allows them to be so busy."
    syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
    syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
    syFieldScriptWait(20);
    // "I'd like that..."
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, ANIM_NOD);
    syFieldScriptWait(20);
    // "So would I!"
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
}

// The welcome's end when Piers has not yet agreed to sail: the household asks the party to stay a
// little longer now that the jewel is recovered, and Piers explains why he cannot.
void scMadraRoomsAskToStayLonger(void)
{
    syFieldScriptSetMessageId((Sint32)MsgMadraRoomsIfYouveAlreadyRecovered);
    fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
    syFieldScriptWait(20);
    // "If you've already recovered your jewel, why don't you stay here for..."
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
    syFieldScriptWait(20);
    // "Unfortunately, we can't..."
    syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
    fdActorFaceActor(ACTOR_SHEBA, ACTOR_JENNA, 0);
    syFieldScriptWait(20);
    // "Hey, have you seen the girl out front?"
    syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTH);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_ELDERS_GRANDSON, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_ELDERS_GRANDSON, 0);
    fdActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    // "We must go...to the island that is my home."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    fdActorFaceActor(ACTOR_KRADEN, ACTOR_PIERS, 0);
    // "Piers, are you... Are you sure?"
    syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
    syFieldScriptWait(60);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    syFieldScriptWait(20);
    // "After what we've been through, I feel I've learned a few things."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorFaceActor(ACTOR_FELIX, ACTOR_PIERS, 0);
    fdActorFaceActor(ACTOR_SHEBA, ACTOR_PIERS, 0);
    fdActorFaceActor(ACTOR_JENNA, ACTOR_PIERS, 0);
    syFieldScriptWait(20);
    fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    // "Like what!?"
    syFieldScriptShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_SHEBA, 0);
    syFieldScriptWait(20);
    // "Like...the fact that you truly are good people."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
    syFieldScriptWait(20);
    // "And that you have gone to great lengths for no other reason than to..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    syFieldScriptWait(20);
    // "I have come to realize that I, too, must do something to help where..."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
    // "Piers..."
    syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
    syFieldScriptWait(20);
    fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHEAST);
    // "I have made my decision."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_KRADEN, 0);
    syFieldScriptWait(20);
    // "And the sooner we depart, the better, right, Kraden?"
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorFaceActor(ACTOR_PIERS, ACTOR_FELIX, 0);
    syFieldScriptWait(20);
    // "We shall set sail, Felix! To my ship! To the Eastern Sea!"
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdScriptSetFlag(FLAG_PIERS_SETTING_SAIL);
    fdActorShowEmote(ACTOR_ELDERS_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    // "Do you truly mean to leave so soon, Piers?"
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_DAUGHTER, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
    fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
    syFieldScriptWait(20);
    // "I may yet again have reason to visit your town, but... yes, I must go."
    syFieldScriptShowMessageThenWait(ACTOR_PIERS, 0, 10);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHWEST, 0);
    fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
    // "You seem quite busy."
    syFieldScriptShowMessageThenWait(ACTOR_ELDER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    syFieldScriptWait(20);
    // "It is their youth that allows them to be so busy."
    syFieldScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
    fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    // "But we'll be back, I promise. Maybe we'll be able to visit at a more..."
    syFieldScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDSON, ANIM_NOD);
    syFieldScriptWait(20);
    // "I'd like that..."
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_GRANDSON, 0, 10);
    fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    syFieldScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_ELDERS_GRANDDAUGHTER, ANIM_NOD);
    syFieldScriptWait(20);
    // "So would I!"
    syFieldScriptShowMessageThenWait(ACTOR_ELDERS_GRANDDAUGHTER, 0, 10);
}

// clang-format off
// The old couple's walks into their back room and out again, Char hopping after them, as they fetch the Djinni for the Healing Fungus.
Sint32 MadraRoomsWifeWalksToBackRoom[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(356, 72),
    SC_SYNC,
    SC_MOVE(324, 0, 73),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

// Felix's script as the old man passes: one step aside.
Sint32 MadraRoomsLeaderStepsAside[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(339, 111),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_END,
};

// The same script as MadraRoomsWifeWalksToBackRoom, under another name. The mushroom lover's script off to the back room to fetch Char.
Sint32 MadraRoomsOldManWalksToBackRoom[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_XZ(356, 72),
    SC_SYNC,
    SC_MOVE(324, 0, 73),
    SC_SYNC,
    SC_POSITION_ORIGIN,
    SC_END,
};

// The mushroom cook's script back to her place.
Sint32 MadraRoomsWifeComesBack[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 0, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 109),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

// The mushroom lover's script back from the back room with Char.
Sint32 MadraRoomsOldManComesBack[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 0, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 97),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

// Char's script once the old man gives him to the party: he follows Felix.
Sint32 MadraRoomsCharFollows[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_POSITION(327, 24, 71),
    SC_SET(SW_FACING, FACING_EAST),
    SC_MOVE(352, 0, 72),
    SC_SYNC,
    SC_MOVE_XZ(373, 97),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
    SCENE_EXITS(MadraRoomsHouses),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTH_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, MadraRoomsHouses, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, MadraRoomsHouses, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, MadraRoomsHouses, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, MadraRoomsHouses, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_SOUTH_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHEAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, MadraRoomsHouses, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, MadraRoomsHouses, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, MadraRoomsHouses, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, MadraRoomsHouses, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_NORTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_HOUSES_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_ROOMS_HOUSES_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS(MadraRoomsCavernHomes),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST, Madra, MADRA_FROM_MADRA_ROOMS_CAVERN_HOMES_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST, Madra, MADRA_FROM_MADRA_ROOMS_CAVERN_HOMES_WEST, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// Madra's houses, shops, inn and sanctum until the party returns with Piers (MADRA_ROOMS_ACTOR_ID, then MADRA_ROOMS_TRAVELERS_ACTOR_ID).
ACTOR_PLACE MadraRoomsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 615, 0, 311, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 364, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 348, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 556, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CHAR), MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 847, 0, 711, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 906, 0, 760, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

// Madra's houses once the party returns with Piers (enum MADRA_ROOMS_ACTOR_ID, then MADRA_ROOMS_RETURN_ACTOR_ID).
ACTOR_PLACE MadraRoomsPlacementsOnReturn[] = {
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 184, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 112, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 109, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 96, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 296, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 88, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 640, 0, 152, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 620, 0, 152, FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 152, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 552, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 624, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 648, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 604, 0, 548, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 432, 0, 616, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 518, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CHAR), MOVEMENT_TYPE_WANDER, 368, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 614, 0, 111, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 636, 0, 111, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1288, 0, 8, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 676, 0, 762, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 730, 0, 712, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

// The jail (MADRA_ROOMS_JAIL_ACTOR_ID).
ACTOR_PLACE MadraRoomsJailPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 522, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 840, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, FLAG_BRIGGS_CAUGHT, MOVEMENT_TYPE_STAND, 504, 0, 808, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 362, 0, 847, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 394, 0, 863, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MOVEMENT_TYPE_STAND, 396, 0, 848, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The left cavern home (MADRA_ROOMS_CAVERN_HOME_ACTOR_ID).
ACTOR_PLACE MadraRoomsSecondRoomsPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 128, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 192, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 356, 0, 109, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, IF_FLAG_SET(FLAG_BRIGGS_CAUGHT), MOVEMENT_TYPE_STAND, 357, 0, 131, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

// The houses' talk and Mind Read before the Kibombo raid. The exits without a name lead from one of the map's rooms to another, 3 to 4, 6 to 7, 11 to 12 and 13 to 14 and back; 16 and 17 have no row in SceneExits.
CUE MadraRoomsCuesBeforeRaid[] = {
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT, 13, CONDITION_ALWAYS, 13},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {CUE_EXIT, 16, CONDITION_ALWAYS, 16},
    {CUE_EXIT, 17, CONDITION_ALWAYS, 17},
    {CUE_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsLandsToSouthOfGondowan)},
    {CUE_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsAskWillWeBeSafe)},
    {CUE_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWaveBigEnoughToMove)},
    {TALK_CUE_FACING(FACING_NORTH), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEverythingHappenedSoSuddenlyEarth)},
    {CUE_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheySayThatBoatBelongs)},
    {CUE_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsBoatThatRanAground)},
    {CUE_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMiladyAndIHaveBeen)},
    {CUE_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherWasOpposed)},
    {CUE_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenElderGetsBack)},
    {CUE_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsDadAndGrandpaHaveGone)},
    {CUE_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWheneverDadGoesOnTrips)},
    {CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToHealer)},
    {CUE_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeMustPassThisTest)},
    {CUE_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWaveThatMovesVeryEarth)},
    {CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToInnkeeper)},
    {CUE_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurInnIsQuiteSafe)},
    {CUE_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeThoseChampaDont)},
    {CUE_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsGuardElderGem)},
    {CUE_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsISleptRightThroughPirate)},
    {CUE_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImGladMyPartner)},
    {CUE_TALK, ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWereHeadingToTolbi)},
    {CUE_TALK, ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHeyIfYouSeeGladiator)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAreWarlikeButThey)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheySayThereArePeople)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfMythicalSeaCreatureHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEverythingsOnFloorAndAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThatBoatsFarSturdier)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsItDidntEvenHaveAny)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWishIKnewWhy)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsUntilMyFatherReturnsIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHeIntendsToFindBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfDadHasToFight)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeHeBringsMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHowCouldOneExplainContinent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfThisIsTestPlease)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsRaidWasJustAnotherPart)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsNewLandBridgesBetweenContinents)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsActuallyIThink)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsNoOneWasSafe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWasHidingUnder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeWereFineBecauseOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MORGAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsGladiatorWhoTakesPrize)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DEKKA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeFoughtInColossoLast)},
    {EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_CUE(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {CUE_TABLE_END},
};

// The jail's talk and Mind Read, by the story flags.
CUE MadraRoomsJailCues[] = {
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_WEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_WEST},
    {CUE_TALK, ACTOR_SHIN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsOhPiersAfterWay)},
    {CUE_TALK, ACTOR_SHINS_FRIEND, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsShinsDevelopedTerribleFear)},
    {TALK_CUE_FACING(FACING_NORTH), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_CUE_FACING(FACING_NORTHWEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {TALK_CUE_FACING(FACING_WEST), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgWhenAndHis)},
    {CUE_TALK, ACTOR_MAN_FOCUSING_ON_ICE, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), SCRIPT_REF(scMadraRoomsTalkToManFocusingOnIce)},
    {CUE_TALK, ACTOR_SHIN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersWasntOneOfChampa)},
    {CUE_TALK, ACTOR_SHINS_FRIEND, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSoThatPiersGuyWasnt)},
    {CUE_TALK, ACTOR_PIERS, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToPiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIN, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsEvenSoundOfNamePiers)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHINS_FRIEND, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgMadraRoomsIUnderstandWhyShinsScared)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS_ON_ISAACS_PARTY, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), MSGNO(MsgSoThisFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FOCUSING_ON_ICE, IF_FLAG_SET(FLAG_MADRA_FOCUSING_MAN_GROANED), MSGNO(MsgMadraIKnewIShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_FOCUSING_ON_ICE | CUE_SUBJECT_OWN_EFFECT, IF_FLAG_SET(FLAG_GABOMBA_MESSAGE_CARRIED), SCRIPT_REF(scMadraRoomsTalkToManFocusingOnIce)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThereWasSomethingReallyFrightening)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHINS_FRIEND, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThatPiersWasStrangeFellow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIERS, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsMindReadPiers)},
    {CUE_TABLE_END},
};

// The left cavern home's talk and Mind Read, by the story flags.
CUE MadraRoomsSecondRoomsCues[] = {
    {CUE_EXIT, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST, CONDITION_ALWAYS, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_EAST},
    {CUE_EXIT, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST, CONDITION_ALWAYS, MADRA_ROOMS_CAVERN_HOMES_TO_MADRA_WEST},
    {CUE_TALK, ACTOR_MAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsInAllThisChaosEveryone)},
    {CUE_TALK, ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsSoBriggsAndHisPirates)},
    {CUE_TALK, ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheresTalkOfMightyTravelers)},
    {CUE_TALK, ACTOR_WOMAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsMadraAndAlhafraOff)},
    {CUE_TALK, ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsItWasSureSneaky)},
    {CUE_TALK, ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSinceThingsAreFinallyCalming)},
    {CUE_TALK, ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOhhOhhh)},
    {CUE_TALK, ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsImSoHappy)},
    {CUE_TALK, ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveToldMySisterNot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsWaitSecondIllBetBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsNowThatBriggsHasBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhatIfWeJustTraveled)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, FLAG_BRIGGS_CAUGHT, MSGNO(MsgMadraRoomsAlhafraHasLongBeenTrade)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsItSoundsLikeOsenia)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_ALHAFRA, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsBusinessPlanOurPeopleHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SISTER_IN_BED, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEyesShiningInDarknessNo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, FLAG_GABOMBA_MESSAGE_CARRIED, MSGNO(MsgMadraRoomsIWasSoScared)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HER_SISTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsShinsNormallySoKind)},
    {EXAMINE_CUE(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingTandooriChicken))},
    {CUE_TABLE_END},
};

// The houses' talk and Mind Read once the Kibombo have raided Madra.
CUE MadraRoomsCuesAfterRaid[] = {
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT, 13, CONDITION_ALWAYS, 13},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {CUE_EXIT, 16, CONDITION_ALWAYS, 16},
    {CUE_EXIT, 17, CONDITION_ALWAYS, 17},
    {CUE_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAreVeryWarlikeTribe)},
    {CUE_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImGladKibombosAttackWasnt)},
    {CUE_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboApparentlyUsedRopes)},
    {CUE_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyHusbandPursuedKibomboAfter)},
    {CUE_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersLeftThatBoat)},
    {CUE_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantStopThinkingAbout)},
    {CUE_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAllKibomboWantedWhenThey)},
    {CUE_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherWasHolding)},
    {CUE_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersWentToKibombo)},
    {CUE_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfMyFatherdBeen)},
    {CUE_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyFatherHasntComeHome)},
    {CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToHealer)},
    {CUE_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfThatTidalWaveHadnt)},
    {CUE_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThemKibomboPossessEvilPowers)},
    {CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToInnkeeper)},
    {CUE_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsThingsHaveBeenBusyHere)},
    {CUE_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHidUnderMyBed)},
    {CUE_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsGuardElderGem)},
    {CUE_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWatchedEntireAttack)},
    {CUE_TALK, ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfYouWantToKnow)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurBestGuess)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhatCanWeDo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfIWereJustLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsInHisYouthMyHusband)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSomePeopleInTownSaw)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWonderIfThatShip)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenMyHusbandHears)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWontEverForgetLook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenPiersLeftHeSaid)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsDadAlwaysKnowsWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopeHesJustLate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHopePowerOfOur)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsEveryoneSaysThemKibombo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWereLuckyKibomboDidntUse)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsTheresNothingWorseThanWicked)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfItWereMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAfterICrawledUnderBed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIHadToGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsKibomboAttackedInWeeHours)},
    {EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_CUE(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {CUE_TABLE_END},
};

// The houses' talk and Mind Read once the party returns with Piers, the welcome, and the mushroom trade.
CUE MadraRoomsCuesOnReturn[] = {
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHWEST},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_A},
    {CUE_EXIT, 3, CONDITION_ALWAYS, 3},
    {CUE_EXIT, 4, CONDITION_ALWAYS, 4},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHWEST},
    {CUE_EXIT, 6, CONDITION_ALWAYS, 6},
    {CUE_EXIT, 7, CONDITION_ALWAYS, 7},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTHEAST},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_SOUTH_B},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_A},
    {CUE_EXIT, 11, CONDITION_ALWAYS, 11},
    {CUE_EXIT, 12, CONDITION_ALWAYS, 12},
    {CUE_EXIT, 13, CONDITION_ALWAYS, 13},
    {CUE_EXIT, 14, CONDITION_ALWAYS, 14},
    {CUE_EXIT, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B, CONDITION_ALWAYS, MADRA_ROOMS_HOUSES_TO_MADRA_NORTHEAST_B},
    {CUE_EXIT, 16, CONDITION_ALWAYS, 16},
    {CUE_EXIT, 17, CONDITION_ALWAYS, 17},
    {CUE_STEP, METATILE_BEHAVIOR_MADRA_ROOMS_WELCOME, FLAG_MADRA_FAREWELL_SAID, SCRIPT_REF(scMadraRoomsWelcomePiersHome)},
    {CUE_TALK, ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurSalesmanSaidKibombosCeremony)},
    {CUE_TALK, ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWonderIfKibomboHave)},
    {CUE_TALK, ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToMushroomLover)},
    {CUE_TALK, ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToMushroomCook)},
    {CUE_TALK, ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSameSailorsCameThroughRecently)},
    {CUE_TALK, ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOneOfSailorsIsaac)},
    {CUE_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIKnowYouWent)},
    {CUE_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeJustHaveToFinish)},
    {CUE_TALK, ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImSoRelievedPiersGot)},
    {CUE_TALK, ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantBelievePiersActually)},
    {CUE_TALK, ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsGondowanIsSuchAwfulDangerous)},
    {CUE_TALK, ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsILikeYouGuysLot)},
    {CUE_TALK, ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsMyDaddyWentToGondowan)},
    {CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToHealer)},
    {CUE_TALK, ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOceansBeenWarmingUpLot)},
    {CUE_TALK, ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhenOceanWarmsEasternSea)},
    {CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsTalkToInnkeeper)},
    {CUE_TALK, ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHasSomeoneBeenTelling)},
    {CUE_TALK, ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOneOfOurGuestsKept)},
    {CUE_TALK, ACTOR_BLACK_CRYSTAL, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsGuardElderGem)},
    {CUE_TALK, ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIGuessThatLot)},
    {CUE_TALK, ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveHeardThatRisingOcean)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsContinentShiftMakesItFeel)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WARY_OF_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSalesmanThatJustCameBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_LOVER, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsMindReadMushroomLover)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSHROOM_COOK, CONDITION_ALWAYS, SCRIPT_REF(scMadraRoomsMindReadMushroomCook)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_STRANGE_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOarsIUnderstandHeyIll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_GROUNDED_SHIP, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWeveHadLotOfStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsAllOfThisCouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsWhyWontMayorOfAlhafra)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JEWEL_KEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsICantBelieveItDespite)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIFeltLittleResponsible)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_ELDERS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsPiersSaidHeWasGoing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDSON, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIWishTheydTellMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDERS_GRANDDAUGHTER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsImSoGladDaddyGot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIKnewTemperaturesWereRising)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsOurNewShipIsSpectacular)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_OF_THE_SANCTUM, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSeafaringTownsKnowManyMore)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIveNeverSeenSingleCustomer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_HELPER, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsSomeoneMustHaveSeenMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_STAYING_AT_THE_INN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsItMustBeVeryStrange)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WHO_SLEPT_THROUGH_THE_RAID_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsIfIHadBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELERS_PARTNER_ON_RETURN, CONDITION_ALWAYS, MSGNO(MsgMadraRoomsHmIWonderIfTidal)},
    {USE_ITEM_ON_ACTOR(ITEM_HEALING_FUNGUS), ACTOR_MUSHROOM_LOVER, FLAG_MADRA_HEALING_FUNGUS_GIVEN, SCRIPT_REF(scMadraRoomsGiveHealingFungus)},
    {USE_ITEM_ON_ACTOR(ITEM_LAUGHING_FUNGUS), ACTOR_MUSHROOM_LOVER, FLAG_MADRA_HEALING_FUNGUS_GIVEN, SCRIPT_REF(scMadraRoomsGiveLaughingFungus)},
    {EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsThereAreSomePotatoesBoiling))},
    {EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheyreMakingNanItLooks))},
    {EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsAllSpicesAreNeatlyArranged))},
    {EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsTheresChickenCurrySlowlySimmering))},
    {EXAMINE_CUE(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgMadraRoomsPotOfMasalaTea))},
    {CUE_TABLE_END},
};

// Shin and then his friend run out of the jail, crying for help.
Sint32 MadraRoomsShinAndFriendRunAway[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(408, 0, 872),
    SC_SYNC,
    SC_MOVE(408, 0, 904),
    SC_SYNC,
    SC_MOVE(408, 0, 920),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

// Shin falls over, a quarter turn in eight ticks.
Sint32 MadraRoomsShinFalls[] = {
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_CALL(scMadraRoomsRotateSprite),
    SC_WAIT(1),
    SC_END,
};

// Piers' shaft of ice lifts Shin and holds him bobbing in the air.
Sint32 MadraRoomsShinLifted[] = {
    SC_SET(SW_MOTION, 0),
    SC_WAIT(100),
    SC_WAIT(36),
    SC_ADD(SW_Y, toFIXED(3)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(5)),
    SC_ADD(SW_X, toFIXED(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(9),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ADD(SW_X, toFIXED(1)),
    SC_ADD(SW_Y, toFIXED(5)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_ADD(SW_X, toFIXED(1)),
    SC_WAIT(9),
    SC_ADD(SW_Y, toFIXED(4)),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ADD(SW_X, toFIXED(1)),
    SC_LABEL(1),
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_ADD(SW_Y, toFIXED(-1)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
    SC_WAIT(2),
    SC_ADD(SW_Y, toFIXED(1)),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(4),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
// clang-format on

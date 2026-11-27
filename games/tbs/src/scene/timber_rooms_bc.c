// Lalivero's rooms are two rooms maps drawn in timber in one overlay, Timber rooms B and C.
//
// The game packs rooms that share a tile style onto one map, and every room on these two is
// Lalivero's, entered from the town.
//
// Their area name is Lalivero, and Lalivero Sanctum for the Sanctum's rooms of TimberRoomsB
// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST and TIMBER_ROOMS_B_SANCTUM).
//
// Each scene has tables for before and after Venus Lighthouse, chosen by
// FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, as the town's are (lalivero.c).
//
// Timber rooms B (SceneIdTimberRoomsB) holds, by the exits that lead in, a house
// (TIMBER_ROOMS_B_FROM_LALIVERO_WEST), a house (TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST), the item
// shop with a room upstairs (TIMBER_ROOMS_B_FROM_LALIVERO_NORTH), a house with an upper floor
// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST), the weapon and armor shop, entered by either of its two
// doors (TIMBER_ROOMS_B_FROM_LALIVERO_SOUTH and TIMBER_ROOMS_B_FROM_LALIVERO_EAST_SOUTHEAST), and
// the Sanctum (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST). Each shopkeeper stands behind a counter and
// trades only with a leader facing north across it (scTimberRoomsBCTalkToWeaponMerchant and the
// others). From any other side they talk. Stepping into the Sanctum by TIMBER_ROOMS_B_SANCTUM makes
// it the place a defeated party wakes. Barrels hold a Lucky Medal and an Antidote.
//
// Timber rooms C (SceneIdTimberRoomsC) holds, by the exits that lead in, Faran's house
// (TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST and TIMBER_ROOMS_C_FROM_LALIVERO_SOUTH), the inn with its
// upper floor and two guest rooms (TIMBER_ROOMS_C_FROM_LALIVERO_NORTHEAST), and the way out to the
// wharf where Babi's ship lies (TIMBER_ROOMS_C_FROM_LALIVERO_NORTHWEST, leading on by
// TIMBER_ROOMS_C_TO_LEMURIAN_SHIP). Before Venus Lighthouse an inn worker apologizes once for her
// distraction (scTimberRoomsBCTalkToInnWorker, FLAG_LALIVERO_INN_WORKER_APOLOGIZED) and goes back
// to her chores. The aerie of Venus Lighthouse leads in by
// TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE, which plays scTimberRoomsBCReportToFaran. There the
// party reports to Faran and Iodem, in one of two versions by FLAG_SAW_SHIP_AT_IDEJIMA. Afterwards
// Iodem calls the party over (scTimberRoomsBCIodemCallsOut, FLAG_IODEM_ASKED_TO_MEET). That sends
// the party outside, where Iodem gives it the Black Orb (lalivero.c). A barrel holds a Lucky Medal.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern Sint32            TimberRoomsBCInnWorkerChores[];
extern SCENE_ENTRANCE    TimberRoomsBCEntrances[];
extern SCENE_EXIT_REGION TimberRoomsBRegions[];
extern SCENE_EXIT_REGION TimberRoomsCRegions[];
extern Uint32            TimberRoomsBCExits[];
extern ACTOR_PLACE       TimberRoomsBActorPlacements[];
extern ACTOR_PLACE       TimberRoomsCActorPlacements[];
extern ACTOR_PLACE       TimberRoomsCActorPlacementsAfterLighthouse[];
extern CUE               TimberRoomsBCues[];
extern CUE               TimberRoomsBCuesAfterLighthouse[];
extern CUE               TimberRoomsCCues[];
extern CUE               TimberRoomsCCuesAfterLighthouse[];

static void              scTimberRoomsBCTellFaranAboutJenna(void);

// An entrance of either scene that sets FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, as after Venus
// Lighthouse, without the report to Faran that TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE plays.
//
// No exit leads to it and the entrance table has no row for it. Only the debug scene selector,
// which exits to any entrance from 0 to 99, reaches it.
enum
{
    TIMBER_ROOMS_ENTRANCE_AFTER_LIGHTHOUSE = 90
};

// The shops' stock rows, whose type column in StockTable says they sell weapons, armor and items,
// and the inn.
enum
{
    LALIVERO_WEAPON_STOCK = 32,
    LALIVERO_ARMOR_STOCK  = 33,
    LALIVERO_ITEM_STOCK   = 34,
    LALIVERO_INN          = 11
};

// The actors of both scenes, numbered by the rows of each scene's actor placement tables.
//
// The people are named by what they say before Venus Lighthouse and by what their sprites show.
typedef enum TIMBER_ROOMS_BC_ACTOR_ID
{
    // The houses, the shops and the Sanctum (TimberRoomsB). A young man with brown hair in a purple
    // shirt, in the house at TIMBER_ROOMS_B_FROM_LALIVERO_WEST.
    //
    // He says "Of course I have seen Sheba's power... It's incredible!"
    ACTOR_VILLAGER_SAW_SHEBAS_POWER = ACTOR_FIRST_PLACED,
    // A woman with fair hair, in the same house.
    //
    // She says "There are some who do not believe Sheba has any powers... Doubting Sheba? That is
    // foolish!"
    ACTOR_WOMAN_DEFENDING_SHEBA,
    // A man in a white turban, in the house at TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST.
    //
    // He says "Could Faran have tricked us all?"
    ACTOR_VILLAGER_DOUBTING_FARAN,
    // A woman in a green dress, in the same house.
    //
    // She says "If Sheba can't use her powers in a time like this, they're useless!"
    ACTOR_WOMAN_DOUBTING_SHEBA,
    // An old man with a cane, upstairs in the item shop at TIMBER_ROOMS_B_FROM_LALIVERO_NORTH.
    //
    // He says "Sheba came flying to the ruins like a shooting star."
    ACTOR_OLD_MAN_SHOOTING_STAR,
    // An old woman, in the same room.
    //
    // She says "Even Sheba couldn't have escaped six of them..."
    ACTOR_OLD_WOMAN_SIX_OF_THEM,
    // A bald, muscular man, in the house at TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST.
    //
    // He says "They say a man named Iodem is coming from Tolbi. I guess he plans to make us do even
    // more work!"
    ACTOR_WORKER,
    // A bald, muscular man lying asleep upstairs in that house.
    //
    // He says "No one's called me, so maybe I'll take a nap..." After Venus Lighthouse, Mind Read
    // hears "I'm worried that we won't get paid now that the construction has ended."
    ACTOR_NAPPING_WORKER,
    // A strongman, in the house at TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST.
    //
    // He says "Sheba said the cave at the ruins leads to the southeast."
    ACTOR_STRONGMAN_RUINS_CAVE,
    // A strongman who walks about the room upstairs in that house.
    //
    // He says "The ruins that form the lighthouse foundation are riddled with caves."
    ACTOR_STRONGMAN_RUINS_RIDDLE,
    ACTOR_WEAPON_MERCHANT,
    ACTOR_ARMOR_MERCHANT,
    ACTOR_ITEM_MERCHANT,
    // The Sanctum's healer.
    ACTOR_HEALER,

    // Faran's house and the inn (TimberRoomsC). Faran's wife, who raised Sheba.
    //
    // She says "We received Sheba from the sky and raised her lovingly... I know Faran will rescue
    // that child."
    ACTOR_FARANS_WIFE = ACTOR_FIRST_PLACED,
    // Faran's son, a small boy with fair hair, younger than Sheba.
    //
    // He says "I just know that Father will save my elder sister, Sheba."
    ACTOR_FARANS_SON,
    // A young woman with long brown hair, worried for Sheba.
    //
    // She says "His wife seems very calm, considering." Mind Read hears "When someone like Sheba is
    // kidnapped, it worries us all!"
    ACTOR_WORRIED_YOUNG_WOMAN,
    // Placed off the map before Venus Lighthouse.
    ACTOR_FARAN,
    // Placed off the map before Venus Lighthouse.
    ACTOR_IODEM,
    // A man with dark hair.
    ACTOR_INNKEEPER,
    // A woman in a pink dress and a white apron who works about the inn, too worried about Sheba to
    // keep her mind on it.
    ACTOR_INN_WORKER,
    // A young man with brown hair in a purple shirt who walks about the hall upstairs in the inn.
    //
    // After Venus Lighthouse he says "Fishing is so difficult nowadays because the ocean is so
    // rough. We have little to cook when we can't fish."
    ACTOR_COOK
} TIMBER_ROOMS_BC_ACTOR_ID;

// The napping worker's pose, from his sprite's animation table
// (graphics/sprite/bald_muscular_man.tsv).
//
// It is one frame held, lying asleep on his side.
enum
{
    ANIM_WORKER_LIE_ASLEEP = 6
};

// The metatiles where, after the report, Iodem calls the party over. The doorways' metatile
// behaviors are the numbers of the exits they lead to.
enum
{
    METATILE_BEHAVIOR_TIMBER_ROOMS_C_IODEM_CALLS_OUT = 51
};

// The inn worker has apologized.
//
// It is the area temp flag FLAG_AREA_TEMP_00, which lasts until the party enters another area.
enum
{
    FLAG_LALIVERO_INN_WORKER_APOLOGIZED = FLAG_AREA_TEMP_00
};

// In the inn (TimberRoomsC), the inn worker sighs between chores.
static Sint32 scTimberRoomsBCInnWorkerSighs(ACTOR *actor)
{
    fdActorSetAttachedEffect(ACTOR_INN_WORKER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return TimberRoomsBCEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    if (MainRoutineStateWork.next == SCENE_TIMBER_ROOMS_B)
    {
        return TimberRoomsBRegions;
    }
    return TimberRoomsCRegions;
}

const Uint32 *scGetExits(void)
{
    return TimberRoomsBCExits;
}

// The people of each scene.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_TIMBER_ROOMS_C)
    {
        // Faran's house and the inn.
        if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0)
        {
            return TimberRoomsCActorPlacementsAfterLighthouse;
        }
        return TimberRoomsCActorPlacements;
    }
    return TimberRoomsBActorPlacements;
}

// The side the leader faces, which is the facing rounded to the nearest of east, south, west and
// north.
static inline Uint16 scTimberRoomsBCLeaderGetSide(void)
{
    return FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
}

// The weapon merchant sells weapons, or talks of Sheba and the sea.
static void scTimberRoomsBCTalkToWeaponMerchant(Sint32 keeper)
{
    ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
    // The side of the counter the leader faces, as scTimberRoomsBCLeaderGetSide rounds it.
    Uint16 side   = FACING_NEAREST_CARDINAL(leader->facing);

    if (side == FACING_NORTH)
    {
        mnShopRun(LALIVERO_WEAPON_STOCK, keeper);
    }
    else if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED))
    {
        fdScriptSetMessageId(MsgLaliveroWentOceanLook);
        // "I went to the ocean to look for Sheba today, but it was so..."
        fdScriptShowMessage(keeper, 0);
    }
    else
    {
        Sint32 message = MsgLaliveroTakenHostageFaran;

        fdScriptSetMessageId(message);
        // "Sheba was taken hostage, and Faran went after them?"
        fdScriptOpenMessage(keeper, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptWait(10);
            // Yes: "Oh my! What's everyone else in Lalivero doing?..."
            fdScriptSetMessageId(message + 1);
        }
        else
        {
            // No: "What are you, a shut-in? That's all anyone has been talking about!"
            fdScriptSetMessageId(message + 2);
        }
        fdScriptShowMessage(keeper, 0);
    }
}

// In Lalivero (TimberRoomsB), the armor merchant opens the armor shop across the counter, and
// otherwise makes small talk.
static void scTimberRoomsBCTalkToArmorMerchant(Sint32 keeper)
{
    if (scTimberRoomsBCLeaderGetSide() == FACING_NORTH)
    {
        mnShopRun(LALIVERO_ARMOR_STOCK, keeper);
    }
    else if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED))
    {
        fdScriptSetMessageId(MsgLaliveroWantedSendShips);
        // "We wanted to send ships out to sea to find Sheba, but we..."
        fdScriptShowMessage(keeper, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgLaliveroWarriorRightPlease);
        // "You're a warrior, right? Please rescue Sheba! I'll give you..."
        fdScriptShowMessage(keeper, 0);
    }
}

// In Lalivero (TimberRoomsB), the item merchant opens the item shop across the counter, and
// otherwise makes small talk.
static void scTimberRoomsBCTalkToItemMerchant(Sint32 keeper)
{
    if (scTimberRoomsBCLeaderGetSide() == FACING_NORTH)
    {
        mnShopRun(LALIVERO_ITEM_STOCK, keeper);
    }
    else if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED))
    {
        fdScriptSetMessageId(MsgLaliveroImpossibleGoOcean);
        // "It is impossible to go out on the ocean now. Be careful when..."
        fdScriptShowMessage(keeper, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgLaliveroGuessPeopleAngara);
        // "I guess it was people from Angara who took Sheba hostage...."
        fdScriptShowMessage(keeper, 0);
    }
}

// In Lalivero Sanctum, the healer offers the Sanctum's services across the counter, and otherwise
// makes small talk of Babi's lighthouse.
static void scTimberRoomsBCTalkToHealer(Sint32 healer)
{
    if (scTimberRoomsBCLeaderGetSide() == FACING_NORTH)
    {
        mnSanctumRun(healer);
    }
    else if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED))
    {
        fdScriptSetMessageId(MsgLaliveroBabiLighthouseStruck);
        // "Babi Lighthouse was struck down, just as I knew it would be."
        fdScriptShowMessage(healer, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgLaliveroBabiIsntBuilding);
        // "Babi isn't building a lighthouse-it's a tower... Maybe he is..."
        fdScriptShowMessage(healer, 0);
    }
}

// In Lalivero (TimberRoomsC), the innkeeper of the inn offers a room for the night across the
// counter, and otherwise makes small talk.
static void scTimberRoomsBCTalkToInnkeeper(Sint32 innkeeper)
{
    if (scTimberRoomsBCLeaderGetSide() == FACING_NORTH)
    {
        mnInnCheckIn(LALIVERO_INN, innkeeper);
    }
    else if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED))
    {
        fdScriptSetMessageId(MsgLaliveroDidntKnowTime);
        // "I didn't know it at the time, but that loud noise was the..."
        fdScriptShowMessage(innkeeper, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgLaliveroWantStayLet);
        // "If you want to stay, I'll let you, because it means money..."
        fdScriptShowMessage(innkeeper, 0);
    }
}

// In Lalivero (TimberRoomsC), before Venus Lighthouse, the inn worker stops her chores to
// apologize.
static void scTimberRoomsBCTalkToInnWorker(void)
{
    Sint16 side;

    side = scTimberRoomsBCLeaderGetSide();
    gmFlagSet(FLAG_LALIVERO_INN_WORKER_APOLOGIZED);
    fdScriptStart();
    fdEndBusyAndReveal();
    fdScriptSetMessageId(MsgLaliveroImSorry);
    fdScriptWait(50);
    fdActorShowEmote(ACTOR_INN_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    fdActorFaceActor(ACTOR_INN_WORKER, ACTOR_PARTY_LEADER, 20);
    // "Oh, I'm sorry..."
    fdScriptShowMessage(ACTOR_INN_WORKER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_INN_WORKER, ANIM_SHAKE_HEAD);
    fdScriptWait(30);
    // "I'm so worried about Sheba, I can't concentrate."
    fdScriptShowMessage(ACTOR_INN_WORKER, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_INN_WORKER, 2);
    fdScriptWait(30);
    // "But no matter how much I worry, there isn't anything I can do."
    fdScriptShowMessage(ACTOR_INN_WORKER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_INN_WORKER, ANIM_NOD);
    fdScriptWait(20);
    // "...Now, to work, to work!"
    fdScriptShowMessage(ACTOR_INN_WORKER, 0);
    if ((Uint16)side == FACING_WEST)
    {
        fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdScriptWait(20);
    }
    fdActorSetScript(ACTOR_INN_WORKER, TimberRoomsBCInnWorkerChores);
    fdScriptEnd();
}

// In Lalivero (TimberRoomsC), the camera finds Iodem, who asks the party to wait for him outside.
static void scTimberRoomsBCIodemCallsOut(void)
{
    gmFlagSet(FLAG_IODEM_ASKED_TO_MEET);
    fdScriptStart();
    fdEndBusyAndReveal();
    fdScriptWait(10);
    fdCameraMoveTo(120, CAMERA_KEEP, 96, 1);
    fdCameraWaitForMove();
    fdScriptWait(30);
    fdScriptSetMessageId(MsgLaliveroPleaseWaitForMeOutside);
    // "Please wait for me outside Faran's house. I wish to speak..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(30);
    fdScriptEnd();
}

// In Lalivero (TimberRoomsC), Iodem, talked to before he has given the Black Orb, repeats his
// request.
static void scTimberRoomsBCTalkToIodem(void)
{
    gmFlagSet(FLAG_IODEM_ASKED_TO_MEET);
    fdScriptSetMessageId(MsgLaliveroPleaseWaitForMeOutside);
    // "Please wait for me outside Faran's house. I wish to speak..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
}

#include "shared/leave_by_exit.c"

// What the party can talk to, touch and search in each scene, before and after Venus Lighthouse.
const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_TIMBER_ROOMS_C)
    {
        if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0)
        {
            return TimberRoomsCCuesAfterLighthouse;
        }
        return TimberRoomsCCues;
    }
    if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0)
    {
        return TimberRoomsBCuesAfterLighthouse;
    }
    return TimberRoomsBCues;
}

// In Lalivero (TimberRoomsC), back from Venus Lighthouse, the party reports to Faran and Iodem.
static void scTimberRoomsBCReportToFaran(void)
{
    fdScriptStart();
    gmPartyRestoreHpPp(1);
    // The party walks in to Faran and Iodem, and the others step out.
    fdActorSetPos(ACTOR_PARTY_LEADER, 96, 184);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_FARAN, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    fdScriptSetMessageId(MsgLaliveroHowDidSearchForSheba);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(10);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
    fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 104, 136);
    fdScriptWait(10);
    syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
    syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
    syFieldActorLaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    // "How did the search for Sheba turn out?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    // Faran and Iodem tell of the search for Sheba.
    fdActorFaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
    // "We were unable to find any sign of her."
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "The seas are too rough to search."
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_FARAN, 2);
    fdScriptWait(20);
    // "And there's no sign of her on land..."
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Tolbi is responsible for Sheba's fate."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "No apology can reflect the sorrow I feel at your loss, Faran..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
    // "All we can do for Sheba now is... hope."
    fdScriptShowMessage(ACTOR_FARAN, 0);
    // The party tells what became of Jenna and the others; the version that saw the ship at Idejima
    // knows Idejima was swept away with it.
    if (gmFlagGet(FLAG_SAW_SHIP_AT_IDEJIMA) == 0)
    {
        scTimberRoomsBCTellFaranAboutJenna();
    }
    else
    {
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_FARAN, 2);
        fdScriptWait(20);
        // "But tell me, what of Jenna and the others?"
        fdScriptShowMessage(ACTOR_FARAN, 0);
        fdScriptWait(10);
        fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
        fdScriptWait(10);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
        fdScriptWait(20);
        // "The same. We've searched all over, but they're nowhere to be..."
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "The ground tore apart from the shock of the lighthouse, and..."
        fdScriptShowMessage(ACTOR_MIA, 0);
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
        fdScriptWait(20);
        // "Apparently, everything at Idejima, including the ship, was..."
        fdScriptShowMessage(ACTOR_IVAN, 0);
        fdScriptWait(20);
        fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
        // "So, Idejima itself was washed out to sea?"
        fdScriptOpenMessage(ACTOR_IODEM, 0);
        fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
        fdScriptWait(20);
        // "Incredible... Then the cave, too, has been flooded?"
        fdScriptShowMessage(ACTOR_IODEM, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdScriptWait(30);
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(20);
        // "But we think Jenna and the others are still alive."
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "They may have been washed out to sea with Idejima."
        fdScriptShowMessage(ACTOR_MIA, 0);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
        fdScriptWait(30);
        // "We have decided to continue searching for Jenna."
        fdScriptShowMessage(ACTOR_IVAN, 0);
        fdScriptWait(10);
        fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
        // "Haven't we, Isaac?"
        fdScriptOpenMessage(ACTOR_IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
    }
    // Will the party keep searching? Faran answers either way.
    fdScriptSetMessageId(MsgLaliveroDoNotWorryAboutSheba);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 20);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        fdScriptWait(30);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
        fdScriptWait(30);
        // "Do not worry about Sheba. I just hope you are able to find..."
        fdScriptShowMessage(ACTOR_FARAN, 0);
        (FieldStateWork->msg)++;
    }
    else
    {
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
        fdScriptWait(30);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        (FieldStateWork->msg)++;
        fdScriptShowMessage(ACTOR_FARAN, 0);
    }
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_FARAN, 2);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    fdScriptWait(40);
    fdScriptWait(10);
    // Faran believes Sheba lived; Iodem explains Adepts.
    fdActorShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdScriptWait(50);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_FARAN, 2);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_FARAN, FACING_EAST, 0);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
    fdScriptWait(30);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 65);
    fdActorFaceDirection(ACTOR_FARAN, FACING_EAST, 0);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(40);
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
    fdScriptWait(50);
    fdActorShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorStartRepeatedMotion(ACTOR_MIA, 2);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_FARAN, FACING_EAST, 0);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
    fdScriptWait(40);
    fdActorShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    // Faran sends them off.
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorStartRepeatedMotion(ACTOR_MIA, 2);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_FARAN, 2);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
    fdScriptWait(30);
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    // The others walk back into the leader.
    fdActorSetSpeed(ACTOR_GARET, 1.2);
    fdActorSetSpeed(ACTOR_MIA, 1.2);
    fdActorSetSpeed(ACTOR_IVAN, 1.2);
    scWalkIntoLeader(ACTOR_GARET);
    scWalkIntoLeader(ACTOR_MIA);
    scWalkIntoLeader(ACTOR_IVAN);
    fdScriptWait(10);
    fdScriptEnd();
}

// The party tells Faran and Iodem what became of Jenna and the others, when it did not see the ship
// at Idejima.
static void scTimberRoomsBCTellFaranAboutJenna(void)
{
    fdScriptSetMessageId(MsgLaliveroTellOthers);
    fdScriptWait(20);

    fdActorRunRepeatedMotion(ACTOR_FARAN, 2);
    fdScriptWait(20);
    // "But tell me, what of Jenna and the others?"
    fdScriptShowMessage(ACTOR_FARAN, 0);
    fdScriptWait(10);

    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdScriptWait(20);
    // "The same. We've searched all over, but they're nowhere to be..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);

    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "The ground tore apart from the shock of the lighthouse..."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);

    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    // "And then Idejima was swept out to sea..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);

    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "My goodness... Then... Jenna and the others must have..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(20);

    fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdScriptWait(20);
    fdScriptWait(25);

    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(30);
    // "Felix said at the lighthouse that he sent Jenna ahead to the..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(30);

    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "But we think they survived."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);

    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "They may have been washed out to sea with Idejima..."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);

    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "So, we have decided to continue searching for Jenna."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);

    fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    // "Haven't we, Isaac?"
    fdScriptOpenMessage(ACTOR_IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
}

// Sets both scenes up as the party enters.
Sint32 scInit(void)
{
    Sint32 sceneId;
    Sint16 entrance;
    ACTOR *actor;

    if (MainRoutineStateWork.entrance == TIMBER_ROOMS_ENTRANCE_AFTER_LIGHTHOUSE)
    {
        gmFlagSet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
    }
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    sceneId                    = MainRoutineStateWork.next;
    if (sceneId == SCENE_TIMBER_ROOMS_B)
    {
        // Each shopkeeper is drawn under the counter's top, and counts as nearer to talk to
        // (ACTOR_NEAR_TALK) so that the leader reaches it across the counter.
        actor                          = fdActorGet(ACTOR_ITEM_MERCHANT);
        actor->priorityFlags           = 0;
        actor->collisionFlags         |= ACTOR_NEAR_TALK;
        actor->sprite->image.priority  = 2;
        actor                          = fdActorGet(ACTOR_WEAPON_MERCHANT);
        actor->priorityFlags           = 0;
        actor->collisionFlags         |= ACTOR_NEAR_TALK;
        actor->sprite->image.priority  = 2;
        actor                          = fdActorGet(ACTOR_ARMOR_MERCHANT);
        actor->collisionFlags         |= ACTOR_NEAR_TALK;
        actor->priorityFlags           = 0;
        actor->sprite->image.priority  = 2;
        fdActorSetAnimation(ACTOR_NAPPING_WORKER, ANIM_WORKER_LIE_ASLEEP);
        entrance = MainRoutineStateWork.entrance;
        if (entrance == TIMBER_ROOMS_B_SANCTUM)
        {
            MainRoutineStateWork.restartScene    = sceneId;
            MainRoutineStateWork.restartEntrance = entrance;
        }
    }
    if (MainRoutineStateWork.next == SCENE_TIMBER_ROOMS_C)
    {
        actor                          = fdActorGet(ACTOR_INNKEEPER);
        actor->collisionFlags         |= ACTOR_NEAR_TALK;
        actor->priorityFlags           = 0;
        actor->sprite->image.priority  = 2;
        if (gmFlagGet(FLAG_LALIVERO_INN_WORKER_APOLOGIZED) != 0)
        {
            fdActorSetScript(ACTOR_INN_WORKER, TimberRoomsBCInnWorkerChores);
        }
        if (MainRoutineStateWork.entrance == TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE)
        {
            scTimberRoomsBCReportToFaran();
            // Iodem and Faran turn toward the leader, as their later actor placements do.
            fdActorSetMovementType(fdActorGet(ACTOR_IODEM), MOVEMENT_TYPE_FACE_LEADER);
            fdActorSetMovementType(fdActorGet(ACTOR_FARAN), MOVEMENT_TYPE_FACE_LEADER);
            MainRoutineStateWork.entrance = TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST;
        }
    }
    return 0;
}

// The inn worker's chores once she has apologized.
//
// She paces between two spots, turning to each side, and sighs now and then.
Sint32 TimberRoomsBCInnWorkerChores[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_SPEED(2.4),
    SC_ACCELERATION(1.2),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(128, 0, 300),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 300),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(50),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(128, 0, 300),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(20),
    SC_CALL(scTimberRoomsBCInnWorkerSighs),
    SC_WAIT(30),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 300),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_ANIMATION(ANIM_NOD),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

SCENE_ENTRANCE TimberRoomsBCEntrances[] = {
    {TIMBER_ROOMS_B_FROM_LALIVERO_WEST,
     CONDITION_ALWAYS, 144,
     0, 168,
     FACING_NORTH, 0,
     0, 8,
     240, 192,
     0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST,
     CONDITION_ALWAYS, 400,
     0, 168,
     FACING_NORTH, 0,
     256, 8,
     496, 192,
     0},
    {TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP,
     CONDITION_ALWAYS, 648,
     0, 72,
     FACING_NORTH, 0,
     488, 8,
     728, 184,
     0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_NORTH,
     CONDITION_ALWAYS, 128,
     0, 472,
     FACING_NORTH, 0,
     0, 216,
     240, 488,
     0},
    {TIMBER_ROOMS_B_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS,
     CONDITION_ALWAYS, 136,
     0, 296,
     FACING_SOUTH, 0,
     0, 216,
     240, 488,
     0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST,
     CONDITION_ALWAYS, 368,
     0, 472,
     FACING_NORTH, 0,
     240, 216,
     480, 488,
     0},
    {TIMBER_ROOMS_B_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS,
     CONDITION_ALWAYS, 392,
     0, 296,
     FACING_SOUTH, 0,
     240, 216,
     480, 488,
     0},
    {TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE,
     CONDITION_ALWAYS, 648,
     0, 280,
     FACING_NORTH, 0,
     480, 216,
     720, 488,
     0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_SOUTH,
     CONDITION_ALWAYS, 96,
     0, 680,
     FACING_NORTH, 0,
     8, 520,
     312, 696,
     0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_EAST_SOUTHEAST,
     CONDITION_ALWAYS, 200,
     0, 600,
     FACING_SOUTH, 0,
     8, 520,
     312, 696,
     0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST,
     CONDITION_ALWAYS, 432,
     0, 728,
     FACING_NORTH, 0,
     312, 520,
     552, 744,
     0},
    {TIMBER_ROOMS_B_SANCTUM, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH, 0, 312, 520, 552, 744, 0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST,
     CONDITION_ALWAYS, 96,
     0, 168,
     FACING_NORTH, 0,
     8, 8,
     312, 192,
     0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_SOUTH,
     CONDITION_ALWAYS, 216,
     0, 88,
     FACING_SOUTH, 0,
     8, 8,
     312, 192,
     0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_NORTHEAST,
     CONDITION_ALWAYS, 224,
     0, 392,
     FACING_NORTH, 0,
     8, 232,
     312, 416,
     0},
    {TIMBER_ROOMS_C_INN_FROM_INN_UPSTAIRS,
     CONDITION_ALWAYS, 88,
     0, 312,
     FACING_SOUTH, 0,
     8, 232,
     312, 416,
     0},
    {TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_INN,
     CONDITION_ALWAYS, 384,
     0, 368,
     FACING_NORTH, 0,
     320, 240,
     632, 400,
     0},
    {TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST,
     CONDITION_ALWAYS, 456,
     0, 376,
     FACING_NORTH, 0,
     320, 240,
     632, 400,
     0},
    {TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST,
     CONDITION_ALWAYS, 504,
     0, 376,
     FACING_NORTH, 0,
     320, 240,
     632, 400,
     0},
    {TIMBER_ROOMS_C_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS,
     CONDITION_ALWAYS, 472,
     0, 72,
     FACING_SOUTH, 0,
     344, 8,
     584, 168,
     0},
    {TIMBER_ROOMS_C_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS,
     CONDITION_ALWAYS, 744,
     0, 72,
     FACING_SOUTH, 0,
     632, 8,
     872, 168,
     0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_NORTHWEST,
     CONDITION_ALWAYS, 832,
     0, 872,
     FACING_NORTH, 0,
     280, 504,
     912, 896,
     0},
    {TIMBER_ROOMS_C_FROM_LEMURIAN_SHIP,
     CONDITION_ALWAYS, 464,
     0, 832,
     FACING_SOUTH, 0,
     280, 504,
     912, 896,
     0},
    {TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE,
     CONDITION_ALWAYS, 96,
     0, 168,
     FACING_NORTH, 0,
     8, 8,
     312, 192,
     0},
    {SCENE_TABLE_END},
};

// clang-format off
SCENE_EXIT_REGION TimberRoomsBRegions[] = {
    {196, 28, 595, 204, 36, 603, CONDITION_ALWAYS, TIMBER_ROOMS_B_TO_LALIVERO_EAST_SOUTHEAST},
    {SCENE_TABLE_END},
};

SCENE_EXIT_REGION TimberRoomsCRegions[] = {
    {212, 28, 83, 220, 36, 91, CONDITION_ALWAYS, TIMBER_ROOMS_C_TO_LALIVERO_SOUTH},
    {SCENE_TABLE_END},
};

Uint32 TimberRoomsBCExits[] = {
    SCENE_EXITS(TimberRoomsB),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_WEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_WEST),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_SOUTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_SOUTHEAST),
    SCENE_EXIT(TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_TO_ITEM_SHOP, TimberRoomsB, TIMBER_ROOMS_B_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_NORTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTH),
    SCENE_EXIT(TIMBER_ROOMS_B_ITEM_SHOP_TO_ITEM_SHOP_UPSTAIRS, TimberRoomsB, TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTHWEST),
    SCENE_EXIT(TIMBER_ROOMS_B_NORTHWEST_HOUSE_TO_NORTHWEST_HOUSE_UPSTAIRS, TimberRoomsB, TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE),
    SCENE_EXIT(TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_TO_NORTHWEST_HOUSE, TimberRoomsB, TIMBER_ROOMS_B_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_SOUTH),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_EAST_SOUTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_EAST_SOUTHEAST),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTHEAST),
    SCENE_EXITS(TimberRoomsC),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_SOUTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_SOUTHWEST),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_SOUTH),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_NORTHEAST),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_INN),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_INN, TimberRoomsC, TIMBER_ROOMS_C_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_WEST, TimberRoomsC, TIMBER_ROOMS_C_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_EAST, TimberRoomsC, TIMBER_ROOMS_C_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_C_GUEST_ROOM_WEST_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST),
    SCENE_EXIT(TIMBER_ROOMS_C_GUEST_ROOM_EAST_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_NORTHWEST),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LEMURIAN_SHIP, LemurianShip, LEMURIAN_SHIP_FROM_TIMBER_ROOMS_C),
    SCENE_EXITS_END,
};

ACTOR_PLACE TimberRoomsBActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        176, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 80, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 136, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 120, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
        616, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        664, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        400, 0, 416, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        550, 0, 368, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 304, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        600, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        132, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 616, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        68, 0, 340, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        432, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE TimberRoomsCActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        240, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 88, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_FARAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 336, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 300, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        544, 0, 368, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE TimberRoomsCActorPlacementsAfterLighthouse[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        240, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FARAN, CONDITION_ALWAYS, MOVEMENT_TYPE_FACE_LEADER,
        104, 0, 96, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_FACE_LEADER,
        128, 0, 96, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 336, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        160, 0, 304, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        544, 0, 368, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

CUE TimberRoomsBCues[] = {
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_TO_ITEM_SHOP, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_ITEM_SHOP_TO_ITEM_SHOP_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_NORTHWEST_HOUSE_TO_NORTHWEST_HOUSE_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_TO_NORTHWEST_HOUSE, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_TALK, ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOfCourseIHaveSeenActor) },
    { CUE_TALK, ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThereAreSomeWhoDoNot) },
    { CUE_TALK, ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll) },
    { CUE_TALK, ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfActorCantUseHerPowers) },
    { CUE_TALK, ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorCameFlyingToTheRuins) },
    { CUE_TALK, ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix) },
    { CUE_TALK, ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayAManNamedIodem) },
    { CUE_TALK, ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOnesCalledMeSoMaybe) },
    { CUE_TALK, ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorSaidTheCaveAtThe) },
    { CUE_TALK, ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIfThePowerThatYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIsTheSaviorOfLalivero) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsALegendarySaviorWillAppearIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIdLikeToBelieveInActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenThoughSheFellOutOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSomethingHappensToActorWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenIodemGetsHereHesGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsBeenSoLongSinceI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIWonderHowActorKnewAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThoseCavesAreTooDangerousFor) },
    { CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToWeaponMerchant) },
    { CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToArmorMerchant) },
    { CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToItemMerchant) },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheLaliveransAreATimidPeople) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantRescueActorIfI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfAnythingHappensToActorIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIBetBabisTowerWillBe) },
    { EXAMINE_CUE(EXAMINE_BARREL), 100, FLAG_TIMBER_ROOMS_B_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CUE(EXAMINE_BARREL), 101, FLAG_TIMBER_ROOMS_B_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { CUE_TABLE_END },
};

CUE TimberRoomsBCuesAfterLighthouse[] = {
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_TO_ITEM_SHOP, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_ITEM_SHOP_TO_ITEM_SHOP_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_NORTHWEST_HOUSE_TO_NORTHWEST_HOUSE_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_TO_NORTHWEST_HOUSE, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_B_TO_LALIVERO_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_TALK, ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheContinentBrokeApartAroundThe) },
    { CUE_TALK, ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhatHappenedToThoseLivingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIShudderToThinkWhatWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWereLuckyThatWeLiveNorth) },
    { CUE_TALK, ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEveryoneSaysThatActorIsOkay) },
    { CUE_TALK, ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIRealizedHowThoughtlessIWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSheIsIndeedTheChild) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeWantToBelieveInSomething) },
    { CUE_TALK, ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorShouldBeFineSheWas) },
    { CUE_TALK, ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorWillWakeWhenTheBeacon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSeeingActorDiveMustHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantWaitUntilActorIs) },
    { CUE_TALK, ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsBabiLighthouseCrumbledWhenAllThe) },
    { CUE_TALK, ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIAlwaysWantedABreakBut) },
    { CUE_TALK, ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoTheRuinsReallyAreConnected) },
    { CUE_TALK, ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouFinallySolvedTheMysteryOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWillTheyContinueOnOrQuit) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImWorriedThatWeWontGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeCantPossiblyDigAHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheMysteryOfTheRuinsShould) },
    { CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToWeaponMerchant) },
    { CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToArmorMerchant) },
    { CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToItemMerchant) },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOneCouldPossiblySwimIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenWillTheOceanBeCalm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheChampaArePeopleFromA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFirstTheLighthouseWasStruckDown) },
    { EXAMINE_CUE(EXAMINE_BARREL), 100, FLAG_TIMBER_ROOMS_B_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CUE(EXAMINE_BARREL), 101, FLAG_TIMBER_ROOMS_B_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { CUE_TABLE_END },
};

CUE TimberRoomsCCues[] = {
    { CUE_STEP, TIMBER_ROOMS_C_TO_LALIVERO_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_TO_LALIVERO_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_TO_INN_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_UPSTAIRS_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_GUEST_ROOM_WEST_TO_INN_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_GUEST_ROOM_EAST_TO_INN_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_TO_LALIVERO_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_TO_LEMURIAN_SHIP, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_TALK, ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeReceivedActorFromTheSky) },
    { CUE_TALK, ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustKnowThatFatherWill) },
    { CUE_TALK, ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsSoPainfulToEvenThink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustWishSomeoneWouldPlease) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThatCoupleRaisedActorAsThough) },
    { CUE_TALK, ACTOR_INN_WORKER, FLAG_LALIVERO_INN_WORKER_APOLOGIZED, SCRIPT_REF(scTimberRoomsBCTalkToInnWorker) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_INN_WORKER, FLAG_LALIVERO_INN_WORKER_APOLOGIZED, SCRIPT_REF(scTimberRoomsBCTalkToInnWorker) },
    { CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToInnkeeper) },
    { CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind) },
    { CUE_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeMustObeyTolbiThatsWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyDidThisHaveToHappen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHousecleaningChangingSheetsItsAllDone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOnceThisIncidentIsOverI) },
    { EXAMINE_CUE(EXAMINE_BARREL), 102, FLAG_TIMBER_ROOMS_C_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { CUE_TABLE_END },
};

CUE TimberRoomsCCuesAfterLighthouse[] = {
    { CUE_STEP, TIMBER_ROOMS_C_TO_LALIVERO_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_TO_LALIVERO_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_TO_INN_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_UPSTAIRS_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_GUEST_ROOM_WEST_TO_INN_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_GUEST_ROOM_EAST_TO_INN_UPSTAIRS, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_TO_LALIVERO_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, TIMBER_ROOMS_C_TO_LEMURIAN_SHIP, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
    { CUE_STEP, METATILE_BEHAVIOR_TIMBER_ROOMS_C_IODEM_CALLS_OUT, FLAG_IODEM_ASKED_TO_MEET, SCRIPT_REF(scTimberRoomsBCIodemCallsOut) },
    { CUE_TALK, ACTOR_IODEM, FLAG_IODEM_FAREWELL_SEEN, SCRIPT_REF(scTimberRoomsBCTalkToIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, FLAG_IODEM_FAREWELL_SEEN, MSGNO(MsgLaliveroInteriorsIMustExplainTheirPromiseTo) },
    { CUE_TALK, ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFaranSaysKnowingHerPowersActor) },
    { CUE_TALK, ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouAreLeavingAfterAllArent) },
    { CUE_TALK, ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayTheLighthouseCrumbledDown) },
    { CUE_TALK, ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouWillFindJennaSomedayFarewell) },
    { CUE_TALK, ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIsaacThatBoatBelongsToYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIDontNeedFameOrFortune) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWontAnyoneStopIsaacAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTolbiIsToBeBlamedFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoThereAreSomePeopleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsPleaseUseThatShipToFind) },
    { CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsBCTalkToInnkeeper) },
    { CUE_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIMustSayItIsNice) },
    { CUE_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWouldTheLightingMakeSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImSleepyTheseBrightNightsHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsGeeWhatAmIGoingTo) },
    { EXAMINE_CUE(EXAMINE_BARREL), 102, FLAG_TIMBER_ROOMS_C_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { EXAMINE_CUE(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { CUE_TABLE_END },
};
// clang-format on

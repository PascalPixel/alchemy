// Log rooms B (SceneIdLogRoomsB), a rooms map drawn in logs, holds only Vault's rooms: the town's
// houses, shops and inn once the party has reached Kalay.
//
// They replace the first visit's rooms (log_rooms_a.c), as SceneIdVaultRevisit (vault_revisit.c)
// replaces the town. Each of its six house doors leads to the entrance here named for the same
// door, and each room leads back out through it.
//
// The thieves have broken out of jail. The talk is of them and of Colosso, the festival about to
// begin in Tolbi.
//
// The rooms, by entrance:
// - LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST, the gem owner's
//       house. He wails that a cat ate his precious stone, and his wife says
// she saw the cat get up on the table and paw it. The big pale blue gem that
//       stood on the round table in front of him on the first visit is gone:
//       scInit draws the table bare from a copy kept at map metatile
//       (0, 120) of the overhead layer.
// - LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTH, the house of a boy who
//       doubts the thieves were from Lunpa.
// - LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST and
//       LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS, the inn. The girl at the inn offers
//       Bones there, and a Bone lies on the barrel beside her. Its stairs lead
// to the upstairs room, where a cook works.
// - LOG_ROOMS_B_FROM_VAULT_REVISIT_EAST, the item shop.
// - LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTH, the weapon and armor
//       shops.
// - LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTHWEST, the mayor's house.
//       The mayor welcomes Isaac back and warns that the thieves have escaped
//       (FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, an area flag, and
//       FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES). A Mind Read on him before that
//       plays the welcome first.
//
// The Bones are how the party gets the dog in Vault to show its hidden passage. The girl's offer
// gives one Bone each time it is accepted. FLAG_VAULT_BONE_RECEIVED, shared with log_rooms_a.c,
// changes only her words. Searching the barrel beside her, with the Bone on it
// (METATILE_BEHAVIOR_LOG_ROOMS_A_INN_BARREL, scVaultFindBone) gives another.
//
// While Isaac stands beside that Bone, scVaultUpdateLeaderBesideBoneFlag sets
// FLAG_VAULT_LEADER_BESIDE_BONE, which turns her talk row off, so that the A button there picks the
// Bone up instead of talking to her.
//
// The shopkeepers open their shops only when spoken to across the counter.
//
// A jar, two barrels and a wooden box keep the first visit's flags, so what the party took then
// stays taken. Two bookcases can be read.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "scene/log_rooms_a.h"
#include "scene/vault_item_icon.h"
#include "game/system_call.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE LogRoomsBEntrances[];
extern Uint32         LogRoomsBExits[];
extern ACTOR_PLACE    LogRoomsBActorPlacements[];
extern CUE            LogRoomsBCues[];

// The townspeople, numbering the actor placement rows from ACTOR_FIRST_PLACED and named by what
// they say on the party's return.
enum
{
    // A young man whose precious stone, he says, a cat ate.
    ACTOR_GEM_OWNER = ACTOR_FIRST_PLACED,
    // A woman with fair hair.
    ACTOR_GEM_OWNERS_WIFE,
    // A burly grey-haired man in the boy's house.
    ACTOR_VILLAGER_HEARD_PRISONERS,
    // A small boy.
    ACTOR_BOY,
    // An old man with a cane who would like to see Colosso once more.
    ACTOR_COLOSSO_HUSBAND,
    // An old woman.
    ACTOR_COLOSSO_WIFE,
    // A small boy in the mayor's house.
    ACTOR_BOY_ON_THE_THIEVES_ESCAPE,
    // A little girl in the mayor's house.
    ACTOR_GIRL_CURIOUS_ABOUT_THE_FESTIVAL,
    // The mayor, an old man with white hair.
    ACTOR_MAYOR,
    // A woman in a green dress.
    ACTOR_MAYORS_WIFE,
    // The girl at the inn, as the Bone's description calls her, a young woman with fair hair.
    ACTOR_GIRL_AT_THE_INN,
    ACTOR_VAULT_WEAPON_MERCHANT,
    ACTOR_VAULT_ARMOR_MERCHANT,
    ACTOR_VAULT_ITEM_MERCHANT,
    // A man with dark hair behind the inn's counter.
    ACTOR_VAULT_INNKEEPER,
    // A woman in an apron, upstairs at the inn.
    ACTOR_COOK
};

// Set once the mayor has told Isaac the thieves escaped from jail.
enum
{
    FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK = FLAG_AREA_TEMP_00
};

#include "shared/vault_leader_beside_bone_flag.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return LogRoomsBEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return LogRoomsBExits;
}

// Gives the actor placements' condition 0 whether each lies inside the view bounds of the room
// entered (fdAssignViewFlags), so only that room's people appear.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    ACTOR_PLACE *placements = LogRoomsBActorPlacements;

    fdAssignViewFlags(placements);
    return placements;
}

// The actor stands still and says the selected message.
static void scLogRoomsBShowStandingActorMessage(Sint32 actorId)
{
    fdScriptStart();
    fdActorSetAnimation(actorId, ANIM_STAND);
    fdScriptShowMessage(actorId, 0);
    fdScriptEnd();
}

// Faces the leader and shows this resident's dialogue while standing still.
static inline void scLogRoomsBFaceAndSpeak(Sint32 actorId, Sint32 msg)
{
    fdScriptSetMessageId(msg);
    fdActorTurnToFaceEachOther(actorId, ACTOR_PARTY_LEADER, 2);
    scLogRoomsBShowStandingActorMessage(actorId);
}

// In the gem owner's house, his wife says she saw the cat get up on the table and paw the gemstone,
// and then it was gone.
static void scLogRoomsBWifeSawCatTakeGem(void)
{
    scLogRoomsBFaceAndSpeak(ACTOR_GEM_OWNERS_WIFE, MsgVaultThoughtSawCatGetUp);
}

// In the house of the burly grey-haired man, a small boy points out that Dodonpa's henchmen never
// attacked Vault, so maybe the thieves were not from Lunpa after all.
static void scLogRoomsBBoyDoubtsThievesWereFromLunpa(void)
{
    scLogRoomsBFaceAndSpeak(ACTOR_BOY, MsgVaultDodonpasHenchmenNeverAttackedUs);
}

// In the item shop, an old man with a cane says it is near time for Colosso to start in Tolbi, and
// he would like to go once more before he dies.
static void scLogRoomsBHusbandLongsForColosso(void)
{
    scLogRoomsBFaceAndSpeak(ACTOR_COLOSSO_HUSBAND, MsgVaultItsNearTimeForColosso);
}

// The first time Isaac speaks to Vault's mayor after reaching Kalay, the mayor welcomes him back
// and warns that the thieves he caught have escaped from jail. Asked whether they have run into
// them, a Yes has him say to watch out for suspicious shadows, a No that it's good, but be careful.
static void scLogRoomsBWelcomeIsaacBack(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultIsaacYouveComeBackVault);
    fdActorTurnToFaceEachOther(ACTOR_MAYOR, ACTOR_PARTY_LEADER, 2);
    fdActorSetAnimation(ACTOR_MAYOR, ANIM_STAND);
    // "Oh, Isaac! You've come back to Vault!"
    fdScriptShowMessageThenWait(ACTOR_MAYOR, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_MAYOR, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Ever since you left, all sorts of crazy things have been..."
    fdScriptShowMessageThenWait(ACTOR_MAYOR, 0, 20);
    fdActorShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    // "Those thieves you caught have escaped from jail."
    fdScriptShowMessageThenWait(ACTOR_MAYOR, 0, 30);
    // The negative answer advances to the next message before continuing. "We were worried that
    // they might come after you. Have you run..."
    fdScriptOpenMessage(ACTOR_MAYOR, 0);
    if (fdScriptAskYesNoWithActorAnimation(0, 0) != 0)
    {
        FieldStateWork->msg++;
    }
    fdScriptShowMessageThenWait(ACTOR_MAYOR, 0, 20);
    gmFlagSet(FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK);
    gmFlagSet(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
    fdScriptEnd();
}

// Spoken to again after his welcome, Vault's mayor tells the party that the escaped thieves really
// wanted revenge on them, so they should take good care of themselves.
static void scLogRoomsBWarnOfThievesRevenge(void)
{
    scLogRoomsBFaceAndSpeak(ACTOR_MAYOR, MsgVaultThoseGuysReallyWantedRevenge);
}

// Unused: the cook upstairs at the inn offers the party her leftover bones, but gives none.
static void scLogRoomsBUnusedOfferLeftoverBones(void)
{
    scLogRoomsBFaceAndSpeak(ACTOR_COOK, MsgVaultDidSomeCookingNowIve);
}

// At Vault's inn, the girl at the inn offers Isaac leftover bones from dinner, or asks "You want
// more bones?" once he has had one (FLAG_VAULT_BONE_RECEIVED). Accepting gives a Bone, unless his
// bags are full and she calls him a greedy lad.
static void scLogRoomsBOfferBones(void)
{
    fdScriptStart();
    fdActorTurnToFaceEachOther(ACTOR_GIRL_AT_THE_INN, ACTOR_PARTY_LEADER, 0);
    if (gmFlagGet(FLAG_VAULT_BONE_RECEIVED) == 0)
    {
        fdScriptSetMessageId(MsgVaultHaveLotLeftoverBonesFrom);
        // "I have a lot of leftover bones from dinner. Here, do you..."
        fdScriptOpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultWantMoreBones);
        // "What? You want more bones?"
        fdScriptOpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
    }
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
    {
        fdScriptWait(20);
        fdScriptShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_GIRL_AT_THE_INN, 2);
        fdScriptWait(20);
        // Give the Bone only after the offer is accepted and space is available.
        if (gmPartyInventoryHasSpace() == 0)
        {
            fdActorSetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
            fdScriptWait(20);
            fdScriptSetMessageId(MsgVaultWowHaveManyThingsArent);
            // "Wow, you have so many things. Aren't you a greedy lad!"
            fdScriptShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
        }
        else
        {
            fdItemDisplayShowItem(ITEM_BONE, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
            fdPartyInventoryGiveItem(ITEM_BONE, 0);
            gmFlagSet(FLAG_VAULT_BONE_RECEIVED);
        }
    }
    else
    {
        // Declining selects the following line and the actor's response pose.
        FieldStateWork->msg++;
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_NOD);
        fdScriptWait(20);
        fdScriptShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    }
    fdActorFaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
    fdScriptEnd();
}

// Mind Read on Vault's mayor before he has welcomed Isaac back: he notices the party, and his
// welcome plays as if Isaac had spoken to him.
static void scLogRoomsBReadMindAndWelcomeIsaacBack(void)
{
    Uint8 *paused;
    Uint8  unpaused = 0;

    fdScriptStart();
    fdActorRunRepeatedMotion(ACTOR_MAYOR, 1);
    fdScriptEnd();
    paused  = &fdActorGet(ACTOR_MAYOR)->paused;
    *paused = 1;
    scLogRoomsBWelcomeIsaacBack();
    paused  = &fdActorGet(ACTOR_MAYOR)->paused;
    *paused = unpaused;
    fdActorSetMovementTypeById(ACTOR_MAYOR, MOVEMENT_TYPE_WANDER);
}

#include "shared/vault_bone.c"

// Spoken to across the counter, Vault's weapon merchant opens his shop. From anywhere else he talks
// of the river of fire on the other side of Mt. Aleph.
static void scLogRoomsBTalkToWeaponMerchant(void)
{
    Uint32 leaderFacing;

    leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
    fdScriptStart();

    if (FACING_IS_NORTH(leaderFacing))
    {
        mnShopRun(VAULT_WEAPON_STOCK, ACTOR_VAULT_WEAPON_MERCHANT);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultTheresRiverFireOnOther);
        // "There's a river of fire on the other side of Mt. Aleph...."
        fdScriptShowMessage(ACTOR_VAULT_WEAPON_MERCHANT, 0);
    }

    fdScriptEnd();
}

// Spoken to across the counter, Vault's armor merchant opens his shop. From anywhere else he says
// the volcano on Mt. Aleph has torn the continent in two.
static void scLogRoomsBTalkToArmorMerchant(void)
{
    Uint32 leaderFacing;

    leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
    fdScriptStart();

    if (FACING_IS_NORTH(leaderFacing))
    {
        mnShopRun(VAULT_ARMOR_STOCK, ACTOR_VAULT_ARMOR_MERCHANT);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultTheySayVolcanoOnMt);
        // "They say the volcano on Mt. Aleph has torn our continent in..."
        fdScriptShowMessage(ACTOR_VAULT_ARMOR_MERCHANT, 0);
    }

    fdScriptEnd();
}

// Spoken to across the counter, Vault's item merchant opens her shop. From anywhere else she says
// they haven't been getting many customers lately.
static void scLogRoomsBTalkToItemMerchant(void)
{
    Uint32 leaderFacing;

    leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
    fdScriptStart();

    if (FACING_IS_NORTH(leaderFacing))
    {
        mnShopRun(VAULT_ITEM_STOCK, ACTOR_VAULT_ITEM_MERCHANT);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultWeHaventBeenGettingMany);
        // "We haven't been getting many customers lately. Maybe I'll..."
        fdScriptShowMessage(ACTOR_VAULT_ITEM_MERCHANT, 0);
    }

    fdScriptEnd();
}

// Spoken to across the counter, Vault's innkeeper offers a room. From anywhere else he grumbles
// that there is nothing to do.
static void scLogRoomsBTalkToInnkeeper(void)
{
    Uint32 leaderFacing;

    leaderFacing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
    fdScriptStart();

    if (FACING_IS_NORTH(leaderFacing))
    {
        mnInnCheckIn(VAULT_INN, ACTOR_VAULT_INNKEEPER);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultMmmmNothingDoNothingDo);
        // "Mmmm. Nothing to do, nothing to do. I hate this! It's all..."
        fdScriptShowMessage(ACTOR_VAULT_INNKEEPER, 0);
    }

    fdScriptEnd();
}

// Mind Read on the girl at the inn: before she has given Isaac a Bone she is tempted to taste the
// cooking; after, she thinks Isaac really likes bones and wonders whether he has a calcium
// deficiency.
static void scLogRoomsBReadInnGirlsMind(void)
{
    fdScriptStart();
    if (gmFlagGet(FLAG_VAULT_BONE_RECEIVED) == 0)
    {
        fdScriptSetMessageId(MsgVaultGeeAlwaysGetHungryWhen);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultHeReallyLikesBonesWonder);
    }
    fdScriptShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    fdScriptEnd();
}

// The rooms' doors, people, Mind Reads and the things to search.
const CUE *scGetCues(void)
{
    return LogRoomsBCues;
}

// Opens the room entered with the window transition. In the gem owner's house the table is drawn
// bare; at the inn the Bone is set on the barrel beside the girl at the inn.
Sint32 scInit(void)
{
    Sint32 entrance;

    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    entrance                   = MainRoutineStateWork.entrance;

    if (entrance == LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST)
    {
        fdMapGridCopyMetatilesTo(0, 120, 8, 67, 4, 3);
        // As on the first visit, he stands on the floor.
        fdActorGet(ACTOR_GEM_OWNER)->motionFlags  = 0;
        fdActorGet(ACTOR_GEM_OWNER)->y.fixed      = 0;
        fdActorGet(ACTOR_GEM_OWNER)->groundHeight = 0;
    }
    else if (entrance == LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST
             || entrance == LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS)
    {
        scVaultPlaceItemIcon(ITEM_BONE, toFIXED(568), toFIXED(16), toFIXED(672));
        sySetTickEvent(scVaultUpdateLeaderBesideBoneFlag, EVENT_PASS_TICK);
    }

    return 0;
}

#include "shared/vault_item_icon.c"

SCENE_ENTRANCE LogRoomsBEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST,
     CONDITION_ALWAYS, 192,
     0, 220,
     FACING_NORTH, 0,
     32, 0,
     272, 240,
     0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTH,
     CONDITION_ALWAYS, 432,
     0, 254,
     FACING_NORTH, 0,
     288, 0,
     528, 272,
     0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST,
     CONDITION_ALWAYS, 720,
     0, 782,
     FACING_NORTH, 0,
     432, 576,
     800, 800,
     0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_EAST,
     CONDITION_ALWAYS, 624,
     0, 236,
     FACING_NORTH, 0,
     560, 32,
     848, 256,
     0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTH,
     CONDITION_ALWAYS, 144,
     0, 492,
     FACING_NORTH, 0,
     16, 288,
     256, 512,
     0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTHWEST,
     CONDITION_ALWAYS, 416,
     0, 526,
     FACING_NORTH, 0,
     320, 288,
     592, 544,
     0},
    {LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS,
     CONDITION_ALWAYS, 744,
     0, 640,
     FACING_SOUTH, 0,
     432, 576,
     800, 800,
     0},
    {LOG_ROOMS_B_INN_UPSTAIRS_FROM_INN,
     CONDITION_ALWAYS, 358,
     0, 632,
     FACING_SOUTH, 0,
     32, 576,
     400, 776,
     0},
    {SCENE_TABLE_END},
};

Uint32 LogRoomsBExits[] = {
    SCENE_EXITS(LogRoomsB),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHEAST,
               VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHEAST),
    SCENE_EXIT(
        LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTH, VaultRevisit, VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTH),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHWEST,
               VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHWEST),
    SCENE_EXIT(
        LOG_ROOMS_B_TO_VAULT_REVISIT_EAST, VaultRevisit, VAULT_REVISIT_FROM_LOG_ROOMS_B_EAST),
    SCENE_EXIT(
        LOG_ROOMS_B_TO_VAULT_REVISIT_NORTH, VaultRevisit, VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTH),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_NORTHWEST,
               VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTHWEST),
    SCENE_EXIT(LOG_ROOMS_B_INN_UPSTAIRS_TO_INN, LogRoomsB, LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(LOG_ROOMS_B_INN_TO_INN_UPSTAIRS, LogRoomsB, LOG_ROOMS_B_INN_UPSTAIRS_FROM_INN),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE LogRoomsBActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0, MOVEMENT_TYPE_STAND,
        160, 0, 136, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_WANDER_NEAR,
        152, 0, 72, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0, MOVEMENT_TYPE_STAND,
        456, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_WANDER,
        368, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, 0, MOVEMENT_TYPE_WANDER,
        744, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, 0, MOVEMENT_TYPE_STAND,
        752, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_STAND,
        496, 0, 416, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, 0, MOVEMENT_TYPE_STAND,
        512, 0, 416, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0, MOVEMENT_TYPE_STAND,
        536, 0, 451, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, 0, MOVEMENT_TYPE_STAND,
        416, 0, 464, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_STAND,
        568, 0, 648, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, 0, MOVEMENT_TYPE_STAND,
        200, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, 0, MOVEMENT_TYPE_STAND,
        88, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, 0, MOVEMENT_TYPE_STAND,
        632, 0, 136, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, 0, MOVEMENT_TYPE_STAND,
        696, 0, 696, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, 0, MOVEMENT_TYPE_WANDER,
        248, 0, 712, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

CUE LogRoomsBCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHEAST },
    { CUE_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTH },
    { CUE_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHWEST },
    { CUE_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_EAST },
    { CUE_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_NORTH },
    { CUE_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_NORTHWEST },
    { CUE_EXIT, 7, CONDITION_ALWAYS, LOG_ROOMS_B_INN_UPSTAIRS_TO_INN },
    { CUE_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_B_INN_TO_INN_UPSTAIRS },
    { CUE_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultWaahACatAteMyPrecious) },
    { CUE_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBWifeSawCatTakeGem) },
    { CUE_TALK, ACTOR_VILLAGER_HEARD_PRISONERS, CONDITION_ALWAYS, MSGNO(MsgVaultThePrisonersSaidTheyWantedTo) },
    { CUE_TALK, ACTOR_BOY, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBBoyDoubtsThievesWereFromLunpa) },
    { CUE_TALK, ACTOR_COLOSSO_HUSBAND, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBHusbandLongsForColosso) },
    { CUE_TALK, ACTOR_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheWarriorsISawFightingAt) },
    { CUE_TALK, ACTOR_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, SCRIPT_REF(scLogRoomsBWelcomeIsaacBack) },
    { CUE_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBWarnOfThievesRevenge) },
    { CUE_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheMayorsFamilyUrnIsOn) },
    { CUE_TALK, ACTOR_BOY_ON_THE_THIEVES_ESCAPE, CONDITION_ALWAYS, MSGNO(MsgVaultThoseThievesYouGoToAll) },
    { CUE_TALK, ACTOR_GIRL_CURIOUS_ABOUT_THE_FESTIVAL, CONDITION_ALWAYS, MSGNO(MsgVaultIHearThatABigFestival) },
    { CUE_TALK, ACTOR_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBTalkToWeaponMerchant) },
    { CUE_TALK, ACTOR_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBTalkToArmorMerchant) },
    { CUE_TALK, ACTOR_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBTalkToItemMerchant) },
    { CUE_TALK, ACTOR_VAULT_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBTalkToInnkeeper) },
    { CUE_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultWeDontGetALotOf) },
    // Unused: the row above always answers the cook first.
    { CUE_TALK, ACTOR_COOK, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBUnusedOfferLeftoverBones) },
    { CUE_TALK, ACTOR_GIRL_AT_THE_INN, FLAG_VAULT_LEADER_BESIDE_BONE, SCRIPT_REF(scLogRoomsBOfferBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultIfIdKnownThisWasGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMyHusbandSaysMyCatAte) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_HEARD_PRISONERS, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoYouSupposeThoseThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY, CONDITION_ALWAYS, MSGNO(MsgVaultDaddyHasntBeenPayingAttentionSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COLOSSO_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgVaultWheneverMyWifeUsedToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultLatelyMyHusbandsBeenGoingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, SCRIPT_REF(scLogRoomsBReadMindAndWelcomeIsaacBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgVaultAsLongAsWeStayAlert) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultWellBeSecuringItInA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_THIEVES_ESCAPE, CONDITION_ALWAYS, MSGNO(MsgVaultItsJustEmbarrassingThatWeLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_CURIOUS_ABOUT_THE_FESTIVAL, CONDITION_ALWAYS, MSGNO(MsgVaultILoveFestivalsIllBetColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultJustImagineHowTerribleItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItsAGoodThingThatGiant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyKeepTheStoreOpenIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultWontAnyoneComeStayTheNight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultAfterYouveBeenInThisBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, SCRIPT_REF(scLogRoomsBReadInnGirlsMind) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, SCRIPT_REF(scVaultFindBone) },
    { EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
    { EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
    { EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOfAngara)) },
    { CUE_TABLE_END },
};
// clang-format on

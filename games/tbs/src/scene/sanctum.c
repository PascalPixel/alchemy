// The Sanctum, the room of the Healers that Vale, Vault, Bilibin, Xian and Altin each have
// (SceneIdSanctum). One room serves them all: the door the party comes in by decides whose Sanctum
// it is, which people stand in it, and which town its door leads back out to. The Healer stands at
// the top of the room before the altar, behind a small stone counter. Talking to him across it,
// facing north, opens the Sanctum's services; from any other side he only talks.
//
// - Vale's Sanctum: the Great Healer, with a Healer on each side of the room
//       who speaks of Psynergy. After the eruption of Mt. Aleph, the Great
// Healer brings Isaac and Garet here from Vale (vale_eruption.c, entrance
//       SANCTUM_FROM_VALE_ERUPTION). Dora and the Mayor are waiting. Isaac and
//       Garet act out what happened in Sol Sanctum while the room is tinted
// sepia, the Great Healer tells what the Wise One showed him, and he asks Isaac to take on the
// quest. For these visits the stone counter is taken away and
//       the Great Healer stands beside the altar.
//
//       A yes brings the Wise One down over the altar to give Isaac and Garet
//       their mission; the screen fades to black over the inn's night music,
// and the two are in Vale's plaza (vale_plaza.c). A no leaves everyone waiting in the room: the
// Great Healer asks again when spoken to, he warns Isaac as he heads for the door, and stepping out
// ends the game, "And so the world began drifting toward its fated destruction...", with a choice
// to start again from the Great Healer (SANCTUM_FROM_QUEST_RESTART, without the acting out) or to
// go to the file screen. After the party has left Vale (FLAG_VALE_PARTY_LEFT), and again after it
// has reached Kalay (FLAG_KALAY_REACHED), the three Healers talk of what the eruption and the Wise
// One changed.
// - Vault's Sanctum: the Healer, and a man with long brown hair in a long robe
//       who wanders the room. A gold statue was stolen from them during the
//       eruption. Once the party has caught the thieves at the inn
//       (FLAG_VAULT_THIEVES_JAILED) the statue stands back behind the Healer,
// and on the party's later visit through Vault (vault_revisit.c) it has been
//       polished and the Healer worries about earthquakes on Mt. Aleph.
// - Bilibin's Sanctum: the Healer and a man like Vault's, who talk of
//       Kolima's curse, before and after it is lifted
// (FLAG_KOLIMA_CURSE_LIFTED).
// - Xian's Sanctum: a Healer who came to Xian from very distant lands.
// - Altin's Sanctum: a Healer who tells of the water-spewing beasts in the
//       mine, until the party has saved Altin from them
//       (FLAG_HYDROS_STATUES_DEFEATED).
//
// Entrance SANCTUM_UNUSED, which no exit leads to, places nobody and gives the room the name
// Sanctum alone.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

const ACTOR_PLACE    *scGetActorPlacements(void);
extern Uint32         SanctumLeaderCircleScript[];
extern Uint32         SanctumGaretCircleScript[];
extern Sint32         ItemIconGrowScript[];
extern Sint32         SanctumItemIconEndScript[];
extern SCENE_ENTRANCE SanctumEntrances[];
extern Uint32         SanctumExits[];
extern ACTOR_PLACE    SanctumNoActorPlacements[];
extern ACTOR_PLACE    SanctumValeActorPlacements[];
extern ACTOR_PLACE    SanctumIvanActorPlacement[];
extern ACTOR_PLACE    SanctumVaultBilibinActorPlacements[];
extern ACTOR_PLACE    SanctumQuestActorPlacements[];
extern ACTOR_PLACE    SanctumAltinXianActorPlacements[];
extern CUE            SanctumValeCues[];
extern CUE            SanctumVaultCues[];
extern CUE            SanctumVaultRevisitCues[];
extern CUE            SanctumQuestCues[];
extern CUE            SanctumValeCuesAfterLeaving[];
extern CUE            SanctumValeCuesAfterKalay[];
extern CUE            SanctumAltinCues[];
extern CUE            SanctumBilibinCues[];
extern CUE            SanctumXianCues[];
extern Sint32         SanctumCursorOffsets[];
extern Sint32         SanctumOrbitingSparkleScript[];
extern Sint32         SanctumSparkEndScript[];

// The metatiles the room's step cues answer.
typedef enum SANCTUM_METATILE_BEHAVIOR
{
    // The doorway, metatiles (10, 16) to (14, 16).
    METATILE_BEHAVIOR_SANCTUM_DOORWAY = 1,
    // The row just inside it, metatiles (10, 15) to (14, 15).
    METATILE_BEHAVIOR_SANCTUM_BEFORE_DOOR
} SANCTUM_METATILE_BEHAVIOR;

// After the quest is refused, the Great Healer has warned Isaac at the door that stepping outside
// the village is final.
#define FLAG_SANCTUM_WARNED_BEFORE_LEAVING FLAG_SCENE_TEMP_00
// The party is starting again from the Great Healer after refusing the quest, so the account of Sol
// Sanctum is skipped.
#define FLAG_SANCTUM_QUEST_RESTARTED       FLAG_SCENE_TEMP_01

// The placed actors, numbering the rows of the table scGetActorPlacements returns for the entrance.
//
// The party's rows take their own ids. Every place's first row is its healer.
typedef enum SANCTUM_ACTOR_ID
{
    // The town's Healer, a man with a white beard in a tall hat and a long purple and blue robe,
    // before the altar at the top of the room.
    //
    // He is the Great Healer in Vale's Sanctum.
    ACTOR_HEALER = ACTOR_FIRST_PLACED,
    // Vale Sanctum (SanctumValeActorPlacements, and SanctumQuestActorPlacements at the quest). The
    // western of Vale's two other Healers, men with long brown hair in long robes, standing on
    // either side of the stone counter.
    //
    // Mind Read hears a villager in cottage_rooms_a.c call the Sanctum's people so: "The healers
    // are supposed to guard Sol Sanctum." Before the quest: "Our Psynergy was given to us by the
    // sacred power of Mt. Aleph."
    ACTOR_WEST_HEALER,
    // The eastern one. "Psynergy is a power that should only be used for good... Never for evil!"
    ACTOR_EAST_HEALER,
    // The quest (SanctumQuestActorPlacements). Isaac's mother, a woman with fair hair tied up.
    ACTOR_DORA,
    // The Mayor, Garet's grandfather, an old man with white hair and a cane.
    //
    // The Great Healer calls him so: "That is all, Mayor."
    ACTOR_GARETS_GRANDFATHER,
    // Appears to the Great Healer.
    ACTOR_WISE_ONE,
    // A copy of the Great Healer for its afterimages.
    ACTOR_HEALER_AFTERIMAGE,
    // Vault Sanctum and Bilibin Sanctum (SanctumVaultBilibinActorPlacements). The other man of
    // Vault's and Bilibin's Sanctums, who wanders the room.
    //
    // He wears the western Healer's sprite (a man with long brown hair in a long robe). He speaks
    // of the healer as someone else and of "our teachings" and "our meditations". In Vault: "I
    // wonder if we'll ever see our stolen gold statue again..."
    ACTOR_SANCTUM_MEMBER = ACTOR_FIRST_PLACED + 1,
    // Vault Sanctum's gold statue, placed off the map until the thieves are jailed
    // (FLAG_VAULT_THIEVES_JAILED).
    //
    // Its sprite is that of the one a chest gives back in log_rooms_a.c.
    ACTOR_GOLD_STATUE
} SANCTUM_ACTOR_ID;

static void scSanctumAskToAcceptQuest(void);
static void scSanctumReceiveWiseOnesMission(void);
static void scSanctumShowHealerAfterimage(void);
static void scSanctumHideHealerAfterimage(void);
static void scSanctumUpdateHealerAfterimage(ACTOR *actor);
static void scSanctumSpawnOrbitingEffect(Sint32 actorId, Sint32 angleStep);

// How far each sparkle circling the Great Healer goes around him in a tick.
#define SANCTUM_SPARKLE_ORBIT_STEP (TRIG_TURN * 9 / 128)

// The sparks the Wise One gives off as he shrinks away after giving Isaac and Garet their mission,
// one each four ticks.
#define WISE_ONE_VANISH_SPARKS     32
static Sint32        scSanctumChooseRestartOption(void);
static void          scSanctumActOutSolSanctum(void);
static void          scSanctumRaiseItemIcon(Sint32 item, FIXED fixedX, FIXED fixedY, FIXED fixedZ);
static void          scSanctumSpawnActorSpark(Sint32 id);
static void          scSanctumRemoveCounter(void);

// An update function: the actor keeps turned towards the actor whose id its timer holds.
static UNREAD_RESULT scSanctumFaceActorNamedByTimer(ACTOR *actor)
{
    ACTOR *target;

    target        = fdActorGet(actor->timer);
    actor->facing = syAtan(target->z.fixed - actor->z.fixed, target->x.fixed - actor->x.fixed);
    RETURN_UNREAD(0);
}

// The entrances of every town's Sanctum.
const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SanctumEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return SanctumExits;
}

// Who stands in the room depends on the door the party came in by: the town's Healers, the
// gathering after the eruption, or nobody.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    switch (MainRoutineStateWork.entrance)
    {
        case SANCTUM_FROM_VALE:
        case SANCTUM_VALE_AFTER_DEFEAT : return SanctumValeActorPlacements;

        case SANCTUM_FROM_VAULT        :
        case SANCTUM_FROM_VAULT_REVISIT:
        case SANCTUM_FROM_LOG_ROOMS_A  :
        case SANCTUM_FROM_BILIBIN      : return SanctumVaultBilibinActorPlacements;

        case SANCTUM_FROM_VALE_ERUPTION:
        case SANCTUM_FROM_QUEST_RESTART: return SanctumQuestActorPlacements;

        case SANCTUM_FROM_ALTIN        :
        case SANCTUM_FROM_XIAN         : return SanctumAltinXianActorPlacements;

        default                        : return SanctumNoActorPlacements;
    }
}

// The cues by the door the party came in by; Vale's Sanctum also changes once the party has left
// Vale and again once it has reached Kalay.
const CUE *scGetCues(void)
{
    switch (MainRoutineStateWork.entrance)
    {
        case SANCTUM_FROM_VAULT:
        case SANCTUM_FROM_LOG_ROOMS_A  : return SanctumVaultCues;
        case SANCTUM_FROM_VAULT_REVISIT: return SanctumVaultRevisitCues;
        case SANCTUM_FROM_VALE_ERUPTION:
        case SANCTUM_FROM_QUEST_RESTART:
        case SANCTUM_QUEST_UNUSED      : return SanctumQuestCues;
        case SANCTUM_FROM_XIAN         : return SanctumXianCues;
        case SANCTUM_FROM_ALTIN        : return SanctumAltinCues;
        case SANCTUM_FROM_BILIBIN      : return SanctumBilibinCues;
        default                        : break;
    }

    if (gmFlagGet(FLAG_KALAY_REACHED) != 0)
    {
        return SanctumValeCuesAfterKalay;
    }
    if (gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0)
    {
        return SanctumValeCuesAfterLeaving;
    }
    return SanctumValeCues;
}

// Talking to the man in a long robe wandering Vault's Sanctum: while the gold statue is missing he
// starts to say what worries the Healer, then thinks better of it; once it is back he speaks of
// fate guiding those who stray. On the later visit through Vault he has polished it: "It's so
// shiny, you won't even recognize it!"
static void scSanctumTalkAboutGoldenStatue(void)
{
    fdScriptStart();

    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
    {
        fdScriptSetMessageId(MsgSanctumWhenStrayFromYourWorldly);
    }
    else
    {
        fdScriptSetMessageId(MsgSanctumHealerMustWorriedAboutNever);
    }

    if (MainRoutineStateWork.entrance == SANCTUM_FROM_VAULT_REVISIT)
    {
        fdScriptSetMessageId(MsgSanctumPolishedGoldStatueReturnedUs);
    }

    fdActorSetAnimation(ACTOR_SANCTUM_MEMBER, ANIM_STAND);
    fdActorFaceEachOther(ACTOR_SANCTUM_MEMBER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(2);
    fdScriptShowMessage(ACTOR_SANCTUM_MEMBER, 0);
    fdScriptEnd();
}

// Mind Read on the man in a long robe wandering Vault's Sanctum: while the gold statue is missing
// he wonders if they will ever see it again; once it is back he thinks of a child who has awakened
// to their teachings; on the later visit through Vault, of how well the dirty statue cleaned up. He
// stops to be read and wanders on after.
static void scSanctumReadMindAboutGoldenStatue(void)
{
    fdScriptStart();

    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
    {
        fdScriptSetMessageId(MsgSanctumWonderIfEverSeeOur);
    }
    else
    {
        fdScriptSetMessageId(MsgSanctumChildHasAwakenedOurTeachings);
    }

    if (MainRoutineStateWork.entrance == SANCTUM_FROM_VAULT_REVISIT)
    {
        fdScriptSetMessageId(MsgSanctumDirtyGoldenStatueCleanedUp);
    }

    fdActorStop(ACTOR_SANCTUM_MEMBER);
    fdActorSetAnimation(ACTOR_SANCTUM_MEMBER, ANIM_STAND);
    fdScriptWait(2);
    fdScriptShowMessage(ACTOR_SANCTUM_MEMBER, 0);
    fdActorSetMovementTypeById(ACTOR_SANCTUM_MEMBER, MOVEMENT_TYPE_WANDER);
    fdScriptEnd();
}

// Talking to the Great Healer after Isaac has refused the quest: he asks "Are you sure?", then
// whether Isaac would like to think it over, then whether he will take the fate of the world in his
// hands. A yes to the first keeps the refusal; no, yes and yes accept. Isaac then walks back to his
// place before the Great Healer, who first steps aside if he was turned to the altar, and the Wise
// One gives the mission as after a first yes.
static void scSanctumAskQuestAgain(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgSanctumAreYouSure);
    // "Are you sure?"
    fdScriptOpenMessage(ACTOR_HEALER, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(20);
    }
    else
    {
        fdScriptWait(20);
        // "Would you like to think it over?"
        fdScriptOpenMessage(ACTOR_HEALER, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptWait(20);
            // "Isaac, will you take the fate of the world in your hands?"
            fdScriptOpenMessage(ACTOR_HEALER, 0);
            if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
            {
                fdScriptWait(20);
                if (fdActorGet(ACTOR_HEALER)->facing >= FACING_NORTHWEST
                    && fdActorGet(ACTOR_HEALER)->facing <= FACING_NORTHEAST)
                {
                    fdActorSetSpeed(ACTOR_HEALER, 0.5);
                    fdActorFaceDirection(ACTOR_HEALER, FACING_EAST, 0);
                    fdScriptWait(10);
                    fdActorGet(ACTOR_HEALER)->steeringFlags &= ~ACTOR_FACES_MOTION;
                    fdActorWalkToAndWait(ACTOR_HEALER, 152, 120);
                    fdScriptWait(1);
                    fdActorGet(ACTOR_HEALER)->steeringFlags |= ACTOR_FACES_MOTION;
                    fdScriptWait(20);
                    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
                    fdScriptWait(20);
                    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 168, 120);
                    fdActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    fdScriptWait(20);
                    fdActorWalkToAndWait(ACTOR_HEALER, 168, 120);
                    fdActorFaceDirection(ACTOR_HEALER, FACING_SOUTH_SOUTHEAST, 0);
                    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
                }
                else
                {
                    fdActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
                    fdScriptWait(20);
                    fdActorFaceDirection(ACTOR_HEALER, FACING_SOUTH_SOUTHEAST, 0);
                    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
                }
                scSanctumReceiveWiseOnesMission();
                fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
                fdPaletteFadeStartInterpolation(120);
                fdScriptWait(120);
                sySoundPlay(MUS_OVERNIGHT_AT_THE_INN);
                sySoundWaitForBGMRestore();
                gmFlagSet(FLAG_SANCTUM_QUEST_ACCEPTED);
                syImportScriptSetExitRequest(SANCTUM_TO_VALE_PLAZA);
            }
        }
    }
    fdScriptEnd();
}

// Who answers "......" when the healer asks whether Isaac's new friends are Adepts: Ivan in every
// edition but the Japanese, Garet before.
#define SANCTUM_NEW_FRIENDS_SPEAKER                                                                \
    WHEN(IVAN_REACTS_TO_NEW_FRIENDS_QUESTION, ACTOR_IVAN, ACTOR_GARET)

// Talking to the Healer on the west side of Vale's Sanctum once the party has reached Kalay:
// "Isaac, are your new friends Adepts by any chance?" A yes makes him marvel that others besides
// Valeans have Psynergy. A no draws a silent "......" from two of the party, and he sees that
// Isaac's friend is angered by his words. The silent pair is Ivan and Mia in the editions of the
// first patch on, which first bring Ivan into the room; before it, Garet and Mia.
static void scSanctumAskAboutNewAdepts(void)
{
    fdScriptStart();
#if IVAN_REACTS_TO_NEW_FRIENDS_QUESTION
    fdScriptAppendActorPlacements(SanctumIvanActorPlacement);
    sySynch(1);
#endif
    fdScriptSetMessageId(MsgSanctumIsaacYourNewFriendsAdepts);
    // "Isaac, are your new friends Adepts by any chance?"
    fdScriptOpenMessage(ACTOR_WEST_HEALER, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        // "So, there are others besides Valeans who have been granted..."
        fdScriptShowMessage(ACTOR_WEST_HEALER, 0);
    }
    else
    {
        FieldStateWork->msg++;
        // "......" from both at once.
        syFieldScriptShowTwoMessagesAndWait(
            SANCTUM_NEW_FRIENDS_SPEAKER, 16, 1, 24, 1, ACTOR_MIA, 7, 16, 1, 14, 0);
        // "Your friend seems angered by your words, Isaac... That..."
        fdScriptShowMessage(ACTOR_WEST_HEALER, 0);
    }
    fdScriptEnd();
}

// Talking to the Healer on the east side of Vale's Sanctum once the party has left Vale: he turns
// to Isaac and asks "Do you feel any change in your Psynergy?" A yes makes him wish the Wise One
// would come to him too; a no, recall that the Great Healer's powers grew when he met the Wise One.
static void scSanctumAskAboutPsynergyChanges(void)
{
    fdScriptStart();
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(10);
    fdScriptSetMessageId(MsgSanctumDoFeelAnyChangeIn);
    // "Do you feel any change in your Psynergy?"
    fdScriptOpenMessage(ACTOR_EAST_HEALER, 0);

    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
    {
        (FieldStateWork->msg)++;
    }

    fdScriptShowMessage(ACTOR_EAST_HEALER, 0);
    fdScriptEnd();
}

// Every visit but a plain one to Vale's Sanctum opens without the area name. In Vault's, the gold
// statue stands behind the Healer once the thieves are jailed. Brought here after the eruption, or
// starting again after refusing, the stone counter is taken away and the Great Healer's question
// begins; a return from a battle or the menu (FLAG_KEEP_PARTY_POSITION) skips it.
Sint32 scInit(void)
{
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(1);
    fdScriptWait(1);
    switch (MainRoutineStateWork.entrance)
    {
        case SANCTUM_FROM_VAULT:
        case SANCTUM_FROM_VAULT_REVISIT:
        case SANCTUM_FROM_LOG_ROOMS_A:
            if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
            {
                fdActorSetPos(ACTOR_GOLD_STATUE, 200, 80);
            }
            gmFlagClear(FLAG_SHOW_AREA_NAME);
            break;
        case SANCTUM_FROM_VALE_ERUPTION:
            scSanctumRemoveCounter();
            if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
            {
                scSanctumAskToAcceptQuest();
            }
            // Fall through.
        case SANCTUM_FROM_ALTIN:
        case SANCTUM_FROM_XIAN:
        case SANCTUM_FROM_BILIBIN: gmFlagClear(FLAG_SHOW_AREA_NAME); break;
        case SANCTUM_FROM_QUEST_RESTART:
            scSanctumRemoveCounter();
            gmFlagSet(FLAG_SANCTUM_QUEST_RESTARTED);
            if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
            {
                scSanctumAskToAcceptQuest();
            }
            gmFlagClear(FLAG_SHOW_AREA_NAME);
            break;
    }
    return 0;
}

// When Isaac heads for the door of Vale's Sanctum after refusing the quest (the row just inside it,
// metatiles (10, 15) to (14, 15)), the view pans back up the room, the Great Healer turns to him
// and shakes his head: "Once you step outside the village, you cannot take back your decision." He
// warns only once (FLAG_SANCTUM_WARNED_BEFORE_LEAVING).
static void scSanctumWarnBeforeLeaving(void)
{
    fdScriptStart();
    fdCameraSetSpeed(1);
    fdCameraMoveToActorById(ACTOR_GARET, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdScriptSetMessageId(MsgSanctumOnceStepOutsideVillageCannot);
    // "Once you step outside the village, you cannot take back your..."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    gmFlagSet(FLAG_SANCTUM_WARNED_BEFORE_LEAVING);
    fdScriptEnd();
}

// The last and palest colour of the room's first palette, a pale cream on its brightest highlights.
#define ROOM_PALEST_COLOR 15

// Stepping into the doorway of Vale's Sanctum after refusing the quest (metatiles (10, 16) to (14,
// 16)) ends the game. Everyone freezes and the room fades to sepia, with its palest colour turning
// white; "And so the world began drifting toward its fated destruction..." shows across the screen,
// the screen fades to black, and the player chooses to start again "From the Great Healer" or to go
// "To the File Screen" ("Restart the game" when there is no saved file).
static void scSanctumLeaveAfterRefusing(void)
{
    PALETTE_FADE *paletteState;

    fdScriptStart();
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STILL);
    fdActorSetAnimation(ACTOR_DORA, ANIM_STILL);
    fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_STILL);
    fdActorSetAnimation(ACTOR_HEALER, ANIM_STILL);
    fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_STILL);
    fdActorSetAnimation(ACTOR_EAST_HEALER, ANIM_STILL);
    fdPaletteFadeApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(120);
    fdScriptWait(180);
    paletteState                                    = PaletteFadeState;
    // ROOM_PALEST_COLOR fades to white rather than sepia: its blue, green and red channels, each
    // held in blue's place.
    paletteState->target[3 * ROOM_PALEST_COLOR]     = RGB_LEVEL_MAX << BLUE_SHIFT;
    paletteState->target[3 * ROOM_PALEST_COLOR + 1] = RGB_LEVEL_MAX << BLUE_SHIFT;
    paletteState->target[3 * ROOM_PALEST_COLOR + 2] = RGB_LEVEL_MAX << BLUE_SHIFT;
    paletteState->page                              = 0;
    paletteState->duration                          = 1;
    paletteState->step                              = 1;
    // DEAD: a byte nothing reads set to 1; the ROM keeps its store.
    paletteState->unusedAfterStep                   = 1;
    fdScriptWait(1);
    mnFieldMessageShowCentered(MsgSanctumWorldBeganDrifting, 1, 0);
    fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    fdPaletteFadeStartInterpolation(120);
    fdScriptWait(120);
    fdScriptWait(60);
    if (scSanctumChooseRestartOption() == 0)
    {
        fdScriptEnd();
        syImportScriptSetExitRequest(SANCTUM_TO_QUEST_RESTART);
    }
    else
    {
        fdScriptEnd();
        syImportScriptSetExitRequest(SANCTUM_TO_FILE_SCREEN);
    }
}

// Keeps this actor at the party leader's drawing priority.
static UNREAD_RESULT scSanctumMatchLeaderPriority(ACTOR *actor)
{
    actor->sprite->image.priority  = fdActorGet(ACTOR_PARTY_LEADER)->sprite->image.priority;
    actor->sprite->shadow.priority = fdActorGet(ACTOR_PARTY_LEADER)->sprite->image.priority;
    RETURN_UNREAD(0);
}

// Has Isaac, Garet, Dora, the Mayor and the two healers nod, waiting for the last of them.
static inline void scSanctumNodAll(void)
{
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
}

// Starts the same six's repeated motions and waits for the last of them.
static inline void scSanctumStartAllRepeatedMotion(void)
{
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    fdActorStartRepeatedMotion(ACTOR_GARET, 1);
    fdActorStartRepeatedMotion(ACTOR_DORA, 1);
    fdActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    fdActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
    fdActorRunRepeatedMotion(ACTOR_EAST_HEALER, 1);
}

// Shows the same emote on the same six.
static inline void scSanctumShowAllEmotes(void)
{
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
}

// Back in Vale's Sanctum after the eruption, Isaac and Garet tell the Great Healer, Dora, the Mayor
// and the Healers what happened in Sol Sanctum, and the Great Healer asks Isaac to take on the
// quest. The account of Jenna and Kraden's capture is acted out in sepia, and the Great Healer
// tells of the hovering stone, the Wise One, who showed him all of it. Starting again after a
// refusal skips straight to the question. A yes gives the mission; a no leaves Isaac free to walk
// out.
static void scSanctumAskToAcceptQuest(void)
{
    Uint32 effectIndex;

    fdScriptStart();
    // A repeat visit skips the account of Sol Sanctum.
    if (gmFlagGet(FLAG_SANCTUM_QUEST_RESTARTED) != 0)
    {
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
        fdActorFaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
        fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
        fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
        fdCameraMoveTo(192, CAMERA_KEEP, 160, 1);
        fdCameraWaitForMove();
        FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        FieldStateWork->transitionFrame = 64;
        fdScriptStartScreenTransition();
        fdScriptWaitTransitionTicks();
        fdScriptWait(120);
    }
    else
    {
        fdPaletteFadeApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
        fdPaletteFadeStartInterpolation(1);
        fdScriptWait(1);
        fdCameraMoveTo(192, CAMERA_KEEP, 160, 1);
        fdCameraWaitForMove();
        FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
        fdScriptStartScreenTransition();
        fdScriptWaitTransitionTicks();
        scSanctumActOutSolSanctum();
        fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
        fdPaletteFadeStartInterpolation(60);
        fdScriptWait(100);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
        fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
        fdScriptWait(30);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorSetMovementTypeById(ACTOR_HEALER, MOVEMENT_TYPE_STAND);
        fdActorSetMovementTypeById(ACTOR_GARETS_GRANDFATHER, MOVEMENT_TYPE_STAND);
        fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
        fdActorSetSpeed(ACTOR_GARET, 0.8);
        fdActorSetSpeed(ACTOR_DORA, 0.8);
        fdActorSetSpeed(ACTOR_GARETS_GRANDFATHER, 0.8);
        fdActorSetSpeed(ACTOR_WEST_HEALER, 0.8);
        fdActorSetSpeed(ACTOR_EAST_HEALER, 0.8);
        fdActorSetSpeed(ACTOR_HEALER, 0.8);
        fdScriptWait(30);
        fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
        fdScriptSetMessageId(MsgSanctumWeHadIdeaTrueSanctum);
        // "We had no idea... The true sanctum..."
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(10);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
        fdScriptWait(30);
        fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_DORA, 2);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_DORA, 0);
        fdScriptWait(20);
        // "And Jenna and Kraden... They're prisoners?"
        fdScriptOpenMessage(ACTOR_DORA, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptWait(20);
            fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
            fdScriptWait(20);
            // "They've been taken against their will... How awful!"
            fdScriptShowMessage(ACTOR_DORA, 0);
            FieldStateWork->msg++;
        }
        else
        {
            fdScriptWait(20);
            fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
            fdScriptWait(20);
            FieldStateWork->msg++;
            // "Well...we can only hope they're all right."
            fdScriptShowMessage(ACTOR_DORA, 0);
        }
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_WEST_HEALER, 1);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_WEST_HEALER, 0);
        fdActorFaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
        fdScriptWait(20);
        // "Do you think they can be saved?"
        fdScriptOpenMessage(ACTOR_WEST_HEALER, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptWait(20);
            fdActorSetAnimationAndWait(ACTOR_WEST_HEALER, ANIM_NOD);
            fdScriptWait(20);
            // "Jenna's brother, Felix, will protect them, won't he?"
            fdScriptShowMessage(ACTOR_WEST_HEALER, 0);
            FieldStateWork->msg++;
        }
        else
        {
            fdScriptWait(20);
            fdActorSetAnimationAndWait(ACTOR_WEST_HEALER, ANIM_SHAKE_HEAD);
            fdScriptWait(20);
            FieldStateWork->msg++;
            // "Then our hopes must lie with Felix."
            fdScriptShowMessage(ACTOR_WEST_HEALER, 0);
        }
        fdScriptWait(20);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
        fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
        fdActorSetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdScriptWait(60);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorWalkToAndWait(ACTOR_GARETS_GRANDFATHER, 224, 120);
        fdScriptWait(10);
        // "What is it, Great Healer? You have been very quiet."
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_DORA, 1);
        fdScriptWait(10);
        // "It's like you're a thousand miles away."
        fdScriptShowMessage(ACTOR_DORA, 0);
        fdScriptWait(30);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 3);
        fdActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdScriptWait(60);
        fdActorFaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(10);
        // "That hovering stone, the one that appeared before Isaac..."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHWEST, 0);
        fdScriptWait(60);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
        fdScriptWait(10);
        // "What about it?"
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_HEALER, FACING_SOUTH_SOUTHEAST, 0);
        fdScriptWait(10);
        // "I have seen it before in my dreams."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHWEST, 0);
        fdScriptWait(60);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
        fdScriptWait(50);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdScriptWait(40);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdScriptWait(60);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 1);
        fdScriptWait(10);
        // "In my mind's eye, it appeared again just now and spoke to me."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(10);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdScriptWait(60);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(10);
        // "You saw it just now?"
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(10);
        // "It must be the Wise One, guardian of the Elemental Stars."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(10);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        fdActorStartRepeatedMotion(ACTOR_GARET, 1);
        fdActorStartRepeatedMotion(ACTOR_DORA, 1);
        fdActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
        fdActorRunRepeatedMotion(ACTOR_EAST_HEALER, 1);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
        fdScriptWait(10);
        // "It told me what happened to you in Sol Sanctum."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(30);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
        fdActorFaceActor(ACTOR_DORA, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        fdScriptWait(60);
        fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
        fdScriptWait(20);
        // "So, what does the Wise One say about the eruption?"
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(30);
        fdActorRunRepeatedMotion(ACTOR_DORA, 2);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(20);
        // "Will the volcano destroy Vale?"
        fdScriptShowMessage(ACTOR_DORA, 0);
        fdScriptWait(20);
        fdScriptWait(40);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 2);
        fdActorSetAnimation(ACTOR_HEALER, ANIM_STILL);
        fdActorSetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_BLINK);
        scSanctumShowHealerAfterimage();
        sySoundPlay(SE_WISE_ONE_QUEST);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdScriptWait(32);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdScriptWait(16);
        for (effectIndex = 0; effectIndex < 6; effectIndex++)
        {
            scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
            fdScriptWait(8);
        }
        fdScriptWait(8);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdScriptWait(32);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdScriptWait(96);
        fdScriptWait(32);
        fdActorSetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_NORMAL);
        fdScriptWait(30);
        scSanctumHideHealerAfterimage();
        fdActorSetAnimation(ACTOR_HEALER, ANIM_STAND);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 2);
        fdScriptWait(20);
        // "It is impossible to stop the eruption now."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorSetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_WEST_HEALER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_EAST_HEALER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdScriptWait(60);
        fdActorRunRepeatedMotion(ACTOR_EAST_HEALER, 1);
        fdScriptWait(20);
        // "Will Vale be..."
        fdScriptShowMessage(ACTOR_EAST_HEALER, 0);
        fdScriptWait(20);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
        fdScriptWait(60);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
        fdScriptWait(20);
        // "Vale will be safe. The Wise One will prevent the lava from reaching us."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(30);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
        fdScriptWait(20);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
        fdActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
        fdScriptWait(20);
        fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_EAST_HEALER, ANIM_NOD);
        fdActorWaitForAnimationChange(ACTOR_EAST_HEALER);
        fdScriptWait(30);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
        fdActorFaceEachOther(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
        fdActorWaitForAnimationChange(ACTOR_GARETS_GRANDFATHER);
        fdScriptWait(60);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 2);
        fdActorSetAnimation(ACTOR_HEALER, ANIM_STILL);
        fdActorSetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_BLINK);
        scSanctumShowHealerAfterimage();
        sySoundPlay(SE_WISE_ONE_QUEST);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdScriptWait(32);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
        fdScriptWait(16);
        fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        fdActorStartRepeatedMotion(ACTOR_GARET, 1);
        fdActorStartRepeatedMotion(ACTOR_DORA, 1);
        fdActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
        fdActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
        fdActorStartRepeatedMotion(ACTOR_EAST_HEALER, 1);
        for (effectIndex = 0; effectIndex < 6; effectIndex++)
        {
            scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
            fdScriptWait(8);
        }
        fdScriptWait(8);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdScriptWait(32);
        scSanctumSpawnOrbitingEffect(ACTOR_HEALER, SANCTUM_SPARKLE_ORBIT_STEP);
        fdScriptWait(128);
        fdActorSetSpriteColorById(ACTOR_HEALER, SPRITE_COLOR_NORMAL);
        fdScriptWait(30);
        scSanctumHideHealerAfterimage();
        fdActorSetAnimation(ACTOR_HEALER, ANIM_STAND);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 2);
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        fdScriptWait(20);
        fdActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
        fdScriptWait(40);
        fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
        // "What's the matter, Great Healer?"
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_DORA, 2);
        fdScriptWait(20);
        // "...The village will be destroyed, won't it?"
        fdScriptShowMessage(ACTOR_DORA, 0);
        fdScriptWait(20);
        fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        fdActorStartRepeatedMotion(ACTOR_GARET, 1);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
        fdActorStartRepeatedMotion(ACTOR_WEST_HEALER, 1);
        fdActorStartRepeatedMotion(ACTOR_EAST_HEALER, 1);
        fdActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
        fdActorSetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdScriptWait(60);
        // "Something even more dreadful than Vale's destruction looms ahead."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
        fdScriptWait(20);
        // "What could be worse than the destruction of Vale?"
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(20);
        // "A terrible power."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdScriptWait(60);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "The Elemental Star gems house incredible power."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
        fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
        fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
        fdActorSetAnimation(ACTOR_EAST_HEALER, ANIM_NOD);
        fdActorWaitForAnimationChange(ACTOR_EAST_HEALER);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
        fdScriptWait(20);
        // "...And Vale has been guarding those gems?"
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(20);
        // "The elemental lighthouses will loose that power upon the world."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(50);
        fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(20);
        // "Once the power of the gems has been released, the whole world will..."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdScriptWait(80);
        fdActorRunRepeatedMotion(ACTOR_DORA, 1);
        fdScriptWait(20);
        // "The Wise One wants us to get the gems back, doesn't he?"
        fdScriptShowMessage(ACTOR_DORA, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdScriptWait(80);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 1);
        fdScriptWait(20);
        // "This world is ours, to save or to destroy. Our inaction may bring..."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_GARETS_GRANDFATHER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_WEST_HEALER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorSetAttachedEffect(ACTOR_EAST_HEALER,
                                 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdScriptWait(60);
        fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "What a pity..."
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 1);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "We cannot hope for salvation. We must save ourselves..."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(40);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
        fdActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(60);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_DORA, 1);
        fdScriptWait(20);
        // "Then what can we do?"
        fdScriptShowMessage(ACTOR_DORA, 0);
        fdScriptWait(60);
        fdActorWalkToAndWait(ACTOR_HEALER, 200, 136);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
        fdActorFaceEachOther(ACTOR_HEALER, ACTOR_GARET, 0);
        fdScriptWait(40);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(50);
        fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(40);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(50);
        fdActorRunRepeatedMotion(ACTOR_HEALER, 1);
        fdScriptWait(20);
        // "Acquiring the Elemental Stars is their fate alone."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdScriptWait(60);
        fdActorSetAttachedEffect(ACTOR_DORA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorRunRepeatedMotion(ACTOR_DORA, 1);
        // "Are you suggesting we place the fate of the world... in the hands..."
        fdScriptShowMessage(ACTOR_DORA, 0);
        fdScriptWait(30);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
        fdScriptWait(20);
        // "Garet is only a child. You can't expect him to bear such a burden..."
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(30);
        fdActorWalkToAndWait(ACTOR_HEALER, 168, 120);
        fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
        fdActorFaceDirection(ACTOR_HEALER, FACING_NORTH_NORTHEAST, 0);
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "The Wise One has spoken."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
        fdScriptWait(60);
        fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
        fdScriptWait(10);
        // "Each of you shares responsibility for this."
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(10);
        fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(20);
        // "Now, each of you must make your own decision!"
        fdScriptShowMessage(ACTOR_HEALER, 0);
        fdScriptWait(20);
        fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
        fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
        fdScriptWait(20);
        // "I don't know what to do. Isaac, you decide."
        fdScriptShowMessage(ACTOR_GARET, 0);
        fdScriptWait(30);
    }
    fdScriptSetMessageId(MsgSanctumIsaacWillAcceptResponsibilityFor);
    fdActorRunRepeatedMotion(ACTOR_HEALER, 1);
    fdScriptWait(20);
    // "Isaac, will you accept responsibility for the fate of the land?"
    fdScriptOpenMessage(ACTOR_HEALER, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        scSanctumReceiveWiseOnesMission();
        fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
        fdPaletteFadeStartInterpolation(120);
        fdScriptWait(120);
        sySoundPlay(MUS_OVERNIGHT_AT_THE_INN);
        sySoundWaitForBGMRestore();
        gmFlagSet(FLAG_SANCTUM_QUEST_ACCEPTED);
        syImportScriptSetExitRequest(SANCTUM_TO_VALE_PLAZA);
    }
    else
    {
        fdScriptWait(30);
        fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
        fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "I can't blame you... I couldn't do it if I were in your shoes."
        fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
        fdScriptWait(20);
        fdActorGet(ACTOR_EAST_HEALER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        fdActorGet(ACTOR_EAST_HEALER)->update = (void (*)(ACTOR *))scSanctumMatchLeaderPriority;
    }
    fdScriptEnd();
}

// Once Isaac accepts, Garet protests and the Mayor tells him the choice was Isaac's; the Great
// Healer calls on the Wise One, who comes down over the altar to give Isaac and Garet their mission
// while a red glow shimmers about the Great Healer and sparkles circle him.
static void scSanctumReceiveWiseOnesMission(void)
{
    Uint32 sparkIndex;

    fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
    fdActorSetSpeed(ACTOR_GARET, 0.8);
    fdActorSetSpeed(ACTOR_DORA, 0.8);
    fdActorSetSpeed(ACTOR_GARETS_GRANDFATHER, 0.8);
    fdActorSetSpeed(ACTOR_WEST_HEALER, 0.8);
    fdActorSetSpeed(ACTOR_EAST_HEALER, 0.8);
    fdActorSetSpeed(ACTOR_HEALER, 0.8);
    fdCameraMoveTo(192, CAMERA_KEEP, 160, 1);
    fdCameraWaitForMove();
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    fdScriptWait(60);
    fdActorSetAnimation(ACTOR_HEALER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
    fdScriptWait(50);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // Garet and the Mayor react to Isaac accepting the burden.
    fdScriptSetMessageId(MsgSanctumAcceptIsaacCantMean);
    // "You accept? Oh, Isaac... You can't mean it..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_GARET, 0);
    fdActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    fdScriptWait(60);
    // "You chose to leave your decision to Isaac. There shall be no..."
    fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(40);
    fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_HEALER, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    fdScriptWait(30);
    // "O, Wise One, these two have accepted your quest. Give us..."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(80);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdPaletteFadeApplyTarget(PALETTE_TINT_PALE, PALETTES_BG);
    fdPaletteFadeStartInterpolation(60);
    fdScriptWait(40);
    fdCameraSetSpeed(0.4);
    fdCameraMoveTo(192, CAMERA_KEEP, 104, 1);
    fdScriptWait(120);
    sySoundPlay(MUS_FOREST_THEME);
    fdScriptPlaySound(SE_MATERIALIZE);
    // The Wise One enters while the healer's afterimages appear.
    fdActorSetPos(ACTOR_WISE_ONE, 200, 8);
    fdActorSetSpeed(ACTOR_WISE_ONE, 0.4);
    fdActorMoveToAndWait(ACTOR_WISE_ONE, 200, 72);
    sySoundPlay(SE_STOP_ACTION_SE);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_HEALER, 2);
    fdActorSetAnimation(ACTOR_HEALER, ANIM_STILL);
    scSanctumShowHealerAfterimage();
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WISE_ONE, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_WISE_ONE, 0);
    fdActorFaceActor(ACTOR_DORA, ACTOR_WISE_ONE, 0);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_WISE_ONE, 0);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_WISE_ONE, 0);
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_WISE_ONE, 0);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorStartRepeatedMotion(ACTOR_DORA, 2);
    fdActorStartRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    fdActorStartRepeatedMotion(ACTOR_WEST_HEALER, 2);
    fdActorStartRepeatedMotion(ACTOR_EAST_HEALER, 2);
    fdScriptWait(40);
    // "You two are quite brave to take on this quest."
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(20);
    fdScriptWait(40);
    // "I shall be watching you. Choose your destiny wisely, young ones."
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(60);
    fdActorSetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_BLINK);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    sySoundPlay(SE_MATERIALIZE);
    for (sparkIndex = 0; sparkIndex < WISE_ONE_VANISH_SPARKS; sparkIndex++)
    {
        scSanctumSpawnActorSpark(ACTOR_WISE_ONE);
        fdScriptWait(4);
        fdActorGet(ACTOR_WISE_ONE)->scaleX -= toFIXED(1) / 100;
        fdActorGet(ACTOR_WISE_ONE)->scaleY -= toFIXED(1) / 100;
    }
    sySoundPlay(SE_STOP_ACTION_SE);
    fdActorSetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
    fdActorSetPos(ACTOR_WISE_ONE, 0, 0);
    fdScriptWait(40);
    fdCameraMoveTo(192, CAMERA_KEEP, 160, 1);
    fdCameraWaitForMove();
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(60);
    fdScriptWait(120);
    scSanctumHideHealerAfterimage();
    fdActorSetAnimation(ACTOR_HEALER, ANIM_STAND);
    sySoundPlay(MUS_AN_UNSETTLING_FEELING);
    fdScriptWait(60);
    fdActorRunRepeatedMotion(ACTOR_HEALER, 2);
    fdScriptWait(20);
    scSanctumStartAllRepeatedMotion();
    fdScriptWait(30);
    // "That was the Wise One..."
    fdScriptShowMessage(ACTOR_DORA, 0);
    fdScriptWait(30);
    fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(30);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    fdScriptWait(20);
    // "What did the Wise One say?"
    fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    fdScriptWait(30);
    // "Felix must unlock the power."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(20);
    scSanctumShowAllEmotes();
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(30);
    // "Find the lighthouses... You'll have the guidance of the Elemental Star..."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(20);
    scSanctumNodAll();
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    fdScriptWait(180);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdScriptWait(60);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    fdActorFaceEachOther(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    fdScriptWait(40);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    fdScriptWait(40);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    // "Um... Then what?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(60);
    scSanctumStartAllRepeatedMotion();
    fdScriptWait(30);
    // "What should they do there?"
    fdScriptShowMessage(ACTOR_DORA, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(80);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    // "Surely, that cannot be all the advice you can offer them, Great Healer..."
    fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdScriptWait(60);
    fdActorRunRepeatedMotion(ACTOR_HEALER, 1);
    fdActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
    fdScriptWait(20);
    scSanctumStartAllRepeatedMotion();
    fdScriptWait(40);
    // "Seek the elementals... The Djinn..."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(20);
    scSanctumNodAll();
    fdScriptWait(30);
    // "They will prove useful."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(30);
    scSanctumNodAll();
    fdScriptWait(120);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(20);
    // "Isn't there anything else?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    fdScriptWait(20);
    // "That is all."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdScriptWait(60);
    fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 1);
    // "That's it!? Isn't there anything else you can tell them?"
    fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_HEALER, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "That is all, Mayor."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdScriptWait(60);
    fdActorFaceActor(ACTOR_HEALER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_HEALER, ANIM_NOD);
    // "I leave it up to you, Isaac...Garet."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptWait(40);
    fdActorWalkToAndWait(ACTOR_HEALER, 168, 176);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_DORA, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_HEALER, 0);
    fdActorWalkToAndWait(ACTOR_HEALER, 200, 200);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_HEALER, 0);
    fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_WEST_HEALER, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_EAST_HEALER, FACING_WEST, 0);
    fdActorWalkTo(ACTOR_HEALER, 200, 272);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorWaitForMoveById(ACTOR_HEALER);
    fdActorSetPos(ACTOR_HEALER, 0, 0);
    fdScriptWait(60);
    scSanctumShowAllEmotes();
    fdScriptWait(60);
    fdScriptWait(120);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_EAST_HEALER, 0);
    fdActorFaceActor(ACTOR_DORA, ACTOR_WEST_HEALER, 0);
    fdActorFaceActor(ACTOR_GARETS_GRANDFATHER, ACTOR_WEST_HEALER, 0);
    fdScriptWait(120);
    fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdScriptWait(60);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_WEST_HEALER, 2);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_WEST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_WEST_HEALER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(80);
    fdActorFaceActor(ACTOR_WEST_HEALER, ACTOR_EAST_HEALER, 0);
    fdActorRunRepeatedMotion(ACTOR_WEST_HEALER, 1);
    fdScriptWait(20);
    // "H-Hey, isn't it almost... time to meditate?"
    fdScriptShowMessage(ACTOR_WEST_HEALER, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_EAST_HEALER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(60);
    fdActorRunRepeatedMotion(ACTOR_WEST_HEALER, 2);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_EAST_HEALER, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_EAST_HEALER, 2);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_EAST_HEALER, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(20);
    // "Oh yeah! Meditation! Let's go!"
    fdScriptShowMessage(ACTOR_EAST_HEALER, 0);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
    fdScriptWait(20);
    fdActorWalkTo(ACTOR_WEST_HEALER, 200, 272);
    fdActorWalkToAndWait(ACTOR_EAST_HEALER, 200, 272);
    fdActorSetPos(ACTOR_WEST_HEALER, 0, 0);
    fdActorSetPos(ACTOR_EAST_HEALER, 0, 0);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    fdScriptWait(60);
    fdActorWalkToAndWait(ACTOR_GARETS_GRANDFATHER, 200, 136);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdActorFaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Well, what good are those guys!? Sending children off with no help..."
    fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(20);
    // "I agree, but..."
    fdScriptShowMessage(ACTOR_DORA, 0);
    fdScriptWait(20);
    fdActorWalkToAndWait(ACTOR_DORA, 168, 168);
    fdActorFaceActor(ACTOR_DORA, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_DORA, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "They didn't give us any other choice."
    fdScriptShowMessage(ACTOR_DORA, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    // "Then, when should they go?"
    fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_DORA, 2);
    fdScriptWait(20);
    // "It doesn't sound like we can afford to wait."
    fdScriptShowMessage(ACTOR_DORA, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    fdActorShowEmote(ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    // "Tomorrow, then?"
    fdScriptShowMessage(ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdScriptWait(60);
    fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    fdScriptWait(20);
    // "Tomorrow? Yes."
    fdScriptShowMessage(ACTOR_DORA, 0);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
}

// Puts a red-tinted copy of the Great Healer on him, the glow of his Psynergy.
static void scSanctumShowHealerAfterimage(void)
{
    Uint32 entryIndex;
    ACTOR *afterimage;
    ACTOR *actor;
    SPR   *sprite;

    actor = fdActorGet(ACTOR_HEALER);
    if (actor != NULL)
    {
        fdActorSetPosFX(ACTOR_HEALER_AFTERIMAGE, actor->x.fixed, actor->z.fixed);
    }
    fdActorSetAnimation(ACTOR_HEALER_AFTERIMAGE, ANIM_STILL);
    afterimage                                  = fdActorGet(ACTOR_HEALER_AFTERIMAGE);
    afterimage->facing                          = fdActorGet(ACTOR_HEALER)->facing;
    fdActorGet(ACTOR_HEALER_AFTERIMAGE)->update = scSanctumUpdateHealerAfterimage;
    sprite                                      = fdActorGet(ACTOR_HEALER_AFTERIMAGE)->sprite;
    for (entryIndex = 0; entryIndex < sprite->count; entryIndex++)
    {
        SPR_ANIMATION *entry = sprite->entries[entryIndex];

        if (entry != NULL && entry->script != 0)
        {
            entry->colorMode = SPRITE_COLOR_RED_TINT;
        }
    }
    sprite->dirty                                       = 1;
    fdActorGet(ACTOR_HEALER_AFTERIMAGE)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->image.priority                              = 2;
}

// Takes the Great Healer's red glow away.
static void scSanctumHideHealerAfterimage(void)
{
    ACTOR *state  = fdActorGet(ACTOR_HEALER_AFTERIMAGE);
    state->update = 0;
    fdActorSetPos(ACTOR_HEALER_AFTERIMAGE, 0, 0);
}

// Each tick moves the red copy a few pixels to another side of the Great Healer, so that it
// shimmers about him.
static void scSanctumUpdateHealerAfterimage(ACTOR *actor)
{
    ACTOR *healer;

    healer         = fdActorGet(ACTOR_HEALER);
    actor->targetX = actor->x.fixed = healer->x.fixed;
    actor->targetY = actor->y.fixed = healer->y.fixed;
    actor->targetZ = actor->z.fixed = healer->z.fixed - toFIXED(2);
    switch (TickCount & 3)
    {
        case 0: actor->targetX = actor->x.fixed = healer->x.fixed - toFIXED(7) / 2; break;
        case 1: actor->targetX = actor->x.fixed = healer->x.fixed + toFIXED(3); break;
        case 2: actor->targetY = actor->y.fixed = healer->y.fixed + toFIXED(2); break;
        case 3: actor->targetZ = actor->z.fixed = healer->z.fixed; break;
    }
}

// Moves a sparkle one step along its ellipse around the Great Healer.
static void scSanctumUpdateOrbitingEffect(ACTOR *effect)
{
    ACTOR *anchor     = (ACTOR *)effect->watchedActor;
    Uint16 orbitAngle = effect->timer;
    FIXED  x;
    FIXED  z;
    FIXED  currentX;

    x                = anchor->x.fixed + syCos(orbitAngle) * 14;
    effect->x.fixed  = x;

    z                = anchor->z.fixed + sySin(orbitAngle) * 10;
    currentX         = effect->x.fixed;
    effect->z.fixed  = z;
    effect->targetZ  = z;
    effect->targetX  = currentX;

    effect->timer   += effect->userValue;
}

// Starts a sparkle circling the actor, high above his feet, going around by the given angle each
// tick.
static void scSanctumSpawnOrbitingEffect(Sint32 actorId, Sint32 angleStep)
{
    ACTOR *center;
    ACTOR *effect;
    SPR   *sprite;
    SPR   *sourceSprite;

    center = fdActorGet(actorId);
    if (center != NULL)
    {
        effect = syFieldActorCreate(
            SPRITE_SPARKLE, center->x.fixed, center->y.fixed + toFIXED(45), center->z.fixed);
        if (effect != NULL)
        {
            sprite = effect->sprite;
            fdActorStartScript(effect, SanctumOrbitingSparkleScript);
            effect->motionFlags    = 0;
            effect->timer          = 0;
            effect->userValue      = angleStep;
            effect->update         = scSanctumUpdateOrbitingEffect;
            sprite->flags          = 0;
            sourceSprite           = center->sprite;
            effect->watchedActor   = (ACTOR_OR_EFFECT *)center;
            sprite->image.priority = sourceSprite->image.priority;
        }
    }
}

// The question's window after the refusal ending, in tiles, and its cursor's x in pixels.
#define QUESTION_X     JAPANESE_OR(7, 2)
#define QUESTION_WIDTH JAPANESE_OR(17, 25)
#define CURSOR_X       JAPANESE_OR(64, 24)

// After the refusal ending, clears the room and asks whether to start again "From the Great Healer"
// (0) or go to the file screen (1).
static Sint32 scSanctumChooseRestartOption(void)
{
    Sint32   slot;
    MENU_SPR cursor;
    WINDOW  *window;
    Sint32   text;
    Sint32   choice;

    fdActorSetPos(ACTOR_HEALER, 0, 0);
    fdActorSetPos(ACTOR_WEST_HEALER, 0, 0);
    fdActorSetPos(ACTOR_EAST_HEALER, 0, 0);
    fdActorSetPos(ACTOR_GARET, 0, 0);
    fdActorSetPos(ACTOR_DORA, 0, 0);
    fdActorSetPos(ACTOR_GARETS_GRANDFATHER, 0, 0);
    fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
    fdPaletteFadeStartInterpolation(1);
    fdScriptWait(1);
    window = mnWindowCreate(QUESTION_X, 7, QUESTION_WIDTH, 5, 1);
    text   = MsgSanctumGreatHealer;
    mnTextDrawMessage(text, window, 16, 0);
    // "To the File Screen", or "Restart the game" when there is no saved file.
    if (mnSaveCountFilesExcludingSent(1) == 0)
    {
        mnTextDrawMessage(text + 2, window, 16, 16);
    }
    else
    {
        mnTextDrawMessage(text + 1, window, 16, 16);
    }
    syFieldMenuArrowCreate(&cursor, &slot);
    syFieldMenuArrowSetPos(&cursor, 72, 60);
    choice = 0;
    while ((PerPush & KEY_A) == 0)
    {
        Sint32 x;

        if (PerRepeat & KEYS_VERTICAL)
        {
            choice ^= 1;
        }
        x = SanctumCursorOffsets[(FrameCount >> 1) & 15] + CURSOR_X;
        syFieldMenuArrowSetPos(&cursor, x, (choice << 4) + 60);
        fdScriptWait(1);
    }
    mnArrowRelease(slot);
    mnWindowClose(window, 1);
    return choice;
}

// Isaac and Garet act out, in sepia, what happened in Sol Sanctum, the item each part of the story
// turns on rising over them.
static void scSanctumActOutSolSanctum(void)
{
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
    fdActorSetSpeed(ACTOR_GARET, 1.5);
    fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(15);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkTo(ACTOR_PARTY_LEADER, 184, 168);
    fdActorGet(ACTOR_GARET)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkToAndWait(ACTOR_GARET, 200, 168);
    fdScriptWait(1);
    fdActorGet(ACTOR_GARET)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_GARET)->steeringFlags        |= ACTOR_FACES_MOTION;
    fdActorJump(ACTOR_GARET, 2, 0);
    fdScriptWait(15);
    fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    fdScriptWait(5);
    fdActorJump(ACTOR_GARET, 2, 0);
    fdScriptWait(25);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(5);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(5);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(15);
    fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_HEALER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_WEST_HEALER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_EAST_HEALER, ANIM_NOD);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_DORA, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    fdScriptWait(15);
    fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdScriptWait(10);
    // The Mars Star rises over them.
    scSanctumRaiseItemIcon(ITEM_MARS_STAR, toFIXED(184), toFIXED(27), toFIXED(168));
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(10);
    fdActorJump(ACTOR_GARET, 4, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(15);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(30);
    fdActorJump(ACTOR_GARET, 4, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(15);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(10);
    fdActorGet(ACTOR_HEALER)->timer             = ACTOR_GARET;
    fdActorGet(ACTOR_HEALER)->update            = (void (*)(ACTOR *))scSanctumFaceActorNamedByTimer;
    fdActorGet(ACTOR_GARETS_GRANDFATHER)->timer = ACTOR_GARET;
    fdActorGet(ACTOR_GARETS_GRANDFATHER)->update =
        (void (*)(ACTOR *))scSanctumFaceActorNamedByTimer;
    fdActorWalkToAndWait(ACTOR_GARET, 196, 180);
    fdActorWalkToAndWait(ACTOR_GARET, 184, 184);
    fdActorWalkToAndWait(ACTOR_GARET, 180, 180);
    fdActorWalkToAndWait(ACTOR_GARET, 168, 168);
    fdActorWalkToAndWait(ACTOR_GARET, 180, 156);
    fdActorWalkTo(ACTOR_GARET, 200, 104);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 192, 168);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(15);
    fdActorGet(ACTOR_GARETS_GRANDFATHER)->update = NULL;
    fdActorGet(ACTOR_HEALER)->update             = NULL;
    fdActorStartRepeatedMotion(ACTOR_HEALER, 2);
    fdActorShowEmote(ACTOR_HEALER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdScriptWait(60);
    fdActorSetAnimation(ACTOR_HEALER, ANIM_STILL);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdScriptWait(10);
    fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
    fdScriptWait(20);
    fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
    fdScriptWait(20);
    fdScriptWait(15);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    fdScriptWait(60);
    fdActorWalkToAndWait(ACTOR_GARET, 208, 168);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdScriptWait(10);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
    fdScriptWait(10);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(10);
    fdActorSetScript(ACTOR_PARTY_LEADER, SanctumLeaderCircleScript);
    fdActorSetScript(ACTOR_GARET, SanctumGaretCircleScript);
    fdActorWaitForScriptEndById(ACTOR_PARTY_LEADER);
    fdActorWaitForScriptEndById(ACTOR_GARET);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
    fdActorSetSpeed(ACTOR_GARET, 1.5);
    fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
    fdActorJump(ACTOR_GARET, 6, 0);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_HEALER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_HEALER, 0);
    fdScriptWait(1);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_GARETS_GRANDFATHER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_DORA, 0);
    fdScriptWait(1);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_HEALER, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_WEST_HEALER, 0);
    fdScriptWait(1);
    fdActorWalkTo(ACTOR_PARTY_LEADER, 192, 168);
    fdActorWalkToAndWait(ACTOR_GARET, 208, 168);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_DORA, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_GARETS_GRANDFATHER, 0);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    fdScriptWait(60);
}

// An item's menu icon leaps up from a place (16.16) and hangs where it stops, for a second in all.
static void scSanctumRaiseItemIcon(Sint32 item, FIXED fixedX, FIXED fixedY, FIXED fixedZ)
{
    ACTOR         *icon;
    SPR           *sprite;
    ICON_TRANSFER *glyph;
    Uint32         frame;

    icon = syFieldActorCreate(SPRITE_BLANK, fixedX, fixedY, fixedZ);
    {
        Uint8 cleared = 0;

        if (icon != NULL)
        {
            fdActorStartScript(icon, ItemIconGrowScript);
            sprite                  = icon->sprite;
            sprite->flags           = cleared;
            sprite->count           = cleared;
            sprite->image.fullColor = 0;
            sprite->image.palette   = 0;
            icon->velocityY         = toFIXED(2);
            icon->gravity           = toFIXED(1) / 4;
            glyph = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
            mnIconDrawItem(item);
            syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
            syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
            for (frame = 0; frame < 60; frame++)
            {
                if (icon->velocityY > -toFIXED(1) / 256 && icon->velocityY < toFIXED(1) / 256)
                    icon->motionFlags = 0;
                sySynch(1);
            }
            fdActorStartScript(icon, SanctumItemIconEndScript);
        }
    }
}

// A glint's update: it drifts up, twinkling between full size and three fifths, and goes once its
// ticks run out.
static void scSanctumUpdateRisingSpark(ACTOR *spark)
{
    FIXED scale;

    // The timer holds the upward step in sixteenths of a pixel.
    spark->y.fixed += spark->timer * (toFIXED(1) / 16);
    spark->targetY  = spark->y.fixed;
    scale           = 0;
    switch ((spark->userValue >> 2) & 3)
    {
        case 0: scale = toFIXED(1); break;
        case 1:
        case 3: scale = toFIXED(4) / 5; break;
        case 2: scale = toFIXED(3) / 5; break;
    }
    spark->scaleX = scale;
    spark->scaleY = scale;
    if (--spark->userValue <= 0)
        fdActorStartScript(spark, SanctumSparkEndScript);
}

// A glint rises from beside the actor, for half a second to a second and a half.
static void scSanctumSpawnActorSpark(Sint32 id)
{
    ACTOR *actor;
    ACTOR *spark;
    SPR   *sprite;
    FIXED  x;
    Sint32 randomOffset;

    actor = fdActorGet(id);
    if (actor == NULL)
        return;
    randomOffset  = syMathRemainderUnsigned(syRandomNext(), 20);
    x             = actor->x.fixed + toFIXED(randomOffset);
    x            -= toFIXED(10);
    spark         = syFieldActorCreate(SPRITE_GLINT,
                               x,
                               actor->y.fixed + INTtoFIXED(syRandomNext() & 15) - toFIXED(8),
                               actor->z.fixed);
    if (spark == NULL)
        return;
    sprite                 = spark->sprite;
    spark->motionFlags     = 0;
    spark->timer           = syMathRemainderUnsigned(syRandomNext(), 10) + 5;
    spark->userValue       = syMathRemainderUnsigned(syRandomNext(), 60) + 30;
    spark->update          = scSanctumUpdateRisingSpark;
    sprite->flags          = 0;
    sprite->image.priority = actor->sprite->image.priority;
}

// Takes away the small stone counter before the altar for the quest's visits: its picture goes, and
// its metatiles (11, 8) to (13, 9) become plain floor.
static void scSanctumRemoveCounter(void)
{
    fdMapGridCopyMetatilesTo(0, 64, 11, 68, 3, 2);
    fdMapGridCopyMetatileAttributes(11, 10, 3, 2, 11, 8);
    sySynch(1);
}

// Whether the leader faces north, across the counter toward the Healer.
static Bool scSanctumIsLeaderFacingHealer(void)
{
    ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);

    if (leader->facing > FACING_NORTHWEST && leader->facing < FACING_NORTHEAST)
    {
        return TRUE;
    }
    return FALSE;
}

// Talking to the Great Healer in Vale's Sanctum: across the counter, the Sanctum's services. From
// any other side he says that wielders of Psynergy are called Adepts, messengers of good; once the
// party has left Vale, that his control over Psynergy has grown since he met the Wise One; once it
// has reached Kalay, that the path to Sol Sanctum is still closed.
static void scSanctumTalkToHealer(void)
{
    if (scSanctumIsLeaderFacingHealer() != 0)
    {
        mnSanctumRun(ACTOR_HEALER);
        return;
    }

    fdScriptStart();
    if (gmFlagGet(FLAG_KALAY_REACHED) != 0)
        fdScriptSetMessageId(MsgSanctumPathSolSanctumStillClosed);
    else if (gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0)
        fdScriptSetMessageId(MsgSanctumMyControlOverPsynergyHas);
    else
        fdScriptSetMessageId(MsgSanctumWieldersPsynergyCalledAdeptsAdepts);
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptEnd();
}

// Talking to the Healer of Vault's Sanctum: across the counter, the Sanctum's services. From any
// other side, "We will help you anytime, as long as you are pure of heart" while the gold statue is
// missing, "It was the hand of fate that returned the gold statue" once it is back, and on the
// later visit through Vault, that there seem to be a lot of earthquakes on Mt. Aleph. It also asks
// Isaac again (`scSanctumAskQuestAgain`) if run during the quest's visits, whose own cue table runs
// that directly instead.
static void scSanctumTalkToVaultHealer(void)
{
    Sint32 entrance;

    if (scSanctumIsLeaderFacingHealer() != 0)
    {
        mnSanctumRun(ACTOR_HEALER);
        return;
    }

    fdScriptStart();

    entrance = MainRoutineStateWork.entrance;

    switch (entrance)
    {
        case SANCTUM_FROM_VAULT:
        case SANCTUM_FROM_LOG_ROOMS_A:
            if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
            {
                fdScriptSetMessageId(MsgSanctumWasHandFateReturnedGold);
            }
            else
            {
                fdScriptSetMessageId(MsgSanctumWeWillHelpAnytimeAs);
            }
            break;
        case SANCTUM_FROM_VAULT_REVISIT:
            fdScriptSetMessageId(MsgSanctumMayWrongButLatelyThere);
            break;
        case SANCTUM_FROM_VALE_ERUPTION:
        case SANCTUM_FROM_QUEST_RESTART:
        case SANCTUM_QUEST_UNUSED:
            fdScriptEnd();
            scSanctumAskQuestAgain();
            return;
        default: break;
    }

    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptEnd();
}

// Talking to the Healer of Bilibin's Sanctum: across the counter, the Sanctum's services. From any
// other side, that this is only a beginning while Kolima is cursed, and once the curse is lifted,
// that it is still not safe outside town.
static void scSanctumTalkAboutCurse(void)
{
    if (scSanctumIsLeaderFacingHealer() != 0)
    {
        mnSanctumRun(ACTOR_HEALER);
        return;
    }

    fdScriptStart();
    if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0)
        fdScriptSetMessageId(MsgSanctumCurseMayOverButWe);
    else
        fdScriptSetMessageId(MsgSanctumAmStartingFeelOnlyBeginning);
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptEnd();
}

// Talking to the Healer of Altin's Sanctum: across the counter, the Sanctum's services. From any
// other side, how the water-spewing beasts appeared in the mine after the night the gems fell from
// the sky; once the party has beaten them, that saving Altin was the hand of Fate.
static void scSanctumTalkAboutAltinMonsters(void)
{
    if (scSanctumIsLeaderFacingHealer() != 0)
    {
        mnSanctumRun(ACTOR_HEALER);
        return;
    }

    fdScriptStart();
    fdScriptSetMessageId(MsgSanctumWasAfterEerieNightWhen);
    if (gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED) != 0)
        fdScriptSetMessageId(MsgSanctumSavedAltinFromMonstersClearly);
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptEnd();
}

// Talking to the Healer of Xian's Sanctum: across the counter, the Sanctum's services; from any
// other side, "I came to Xian from very distant lands to spread good will."
static void scSanctumTalkAboutJourneyFromXian(void)
{
    if (scSanctumIsLeaderFacingHealer() != 0)
    {
        mnSanctumRun(ACTOR_HEALER);
        return;
    }

    fdScriptStart();
    fdScriptSetMessageId(MsgSanctumCameXianFromVeryDistant);
    // "I came to Xian from very distant lands to spread good will."
    fdScriptShowMessage(ACTOR_HEALER, 0);
    fdScriptEnd();
}

// clang-format off
Uint32 SanctumLeaderCircleScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(2.5),
    SC_ACCELERATION(0.5),
    SC_LABEL(1),
    SC_MOVE(204, 0, 188),
    SC_SYNC,
    SC_MOVE(216, 0, 192),
    SC_SYNC,
    SC_MOVE(236, 0, 188),
    SC_SYNC,
    SC_MOVE(240, 0, 168),
    SC_SYNC,
    SC_MOVE(236, 0, 156),
    SC_SYNC,
    SC_MOVE(216, 0, 144),
    SC_SYNC,
    SC_MOVE(204, 0, 156),
    SC_SYNC,
    SC_MOVE(192, 0, 168),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

Uint32 SanctumGaretCircleScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_SPEED(2.5),
    SC_ACCELERATION(0.5),
    SC_LABEL(1),
    SC_MOVE(204, 0, 156),
    SC_SYNC,
    SC_MOVE(184, 0, 144),
    SC_SYNC,
    SC_MOVE(172, 0, 156),
    SC_SYNC,
    SC_MOVE(160, 0, 168),
    SC_SYNC,
    SC_MOVE(172, 0, 188),
    SC_SYNC,
    SC_MOVE(184, 0, 192),
    SC_SYNC,
    SC_MOVE(204, 0, 188),
    SC_SYNC,
    SC_MOVE(208, 0, 168),
    SC_SYNC,
    SC_LOOP(4, SC_LABEL(1)),
    SC_ANIMATION(ANIM_STAND),
    SC_END,
};

#include "shared/item_icon_grow_script.c"

Sint32 SanctumItemIconEndScript[] = {
    SC_WAIT(60),
    SC_RELEASE,
};

SCENE_ENTRANCE SanctumEntrances[] = {
    { SANCTUM_UNUSED, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SANCTUM_FROM_VALE, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_VALE_AFTER_DEFEAT, CONDITION_ALWAYS, 199, 0, 164, FACING_SOUTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_VAULT, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_VAULT_REVISIT, CONDITION_ALWAYS, 199, 0, 246, FACING_NORTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_LOG_ROOMS_A, CONDITION_ALWAYS, 199, 0, 164, FACING_SOUTH, 0, 64, 8, 336, 272, 0 },
    { SANCTUM_FROM_VALE_ERUPTION, CONDITION_ALWAYS, 192, 0, 168, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_QUEST_RESTART, CONDITION_ALWAYS, 192, 0, 168, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_ALTIN, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_XIAN, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SANCTUM_FROM_BILIBIN, CONDITION_ALWAYS, 200, 0, 240, FACING_NORTH, 0, 128, 0, 320, 272, 0 },
    { SCENE_TABLE_END },
};

Uint32 SanctumExits[] = {
    SCENE_EXITS(Sanctum),
    SCENE_EXIT(SANCTUM_TO_VALE, Vale, VALE_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_VAULT, Vault, VAULT_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_QUEST_RESTART, Sanctum, SANCTUM_FROM_QUEST_RESTART),
    SCENE_EXIT(SANCTUM_TO_VALE_PLAZA, ValePlaza, VALE_PLAZA_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_ALTIN, Altin, ALTIN_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_BILIBIN, Bilibin, BILIBIN_FROM_SANCTUM),
    SCENE_EXIT(SANCTUM_TO_XIAN, Xian, XIAN_FROM_SANCTUM_WEST),
    SCENE_EXIT(SANCTUM_TO_FILE_SCREEN, FileScreen, FILE_SCREEN_FROM_SANCTUM),
    SCENE_EXITS_END,
};

ACTOR_PLACE SanctumNoActorPlacements[] = {
    { SCENE_TABLE_END },
};

ACTOR_PLACE SanctumValeActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 130, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

#if IVAN_REACTS_TO_NEW_FRIENDS_QUESTION
ACTOR_PLACE SanctumIvanActorPlacement[] = {
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};
#endif

ACTOR_PLACE SanctumVaultBilibinActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 130, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        200, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SANCTUM_GOLD_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE SanctumQuestActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        208, 0, 168, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        224, 0, 216, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 198, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 208, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE SanctumAltinXianActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 132, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

CUE SanctumValeCues[] = {
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkToHealer) },
    { CUE_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumOurPsynergyWasGivenToUs) },
    { CUE_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumPsynergyIsAPowerThatShould) },
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { CUE_TABLE_END },
};

CUE SanctumVaultCues[] = {
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkToVaultHealer) },
    { CUE_TALK, ACTOR_SANCTUM_MEMBER, FLAG_VAULT_THIEVES_JAILED, SCRIPT_REF(scSanctumTalkAboutGoldenStatue) },
    { CUE_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumGoOnIfThatEasesThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumReadMindAboutGoldenStatue) },
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VAULT },
    { CUE_TABLE_END },
};

CUE SanctumVaultRevisitCues[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VAULT_REVISIT },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkToVaultHealer) },
    { CUE_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkAboutGoldenStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumIHopeTheEarthquakesArentA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumReadMindAboutGoldenStatue) },
    { CUE_TABLE_END },
};

CUE SanctumQuestCues[] = {
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumAskQuestAgain) },
    { CUE_TALK, ACTOR_GARET, CONDITION_ALWAYS, MSGNO(MsgSanctumItIsATerribleBurdenTo) },
    { CUE_TALK, ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgSanctumMotherWouldNeverDoubtYourCourage) },
    { CUE_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgSanctumIDontBlameYouICouldnt) },
    { CUE_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWasItRightToRefuseWhen) },
    { CUE_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsClearThatTheGreatHealer) },
    { CUE_STEP, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SCRIPT_REF(scSanctumLeaveAfterRefusing) },
    { CUE_STEP, METATILE_BEHAVIOR_SANCTUM_BEFORE_DOOR, FLAG_SANCTUM_WARNED_BEFORE_LEAVING, SCRIPT_REF(scSanctumWarnBeforeLeaving) },
    { CUE_TABLE_END },
};

CUE SanctumValeCuesAfterLeaving[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkToHealer) },
    { CUE_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMtAlephStems) },
    { CUE_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumAskAboutPsynergyChanges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumThePowerOfMyPsynergyHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheAngaranContinentWillCertainlySee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumMyPsynergyHasntChangedAtAll) },
    { CUE_TABLE_END },
};

CUE SanctumValeCuesAfterKalay[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_VALE },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkToHealer) },
    { CUE_TALK, ACTOR_WEST_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumAskAboutNewAdepts) },
    { CUE_TALK, ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhyIsItThatDifferentAdepts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItllBeDifficultToReachSol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumISeeABeautifulGirlHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumItsNotFairIWantTo) },
    { CUE_TABLE_END },
};

CUE SanctumAltinCues[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_ALTIN },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkAboutAltinMonsters) },
    // Unused: a copy of the row above, which always answers first.
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkAboutAltinMonsters) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_HYDROS_STATUES_DEFEATED, MSGNO(MsgSanctumPleaseSaveTheHelplessInAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumAlmightyFateMustHaveHeardMe) },
    { CUE_TABLE_END },
};

CUE SanctumBilibinCues[] = {
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkAboutCurse) },
    { CUE_TALK, ACTOR_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumPleaseGiveMeStrengthAtTimes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumWillITurnIntoATree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgSanctumSomeoneWillSaveUsJustKeep) },
    { CUE_TALK, ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumHowAboutThatOurWishesHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumTheSignsOfDangerAreActually) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_MEMBER, CONDITION_ALWAYS, MSGNO(MsgSanctumPeaceWillBeKeptAsLong) },
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_BILIBIN },
    { CUE_TABLE_END },
};

CUE SanctumXianCues[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_SANCTUM_DOORWAY, CONDITION_ALWAYS, SANCTUM_TO_XIAN },
    { CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scSanctumTalkAboutJourneyFromXian) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSanctumWhenYouStayInXianFor) },
    { CUE_TABLE_END },
};
// clang-format on

// The choice cursor's offset to the right in pixels, swaying a step every two ticks.
Sint32 SanctumCursorOffsets[] = {
    0,
    0,
    0,
    1,
    1,
    1,
    2,
    2,
    3,
    4,
    3,
    2,
    2,
    1,
    1,
    1,
};

// A sparkle circling the Great Healer rises half a pixel a tick for 96 ticks, then is gone.
Sint32 SanctumOrbitingSparkleScript[] = {
    SC_ADD(SW_Y, -toFIXED(1) / 2),
    SC_ADD(SW_TARGET_Y, -toFIXED(1) / 2),
    SC_WAIT(1),
    SC_LOOP(96, SC_TOP),
    SC_RELEASE,
};

Sint32 SanctumSparkEndScript[] = {
    SC_RELEASE,
};

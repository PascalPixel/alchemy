// Vault (SceneIdVault), "the town at the heart of Angara", as the party first finds it after Mt.
// Aleph's eruption.
//
// This scene is its streets, with the townspeople, Ivan, the bell and the dog. The houses, the inn
// and its loft, where the search for Hammet's stolen Shaman's Rod plays out, are log_rooms_a.c.
//
// scGetActorPlacements and scGetCues choose their tables by FLAG_VAULT_THIEVES_JAILED. Once the
// party has reached Kalay (FLAG_KALAY_REACHED), scInit sends it straight on to the town's later
// scene (VAULT_TO_VAULT_REVISIT, SceneIdVaultRevisit, vault_revisit.c).
//
// The story flags change the streets in order:
// - While the rod is missing, the townspeople talk of the eruption, the
//   theft and the travelers who left in a hurry. Mind Read on them blames
//   the three strangers staying at the inn. One of the three loiters in
// town until the party has confronted them in the loft
//       (FLAG_VAULT_THIEVES_ATTACKED). Once the suspects' minds have been read
//       (FLAG_VAULT_SUSPECTS_MIND_READ), the young man fixing a roof is
// missing, and a woman walks about looking for him.
// - While Ivan is helping the search (FLAG_VAULT_IVAN_JOINED_ROD_SEARCH),
//   leaving by either gate has him stay behind (scVaultIvanStaysBehind).
//   scInit has him rejoin when the party comes back in by a gate
//   or out of the Vault Sanctum.
// - Once the thieves are jailed, the townspeople thank the party. The dog,
//   given a Bone (scVaultGiveBoneToDog, FLAG_VAULT_DOG_SHOWN_PASSAGE), runs
//   to a spot and jumps there three times. The passage it shows opens only
//   in the later scene.
//
// Sap, a Venus Djinni, hides in the tree east of the bell tower. The first ring since the party
// came in by a gate (FLAG_VAULT_SAP_CALLED_BY_BELL) calls it out: it leaps onto the cliff top above
// the cave mouth beyond the tree. It joins only in the later scene.
//
// The well in the south-west rumbles with water, and the wooden box beside the mayor's house in the
// north-west holds a Nut.
//
// The town lies on terraces. Its paths leave north, past the bell tower, and south, under the rope
// bridge in the south-east. The mayor's house is the big house in the north-west, with the jail's
// iron door in the cliff below it and the Sanctum's double door further south in the same cliff.
// The inn stands in the middle, a ladder against its west wall up to the hole a fiery rock burnt in
// its roof. East of it are a house, the weapon and armor shop's door in the cliff under the bell
// tower, the item shop's door in the cliff further east, and the house by the rope bridge. Six
// gravestones stand south of the inn, where the dog sits.
//
// The doors open with an animation (Vault*DoorMetatiles). They lead into the houses, shops and inn
// (SceneIdLogRoomsA), the jail (SceneIdStoneRooms) and the Vault Sanctum (VAULT_TO_SANCTUM). The
// metatile behaviors the scripts answer are VAULT_METATILE_BEHAVIOR (vault.h).
//
// The rope bridge in the town's south-east spans the path that runs south under it. Stepping onto
// either end of it or off its west end switches the metatiles beneath it between the deck and the
// path (shared/vault_toggle.c).

#include "scene/vault.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "graphics/graphics.h"
#include "scene/vault_range.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE VaultEntrances[];
extern Uint32         VaultExits[];
extern ACTOR_PLACE    VaultActorPlacements[];
extern ACTOR_PLACE    VaultActorPlacementsAfterThieves[];
extern CUE            VaultCues[];
extern CUE            VaultCuesAfterThieves[];
extern Sint32         VaultDriftScriptA[];
extern Sint32         VaultDriftScriptB[];

// The unit of the unused drift's random speeds: a tenth of a pixel a tick.
enum
{
    VAULT_DRIFT_RATE = toFIXED(1) / 10
};

// The townspeople, named by what they say.
enum
{
    // A young man with brown hair outside the house in the east, by the rope bridge. "Welcome to
    // Vault, the town at the heart of Angara."
    ACTOR_WELCOMING_YOUNG_MAN          = 8,
    // A woman with fair hair who asks about the eruption, east of the six gravestones.
    ACTOR_ERUPTION_WITNESS             = 9,
    // A woman in a green dress who wanders north of the inn.
    ACTOR_WOMAN_ON_STEALING            = 11,
    // A burly grey-haired man up on the bell tower, beside the bell.
    ACTOR_VILLAGER_ON_HAMMET           = 10,
    // An old man with a cane, east of the jail's door.
    ACTOR_OLD_MAN_ON_HAMMETS_SERVANT   = 12,
    // An old woman who wanders east of the inn's door.
    ACTOR_OLD_WOMAN_ON_THE_TRAVELERS   = 13,
    // The two children who walk round the well, half a circle apart (VaultBoyRoundTheWellRoute,
    // VaultGirlRoundTheWellRoute).
    ACTOR_BOY_ROUND_THE_WELL           = 14,
    ACTOR_GIRL_ROUND_THE_WELL          = 15,
    // A stocky, muscular man who wanders near the west end of the rope bridge.
    ACTOR_STRONGMAN_ON_WATCH           = 16,
    // One of the three thieves, drawn with long purple hair as in the loft, standing beside the
    // Sanctum's door until the party confronts them (FLAG_VAULT_THIEVES_ATTACKED).
    ACTOR_THIEF_IN_TOWN                = 17,
    // A bearded man in a long robe beside the Sanctum's door.
    ACTOR_STRANDED_TRAVELER            = 18,
    // The dog that sits among the six gravestones.
    ACTOR_VAULT_DOG                    = 19,
    // A young man at the foot of the ladder against the inn's west wall, who is meant to fix the
    // roof and is later found tied up in the inn's loft.
    ACTOR_YOUNG_MAN_FIXING_THE_ROOF    = 20,
    // A woman with fair hair who paces in front of the inn's west end, looking for
    // ACTOR_YOUNG_MAN_FIXING_THE_ROOF.
    ACTOR_WOMAN_WANTING_THE_ROOF_FIXED = 21,
    // Sap, a Venus Djinni, hiding in the tree east of the bell tower until the bell calls it.
    ACTOR_SAP                          = 23
};

static void scVaultShowStandingActorMessage(Sint32 actorId);
static void scVaultShowMessageAndWait(Sint32 actorId, Sint32 delay);
static void scVaultFaceEachOtherAndWait(Sint32 actorId, Sint32 other, Sint32 delay);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

#include "shared/vault_nearby.c"

// The woman wanting the roof fixed paces in front of the inn looking for the man who should be
// fixing it, and stops when Isaac comes up in front of her, or while he talks to someone or reads a
// mind.
static UNREAD_RESULT scVaultUpdateWomanWantingRoofFixed(ACTOR *woman)
{
    WIN_STATE   *window = (WIN_STATE *)WindowStateWork[0];
    FIELD_STATE *state  = FieldStateWork;
    Sint32       force  = 0;
    Sint32       range  = STOP_RANGE;
    ACTOR       *leader;

    if (woman->targetX == ACTOR_NO_TARGET)
        RETURN_UNREAD(0);

    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if ((Sint16)state->talk != 0 || window->mindRead != 0)
    {
        range = TALK_STOP_RANGE;
        force = 1;
    }
    scVaultStopIfFacingNear(woman, leader, range, force);
    RETURN_UNREAD(0);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return VaultEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return VaultExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
        return VaultActorPlacementsAfterThieves;
    return VaultActorPlacements;
}

// Counts the Venus Djinn that must leave Ivan before he leaves the party.
static int scVaultGetIvanVenusDjinnCount(void)
{
    return gmUnitGet(IVAN)->djinnHeldCounts[ELEMENT_VENUS];
}

#include "shared/vault_djinn.c"

// While Ivan is helping look for the rod, leaving Vault by the path north, past the bell tower
// (metatiles (13, 7) and (14, 7)), or the path south, under the rope bridge (metatiles (24, 30) and
// (25, 30)), Ivan steps out of the party and stays behind. "Are you leaving? I'm still worried
// about the rod, so I'll just wait here until you two return."
//
// Before he joins, or once the thieves are jailed, the party simply leaves. If Ivan holds Flint, he
// gives it back to Isaac first.
static void scVaultIvanStaysBehind(void)
{
    ACTOR       *actor;
    FIELD_STATE *state;

    state = FieldStateWork;
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0
        || gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) == 0)
    {
        syImportScriptSetExitRequest(
            state->stepCue - (METATILE_BEHAVIOR_VAULT_SOUTH_GATE - VAULT_TO_WORLD_MAP_SOUTH));
        return;
    }
    fdScriptStart();
    actor = fdActorGet(ACTOR_PARTY_LEADER);
    if (actor != NULL)
    {
        fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    fdActorSetSpeed(ACTOR_IVAN, 0.8);
    if (state->stepCue == METATILE_BEHAVIOR_VAULT_SOUTH_GATE)
    {
        fdActorWalkToAndWait(ACTOR_IVAN, 400, 448);
    }
    else
    {
        fdCameraSetSpeed(0.8);
        fdCameraMoveTo(224, CAMERA_KEEP, 162, 1);
        fdActorWalkToAndWait(ACTOR_IVAN, 224, 162);
        fdCameraWaitForArrival();
    }
    fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    fdScriptWait(20);
    fdScriptSetMessageId(MsgVaultLeavingImStillWorriedAbout);
    // "Are you leaving? I'm still worried about the rod, so I'll..."
    fdScriptShowMessageThenWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    if (scVaultGetIvanVenusDjinnCount() != 0)
    {
        fdScriptSetMessageId(MsgVaultWaitDontWantTakeYour);
        // "Oh, wait, I don't want to take your Djinni. I'll just leave..."
        fdScriptShowMessage(ACTOR_IVAN, 0);
        scVaultReturnIsaacsDjinni();
        sySynch(20);
    }
    fdPartyRemoveMemberRestored(IVAN);
    syImportScriptSetExitRequest(state->stepCue
                                 - (METATILE_BEHAVIOR_VAULT_SOUTH_GATE - VAULT_TO_WORLD_MAP_SOUTH));
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptEnd();
}

// Isaac peers into the well in the south-west of Vault (metatiles (7, 24) to (8, 25)): "You can
// hear water rumbling down there."
static void scVaultPeerIntoWell(void)
{
    fdScriptStart();
    mnFieldMessageShow(MsgFieldPeeredWell, 1);
    mnFieldMessageShow(MsgVaultCanHearWaterRumblingDown, 1);
    fdScriptEnd();
}

const CUE *scGetCues(void)
{
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
        return VaultCuesAfterThieves;
    return VaultCues;
}

// Asks the current question and selects its ordinary or alternate reply.
static inline void scVaultAskYesNoAndShowAnswer(Sint32 speaker)
{
    fdScriptOpenMessage(speaker, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
        ++FieldStateWork->msg;
    fdScriptShowMessage(speaker, 0);
}

// While the rod is missing, the fair-haired woman east of the six gravestones asks, "Wasn't the
// eruption of Mt. Aleph incredible?", and answers Isaac's yes or no.
static void scVaultAskAboutEruption(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultWasntEruptionMtAlephIncredible);
    scVaultFaceEachOtherAndWait(ACTOR_ERUPTION_WITNESS, ACTOR_PARTY_LEADER, 2);
    fdScriptOpenMessage(ACTOR_ERUPTION_WITNESS, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
    {
        FieldStateWork->msg++;
    }
    fdScriptShowMessage(ACTOR_ERUPTION_WITNESS, 0);
    fdActorFaceDirection(ACTOR_ERUPTION_WITNESS, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptEnd();
}

// While the rod is missing, the old woman east of the inn's door says some travelers left in a big
// hurry, as if on the run, and asks whether the party saw them coming in.
static void scVaultAskAboutFleeingTravelers(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultThoseTravelersLeftInBig);
    fdActorSetAnimation(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, ANIM_STAND);
    scVaultFaceEachOtherAndWait(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, ACTOR_PARTY_LEADER, 2);
    scVaultAskYesNoAndShowAnswer(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
    fdScriptEnd();
}

// The purple-haired thief standing beside the Sanctum's door asks, "Are you accusing us of stealing
// Hammet's treasured Shaman's Rod?"
static void scVaultThiefDeniesStealing(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultAccusingUsStealingHammetsTreasured);
    scVaultFaceEachOtherAndWait(ACTOR_THIEF_IN_TOWN, ACTOR_PARTY_LEADER, 2);
    scVaultAskYesNoAndShowAnswer(ACTOR_THIEF_IN_TOWN);
    fdScriptEnd();
}

// While the rod is missing, the robed traveler beside the Sanctum's door asks, "Hmm, off on an
// adventure, are you?"
static void scVaultAskAboutAdventure(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultOffOnAdventure);
    scVaultFaceEachOtherAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
    scVaultAskYesNoAndShowAnswer(ACTOR_STRANDED_TRAVELER);
    fdScriptEnd();
}

// While the rod is missing, the woman in green north of the inn stops and says some folks got
// robbed during the chaos of the eruption.
static void scVaultTalkOfStealingInEruption(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultGuessFolksGot);
    scVaultShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
    fdScriptEnd();
}

// While the rod is missing, the strongman by the rope bridge stops and says nobody steals on his
// watch, so the thieves must be from out of town.
static void scVaultStrongmanOnWatchSuspectsOutsiders(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultNobodysStealingAnything);
    scVaultShowStandingActorMessage(ACTOR_STRONGMAN_ON_WATCH);
    fdScriptEnd();
}

// While the rod is missing, the dog among the six gravestones turns to Isaac and barks. "Ruff!
// Rrruff!!"
static void scVaultTalkToDog(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultRuffRrruff);
    fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
    scVaultFaceEachOtherAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
    fdScriptShowMessage(ACTOR_VAULT_DOG, 0);
    fdScriptEnd();
}

// While the rod is missing, the boy walking round the well stops: poor Ivan was left at the mayor's
// to look for the stolen rod.
static void scVaultPityIvan(void)
{
    ACTOR *actor        = fdActorGet(ACTOR_BOY_ROUND_THE_WELL);
    Sint16 savedFacing  = actor->facing;

    actor->timer       |= VILLAGER_TALKED_TO;
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultPoorGuyLeft);
    fdActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, ANIM_STILL);
    scVaultFaceEachOtherAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
    scVaultShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
    actor->facing = savedFacing;
    sySynch(1);
    fdScriptEnd();
    actor->timer &= VILLAGER_IS_BOY;
}

// While the rod is missing, the girl walking round the well stops: Master Hammet's caravan left
// town as soon as the eruption stopped.
static void scVaultTalkOfHammetsCaravan(void)
{
    ACTOR *actor        = fdActorGet(ACTOR_GIRL_ROUND_THE_WELL);
    Sint16 savedFacing  = actor->facing;

    actor->timer       |= VILLAGER_TALKED_TO;
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultMasterHammetsCaravan);
    fdActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, ANIM_STILL);
    scVaultFaceEachOtherAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
    scVaultShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
    actor->facing = savedFacing;
    sySynch(1);
    fdScriptEnd();
    actor->timer &= VILLAGER_IS_BOY;
}

// Once the suspects' minds are read, the woman pacing in front of the inn turns to Isaac, cross,
// and says the young man should be fixing the roof but she can't find him anywhere; he's probably
// slacking off again.
static void scVaultLookForManFixingRoof(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultFixingRoofCant);
    scVaultFaceEachOtherAndWait(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, ACTOR_PARTY_LEADER, 2);
    fdActorShowEmote(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    fdScriptWait(30);
    fdScriptOpenMessage(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, 0);
    fdScriptEnd();
}

// Stops an actor, faces the leader and shows the current message.
static void scVaultShowStandingActorMessage(Sint32 actorId)
{
    fdScriptStart();
    fdActorSetAnimation(actorId, ANIM_STAND);
    scVaultFaceEachOtherAndWait(actorId, ACTOR_PARTY_LEADER, 2);
    fdScriptShowMessage(actorId, 0);
    fdScriptEnd();
}

#include "shared/vault_talk.c"

// Once the thieves are jailed, the young man outside the house in the east asks, "So, you're
// hitting the road again?"
static void scVaultAskAboutLeavingAgain(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultYoureHittingRoadAgain);
    scVaultFaceEachOtherAndWait(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 2);
    scVaultAskYesNoAndShowAnswer(ACTOR_WELCOMING_YOUNG_MAN);
    fdScriptEnd();
}

// Once the thieves are jailed, the woman in green north of the inn says, "It's great that you
// caught those thieves!"
static void scVaultPraiseThievesCapture(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultGreatCaughtThieves);
    scVaultShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
    fdScriptEnd();
}

// Once the thieves are jailed, the old man east of the jail's door says that Hammet's servant,
// Ivan, went out to Lunpa. With Ivan in the party: "I see, Ivan... I guess it was impossible to
// rescue Master Hammet."
static void scVaultTalkAboutIvan(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultTalkingAboutHammetsServantIvan);
    if (gmFlagGet(FLAG_IN_PARTY(IVAN)) != 0)
    {
        ++FieldStateWork->msg;
    }
    scVaultShowStandingActorMessage(ACTOR_OLD_MAN_ON_HAMMETS_SERVANT);
    fdScriptEnd();
}

// Once the thieves are jailed, the old woman east of the inn's door says the eruption devastated
// roads throughout Angara, and the way to Kalay waits on the southern bridge's repair.
static void scVaultTalkOfDamagedRoads(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultEruptionDevastatedRoads);
    scVaultShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
    fdScriptEnd();
}

// Once the thieves are jailed, the boy walking round the well asks, "Ivan didn't go to Lunpa all
// alone, did he?" With Ivan in the party he laughs that he shouldn't have worried, and asks when
// Ivan joined up with Isaac.
static void scVaultAskWhetherIvanWentToLunpa(void)
{
    ACTOR *actor        = fdActorGet(ACTOR_BOY_ROUND_THE_WELL);
    Sint16 savedFacing  = actor->facing;

    actor->timer       |= VILLAGER_TALKED_TO;
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultDidntGoLunpa);
    if (gmFlagGet(FLAG_IN_PARTY(IVAN)) != 0)
        ++FieldStateWork->msg;
    fdActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, ANIM_STILL);
    scVaultFaceEachOtherAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
    scVaultShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
    actor->facing = savedFacing;
    sySynch(1);
    fdScriptEnd();
    actor->timer &= VILLAGER_IS_BOY;
}

// Once the thieves are jailed, the girl walking round the well stops: everyone is grateful the
// party captured the thieves.
static void scVaultThankForCapturingThieves(void)
{
    ACTOR *actor        = fdActorGet(ACTOR_GIRL_ROUND_THE_WELL);
    Sint16 savedFacing  = actor->facing;

    actor->timer       |= VILLAGER_TALKED_TO;
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultEveryoneGratefulCaptured);
    fdActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, ANIM_STILL);
    scVaultFaceEachOtherAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
    scVaultShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
    actor->facing = savedFacing;
    sySynch(1);
    fdScriptEnd();
    actor->timer &= VILLAGER_IS_BOY;
}

// Once the thieves are jailed, the strongman by the rope bridge asks about the prisoners. "Those
// men you captured... They're in jail now?"
static void scVaultAskAboutPrisoners(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultThoseMenCapturedTheyreIn);
    fdActorSetAnimation(ACTOR_STRONGMAN_ON_WATCH, ANIM_STAND);
    scVaultFaceEachOtherAndWait(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
    scVaultAskYesNoAndShowAnswer(ACTOR_STRONGMAN_ON_WATCH);
    fdScriptEnd();
}

// Once the thieves are jailed, the robed traveler beside the Sanctum's door asks, "Are you the ones
// who captured the thieves?"
static void scVaultAskWhoCapturedThieves(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultOnesWhoCapturedThieves);
    scVaultFaceEachOtherAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
    scVaultAskYesNoAndShowAnswer(ACTOR_STRANDED_TRAVELER);
    fdScriptEnd();
}

// Once the thieves are jailed, the dog south of the six gravestones barks. "Ruff! Rrruff!!" If
// Isaac carries a Bone and the dog has not yet shown its spot, the item menu opens so that he can
// give it one.
static void scVaultTalkToDogAfterCapture(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultRuffRrruffAfterCapture);
    fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
    scVaultFaceEachOtherAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
    fdScriptShowMessage(ACTOR_VAULT_DOG, 0);
    fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STAND);
    if (gmPartyInventoryFindMember(ITEM_BONE) != -1 && gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0)
    {
        FieldStateWork->fieldMenu = 1;
    }
    fdScriptEnd();
}

// Once the thieves are jailed, the young man back at the foot of the inn's ladder nods and thanks
// the party for the other day, and will pray for their safety.
static void scVaultThankBeforeDeparture(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultThankForOtherDayLeaving);
    scVaultFaceEachOtherAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, ACTOR_PARTY_LEADER, 2);
    fdActorSetAnimationAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, ANIM_NOD);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0);
    fdScriptEnd();
}

// Mind Read on the woman in green north of the inn: no man should steal during the panic of an
// eruption, it's unforgivable. Once the thieves are jailed she wonders how to punish them: "Public
// humiliation? Or perhaps a beating!"
static void scVaultReadMindOnStealing(void)
{
    fdScriptStart();
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
        fdScriptSetMessageId(MsgVaultManShouldStealFromAnother);
    else
        fdScriptSetMessageId(MsgVaultHowPunishPrisoners);
    scVaultShowThought(ACTOR_WOMAN_ON_STEALING);
    fdScriptEnd();
}

// Mind Read on the old woman east of the inn's door: that group of travelers was a strange bunch,
// seemingly heading for the north-eastern Goma Range. Once the thieves are jailed she thinks that,
// with the road out, the only way on is the cave in the Goma Range.
static void scVaultReadMindOnTravelers(void)
{
    fdScriptStart();
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
    {
        fdScriptSetMessageId(MsgVaultGroupTravelersWasStrangeBunch);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultWithRoadOutOnlyWay);
    }
    scVaultShowThought(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
    fdScriptEnd();
}

// Mind Read on the boy walking round the well: "I bet it was those three creeps at the inn who
// stole the rod." Once the thieves are jailed he wonders where Ivan went all by himself, or, with
// Ivan in the party, "But when did they join Ivan?"
static void scVaultReadMindOnIvan(void)
{
    fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->timer |= VILLAGER_TALKED_TO;
    fdScriptStart();
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
    {
        fdScriptSetMessageId(MsgVaultBetWasThoseThreeCreeps);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultWhereDidIvanGoBy);
        if (gmFlagGet(FLAG_IN_PARTY(IVAN)) != 0)
            ++FieldStateWork->msg;
    }
    scVaultShowThought(ACTOR_BOY_ROUND_THE_WELL);
    fdScriptEnd();
    fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->timer &= VILLAGER_IS_BOY;
}

// Mind Read on the girl walking round the well: it doesn't matter how rich you are if it can't save
// you from a volcano. Once the thieves are jailed, the party must be stronger than they look.
static void scVaultReadMindOnWealth(void)
{
    fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->timer |= VILLAGER_TALKED_TO;
    fdScriptStart();
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
        fdScriptSetMessageId(MsgVaultSupposeDoesntMatterHowRich);
    else
        fdScriptSetMessageId(MsgVaultMustStrongerThanLookHave);
    scVaultShowThought(ACTOR_GIRL_ROUND_THE_WELL);
    fdScriptEnd();
    fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->timer &= VILLAGER_IS_BOY;
}

// Mind Read on the strongman by the rope bridge: everyone knows the three at the inn stole the rod.
// Once the thieves are jailed he feels much better.
static void scVaultReadMindOnSuspects(void)
{
    fdScriptStart();
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
        fdScriptSetMessageId(MsgVaultEveryoneKnowsThoseThreeAt);
    else
        fdScriptSetMessageId(MsgVaultFeelMuchBetter);
    scVaultShowThought(ACTOR_STRONGMAN_ON_WATCH);
    fdScriptEnd();
}

// Mind Read on the dog among the six gravestones. While the rod is missing: "Stealing in the midst
// of a volcanic eruption... Humans value money more than they value their own lives." Once the
// thieves are jailed: "If you bring me a bone, I'll tell you something useful." Once it has had
// one: "A careful search will reveal a passage leading underground nearby."
static void scVaultReadDogMind(void)
{
    ACTOR *actor  = fdActorGet(ACTOR_VAULT_DOG);
    actor->paused = 1;
    fdScriptStart();
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
    {
        fdScriptSetMessageId(MsgVaultStealingInMidstVolcanicEruption);
        fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
        fdScriptWait(2);
    }
    else if (gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0)
    {
        fdScriptSetMessageId(MsgVaultCarefulSearchWillRevealPassage);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultIfBringMeBoneIll);
    }
    fdScriptShowMessage(ACTOR_VAULT_DOG, 0);
    fdScriptEnd();
    actor->paused = 0;
}

// Mind Read on the woman pacing in front of the inn: "That sneak... He's just wasting time instead
// of fixing the roof!"
static void scVaultReadWomanWantingTheRoofFixedMind(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultSneakWastingTime);
    scVaultShowThought(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED);
    fdScriptEnd();
}

#include "shared/vault_toggle.c"

#include "shared/vault_door_exit.c"

// The rooms behind the town's doors (shared/vault_doors.c).
#define VAULT_GEM_OWNERS_HOUSE_EXIT      VAULT_TO_LOG_ROOMS_A_EAST_SOUTHEAST
#define VAULT_CHILDS_HOUSE_EXIT          VAULT_TO_LOG_ROOMS_A_SOUTHEAST
#define VAULT_INN_EXIT                   VAULT_TO_LOG_ROOMS_A_SOUTHWEST
#define VAULT_ITEM_SHOP_EXIT             VAULT_TO_LOG_ROOMS_A_EAST
#define VAULT_WEAPON_AND_ARMOR_SHOP_EXIT VAULT_TO_LOG_ROOMS_A_NORTH_NORTHEAST
#define VAULT_MAYORS_HOUSE_EXIT          VAULT_TO_LOG_ROOMS_A_NORTHWEST
#include "shared/vault_doors.c"

#include "shared/vault_map_metatiles.c"

#define VAULT_JAIL_EXIT VAULT_TO_STONE_ROOMS
#include "shared/vault_jail_door.c"

// Isaac climbs the ladder against the inn's west wall to the hole in its roof (metatile (9, 16) at
// the top) and goes in, into the inn's loft, the room with the crate (VAULT_TO_LOG_ROOMS_A_WEST).
//
// The young man who should be fixing the roof stands at the ladder's foot until the suspects' minds
// have been read.
static void scVaultClimbIntoInnLoft(void)
{
    sySoundPlay(SE_MAP_EXIT);
    scVaultWalkThroughExit(152, 264, VAULT_TO_LOG_ROOMS_A_WEST);
}

// Moves an actor through a jump and waits up to sixty ticks for it to settle.
static void scVaultJumpActorToAndWait(Sint32 actorId, Sint32 x, Sint32 z, Sint32 verticalSpeed)
{
    ACTOR *actor = fdActorGet(actorId);
    int    ticks;
    fdActorSetSpeed(actorId, 3);
    actor->gravity   = toFIXED(1) / 2;
    actor->bounce    = 0;
    actor->velocityY = verticalSpeed;
    fdActorSetSpriteFlags(actor, 0);
    fdActorMoveToAndWait(actorId, x, z);
    fdActorSetPos(actorId, x, z);
    for (ticks = 60; ticks != 0; --ticks)
    {
        sySynch(1);
        if ((FIXEDtoINT(actor->velocityY)) == 0)
            break;
    }
    fdActorSetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    actor->gravity = toFIXED(1);
}

// Up on the bell tower, Isaac rings the bell (metatile (17, 8)). The first time since the party
// came in by a path, Sap the Venus Djinni pops up, startled, on top of the tree east of the tower,
// hops twice and leaps away north-east onto the cliff top above the cave mouth.
static void scVaultRingBell(void)
{
    fdScriptStart();
    sySoundPlay(SE_BELL);
    fdScriptWait(40);
    if (fdScriptGetFlag(FLAG_VAULT_SAP_CALLED_BY_BELL) == 0)
    {
        fdActorSetAttachedEffect(ACTOR_SAP, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorJump(ACTOR_SAP, 4, 0);
        fdScriptWait(12);
        fdActorJump(ACTOR_SAP, 4, 0);
        fdScriptWait(20);
        scVaultJumpActorToAndWait(ACTOR_SAP, 392, 104, toFIXED(7));
        fdScriptWait(20);
        fdActorWalkToAndWait(ACTOR_SAP, 408, 104);
        fdActorWalkToAndWait(ACTOR_SAP, 408, 120);
        gmFlagSet(FLAG_VAULT_SAP_CALLED_BY_BELL);
    }
    fdScriptEnd();
}

// Given a Bone, the dog runs into the middle of the six gravestones and jumps there three times,
// where Reveal later finds the way down to Vault Cave.
static void scVaultGiveBoneToDog(void)
{
    gmPartyInventoryDiscard(ITEM_BONE);
    fdScriptStart();
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_VAULT_DOG, 2);
    fdActorSetSpeed(ACTOR_VAULT_DOG, 0.8);
    fdActorWalkToAndWait(ACTOR_VAULT_DOG, 216, 408);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
    fdActorJump(ACTOR_VAULT_DOG, 6, 0);
    fdScriptWait(30);
    fdActorJump(ACTOR_VAULT_DOG, 6, 0);
    fdScriptWait(30);
    fdActorJump(ACTOR_VAULT_DOG, 6, 0);
    fdScriptWait(30);
    fdActorWalkToAndWait(ACTOR_VAULT_DOG, 216, 392);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
    gmFlagSet(FLAG_VAULT_DOG_SHOWN_PASSAGE);
    fdScriptEnd();
}

// Arriving in Vault puts everyone where the story has them. Coming back in by either path, or out
// of the Sanctum, while Ivan waits in town, he greets the party, "Welcome back, you two! Let's go
// find the rod!", and joins again.
Sint32 scInit(void)
{
    Sint32 suspectsMindRead;
    ACTOR *actor;

    // Once the party has reached Kalay (FLAG_KALAY_REACHED), the town is its later scene,
    // SceneIdVaultRevisit (vault_revisit.c).
    if (gmFlagGet(FLAG_KALAY_REACHED) != 0)
    {
        syImportScriptSetExitRequest(VAULT_TO_VAULT_REVISIT);
    }
    if (gmFlagGet(FLAG_VAULT_ON_BRIDGE) != 0)
    {
        fdMapGridCopyMetatileAttributes(55, 26, 4, 2, 23, 26);
    }
    // A hidden crate on the well, the one vault_revisit.c places too, where what it is for is
    // asked.
    fdActorCreateConfigured(128, 0, 420, SPRITE_CRATE);
    actor         = fdActorGet(ACTOR_BOY_ROUND_THE_WELL);
    actor->timer  = VILLAGER_IS_BOY;
    actor->update = (void (*)(ACTOR *))scVaultUpdateWalkingVillager;
    actor         = fdActorGet(ACTOR_GIRL_ROUND_THE_WELL);
    actor->timer  = 0;
    actor->update = (void (*)(ACTOR *))scVaultUpdateWalkingVillager;
    if (gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0)
    {
        fdActorSetPos(ACTOR_VAULT_DOG, 216, 392);
    }
    suspectsMindRead = gmFlagGet(FLAG_VAULT_SUSPECTS_MIND_READ);
    if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
    {
        // DEAD: gmFlagGet answers 0 or 1, so the mask changes nothing; the ROM keeps it.
        if ((suspectsMindRead & 1) != 0)
        {
            fdActorGet(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED)->update =
                (void (*)(ACTOR *))scVaultUpdateWomanWantingRoofFixed;
        }
    }
    // Back through the gates while Ivan waits in Vault: "Welcome back, you two! Let's go find the
    // rod!" The entrances up to VAULT_NORTH_UNUSED are the gates': the default one inside the north
    // gate, the south gate's and the unused one inside the north gate. XXX: the World Map's north
    // exit leads to VAULT_FROM_WORLD_MAP_NORTH (3), which VaultEntrances lacks, so the party stands
    // at the default row's place but keeps entrance 3 and fails this test, and Ivan's welcome back
    // would not play there; a recording of that exit into Vault with
    // FLAG_VAULT_IVAN_JOINED_ROD_SEARCH set, Ivan out of the party, settles it.
    if (MainRoutineStateWork.entrance <= VAULT_NORTH_UNUSED)
    {
        if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
        {
            gmFlagClear(FLAG_VAULT_SAP_CALLED_BY_BELL);
            if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
            {
                if (gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0)
                {
                    fdScriptStart();
                    actor = fdActorGet(ACTOR_PARTY_LEADER);
                    if (actor != NULL)
                    {
                        fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
                    }
                    if (MainRoutineStateWork.entrance == VAULT_FROM_WORLD_MAP_SOUTH)
                    {
                        fdActorSetPos(ACTOR_IVAN, 400, 448);
                    }
                    else
                    {
                        fdActorSetPos(ACTOR_IVAN, 224, 162);
                    }
                    fdActorTurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
                    fdScriptStartScreenTransition();
                    fdScriptWaitTransitionTicks();
                    fdScriptWait(30);
                    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
                    fdScriptSetMessageId(MsgVaultWelcomeBackTwo);
                    // "Welcome back, you two! Let's go find the rod!"
                    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
                    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
                    fdActorSetSpeed(ACTOR_IVAN, 0.8);
                    scWalkIntoLeader(ACTOR_IVAN);
                    fdScriptAddPartyMember(IVAN, 0);
                    fdScriptEnd();
                }
            }
        }
    }
    if (gmFlagGet(FLAG_VAULT_SAP_CALLED_BY_BELL) != 0)
    {
        fdActorSetPos(ACTOR_SAP, 408, 120);
    }
    // The same welcome out of the Vault Sanctum while Ivan is out of the party.
    if (MainRoutineStateWork.entrance == VAULT_FROM_SANCTUM)
    {
        if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) == 0)
        {
            if (gmFlagGet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0)
            {
                if (gmFlagGet(FLAG_IN_PARTY(IVAN)) == 0)
                {
                    fdScriptStart();
                    fdActorSetPos(ACTOR_IVAN, 40, 310);
                    fdActorTurnToFaceEachOther(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
                    fdScriptStartScreenTransition();
                    fdScriptWaitTransitionTicks();
                    fdScriptWait(30);
                    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
                    fdScriptSetMessageId(MsgVaultWelcomeBackTwo);
                    // "Welcome back, you two! Let's go find the rod!"
                    fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
                    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
                    fdActorSetSpeed(ACTOR_IVAN, 0.8);
                    scWalkIntoLeader(ACTOR_IVAN);
                    fdScriptAddPartyMember(IVAN, 0);
                    fdScriptEnd();
                }
            }
        }
        gmFlagClear(FLAG_SHOW_AREA_NAME);
    }
    else
    {
        if (MainRoutineStateWork.entrance == VAULT_FROM_LOG_ROOMS_A_WEST)
        {
            if (gmFlagGet(FLAG_VAULT_THIEVES_JAILED) != 0)
            {
                fdActorSetPos(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0, 0);
            }
        }
    }
    return 0;
}

#include "shared/actor_distance.c"
#include "shared/vault_range.c"

static void scVaultShowMessageAndWait(Sint32 actorId, Sint32 delay)
{
    fdScriptShowMessage(actorId, 0);
    fdScriptWait(delay);
}

static void scVaultFaceEachOtherAndWait(Sint32 actorId, Sint32 other, Sint32 delay)
{
    fdActorTurnToFaceEachOther(actorId, other, 0);
    fdScriptWait(delay);
}

// A drifting sprite loses this share of each rate a tick: a 28th.
#define DRIFT_DAMPING 28

// Applies drift and gradually damps both rates toward zero.
static void scVaultUpdateDriftingEffect(ACTOR *effect)
{
    Sint32 horizontalRate;
    Sint32 verticalRate;

    effect->x.fixed += effect->speedLimit;
    effect->targetX  = effect->x.fixed;

    if (effect->timer != 0)
    {
        effect->y.fixed += effect->acceleration;
        effect->targetY  = effect->y.fixed;
    }
    else
    {
        effect->z.fixed += effect->acceleration;
        effect->targetZ  = effect->z.fixed;
        effect->y.fixed += toFIXED(1) / 64;
        effect->targetY  = effect->y.fixed;
    }

    horizontalRate       = effect->speedLimit;
    effect->speedLimit   = horizontalRate - syMathDivide(horizontalRate, DRIFT_DAMPING);
    verticalRate         = effect->acceleration;
    effect->acceleration = verticalRate - syMathDivide(verticalRate, DRIFT_DAMPING);
}

// Spawns a drifting sprite near the dog, sharing its drawing priority.
UNUSED static void scVaultSpawnDriftingEffect(Sint32 flags)
{
    ACTOR *anchor;
    ACTOR *effect;
    SPR   *sprite;
    Sint32 x;
    FIXED  y;
    Sint32 z;

    anchor = fdActorGet(ACTOR_VAULT_DOG);
    if (anchor == NULL)
        return;
    // Up to four pixels either way of the dog, syRandomBelow(8) - 4 on each roll.
    x        = syRandomNext();
    z        = syRandomNext();
    x      <<= 3;
    x        = (Uint32)x >> 16;
    z      <<= 3;
    x       -= 4;
    z        = (Uint32)z >> 16;
    z       -= 4;
    x        = toFIXED(x);
    z        = INTtoFIXED(z);
    x       += anchor->x.fixed;
    y        = anchor->y.fixed;
    z       += anchor->z.fixed;
    // XXX (no evidence): what this drift was meant to draw; SPRITE_NO_IMAGE's row in sprites.tsv is
    // empty and nothing calls this function.
    effect   = fdActorCreate(SPRITE_NO_IMAGE, x, y, z);
    if (effect == NULL)
        return;
    sprite = effect->sprite;
    // UB: fdSpriteCreate makes no sprite for SPRITE_NO_IMAGE, so sprite is NULL and the two stores
    // through it below write near address 0.
#if FIX_NULL_DEREFERENCE
    if (sprite == NULL)
        return;
#endif
    switch (syRandomNext() & 1)
    {
        case 1:
            fdActorSelectAnimation(effect, 3);
            fdActorStartScript(effect, VaultDriftScriptA);
            break;
        default:
            fdActorSelectAnimation(effect, 2);
            fdActorStartScript(effect, VaultDriftScriptB);
            break;
    }
    effect->motionFlags = 0;
    if (flags & 2)
    {
        Sint32 driftMultiplier;
        Sint32 directionBias;

        driftMultiplier   = (Uint32)syRandomNext() % 10 + 5;
        // x, done with once the effect is placed, carries the 1.
        x                 = 1;
        flags            &= x;
        directionBias     = flags ^ x;
        directionBias   <<= 2;
        driftMultiplier  += directionBias;
        effect->acceleration = (2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
        driftMultiplier      = (Uint32)syRandomNext() % 15 - 7;
        effect->speedLimit   = VAULT_DRIFT_RATE * driftMultiplier;
        effect->timer        = 0;
    }
    else
    {
        Sint32 driftMultiplier;

        driftMultiplier      = (Uint32)syRandomNext() % 10 + 8;
        effect->speedLimit   = (2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
        driftMultiplier      = (Uint32)syRandomNext() % 14 + 1;
        effect->acceleration = VAULT_DRIFT_RATE * driftMultiplier;
        effect->timer        = 1;
    }
    effect->update         = scVaultUpdateDriftingEffect;
    sprite->flags          = 0;
    sprite->image.priority = anchor->sprite->image.priority;
}

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

// ACTOR_WOMAN_WANTING_THE_ROOF_FIXED's search for the young man fixing the roof, in front of the
// inn's west end. She walks to one end of her two-metatile path and the other, looks west and east,
// then shakes her head, for as long as the party stays on the map.
Sint32 VaultWomanWantingTheRoofFixedRoute[] = {
    SC_SPEED(0.4),
    SC_ACCELERATION(0.2),
    SC_LABEL(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(168, 0, 312),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(240),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(200, 0, 312),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_ANIMATION(ANIM_SHAKE_HEAD),
    SC_WAIT(240),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    // Unused: past the route's end, a solid stroll round her spot that no actor is given.
    SC_SPEED(0.4),
    SC_ACCELERATION(0.2),
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_SET(SW_RADIUS, toFIXED(40)),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_WANDER_HOME(8, 16, 24),
    SC_SYNC,
    SC_ANIMATION(ANIM_STILL),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

#include "shared/vault_well_routes.c"

SCENE_ENTRANCE VaultEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 223, 0, 135, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {VAULT_NORTH_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_EAST_SOUTHEAST,
     CONDITION_ALWAYS, 408,
     0, 344,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_FROM_LOG_ROOMS_A_SOUTHEAST,
     CONDITION_ALWAYS, 312,
     0, 328,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_FROM_LOG_ROOMS_A_SOUTHWEST,
     CONDITION_ALWAYS, 216,
     0, 312,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_FROM_LOG_ROOMS_A_EAST,
     CONDITION_ALWAYS, 376,
     0, 248,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_FROM_LOG_ROOMS_A_NORTH_NORTHEAST,
     CONDITION_ALWAYS, 296,
     0, 200,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_FROM_LOG_ROOMS_A_NORTHWEST,
     CONDITION_ALWAYS, 120,
     0, 168,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_FROM_SANCTUM, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_STONE_ROOMS, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_WEST,
     CONDITION_ALWAYS, 152,
     0, 272,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {SCENE_TABLE_END},
};

Uint32 VaultExits[] = {
    SCENE_EXITS(Vault),
    SCENE_EXIT(VAULT_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_VAULT_SOUTH),
    SCENE_EXIT(VAULT_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_VAULT_NORTH),
    SCENE_EXIT(
        VAULT_TO_LOG_ROOMS_A_EAST_SOUTHEAST, LogRoomsA, LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_SOUTHEAST, LogRoomsA, LOG_ROOMS_A_FROM_VAULT_SOUTHEAST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_SOUTHWEST, LogRoomsA, LOG_ROOMS_A_FROM_VAULT_SOUTHWEST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_EAST, LogRoomsA, LOG_ROOMS_A_FROM_VAULT_EAST),
    SCENE_EXIT(
        VAULT_TO_LOG_ROOMS_A_NORTH_NORTHEAST, LogRoomsA, LOG_ROOMS_A_FROM_VAULT_NORTH_NORTHEAST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_NORTHWEST, LogRoomsA, LOG_ROOMS_A_FROM_VAULT_NORTHWEST),
    SCENE_EXIT(VAULT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT),
    SCENE_EXIT(VAULT_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_VAULT),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_WEST, LogRoomsA, LOG_ROOMS_A_FROM_VAULT_WEST),
    SCENE_EXIT(VAULT_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE VaultActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        381, 0, 342, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 396, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 150, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 204, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        124, 0, 196, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 300, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (Sint32)VaultBoyRoundTheWellRoute,
        132, 0, 380, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (Sint32)VaultGirlRoundTheWellRoute,
        132, 0, 420, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        332, 0, 412, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        92, 0, 284, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 392, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, FLAG_VAULT_SUSPECTS_MIND_READ, MOVEMENT_TYPE_STAND,
        152, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_MIND_READ), (Sint32)VaultWomanWantingTheRoofFixedRoute,
        168, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    // XXX (no evidence): what this ivy was for; it stands above the map's top edge where no camera shows it, is not solid, and only the field's passes over every actor touch it.
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        344, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE VaultActorPlacementsAfterThieves[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        381, 0, 342, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 396, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 150, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 204, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        124, 0, 196, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        244, 0, 300, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (Sint32)VaultBoyRoundTheWellRoute,
        132, 0, 380, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (Sint32)VaultGirlRoundTheWellRoute,
        132, 0, 420, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        332, 0, 412, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 440, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, FLAG_VAULT_THIEVES_JAILED, (Sint32)VaultWomanWantingTheRoofFixedRoute,
        168, 0, 312, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        344, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

CUE VaultCues[] = {
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_SOUTH_GATE, CONDITION_ALWAYS, SCRIPT_REF(scVaultIvanStaysBehind) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_NORTH_GATE, CONDITION_ALWAYS, SCRIPT_REF(scVaultIvanStaysBehind) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_BRIDGE_END, CONDITION_ALWAYS, SCRIPT_REF(scVaultRaisePathToBridge) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scVaultLowerPathUnderBridge) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterGemOwnersHouse) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterChildsHouse) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_INN_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterInn) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterItemShop) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterWeaponAndArmorShop) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterMayorsHouse) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, SCRIPT_REF(scVaultOpenDoorwayAndEnter) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterJail) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_INN_LOFT_LADDER, CONDITION_ALWAYS, SCRIPT_REF(scVaultClimbIntoInnLoft) },
    { CUE_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { CUE_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, SCRIPT_REF(scVaultAskAboutEruption) },
    { CUE_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetAWealthyLandownerAnd) },
    { CUE_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, SCRIPT_REF(scVaultTalkOfStealingInEruption) },
    { CUE_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetsServantCantReturnTo) },
    { CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, SCRIPT_REF(scVaultAskAboutFleeingTravelers) },
    { CUE_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultPityIvan) },
    { CUE_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultTalkOfHammetsCaravan) },
    { CUE_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultStrongmanOnWatchSuspectsOutsiders) },
    { CUE_TALK, ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, SCRIPT_REF(scVaultThiefDeniesStealing) },
    { CUE_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, SCRIPT_REF(scVaultAskAboutAdventure) },
    { CUE_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, SCRIPT_REF(scVaultTalkToDog) },
    { CUE_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultAFieryRockBurntAHole) },
    { CUE_TALK, ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, SCRIPT_REF(scVaultLookForManFixingRoof) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultISupposeTheseChildrenCantGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultMtAlephsEruptionWasBeyondDescription) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoesAWealthyManLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnStealing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultPoorIvanButTheRodMay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgVaultHmphItLooksLikeTheyreOnto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultIWasJustAboutToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultFixTheRoofWhyPayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadWomanWantingTheRoofFixedMind) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_BELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRingBell) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultPeerIntoWell) },
    { EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { CUE_TABLE_END },
};

CUE VaultCuesAfterThieves[] = {
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_SOUTH_GATE, CONDITION_ALWAYS, SCRIPT_REF(scVaultIvanStaysBehind) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_NORTH_GATE, CONDITION_ALWAYS, SCRIPT_REF(scVaultIvanStaysBehind) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_BRIDGE_END, CONDITION_ALWAYS, SCRIPT_REF(scVaultRaisePathToBridge) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, SCRIPT_REF(scVaultLowerPathUnderBridge) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterGemOwnersHouse) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterChildsHouse) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_INN_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterInn) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterItemShop) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterWeaponAndArmorShop) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterMayorsHouse) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, SCRIPT_REF(scVaultOpenDoorwayAndEnter) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, SCRIPT_REF(scVaultEnterJail) },
    { CUE_STEP, METATILE_BEHAVIOR_VAULT_INN_LOFT_LADDER, CONDITION_ALWAYS, SCRIPT_REF(scVaultClimbIntoInnLoft) },
    { CUE_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, SCRIPT_REF(scVaultAskAboutLeavingAgain) },
    { CUE_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultSmokeIsStillRisingFromMt) },
    { CUE_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetTheWealthyMerchantI) },
    { CUE_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, SCRIPT_REF(scVaultPraiseThievesCapture) },
    { CUE_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, SCRIPT_REF(scVaultTalkAboutIvan) },
    { CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, SCRIPT_REF(scVaultTalkOfDamagedRoads) },
    { CUE_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultAskWhetherIvanWentToLunpa) },
    { CUE_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultThankForCapturingThieves) },
    { CUE_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultAskAboutPrisoners) },
    { CUE_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, SCRIPT_REF(scVaultAskWhoCapturedThieves) },
    { CUE_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, SCRIPT_REF(scVaultTalkToDogAfterCapture) },
    { CUE_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, SCRIPT_REF(scVaultThankBeforeDeparture) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultYouveMadeOurTownPeacefulAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultNowMtAlephHasCalmedDown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultOfAllThePlacesWhyWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnStealing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultNobodyGetsIntoLunpaExceptFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultThereIsAlwaysAFightWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, SCRIPT_REF(scVaultReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultSniffAhTheSmellOfCooking) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_BELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRingBell) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultPeerIntoWell) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, SCRIPT_REF(scVaultGiveBoneToDog) },
    { EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { CUE_TABLE_END },
};
// clang-format on

#include "shared/vault_door_metatiles.c"

Sint32 VaultDriftScriptA[] = {
    SC_WAIT(1),
    SC_LOOP(118, SC_TOP),
    SC_RELEASE,
};

Sint32 VaultDriftScriptB[] = {
    SC_WAIT(1),
    SC_LOOP(38, SC_TOP),
    SC_RELEASE,
};

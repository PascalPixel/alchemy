// Vault (SceneIdVaultRevisit), the town once the party has reached Kalay. It has the same layout as
// Vault's first scene (vault.c), and replaces that scene for the rest of the game.
//
// The World Map still leads into the first scene. Once FLAG_KALAY_REACHED is set, that scene's
// setup sends the party straight on here (VAULT_REVISIT_FROM_VAULT). Vault Sanctum, Vault Cave, the
// jail and the houses (log_rooms_b.c) lead back here directly.
//
// The townspeople stand where they stood before, and the news is that the thieves the party caught
// have broken out of jail, that the bridge to Kalay is fixed, and that Hammet is still in Lunpa.
// Some speak in plain message rows; the young man by the rope bridge, the woman in green, the old
// man by the jail, the old woman by the inn, the two children round the well and the strongman
// speak through scripts. Those who greet the party remember it in area flags
// (FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED to FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL), so that a Mind
// Read on one not yet spoken to plays the greeting first. The old man's praise also sets
// FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES, as the mayor's welcome indoors does.
//
// This is where the walkthrough sends the player back for two things. Up on the bell tower, ringing
// the bell startles Sap, a Venus Djinni, out of the tree east of the tower onto the cliff top
// beside the cave mouth; the party reaches it there through Vault Cave, and talking to it makes it
// join. Coming into town from the World Map sends Sap back into its tree until it has joined
// (FLAG_VAULT_SAP_CALLED_BY_BELL). And Reveal among the six gravestones south of the inn shows a
// hole, the hidden passage down into Vault Cave. The dog, given a Bone from the inn, runs to that
// spot and jumps there three times as a hint (FLAG_VAULT_DOG_SHOWN_PASSAGE, shared with vault.c).
// The cave mouth in the north-east cliff is the cave's other way in.
//
// The wooden box beside the mayor's house still holds a Nut if it was not taken before, and the
// well rumbles with water. The metatiles the scripts answer are VAULT_METATILE_BEHAVIOR (vault.h).
//
// The inn's roof is mended: scInit copies a 3-by-3 block of metatiles from (45,
// 41) to (8, 45), which draws the thatch whole where vault.c's town shows the
// hole the fiery rock burnt in it.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "scene/vault.h"
#include "scene/vault_range.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE VaultRevisitEntrances[];
extern Uint32         VaultRevisitExits[];
extern ACTOR_PLACE    VaultRevisitActorPlacements[];
extern CUE            VaultRevisitCues[];

// The townspeople, the dog and Sap, standing where vault.c places them.
typedef enum VAULT_REVISIT_ACTOR_ID
{
    // The young man with brown hair outside the gem owner's house in the east, by the rope bridge,
    // who recognizes the party.
    ACTOR_WELCOMING_YOUNG_MAN = ACTOR_FIRST_PLACED,
    // A woman with fair hair east of the six gravestones, who brings the news that the bridge to
    // Kalay is fixed.
    ACTOR_WOMAN_ON_THE_FIXED_BRIDGE,
    // A burly grey-haired man up on the bell tower, beside the bell, who worries about Hammet.
    ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET,
    // A woman in a green dress who wanders north of the inn.
    ACTOR_WOMAN_ON_LUNPAS_THIEVES,
    // An old man with a cane east of the jail's door, who has been talking with the mayor.
    ACTOR_MAYORS_VISITOR,
    // An old woman who wanders east of the inn's door.
    ACTOR_OLD_WOMAN_ON_THE_VOLCANO,
    // vault.c's two children, walking round the well again. The boy greets the party. "Hey, it's
    // that warrior guy!"
    ACTOR_BOY_ROUND_THE_WELL,
    // Wishes Vault's elders were as brave as the party.
    ACTOR_GIRL_ROUND_THE_WELL,
    // The strongman who wanders near the west end of the rope bridge. "Did you guys check out the
    // jail?"
    ACTOR_STRONGMAN_ON_WATCH,
    // A bearded man in a long robe beside the Sanctum's door, who takes the thieves' escape calmly.
    ACTOR_VILLAGER_DIVINE_PLAN,
    // The dog south of the six gravestones.
    ACTOR_VAULT_DOG,
    // The young man at the foot of the ladder against the inn's west wall, the roof now mended.
    ACTOR_YOUNG_MAN_FIXING_THE_ROOF,
    // Sap, a Venus Djinni, hiding in the tree east of the bell tower until the bell calls it. The
    // row before it, SPRITE_IVY placed at 0, 0, is never referred to.
    ACTOR_SAP = ACTOR_FIRST_PLACED + 13
} VAULT_REVISIT_ACTOR_ID;

// Area flags for what the townspeople have already said, kept while the party stays in Vault's
// scenes. A Mind Read before someone has spoken plays the conversation first.
enum
{
    FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED        = FLAG_AREA_TEMP_00,
    FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY = FLAG_AREA_TEMP_05,
    FLAG_VAULT_REVISIT_PARTY_PRAISED              = FLAG_AREA_TEMP_06,
    FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE   = FLAG_AREA_TEMP_07,
    FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL           = FLAG_AREA_TEMP_08
};

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/actor_distance.c"
#include "shared/vault_range.c"
#include "shared/vault_nearby.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return VaultRevisitEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return VaultRevisitExits;
}

// The townspeople, the dog and Sap.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    return VaultRevisitActorPlacements;
}

// Stands the person still while they say the message already chosen.
static void scVaultRevisitShowStandingActorMessage(Sint32 actorId)
{
    fdScriptStart();
    fdActorSetAnimation(actorId, ANIM_STAND);
    fdScriptShowMessage(actorId, 0);
    fdScriptEnd();
}

// The young man outside the gem owner's house, by the rope bridge, turns to Isaac and recognizes
// the party: "You're the warriors who captured the thieves a while back, aren't you? You've
// returned to Vault!"
static void scVaultRevisitRecognizeWarriors(void)
{
    fdScriptStart();
    fdActorFaceActor(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 2);
    gmFlagSet(FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY);
    fdScriptSetMessageId(MsgVaultWarriorsWhoCaptured);
    // "You! You're the warriors who captured the thieves a while..."
    fdScriptShowMessage(ACTOR_WELCOMING_YOUNG_MAN, 0);
    fdScriptEnd();
}

// The woman in green who wanders north of the inn stops and turns to Isaac: there have been rumors
// of thieves from Lunpa running wild, and she wonders why they have been so quiet recently.
static void scVaultRevisitTalkOfLunpaThieves(void)
{
    fdScriptSetMessageId(MsgVaultRumorsThievesLunpa);
    fdActorTurnToFaceEachOther(ACTOR_WOMAN_ON_LUNPAS_THIEVES, ACTOR_PARTY_LEADER, 2);
    scVaultRevisitShowStandingActorMessage(ACTOR_WOMAN_ON_LUNPAS_THIEVES);
}

// The old man with a cane east of the jail's door turns to Isaac: he was just talking about the
// party with the mayor, and they are so strong that the thieves are welcome to attack.
static void scVaultRevisitPraisePartyStrength(void)
{
    fdScriptStart();
    fdActorFaceActor(ACTOR_MAYORS_VISITOR, ACTOR_PARTY_LEADER, 2);
    gmFlagSet(FLAG_VAULT_REVISIT_PARTY_PRAISED);
    gmFlagSet(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
    fdScriptSetMessageId(MsgVaultTalkingMayorStrong);
    // "Oh, it's you... I was just talking about you with our mayor...."
    fdScriptShowMessage(ACTOR_MAYORS_VISITOR, 0);
    fdScriptEnd();
}

// The old woman who wanders east of the inn's door stops and turns to Isaac: if the volcano stays
// calm and the southern bridge stays fixed, Vault won't have any worries at all.
static void scVaultRevisitHopeVolcanoStaysCalm(void)
{
    fdScriptSetMessageId(MsgVaultVolcanoStaysCalm);
    fdActorTurnToFaceEachOther(ACTOR_OLD_WOMAN_ON_THE_VOLCANO, ACTOR_PARTY_LEADER, 2);
    scVaultRevisitShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_VOLCANO);
}

// The boy walking round the well stops and turns to Isaac. The first time, an exclamation mark pops
// up over him: "Hey, it's that warrior guy! Yeah, and Ivan, too." Then, each time: "I'm so glad you
// came back to Vault!" He goes back to walking round the well.
static void scVaultRevisitGreetWarriorGuy(void)
{
    ACTOR *actor   = fdActorGet(ACTOR_BOY_ROUND_THE_WELL);
    Uint16 facing  = actor->facing;

    actor->timer  |= VILLAGER_TALKED_TO;
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultWarriorGuy);
    fdActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, ANIM_STILL);
    fdActorTurnToFaceEachOther(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
    if (fdScriptGetFlag(FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED) == 0)
    {
        fdActorShowEmote(ACTOR_BOY_ROUND_THE_WELL, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
        // "Hey, it's that warrior guy!"
        fdScriptShowMessageThenWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
        // "Yeah, and Ivan, too."
        fdScriptShowMessageThenWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
        gmFlagSet(FLAG_VAULT_REVISIT_WARRIOR_GUY_GREETED);
    }
    fdScriptSetMessageId(MsgVaultWarriorGuy + 2);
    // "I'm so glad you came back to Vault!"
    fdScriptShowMessageThenWait(ACTOR_BOY_ROUND_THE_WELL, 0, 10);
    actor->facing = facing;
    sySynch(1);
    fdScriptEnd();
    actor->timer = VILLAGER_IS_BOY;
    gmFlagSet(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE);
}

// The girl walking round the well stops and turns to Isaac: "I wish Vault's elders were as brave as
// you!"
static void scVaultRevisitWishEldersWereBrave(void)
{
    ACTOR *actor;
    Sint16 originalFacing;

    actor           = fdActorGet(ACTOR_GIRL_ROUND_THE_WELL);
    originalFacing  = actor->facing;
    actor->timer   |= VILLAGER_TALKED_TO;
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultWishVaultsElders);
    fdActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, ANIM_STILL);
    fdActorTurnToFaceEachOther(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
    // "I wish Vault's elders were as brave as you!"
    fdScriptShowMessageThenWait(ACTOR_GIRL_ROUND_THE_WELL, 0, 10);
    actor->facing = originalFacing;
    sySynch(1);
    fdScriptEnd();
    actor->timer = 0;
}

// The strongman by the rope bridge asks, "Oh, so did you guys check out the jail?" If Isaac nods,
// he wonders how the thugs broke out, the bars being so strong; if he shakes his head, the
// strongman tells him the thieves broke out and got away.
static void scVaultRevisitAskAboutJail(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultGuysCheckJail);
    fdActorTurnToFaceEachOther(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
    // "Oh, so did you guys check out the jail?"
    fdScriptOpenMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
    {
        FieldStateWork->msg += 1;
    }
    fdScriptShowMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
    gmFlagSet(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
    fdScriptEnd();
}

// Mind Read on the young man by the rope bridge before he has spoken: he turns to Isaac and
// recognizes the party aloud, as when talked to.
static void scVaultRevisitReadMindAndRecognizeWarriors(void)
{
    fdScriptStart();
    fdActorRunRepeatedMotion(ACTOR_WELCOMING_YOUNG_MAN, 1);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 20);
    gmFlagSet(FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY);
    fdScriptSetMessageId(MsgVaultWarriorsWhoCaptured);
    // "You! You're the warriors who captured the thieves a while..."
    fdScriptShowMessageThenWait(ACTOR_WELCOMING_YOUNG_MAN, 0, 20);
    fdScriptEnd();
}

// Mind Read on the woman in green north of the inn: "Once Dodonpa took over, Lunpa's outlaws became
// no better than thieves. There's gotta be a reason those guys are being so quiet."
static void scVaultRevisitReadMindOnDodonpa(void)
{
    fdScriptSetMessageId(MsgVaultOnceDodonpaTook);
    fdActorGet(ACTOR_WOMAN_ON_LUNPAS_THIEVES)->paused = 1;
    scVaultRevisitShowStandingActorMessage(ACTOR_WOMAN_ON_LUNPAS_THIEVES);
    fdActorGet(ACTOR_WOMAN_ON_LUNPAS_THIEVES)->paused = 0;
}

// Mind Read on the old man by the jail's door before he has spoken: he turns to Isaac and praises
// the party aloud, as when talked to.
static void scVaultRevisitReadMindAndPraisePartyStrength(void)
{
    fdScriptStart();
    fdActorRunRepeatedMotion(ACTOR_MAYORS_VISITOR, 1);
    fdScriptWait(20);
    fdActorFaceActor(ACTOR_MAYORS_VISITOR, ACTOR_PARTY_LEADER, 20);
    gmFlagSet(FLAG_VAULT_REVISIT_PARTY_PRAISED);
    gmFlagSet(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
    fdScriptSetMessageId(MsgVaultTalkingMayorStrong);
    // "Oh, it's you... I was just talking about you with our mayor...."
    fdScriptShowMessageThenWait(ACTOR_MAYORS_VISITOR, 0, 20);
    fdScriptEnd();
}

// Mind Read on the old woman east of the inn's door: "It would have been great to have everything
// solved before winter came."
static void scVaultRevisitReadMindOnWinter(void)
{
    fdScriptSetMessageId(MsgVaultGreatEverythingSolved);
    fdActorGet(ACTOR_OLD_WOMAN_ON_THE_VOLCANO)->paused = 1;
    scVaultRevisitShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_VOLCANO);
    fdActorGet(ACTOR_OLD_WOMAN_ON_THE_VOLCANO)->paused = 0;
}

// Mind Read on the boy walking round the well: watching the party makes him feel brave, and he
// hopes they'll tell him all about their travels sometime. If he has not spoken to them yet, he
// greets them aloud instead.
static void scVaultRevisitReadMindOnFeelingBrave(void)
{
    fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->timer |= VILLAGER_TALKED_TO;
    fdScriptStart();
    if (fdScriptGetFlag(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE) != 0)
    {
        fdScriptSetMessageId(MsgVaultWatchingGuysMakes);
        scVaultRevisitShowStandingActorMessage(ACTOR_BOY_ROUND_THE_WELL);
    }
    else
    {
        scVaultRevisitGreetWarriorGuy();
        gmFlagSet(FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE);
    }
    fdScriptEnd();
    fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->timer = VILLAGER_IS_BOY;
}

// Mind Read on the girl walking round the well: "Grown-ups always get so scared whenever anything
// happens. I get so tired of that..."
static void scVaultRevisitReadMindOnScaredGrownUps(void)
{
    fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->timer |= VILLAGER_TALKED_TO;
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultGrownUpsAlways);
    scVaultRevisitShowStandingActorMessage(ACTOR_GIRL_ROUND_THE_WELL);
    fdScriptEnd();
    fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->timer = 0;
}

// Mind Read on the strongman by the rope bridge: it's not easy to grab the key to that jail, so the
// thieves must know the secret of the lock, and that must have been great! If he has not asked
// about the jail yet, he asks aloud instead, then wanders off again.
static void scVaultRevisitReadMindOnJailKey(void)
{
    Sint32 askedAboutJail;

    askedAboutJail = fdScriptGetFlag(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
    if (askedAboutJail == 0)
    {
        fdScriptStart();
        fdActorGet(ACTOR_STRONGMAN_ON_WATCH)->paused = 1;
        fdActorSetAnimation(ACTOR_STRONGMAN_ON_WATCH, ANIM_STAND);
        fdActorRunRepeatedMotion(ACTOR_STRONGMAN_ON_WATCH, 1);
        fdScriptWait(20);
        fdScriptSetMessageId(MsgVaultGuysCheckJail);
        fdActorTurnToFaceEachOther(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
        // "Oh, so did you guys check out the jail?"
        fdScriptOpenMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
        {
            FieldStateWork->msg++;
        }
        fdScriptShowMessage(ACTOR_STRONGMAN_ON_WATCH, 0);
        fdActorGet(ACTOR_STRONGMAN_ON_WATCH)->paused = 0;
        fdActorSetMovementTypeById(ACTOR_STRONGMAN_ON_WATCH, MOVEMENT_TYPE_WANDER);
        fdScriptEnd();
        gmFlagSet(FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL);
    }
    else
    {
        fdScriptSetMessageId(MsgVaultNotLikeEasy);
        fdActorGet(ACTOR_STRONGMAN_ON_WATCH)->paused = 1;
        scVaultRevisitShowStandingActorMessage(ACTOR_STRONGMAN_ON_WATCH);
        fdActorGet(ACTOR_STRONGMAN_ON_WATCH)->paused = 0;
    }
}

// Jumps the actor to (x, z), launched upward at `velocityY`, and waits there until it lands, a
// second at most.
static void scVaultRevisitJumpActorToAndWait(Sint32 actorId, Sint32 x, Sint32 z, FIXED velocityY)
{
    ACTOR *jumpingActor = fdActorGet(actorId);
    Sint32 ticks;
    fdActorSetSpeed(actorId, 3);
    jumpingActor->gravity   = toFIXED(1) / 2;
    jumpingActor->bounce    = 0;
    jumpingActor->velocityY = velocityY;
    fdActorSetSpriteFlags(jumpingActor, 0);
    fdActorMoveToAndWait(actorId, x, z);
    fdActorSetPos(actorId, x, z);
    for (ticks = TICKS_PER_SECOND; ticks != 0; --ticks)
    {
        sySynch(1);
        if ((FIXEDtoINT(jumpingActor->velocityY)) == 0)
            break;
    }
    fdActorSetSpriteFlags(jumpingActor, SPRITE_FLAG_SHADOW);
    jumpingActor->gravity = toFIXED(1);
}

// Up on the bell tower, Isaac rings the bell (metatile (17, 8)). The first time since the party
// came into town, Sap the Venus Djinni pops up, startled, on top of the tree east of the tower,
// hops twice and leaps away north-east onto the cliff top beside the cave mouth, where it waits.
static void scVaultRevisitRingBell(void)
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
        scVaultRevisitJumpActorToAndWait(ACTOR_SAP, 392, 104, toFIXED(7));
        fdScriptWait(20);
        fdActorWalkToAndWait(ACTOR_SAP, 408, 104);
        fdActorWalkToAndWait(ACTOR_SAP, 408, 120);
        gmFlagSet(FLAG_VAULT_SAP_CALLED_BY_BELL);
    }
    fdScriptEnd();
}

#include "shared/vault_door_exit.c"

// The rooms behind the town's doors (shared/vault_doors.c).
#define VAULT_GEM_OWNERS_HOUSE_EXIT      VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHEAST
#define VAULT_CHILDS_HOUSE_EXIT          VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTH
#define VAULT_INN_EXIT                   VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHWEST
#define VAULT_ITEM_SHOP_EXIT             VAULT_REVISIT_TO_LOG_ROOMS_B_EAST
#define VAULT_WEAPON_AND_ARMOR_SHOP_EXIT VAULT_REVISIT_TO_LOG_ROOMS_B_NORTH
#define VAULT_MAYORS_HOUSE_EXIT          VAULT_REVISIT_TO_LOG_ROOMS_B_NORTHWEST
#include "shared/vault_doors.c"

#include "shared/vault_map_metatiles.c"

#define VAULT_JAIL_EXIT VAULT_REVISIT_TO_STONE_ROOMS
#include "shared/vault_jail_door.c"

#include "shared/vault_toggle.c"

// On the cliff top beside the cave mouth, where the bell sent it, talking to Sap, a Venus Djinni,
// makes it join the party.
static void scVaultRevisitSapJoins(void)
{
    fdDjinnJoinParty(ACTOR_SAP, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_SAP));
}

// The dog jumps once on the hidden passage's spot, then pauses.
static inline void scVaultRevisitDogJumpAtPassage(void)
{
    fdActorJump(ACTOR_VAULT_DOG, 6, 0);
    fdScriptWait(30);
}

// Isaac gives the dog south of the six gravestones a Bone. It trots north to the middle of the
// gravestones (metatile (13, 25)), turns south and jumps there three times, then steps one metatile
// north and sits facing the spot, where Reveal will show the passage.
static void scVaultRevisitGiveBoneToDog(void)
{
    gmPartyInventoryDiscard(ITEM_BONE);
    fdScriptStart();
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_VAULT_DOG, 2);
    fdActorSetSpeed(ACTOR_VAULT_DOG, 0.8);
    fdActorWalkToAndWait(ACTOR_VAULT_DOG, 216, 408);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
    scVaultRevisitDogJumpAtPassage();
    scVaultRevisitDogJumpAtPassage();
    scVaultRevisitDogJumpAtPassage();
    fdActorWalkToAndWait(ACTOR_VAULT_DOG, 216, 392);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
    gmFlagSet(FLAG_VAULT_DOG_SHOWN_PASSAGE);
    fdScriptEnd();
}

// When Reveal wears off, the hole among the six gravestones (metatile (13, 25)) is grass again and
// can no longer be walked into.
static void scVaultRevisitHidePassage(void)
{
    fdMapGridCopyMetatileAttributes(41, 43, 1, 1, 13, 25);
    fdMapGridCopyMetatilesTo(40, 42, 12, 22, 3, 3);
}

// Reveal, cast near the six gravestones south of the inn, shows a dark hole in the middle of them
// (metatile (13, 25)), where the dog jumps: the hidden passage down into Vault Cave, which Isaac
// can walk into while the spell lasts.
static void scVaultRevisitRevealPassage(void)
{
    fdMapGridCopyMetatileAttributes(37, 43, 1, 1, 13, 25);
    fdMapGridCopyMetatilesTo(36, 42, 12, 22, 3, 3);
}

// The dog south of the six gravestones turns to Isaac and barks. "Ruff! Rrruff!!" If Isaac carries
// a Bone and the dog has not yet shown its spot, the item menu opens so that he can give it one.
static void scVaultRevisitTalkToDog(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgVaultRuffRrruffAfterCapture);
    fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STILL);
    fdActorTurnToFaceEachOther(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(2);
    // "Ruff! Rrruff!!"
    fdScriptShowMessage(ACTOR_VAULT_DOG, 0);
    fdActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STAND);

    if (gmPartyInventoryFindMember(ITEM_BONE) != -1 && gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0)
    {
        FieldStateWork->fieldMenu = 1;
    }

    fdScriptEnd();
}

// Isaac peers into the well in the south-west of Vault (metatiles (7, 24) to (8, 25)): "You can
// hear water rumbling down there."
static void scVaultRevisitPeerIntoWell(void)
{
    fdScriptStart();
    mnFieldMessageShow(MsgFieldPeeredWell, 1);
    mnFieldMessageShow(MsgVaultCanHearWaterRumblingDown, 1);
    fdScriptEnd();
}

// The town's scripts: its people, doors, bell, well and box, the dog's Bone and Reveal among the
// gravestones.
const CUE *scGetCues(void)
{
    return VaultRevisitCues;
}

// Patched editions keep Sap on the cliff top when the party keeps its position, as on a return from
// battle.
static inline Sint32 scVaultRevisitShouldKeepBellDjinniPos(void)
{
#if VAULT_SAP_STAYS_AFTER_BATTLE
    return gmFlagGet(FLAG_KEEP_PARTY_POSITION);
#else
    return 0;
#endif
}

// Sets up Vault as the party finds it after reaching Kalay.
Sint32 scInit(void)
{
    if (gmFlagGet(FLAG_VAULT_ON_BRIDGE))
        fdMapGridCopyMetatileAttributes(55, 26, 4, 2, 23, 26);
    // A crate on the well, drawn hidden (SPRITE_COLOR_HIDDEN) and never a talk or Psynergy target,
    // as vault.c makes it. XXX (no evidence): what it is for; nothing refers to it again, and every
    // pixel of it is transparent.
    fdActorCreateConfigured(128, 0, 420, SPRITE_CRATE);
    fdMapGridCopyMetatilesTo(45, 41, 8, 45, 3, 3);
    sySynch(1);
    fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->update  = (void (*)(ACTOR *))scVaultUpdateWalkingVillager;
    fdActorGet(ACTOR_BOY_ROUND_THE_WELL)->timer   = VILLAGER_IS_BOY;
    fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->update = (void (*)(ACTOR *))scVaultUpdateWalkingVillager;
    fdActorGet(ACTOR_GIRL_ROUND_THE_WELL)->timer  = 0;
    if (gmFlagGet(FLAG_VAULT_DOG_SHOWN_PASSAGE))
        fdActorSetPos(ACTOR_VAULT_DOG, 216, 392);
    if (MainRoutineStateWork.entrance <= VAULT_REVISIT_NORTH_UNUSED
        && !gmFlagGet(FLAG_DJINNI(DJINNI_SAP)) && !scVaultRevisitShouldKeepBellDjinniPos())
        gmFlagClear(FLAG_VAULT_SAP_CALLED_BY_BELL);
    if (gmFlagGet(FLAG_VAULT_SAP_CALLED_BY_BELL) && !gmFlagGet(FLAG_DJINNI(DJINNI_SAP)))
        fdActorSetPos(ACTOR_SAP, 408, 120);
    if (MainRoutineStateWork.entrance == VAULT_REVISIT_FROM_SANCTUM)
        gmFlagClear(FLAG_SHOW_AREA_NAME);
    if (MainRoutineStateWork.entrance == VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE)
        gmFlagClear(FLAG_ARRIVAL_STAIRS_UP);
    return 0;
}

#include "shared/field_effect_duration_scripts.c"

const Sint32 *FieldEffectDurationScripts[] = {
    FieldEffectShortScript,
    FieldEffectMediumScript,
    FieldEffectLongScript,
};

#include "shared/vault_well_routes.c"

SCENE_ENTRANCE VaultRevisitEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_VAULT, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_NORTH_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHEAST,
     CONDITION_ALWAYS, 408,
     0, 344,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTH,
     CONDITION_ALWAYS, 312,
     0, 328,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHWEST,
     CONDITION_ALWAYS, 216,
     0, 312,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_EAST,
     CONDITION_ALWAYS, 376,
     0, 248,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTH,
     CONDITION_ALWAYS, 296,
     0, 200,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTHWEST,
     CONDITION_ALWAYS, 120,
     0, 168,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_SANCTUM, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_REVISIT_FROM_STONE_ROOMS,
     CONDITION_ALWAYS, 70,
     0, 183,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_PASSAGE,
     CONDITION_ALWAYS, 217,
     0, 409,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VAULT_REVISIT_FROM_VAULT_CAVE_MOUTH,
     CONDITION_ALWAYS, 420,
     0, 136,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {SCENE_TABLE_END},
};

Uint32 VaultRevisitExits[] = {
    SCENE_EXITS(VaultRevisit),
    SCENE_EXIT(VAULT_REVISIT_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_VAULT_SOUTH),
    SCENE_EXIT(VAULT_REVISIT_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_VAULT_NORTH),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHEAST,
               LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTH, LogRoomsB, LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTH),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_SOUTHWEST,
               LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_EAST, LogRoomsB, LOG_ROOMS_B_FROM_VAULT_REVISIT_EAST),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_NORTH, LogRoomsB, LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTH),
    SCENE_EXIT(VAULT_REVISIT_TO_LOG_ROOMS_B_NORTHWEST,
               LogRoomsB,
               LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTHWEST),
    SCENE_EXIT(VAULT_REVISIT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_VAULT_REVISIT),
    SCENE_EXIT(VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE,
               VaultCavePassage,
               VAULT_CAVE_PASSAGE_FROM_VAULT_REVISIT),
    SCENE_EXIT(
        VAULT_REVISIT_TO_VAULT_CAVE_MOUTH, VaultCaveMouth, VAULT_CAVE_MOUTH_FROM_VAULT_REVISIT),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE VaultRevisitActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        381, 0, 342, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 396, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 150, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
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
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 440, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 296, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        344, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

#include "shared/vault_door_metatiles.c"

CUE VaultRevisitCues[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_VAULT_SOUTH_GATE, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_SOUTH },
    { CUE_EXIT, METATILE_BEHAVIOR_VAULT_NORTH_GATE, CONDITION_ALWAYS, VAULT_REVISIT_TO_WORLD_MAP_NORTH },
    { CUE_EXIT, METATILE_BEHAVIOR_VAULT_HIDDEN_PASSAGE, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_PASSAGE },
    { CUE_EXIT, METATILE_BEHAVIOR_VAULT_CLIFF_CAVE_MOUTH, CONDITION_ALWAYS, VAULT_REVISIT_TO_VAULT_CAVE_MOUTH },
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
    { CUE_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitRecognizeWarriors) },
    { CUE_TALK, ACTOR_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultYouveProbablyAlreadyHeardButThe) },
    { CUE_TALK, ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultIWonderWhatHappenedToHammet) },
    { CUE_TALK, ACTOR_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitTalkOfLunpaThieves) },
    { CUE_TALK, ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitPraisePartyStrength) },
    { CUE_TALK, ACTOR_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitHopeVolcanoStaysCalm) },
    { CUE_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitGreetWarriorGuy) },
    { CUE_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitWishEldersWereBrave) },
    { CUE_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitAskAboutJail) },
    { CUE_TALK, ACTOR_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultTheThievesEscapedButIThink) },
    { CUE_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitTalkToDog) },
    { CUE_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThereYouSeeTheRoofRepairs) },
    { CUE_TALK, ACTOR_SAP, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitSapJoins) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_BELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitRingBell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_WELCOMING_YOUNG_MAN, FLAG_VAULT_REVISIT_YOUNG_MAN_RECOGNIZED_PARTY, SCRIPT_REF(scVaultRevisitReadMindAndRecognizeWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultICantBelieveTheyreHereWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_FIXED_BRIDGE, CONDITION_ALWAYS, MSGNO(MsgVaultWhenThoseGuysFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WONDERING_ABOUT_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultYeahSomeWarriorsFromKalayCame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_LUNPAS_THIEVES, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitReadMindOnDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_MAYORS_VISITOR, FLAG_VAULT_REVISIT_PARTY_PRAISED, SCRIPT_REF(scVaultRevisitReadMindAndPraisePartyStrength) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_VISITOR, CONDITION_ALWAYS, MSGNO(MsgVaultThoseStupidThievesEscapedAndWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_VOLCANO, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitReadMindOnWinter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_BOY_ROUND_THE_WELL, FLAG_VAULT_REVISIT_BOY_ROUND_THE_WELL_SPOKE, SCRIPT_REF(scVaultRevisitReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitReadMindOnFeelingBrave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitReadMindOnScaredGrownUps) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_STRONGMAN_ON_WATCH, FLAG_VAULT_REVISIT_ASKED_ABOUT_JAIL, SCRIPT_REF(scVaultRevisitReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitReadMindOnJailKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DIVINE_PLAN, CONDITION_ALWAYS, MSGNO(MsgVaultIKeepWonderingWasOurStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, IF_FLAG_SET(FLAG_VAULT_DOG_SHOWN_PASSAGE), MSGNO(MsgVaultCarefulSearchWillRevealPassage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, MSGNO(MsgVaultIfYouBringMeSomeBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultThatsTheLastOfTheRoof) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_VAULT_WELL, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitPeerIntoWell) },
    { EXAMINE_CUE(EXAMINE_WOODEN_BOX), METATILE_BEHAVIOR_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, SCRIPT_REF(scVaultRevisitGiveBoneToDog) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitRevealPassage) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scVaultRevisitHidePassage) },
    { CUE_TABLE_END },
};
// clang-format on

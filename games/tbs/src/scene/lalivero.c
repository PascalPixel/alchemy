// Lalivero (SceneIdLalivero), the walled desert town by Babi Lighthouse, with a gate in its north
// and south walls and a stone obelisk in a pool at its middle. Its houses, shops, inn and Sanctum
// are timber_rooms_bc.c.
//
// The story brings the party here twice, and the town's people and what answers the party change
// with FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, set at Venus Lighthouse's aerie.
//
// The first time, the party walks in through the south gate with Iodem and finds two of Tolbi's
// soldiers lying beaten in the gateway (scLaliveroWatchWoundedSoldiers, once,
// FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN). They tell Iodem that Sheba's captors went to the ruins
// beneath Babi Lighthouse, with Faran after them. The townsfolk talk of Sheba, and the soldier at
// the door of the shack in the north-west lets no one in.
//
// The second time, the party comes back from Venus Lighthouse into Faran's house in the south-west.
// A few steps out of its door Garet stops Isaac, for Iodem asked them to wait; Iodem comes out,
// talks of Lemuria and gives the party the Black Orb that sails Babi's ship
// (scLaliveroIodemGivesBlackOrb, once, FLAG_IODEM_FAREWELL_SEEN). If the party saw the ship at
// Idejima, they talk of it too (scLaliveroTalkAboutIdejimaShip). From then on the soldier at the
// shack lets the party in (FLAG_LALIVERO_WHARF_OPEN), Garet turns Isaac back from the south gate,
// and a sign and a wall close the north gate.
//
// On both visits Torch, a Mars Djinni, waits on a roof until spoken to, and a jar, a stone pillar
// and a chest hold a Water of Life, a Psy Crystal and a Warrior's Helm. Each door opens before
// Isaac walks in, and the hatches in two of the flat roofs lead down into Faran's house and the
// weapon and armor shop. Both gates lead out to the World Map.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

Sint32                scInit(void);
extern Sint32         LaliveroCryingScript[];
extern SCENE_ENTRANCE LaliveroEntrances[];
extern Uint32         LaliveroExits[];
extern ACTOR_PLACE    LaliveroActorPlacements[];
extern ACTOR_PLACE    LaliveroActorPlacementsAfterLighthouse[];
extern Sint16         LaliveroDoorMetatiles[][2];
extern Uint16         LaliveroDoorOpenSteps[];
extern Uint16         LaliveroGateOpenSteps[];
extern CUE            LaliveroCues[];
extern CUE            LaliveroCuesAfterLighthouse[];

static void           scLaliveroTalkAboutIdejimaShip(void);

// Entrances that set up the visit after Venus Lighthouse, the first as if the party had also seen
// the ship at Idejima (FLAG_SAW_SHIP_AT_IDEJIMA). No exit leads to them and the entrance table has
// no row for them (the aerie's exit leads into Faran's house,
// TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE). Only the debug scene selector, which exits to any
// entrance from 0 to 99, reaches them.
enum
{
    LALIVERO_ENTRANCE_AFTER_LIGHTHOUSE         = 90,
    LALIVERO_ENTRANCE_AFTER_LIGHTHOUSE_QUIETLY = 91
};

// The placed actors, numbered in the order of LaliveroActorPlacements.
// LaliveroActorPlacementsAfterLighthouse, for the later visit, places the same people in the same
// rows, then the sign and the Black Orb. Its rows for Garet, Ivan and Mia take their own actor ids
// (fdScriptSpawnActorPlacements).
typedef enum LALIVERO_ACTOR_ID
{
    // Tolbi's soldiers, lying beaten in the street on the first visit. "Uhhnn... We were taken by
    // surprise..." Later, "Ha ha! I thought they had done me in for sure, but I'm all better now."
    ACTOR_WEST_SOLDIER = ACTOR_FIRST_PLACED,
    // "They were terrible! We were no match..."
    ACTOR_EAST_SOLDIER,
    // A young man with brown hair who sobs for Sheba (LaliveroCryingScript). "Ohh... They took
    // Sheba and headed for the ruins."
    ACTOR_VILLAGER_THEY_TOOK_SHEBA,
    // A woman with fair hair. "Sheba is our only hope. If something happens to her, Lalivero is
    // finished." Later she asks whether the party is leaving Lalivero.
    ACTOR_WOMAN_OUR_ONLY_HOPE,
    // A man in a white turban. "This all happened because Tolbi took Sheba away from us..."
    ACTOR_VILLAGER_BLAMING_TOLBI,
    // A woman in a green dress. "Faran followed Sheba. I'm sure he'll rescue her."
    ACTOR_WOMAN_TRUSTING_FARAN,
    // An old man with a cane.
    ACTOR_OLD_MAN_RUMBLING_WARNING,
    // An old woman.
    ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE,
    // A small boy with fair hair. "If I were bigger, I would have saved Sheba myself!"
    ACTOR_BOY_IF_I_WERE_BIGGER,
    // A little girl with orange pigtails. "I bet Sheba can use her powers to escape them."
    ACTOR_GIRL_SHEBAS_POWERS,
    // A soldier who keeps the door to Babi's ship. "Babi's most precious belonging is kept here. No
    // one may enter!"
    ACTOR_WHARF_GUARD,
    // Three plants, SPRITE_SPROUT, the sprout Growth grows at Alpine Crossing and Suhalla Gate.
    // Here they stand half grown, block nothing and no cue answers them.
    ACTOR_FIRST_PLANT,
    // Placed off the map until needed.
    ACTOR_IODEM = ACTOR_FIRST_PLACED + 14,
    // Torch, a Mars Djinni, placed until it joins.
    ACTOR_TORCH,
    // Placed after Venus Lighthouse. "No trespassing. Danger beyond this point."
    ACTOR_NORTH_EXIT_SIGN,
    // The orb Iodem holds out, drawn from its item icon.
    ACTOR_BLACK_ORB
} LALIVERO_ACTOR_ID;

// The metatiles of the town the scene gives a meaning of its own. Each door's and roof hatch's
// metatile is the number of the exit it leads by.
typedef enum LALIVERO_METATILE_BEHAVIOR
{
    // The Sanctum's door, a gate two metatiles wide.
    METATILE_BEHAVIOR_LALIVERO_SANCTUM_GATE         = LALIVERO_TO_TIMBER_ROOMS_B_NORTHEAST,
    // Two metatiles a few steps from Faran's door, (6, 23) and (6, 24), where Garet stops Isaac for
    // Iodem's farewell.
    METATILE_BEHAVIOR_LALIVERO_OUTSIDE_FARANS_HOUSE = 50,
    // The south gateway, metatiles (12, 25) to (18, 25), where after the farewell Garet turns Isaac
    // back toward the ship.
    METATILE_BEHAVIOR_LALIVERO_SOUTH_GATE           = 51
} LALIVERO_METATILE_BEHAVIOR;

// A metatile of the wall along the town's south edge (METATILE_BEHAVIOR_WALL), whose attributes
// scInit copies onto metatiles the party may not cross.
enum
{
    WALL_METATILE_X = 20,
    WALL_METATILE_Y = 23
};

// Talking to Torch, a Mars Djinni on a roof in Lalivero, makes it join the party, on either visit.
static void scLaliveroTalkToTorch(void)
{
    fdDjinnJoinParty(ACTOR_TORCH, ELEMENT_MARS, DJINNI_INDEX(DJINNI_TORCH));
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return LaliveroEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return LaliveroExits;
}

// The townsfolk, who stand differently once the party is back from Venus Lighthouse.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0)
    {
        return LaliveroActorPlacementsAfterLighthouse;
    }
    return LaliveroActorPlacements;
}

// On the first visit to Lalivero, the young man sobbing for Sheba stops to say she was taken to the
// ruins, then turns away north and sobs again.
static void scLaliveroTalkToVillagerTheyTookSheba(Sint32 actorId)
{
    fdActorGet(actorId)->scaleX = toFIXED(1);
    fdActorGet(actorId)->scaleY = toFIXED(1);
    fdScriptSetMessageId(MsgLaliveroOhhTheyTookShebaHeaded);
    // "Ohh... They took Sheba and headed for the ruins."
    fdScriptShowMessage(actorId, 0);
    fdActorFaceDirection(actorId, FACING_NORTH, 0);
    fdScriptWait(20);
    fdActorSetScript(actorId, LaliveroCryingScript);
}

// After Venus Lighthouse, the fair-haired woman in Lalivero asks whether the party is leaving
// Lalivero, and wishes it well either way.
static void scLaliveroWomanAsksIfLeaving(Sint32 actorId)
{
    Sint32 message = MsgLaliveroLeavingLalivero;
    fdScriptSetMessageId(message);
    // "Are you leaving Lalivero?"
    fdScriptOpenMessage(actorId, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(10);
        fdScriptSetMessageId(message + 1);
    }
    else
    {
        fdScriptSetMessageId(message + 2);
    }
    fdScriptShowMessage(actorId, 0);
}

// After Venus Lighthouse, the soldier at the door of the shack in the north-west of Lalivero says
// he was told to let the party in, and steps aside west of the door (FLAG_LALIVERO_WHARF_OPEN).
static void scLaliveroWharfGuardLetsPartyThrough(void)
{
    gmFlagSet(FLAG_LALIVERO_WHARF_OPEN);
    fdScriptSetMessageId(MsgLaliveroWasToldLetInIf);
    // "I was told to let you in if you showed up... The door is open."
    fdScriptShowMessage(ACTOR_WHARF_GUARD, 0);
    fdActorSetSpeed(ACTOR_WHARF_GUARD, 1);
    fdActorWalkByAndWait(ACTOR_WHARF_GUARD, -16, 0);
    fdActorFaceDirection(ACTOR_WHARF_GUARD, FACING_EAST, 0);
    fdScriptWait(10);
}

// Walking north into one of Lalivero's doors opens it, or the Sanctum's gate, and Isaac walks in.
static void scLaliveroEnterDoor(void)
{
    Sint32 door                                 = FieldStateWork->stepCue;
    Sint32 x                                    = LaliveroDoorMetatiles[door][0];
    Sint32 y                                    = LaliveroDoorMetatiles[door][1];

    fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = ACTOR_FALLS;
    sySoundPlay(SE_DOOR_OPEN);
    if (door == METATILE_BEHAVIOR_LALIVERO_SANCTUM_GATE)
    {
        fdMapGridAnimateMetatiles(LaliveroGateOpenSteps, (Uint16)x, (Uint16)y);
        fdActorWalkBy(ACTOR_PARTY_LEADER, 0, -16);
    }
    else
    {
        fdMapGridAnimateMetatiles(LaliveroDoorOpenSteps, (Uint16)x, (Uint16)y);
        fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    }
    fdScriptWait(10);
    FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
    syImportScriptSetExitRequest(door);
}

// Stepping onto the hatch in the flat roof of Faran's house (metatile (6, 18)) or of the weapon and
// armor shop (metatile (26, 15)) in Lalivero takes Isaac down inside.
static void scLaliveroEnterDoorway(void)
{
    FIELD_STATE *state     = FieldStateWork;
    Sint16      *cue       = (Sint16 *)&state->stepCue;
    Sint32       exit      = *cue;

    state->transitionFrame = MAP_TRANSITION_TICKS;
    syImportScriptSetExitRequest(exit);
}

// On the first visit to Lalivero, walking in through the south gate with Iodem, the party finds two
// more of Tolbi's soldiers lying beaten in the gateway, who tell Iodem that Sheba's captors went to
// the ruins beneath Babi Lighthouse.
static void scLaliveroWatchWoundedSoldiers(void)
{
    ACTOR *leader;

    fdScriptStart();
    fdScriptSetMessageId(MsgLaliveroMoreTolbisSoldiersLayDefeated);
    // The party walks in from the south; the two soldiers lie on the ground.
    fdActorSetPos(ACTOR_PARTY_LEADER, 248, 424);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorSetAnimation(ACTOR_WEST_SOLDIER, ANIM_STILL);
    fdActorSetAnimation(ACTOR_EAST_SOLDIER, ANIM_STILL);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(20);
    // Iodem steps out from the party and looks them over.
    syFieldActorLaunchFromLeader(ACTOR_IODEM, 8, -16, FACING_NORTH);
    fdActorWaitForMoveById(ACTOR_IODEM);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdActorSetSpeed(ACTOR_IODEM, 1);
    fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 40);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "More of Tolbi's soldiers lay defeated..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(30);
    // "Is this more of their work?"
    fdScriptOpenMessage(ACTOR_IODEM, 0);
    fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
    fdScriptWait(30);
    // The soldiers know Iodem's voice: seven attackers took Sheba.
    fdActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
    fdScriptWait(20);
    // "That voice... It's Iodem..."
    fdScriptShowMessage(ACTOR_WEST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 30);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "You survived!"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdActorSetAnimation(ACTOR_WEST_SOLDIER, ANIM_STAND);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
    fdScriptWait(30);
    // "We suffered great losses..."
    fdScriptShowMessage(ACTOR_WEST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(20);
    // "Were you fighting a group of six people?"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
    fdScriptWait(20);
    // "There were...seven..."
    fdScriptShowMessage(ACTOR_EAST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 0);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_EAST_SOLDIER, 30);
    fdActorSetAnimation(ACTOR_EAST_SOLDIER, ANIM_STAND);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    // "You've survived as well!"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "There were seven? Is this the same group you were following..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
    fdScriptWait(30);
    // "I'm certain of it. One of them must have been...Sheba."
    fdScriptShowMessage(ACTOR_EAST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 20);
    // "Sheba, you say... Is this true!?"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
    fdScriptWait(20);
    // "The scholar Kraden was protecting her. I'm sure it was Sheba..."
    fdScriptShowMessage(ACTOR_WEST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(30);
    // "Did you hear that, Isaac? What do they want with Sheba?"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(20);
    fdScriptWait(20);
    // Iodem paces while they tell where the attackers went.
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    fdActorSetSpeed(ACTOR_IODEM, 1.8);
    fdActorWalkToAndWait(ACTOR_IODEM, 256, 360);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    fdScriptWait(30);
    // "Sheba... They all..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
    fdScriptWait(20);
    // "Once they finished with us, they headed for Babi Lighthouse."
    fdScriptShowMessage(ACTOR_EAST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    fdScriptWait(20);
    // "What do they hope to find at Babi Lighthouse?"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_EAST_SOLDIER, ANIM_NOD);
    fdScriptWait(30);
    // "They spoke of the ruins beneath the lighthouse..."
    fdScriptShowMessage(ACTOR_EAST_SOLDIER, 0);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_IODEM, 1.6);
    fdActorWalkToAndWait(ACTOR_IODEM, 256, 384);
    fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    fdScriptWait(20);
    // "Ruins? Of course! The foundations of the lighthouse!"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
    fdScriptWait(20);
    // "And Faran, the master of the Lalivero, followed them into..."
    fdScriptShowMessage(ACTOR_WEST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    fdScriptWait(40);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(20);
    // "I see. Faran, too..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetSpeed(ACTOR_IODEM, 1.2);
    // Iodem resolves to follow; the soldiers send the party on.
    fdActorWalkByAndWait(ACTOR_IODEM, 0, 16);
    fdScriptWait(10);
    // "Isaac! We must head for Babi Lighthouse as well."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_WEST_SOLDIER, 40);
    fdActorFaceActor(ACTOR_IODEM, ACTOR_EAST_SOLDIER, 40);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(30);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "Are you concerned about these soldiers?"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_EAST_SOLDIER, 2);
    fdScriptWait(20);
    // "We're fine... You must hurry to the lighthouse."
    fdScriptShowMessage(ACTOR_EAST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_EAST_SOLDIER, 30);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_WEST_SOLDIER, 2);
    fdScriptWait(20);
    // "The lighthouse... Sheba... If anything happens... Just hurry..."
    fdScriptShowMessage(ACTOR_WEST_SOLDIER, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IODEM, FACING_WEST, 0);
    fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WEST_SOLDIER, 70);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdScriptWait(40);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "Even the soldiers insist, Isaac! Come on!"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // Iodem walks back into the leader.
    fdActorSetSpeed(ACTOR_IODEM, 1.2);
    fdActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetDestination(ACTOR_IODEM, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_IODEM);
    fdActorSetPos(ACTOR_IODEM, 0, 0);
    fdScriptWait(10);
    fdScriptEnd();
}

// After Venus Lighthouse, a few steps out of Faran's house in the south-west of Lalivero (metatiles
// (6, 23) and (6, 24)), Garet stops Isaac, for Iodem asked them to wait. Iodem comes out of the
// house, talks of Lemuria and of Babi's promise, and gives the party the Black Orb that sails
// Babi's ship. Plays once.
static void scLaliveroIodemGivesBlackOrb(void)
{
    ACTOR *leader;

    gmFlagSet(FLAG_IODEM_FAREWELL_SEEN);
    fdScriptStart();
    fdScriptSetMessageId(MsgLaliveroWhereGoingIsaacIodemAsked);
    // Garet reminds the leader that Iodem asked them to wait; the answer picks Mia's reply.
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 104, 376);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    syFieldActorLaunchFromLeader(ACTOR_GARET, -32, 0, FACING_EAST);
    syFieldActorLaunchFromLeader(ACTOR_MIA, -16, 16, FACING_NORTHEAST);
    fdActorLaunchFromLeader(ACTOR_IVAN, 0, 16, FACING_NORTH);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdScriptWait(30);
    // "Where are you going, Isaac? Iodem asked us to wait. Did you..."
    fdScriptOpenMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdScriptWait(10);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "We're going to need to work on your memory, Isaac."
        fdScriptShowMessage(ACTOR_MIA, 0);
        FieldStateWork->msg += 1;
    }
    else
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        FieldStateWork->msg += 1;
        // "I know we need to leave, but we should hear what Iodem has..."
        fdScriptShowMessage(ACTOR_MIA, 0);
    }
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
    // "We can delay our departure a little longer. Let's wait."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(40);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    // The party lines up facing Iodem's door.
    fdActorWalkByAndWait(ACTOR_PARTY_LEADER, -16, 0);
    fdActorWalkBy(ACTOR_IVAN, 0, -16);
    fdActorWalkBy(ACTOR_MIA, 0, -8);
    fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(80);
    // Iodem comes out: the door opens (metatiles 34, 0 drawn at 4, 18) and closes behind him
    // (metatiles 32, 0).
    fdActorSetSpeed(ACTOR_IODEM, 0.8);
    fdActorGet(ACTOR_IODEM)->motionFlags = ACTOR_FALLS;
    fdActorSetSpritePriority(ACTOR_IODEM, 2);
    fdMapGridCopyMetatilesLayered(34, 0, 1, 2, 4, 18);
    sySoundPlay(SE_DOOR_OPEN);
    fdScriptWait(20);
    fdActorSetPos(ACTOR_IODEM, 72, 312);
#if IODEM_DOOR_WALK_RESET
    fdActorGet(ACTOR_IODEM)->targetZ = ACTOR_NO_TARGET;
    fdActorSetPos(ACTOR_IODEM, 72, 312);
#endif
    fdScriptWait(20);
#if IODEM_DOOR_WALK_RESET
    fdActorWalkToAndWait(ACTOR_IODEM, 72, 328);
#else
    fdActorWalkByAndWait(ACTOR_IODEM, 0, 16);
#endif
    // The door shuts.
    fdMapGridCopyMetatilesLayered(32, 0, 1, 2, 4, 18);
    sySoundPlay(SE_DOOR_CLOSE);
    fdScriptWait(20);
    // "Thank you for waiting for me."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(40);
    fdActorWalkByAndWait(ACTOR_IODEM, 16, 0);
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
    fdScriptWait(20);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // "As you no doubt have guessed, I need to speak with you about..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    // "Yes, the island of the ancients far out to sea."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "When you could not find Jenna, you returned to Venus Lighthouse."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    // "Yeah. The base of the lighthouse was flooded, so we couldn't..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "And that's when you gazed out to sea, as you promised Babi..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "Yes... And we saw an area wrapped in mist."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // "I remembered how Babi said Lemuria was always covered in mist."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorFaceEachOther(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdScriptWait(40);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // "I am not sure, but I am beginning to think that may be..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "If it is, then Babi's life..."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(20);
    // "...could be saved? Yes."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdScriptWait(20);
    // "Then I guess we have no choice but to go..."
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // "Good. Then it's settled. I'll make arrangements for you to..."
        fdScriptShowMessage(ACTOR_IODEM, 0);
        FieldStateWork->msg += 1;
    }
    else
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        FieldStateWork->msg += 1;
        // "You swore an oath. It is meaningless unless you go immediately."
        fdScriptShowMessage(ACTOR_IODEM, 0);
    }
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "But...we have to find Jenna first..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
    // "That is the other reason I wanted to see you... to give you..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(20);
    // Iodem takes out the Black Orb.
    fdActorSetPos(ACTOR_BLACK_ORB, 88, 332);
    // "Iodem takes out the Black Orb."
    fdScriptShowMessage(-1, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorJump(ACTOR_MIA, 4, 13);
    fdActorJump(ACTOR_MIA, 4, 30);
    // "Oh, isn't that..."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // "You will need it to sail Babi's ship."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    // Arriving through the aerie, the party also talks about the ship they saw at Idejima.
    if (gmFlagGet(FLAG_SAW_SHIP_AT_IDEJIMA) != 0)
    {
        scLaliveroTalkAboutIdejimaShip();
    }
    fdScriptSetMessageId(MsgLaliveroBabiToldMeShipAncients);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "Babi told me the ship of the ancients is an incredible vessel."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // "It will make your search for Jenna easier."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "We appreciate it."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "But I want you to find Lemuria as soon as possible."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "We promised Babi we would find Lemuria. We will not fail him."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // He holds the orb out, the leader takes it, and he steps back.
    fdActorSetSpeed(ACTOR_BLACK_ORB, 0.8);
    fdActorSetDestinationOffset(ACTOR_BLACK_ORB, 0, 16);
    fdActorWalkByAndWait(ACTOR_IODEM, 0, 16);
    fdScriptWait(30);
    fdActorSetPos(ACTOR_BLACK_ORB, 0, 0);
    FieldStateWork->msg += 1;
    gmPartyGiveItem(ITEM_BLACK_ORB, 0);
    fdScriptWait(10);
    fdActorGet(ACTOR_IODEM)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
    fdActorGet(ACTOR_IODEM)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTH + TRIG_TURN / 256, 0);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    // "The ship is docked at the edge of town. You will know it..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // "Farewell! And bring back news of Lemuria!"
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(50);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    fdScriptWait(30);
    // Iodem goes back inside.
    fdActorWalkByAndWait(ACTOR_IODEM, -16, 0);
    fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    fdScriptWait(20);
    fdMapGridCopyMetatilesLayered(34, 0, 1, 2, 4, 18);
    sySoundPlay(SE_DOOR_OPEN);
    fdScriptWait(10);
    fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
    fdActorSetPos(ACTOR_IODEM, 0, 0);
    fdScriptWait(10);
    // The door shuts.
    fdMapGridCopyMetatilesLayered(32, 0, 1, 2, 4, 18);
    sySoundPlay(SE_DOOR_CLOSE);
    fdScriptWait(50);
    // The others are eager to sail, then walk back into the leader.
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
    fdActorFaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    fdScriptWait(20);
    // "Right on, Isaac! We finally get to see the ocean!"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(20);
    // "To sea we go!"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "It is time to depart."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(40);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(30);
    fdActorSetSpeed(ACTOR_GARET, 1.2);
    fdActorSetSpeed(ACTOR_MIA, 1.2);
    fdActorSetSpeed(ACTOR_IVAN, 1.2);
    fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetDestination(ACTOR_GARET, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorSetPos(ACTOR_GARET, 0, 0);
    fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetDestination(ACTOR_MIA, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_MIA);
    fdActorSetPos(ACTOR_MIA, 0, 0);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetDestination(ACTOR_IVAN, leader->x.part[0].integer, leader->z.part[0].integer);
    }
    fdActorWaitForMoveById(ACTOR_IVAN);
    fdActorSetPos(ACTOR_IVAN, 0, 0);
    fdScriptWait(10);
    fdScriptEnd();
}

// During Iodem's farewell, if the party saw the ship at Idejima, Iodem says it was not Babi's,
// which is here in Lalivero, and Garet guesses it was Saturos's.
static void scLaliveroTalkAboutIdejimaShip(void)
{
    fdScriptSetMessageId(MsgLaliveroThoughtShipWeSawAt);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
    fdScriptWait(20);
    // "I thought the ship we saw at Idejima was Babi's..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "His ship is here in Lalivero."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 55);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    // "So the ship at the peninsula was Saturos's ship!"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // "Perhaps, but we have no way of finding that out now..."
    fdScriptShowMessage(ACTOR_IODEM, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    // "Yes... They're gone..."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(30);
    // "But now we can sail the ship of the ancients!"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdScriptWait(65);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(40);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // "It certainly will aid us in our search for Jenna."
    fdScriptShowMessage(ACTOR_IVAN, 0);
}

// After Iodem's farewell, walking out through Lalivero's south gate (metatiles (12, 25) to (18,
// 25)) makes Garet ask whether they are not taking Babi's ship, and Isaac steps back into town.
static void scLaliveroTurnBackToTheShip(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgLaliveroWhereGoingArentWeTaking);
    // "Where are you going? Aren't we taking Babi's ship? Then we..."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
    fdScriptEnd();
}

// What the party can talk to, touch and search, which changes once they are back from Venus
// Lighthouse.
const CUE *scGetCues(void)
{
    if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0)
    {
        return LaliveroCuesAfterLighthouse;
    }
    return LaliveroCues;
}

// Sets Lalivero up as the party enters: the Black Orb Iodem will hold out, the wounded soldiers of
// the first visit (and their scene), or the sign at the closed north gate after Venus Lighthouse.
Sint32 scInit(void)
{
    ACTOR         *actor;
    SPR           *sprite;
    ICON_TRANSFER *icon;

    // Coming back from the lighthouse switches the town to its later state.
    if (MainRoutineStateWork.entrance == LALIVERO_ENTRANCE_AFTER_LIGHTHOUSE)
    {
        gmFlagSet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
        gmFlagSet(FLAG_SAW_SHIP_AT_IDEJIMA);
    }
    if (MainRoutineStateWork.entrance == LALIVERO_ENTRANCE_AFTER_LIGHTHOUSE_QUIETLY)
    {
        gmFlagSet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
    }
    FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    FieldStateWork->transitionFrame = 24;

    // The three plants stand half grown and block nothing.
    fdActorSetAnimation(ACTOR_FIRST_PLANT, SPROUT_ANIMATION_HALF_GROWN);
    fdActorGet(ACTOR_FIRST_PLANT)->collisionFlags = 0;
    fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_PLANT), 0);
    fdActorSetAnimation(ACTOR_FIRST_PLANT + 1, SPROUT_ANIMATION_HALF_GROWN);
    fdActorGet(ACTOR_FIRST_PLANT + 1)->collisionFlags = 0;
    fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_PLANT + 1), 0);
    fdActorSetAnimation(ACTOR_FIRST_PLANT + 2, SPROUT_ANIMATION_HALF_GROWN);
    fdActorGet(ACTOR_FIRST_PLANT + 2)->collisionFlags = 0;
    fdActorSetSpriteFlags(fdActorGet(ACTOR_FIRST_PLANT + 2), 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_BLACK_ORB), 0);

    // The Black Orb Iodem hands over is drawn from its item icon, floating 10 pixels up.
    actor                    = fdActorGet(ACTOR_BLACK_ORB);
    actor->retainSpriteCache = 1;
    actor->motionFlags       = 0;
    sprite                   = actor->sprite;
    actor->y.fixed           = toFIXED(10);
    sprite->count            = 0;
    sprite->image.fullColor  = 0;
    sprite->image.palette    = 0;
    icon                     = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
    mnIconDrawItem(ITEM_BLACK_ORB);
    syVRAMCacheLoad(sprite->slot, ICON_TILE_BYTES, icon->tiles);
    syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);

    if (gmFlagGet(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED))
    {
        // A sign now stands at the north exit, and the three open metatiles of the gap in the north
        // wall become wall, closing the way north.
        fdActorSetSpriteFlags(fdActorGet(ACTOR_NORTH_EXIT_SIGN), 0);
        fdActorGet(ACTOR_NORTH_EXIT_SIGN)->collisionFlags = 0;
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 14, 4);
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 15, 4);
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 16, 4);
        if (gmFlagGet(FLAG_LALIVERO_WHARF_OPEN))
        {
            fdActorSetPos(ACTOR_WHARF_GUARD, 56, 184);
        }
    }
    else
    {
        // The first visit: the two soldiers lie on the ground, drawn under the party with their own
        // collision off, and the two metatiles each covers become wall instead. The third metatile
        // copied, at x 78, lies outside the town's 32 columns, where the camera never goes, and
        // changes nothing.
        actor                    = fdActorGet(ACTOR_WEST_SOLDIER);
        actor->collisionFlags    = 0;
        actor->priorityFlags    |= ACTOR_PRIORITY_UNDERFOOT;
        actor->sprite->flags     = 0;
        actor->sprite->rotation  = FACING_NORTH;
        actor                    = fdActorGet(ACTOR_EAST_SOLDIER);
        actor->collisionFlags    = 0;
        actor->priorityFlags    |= ACTOR_PRIORITY_UNDERFOOT;
        actor->sprite->flags     = 0;
        actor->sprite->rotation  = FACING_SOUTH;
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 13, 23);
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 14, 23);
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 78, 23);
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 17, 23);
        fdMapGridCopyMetatileAttributes(WALL_METATILE_X, WALL_METATILE_Y, 1, 1, 18, 23);
        if ((MainRoutineStateWork.entrance == LALIVERO_FROM_WORLD_MAP_EAST
             || MainRoutineStateWork.entrance == LALIVERO_FROM_WORLD_MAP_WEST)
            && !fdScriptGetFlag(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN))
        {
            gmFlagSet(FLAG_LALIVERO_WOUNDED_SOLDIERS_SEEN);
            // The woman and the girl who wander nearest the soldiers stay put during the script.
            actor         = fdActorGet(ACTOR_WOMAN_OUR_ONLY_HOPE);
            actor->paused = 1;
            actor         = fdActorGet(ACTOR_GIRL_SHEBAS_POWERS);
            actor->paused = 1;
            scLaliveroWatchWoundedSoldiers();
            actor         = fdActorGet(ACTOR_WOMAN_OUR_ONLY_HOPE);
            actor->paused = 0;
            actor         = fdActorGet(ACTOR_GIRL_SHEBAS_POWERS);
            actor->paused = 0;
        }
    }
    return 0;
}

// The sobbing of ACTOR_VILLAGER_THEY_TOOK_SHEBA: the sprite squashes and stretches twice, then
// rests 80 ticks.
Sint32 LaliveroCryingScript[] = {
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 4),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE LaliveroEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LALIVERO_FROM_TIMBER_ROOMS_C_NORTHWEST,
     CONDITION_ALWAYS, 72,
     0, 184,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_WEST,
     CONDITION_ALWAYS, 152,
     0, 216,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_C_NORTHEAST,
     CONDITION_ALWAYS, 200,
     0, 168,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_NORTHWEST,
     CONDITION_ALWAYS, 296,
     0, 168,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_NORTH,
     CONDITION_ALWAYS, 360,
     0, 168,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_NORTHEAST,
     CONDITION_ALWAYS, 432,
     0, 152,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_C_SOUTHWEST,
     CONDITION_ALWAYS, 72,
     0, 328,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_C_SOUTH,
     CONDITION_ALWAYS, 104,
     0, 312,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_SOUTH,
     CONDITION_ALWAYS, 360,
     0, 280,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_SOUTHEAST,
     CONDITION_ALWAYS, 440,
     0, 328,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_TIMBER_ROOMS_B_EAST_SOUTHEAST,
     CONDITION_ALWAYS, 424,
     0, 264,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_WORLD_MAP_EAST,
     CONDITION_ALWAYS, 248,
     0, 416,
     FACING_NORTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {LALIVERO_FROM_WORLD_MAP_WEST,
     CONDITION_ALWAYS, 248,
     0, 64,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {SCENE_TABLE_END},
};

Uint32 LaliveroExits[] = {
    SCENE_EXITS(Lalivero),
    SCENE_EXIT(
        LALIVERO_TO_TIMBER_ROOMS_C_NORTHWEST, TimberRoomsC, TIMBER_ROOMS_C_FROM_LALIVERO_NORTHWEST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_WEST, TimberRoomsB, TIMBER_ROOMS_B_FROM_LALIVERO_WEST),
    SCENE_EXIT(
        LALIVERO_TO_TIMBER_ROOMS_C_NORTHEAST, TimberRoomsC, TIMBER_ROOMS_C_FROM_LALIVERO_NORTHEAST),
    SCENE_EXIT(
        LALIVERO_TO_TIMBER_ROOMS_B_NORTHWEST, TimberRoomsB, TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_NORTH, TimberRoomsB, TIMBER_ROOMS_B_FROM_LALIVERO_NORTH),
    SCENE_EXIT(
        LALIVERO_TO_TIMBER_ROOMS_B_NORTHEAST, TimberRoomsB, TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST),
    SCENE_EXIT(
        LALIVERO_TO_TIMBER_ROOMS_C_SOUTHWEST, TimberRoomsC, TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_C_SOUTH, TimberRoomsC, TIMBER_ROOMS_C_FROM_LALIVERO_SOUTH),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_SOUTH, TimberRoomsB, TIMBER_ROOMS_B_FROM_LALIVERO_SOUTH),
    SCENE_EXIT(
        LALIVERO_TO_TIMBER_ROOMS_B_SOUTHEAST, TimberRoomsB, TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST),
    SCENE_EXIT(LALIVERO_TO_TIMBER_ROOMS_B_EAST_SOUTHEAST,
               TimberRoomsB,
               TIMBER_ROOMS_B_FROM_LALIVERO_EAST_SOUTHEAST),
    SCENE_EXIT(LALIVERO_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_LALIVERO_EAST),
    SCENE_EXIT(LALIVERO_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_LALIVERO_WEST),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE LaliveroActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        224, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        288, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (Sint32)LaliveroCryingScript,
        248, 0, 96, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        312, 0, 280, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        304, 0, 200, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 296, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        440, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        408, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        200, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        248, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 160, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 128, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 400, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_TORCH), MOVEMENT_TYPE_STAND,
        200, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE LaliveroActorPlacementsAfterLighthouse[] = {
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 376, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        248, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        312, 0, 296, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        304, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 296, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        440, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        408, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        200, 0, 184, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        248, 0, 232, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 160, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 128, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        440, 0, 400, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_TORCH), MOVEMENT_TYPE_STAND,
        200, 0, 136, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

// The metatile of each door by its metatile behaviour, and a door opening and the Sanctum's gate opening.
Sint16 LaliveroDoorMetatiles[][2] = {
    { 0, 0 }, { 4, 9 }, { 9, 11 }, { 12, 8 }, { 18, 8 }, { 22, 8 }, { 26, 7 }, { 4, 18 },
    { 0, 0 }, { 22, 15 }, { 27, 18 }, { 0, 0 },
};
Uint16 LaliveroDoorOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(33, 0, 1, 2, 8), MAPGRID_ANIMATION_STEP(34, 0, 1, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 LaliveroGateOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(34, 2, 2, 2, 8), MAPGRID_ANIMATION_STEP(36, 2, 2, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

CUE LaliveroCues[] = {
    { CUE_EXIT, LALIVERO_TO_WORLD_MAP_EAST, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_EAST },
    { CUE_EXIT, LALIVERO_TO_WORLD_MAP_WEST, CONDITION_ALWAYS, LALIVERO_TO_WORLD_MAP_WEST },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_C_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_C_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_LALIVERO_SANCTUM_GATE, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_C_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { CUE_STEP, LALIVERO_TO_TIMBER_ROOMS_C_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoorway) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { CUE_STEP, LALIVERO_TO_TIMBER_ROOMS_B_EAST_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoorway) },
    { CUE_TALK, ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroUhhnnWeWereTakenBySurprise) },
    { CUE_TALK, ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyWereTerribleWeWereNo) },
    { CUE_TALK, ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroTalkToVillagerTheyTookSheba) },
    { CUE_TALK, ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorIsOurOnlyHopeIf) },
    { CUE_TALK, ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisAllHappenedBecauseTolbiTook) },
    { CUE_TALK, ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranFollowedActorImSureHell) },
    { CUE_TALK, ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheRumblingAtTheRuinsMust) },
    { CUE_TALK, ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroThereWasAnEarthquakeAtThe) },
    { CUE_TALK, ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfIWereBiggerIWould) },
    { CUE_TALK, ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroIBetActorCanUseHer) },
    { CUE_TALK, ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroBabisMostPreciousBelongingIsKept) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfOnlyTheyDidntHaveActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyUsedFireOnUsThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroCursesWeCouldntStopActorAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhatDoTheyPlanToDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeNeverShouldHaveLetTolbi) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroFaranStillDoesNotReturnWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroWeHaveSeenManyOmensOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheStonesWeStackedForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroITriedToStepInFront) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheyAllHaveALessonComing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWonderWhatBabisMostPrecious) },
    { CUE_TALK, ACTOR_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroTalkToTorch) },
    { EXAMINE_CUE(EXAMINE_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { EXAMINE_CUE(EXAMINE_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    // XXX (no evidence): why this chest opens only facing west, when

    // LaliveroCuesAfterLighthouse opens it from any side; the chest and the ground around it are the same on both visits.
    { EXAMINE_CHEST_FACING(FACING_WEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { CUE_TABLE_END },
};

CUE LaliveroCuesAfterLighthouse[] = {
    { CUE_STEP, METATILE_BEHAVIOR_LALIVERO_OUTSIDE_FARANS_HOUSE, FLAG_IODEM_FAREWELL_SEEN, SCRIPT_REF(scLaliveroIodemGivesBlackOrb) },
    { CUE_STEP, METATILE_BEHAVIOR_LALIVERO_SOUTH_GATE, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroTurnBackToTheShip) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_C_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_C_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_LALIVERO_SANCTUM_GATE, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_C_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { CUE_STEP, LALIVERO_TO_TIMBER_ROOMS_C_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoorway) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), LALIVERO_TO_TIMBER_ROOMS_B_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoor) },
    { CUE_STEP, LALIVERO_TO_TIMBER_ROOMS_B_EAST_SOUTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroEnterDoorway) },
    { CUE_TALK, ACTOR_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, SCRIPT_REF(scLaliveroWharfGuardLetsPartyThrough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), CUE_SUBJECT_OWN_EFFECT | ACTOR_WHARF_GUARD, FLAG_LALIVERO_WHARF_OPEN, SCRIPT_REF(scLaliveroWharfGuardLetsPartyThrough) },
    { CUE_TALK, ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroHaHaIThoughtTheyHad) },
    { CUE_TALK, ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWillNeverLetMyGuard) },
    { CUE_TALK, ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroWhereCouldActorBeNowIm) },
    { CUE_TALK, ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroWomanAsksIfLeaving) },
    { CUE_TALK, ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroISawTheBeaconOfVenus) },
    { CUE_TALK, ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheExplosionInTheVenusRuins) },
    { CUE_TALK, ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroThePeninsulaBehindVenusLighthouseIs) },
    { CUE_TALK, ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroICantBelieveYouEscapedVenus) },
    { CUE_TALK, ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfYouDefeatedThatEvilBunch) },
    { CUE_TALK, ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorJumpedIntoTheOceanFrom) },
    { CUE_TALK, ACTOR_NORTH_EXIT_SIGN, CONDITION_ALWAYS, MSGNO(MsgLaliveroNoTrespassingDangerBeyondThisPoint) },
    { CUE_TALK, ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroApparentlyAShipHasBeenDocked) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIAmAllHealedNowBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgLaliveroIWantedToBeAPart) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_THEY_TOOK_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroIfActorUsedHerGreatPowers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_OUR_ONLY_HOPE, CONDITION_ALWAYS, MSGNO(MsgLaliveroAfterEverythingThatsHappenedLaliveroNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_BLAMING_TOLBI, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseHasBeenTheSymbol) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_TRUSTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroIodemOfTolbiDoesntSeemToo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_RUMBLING_WARNING, CONDITION_ALWAYS, MSGNO(MsgLaliveroTheOceanIsSoRoughSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_EARLIER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgLaliveroVenusLighthouseIsStillStandingStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_IF_I_WERE_BIGGER, CONDITION_ALWAYS, MSGNO(MsgLaliveroYouMustBeGreatWarriorsTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SHEBAS_POWERS, CONDITION_ALWAYS, MSGNO(MsgLaliveroActorWouldBeFineEvenAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WHARF_GUARD, CONDITION_ALWAYS, MSGNO(MsgLaliveroThisSureSeemsLikeAStrange) },
    { CUE_TALK, ACTOR_TORCH, CONDITION_ALWAYS, SCRIPT_REF(scLaliveroTalkToTorch) },
    { EXAMINE_CUE(EXAMINE_JAR), 100, FLAG_LALIVERO_WATER_OF_LIFE, FIND_ITEM(ITEM_WATER_OF_LIFE) },
    { EXAMINE_CUE(EXAMINE_STONE_PILLAR), 101, FLAG_LALIVERO_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_CUE(EXAMINE_CHEST), 102, FLAG_LALIVERO_WARRIORS_HELM, FIND_ITEM(ITEM_WARRIORS_HELM) },
    { CUE_TABLE_END },
};
// clang-format on

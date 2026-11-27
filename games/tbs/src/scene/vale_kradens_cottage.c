// Vale (SceneIdValeKradensCottage) is the part of the village around Kraden's cottage, by the fence
// that closes off the lands beyond.
//
// Kraden, "a trained Alchemy sage" in his own words, came to Vale to study Mt. Aleph and Alchemy.
//
// One actor placement table serves every visit. scInit removes the people a visit does not use, and
// scGetCues picks the scripts for the time of the story.
//
// On the stormy night of the prologue (FLAG_VALE_STORM), the storm's scene (Vale,
// SceneIdValePrologue) leads here by VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_NORTH and
// VALE_PROLOGUE_TO_VALE_KRADENS_COTTAGE_SOUTH. The party arrives by
// VALE_KRADENS_COTTAGE_FROM_VALE_NORTH and VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH, and the two paths
// lead back (VALE_KRADENS_COTTAGE_TO_VALE_PROLOGUE_NORTH and
// VALE_KRADENS_COTTAGE_TO_VALE_PROLOGUE_SOUTH).
//
// A rockslide has broken the fence and let monsters in. Initialize starts the lightning and leaves
// the two rocks that Isaac cannot budge across the way. It copies METATILE_BEHAVIOR_WALL over the
// path's six metatiles below the west rock and under the east one. Two of those metatiles are
// METATILE_BEHAVIOR_KRADENS_COTTAGE_PAST_THE_STRANGERS by day. It also lays a hurt man by the
// fence.
//
// Talking to the hurt man, or walking up to him
// (METATILE_BEHAVIOR_KRADENS_COTTAGE_BESIDE_THE_SURVIVOR), plays
// scValeKradensCottageTalkToRockslideSurvivor. He asks whether he will die. A yes leaves him giving
// up (FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP). A no has him find he is not hurt at all, warn them of
// the monsters on their way to the plaza and run off (FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED), after
// which he is no longer placed.
//
// Two metatiles start a monster battle each, once. They are the first two slots of the encounter
// zone of the scripted battles of Vale, Vault and the world map.
//
// Three years later, the day the story begins, Isaac comes up with Garet and Jenna to find two
// strangers outside.
//
// - Walking past the strangers
//   (METATILE_BEHAVIOR_KRADENS_COTTAGE_PAST_THE_STRANGERS)
//   plays
//   scValeKradensCottageMeetSaturosAndMenardi. The three overhear the two
//   speak of Sol Sanctum and of using Kraden, confront them and are let go.
//   The two call each other Saturos and Menardi
//   (FLAG_MET_SATUROS_AND_MENARDI). The strangers then stand aside, turning
// to watch the leader.
// - Before the cottage
// (METATILE_BEHAVIOR_KRADENS_COTTAGE_BEFORE_THE_COTTAGE), the  party
// overhears Kraden wondering  what the strangers know of the Elemental Stars
// (scValeKradensCottageOverhearKraden, FLAG_KRADEN_WONDERED_ABOUT_STRANGERS).
// - Talking to Kraden plays scValeKradensCottageKradenPlansSanctumVisit. Garet
//   brings up Saturos and Menardi. Kraden talks the three into a secret
// look at Sol Sanctum on the forbidden mountain, then goes in to prepare.
// - VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG then takes the party
//   into the cottage (SceneIdCottageRoomsB,
//   COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG).
// - Coming back out by VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH, Kraden
//   joins them for Mt. Aleph (scValeKradensCottageKradenSetsOff,
//   FLAG_SANCTUM_VISIT_PLANNED). From then on he and the strangers are
// gone.
//
// On that day a small boy at the fence hears growls beyond it, and a jar holds an Elixir.
//
// After the party has left Vale (FLAG_VALE_PARTY_LEFT) the child means to climb the fence one day.
// After Kalay (FLAG_KALAY_REACHED) a woman with fair hair visits the empty cottage, missing Kraden.
//
// By day the scene has these ways out.
//
// - The cottage's door (scValeKradensCottageEnterCottage) leads into the
//   cottage (VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_NORTH, to
//   SceneIdCottageRoomsB's
// COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_NORTH).
// - VALE_KRADENS_COTTAGE_TO_VALE_NORTH and VALE_KRADENS_COTTAGE_TO_VALE_SOUTH
//   go down into the village (SceneIdVale).
// - VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST and
//   VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST go into Vale Cave's way in
//   (SceneIdValeCaveMouth).
//
// Coming back from Vale Cave's way in by VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST clears
// FLAG_SHOW_AREA_NAME, so the area name is not shown again.
//
// The jar's row finds nothing on the stormy night. After Kalay it is there only from the third
// patch on.

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

// Menardi's answers to "Were you eavesdropping on our conversation?", counted from
// MsgValeTheyKnowLittleOfThe.
typedef enum EAVESDROP_MESSAGE
{
    EAVESDROP_MENARDI_FORBIDS_SPYING = 10,
    EAVESDROP_MENARDI_TAKES_WORD     = 11
} EAVESDROP_MESSAGE;

// Saturos's answers to "And this appointment is important enough to drive us away?", counted from
// MsgValeYoureTheOnesSneakingAround.
typedef enum APPOINTMENT_MESSAGE
{
    APPOINTMENT_SATUROS_GIVES_WAY     = 5,
    APPOINTMENT_KRADEN_SENT_THEM_AWAY = 6
} APPOINTMENT_MESSAGE;

// What the metatiles of this part of Vale do, by the cue rows that answer them. The ways out are
// numbered as the day's exits, and lead to the stormy night's on that night.
typedef enum VALE_KRADENS_COTTAGE_METATILE_BEHAVIOR
{
    // The way down into the village by the north.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_NORTH_WAY_TO_VALE = 1,
    // The way down into the village by the south.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_SOUTH_WAY_TO_VALE,
    // The cottage's front door.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_DOOR,
    // The lower path up to Vale Cave's way in.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_WEST_PATH_TO_VALE_CAVE,
    // The ground before the cottage, where the party overhears Kraden.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_BEFORE_THE_COTTAGE,
    // The raised path up to Vale Cave's way in.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_EAST_PATH_TO_VALE_CAVE,
    // The way past the two strangers, where the party meets Saturos and Menardi.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_PAST_THE_STRANGERS,
    // On the stormy night, the metatiles where a monster attacks, each once.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_FIRST_MONSTER = 10,
    METATILE_BEHAVIOR_KRADENS_COTTAGE_SECOND_MONSTER,
    // On the stormy night, beside the man hurt in the rockslide.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_BESIDE_THE_SURVIVOR = 20,
    // The jar that holds an Elixir.
    METATILE_BEHAVIOR_KRADENS_COTTAGE_JAR                 = 100
} VALE_KRADENS_COTTAGE_METATILE_BEHAVIOR;

Sint32                scInit(void);
extern Sint32         ValeKradensCottageRockslideSurvivorEscapeActions[];
extern Sint32         ValeKradensCottageRockslideSurvivorSquashes[];
extern Sint32         ValeKradensCottageFaceTargetActions[];
extern SCENE_ENTRANCE ValeKradensCottageEntrances[];
extern Uint32         ValeKradensCottageExits[];
extern ACTOR_PLACE    ValeKradensCottageActorPlacements[];
extern CUE            ValeKradensCottageCues[];
extern CUE            ValeKradensCottageStormCues[];
extern CUE            ValeKradensCottageCuesAfterKalay[];
extern Uint16         ValeKradensCottageDoorOpens[];

// The placed actors, numbered in the order of ValeKradensCottageActorPlacements. Garet and Jenna,
// placed between Kraden and Saturos, keep their own slots.
typedef enum VALE_KRADENS_COTTAGE_ACTOR_ID
{
    // This is a boulder by the fence. It has the sprite of the boulder Lift raises at the Alpine
    // Crossing (MsgMountainIsaacDidLiftBoulder) and in Gondowan Cave.
    //
    // Its Lift row runs no script, like ACTOR_RIVER_BOULDER's in gondowan_cave.c. The stormy
    // night's scripts have no row for it.
    //
    // Cast on it, Lift's two hands close on it and burst into sparkles. The boulder stays where it
    // is, and no message follows, since the row answers the cast.
    ACTOR_FENCE_BOULDER = ACTOR_FIRST_PLACED,
    // The west rock. "It's a huge rock that Isaac's strength can't even budge."
    ACTOR_WEST_ROCK,
    ACTOR_EAST_ROCK,
    // A young man with brown hair who lies by the broken fence on the stormy night.
    // "Rockslide...destroyed fence... Monsters everywhere..."
    ACTOR_ROCKSLIDE_SURVIVOR,
    // A small boy with fair hair, by day, younger than Isaac. "Do you hear those awful growls
    // coming from the other side of this fence?"
    ACTOR_BOY_BY_THE_FENCE,
    ACTOR_KRADEN,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    // A woman with fair hair, after Kalay. "You must have so many memories of this cottage, don't
    // you?"
    ACTOR_COTTAGE_VISITOR
} VALE_KRADENS_COTTAGE_ACTOR_ID;

#include "shared/actor_facing.c"

// The front door of Kraden's cottage stands open since Kraden set off, so walking in
// (METATILE_BEHAVIOR_KRADENS_COTTAGE_DOOR) skips its opening.
#define FLAG_VALE_KRADENS_DOOR_OPEN FLAG_SCENE_TEMP_42

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return ValeKradensCottageEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return ValeKradensCottageExits;
}

// Returns the one actor placement table for every visit. scInit removes the actors a visit does not
// use.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    return ValeKradensCottageActorPlacements;
}

// Selects the scripts for the current visit to Vale.
const CUE *scGetCues(void)
{
    if (gmFlagGet(FLAG_KALAY_REACHED) != 0)
    {
        return ValeKradensCottageCuesAfterKalay;
    }
    if (gmFlagGet(FLAG_VALE_STORM) != 0)
    {
        return ValeKradensCottageStormCues;
    }
    return ValeKradensCottageCues;
}

// Finishes the party's nod together before the next line of dialogue.
static inline void scValeKradensCottageNodTogether(void)
{
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
}

// Turns all three party members toward the strangers.
static inline void scValeKradensCottageFacePartySouth(void)
{
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
}

// Starts a companion walking back to Isaac's current pixel position.
static inline void scValeKradensCottageWalkTowardLeader(Sint32 companionId)
{
    fdActorSetAnimation(companionId, ANIM_WALK);
    scWalkTowardLeader(companionId);
}

// In Vale by Kraden's cottage, the child at the fence asks whether Isaac hears the growls beyond
// it.
static void scValeKradensCottageTalkToBoyByTheFence(void)
{
    Sint32 msg;

    fdScriptStart();
    if (gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0)
    {
        fdScriptSetMessageId(MsgValeIllClimbTheFenceSomeday);
        // "Once I'm as old as you, Isaac, I'm going to climb that..."
        fdScriptShowMessage(ACTOR_BOY_BY_THE_FENCE, 0);
    }
    else
    {
        msg = MsgValeHearAwfulGrowls;
        fdScriptSetMessageId(msg);
        fdActorFaceActor(ACTOR_BOY_BY_THE_FENCE, ACTOR_PARTY_LEADER, 10);
        fdActorRunRepeatedMotion(ACTOR_BOY_BY_THE_FENCE, 2);
        fdScriptWait(6);
        // "Do you hear those awful growls coming from the other side of..."
        fdScriptOpenMessage(ACTOR_BOY_BY_THE_FENCE, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptSetMessageId(msg + 1);
        }
        else
        {
            fdScriptSetMessageId(msg + 2);
        }
        fdActorStartRepeatedMotion(ACTOR_BOY_BY_THE_FENCE, 3);
        fdScriptShowMessage(ACTOR_BOY_BY_THE_FENCE, 0);
        fdActorFaceDirection(ACTOR_BOY_BY_THE_FENCE, FACING_NORTH, 10);
    }
    fdScriptEnd();
}

// In Vale, talking to Kraden outside his cottage has him talk Isaac, Garet and Jenna into a secret
// look at Sol Sanctum.
static void scValeKradensCottageKradenPlansSanctumVisit(void)
{
    ACTOR *leader;
    Sint32 x, z;
    Sint32 sneakingMsg;

    if (gmFlagGet(FLAG_SANCTUM_VISIT_PLANNED) == 0)
    {
        // Isaac walks up to Kraden, who greets him.
        fdScriptStart();
        fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
        fdScriptSetMessageId(MsgValeKradenOhIsaac);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
        fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 232, 264);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_KRADEN, 20);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
        // "Oh! Isaac..."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
        // Garet and Jenna step out of Isaac and stand either side of him.
        leader = fdActorGet(ACTOR_PARTY_LEADER);
        x      = leader->x.part[0].integer;
        z      = leader->z.part[0].integer;
        fdActorSetPos(ACTOR_JENNA, x, z);
        fdActorSetPos(ACTOR_GARET, x, z);
        fdActorSetSpeed(ACTOR_JENNA, 0.5);
        fdActorSetSpeed(ACTOR_GARET, 0.5);
        fdActorWalkTo(ACTOR_JENNA, 248, 264);
        fdActorWalkToAndWait(ACTOR_GARET, 216, 264);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
        fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
        fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
        fdScriptWait(4);
        // Jenna sees something is bothering Kraden; he hedges.
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
        fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
        fdScriptWait(10);
        // "You look like something's bothering you."
        fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
        fdScriptWait(10);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
        // "Me? Ah, well..."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
        // Garet names Saturos and Menardi; Kraden finds them persistent.
        fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 40);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        // "It's those two, isn't it? Saturos and Menardi were their..."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 10);
        fdActorSetAnimation(ACTOR_KRADEN, ANIM_NOD);
        // "Are they still out there? They're very persistent..."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 8);
        // Jenna asks whether they wanted something from him; it seems so.
        fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 20);
        // "Did they want something from you, Kraden?"
        fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
        fdScriptWait(10);
        // "That seems to be the case..."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 6);
        // The three are puzzled, and Jenna asks what he means.
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 20);
        fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
        // "Seems to be?"
        fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
        // Kraden: they spoke of Mt. Aleph and Sol Sanctum as if they had seen them.
        fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
        // "Mt. Aleph and Sol Sanctum... They spoke as if they'd seen..."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 20);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
        // Kraden looks from one to another and asks whether that is not strange.
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
        // "Isn't that strange?"
        fdScriptOpenMessage(ACTOR_KRADEN, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptSetMessageId(MsgValeYouCannotEnterMtAleph);
        }
        else
        {
            fdScriptSetMessageId(MsgValeKradenIWaitedYearsFor);
        }
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
        fdScriptWait(10);
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
        // Jenna fears they are thieves; Garet would tell the high elders.
        fdScriptSetMessageId(MsgValeJennaTheyMightBeThieves);
        fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
        fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 10);
        // "And if they entered secretly, then they might be thieves!"
        fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 6);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
        fdActorJump(ACTOR_GARET, 4, 30);
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 10);
        // "We have to go tell the high elders and the villagers!"
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 6);
        // The three agree and turn to go.
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 10);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 0);
        fdActorFaceActor(ACTOR_KRADEN, ACTOR_GARET, 10);
        fdActorFaceActor(ACTOR_KRADEN, ACTOR_JENNA, 10);
        scValeKradensCottageNodTogether();
        fdScriptWait(10);
        fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
        fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 16);
        // Kraden stops them: nothing can be done until what he was told is confirmed.
        fdActorSetAttachedEffect(ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 3);
        fdScriptWait(10);
        // "Wait, everyone!"
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 6);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
        fdScriptWait(10);
        // "I still don't know how accurate the things they said were..."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 6);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
        // "We can do nothing until we confirm what they told me."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 6);
        // Jenna asks what to do; Kraden means to go and confirm it.
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 10);
        // "Then what should we do?"
        fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 6);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
        fdScriptWait(6);
        // "Well, we go confirm it."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
        // The three start; Garet asks whether he means Sol Sanctum itself, and Jenna thinks it
        // impossible.
        fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
        fdActorJump(ACTOR_GARET, 2, 0);
        fdActorJump(ACTOR_JENNA, 2, 10);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
        // "...Do you mean we should go into Sol Sanctum ourselves?"
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 6);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 10);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
        fdScriptWait(16);
        fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_JENNA, 40);
        // "Isn't that impossible?"
        fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
        // "Why would it be?"
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 6);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 30);
        // Kraden asks whether the ban on climbing Mt. Aleph should stop them; either way, they will
        // only look.
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
        fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
        // "Just because climbing Mt. Aleph is forbidden?"
        fdScriptOpenMessage(ACTOR_KRADEN, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptSetMessageId(MsgValeKradenWeOnlyCheckThe);
        }
        else
        {
            fdScriptSetMessageId(MsgValeKradenWeOnlyCheckMt);
        }
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
        // Garet asks Isaac whether it is fine if they are not seen.
        sneakingMsg = MsgValeFineIfTheyDontSee;
        fdScriptSetMessageId(sneakingMsg);
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        // "We'll be fine if they don't see us, right?"
        fdScriptOpenMessage(ACTOR_GARET, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptSetMessageId(sneakingMsg + 1);
        }
        else
        {
            fdScriptSetMessageId(sneakingMsg + 2);
        }
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 6);
        // Jenna makes it their secret; a yes earns the warning not even to tell Isaac's mother, a
        // no only nods and skips it.
        fdScriptSetMessageId(MsgValeJennaOurSecret);
        fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 10);
        fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
        // "This will just be our secret, OK?"
        fdScriptOpenMessage(ACTOR_JENNA, 0);
        fdScriptWait(4);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
        {
            fdActorJump(ACTOR_JENNA, 2, 20);
            // "No, Isaac! You can't even tell your mother!"
            fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 10);
        }
        else
        {
            fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
            fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
            fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
            fdScriptWait(8);
            fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
            FieldStateWork->msg++;
        }
        // Kraden: then it is decided.
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
        fdScriptWait(10);
        // "Then it's decided!"
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 10);
        scValeKradensCottageNodTogether();
        fdScriptWait(10);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
        fdScriptWait(20);
        // Kraden makes Isaac the leader; Garet and Jenna squabble over who would be nervous.
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
        fdScriptOpenMessage(ACTOR_KRADEN, 0);
        fdScriptWait(4);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
        {
            fdScriptSetMessageId(MsgValeGaretYouCanHandleThe);
        }
        else
        {
            fdScriptSetMessageId(MsgValeGaretIllTakeOverIf);
        }
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 10);
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 6);
        fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 10);
        fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
        fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 6);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 10);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
        // Kraden hops, eager to start.
        fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
        fdActorJump(ACTOR_KRADEN, 4, 40);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 20);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 10);
        // Kraden opens his door and goes in to prepare; the screen closes and the party follows him
        // through VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG.
        sySoundPlay(SE_DOOR_OPEN);
        fdMapGridAnimateMetatiles(ValeKradensCottageDoorOpens, 43, 8);
        fdActorSetSpeed(ACTOR_KRADEN, 1);
        fdActorWalkToAndWait(ACTOR_KRADEN, 232, 218);
        fdActorSetPos(ACTOR_KRADEN, 0, 0);
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
        fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
        FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
        fdScriptFinishScreenTransition();
        fdScriptWaitTransitionTicks();
        syImportScriptSetExitRequest(VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG);
        fdScriptEnd();
    }
}

// In Vale, when the party comes back out of Kraden's cottage, Kraden comes out ready and everyone
// joins Isaac for Mt. Aleph.
static void scValeKradensCottageKradenSetsOff(void)
{
    fdScriptStart();
    fdActorSetPos(ACTOR_KRADEN, 0, 0);
    fdActorSetPos(ACTOR_GARET, 216, 264);
    fdActorSetPos(ACTOR_JENNA, 248, 264);
    fdActorGet(ACTOR_GARET)->facing = FACING_NORTH;
    fdActorGet(ACTOR_JENNA)->facing = FACING_NORTH;
    fdMapGridAnimateMetatiles(ValeKradensCottageDoorOpens, 43, 8);
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(40);
    // Kraden comes out of the door: this is their best bet.
    fdActorSetSpeed(ACTOR_KRADEN, 0.8);
    fdActorSetPos(ACTOR_KRADEN, 230, 220);
    fdActorWalkToAndWait(ACTOR_KRADEN, 230, 232);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
    fdScriptSetMessageId(MsgValeKradenOurBestBet);
    // "This is our best bet."
    fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
    // "All right! Let's go!"
    fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
    // Isaac nods to Garet, then to Jenna.
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 10);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    fdActorSetSpeed(ACTOR_GARET, 0.8);
    fdActorSetSpeed(ACTOR_JENNA, 0.8);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    // Garet, Jenna and Kraden walk into Isaac and vanish; the strangers are gone too.
    scValeKradensCottageWalkTowardLeader(ACTOR_GARET);
    scValeKradensCottageWalkTowardLeader(ACTOR_JENNA);
    scValeKradensCottageWalkTowardLeader(ACTOR_KRADEN);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorSetPos(ACTOR_GARET, 0, 0);
    fdActorSetPos(ACTOR_JENNA, 0, 0);
    fdActorWaitForMoveById(ACTOR_KRADEN);
    fdActorSetPos(ACTOR_KRADEN, 0, 0);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
    fdActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
    fdActorSetPos(ACTOR_SATUROS, 0, 0);
    fdActorSetPos(ACTOR_MENARDI, 0, 0);
    gmFlagSet(FLAG_SANCTUM_VISIT_PLANNED);
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fdPaletteFadeApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    gmFlagSet(FLAG_VALE_KRADENS_DOOR_OPEN);
    fdScriptEnd();
}

// In Vale, walking into Kraden's cottage opens the door, unless Kraden left it open.
static void scValeKradensCottageEnterCottage(void)
{
    if (gmFlagGet(FLAG_VALE_KRADENS_DOOR_OPEN) == 0)
    {
        sySoundPlay(SE_DOOR_OPEN);
        fdMapGridPlayMapEdit(ValeKradensCottageDoorOpens, 43, 8);
    }
    fdActorWalkTo(ACTOR_PARTY_LEADER, 229, 217);
    syImportScriptSetExitRequest(VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_NORTH);
}

// In Vale on the way to Kraden's cottage, Isaac, Garet and Jenna walk past two strangers and
// overhear them.
static void scValeKradensCottageMeetSaturosAndMenardi(void)
{
    ACTOR *actor;
    Sint32 x, z;

    if (fdScriptGetFlag(FLAG_MET_SATUROS_AND_MENARDI) != 0)
    {
        return;
    }

    fdScriptStart();
    sySoundPlay(SOUND_FADE_OUT_BGM);
    gmFlagSet(FLAG_MET_SATUROS_AND_MENARDI);

    // Saturos is overheard: the villagers know little of Sol Sanctum.
    fdScriptSetMessageId(MsgValeTheyKnowLittleOfThe);
    // "They seem to know little of Sol Sanctum's mysteries."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);

    // Isaac hurries forward and Garet and Jenna step out beside him.
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 392, 328);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);

    actor = fdActorGet(ACTOR_PARTY_LEADER);
    x     = actor->x.part[0].integer;
    z     = actor->z.part[0].integer;
    fdActorSetPos(ACTOR_JENNA, x, z);
    fdActorSetPos(ACTOR_GARET, x, z);

    fdActorSetSpeed(ACTOR_JENNA, 1.2);
    fdActorSetSpeed(ACTOR_GARET, 1.2);
    fdActorWalkTo(ACTOR_JENNA, 376, 328);
    fdActorWalkToAndWait(ACTOR_GARET, 408, 328);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STILL);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
    scValeKradensCottageFacePartySouth();
    // The camera pans down to the strangers, and the ominous music plays.
    fdCameraSetSpeed(6);
    fdCameraMoveTo(215, CAMERA_KEEP, 345, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    sySoundPlay(MUS_CHANCE_MEETING_WITH_SWORN_ENEMIES);

    // Saturos and Menardi talk of the elders and of using Kraden, stubborn and cunning as he is.
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    // "The elders of Vale must not permit anyone to enter it, not..."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
    // "Kraden... Can we use him?"
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    // "Hmm... His knowledge is no meager thing. ...We can use him."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH, 60);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    // "But he's more stubborn than we expected..."
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    // "And cunning, too... Cunning beyond measure."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
    // "If he won't go willingly, then we can always..."
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 6);
    // They sense they are watched; Saturos looks about, and both walk round to face the party.
    fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
    fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 60);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 40);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 40);
    fdActorWalkToAndWait(ACTOR_SATUROS, 232, 360);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_EAST, 10);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdScriptWait(10);
    fdActorWalkTo(ACTOR_SATUROS, 392, 360);
    fdActorWalkToAndWait(ACTOR_MENARDI, 216, 360);
    fdActorWalkTo(ACTOR_MENARDI, 376, 360);
    scValeKradensCottageFacePartySouth();
    fdCameraMoveTo(393, CAMERA_KEEP, 339, 1);
    fdActorWalkTo(ACTOR_SATUROS, 392, 360);
    fdActorWalkToAndWait(ACTOR_MENARDI, 376, 360);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_STILL);
    fdActorSetAnimation(ACTOR_MENARDI, ANIM_STILL);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 30);
    // Saturos demands what they are doing; Garet throws it back; Menardi asks whether they were
    // eavesdropping.
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    // "You there! What are you doing?"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    // "What are we doing?"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
    fdScriptOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0);

    // "Were you eavesdropping on our conversation?"
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        // "Oh ho! Well... I can't permit you to continue your spying."
        fdScriptSetMessageId(MsgValeTheyKnowLittleOfThe + EAVESDROP_MENARDI_FORBIDS_SPYING);
    }
    else
    {
        // "Hmph! I guess that I should take you at your word..."
        fdScriptSetMessageId(MsgValeTheyKnowLittleOfThe + EAVESDROP_MENARDI_TAKES_WORD);
    }

    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);

    // Garet accuses them of sneaking around; Menardi guesses Kraden's engagement was with the
    // children, and Jenna says they are off to meet him.
    fdScriptSetMessageId(MsgValeYoureTheOnesSneakingAround);
    // "Hey, you're the ones who were sneaking around!"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 20);
    // "Sneaking... Who are you, to accuse me!"
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 60);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    // "So Kraden's engagement was with you..."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
    fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
    // "Yeah, we're off to meet him right now."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
    // Saturos asks whether their appointment is important enough to drive the strangers away.
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTHWEST, 20);
    // "And this appointment is important enough to drive us away?"
    fdScriptOpenMessage(ACTOR_SATUROS, 0);

    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        // "Pfeh! If your errand is more important than our own, then..."
        fdScriptSetMessageId(MsgValeYoureTheOnesSneakingAround + APPOINTMENT_SATUROS_GIVES_WAY);
    }
    else
    {
        // "Nevertheless, Kraden sent us away. He awaits you."
        fdScriptSetMessageId(MsgValeYoureTheOnesSneakingAround + APPOINTMENT_KRADEN_SENT_THEM_AWAY);
    }

    fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 20);
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    fdActorFaceActor(ACTOR_SATUROS, ACTOR_GARET, 30);
    fdActorFaceActor(ACTOR_SATUROS, ACTOR_JENNA, 30);
    fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    // Saturos lets them go; Jenna is surprised and Menardi doubts him, but he sees no reason to
    // delay the children.
    fdScriptSetMessageId(MsgValeSaturosGo);
    // "Go..."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 6);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorStartRepeatedMotion(ACTOR_GARET, 1);
    fdActorStartRepeatedMotion(ACTOR_JENNA, 1);
    fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    fdScriptWait(40);
    fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 10);
    // "You're letting us go?"
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 10);
    // "Are you sure, Saturos?"
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MENARDI, 0, 10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 20);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    // "Menardi, we have no reason to delay these children any further."
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 20);
    fdActorSetSpeed(ACTOR_MENARDI, 0.5);
    fdActorSetSpeed(ACTOR_SATUROS, 0.5);

    // The pair back away south without turning round.
    actor                 = fdActorGet(ACTOR_SATUROS);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;
    actor                 = fdActorGet(ACTOR_MENARDI);
    actor->steeringFlags &= ~ACTOR_FACES_MOTION;

    fdActorWalkTo(ACTOR_SATUROS, 392, 376);
    fdActorWalkToAndWait(ACTOR_MENARDI, 376, 376);
    fdScriptWait(6);

    actor                 = fdActorGet(ACTOR_SATUROS);
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    actor                 = fdActorGet(ACTOR_MENARDI);
    actor->steeringFlags |= ACTOR_FACES_MOTION;

    fdActorSetAnimation(ACTOR_SATUROS, ANIM_STILL);
    fdActorSetAnimation(ACTOR_MENARDI, ANIM_STILL);
    fdScriptWait(20);
    // Garet: let's go. The village music returns and Garet and Jenna walk back into Isaac; the
    // strangers keep turning to watch him.
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    // "Let's... Let's go, Isaac."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    // Isaac turns to Garet, east of him.
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST + 1, 20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    sySoundPlay(SOUND_FADE_OUT_BGM);

    scValeKradensCottageWalkTowardLeader(ACTOR_GARET);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdActorSetPos(ACTOR_GARET, 0, 0);

    scValeKradensCottageWalkTowardLeader(ACTOR_JENNA);
    fdActorWaitForMoveById(ACTOR_JENNA);
    fdActorSetPos(ACTOR_JENNA, 0, 0);
    syFieldActorWatchAndStartScript(ACTOR_SATUROS,
                                    ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                    ValeKradensCottageFaceTargetActions);
    syFieldActorWatchAndStartScript(ACTOR_MENARDI,
                                    ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                    ValeKradensCottageFaceTargetActions);
    gmPlayMusic();
    fdScriptEnd();
}

// In Vale, walking up to Kraden's cottage finds Kraden pacing.
static void scValeKradensCottageOverhearKraden(void)
{
    ACTOR *leader;

    if (fdScriptGetFlag(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS) == 0)
    {
        fdScriptStart();
        fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
        // Kraden wonders aloud who they were, while Garet and Jenna step out of Isaac behind him.
        fdScriptSetMessageId(MsgValeWho);
        // "Just who were they?"
        fdScriptShowMessage(ACTOR_KRADEN, 0);
        leader = fdActorGet(ACTOR_PARTY_LEADER);
        {
            Sint32 x = leader->x.part[0].integer;
            Sint32 z = leader->z.part[0].integer;

            fdActorSetPos(ACTOR_JENNA, x, z);
            fdActorSetPos(ACTOR_GARET, x, z);
        }
        fdActorSetSpeed(ACTOR_JENNA, 0.8);
        fdActorSetSpeed(ACTOR_GARET, 0.8);
        fdActorWalkTo(ACTOR_JENNA, 296, 328);
        fdActorWalkToAndWait(ACTOR_GARET, 280, 328);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
        fdActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
        fdActorSetAnimation(ACTOR_GARET, ANIM_STILL);
        fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
        // The camera goes up to the door. Pacing, Kraden says they know more of Sol Sanctum than
        // Vale's elders.
        fdCameraMoveTo(232, CAMERA_KEEP, 240, 1);
        fdCameraWaitForMove();
        fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
        // "They already know too much about Sol Sanctum..."
        fdScriptShowMessage(ACTOR_KRADEN, 0);
        fdActorSetSpeed(ACTOR_KRADEN, 0.2);
        fdActorWalkToAndWait(ACTOR_KRADEN, 216, 232);
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
        fdScriptWait(10);
        // "...things that even the elders of Vale don't know."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 40);
        fdActorJump(ACTOR_KRADEN, 2, 10);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 10);
        // "And what were they saying about the elements?"
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 10);
        // He turns over what they said of the elements and the Elemental Stars, things even he does
        // not know.
        fdActorWalkToAndWait(ACTOR_KRADEN, 248, 232);
        fdScriptWait(40);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
        fdScriptWait(20);
        // "The elements of Alchemy: earth, water, fire, and wind..."
        fdScriptShowMessage(ACTOR_KRADEN, 0);
        fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 1);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
        // "And the four Elemental Stars are the key?"
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 2);
        fdActorWalkToAndWait(ACTOR_KRADEN, 232, 232);
        fdScriptWait(2);
        fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 4);
        fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
        fdScriptWait(4);
        // "These are things even I, a trained Alchemy sage, do not know."
        fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 2);
        fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
        fdScriptWait(20);
        // The camera returns to Isaac, and Garet and Jenna walk back into him.
        fdCameraFollowActor(ACTOR_PARTY_LEADER, 1);
        fdCameraWaitForMove();
        scWalkIntoLeader(ACTOR_GARET);
        scWalkIntoLeader(ACTOR_JENNA);
        gmFlagSet(FLAG_KRADEN_WONDERED_ABOUT_STRANGERS);
        fdScriptEnd();
    }
}

// Arriving in Kraden's part of Vale places the actors for the storm or for the day, and plays the
// arrival from the cottage.
Sint32 scInit(void)
{
    if (MainRoutineStateWork.entrance == VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_UNUSED
        || MainRoutineStateWork.entrance == VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST)
    {
        gmFlagClear(FLAG_SHOW_AREA_NAME);
    }
    if (fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) != 0)
    {
        gmFlagClear(FLAG_VALE_KRADENS_DOOR_OPEN);
    }
    // The rockslide chapter replaces residents with debris and a survivor.
    if (gmFlagGet(FLAG_VALE_STORM) != 0)
    {
        fdWeatherStartRain();
        fdWeatherStartLightning();
        fdScriptStart();
        fdActorSetPos(ACTOR_BOY_BY_THE_FENCE, 0, 0);
        fdActorSetPos(ACTOR_KRADEN, 0, 0);
        fdActorSetPos(ACTOR_SATUROS, 0, 0);
        fdActorSetPos(ACTOR_MENARDI, 0, 0);
        fdActorSetPos(ACTOR_JENNA, 0, 0);
        fdActorGet(ACTOR_FENCE_BOULDER)->collisionFlags |= ACTOR_NOT_TARGETED;
        fdActorSetPos(ACTOR_ROCKSLIDE_SURVIVOR, 83, 265);
        fdActorWalkToAndWait(ACTOR_ROCKSLIDE_SURVIVOR, 83, 273);
        fdActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_LIE);
        fdActorGet(ACTOR_ROCKSLIDE_SURVIVOR)->radius = 12;
        fdActorSetScript(ACTOR_ROCKSLIDE_SURVIVOR, ValeKradensCottageRockslideSurvivorSquashes);
        if (gmFlagGet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) != 0)
        {
            fdActorSetPos(ACTOR_ROCKSLIDE_SURVIVOR, 0, 0);
        }
        fdScriptEnd();
        fdMapGridCopyMetatileAttributes(9, 24, 1, 1, 14, 21);
        fdMapGridCopyMetatileAttributes(9, 24, 1, 1, 15, 21);
        fdMapGridCopyMetatileAttributes(9, 24, 1, 1, 23, 19);
        fdMapGridCopyMetatileAttributes(9, 24, 1, 1, 24, 19);
        fdMapGridCopyMetatileAttributes(9, 24, 1, 1, 23, 20);
        fdMapGridCopyMetatileAttributes(9, 24, 1, 1, 24, 20);
    }
    else
    {
        fdActorSetPos(ACTOR_WEST_ROCK, 0, 0);
        fdActorSetPos(ACTOR_EAST_ROCK, 0, 0);
        fdActorSetPos(ACTOR_GARET, 0, 0);
        fdActorSetPos(ACTOR_ROCKSLIDE_SURVIVOR, 0, 0);
        fdScriptStart();
        if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
        {
            if (MainRoutineStateWork.entrance == VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH)
            {
                scValeKradensCottageKradenSetsOff();
            }
        }
        if (gmFlagGet(FLAG_SANCTUM_VISIT_PLANNED) != 0)
        {
            fdActorSetPos(ACTOR_KRADEN, 0, 0);
            fdActorSetPos(ACTOR_SATUROS, 0, 0);
            fdActorSetPos(ACTOR_MENARDI, 0, 0);
        }
        else
        {
            if (gmFlagGet(FLAG_MET_SATUROS_AND_MENARDI) != 0)
            {
                fdActorSetPos(ACTOR_SATUROS, 392, 376);
                fdActorSetPos(ACTOR_MENARDI, 376, 376);
                syFieldActorWatchAndStartScript(ACTOR_SATUROS,
                                                ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                                ValeKradensCottageFaceTargetActions);
                syFieldActorWatchAndStartScript(ACTOR_MENARDI,
                                                ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                                ValeKradensCottageFaceTargetActions);
            }
        }
        if (gmFlagGet(FLAG_KALAY_REACHED) != 0)
        {
            fdActorSetPos(ACTOR_COTTAGE_VISITOR, 132, 264);
        }
        fdScriptEnd();
    }
    return 0;
}

// In Vale on the stormy night, the man hurt in the rockslide asks whether he will die.
static void scValeKradensCottageTalkToRockslideSurvivor(void)
{
    ACTOR *leader;
    Sint32 msg;

    if (fdScriptGetFlag(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED) == 0)
    {
        if (fdScriptGetFlag(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP) != 0)
        {
            fdScriptStart();
            fdActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
            fdScriptSetMessageId(MsgValeSaveYourselves);
            // "This is it for me. Save yourselves..."
            fdScriptShowMessage(ACTOR_ROCKSLIDE_SURVIVOR, 0);
            fdScriptEnd();
        }
        else
        {
            fdScriptStart();
            fdActorStop(ACTOR_ROCKSLIDE_SURVIVOR);
            fdActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 1);
            // He gasps out the rockslide and the monsters; Isaac and Garet hurry over to him.
            msg = MsgValeRockslideDestroyedFence;
            fdScriptSetMessageId(msg);
            // "Rockslide...destroyed fence... Monsters everywhere..."
            fdScriptShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 20);
            fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
            fdCameraMoveTo(98, CAMERA_KEEP, 283, 1);
            fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 94, 293);
            fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
            leader = fdActorGet(ACTOR_PARTY_LEADER);
            if (leader != NULL)
            {
                fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
            }
            fdActorWalkToAndWait(ACTOR_GARET, 110, 279);
            fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 40);
            fdActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
            fdScriptWait(40);
            // He asks whether he will die. Yes: he gives up and tells them to save themselves. "I'm
            // hurt pretty badly... Do you think I'll die?"
            fdScriptOpenMessage(ACTOR_ROCKSLIDE_SURVIVOR, 0);
            if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
            {
                fdActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
                fdScriptWait(20);
                fdScriptSetMessageId(msg + 2);
                // "I knew it... This is it for me. Save yourselves..."
                fdScriptShowMessage(ACTOR_ROCKSLIDE_SURVIVOR, 0);
                gmFlagSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP);
            }
            else
            {
                // No: he laments, then jumps up, finds he is not hurt at all, and warns them of the
                // monsters on the way to the plaza.
                fdActorRunRepeatedMotion(ACTOR_ROCKSLIDE_SURVIVOR, 2);
                fdScriptWait(20);
                fdScriptSetMessageId(msg + 3);
                // "This is awful. I'm left here for the monsters to..."
                fdScriptShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 40);
                fdActorFaceActor(ACTOR_ROCKSLIDE_SURVIVOR, ACTOR_PARTY_LEADER, 0);
                fdActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_STAND);
                fdActorJump(ACTOR_ROCKSLIDE_SURVIVOR, 4, 40);
                fdActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR,
                                    YOUNG_MAN_WITH_BROWN_HAIR_ANIMATION_LOOK_HIMSELF_OVER);
                fdActorShowEmote(ACTOR_ROCKSLIDE_SURVIVOR, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
                // "Wait... That's strange... I'm not hurt at all!"
                fdScriptShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 10);
                fdActorSetAnimation(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_STAND);
                fdScriptWait(10);
                fdActorSetAnimationAndWait(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_NOD);
                // "But I'm serious about the monsters, so be careful on your..."
                fdScriptShowMessageThenWait(ACTOR_ROCKSLIDE_SURVIVOR, 0, 10);
                fdActorSetAnimationAndWait(ACTOR_ROCKSLIDE_SURVIVOR, ANIM_NOD);
                syFieldActorWatchAndStartScript(ACTOR_PARTY_LEADER,
                                                ACTOR_WATCH_KEEP_MOTION | ACTOR_ROCKSLIDE_SURVIVOR,
                                                ValeKradensCottageFaceTargetActions);
                syFieldActorWatchAndStartScript(ACTOR_GARET,
                                                ACTOR_WATCH_KEEP_MOTION | ACTOR_ROCKSLIDE_SURVIVOR,
                                                ValeKradensCottageFaceTargetActions);
                fdActorRunScriptAndWait(ACTOR_ROCKSLIDE_SURVIVOR,
                                        ValeKradensCottageRockslideSurvivorEscapeActions);
                fdActorStop(ACTOR_PARTY_LEADER);
                fdActorStop(ACTOR_GARET);
                fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
                fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 60);
                fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
                fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
                gmFlagSet(FLAG_VALE_ROCKSLIDE_SURVIVOR_ESCAPED);
            }
            // Garet walks back into Isaac.
            scWalkIntoLeader(ACTOR_GARET);
            fdScriptEnd();
        }
    }
}

// In Vale on the stormy night, a monster attacks by the broken fence.
static void scValeKradensCottageFightFirstMonster(void)
{
    FIELD_STATE *state;

    fdScriptStart();

    state                  = FieldStateWork;
    state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    state->transitionFrame = 64;

    gmFlagSet(FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT);
    fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 0);
    gmFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
    fdScriptEnd();
}

// In Vale on the stormy night, a second monster attacks by the broken fence.
static void scValeKradensCottageFightSecondMonster(void)
{
    FIELD_STATE *state;

    fdScriptStart();

    state                  = FieldStateWork;
    state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    state->transitionFrame = 64;

    gmFlagSet(FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT);
    fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_SCRIPTED_VALE_VAULT, 1);
    gmFlagSet(FLAG_VALE_STORM_MONSTER_FOUGHT);
    fdScriptEnd();
}

// A villager at Kraden's cottage asks Isaac about his memories of it. This is in Vale after the
// party has reached Kalay.
static void scValeKradensCottageTalkToCottageVisitor(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgValeMemoriesOfThisCottage);
    // "You must have so many memories of this cottage, don't you?" Yes: "I thought so. You used to
    // visit Kraden a lot, Isaac." No: "I understand. You don't have time to dwell on old
    // memories..."
    fdScriptAskYesNo(ACTOR_COTTAGE_VISITOR, 0);
    fdScriptEnd();
}

// Leads the party away from the broken fence, then leaves the scene.
Sint32 ValeKradensCottageRockslideSurvivorEscapeActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(72, 0, 294),
    SC_SYNC,
    SC_MOVE(77, 0, 329),
    SC_SYNC,
    SC_MOVE(70, 0, 415),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_END,
};

// The survivor squashes flat and recovers every 68 ticks as he lies by the fence.
Sint32 ValeKradensCottageRockslideSurvivorSquashes[] = {
    SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 32),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 64),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 128),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 64),
    SC_ADD(SW_SCALE_X, -toFIXED(1) / 128),
    SC_WAIT(2),
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_WAIT(60),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// Keeps a watched actor facing its current target.
Sint32 ValeKradensCottageFaceTargetActions[] = {
    SC_CALL(scTurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE ValeKradensCottageEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 228, 0, 172, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_NORTH,
     CONDITION_ALWAYS, 454,
     0, 290,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_SOUTH,
     CONDITION_ALWAYS, 454,
     0, 443,
     FACING_WEST, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_NORTH,
     CONDITION_ALWAYS, 229,
     0, 233,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_WEST,
     CONDITION_ALWAYS, 72,
     0, 80,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VALE_KRADENS_COTTAGE_FROM_VALE_CAVE_MOUTH_EAST,
     CONDITION_ALWAYS, 104,
     0, 104,
     FACING_SOUTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH,
     CONDITION_ALWAYS, 232,
     0, 264,
     FACING_NORTH, 0,
     -1,
     -1,
     -1,
     -1,
     0},
    {SCENE_TABLE_END},
};

Uint32 ValeKradensCottageExits[] = {
    SCENE_EXITS(ValeKradensCottage),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_NORTH, Vale, VALE_FROM_VALE_KRADENS_COTTAGE_NORTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_SOUTH, Vale, VALE_FROM_VALE_KRADENS_COTTAGE_SOUTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_NORTH,
               CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_NORTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST,
               ValeCaveMouth,
               VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_WEST),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST,
               ValeCaveMouth,
               VALE_CAVE_MOUTH_FROM_VALE_KRADENS_COTTAGE_EAST),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_PROLOGUE_NORTH,
               ValePrologue,
               VALE_PROLOGUE_FROM_VALE_KRADENS_COTTAGE_NORTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_VALE_PROLOGUE_SOUTH,
               ValePrologue,
               VALE_PROLOGUE_FROM_VALE_KRADENS_COTTAGE_SOUTH),
    SCENE_EXIT(VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG,
               CottageRoomsB,
               COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG),
    SCENE_EXITS_END,
};

// clang-format off
ACTOR_PLACE ValeKradensCottageActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        72, 0, 256, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        240, 0, 353, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        384, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, (Sint32)ValeKradensCottageRockslideSurvivorSquashes,
        83, 0, 273, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        92, 0, 268, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 232, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 1608, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 344, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 344, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

CUE ValeKradensCottageCues[] = {
    { CUE_TALK, ACTOR_KRADEN, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageKradenPlansSanctumVisit) },
    { CUE_TALK, ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageTalkToBoyByTheFence) },
    { CUE_TALK, ACTOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgValeDoNotKeepKradenWaiting) },
    { CUE_TALK, ACTOR_MENARDI, CONDITION_ALWAYS, MSGNO(MsgValeLeaveBeforeWeChangeMinds) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeCuriousBeyondFence) },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_NORTH_WAY_TO_VALE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_NORTH },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_SOUTH_WAY_TO_VALE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_SOUTH },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_KRADENS_COTTAGE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageEnterCottage) },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_WEST_PATH_TO_VALE_CAVE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_EAST_PATH_TO_VALE_CAVE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST },
    { CUE_STEP, METATILE_BEHAVIOR_KRADENS_COTTAGE_BEFORE_THE_COTTAGE, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageOverhearKraden) },
    { CUE_STEP, METATILE_BEHAVIOR_KRADENS_COTTAGE_PAST_THE_STRANGERS, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageMeetSaturosAndMenardi) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
    { EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_KRADENS_COTTAGE_JAR, FLAG_VALE_KRADENS_COTTAGE_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { CUE_TABLE_END },
};

CUE ValeKradensCottageStormCues[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_NORTH_WAY_TO_VALE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_PROLOGUE_NORTH },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_SOUTH_WAY_TO_VALE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_PROLOGUE_SOUTH },
    { CUE_TALK, ACTOR_WEST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { CUE_TALK, ACTOR_EAST_ROCK, CONDITION_ALWAYS, MSGNO(MsgValeRockCannotBeMoved) },
    { CUE_TALK, ACTOR_ROCKSLIDE_SURVIVOR, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageTalkToRockslideSurvivor) },
    { CUE_STEP, METATILE_BEHAVIOR_KRADENS_COTTAGE_BESIDE_THE_SURVIVOR, FLAG_VALE_ROCKSLIDE_SURVIVOR_GAVE_UP, SCRIPT_REF(scValeKradensCottageTalkToRockslideSurvivor) },
    { CUE_STEP, METATILE_BEHAVIOR_KRADENS_COTTAGE_FIRST_MONSTER, FLAG_VALE_STORM_FIRST_MONSTER_FOUGHT, SCRIPT_REF(scValeKradensCottageFightFirstMonster) },
    { CUE_STEP, METATILE_BEHAVIOR_KRADENS_COTTAGE_SECOND_MONSTER, FLAG_VALE_STORM_SECOND_MONSTER_FOUGHT, SCRIPT_REF(scValeKradensCottageFightSecondMonster) },
    { EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_KRADENS_COTTAGE_JAR, CONDITION_ALWAYS, 0 },
    { CUE_TABLE_END },
};

CUE ValeKradensCottageCuesAfterKalay[] = {
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_NORTH_WAY_TO_VALE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_NORTH },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_SOUTH_WAY_TO_VALE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_SOUTH },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_KRADENS_COTTAGE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageEnterCottage) },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_WEST_PATH_TO_VALE_CAVE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_WEST },
    { CUE_EXIT, METATILE_BEHAVIOR_KRADENS_COTTAGE_EAST_PATH_TO_VALE_CAVE, CONDITION_ALWAYS, VALE_KRADENS_COTTAGE_TO_VALE_CAVE_MOUTH_EAST },
    { CUE_TALK, ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeWonderBeyondFence) },
    { CUE_TALK, ACTOR_COTTAGE_VISITOR, CONDITION_ALWAYS, SCRIPT_REF(scValeKradensCottageTalkToCottageVisitor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_BY_THE_FENCE, CONDITION_ALWAYS, MSGNO(MsgValeAdventuresBeyondFence) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COTTAGE_VISITOR, CONDITION_ALWAYS, MSGNO(MsgValeLonelyWithoutKraden) },
    { PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_FENCE_BOULDER, CONDITION_ALWAYS, 0 },
#if KRADENS_JAR_ELIXIR_AFTER_KALAY
    { EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_KRADENS_COTTAGE_JAR, FLAG_VALE_KRADENS_COTTAGE_JAR_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
#endif
    { CUE_TABLE_END },
};
// clang-format on

// The cottage's arched front door opening. It is two pictures from the layout's metatiles below the
// village, drawn over the closed door on the middle layer.
Uint16 ValeKradensCottageDoorOpens[] = {
    MAPGRID_ANIMATION_STEP(2, 36, 2, 2, 1),
    MAPGRID_ANIMATION_STEP(4, 36, 2, 2, 1),
    MAPGRID_ANIMATION_STEPS_END,
};

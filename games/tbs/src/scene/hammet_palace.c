// Hammet Palace (SceneIdHammetPalace), the rooms of Lord Hammet's palace in Kalay.
//
// The scene before its door, also Hammet Palace in the game's area names, is kalay.c's
// HammetPalaceGrounds.
//
// Two scripts in the room where Lady Layana stands tick the party's part in Hammet's rescue from
// Lunpa. Between them three flags pick where she stands and what she says:
//
// - Ivan's report (HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS),
//   when the guards at the door bring the party in (kalay.c). It ends by
//   setting FLAG_IVAN_REPORTED_TO_LAYANA.
//
// - Between the two, until the party has heard in Lunpa that Hammet is held
//   there (area temp flag FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA, set by
//   kalay.c's scripts), Layana talks of Dodonpa's spies.
//
//   Afterwards she stands elsewhere in her room, and the first time they
//   speak she scolds "Isaac, you sneaked into Lunpa? That's far too
//   dangerous!" (FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS). From then on
//   she talks of Dodonpa's greed.
//
// - Hammet's homecoming (HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM, the
//   first time once Hammet is free, FLAG_DODONPA_LOCKED_UP;
//   FLAG_HAMMET_BACK_HOME_SEEN), in which Hammet sends the party on to
//   Hesperia.
//
//   Afterwards Hammet stands beside Layana. The door to the underground
//   passage, opened for the party in thanks, is drawn open on entering by
//   HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING or
//   HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM with
// FLAG_KALAY_TUNNEL_OPEN set.
//
// The people of the palace (ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK to ACTOR_ASKING_ABOUT_THE_SILK_TRADE)
// have one line before Hammet is free and another after.
//
// Walking north into the doors of the entrance room and of the landing opens them. The other exits
// join the rooms plainly.
//
// HAMMET_PALACE_TO_HAMMET_PALACE_GROUNDS leads out to the front of the palace
// (HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE), and HAMMET_PALACE_TO_KALAY_TUNNEL_TREASURE_ROOM down
// into the Kalay Tunnel (KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE).
//
// Searching finds a turban, the symbol of Kalay's soldiers, and two books, "Business and You" and
// "The Good Host". A chest holds a Water Jacket and a barrel a Nut.

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "scene/kalay.h"
#include "text/msg_ids.h"

extern Sint32         HammetPalacePartyActions[];
extern SCENE_ENTRANCE HammetPalaceEntrancesOther[];
extern SCENE_ENTRANCE HammetPalaceEntrances[];
extern Uint32         HammetPalaceExits[];
extern ACTOR_PLACE    HammetPalaceActorPlacementsOther[];
extern ACTOR_PLACE    HammetPalaceActorPlacements[];
extern CUE            HammetPalaceCuesOther[];
extern CUE            HammetPalaceCues[];

// The tileset animations that swing the palace's doors open as the leader walks through one
// (scHammetPalaceLeaveThroughDoor runs both, whichever door it is): the doors two metatiles across,
// as Layana's room's, and the arched doors one metatile across, as the west wing's.
typedef enum HAMMET_PALACE_DOOR_ANIMATION
{
    HAMMET_PALACE_WIDE_DOORS_ANIMATION   = 1,
    HAMMET_PALACE_NARROW_DOORS_ANIMATION = 2
} HAMMET_PALACE_DOOR_ANIMATION;

// The palace's actors, numbered in the order of HammetPalaceActorPlacements from
// ACTOR_FIRST_PLACED.
//
// Its rows for Garet, Ivan and Mia keep the party's own slots.
typedef enum HAMMET_PALACE_ACTOR_ID
{
    ACTOR_LAYANA = ACTOR_FIRST_PLACED,
    // Beside Layana, and kept out of sight until he is free.
    ACTOR_HAMMET,
    // The soldiers of Kalay in Ivan's report (SPRITE_SOLDIER_IN_TURBAN, as the guards at the palace
    // door in kalay.c), out of sight at other times.
    ACTOR_FIRST_SOLDIER,
    ACTOR_SECOND_SOLDIER,
    // The people of the palace, each with one line before Hammet is free and another after. A young
    // man with brown hair in a purple shirt who worries that the townsfolk will learn Hammet is a
    // hostage.
    //
    // "The townsfolk don't know about Lord Hammet yet, do they?"
    //
    // After: "I hear Hammet was saved by you people..."
    ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK,
    // A woman with fair hair who asks whether Hammet will be freed.
    //
    // "Lord Hammet will be released soon. Right?"
    //
    // After: "They say Hammet's return is supposed to be a secret for now."
    ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED,
    // A grey-haired man in a black suit who tells of Layana's grief, and later of the door Layana
    // had opened for the party.
    //
    // "Layana is putting up a strong front whenever people are around."
    //
    // After: "I left the door to the underground passage open, so go have a look."
    ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN,
    // A bald man in red who fears Dodonpa's men.
    //
    // "Merchants are supposed to be protected under the laws of Angara."
    //
    // After: "I wonder if any of Dodonpa's thugs have come here after Hammet..."
    ACTOR_AFRAID_OF_DODONPAS_THUGS,
    // A young man with brown hair in green who asks whether Hammet's silk trade has ended.
    //
    // "Has the legacy of Lord Hammet's silk trading come to an end?"
    //
    // After: "The secret passage is locked because Hammet's treasure is in there."
    ACTOR_ASKING_ABOUT_THE_SILK_TRADE
} HAMMET_PALACE_ACTOR_ID;

static void        scHammetPalaceSetUpEntrance(void);
static void        scHammetPalaceIvanReportsToLayana(void);
static void        scHammetPalaceFacePartyOutward(Sint32 waitTicks);
static void        scHammetPalaceFacePartyNorth(Sint32 waitTicks);
static void        scHammetPalaceHammetsHomecoming(void);

// Skips messages of the conversation, past an answer not taken.
static inline void scHammetPalaceAdvanceMessage(Sint32 amount)
{
    FieldStateWork->msg += amount;
}

// Selects Hammet Palace's entrances, or the fallback table for any other scene.
const SCENE_ENTRANCE *scGetEntrances(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_HAMMET_PALACE)
    {
        return HammetPalaceEntrances;
    }
    return HammetPalaceEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return HammetPalaceExits;
}

// Hammet Palace's actors, the people only of the room the party enters (CONDITION_IN_VIEW).
const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_HAMMET_PALACE)
    {
        fdAssignViewFlags(HammetPalaceActorPlacements);
        return HammetPalaceActorPlacements;
    }
    return HammetPalaceActorPlacementsOther;
}

// Hammet Palace's cues, or none for any other scene.
const CUE *scGetCues(void)
{
    Sint16 sceneId = MainRoutineStateWork.next;

    if (sceneId == SCENE_HAMMET_PALACE)
    {
        return HammetPalaceCues;
    }
    return HammetPalaceCuesOther;
}

// Talking to one of the people of Hammet Palace before Hammet is free: he hopes for his release.
static void scHammetPalaceAskWhetherHammetWillBeReleased(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgKalayLordHammetWillReleasedSoon);
    // "Lord Hammet will be released soon. Right?" Yes: "I hope so. Otherwise, Layana will be
    // overwhelmed by sorrow." No: "It wouldn't hurt you to lie a little, you know!"
    fdScriptAskYesNo(ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, 0);
    fdScriptEnd();
}

// Talking to one of the people of Hammet Palace before Hammet is free: he worries for Hammet's silk
// trade.
static void scHammetPalaceAskWhetherSilkTradeHasEnded(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgKalayHasLegacyLordHammetsSilk);
    // "Has the legacy of Lord Hammet's silk trading come to an end?" Yes: "How can you say that!
    // Lady Layana would be devastated!" No: "You're right. I never should have said that. I feel
    // terrible..."
    fdScriptAskYesNo(ACTOR_ASKING_ABOUT_THE_SILK_TRADE, 0);
    fdScriptEnd();
}

// Layana scolds Isaac for sneaking into Lunpa.
static void scHammetPalaceLayanaScoldsLunpaVisit(void)
{
    fdScriptStart();
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    fdScriptSetMessageId(MsgKalayIsaacSneakedIntoLunpaThats);
    // "Isaac, you sneaked into Lunpa? That's far too dangerous!"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 2);
    // "But I am happy to learn that Hammet is not hurt."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "Thank you, Isaac... But please, do not go to Lunpa again."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "Please, fulfill your quest."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    gmFlagSet(FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS);
    fdScriptEnd();
}

// Walking north into a door of the entrance room or the landing: it opens, and the leader walks
// through it to the next room.
//
// Each step cue's metatile is the number of its exit. The door to Layana's room thumps open, and
// the leader walks straight on rather than first lining up with the middle of his column.
static void scHammetPalaceLeaveThroughDoor(void)
{
    FIELD_STATE *state;

    state = FieldStateWork;
    fdScriptStart();
    fdScriptWait(10);
    if (state->stepCue == HAMMET_PALACE_ENTRANCE_ROOM_TO_LAYANAS_ROOM)
    {
        sySoundPlay(SE_THUMP);
    }
    else
    {
        sySoundPlay(SE_DOOR_OPEN);
    }
    fdTilesetAnimationResume(HAMMET_PALACE_WIDE_DOORS_ANIMATION);
    fdTilesetAnimationResume(HAMMET_PALACE_NARROW_DOORS_ANIMATION);
    fdScriptWait(10);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    if (state->stepCue == HAMMET_PALACE_ENTRANCE_ROOM_TO_LAYANAS_ROOM)
    {
        fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    }
    else
    {
        fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    fdScriptWait(16);
    syImportScriptSetExitRequest(state->stepCue);
    fdTilesetAnimationPause(HAMMET_PALACE_WIDE_DOORS_ANIMATION);
    fdTilesetAnimationPause(HAMMET_PALACE_NARROW_DOORS_ANIMATION);
    fdScriptEnd();
}

// Sets up Hammet Palace as the party enters.
Sint32 scInit(void)
{
    FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    if (MainRoutineStateWork.next == SCENE_HAMMET_PALACE)
    {
        scHammetPalaceSetUpEntrance();
    }
    return 0;
}

// Sets up the room the party enters by: every door drawn shut, Layana and Hammet where the story
// has them, and the door to the underground passage open once Layana has had it opened.
static void scHammetPalaceSetUpEntrance(void)
{
    ACTOR *actor;
    Sint32 facing;

    fdTilesetAnimationPause(HAMMET_PALACE_WIDE_DOORS_ANIMATION);
    fdTilesetAnimationPause(HAMMET_PALACE_NARROW_DOORS_ANIMATION);

    switch (MainRoutineStateWork.entrance)
    {
        case HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM:
            if (gmFlagGet(FLAG_DODONPA_LOCKED_UP) != 0)
            {
                // Hammet is home.
                actor         = fdActorGet(ACTOR_LAYANA);
                facing        = FACING_EAST_SOUTHEAST;
                actor->facing = facing;

                if (gmFlagGet(FLAG_HAMMET_BACK_HOME_SEEN) == 0)
                {
                    scHammetPalaceHammetsHomecoming();
                }
            }
            else
            {
                fdActorSetPos(ACTOR_HAMMET, 0, 0);
                if (gmFlagGet(FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA) != 0)
                {
                    fdActorSetPos(ACTOR_LAYANA, 906, 422);
                    actor         = fdActorGet(ACTOR_LAYANA);
                    facing        = FACING_NORTH_NORTHEAST;
                    actor->facing = facing;
                }
            }
            break;

        case HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING:
        case HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM:
            if (gmFlagGet(FLAG_KALAY_TUNNEL_OPEN) != 0)
            {
                // The door to the underground passage stands open.
                fdMapGridCopyMetatilesTo(58, 70, 54, 70, 4, 3);
                fdMapGridCopyMetatileAttributes(55, 9, 2, 1, 55, 8);
                fdMapGridCenterOnFollowedPoint();
                sySynch(1);
            }
            break;

        case HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS:
            fdActorSetPos(ACTOR_HAMMET, 0, 0);
            if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0)
            {
                scHammetPalaceIvanReportsToLayana();
            }
            break;

        default: break;
    }
}

// Ivan's report in Hammet Palace, when the guards at the door bring the party to Layana: she had
// hoped Hammet came back with him.
static void scHammetPalaceIvanReportsToLayana(void)
{
    FIELD_STATE  *state;
    Sint32        advanceAfterReply;
    const Sint32 *partyScript;

    fdScriptStart();
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    sySynch(1);
    fdPaletteFadeApplySource(PALETTE_TINT_SEPIA, PALETTES_ALL);
    // The report opens in sepia, with the party placed under the closed screen; the colours come
    // back after the silent exchange of nods that opens it.
    fdPaletteFadeApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(1);
    sySynch(1);
    state                  = FieldStateWork;
    state->transitionFrame = 24;
    state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
    fdActorSetPos(ACTOR_LAYANA, 856, 440);
    fdActorSetPos(ACTOR_PARTY_LEADER, 856, 486);
    fdActorSetPos(ACTOR_GARET, 848, 502);
    fdActorSetPos(ACTOR_IVAN, 872, 486);
    fdActorSetPos(ACTOR_MIA, 880, 502);
    fdActorSetPos(ACTOR_FIRST_SOLDIER, 840, 518);
    fdActorSetPos(ACTOR_SECOND_SOLDIER, 888, 518);
    fdCameraMoveTo(864, CAMERA_KEEP, 472, 0);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(40);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorStartRepeatedMotion(ACTOR_GARET, 1);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    scHammetPalaceFacePartyNorth(20);
    fdActorSetAttachedEffect(ACTOR_LAYANA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorStartRepeatedMotion(ACTOR_LAYANA, 2);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHWEST, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorStartRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST_NORTHEAST, 20);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST_NORTHWEST, 40);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH_NORTHWEST, 20);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 2);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(40);
    sySynch(60);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdScriptSetMessageId(MsgKalayWhenHeardWereBackIvan);
    // "When I heard that you were back, Ivan, I had dared to..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    // "We thought Lord Hammet had been released from Lunpa, too."
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_SHAKE_HEAD);
    // "We sent the ransom money they demanded, so he should return..."
    fdScriptShowMessageThenWait(ACTOR_SECOND_SOLDIER, 0, 10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
    // "What's been happening in Lunpa these days?"
    syFieldScriptShowMessageThenWait(
        SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
    // "The leadership in Lunpa passed from its founder, Lunpa, the..."
    fdScriptShowMessageThenWait(ACTOR_SECOND_SOLDIER, 0, 10);
    fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
    // "Dodonpa is a very evil man. His father, Donpa, is..."
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
    // "When did you send the ransom money?"
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Let's see... First, the volcano erupted..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
    // "And the ransom note came right after the eruption."
    fdScriptShowMessageThenWait(ACTOR_SECOND_SOLDIER, 0, 10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 10);
    fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    // "We rushed the money to them right away. I remember it well."
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
    // "It's been quite a while since the eruption..."
    syFieldScriptShowMessageThenWait(
        SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 10);
    // "Is it possible they have no intention of letting Lord Hammet go?" Yes: "You think so, too,
    // Isaac?" No: "I don't want to believe it either, but something seems wrong..."
    fdScriptAskYesNo(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0);
    fdActorSetAttachedEffect(ACTOR_LAYANA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    // "Are you saying Dodonpa won't release Hammet!?"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    // "I agree with Ivan. If they were going to release him, they'd..."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "I am sorry, Lady Layana, but I think Ivan might be right."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 40);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_EAST_SOUTHEAST, 40);
    fdActorSetSpeed(ACTOR_LAYANA, 0.4);
    fdActorWalkToAndWait(ACTOR_LAYANA, 892, 440);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_NORTH_NORTHEAST, 20);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    scHammetPalaceFacePartyOutward(60);
    scHammetPalaceFacePartyNorth(40);
    fdActorWalkToAndWait(ACTOR_LAYANA, 856, 440);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_WEST_NORTHWEST, 20);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST_NORTHWEST, 40);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH_NORTHWEST, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    // "What should we do? How can we rescue Hammet?"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "What... What if something has happened to him!?"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    // "Let us handle this!"
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 2);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHWEST, 10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST_NORTHWEST, 40);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_EAST_NORTHEAST, 20);
    // "My lady, this is exactly what the soldiers of Kalay are here for!"
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH_NORTHWEST, 20);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    // Yes, Lady Layana! Please let us handle this!
    fdScriptShowMessageThenWait(ACTOR_SECOND_SOLDIER, 0, 10);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_WEST_NORTHWEST, 40);
    fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_NORTH_NORTHWEST, 10);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    // "Command us to attack, and we shall!"
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 20);
    scHammetPalaceFacePartyNorth(20);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 2);
    fdScriptWait(40);
    // "I cannot do that... Not without thinking it over carefully..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAttachedEffect(ACTOR_LAYANA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    // "How can we guarantee Hammet's safety if we send soldiers to attack?"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "I cannot do it. I cannot put Hammet's life at risk..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 20);
    // "Can't we do something to help?" Yes: "Oh! Well, it's about time you started agreeing with
    // me, Isaac!" No: "What!? Look, we both know Hammet's life is in danger!"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    advanceAfterReply = 1;
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 1)
    {
        FieldStateWork->msg++;
        advanceAfterReply = 0;
    }
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    if (advanceAfterReply != 0)
    {
        FieldStateWork->msg++;
    }
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 10);
    fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 40);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHWEST, 20);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 20);
    // "What are you two talking about?"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 10);
    // "Garet thinks that we can help."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
    // "It is too dangerous... If you fail, your lives would be in danger, too."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "Besides, don't you have your own quests to pursue?"
    fdScriptOpenMessage(ACTOR_LAYANA, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
        // "I thought so... I knew there had to be a reason Ivan had stayed with you, instead of
        // returning home directly from Vault."
        fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
        FieldStateWork->msg++;
    }
    else
    {
        fdScriptWait(20);
        fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
        FieldStateWork->msg++;
        // "I do not believe that... Ivan would not have stayed with you for so long if you did not
        // have an important event ahead."
        fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    }
    scHammetPalaceFacePartyNorth(20);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 40);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 10);
    // "Then what will happen to Lord Hammet?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    // "Hammet is a good hostage for them... They will not kill him."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 40);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 2);
    // "We shall wait for another message from Lunpa."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 40);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_IVAN, 0.8);
    fdActorWalkToAndWait(ACTOR_IVAN, 868, 472);
    // "Lady Layana, I cannot leave like this..."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "It will be fine, Ivan. Hammet would not want to distract you from..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    scHammetPalaceFacePartyOutward(40);
    scHammetPalaceFacePartyNorth(20);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    // "I apologize. I have said more than I should..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    // "It sounds like Hammet knows about our mission ...doesn't it?"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHWEST, 10);
    // "I suppose I can keep it a secret no longer..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 10);
    // "Very well, I shall tell you... He swore an oath to a Jupiter Adept."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(40);
    // "A Jupiter Adept!?"
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "Have you heard of a village called Vale?"
    fdScriptOpenMessage(ACTOR_LAYANA, 0);
    fdActorStartRepeatedMotion(ACTOR_GARET, 1);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
    {
        fdScriptWait(20);
        fdActorSetAttachedEffect(ACTOR_LAYANA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdScriptWait(40);
        // "You are from Vale? Really? Then the prophecy was true."
        fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
        FieldStateWork->msg += 2;
    }
    else
    {
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        FieldStateWork->msg++;
        // "Give me a break! You don't know Vale!? It's our hometown!"
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
        fdActorSetAttachedEffect(ACTOR_LAYANA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
        fdScriptWait(40);
        // "You are from Vale...? Really? Then the prophecy was true."
        fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    }
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 10);
    // "What prophecy?"
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "Three years after the terrible storm that struck only Mt. Aleph..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(40);
    // "Ivan must depart on a journey with warriors from Vale... So it was foretold."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "I've heard nothing of any such prophecy."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "It happened several years before Hammet founded Kalay... He was off traveling."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    // "Hammet felt he had reached an impasse as a merchant then."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    scHammetPalaceFacePartyNorth(10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "He risked his life on a journey to find a new source of business..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdActorSetAttachedEffect(ACTOR_LAYANA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    // "He was in mortal danger at the southwestern edge of the world, when some Adepts appeared and
    // saved his life."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    scHammetPalaceFacePartyOutward(40);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "He had lost consciousness. When he awoke, he found himself in their village."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    scHammetPalaceFacePartyNorth(20);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    // "Just a moment... Am I from that village?"
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // Yes. The Adept gave him the Shaman's Rod and you... along with some doubloons.
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Doubloons... Were they golden doubloons?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
    // Yes... Let's see... You are...called...
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    // "I am Mia, Lady Layana."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "It was a lot of gold, Mia, but he also received some business advice."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    // "Business advice? Hammet? The greatest merchant ever known?"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHWEST, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // Yes, Garet. Hammet has the Adepts to thank for his success.
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_SHAKE_HEAD);
    // "Wow... I've never heard anything like that..."
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // "He was advised to buy silk in Xian and sell it in Tolbi."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 10);
    fdActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "To see anything sell so well, it's like a dream come true."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHWEST, 10);
    // "There's no denying that was when Hammet picked up his knack for business."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
    // "Did he have to promise them anything in return?"
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 10);
    // "Ah, yes, Ivan... It is something you will need on your journey."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    scHammetPalaceFacePartyOutward(40);
    scHammetPalaceFacePartyNorth(20);
    fdActorRunRepeatedMotion(ACTOR_GARET, 1);
    // "Something we need on our journey..."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "The Adept asked Hammet to use the Shaman's Rod to obtain it."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "He looked for this object wherever he went... But he could never find it."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    // "Then, three years ago, a storm struck, one that hit only Mt. Aleph..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "Hammet was worried that time was running out..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 1);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    fdScriptWait(20);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    // "But he had managed to unearth a clue just in time."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimation(ACTOR_LAYANA, ANIM_NOD);
    // "That, Ivan, is why he took you with the caravan on his last journey north."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    // "I had no idea... So that must be why he left the rod in my care. But then..."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
    // "No, Ivan... You mustn't blame yourself..."
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 10);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "It was my carelessness that caused the rod to be stolen. And ultimately, that's why Lord
    // Hammet is being held in Lunpa..."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "You must continue your journey and complete your quest. You can't blame yourself."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 10);
    // "But what will happen to Hammet?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
    // "I will handle this."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorStartRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
    // "And don't forget the soldiers of Kalay!"
    fdScriptShowMessageThenWait(ACTOR_FIRST_SOLDIER, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    // "Continue on your journey, and do not worry about Hammet!"
    fdScriptShowMessageThenWait(ACTOR_SECOND_SOLDIER, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "I am sorry I cannot tell you more about the Shaman's Rod..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
    // "No, don't apologize..."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    // "The Shaman's Rod'll help us find that thing we need for our quest, right?"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    // "Will you look for it?"
    fdScriptOpenMessage(ACTOR_LAYANA, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
    {
        fdScriptWait(20);
        scHammetPalaceFacePartyNorth(10);
        fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
        fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        fdScriptWait(10);
        // "If Isaac says we will, then there's nothing to worry about!"
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
        FieldStateWork->msg += 2;
    }
    else
    {
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        FieldStateWork->msg++;
        // "What's wrong with you, Isaac? Did you forget why we're here?"
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
        scHammetPalaceFacePartyNorth(10);
        fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
        fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        fdScriptWait(10);
        // "If we need it for our quest, we'll have to find it sooner or later."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    }
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "Very well... Take care of yourselves."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 40);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHWEST, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 1);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // "Return to your posts, everyone!"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_SECOND_SOLDIER, ANIM_NOD);
    fdActorSetSpeed(ACTOR_FIRST_SOLDIER, 1);
    fdActorSetSpeed(ACTOR_SECOND_SOLDIER, 1);
    fdActorWalkTo(ACTOR_FIRST_SOLDIER, 848, 540);
    fdActorWalkToAndWait(ACTOR_SECOND_SOLDIER, 880, 540);
    fdActorSetPos(ACTOR_FIRST_SOLDIER, 0, 0);
    fdActorSetPos(ACTOR_SECOND_SOLDIER, 0, 0);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_GARET, 1);
    fdActorSetSpeed(ACTOR_IVAN, 1);
    fdActorSetSpeed(ACTOR_MIA, 1);
    partyScript = HammetPalacePartyActions;
    fdActorSetScript(ACTOR_GARET, partyScript);
    fdActorSetScript(ACTOR_IVAN, partyScript);
    fdActorRunScriptAndWait(ACTOR_MIA, partyScript);
    state                  = FieldStateWork;
    state->transitionFrame = MAP_TRANSITION_TICKS;
    state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    gmFlagClear(FLAG_SHOW_AREA_NAME);
    gmFlagSet(FLAG_IVAN_REPORTED_TO_LAYANA);
    fdScriptEnd();
}

// Turns the party outward from the group, then optionally waits.
static void scHammetPalaceFacePartyOutward(Sint32 waitTicks)
{
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    if (waitTicks != 0)
    {
        fdScriptWait(waitTicks);
    }
}

// Turns all four party members north toward Layana, then optionally waits.
static void scHammetPalaceFacePartyNorth(Sint32 waitTicks)
{
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    if (waitTicks != 0)
    {
        fdScriptWait(waitTicks);
    }
}

// Hammet's homecoming in Hammet Palace, the first time the party walks into Layana's room once he
// is free.
static void scHammetPalaceHammetsHomecoming(void)
{
    ACTOR        *leader;
    FIELD_STATE  *state;
    Sint32        answeredYes;
    const Sint32 *partyScript;

    fdScriptStart();
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    sySynch(1);
    fdCameraMoveTo(864, CAMERA_KEEP, 536, 0);
    fdMapGridCenterOnFollowedPoint();
    sySynch(1);
    fdActorSetPos(ACTOR_PARTY_LEADER, 864, 630);
    FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    FieldStateWork->transitionFrame = 40;
    fdScriptStartScreenTransition();
    fdCameraSetSpeed(0.4);
    fdCameraMoveTo(864, CAMERA_KEEP, 472, 1);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
    fdActorSetSpeed(ACTOR_GARET, 0.8);
    fdActorSetSpeed(ACTOR_IVAN, 0.8);
    fdActorSetSpeed(ACTOR_MIA, 0.8);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 864, 498);
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
    }
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetPosFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
    }
    leader = fdActorGet(ACTOR_PARTY_LEADER);
    if (leader != NULL)
    {
        fdActorSetPosFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
    }
    fdActorWalkTo(ACTOR_PARTY_LEADER, 856, 486);
    fdActorWalkTo(ACTOR_GARET, 848, 502);
    fdActorWalkTo(ACTOR_IVAN, 872, 486);
    fdActorWalkToAndWait(ACTOR_MIA, 880, 502);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    fdScriptWait(10);
    scHammetPalaceFacePartyNorth(10);
    fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 20);
    fdScriptSetMessageId(MsgKalayItsIvanHisCompanionsPerfect);
    // "So! It's Ivan and his companions... Perfect timing!"
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 20);
    // "So, it's you..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
    // "I told you time and time again not to go to Lunpa, but you..."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(60);
    fdActorShowEmote(ACTOR_HAMMET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_WEST_SOUTHWEST, 10);
    // "Please, Layana, don't be so harsh on them."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_EAST_SOUTHEAST, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
    // "Naturally, I'm happy that they rescued you."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 40);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 2);
    // "But this was something that Kalay should have handled alone."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 10);
    // "You had a more pressing quest at hand, no?"
    fdScriptOpenMessage(ACTOR_LAYANA, 0);
    answeredYes = 1;
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
    {
        fdScriptWait(10);
        fdActorSetAnimation(ACTOR_LAYANA, ANIM_NOD);
    }
    else
    {
        fdScriptWait(10);
        scHammetPalaceAdvanceMessage(1);
        fdActorSetAnimation(ACTOR_LAYANA, ANIM_SHAKE_HEAD);
        answeredYes = 0;
    }
    // Yes: "Yes. Your mission is to recover the Elemental Stars." No: "Have you completely
    // forgotten your own quest!?"
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    if (answeredYes != 0)
    {
        scHammetPalaceAdvanceMessage(1);
    }
    fdActorStartRepeatedMotion(ACTOR_HAMMET, 2);
    fdActorSetAttachedEffect(ACTOR_HAMMET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(80);
    // "Layana, you shouldn't..."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_EAST_SOUTHEAST, 10);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 3);
    // "I... I had to rescue Hammet, no matter the cost..."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 20);
    fdActorShowEmote(ACTOR_LAYANA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdActorFaceDirection(ACTOR_LAYANA, FACING_SOUTH_SOUTHEAST, 60);
    // "It's already done. There is nothing more to be said."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    // "Hold on! We all wanted to save Hammet as badly as Ivan did."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 10);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
    // "It doesn't matter, Garet. I'm the one at fault!"
    syFieldScriptShowMessageThenWait(
        SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
    fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
    // "I made a mistake. We will leave immediately on Felix's trail!"
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 20);
    fdActorSetAnimationAndWait(ACTOR_LAYANA, ANIM_NOD);
    // Yes. That would be best. Leave immediately.
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 20);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    // "Isaac, we should be going."
    fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
    fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // "Ivan, is that really what you want?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(80);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
    // "Well, if Ivan says so... Should we go, Isaac?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(0, 0) == 0)
    {
        fdScriptWait(20);
        scHammetPalaceAdvanceMessage(1);
    }
    else
    {
        fdScriptWait(20);
        // "If both Isaac and Ivan want to leave, then I guess that's it."
        fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    }
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    scHammetPalaceFacePartyNorth(20);
    fdActorRunRepeatedMotion(ACTOR_HAMMET, 2);
    // "Take your Shaman's Rod and go to Hesperia."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    // "To Hesperia...?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 10);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHEAST, 10);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_NOD);
    // "That is what the Jupiter Adept said."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Hesperia... Where is that?"
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_IVAN, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_HAMMET, 1);
    fdActorFaceDirection(ACTOR_HAMMET, FACING_SOUTH_SOUTHWEST, 10);
    // "It is the continent to the west of Angara."
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    // "What are we going to find in Hesperia if we go?"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_HAMMET, ANIM_SHAKE_HEAD);
    // "That, I do not know..."
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_HAMMET, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_MIA, 1);
    // "The continent to the west? On the other side of the sea?"
    fdScriptShowMessageThenWait(ACTOR_MIA, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_LAYANA, 1);
    fdScriptWait(20);
    // "Do not waste time chattering. You must set out immediately."
    fdScriptShowMessageThenWait(ACTOR_LAYANA, 0, 10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    partyScript = HammetPalacePartyActions;
    fdActorSetScript(ACTOR_GARET, partyScript);
    fdActorSetScript(ACTOR_IVAN, partyScript);
    fdActorRunScriptAndWait(ACTOR_MIA, partyScript);
    state                  = FieldStateWork;
    state->transitionFrame = MAP_TRANSITION_TICKS;
    state->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    gmFlagClear(FLAG_SHOW_AREA_NAME);
    gmFlagSet(FLAG_HAMMET_BACK_HOME_SEEN);
    fdScriptEnd();
}

// Garet, Ivan and Mia walk into Isaac and fall in behind him.
Sint32 HammetPalacePartyActions[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(856, 0, 486),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

SCENE_ENTRANCE HammetPalaceEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE HammetPalaceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {HAMMET_PALACE_FROM_HAMMET_PALACE_GROUNDS,
     CONDITION_ALWAYS, 240,
     0, 648,
     FACING_NORTH, 0,
     45, 360,
     440, 680,
     0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_WEST_WING,
     CONDITION_ALWAYS, 88,
     0, 552,
     FACING_SOUTH, 0,
     45, 360,
     440, 680,
     0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_EAST_WING,
     CONDITION_ALWAYS, 392,
     0, 552,
     FACING_SOUTH, 0,
     45, 360,
     440, 680,
     0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_LAYANAS_ROOM,
     CONDITION_ALWAYS, 240,
     0, 408,
     FACING_SOUTH, 0,
     45, 360,
     440, 680,
     0},
    {HAMMET_PALACE_ENTRANCE_ROOM_FROM_LANDING,
     CONDITION_ALWAYS, 152,
     0, 432,
     FACING_SOUTH, 0,
     45, 360,
     440, 680,
     0},
    {HAMMET_PALACE_WEST_WING_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 264,
     0, 136,
     FACING_NORTH, 0,
     40, 20,
     305, 285,
     0},
    {HAMMET_PALACE_EAST_WING_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 504,
     0, 152,
     FACING_NORTH, 0,
     380, 20,
     755, 290,
     0},
    {HAMMET_PALACE_EAST_WING_FROM_PASSAGE_DOOR,
     CONDITION_ALWAYS, 448,
     0, 112,
     FACING_SOUTH, 0,
     380, 20,
     755, 290,
     0},
    {HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 864,
     0, 576,
     FACING_NORTH, 0,
     750, 370,
     985, 635,
     0},
    {HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING,
     CONDITION_ALWAYS, 928,
     0, 224,
     FACING_WEST, 0,
     800, 20,
     1040, 250,
     0},
    {HAMMET_PALACE_FROM_KALAY_TUNNEL_TREASURE_ROOM,
     CONDITION_ALWAYS, 904,
     0, 72,
     FACING_SOUTH, 0,
     800, 20,
     1040, 250,
     0},
    {HAMMET_PALACE_BEDROOM_FROM_LANDING,
     CONDITION_ALWAYS, 616,
     0, 824,
     FACING_NORTH, 0,
     465, 640,
     705, 875,
     0},
    {HAMMET_PALACE_LANDING_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 128,
     0, 808,
     FACING_EAST, 0,
     20, 730,
     260, 890,
     0},
    {HAMMET_PALACE_LANDING_FROM_BEDROOM,
     CONDITION_ALWAYS, 168,
     0, 808,
     FACING_SOUTH, 0,
     20, 730,
     260, 890,
     0},
    {HAMMET_PALACE_LAYANAS_ROOM_FROM_HAMMET_PALACE_GROUNDS,
     CONDITION_ALWAYS, 864,
     0, 576,
     FACING_NORTH, 0,
     750, 370,
     985, 635,
     0},
    {SCENE_TABLE_END},
};

Uint32 HammetPalaceExits[] = {
    SCENE_EXITS(HammetPalace),
    SCENE_EXIT(HAMMET_PALACE_TO_HAMMET_PALACE_GROUNDS,
               HammetPalaceGrounds,
               HAMMET_PALACE_GROUNDS_FROM_HAMMET_PALACE),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_WEST_WING,
               HammetPalace,
               HAMMET_PALACE_WEST_WING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_EAST_WING,
               HammetPalace,
               HAMMET_PALACE_EAST_WING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_LAYANAS_ROOM,
               HammetPalace,
               HAMMET_PALACE_LAYANAS_ROOM_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_ENTRANCE_ROOM_TO_LANDING,
               HammetPalace,
               HAMMET_PALACE_LANDING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(HAMMET_PALACE_WEST_WING_TO_ENTRANCE_ROOM,
               HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_WEST_WING),
    SCENE_EXIT(HAMMET_PALACE_EAST_WING_TO_ENTRANCE_ROOM,
               HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_EAST_WING),
    SCENE_EXIT(HAMMET_PALACE_EAST_WING_TO_PASSAGE_DOOR,
               HammetPalace,
               HAMMET_PALACE_PASSAGE_DOOR_FROM_EAST_WING),
    SCENE_EXIT(HAMMET_PALACE_LAYANAS_ROOM_TO_ENTRANCE_ROOM,
               HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_LAYANAS_ROOM),
    SCENE_EXIT(HAMMET_PALACE_PASSAGE_DOOR_TO_EAST_WING,
               HammetPalace,
               HAMMET_PALACE_EAST_WING_FROM_PASSAGE_DOOR),
    SCENE_EXIT(HAMMET_PALACE_TO_KALAY_TUNNEL_TREASURE_ROOM,
               KalayTunnelTreasureRoom,
               KALAY_TUNNEL_TREASURE_ROOM_FROM_HAMMET_PALACE),
    SCENE_EXIT(HAMMET_PALACE_BEDROOM_TO_LANDING, HammetPalace, HAMMET_PALACE_LANDING_FROM_BEDROOM),
    SCENE_EXIT(HAMMET_PALACE_LANDING_TO_ENTRANCE_ROOM,
               HammetPalace,
               HAMMET_PALACE_ENTRANCE_ROOM_FROM_LANDING),
    SCENE_EXIT(HAMMET_PALACE_LANDING_TO_BEDROOM, HammetPalace, HAMMET_PALACE_BEDROOM_FROM_LANDING),
    SCENE_EXITS_END,
};

ACTOR_PLACE HammetPalaceActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
ACTOR_PLACE HammetPalaceActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LAYANA, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        856, 0, 440, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_HAMMET, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        872, 0, 440, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        616, 0, 113, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        712, 0, 246, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_BLACK_SUIT, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        212, 0, 522, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED_KALAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        169, 0, 114, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN_KALAY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 114, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

CUE HammetPalaceCuesOther[] = {
    { CUE_TABLE_END },
};

CUE HammetPalaceCues[] = {
    { CUE_EXIT, 1, CONDITION_ALWAYS, HAMMET_PALACE_TO_HAMMET_PALACE_GROUNDS },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), HAMMET_PALACE_ENTRANCE_ROOM_TO_WEST_WING, CONDITION_ALWAYS, SCRIPT_REF(scHammetPalaceLeaveThroughDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), HAMMET_PALACE_ENTRANCE_ROOM_TO_EAST_WING, CONDITION_ALWAYS, SCRIPT_REF(scHammetPalaceLeaveThroughDoor) },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), HAMMET_PALACE_ENTRANCE_ROOM_TO_LAYANAS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scHammetPalaceLeaveThroughDoor) },
    { CUE_EXIT, 5, CONDITION_ALWAYS, HAMMET_PALACE_ENTRANCE_ROOM_TO_LANDING },
    { CUE_EXIT, 6, CONDITION_ALWAYS, HAMMET_PALACE_WEST_WING_TO_ENTRANCE_ROOM },
    { CUE_EXIT, 7, CONDITION_ALWAYS, HAMMET_PALACE_EAST_WING_TO_ENTRANCE_ROOM },
    { CUE_EXIT, 8, CONDITION_ALWAYS, HAMMET_PALACE_EAST_WING_TO_PASSAGE_DOOR },
    { CUE_EXIT, 9, CONDITION_ALWAYS, HAMMET_PALACE_LAYANAS_ROOM_TO_ENTRANCE_ROOM },
    { CUE_EXIT, 10, CONDITION_ALWAYS, HAMMET_PALACE_PASSAGE_DOOR_TO_EAST_WING },
    { CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 11, CONDITION_ALWAYS, HAMMET_PALACE_TO_KALAY_TUNNEL_TREASURE_ROOM },
    { CUE_EXIT, 12, CONDITION_ALWAYS, HAMMET_PALACE_BEDROOM_TO_LANDING },
    { CUE_EXIT, 13, CONDITION_ALWAYS, HAMMET_PALACE_LANDING_TO_ENTRANCE_ROOM },
    { STEP_CUE_WALKING_INTO(FACING_NORTH), HAMMET_PALACE_LANDING_TO_BEDROOM, CONDITION_ALWAYS, SCRIPT_REF(scHammetPalaceLeaveThroughDoor) },
    { CUE_TALK, ACTOR_LAYANA, FLAG_KALAY_HEARD_HAMMET_HELD_IN_LUNPA, MSGNO(MsgHammetPalaceDodonpaIsAVeryShrewdMan) },
    { CUE_TALK, ACTOR_LAYANA, FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS, SCRIPT_REF(scHammetPalaceLayanaScoldsLunpaVisit) },
    { CUE_TALK, ACTOR_LAYANA, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceDodonpaPlansToUseHammetTo) },
    { CUE_TALK, ACTOR_LAYANA, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceNoOneInKalayKnowsHammet) },
    { CUE_TALK, ACTOR_HAMMET, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTakeYourShamansRodAndSet) },
    { CUE_TALK, ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceTheTownsfolkDontKnowAboutLord) },
    { CUE_TALK, ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceIHearHammetWasSavedBy) },
    { CUE_TALK, ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, FLAG_DODONPA_LOCKED_UP, SCRIPT_REF(scHammetPalaceAskWhetherHammetWillBeReleased) },
    { CUE_TALK, ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheySayHammetsReturnIsSupposed) },
    { CUE_TALK, ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceLayanaIsPuttingUpAStrong) },
    { CUE_TALK, ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceILeftTheDoorToThe) },
    { CUE_TALK, ACTOR_AFRAID_OF_DODONPAS_THUGS, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceMerchantsAreSupposedToBeProtected) },
    { CUE_TALK, ACTOR_AFRAID_OF_DODONPAS_THUGS, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceIWonderIfAnyOfDodonpas) },
    { CUE_TALK, ACTOR_ASKING_ABOUT_THE_SILK_TRADE, FLAG_DODONPA_LOCKED_UP, SCRIPT_REF(scHammetPalaceAskWhetherSilkTradeHasEnded) },
    { CUE_TALK, ACTOR_ASKING_ABOUT_THE_SILK_TRADE, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheSecretPassageIsLockedBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAYANA, FLAG_KALAY_LAYANA_SAID_LUNPA_TOO_DANGEROUS, MSGNO(MsgHammetPalaceIsThisTheRightDecisionHammet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAYANA, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceIDontCareAboutTheMoney) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LAYANA, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceImSoHappyHammetMadeIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMET, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceLayanasWordsPainHerMoreThan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceImSureTheVillagersWouldPanic) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_ABOUT_THE_TOWNSFOLK, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceImSoRelievedHammetMadeIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceEveryonesWorriedAboutLordHammetBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_IF_HAMMET_WILL_BE_RELEASED, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceICantLetOnToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceLadyLayanaWasCryingAgainLast) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEFT_THE_PASSAGE_DOOR_OPEN, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceLayanaOrderedTheDoorToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFRAID_OF_DODONPAS_THUGS, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceAndToThinkIAlmostWent) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_AFRAID_OF_DODONPAS_THUGS, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceWeWillNeverLetDodonpasHenchmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_THE_SILK_TRADE, FLAG_DODONPA_LOCKED_UP, MSGNO(MsgHammetPalaceLordHammetIsALegendAmong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ASKING_ABOUT_THE_SILK_TRADE, CONDITION_ALWAYS, MSGNO(MsgHammetPalaceTheKeyToTheSecretPassage) },
    { EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_HAMMET_PALACE_WATER_JACKET_CHEST, FIND_ITEM(ITEM_WATER_JACKET) },
    { EXAMINE_CUE(EXAMINE_BARREL), 101, FLAG_HAMMET_PALACE_NUT_BARREL, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_CUE(EXAMINE_WARDROBE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresATurbanItsTheSymbol)) },
    { EXAMINE_CUE(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresABookEntitledBusinessAnd)) },
    { EXAMINE_CUE(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgHammetPalaceTheresABookEntitledTheGood)) },
    { CUE_TABLE_END },
};
// clang-format on

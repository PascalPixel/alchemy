// Elemental Star Room (SceneIdSolSanctumEruption), the chamber of
// SceneIdSolSanctumElementalStarRoom (sol_sanctum_elemental_star_room.c) as it collapses once the
// Mars Star is taken.
//
// The scene is one script, with no cues of its own. scInit plays it on arrival at
// SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM, in two parts:
// - scSolSanctumEruptionPlayHostageScript: the chamber quakes, and the Wise
// One  appears and stills it. Saturos's group leaves with Jenna and Kraden,
// rising out of the chamber one by one. Jenna leaves the party.
// - scSolSanctumEruptionPlayWiseOneRescue: the Wise One has Isaac keep the Mars
//       Star, then sends Isaac and Garet away in a white flash. It sets
//       FLAG_SOL_SANCTUM_ERUPTED, which every room of Sol Sanctum tests from
// then on, and FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED. Then it takes
//       SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS to the Luna and Sol
//       rooms (SceneIdSolSanctumLunaSolRooms).
//
// The effects are the script's own.
//
// Sixteen blue crystals are thrown out in a ring (scSolSanctumEruptionThrowEruptionRing). They
// become embers launched from a pool at a varying rate. Two events do that,
// scSolSanctumEruptionStepEmbers and scSolSanctumEruptionUpdateEmberFrequency, which the first part
// adds and the second removes. Once Saturos's group starts to leave
// (FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING), the embers fall silently.
//
// The breaking floor's metatiles flicker by event until the Wise One holds them. Ten lights rise at
// each of the chamber's four corners as the Wise One tells of the stars
// (scSolSanctumEruptionRaiseElementalLights). The Wise One glows through a cycle of palettes.
//
// Saturos's group and their hostages leave one by one from the platform where the party arrived
// (scSolSanctumEruptionRaiseAndShrinkActor). Each walks onto it and rises 30 pixels at full size.
// Then he rises on as he shrinks away, over a shadow that shrinks with him. The platform's actor,
// ACTOR_PLATFORM_SHADOW, draws that shadow.

#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "graphics/graphics.h"
#include "scene/sol_sanctum_elemental_star_lights.h"
#include "scene/script_psynergy.h"
#include "text/msg_ids.h"

// The embers the eruption throws, one for each of the crystal actors they reuse once the crystals
// are gone.
enum
{
    EMBER_COUNT = 16
};

// The lights that rise at a corner of the chamber, one after another.
enum
{
    RISING_LIGHT_COUNT = 10
};

// Unused: eight bytes before the embers' states that nothing reads.
Uint8                  SolSanctumEruptionUnusedBeforeEmberState[8] BSS_DATA;
// Each ember's state, and the shake an ember's landing starts: ten times its pixels, three less
// each tick.
Sint32                 EmberState[EMBER_COUNT] BSS_DATA;
Sint32 EmberTimer      BSS_DATA;
// Unused: twelve bytes after the ember timer that nothing reads.
Uint8                  SolSanctumEruptionUnusedAfterEmberTimer[12] BSS_DATA;
// The elemental lights the chamber throws out: each one's actor, and its age in ticks, 0 for none.
ACTOR                 *RisingLights[RISING_LIGHT_COUNT] BSS_DATA;
// Unused: eight bytes after the lights' actors that nothing reads.
Uint8                  SolSanctumEruptionUnusedAfterRisingLights[8] BSS_DATA;
Sint32                 RisingLightTimers[RISING_LIGHT_COUNT] BSS_DATA;
Sint32 EmberMask       BSS_DATA;
Sint32 EmberLevel      BSS_DATA;
Sint32 EmberLevelTimer BSS_DATA;

extern Sint32          EruptionRisingLightOrigins[];
extern Uint32          SolSanctumEruptionExits[];
extern ACTOR_PLACE     SolSanctumEruptionActorPlacements[];
extern CUE             SolSanctumEruptionCues[];
extern Sint32          EruptionEmberScript[];

// The eruption's actors, numbered in the order of SolSanctumEruptionActorPlacements.
typedef enum COLLAPSE_ACTOR_ID
{
    // The shadow on the platform under each who leaves with Saturos: a blank actor, whose sprite
    // draws only its shadow, shrunk with the departing actor, whose own shadow is turned off
    // (scSolSanctumEruptionRaiseAndShrinkActor).
    ACTOR_PLATFORM_SHADOW = ACTOR_FIRST_PLACED,
    ACTOR_KRADEN,
    ACTOR_SATUROS,
    ACTOR_MENARDI,
    // Felix in his mask, before the star room unmasks him.
    ACTOR_FELIX_MASKED,
    ACTOR_FELIX,
    ACTOR_ALEX
} COLLAPSE_ACTOR_ID;

// Saturos's group has started to leave, and the embers fall without their sound.
#define FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING FLAG_SCENE_TEMP_46

static void scSolSanctumEruptionRaiseAndShrinkActor(Sint32 actorId);
static void scSolSanctumEruptionThrowEruptionRing(void);
static void scSolSanctumEruptionResetEmberEmission(void);
static void scSolSanctumEruptionStepEmbers(void);
static void scSolSanctumEruptionUpdateEmberFrequency(void);
static void scSolSanctumEruptionPlayWiseOneRescue(void);
static void scSolSanctumEruptionPlaceBeforeStar(Sint32 actorId);
static void scSolSanctumEruptionDrawFirstCollapseTiles(Sint32 broken);
static void scSolSanctumEruptionDrawSecondCollapseTiles(Sint32 broken);
static void scSolSanctumEruptionFlickerFirstCollapseTiles(void);
static void scSolSanctumEruptionFlickerSecondCollapseTiles(void);
static void scSolSanctumEruptionRaiseElementalLights(Uint32 corner);
static void scSolSanctumEruptionUpdateElementalLights(void);
static void scSolSanctumEruptionFadeCameraTo(Sint32 x, Sint32 z);
static void scSolSanctumEruptionClearStatueMetatiles(void);
static void scSolSanctumEruptionSetPaleTint(Sint32 enabled, Sint32 ticks, Sint32 waitTicks);
static void scSolSanctumEruptionFlashWiseOne(ACTOR *actor);
static void scSolSanctumEruptionSpawnEffectPair(ACTOR *parent);
static void scSolSanctumEruptionUpdateWiseOnePalette(void);

// The four corners of the chamber where scSolSanctumEruptionRaiseElementalLights raises lights, in
// the order the Wise One's script visits them.
typedef enum SOL_SANCTUM_ERUPTION_CORNER
{
    CHAMBER_NORTHWEST = 0,
    CHAMBER_SOUTHWEST = 1,
    CHAMBER_NORTHEAST = 2,
    CHAMBER_SOUTHEAST = 3
} SOL_SANCTUM_ERUPTION_CORNER;

// The Wise One, and the crystals, which become the embers.
enum
{
    ACTOR_WISE_ONE      = ACTOR_ALEX + 1,
    // The sixteen crystals thrown out in a ring.
    ACTOR_FIRST_CRYSTAL = ACTOR_WISE_ONE + 1,
    CRYSTAL_COUNT       = EMBER_COUNT,
    // The same actors, reused as the embers once the crystals are gone.
    ACTOR_FIRST_EMBER   = ACTOR_FIRST_CRYSTAL
};

// SPRITE_BLUE_CRYSTAL's animations (blue_crystal.tsv): the crystal glinting, and shattering into
// shards that vanish.
enum
{
    BLUE_CRYSTAL_ANIMATION_GLINT   = 2,
    BLUE_CRYSTAL_ANIMATION_SHATTER = 3
};

// SPRITE_WISE_ONE's animations (wise_one.tsv): its eye lowering and held low as it holds the floor
// with Psynergy, and lifting again into its blinking idle.
enum
{
    WISE_ONE_ANIMATION_LOWER_EYE = 2,
    WISE_ONE_ANIMATION_RAISE_EYE = 3
};

// A light stretches for ELEMENTAL_LIGHT_STRETCH_TICKS, narrowing by nine tenths of its width, then
// is lifted out of view until it is done with at ELEMENTAL_LIGHT_TICKS.
#define ELEMENTAL_LIGHT_STRETCH_TICKS    8
#define ELEMENTAL_LIGHT_TICKS            14
#define ELEMENTAL_LIGHT_SCALE_X_DECREASE (toFIXED(9) / 10 / ELEMENTAL_LIGHT_STRETCH_TICKS)

// EmberState: free, in flight, then counting ticks from its landing, when it bursts, until it is
// put away and then freed.
enum
{
    EMBER_FREE     = 0,
    EMBER_FLYING   = 1,
    EMBER_LANDED   = 2,
    EMBER_PUT_AWAY = 19,
    EMBER_DONE     = 20
};

// EmberMask for an ember every `ticks` ticks, a power of two: one is launched when the tick count
// has none of the mask's bits.
#define EMBER_EVERY(ticks)      ((ticks) - 1)
#define EMBER_EMISSION_DISABLED 999
#define LIGHT_SCAN_STILL_ACTIVE 888

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return SolSanctumStarRoomEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return SolSanctumEruptionExits;
}

// Returns everyone the script moves, all placed off the map until it does.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    return SolSanctumEruptionActorPlacements;
}

// Returns the empty cue table, because the scene is one script.
const CUE *scGetCues(void)
{
    return SolSanctumEruptionCues;
}

// Stages Saturos, Menardi, Alex and Felix around Jenna and Kraden.
static inline void scSolSanctumEruptionPlaceHostageGroup(void)
{
    fdActorSetPos(ACTOR_JENNA, 475, 332);
    fdActorSetPos(ACTOR_KRADEN, 491, 332);
    fdActorSetPos(ACTOR_MENARDI, 459, 348);
    fdActorSetPos(ACTOR_SATUROS, 507, 348);
    fdActorSetPos(ACTOR_FELIX, 471, 306);
    fdActorSetPos(ACTOR_ALEX, 479, 362);
}

// Elemental Star Room, collapsing: Saturos's group leaves with its hostages.
static void scSolSanctumEruptionPlayHostageScript(void)
{
    ACTOR         *actor;
    WEATHER_STATE *sparkles;

    sparkles = WeatherStateWork;
    fdScriptStart();
    sparkles->stopped = 0;
    // Reopen the chamber during the first quake and stage the two groups.
    sySoundPlay(SE_RUMBLE);
    fdMapGridSetShake(1, 1, 1);
    fdActorSetPos(ACTOR_PARTY_LEADER, 232, 156);
    fdActorSetPos(ACTOR_GARET, 218, 172);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    scSolSanctumEruptionPlaceHostageGroup();
    fdActorSetSpritePriority(ACTOR_WISE_ONE, 1);
    fdCameraMoveTo(232, CAMERA_KEEP, 156, 0);
    fdMapGridCenterOnFollowedPoint();
    FieldStateWork->transitionFrame = 8;
    fdScriptStartScreenTransition();
    fdScriptWaitTransitionTicks();
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(4);
    fdScriptWait(4);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(4);
    fdScriptWait(4);
    scSolSanctumEruptionThrowEruptionRing();
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(4);
    fdScriptWait(16);
    sySoundPlay(SE_CLATTER);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(4);
    fdScriptWait(4);
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(4);
    fdScriptWait(4);
    sySoundPlay(SE_CLATTER);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(48);
    fdScriptWait(48);
    fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
    fdActorJump(ACTOR_GARET, 6, 20);
    // Off screen, Menardi: "What happened?" Saturos: "Not again!" Jenna: the water's gone. Alex:
    // without the Elemental Stars the chamber is collapsing. Kraden: is this the "terrible
    // retribution"?
    scSolSanctumEruptionSetPaleTint(1, 20, 20);
    scSolSanctumEruptionSetPaleTint(0, 20, 40);
    fdScriptSetMessageId(MsgSolSanctumJennaWhatHappened);
    // "What's the matter? What happened?"
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    // "No! Not again!"
    fdScriptShowMessage(ACTOR_SATUROS, 0);
    scSolSanctumEruptionSetPaleTint(1, 20, 0);
    fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 20);
    scSolSanctumEruptionSetPaleTint(0, 20, 0);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 0);
    fdScriptWait(40);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 30);
    scSolSanctumEruptionSetPaleTint(1, 20, 0);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    scSolSanctumEruptionSetPaleTint(0, 20, 20);
    // The Wise One, a floating rock, appears over the chamber and the quake stops.
    fdActorSetSpriteFlags(fdActorGet(ACTOR_WISE_ONE), 0);
    fdActorSetPos(ACTOR_WISE_ONE, 325, 302);
    actor              = fdActorGet(ACTOR_WISE_ONE);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FLOATS;
    sparkles->stopped  = 1;
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdMapGridWaitForShakeToSettle();
    fdScriptWait(150);
    // Menardi: "It's over... But what was it?" Jenna: "What is that?!?" Saturos: "No!" The camera
    // pans to the Wise One.
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 20);
    fdScriptShowMessage(ACTOR_JENNA, 0);
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GARET, 20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 10);
    scSolSanctumEruptionPlaceHostageGroup();
    fdCameraSetSpeed(6.4);
    fdCameraMoveTo(328, CAMERA_KEEP, 299, 1);
    fdCameraWaitForMove();
    sySoundPlay(SE_MAP_TRANSITION);
    fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(20, 20, 20), PALETTES_SPRITES);
    fdPaletteFadeStartInterpolation(20);
    sySynch(20);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_SPRITES);
    fdPaletteFadeStartInterpolation(20);
    fdScriptWait(200);
    // Garet: "That rock... It's floating... Do you know what it is?" Yes: "Yeah, I know it's a
    // rock, stupid!" No: someone is lifting it with Psynergy. Kraden: "Could that be... the Wise
    // One?!?"
    fdScriptOpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptSetMessageId(MsgSolSanctumIKnowItsARock);
    }
    else
    {
        fdScriptSetMessageId(MsgSolSanctumSomeoneIsLiftingIt);
    }
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 80);
    fdScriptSetMessageId(MsgSolSanctumKradenCouldThatBeThe);
    // "Could that be... the Wise One?!?"
    fdScriptShowMessageThenWait(ACTOR_KRADEN, 0, 20);
    scSolSanctumEruptionSetPaleTint(1, 20, 0);
    sparkles->stopped = 0;
    sySoundPlay(SE_RUMBLE);
    fdMapGridSetShake(1, 2, 1);
    fdScriptWait(80);
    sparkles->stopped = 1;
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdMapGridWaitForShakeToSettle();
    scSolSanctumEruptionSetPaleTint(0, 20, 60);
    // Alex: "Such power... Incredible!" Garet: "!!!" Jenna: "Wow..."
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 30);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_NORTHWEST, 40);
    scSolSanctumEruptionSetPaleTint(1, 20, 20);
    scSolSanctumEruptionSetPaleTint(0, 20, 20);
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 30);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_EAST_SOUTHEAST, 40);
    scSolSanctumEruptionSetPaleTint(1, 20, 20);
    scSolSanctumEruptionSetPaleTint(0, 20, 20);
    fdScriptShowMessageThenWait(ACTOR_JENNA, 0, 30);
    // Embers start to rain from the Wise One.
    scSolSanctumEruptionResetEmberEmission();
    sySetTickEvent(scSolSanctumEruptionStepEmbers, EVENT_PASS_TICK);
    sySetTickEvent(scSolSanctumEruptionUpdateEmberFrequency, EVENT_PASS_TICK);
    fdScriptWait(240);
    // Saturos: "It must be the guardian of the Elemental Stars!" The camera moves to his group, all
    // facing west.
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 30);
    scSolSanctumEruptionPlaceHostageGroup();
    fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_ALEX, FACING_WEST, 0);
    actor = fdActorGet(ACTOR_JENNA);
    fdCameraMoveToFX(
        toFIXED(actor->x.part[0].integer), CAMERA_KEEP, toFIXED(actor->z.part[0].integer), 1);
    fdCameraWaitForMove();
    fdScriptWait(40);
    fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
    fdScriptWait(20);
    // Felix: they should escape while they can. Menardi: not without the Mars Star. Alex sides with
    // Felix, and Saturos agrees to leave it.
    fdScriptShowMessage(ACTOR_FELIX, 0);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 10);
    fdActorSetAnimation(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 20);
    fdActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    // Jenna: "What about Isaac and Garet? You can't just leave them here!"
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 40);
    fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0, 10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 40);
    fdActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 40);
    fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 10);
    // Saturos: "Forget about them." Menardi: they might survive. Saturos: "So what do you suggest
    // we do?" Alex: take Jenna, and they will bring the Star to get her back.
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_WEST, 10);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 10);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_NOD);
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 40);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 60);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 20);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 20);
    // Alex: "Regardless, Kraden must come with us."
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
    fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    // Felix steps forward: Alex is breaking their promise. Alex: conditions have changed. Felix
    // will not let them hurt Jenna; Kraden offers himself in her place.
    fdActorSetSpeed(ACTOR_FELIX, 1);
    fdActorWalkToAndWait(ACTOR_FELIX, 471, 314);
    fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
    fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 10);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 10);
    fdActorSetAnimation(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 10);
    fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
    fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 10);
    fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 20);
    // Saturos: "you cannot help Jenna now." Menardi: if they leave her, she will die; is that what
    // Felix wants?
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 30);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_EAST, 20);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 30);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_SHAKE_HEAD);
    fdScriptWait(10);
    fdScriptShowMessageThenWait(ACTOR_MENARDI, 0, 10);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHWEST, 20);
    fdActorRunRepeatedMotion(ACTOR_FELIX, 2);
    fdScriptWait(20);
    fdActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 0);
    fdScriptWait(30);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
    fdScriptWait(40);
    fdActorRunRepeatedMotion(ACTOR_FELIX, 1);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
    fdScriptWait(40);
    fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
    fdScriptWait(30);
    // Alex: "Perhaps we ought to continue this elsewhere, Felix." Jenna: "Felix..." Alex:
    // "Please...can we go now?"
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0, 30);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 40);
    fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    fdActorShowEmote(ACTOR_SATUROS, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 60);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    fdScriptWait(40);
    fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    fdActorFaceEachOther(ACTOR_SATUROS, ACTOR_MENARDI, 20);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_SATUROS, 0.6);
    fdActorSetSpeed(ACTOR_MENARDI, 0.6);
    fdActorWalkTo(ACTOR_MENARDI, 475, 348);
    fdActorWalkTo(ACTOR_SATUROS, 491, 348);
    fdActorWaitForMoveById(ACTOR_MENARDI);
    fdActorWaitForMoveById(ACTOR_SATUROS);
    fdActorSetAnimation(ACTOR_MENARDI, ANIM_STAND);
    fdActorSetAnimation(ACTOR_SATUROS, ANIM_STAND);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 10);
    // Menardi: "Let's make haste!" She shoves Jenna, who stumbles away.
    fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 2);
    fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI, 0);
    fdActorSetSpeed(ACTOR_MENARDI, 1.6);
    fdActorSetSpeed(ACTOR_JENNA, 1.2);
    fdActorWalkToAndWait(ACTOR_MENARDI, 475, 338);
    actor                = fdActorGet(ACTOR_MENARDI);
    actor->steeringFlags = (Uint8)~ACTOR_FACES_MOTION & actor->steeringFlags;
    fdActorWalkToAndWait(ACTOR_MENARDI, 475, 348);
    fdActorRunRepeatedMotion(ACTOR_JENNA, 1);
    fdActorJump(ACTOR_JENNA, 4, 0);
    fdActorWalkToAndWait(ACTOR_JENNA, 459, 316);
    fdActorSetSpeed(ACTOR_JENNA, 0.5);
    // Felix: "Hey, don't push my sister!" Menardi: "You owe us your life!" Saturos: "Menardi!"
    fdActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    fdActorStartRepeatedMotion(ACTOR_FELIX, 3);
    fdScriptShowMessageThenWait(ACTOR_FELIX, 0, 30);
    fdActorRunRepeatedMotion(ACTOR_MENARDI, 1);
    fdScriptWait(20);
    actor->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorSetSpeed(ACTOR_MENARDI, 1);
    fdActorWalkToAndWait(ACTOR_MENARDI, 475, 332);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
    fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MENARDI, 0);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHWEST, 0);
    fdActorStartRepeatedMotion(ACTOR_SATUROS, 2);
    fdScriptShowMessageThenWait(ACTOR_SATUROS, 0, 20);
    fdActorShowEmote(ACTOR_MENARDI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 20);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_SHAKE_HEAD);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHWEST, 20);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    fdScriptWait(20);
    fdActorWalkTo(ACTOR_JENNA, 432, 316);
    fdActorWalkToAndWait(ACTOR_FELIX, 422, 311);
    fdActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
    fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 20);
    // Felix leads Jenna aside. Saturos: "Master Kraden, you will be joining us, of course!" All
    // gather under the departure point.
    fdActorRunRepeatedMotion(ACTOR_SATUROS, 2);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH_NORTHEAST, 20);
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_SATUROS, 0, 40);
    fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 30);
    fdActorSetSpeed(ACTOR_KRADEN, 0.5);
    fdActorWalkToAndWait(ACTOR_KRADEN, 491, 296);
    fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
    fdActorSetSpeed(ACTOR_SATUROS, 1);
    fdActorSetSpeed(ACTOR_MENARDI, 1);
    fdActorSetSpeed(ACTOR_FELIX, 1);
    fdActorSetSpeed(ACTOR_ALEX, 1);
    fdActorWalkToAndWait(ACTOR_SATUROS, 471, 308);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_SOUTH_SOUTHWEST, 0);
    fdActorWalkToAndWait(ACTOR_MENARDI, 455, 308);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_SOUTH_SOUTHEAST, 0);
    fdActorWalkToAndWait(ACTOR_ALEX, 487, 308);
    fdActorFaceDirection(ACTOR_SATUROS, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_MENARDI, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH_NORTHEAST, 0);
    fdCameraSetSpeed(0.5);
    fdCameraMoveTo(472, CAMERA_KEEP, 300, 1);
    fdCameraWaitForMove();
    fdScriptWait(40);
    gmFlagSet(FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING);
    // One by one they rise out of the chamber: Saturos, Kraden, Menardi, then Jenna ("Isaac, Garet!
    // Don't die!") and Felix, and last Alex, who regrets leaving the Mars Star.
    fdActorSetSpeed(ACTOR_SATUROS, 0.5);
    fdActorSetSpeed(ACTOR_MENARDI, 0.5);
    fdActorSetSpeed(ACTOR_ALEX, 0.5);
    fdActorSetAnimationAndWait(ACTOR_SATUROS, ANIM_NOD);
    scSolSanctumEruptionRaiseAndShrinkActor(ACTOR_SATUROS);
    fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
    scSolSanctumEruptionRaiseAndShrinkActor(ACTOR_KRADEN);
    fdActorSetAnimationAndWait(ACTOR_MENARDI, ANIM_NOD);
    scSolSanctumEruptionRaiseAndShrinkActor(ACTOR_MENARDI);
    fdActorFaceDirection(ACTOR_JENNA, FACING_WEST_NORTHWEST, 40);
    fdActorStartRepeatedMotion(ACTOR_JENNA, 2);
    fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 40);
    fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHWEST, 30);
    fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
    scSolSanctumEruptionRaiseAndShrinkActor(ACTOR_JENNA);
    scSolSanctumEruptionRaiseAndShrinkActor(ACTOR_FELIX);
    fdActorFaceDirection(ACTOR_ALEX, FACING_WEST_SOUTHWEST, 40);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 30);
    fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    fdScriptShowMessageThenWait(ACTOR_ALEX, 0, 30);
    scSolSanctumEruptionRaiseAndShrinkActor(ACTOR_ALEX);
    scSolSanctumEruptionPlayWiseOneRescue();
    fdPartyRemoveMemberRestored(JENNA);
}

// Lifts one departing actor and shrinks it together with its shadow on the platform
// (ACTOR_PLATFORM_SHADOW).
static void scSolSanctumEruptionRaiseAndShrinkActor(Sint32 actorId)
{
    ACTOR *actor;
    ACTOR *shadow;
    Uint8  frame;

    shadow         = fdActorGet(ACTOR_PLATFORM_SHADOW);
    shadow->scaleX = toFIXED(1);
    shadow->scaleY = toFIXED(1);
    fdActorWalkToAndWait(actorId, 471, 290);
    fdActorFaceDirection(actorId, FACING_NORTH, 0);
    fdScriptWait(10);
    fdActorSetPos(ACTOR_PLATFORM_SHADOW, 471, 290);
    actor = fdActorGet(actorId);
    fdActorSetSpriteFlags(fdActorGet(actorId), 0);
    fdActorSetSpriteColorById(actorId, SPRITE_COLOR_BLINK);
    actor->motionFlags = 0;
    sySoundPlay(SE_ASCEND);
    for (frame = 0; frame != 60; frame++)
    {
        actor->y.fixed += toFIXED(1) / 2;
        fdScriptWait(1);
    }
    sySoundPlay(SE_WARP);
    for (frame = 0; frame != 90; frame++)
    {
        actor->y.fixed += toFIXED(1) / 10;
        actor->scaleX  -= toFIXED(1) / 100;
        actor->scaleY  -= toFIXED(1) / 100;
        shadow->scaleX -= toFIXED(1) / 100;
        shadow->scaleY -= toFIXED(1) / 100;
        fdScriptWait(1);
    }
    fdActorSetPos(actorId, 0, 0);
    fdActorSetPos(ACTOR_PLATFORM_SHADOW, 0, 0);
}

// Unused: an empty function nothing refers to.
UNUSED static void scSolSanctumEruptionUnusedNoOperationA(void)
{
}

// Unused: an empty function nothing refers to.
UNUSED static void scSolSanctumEruptionUnusedNoOperationB(void)
{
}

// Plays the collapse after the cut from the Elemental Star Room.
Sint32 scInit(void)
{
    if (MainRoutineStateWork.entrance == SOL_SANCTUM_ERUPTION_FROM_SOL_SANCTUM_ELEMENTAL_STAR_ROOM)
    {
        fdEffectInitSparkles();
        scSolSanctumEruptionPlayHostageScript();
    }
    return 0;
}

// Keeps a flying ember sprite rotated a quarter-turn from its facing.
static Sint32 scSolSanctumEruptionAlignEmberToHeading(ACTOR *actor)
{
    actor->sprite->rotation = actor->facing + DEGtoANG(90);
    return 1;
}

// Launches sixteen crystals outward, then resets their actors for ember emission.
static void scSolSanctumEruptionThrowEruptionRing(void)
{
    ACTOR *actor;
    ACTOR *spun;
    Uint32 crystalIndex;
    Sint32 angle;
    SPR   *sprite;
    FIXED  offsetX;
    FIXED  offsetZ;

    for (crystalIndex = ACTOR_FIRST_CRYSTAL; crystalIndex < ACTOR_FIRST_CRYSTAL + CRYSTAL_COUNT;
         crystalIndex++)
    {
        actor = fdActorGet(crystalIndex);
        fdActorStop(crystalIndex);
        fdActorSetSpriteFlags(actor, 0);
        fdActorSelectAnimation(actor, BLUE_CRYSTAL_ANIMATION_GLINT);
        actor->sprite->image.priority = 0;
        actor->motionFlags            = 0;
        actor->speedLimit             = toFIXED(8);
        actor->acceleration           = toFIXED(3) / 4;
        actor->scaleX                 = toFIXED(9) / 5;
        actor->scaleY                 = toFIXED(9) / 5;
        actor->x.fixed                = toFIXED(232);
        actor->y.fixed                = toFIXED(20);
        actor->z.fixed                = toFIXED(132);
    }
    sySoundPlay(SE_CRASH);
    for (crystalIndex = 0; crystalIndex < CRYSTAL_COUNT; crystalIndex++)
    {
        spun             = fdActorGet(crystalIndex + ACTOR_FIRST_CRYSTAL);
        sprite           = spun->sprite;
        angle            = crystalIndex * FACING_STEP;
        sprite->rotation = angle - DEGtoANG(90);
        offsetX          = syMulFX(syCos(angle), toFIXED(256));
        offsetZ          = syMulFX(sySin(angle), toFIXED(256));
        syFieldActorSetMoveTargetFX(
            spun, spun->x.fixed + offsetX, spun->y.fixed, spun->z.fixed + offsetZ);
    }
    fdScriptWait(20);
    fdActorWaitForMoveById(ACTOR_FIRST_CRYSTAL);
    for (crystalIndex = ACTOR_FIRST_CRYSTAL; crystalIndex < ACTOR_FIRST_CRYSTAL + CRYSTAL_COUNT;
         crystalIndex++)
    {
        actor = fdActorGet(crystalIndex);
        fdActorStop(crystalIndex);
        actor->scaleX  = toFIXED(1);
        actor->scaleY  = toFIXED(1);
        actor->x.fixed = 0;
        actor->y.fixed = 0;
        actor->z.fixed = 0;
        SET_VELOCITY(actor, 0, 0, 0);
        actor->targetX = ACTOR_NO_TARGET;
        actor->targetY = ACTOR_NO_TARGET;
        actor->targetZ = ACTOR_NO_TARGET;
    }
}

// Resets the sixteen ember slots and their emission timers.
static void scSolSanctumEruptionResetEmberEmission(void)
{
    Uint32 emberIndex;

    EmberMask       = EMBER_EVERY(64);
    EmberTimer      = 0;
    EmberLevel      = 0;
    EmberLevelTimer = 120;
    for (emberIndex = 0; emberIndex < EMBER_COUNT; emberIndex++)
    {
        EmberState[emberIndex] = EMBER_FREE;
    }
}

// Retires landed embers and launches new ones, shaking the map.
static void scSolSanctumEruptionStepEmbers(void)
{
    WEATHER_STATE *sparkles;
    Sint32         shakePixels;
    Uint8          emberIndex;
    ACTOR         *landedEmber;
    ACTOR         *ember;
    Sint16         angle;
    FIXED          x;
    FIXED          z;

    sparkles    = WeatherStateWork;
    shakePixels = (Uint32)EmberTimer / 10;
    if (shakePixels != 0)
    {
        sparkles->stopped = 0;
        fdMapGridSetShakeFX(toFIXED(shakePixels), toFIXED(shakePixels), toFIXED(1));
    }
    else
    {
        sparkles->stopped = 1;
        fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    }
    if (EmberTimer != 0)
        EmberTimer -= 3;

    for (emberIndex = 0; emberIndex < EMBER_COUNT; emberIndex++)
    {
        if (EmberState[emberIndex] != EMBER_FREE)
        {
            landedEmber = fdActorGet(emberIndex + ACTOR_FIRST_EMBER);
            if (landedEmber->targetX == ACTOR_NO_TARGET && landedEmber->targetZ == ACTOR_NO_TARGET)
            {
                EmberState[emberIndex]++;
                if (EmberState[emberIndex] == EMBER_LANDED)
                    fdActorSelectAnimation(landedEmber, BLUE_CRYSTAL_ANIMATION_SHATTER);
                if (EmberState[emberIndex] == EMBER_PUT_AWAY)
                {
                    landedEmber->x.fixed = 0;
                    landedEmber->y.fixed = 0;
                    landedEmber->z.fixed = 0;
                    SET_VELOCITY(landedEmber, 0, 0, 0);
                    landedEmber->targetX = ACTOR_NO_TARGET;
                    landedEmber->targetY = ACTOR_NO_TARGET;
                    landedEmber->targetZ = ACTOR_NO_TARGET;
                    fdActorSetSpriteColor(landedEmber, SPRITE_COLOR_HIDDEN);
                }
                else if (EmberState[emberIndex] == EMBER_DONE)
                {
                    EmberState[emberIndex] = EMBER_FREE;
                }
            }
        }
    }

    if (EmberMask == EMBER_EMISSION_DISABLED)
        return;
    if (EmberMask & TickCount)
        return;
    for (emberIndex = 0; emberIndex < EMBER_COUNT; emberIndex++)
    {
        angle = syMathRemainderUnsigned(syRandomNext(), TRIG_TURN - 1);
        ember = fdActorGet(emberIndex + ACTOR_FIRST_EMBER);
        if (EmberState[emberIndex] == EMBER_FREE)
        {
            if (gmFlagGet(FLAG_SOL_SANCTUM_ERUPTION_SATUROS_LEAVING) == 0)
                sySoundPlay(SE_GLITTER);
            EmberState[emberIndex] = EMBER_FLYING;
            ember->motionFlags     = 0;
            ember->speedLimit      = toFIXED(8);
            ember->acceleration    = toFIXED(1);
            fdActorSetSpriteFlags(ember, 0);
            ember->sprite->image.priority = 1;
            fdActorSelectAnimation(ember, BLUE_CRYSTAL_ANIMATION_GLINT);
            fdActorStartScript(ember, EruptionEmberScript);
            ember->x.fixed =
                syMulFX(syCos((Uint16)angle), toFIXED(syRandomBelow(256)) + toFIXED(256))
                + toFIXED(325);
            ember->y.fixed = 0;
            ember->z.fixed =
                syMulFX(sySin((Uint16)angle), toFIXED(syRandomBelow(256)) + toFIXED(256))
                + toFIXED(302);
            x = syMulFX(syCos((Uint16)angle), toFIXED(syRandomNext() & 63) + toFIXED(8));
            z = syMulFX(sySin((Uint16)angle), toFIXED(syRandomNext() & 63) + toFIXED(8));
            syFieldActorSetMoveTargetFX(ember, x + toFIXED(325), 0, z + toFIXED(286));
            fdActorSetSpriteColor(ember, SPRITE_COLOR_NORMAL);
            EmberTimer = 30;
            return;
        }
    }
}

// Varies the ember emission interval and its duration.
static void scSolSanctumEruptionUpdateEmberFrequency(void)
{
    Uint32 emissionLevel;

    if (EmberLevelTimer != 0)
    {
        EmberLevelTimer--;
        return;
    }
    if (EmberLevel != 0)
    {
        EmberLevel--;
    }
    else
    {
        EmberLevel = syRandomBelow(4);
    }
    emissionLevel = EmberLevel;
    switch (emissionLevel)
    {
        case 3:
            EmberMask       = EMBER_EVERY(4);
            EmberLevelTimer = (syRandomBelow(20)) + 40;
            break;
        case 2:
            EmberMask       = EMBER_EVERY(16);
            EmberLevelTimer = (syRandomBelow(40)) + 80;
            break;
        case 1:
            EmberMask       = EMBER_EVERY(64);
            EmberLevelTimer = (syRandomBelow(80)) + 160;
            break;
        default:
            EmberMask       = EMBER_EVERY(128);
            EmberLevelTimer = (syRandomBelow(160)) + 320;
            break;
    }
}

// Ends the Wise One's palette animation and clears its attached effect.
static inline void scSolSanctumEruptionStopWiseOnePalette(void)
{
    fdActorSetAnimationAndWait(ACTOR_WISE_ONE, WISE_ONE_ANIMATION_RAISE_EYE);
    syCloseEvent(scSolSanctumEruptionUpdateWiseOnePalette);
    sySynch(1);
    fdActorSetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
}

// Elemental Star Room, collapsing: the Wise One sends Isaac and Garet out.
static void scSolSanctumEruptionPlayWiseOneRescue(void)
{
    WEATHER_STATE *sparkles;
    ACTOR         *wiseOne;
    ACTOR         *center;
    ACTOR         *leader;
    ACTOR         *starOrigin;
    ACTOR         *garet;
    SPR           *leaderSprite;
    SPR           *garetSprite;
    ACTOR         *star;
    SPR           *starSprite;
    ICON_TRANSFER *glyph;
    Uint8          crystalIndex;
    Uint32         frame;
    Sint32         flickerDelay;

    sparkles = WeatherStateWork;
    wiseOne  = fdActorGet(ACTOR_WISE_ONE);
    syCloseEvent(scSolSanctumEruptionUpdateEmberFrequency);
    EmberMask = EMBER_EVERY(4);
    fdScriptWait(80);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(40);
    fdScriptWait(40);
    syCloseEvent(scSolSanctumEruptionStepEmbers);
    for (crystalIndex = 0; crystalIndex < CRYSTAL_COUNT; crystalIndex++)
    {
        fdActorSetPos(ACTOR_FIRST_CRYSTAL + crystalIndex, 0, 0);
    }
    sySynch(1);
    fdActorSetPos(ACTOR_WISE_ONE, 0, 0);
    scSolSanctumEruptionPlaceBeforeStar(ACTOR_PARTY_LEADER);
    scSolSanctumEruptionPlaceBeforeStar(ACTOR_GARET);
    sparkles->stopped = 0;
    fdMapGridSetShake(1, 1, 1);
    fdScriptWait(80);
    scSolSanctumEruptionClearStatueMetatiles();
    center              = fdCameraGetActor();
    center->motionFlags = 0;
    center->x.fixed     = toFIXED(231);
    center->z.fixed     = toFIXED(144);
    center->targetX     = ACTOR_NO_TARGET;
    center->targetY     = ACTOR_NO_TARGET;
    center->targetZ     = ACTOR_NO_TARGET;
    center->y.fixed     = 0;
    center->velocityX   = 0;
    center->velocityZ   = 0;
    sySynch(4);
    fdMapGridCenterOnFollowedPoint();
    sySynch(4);
    fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
    fdActorSetSpritePriority(ACTOR_GARET, 3);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(40);
    fdScriptWait(40);

    leader       = fdActorGet(ACTOR_PARTY_LEADER);
    garet        = fdActorGet(ACTOR_GARET);
    leaderSprite = leader->sprite;
    garetSprite  = garet->sprite;
    for (frame = 0; frame < 20; frame++)
    {
        leaderSprite->rotation += TRIG_TURN / 256;
        garetSprite->rotation  -= TRIG_TURN / 256;
        leader->x.fixed        += toFIXED(3) / 8;
        garet->x.fixed         -= toFIXED(3) / 8;
        sySynch(1);
    }
    fdScriptWait(40);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
    fdActorSetSpeed(ACTOR_GARET, 2);
    fdActorGet(ACTOR_PARTY_LEADER)->sprite->rotation = 0;
    fdActorGet(ACTOR_GARET)->sprite->rotation        = 0;
    fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
    fdActorJump(ACTOR_GARET, 6, 0);
    fdActorSetDestination(ACTOR_PARTY_LEADER, 246, 150);
    fdActorMoveToAndWait(ACTOR_GARET, 220, 150);
    fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    fdActorSetSpritePriority(ACTOR_GARET, 2);
    fdActorGet(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    fdActorGet(ACTOR_GARET)->priorityFlags        |= ACTOR_PRIORITY_AUTOMATIC;
    fdActorStop(ACTOR_PARTY_LEADER);
    fdActorStop(ACTOR_GARET);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_NORTHWEST, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    fdScriptWait(80);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
    fdScriptWait(60);

    // Garet: "Well, this turned out badly, wouldn't you say?" Yes: "It's a miracle we survived."
    // No: "How can you be so calm?"
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptSetMessageId(MsgSolSanctumWellTurnedBadly);
    // "Well, this turned out badly, wouldn't you say?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
        fdScriptSetMessageId(MsgSolSanctumWellTurnedBadly + 1);
    }
    else
    {
        fdActorRunRepeatedMotion(ACTOR_GARET, 1);
        fdScriptSetMessageId(MsgSolSanctumWellTurnedBadly + 2);
    }
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 60);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(40);
    fdActorJump(ACTOR_GARET, 2, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(40);
    fdScriptSetMessageId(MsgSolSanctumFriendsGone);
    // "Jenna and Kraden are gone!"
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    fdScriptWait(60);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(80);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    // Garet: "You don't think they'll hurt Jenna and Kraden, do you?" Either way, he trusts Felix
    // to look after them. "You don't think they'll hurt Jenna and Kraden, do you?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(10);
        fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
        fdScriptWait(60);
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
        fdScriptWait(10);
        fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
        fdScriptSetMessageId(MsgSolSanctumThanksALot);
    }
    else
    {
        fdScriptWait(10);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(10);
        fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
        fdScriptWait(60);
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
        fdScriptWait(10);
        fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
        fdScriptSetMessageId(MsgSolSanctumTheyllBeSafe);
    }
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 40);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    fdScriptWait(40);

    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 0);
    fdCameraSetSpeed(2);
    fdCameraMoveTo(284, CAMERA_KEEP, 92, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    fdCameraSetSpeed(1.5);
    fdCameraMoveTo(127, CAMERA_KEEP, 162, 1);
    fdCameraWaitForMove();
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdCameraSetSpeed(4);
    fdCameraMoveTo(304, CAMERA_KEEP, 294, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
    fdCameraSetSpeed(1);
    fdCameraMoveTo(400, CAMERA_KEEP, 215, 1);
    fdCameraWaitForMove();
    fdScriptWait(60);
    fdCameraSetSpeed(4);
    fdCameraMoveTo(273, CAMERA_KEEP, 145, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    // Garet: "This really is terrible..." and they have a long way to go.
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptSetMessageId(MsgSolSanctumThisIsTerrible);
    // "This really is terrible..."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST_NORTHEAST, 0);
    fdScriptWait(10);
    // "We've got a long way to go if we're gonna make it out of here."
    fdScriptShowMessageThenWait(ACTOR_GARET, 0, 10);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(10);

    // A violent tremor throws them aside.
    sySoundPlay(MUS_DUNGEON_THEME);
    scSolSanctumEruptionSetPaleTint(1, 4, 0);
    sparkles->stopped = 0;
    fdMapGridSetShake(5, 5, 1);
    fdScriptWait(10);
    scSolSanctumEruptionSetPaleTint(0, 40, 0);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
    fdActorSetSpeed(ACTOR_GARET, 2);
    fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
    fdActorJump(ACTOR_GARET, 6, 0);
    fdActorSetDestination(ACTOR_PARTY_LEADER, 243, 144);
    fdActorSetDestination(ACTOR_GARET, 202, 144);
    fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
    fdScriptWait(20);
    sparkles->stopped = 1;
    fdMapGridSetShake(1, 1, 1);
    fdScriptWait(60);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(80);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
    fdActorSetSpeed(ACTOR_GARET, 0.4);
    fdActorWalkTo(ACTOR_GARET, 220, 150);
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 246, 150);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 0);
    fdScriptWait(60);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(40);
    // Garet: "We're in big trouble..." Yes: "Let's get out of here..." No: "Quit acting so tough!"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
        fdScriptWait(10);
        fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    }
    else
    {
        fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
        fdScriptWait(10);
        fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
        fdScriptSetMessageId(MsgSolSanctumQuitActingTough);
    }
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_GARET, 0);

    // The Wise One drifts in from the west, unseen by Isaac; Garet walks off and calls him: "I'm
    // over here. Come on, we've gotta go."
    fdActorSetPos(ACTOR_WISE_ONE, 110, 152);
    sySynch(1);
    fdActorSetSpeed(ACTOR_WISE_ONE, 1.2);
    fdActorSetDestination(ACTOR_WISE_ONE, 171, 152);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_EAST, 0);
    fdActorSetSpriteFlags(fdActorGet(ACTOR_WISE_ONE), SPRITE_FLAG_SHADOW);
    fdActorSetSpeed(ACTOR_GARET, 1);
    fdActorWalkTo(ACTOR_GARET, 217, 182);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_SOUTHWEST, 0);
    fdScriptWait(10);
    fdCameraSetSpeed(2);
    fdCameraMoveTo(217, CAMERA_KEEP, 176, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(20);
    fdActorJump(ACTOR_PARTY_LEADER, 2, 0);
    fdScriptWait(10);
    fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
    fdScriptWait(4);
    fdActorWalkToAndWait(ACTOR_GARET, 231, 175);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdScriptSetMessageId(MsgSolSanctumOverHere);
    // "Hey, Isaac... Isaac, I'm over here. Come on, we've gotta go."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(40);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
    fdScriptWait(60);
    fdActorStartRepeatedMotion(ACTOR_GARET, 1);
    fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    fdScriptWait(40);

    scSolSanctumEruptionSetPaleTint(1, 20, 0);
    sparkles->stopped = 0;
    fdMapGridSetShake(1, 1, 1);
    fdScriptWait(40);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorStartRepeatedMotion(ACTOR_GARET, 2);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    fdPaletteFadeStartInterpolation(40);
    sparkles->stopped = 1;
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdScriptWait(120);
    // The Wise One: "Why are you standing there?" and "This place has become dangerous..." A jolt;
    // Garet: "You're the dangerous one!"
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(100);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(20);
    scSolSanctumEruptionSetPaleTint(1, 10, 0);
    sparkles->stopped = 0;
    fdMapGridSetShake(2, 2, 1);
    fdScriptWait(20);
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
    fdActorSetSpeed(ACTOR_GARET, 2);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_GARET)->steeringFlags        &= ~ACTOR_FACES_MOTION;
    fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
    fdActorJump(ACTOR_GARET, 4, 0);
    fdActorSetDestination(ACTOR_PARTY_LEADER, 256, 150);
    fdActorSetDestination(ACTOR_GARET, 231, 180);
    fdActorWaitForMoveById(ACTOR_GARET);
    scSolSanctumEruptionSetPaleTint(0, 40, 0);
    fdScriptWait(20);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_GARET)->steeringFlags        |= ACTOR_FACES_MOTION;
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(40);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    fdScriptWait(40);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(20);
    fdActorShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
    fdScriptWait(10);

    // The floor by the Wise One starts to break up; the Wise One glows and holds it together with
    // Psynergy.
    sySoundPlay(SE_TREMOR);
    sySetTickEvent(scSolSanctumEruptionFlickerFirstCollapseTiles, EVENT_PASS_TICK);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_WISE_ONE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    fdScriptWait(40);
    fdCameraSetSpeed(1);
    fdCameraMoveTo(186, CAMERA_KEEP, 166, 1);
    fdActorSetDestination(ACTOR_WISE_ONE, 130, 113);
    fdActorWaitForMoveById(ACTOR_WISE_ONE);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(20);
    sparkles->stopped = 0;
    fdMapGridSetShake(2, 2, 1);
    fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
    fdPaletteFadeStartInterpolation(20);
    fdScriptWait(20);
    scStartScriptPsynergy();
    fdActorSetAnimationAndWait(ACTOR_WISE_ONE, WISE_ONE_ANIMATION_LOWER_EYE);
    for (frame = 0; frame < 40; frame++)
    {
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
    }
    sySetTickEvent(scSolSanctumEruptionUpdateWiseOnePalette, EVENT_PASS_TICK);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    fdPaletteFadeStartInterpolation(60);
    fdScriptWait(30);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    syCloseEvent(scSolSanctumEruptionFlickerFirstCollapseTiles);
    fdMapGridSetShake(1, 1, 1);
    flickerDelay = 16;
    while (flickerDelay--)
    {
        scSolSanctumEruptionDrawFirstCollapseTiles(0);
        sySynch(flickerDelay);
        scSolSanctumEruptionDrawFirstCollapseTiles(1);
        sySynch(flickerDelay);
    }
    scSolSanctumEruptionDrawFirstCollapseTiles(0);
    sparkles->stopped = 1;
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdMapGridWaitForShakeToSettle();
    scSolSanctumEruptionStopWiseOnePalette();
    fdScriptWait(60);
    // The Wise One: "The volcano will soon erupt... I cannot hold it long." and, after drifting
    // closer, "You must leave this place."
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(40);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    fdCameraSetSpeed(1);
    fdCameraMoveTo(218, CAMERA_KEEP, 181, 1);
    fdActorSetSpeed(ACTOR_WISE_ONE, 1);
    fdActorSetDestination(ACTOR_WISE_ONE, 169, 151);
    fdActorWaitForMoveById(ACTOR_WISE_ONE);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_EAST_SOUTHEAST, 0);
    fdScriptWait(40);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
    fdScriptWait(40);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_EAST_NORTHEAST, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST_SOUTHWEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
    fdCameraMoveTo(224, CAMERA_KEEP, 158, 1);
    fdCameraWaitForMove();
    // Lights rise at the chamber's first corner. The Wise One: the Elemental Djinn have returned;
    // "Take out the Elemental Star..." Refusing, "Isaac cannot resist."
    scSolSanctumEruptionRaiseElementalLights(CHAMBER_NORTHWEST);
    fdActorRunRepeatedMotion(ACTOR_WISE_ONE, 2);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(40);
    fdScriptOpenMessage(ACTOR_WISE_ONE, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(40);
    }
    else
    {
        mnFieldMessageShow(MsgSolSanctumCannotResist, 1);
        fdScriptWait(40);
    }

    starOrigin = fdActorGet(ACTOR_PARTY_LEADER);
    star       = syFieldActorCreate(
        SPRITE_BLANK, starOrigin->x.fixed, starOrigin->y.fixed + toFIXED(36), starOrigin->z.fixed);
    if (star != NULL)
    {
        glyph             = syHeapSlotAllocate(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
        starSprite        = star->sprite;
        starSprite->flags = 0;
        starSprite->count = 0;
        starSprite->image.fullColor = 0;
        starSprite->image.palette   = 0;
        starSprite->image.priority  = 1;
        mnIconDrawItem(ITEM_MARS_STAR);
        syVRAMCacheLoad(starSprite->slot, ICON_TILE_BYTES, glyph->tiles);
        syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
        fdItemDisplayShowPicture(star, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
        fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
    }
    scSolSanctumEruptionSetPaleTint(1, 20, 0);
    for (frame = 0; frame < 24; frame++)
    {
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
        star->scaleX = toFIXED(2) / 5;
        star->scaleY = toFIXED(2) / 5;
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
        star->scaleX = toFIXED(1);
        star->scaleY = toFIXED(1);
    }
    fdActorSetSpriteColorById(ACTOR_WISE_ONE, SPRITE_COLOR_NORMAL);
    scSolSanctumEruptionSetPaleTint(0, 20, 0);
    // The Wise One: "Return it to its bag." and the Elemental Stars are moving away from here.
    fdScriptSetMessageId(MsgSolSanctumReturnStarToBag);
    // "Return it to its bag."
    fdScriptShowMessageThenWait(ACTOR_WISE_ONE, 0, 20);
    if (star != NULL)
    {
        fdActorRelease(star);
    }
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHWEST, 60);
    // "The Elemental Stars are... moving away from here..."
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(40);

    // The camera visits the other three corners as lights rise there. The Wise One: the magma flows
    // freely, there will be no chamber for the Stars to return to, and the world is exposed to
    // Alchemy. Garet: "Alchemy? A threat?"
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    fdCameraSetSpeed(4);
    scSolSanctumEruptionFadeCameraTo(232, 464);
    scSolSanctumEruptionRaiseElementalLights(CHAMBER_SOUTHWEST);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    scSolSanctumEruptionFadeCameraTo(711, 144);
    scSolSanctumEruptionRaiseElementalLights(CHAMBER_NORTHEAST);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    scSolSanctumEruptionFadeCameraTo(711, 464);
    scSolSanctumEruptionRaiseElementalLights(CHAMBER_SOUTHEAST);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_EAST_SOUTHEAST, 0);
    fdActorSetPos(ACTOR_GARET, 582, 345);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdActorSetPos(ACTOR_GARET, 231, 180);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHWEST, 0);
    sySynch(20);
    // Back with the party, the Wise One: misused, Alchemy is dangerous; its power is released if
    // the Stars light the four lighthouses.
    scSolSanctumEruptionFadeCameraTo(219, 171);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(10);
    fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(40);

    sparkles->stopped = 0;
    fdMapGridSetShake(4, 4, 1);
    fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
    fdPaletteFadeStartInterpolation(20);
    fdScriptWait(20);
    // The floor breaks up again in two places; the Wise One floats to each and holds it together.
    sySoundPlay(SE_TREMOR);
    sySetTickEvent(scSolSanctumEruptionFlickerSecondCollapseTiles, EVENT_PASS_TICK);
    fdScriptWait(20);
    fdCameraSetSpeed(1);
    fdCameraMoveTo(184, CAMERA_KEEP, 132, 1);
    fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
    fdActorJump(ACTOR_GARET, 6, 0);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_GARET)->steeringFlags        &= ~ACTOR_FACES_MOTION;
    fdActorSetDestination(ACTOR_PARTY_LEADER, 245, 145);
    fdActorSetDestination(ACTOR_GARET, 215, 168);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdScriptWait(80);
    fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    fdActorGet(ACTOR_GARET)->steeringFlags        |= ACTOR_FACES_MOTION;
    fdActorSetDestination(ACTOR_WISE_ONE, 184, 87);
    fdActorWaitForMoveById(ACTOR_WISE_ONE);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_WISE_ONE, WISE_ONE_ANIMATION_LOWER_EYE);
    for (frame = 0; frame < 40; frame++)
    {
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
    }
    sySetTickEvent(scSolSanctumEruptionUpdateWiseOnePalette, EVENT_PASS_TICK);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    fdPaletteFadeStartInterpolation(60);
    fdScriptWait(30);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    syCloseEvent(scSolSanctumEruptionFlickerSecondCollapseTiles);
    fdMapGridSetShake(2, 2, 1);
    flickerDelay = 8;
    while (flickerDelay--)
    {
        scSolSanctumEruptionDrawSecondCollapseTiles(0);
        sySynch(flickerDelay);
        scSolSanctumEruptionDrawSecondCollapseTiles(1);
        sySynch(flickerDelay);
    }
    scSolSanctumEruptionDrawSecondCollapseTiles(0);
    fdMapGridSetShake(1, 1, 1);
    scSolSanctumEruptionStopWiseOnePalette();
    fdScriptWait(60);

    fdMapGridSetShake(4, 4, 1);
    fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
    fdPaletteFadeStartInterpolation(20);
    fdScriptWait(20);
    sySoundPlay(SE_TREMOR);
    sySetTickEvent(scSolSanctumEruptionFlickerFirstCollapseTiles, EVENT_PASS_TICK);
    fdScriptWait(40);
    fdActorMoveToAndWait(ACTOR_WISE_ONE, 127, 110);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_WISE_ONE, WISE_ONE_ANIMATION_LOWER_EYE);
    for (frame = 0; frame < 40; frame++)
    {
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
    }
    sySetTickEvent(scSolSanctumEruptionUpdateWiseOnePalette, EVENT_PASS_TICK);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    fdPaletteFadeStartInterpolation(60);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdScriptWait(30);
    syCloseEvent(scSolSanctumEruptionFlickerFirstCollapseTiles);
    fdMapGridSetShake(2, 2, 1);
    flickerDelay = 8;
    while (flickerDelay--)
    {
        scSolSanctumEruptionDrawFirstCollapseTiles(0);
        sySynch(flickerDelay);
        scSolSanctumEruptionDrawFirstCollapseTiles(1);
        sySynch(flickerDelay);
    }
    scSolSanctumEruptionDrawFirstCollapseTiles(0);
    fdMapGridSetShake(1, 1, 1);
    sySoundPlay(SE_TREMOR);
    sySoundPlay(MUS_MOUNT_ALEPH_ERUPTS);
    fdMapGridSetShake(4, 4, 1);
    fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(30, 12, 4), PALETTES_BG);
    fdPaletteFadeStartInterpolation(20);
    fdScriptWait(20);
    sySoundPlay(SE_TREMOR);
    sySetTickEvent(scSolSanctumEruptionFlickerSecondCollapseTiles, EVENT_PASS_TICK);
    scSolSanctumEruptionStopWiseOnePalette();
    fdScriptWait(60);
    fdActorMoveToAndWait(ACTOR_WISE_ONE, 184, 87);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_SOUTH, 0);
    fdScriptWait(10);
    scSolSanctumEruptionStopWiseOnePalette();
    sySoundPlay(SE_RUMBLE);
    fdScriptWait(100);
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    fdScriptWait(60);
    // The Wise One: "It's erupting... Flee, now!" Garet: "But how..." The Wise One: "I will help
    // you..." It glows and the chamber fades to white.
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdActorRunRepeatedMotion(ACTOR_GARET, 3);
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(20);
    sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
    fdActorFaceDirection(ACTOR_WISE_ONE, FACING_SOUTH_SOUTHEAST, 0);
    fdScriptWait(20);
    fdScriptShowMessage(ACTOR_WISE_ONE, 0);
    fdScriptWait(20);
    for (frame = 0; frame < 40; frame++)
    {
        scSolSanctumEruptionFlashWiseOne(wiseOne);
        sySynch(1);
    }
    sySetTickEvent(scSolSanctumEruptionUpdateWiseOnePalette, EVENT_PASS_TICK);
    fdScriptWait(20);
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_SPRITES);
    fdPaletteFadeStartInterpolation(60);
    sySynch(100);
    fdPaletteFadeApplyTarget(RGB_WHITE, PALETTES_BG);
    fdPaletteFadeStartInterpolation(60);
    sySynch(60);
    syCloseEvent(scSolSanctumEruptionUpdateWiseOnePalette);
    sparkles->stopped = 1;
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    fdMapGridWaitForShakeToSettle();
    scEndScriptPsynergy();
    gmFlagSet(FLAG_SOL_SANCTUM_ERUPTED);
    gmFlagSet(FLAG_SOL_SANCTUM_STAR_ROOM_COLLAPSED);
    syImportScriptSetExitRequest(SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS);
    // XXX (no evidence): what flag 0x100 records; nothing else sets it and nothing in the game
    // reads it.
    gmFlagSet(0x100);
}

// Places an actor before the star, facing south, and draws it above the chamber floor.
static void scSolSanctumEruptionPlaceBeforeStar(Sint32 actorId)
{
    ACTOR *actor;

    actor = fdActorGet(actorId);
    fdActorSetPos(actorId, 232, 125);
    actor->facing = FACING_SOUTH;
    fdActorSetSpritePriority(actorId, 3);
}

// Draws the patch of floor that breaks up first, by the Wise One, broken or whole. The patch is on
// map layer 2, whose metatiles start at the map's (56, 0): its metatiles (7, 7) to (8, 9), under
// where the Wise One floats. Broken takes the metatiles layer 1 keeps at the same place (from the
// map's (0, 40)); whole takes layer 2's own spare metatiles.
static void scSolSanctumEruptionDrawFirstCollapseTiles(Sint32 broken)
{
    if (broken != 0)
    {
        Sint32 metatileSize;
        metatileSize = 1;
        fdMapGridCopyMetatilesTo(8, 47, 64, 7, metatileSize, metatileSize);
        fdMapGridCopyMetatilesTo(7, 48, 63, 8, 2, metatileSize);
        fdMapGridCopyMetatilesTo(7, 49, 63, 9, 2, metatileSize);
    }
    else
    {
        Sint32 metatileSize;
        metatileSize = 1;
        fdMapGridCopyMetatilesTo(56, 0, 64, 7, metatileSize, metatileSize);
        fdMapGridCopyMetatilesTo(56, 0, 63, 8, metatileSize, metatileSize);
        fdMapGridCopyMetatilesTo(56, 0, 63, 9, 2, metatileSize);
        fdMapGridCopyMetatilesTo(58, 25, 64, 8, metatileSize, metatileSize);
    }
    fdMapGridCenterOnFollowedPoint();
}

// Draws the patch of floor that breaks up second, broken or whole: layer 2's metatiles (9, 5) to
// (11, 7), north east of the first patch, again from layer 1's metatiles at the same place or from
// layer 2's spare metatiles.
static void scSolSanctumEruptionDrawSecondCollapseTiles(Sint32 broken)
{
    if (broken != 0)
    {
        Sint32 patchSize;
        patchSize = 2;
        fdMapGridCopyMetatilesTo(9, 45, 65, 5, patchSize, patchSize);
        fdMapGridCopyMetatilesTo(11, 46, 67, 6, 1, patchSize);
    }
    else
    {
        Sint32 patchSize;
        patchSize = 2;
        fdMapGridCopyMetatilesTo(89, 2, 65, 5, patchSize, patchSize);
        fdMapGridCopyMetatilesTo(102, 32, 67, 6, 1, patchSize);
    }
    fdMapGridCenterOnFollowedPoint();
}

// The first patch of floor breaking up: every other tick it is drawn broken or whole at random.
static void scSolSanctumEruptionFlickerFirstCollapseTiles(void)
{
    if ((TickCount & 1) == 0)
    {
        if (syMathRemainderUnsigned(syRandomNext(), 100) > 50)
        {
            scSolSanctumEruptionDrawFirstCollapseTiles(1);
        }
        else
        {
            scSolSanctumEruptionDrawFirstCollapseTiles(0);
        }
    }
}

// The second patch of floor breaking up, as the first.
static void scSolSanctumEruptionFlickerSecondCollapseTiles(void)
{
    if ((TickCount & 1) == 0)
    {
        if (syMathRemainderUnsigned(syRandomNext(), 100) > 50)
        {
            scSolSanctumEruptionDrawSecondCollapseTiles(1);
        }
        else
        {
            scSolSanctumEruptionDrawSecondCollapseTiles(0);
        }
    }
}

// Tints the chamber and raises ten lights, one after another, at one of its four corners, then
// waits until the last has risen out of view.
static void scSolSanctumEruptionRaiseElementalLights(Uint32 corner)
{
    Uint32  lightIndex, clear;
    FIXED   x;
    Sint32  y;
    FIXED   z;
    Sint32 *origin;
    ACTOR  *actor;
    SPR    *sprite;

    for (lightIndex = 0; lightIndex < CRYSTAL_COUNT; lightIndex++)
        fdActorReleaseById(lightIndex + ACTOR_FIRST_CRYSTAL);
    switch (corner)
    {
        case CHAMBER_NORTHWEST:
            fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(18, 14, 14), PALETTES_BG);
            break;
        case CHAMBER_SOUTHWEST:
            fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(18, 14, 18), PALETTES_BG);
            break;
        case CHAMBER_NORTHEAST:
            fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(14, 18, 18), PALETTES_BG);
            break;
        case CHAMBER_SOUTHEAST:
            fdPaletteFadeApplyTarget(PALETTE_TINT_BLEND | RGB(18, 18, 14), PALETTES_BG);
            break;
    }
    fdPaletteFadeStartInterpolation(60);
    sySoundPlay(SE_RISING_CHIMES);
    lightIndex = 0;
    clear      = lightIndex;
    for (origin = &EruptionRisingLightOrigins[0]; lightIndex < RISING_LIGHT_COUNT;
         lightIndex++, origin += 2)
    {
        x = origin[0];
        y = origin[1];
        z = 0;
        switch (corner)
        {
            case CHAMBER_NORTHWEST:
                x += toFIXED(232);
                z  = toFIXED(144);
                break;
            case CHAMBER_SOUTHWEST:
                x += toFIXED(232);
                z  = toFIXED(464);
                break;
            case CHAMBER_NORTHEAST:
                x += toFIXED(711);
                z  = toFIXED(144);
                break;
            case CHAMBER_SOUTHEAST:
                x += toFIXED(711);
                z  = toFIXED(464);
                break;
        }
        RisingLightTimers[lightIndex] = clear;
        actor                         = syFieldActorCreate(SPRITE_PARTICLE, x, y, z);
        RisingLights[lightIndex]      = actor;
        actor->motionFlags            = clear;
        sprite                        = actor->sprite;
        sprite->flags                 = clear;
        sprite->image.priority        = 1;
        fdActorSelectAnimation(actor, 6);
        sySynch(6);
    }
    if (corner == CHAMBER_NORTHWEST)
    {
        fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    }
    sySynch(20);
    sySetTickEvent(scSolSanctumEruptionUpdateElementalLights, EVENT_PASS_TICK);
    sySoundPlay(SE_GLITTER);
    // The lights rise in turn, six ticks apart.
    RisingLightTimers[0] = 1;
    sySynch(6);
    RisingLightTimers[1] = 1;
    sySynch(6);
    RisingLightTimers[2] = 1;
    sySynch(6);
    RisingLightTimers[3] = 1;
    sySynch(6);
    RisingLightTimers[4] = 1;
    sySynch(6);
    RisingLightTimers[5] = 1;
    sySynch(6);
    RisingLightTimers[6] = 1;
    sySynch(6);
    RisingLightTimers[7] = 1;
    sySynch(6);
    RisingLightTimers[8] = 1;
    sySynch(6);
    RisingLightTimers[9] = 1;
    sySynch(6);
    for (;;)
    {
        for (lightIndex = 0; lightIndex < RISING_LIGHT_COUNT; lightIndex++)
        {
            if (RisingLightTimers[lightIndex] != 0)
            {
                lightIndex = LIGHT_SCAN_STILL_ACTIVE;
                break;
            }
        }
        if (lightIndex != LIGHT_SCAN_STILL_ACTIVE)
            break;
        sySynch(1);
    }
    sySynch(40);
    syCloseEvent(scSolSanctumEruptionUpdateElementalLights);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    fdPaletteFadeStartInterpolation(40);
}

// Stretches each active light for ELEMENTAL_LIGHT_STRETCH_TICKS, then lifts it out of view.
static void scSolSanctumEruptionUpdateElementalLights(void)
{
    Uint32 lightIndex;
    Uint32 age;
    ACTOR *actor;

    for (lightIndex = 0; lightIndex < RISING_LIGHT_COUNT; lightIndex++)
    {
        age = RisingLightTimers[lightIndex];
        if (age != 0)
        {
            actor = RisingLights[lightIndex];
            if (age <= ELEMENTAL_LIGHT_STRETCH_TICKS)
            {
                actor->scaleX  -= ELEMENTAL_LIGHT_SCALE_X_DECREASE;
                actor->scaleY  += toFIXED(1) / 2;
                actor->y.fixed += toFIXED(3) / 10;
                actor->targetY += toFIXED(3) / 10;
            }
            else
            {
                actor->y.fixed += toFIXED(20);
                actor->targetY += toFIXED(20);
            }
            age                           = RisingLightTimers[lightIndex] + 1;
            RisingLightTimers[lightIndex] = age;
            if (age > ELEMENTAL_LIGHT_TICKS)
            {
                RisingLightTimers[lightIndex] = 0;
            }
        }
    }
}

// Fades out, moves the camera and its focus actor, then fades back in.
static void scSolSanctumEruptionFadeCameraTo(Sint32 x, Sint32 z)
{
    ACTOR *center;

    center  = fdCameraGetActor();
    z      *= toFIXED(1);
    x      *= toFIXED(1);
    fdCameraMoveWithinMapBounds(x, CAMERA_KEEP, z, 1);
    fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
    fdPaletteFadeStartInterpolation(20);
    sySynch(40);
    center->x.fixed   = x;
    center->z.fixed   = z;
    center->targetX   = ACTOR_NO_TARGET;
    center->targetZ   = ACTOR_NO_TARGET;
    center->velocityX = 0;
    center->velocityZ = 0;
    sySynch(5);
    fdMapGridCenterOnFollowedPoint();
    sySynch(5);
    fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    fdPaletteFadeStartInterpolation(20);
    sySynch(30);
}

// Makes the metatiles of the four statues at the chamber's corners plain floor, taking the metatile
// behavior and flags of the floor west of each, once the statues' metatile behaviors are saved in
// metatiles 10 to 13 of the map's top row, which nothing reads back.
static void scSolSanctumEruptionClearStatueMetatiles(void)
{
    // Mars (north west), Jupiter (south west), Mercury (north east) and Venus (south east), as
    // sol_sanctum_elemental_star_room.c places them.
    fdMapGridCopyMetatileAttributes(14, 8, 1, 1, 10, 0);
    fdMapGridCopyMetatileAttributes(14, 28, 1, 1, 11, 0);
    fdMapGridCopyMetatileAttributes(44, 8, 1, 1, 12, 0);
    fdMapGridCopyMetatileAttributes(44, 28, 1, 1, 13, 0);
    fdMapGridCopyMetatileAttributes(13, 8, 1, 1, 14, 8);
    fdMapGridCopyMetatileAttributes(13, 28, 1, 1, 14, 28);
    fdMapGridCopyMetatileAttributes(43, 8, 1, 1, 44, 8);
    fdMapGridCopyMetatileAttributes(43, 28, 1, 1, 44, 28);
}

// Fades the backgrounds into or out of the pale tint the room flashes.
static void scSolSanctumEruptionSetPaleTint(Sint32 enabled, Sint32 ticks, Sint32 waitTicks)
{
    if (enabled == 1)
    {
        sySoundPlay(SE_MATERIALIZE);
        fdPaletteFadeApplyTarget(PALETTE_TINT_CUSTOM | RGB(18, 18, 14), PALETTES_BG);
    }
    else
    {
        sySoundPlay(SOUND_FADE_OUT_ACTION_SE);
        fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
    }
    fdPaletteFadeStartInterpolation(ticks);
    if (waitTicks != 0)
    {
        fdScriptWait(waitTicks);
    }
}

// Makes the Wise One flash white as it uses Psynergy, golden rings rising round it every sixteen
// ticks.
static void scSolSanctumEruptionFlashWiseOne(ACTOR *actor)
{
    if ((TickCount & 2) != 0)
    {
        fdActorSetColorMode(actor, SPRITE_COLOR_WHITE);
    }
    else
    {
        fdActorSetColorMode(actor, SPRITE_COLOR_NORMAL);
    }
    if ((TickCount & 15) == 0)
    {
        scSolSanctumEruptionSpawnEffectPair(actor);
    }
}

// Cycles the actor through its colours swapped six ways as it glows, golden rings rising round it
// every sixteen ticks.
static void scSolSanctumEruptionCycleActorPalette(ACTOR *actor)
{
    if (TickCount & 1)
    {
        fdActorSetColorMode(actor, syMathRemainderUnsigned(TickCount >> 1, 6));
    }
    if ((TickCount & 15) == 0)
    {
        scSolSanctumEruptionSpawnEffectPair(actor);
    }
}

#include "shared/field_effect_vertical.c"

// Sends golden rings up round the Wise One, to SE_ERUPTION_COLUMNS: two halves that rise together,
// one in front of it and one mirrored behind.
static void scSolSanctumEruptionSpawnEffectPair(ACTOR *parent)
{
    ACTOR              *pair[2];
    ACTOR              *child;
    SPR                *sprite;
    FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
    Sint32              effectIndex;

    sySoundPlay(SE_ERUPTION_COLUMNS);
    for (effectIndex = 0; effectIndex < 2; ++effectIndex)
    {
        child = syFieldActorCreate(
            SPRITE_GOLDEN_RINGS, parent->x.fixed, parent->y.fixed, parent->z.fixed);
        pair[effectIndex] = child;
        if (child != NULL)
        {
            child->groundHeight = parent->groundHeight;
            sprite              = child->sprite;
            child->motionFlags  = 0;
            child->timer        = 0;
            child->watchedActor = (ACTOR_OR_EFFECT *)parent;
            if (sprite != NULL)
            {
                fdSpriteStartAnimation(sprite, 0);
                sprite->flags = 0;
                syVRAMCacheFreeEntry(sprite->slot);
                sprite->slot             = effectState->tileSlot;
                sprite->retainCacheEntry = 1;
                sprite->image.tile = (VramCacheEntries[sprite->slot].offset >> 5) & OBJ_TILE_MASK;
                sprite->image.fullColor     = 0;
                sprite->image.shape         = RING_HALF_SHAPE;
                sprite->image.size          = RING_HALF_SIZE;
                sprite->entries[0]->picture = 0;
            }
        }
    }
    pair[0]->update                 = scFieldEffectRiseRingFrontHalf;
    pair[0]->sprite->image.priority = fdActorGet(ACTOR_WISE_ONE)->sprite->image.priority;
    {
        ACTOR *wiseOne                       = fdActorGet(ACTOR_WISE_ONE);
        ACTOR *risingEffect                  = pair[1];

        risingEffect->sprite->image.priority = wiseOne->sprite->image.priority;
        risingEffect->update                 = scFieldEffectRiseRingBackHalf;
        risingEffect->priorityFlags          = ACTOR_PRIORITY_UNDERFOOT;
    }
}

#include "shared/script_psynergy.c"

// Cycles the Wise One's palette while it glows.
static void scSolSanctumEruptionUpdateWiseOnePalette(void)
{
    scSolSanctumEruptionCycleActorPalette(fdActorGet(ACTOR_WISE_ONE));
}

#include "shared/sol_sanctum_elemental_star_lights.c"

// Unused: an actor script nothing starts. It sends an actor up at 6 pixels a tick, growing it by a
// 25th each tick of its loop, then sets it back to full size.
Sint32 SolSanctumEruptionUnusedRiseAndGrowScript[] = {
    SC_SET(SW_VELOCITY_Y, toFIXED(6)),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, toFIXED(1) / 25),
    SC_ADD(SW_SCALE_Y, toFIXED(1) / 25),
    SC_WAIT(1),
    SC_LOOP(18, SC_TO(1)),
    SC_SET(SW_SCALE_X, toFIXED(1)),
    SC_SET(SW_SCALE_Y, toFIXED(1)),
    SC_END,
};

// clang-format off
// Where each of the ten elemental lights springs from: its x from its chamber's, and its height, in 16.16 pixels.
Sint32 EruptionRisingLightOrigins[] = {
    -toFIXED(6), 0,
    toFIXED(10), toFIXED(10),
    toFIXED(8), toFIXED(25),
    toFIXED(4), toFIXED(30),
    -toFIXED(5), toFIXED(20),
    toFIXED(2), toFIXED(5),
    -toFIXED(6), toFIXED(35),
    -toFIXED(8), toFIXED(15),
    toFIXED(2), toFIXED(40),
    -toFIXED(2), toFIXED(15),
};

#include "shared/sol_sanctum_star_room_entrances.c"

Uint32 SolSanctumEruptionExits[] = {
    SCENE_EXITS(SolSanctumEruption),
    SCENE_EXIT(SOL_SANCTUM_ERUPTION_TO_SOL_SANCTUM_LUNA_SOL_ROOMS, SolSanctumLunaSolRooms, SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_ERUPTION),
    SCENE_EXITS_END,
};

ACTOR_PLACE SolSanctumEruptionActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SATUROS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MENARDI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX_MASKED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WISE_ONE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_CRYSTAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

CUE SolSanctumEruptionCues[] = {
    { CUE_TABLE_END },
};

#include "shared/sol_sanctum_star_metatiles.c"

Sint32 EruptionEmberScript[] = {
    SC_CALL(scSolSanctumEruptionAlignEmberToHeading),
    SC_END,
};

#include "shared/sol_sanctum_elemental_star_light_tables.c"
// clang-format on

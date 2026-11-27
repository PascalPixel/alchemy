// The gangway out over the sea behind the shack in the north-west of Lalivero, where the Lemurian
// ship lies sunk (SceneIdLemurianShip, the area name Lemurian ship), and the game's last scene.
//
// The party comes in from the shack (timber_rooms_bc.c) once Iodem has given it the Black Orb and
// the guard lets it by. A long wooden gangway with rails runs north over the sea, and the ship lies
// beneath the water beside it, seen through the waves. Walking back off the gangway's south end
// goes back into the shack.
//
// Using the Black Orb at the gangway's north end raises the ship (scLemurianShipRaiseTheShip): the
// sea glows, spray rises, the screen shakes, and the ship comes up out of the water beside the
// gangway and rides the swell with a glow along its hull. The party then talks the journey over,
// with three yes-or-no questions, climbs aboard and sails away west as black bars close in from
// above and below (scLemurianShipSetSail). The game then fades to the staff credits and the clear
// screen (TITLE_FROM_LEMURIAN_SHIP), keeping the music (FLAG_KEEP_MUSIC_ONCE).
//
// The sunk ship is the map's two upper layers drawn 90 pixels low, under the sea's layer, which
// ripples by a line scroll (scRippleStart). Reveal stops the ripple while it shows. Arriving by
// LEMURIAN_SHIP_WITH_BLACK_ORB, which only the debug scene selector reaches, first gives the leader
// the Black Orb.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/line_scroll.h"
#include "field/line_scroll_wave_sine.h"
#include "graphics/graphics.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

Sint32                scInit(void);
extern Uint16         ShipGlowPalettes[];
extern Sint32         SprayFallScript[];
extern Sint16         RippleSine[LINE_SCROLL_WAVE_ENTRIES];
extern SCENE_ENTRANCE LemurianShipEntrances[];
extern Uint32         LemurianShipExits[];
extern ACTOR_PLACE    LemurianShipActorPlacements[];
extern CUE            LemurianShipCues[];
extern Sint32         OrbGlowActive;
extern Sint32         OrbGlowPhase;
extern Sint32         OrbGlowStrength;
extern Sint32         OrbGlowFadeTicks;
extern Sint32         SprayActive;
extern Sint32         SwellActive;
extern Sint32         SwellPhase;
extern Sint32         ShipHullY;
extern Sint32         ShipDeckY;
extern Sint32         LeaderDeckY;
extern Sint32         GaretDeckY;
extern Sint32         MiaDeckY;
extern Sint32         IvanDeckY;
extern Uint16         ShipGlowStep;
extern Sint32         LeaderBoardsShip[];
extern Sint32         GaretBoardsShip[];
extern Sint32         IvanBoardsShip[];
extern Sint32         MiaBoardsShip[];

// The ship's glow (scShipGlowCyclePalette): SHIP_GLOW_COLORS of ShipGlowPalettes at a time, from a
// start that moves one colour every SHIP_GLOW_STEP_TICKS ticks, through as many starts as colours
// shown.
#define SHIP_GLOW_COLORS                    6
#define SHIP_GLOW_STEP_TICKS                6

// The curtains (scCurtainClose): three rows of CURTAIN_ROW_PARTS sprites, 32 by 16, one row sliding
// down from above the screen and two up from below, CURTAIN_START_GAP pixels in all, until they
// rest at the top and at CURTAIN_BOTTOM_Y.
#define CURTAIN_PART_WIDTH                  32
#define CURTAIN_PART_HEIGHT                 16
#define CURTAIN_ROW_PARTS                   8
#define CURTAIN_BOTTOM_Y                    136
#define CURTAIN_START_GAP                   48

// The ticks from the curtains starting to close to the ship sailing west: 30 in the French, Spanish
// and Italian editions, 80 before.
#define LEMURIAN_SHIP_DEPARTURE_DELAY_TICKS WHEN(SHORTER_ENDING_SHIP_DEPARTURE_DELAY, 30, 80)

static void scRippleStop(void);
static void scCurtainStart(void);

enum
{
    // The gangway, in two pieces (SPRITE_GANGWAY): its northern half.
    ACTOR_GANGWAY_NORTH = ACTOR_FIRST_PLACED,
    ACTOR_GANGWAY_SOUTH
};

// The deck height of a party member who has not yet boarded. The swell (scShipUpdateSwell) moves
// only those who have.
#define SHIP_NOT_ON_DECK                  (-toFIXED(1))

// The sprite priority of an actor's picture and of its shadow.
#define SET_IMAGE_PRIORITY(actor, level)  ((void)(actor->sprite->image.priority = (level)))
#define SET_SHADOW_PRIORITY(actor, level) ((void)(actor->sprite->shadow.priority = (level)))

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return LemurianShipEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return LemurianShipExits;
}

// The gangway's two pieces. The party's own actors fill the first three rows.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    return LemurianShipActorPlacements;
}

// Walking off the south end of the gangway to the Lemurian ship (metatiles (18,
// 20) to (20, 20)) goes back into the shack in Lalivero. The backdrop colour is
// cleared to black for the way out.
static void scLemurianShipLeaveTheShip(void)
{
    syImportScriptSetExitRequest(FieldStateWork->stepCue);
    IO_WRITE16(0, &PLTT_BACKDROP);
}

// Casting Reveal on the gangway to the Lemurian ship stops the sea rippling while the grey circle
// of Reveal shows; there is nothing hidden here.
static void scRippleStopForReveal(void)
{
    scRippleStop();
}

// The sea around the gangway to the Lemurian ship ripples, from the moment the party arrives and
// again as Reveal ends. The ripple sways the sea's layer across and up and down at once
// (scLineScrollInitWave).
static void scRippleStart(void)
{
    scLineScrollInitWave(
        0, toFIXED(4), toFIXED(1), toFIXED(1) / 8, toFIXED(1), toFIXED(1) / 2, toFIXED(1) / 4);
}

// The way back, Reveal, and the Black Orb at the gangway's north end.
const CUE *scGetCues(void)
{
    return LemurianShipCues;
}

// Runs the glow along the risen ship's hull, a colour at a time.
static void scShipGlowCyclePalette(void)
{
    Uint16 paletteIndex;

    paletteIndex = syMathDivideUnsigned(ShipGlowStep, SHIP_GLOW_STEP_TICKS);
    syDMAXCopy(&ShipGlowPalettes[paletteIndex],
               (void *)&BG_PLTT_COLOR(7, 4),
               DMA_COPY_HALFWORDS(SHIP_GLOW_COLORS),
               REG_DMA3);
    ShipGlowStep++;
    if (ShipGlowStep > SHIP_GLOW_COLORS * SHIP_GLOW_STEP_TICKS - 1)
        ShipGlowStep = 0;
}

// The water drop's burst into a ring of spray (water_drop.png), beyond enum ACTOR_ANIMATION.
enum
{
    ANIM_DROP_BURST = 1
};

// A drop of spray rises and wobbles for its timer, then bursts into a ring of spray and is released
// 20 ticks later by SprayFallScript.
static void scLemurianShipSprayUpdate(ACTOR *drop)
{
    if (drop->timer != 0)
    {
        Sint32 drift;

        drop->timer--;
        drift          = syRandomNext() - syRandomNext();
        drop->x.fixed += drift;
        drop->y.fixed += toFIXED(4) / 5;
    }
    else if (drop->userValue != 0)
    {
        drop->userValue = 0;
        fdActorSelectAnimation(drop, ANIM_DROP_BURST);
        drop->wait = 20;
        fdActorStartScript(drop, SprayFallScript);
    }
}

// Every 40 ticks, the Black Orb's glow fades a step, down to 4.
static void scOrbGlowFade(void)
{
    Sint32 tick      = OrbGlowFadeTicks + 1;

    OrbGlowFadeTicks = tick;
    if (tick == 40)
    {
        if (OrbGlowStrength > 4)
        {
            OrbGlowStrength  -= 1;
            OrbGlowFadeTicks  = 0;
        }
    }
}

// Keeps the raised ship alive with a glow, a swell and spray.
static void scShipUpdateSwell(void)
{
    MAP_GRID_SCROLL_STATE *grid;
    ACTOR                 *actor;
    FIXED                  swell;
    Sint32                 x, z;

    grid = MapGridState[0];
    if (OrbGlowActive != 0)
    {
        // The blend is a halfword in the tick, written through its address.
        Uint16 blend;

        swell   = syMulFX(sySin(OrbGlowPhase << 9), 3);
        *&blend = OrbGlowStrength + ((swell + 8) << 8);
        IO_WRITE16(blend, &REG_BLDALPHA);
        OrbGlowPhase++;
    }
    if (SwellActive != 0)
    {
        swell                                        = toFIXED(syMulFX(sySin(SwellPhase << 9), 2));
        grid->layers[MAPGRID_LAYER_MIDDLE].offsetY   = ShipHullY + swell;
        grid->layers[MAPGRID_LAYER_OVERHEAD].offsetY = ShipDeckY + swell;
        if (LeaderDeckY != SHIP_NOT_ON_DECK)
        {
            actor               = fdActorGet(ACTOR_PARTY_LEADER);
            actor->y.fixed      = LeaderDeckY + swell;
            actor->groundHeight = LeaderDeckY + swell;
            actor->motionFlags  = 0;
        }
        if (GaretDeckY != SHIP_NOT_ON_DECK)
        {
            actor               = fdActorGet(ACTOR_GARET);
            actor->y.fixed      = GaretDeckY + swell;
            actor->groundHeight = LeaderDeckY + swell;
            actor->motionFlags  = 0;
        }
        if (MiaDeckY != SHIP_NOT_ON_DECK)
        {
            actor               = fdActorGet(ACTOR_MIA);
            actor->y.fixed      = MiaDeckY + swell;
            actor->groundHeight = LeaderDeckY + swell;
            actor->motionFlags  = 0;
        }
        if (IvanDeckY != SHIP_NOT_ON_DECK)
        {
            actor               = fdActorGet(ACTOR_IVAN);
            actor->y.fixed      = IvanDeckY + swell;
            actor->groundHeight = LeaderDeckY + swell;
            actor->motionFlags  = 0;
        }
        SwellPhase++;
    }
    if (SprayActive != 0 && (TickCount & 1) != 0)
    {
        POS sprayPosition;

        x                = grid->view[0] & FIELD_PIXEL_MASK;
        z                = grid->view[1] & FIELD_PIXEL_MASK;
        x               += syRandomNext() * DISPLAY_WIDTH;
        sprayPosition.x  = x;
        sprayPosition.y  = 0;
        z               += syRandomNext() * DISPLAY_HEIGHT;
        z               += toFIXED(30);
        sprayPosition.z  = z;
        actor            = syFieldActorCreate(
            SPRITE_WATER_DROP, sprayPosition.x, sprayPosition.y, sprayPosition.z);
        if (actor != NULL)
        {
            actor->update                 = scLemurianShipSprayUpdate;
            actor->timer                  = 60;
            actor->userValue              = 1;
            actor->motionFlags            = 0;
            actor->priorityFlags          = ACTOR_PRIORITY_UNDERFOOT;
            actor->sprite->image.priority = 2;
            fdActorSetSpriteFlags(actor, ACTOR_BLEND_NORMAL);
            fdActorSelectAnimation(actor, ANIM_STILL);
        }
    }
}

// Remembers the leader's height on deck, so that from then on he bobs with the ship.
static Sint32 scLemurianShipLeaderStepOnDeck(ACTOR *interpreter)
{
    LeaderDeckY = fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
    return 0;
}

// Remembers Garet's height on deck, so that from then on he bobs with the ship.
static Sint32 scLemurianShipGaretStepOnDeck(ACTOR *interpreter)
{
    GaretDeckY = fdActorGet(ACTOR_GARET)->y.fixed;
    return 0;
}

// Remembers Mia's height on deck, so that from then on she bobs with the ship.
static Sint32 scLemurianShipMiaStepOnDeck(ACTOR *interpreter)
{
    MiaDeckY = fdActorGet(ACTOR_MIA)->y.fixed;
    return 0;
}

// Remembers Ivan's height on deck, so that from then on he bobs with the ship.
static Sint32 scLemurianShipIvanStepOnDeck(ACTOR *interpreter)
{
    IvanDeckY = fdActorGet(ACTOR_IVAN)->y.fixed;
    return 0;
}

// On the gangway beside the risen Lemurian ship, Garet, Mia and Ivan step out and talk the journey
// over with Isaac, who answers three questions: whether the journey ends when they find Jenna,
// whether to find her before Lemuria, and whether they will be all right on the rough sea. Each
// answer picks one of two replies. Then all four hop up onto the deck, Mia last after a look back
// at the shore, and the ship sails away west as black bars close in from above and below and the
// ending theme plays, until the screen fades to the staff credits.
static void scLemurianShipSetSail(void)
{
    MAP_GRID_SCROLL_STATE *grid;
    ACTOR                 *ship;
    FIELD_STATE           *state;

    fdScriptStart();
    fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    fdMapGridCopyMetatileAttributes(18, 0, 3, 1, 18, 12);
    // Garet, Mia and Ivan step out around the leader. Garet: "It looks like it's finally time to
    // leave Gondowan." Ivan and Mia say how much has happened.
    fdScriptSetMessageId(MsgFieldLooksLikeFinally);
    syFieldActorLaunchFromLeader(ACTOR_GARET, -10, 16, FACING_NORTH);
    syFieldActorLaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
    fdActorLaunchFromLeader(ACTOR_IVAN, 10, 16, FACING_NORTH);
    fdActorWaitForMoveById(ACTOR_GARET);
    fdScriptWait(50);
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(20);
    // "It looks like it's finally time to leave Gondowan."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    // "It's so strange... We've been traveling for so long, and yet..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    fdScriptWait(40);
    // "Now that I think about it, so many things have happened."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    fdScriptWait(60);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(60);
    // The first question, Garet's: will the journey be over once they find Jenna? The answer picks
    // which of his two replies he says.
    fdActorRunRepeatedMotion(ACTOR_GARET, 2);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdScriptWait(20);
    // "But our journey will be over as soon as we find Jenna, right?"
    fdScriptOpenMessage(ACTOR_GARET, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
        // "The end of one journey, but the beginning of another? No way!"
        fdScriptShowMessage(ACTOR_GARET, 0);
        FieldStateWork->msg++;
    }
    else
    {
        fdScriptWait(30);
        fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
        FieldStateWork->msg++;
        // "What did you say? Our next journey? What journey is that?"
        fdScriptShowMessage(ACTOR_GARET, 0);
    }
    fdScriptWait(10);
    // Ivan and Mia remind Garet of their promise to Babi, and Lemuria comes up.
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
    // "You didn't forget, did you, Garet?"
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
    fdScriptWait(30);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    fdActorRunRepeatedMotion(ACTOR_MIA, 2);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
    fdScriptWait(30);
    // "Our promise to Babi."
    fdScriptShowMessage(ACTOR_MIA, 0);
    fdScriptWait(10);
    fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
    // "Oh... Lemuria!"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
    fdScriptWait(20);
    // "I wonder what kind of place Lemuria is."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    fdScriptWait(20);
    // The second question, Mia's: must they find Jenna before going to Lemuria? Garet answers
    // either way. "We have to find Jenna before going to Lemuria, don't we?"
    fdScriptOpenMessage(ACTOR_MIA, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
        fdScriptWait(20);
        // "Yeah. That's important."
        fdScriptShowMessage(ACTOR_GARET, 0);
        FieldStateWork->msg++;
    }
    else
    {
        fdScriptWait(20);
        fdActorRunRepeatedMotion(ACTOR_GARET, 2);
        fdScriptWait(20);
        fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
        fdScriptWait(20);
        FieldStateWork->msg++;
        // "Huh? Jenna's going to be very sad when I tell her what you said!"
        fdScriptShowMessage(ACTOR_GARET, 0);
    }
    fdScriptWait(10);
    // Ivan: their first time on the ocean; Garet: the ocean Kraden wanted him to see.
    fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    // "At any rate, this will be our first time on the ocean."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(20);
    // "So this is the ocean Kraden wanted me to see."
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(20);
    fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    // The third question, Mia's: the seas are rough; will they be all right? "The seas are very
    // rough. Do you think we will be okay?"
    fdScriptOpenMessage(ACTOR_MIA, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    fdScriptWait(20);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(30);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        fdScriptWait(20);
        // "This is a Lemurian ship. We'll be fine."
        fdScriptShowMessage(ACTOR_MIA, 0);
        FieldStateWork->msg++;
    }
    else
    {
        fdScriptWait(30);
        fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
        fdScriptWait(20);
        FieldStateWork->msg++;
        // "I guess it's hard to say. Still, we must go, just the same..."
        fdScriptShowMessage(ACTOR_MIA, 0);
    }
    fdScriptWait(10);
    // Garet: "Well... Shall we, then?" Ivan: "To sea..." Mia: Jenna is waiting for them. The
    // departure theme starts.
    fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    fdScriptWait(30);
    // "Well... Shall we, then?"
    fdScriptShowMessage(ACTOR_GARET, 0);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdScriptWait(30);
    fdScriptWait(10);
    fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    fdScriptWait(30);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(20);
    // "To sea..."
    fdScriptShowMessage(ACTOR_IVAN, 0);
    fdScriptWait(10);
    fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    fdScriptWait(20);
    // "Jenna is waiting for us!"
    fdScriptShowMessage(ACTOR_MIA, 0);
    sySoundPlay(SOUND_FADE_OUT_BGM);
    fdScriptWait(10);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(40);
    // Everyone boards by their own scripts, hopping up onto the deck (SE_HIGH_JUMP each); the
    // camera follows up to the ship.
    fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
    fdActorSetSpeed(ACTOR_GARET, 1.2);
    fdActorSetSpeed(ACTOR_IVAN, 1.2);
    fdActorSetSpeed(ACTOR_MIA, 1.2);
    fdActorSetScript(ACTOR_PARTY_LEADER, LeaderBoardsShip);
    fdScriptWait(50);
    fdCameraMoveTo(264, CAMERA_KEEP, 200, 1);
    fdActorSetScript(ACTOR_GARET, GaretBoardsShip);
    fdScriptWait(50);
    fdActorSetScript(ACTOR_IVAN, IvanBoardsShip);
    fdActorWaitForScriptEndById(ACTOR_IVAN);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTH, 0);
    // Mia, last, looks back at the shore before she boards.
    fdActorWalkByAndWait(ACTOR_MIA, 0, -32);
    fdScriptWait(30);
    fdActorFaceDirection(ACTOR_MIA, FACING_SOUTH, 0);
    fdScriptWait(60);
    fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
    fdScriptWait(20);
    fdActorSetScript(ACTOR_MIA, MiaBoardsShip);
    fdActorWaitForScriptEndById(ACTOR_MIA);
    fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    fdScriptWait(20);
    // On deck, all four turn west together and nod.
    fdCameraMoveTo(216, CAMERA_KEEP, 168, 1);
    fdCameraWaitForMove();
    fdScriptWait(20);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
    fdScriptWait(40);
    fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
    fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
    fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    fdScriptWait(30);
    // The ship sails west and the curtains close: the sea's layer and the gangway's north piece
    // slide away east past it, ever faster, and the camera pans along the deck to the stern.
    sySoundPlay(MUS_THE_BROKEN_SEAL_ENDING);
    sySoundSetBGMTempo(SOUND_TEMPO_SCALE_NORMAL * 15 / 16);
    scCurtainStart();
    fdScriptWait(LEMURIAN_SHIP_DEPARTURE_DELAY_TICKS);
    grid                                      = MapGridState[0];
    ship                                      = fdActorGet(ACTOR_GANGWAY_NORTH);
    ship->acceleration                        = toFIXED(0.002);
    ship->speedLimit                          = toFIXED(2);
    grid->layers[MAPGRID_LAYER_GROUND].speedX = -toFIXED(3) / 16;
    fdActorWalkByAndWait(ACTOR_GANGWAY_NORTH, 60, 0);
    grid->layers[MAPGRID_LAYER_GROUND].speedX = -toFIXED(3) / 8;
    fdActorWalkByAndWait(ACTOR_GANGWAY_NORTH, 60, 0);
    fdScriptWait(80);
    fdScriptWait(100);
    fdCameraSetSpeedAndAcceleration(2.2, 0.01);
    fdCameraMoveTo(808, CAMERA_KEEP, 168, 1);
    fdScriptWait(300);
    state             = FieldStateWork;
    state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    IO_WRITE16(0, &PLTT_BACKDROP);
    state->transitionFrame = 96;
    fdScriptFinishScreenTransition();
    fdScriptWaitTransitionTicks();
    fdScriptWait(30);
    gmFlagSet(FLAG_KEEP_MUSIC_ONCE);
    fdScriptSetDestination(SCENE_TITLE, TITLE_FROM_LEMURIAN_SHIP);
}

// The water's ripple, this scene's view of the line scroll's state (LineScroll, LINE_SCROLL_STATE).
//
// It holds two pages of per-line scroll offsets, swapped each tick and built from a sine table.
// Here a line is one word, BG3's x and y, where LINE_SCROLL_STATE has three.
typedef struct RIPPLE_SCROLL_STATE
{
    Uint16 pages[2][960]; // per page: 160 lines of BG3 x, y; the rest unused
    Uint8  page;          // the page the HBlank DMA shows; the other is built
    Uint8  mode;
    Uint16 phase;         // ticks built so far, added to BG3's y
    Uint8  unused[4];     // zeroed with the block; nothing reads or writes it
    FIXED  frequency[2];  // x, y: 16.16 sine entries per tick and per row
    FIXED  step[2];       // x, y: 16.16 sine entries a line
    FIXED  amplitude[2];  // x, y: 16.16 scale on the sine, then divided by 256
} RIPPLE_SCROLL_STATE;

// One line's ripple offset.
static inline Sint32 scLemurianShipWaveOffset(Sint32 angle, FIXED amplitude, Uint16 base)
{
    return (Uint16)(syMulFX(RippleSine[FIXEDtoINT(angle) & (LINE_SCROLL_WAVE_ENTRIES - 1)],
                            amplitude)
                    / LINE_SCROLL_WAVE_SCALE)
         + base;
}

// Using the Black Orb at the north end of the gangway (metatiles (18, 12) to (20, 14)) raises the
// Lemurian ship. Isaac walks to the end and faces the sea; the water glows brighter and brighter,
// flashing three times, spray rises all over it and the screen shakes harder, and the ship slowly
// comes up out of the sea beside the gangway. A white flash, the puzzle-solved jingle, and the
// departure follows (scLemurianShipSetSail).
static void scLemurianShipRaiseTheShip(void)
{
    MAP_GRID_SCROLL_STATE *grid;
    Sint32                 i;
    Sint32                 flash;
    Sint32                 white;
    Sint32                 dark;
    FIXED                  risen;
    Uint16                 count; // each control, read into the tick by syIORead16

    grid = MapGridState[0];

    fdScriptStart();
    fdEndBusyAndReveal();
    // The leader steps to the water's edge; the backdrop brightens 16 steps.
    fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 312, 232);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdScriptWait(40);
    sySoundPlay(SE_SPIN);
    for (i = 0; i <= RGB_LEVEL_MAX / 2; i++)
    {
        IO_WRITE16(RGB(0, i, i * 2), &PLTT_BACKDROP);
        fdScriptWait(10);
    }
    IO_WRITE16(RGB(0, 16, 31), &PLTT_BACKDROP);
    {
        Uint16 bright = BLEND_ALPHA(16, 16);

        for (flash = 0; flash < 3; flash++)
        {
            sySoundPlay(SE_POWER_UP);
            REG_BLDALPHA = bright;
            fdScriptWait(3);
            IO_WRITE16(BLEND_ALPHA(16, 8), &REG_BLDALPHA);
            fdScriptWait(65);
        }
    }
    // The orb's glow and the spray begin, and the water lightens in three steps.
    OrbGlowActive = 1;
    OrbGlowPhase  = 0;
    sySetTickEvent(scShipUpdateSwell, EVENT_PASS_TICK);
    SprayActive = 1;
    fdScriptWait(20);
    sySoundPlay(SE_RUSH);
    fdMapGridSetShake(1, 1, 1);
    fdScriptWait(60);
    SprayActive = 1;
    fdMapGridSetShake(2, 2, 1);
    fdScriptWait(60);
    fdMapGridSetShake(3, 3, 1);
    OrbGlowFadeTicks = 0;
    sySetTickEvent(scOrbGlowFade, EVENT_PASS_TICK);
    // The ship rises 90 pixels, 0.2 pixels a tick.
    risen = 0;
    do
    {
        grid->layers[MAPGRID_LAYER_MIDDLE].offsetY   += toFIXED(1) / 5;
        grid->layers[MAPGRID_LAYER_OVERHEAD].offsetY += toFIXED(1) / 5;
        risen                                        += toFIXED(1) / 5;
        sySynch(1);
    } while (risen < toFIXED(90));
    syCloseEvent(scOrbGlowFade);
    SprayActive = 0;
    syIORead16(&REG_BG3CNT, &count);
    count      = (count & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
    REG_BG3CNT = count;
    syIORead16(&REG_BG2CNT, &count);
    count      = (count & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
    REG_BG2CNT = count;
    syIORead16(&REG_BG1CNT, &count);
    count         = (count & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
    REG_BG1CNT    = count;
    OrbGlowActive = 0;
    sySoundPlay(SE_STOP_ACTION_SE);
    sySynch(1);
    // A flash of white; the swell starts as it clears.
    sySoundPlay(SE_CRASH);
    IO_WRITE16(BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN, &REG_BLDCNT);
    for (white = 0; white <= BLEND_FADE_MAX; white++)
    {
        REG_BLDY = white;
        fdScriptWait(1);
    }
    fdScriptWait(40);
    fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    ShipHullY   = grid->layers[MAPGRID_LAYER_MIDDLE].offsetY;
    ShipDeckY   = grid->layers[MAPGRID_LAYER_OVERHEAD].offsetY;
    SwellActive = 1;
    for (dark = BLEND_FADE_MAX; dark >= 0; dark--)
    {
        REG_BLDY = dark;
        fdScriptWait(8);
    }
    sySetTickEvent(scShipGlowCyclePalette, EVENT_PASS_TICK);
    sySoundPlay(MUS_PUZZLE_SOLVED);
    mnSoundWaitForJingle();
    fdScriptWait(20);
    fdScriptEnd();
    scLemurianShipSetSail();
}

// Lays the Lemurian ship under the sea: its two layers are lowered 90 pixels and drawn behind the
// sea's, which is blended over them, while the gangway and the party are drawn above. The sea
// starts rippling.
Sint32 scInit(void)
{
    MAP_GRID_SCROLL_STATE *grid;
    Uint16                 count; // each control, read into the tick by syIORead16

    grid = MapGridState[0];
    if (MainRoutineStateWork.entrance == LEMURIAN_SHIP_WITH_BLACK_ORB)
    {
        gmInventoryAddItem(ACTOR_PARTY_LEADER, ITEM_BLACK_ORB);
    }
    FieldStateWork->transition                      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    fdActorGet(ACTOR_GANGWAY_NORTH)->collisionFlags = 0;
    fdActorGet(ACTOR_GANGWAY_NORTH)->priorityFlags  = ACTOR_PRIORITY_UNDERFOOT;
    fdActorGet(ACTOR_GANGWAY_SOUTH)->collisionFlags = 0;
    fdActorGet(ACTOR_GANGWAY_SOUTH)->priorityFlags  = ACTOR_PRIORITY_UNDERFOOT;
    {
        ACTOR *actor = fdActorGet(ACTOR_GANGWAY_NORTH);

        SET_IMAGE_PRIORITY(actor, 1);
    }
    {
        ACTOR *actor = fdActorGet(ACTOR_GANGWAY_SOUTH);

        SET_IMAGE_PRIORITY(actor, 1);
    }
    {
        ACTOR *actor = fdActorGet(ACTOR_PARTY_LEADER);

        SET_IMAGE_PRIORITY(actor, 1);
        SET_SHADOW_PRIORITY(actor, 1);
    }
    {
        ACTOR *actor = fdActorGet(ACTOR_GARET);

        SET_IMAGE_PRIORITY(actor, 1);
        SET_SHADOW_PRIORITY(actor, 1);
    }
    {
        ACTOR *actor = fdActorGet(ACTOR_IVAN);

        SET_IMAGE_PRIORITY(actor, 1);
        SET_SHADOW_PRIORITY(actor, 1);
    }
    {
        ACTOR *actor = fdActorGet(ACTOR_MIA);

        SET_IMAGE_PRIORITY(actor, 1);
        SET_SHADOW_PRIORITY(actor, 1);
    }
    syIORead16(&REG_BG3CNT, &count);
    count      = (count & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(2);
    REG_BG3CNT = count;
    syIORead16(&REG_BG2CNT, &count);
    count      = (count & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
    REG_BG2CNT = count;
    syIORead16(&REG_BG1CNT, &count);
    count      = (count & ~BGCNT_PRIORITY(3)) | BGCNT_PRIORITY(3);
    REG_BG1CNT = count;
    IO_WRITE16(BLDCNT_TGT1_BG3 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2
                   | BLDCNT_TGT2_BD,
               &REG_BLDCNT);
    IO_WRITE16(BLEND_ALPHA(16, 8), &REG_BLDALPHA);
    {
        MAP_GRID_LAYER_SCROLL *layer  = grid->layers + 1;

        layer->offsetY               -= toFIXED(90);
        layer                         = grid->layers + 2;
        layer->offsetY               -= toFIXED(90);
    }
    fdMapGridCenterOnFollowedPoint();
    scRippleStart();
    return 0;
}

// Arms DMA 0 to copy the shown ripple page into background 3's scroll registers.
void fdLineScrollArmDMA(void)
{
    RIPPLE_SCROLL_STATE *state;
    Uint32              *src;
    volatile Uint16     *dma0;
    volatile Uint32     *dest;

    state                  = (RIPPLE_SCROLL_STATE *)LineScroll;
    src                    = (Uint32 *)state->pages[state->page];

    dma0                   = REG_DMA0;
    dest                   = (volatile Uint32 *)&REG_BG3HOFS;
    dma0[DMA_CNT_H_INDEX] &= ~DMA_START_TIMING_DRQ_REPEAT;
    dma0[DMA_CNT_H_INDEX] &= ~DMA_ENABLE;
    dma0[DMA_CNT_H_INDEX];

    *dest = *src++;
    syDMAXCopy(src, (void *)dest, DMA_HBLANK_WORD_PER_LINE, (volatile Uint32 *)dma0);
}

// Builds the other ripple page and swaps it in.
void fdLineScrollBuildPage(void)
{
    RIPPLE_SCROLL_STATE *state;
    Uint16              *line;
    Sint32               angle;
    Uint16               base;
    Sint32               amplitude;
    Sint32               step;
    Uint16               y;
    Sint32               i;

    state     = (RIPPLE_SCROLL_STATE *)LineScroll;
    y         = BgScrollWork[3].y;
    line      = state->pages[state->page ^ 1];
    step      = state->step[0];
    angle     = (state->phase + y) * state->frequency[0];
    amplitude = state->amplitude[0];
    base      = BgScrollWork[3].x;
    for (i = 0; i != DISPLAY_HEIGHT; i++)
    {
        *line  = scLemurianShipWaveOffset(angle, amplitude, base);
        angle += step;
        line  += 2;
    }
    line      = state->pages[state->page ^ 1] + 1;
    step      = state->step[1];
    angle     = (state->phase + y) * state->frequency[1];
    amplitude = state->amplitude[1];
    for (i = 0; i != DISPLAY_HEIGHT; i++)
    {
        *line  = scLemurianShipWaveOffset(angle, amplitude, y);
        angle += step;
        line  += 2;
    }
    state->phase++;
    state->page ^= 1;
}

#include "shared/lemurian_ship_ripple.c"

// Takes the ripple's events off and stops DMA 0.
static void scRippleStop(void)
{
    volatile Uint16 *reg;

    syCloseEvent(fdLineScrollArmDMA);
    syCloseEvent(fdLineScrollBuildPage);
    reg                  = REG_DMA0;
    reg[DMA_CNT_H_INDEX] = ~DMA_START_TIMING_DRQ_REPEAT & reg[DMA_CNT_H_INDEX];
    reg[DMA_CNT_H_INDEX] = ~DMA_ENABLE & reg[DMA_CNT_H_INDEX];
    reg[DMA_CNT_H_INDEX];
}

// The curtains' sprites, which close in from above and below as the ship sails.
static MENU_SPR CurtainSprites[3 * CURTAIN_ROW_PARTS];
static Sint16   CurtainGap;
static Sint16   CurtainVramSlot;

// Writes one curtain sprite: its queue link, its attributes 0 and 1, and its attribute 2.
#if FIX_TYPE_PUNNING
#    define CURTAIN_PART(attributes, tile)                                                         \
        (part->next            = NULL,                                                             \
         part->oam.word.attr01 = (attributes),                                                     \
         part->oam.word.attr23 = (tile),                                                           \
         part++)
#else
// UB: the parts are written through a word cursor, the link among them.
#    define CURTAIN_PART(attributes, tile) (*word++ = 0, *word++ = (attributes), *word++ = (tile))
#endif

// Each tick from scCurtainStart to the end of the game: closes the curtains a pixel more, until
// they rest, and queues their 24 sprites.
static void scCurtainClose(void)
{
#if FIX_TYPE_PUNNING
    MENU_SPR *part;
#else
    Uint32 *word;
#endif
    Uint32 tile;
    Sint32 gap;
    Sint32 edge;
    Uint32 i;

    tile = VramCacheEntries[CurtainVramSlot].offset >> 5;
#if FIX_TYPE_PUNNING
    part = CurtainSprites;
#else
    word = (Uint32 *)CurtainSprites;
#endif
    if (CurtainGap != 0)
        CurtainGap--;
    // The top row, then the bottom two.
    for (i = 0; i < CURTAIN_ROW_PARTS; i++)
    {
        Sint32 topEdge;

        gap     = CurtainGap;
        topEdge = -gap / 2;
        CURTAIN_PART((topEdge & (OBJ_Y_RANGE - 1)) | OBJ_X(i * CURTAIN_PART_WIDTH) | OBJ_SIZE_32X16,
                     tile);
    }
    edge = (gap / 2 + CURTAIN_BOTTOM_Y) & (OBJ_Y_RANGE - 1);
    for (i = 0; i < CURTAIN_ROW_PARTS; i++)
        CURTAIN_PART(OBJ_X(i * CURTAIN_PART_WIDTH) | edge | OBJ_SIZE_32X16, tile);
    i    = 0;
    edge = (CurtainGap / 2 + CURTAIN_BOTTOM_Y + CURTAIN_PART_HEIGHT) & (OBJ_Y_RANGE - 1);
    for (; i < CURTAIN_ROW_PARTS; i++)
        CURTAIN_PART(OBJ_X(i * CURTAIN_PART_WIDTH) | edge | OBJ_SIZE_32X16, tile);
    for (i = 0; i < 3 * CURTAIN_ROW_PARTS; i++)
        syOAMAddSpriteAtDepth(&CurtainSprites[i], OAM_DEPTH_FRONT);
}

// Fills a curtain sprite's tiles with colour 1 and starts the curtains CURTAIN_START_GAP pixels
// apart from where they rest.
static void scCurtainStart(void)
{
    Uint8 *tiles;

    tiles =
        syHeapCallAlternatePool(syHeapAllocateEWRAM, CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2);
    CurtainVramSlot = syVRAMCacheFindFreeEntry();
    DMA_FILL32(PIXELS_4BPP(1), tiles, CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2);
    syVRAMCacheLoad(CurtainVramSlot, CURTAIN_PART_WIDTH * CURTAIN_PART_HEIGHT / 2, tiles);

    CurtainGap = CURTAIN_START_GAP;
    sySetTickEvent(scCurtainClose, EVENT_PASS_TICK);
}

// The ship's glow.
//
// scShipGlowCyclePalette shows six of these colours at a time, from a start that moves one colour
// every 6 ticks.
Uint16 ShipGlowPalettes[] = {
    RGB(0, 3, 22),
    RGB(0, 14, 26),
    RGB(0, 25, 31),
    RGB(0, 14, 26),
    RGB(0, 3, 22),
    RGB(0, 0, 18),
    RGB(0, 3, 22),
    RGB(0, 14, 26),
    RGB(0, 25, 31),
    RGB(0, 14, 26),
    RGB(0, 3, 22),
    RGB(0, 0, 0),
};

// A drop of spray, at the end of its rise: released.
Sint32 SprayFallScript[] = {
    SC_RELEASE,
};

// The ripple's sine, the line scroll's LineScrollWaveSine kept again in this overlay.
Sint16         RippleSine[LINE_SCROLL_WAVE_ENTRIES] = {LINE_SCROLL_WAVE_SINE};

SCENE_ENTRANCE LemurianShipEntrances[]              = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 408, 0, 200, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {LEMURIAN_SHIP_FROM_TIMBER_ROOMS_C,
     CONDITION_ALWAYS, 312,
     0, 296,
     FACING_NORTH, 0,
     -1,
     -1,
     -1,
     328, 0},
    {SCENE_TABLE_END},
};

Uint32 LemurianShipExits[] = {
    SCENE_EXITS(LemurianShip),
    SCENE_EXIT(LEMURIAN_SHIP_TO_TIMBER_ROOMS_C, TimberRoomsC, TIMBER_ROOMS_C_FROM_LEMURIAN_SHIP),
    SCENE_EXITS_END,
};

ACTOR_PLACE LemurianShipActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    0,
                    0,
                    0,
                    FACING_EAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(ACTOR_IVAN,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    0,
                    0,
                    0,
                    FACING_EAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(ACTOR_MIA,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    0,
                    0,
                    0,
                    FACING_EAST,
                    TALK_FACING_PARTY,
                    0),
    ACTOR_PLACEMENT(SPRITE_GANGWAY,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    312,
                    0,
                    248,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    ACTOR_PLACEMENT(SPRITE_GANGWAY,
                    CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND,
                    312,
                    0,
                    312,
                    FACING_SOUTH,
                    TALK_FACING_KEEP,
                    0),
    {SCENE_TABLE_END},
};

CUE LemurianShipCues[] = {
    {CUE_STEP, 1, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipLeaveTheShip)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scRippleStopForReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES),
     0, CONDITION_ALWAYS,
     SCRIPT_REF(scRippleStart)},
    {USE_ITEM(ITEM_BLACK_ORB), 10, CONDITION_ALWAYS, SCRIPT_REF(scLemurianShipRaiseTheShip)},
    {CUE_TABLE_END},
};

// The Black Orb's glow: whether it shows, its phase in ticks, its strength (BLDALPHA's first
// weight, a step weaker every 40 ticks down to 4) and the ticks into that step.
Sint32 OrbGlowActive      = 0;
Sint32 OrbGlowPhase       = 0;
Sint32 OrbGlowStrength    = 16;
Sint32 OrbGlowFadeTicks   = 0;

// Set while spray flies about the rising ship.
Sint32 SprayActive        = 0;

// The raised ship's swell: whether it runs, its phase in ticks, the hull's and deck's layer heights
// it rocks about, and the height on the deck of each of the party (16.16 pixels), SHIP_NOT_ON_DECK
// for one not on it.
Sint32 SwellActive        = 0;
Sint32 SwellPhase         = 0;
Sint32 ShipHullY          = 0;
Sint32 ShipDeckY          = 0;
Sint32 LeaderDeckY        = SHIP_NOT_ON_DECK;
Sint32 GaretDeckY         = SHIP_NOT_ON_DECK;
Sint32 MiaDeckY           = SHIP_NOT_ON_DECK;
Sint32 IvanDeckY          = SHIP_NOT_ON_DECK;

// The ticks into the glow's run along the risen ship's hull, SHIP_GLOW_STEP_TICKS for each of its
// SHIP_GLOW_COLORS colours.
Uint16 ShipGlowStep       = 0;

Sint32 LeaderBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -56),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(scLemurianShipLeaderStepOnDeck),
    SC_END,
};

Sint32 GaretBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -40),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(scLemurianShipGaretStepOnDeck),
    SC_END,
};

Sint32 IvanBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-20, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -32),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(1),
    SC_CALL(scLemurianShipIvanStepOnDeck),
    SC_END,
};

Sint32 MiaBoardsShip[] = {
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -8),
    SC_SYNC,
    SC_SET(SW_VELOCITY_Y, toFIXED(8)),
    SC_SE(SE_HIGH_JUMP),
    SC_MOVE_BY(0, 0, -24),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(20),
    SC_ANIMATION(2),
    SC_MOVE_BY(-8, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -8),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_CALL(scLemurianShipMiaStepOnDeck),
    SC_END,
};

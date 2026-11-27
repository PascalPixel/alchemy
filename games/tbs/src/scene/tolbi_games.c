// Tolbi's three games of chance, which the party can play whenever it is in town: Lucky Dice, Lucky
// Wheels and Tolbi Spring. None of them is part of the story.
//
// Lucky Dice and Lucky Wheels are two rooms of one map (SceneIdStuccoRoomsC, whose area name is
// Tolbi), each with its own camera box.
//
// - Lucky Dice is played in the small tent with a die on its front. Two
//       fair-haired young men stand behind the counter at the back. The one on
//       the left asks ten coins for each level of the party's average level
// and, once paid, sends Isaac to the green table to throw. When the game ends
//       he says how many coins were won or lost, with a fanfare that grows with
//       the win. Boards on the wall give the rules, and three players, an old
//       man with a cane, a burly grey-haired man and a woman in green, talk of
//       their luck. A soldier talks of the game until Babi is saved
//       (FLAG_BABI_SAVED, the finals day), then of the scalpers at the Colosso
//       finals, and is gone once Colosso is over (FLAG_COLOSSO_OVER).
// - Lucky Wheels, the slot machine, is played in the room up the stairs
//       beside the inn. The young woman in green behind its curved counter
//       takes Game Tickets, and warns when the bag has six free slots or fewer.
//       When the game ends she hands over an item for each line won, one of the
//       three of its kind (shirts, boots, rings, or stars and hearts for items;
//       a moon wins any kind), never the one she gave last time; or, when
//       nothing was won, offers another game. Green boards by the counter give
//       the rules and the prizes.
//
// Tolbi Spring (SceneIdTolbiSpring) is the fountain in the middle of Tolbi's square, reached
// through the gate in its railing. Isaac stands on the white step between its two signs and
// searches the spring. The first time each visit its water sinks and fades away, uncovering a
// target of rings on the basin's floor with two crabs and two turtles moving about on it. He turns
// his back to the spring and tosses a coin or a Lucky Medal into it. A crab or turtle the token
// lands beside knocks it away, and the rings pull it toward the centre until it settles in one of
// five. A coin wins 20, 10, 2 or 1 coins, from the centre out, or nothing in the fifth. A medal
// always wins an item, drawn by weight from fifteen prizes, the nearer the centre the fewer and
// better (Assassin Blade first, Water of Life last), and the item won has its weight halved for
// later tosses. Where the token settles is measured from TOLBI_SPRING_CENTER_X and _Z, against the
// rings painted on the basin's floor (scTolbiSpringUpdateTossSimulation). In the Japanese edition
// the coin and medal counts show an `=` after each number.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/lucky_wheels.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

Sint32                      scInit(void);
extern const SCENE_ENTRANCE TolbiSpringEntrances[];
extern const Uint32         TolbiSpringExits[];
extern const ACTOR_PLACE    TolbiSpringActorPlacements[];
extern const ACTOR_PLACE    StuccoRoomsCActorPlacements[];
extern const CUE            StuccoRoomsCTriggers[];
extern const CUE            TolbiSpringCues[];
extern const Uint16         TolbiSpringAlphaSteps[];
extern const Uint32         TolbiSpringPrizeItems[];
extern const Uint16         TolbiSpringCoinPrizes[];
extern const Uint32         StuccoRoomsCLuckyWheelsPrizeItems[];
extern const Uint8          TolbiSpringCrabFrames[];
extern const Uint8          TolbiSpringCircleFrames[];
extern const Uint8          TolbiSpringActorPixelX[];
extern const Uint8          TolbiSpringActorPixelZ[];
extern const Uint16         TolbiSpringActorHeadings[];

// The spring's water has sunk and faded away this visit.
#define FLAG_TOLBI_SPRING_DRAINED FLAG_SCENE_TEMP_00

// The party's coins as Lucky Dice starts, kept in SceneLocalState so the attendant can tell the
// change when the party comes back.
#define sLuckyDiceCoinsBefore     (*(Sint32 *)SceneLocalState)

// UB: the coin prizes are read through a one-element view of their table; UBFIX reads the table.
#if FIX_POINTER_OUTSIDE_OBJECT
#    define TOLBI_SPRING_COIN_PRIZE(ring) TolbiSpringCoinPrizes[ring]
#else
#    define TOLBI_SPRING_COIN_PRIZE(ring)                                                          \
        (((struct { Uint16 coins[1]; } *)TolbiSpringCoinPrizes)->coins[ring])
#endif

static void   scTolbiSpringDrainSpring(void);
static void   scStuccoRoomsCOfferLuckyWheels(Sint32 spoken);
static Sint32 scTolbiSpringRunSpringGame(void);
static Sint32 scStuccoRoomsCSelectLuckyWheelsPrize(Sint32 category);
static void   scTolbiSpringInitTossSimulation(void);
static Sint32 scTolbiSpringRunToss(Sint32 tossKind);

// What the player throws into the spring, chosen by mnSelectCoinsOrMedals.
typedef enum SPRING_TOSS_KIND
{
    SPRING_TOSS_COIN,
    SPRING_TOSS_MEDAL
} SPRING_TOSS_KIND;

// The people and boards of the Lucky Dice tent and the Lucky Wheels room, numbered in the order of
// StuccoRoomsCActorPlacements from ACTOR_FIRST_PLACED.
typedef enum STUCCO_ROOMS_C_ACTOR_ID
{
    // A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) behind the curved counter of the Lucky
    // Wheels room, who runs the slot machine (scStuccoRoomsCTalkToWheelsAttendant).
    ACTOR_LUCKY_WHEELS_ATTENDANT = ACTOR_FIRST_PLACED,
    // A young man with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR), the left of the two behind the
    // Lucky Dice counter, who runs the game (scStuccoRoomsCOfferLuckyDice).
    ACTOR_LUCKY_DICE_ATTENDANT,
    // The other young man behind the Lucky Dice counter, on the right, who points the party to the
    // rules: `Read the sign if ya don't know the rules.`
    ACTOR_LUCKY_DICE_RULES_ATTENDANT,
    // A Lucky Dice player, a burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR) at the west
    // end of the yellow rug.
    ACTOR_LUCKY_DICE_PLAYER_FIRST_TIME,
    // An old man with a cane (SPRITE_OLD_MAN_WITH_CANE) by the green table.
    ACTOR_LUCKY_DICE_PLAYER_TRIPLE_DIGITS,
    // A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS) by the east wall.
    ACTOR_LUCKY_DICE_PLAYER_OUT_OF_COINS,
    // The green board of Lucky Wheels prizes north-west of the counter. The map draws it; an unseen
    // actor (SPRITE_BLANK) stands on it to be read (scStuccoRoomsCReadLuckyWheelsPrizes).
    ACTOR_LUCKY_WHEELS_PRIZE_SIGN,
    // The green board of Lucky Wheels rules east of the counter, read the same way
    // (scStuccoRoomsCReadLuckyWheelsRules).
    ACTOR_LUCKY_WHEELS_RULES_SIGN,
    // The Lucky Dice rules, on a green board east of the counter, read the same way.
    ACTOR_LUCKY_DICE_RULES_SIGN,
    // A soldier by the red rug in the Lucky Dice tent, gone after Colosso, drawn with the
    // long-haired fighter's sprite (SPRITE_PERSON_WITH_SWORD).
    ACTOR_LUCKY_DICE_SOLDIER,
    // The Lucky Dice rules for pairs, on a green board west of the counter.
    ACTOR_LUCKY_DICE_PAIRS_SIGN
} STUCCO_ROOMS_C_ACTOR_ID;

// Tolbi Spring's own actors (TolbiSpring), numbered in the order of TolbiSpringActorPlacements from
// ACTOR_FIRST_PLACED.
//
// The toss draws the coin or the medal with its shadow and three trailing copies, which
// TOLBI_SPRING_COIN_TRAIL and TOLBI_SPRING_MEDAL_TRAIL number.
typedef enum TOLBI_SPRING_ACTOR_ID
{
    ACTOR_TOLBI_SPRING_COIN_SHADOW = ACTOR_FIRST_PLACED + 3,
    // The coin the party tosses (SPRITE_SPRING_COIN).
    ACTOR_TOLBI_SPRING_COIN,
    ACTOR_TOLBI_SPRING_MEDAL_SHADOW = ACTOR_TOLBI_SPRING_COIN + 4,
    // The Lucky Medal the party tosses (SPRITE_SPRING_LUCKY_MEDAL).
    ACTOR_TOLBI_SPRING_MEDAL,
    // The four obstacles that move in the spring during a toss, in the order of Obstacles.
    //
    // They are two crabs (SPRITE_CRAB), then two turtles (SPRITE_TURTLE).
    ACTOR_TOLBI_SPRING_FIRST_OBSTACLE,
    // The sign east of the white step, on Lucky Medals (SPRITE_SIGNPOST).
    ACTOR_TOLBI_SPRING_MEDAL_SIGN = ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 4,
    // The sign west of the white step, on tossing a coin.
    ACTOR_TOLBI_SPRING_COIN_SIGN,
    // The tufts of grass at the foot of each sign.
    //
    // They are the signs' sprite in its second tick, mirrored and drawn underfoot (scInit).
    ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS,
    ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS,
    // A visitor carrying a pack (SPRITE_MAN_CARRYING_A_PACK) west of the signs, who explains the
    // game.
    ACTOR_TOLBI_SPRING_VISITOR_MYSTERIOUS_POWERS,
    // A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) by the spring's north-west corner, who
    // wishes he had a Lucky Medal.
    ACTOR_TOLBI_SPRING_MEDAL_SEEKER,
    // A visitor, a woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) east of the spring, who has
    // only one Lucky Medal.
    ACTOR_TOLBI_SPRING_VISITOR_ONE_MEDAL
} TOLBI_SPRING_ACTOR_ID;

// Animations of the spring's sprites, as their animation tables and ticks under graphics/sprite
// draw them. The coin's and the Lucky Medal's sprite sheets (spring_coin.png,
// spring_lucky_medal.png) are drawn alike.
typedef enum TOLBI_SPRING_ANIMATION
{
    ANIM_TOKEN_TUMBLE     = 0, // turning over once as it is tossed
    ANIM_TOKEN_FLOAT      = 1, // turning as it drifts toward the rings
    ANIM_TOKEN_REST       = 2, // lying still
    ANIM_TOKEN_SHADOW     = 3, // a grey spot on the ground below it
    // A twinkle, as each of the three marks trailing a flying token.
    ANIM_TOKEN_GLINT      = 4,
    ANIM_CRAB_SCUTTLE     = 1, // stepping sideways (crab.png)
    ANIM_CRAB_RAISE_CLAWS = 3, // snapping its raised claws (crab.png)
    ANIM_TURTLE_SWIM      = 2, // paddling (turtle.png)
    ANIM_TURTLE_HIDE      = 3, // head and flippers drawn in (turtle.png)
    // The signpost sprite sheet's second picture, a tuft of grass (signpost.png).
    ANIM_SIGN_GRASS       = 2
} TOLBI_SPRING_ANIMATION;

// The actor that draws toss->trail[n] of a tossed coin or medal, n from 1 to 3.
//
// They are the rows before each shadow, the oldest copy first.
#define TOLBI_SPRING_COIN_TRAIL(n)  (ACTOR_TOLBI_SPRING_COIN_SHADOW - (n))
#define TOLBI_SPRING_MEDAL_TRAIL(n) (ACTOR_TOLBI_SPRING_MEDAL_SHADOW - (n))

// The two metatiles searched to toss into Tolbi Spring from the white step at metatile (7, 9): the
// spring's rim north of it, metatile (7, 8), and the ground south of it, metatile (7, 10), in front
// of Isaac when he stands with his back to the spring.
typedef enum TOLBI_SPRING_METATILE_BEHAVIOR
{
    METATILE_BEHAVIOR_TOLBI_SPRING_WATER = 80,
} TOLBI_SPRING_METATILE_BEHAVIOR;

// Lucky Dice costs this many coins for each level of the party's average level; a win of
// LUCKY_DICE_MEDIUM_WIN coins or more, or of LUCKY_DICE_LARGE_WIN, is greeted with a bigger jingle.
enum
{
    LUCKY_DICE_COINS_PER_LEVEL = 10,
    LUCKY_DICE_MEDIUM_WIN      = 5000,
    LUCKY_DICE_LARGE_WIN       = 20000
};

// The kinds of prize a line of Lucky Wheels wins, three items each in
// StuccoRoomsCLuckyWheelsPrizeItems: those before LUCKY_WHEELS_PRIZE_ANYTHING, which wins any of
// them.
enum
{
    LUCKY_WHEELS_KINDS = LUCKY_WHEELS_PRIZE_ANYTHING
};

// The slot machine's report in the holdings' wonPrizes: the kinds won, ended by
// LUCKY_WHEELS_PRIZE_LIST_END, or LUCKY_WHEELS_PRIZE_LIST_DONE once the attendant has handed them
// over; from LUCKY_WHEELS_LAST_GIVEN on, the item of each kind she gave last.
enum
{
    LUCKY_WHEELS_LAST_GIVEN = 8
};

// The pixels the spring's water sinks before it fades, one every fourth tick.
enum
{
    SPRING_WATER_SINK_PIXELS = 16
};

// A medal's prizes (MAIN_ROUTINE_STATE's prizeWeights): its ring allows three more for each ring
// out from the centre.
enum
{
    SPRING_MEDAL_PRIZES          = 15,
    SPRING_MEDAL_PRIZES_PER_RING = 3
};

// The ring a tossed token settles in: 0 the innermost to 3, or outside them all.
enum
{
    SPRING_RING_OUTSIDE = 4
};

// Which way a crab scuttles, in its obstacle's heading; a turtle's heading is an angle round its
// ellipse.
enum
{
    CRAB_HEADING_EAST = 0,
    CRAB_HEADING_WEST = 1
};

// The second crab rests this many ticks at each end of its run.
enum
{
    SPRING_CRAB_REST_TICKS = 30
};

// The token's velocity keeps SPRING_TOKEN_PULLED_DAMPING / SPRING_TOKEN_DAMPING_SCALE of itself
// each tick while the rings pull it, and SPRING_TOKEN_SETTLING_DAMPING / SPRING_TOKEN_DAMPING_SCALE
// once they let go.
enum
{
    SPRING_TOKEN_PULLED_DAMPING   = 253,
    SPRING_TOKEN_SETTLING_DAMPING = 220,
    SPRING_TOKEN_DAMPING_SCALE    = 256
};

// The spring's centre, which the prize rings pull the tossed token toward.
#define TOLBI_SPRING_CENTER_X        toFIXED(120)
#define TOLBI_SPRING_CENTER_Z        toFIXED(71)

// The window of the party's coins and Lucky Medals, and the menu of what to toss, in window
// metatiles, with the counts' x in pixels: the European editions' longer words widen both and move
// the counts right.
#define TOLBI_SPRING_COUNT_WIDTH     BY_LANGUAGE(17, 17, 19, 19, 19, 19)
#define TOLBI_SPRING_COUNT_NUMBER_X  BY_LANGUAGE(56, 72, 88, 88, 88, 88)
#define TOLBI_SPRING_TOSS_MENU_X     BY_LANGUAGE(22, 20, 19, 19, 19, 19)
#define TOLBI_SPRING_TOSS_MENU_WIDTH BY_LANGUAGE(7, 9, 10, 10, 10, 10)

// The tossed coin or medal, as its last four positions for the trail, its velocity and the ticks
// the prize rings still pull it.
typedef struct SPRING_TOSS_MOTION
{
    Uint8   unused[2]; // nothing reads or writes it
    Sint16  launchFrame;
    VECTOR3 trail[4];  // 16.16 fixed-point pixels
    VECTOR3 shadow;
    VECTOR3 velocity;  // 16.16 fixed-point pixels per tick
    Sint32  attractionFrame;
} SPRING_TOSS_MOTION;

// One of the four obstacles moving in the spring during a toss.
typedef struct SPRING_OBSTACLE
{
    VECTOR3 pos; // 16.16 fixed-point pixels
    Sint16  heading;
    Sint16  pauseOrPhase;
    Sint16  speedMode;
    Sint16  reactionFrame;
    Sint16  hitCooldownFrame;
} SPRING_OBSTACLE;

static inline void scIOSetBlendControl(Sint32 value)
{
    REG_BLDCNT = value;
}

static inline void scIOSetBlendAlpha(Sint32 value)
{
    REG_BLDALPHA = value;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
    return TolbiSpringEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
    return NULL;
}

const Uint32 *scGetExits(void)
{
    return TolbiSpringExits;
}

// Selects the actors of Tolbi Spring or of the Lucky Dice and Lucky Wheels rooms.
const ACTOR_PLACE *scGetActorPlacements(void)
{
    if (MainRoutineStateWork.next == SCENE_TOLBI_SPRING)
    {
        return TolbiSpringActorPlacements;
    }
    return StuccoRoomsCActorPlacements;
}

// The man with a pack by Tolbi Spring's north-west corner asks whether Isaac has a Lucky Medal.
//
// If he has, the man says it will be extra lucky tossed in the spring; if not, that he would want
// one himself to try his luck.
static void scTolbiSpringAskAboutLuckyMedal(Sint32 actorId)
{
    Sint32 question = MsgTolbiLuckyMedal;
    fdScriptSetMessageId(question);
    // "Do you have a Lucky Medal?" Yes: "Oh, you do? How...lucky! That means you'll be extra lucky
    // if you toss it in the fountain!" No: "Hmm... If I were going to try my luck, I'd want to use
    // a Lucky Medal."
    fdScriptOpenMessage(actorId, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(10);
        fdScriptSetMessageId(question + 1);
    }
    else
    {
        fdScriptSetMessageId(question + 2);
    }
    fdScriptShowMessage(actorId, 0);
}

// The burly grey-haired man at the west end of the yellow rug in the Lucky Dice tent asks whether
// this is Isaac's first time to play.
//
// Yes, and he warns that once you start you won't want to stop; no, and he says Lucky Dice is all
// the rage in Tolbi.
static void scStuccoRoomsCAskAboutFirstDiceGame(Sint32 actorId)
{
    Sint32 question = MsgTolbiYerFirstTime;
    fdScriptSetMessageId(question);
    // "Is this yer first time to play Lucky Dice?" Yes: "Once ya start playing, ya won't want to
    // stop. There's no other game in all the lands that's this much fun." No: "Lucky Dice is all
    // the rage in Tolbi. You can play it year-round."
    fdScriptOpenMessage(actorId, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        fdScriptWait(10);
        fdScriptSetMessageId(question + 1);
    }
    else
    {
        fdScriptSetMessageId(question + 2);
    }
    fdScriptShowMessage(actorId, 0);
}

// Isaac searches Tolbi Spring from the white step between its signs, facing the spring or with his
// back to it (metatile (7, 8) or (7, 10)), to toss a coin or a Lucky Medal.
//
// The first time each visit, the water sinks and fades away to uncover the target of rings on the
// basin's floor (scTolbiSpringDrainSpring). He then stands on the step with his back to the spring,
// and the party chooses what to toss (scTolbiSpringRunSpringGame).
static void scTolbiSpringTossCoinOrMedal(void)
{
    Sint32 leader = MainRoutineStateWork.leaderActorId;

    if (fdScriptGetFlag(FLAG_TOLBI_SPRING_DRAINED) == 0)
    {
        gmFlagSet(FLAG_TOLBI_SPRING_DRAINED);
        scTolbiSpringDrainSpring();
    }
    fdScriptStart();
    fdActorWalkToAndWait(leader, 120, 152);
    fdActorFaceDirection(leader, FACING_SOUTH, 0);
    scTolbiSpringRunSpringGame();
    fdScriptEnd();
}

// Isaac reads the green board east of the Lucky Wheels counter: pull the lever up to five times to
// line up matching icons, press the star to hold a wheel, and play 1, 3, 5 or 7 lines for 1 to 4
// Game Tickets.
static void scStuccoRoomsCReadLuckyWheelsRules(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgTolbiLuckyWheelsRulesPullLever);
    // "Lucky Wheels Rules Pull the lever and try to line up..."
    fdScriptOpenMessage(-1, 0);
    fdScriptEnd();
}

// Isaac reads the green board north-west of the Lucky Wheels counter: shirts win undershirts, rings
// win rings, boots win boots, stars and hearts win items, and moons can win anything.
static void scStuccoRoomsCReadLuckyWheelsPrizes(void)
{
    fdScriptStart();
    fdScriptSetMessageId(MsgTolbiLuckyWheelsPrizesPrizesDetermined);
    // "Lucky Wheels Prizes Prizes are determined by which icons you..."
    fdScriptOpenMessage(-1, 0);
    fdScriptEnd();
}

// The young woman behind the Lucky Wheels counter invites Isaac to play, and if he has Game Tickets
// and agrees, sends him to the slot machine.
//
// Without tickets she tells him he needs them to play. See scStuccoRoomsCOfferLuckyWheels.
static void scStuccoRoomsCTalkToWheelsAttendant(void)
{
    scStuccoRoomsCOfferLuckyWheels(0);
}

// Selects the cues of Tolbi Spring or of the Lucky Dice and Lucky Wheels rooms.
const CUE *scGetCues(void)
{
    if (MainRoutineStateWork.next == SCENE_TOLBI_SPRING)
    {
        return TolbiSpringCues;
    }
    return StuccoRoomsCTriggers;
}

// The setup of both scenes, and what the attendants say when a game ends.
//
// Entering Tolbi Spring readies the crabs and turtles, and keeps the water hidden if it was drained
// earlier this visit. Back in the Lucky Dice tent after a game, Isaac steps up to the counter and
// the attendant says how many coins were won or lost, to a fanfare that grows with the win. Back
// from the slot machine, the Lucky Wheels attendant says `Congratulations!` and hands over an item
// for each line won, or offers another game when nothing was won.
Sint32 scInit(void)
{
    Sint32 coinChange;
    Sint8 *wonPrizes;
    Sint32 prizeIndex;
    Sint32 item;

    if (MainRoutineStateWork.next == SCENE_TOLBI_SPRING)
    {
        FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        IO_WRITE16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL, &REG_BLDCNT);
        IO_WRITE16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
        fdActorSetAnimation(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS, ANIM_SIGN_GRASS);
        fdActorSetAnimation(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS, ANIM_SIGN_GRASS);
        fdActorGet(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS)->scaleX        = -toFIXED(1);
        fdActorGet(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS)->scaleX        = -toFIXED(1);
        fdActorGet(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        fdActorGet(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        fdScriptStartScreenTransition();
        if (MainRoutineStateWork.entrance == TOLBI_SPRING_FROM_TOLBI)
        {
            scTolbiSpringInitTossSimulation();
            if (gmFlagGet(FLAG_TOLBI_SPRING_DRAINED))
            {
                scIOSetBlendControl(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
                scIOSetBlendAlpha(BLEND_ALPHA(0, 16));
            }
        }
    }
    else
    {
        if (gmFlagGet(FLAG_COLOSSO_OVER))
        {
            fdActorSetPos(ACTOR_LUCKY_DICE_SOLDIER, 0, 0);
        }
        OamCopyEnabled             = 1;
        FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
        if (MainRoutineStateWork.entrance == STUCCO_ROOMS_C_LUCKY_DICE_FROM_TOLBI)
        {
            fdActorSetSpriteColorById(ACTOR_LUCKY_WHEELS_ATTENDANT, SPRITE_COLOR_GRB);
            fdActorSetSpriteColorById(ACTOR_LUCKY_DICE_ATTENDANT, SPRITE_COLOR_RBG);
        }
        // Back from Lucky Dice, the attendant tells the coins won or lost.
        if (MainRoutineStateWork.entrance == STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE
            && !gmFlagGet(FLAG_KEEP_PARTY_POSITION))
        {
            fdScriptStart();
            fdActorSetSpriteColorById(ACTOR_LUCKY_WHEELS_ATTENDANT, SPRITE_COLOR_GRB);
            fdActorSetSpriteColorById(ACTOR_LUCKY_DICE_ATTENDANT, SPRITE_COLOR_RBG);
            fdScriptStartScreenTransition();
            fdScriptWaitTransitionTicks();
            fdScriptWait(10);
            fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 112);
            fdScriptWait(20);
            coinChange = MainRoutineStateWork.holdings.coins - sLuckyDiceCoinsBefore;
            if (coinChange > 0)
            {
                if (coinChange >= LUCKY_DICE_LARGE_WIN)
                {
                    sySoundPlay(MUS_SUPER_LUCKY);
                }
                else if (coinChange >= LUCKY_DICE_MEDIUM_WIN)
                {
                    sySoundPlay(MUS_VERY_LUCKY);
                }
                else
                {
                    sySoundPlay(MUS_LUCKY);
                }
                fdScriptWait(20);
                fdScriptSetMessageId(MsgTolbiYaWonNumber);
                mnMessageArgumentPush(coinChange, MESSAGE_ARGUMENT_NUMBER);
                // "Ya won ... coin! Talk about lucky!"
                fdScriptShowMessage(ACTOR_LUCKY_DICE_ATTENDANT, 0);
                mnSoundWaitForJingle();
            }
            else if (coinChange < 0)
            {
                fdScriptSetMessageId(MsgTolbiYaLostNumber);
                mnMessageArgumentPush(-coinChange, MESSAGE_ARGUMENT_NUMBER);
                // "Ya lost ... coin. Better luck next time."
                fdScriptShowMessage(ACTOR_LUCKY_DICE_ATTENDANT, 0);
            }
            fdScriptEnd();
        }
        // Back from the slot machine, the attendant hands over what was won.
        if (MainRoutineStateWork.entrance == STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS
            && !gmFlagGet(FLAG_KEEP_PARTY_POSITION))
        {
            wonPrizes = MainRoutineStateWork.holdings.wonPrizes;
            fdScriptStart();
            fdScriptStartScreenTransition();
            fdScriptWaitTransitionTicks();
            fdScriptWait(10);
            if (wonPrizes[0] == LUCKY_WHEELS_PRIZE_LIST_END)
            {
                scStuccoRoomsCOfferLuckyWheels(1);
            }
            else if (wonPrizes[0] != LUCKY_WHEELS_PRIZE_LIST_DONE)
            {
                fdScriptSetMessageId(MsgTolbiCongratulations);
                // "Congratulations!"
                fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
                if (*wonPrizes != LUCKY_WHEELS_PRIZE_LIST_END)
                {
                    for (prizeIndex = 0; wonPrizes[prizeIndex] != LUCKY_WHEELS_PRIZE_LIST_END;
                         prizeIndex++)
                    {
                        if (prizeIndex == 0)
                        {
                            fdScriptSetMessageId(MsgTolbiWonItemGo);
                        }
                        else
                        {
                            fdScriptSetMessageId(MsgTolbiAndYouWonItem);
                        }
                        item = scStuccoRoomsCSelectLuckyWheelsPrize(wonPrizes[prizeIndex]);
                        mnMessageArgumentPush(item, MESSAGE_ARGUMENT_ITEM);
                        // "You won ...! Here you go.", then "And you won ...! Here you go."
                        fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
                        gmItemShowFound(item, 3);
                        gmPartyGiveItem(item, 0);
                        fdScriptWait(10);
                        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
                        fdScriptWait(30);
                    }
                }
                MainRoutineStateWork.holdings.wonPrizes[0] = LUCKY_WHEELS_PRIZE_LIST_DONE;
                fdScriptSetMessageId(MsgTolbiComeAgain);
                // "Come again!"
                fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
            }
            fdScriptEnd();
        }
    }
    return 0;
}

// Draws one picture of the prize being shown in the middle of Tolbi Spring, with SE_CAST, and holds
// it 8 ticks.
static inline void scTolbiSpringShowPrizePicture(Sint32 srcColumn, Sint32 srcRow)
{
    fdMapGridCopyMetatilesTo(srcColumn, srcRow, 70, 0, 3, 8);
    sySoundPlay(SE_CAST);
    fdScriptWait(8);
}

// Tolbi Spring gives a Lucky Medal's prize: a roar, Isaac turns to face the spring, and a sea
// dragon of green-blue scales rises from its north edge, its long neck stretching south over the
// rings until its head reaches the rim before him, the prize held up over his head as he raises his
// arms; "Isaac got an Adept's Helm." (or whatever the medal won), and the dragon draws back north
// the way it came.
//
// The dragon is the pictures: each is 3 by 8 metatiles copied over the spring's middle from spare
// metatiles of the map, one every 8 ticks, the head a step further south in each as it comes and a
// step back north in each as it goes.
static void scTolbiSpringRevealPrize(Sint32 item)
{
    fdScriptStart();
    fdScriptWait(30);
    sySoundPlay(SE_BOSS_ROAR);
    fdScriptWait(100);
    fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    fdScriptWait(40);

    fdMapGridCopyMetatilesTo(82, 20, 70, 0, 3, 8);
    fdScriptWait(3);
    scTolbiSpringShowPrizePicture(85, 20);
    scTolbiSpringShowPrizePicture(88, 20);
    scTolbiSpringShowPrizePicture(91, 20);
    scTolbiSpringShowPrizePicture(94, 20);
    scTolbiSpringShowPrizePicture(97, 20);
    scTolbiSpringShowPrizePicture(100, 20);

    scTolbiSpringShowPrizePicture(79, 29);
    scTolbiSpringShowPrizePicture(82, 29);
    scTolbiSpringShowPrizePicture(85, 29);
    scTolbiSpringShowPrizePicture(88, 29);
    scTolbiSpringShowPrizePicture(91, 29);
    scTolbiSpringShowPrizePicture(94, 29);
    scTolbiSpringShowPrizePicture(97, 29);
    fdMapGridCopyMetatilesTo(100, 29, 70, 0, 3, 8);
    sySoundPlay(SE_CAST);

    fdScriptWait(70);
    sySoundPlay(SE_RECOVERY);
    gmItemShowFound(item, 3);
    gmPartyGiveItem(item, 0);
    fdScriptWait(20);

    scTolbiSpringShowPrizePicture(97, 29);
    scTolbiSpringShowPrizePicture(94, 29);
    scTolbiSpringShowPrizePicture(91, 29);
    scTolbiSpringShowPrizePicture(88, 29);
    scTolbiSpringShowPrizePicture(85, 29);
    scTolbiSpringShowPrizePicture(82, 29);
    scTolbiSpringShowPrizePicture(100, 20);
    scTolbiSpringShowPrizePicture(97, 20);
    scTolbiSpringShowPrizePicture(94, 20);
    scTolbiSpringShowPrizePicture(91, 20);
    scTolbiSpringShowPrizePicture(88, 20);
    scTolbiSpringShowPrizePicture(85, 20);
    scTolbiSpringShowPrizePicture(82, 20);
    scTolbiSpringShowPrizePicture(79, 20);
    fdScriptEnd();
}

// Tolbi Spring's water sinks 16 pixels in the basin and fades away, uncovering the target of rings
// on its floor.
//
// It starts with SE_BURST. The water is the overhead layer, scrolled down a pixel every fourth tick
// and then faded out through TolbiSpringAlphaSteps.
static void scTolbiSpringDrainSpring(void)
{
    MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
    MAP_GRID_LAYER_SCROLL *spring;
    const Uint16          *alphaSteps;
    Sint32                 stepIndex;

    sySoundPlay(SE_BURST);
    spring = &grid->layers[MAPGRID_LAYER_OVERHEAD];
    for (stepIndex = 0; stepIndex < SPRING_WATER_SINK_PIXELS; stepIndex++)
    {
        spring->offsetY -= toFIXED(1);
        sySynch(4);
    }
    alphaSteps = TolbiSpringAlphaSteps;
    for (stepIndex = 0; stepIndex < 8; stepIndex++)
    {
        IO_WRITE16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL, &REG_BLDCNT);
        IO_WRITE16(*alphaSteps++, &REG_BLDALPHA);
        sySynch(8);
    }
}

// The Lucky Wheels attendant asks whether Isaac will use his Game Tickets, warns him when the bag
// is full or nearly so, and sends him to the slot machine.
//
// spoken is 1 when the game has just ended with nothing won: she says `Too bad...` and offers
// another game, or says to come back with tickets.
static void scStuccoRoomsCOfferLuckyWheels(Sint32 spoken)
{
    Sint32 tickets = gmPartyInventoryCountItem(ITEM_GAME_TICKET);
    Sint32 room;

    fdActorSnapshot();
    if (spoken == 0)
    {
        Sint32 message = MsgTolbiStepRightTry;

        fdScriptSetMessageId(message);
        // "Step right up and try your hand at lucky wheels!"
        fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
        if (tickets == 0)
        {
            // "You gotta have game tickets to play."
            fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
            return;
        }
        fdScriptSetMessageId(message + 2);
        mnMessageArgumentPush(tickets, MESSAGE_ARGUMENT_NUMBER);
        // "You have ... game ticket, Isaac. Would you like to use these..."
        fdScriptOpenMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
        {
            // "The rules are listed on the sign. Take a look, take a look."
            fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
            return;
        }
        room = gmPartyInventoryCountFreeSlots();
        if (room == 0)
        {
            fdScriptSetMessageId(message + 4);
            // "You can't carry any more items. Are you sure you want to play?"
            fdScriptOpenMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
        }
        else if (room <= 6)
        {
            fdScriptSetMessageId(message + 5);
            // "If you win anything, you might not be able to take it with..."
            fdScriptOpenMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
        }
        if (room <= 6 && fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
        {
            fdScriptSetMessageId(MsgTolbiGetRidItems);
            // "Then get rid of some items and come again."
            fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
            return;
        }
    }
    else
    {
        if (tickets == 0)
        {
            fdScriptSetMessageId(MsgTolbiBadComeBack);
            // "Too bad... Come back after you get some tickets."
            fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
            return;
        }
        fdScriptSetMessageId(MsgTolbiBadLikeUse);
        // "Too bad... Would you like to use your tickets to keep playing?"
        fdScriptOpenMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
        if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
        {
            fdScriptSetMessageId(MsgTolbiComeAgain);
            // "Come again!"
            fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
            return;
        }
    }
    fdScriptSetMessageId(MsgTolbiStepRightEnjoy);
    // "Step right up and enjoy yourself."
    fdScriptShowMessage(ACTOR_LUCKY_WHEELS_ATTENDANT, 0);
    fdScriptLeaveFor(MODE_SLOTS, 0);
    fdScriptSetAfterBattleDestination(SCENE_STUCCO_ROOMS_C,
                                      STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS);
}

// The fair-haired young man on the left behind the Lucky Dice counter offers a game for ten coins a
// level of the party's average level. If Isaac agrees, he walks down to the green table and the
// game of Lucky Dice begins.
//
// Without enough coins he is told `Ya gotta pay to play.`
static void scStuccoRoomsCOfferLuckyDice(void)
{
    Uint32 cost;
    Uint32 coins;

    cost = gmPartyGetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;
    fdScriptStart();
    coins = MainRoutineStateWork.holdings.coins;
    if (coins < cost)
    {
        fdScriptSetMessageId(MsgTolbiYaDontEnough);
        // "Hey! Ya don't have enough coins! Ya gotta pay to play."
        fdScriptOpenMessage(ACTOR_LUCKY_DICE_ATTENDANT, 0);
        return;
    }
    sLuckyDiceCoinsBefore = coins;
    fdActorSnapshot();
    fdScriptSetMessageId(MsgTolbiBuddyWannaTry);
    mnMessageArgumentPush(cost, MESSAGE_ARGUMENT_NUMBER);
    // "Hey, buddy, wanna try yer luck at the dice? It's ... coin to..."
    fdScriptOpenMessage(ACTOR_LUCKY_DICE_ATTENDANT, 0);
    if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
    {
        // "OK! Then take the dice."
        fdScriptShowMessage(ACTOR_LUCKY_DICE_ATTENDANT, 0);
        fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 128);
        fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 120, 152);
        fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
        fdScriptWait(20);
        fdScriptLeaveFor(MODE_DICE, 0);
        fdScriptSetAfterBattleDestination(SCENE_STUCCO_ROOMS_C,
                                          STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE);
    }
    else
    {
        FieldStateWork->msg++;
        // "No? Oh, well. Who's next?"
        fdScriptShowMessage(ACTOR_LUCKY_DICE_ATTENDANT, 0);
    }
    fdScriptEnd();
}

// The party chooses whether to toss a coin or a Lucky Medal, presses A to toss it, and wins coins
// or an item by the ring it settles in.
//
// A coin that settles outside the four inner rings wins nothing. A medal's prize is drawn by weight
// from the prizes its ring allows, and its weight is halved for later tosses.
static Sint32 scTolbiSpringRunSpringGame(void)
{
    Sint32              choice = 0;
    Sint32              text;
    Sint32              cancel = -1; // what the menus answer when B is pressed
    Sint32              coins;
    Sint32              medals;
    WINDOW             *window;
    Sint32              result;
    Sint32              totalWeight;
    Sint32              prizeIndex;
    Sint32              prizeRoll;
    Sint32              msg;
    MAIN_ROUTINE_STATE *routine;

    fdScriptStart();
    fdEndBusyAndReveal();
    msg = MsgTolbiThrow;
#if LANGUAGE != LANGUAGE_JAPANESE
    routine = &MainRoutineStateWork;
prompt:
    coins  = routine->holdings.coins;
    medals = gmPartyInventoryCountItem(ITEM_LUCKY_MEDAL);
    fdScriptSetMessageId(msg);
    // "What will you throw?"
    fdScriptOpenMessage(-1, 0);
    window = mnWindowCreate(0, 0, TOLBI_SPRING_COUNT_WIDTH, 4, 2);
    text   = MsgTolbiCoins;
    mnTextDrawMessageAsTiles(text, window, 0, 0);
    mnTextDrawNumberInWindow(coins, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 0);
    mnTextDrawMessageAsTiles(text + 1, window, 0, 8);
    mnTextDrawNumberInWindow(medals, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 8);
    choice = mnSelectCoinsOrMedals(choice);
    mnWindowClose(window, 2);
    mnMessageCloseFinishedWindows();
    if (choice == cancel)
    {
        fdScriptEnd();
        return 0;
    }
    if (choice == SPRING_TOSS_COIN)
    {
        if (coins == 0)
        {
            fdScriptSetMessageId(msg + 1);
            // "You don't have any coins!"
            fdScriptShowMessage(-1, 0);
            sySynch(1);
            goto prompt;
        }
    }
    else if (choice == SPRING_TOSS_MEDAL)
    {
        if (medals == 0)
        {
            fdScriptSetMessageId(msg + 2);
            // "You have no Lucky Medals!"
            fdScriptShowMessage(-1, 0);
            sySynch(1);
            goto prompt;
        }
        if (gmPartyInventoryCountFreeSlots() == 0)
        {
            fdScriptSetMessageId(msg + 4);
            // "You can't carry more items. Do you still want to play?"
            fdScriptOpenMessage(-1, 0);
            if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
            {
                goto end;
            }
        }
    }
    window = mnWindowCreate(TOLBI_SPRING_TOSS_MENU_X, 15, TOLBI_SPRING_TOSS_MENU_WIDTH, 4, 2);
    text   = MsgTolbiToss;
    mnTextDrawMessageAsTiles(text, window, 0, 0);
    mnTextDrawMessageAsTiles(text + 1, window, 0, 8);
    sySynch(5);
    sySoundPlay(SE_TOLBI_SPRING_TOSS_PROMPT);
    for (;;)
    {
        if (PerPush & KEY_A)
        {
            sySoundPlay(SE_MENU_CONFIRM);
            result = 0;
            break;
        }
        if (PerPush & KEY_B)
        {
            sySoundPlay(SE_MENU_CANCEL);
            result = -1;
            break;
        }
        sySynch(1);
    }
    mnWindowClose(window, 2);
#else
    for (;;)
    {
        text    = MsgTolbiCoins;
        routine = &MainRoutineStateWork;
        coins   = routine->holdings.coins;
        medals  = gmPartyInventoryCountItem(ITEM_LUCKY_MEDAL);
        fdScriptSetMessageId(msg);
        // "What will you throw?"
        fdScriptOpenMessage(-1, 0);
        window = mnWindowCreate(0, 0, TOLBI_SPRING_COUNT_WIDTH, 4, 2);
        mnTextDrawMessageAsTiles(text, window, 0, 0);
        mnTextDrawNumberInWindow(coins, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 0);
        // The marker after each count, MsgTolbiCounterMarker, follows the medal label.
        mnTextDrawMessageAsTiles(text + 2, window, 104, 0);
        mnTextDrawMessageAsTiles(text + 1, window, 0, 8);
        mnTextDrawNumberInWindow(medals, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 8);
        mnTextDrawMessageAsTiles(text + 2, window, 104, 8);
        choice = mnSelectCoinsOrMedals(choice);
        mnWindowClose(window, 2);
        mnMessageCloseFinishedWindows();
        if (choice == cancel)
        {
            fdScriptEnd();
            return 0;
        }
        if (choice == SPRING_TOSS_COIN)
        {
            if (coins == 0)
            {
                fdScriptSetMessageId(msg + 1);
                // "You don't have any coins!"
                fdScriptShowMessage(-1, 0);
                sySynch(1);
                continue;
            }
        }
        else if (choice == SPRING_TOSS_MEDAL)
        {
            if (medals == 0)
            {
                fdScriptSetMessageId(msg + 2);
                // "You have no Lucky Medals!"
                fdScriptShowMessage(-1, 0);
                sySynch(1);
                continue;
            }
            if (gmPartyInventoryCountFreeSlots() == 0)
            {
                fdScriptSetMessageId(msg + 4);
                // "You can't carry more items. Do you still want to play?"
                fdScriptOpenMessage(-1, 0);
                if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
                {
                    fdScriptEnd();
                    return 0;
                }
            }
        }
        window = mnWindowCreate(TOLBI_SPRING_TOSS_MENU_X, 15, TOLBI_SPRING_TOSS_MENU_WIDTH, 4, 2);
        result = MsgTolbiToss;
        mnTextDrawMessageAsTiles(result, window, 0, 0);
        mnTextDrawMessageAsTiles(result + 1, window, 0, 8);
        sySynch(5);
        sySoundPlay(SE_TOLBI_SPRING_TOSS_PROMPT);
        for (;;)
        {
            if (PerPush & KEY_A)
            {
                sySoundPlay(SE_MENU_CONFIRM);
                result = 0;
                break;
            }
            if (PerPush & KEY_B)
            {
                sySoundPlay(SE_MENU_CANCEL);
                result = -1;
                break;
            }
            sySynch(1);
        }
        mnWindowClose(window, 2);
        break;
    }
#endif
    // Charge the chosen token only after the player confirms the toss.
    if (result != cancel)
    {
        if (choice == SPRING_TOSS_COIN)
        {
            gmPartyAdjustCoins(-1);
        }
        else if (choice == SPRING_TOSS_MEDAL)
        {
            gmPartyInventoryRemove(ITEM_LUCKY_MEDAL);
        }
        result = scTolbiSpringRunToss(choice);
        if (choice == SPRING_TOSS_COIN)
        {
            if (result != SPRING_RING_OUTSIDE)
            {
                gmPartyAdjustCoins(TOLBI_SPRING_COIN_PRIZE(result));
                sySoundPlay(MUS_LUCKY);
                mnMessageArgumentPush(TOLBI_SPRING_COIN_PRIZE(result), MESSAGE_ARGUMENT_NUMBER);
                fdScriptSetMessageId(MsgTolbiWonNumberCoin);
                // "You won ... coin!"
                fdScriptShowMessage(-1, 0);
            }
            else
            {
                sySoundPlay(SE_MENU_CANCEL);
                fdScriptWait(10);
            }
        }
        else
        {
            // The prize ring limits the weighted pool of medal rewards.
            totalWeight = 0;
            prizeIndex  = 0;
            if (prizeIndex < result * SPRING_MEDAL_PRIZES_PER_RING + SPRING_MEDAL_PRIZES_PER_RING)
            {
                do
                {
                    // FAKEMATCH(temporary): weight, the prize's weight held for
                    // * the one sum; as the weight added directly.
                    Sint8 weight  = *(routine->holdings.prizeWeights + prizeIndex);

                    totalWeight  += weight;
                    prizeIndex++;
                } while (prizeIndex
                         < result * SPRING_MEDAL_PRIZES_PER_RING + SPRING_MEDAL_PRIZES_PER_RING);
            }
            prizeRoll = syRandomBelow(totalWeight);
            for (prizeIndex = 0; prizeIndex < SPRING_MEDAL_PRIZES; prizeIndex++)
            {
                prizeRoll -= routine->holdings.prizeWeights[prizeIndex];
                if (prizeRoll < 0)
                {
                    break;
                }
            }
            if (prizeIndex == SPRING_MEDAL_PRIZES)
            {
                prizeIndex = SPRING_MEDAL_PRIZES - 1;
            }
            scTolbiSpringRevealPrize(TolbiSpringPrizeItems[prizeIndex]);
            // FAKEMATCH(dead): stepping routine away and back.
            routine++;
            routine--;
            {
                Sint32 slot = prizeIndex + offsetof(MAIN_ROUTINE_STATE, holdings.prizeWeights) - 1;

                if (routine->holdings.prizeWeights[prizeIndex] > 1)
                {
                    // FAKEMATCH(dead): the halving in both arms of a test of
                    // * slot.
                    if (slot)
                        routine->holdings.prizeWeights[prizeIndex] /= 2;
                    else
                        routine->holdings.prizeWeights[prizeIndex] /= 2;
                }
            }
        }
    }
#if LANGUAGE != LANGUAGE_JAPANESE
end:
#endif
    fdScriptEnd();
    return 0;
}

// Chooses which of the three items of a kind the Lucky Wheels attendant hands over, never the one
// she gave last time. A moon wins a kind at random.
static Sint32 scStuccoRoomsCSelectLuckyWheelsPrize(Sint32 category)
{
    Sint32 prevVariant;
    Sint32 variant;

    if (category < 0)
    {
        return 0;
    }

    if (category == LUCKY_WHEELS_PRIZE_ANYTHING)
    {
        category = syRandomBelow(LUCKY_WHEELS_KINDS);
    }

    prevVariant = MainRoutineStateWork.holdings.wonPrizes[LUCKY_WHEELS_LAST_GIVEN + category];

    variant     = (prevVariant + (Sint32)syRandomBelow(2) + 4) % 3;

    MainRoutineStateWork.holdings.wonPrizes[LUCKY_WHEELS_LAST_GIVEN + category] = variant;

    return StuccoRoomsCLuckyWheelsPrizeItems[category * 3 + variant];
}

// Places the actor at the three coordinates and sets its facing, animation speed and colours.
static void scTolbiSpringSetActorAppearance(Sint32        actorId,
                                            const Sint32 *coordinate,
                                            Sint32        facing,
                                            Sint32        colorVariant,
                                            Sint32        animationSpeed)
{
    ACTOR *actor = fdActorGet(actorId);
    SPR   *sprite;
    Uint32 count;

    if (actor != NULL)
    {
        actor->x.fixed       = *coordinate++;
        actor->y.fixed       = *coordinate++;
        actor->z.fixed       = *coordinate;
        actor->facing        = facing;
        actor->motionFlags   = 0;
        actor->sprite->flags = 0;
        fdActorSetAnimationSpeed(actor, animationSpeed);
    }
    // BUG: actor is read even when fdActorGet found none.
    sprite = actor->sprite;
    count  = sprite->count;
    if (count != 0)
    {
        Sint32          noPicture = SPRITE_PICTURE_NONE;
        SPR_ANIMATION **animation = sprite->entries;
        Uint32          left      = count;

        do
        {
            SPR_ANIMATION *entry = *animation++;

            if (entry->colorMode != colorVariant)
            {
                Uint32 picture;

                entry->colorMode = colorVariant;
                // No picture is cached in the new colours yet.
                picture          = entry->picture | noPicture;
                entry->picture   = picture;
            }
        } while (--left != 0);
    }
}

// Sets whether the specified actor draws its sprite.
static void scTolbiSpringSetSpriteStorageKind(Sint32 actorId, Sint32 storageKind)
{
    ACTOR *actor = fdActorGet(actorId);

    if (actor != NULL)
    {
        actor->spriteStorageKind = storageKind;
    }
}

// The coin or Lucky Medal in flight.
static SPRING_TOSS_MOTION Toss ALIGNED(16);

// What is tossed: a coin, or SPRING_TOSS_MEDAL for a Lucky Medal.
static Sint32                  TossKind;

// Three words nothing refers to.
//
// Removing them would move the obstacle state.
static Sint32                  TossUnused[3] UNUSED;

// The two crabs and two turtles moving in the spring.
static SPRING_OBSTACLE         Obstacles[4];

// The ticks since the toss, whether it has come to rest, and the ring it rests in.
static Sint32                  TossTick;
static Sint32                  TossEnded;
static Sint32                  TossResult;

// Each tick of the spring game: the tossed token flies, lands, is drawn toward the centre by the
// rings and bounces off the basin's sloping sides; the crabs and turtles move and knock it away
// when it comes near; and the token, its shadow and its trail are drawn.
static void                    scTolbiSpringUpdateTossSimulation(void)
{
    SPRING_TOSS_MOTION *toss;
    SPRING_OBSTACLE    *obstacle;
    Sint32              index;
    Sint32              height;
    FIXED               edgeInset;
    Sint32              motionStep;
    Sint32              speedMode;
    Sint32              cycleFrame;
    FIXED               minX;
    FIXED               maxX;
    FIXED               minZ;
    FIXED               maxZ;
    // LEFTOVER(frame): an unused local the ROM's frame reserves and never
    // * touches.
    UNUSED VECTOR3      spare;

    toss = &Toss;
    // Retain the previous three positions for the visible toss trail.
    for (index = 3; index != 0; index--)
    {
        toss->trail[index].x = toss->trail[index - 1].x;
        toss->trail[index].y = toss->trail[index - 1].y;
        toss->trail[index].z = toss->trail[index - 1].z;
    }

    // Delay motion until the leader's launch animation has played.
    if (toss->launchFrame > 31)
    {
        toss->trail[0].x += toss->velocity.x;
        height            = toss->trail[0].y + toss->velocity.y;
        toss->trail[0].y  = height;
        toss->trail[0].z += toss->velocity.z;

        if (height <= 0)
        {
            toss->trail[0].y = 0;
            if (toss->velocity.y != 0)
            {
                toss->velocity.y = 0;
                if (TossKind == SPRING_TOSS_MEDAL)
                {
                    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_MEDAL), ANIM_TOKEN_FLOAT);
                }
                else
                {
                    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_COIN), ANIM_TOKEN_FLOAT);
                }
            }

            if (toss->attractionFrame > 0)
            {
                Sint32 deltaX;
                Sint32 deltaZ;
                Sint32 distance;

                deltaX            = (TOLBI_SPRING_CENTER_X - toss->trail[0].x) >> 8;
                deltaZ            = (TOLBI_SPRING_CENTER_Z - toss->trail[0].z) >> 8;
                // BUG: an exact center position leaves a zero divisor below.
                distance          = sySquartIWRAM(deltaX * deltaX + deltaZ * deltaZ);
                toss->velocity.x += ((toFIXED(1) / 10) * deltaX) / distance;
                toss->velocity.z += ((toFIXED(1) / 10) * deltaZ) / distance;
                toss->velocity.x =
                    toss->velocity.x * SPRING_TOKEN_PULLED_DAMPING / SPRING_TOKEN_DAMPING_SCALE;
                toss->velocity.z =
                    toss->velocity.z * SPRING_TOKEN_PULLED_DAMPING / SPRING_TOKEN_DAMPING_SCALE;
                toss->attractionFrame = toss->attractionFrame - 1;
            }
            else
            {
                toss->velocity.x =
                    toss->velocity.x * SPRING_TOKEN_SETTLING_DAMPING / SPRING_TOKEN_DAMPING_SCALE;
                toss->velocity.z =
                    toss->velocity.z * SPRING_TOKEN_SETTLING_DAMPING / SPRING_TOKEN_DAMPING_SCALE;
                if (toss->velocity.x > -toFIXED(1) / 64 && toss->velocity.x < toFIXED(1) / 64)
                {
                    toss->velocity.x = 0;
                }
                if (toss->velocity.z > -toFIXED(1) / 64 && toss->velocity.z < toFIXED(1) / 64)
                {
                    toss->velocity.z = 0;
                }
                if (toss->velocity.x == 0 && toss->velocity.z == 0)
                {
                    if (TossKind == SPRING_TOSS_MEDAL)
                    {
                        fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_MEDAL),
                                               ANIM_TOKEN_REST);
                        scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(1), 0);
                        scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(2), 0);
                        scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(3), 0);
                    }
                    else
                    {
                        fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_COIN),
                                               ANIM_TOKEN_REST);
                        scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(1), 0);
                        scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(2), 0);
                        scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(3), 0);
                    }
                    {
                        Sint32 deltaX = FIXEDtoINT(TOLBI_SPRING_CENTER_X - toss->trail[0].x);
                        Sint32 deltaZ = FIXEDtoINT(TOLBI_SPRING_CENTER_Z - toss->trail[0].z);
                        Sint32 distanceSquared = deltaX * deltaX + deltaZ * deltaZ;

                        // Classify squared pixel distance into the five prize rings.
                        TossEnded              = 1;
                        if (distanceSquared <= 224)
                        {
                            TossResult = 0;
                        }
                        else if (distanceSquared <= 624)
                        {
                            TossResult = 1;
                        }
                        else if (distanceSquared <= 1088)
                        {
                            TossResult = 2;
                        }
                        else if (distanceSquared <= 1680)
                        {
                            TossResult = 3;
                        }
                        else
                        {
                            TossResult = SPRING_RING_OUTSIDE;
                        }
                    }
                }
            }

            // Narrow the bounds near the spring's sloping corners.
            minX = toFIXED(48);
            maxX = toFIXED(192);
            minZ = toFIXED(24);
            maxZ = toFIXED(120);
            if (toss->trail[0].z < toFIXED(42))
            {
                edgeInset = (toFIXED(42) - toss->trail[0].z) * 42 / 18;
                minX      = toFIXED(48) + edgeInset;
                if (minX > toFIXED(90))
                {
                    minX = toFIXED(90);
                }
                maxX = toFIXED(192) - edgeInset;
                if (maxX < toFIXED(150))
                {
                    maxX = toFIXED(150);
                }
            }
            if (toss->trail[0].z > toFIXED(102))
            {
                edgeInset = (toss->trail[0].z * 42 - toFIXED(102) * 42) / 18;
                minX      = toFIXED(48) + edgeInset;
                if (minX > toFIXED(90))
                {
                    minX = toFIXED(90);
                }
                maxX = toFIXED(192) - edgeInset;
                if (maxX < toFIXED(150))
                {
                    maxX = toFIXED(150);
                }
            }
            if (toss->trail[0].x < toFIXED(90))
            {
                edgeInset = (toFIXED(90) - toss->trail[0].x) * 18 / 42;
                minZ      = toFIXED(24) + edgeInset;
                if (minZ > toFIXED(42))
                {
                    minZ = toFIXED(42);
                }
                maxZ = toFIXED(120) - edgeInset;
                if (maxZ < toFIXED(102))
                {
                    maxZ = toFIXED(102);
                }
            }
            if (toss->trail[0].x > toFIXED(150))
            {
                edgeInset = (toss->trail[0].x * 18 - toFIXED(150) * 18) / 42;
                minZ      = toFIXED(24) + edgeInset;
                if (minZ > toFIXED(42))
                {
                    minZ = toFIXED(42);
                }
                maxZ = toFIXED(120) - edgeInset;
                if (maxZ < toFIXED(102))
                {
                    maxZ = toFIXED(102);
                }
            }

            // Reflect outward motion at the boundary, halving the reflected speed.
            if (toss->trail[0].x < minX)
            {
                toss->trail[0].x = minX;
                if (toss->velocity.x < 0)
                {
                    toss->velocity.x = -toss->velocity.x / 2;
                }
            }
            if (toss->trail[0].x > maxX)
            {
                toss->trail[0].x = maxX;
                if (toss->velocity.x > 0)
                {
                    toss->velocity.x = -toss->velocity.x / 2;
                }
            }
            if (toss->trail[0].z < minZ)
            {
                toss->trail[0].z = minZ;
                if (toss->velocity.z < 0)
                {
                    toss->velocity.z = -toss->velocity.z / 2;
                }
            }
            if (toss->trail[0].z > maxZ)
            {
                toss->trail[0].z = maxZ;
                if (toss->velocity.z > 0)
                {
                    toss->velocity.z = -toss->velocity.z / 2;
                }
            }
        }
        else
        {
            toss->velocity.y -= toFIXED(1) / 4;
        }
    }

    // The two crabs scuttle east and west; the two turtles swim round ellipses, the second hiding
    // in its shell for the last quarter of every 512 ticks.
    for (index = 0; index != 4; index++)
    {
        obstacle = &Obstacles[index];

        if (obstacle->reactionFrame > 0)
        {
            obstacle->reactionFrame = obstacle->reactionFrame - 1;
        }
        if (obstacle->hitCooldownFrame > 0)
        {
            obstacle->hitCooldownFrame = obstacle->hitCooldownFrame - 1;
        }

        if (index <= 1)
        {
            speedMode  = obstacle->speedMode;
            motionStep = toFIXED(1);
            if (speedMode == 1)
            {
                motionStep = motionStep * 2;
            }
            if (speedMode == 2)
            {
                motionStep = motionStep * 3;
            }
            if (obstacle->reactionFrame > 0)
            {
                if (index == 0)
                {
                    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE),
                                           ANIM_CRAB_RAISE_CLAWS);
                }
                else
                {
                    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1),
                                           ANIM_CRAB_RAISE_CLAWS);
                }
            }
            else
            {
                if (index == 0)
                {
                    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE),
                                           ANIM_CRAB_SCUTTLE);
                }
                else
                {
                    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1),
                                           ANIM_CRAB_SCUTTLE);
                }
                if (obstacle->pauseOrPhase == 0)
                {
                    if (obstacle->heading == CRAB_HEADING_EAST)
                    {
                        obstacle->pos.x = obstacle->pos.x + motionStep;
                    }
                    else
                    {
                        obstacle->pos.x = obstacle->pos.x - motionStep;
                    }
                    if (obstacle->pos.x <= toFIXED(64))
                    {
                        obstacle->heading = CRAB_HEADING_EAST;
                        if (index == 1)
                        {
                            obstacle->pauseOrPhase = SPRING_CRAB_REST_TICKS;
                        }
                    }
                    if (obstacle->pos.x > (toFIXED(176) - 1))
                    {
                        obstacle->heading = CRAB_HEADING_WEST;
                        if (index == 1)
                        {
                            obstacle->pauseOrPhase = SPRING_CRAB_REST_TICKS;
                        }
                    }
                }
                else
                {
                    obstacle->pauseOrPhase = obstacle->pauseOrPhase - 1;
                }
            }
        }
        else if (index == 2)
        {
            speedMode  = obstacle->speedMode;
            motionStep = -(TRIG_TURN / 1024);
            if (speedMode == 1)
            {
                motionStep = motionStep * 2;
            }
            if (speedMode == 2)
            {
                motionStep = motionStep * 3;
            }
            if (obstacle->reactionFrame > 0)
            {
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2),
                                       ANIM_TURTLE_HIDE);
            }
            else
            {
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2),
                                       ANIM_TURTLE_SWIM);
                obstacle->pos.x        = sySin(obstacle->heading) * 48 + toFIXED(112);
                obstacle->pos.z        = syCos(obstacle->heading) * 40 + toFIXED(72);
                cycleFrame             = (Uint16)obstacle->heading + motionStep;
                obstacle->heading      = cycleFrame;
                obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
            }
        }
        else
        {
            cycleFrame = obstacle->pauseOrPhase & 511;
            speedMode  = obstacle->speedMode;
            motionStep = TRIG_TURN / 1024;
            if (speedMode == 1)
            {
                motionStep = motionStep * 2;
            }
            if (speedMode == 2)
            {
                motionStep = motionStep * 3;
            }
            if (obstacle->reactionFrame > 0)
            {
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                                       ANIM_TURTLE_HIDE);
            }
            else if (cycleFrame <= 383)
            {
                obstacle->pos.x   = sySin(obstacle->heading) * 52 + toFIXED(112);
                obstacle->pos.z   = syCos(obstacle->heading) * 24 + toFIXED(72);
                obstacle->heading = obstacle->heading + motionStep;
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                                       ANIM_TURTLE_SWIM);
            }
            else
            {
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                                       ANIM_TURTLE_HIDE);
            }
            obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
        }

        // A nearby obstacle redirects the token and shortens its attraction time.
        if (obstacle->hitCooldownFrame == 0 && toss->trail[0].y == 0)
        {
            Sint32 deltaX;
            Sint32 deltaZ;
            Sint32 distanceSquared;

            deltaX          = FIXEDtoINT(obstacle->pos.x - toss->trail[0].x);
            deltaZ          = FIXEDtoINT(obstacle->pos.z - toss->trail[0].z);
            distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
            if (distanceSquared <= 119 && toss->attractionFrame > 30)
            {
                FIXED  speed;
                Sint32 nextMode;

                speed = toFIXED(3);
                if (index <= 1)
                {
                    if (obstacle->heading == CRAB_HEADING_EAST)
                    {
                        if (toss->velocity.x < speed)
                        {
                            toss->velocity.x      = speed;
                            toss->attractionFrame = toss->attractionFrame - 100;
                        }
                    }
                    else
                    {
                        if (toss->velocity.x > -speed)
                        {
                            toss->velocity.x      = -speed;
                            toss->attractionFrame = toss->attractionFrame - 100;
                        }
                    }
                }
                else
                {
                    Sint32 distance;

                    // BUG: coincident positions leave a zero divisor below.
                    distance              = sySquartIWRAM(distanceSquared);
                    toss->velocity.x      = -deltaX * speed / distance;
                    toss->velocity.z      = -deltaZ * speed / distance;
                    toss->attractionFrame = toss->attractionFrame - 100;
                }
                sySoundPlay(SE_TOLBI_SPRING_OBSTACLE_HIT);
                nextMode                   = (obstacle->speedMode + 1) % 3;
                obstacle->reactionFrame    = 36;
                obstacle->speedMode        = nextMode;
                obstacle->hitCooldownFrame = 30;
            }
        }

        switch (index)
        {
            case 0:
                scTolbiSpringSetActorAppearance(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE,
                                                syVector3Coordinates(&obstacle->pos),
                                                0,
                                                TolbiSpringCrabFrames[obstacle->speedMode],
                                                obstacle->speedMode * ANIM_STEP_NORMAL
                                                    + ANIM_STEP_NORMAL);
                break;
            case 1:
                scTolbiSpringSetActorAppearance(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1,
                                                syVector3Coordinates(&obstacle->pos),
                                                0,
                                                TolbiSpringCrabFrames[obstacle->speedMode],
                                                obstacle->speedMode * ANIM_STEP_NORMAL
                                                    + ANIM_STEP_NORMAL);
                break;
            case 2:
                scTolbiSpringSetActorAppearance(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2,
                                                syVector3Coordinates(&obstacle->pos),
                                                DEGtoANG(180) - obstacle->heading,
                                                TolbiSpringCircleFrames[obstacle->speedMode],
                                                obstacle->speedMode * ANIM_STEP_NORMAL
                                                    + ANIM_STEP_NORMAL);
                break;
            case 3:
                scTolbiSpringSetActorAppearance(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3,
                                                syVector3Coordinates(&obstacle->pos),
                                                TRIG_TURN - 1 - obstacle->heading,
                                                TolbiSpringCircleFrames[obstacle->speedMode],
                                                obstacle->speedMode * ANIM_STEP_NORMAL
                                                    + ANIM_STEP_NORMAL);
                break;
        }
    }

    toss->shadow.x = toss->trail[0].x;
    toss->shadow.y = 0;
    toss->shadow.z = toss->trail[0].z;

    if (TossKind == SPRING_TOSS_MEDAL)
    {
        scTolbiSpringSetActorAppearance(ACTOR_TOLBI_SPRING_MEDAL,
                                        syVector3Coordinates(&toss->trail[0]),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(ACTOR_TOLBI_SPRING_MEDAL_SHADOW,
                                        syVector3Coordinates(&toss->shadow),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(1),
                                        syVector3Coordinates(&toss->trail[1]),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(2),
                                        syVector3Coordinates(&toss->trail[2]),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(3),
                                        syVector3Coordinates(&toss->trail[3]),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        fdActorSelectAnimation(fdActorGet(TOLBI_SPRING_MEDAL_TRAIL(1)), ANIM_TOKEN_GLINT);
        fdActorSelectAnimation(fdActorGet(TOLBI_SPRING_MEDAL_TRAIL(2)), ANIM_TOKEN_GLINT);
        fdActorSelectAnimation(fdActorGet(TOLBI_SPRING_MEDAL_TRAIL(3)), ANIM_TOKEN_GLINT);
    }
    else
    {
        scTolbiSpringSetActorAppearance(
            ACTOR_TOLBI_SPRING_COIN, syVector3Coordinates(&toss->trail[0]), 0, 0, ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(ACTOR_TOLBI_SPRING_COIN_SHADOW,
                                        syVector3Coordinates(&toss->shadow),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(TOLBI_SPRING_COIN_TRAIL(1),
                                        syVector3Coordinates(&toss->trail[1]),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(TOLBI_SPRING_COIN_TRAIL(2),
                                        syVector3Coordinates(&toss->trail[2]),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        scTolbiSpringSetActorAppearance(TOLBI_SPRING_COIN_TRAIL(3),
                                        syVector3Coordinates(&toss->trail[3]),
                                        0,
                                        0,
                                        ANIM_STEP_NORMAL);
        fdActorSelectAnimation(fdActorGet(TOLBI_SPRING_COIN_TRAIL(1)), ANIM_TOKEN_GLINT);
        fdActorSelectAnimation(fdActorGet(TOLBI_SPRING_COIN_TRAIL(2)), ANIM_TOKEN_GLINT);
        fdActorSelectAnimation(fdActorGet(TOLBI_SPRING_COIN_TRAIL(3)), ANIM_TOKEN_GLINT);
    }

    if (toss->launchFrame != -1)
    {
        toss->launchFrame = toss->launchFrame + 1;
    }
}

// Puts the crabs and turtles at their starting places, the token out of sight, and starts the
// spring game's ticks.
static void scTolbiSpringInitTossSimulation(void)
{
    SPRING_TOSS_MOTION *toss          = &Toss;
    Sint32              obstacleIndex = 0;
    const Uint8        *pixelX;
    const Uint16       *headings;
    SPRING_OBSTACLE    *obstacle;
    const Uint8        *pixelZ;

    pixelX   = TolbiSpringActorPixelX;
    obstacle = Obstacles;
    headings = TolbiSpringActorHeadings;
    pixelZ   = TolbiSpringActorPixelZ;

    do
    {
        obstacle->pos.x            = toFIXED(*pixelX);
        obstacle->pos.z            = toFIXED(*pixelZ);
        obstacle->pos.y            = 0;
        obstacle->heading          = *headings;
        obstacle->pauseOrPhase     = 0;
        obstacle->speedMode        = 0;
        obstacle->reactionFrame    = 0;
        obstacle->hitCooldownFrame = 0;

        obstacleIndex++;
        pixelX++;
        pixelZ++;
        headings++;
        obstacle++;
    } while (obstacleIndex != 4);

    toss->trail[0].x      = -toFIXED(30);
    toss->trail[0].y      = 0;
    toss->trail[0].z      = toFIXED(100);
    toss->velocity.x      = 0;
    toss->velocity.y      = 0;
    toss->velocity.z      = 0;
    toss->attractionFrame = 0;

    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2), ANIM_TURTLE_SWIM);
    fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3), ANIM_TURTLE_SWIM);
    sySetTickEvent(scTolbiSpringUpdateTossSimulation, EVENT_PASS_TICK + 3);
}

// Isaac tosses the chosen coin or medal over his shoulder; returns the ring it settles in.
static Sint32 scTolbiSpringRunToss(Sint32 tossKind)
{
    SPRING_TOSS_MOTION *toss;

    toss              = &Toss;
    toss->trail[0].y  = 0;
    toss->trail[1].y  = 0;
    toss->trail[2].y  = 0;
    toss->trail[3].y  = 0;
    TossKind          = tossKind;
    TossEnded         = 0;
    toss->launchFrame = -1;
    for (TossTick = 0;; TossTick++)
    {
        if (TossTick == 50)
        {
            sySoundPlay(SE_TOLBI_SPRING_TOSS);
        }
        if (TossTick == 16)
        {
            fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_LEADER_TOSS);
            toss->launchFrame     = 0;
            toss->velocity.x      = toFIXED(13) / 10;
            toss->velocity.y      = toFIXED(4);
            toss->velocity.z      = -toFIXED(2);
            toss->trail[0].x      = toFIXED(120);
            toss->trail[0].y      = toFIXED(16);
            toss->trail[0].z      = toFIXED(152);
            toss->attractionFrame = 300;
            if (TossKind == SPRING_TOSS_MEDAL)
            {
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_MEDAL_SHADOW),
                                       ANIM_TOKEN_SHADOW);
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_MEDAL), ANIM_TOKEN_TUMBLE);
                scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(1), 1);
                scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(2), 1);
                scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(3), 1);
            }
            else
            {
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_COIN_SHADOW),
                                       ANIM_TOKEN_SHADOW);
                fdActorSelectAnimation(fdActorGet(ACTOR_TOLBI_SPRING_COIN), ANIM_TOKEN_TUMBLE);
                scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(1), 1);
                scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(2), 1);
                scTolbiSpringSetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(3), 1);
            }
        }
        sySynch(1);
        if (TossEnded == 1)
        {
            break;
        }
    }
    return TossResult;
}

const SCENE_ENTRANCE TolbiSpringEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SPRING_FROM_TOLBI, CONDITION_ALWAYS, 120, 0, 168, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {STUCCO_ROOMS_C_LUCKY_DICE_FROM_TOLBI,
     CONDITION_ALWAYS, 128,
     0, 176,
     FACING_NORTH, 0,
     8, 8,
     248, 200,
     0},
    {STUCCO_ROOMS_C_LUCKY_WHEELS_FROM_TOLBI,
     CONDITION_ALWAYS, 96,
     0, 440,
     FACING_NORTH, 0,
     8, 288,
     248, 456,
     0},
    {STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS,
     CONDITION_ALWAYS, 144,
     0, 390,
     FACING_NORTH, 0,
     8, 288,
     248, 456,
     0},
    {STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE,
     CONDITION_ALWAYS, 120,
     0, 152,
     FACING_WEST, 0,
     8, 8,
     248, 200,
     0},
    {SCENE_TABLE_END},
};

const Uint32 TolbiSpringExits[] = {
    SCENE_EXITS(TolbiSpring),
    SCENE_EXIT_UNLESS(TOLBI_SPRING_TO_TOLBI, Tolbi, TOLBI_FROM_TOLBI_SPRING, EXIT_FLAG_NONE),
    SCENE_EXITS(StuccoRoomsC),
    SCENE_EXIT_UNLESS(
        STUCCO_ROOMS_C_LUCKY_DICE_TO_TOLBI, Tolbi, TOLBI_FROM_LUCKY_DICE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        STUCCO_ROOMS_C_LUCKY_WHEELS_TO_TOLBI, Tolbi, TOLBI_FROM_LUCKY_WHEELS, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
const ACTOR_PLACE TolbiSpringActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        176, 0, 144, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        72, 0, 144, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        176, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        72, 0, 168, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        48, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        24, 0, 72, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 96, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

const ACTOR_PLACE StuccoRoomsCActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        144, 0, 360, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        120, 0, 82, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 82, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        88, 0, 128, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        96, 0, 168, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 104, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        96, 0, 336, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 392, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        160, 0, 112, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        184, 0, 152, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        80, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

const CUE StuccoRoomsCTriggers[] = {
    { CUE_EXIT, STUCCO_ROOMS_C_LUCKY_DICE_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_C_LUCKY_DICE_TO_TOLBI },
    { CUE_EXIT, STUCCO_ROOMS_C_LUCKY_WHEELS_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_C_LUCKY_WHEELS_TO_TOLBI },
    { CUE_TALK, ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsCTalkToWheelsAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_WHEELS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringHmmIWonderIfTheseKids) },
    { CUE_TALK, ACTOR_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsCOfferLuckyDice) },
    { CUE_TALK, ACTOR_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringReadTheSignIfYaDont) },
    { CUE_TALK, ACTOR_LUCKY_DICE_PLAYER_FIRST_TIME, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsCAskAboutFirstDiceGame) },
    { CUE_TALK, ACTOR_LUCKY_DICE_PLAYER_TRIPLE_DIGITS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWhoaGettingTripleDigitsIsReal) },
    { CUE_TALK, ACTOR_LUCKY_DICE_PLAYER_OUT_OF_COINS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringIDontWantToStopPlaying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWinningOrLosingIsPureLuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTheRulesForLuckyDiceAint) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_PLAYER_FIRST_TIME, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringSometimesPeopleWhoHaveNeverPlayed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_PLAYER_TRIPLE_DIGITS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringThinkingWontHelpYouWinThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_PLAYER_OUT_OF_COINS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringMyLuckHasRunOutFor) },
    { CUE_TALK, ACTOR_LUCKY_WHEELS_PRIZE_SIGN, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsCReadLuckyWheelsPrizes) },
    { CUE_TALK, ACTOR_LUCKY_WHEELS_RULES_SIGN, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsCReadLuckyWheelsRules) },
    { CUE_TALK, ACTOR_LUCKY_DICE_RULES_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringLuckyDiceThrowTheDiceOn) },
    { CUE_TALK, ACTOR_LUCKY_DICE_PAIRS_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringLuckyDiceIfTwoNumbersMatch) },
    { CUE_TALK, ACTOR_LUCKY_DICE_SOLDIER, FLAG_BABI_SAVED, MSGNO(MsgTolbiSpringYeahThatDiceGameLooksKinda) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_SOLDIER, FLAG_BABI_SAVED, MSGNO(MsgTolbiSpringILikePlayingLuckyDiceBut) },
    { CUE_TALK, ACTOR_LUCKY_DICE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringOhThereAreAlwaysScalpersAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_DICE_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTheresNoWayPoorSoldiersLike) },
    { CUE_TABLE_END },
};

const CUE TolbiSpringCues[] = {
    { CUE_EXIT, TOLBI_SPRING_TO_TOLBI, CONDITION_ALWAYS, TOLBI_SPRING_TO_TOLBI },
    // XXX: TolbiSpringExits has no exit 2 and no entrance 2 is named; whether a metatile of the spring's map carries behavior 2 would settle what it was.
    { CUE_EXIT, 2, CONDITION_ALWAYS, 2 },
    { CUE_TALK, ACTOR_TOLBI_SPRING_MEDAL_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiTossLuckyMedal) },
    { CUE_TALK, ACTOR_TOLBI_SPRING_COIN_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiFaceAwayTolbi) },
    { CUE_TALK, ACTOR_TOLBI_SPRING_VISITOR_MYSTERIOUS_POWERS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTolbiSpringHasMysteriousPowersStand) },
    { CUE_TALK, ACTOR_TOLBI_SPRING_MEDAL_SEEKER, CONDITION_ALWAYS, SCRIPT_REF(scTolbiSpringAskAboutLuckyMedal) },
    { CUE_TALK, ACTOR_TOLBI_SPRING_VISITOR_ONE_MEDAL, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTooBadIOnlyHaveOne) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR_MYSTERIOUS_POWERS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringIGetNervousWhenIToss) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_MEDAL_SEEKER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWhyWontAnyoneGiveMeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR_ONE_MEDAL, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringThisIsALuckyMedalSo) },
    { EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_TOLBI_SPRING_WATER, CONDITION_ALWAYS, SCRIPT_REF(scTolbiSpringTossCoinOrMedal) },
    { CUE_TABLE_END },
};

// The water's fade as the spring drains, from mostly water to none, one step every eighth tick.
const Uint16 TolbiSpringAlphaSteps[] = {
    BLEND_ALPHA(10, 9), BLEND_ALPHA(8, 10), BLEND_ALPHA(6, 11), BLEND_ALPHA(4, 12),
    BLEND_ALPHA(3, 13), BLEND_ALPHA(2, 14), BLEND_ALPHA(1, 15), BLEND_ALPHA(0, 16),
};

// The prizes a Lucky Medal can win, SPRING_MEDAL_PRIZES_PER_RING to a ring, one drawn by its weight.
const Uint32 TolbiSpringPrizeItems[] = {
    ITEM_ASSASSIN_BLADE, ITEM_EARTH_SHIELD, ITEM_SPIRIT_ARMOR, ITEM_ADEPTS_HELM, ITEM_BURNING_AXE,
    ITEM_COCKTAIL_DRESS, ITEM_GLITTERING_TIARA, ITEM_GUARDIAN_ARMLET, ITEM_GRIEVOUS_MACE,
    ITEM_NINJA_HOOD, ITEM_BATTLE_GLOVES, ITEM_KIMONO, ITEM_POTION, ITEM_PSY_CRYSTAL,
    ITEM_WATER_OF_LIFE,
};

// The coins a tossed coin wins, by the ring it rests in.
const Uint16 TolbiSpringCoinPrizes[] = {
    20, 10, 2, 1, 0, 0,
};

// What Lucky Wheels wins, three items to each of its LUCKY_WHEELS_KINDS kinds.
const Uint32 StuccoRoomsCLuckyWheelsPrizeItems[] = {
    ITEM_MYTHRIL_SHIRT, ITEM_SILK_SHIRT, ITEM_RUNNING_SHIRT, ITEM_HYPER_BOOTS, ITEM_QUICK_BOOTS, ITEM_FUR_BOOTS,
    ITEM_ADEPT_RING, ITEM_WAR_RING, ITEM_SLEEP_RING, ITEM_POTION, ITEM_VIAL, ITEM_NUT,
    ITEM_WATER_OF_LIFE, ITEM_PSY_CRYSTAL, ITEM_ELIXIR,
};

// The crabs' animation for each speed, and the turtles'.
const Uint8 TolbiSpringCrabFrames[] = {
    0, 1, 9,
};
const Uint8 TolbiSpringCircleFrames[] = {
    0, 3, 4,
};

// Where each obstacle starts, in pixels, and its heading.
const Uint8 TolbiSpringActorPixelX[] = {
    80, 160, 80, 72,
};
const Uint8 TolbiSpringActorPixelZ[] = {
    32, 104, 68, 72,
};
const Uint16 TolbiSpringActorHeadings[] = {
    CRAB_HEADING_EAST, CRAB_HEADING_WEST, FACING_EAST, FACING_WEST,
};
// clang-format on

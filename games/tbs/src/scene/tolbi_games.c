/// @file
/// Tolbi's three games of chance, which the party can play whenever it is in
/// town: Lucky Dice, Lucky Wheels and Tolbi Spring. None of them is part of
/// the story.
///
/// Lucky Dice and Lucky Wheels are two rooms of one map (SceneId_StuccoRoomsC,
/// whose area name is Tolbi), each with its own camera box.
///
/// - Lucky Dice is played in the small tent with a die on its front. Two
///   fair-haired young men stand behind the counter at the back. The one on
///   the left asks ten coins for each level of the party's average level and,
///   once paid, sends Isaac to the green table to throw. When the game ends
///   he says how many coins were won or lost, with a fanfare that grows with
///   the win. Boards on the wall give the rules, and three players, an old
///   man with a cane, a burly grey-haired man and a woman in green, talk of
///   their luck. A soldier talks of the game until Babi is saved
///   (FLAG_BABI_SAVED, the finals day), then of the scalpers at the Colosso
///   finals, and is gone once Colosso is over (FLAG_COLOSSO_OVER).
/// - Lucky Wheels, the slot machine, is played in the room up the stairs
///   beside the inn. The young woman in green behind its curved counter
///   takes Game Tickets, and warns when the bag has six free slots or fewer.
///   When the game ends she hands over an item for each line won, one of the
///   three of its kind (shirts, boots, rings, or stars and hearts for items;
///   a moon wins any kind), never the one she gave last time; or, when
///   nothing was won, offers another game. Green boards by the counter give
///   the rules and the prizes.
///
/// Tolbi Spring (SceneId_TolbiSpring) is the fountain in the middle of
/// Tolbi's square, reached through the gate in its railing. Isaac stands on
/// the white step between its two signs and searches the spring. The first
/// time each visit its water sinks and fades away, uncovering a target of
/// rings on the basin's floor with two crabs and two turtles moving about on
/// it. He turns his back to the spring and tosses a coin or a Lucky Medal
/// into it. A crab or turtle the token lands beside knocks it away, and the
/// rings pull it toward the centre until it settles in one of five. A coin wins
/// 20, 10, 2 or 1 coins, from the centre out, or nothing in the fifth. A medal
/// always wins an item, drawn by weight from fifteen prizes, the nearer the
/// centre the fewer and better (Assassin Blade first, Water of Life last), and
/// the item won has its weight halved for later tosses. Where the token
/// settles is measured from TOLBI_SPRING_CENTER_X and _Z; the rings end at
/// squared distances of 224, 624, 1,088 and 1,680 square pixels, which match
/// the rings painted on the basin's floor. In the editions before
/// TBS_PATCH_1 the coin and medal counts show an `=` after each number.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/map_grid_scroll.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "game/inventory.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "shop/shop.h"
#include "sound/sound.h"
#include "system/heap_slots.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/system.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

s32 Scene_Initialize(void);
extern const struct SceneEntrance gTolbiSpring_Entrances[];
extern const u32 gTolbiSpring_Exits[];
extern const struct ActorPlacement gTolbiSpringActorPlacements[];
extern const struct ActorPlacement gTolbiSpringActorPlacementsOther[];
extern const struct Trigger gTolbiSpringTriggersOther[];
extern const struct Trigger gTolbiSpringTriggers[];
extern const u16 gTolbiSpring_AlphaSteps[];
extern const u32 gTolbiSpring_PrizeItems[];
extern const u16 gTolbiSpring_CoinPrizes[];
extern const u32 gTolbiSpring_LuckyWheelsPrizeItems[];
extern const u8 gTolbiSpring_SliderFrames[];
extern const u8 gTolbiSpring_CircleFrames[];
extern const u8 gTolbiSpring_ActorPixelX[];
extern const u8 gTolbiSpring_ActorPixelZ[];
extern const u16 gTolbiSpring_ActorHeadings[];

/// The spring's water has sunk and faded away this visit.
#define FLAG_TOLBI_SPRING_DRAINED FLAG_SCENE_TEMP_00

/// The party's coins as Lucky Dice starts, kept in gSceneLocalState so the
/// attendant can tell the change when the party comes back.
#define sLuckyDiceCoinsBefore (*(s32 *)gSceneLocalState)

// UB: the coin prizes are read through a one-element view of their table;
// UBFIX reads the table.
#ifdef UBFIX
#define TOLBI_SPRING_COIN_PRIZE(ring) gTolbiSpring_CoinPrizes[ring]
#else
#define TOLBI_SPRING_COIN_PRIZE(ring)                                          \
  (((struct { u16 coins[1]; } *)gTolbiSpring_CoinPrizes)->coins[ring])
#endif

static void TolbiSpring_DrainSpring(void);
static void TolbiSpring_OfferLuckyWheels(s32 spoken);
static s32 TolbiSpring_RunSpringGame(void);
static s32 TolbiSpring_SelectLuckyWheelsPrize(s32 category);
static void TolbiSpring_InitializeTossSimulation(void);
static s32 TolbiSpring_RunToss(s32 tossKind);

/// What the player throws into the spring, chosen by Menu_SelectCoinsOrMedals.
enum SpringTossKind { SPRING_TOSS_COIN, SPRING_TOSS_MEDAL };

/// The people and boards of the Lucky Dice tent and the Lucky Wheels room,
/// numbered in the order of gTolbiSpringActorPlacementsOther from
/// ACTOR_FIRST_PLACED.
enum TolbiSpringGameRoomActorId {
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) behind the curved
  /// counter of the Lucky Wheels room, who runs the slot machine
  /// (TolbiSpring_TalkToWheelsAttendant).
  ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT = ACTOR_FIRST_PLACED,
  /// A young man with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR), the left
  /// of the two behind the Lucky Dice counter, who runs the game
  /// (TolbiSpring_OfferLuckyDice).
  ACTOR_TOLBI_SPRING_DICE_ATTENDANT,
  /// The other young man behind the Lucky Dice counter, on the right, who
  /// points the party to the rules: `Read the sign if ya don't know the
  /// rules.`
  ACTOR_TOLBI_SPRING_DICE_RULES_ATTENDANT,
  /// A Lucky Dice player, a burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR) at the west end of the yellow rug.
  ACTOR_TOLBI_SPRING_DICE_PLAYER_FIRST_TIME,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE) by the green table.
  ACTOR_TOLBI_SPRING_DICE_PLAYER_TRIPLE_DIGITS,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS) by the east
  /// wall.
  ACTOR_TOLBI_SPRING_DICE_PLAYER_OUT_OF_COINS,
  /// The green board of Lucky Wheels prizes north-west of the counter. The map
  /// draws it; an unseen actor (SPRITE_BLANK) stands on it to be read
  /// (TolbiSpring_ReadLuckyWheelsPrizes).
  ACTOR_TOLBI_SPRING_WHEELS_PRIZE_SIGN,
  /// The green board of Lucky Wheels rules east of the counter, read the same
  /// way (TolbiSpring_ReadLuckyWheelsRules).
  ACTOR_TOLBI_SPRING_WHEELS_RULES_SIGN,
  /// The Lucky Dice rules, on a green board east of the counter, read the
  /// same way.
  ACTOR_TOLBI_SPRING_DICE_RULES_SIGN,
  /// A soldier by the red rug in the Lucky Dice tent, gone after Colosso, drawn
  /// with the long-haired fighter's
  /// sprite (SPRITE_PERSON_WITH_SWORD).
  ACTOR_TOLBI_SPRING_SOLDIER,
  /// The Lucky Dice rules for pairs, on a green board west of the counter.
  ACTOR_TOLBI_SPRING_DICE_PAIRS_SIGN
};

/// Tolbi Spring's own actors (TolbiSpring), numbered in the order of
/// gTolbiSpringActorPlacements from ACTOR_FIRST_PLACED.
///
/// The toss draws the coin or the medal with its shadow and three trailing
/// copies, which TOLBI_SPRING_COIN_TRAIL and TOLBI_SPRING_MEDAL_TRAIL number.
enum TolbiSpringActorId {
  ACTOR_TOLBI_SPRING_COIN_SHADOW = ACTOR_FIRST_PLACED + 3,
  /// The coin the party tosses (SPRITE_SPRING_COIN).
  ACTOR_TOLBI_SPRING_COIN,
  ACTOR_TOLBI_SPRING_MEDAL_SHADOW = ACTOR_TOLBI_SPRING_COIN + 4,
  /// The Lucky Medal the party tosses (SPRITE_SPRING_LUCKY_MEDAL).
  ACTOR_TOLBI_SPRING_MEDAL,
  /// The four obstacles that move in the spring during a toss, in the order
  /// of sObstacles.
  ///
  /// They are two crabs (SPRITE_CRAB), then two turtles (SPRITE_TURTLE).
  ACTOR_TOLBI_SPRING_FIRST_OBSTACLE,
  /// The sign east of the white step, on Lucky Medals (SPRITE_SIGNPOST).
  ACTOR_TOLBI_SPRING_MEDAL_SIGN = ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 4,
  /// The sign west of the white step, on tossing a coin.
  ACTOR_TOLBI_SPRING_COIN_SIGN,
  /// The tufts of grass at the foot of each sign.
  ///
  /// They are the signs' sprite in its second frame, mirrored and drawn
  /// underfoot (Scene_Initialize).
  ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS,
  ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS,
  /// A visitor carrying a pack (SPRITE_MAN_CARRYING_A_PACK) west of the
  /// signs, who explains the game.
  ACTOR_TOLBI_SPRING_VISITOR_MYSTERIOUS_POWERS,
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) by the spring's
  /// north-west corner, who wishes he had a Lucky Medal.
  ACTOR_TOLBI_SPRING_MEDAL_SEEKER,
  /// A visitor, a woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) east of
  /// the spring, who has only one Lucky Medal.
  ACTOR_TOLBI_SPRING_VISITOR_ONE_MEDAL
};

/// Animations of the spring's sprites, as their animation tables and frames
/// under graphics/sprite draw them. The coin's and the Lucky Medal's sheets
/// (spring_coin.png, spring_lucky_medal.png) are drawn alike.
enum TolbiSpringAnimation {
  ANIM_TOKEN_TUMBLE = 0, ///< turning over once as it is tossed
  ANIM_TOKEN_FLOAT = 1,  ///< turning as it drifts toward the rings
  ANIM_TOKEN_REST = 2,   ///< lying still
  ANIM_TOKEN_SHADOW = 3, ///< a grey spot on the ground below it
  /// A twinkle, as each of the three marks trailing a flying token.
  ANIM_TOKEN_GLINT = 4,
  ANIM_CRAB_SCUTTLE = 1,     ///< stepping sideways (crab.png)
  ANIM_CRAB_RAISE_CLAWS = 3, ///< snapping its raised claws (crab.png)
  ANIM_TURTLE_SWIM = 2,      ///< paddling (turtle.png)
  ANIM_TURTLE_HIDE = 3,      ///< head and flippers drawn in (turtle.png)
  /// The signpost sheet's second picture, a tuft of grass (signpost.png).
  ANIM_SIGN_GRASS = 2
};

/// The actor that draws toss->trail[n] of a tossed coin or medal, n from
/// 1 to 3.
///
/// They are the rows before each shadow, the oldest copy first.
#define TOLBI_SPRING_COIN_TRAIL(n) (ACTOR_TOLBI_SPRING_COIN_SHADOW - (n))
#define TOLBI_SPRING_MEDAL_TRAIL(n) (ACTOR_TOLBI_SPRING_MEDAL_SHADOW - (n))

/// The two cells searched to toss into Tolbi Spring from the white step at
/// cell (7, 9): the spring's rim north of it, cell (7, 8), and the ground
/// south of it, cell (7, 10), in front of Isaac when he stands with his back
/// to the spring.
enum TolbiSpringCellBehavior {
  CELL_BEHAVIOR_TOLBI_SPRING_WATER = 80,
};

/// Lucky Dice costs this many coins for each level of the party's average
/// level; a win of LUCKY_DICE_MEDIUM_WIN coins or more, or of
/// LUCKY_DICE_LARGE_WIN, is greeted with a bigger jingle.
enum {
  LUCKY_DICE_COINS_PER_LEVEL = 10,
  LUCKY_DICE_MEDIUM_WIN = 5000,
  LUCKY_DICE_LARGE_WIN = 20000
};

/// What a line of Lucky Wheels wins, as the slot machine reports it: one of
/// LUCKY_WHEELS_KINDS kinds of prize, three items each in
/// gTolbiSpring_LuckyWheelsPrizeItems, or a moon, which wins any kind.
enum { LUCKY_WHEELS_KINDS = 5, LUCKY_WHEELS_MOON = 5 };

/// The pixels the spring's water sinks before it fades, one every fourth
/// frame.
enum { SPRING_WATER_SINK_PIXELS = 16 };

/// A medal's prizes (MainRoutineWork's prizeWeights): its ring allows three
/// more for each ring out from the centre.
enum { SPRING_MEDAL_PRIZES = 15, SPRING_MEDAL_PRIZES_PER_RING = 3 };

/// The second crab rests this many frames at each end of its run.
enum { SPRING_CRAB_REST_FRAMES = 30 };

/// The token's velocity keeps SPRING_TOKEN_PULLED_DAMPING /
/// SPRING_TOKEN_DAMPING_SCALE of itself each frame while the rings pull it,
/// and SPRING_TOKEN_SETTLING_DAMPING / SPRING_TOKEN_DAMPING_SCALE once they let
/// go.
enum {
  SPRING_TOKEN_PULLED_DAMPING = 253,
  SPRING_TOKEN_SETTLING_DAMPING = 220,
  SPRING_TOKEN_DAMPING_SCALE = 256
};

/// The spring's centre, which the prize rings pull the tossed token toward.
#define TOLBI_SPRING_CENTER_X toFIXED(120)
#define TOLBI_SPRING_CENTER_Z toFIXED(71)

// The window of the party's coins and Lucky Medals, and the menu of what to
// toss, in window cells, with the counts' x in pixels: the European editions'
// longer words widen both and move the counts right.
#if LANGUAGE == LANGUAGE_ENGLISH || LANGUAGE == LANGUAGE_JAPANESE
#define TOLBI_SPRING_COUNT_WIDTH 17
#else
#define TOLBI_SPRING_COUNT_WIDTH 19
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define TOLBI_SPRING_COUNT_NUMBER_X 56
#define TOLBI_SPRING_TOSS_MENU_X 22
#define TOLBI_SPRING_TOSS_MENU_WIDTH 7
#elif LANGUAGE == LANGUAGE_ENGLISH
#define TOLBI_SPRING_COUNT_NUMBER_X 72
#define TOLBI_SPRING_TOSS_MENU_X 20
#define TOLBI_SPRING_TOSS_MENU_WIDTH 9
#else
#define TOLBI_SPRING_COUNT_NUMBER_X 88
#define TOLBI_SPRING_TOSS_MENU_X 19
#define TOLBI_SPRING_TOSS_MENU_WIDTH 10
#endif

/// The tossed coin or medal, as its last four positions for the trail, its
/// velocity and the frames the prize rings still pull it.
struct SpringTossMotion {
  u8 unused[2]; ///< nothing reads or writes it
  s16 launchFrame;
  struct Vector3 trail[4]; ///< 16.16 fixed-point pixels
  struct Vector3 shadow;
  struct Vector3 velocity; ///< 16.16 fixed-point pixels per frame
  s32 attractionFrames;
};

/// One of the four obstacles moving in the spring during a toss.
struct SpringObstacle {
  struct Vector3 position; ///< 16.16 fixed-point pixels
  s16 heading;
  s16 pauseOrPhase;
  s16 speedMode;
  s16 reactionFrames;
  s16 hitCooldownFrames;
};

static inline void Io_SetBlendControl(s32 value) { REG_BLDCNT = value; }

static inline void Io_SetBlendAlpha(s32 value) { REG_BLDALPHA = value; }

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gTolbiSpring_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gTolbiSpring_Exits; }

/// Selects the actors of Tolbi Spring or of the Lucky Dice and Lucky
/// Wheels rooms.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_TOLBI_SPRING) {
    return gTolbiSpringActorPlacements;
  }
  return gTolbiSpringActorPlacementsOther;
}

/// The man with a pack by Tolbi Spring's north-west corner asks whether Isaac
/// has a Lucky Medal.
///
/// If he has, the man says it will be extra lucky tossed in the spring; if
/// not, that he would want one himself to try his luck.
static void TolbiSpring_AskAboutLuckyMedal(s32 actorId) {
  s32 question = MsgTolbiLuckyMedal;
  Event_SetMessageId(question);
  // "Do you have a Lucky Medal?"
  Event_OpenMessage(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(question + 1);
  } else {
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessage(actorId, 0);
}

/// The burly grey-haired man at the west end of the yellow rug in the Lucky
/// Dice tent asks whether this is Isaac's first time to play.
///
/// Yes, and he warns that once you start you won't want to stop; no, and he
/// says Lucky Dice is all the rage in Tolbi.
static void TolbiSpring_AskAboutFirstDiceGame(s32 actorId) {
  s32 question = MsgTolbiYerFirstTime;
  Event_SetMessageId(question);
  // "Is this yer first time to play Lucky Dice?"
  Event_OpenMessage(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(question + 1);
  } else {
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessage(actorId, 0);
}

/// Isaac searches Tolbi Spring from the white step between its signs, facing
/// the spring or with his back to it (cell (7, 8) or (7, 10)), to toss a coin
/// or a Lucky Medal.
///
/// The first time each visit, the water sinks and fades away to uncover the
/// target of rings on the basin's floor (TolbiSpring_DrainSpring). He then
/// stands on the step with his back to the spring, and the party chooses what
/// to toss (TolbiSpring_RunSpringGame).
static void TolbiSpring_TossCoinOrMedal(void) {
  s32 leader = gMainRoutineWork.leaderActorId;

  if (Event_GetFlag(FLAG_TOLBI_SPRING_DRAINED) == 0) {
    GameFlag_Set(FLAG_TOLBI_SPRING_DRAINED);
    TolbiSpring_DrainSpring();
  }
  Event_Start();
  Actor_WalkToAndWait(leader, 120, 152);
  Actor_FaceDirection(leader, FACING_SOUTH, 0);
  TolbiSpring_RunSpringGame();
  Event_End();
}

/// Isaac reads the green board east of the Lucky Wheels counter: pull the
/// lever up to five times to line up matching icons, press the star to hold
/// a wheel, and play 1, 3, 5 or 7 lines for 1 to 4 Game Tickets.
static void TolbiSpring_ReadLuckyWheelsRules(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiLuckyWheelsRulesPullLever);
  // "Lucky Wheels Rules Pull the lever and try to line up..."
  Event_OpenMessage(-1, 0);
  Event_End();
}

/// Isaac reads the green board north-west of the Lucky Wheels counter: shirts
/// win undershirts, rings win rings, boots win boots, stars and hearts win
/// items, and moons can win anything.
static void TolbiSpring_ReadLuckyWheelsPrizes(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiLuckyWheelsPrizesPrizesDetermined);
  // "Lucky Wheels Prizes Prizes are determined by which icons you..."
  Event_OpenMessage(-1, 0);
  Event_End();
}

/// The young woman behind the Lucky Wheels counter invites Isaac to play,
/// and if he has Game Tickets and agrees, sends him to the slot machine.
///
/// Without tickets she tells him he needs them to play. See
/// TolbiSpring_OfferLuckyWheels.
static void TolbiSpring_TalkToWheelsAttendant(void) {
  TolbiSpring_OfferLuckyWheels(0);
}

/// Selects the triggers of Tolbi Spring or of the Lucky Dice and Lucky Wheels
/// rooms.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_TOLBI_SPRING) {
    return gTolbiSpringTriggers;
  }
  return gTolbiSpringTriggersOther;
}

/// The setup of both scenes, and what the attendants say when a game ends.
///
/// Entering Tolbi Spring readies the crabs and turtles, and keeps the water
/// hidden if it was drained earlier this visit. Back in the Lucky Dice tent
/// after a game, Isaac steps up to the counter and the attendant says how many
/// coins were won or lost, to a fanfare that grows with the win. Back from
/// the slot machine, the Lucky Wheels attendant says `Congratulations!` and
/// hands over an item for each line won, or offers another game when nothing
/// was won.
s32 Scene_Initialize(void) {
  s32 coinChange;
  s8 *wonPrizes;
  s32 prizeIndex;
  s32 item;

  if (gMainRoutineWork.next == SCENE_TOLBI_SPRING) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
               &REG_BLDCNT);
    Io_Write16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
    Actor_SetAnimation(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS, ANIM_SIGN_GRASS);
    Actor_SetAnimation(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS, ANIM_SIGN_GRASS);
    Actor_Get(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS)->scaleX = -FIXED_ONE;
    Actor_Get(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS)->scaleX = -FIXED_ONE;
    Actor_Get(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_Get(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Event_StartScreenTransition();
    if (gMainRoutineWork.entrance == TOLBI_SPRING_FROM_TOLBI) {
      TolbiSpring_InitializeTossSimulation();
      if (GameFlag_Get(FLAG_TOLBI_SPRING_DRAINED)) {
        Io_SetBlendControl(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND |
                           BLDCNT_TGT2_ALL);
        Io_SetBlendAlpha(BLEND_ALPHA(0, 16));
      }
    }
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER)) {
      Actor_SetPosition(ACTOR_TOLBI_SPRING_SOLDIER, 0, 0);
    }
    gOamCopyEnabled = 1;
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gMainRoutineWork.entrance == STUCCO_ROOMS_C_LUCKY_DICE_FROM_TOLBI) {
      Actor_SetSpriteColorById(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT,
                               SPRITE_COLOR_GRB);
      Actor_SetSpriteColorById(ACTOR_TOLBI_SPRING_DICE_ATTENDANT,
                               SPRITE_COLOR_RBG);
    }
    // Lucky Dice returns here to report the change in the party's coins.
    if (gMainRoutineWork.entrance == STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Event_Start();
      Actor_SetSpriteColorById(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT,
                               SPRITE_COLOR_GRB);
      Actor_SetSpriteColorById(ACTOR_TOLBI_SPRING_DICE_ATTENDANT,
                               SPRITE_COLOR_RBG);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(10);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 112);
      Event_Wait(20);
      coinChange = gMainRoutineWork.holdings.coins - sLuckyDiceCoinsBefore;
      if (coinChange > 0) {
        if (coinChange >= LUCKY_DICE_LARGE_WIN) {
          Sound_Play(MUS_SUPER_LUCKY);
        } else if (coinChange >= LUCKY_DICE_MEDIUM_WIN) {
          Sound_Play(MUS_VERY_LUCKY);
        } else {
          Sound_Play(MUS_LUCKY);
        }
        Event_Wait(20);
        Event_SetMessageId(MsgTolbiYaWonNumber);
        MessageArgument_Push(coinChange, MESSAGE_ARGUMENT_NUMBER);
        // "Ya won ... coin! Talk about lucky!"
        Event_ShowMessage(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
        Sound_WaitForJingle();
      } else if (coinChange < 0) {
        Event_SetMessageId(MsgTolbiYaLostNumber);
        MessageArgument_Push(-coinChange, MESSAGE_ARGUMENT_NUMBER);
        // "Ya lost ... coin. Better luck next time."
        Event_ShowMessage(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
      }
      Event_End();
    }
    // Lucky Wheels returns item categories for the attendant to award.
    if (gMainRoutineWork.entrance == STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      wonPrizes = gMainRoutineWork.holdings.wonPrizes;
      Event_Start();
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(10);
      if (wonPrizes[0] == -1) {
        TolbiSpring_OfferLuckyWheels(1);
      } else if (wonPrizes[0] != -2) {
        Event_SetMessageId(MsgTolbiCongratulations);
        // "Congratulations!"
        Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
        if (*wonPrizes != -1) {
          for (prizeIndex = 0; wonPrizes[prizeIndex] != -1; prizeIndex++) {
            if (prizeIndex == 0) {
              Event_SetMessageId(MsgTolbiWonItemGo);
            } else {
              Event_SetMessageId(MsgTolbiAndYouWonItem);
            }
            item = TolbiSpring_SelectLuckyWheelsPrize(wonPrizes[prizeIndex]);
            MessageArgument_Push(item, MESSAGE_ARGUMENT_ITEM);
            Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
            Item_ShowFound(item, 3);
            Party_GiveItem(item, 0);
            Event_Wait(10);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
            Event_Wait(30);
          }
        }
        gMainRoutineWork.holdings.wonPrizes[0] = -2;
        Event_SetMessageId(MsgTolbiComeAgain);
        // "Come again!"
        Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      }
      Event_End();
    }
  }
  return 0;
}

/// Draws one picture of the prize being shown in the middle of Tolbi Spring,
/// with SE_CAST, and holds it 8 frames.
static inline void TolbiSpring_ShowPrizePicture(s32 sourceColumn,
                                                s32 sourceRow) {
  MapGrid_CopyCellsTo(sourceColumn, sourceRow, 70, 0, 3, 8);
  Sound_Play(SE_CAST);
  Event_Wait(8);
}

/// Tolbi Spring gives a Lucky Medal's prize: a roar, Isaac turns to face the
/// spring, and a sea dragon of green-blue scales rises from its north edge,
/// its long neck stretching south over the rings until its head reaches the
/// rim before him, the prize held up over his head as he raises his arms;
/// "Isaac got an Adept's Helm." (or whatever the medal won), and the dragon
/// draws back north the way it came.
///
/// The dragon is the pictures: each is 3 by 8 cells copied over the spring's
/// middle from spare cells of the map, one every 8 frames, the head a step
/// further south in each as it comes and a step back north in each as it goes.
static void TolbiSpring_RevealPrize(s32 item) {
  Event_Start();
  Event_Wait(30);
  Sound_Play(SE_BOSS_ROAR);
  Event_Wait(100);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_Wait(40);

  MapGrid_CopyCellsTo(82, 20, 70, 0, 3, 8);
  Event_Wait(3);
  TolbiSpring_ShowPrizePicture(85, 20);
  TolbiSpring_ShowPrizePicture(88, 20);
  TolbiSpring_ShowPrizePicture(91, 20);
  TolbiSpring_ShowPrizePicture(94, 20);
  TolbiSpring_ShowPrizePicture(97, 20);
  TolbiSpring_ShowPrizePicture(100, 20);

  TolbiSpring_ShowPrizePicture(79, 29);
  TolbiSpring_ShowPrizePicture(82, 29);
  TolbiSpring_ShowPrizePicture(85, 29);
  TolbiSpring_ShowPrizePicture(88, 29);
  TolbiSpring_ShowPrizePicture(91, 29);
  TolbiSpring_ShowPrizePicture(94, 29);
  TolbiSpring_ShowPrizePicture(97, 29);
  MapGrid_CopyCellsTo(100, 29, 70, 0, 3, 8);
  Sound_Play(SE_CAST);

  Event_Wait(70);
  Sound_Play(SE_RECOVERY);
  Item_ShowFound(item, 3);
  Party_GiveItem(item, 0);
  Event_Wait(20);

  TolbiSpring_ShowPrizePicture(97, 29);
  TolbiSpring_ShowPrizePicture(94, 29);
  TolbiSpring_ShowPrizePicture(91, 29);
  TolbiSpring_ShowPrizePicture(88, 29);
  TolbiSpring_ShowPrizePicture(85, 29);
  TolbiSpring_ShowPrizePicture(82, 29);
  TolbiSpring_ShowPrizePicture(100, 20);
  TolbiSpring_ShowPrizePicture(97, 20);
  TolbiSpring_ShowPrizePicture(94, 20);
  TolbiSpring_ShowPrizePicture(91, 20);
  TolbiSpring_ShowPrizePicture(88, 20);
  TolbiSpring_ShowPrizePicture(85, 20);
  TolbiSpring_ShowPrizePicture(82, 20);
  TolbiSpring_ShowPrizePicture(79, 20);
  Event_End();
}

/// Tolbi Spring's water sinks 16 pixels in the basin and fades away,
/// uncovering the target of rings on its floor.
///
/// It starts with SE_BURST. The water is the overhead layer, scrolled down a
/// pixel every fourth frame and then faded out through
/// gTolbiSpring_AlphaSteps.
static void TolbiSpring_DrainSpring(void) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct MapGridLayerScroll *spring;
  const u16 *alphaSteps;
  s32 stepIndex;

  Sound_Play(SE_BURST);
  spring = &grid->layers[MAPGRID_LAYER_OVERHEAD];
  for (stepIndex = 0; stepIndex < SPRING_WATER_SINK_PIXELS; stepIndex++) {
    spring->offsetY -= toFIXED(1);
    wait(4);
  }
  alphaSteps = gTolbiSpring_AlphaSteps;
  for (stepIndex = 0; stepIndex < 8; stepIndex++) {
    Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
               &REG_BLDCNT);
    Io_Write16(*alphaSteps++, &REG_BLDALPHA);
    wait(8);
  }
}

/// The Lucky Wheels attendant asks whether Isaac will use his Game Tickets,
/// warns him when the bag is full or nearly so, and sends him to the slot
/// machine.
///
/// spoken is 1 when the game has just ended with nothing won: she says `Too
/// bad...` and offers another game, or says to come back with tickets.
static void TolbiSpring_OfferLuckyWheels(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(ITEM_GAME_TICKET);
  s32 room;

  Actor_Snapshot();
  if (spoken == 0) {
    s32 message = MsgTolbiStepRightTry;

    Event_SetMessageId(message);
    // "Step right up and try your hand at lucky wheels!"
    Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (tickets == 0) {
      // "You gotta have game tickets to play."
      Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    Event_SetMessageId(message + 2);
    MessageArgument_Push(tickets, MESSAGE_ARGUMENT_NUMBER);
    // "You have ... game ticket, Isaac. Would you like to use these..."
    Event_OpenMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
      // "The rules are listed on the sign. Take a look, take a look."
      Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Event_SetMessageId(message + 4);
      // "You can't carry any more items. Are you sure you want to play?"
      Event_OpenMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    } else if (room <= 6) {
      Event_SetMessageId(message + 5);
      // "If you win anything, you might not be able to take it with..."
      Event_OpenMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    }
    if (room <= 6 &&
        Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
      Event_SetMessageId(MsgTolbiGetRidItems);
      // "Then get rid of some items and come again."
      Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
  } else {
    if (tickets == 0) {
      Event_SetMessageId(MsgTolbiBadComeBack);
      // "Too bad... Come back after you get some tickets."
      Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    Event_SetMessageId(MsgTolbiBadLikeUse);
    // "Too bad... Would you like to use your tickets to keep playing?"
    Event_OpenMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
      Event_SetMessageId(MsgTolbiComeAgain);
      // "Come again!"
      Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
  }
  Event_SetMessageId(MsgTolbiStepRightEnjoy);
  // "Step right up and enjoy yourself."
  Event_ShowMessage(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
  Event_LeaveFor(SEQUENCE_SLOTS, 0);
  Event_SetAfterBattleDestination(SCENE_STUCCO_ROOMS_C,
                                  STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS);
}

/// The fair-haired young man on the left behind the Lucky Dice counter offers
/// a game for ten coins a level of the party's average level. If Isaac agrees,
/// he walks down to the green table and the game of Lucky Dice begins.
///
/// Without enough coins he is told `Ya gotta pay to play.`
static void TolbiSpring_OfferLuckyDice(void) {
  u32 cost;
  u32 coins;

  cost = Party_GetAverageLevel() * LUCKY_DICE_COINS_PER_LEVEL;
  Event_Start();
  coins = gMainRoutineWork.holdings.coins;
  if (coins < cost) {
    Event_SetMessageId(MsgTolbiYaDontEnough);
    // "Hey! Ya don't have enough coins! Ya gotta pay to play."
    Event_OpenMessage(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
    return;
  }
  sLuckyDiceCoinsBefore = coins;
  Actor_Snapshot();
  Event_SetMessageId(MsgTolbiBuddyWannaTry);
  MessageArgument_Push(cost, MESSAGE_ARGUMENT_NUMBER);
  // "Hey, buddy, wanna try yer luck at the dice? It's ... coin to..."
  Event_OpenMessage(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    // "OK! Then take the dice."
    Event_ShowMessage(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 128);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 152);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_Wait(20);
    Event_LeaveFor(SEQUENCE_DICE, 0);
    Event_SetAfterBattleDestination(SCENE_STUCCO_ROOMS_C,
                                    STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE);
  } else {
    gFieldWork->message++;
    // "No? Oh, well. Who's next?"
    Event_ShowMessage(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
  }
  Event_End();
}

/// The party chooses whether to toss a coin or a Lucky Medal, presses A to
/// toss it, and wins coins or an item by the ring it settles in.
///
/// A coin that settles outside the four inner rings wins nothing. A medal's
/// prize is drawn by weight from the prizes its ring allows, and its weight
/// is halved for later tosses.
static s32 TolbiSpring_RunSpringGame(void) {
  s32 choice = 0;
  s32 text;
  s32 cancel = -1; // what the menus answer when B is pressed
  s32 coins;
  s32 medals;
  struct Window *window;
  s32 result;
  s32 totalWeight;
  s32 prizeIndex;
  s32 prizeRoll;
  s32 message;
  struct MainRoutineWork *routine;

  Event_Start();
  Field_EndBusyAndReveal();
  message = MsgTolbiThrow;
#if TBS_PATCH_1
  routine = &gMainRoutineWork;
prompt:
  coins = routine->holdings.coins;
  medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
  Event_SetMessageId(message);
  // "What will you throw?"
  Event_OpenMessage(-1, 0);
  window = Window_Create(0, 0, TOLBI_SPRING_COUNT_WIDTH, 4, 2);
  text = MsgTolbiCoins;
  Text_DrawMessageAsTiles(text, window, 0, 0);
  Text_DrawNumberInWindow(coins, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 0);
  Text_DrawMessageAsTiles(text + 1, window, 0, 8);
  Text_DrawNumberInWindow(medals, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 8);
  choice = Menu_SelectCoinsOrMedals(choice);
  Window_Close(window, 2);
  Message_CloseFinishedWindows();
  if (choice == cancel) {
    Event_End();
    return 0;
  }
  if (choice == SPRING_TOSS_COIN) {
    if (coins == 0) {
      Event_SetMessageId(message + 1);
      // "You don't have any coins!"
      Event_ShowMessage(-1, 0);
      wait(1);
      goto prompt;
    }
  } else if (choice == SPRING_TOSS_MEDAL) {
    if (medals == 0) {
      Event_SetMessageId(message + 2);
      // "You have no Lucky Medals!"
      Event_ShowMessage(-1, 0);
      wait(1);
      goto prompt;
    }
    if (PartyInventory_CountFreeSlots() == 0) {
      Event_SetMessageId(message + 4);
      // "You can't carry more items. Do you still want to play?"
      Event_OpenMessage(-1, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
        goto end;
      }
    }
  }
  window = Window_Create(TOLBI_SPRING_TOSS_MENU_X, 15,
                         TOLBI_SPRING_TOSS_MENU_WIDTH, 4, 2);
  text = MsgTolbiToss;
  Text_DrawMessageAsTiles(text, window, 0, 0);
  Text_DrawMessageAsTiles(text + 1, window, 0, 8);
  wait(5);
  Sound_Play(SE_TOLBI_SPRING_TOSS_PROMPT);
  for (;;) {
    if (gKeysPressed & KEY_A) {
      Sound_Play(SE_MENU_CONFIRM);
      result = 0;
      break;
    }
    if (gKeysPressed & KEY_B) {
      Sound_Play(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    wait(1);
  }
  Window_Close(window, 2);
#else
  for (;;) {
    text = MsgTolbiCoins;
    routine = &gMainRoutineWork;
    coins = routine->holdings.coins;
    medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
    Event_SetMessageId(message);
    // "What will you throw?"
    Event_OpenMessage(-1, 0);
    window = Window_Create(0, 0, TOLBI_SPRING_COUNT_WIDTH, 4, 2);
    Text_DrawMessageAsTiles(text, window, 0, 0);
    Text_DrawNumberInWindow(coins, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 0);
    // The marker after each count, MsgTolbiCounterMarker, follows the medal
    // label.
    Text_DrawMessageAsTiles(text + 2, window, 104, 0);
    Text_DrawMessageAsTiles(text + 1, window, 0, 8);
    Text_DrawNumberInWindow(medals, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 8);
    Text_DrawMessageAsTiles(text + 2, window, 104, 8);
    choice = Menu_SelectCoinsOrMedals(choice);
    Window_Close(window, 2);
    Message_CloseFinishedWindows();
    if (choice == cancel) {
      Event_End();
      return 0;
    }
    if (choice == SPRING_TOSS_COIN) {
      if (coins == 0) {
        Event_SetMessageId(message + 1);
        // "You don't have any coins!"
        Event_ShowMessage(-1, 0);
        wait(1);
        continue;
      }
    } else if (choice == SPRING_TOSS_MEDAL) {
      if (medals == 0) {
        Event_SetMessageId(message + 2);
        // "You have no Lucky Medals!"
        Event_ShowMessage(-1, 0);
        wait(1);
        continue;
      }
      if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessageId(message + 4);
        // "You can't carry more items. Do you still want to play?"
        Event_OpenMessage(-1, 0);
        if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
          Event_End();
          return 0;
        }
      }
    }
    window = Window_Create(TOLBI_SPRING_TOSS_MENU_X, 15,
                           TOLBI_SPRING_TOSS_MENU_WIDTH, 4, 2);
    result = MsgTolbiToss;
    Text_DrawMessageAsTiles(result, window, 0, 0);
    Text_DrawMessageAsTiles(result + 1, window, 0, 8);
    wait(5);
    Sound_Play(SE_TOLBI_SPRING_TOSS_PROMPT);
    for (;;) {
      if (gKeysPressed & KEY_A) {
        Sound_Play(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Sound_Play(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      wait(1);
    }
    Window_Close(window, 2);
    break;
  }
#endif
  // Charge the chosen token only after the player confirms the toss.
  if (result != cancel) {
    if (choice == SPRING_TOSS_COIN) {
      Party_AdjustCoins(-1);
    } else if (choice == SPRING_TOSS_MEDAL) {
      PartyInventory_Remove(ITEM_LUCKY_MEDAL);
    }
    result = TolbiSpring_RunToss(choice);
    if (choice == SPRING_TOSS_COIN) {
      if (result != 4) {
        Party_AdjustCoins(TOLBI_SPRING_COIN_PRIZE(result));
        Sound_Play(MUS_LUCKY);
        MessageArgument_Push(TOLBI_SPRING_COIN_PRIZE(result),
                             MESSAGE_ARGUMENT_NUMBER);
        Event_SetMessageId(MsgTolbiWonNumberCoin);
        // "You won ... coin!"
        Event_ShowMessage(-1, 0);
      } else {
        Sound_Play(SE_MENU_CANCEL);
        Event_Wait(10);
      }
    } else {
      // The prize ring limits the weighted pool of medal rewards.
      totalWeight = 0;
      prizeIndex = 0;
      if (prizeIndex < result * SPRING_MEDAL_PRIZES_PER_RING +
                           SPRING_MEDAL_PRIZES_PER_RING) {
        do {
          /* FAKEMATCH(temporary): weight, the prize's weight held for the one
           * sum; as the weight added directly. */
          s8 weight = *(routine->holdings.prizeWeights + prizeIndex);

          totalWeight += weight;
          prizeIndex++;
        } while (prizeIndex < result * SPRING_MEDAL_PRIZES_PER_RING +
                                  SPRING_MEDAL_PRIZES_PER_RING);
      }
      prizeRoll = Random_Below(totalWeight);
      for (prizeIndex = 0; prizeIndex < SPRING_MEDAL_PRIZES; prizeIndex++) {
        prizeRoll -= routine->holdings.prizeWeights[prizeIndex];
        if (prizeRoll < 0) {
          break;
        }
      }
      if (prizeIndex == SPRING_MEDAL_PRIZES) {
        prizeIndex = SPRING_MEDAL_PRIZES - 1;
      }
      TolbiSpring_RevealPrize(gTolbiSpring_PrizeItems[prizeIndex]);
      /* FAKEMATCH(dead): stepping routine away and back. */
      routine++;
      routine--;
      {
        s32 slot = prizeIndex +
                   offsetof(struct MainRoutineWork, holdings.prizeWeights) - 1;
        /* FAKEMATCH(temporary): a byte pointer one past routine, indexed by
         * slot; as routine->holdings.prizeWeights[prizeIndex]. */
        s8 *weights = (s8 *)routine + 1;

        if (weights[slot] > 1) {
          /* FAKEMATCH(dead): the halving in both arms of a test of slot. */
          if (slot)
            weights[slot] /= 2;
          else
            weights[slot] /= 2;
        }
      }
    }
  }
#if TBS_PATCH_1
end:
#endif
  Event_End();
  return 0;
}

/// Chooses which of the three items of a kind the Lucky Wheels attendant hands
/// over, never the one she gave last time. A moon wins a kind at random.
static s32 TolbiSpring_SelectLuckyWheelsPrize(s32 category) {
  s32 previousVariant;
  s32 variant;

  if (category < 0) {
    return 0;
  }

  if (category == LUCKY_WHEELS_MOON) {
    category = Random_Below(LUCKY_WHEELS_KINDS);
  }

  previousVariant = gMainRoutineWork.holdings.wonPrizes[8 + category];

  variant = (previousVariant + (s32)Random_Below(2) + 4) % 3;

  gMainRoutineWork.holdings.wonPrizes[8 + category] = variant;

  return gTolbiSpring_LuckyWheelsPrizeItems[category * 3 + variant];
}

/// Places the actor at the three coordinates and sets its facing, animation
/// speed and colours.
static void TolbiSpring_SetActorAppearance(s32 actorId, const s32 *coordinate,
                                           s32 facing, s32 colorVariant,
                                           s32 animationSpeed) {
  struct Actor *actor = Actor_Get(actorId);
  struct Sprite *sprite;
  u32 count;

  if (actor != NULL) {
    actor->x.fixed = *coordinate++;
    actor->y.fixed = *coordinate++;
    actor->z.fixed = *coordinate;
    actor->facing = facing;
    actor->motionFlags = 0;
    actor->sprite->flags = 0;
    Actor_SetAnimationSpeed(actor, animationSpeed);
  }
  // BUG: actor is read even when Actor_Get found none.
  sprite = actor->sprite;
  count = sprite->count;
  if (count != 0) {
    s32 noPicture = SPRITE_PICTURE_NONE;
    struct SpriteAnimation **animation = sprite->entries;
    u32 left = count;

    do {
      struct SpriteAnimation *entry = *animation++;

      if (entry->colorMode != colorVariant) {
        u32 picture;

        entry->colorMode = colorVariant;
        // No picture is cached in the new colours yet.
        picture = entry->picture | noPicture;
        entry->picture = picture;
      }
    } while (--left != 0);
  }
}

/// Sets whether the specified actor draws its sprite.
static void TolbiSpring_SetSpriteStorageKind(s32 actorId, s32 storageKind) {
  struct Actor *actor = Actor_Get(actorId);

  if (actor != NULL) {
    actor->spriteStorageKind = storageKind;
  }
}

static struct SpringTossMotion sToss ALIGNED(16);

static s32 sTossKind;

/// Three words nothing refers to.
///
/// Removing them would move the obstacle state.
static s32 sTossUnused[3] UNUSED;

static struct SpringObstacle sObstacles[4];

static s32 sTossFrame;
static s32 sTossEnded;
static s32 sTossResult;

/// Advances the toss, obstacles, collisions and displayed trail each frame.
static void TolbiSpring_UpdateTossSimulation(void) {
  struct SpringTossMotion *toss;
  struct SpringObstacle *obstacle;
  s32 index;
  s32 height;
  FIXED edgeInset;
  s32 motionStep;
  s32 speedMode;
  s32 cycleFrame;
  FIXED minX;
  FIXED maxX;
  FIXED minZ;
  FIXED maxZ;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED struct Vector3 spare;

  toss = &sToss;
  // Retain the previous three positions for the visible toss trail.
  for (index = 3; index != 0; index--) {
    toss->trail[index].x = toss->trail[index - 1].x;
    toss->trail[index].y = toss->trail[index - 1].y;
    toss->trail[index].z = toss->trail[index - 1].z;
  }

  // Delay motion until the leader's launch animation has played.
  if (toss->launchFrame > 31) {
    toss->trail[0].x += toss->velocity.x;
    height = toss->trail[0].y + toss->velocity.y;
    toss->trail[0].y = height;
    toss->trail[0].z += toss->velocity.z;

    if (height <= 0) {
      toss->trail[0].y = 0;
      if (toss->velocity.y != 0) {
        toss->velocity.y = 0;
        if (sTossKind == SPRING_TOSS_MEDAL) {
          Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_MEDAL),
                                ANIM_TOKEN_FLOAT);
        } else {
          Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_COIN),
                                ANIM_TOKEN_FLOAT);
        }
      }

      if (toss->attractionFrames > 0) {
        s32 deltaX;
        s32 deltaZ;
        s32 distance;

        deltaX = (TOLBI_SPRING_CENTER_X - toss->trail[0].x) >> 8;
        deltaZ = (TOLBI_SPRING_CENTER_Z - toss->trail[0].z) >> 8;
        // BUG: an exact center position leaves a zero divisor below.
        distance = Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
        toss->velocity.x += ((toFIXED(1) / 10) * deltaX) / distance;
        toss->velocity.z += ((toFIXED(1) / 10) * deltaZ) / distance;
        toss->velocity.x = toss->velocity.x * SPRING_TOKEN_PULLED_DAMPING /
                           SPRING_TOKEN_DAMPING_SCALE;
        toss->velocity.z = toss->velocity.z * SPRING_TOKEN_PULLED_DAMPING /
                           SPRING_TOKEN_DAMPING_SCALE;
        toss->attractionFrames = toss->attractionFrames - 1;
      } else {
        toss->velocity.x = toss->velocity.x * SPRING_TOKEN_SETTLING_DAMPING /
                           SPRING_TOKEN_DAMPING_SCALE;
        toss->velocity.z = toss->velocity.z * SPRING_TOKEN_SETTLING_DAMPING /
                           SPRING_TOKEN_DAMPING_SCALE;
        if (toss->velocity.x > -toFIXED(1) / 64 &&
            toss->velocity.x < toFIXED(1) / 64) {
          toss->velocity.x = 0;
        }
        if (toss->velocity.z > -toFIXED(1) / 64 &&
            toss->velocity.z < toFIXED(1) / 64) {
          toss->velocity.z = 0;
        }
        if (toss->velocity.x == 0 && toss->velocity.z == 0) {
          if (sTossKind == SPRING_TOSS_MEDAL) {
            Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_MEDAL),
                                  ANIM_TOKEN_REST);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(1), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(2), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(3), 0);
          } else {
            Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_COIN),
                                  ANIM_TOKEN_REST);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(1), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(2), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(3), 0);
          }
          {
            s32 deltaX = FIXEDtoINT(TOLBI_SPRING_CENTER_X - toss->trail[0].x);
            s32 deltaZ = FIXEDtoINT(TOLBI_SPRING_CENTER_Z - toss->trail[0].z);
            s32 distanceSquared = deltaX * deltaX + deltaZ * deltaZ;

            // Classify squared pixel distance into the five prize rings.
            sTossEnded = 1;
            if (distanceSquared <= 224) {
              sTossResult = 0;
            } else if (distanceSquared <= 624) {
              sTossResult = 1;
            } else if (distanceSquared <= 1088) {
              sTossResult = 2;
            } else if (distanceSquared <= 1680) {
              sTossResult = 3;
            } else {
              sTossResult = 4;
            }
          }
        }
      }

      // Narrow the bounds near the spring's sloping corners.
      minX = toFIXED(48);
      maxX = toFIXED(192);
      minZ = toFIXED(24);
      maxZ = toFIXED(120);
      if (toss->trail[0].z < toFIXED(42)) {
        edgeInset = (toFIXED(42) - toss->trail[0].z) * 42 / 18;
        minX = toFIXED(48) + edgeInset;
        if (minX > toFIXED(90)) {
          minX = toFIXED(90);
        }
        maxX = toFIXED(192) - edgeInset;
        if (maxX < toFIXED(150)) {
          maxX = toFIXED(150);
        }
      }
      if (toss->trail[0].z > toFIXED(102)) {
        edgeInset = (toss->trail[0].z * 42 - toFIXED(102) * 42) / 18;
        minX = toFIXED(48) + edgeInset;
        if (minX > toFIXED(90)) {
          minX = toFIXED(90);
        }
        maxX = toFIXED(192) - edgeInset;
        if (maxX < toFIXED(150)) {
          maxX = toFIXED(150);
        }
      }
      if (toss->trail[0].x < toFIXED(90)) {
        edgeInset = (toFIXED(90) - toss->trail[0].x) * 18 / 42;
        minZ = toFIXED(24) + edgeInset;
        if (minZ > toFIXED(42)) {
          minZ = toFIXED(42);
        }
        maxZ = toFIXED(120) - edgeInset;
        if (maxZ < toFIXED(102)) {
          maxZ = toFIXED(102);
        }
      }
      if (toss->trail[0].x > toFIXED(150)) {
        edgeInset = (toss->trail[0].x * 18 - toFIXED(150) * 18) / 42;
        minZ = toFIXED(24) + edgeInset;
        if (minZ > toFIXED(42)) {
          minZ = toFIXED(42);
        }
        maxZ = toFIXED(120) - edgeInset;
        if (maxZ < toFIXED(102)) {
          maxZ = toFIXED(102);
        }
      }

      // Reflect outward motion at the boundary, halving the reflected speed.
      if (toss->trail[0].x < minX) {
        toss->trail[0].x = minX;
        if (toss->velocity.x < 0) {
          toss->velocity.x = -toss->velocity.x / 2;
        }
      }
      if (toss->trail[0].x > maxX) {
        toss->trail[0].x = maxX;
        if (toss->velocity.x > 0) {
          toss->velocity.x = -toss->velocity.x / 2;
        }
      }
      if (toss->trail[0].z < minZ) {
        toss->trail[0].z = minZ;
        if (toss->velocity.z < 0) {
          toss->velocity.z = -toss->velocity.z / 2;
        }
      }
      if (toss->trail[0].z > maxZ) {
        toss->trail[0].z = maxZ;
        if (toss->velocity.z > 0) {
          toss->velocity.z = -toss->velocity.z / 2;
        }
      }
    } else {
      toss->velocity.y -= toFIXED(1) / 4;
    }
  }

  // The two crabs scuttle east and west; the two turtles swim round ellipses,
  // the second hiding in its shell for the last quarter of every 512 frames.
  for (index = 0; index != 4; index++) {
    obstacle = &sObstacles[index];

    if (obstacle->reactionFrames > 0) {
      obstacle->reactionFrames = obstacle->reactionFrames - 1;
    }
    if (obstacle->hitCooldownFrames > 0) {
      obstacle->hitCooldownFrames = obstacle->hitCooldownFrames - 1;
    }

    if (index <= 1) {
      speedMode = obstacle->speedMode;
      motionStep = toFIXED(1);
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        if (index == 0) {
          Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE),
                                ANIM_CRAB_RAISE_CLAWS);
        } else {
          Actor_SelectAnimation(
              Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1),
              ANIM_CRAB_RAISE_CLAWS);
        }
      } else {
        if (index == 0) {
          Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE),
                                ANIM_CRAB_SCUTTLE);
        } else {
          Actor_SelectAnimation(
              Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1),
              ANIM_CRAB_SCUTTLE);
        }
        if (obstacle->pauseOrPhase == 0) {
          if (obstacle->heading == 0) {
            obstacle->position.x = obstacle->position.x + motionStep;
          } else {
            obstacle->position.x = obstacle->position.x - motionStep;
          }
          if (obstacle->position.x <= toFIXED(64)) {
            obstacle->heading = 0;
            if (index == 1) {
              obstacle->pauseOrPhase = SPRING_CRAB_REST_FRAMES;
            }
          }
          if (obstacle->position.x > (toFIXED(176) - 1)) {
            obstacle->heading = 1;
            if (index == 1) {
              obstacle->pauseOrPhase = SPRING_CRAB_REST_FRAMES;
            }
          }
        } else {
          obstacle->pauseOrPhase = obstacle->pauseOrPhase - 1;
        }
      }
    } else if (index == 2) {
      speedMode = obstacle->speedMode;
      motionStep = -(TRIG_TURN / 1024);
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2),
                              ANIM_TURTLE_HIDE);
      } else {
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2),
                              ANIM_TURTLE_SWIM);
        obstacle->position.x = Trig_Sin(obstacle->heading) * 48 + toFIXED(112);
        obstacle->position.z = Trig_Cos(obstacle->heading) * 40 + toFIXED(72);
        cycleFrame = (u16)obstacle->heading + motionStep;
        obstacle->heading = cycleFrame;
        obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
      }
    } else {
      cycleFrame = obstacle->pauseOrPhase & 511;
      speedMode = obstacle->speedMode;
      motionStep = TRIG_TURN / 1024;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                              ANIM_TURTLE_HIDE);
      } else if (cycleFrame <= 383) {
        obstacle->position.x = Trig_Sin(obstacle->heading) * 52 + toFIXED(112);
        obstacle->position.z = Trig_Cos(obstacle->heading) * 24 + toFIXED(72);
        obstacle->heading = obstacle->heading + motionStep;
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                              ANIM_TURTLE_SWIM);
      } else {
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                              ANIM_TURTLE_HIDE);
      }
      obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
    }

    // A nearby obstacle redirects the token and shortens its attraction time.
    if (obstacle->hitCooldownFrames == 0 && toss->trail[0].y == 0) {
      s32 deltaX;
      s32 deltaZ;
      s32 distanceSquared;

      deltaX = FIXEDtoINT(obstacle->position.x - toss->trail[0].x);
      deltaZ = FIXEDtoINT(obstacle->position.z - toss->trail[0].z);
      distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
      if (distanceSquared <= 119 && toss->attractionFrames > 30) {
        FIXED speed;
        s32 nextMode;

        speed = toFIXED(3);
        if (index <= 1) {
          if (obstacle->heading == 0) {
            if (toss->velocity.x < speed) {
              toss->velocity.x = speed;
              toss->attractionFrames = toss->attractionFrames - 100;
            }
          } else {
            if (toss->velocity.x > -speed) {
              toss->velocity.x = -speed;
              toss->attractionFrames = toss->attractionFrames - 100;
            }
          }
        } else {
          s32 distance;

          // BUG: coincident positions leave a zero divisor below.
          distance = Iwram_Sqrt(distanceSquared);
          toss->velocity.x = -deltaX * speed / distance;
          toss->velocity.z = -deltaZ * speed / distance;
          toss->attractionFrames = toss->attractionFrames - 100;
        }
        Sound_Play(SE_TOLBI_SPRING_OBSTACLE_HIT);
        nextMode = (obstacle->speedMode + 1) % 3;
        obstacle->reactionFrames = 36;
        obstacle->speedMode = nextMode;
        obstacle->hitCooldownFrames = 30;
      }
    }

    switch (index) {
    case 0:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE,
          Vector3_Coordinates(&obstacle->position), 0,
          gTolbiSpring_SliderFrames[obstacle->speedMode],
          obstacle->speedMode * SPRITE_ANIMATION_STEP_NORMAL +
              SPRITE_ANIMATION_STEP_NORMAL);
      break;
    case 1:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1,
          Vector3_Coordinates(&obstacle->position), 0,
          gTolbiSpring_SliderFrames[obstacle->speedMode],
          obstacle->speedMode * SPRITE_ANIMATION_STEP_NORMAL +
              SPRITE_ANIMATION_STEP_NORMAL);
      break;
    case 2:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2,
          Vector3_Coordinates(&obstacle->position),
          DEGtoANG(180) - obstacle->heading,
          gTolbiSpring_CircleFrames[obstacle->speedMode],
          obstacle->speedMode * SPRITE_ANIMATION_STEP_NORMAL +
              SPRITE_ANIMATION_STEP_NORMAL);
      break;
    case 3:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3,
          Vector3_Coordinates(&obstacle->position),
          TRIG_TURN - 1 - obstacle->heading,
          gTolbiSpring_CircleFrames[obstacle->speedMode],
          obstacle->speedMode * SPRITE_ANIMATION_STEP_NORMAL +
              SPRITE_ANIMATION_STEP_NORMAL);
      break;
    }
  }

  toss->shadow.x = toss->trail[0].x;
  toss->shadow.y = 0;
  toss->shadow.z = toss->trail[0].z;

  if (sTossKind == SPRING_TOSS_MEDAL) {
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_MEDAL,
                                   Vector3_Coordinates(&toss->trail[0]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_MEDAL_SHADOW,
                                   Vector3_Coordinates(&toss->shadow), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(1),
                                   Vector3_Coordinates(&toss->trail[1]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(2),
                                   Vector3_Coordinates(&toss->trail[2]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(3),
                                   Vector3_Coordinates(&toss->trail[3]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    Actor_SelectAnimation(Actor_Get(TOLBI_SPRING_MEDAL_TRAIL(1)),
                          ANIM_TOKEN_GLINT);
    Actor_SelectAnimation(Actor_Get(TOLBI_SPRING_MEDAL_TRAIL(2)),
                          ANIM_TOKEN_GLINT);
    Actor_SelectAnimation(Actor_Get(TOLBI_SPRING_MEDAL_TRAIL(3)),
                          ANIM_TOKEN_GLINT);
  } else {
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_COIN,
                                   Vector3_Coordinates(&toss->trail[0]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_COIN_SHADOW,
                                   Vector3_Coordinates(&toss->shadow), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_COIN_TRAIL(1),
                                   Vector3_Coordinates(&toss->trail[1]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_COIN_TRAIL(2),
                                   Vector3_Coordinates(&toss->trail[2]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_COIN_TRAIL(3),
                                   Vector3_Coordinates(&toss->trail[3]), 0, 0,
                                   SPRITE_ANIMATION_STEP_NORMAL);
    Actor_SelectAnimation(Actor_Get(TOLBI_SPRING_COIN_TRAIL(1)),
                          ANIM_TOKEN_GLINT);
    Actor_SelectAnimation(Actor_Get(TOLBI_SPRING_COIN_TRAIL(2)),
                          ANIM_TOKEN_GLINT);
    Actor_SelectAnimation(Actor_Get(TOLBI_SPRING_COIN_TRAIL(3)),
                          ANIM_TOKEN_GLINT);
  }

  if (toss->launchFrame != -1) {
    toss->launchFrame = toss->launchFrame + 1;
  }
}

/// Resets four obstacles and schedules the spring simulation.
static void TolbiSpring_InitializeTossSimulation(void) {
  struct SpringTossMotion *toss = &sToss;
  s32 obstacleIndex = 0;
  const u8 *pixelX;
  const u16 *headings;
  struct SpringObstacle *obstacle;
  const u8 *pixelZ;

  pixelX = gTolbiSpring_ActorPixelX;
  obstacle = sObstacles;
  headings = gTolbiSpring_ActorHeadings;
  pixelZ = gTolbiSpring_ActorPixelZ;

  do {
    obstacle->position.x = toFIXED(*pixelX);
    obstacle->position.z = toFIXED(*pixelZ);
    obstacle->position.y = 0;
    obstacle->heading = *headings;
    obstacle->pauseOrPhase = 0;
    obstacle->speedMode = 0;
    obstacle->reactionFrames = 0;
    obstacle->hitCooldownFrames = 0;

    obstacleIndex++;
    pixelX++;
    pixelZ++;
    headings++;
    obstacle++;
  } while (obstacleIndex != 4);

  toss->trail[0].x = -toFIXED(30);
  toss->trail[0].y = 0;
  toss->trail[0].z = toFIXED(100);
  toss->velocity.x = 0;
  toss->velocity.y = 0;
  toss->velocity.z = 0;
  toss->attractionFrames = 0;

  Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2),
                        ANIM_TURTLE_SWIM);
  Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                        ANIM_TURTLE_SWIM);
  Task_AddFrame(TolbiSpring_UpdateTossSimulation, TASK_PASS_FRAME + 3);
}

/// Launches the chosen coin or medal and waits for its final prize ring.
static s32 TolbiSpring_RunToss(s32 tossKind) {
  struct SpringTossMotion *toss;

  toss = &sToss;
  toss->trail[0].y = 0;
  toss->trail[1].y = 0;
  toss->trail[2].y = 0;
  toss->trail[3].y = 0;
  sTossKind = tossKind;
  sTossEnded = 0;
  toss->launchFrame = -1;
  for (sTossFrame = 0;; sTossFrame++) {
    if (sTossFrame == 50) {
      Sound_Play(SE_TOLBI_SPRING_TOSS);
    }
    if (sTossFrame == 16) {
      Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_TOSS);
      toss->launchFrame = 0;
      toss->velocity.x = toFIXED(13) / 10;
      toss->velocity.y = toFIXED(4);
      toss->velocity.z = -toFIXED(2);
      toss->trail[0].x = toFIXED(120);
      toss->trail[0].y = toFIXED(16);
      toss->trail[0].z = toFIXED(152);
      toss->attractionFrames = 300;
      if (sTossKind == SPRING_TOSS_MEDAL) {
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_MEDAL_SHADOW),
                              ANIM_TOKEN_SHADOW);
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_MEDAL),
                              ANIM_TOKEN_TUMBLE);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(1), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(2), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(3), 1);
      } else {
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_COIN_SHADOW),
                              ANIM_TOKEN_SHADOW);
        Actor_SelectAnimation(Actor_Get(ACTOR_TOLBI_SPRING_COIN),
                              ANIM_TOKEN_TUMBLE);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(1), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(2), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(3), 1);
      }
    }
    wait(1);
    if (sTossEnded == 1) {
      break;
    }
  }
  return sTossResult;
}

const struct SceneEntrance gTolbiSpring_Entrances[] = {
    {0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {TOLBI_SPRING_FROM_TOLBI, CONDITION_ALWAYS, 120, 0, 168, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 128, 0, 176, FACING_NORTH, 0, 8, 8, 248, 200, 0},
    {11, CONDITION_ALWAYS, 96, 0, 440, FACING_NORTH, 0, 8, 288, 248, 456, 0},
    {STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS, CONDITION_ALWAYS, 144, 0, 390,
     FACING_NORTH, 0, 8, 288, 248, 456, 0},
    {STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE, CONDITION_ALWAYS, 120, 0, 152,
     FACING_WEST, 0, 8, 8, 248, 200, 0},
    {SCENE_TABLE_END},
};

const u32 gTolbiSpring_Exits[] = {
    SCENE_EXITS(TolbiSpring),
    SCENE_EXIT_UNLESS(TOLBI_SPRING_TO_TOLBI, Tolbi, TOLBI_FROM_TOLBI_SPRING,
                      0xffffffff),
    SCENE_EXITS(StuccoRoomsC),
    SCENE_EXIT_UNLESS(STUCCO_ROOMS_C_LUCKY_DICE_TO_TOLBI, Tolbi,
                      TOLBI_FROM_LUCKY_DICE, 0xffffffff),
    SCENE_EXIT_UNLESS(STUCCO_ROOMS_C_LUCKY_WHEELS_TO_TOLBI, Tolbi,
                      TOLBI_FROM_LUCKY_WHEELS, 0xffffffff),
    SCENE_EXITS_END,
};

// clang-format off
const struct ActorPlacement gTolbiSpringActorPlacements[] = {
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

const struct ActorPlacement gTolbiSpringActorPlacementsOther[] = {
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

const struct Trigger gTolbiSpringTriggersOther[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, STUCCO_ROOMS_C_LUCKY_DICE_TO_TOLBI },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, STUCCO_ROOMS_C_LUCKY_WHEELS_TO_TOLBI },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, CONDITION_ALWAYS, EVENT(TolbiSpring_TalkToWheelsAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringHmmIWonderIfTheseKids) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_DICE_ATTENDANT, CONDITION_ALWAYS, EVENT(TolbiSpring_OfferLuckyDice) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringReadTheSignIfYaDont) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_DICE_PLAYER_FIRST_TIME, CONDITION_ALWAYS, EVENT(TolbiSpring_AskAboutFirstDiceGame) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_DICE_PLAYER_TRIPLE_DIGITS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWhoaGettingTripleDigitsIsReal) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_DICE_PLAYER_OUT_OF_COINS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringIDontWantToStopPlaying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_DICE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWinningOrLosingIsPureLuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_DICE_RULES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTheRulesForLuckyDiceAint) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_DICE_PLAYER_FIRST_TIME, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringSometimesPeopleWhoHaveNeverPlayed) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_DICE_PLAYER_TRIPLE_DIGITS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringThinkingWontHelpYouWinThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_DICE_PLAYER_OUT_OF_COINS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringMyLuckHasRunOutFor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_WHEELS_PRIZE_SIGN, CONDITION_ALWAYS, EVENT(TolbiSpring_ReadLuckyWheelsPrizes) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_WHEELS_RULES_SIGN, CONDITION_ALWAYS, EVENT(TolbiSpring_ReadLuckyWheelsRules) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_DICE_RULES_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringLuckyDiceThrowTheDiceOn) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_DICE_PAIRS_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringLuckyDiceIfTwoNumbersMatch) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_SOLDIER, FLAG_BABI_SAVED, MSGNO(MsgTolbiSpringYeahThatDiceGameLooksKinda) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_SOLDIER, FLAG_BABI_SAVED, MSGNO(MsgTolbiSpringILikePlayingLuckyDiceBut) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringOhThereAreAlwaysScalpersAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_SOLDIER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTheresNoWayPoorSoldiersLike) },
    { TRIGGER_TABLE_END },
};

const struct Trigger gTolbiSpringTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, TOLBI_SPRING_TO_TOLBI },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_MEDAL_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiTossLuckyMedal) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_COIN_SIGN, CONDITION_ALWAYS, MSGNO(MsgTolbiFaceAwayTolbi) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR_MYSTERIOUS_POWERS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTolbiSpringHasMysteriousPowersStand) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_MEDAL_SEEKER, CONDITION_ALWAYS, EVENT(TolbiSpring_AskAboutLuckyMedal) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR_ONE_MEDAL, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringTooBadIOnlyHaveOne) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR_MYSTERIOUS_POWERS, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringIGetNervousWhenIToss) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_MEDAL_SEEKER, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringWhyWontAnyoneGiveMeA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR_ONE_MEDAL, CONDITION_ALWAYS, MSGNO(MsgTolbiSpringThisIsALuckyMedalSo) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SPRING_WATER, CONDITION_ALWAYS, EVENT(TolbiSpring_TossCoinOrMedal) },
    { TRIGGER_TABLE_END },
};

/// The water's fade as the spring drains, from mostly water to none, one
/// step every eighth frame.
const u16 gTolbiSpring_AlphaSteps[] = {
    BLEND_ALPHA(10, 9), BLEND_ALPHA(8, 10), BLEND_ALPHA(6, 11), BLEND_ALPHA(4, 12),
    BLEND_ALPHA(3, 13), BLEND_ALPHA(2, 14), BLEND_ALPHA(1, 15), BLEND_ALPHA(0, 16),
};

const u32 gTolbiSpring_PrizeItems[] = {
    ITEM_ASSASSIN_BLADE, ITEM_EARTH_SHIELD, ITEM_SPIRIT_ARMOR, ITEM_ADEPTS_HELM, ITEM_BURNING_AXE,
    ITEM_COCKTAIL_DRESS, ITEM_GLITTERING_TIARA, ITEM_GUARDIAN_ARMLET, ITEM_GRIEVOUS_MACE,
    ITEM_NINJA_HOOD, ITEM_BATTLE_GLOVES, ITEM_KIMONO, ITEM_POTION, ITEM_PSY_CRYSTAL,
    ITEM_WATER_OF_LIFE,
};

const u16 gTolbiSpring_CoinPrizes[] = {
    20, 10, 2, 1, 0, 0,
};

const u32 gTolbiSpring_LuckyWheelsPrizeItems[] = {
    ITEM_MYTHRIL_SHIRT, ITEM_SILK_SHIRT, ITEM_RUNNING_SHIRT, ITEM_HYPER_BOOTS, ITEM_QUICK_BOOTS, ITEM_FUR_BOOTS,
    ITEM_ADEPT_RING, ITEM_WAR_RING, ITEM_SLEEP_RING, ITEM_POTION, ITEM_VIAL, ITEM_NUT,
    ITEM_WATER_OF_LIFE, ITEM_PSY_CRYSTAL, ITEM_ELIXIR,
};

const u8 gTolbiSpring_SliderFrames[] = {
    0, 1, 9,
};

const u8 gTolbiSpring_CircleFrames[] = {
    0, 3, 4,
};

const u8 gTolbiSpring_ActorPixelX[] = {
    80, 160, 80, 72,
};

const u8 gTolbiSpring_ActorPixelZ[] = {
    32, 104, 68, 72,
};

const u16 gTolbiSpring_ActorHeadings[] = {
    FACING_EAST, 1, FACING_EAST, FACING_WEST,
};
// clang-format on

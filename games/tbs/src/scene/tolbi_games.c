/// @file
/// Two maps of Tolbi where the party plays for prizes whenever it is in town;
/// neither has a story script.
///
/// - Stucco rooms C (SceneId_StuccoRoomsC), a sheet of rooms drawn in stucco,
///   both of them Tolbi's, whose area name is Tolbi: the rooms of Lucky Dice
///   and Lucky Wheels; STUCCO_ROOMS_C_LUCKY_DICE_TO_TOLBI and
///   STUCCO_ROOMS_C_LUCKY_WHEELS_TO_TOLBI lead back into SceneId_Tolbi. The
///   Lucky Dice attendant asks ten coins for each level of the party's average
///   level: the coin total is saved in the save block and the party goes to the
///   Lucky Dice game, which returns to STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE,
///   where the setup has the attendant announce the coins won or lost, with a
///   fanfare by the size of the win. The Lucky Wheels attendant takes Game
///   Tickets and warns when the bag has six free slots or fewer; the game
///   returns to STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS with the prize
///   categories won, and the setup has the attendant hand over an item for
///   each, a different one of the category's three than last time (Moons choose
///   any category), or offers another game when nothing was won. Signs give
///   both games' rules and the Lucky Wheels prizes. A soldier talks of Lucky
///   Dice until Babi is saved (FLAG_BABI_SAVED, the finals day), then of the
///   scalpers at the Colosso finals, and is gone once they have been fought
///   (FLAG_COLOSSO_OVER).
/// - Tolbi Spring (SceneId_TolbiSpring); TOLBI_SPRING_TO_TOLBI leads back into
///   Tolbi (TOLBI_FROM_TOLBI_SPRING). Searching the spring raises it and blends
///   it into view the first time in each visit (FLAG_TOLBI_SPRING_RISEN),
///   stands the leader with their back to it, and offers a coin or a Lucky
///   Medal to toss. The toss is a small physics game: the token flies over the
///   spring past four moving obstacles and is pulled toward the prize rings
///   until it settles in one of five. A coin wins 20, 10, 2 or 1 coins by ring,
///   or nothing in the fifth; a medal wins an item drawn by weight from fifteen
///   prizes, the nearer the centre the fewer and better (Assassin Blade first,
///   Water of Life last), and the item won has its weight halved for later
///   tosses. Where the token settles is measured from TOLBI_SPRING_CENTER_X and
///   _Z, the rings ending at squared distances of 224, 624, 1,088 and 1,680
///   square pixels, which match the rings painted in the spring. The spring
///   opens to show the prize as it is given. In the editions before TBS_PATCH_1
///   the coin and medal counts show an `=` after each number.

#include "keystate.h"
#include "unit/scene/tolbi_games.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/shop/shop.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "script_layer.h"
#include "scene_tables.h"
#include "call.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "heap_slots.h"
#include "edition.h"
#include "field_service.h"
#include "save_block.h"
#include "inventory.h"
#include "menu_list.h"
#include "iwram_call.h"
#include "ability_ids.h"
#include "iowrite.h"
#include "frame.h"
#include "fixed_math.h"
#include "item_ids.h"
#include "map_grid.h"
#include "flags.h"
#include "songs.h"
#include "common/colosso_story_flags.h"

/// The spring has risen and faded into view this visit.
/// ★ Named from its area name: Tolbi Spring
#define FLAG_TOLBI_SPRING_RISEN FLAG_SCENE_TEMP_00

static void TolbiSpring_RiseAndFadeIn(void);
static void TolbiSpring_OfferLuckyWheels(s32 spoken);
static s32 TolbiSpring_RunSpringGame(void);
static s32 TolbiSpring_SelectLuckyWheelsPrize(s32 category);
static void TolbiSpring_InitializeTossSimulation(void);
static s32 TolbiSpring_RunToss(s32 tossKind);

enum SpringTossKind { SPRING_TOSS_COIN, SPRING_TOSS_MEDAL };

/// The actors of the Lucky Dice and Lucky Wheels rooms (StuccoRoomsC),
/// numbered in the order of gTolbiSpringActorTemplatesOther from
/// ACTOR_FIRST_PLACED.
enum TolbiSpringGameRoomActorId {
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN), who runs Lucky
  /// Wheels (TolbiSpring_TalkToWheelsAttendant).
  /// ★ Named from the game: "Step right up and try your hand at lucky wheels!"
  /// (MsgTolbiStepRightTry)
  ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT = ACTOR_FIRST_PLACED,
  /// A young man with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR), who runs
  /// Lucky Dice (TolbiSpring_OfferLuckyDice).
  /// ★ Named from the game: "Hey, buddy, wanna try yer luck at the dice?"
  /// (MsgTolbiBuddyWannaTry)
  ACTOR_TOLBI_SPRING_DICE_ATTENDANT,
  /// Beside the Lucky Dice attendant, in his sprite: likely a second
  /// attendant, because the Colosso's attendants wear the same sprite.
  /// ★ Named from the game: "Read the sign if ya don't know the rules."
  /// (MsgTolbiSpringReadTheSignIfYaDont)
  ACTOR_TOLBI_SPRING_DICE_RULES_ATTENDANT,
  /// A Lucky Dice player, a burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "Is this yer first time to play Lucky Dice?"
  /// (MsgTolbiYerFirstTime)
  ACTOR_TOLBI_SPRING_DICE_PLAYER_FIRST_TIME,
  /// An old man with a cane (SPRITE_OLD_MAN_WITH_CANE).
  /// ★ Named from the game: "Getting triple digits is real tough!"
  /// (MsgTolbiSpringWhoaGettingTripleDigitsIsReal)
  ACTOR_TOLBI_SPRING_DICE_PLAYER_TRIPLE_DIGITS,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "I don't want to stop playing Lucky Dice"
  /// (MsgTolbiSpringIDontWantToStopPlaying)
  ACTOR_TOLBI_SPRING_DICE_PLAYER_OUT_OF_COINS,
  /// The Lucky Wheels prize sign the layout draws, read through an
  /// actor with a blank sprite (TolbiSpring_ReadLuckyWheelsPrizes).
  /// ★ Named from the game: "Lucky Wheels Prizes"
  /// (MsgTolbiLuckyWheelsPrizesPrizesDetermined)
  ACTOR_TOLBI_SPRING_WHEELS_PRIZE_SIGN,
  /// The Lucky Wheels rules sign, read the same way
  /// (TolbiSpring_ReadLuckyWheelsRules).
  /// ★ Named from the game: "Lucky Wheels Rules"
  /// (MsgTolbiLuckyWheelsRulesPullLever)
  ACTOR_TOLBI_SPRING_WHEELS_RULES_SIGN,
  /// ★ Named from the game: "Throw the dice on the table"
  /// (MsgTolbiSpringLuckyDiceThrowTheDiceOn)
  ACTOR_TOLBI_SPRING_DICE_RULES_SIGN,
  /// A soldier, gone after Colosso, drawn with the long-haired fighter's
  /// sprite (SPRITE_PERSON_WITH_SWORD).
  /// ★ Named from the game: "poor soldiers like us"
  /// (MsgTolbiSpringTheresNoWayPoorSoldiersLike)
  ACTOR_TOLBI_SPRING_SOLDIER,
  /// ★ Named from the game: "If two numbers match on either"
  /// (MsgTolbiSpringLuckyDiceIfTwoNumbersMatch)
  ACTOR_TOLBI_SPRING_DICE_PAIRS_SIGN
};

/// Tolbi Spring's own actors (TolbiSpring), numbered in the order of
/// gTolbiSpringActorTemplates from ACTOR_FIRST_PLACED. The toss draws
/// the coin or the medal with its shadow and three trailing copies, which
/// TOLBI_SPRING_COIN_TRAIL and TOLBI_SPRING_MEDAL_TRAIL number.
enum TolbiSpringActorId {
  /// ★ Named from what the player sees: the tossed coin's shadow, drawn at
  /// ground level below it as it flies (its render)
  ACTOR_TOLBI_SPRING_COIN_SHADOW = ACTOR_FIRST_PLACED + 3,
  /// The coin the party tosses (SPRITE_SPRING_COIN).
  /// ★ Named from the game: "Face away from Tolbi Spring and toss a coin into
  /// it." (MsgTolbiFaceAwayTolbi)
  ACTOR_TOLBI_SPRING_COIN,
  /// ★ Named from what the player sees: the tossed Lucky Medal's shadow,
  /// drawn at ground level below it as it flies (its render)
  ACTOR_TOLBI_SPRING_MEDAL_SHADOW = ACTOR_TOLBI_SPRING_COIN + 4,
  /// The Lucky Medal the party tosses (SPRITE_SPRING_LUCKY_MEDAL).
  /// ★ Named from the game: "If you toss a Lucky Medal"
  /// (MsgTolbiTossLuckyMedal)
  ACTOR_TOLBI_SPRING_MEDAL,
  /// The four obstacles that move in the spring during a toss, in the order
  /// of sObstacles: two crabs (SPRITE_CRAB), then two turtles
  /// (SPRITE_TURTLE).
  /// ★ Named from what the player sees: two crabs and two turtles moving in
  /// the spring's water in the way of the toss (its render)
  ACTOR_TOLBI_SPRING_FIRST_OBSTACLE,
  /// A sign east of the spring (SPRITE_SIGNPOST).
  /// ★ Named from the game: "If you toss a Lucky Medal"
  /// (MsgTolbiTossLuckyMedal)
  ACTOR_TOLBI_SPRING_MEDAL_SIGN = ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 4,
  /// A sign west of the spring, in the same sprite.
  /// ★ Named from the game: "Face away from Tolbi Spring and toss a coin into
  /// it." (MsgTolbiFaceAwayTolbi)
  ACTOR_TOLBI_SPRING_COIN_SIGN,
  /// The tufts of grass at the foot of each sign: the signs' sprite in its
  /// second frame, mirrored and drawn underfoot (TolbiSpring_Initialize).
  /// ★ Named from what the player sees: a tuft of grass at the foot of each
  /// sign (its render)
  ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS,
  /// ★ Named from what the player sees: a tuft of grass at the foot of the
  /// west sign (its render)
  ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS,
  /// A visitor, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK).
  /// ★ Named from the game: "Tolbi Spring has mysterious powers."
  /// (MsgTolbiSpringTolbiSpringHasMysteriousPowersStand)
  ACTOR_TOLBI_SPRING_VISITOR_MYSTERIOUS_POWERS,
  /// A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) who asks for a medal.
  /// ★ Named from the game: "Do you have a Lucky Medal?" (MsgTolbiLuckyMedal)
  ACTOR_TOLBI_SPRING_MEDAL_SEEKER,
  /// A visitor, a woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Too bad I only have one Lucky Medal."
  /// (MsgTolbiSpringTooBadIOnlyHaveOne)
  ACTOR_TOLBI_SPRING_VISITOR_ONE_MEDAL
};

/// The actor that draws toss->trail[n] of a tossed coin or medal, n from
/// 1 to 3: the rows before each shadow, the oldest copy first.
#define TOLBI_SPRING_COIN_TRAIL(n) (ACTOR_TOLBI_SPRING_COIN_SHADOW - (n))
#define TOLBI_SPRING_MEDAL_TRAIL(n) (ACTOR_TOLBI_SPRING_MEDAL_SHADOW - (n))

/// What searching answers in Tolbi Spring.
enum TolbiSpringMetatileBehavior {
  MB_TOLBI_SPRING_WATER = 80, ///< searched to toss a coin or medal
};

// FLAG_TOLBI_SPRING_RISEN is set once the spring has risen and faded in.

/// The spring's centre, which the prize rings pull the tossed token toward.
#define TOLBI_SPRING_CENTER_X PIXELS(120)
#define TOLBI_SPRING_CENTER_Z PIXELS(71)

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

/// The tossed coin or medal: its last four positions for the trail, its
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

static __inline__ void Io_SetBlendControl(s32 value) { REG_BLDCNT = value; }

static __inline__ void Io_SetBlendAlpha(s32 value) { REG_BLDALPHA = value; }

const struct SceneEntrance *TolbiSpring_GetEntrances(void) {
  return gTolbiSpring_Entrances;
}

const struct SceneExitRegion *TolbiSpring_GetRegions(void) { return NULL; }

const u32 *TolbiSpring_GetExits(void) { return gTolbiSpring_Exits; }

/// Selects the actors of Tolbi Spring or of the Lucky Dice and Lucky
/// Wheels rooms.
const struct ActorTemplate *TolbiSpring_GetActorTemplates(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_TolbiSpring) {
    return gTolbiSpringActorTemplates;
  }
  return gTolbiSpringActorTemplatesOther;
}

/// A visitor at Tolbi Spring asks whether the party has a Lucky Medal.
static void TolbiSpring_AskAboutLuckyMedal(s32 actorId) {
  s32 question = MsgTolbiLuckyMedal;
  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(10);
    Event_SetMessageId(question + 1);
  } else {
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessageAndWait(actorId, 0);
}

/// A Lucky Dice player in Tolbi asks whether this is the party's first game.
static void TolbiSpring_AskAboutFirstDiceGame(s32 actorId) {
  s32 question = MsgTolbiYerFirstTime;
  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_WaitFrames(10);
    Event_SetMessageId(question + 1);
  } else {
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessageAndWait(actorId, 0);
}

/// Searching Tolbi Spring: the spring rises into view the first time in the
/// visit, and the leader stands with their back to it to toss a coin or a
/// Lucky Medal.
static void TolbiSpring_TossCoinOrMedal(void) {
  s32 leader = gSaveBlock.leaderActorId;

  if (Event_GetFlag(FLAG_TOLBI_SPRING_RISEN) == 0) {
    GameFlag_Set(FLAG_TOLBI_SPRING_RISEN);
    TolbiSpring_RiseAndFadeIn();
  }
  Event_Start();
  Actor_SetPositionAndReset(leader, 120, 152);
  Actor_ArmCallback(leader, FACING_SOUTH, 0);
  TolbiSpring_RunSpringGame();
  Event_End();
}

/// Reads the Lucky Wheels rules sign in Tolbi.
static void TolbiSpring_ReadLuckyWheelsRules(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiLuckyWheelsRulesPullLever);
  FieldMessage_ShowAtActor(-1, 0);
  Event_End();
}

/// Reads the Lucky Wheels prize sign in Tolbi.
static void TolbiSpring_ReadLuckyWheelsPrizes(void) {
  Event_Start();
  Event_SetMessageId(MsgTolbiLuckyWheelsPrizesPrizesDetermined);
  FieldMessage_ShowAtActor(-1, 0);
  Event_End();
}

/// Talking to the Lucky Wheels attendant in Tolbi.
static void TolbiSpring_TalkToWheelsAttendant(void) {
  TolbiSpring_OfferLuckyWheels(0);
}

/// Selects the triggers of Tolbi Spring or of the Lucky Dice and Lucky Wheels
/// rooms.
const struct Trigger *TolbiSpring_GetTriggers(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_TolbiSpring) {
    return gTolbiSpringTriggers;
  }
  return gTolbiSpringTriggersOther;
}

/// The setup of both maps. Tolbi Spring: the blend, the two mirrored props
/// drawn underfoot and, from Tolbi (TOLBI_SPRING_FROM_TOLBI), the toss's
/// obstacles, with the spring shown at once if it has risen in this visit. The
/// Lucky Dice and Lucky Wheels rooms: the soldier removed after
/// FLAG_COLOSSO_OVER, and the returns from the games,
/// STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE announcing Lucky Dice's coins and
/// STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS handing over Lucky Wheels' prizes.
s32 TolbiSpring_Initialize(void) {
  s32 coinChange;
  s8 *wonPrizes;
  s32 prizeIndex;
  s32 item;

  if (gSaveBlock.sceneId == (s32)&SceneId_TolbiSpring) {
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
               &REG_BLDCNT);
    Io_Write16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
    Import_ActorSetAnimation(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS, 2);
    Import_ActorSetAnimation(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS, 2);
    Actor_GetById(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS)->scaleX = -Q16_ONE;
    Actor_GetById(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS)->scaleX = -Q16_ONE;
    Actor_GetById(ACTOR_TOLBI_SPRING_EAST_SIGN_GRASS)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_TOLBI_SPRING_WEST_SIGN_GRASS)->priorityFlags =
        ACTOR_PRIORITY_UNDERFOOT;
    Event_StartScreenTransition();
    if (gSaveBlock.entrance == TOLBI_SPRING_FROM_TOLBI) {
      TolbiSpring_InitializeTossSimulation();
      if (GameFlag_Get(FLAG_TOLBI_SPRING_RISEN)) {
        Io_SetBlendControl(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND |
                           BLDCNT_TGT2_ALL);
        Io_SetBlendAlpha(BLEND_ALPHA(0, 16));
      }
    }
  } else {
    if (GameFlag_Get(FLAG_COLOSSO_OVER)) {
      Import_ActorSetPosition(ACTOR_TOLBI_SPRING_SOLDIER, 0, 0);
    }
    gOamCopyEnabled = 1;
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gSaveBlock.entrance == STUCCO_ROOMS_C_LUCKY_DICE_FROM_TOLBI) {
      Actor_ConfigureSpriteColor(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT,
                                 SPRITE_COLOR_GRB);
      Actor_ConfigureSpriteColor(ACTOR_TOLBI_SPRING_DICE_ATTENDANT,
                                 SPRITE_COLOR_RBG);
    }
    // Lucky Dice returns here to report the change in the party's coins.
    if (gSaveBlock.entrance == STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Event_Start();
      Actor_ConfigureSpriteColor(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT,
                                 SPRITE_COLOR_GRB);
      Actor_ConfigureSpriteColor(ACTOR_TOLBI_SPRING_DICE_ATTENDANT,
                                 SPRITE_COLOR_RBG);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_WaitFrames(10);
      Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 120, 112);
      Event_WaitFrames(20);
      coinChange = gSaveBlock.coins - *(s32 *)gSceneLocalState;
      if (coinChange > 0) {
        if (coinChange > 19999) {
          Sound_PlaySong(MUS_FANFARE_LARGE);
        } else if (coinChange > 4999) {
          Sound_PlaySong(MUS_FANFARE_MEDIUM);
        } else {
          Sound_PlaySong(MUS_FANFARE_SMALL);
        }
        Event_WaitFrames(20);
        Event_SetMessageId(MsgTolbiYaWonNumber);
        MessageArgument_Push(coinChange, 5);
        Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
        Sound_WaitForFanfare();
      } else if (coinChange < 0) {
        Event_SetMessageId(MsgTolbiYaLostNumber);
        MessageArgument_Push(-coinChange, 5);
        Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
      }
      Event_End();
    }
    // Lucky Wheels returns item categories for the attendant to award.
    if (gSaveBlock.entrance == STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS &&
        !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      wonPrizes = gSaveBlock.wonPrizes;
      Event_Start();
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_WaitFrames(10);
      if (wonPrizes[0] == -1) {
        TolbiSpring_OfferLuckyWheels(1);
      } else if (wonPrizes[0] != -2) {
        Event_SetMessageId(MsgTolbiCongratulations);
        Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
        if (*wonPrizes != -1) {
          for (prizeIndex = 0; wonPrizes[prizeIndex] != -1; prizeIndex++) {
            if (prizeIndex == 0) {
              Event_SetMessageId(MsgTolbiWonItemGo);
            } else {
              Event_SetMessageId(MsgTolbiWonItemGo2);
            }
            item = TolbiSpring_SelectLuckyWheelsPrize(wonPrizes[prizeIndex]);
            MessageArgument_Push(item, 2);
            Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
            ItemDisplay_ShowItem(item, 3);
            PartyInventory_GiveItem(item, 0);
            Event_WaitFrames(10);
            Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
            Event_WaitFrames(30);
          }
        }
        gSaveBlock.wonPrizes[0] = -2;
        Event_SetMessageId(MsgTolbiComeAgain);
        Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      }
      Event_End();
    }
  }
  return 0;
}

/// Displays one frame of the spring's opening or closing prize animation.
static inline void TolbiSpring_ShowPrizeFrame(s32 sourceColumn, s32 sourceRow) {
  MapGrid_CopyCellsTo(sourceColumn, sourceRow, 70, 0, 3, 8);
  Sound_PlaySong(SE_CAST);
  Event_WaitFrames(8);
}

/// Animates the spring's prize reveal, gives the item, and closes it.
static void TolbiSpring_RevealPrize(s32 item) {
  Event_Start();
  Event_WaitFrames(30);
  Sound_PlaySong(SE_BOSS_ROAR);
  Event_WaitFrames(100);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Event_WaitFrames(40);

  MapGrid_CopyCellsTo(82, 20, 70, 0, 3, 8);
  Event_WaitFrames(3);
  TolbiSpring_ShowPrizeFrame(85, 20);
  TolbiSpring_ShowPrizeFrame(88, 20);
  TolbiSpring_ShowPrizeFrame(91, 20);
  TolbiSpring_ShowPrizeFrame(94, 20);
  TolbiSpring_ShowPrizeFrame(97, 20);
  TolbiSpring_ShowPrizeFrame(100, 20);

  TolbiSpring_ShowPrizeFrame(79, 29);
  TolbiSpring_ShowPrizeFrame(82, 29);
  TolbiSpring_ShowPrizeFrame(85, 29);
  TolbiSpring_ShowPrizeFrame(88, 29);
  TolbiSpring_ShowPrizeFrame(91, 29);
  TolbiSpring_ShowPrizeFrame(94, 29);
  TolbiSpring_ShowPrizeFrame(97, 29);
  MapGrid_CopyCellsTo(100, 29, 70, 0, 3, 8);
  Sound_PlaySong(SE_CAST);

  Event_WaitFrames(70);
  Sound_PlaySong(SE_RECOVERY);
  ItemDisplay_ShowItem(item, 3);
  PartyInventory_GiveItem(item, 0);
  Event_WaitFrames(20);

  TolbiSpring_ShowPrizeFrame(97, 29);
  TolbiSpring_ShowPrizeFrame(94, 29);
  TolbiSpring_ShowPrizeFrame(91, 29);
  TolbiSpring_ShowPrizeFrame(88, 29);
  TolbiSpring_ShowPrizeFrame(85, 29);
  TolbiSpring_ShowPrizeFrame(82, 29);
  TolbiSpring_ShowPrizeFrame(100, 20);
  TolbiSpring_ShowPrizeFrame(97, 20);
  TolbiSpring_ShowPrizeFrame(94, 20);
  TolbiSpring_ShowPrizeFrame(91, 20);
  TolbiSpring_ShowPrizeFrame(88, 20);
  TolbiSpring_ShowPrizeFrame(85, 20);
  TolbiSpring_ShowPrizeFrame(82, 20);
  TolbiSpring_ShowPrizeFrame(79, 20);
  Event_End();
}

/// Raises the spring's displayed layer and blends it into view.
static void TolbiSpring_RiseAndFadeIn(void) {
  u8 *springLayer = gMapGridWork[0];
  const u16 *alphaSteps;
  s32 stepIndex;

  Sound_PlaySong(SE_BURST);
  // This value belongs to layer 1; its individual field remains unnamed.
  springLayer += offsetof(struct MapGridScrollAnimView, layers[1]) + 4;
  for (stepIndex = 0; stepIndex < 16; stepIndex++) {
    *(s32 *)(springLayer + 12) -= PIXELS(1);
    WaitFrames(4);
  }
  alphaSteps = gTolbiSpring_AlphaSteps;
  for (stepIndex = 0; stepIndex < 8; stepIndex++) {
    Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
               &REG_BLDCNT);
    Io_Write16(*alphaSteps++, &REG_BLDALPHA);
    WaitFrames(8);
  }
}

/// The Lucky Wheels attendant in Tolbi checks tickets and inventory space
/// before sending the party to the game, which returns to
/// STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS; `spoken` is 1 after a game with
/// no prize, to offer another.
static void TolbiSpring_OfferLuckyWheels(s32 spoken) {
  s32 tickets = PartyInventory_CountItem(ITEM_GAME_TICKET);
  s32 room;

  Actor_Snapshot();
  if (spoken == 0) {
    s32 message = MsgTolbiStepRightTry;

    Event_SetMessageId(message);
    Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (tickets == 0) {
      Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    Event_SetMessageId(message + 2);
    MessageArgument_Push(tickets, 5);
    FieldMessage_ShowAtActor(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
      Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    room = PartyInventory_CountFreeSlots();
    if (room == 0) {
      Event_SetMessageId(message + 4);
      FieldMessage_ShowAtActor(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    } else if (room <= 6) {
      Event_SetMessageId(message + 5);
      FieldMessage_ShowAtActor(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    }
    if (room <= 6 && Event_AskYesNoWithActorAnimation(0, 0) != 0) {
      Event_SetMessageId(MsgTolbiGetRidItems);
      Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
  } else {
    if (tickets == 0) {
      Event_SetMessageId(MsgTolbiBadComeBack);
      Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
    Event_SetMessageId(MsgTolbiBadLikeUse);
    FieldMessage_ShowAtActor(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
      Event_SetMessageId(MsgTolbiComeAgain);
      Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
      return;
    }
  }
  Event_SetMessageId(MsgTolbiStepRightEnjoy);
  Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_WHEELS_ATTENDANT, 0);
  Event_LeaveFor(SEQUENCE_LUCKY_WHEELS, 0);
  Event_SetAfterBattleDestination((s32)&SceneId_StuccoRoomsC,
                                  STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_WHEELS);
}

/// Talking to the Lucky Dice attendant in Tolbi: offers a game for ten coins a
/// level of the party's average level and saves the coin total for the return
/// to STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE.
static void TolbiSpring_OfferLuckyDice(void) {
  u32 cost;
  u32 coins;

  cost = Party_GetAverageLevel() * 10;
  Event_Start();
  coins = gSaveBlock.coins;
  if (coins < cost) {
    Event_SetMessageId(MsgTolbiYaDontEnough);
    FieldMessage_ShowAtActor(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
    return;
  }
  *(u32 *)gSceneLocalState = coins;
  Actor_Snapshot();
  Event_SetMessageId(MsgTolbiBuddyWannaTry);
  MessageArgument_Push(cost, 5);
  FieldMessage_ShowAtActor(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
    Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 120, 128);
    Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 120, 152);
    Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Event_WaitFrames(20);
    Event_LeaveFor(SEQUENCE_LUCKY_DICE, 0);
    Event_SetAfterBattleDestination((s32)&SceneId_StuccoRoomsC,
                                    STUCCO_ROOMS_C_AFTER_OFFER_LUCKY_DICE);
  } else {
    gFieldWork->message++;
    Event_ShowMessageAndWait(ACTOR_TOLBI_SPRING_DICE_ATTENDANT, 0);
  }
  Event_End();
}

/// Chooses a coin or medal, runs its toss, and awards the resulting prize.
static s32 TolbiSpring_RunSpringGame(void) {
  s32 choice = 0;
  s32 text;
  s32 cancel = -1;
  s32 coins;
  s32 medals;
  struct Window *window;
  s32 result;
  s32 totalWeight;
  s32 prizeIndex;
  s32 prizeRoll;
  s32 message;
  struct SaveBlock *state;

  Event_Start();
  Field_EndBusyAndReveal();
  message = MsgTolbiThrow;
#if TBS_PATCH_1
  state = &gSaveBlock;
prompt:
  coins = state->coins;
  medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
  Event_SetMessageId(message);
  Event_OpenMessage(-1, 0);
  window = Window_Create(0, 0, TOLBI_SPRING_COUNT_WIDTH, 4, 2);
  {
    text = MsgTolbiCoins;
    Text_DrawMessageAsTiles(text, window, 0, 0);
    Text_DrawNumberInWindow(coins, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 0);
    Text_DrawMessageAsTiles(text + 1, window, 0, 8);
    Text_DrawNumberInWindow(medals, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 8);
  }
  choice = Menu_SelectCoinsOrMedals(choice);
  Window_Close(window, 2);
  TextPrinter_CloseFinishedWindows();
  if (choice == cancel) {
    Event_End();
    return 0;
  }
  if (choice == SPRING_TOSS_COIN) {
    if (coins == 0) {
      Event_SetMessageId(message + 1);
      Event_ShowMessage(-1, 0);
      WaitFrames(1);
      goto prompt;
    }
  } else if (choice == SPRING_TOSS_MEDAL) {
    if (medals == 0) {
      Event_SetMessageId(message + 2);
      Event_ShowMessage(-1, 0);
      WaitFrames(1);
      goto prompt;
    }
    if (PartyInventory_CountFreeSlots() == 0) {
      Event_SetMessageId(message + 4);
      Event_OpenMessage(-1, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
        goto end;
      }
    }
  }
  window = Window_Create(TOLBI_SPRING_TOSS_MENU_X, 15,
                         TOLBI_SPRING_TOSS_MENU_WIDTH, 4, 2);
  {
    text = MsgTolbiToss;
    Text_DrawMessageAsTiles(text, window, 0, 0);
    Text_DrawMessageAsTiles(text + 1, window, 0, 8);
  }
  WaitFrames(5);
  Event_PlaySong(SE_TOLBI_SPRING_TOSS_PROMPT);
  for (;;) {
    if (gKeysPressed & KEY_A) {
      Event_PlaySong(SE_MENU_CONFIRM);
      result = 0;
      break;
    }
    if (gKeysPressed & KEY_B) {
      Event_PlaySong(SE_MENU_CANCEL);
      result = -1;
      break;
    }
    WaitFrames(1);
  }
  Window_Close(window, 2);
#else
  /* FAKEMATCH(barrier): message kept in r10 and the coin label in r9; as a
   * plain local, the two swap and 8 of 290 instructions differ in tbs-ja. */
  asm("" : "+r"(message));
  for (;;) {
    text = MsgTolbiCoins;
    /* FAKEMATCH(barrier): the coin label kept in r9 and the marker in the pool;
     * as a plain local, the marker takes fp, state r9, and 10 of 290
     * instructions differ in tbs-ja. */
    asm("" : "+r"(text));
    state = &gSaveBlock;
    coins = state->coins;
    medals = PartyInventory_CountItem(ITEM_LUCKY_MEDAL);
    Event_SetMessageId(message);
    Event_OpenMessage(-1, 0);
    window = Window_Create(0, 0, TOLBI_SPRING_COUNT_WIDTH, 4, 2);
    Text_DrawMessageAsTiles(text, window, 0, 0);
    Text_DrawNumberInWindow(coins, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 0);
    Text_DrawMessageAsTiles(MsgTolbiCounterMarker, window, 104, 0);
    Text_DrawMessageAsTiles(text + 1, window, 0, 8);
    Text_DrawNumberInWindow(medals, 6, window, TOLBI_SPRING_COUNT_NUMBER_X, 8);
    Text_DrawMessageAsTiles(MsgTolbiCounterMarker, window, 104, 8);
    choice = Menu_SelectCoinsOrMedals(choice);
    Window_Close(window, 2);
    TextPrinter_CloseFinishedWindows();
    if (choice == cancel) {
      Event_End();
      return 0;
    }
    if (choice == SPRING_TOSS_COIN) {
      if (coins == 0) {
        Event_SetMessageId(message + 1);
        Event_ShowMessage(-1, 0);
        WaitFrames(1);
        continue;
      }
    } else if (choice == SPRING_TOSS_MEDAL) {
      if (medals == 0) {
        Event_SetMessageId(message + 2);
        Event_ShowMessage(-1, 0);
        WaitFrames(1);
        continue;
      }
      if (PartyInventory_CountFreeSlots() == 0) {
        Event_SetMessageId(message + 4);
        Event_OpenMessage(-1, 0);
        if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
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
    WaitFrames(5);
    Event_PlaySong(SE_TOLBI_SPRING_TOSS_PROMPT);
    for (;;) {
      if (gKeysPressed & KEY_A) {
        Event_PlaySong(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if (gKeysPressed & KEY_B) {
        Event_PlaySong(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      WaitFrames(1);
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
#ifdef UBFIX
        Party_AdjustCoins(gTolbiSpring_CoinPrizes[result]);
#else
        // UB: the original view indexes beyond its one-element array.
        // Plain arrays change register allocation and add four bytes in tbs-en.
        Party_AdjustCoins(((struct { u16 coins[1]; } *)gTolbiSpring_CoinPrizes)
                              ->coins[result]);
#endif
        Event_PlaySong(MUS_FANFARE_SMALL);
#ifdef UBFIX
        MessageArgument_Push(gTolbiSpring_CoinPrizes[result], 5);
#else
        // UB: retain the same native one-element view for this second read.
        MessageArgument_Push(
            ((struct { u16 coins[1]; } *)gTolbiSpring_CoinPrizes)
                ->coins[result],
            5);
#endif
        Event_SetMessageId(MsgTolbiWonNumberCoin);
        Event_ShowMessage(-1, 0);
      } else {
        Event_PlaySong(SE_MENU_CANCEL);
        Event_WaitFrames(10);
      }
    } else {
      // The prize ring limits the weighted pool of medal rewards.
      totalWeight = 0;
      prizeIndex = 0;
      if (prizeIndex < result * 3 + 3) {
        do {
          u8 prizeWeight = ((u8 *)state)[prizeIndex + 285];
          totalWeight += (s8)prizeWeight;
          prizeIndex++;
        } while (prizeIndex < result * 3 + 3);
      }
      prizeRoll = (u32)(Random_Next() * totalWeight) >> 16;
      for (prizeIndex = 0; prizeIndex < 15; prizeIndex++) {
        prizeRoll -= state->prizeWeights[prizeIndex];
        if (prizeRoll < 0) {
          break;
        }
      }
      if (prizeIndex == 15) {
        prizeIndex = 14;
      }
      // Reveal the selected item, then reduce its chance on later tosses.
#ifdef UBFIX
      TolbiSpring_RevealPrize(gTolbiSpring_PrizeItems[prizeIndex]);
#else
      // UB: use the whole item array in modern C; preserve the native view.
      TolbiSpring_RevealPrize(
          ((struct { s32 prize[1]; } *)gTolbiSpring_PrizeItems)
              ->prize[prizeIndex]);
#endif

      /* FAKEMATCH(dead): the pointer pair, so the prize-weight base is computed
       * again after the call; without it, 6 of 280 instructions differ in
       * tbs-en. */
      state++;
      state--;
      {
        /* FAKEMATCH(pin): the slot in r1, where the native keeps it; as a
         * plain s32, 5 of 280 instructions differ in tbs-en. */
        register s32 slot asm("r1") = prizeIndex + 284;
        s8 *prizeWeight = (s8 *)state + 1;

        if (prizeWeight[slot] > 1) {
          prizeWeight[slot] /= 2;
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

/// Chooses an item variant in the prize category, avoiding its last variant.
static s32 TolbiSpring_SelectLuckyWheelsPrize(s32 category) {
  s32 previousVariant;
  s32 variant;

  if (category < 0) {
    return 0;
  }

  if (category == 5) {
    category = ((unsigned int)(Import_RandomNext() * 5) >> 16);
  }

  previousVariant = gSaveBlock.wonPrizes[8 + category];

  variant = (previousVariant +
             (s32)((unsigned int)(Import_RandomNext() * 2) >> 16) + 4) %
            3;

  gSaveBlock.wonPrizes[8 + category] = variant;

  return gTolbiSpring_LuckyWheelsPrizeItems[category * 3 + variant];
}

/// Copies three coordinates and sets the actor's facing, child value and
/// colors.
static void TolbiSpring_SetActorAppearance(s32 actorId, const s32 *coordinate,
                                           s32 facing, s32 colorVariant,
                                           s32 childValue) {
  struct Actor *actor = Actor_GetById(actorId);
  struct Sprite *sprite;
  u32 entryCount;

  if (actor != 0) {
    actor->x.fixed = *coordinate++;
    actor->y.fixed = *coordinate++;
    actor->z.fixed = *coordinate;
    actor->facing = facing;
    actor->motionFlags = 0;
    actor->sprite->flags = 0;
    Actor_SetAnimationSpeed(actor, childValue);
  }
  // BUG: the original still dereferences actor here when lookup returned
  // NULL.
  sprite = actor->sprite;
  entryCount = sprite->count;
  if (entryCount != 0) {
    u32 invalidFrame = 255;
    struct SpriteAnim **entries = sprite->entries;
    u32 entriesLeft = entryCount;

    do {
      struct SpriteAnim *entry = *entries++;

      if (entry->colorMode != colorVariant) {
        entry->colorMode = colorVariant;
        {
          u32 frame = entry->frame;

          // Invalidate the cached frame so the new colors are applied.
          frame |= invalidFrame;
          entry->frame = frame;
        }
      }
    } while (--entriesLeft != 0);
  }
}

/// Sets whether the specified actor draws its sprite.
static void TolbiSpring_SetSpriteStorageKind(s32 actorId, s32 storageKind) {
  struct Actor *actor = Actor_GetById(actorId);

  if (actor != 0) {
    u8 *spriteStorageKind = &actor->spriteStorageKind;

    *spriteStorageKind = storageKind;
  }
}

static struct SpringTossMotion sToss;

static s32 sTossKind;

/// Three words nothing refers to; removing them would move the obstacle state.
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
  s32 edgeInset;
  s32 motionStep;
  s32 speedMode;
  s32 cycleFrame;
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
  /* FAKEMATCH(dead): the unused vector keeps twelve frame bytes; without it,
   * 2 of 947 instructions differ in tbs-en. */
  UNUSED struct Vector3 spare;

  toss = &sToss;
  // Retain the previous three positions for the visible toss trail.
  index = 3;
  do {
    toss->trail[index].x = toss->trail[index - 1].x;
    toss->trail[index].y = toss->trail[index - 1].y;
    toss->trail[index].z = toss->trail[index - 1].z;
    index--;
  } while (index != 0);

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
          Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_MEDAL), 1);
        } else {
          Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_COIN), 1);
        }
      }

      // Pull the floating token toward the prize rings, then let it slow down.
      if (toss->attractionFrames > 0) {
        s32 deltaX;
        s32 deltaZ;
        s32 distance;

        deltaX = (TOLBI_SPRING_CENTER_X - toss->trail[0].x) >> 8;
        deltaZ = (TOLBI_SPRING_CENTER_Z - toss->trail[0].z) >> 8;
        // BUG: an exact center position leaves a zero divisor below.
        distance = Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
        toss->velocity.x += ((PIXELS(1) / 10) * deltaX) / distance;
        toss->velocity.z += ((PIXELS(1) / 10) * deltaZ) / distance;
        toss->velocity.x = toss->velocity.x * 253 / 256;
        toss->velocity.z = toss->velocity.z * 253 / 256;
        toss->attractionFrames = toss->attractionFrames - 1;
      } else {
        toss->velocity.x = toss->velocity.x * 220 / 256;
        toss->velocity.z = toss->velocity.z * 220 / 256;
        if (toss->velocity.x > -PIXELS(1) / 64 &&
            toss->velocity.x < PIXELS(1) / 64) {
          toss->velocity.x = 0;
        }
        if (toss->velocity.z > -PIXELS(1) / 64 &&
            toss->velocity.z < PIXELS(1) / 64) {
          toss->velocity.z = 0;
        }
        if (toss->velocity.x == 0 && toss->velocity.z == 0) {
          if (sTossKind == SPRING_TOSS_MEDAL) {
            Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_MEDAL), 2);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(1), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(2), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(3), 0);
          } else {
            Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_COIN), 2);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(1), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(2), 0);
            TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(3), 0);
          }
          {
            s32 deltaX = (TOLBI_SPRING_CENTER_X - toss->trail[0].x) >> 16;
            s32 deltaZ = (TOLBI_SPRING_CENTER_Z - toss->trail[0].z) >> 16;
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
      minX = PIXELS(48);
      maxX = PIXELS(192);
      minZ = PIXELS(24);
      maxZ = PIXELS(120);
      if (toss->trail[0].z < PIXELS(42)) {
        edgeInset = (PIXELS(42) - toss->trail[0].z) * 42 / 18;
        minX = PIXELS(48) + edgeInset;
        if (minX > PIXELS(90)) {
          minX = PIXELS(90);
        }
        maxX = PIXELS(192) - edgeInset;
        if (maxX < PIXELS(150)) {
          maxX = PIXELS(150);
        }
      }
      if (toss->trail[0].z > PIXELS(102)) {
        edgeInset = (toss->trail[0].z * 42 - PIXELS(102) * 42) / 18;
        minX = PIXELS(48) + edgeInset;
        if (minX > PIXELS(90)) {
          minX = PIXELS(90);
        }
        maxX = PIXELS(192) - edgeInset;
        if (maxX < PIXELS(150)) {
          maxX = PIXELS(150);
        }
      }
      if (toss->trail[0].x < PIXELS(90)) {
        edgeInset = (PIXELS(90) - toss->trail[0].x) * 18 / 42;
        minZ = PIXELS(24) + edgeInset;
        if (minZ > PIXELS(42)) {
          minZ = PIXELS(42);
        }
        maxZ = PIXELS(120) - edgeInset;
        if (maxZ < PIXELS(102)) {
          maxZ = PIXELS(102);
        }
      }
      if (toss->trail[0].x > PIXELS(150)) {
        edgeInset = (toss->trail[0].x * 18 - PIXELS(150) * 18) / 42;
        minZ = PIXELS(24) + edgeInset;
        if (minZ > PIXELS(42)) {
          minZ = PIXELS(42);
        }
        maxZ = PIXELS(120) - edgeInset;
        if (maxZ < PIXELS(102)) {
          maxZ = PIXELS(102);
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
      toss->velocity.y -= PIXELS(1) / 4;
    }
  }

  // Two obstacles slide across X; two follow timed elliptical paths.
  index = 0;
  do {
    obstacle = &sObstacles[index];

    if (obstacle->reactionFrames > 0) {
      obstacle->reactionFrames = obstacle->reactionFrames - 1;
    }
    if (obstacle->hitCooldownFrames > 0) {
      obstacle->hitCooldownFrames = obstacle->hitCooldownFrames - 1;
    }

    if (index <= 1) {
      speedMode = obstacle->speedMode;
      motionStep = PIXELS(1);
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        if (index == 0) {
          Actor_SelectAnimation(
              Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE), 3);
        } else {
          Actor_SelectAnimation(
              Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1), 3);
        }
      } else {
        if (index == 0) {
          Actor_SelectAnimation(
              Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE), 1);
        } else {
          Actor_SelectAnimation(
              Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1), 1);
        }
        if (obstacle->pauseOrPhase == 0) {
          if (obstacle->heading == 0) {
            obstacle->position.x = obstacle->position.x + motionStep;
          } else {
            obstacle->position.x = obstacle->position.x - motionStep;
          }
          if (obstacle->position.x <= PIXELS(64)) {
            obstacle->heading = 0;
            if (index == 1) {
              obstacle->pauseOrPhase = 30;
            }
          }
          if (obstacle->position.x > (PIXELS(176) - 1)) {
            obstacle->heading = 1;
            if (index == 1) {
              obstacle->pauseOrPhase = 30;
            }
          }
        } else {
          obstacle->pauseOrPhase = obstacle->pauseOrPhase - 1;
        }
      }
    } else if (index == 2) {
      speedMode = obstacle->speedMode;
      motionStep = -64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        Actor_SelectAnimation(
            Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2), 3);
      } else {
        Actor_SelectAnimation(
            Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2), 2);
        obstacle->position.x = Trig_Sin(obstacle->heading) * 48 + PIXELS(112);
        obstacle->position.z = Trig_Cos(obstacle->heading) * 40 + PIXELS(72);
        cycleFrame = (u16)obstacle->heading + motionStep;
        obstacle->heading = cycleFrame;
        obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
      }
    } else {
      cycleFrame = obstacle->pauseOrPhase & 511;
      speedMode = obstacle->speedMode;
      motionStep = 64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
        Actor_SelectAnimation(
            Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3), 3);
      } else if (cycleFrame <= 383) {
        obstacle->position.x = Trig_Sin(obstacle->heading) * 52 + PIXELS(112);
        obstacle->position.z = Trig_Cos(obstacle->heading) * 24 + PIXELS(72);
        obstacle->heading = obstacle->heading + motionStep;
        Actor_SelectAnimation(
            Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3), 2);
      } else {
        Actor_SelectAnimation(
            Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3), 3);
      }
      obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
    }

    // A nearby obstacle redirects the token and shortens its attraction time.
    if (obstacle->hitCooldownFrames == 0 && toss->trail[0].y == 0) {
      s32 deltaX;
      s32 deltaZ;
      s32 distanceSquared;

      deltaX = (obstacle->position.x - toss->trail[0].x) >> 16;
      deltaZ = (obstacle->position.z - toss->trail[0].z) >> 16;
      distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
      if (distanceSquared <= 119 && toss->attractionFrames > 30) {
        s32 speed;
        s32 nextMode;

        speed = PIXELS(3);
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
        Sound_PlaySong(SE_TOLBI_SPRING_OBSTACLE_HIT);
        nextMode = (obstacle->speedMode + 1) % 3;
        obstacle->reactionFrames = 36;
        obstacle->speedMode = nextMode;
        obstacle->hitCooldownFrames = 30;
      }
    }

    // Apply the obstacle's position, orientation and speed-dependent colors.
    switch (index) {
    case 0:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE,
          Vector3_Coordinates(&obstacle->position), 0,
          gTolbiSpring_SliderFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 1:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 1,
          Vector3_Coordinates(&obstacle->position), 0,
          gTolbiSpring_SliderFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 2:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2,
          Vector3_Coordinates(&obstacle->position), 0x8000 - obstacle->heading,
          gTolbiSpring_CircleFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    case 3:
      TolbiSpring_SetActorAppearance(
          ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3,
          Vector3_Coordinates(&obstacle->position), 0xffff - obstacle->heading,
          gTolbiSpring_CircleFrames[obstacle->speedMode],
          (obstacle->speedMode << 4) + 16);
      break;
    }

    index++;
  } while (index != 4);

  // Draw the token, its ground-level shadow and the three trailing copies.
  toss->shadow.x = toss->trail[0].x;
  toss->shadow.y = 0;
  toss->shadow.z = toss->trail[0].z;

  if (sTossKind == SPRING_TOSS_MEDAL) {
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_MEDAL,
                                   Vector3_Coordinates(&toss->trail[0]), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_MEDAL_SHADOW,
                                   Vector3_Coordinates(&toss->shadow), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(1),
                                   Vector3_Coordinates(&toss->trail[1]), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(2),
                                   Vector3_Coordinates(&toss->trail[2]), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_MEDAL_TRAIL(3),
                                   Vector3_Coordinates(&toss->trail[3]), 0, 0,
                                   16);
    Actor_SelectAnimation(Actor_GetById(TOLBI_SPRING_MEDAL_TRAIL(1)), 4);
    Actor_SelectAnimation(Actor_GetById(TOLBI_SPRING_MEDAL_TRAIL(2)), 4);
    Actor_SelectAnimation(Actor_GetById(TOLBI_SPRING_MEDAL_TRAIL(3)), 4);
  } else {
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_COIN,
                                   Vector3_Coordinates(&toss->trail[0]), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(ACTOR_TOLBI_SPRING_COIN_SHADOW,
                                   Vector3_Coordinates(&toss->shadow), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_COIN_TRAIL(1),
                                   Vector3_Coordinates(&toss->trail[1]), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_COIN_TRAIL(2),
                                   Vector3_Coordinates(&toss->trail[2]), 0, 0,
                                   16);
    TolbiSpring_SetActorAppearance(TOLBI_SPRING_COIN_TRAIL(3),
                                   Vector3_Coordinates(&toss->trail[3]), 0, 0,
                                   16);
    Actor_SelectAnimation(Actor_GetById(TOLBI_SPRING_COIN_TRAIL(1)), 4);
    Actor_SelectAnimation(Actor_GetById(TOLBI_SPRING_COIN_TRAIL(2)), 4);
    Actor_SelectAnimation(Actor_GetById(TOLBI_SPRING_COIN_TRAIL(3)), 4);
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
    obstacle->position.x = *pixelX << 16;
    obstacle->position.z = *pixelZ << 16;
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

  toss->trail[0].x = -PIXELS(30);
  toss->trail[0].y = 0;
  toss->trail[0].z = PIXELS(100);
  toss->velocity.x = 0;
  toss->velocity.y = 0;
  toss->velocity.z = 0;
  toss->attractionFrames = 0;

  Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 2),
                        2);
  Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_FIRST_OBSTACLE + 3),
                        2);

  {
    s32 budget = 0xc83;
    void (*task)(void) = TolbiSpring_UpdateTossSimulation;

    Task_AddFrame(task, budget);
  }
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
      Sound_PlaySong(SE_TOLBI_SPRING_TOSS);
    }
    // Release the token on the leader's throw frame.
    if (sTossFrame == 16) {
      Import_ActorSetAnimation(gSaveBlock.leaderActorId, ANIM_LEADER_TOSS);
      toss->launchFrame = 0;
      toss->velocity.x = 13 * PIXELS(1) / 10;
      toss->velocity.y = PIXELS(4);
      toss->velocity.z = -PIXELS(2);
      toss->trail[0].x = PIXELS(120);
      toss->trail[0].y = PIXELS(16);
      toss->trail[0].z = PIXELS(152);
      toss->attractionFrames = 300;
      if (sTossKind == SPRING_TOSS_MEDAL) {
        Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_MEDAL_SHADOW),
                              3);
        Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_MEDAL), 0);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(1), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(2), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_MEDAL_TRAIL(3), 1);
      } else {
        Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_COIN_SHADOW), 3);
        Actor_SelectAnimation(Actor_GetById(ACTOR_TOLBI_SPRING_COIN), 0);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(1), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(2), 1);
        TolbiSpring_SetSpriteStorageKind(TOLBI_SPRING_COIN_TRAIL(3), 1);
      }
    }
    WaitFrames(1);
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
                      TOLBI_FROM_HOUSE_NORTHEAST, 0xffffffff),
    SCENE_EXITS_END,
};

// clang-format off
const struct ActorTemplate gTolbiSpringActorTemplates[] = {
    { SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(176), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(72), 0, PIXELS(144), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(176), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(72), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(48), 0, PIXELS(136), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(24), 0, PIXELS(72), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(96), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct ActorTemplate gTolbiSpringActorTemplatesOther[] = {
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(144), 0, PIXELS(360), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(120), 0, PIXELS(82), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(82), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(128), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(96), 0, PIXELS(168), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(104), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(96), 0, PIXELS(336), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(168), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(160), 0, PIXELS(112), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(184), 0, PIXELS(152), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(80), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
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
    { TRIGGERS_END },
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
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_TOLBI_SPRING_WATER, CONDITION_ALWAYS, EVENT(TolbiSpring_TossCoinOrMedal) },
    { TRIGGERS_END },
};

/// BLDALPHA steps.
const u16 gTolbiSpring_AlphaSteps[] = {
    0x90a, 0xa08, 0xb06, 0xc04, 0xd03, 0xe02, 0xf01, 0x1000,
};

const u32 gTolbiSpring_PrizeItems[] = {
    23, 124, 81, 152, 37, 108, 171, 142,
    48, 160, 133, 97, 183, 186, 189,
};

const u16 gTolbiSpring_CoinPrizes[] = {
    20, 10, 2, 1, 0, 0,
};

const u32 gTolbiSpring_LuckyWheelsPrizeItems[] = {
    250, 251, 252, 256, 257, 258, 262, 263,
    264, 183, 182, 181, 189, 186, 188,
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
    FACING_EAST, 0x1, FACING_EAST, FACING_WEST,
};
// clang-format on

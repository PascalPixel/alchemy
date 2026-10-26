#ifndef STUCCO_ROOMS_B_H
#define STUCCO_ROOMS_B_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "common/colosso_story_flags.h"
#include "flags.h"

/// The placed actors, numbering the rows of the actor template
/// table from ACTOR_FIRST_PLACED. Both tables hold the same people in the
/// same order up to the healer, except that after Colosso
/// (gStuccoRoomsBActorTemplatesAfterColosso) two others stand in the
/// rows of ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST and
/// ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, and six more rows follow the
/// healer.
enum StuccoRoomsBActorId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who runs a
  /// makeshift inn during the festival.
  /// ★ Named from the game: "I just line up some beds, and presto! I'm an
  /// innkeeper!" (MsgTolbiInteriorsTolbiIsPackedWithTouristsDuring)
  ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who offers a stay.
  /// ★ Named from the game: "Do you want to stay with us?" (MsgTolbiWantStay)
  ACTOR_STUCCO_ROOMS_B_HOST,
  /// A strongman (SPRITE_STRONGMAN), during the festival.
  /// ★ Named from the game: "I couldn't sleep at all last night."
  /// (MsgTolbiInteriorsICouldntSleepAtAllLast)
  ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
  /// A stout man (SPRITE_STOUT_MAN), during the festival.
  /// ★ Named from the game: "The crowds at the colosseum made me feel sick"
  /// (MsgTolbiInteriorsTheCrowdsAtTheColosseumMade)
  ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST,
  /// An old woman (SPRITE_OLD_WOMAN); on finals day, "I'm stuck
  /// housesitting!"
  /// ★ Named from the game: "We paid good money to rent this house"
  /// (MsgTolbiInteriorsWePaidGoodMoneyToRent)
  ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "so I'm just going to watch the finals."
  /// (MsgTolbiInteriorsIReallyWantToSeeColosso)
  ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Wahhh! I want to see Colosso, but no one will
  /// take me!" (MsgTolbiInteriorsWahhhIWantToSeeColosso)
  ACTOR_STUCCO_ROOMS_B_CRYING_BOY,
  /// A strongman (SPRITE_STRONGMAN). "I wonder if they really can't find
  /// Babi..."
  /// ★ Named from the game: "It's strange that Babi's soldiers are hanging out
  /// all around town." (MsgTolbiInteriorsItsStrangeThatBabisSoldiersAre)
  ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER,
  /// A strongman (SPRITE_STRONGMAN). "Babi wasn't at Colosso according to all
  /// the spectators..."
  /// ★ Named from the game: "That sounds ludicrous!"
  /// (MsgTolbiInteriorsHuhThenTheyFoundHimIn)
  ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC,
  /// A stout man (SPRITE_STOUT_MAN).
  /// ★ Named from the game: "We're going to make some money at Colosso."
  /// (MsgTolbiInteriorsWereGoingToMakeSomeMoney)
  ACTOR_STUCCO_ROOMS_B_MERCHANT,
  /// A stout man (SPRITE_STOUT_MAN), after Colosso.
  /// ★ Named from the game: "selling my wares"
  /// (MsgTolbiInteriorsThereWontBeManyCustomersUntil)
  ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT,
  /// A bald man in red (SPRITE_BALD_MAN_IN_RED) behind the weapon shop's
  /// counter (StuccoRoomsB_TalkToWeaponMerchant).
  /// ★ Named from the game: "Take a look at our fine weapons."
  /// (MsgWeaponShopWelcome)
  ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT,
  /// A young man in green (SPRITE_YOUNG_MAN_IN_GREEN) behind the armor shop's
  /// counter (StuccoRoomsB_TalkToArmorMerchant).
  /// ★ Named from the game: "Ask me anything about armor."
  /// (MsgArmorShopWelcome)
  ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT,
  /// A young woman with a ponytail (SPRITE_WOMAN_WITH_PONYTAIL) behind the item
  /// shop's counter (StuccoRoomsB_TalkToItemMerchant).
  /// ★ Named from the game: "Welcome! We have just the gear you need."
  /// (MsgItemShopWelcome)
  ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT,
  /// The inn's master, a man with dark hair (SPRITE_MAN_WITH_DARK_HAIR). His
  /// own thought: "I'm the one who really wants to skip work to watch the
  /// finals!"
  /// ★ Named from the game: "If the master slips out, the lady will be upset."
  /// (MsgTolbiInteriorsIfTheMasterSlipsOutThe)
  ACTOR_STUCCO_ROOMS_B_INN_MASTER,
  /// The inn's lady, a woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON).
  /// "This year I'm not letting anyone skip work for Colosso!"
  /// ★ Named from the game: "If the master slips out, the lady will be upset."
  /// (MsgTolbiInteriorsIfTheMasterSlipsOutThe)
  ACTOR_STUCCO_ROOMS_B_INN_LADY,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "We're so busy running the inn"
  /// (MsgTolbiInteriorsWereSoBusyRunningTheInn)
  ACTOR_STUCCO_ROOMS_B_INN_WORKER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who, after Colosso,
  /// hides the Lucky Medal she found in a barrel
  /// (FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL).
  /// ★ Named from the game: "Hey, what's this?" (MsgTolbiHeyWhatsThis)
  ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
  /// The inn's cook (SPRITE_CHEF).
  /// ★ Named from the game: "The innkeeper told me to cook a gourmet meal"
  /// (MsgTolbiInteriorsTheInnkeeperToldMeToCook)
  ACTOR_STUCCO_ROOMS_B_CHEF,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) waiting at the
  /// tables.
  /// ★ Named from the game: "I'm not very interested in Colosso. It's so
  /// barbaric." (MsgTolbiInteriorsImNotVeryInterestedInColosso)
  ACTOR_STUCCO_ROOMS_B_WAITRESS,
  /// A young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR).
  /// ★ Named from the game: "we can't even enter Colosso."
  /// (MsgTolbiInteriorsOhNoWeBarelyMadeIt)
  ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT,
  /// A young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR).
  /// ★ Named from the game: "we could have entered Colosso."
  /// (MsgTolbiInteriorsIfWedGottenHereJustA)
  ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT,
  /// Tolbi Sanctum's healer, a man in a tall hat (SPRITE_MAN_IN_TALL_HAT),
  /// who opens the sanctum across his counter (StuccoRoomsB_TalkToHealer).
  /// ★ Named from the game: "Welcome, weary wanderers. What aid do you seek?"
  /// (MsgSanctumWelcome)
  ACTOR_STUCCO_ROOMS_B_HEALER,
  /// After Colosso, passengers stranded at an inn because no ship sails: the
  /// angry one, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), his wife,
  /// an old woman (SPRITE_OLD_WOMAN), and a third, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "Making passengers row their own cruise ship is
  /// unprecedented!" (MsgTolbiInteriorsYoureSayingIfWeRowWe)
  ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER,
  /// ★ Named from the game: "The ships cannot go out for any reason."
  /// (MsgTolbiHeWontSailShipEven)
  ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE,
  /// ★ Named from the game: "What kind of person would refuse such an
  /// offer?" (MsgTolbiHeWontSailShipEven)
  ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), after Colosso.
  /// ★ Named from the game: "Tolbi gets so boring"
  /// (MsgTolbiInteriorsTolbiGetsSoBoringOnceThe)
  ACTOR_STUCCO_ROOMS_B_BORED_BOY,
  /// After Colosso, a young man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who rode the ship from Kalay with the
  /// party.
  /// ★ Named from the game: "We rode the same ship from Kalay!"
  /// (MsgTolbiWarriorRemember)
  ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), after Colosso.
  /// ★ Named from the game: "I got to see Colosso"
  /// (MsgTolbiInteriorsIGotToSeeColossoSo)
  ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), after Colosso,
  /// in ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST's row.
  /// ★ Named from the game: "I rented my house to a rich family and tried to
  /// head east... but Silk Road is blocked"
  /// (MsgTolbiInteriorsIRentedMyHouseToA)
  ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER = ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), after Colosso, in
  /// ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST's row. "The tourists are all in a
  /// fix, because the ships aren't sailing."
  /// ★ Named from the game: "I heard of a cave that leads to the Karagol's far
  /// shore" (MsgTolbiInteriorsIHeardOfACaveThat)
  ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST = ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST
};

/// The metatile behaviors the rooms' coord and bg events answer: the doors,
/// counted from MB_STUCCO_ROOMS_B_FIRST_DOOR, and the things the party can
/// search.
enum StuccoRoomsBMetatileBehavior {
  MB_STUCCO_ROOMS_B_FIRST_DOOR = 14,
  /// The inn's sign.
  /// ★ Named from the game: "checked the sign."
  /// (MsgTolbiInteriorsIsaacCheckedTheSignInnFull)
  MB_STUCCO_ROOMS_B_INN_SIGN = 40,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_STUCCO_ROOMS_B_VIAL_BARREL = 100,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_STUCCO_ROOMS_B_MEDAL_BARREL = 101,
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  MB_STUCCO_ROOMS_B_HIDDEN_MEDAL_BARREL = 102, ///< where the woman hid her find
  /// ★ Named from the game: "Tolbi pizza."
  /// (MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)
  MB_STUCCO_ROOMS_B_PIZZA_OVEN = 200,
  /// ★ Named from the game: "boiling the pasta"
  /// (MsgTolbiInteriorsHeIsBoilingThePastaIt)
  MB_STUCCO_ROOMS_B_PASTA_OVEN = 201,
  /// ★ Named from the game: "It's Tolbi seafood pasta!"
  /// (MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)
  MB_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN = 202,
};

/// A facing rounded to the nearest of east, south, west and north.
#define FACING_NEAREST_CARDINAL(facing) ((u16)(((facing) + 0x2000) & ~0x3fff))

#endif

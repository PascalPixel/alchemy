#ifndef TOLBI_INTERIORS_H
#define TOLBI_INTERIORS_H

#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "common/colosso_story_flags.h"
#include "flags.h"

/// The placed object events, numbering the rows of the object event template
/// table from LOCALID_FIRST_PLACED. Both tables hold the same people in the
/// same order up to the healer, except that after Colosso
/// (gTolbiInteriorsObjectEventTemplatesAfterColosso) two others stand in the
/// rows of LOCALID_TOLBI_INTERIORS_SLEEPLESS_GUEST and
/// LOCALID_TOLBI_INTERIORS_CROWD_SICK_GUEST, and six more rows follow the
/// healer.
enum TolbiInteriorsLocalId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who runs a
  /// makeshift inn during the festival.
  /// ★ Named from the game: "I just line up some beds, and presto! I'm an
  /// innkeeper!" (MsgTolbiInteriorsTolbiIsPackedWithTouristsDuring)
  LOCALID_TOLBI_INTERIORS_MAKESHIFT_INNKEEPER = LOCALID_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who offers a stay.
  /// ★ Named from the game: "Do you want to stay with us?" (MsgTolbiWantStay)
  LOCALID_TOLBI_INTERIORS_HOST,
  /// A strongman (SPRITE_STRONGMAN), during the festival.
  /// ★ Named from the game: "I couldn't sleep at all last night."
  /// (MsgTolbiInteriorsICouldntSleepAtAllLast)
  LOCALID_TOLBI_INTERIORS_SLEEPLESS_GUEST,
  /// A stout man (SPRITE_STOUT_MAN), during the festival.
  /// ★ Named from the game: "The crowds at the colosseum made me feel sick"
  /// (MsgTolbiInteriorsTheCrowdsAtTheColosseumMade)
  LOCALID_TOLBI_INTERIORS_CROWD_SICK_GUEST,
  /// An old woman (SPRITE_OLD_WOMAN); on finals day, "I'm stuck
  /// housesitting!"
  /// ★ Named from the game: "We paid good money to rent this house"
  /// (MsgTolbiInteriorsWePaidGoodMoneyToRent)
  LOCALID_TOLBI_INTERIORS_HOUSE_TENANT,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  /// ★ Named from the game: "so I'm just going to watch the finals."
  /// (MsgTolbiInteriorsIReallyWantToSeeColosso)
  LOCALID_TOLBI_INTERIORS_FINALS_WATCHER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
  /// ★ Named from the game: "Wahhh! I want to see Colosso, but no one will
  /// take me!" (MsgTolbiInteriorsWahhhIWantToSeeColosso)
  LOCALID_TOLBI_INTERIORS_CRYING_BOY,
  /// A strongman (SPRITE_STRONGMAN). "I wonder if they really can't find
  /// Babi..."
  /// ★ Named from the game: "It's strange that Babi's soldiers are hanging out
  /// all around town." (MsgTolbiInteriorsItsStrangeThatBabisSoldiersAre)
  LOCALID_TOLBI_INTERIORS_SOLDIER_WATCHER,
  /// A strongman (SPRITE_STRONGMAN). "Babi wasn't at Colosso according to all
  /// the spectators..."
  /// ★ Named from the game: "That sounds ludicrous!"
  /// (MsgTolbiInteriorsHuhThenTheyFoundHimIn)
  LOCALID_TOLBI_INTERIORS_BABI_SKEPTIC,
  /// A stout man (SPRITE_STOUT_MAN).
  /// ★ Named from the game: "We're going to make some money at Colosso."
  /// (MsgTolbiInteriorsWereGoingToMakeSomeMoney)
  LOCALID_TOLBI_INTERIORS_MERCHANT,
  /// A stout man (SPRITE_STOUT_MAN), after Colosso.
  /// ★ Named from the game: "selling my wares"
  /// (MsgTolbiInteriorsThereWontBeManyCustomersUntil)
  LOCALID_TOLBI_INTERIORS_TRAVELING_MERCHANT,
  LOCALID_TOLBI_INTERIORS_WEAPON_MERCHANT, ///< a bald man in red
                                           ///< (SPRITE_BALD_MAN_IN_RED)
  /// A young man in green (SPRITE_YOUNG_MAN_IN_GREEN).
  LOCALID_TOLBI_INTERIORS_ARMOR_MERCHANT,
  /// A young woman with a ponytail (SPRITE_WOMAN_WITH_PONYTAIL).
  LOCALID_TOLBI_INTERIORS_ITEM_MERCHANT,
  /// The inn's master, a man with dark hair (SPRITE_MAN_WITH_DARK_HAIR). His
  /// own thought: "I'm the one who really wants to skip work to watch the
  /// finals!"
  /// ★ Named from the game: "If the master slips out, the lady will be upset."
  /// (MsgTolbiInteriorsIfTheMasterSlipsOutThe)
  LOCALID_TOLBI_INTERIORS_INN_MASTER,
  /// The inn's lady, a woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON).
  /// "This year I'm not letting anyone skip work for Colosso!"
  /// ★ Named from the game: "If the master slips out, the lady will be upset."
  /// (MsgTolbiInteriorsIfTheMasterSlipsOutThe)
  LOCALID_TOLBI_INTERIORS_INN_LADY,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  /// ★ Named from the game: "We're so busy running the inn"
  /// (MsgTolbiInteriorsWereSoBusyRunningTheInn)
  LOCALID_TOLBI_INTERIORS_INN_WORKER,
  LOCALID_TOLBI_INTERIORS_MEDAL_WOMAN, ///< SPRITE_WOMAN_WITH_FAIR_HAIR
  LOCALID_TOLBI_INTERIORS_CHEF,        ///< SPRITE_CHEF
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) waiting at the
  /// tables.
  LOCALID_TOLBI_INTERIORS_WAITRESS,
  /// A young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR).
  /// ★ Named from the game: "we can't even enter Colosso."
  /// (MsgTolbiInteriorsOhNoWeBarelyMadeIt)
  LOCALID_TOLBI_INTERIORS_LATE_ENTRANT,
  /// A young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR).
  /// ★ Named from the game: "we could have entered Colosso."
  /// (MsgTolbiInteriorsIfWedGottenHereJustA)
  LOCALID_TOLBI_INTERIORS_SECOND_LATE_ENTRANT,
  LOCALID_TOLBI_INTERIORS_HEALER, ///< a man in a tall hat
                                  ///< (SPRITE_MAN_IN_TALL_HAT)
  /// After Colosso, passengers stranded at an inn because no ship sails: the
  /// angry one, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), his wife,
  /// an old woman (SPRITE_OLD_WOMAN), and a third, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS).
  /// ★ Named from the game: "Making passengers row their own cruise ship is
  /// unprecedented!" (MsgTolbiInteriorsYoureSayingIfWeRowWe)
  LOCALID_TOLBI_INTERIORS_ANGRY_PASSENGER,
  LOCALID_TOLBI_INTERIORS_PASSENGER_WIFE,
  LOCALID_TOLBI_INTERIORS_THIRD_PASSENGER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), after Colosso.
  /// ★ Named from the game: "Tolbi gets so boring"
  /// (MsgTolbiInteriorsTolbiGetsSoBoringOnceThe)
  LOCALID_TOLBI_INTERIORS_BORED_BOY,
  /// After Colosso, a young man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who rode the ship from Kalay with the
  /// party.
  /// ★ Named from the game: "We rode the same ship from Kalay!"
  /// (MsgTolbiWarriorRemember)
  LOCALID_TOLBI_INTERIORS_FELLOW_PASSENGER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), after Colosso.
  /// ★ Named from the game: "I got to see Colosso"
  /// (MsgTolbiInteriorsIGotToSeeColossoSo)
  LOCALID_TOLBI_INTERIORS_COLOSSO_FAN,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), after Colosso,
  /// in LOCALID_TOLBI_INTERIORS_SLEEPLESS_GUEST's row.
  /// ★ Named from the game: "I rented my house to a rich family and tried to
  /// head east... but Silk Road is blocked"
  /// (MsgTolbiInteriorsIRentedMyHouseToA)
  LOCALID_TOLBI_INTERIORS_STRANDED_TRAVELER =
      LOCALID_TOLBI_INTERIORS_SLEEPLESS_GUEST,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), after Colosso, in
  /// LOCALID_TOLBI_INTERIORS_CROWD_SICK_GUEST's row. "The tourists are all in a
  /// fix, because the ships aren't sailing."
  /// ★ Named from the game: "I heard of a cave that leads to the Karagol's far
  /// shore" (MsgTolbiInteriorsIHeardOfACaveThat)
  LOCALID_TOLBI_INTERIORS_CAVE_RUMOR_GUEST =
      LOCALID_TOLBI_INTERIORS_CROWD_SICK_GUEST
};

/// The things the party can search, which its bg events answer.
enum TolbiInteriorsMarker {
  /// The inn's sign.
  /// ★ Named from the game: "checked the sign."
  /// (MsgTolbiInteriorsIsaacCheckedTheSignInnFull)
  TOLBI_INTERIORS_INN_SIGN = 40,
  TOLBI_INTERIORS_VIAL_BARREL = 100,
  TOLBI_INTERIORS_MEDAL_BARREL = 101,
  TOLBI_INTERIORS_HIDDEN_MEDAL_BARREL = 102, ///< where the woman hid her find
  /// ★ Named from the game: "Tolbi pizza."
  /// (MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)
  TOLBI_INTERIORS_PIZZA_OVEN = 200,
  /// ★ Named from the game: "boiling the pasta"
  /// (MsgTolbiInteriorsHeIsBoilingThePastaIt)
  TOLBI_INTERIORS_PASTA_OVEN = 201,
  /// ★ Named from the game: "It's Tolbi seafood pasta!"
  /// (MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)
  TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN = 202,
};

/// A facing rounded to the nearest of east, south, west and north.
#define FACING_NEAREST_CARDINAL(facing) ((u16)(((facing) + 0x2000) & ~0x3fff))

#endif

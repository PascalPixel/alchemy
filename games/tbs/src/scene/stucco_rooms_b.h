#ifndef ALCHEMY_STUCCO_ROOMS_B_H
#define ALCHEMY_STUCCO_ROOMS_B_H

#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "shared/colosso_story_flags.h"
#include "flags.h"

/// The placed actors, numbering the rows of the actor placement
/// table from ACTOR_FIRST_PLACED. Both tables hold the same people in the
/// same order up to the healer, except that after Colosso
/// (gStuccoRoomsBActorPlacementsAfterColosso) two others stand in the
/// rows of ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST and
/// ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, and six more rows follow the
/// healer.
enum StuccoRoomsBActorId {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who runs a
  /// makeshift inn during the festival.
  ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who offers a stay.
  ACTOR_STUCCO_ROOMS_B_HOST,
  /// A strongman (SPRITE_STRONGMAN), during the festival.
  ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
  /// A stout man (SPRITE_STOUT_MAN), during the festival.
  ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST,
  /// An old woman (SPRITE_OLD_WOMAN); on finals day, "I'm stuck
  /// housesitting!"
  ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR).
  ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR).
  ACTOR_STUCCO_ROOMS_B_CRYING_BOY,
  /// A strongman (SPRITE_STRONGMAN). "I wonder if they really can't find
  /// Babi..."
  ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER,
  /// A strongman (SPRITE_STRONGMAN). "Babi wasn't at Colosso according to all
  /// the spectators..."
  ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC,
  /// A stout man (SPRITE_STOUT_MAN).
  ACTOR_STUCCO_ROOMS_B_MERCHANT,
  /// A stout man (SPRITE_STOUT_MAN), after Colosso.
  ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT,
  /// A bald man in red (SPRITE_BALD_MAN_IN_RED) behind the weapon shop's
  /// counter (StuccoRoomsB_TalkToWeaponMerchant).
  ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT,
  /// A young man in green (SPRITE_YOUNG_MAN_IN_GREEN) behind the armor shop's
  /// counter (StuccoRoomsB_TalkToArmorMerchant).
  ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT,
  /// A young woman with a ponytail (SPRITE_WOMAN_WITH_PONYTAIL) behind the item
  /// shop's counter (StuccoRoomsB_TalkToItemMerchant).
  ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT,
  /// The inn's master, a man with dark hair (SPRITE_MAN_WITH_DARK_HAIR). His
  /// own thought: "I'm the one who really wants to skip work to watch the
  /// finals!"
  ACTOR_STUCCO_ROOMS_B_INN_MASTER,
  /// The inn's lady, a woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON).
  /// "This year I'm not letting anyone skip work for Colosso!"
  ACTOR_STUCCO_ROOMS_B_INN_LADY,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR).
  ACTOR_STUCCO_ROOMS_B_INN_WORKER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who, after Colosso,
  /// hides the Lucky Medal she found in a barrel
  /// (FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL).
  ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
  /// The inn's cook (SPRITE_CHEF).
  ACTOR_STUCCO_ROOMS_B_CHEF,
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) waiting at the
  /// tables.
  ACTOR_STUCCO_ROOMS_B_WAITRESS,
  /// A young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR).
  ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT,
  /// A young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR).
  ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT,
  /// Tolbi Sanctum's healer, a man in a tall hat (SPRITE_MAN_IN_TALL_HAT),
  /// who opens the sanctum across his counter (StuccoRoomsB_TalkToHealer).
  ACTOR_STUCCO_ROOMS_B_HEALER,
  /// After Colosso, passengers stranded at an inn because no ship sails: the
  /// angry one, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), his wife,
  /// an old woman (SPRITE_OLD_WOMAN), and a third, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS).
  ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER,
  ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE,
  ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), after Colosso.
  ACTOR_STUCCO_ROOMS_B_BORED_BOY,
  /// After Colosso, a young man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who rode the ship from Kalay with the
  /// party.
  ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), after Colosso.
  ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), after Colosso,
  /// in ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST's row.
  ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER = ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), after Colosso, in
  /// ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST's row. "The tourists are all in a
  /// fix, because the ships aren't sailing."
  ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST = ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST
};

/// The cell behaviors the rooms' coord and examine triggers answer: the
/// doors, counted from CELL_BEHAVIOR_STUCCO_ROOMS_B_FIRST_DOOR, and the things
/// the party can search.
enum StuccoRoomsBCellBehavior {
  CELL_BEHAVIOR_STUCCO_ROOMS_B_FIRST_DOOR = 14,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN = 40,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL = 100,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL = 101,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_HIDDEN_MEDAL_BARREL =
      102, ///< where the woman hid her find
  CELL_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN = 200,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN = 201,
  CELL_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN = 202,
};

/// A facing rounded to the nearest of east, south, west and north.
#define FACING_NEAREST_CARDINAL(facing)                                        \
  ((u16)(((facing) + TRIG_TURN / 8) & ~0x3fff))

#endif

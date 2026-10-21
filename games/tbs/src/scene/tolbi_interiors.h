#ifndef TOLBI_INTERIORS_H
#define TOLBI_INTERIORS_H

#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "common/colosso_story_flags.h"
#include "flags.h"

/// The placed actors, numbering the rows of the placement table from
/// ACTOR_FIRST_PLACED. Both tables hold the same people in the same order up
/// to the healer, except that after Colosso
/// (gTolbiInteriorsPlacementsAfterColosso) two others stand in the rows of
/// TOLBI_INTERIORS_SLEEPLESS_GUEST and TOLBI_INTERIORS_CROWD_SICK_GUEST, and
/// six more rows follow the healer.
enum TolbiInteriorsActor {
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who runs a
  /// makeshift inn during the festival. "Me, I just line up some beds,
  /// and presto! I'm an innkeeper!"
  TOLBI_INTERIORS_MAKESHIFT_INNKEEPER = ACTOR_FIRST_PLACED,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who offers a stay.
  /// "Do you want to stay with us?"
  TOLBI_INTERIORS_HOST,
  /// A strongman (SPRITE_STRONGMAN), during the festival. "I couldn't sleep
  /// at all last night. The guy next
  /// to me snored the whole time!"
  TOLBI_INTERIORS_SLEEPLESS_GUEST,
  /// A stout man (SPRITE_STOUT_MAN), during the festival. "The crowds at the
  /// colosseum made me feel sick, so
  /// I came back!"
  TOLBI_INTERIORS_CROWD_SICK_GUEST,
  /// An old woman (SPRITE_OLD_WOMAN). "We paid good money to rent this house
  /// during the festival, so get
  /// out!"; on finals day, "I'm stuck housesitting!"
  TOLBI_INTERIORS_HOUSE_TENANT,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR). "I really want
  /// to see Colosso, too! But the colosseum gets too crowded,
  /// so I'm just going to watch the finals."
  TOLBI_INTERIORS_FINALS_WATCHER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR). "Wahhh! I want to
  /// see Colosso, but no one will take me!"
  TOLBI_INTERIORS_CRYING_BOY,
  /// A strongman (SPRITE_STRONGMAN). "I wonder if they really can't find
  /// Babi..."; Mind Read: "It's strange
  /// that Babi's soldiers are hanging out all around town."
  TOLBI_INTERIORS_SOLDIER_WATCHER,
  /// A strongman (SPRITE_STRONGMAN). "Babi wasn't at Colosso according to all
  /// the spectators..."; on finals
  /// day, "Then they found him in time for the finals? That sounds
  /// ludicrous!"
  TOLBI_INTERIORS_BABI_SKEPTIC,
  /// A stout man (SPRITE_STOUT_MAN). "We're going to make some money at
  /// Colosso. But of course, Babi always
  /// makes the most."
  TOLBI_INTERIORS_MERCHANT,
  /// A stout man (SPRITE_STOUT_MAN); after Colosso, "I'll be traveling around
  /// selling my wares until then."
  TOLBI_INTERIORS_TRAVELING_MERCHANT,
  TOLBI_INTERIORS_WEAPON_MERCHANT, ///< a bald man in red
                                   ///< (SPRITE_BALD_MAN_IN_RED)
  /// A young man in green (SPRITE_YOUNG_MAN_IN_GREEN).
  TOLBI_INTERIORS_ARMOR_MERCHANT,
  /// A young woman with a ponytail (SPRITE_WOMAN_WITH_PONYTAIL).
  TOLBI_INTERIORS_ITEM_MERCHANT,
  /// The inn's master, a man with dark hair (SPRITE_MAN_WITH_DARK_HAIR), whose
  /// worker TOLBI_INTERIORS_INN_WORKER thinks "If the
  /// master slips out, the lady will be upset." His own thought: "I'm the one
  /// who really wants to skip work to watch the finals!"
  TOLBI_INTERIORS_INN_MASTER,
  /// The inn's lady, a woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON).
  /// "This year I'm not letting anyone skip work for
  /// Colosso!"
  TOLBI_INTERIORS_INN_LADY,
  /// A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). "We're so
  /// busy
  /// running the inn we can't go to Colosso. It's so boring."
  TOLBI_INTERIORS_INN_WORKER,
  TOLBI_INTERIORS_MEDAL_WOMAN, ///< SPRITE_WOMAN_WITH_FAIR_HAIR
  TOLBI_INTERIORS_CHEF,        ///< SPRITE_CHEF
  /// A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) waiting at the
  /// tables.
  TOLBI_INTERIORS_WAITRESS,
  /// A young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR). "Oh, no! We
  /// barely made it to Tolbi, and now we can't even enter
  /// Colosso."
  TOLBI_INTERIORS_LATE_ENTRANT,
  /// A young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR). "If we'd
  /// gotten here just a little earlier, we could have entered
  /// Colosso."
  TOLBI_INTERIORS_SECOND_LATE_ENTRANT,
  TOLBI_INTERIORS_HEALER, ///< a man in a tall hat (SPRITE_MAN_IN_TALL_HAT)
  /// After Colosso, passengers stranded at an inn because no ship sails: the
  /// angry one, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), his wife,
  /// an
  /// old woman (SPRITE_OLD_WOMAN), and a third, a woman in a green dress
  /// (SPRITE_WOMAN_IN_GREEN_DRESS). "Making passengers row their own cruise
  /// ship is unprecedented!"
  TOLBI_INTERIORS_ANGRY_PASSENGER,
  TOLBI_INTERIORS_PASSENGER_WIFE,
  TOLBI_INTERIORS_THIRD_PASSENGER,
  /// A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), after Colosso.
  /// "Tolbi gets so boring once the festival ends! I wanna go
  /// home!"
  TOLBI_INTERIORS_BORED_BOY,
  /// After Colosso, a young man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR)
  /// who rode the ship from Kalay with the party. "It's
  /// me! We rode the same ship from Kalay!"
  TOLBI_INTERIORS_FELLOW_PASSENGER,
  /// A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), after Colosso. "I
  /// got to see Colosso, so I don't care if I can't get
  /// across the Karagol."
  TOLBI_INTERIORS_COLOSSO_FAN,
  /// A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), after Colosso,
  /// in TOLBI_INTERIORS_SLEEPLESS_GUEST's row. "I rented my
  /// house to a rich family and tried to head east... but Silk Road is
  /// blocked"
  TOLBI_INTERIORS_STRANDED_TRAVELER = TOLBI_INTERIORS_SLEEPLESS_GUEST,
  /// A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), after Colosso, in
  /// TOLBI_INTERIORS_CROWD_SICK_GUEST's row. "The tourists
  /// are all in a fix, because the ships aren't sailing."; Mind Read: "I
  /// heard of a cave that leads to the Karagol's far shore somewhere."
  TOLBI_INTERIORS_CAVE_RUMOR_GUEST = TOLBI_INTERIORS_CROWD_SICK_GUEST
};

/// Map objects the party can search.
enum TolbiInteriorsMapObject {
  TOLBI_INTERIORS_INN_SIGN = 40, ///< the inn's sign. "Inn Full / No Vacancies"
  TOLBI_INTERIORS_VIAL_BARREL = 100,
  TOLBI_INTERIORS_MEDAL_BARREL = 101,
  TOLBI_INTERIORS_HIDDEN_MEDAL_BARREL = 102, ///< where the woman hid her find
  TOLBI_INTERIORS_PIZZA_OVEN = 200,
  TOLBI_INTERIORS_PASTA_OVEN = 201,
  TOLBI_INTERIORS_SEAFOOD_PASTA_OVEN = 202,
};

/// A facing rounded to the nearest of east, south, west and north.
#define FACING_NEAREST_CARDINAL(facing) ((u16)(((facing) + 0x2000) & ~0x3fff))

#endif

/* The German counted nouns, singular and plural, by kind. */

#include "types.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_GERMAN)
extern const Sint8 Article_Empty[];
extern const Uint8 LocalizedWord_Battles[];
extern const Uint8 LocalizedWord_Battle[];
extern const Uint8 LocalizedWord_Tickets[];
extern const Uint8 LocalizedWord_Ticket[];
extern const Uint8 LocalizedWord_Rounds[];
extern const Uint8 LocalizedWord_Round[];
extern const Uint8 LocalizedWord_Coins[];
extern const Uint8 LocalizedWord_Coin[];
extern const Uint8 LocalizedWord_Points[];
extern const Uint8 LocalizedWord_Point[];

/* clang-format off */
/* {singular, plural}: window.c reads [(kind & 7) * 2 + plural]. */
const Uint8 *const LocalizedWord_CountedNouns[16] = {
	LocalizedWord_Point, LocalizedWord_Points, /* Punkt, Punkte */
	LocalizedWord_Coin, LocalizedWord_Coins, /* Münze, Münzen */
	LocalizedWord_Round, LocalizedWord_Rounds, /* Runde, Runden */
	LocalizedWord_Ticket, LocalizedWord_Tickets, /* Ticket, Tickets */
	LocalizedWord_Battle, LocalizedWord_Battles, /* Kampf, Käpfe (sic) */
	(const Uint8 *)Article_Empty, (const Uint8 *)Article_Empty,
	(const Uint8 *)Article_Empty, (const Uint8 *)Article_Empty,
	(const Uint8 *)Article_Empty, 0,
};
/* clang-format on */
#endif

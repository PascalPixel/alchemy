/* The German counted nouns, singular and plural, by kind. */

#include "types.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_GERMAN)
extern const Sint8 ArticleEmpty[];
extern const Uint8 LocalizedWordBattles[];
extern const Uint8 LocalizedWordBattle[];
extern const Uint8 LocalizedWordTickets[];
extern const Uint8 LocalizedWordTicket[];
extern const Uint8 LocalizedWordRounds[];
extern const Uint8 LocalizedWordRound[];
extern const Uint8 LocalizedWordCoins[];
extern const Uint8 LocalizedWordCoin[];
extern const Uint8 LocalizedWordPoints[];
extern const Uint8 LocalizedWordPoint[];

/* clang-format off */
/* {singular, plural}: window.c reads [(kind & 7) * 2 + plural]. */
const Uint8 *const LocalizedWordCountedNouns[16] = {
	LocalizedWordPoint, LocalizedWordPoints, /* Punkt, Punkte */
	LocalizedWordCoin, LocalizedWordCoins, /* Münze, Münzen */
	LocalizedWordRound, LocalizedWordRounds, /* Runde, Runden */
	LocalizedWordTicket, LocalizedWordTickets, /* Ticket, Tickets */
	LocalizedWordBattle, LocalizedWordBattles, /* Kampf, Käpfe (sic) */
	(const Uint8 *)ArticleEmpty, (const Uint8 *)ArticleEmpty,
	(const Uint8 *)ArticleEmpty, (const Uint8 *)ArticleEmpty,
	(const Uint8 *)ArticleEmpty, 0,
};
/* clang-format on */
#endif

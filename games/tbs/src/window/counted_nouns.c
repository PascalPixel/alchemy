/* The German counted nouns, singular and plural, by kind. */

#include "window/articles.h"

#if LANGUAGE == LANGUAGE_GERMAN
/* {singular, plural}: window.c reads [(kind & 7) * 2 + plural]. */
/* clang-format off */
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

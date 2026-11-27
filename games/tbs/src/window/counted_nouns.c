/* @file */
/* The German counted nouns, singular and plural, by kind. */

#include "window/articles.h"

#if LANGUAGE == LANGUAGE_GERMAN
/* {singular, plural}: window.c reads [(kind & 7) * 2 + plural]. */
/* clang-format off */
const u8 *const LocalizedWord_CountedNouns[16] = {
    LocalizedWord_Point, LocalizedWord_Points, /* Punkt, Punkte */
    LocalizedWord_Coin, LocalizedWord_Coins, /* Münze, Münzen */
    LocalizedWord_Round, LocalizedWord_Rounds, /* Runde, Runden */
    LocalizedWord_Ticket, LocalizedWord_Tickets, /* Ticket, Tickets */
    LocalizedWord_Battle, LocalizedWord_Battles, /* Kampf, Käpfe (sic) */
    (const u8 *)Article_Empty, (const u8 *)Article_Empty,
    (const u8 *)Article_Empty, (const u8 *)Article_Empty,
    (const u8 *)Article_Empty, 0,
};
/* clang-format on */
#endif

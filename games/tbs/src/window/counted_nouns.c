/// @file
/// The German counted nouns, singular and plural, by kind.

#include "module/window/articles.h"
#include "types.h"

#if (LANGUAGE == LANGUAGE_GERMAN)
/// {singular, plural}: window.c reads [(kind & 7) * 2 + plural].
// clang-format off
const u8 *const gLocalizedWord_CountedNouns[16] = {
    gLocalizedWord_Point, gLocalizedWord_Points, // Punkt, Punkte
    gLocalizedWord_Coin, gLocalizedWord_Coins, // Münze, Münzen
    gLocalizedWord_Round, gLocalizedWord_Rounds, // Runde, Runden
    gLocalizedWord_Ticket, gLocalizedWord_Tickets, // Ticket, Tickets
    gLocalizedWord_Battle, gLocalizedWord_Battles, // Kampf, Käpfe (sic)
    (const u8 *)gArticle_Empty, (const u8 *)gArticle_Empty,
    (const u8 *)gArticle_Empty, (const u8 *)gArticle_Empty,
    (const u8 *)gArticle_Empty, 0,
};
// clang-format on
#endif

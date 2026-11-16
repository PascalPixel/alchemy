/// @file
/// The German counted nouns, singular and plural, by kind.

#include "types.h"
#include "edition.h"

#if (LANGUAGE == LANGUAGE_GERMAN)
extern const s8 gArticle_Empty[];
extern const u8 gLocalizedWord_Battles[];
extern const u8 gLocalizedWord_Battle[];
extern const u8 gLocalizedWord_Tickets[];
extern const u8 gLocalizedWord_Ticket[];
extern const u8 gLocalizedWord_Rounds[];
extern const u8 gLocalizedWord_Round[];
extern const u8 gLocalizedWord_Coins[];
extern const u8 gLocalizedWord_Coin[];
extern const u8 gLocalizedWord_Points[];
extern const u8 gLocalizedWord_Point[];

// clang-format off
/// {singular, plural}: window.c reads [(kind & 7) * 2 + plural].
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

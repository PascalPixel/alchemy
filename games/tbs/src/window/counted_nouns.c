#include "module/window/articles.h"
#include "types.h"

//--------------------------------------------------------------------------
// Counted nouns
// The German counted nouns, singular and plural, by kind.
//--------------------------------------------------------------------------

#if (LANGUAGE == LANGUAGE_GERMAN)
/// {singular, plural}: window.c reads [(kind & 7) * 2 + plural]. The reader
/// is the message decoder's counted-noun code (MESSAGE_LOCALIZED_WORD,
/// window/window/decode.c), plural set unless the last number shown was -1, 0
/// or 1.
/// Words 5 to 7 are empty, and word 7's plural is a null pointer.
// clang-format off
const u8 *const gLocalizedWord_CountedNouns[16] = {
    gLocalizedWord_Point, gLocalizedWord_Points, // Punkt, Punkte
    gLocalizedWord_Coin, gLocalizedWord_Coins, // Münze, Münzen
    gLocalizedWord_Round, gLocalizedWord_Rounds, // Runde, Runden
    gLocalizedWord_Ticket, gLocalizedWord_Tickets, // Ticket, Tickets
    gLocalizedWord_Battle, gLocalizedWord_Battles, // Kampf, Käpfe (ROM spelling retained: the m is missing)
    (const u8 *)gArticle_Empty, (const u8 *)gArticle_Empty,
    (const u8 *)gArticle_Empty, (const u8 *)gArticle_Empty,
    (const u8 *)gArticle_Empty, 0,
};
// clang-format on
#endif

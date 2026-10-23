#include "unit/window/articles.h"
#include "types.h"

//--------------------------------------------------------------------------
// Number words
// The German counted nouns, singular and plural, by kind.
//--------------------------------------------------------------------------

#if (LANGUAGE == LANGUAGE_GERMAN)
/// {singular, plural}: window.c reads [(kind & 7) * 2 + plural]. The reader
/// is the message decoder's counted-noun code (MESSAGE_LOCALIZED_WORD,
/// window/window/decode.c), plural set unless the last number shown was -1, 0
/// or 1.
/// Words 5 to 7 are empty, and word 7's plural is a null pointer.
// clang-format off
const u8 *const gLocalizedWord_Numbers[16] = {
    gLocalizedWord_Number9, gLocalizedWord_Number8, // Punkt, Punkte
    gLocalizedWord_Number7, gLocalizedWord_Number6, // Münze, Münzen
    gLocalizedWord_Number5, gLocalizedWord_Number4, // Runde, Runden
    gLocalizedWord_Number3, gLocalizedWord_Number2, // Ticket, Tickets
    gLocalizedWord_Number1, gLocalizedWord_Number0, // Kampf, Käpfe (ROM spelling retained: the m is missing)
    (const u8 *)gArticle_Empty, (const u8 *)gArticle_Empty,
    (const u8 *)gArticle_Empty, (const u8 *)gArticle_Empty,
    (const u8 *)gArticle_Empty, 0,
};
// clang-format on
#endif

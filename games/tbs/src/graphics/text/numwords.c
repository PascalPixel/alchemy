#include "unit/window/articles.h"
#include "types.h"

//--------------------------------------------------------------------------
// Number words
// The German counted nouns, singular and plural, by kind.
//--------------------------------------------------------------------------

#if (LANGUAGE == LANGUAGE_GERMAN)
/// {singular, plural}: window.c reads [(kind & 7) * 2 + plural]. The reader
/// is the message decoder's counted-noun code (MSG_LOCALIZED_WORD,
/// window/window/decode.c), plural set unless the last number shown was -1, 0
/// or 1.
/// Words 5 to 7 are empty, and word 7's plural is a null pointer.
// clang-format off
const u8 *const gUiText_NumberWords[16] = {
    gUiText_NumberWord9, gUiText_NumberWord8, // Punkt, Punkte
    gUiText_NumberWord7, gUiText_NumberWord6, // Münze, Münzen
    gUiText_NumberWord5, gUiText_NumberWord4, // Runde, Runden
    gUiText_NumberWord3, gUiText_NumberWord2, // Ticket, Tickets
    gUiText_NumberWord1, gUiText_NumberWord0, // Kampf, Käpfe (ROM spelling retained: the m is missing)
    (const u8 *)gUiText_ArticleEmpty, (const u8 *)gUiText_ArticleEmpty,
    (const u8 *)gUiText_ArticleEmpty, (const u8 *)gUiText_ArticleEmpty,
    (const u8 *)gUiText_ArticleEmpty, 0,
};
// clang-format on
#endif

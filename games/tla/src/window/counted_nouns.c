#include "types.h"
#include "edition.h"

//--------------------------------------------------------------------------
// The German counted nouns, singular and plural, by kind.
//--------------------------------------------------------------------------

#if (LANGUAGE == LANGUAGE_GERMAN)
extern const s8 gText_ArticleEmpty[];
extern const u8 gText_NumberWord0[];
extern const u8 gText_NumberWord1[];
extern const u8 gText_NumberWord2[];
extern const u8 gText_NumberWord3[];
extern const u8 gText_NumberWord4[];
extern const u8 gText_NumberWord5[];
extern const u8 gText_NumberWord6[];
extern const u8 gText_NumberWord7[];
extern const u8 gText_NumberWord8[];
extern const u8 gText_NumberWord9[];

/// {singular, plural}: window.c reads [(kind & 7) * 2 + plural]
// clang-format off
const u8 *const gText_NumberWords[16] = {
    gText_NumberWord9, gText_NumberWord8, // Punkt, Punkte
    gText_NumberWord7, gText_NumberWord6, // Münze, Münzen
    gText_NumberWord5, gText_NumberWord4, // Runde, Runden
    gText_NumberWord3, gText_NumberWord2, // Ticket, Tickets
    gText_NumberWord1, gText_NumberWord0, // Kampf, Käpfe (as the ROM spells it)
    (const u8 *)gText_ArticleEmpty, (const u8 *)gText_ArticleEmpty,
    (const u8 *)gText_ArticleEmpty, (const u8 *)gText_ArticleEmpty,
    (const u8 *)gText_ArticleEmpty, 0,
};
// clang-format on
#endif

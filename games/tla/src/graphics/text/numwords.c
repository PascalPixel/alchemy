#include "types.h"
#include "edition.h"

//--------------------------------------------------------------------------
// The German counted nouns, singular and plural, by kind.
//--------------------------------------------------------------------------

#if (LANGUAGE == LANGUAGE_GERMAN)
extern const s8 gUiText_ArticleEmpty[];
extern const u8 gUiText_NumberWord0[];
extern const u8 gUiText_NumberWord1[];
extern const u8 gUiText_NumberWord2[];
extern const u8 gUiText_NumberWord3[];
extern const u8 gUiText_NumberWord4[];
extern const u8 gUiText_NumberWord5[];
extern const u8 gUiText_NumberWord6[];
extern const u8 gUiText_NumberWord7[];
extern const u8 gUiText_NumberWord8[];
extern const u8 gUiText_NumberWord9[];

// {singular, plural}: window.c reads [(kind & 7) * 2 + plural]
// clang-format off
const u8 *const gUiText_NumberWords[16] = {
    gUiText_NumberWord9, gUiText_NumberWord8, // Punkt, Punkte
    gUiText_NumberWord7, gUiText_NumberWord6, // Münze, Münzen
    gUiText_NumberWord5, gUiText_NumberWord4, // Runde, Runden
    gUiText_NumberWord3, gUiText_NumberWord2, // Ticket, Tickets
    gUiText_NumberWord1, gUiText_NumberWord0, // Kampf, Käpfe (as the ROM spells it)
    (const u8 *)gUiText_ArticleEmpty, (const u8 *)gUiText_ArticleEmpty,
    (const u8 *)gUiText_ArticleEmpty, (const u8 *)gUiText_ArticleEmpty,
    (const u8 *)gUiText_ArticleEmpty, 0,
};
// clang-format on
#endif

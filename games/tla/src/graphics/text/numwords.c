#include "types.h"
#include "edition.h"

//--------------------------------------------------------------------------
// The German counted nouns, singular and plural, by kind.
//--------------------------------------------------------------------------

#if defined(TLA_EDITION_DE)
extern const s8 UiText_ArticleEmpty[];
extern const u8 UiText_NumberWord0[];
extern const u8 UiText_NumberWord1[];
extern const u8 UiText_NumberWord2[];
extern const u8 UiText_NumberWord3[];
extern const u8 UiText_NumberWord4[];
extern const u8 UiText_NumberWord5[];
extern const u8 UiText_NumberWord6[];
extern const u8 UiText_NumberWord7[];
extern const u8 UiText_NumberWord8[];
extern const u8 UiText_NumberWord9[];

// {singular, plural}: window.c reads [(kind & 7) * 2 + plural]
const u8 *const UiText_NumberWords[16] =
{
    UiText_NumberWord9, UiText_NumberWord8, // Punkt, Punkte
    UiText_NumberWord7, UiText_NumberWord6, // Münze, Münzen
    UiText_NumberWord5, UiText_NumberWord4, // Runde, Runden
    UiText_NumberWord3, UiText_NumberWord2, // Ticket, Tickets
    UiText_NumberWord1, UiText_NumberWord0, // Kampf, Käpfe (as the ROM spells it)
    (const u8 *)UiText_ArticleEmpty, (const u8 *)UiText_ArticleEmpty,
    (const u8 *)UiText_ArticleEmpty, (const u8 *)UiText_ArticleEmpty,
    (const u8 *)UiText_ArticleEmpty, 0,
};
#endif

#include "unit/graphics/text/article.h"
#include "types.h"

//--------------------------------------------------------------------------
// 数える語
// The German counted nouns, singular and plural, by kind.
//--------------------------------------------------------------------------

#if defined(TBS_EDITION_DE)
// {単数, 複数}: window.c が [(種類 & 7) * 2 + 複数] で読む
const u8 *const UiText_NumberWords[16] =
{
	UiText_NumberWord9, UiText_NumberWord8, // Punkt, Punkte
	UiText_NumberWord7, UiText_NumberWord6, // Münze, Münzen
	UiText_NumberWord5, UiText_NumberWord4, // Runde, Runden
	UiText_NumberWord3, UiText_NumberWord2, // Ticket, Tickets
	UiText_NumberWord1, UiText_NumberWord0, // Kampf, Käpfe
	UiText_ArticleEmpty, UiText_ArticleEmpty,
	UiText_ArticleEmpty, UiText_ArticleEmpty,
	UiText_ArticleEmpty, 0,
};
#endif

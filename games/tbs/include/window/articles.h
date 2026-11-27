#ifndef ALCHEMY_ARTICLES_H
#define ALCHEMY_ARTICLES_H

/* The words messages put before and after a name in each edition (articles,
   possessives, the German counted nouns), the kana marks and the window
   sprites' motion curves (window/articles.c). */

#include "types.h"

/* The grammatical gender of an item's name (LocalizedWord_ItemGenders), in
   German and Italian, which picks its possessive ("your",
   LocalizedWord_ItemPossessives): "dein" or "deine", "il tuo" or "la tua". */
typedef enum ITEM_GENDER {
	ITEM_MASCULINE = 0, /* also the unnamed items' */
	ITEM_FEMININE = 1,
	ITEM_NEUTER = 2 /* German only */
} ITEM_GENDER;

extern const Sint8 Article_Empty[];
#if LANGUAGE == LANGUAGE_GERMAN
extern const Uint8 LocalizedWord_Battles[];
extern const Uint8 LocalizedWord_Battle[];
extern const Uint8 LocalizedWord_Tickets[];
extern const Uint8 LocalizedWord_Ticket[];
extern const Uint8 LocalizedWord_Rounds[];
extern const Uint8 LocalizedWord_Round[];
extern const Uint8 LocalizedWord_Coins[];
extern const Uint8 LocalizedWord_Coin[];
extern const Uint8 LocalizedWord_Points[];
extern const Uint8 LocalizedWord_Point[];
#endif
extern Uint16	   Font_MarkGlyphCodes[];
extern const Uint8 WindowSprite_CursorBobOffsets[80];
extern const Uint8 WindowSprite_WobbleTail[16];

struct ARTICLE_TABLE;

/* The articles a message puts before a name, by article class, in the languages
   that have them. */
extern const struct ARTICLE_TABLE Article_Table;
/* Spanish's indefinite articles, by article class; Article_Table holds its
   definite ones. */
extern const struct ARTICLE_TABLE Article_Indefinite;

#endif

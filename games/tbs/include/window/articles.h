#ifndef ALCHEMY_ARTICLES_H
#define ALCHEMY_ARTICLES_H

// The words messages put before and after a name in each edition (articles, possessives, the German
// counted nouns), the kana marks and the window sprites' motion curves (window/articles.c).

#include "types.h"

// The grammatical gender of an item's name (LocalizedWordItemGenders), in German and Italian, which
// picks its possessive ("your", LocalizedWordItemPossessives): "dein" or "deine", "il tuo" or "la
// tua".
typedef enum ITEM_GENDER
{
    ITEM_MASCULINE = 0, // also the unnamed items'
    ITEM_FEMININE  = 1,
    ITEM_NEUTER    = 2  // German only
} ITEM_GENDER;

extern const Sint8 ArticleEmpty[];
#if LANGUAGE == LANGUAGE_GERMAN
extern const Uint8 LocalizedWordBattles[];
extern const Uint8 LocalizedWordBattle[];
extern const Uint8 LocalizedWordTickets[];
extern const Uint8 LocalizedWordTicket[];
extern const Uint8 LocalizedWordRounds[];
extern const Uint8 LocalizedWordRound[];
extern const Uint8 LocalizedWordCoins[];
extern const Uint8 LocalizedWordCoin[];
extern const Uint8 LocalizedWordPoints[];
extern const Uint8 LocalizedWordPoint[];
#endif
extern Uint16      FontMarkGlyphCodes[];
extern const Uint8 WindowSpriteCursorBobOffsets[80];
extern const Uint8 WindowSpriteWobbleTail[16];

struct ARTICLE_TABLE;

// The articles a message puts before a name, by article class, in the languages that have them.
extern const struct ARTICLE_TABLE ArticleTable;
// Spanish's indefinite articles, by article class; ArticleTable holds its definite ones.
extern const struct ARTICLE_TABLE ArticleIndefinite;

#endif

#ifndef ALCHEMY_ARTICLES_H
#define ALCHEMY_ARTICLES_H

/// @file
/// The words messages put before and after a name in each edition (articles,
/// possessives, the German counted nouns), the kana marks and the window
/// sprites' motion curves (window/articles.c).

#include "types.h"

/// The grammatical gender of an item's name (gLocalizedWord_ItemGenders), in
/// German and Italian, which picks its possessive ("your",
/// gLocalizedWord_ItemPossessives): "dein" or "deine", "il tuo" or "la tua".
enum ItemGender {
  ITEM_MASCULINE = 0, ///< also the unnamed items'
  ITEM_FEMININE = 1,
  ITEM_NEUTER = 2 ///< German only
};

extern const s8 gArticle_Empty[];
#if LANGUAGE == LANGUAGE_GERMAN
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
#endif
extern u16 gFont_MarkGlyphCodes[];
extern const u8 gWindowSprite_CursorBobOffsets[80];
extern const u8 gWindowSprite_WobbleTail[16];

#endif

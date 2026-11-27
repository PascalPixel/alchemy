/* Window text tables: each edition's articles and possessives, the kana marks
   and the sprite motion curves. */

#include "window/text_font.h"

/* Articles and possessives, by edition. */

#if LANGUAGE == LANGUAGE_GERMAN

const Sint8 Article_Empty[] = "";

static const Sint8 sArticleEin[] = "ein ";
static const Sint8 sArticleEine[] = "eine ";
static const Sint8 sArticleDie[] = "die ";
static const Sint8 sArticleDas[] = "das ";

/* The article of each article class. */
const Sint8 *const Article_Table[8] = {
	Article_Empty,
	sArticleEin,
	sArticleEine,
	sArticleDie,
	sArticleDas,
	Article_Empty,
	Article_Empty,
	Article_Empty,
};

/* Counted nouns, each plural before its singular; LocalizedWord_CountedNouns
   (window/counted_nouns.c) pairs them as {singular, plural}. */
const Uint8 LocalizedWord_Battles[] = "K\xe4pfe";
const Uint8 LocalizedWord_Battle[] = "Kampf";
const Uint8 LocalizedWord_Tickets[] = "Tickets";
const Uint8 LocalizedWord_Ticket[] = "Ticket";
const Uint8 LocalizedWord_Rounds[] = "Runden";
const Uint8 LocalizedWord_Round[] = "Runde";
const Uint8 LocalizedWord_Coins[] = "M\xfcnzen";
const Uint8 LocalizedWord_Coin[] = "M\xfcnze";
const Uint8 LocalizedWord_Points[] = "Punkte";
const Uint8 LocalizedWord_Point[] = "Punkt";

static const Sint8 sPossessiveDein[] = "dein ";
static const Sint8 sPossessiveDeine[] = "deine ";

/* The possessive ("your") for each item gender (LocalizedWord_ItemGenders). */
const Sint8 *const LocalizedWord_ItemPossessives[4] = {
	sPossessiveDein,
	sPossessiveDeine,
	sPossessiveDein,
	Article_Empty,
};

#elif LANGUAGE == LANGUAGE_SPANISH

const Sint8 Article_Empty[] = "";

static const Sint8 sArticleEl[] = "el ";
static const Sint8 sArticleLa[] = "la ";
static const Sint8 sArticleLos[] = "los ";
static const Sint8 sArticleLas[] = "las ";

/* The definite article of each article class. */
const Sint8 *const Article_Table[5] = {
	Article_Empty,
	sArticleEl,
	sArticleLa,
	sArticleLos,
	sArticleLas,
};

static const Sint8 sArticleUn[] = "un ";
static const Sint8 sArticleUna[] = "una ";
static const Sint8 sArticleUnos[] = "unos ";
static const Sint8 sArticleUnas[] = "unas ";

/* The indefinite article of each article class. */
const Sint8 *const Article_Indefinite[5] = {
	Article_Empty,
	sArticleUn,
	sArticleUna,
	sArticleUnos,
	sArticleUnas,
};

static const Sint8 sPossessiveTu[] = "tu ";
static const Sint8 sPossessiveTus[] = "tus ";

/* The possessive ("your") for {singular, plural}. */
const Sint8 *const LocalizedWord_ItemPossessives[2] = {
	sPossessiveTu,
	sPossessiveTus,
};

#elif LANGUAGE == LANGUAGE_FRENCH

const Sint8 Article_Empty[] = "";

static const Sint8 sArticleLe[] = "le ";
static const Sint8 sArticleLa[] = "la ";
static const Sint8 sArticleL[] = "l' ";
static const Sint8 sArticleLes[] = "les ";
static const Sint8 sArticleUn[] = "un ";
static const Sint8 sArticleUne[] = "une ";
static const Sint8 sArticleDes[] = "des ";

/* The article of each article class. */
const Sint8 *const Article_Table[8] = {
	Article_Empty,
	sArticleLe,
	sArticleLa,
	sArticleL,
	sArticleLes,
	sArticleUn,
	sArticleUne,
	sArticleDes,
};

const Sint8 LocalizedWord_ItemPronoun[] = "vous ";

#elif LANGUAGE == LANGUAGE_ITALIAN

const Sint8 Article_Empty[] = "";

static const Sint8 sArticleIl[] = "il ";
static const Sint8 sArticleL[] = "l' ";
static const Sint8 sArticleLa[] = "la ";
static const Sint8 sArticleLo[] = "lo ";
static const Sint8 sArticleLe[] = "le ";
static const Sint8 sArticleGli[] = "gli ";
static const Sint8 sArticleUn[] = "un ";
static const Sint8 sArticleUnElided[] = "un' ";
static const Sint8 sArticleUno[] = "uno ";
static const Sint8 sArticleUna[] = "una ";

/* The article of each article class. */
const Sint8 *const Article_Table[11] = {
	Article_Empty,
	sArticleIl,
	sArticleL,
	sArticleLa,
	sArticleLo,
	sArticleLe,
	sArticleGli,
	sArticleUn,
	sArticleUnElided,
	sArticleUno,
	sArticleUna,
};

static const Sint8 sPossessiveIlTuo[] = "Il tuo ";
static const Sint8 sPossessiveLaTua[] = "la tua ";

/* The possessive ("your") for each item gender (LocalizedWord_ItemGenders). */
const Sint8 *const LocalizedWord_ItemPossessives[4] = {
	sPossessiveIlTuo,
	sPossessiveLaTua,
	sPossessiveIlTuo,
	Article_Empty,
};

#elif LANGUAGE != LANGUAGE_JAPANESE

const Sint8 Article_Empty[] = "";

static const Sint8 sArticleA[] = "a ";
static const Sint8 sArticleAn[] = "an ";
static const Sint8 sArticleSome[] = "some ";
static const Sint8 sArticleThe[] = "the ";

/* The article of each article class. */
const Sint8 *const Article_Table[8] = {
	Article_Empty,
	sArticleA,
	sArticleAn,
	sArticleSome,
	Article_Empty,
	sArticleThe,
	Article_Empty,
	Article_Empty,
};

#endif

#if LANGUAGE == LANGUAGE_JAPANESE
/* The glyph drawn over a kana for the mark in its code's top two bits: none,
   the voiced mark (dakuten) or the semi-voiced mark (handakuten). */
/* clang-format off */
Uint16 Font_MarkGlyphCodes[] __attribute__((section(".rodata"))) = {
	TEXT_MARK_NONE,
	TEXT_MARK_VOICED,
	TEXT_MARK_SEMIVOICED,
	TEXT_MARK_NONE
};
#endif

/* The next-page arrow's bob (WINDOW_SPRITE_MOTION_PAGE_ARROW): a smooth rise and fall */
/* of 8 pixels over 80 frames. */
const Uint8 WindowSprite_CursorBobOffsets[80] = {
	0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
	5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
	7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
	7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
	5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

/* A swing along an arc of radius 4, as {x, y} pairs: x runs 0, 4, 0, -4, 0 */
/* while y rises at each side. */
const Sint8 WindowSprite_WobbleOffsets[40] = {
	0, 4, 1, 4, 2, 3, 3, 2,
	4, 1, 4, 0, 4, 1, 3, 2,
	2, 3, 1, 4, 0, 4, -1, 4,
	-2, 3, -3, 2, -4, 1, -4, 0,
	-4, 1, -3, 2, -2, 3, -1, 4,
};

/* Unused: 16 bytes after the wobble that nothing reads. */
const Uint8 WindowSprite_WobbleTail[16] = {
	4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/* A hop of up to 8 pixels over 16 frames, up for WINDOW_SPRITE_MOTION_HOP_UP and down */
/* for the hops down. */
const Uint8 WindowSprite_BounceOffsets[16] = {
	0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
/* clang-format on */

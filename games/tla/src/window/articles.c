// Window text tables: each edition's articles and possessives, the kana marks and the sprite motion
// curves. mnMessageDecode copies an article table whole (ARTICLE_TABLE); each table's words come
// just before it.

#include "graphics/text_font.h"

#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
// clang-format off
// The glyph drawn over a kana for the mark in its code's top two bits: none, the voiced mark (dakuten) or the semi-voiced mark (handakuten).
const Uint16 FontMarkGlyphCodes[] = {
    TEXT_MARK_NONE,
    TEXT_MARK_VOICED,
    TEXT_MARK_SEMIVOICED,
    TEXT_MARK_NONE
};
#endif

// Unused: two fractions of one tenth (2^32 / 10, rounded up); nothing reads them.
const Uint32 UnusedTenthFractions[2] = {0x1999999a, 0x1999999a};

#if (LANGUAGE == LANGUAGE_GERMAN)

const Sint8 ArticleEmpty[] = "";

static const Sint8 ArticleEin[] = "ein ";
static const Sint8 ArticleEine[] = "eine ";
static const Sint8 ArticleDie[] = "die ";
static const Sint8 ArticleDas[] = "das ";

// The article of each article class.
const Sint8 *const ArticleTable[8] = {
    ArticleEmpty,
    ArticleEin,
    ArticleEine,
    ArticleDie,
    ArticleDas,
    ArticleEmpty,
    ArticleEmpty,
    ArticleEmpty,
};

// Counted nouns, each plural before its singular; LocalizedWordCountedNouns (counted_nouns.c) pairs them as {singular, plural}.
const Uint8 LocalizedWordBattles[] = "K\xe4pfe";
const Uint8 LocalizedWordBattle[] = "Kampf";
const Uint8 LocalizedWordTickets[] = "Tickets";
const Uint8 LocalizedWordTicket[] = "Ticket";
const Uint8 LocalizedWordRounds[] = "Runden";
const Uint8 LocalizedWordRound[] = "Runde";
const Uint8 LocalizedWordCoins[] = "M\xfcnzen";
const Uint8 LocalizedWordCoin[] = "M\xfcnze";
const Uint8 LocalizedWordPoints[] = "Punkte";
const Uint8 LocalizedWordPoint[] = "Punkt";

static const Sint8 PossessiveDein[] = "dein ";
static const Sint8 PossessiveDeine[] = "deine ";

// The possessive ("your") for each item gender (LocalizedWordItemGenders).
const Sint8 *const LocalizedWordItemPossessives[4] = {
    PossessiveDein,
    PossessiveDeine,
    PossessiveDein,
    ArticleEmpty,
};

#elif (LANGUAGE == LANGUAGE_SPANISH)

const Sint8 ArticleEmpty[] = "";

static const Sint8 ArticleEl[] = "el ";
static const Sint8 ArticleLa[] = "la ";
static const Sint8 ArticleLos[] = "los ";
static const Sint8 ArticleLas[] = "las ";

// The definite article of each article class.
const Sint8 *const ArticleTable[5] = {
    ArticleEmpty,
    ArticleEl,
    ArticleLa,
    ArticleLos,
    ArticleLas,
};

static const Sint8 ArticleUn[] = "un ";
static const Sint8 ArticleUna[] = "una ";
static const Sint8 ArticleUnos[] = "unos ";
static const Sint8 ArticleUnas[] = "unas ";

// The indefinite article of each article class.
const Sint8 *const ArticleIndefinite[5] = {
    ArticleEmpty,
    ArticleUn,
    ArticleUna,
    ArticleUnos,
    ArticleUnas,
};

static const Sint8 PossessiveTu[] = "tu ";
static const Sint8 PossessiveTus[] = "tus ";

// The possessive ("your") for {singular, plural}.
const Sint8 *const LocalizedWordItemPossessives[2] = {
    PossessiveTu,
    PossessiveTus,
};

#elif (LANGUAGE == LANGUAGE_FRENCH)

static const Sint8 ArticleDeElided[] = "d'";
static const Sint8 ArticleDe[] = "de ";

// "de", elided and whole.
const Sint8 *const TextDeArticles[2] = {
    ArticleDeElided,
    ArticleDe,
};

const Sint8 ArticleEmpty[] = "";

static const Sint8 ArticleLe[] = "le ";
static const Sint8 ArticleLa[] = "la ";
static const Sint8 ArticleL[] = "l' ";
static const Sint8 ArticleLes[] = "les ";
static const Sint8 ArticleUn[] = "un ";
static const Sint8 ArticleUne[] = "une ";
static const Sint8 ArticleDes[] = "des ";

// The article of each article class.
const Sint8 *const ArticleTable[8] = {
    ArticleEmpty,
    ArticleLe,
    ArticleLa,
    ArticleL,
    ArticleLes,
    ArticleUn,
    ArticleUne,
    ArticleDes,
};

const Sint8 LocalizedWordItemPronoun[] = "vous ";

#elif (LANGUAGE == LANGUAGE_ITALIAN)

const Sint8 ArticleEmpty[] = "";

static const Sint8 ArticleIl[] = "il ";
static const Sint8 ArticleL[] = "l' ";
static const Sint8 ArticleLa[] = "la ";
static const Sint8 ArticleLo[] = "lo ";
static const Sint8 ArticleLe[] = "le ";
static const Sint8 ArticleGli[] = "gli ";
static const Sint8 ArticleUn[] = "un ";
static const Sint8 ArticleUnElided[] = "un' ";
static const Sint8 ArticleUno[] = "uno ";
static const Sint8 ArticleUna[] = "una ";

// The article of each article class.
const Sint8 *const ArticleTable[11] = {
    ArticleEmpty,
    ArticleIl,
    ArticleL,
    ArticleLa,
    ArticleLo,
    ArticleLe,
    ArticleGli,
    ArticleUn,
    ArticleUnElided,
    ArticleUno,
    ArticleUna,
};

static const Sint8 PossessiveIlTuo[] = "Il tuo ";
static const Sint8 PossessiveLaTua[] = "la tua ";

// The possessive ("your") for each item gender (LocalizedWordItemGenders).
const Sint8 *const LocalizedWordItemPossessives[4] = {
    PossessiveIlTuo,
    PossessiveLaTua,
    PossessiveIlTuo,
    ArticleEmpty,
};

#elif (LANGUAGE == LANGUAGE_ENGLISH)

const Sint8 ArticleEmpty[] = "";

static const Sint8 ArticleA[] = "a ";
static const Sint8 ArticleAn[] = "an ";
static const Sint8 ArticleSome[] = "some ";
static const Sint8 ArticleThe[] = "the ";

// The article of each article class.
const Sint8 *const ArticleTable[8] = {
    ArticleEmpty,
    ArticleA,
    ArticleAn,
    ArticleSome,
    ArticleEmpty,
    ArticleThe,
    ArticleEmpty,
    ArticleEmpty,
};

#endif

// Sprite motion.

// The next-page arrow's bob (WINDOW_SPRITE_MOTION_PAGE_ARROW): a smooth rise and fall of 8 pixels over 80 ticks.
const Uint8 WindowSpriteCursorBobOffsets[80] = {
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
    7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
    7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

// A swing along an arc of radius 4, as {x, y} pairs: x runs 0, 4, 0, -4, 0 while y rises at each side.
const Sint8 WindowSpriteWobbleOffsets[40] = {
    0, 4, 1, 4, 2, 3, 3, 2,
    4, 1, 4, 0, 4, 1, 3, 2,
    2, 3, 1, 4, 0, 4, -1, 4,
    -2, 3, -3, 2, -4, 1, -4, 0,
    -4, 1, -3, 2, -2, 3, -1, 4,
};

// Nothing in the game reads these 16 bytes.
const Uint8 WindowSpriteWobbleTail[16] = {
    4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// A hop of up to 8 pixels over 16 ticks, for WINDOW_SPRITE_MOTION_HOP_UP and the other hops.
const Uint8 WindowSpriteBounceOffsets[16] = {
    0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
// clang-format on

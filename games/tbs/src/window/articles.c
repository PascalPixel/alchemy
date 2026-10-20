/// @file
/// Window text tables: each edition's articles and possessives, the kana
/// marks and the sprite motion curves.

#include "unit/window/articles.h"
#include "types.h"
#include "edition.h"
#include "text_font.h"

// Articles and possessives, by edition. An item or name carries an article
// class (the {article_class:N} code of its message), and the message builder
// (UiText_AppendArticleName in window/decode.c) copies the class's word in
// front of the name. The decoder copies each article table whole as a struct
// ArticleTable and each possessive table as a struct ItemPossessiveTable;
// French's one pronoun is a plain string. Each table's words come before it in
// .rodata.

#if (LANGUAGE == LANGUAGE_GERMAN)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleEin[] = "ein ";
static const s8 sArticleEine[] = "eine ";
static const s8 sArticleDie[] = "die ";
static const s8 sArticleDas[] = "das ";

/// The article of each article class.
const s8 *const gUiText_ArticleTable[8] = {
    gUiText_ArticleEmpty, sArticleEin,          sArticleEine,
    sArticleDie,          sArticleDas,          gUiText_ArticleEmpty,
    gUiText_ArticleEmpty, gUiText_ArticleEmpty,
};

/// Counted nouns, each plural before its singular; gUiText_NumberWords
/// (graphics/text/numwords.c) pairs them as {singular, plural}. "K\xe4pfe" is
/// the ROM's spelling of K\xe4mpfe.
const u8 gUiText_NumberWord0[] = "K\xe4pfe";
const u8 gUiText_NumberWord1[] = "Kampf";
const u8 gUiText_NumberWord2[] = "Tickets";
const u8 gUiText_NumberWord3[] = "Ticket";
const u8 gUiText_NumberWord4[] = "Runden";
const u8 gUiText_NumberWord5[] = "Runde";
const u8 gUiText_NumberWord6[] = "M\xfcnzen";
const u8 gUiText_NumberWord7[] = "M\xfcnze";
const u8 gUiText_NumberWord8[] = "Punkte";
const u8 gUiText_NumberWord9[] = "Punkt";

static const s8 sPossessiveDein[] = "dein ";
static const s8 sPossessiveDeine[] = "deine ";

/// The possessive ("your") for each item gender (gUiText_ItemGenders).
const s8 *const gUiText_ItemPossessives[4] = {
    sPossessiveDein,
    sPossessiveDeine,
    sPossessiveDein,
    gUiText_ArticleEmpty,
};

#elif (LANGUAGE == LANGUAGE_SPANISH)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleEl[] = "el ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleLos[] = "los ";
static const s8 sArticleLas[] = "las ";

/// The definite article of each article class.
const s8 *const gUiText_ArticleTable[5] = {
    gUiText_ArticleEmpty, sArticleEl, sArticleLa, sArticleLos, sArticleLas,
};

static const s8 sArticleUn[] = "un ";
static const s8 sArticleUna[] = "una ";
static const s8 sArticleUnos[] = "unos ";
static const s8 sArticleUnas[] = "unas ";

/// The indefinite article of each article class.
const s8 *const gUiText_IndefiniteArticles[5] = {
    gUiText_ArticleEmpty, sArticleUn, sArticleUna, sArticleUnos, sArticleUnas,
};

static const s8 sPossessiveTu[] = "tu ";
static const s8 sPossessiveTus[] = "tus ";

/// The possessive ("your") for {singular, plural}.
const s8 *const gUiText_ItemPossessives[2] = {
    sPossessiveTu,
    sPossessiveTus,
};

#elif (LANGUAGE == LANGUAGE_FRENCH)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleLe[] = "le ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleL[] = "l' ";
static const s8 sArticleLes[] = "les ";
static const s8 sArticleUn[] = "un ";
static const s8 sArticleUne[] = "une ";
static const s8 sArticleDes[] = "des ";

/// The article of each article class.
const s8 *const gUiText_ArticleTable[8] = {
    gUiText_ArticleEmpty, sArticleLe, sArticleLa,  sArticleL,
    sArticleLes,          sArticleUn, sArticleUne, sArticleDes,
};

const s8 gUiText_ItemPronoun[] = "vous ";

#elif (LANGUAGE == LANGUAGE_ITALIAN)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleIl[] = "il ";
static const s8 sArticleL[] = "l' ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleLo[] = "lo ";
static const s8 sArticleLe[] = "le ";
static const s8 sArticleGli[] = "gli ";
static const s8 sArticleUn[] = "un ";
static const s8 sArticleUnElided[] = "un' ";
static const s8 sArticleUno[] = "uno ";
static const s8 sArticleUna[] = "una ";

/// The article of each article class.
const s8 *const gUiText_ArticleTable[11] = {
    gUiText_ArticleEmpty, sArticleIl,  sArticleL,   sArticleLa,
    sArticleLo,           sArticleLe,  sArticleGli, sArticleUn,
    sArticleUnElided,     sArticleUno, sArticleUna,
};

static const s8 sPossessiveIlTuo[] = "Il tuo ";
static const s8 sPossessiveLaTua[] = "la tua ";

/// The possessive ("your") for each item gender (gUiText_ItemGenders).
const s8 *const gUiText_ItemPossessives[4] = {
    sPossessiveIlTuo,
    sPossessiveLaTua,
    sPossessiveIlTuo,
    gUiText_ArticleEmpty,
};

#elif (LANGUAGE != LANGUAGE_JAPANESE)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleA[] = "a ";
static const s8 sArticleAn[] = "an ";
static const s8 sArticleSome[] = "some ";
static const s8 sArticleThe[] = "the ";

/// The article of each article class.
const s8 *const gUiText_ArticleTable[8] = {
    gUiText_ArticleEmpty, sArticleA,
    sArticleAn,           sArticleSome,
    gUiText_ArticleEmpty, sArticleThe,
    gUiText_ArticleEmpty, gUiText_ArticleEmpty,
};

#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The glyph drawn over a kana for the mark in its code's top two bits: none,
/// the voiced mark (dakuten) or the semi-voiced mark (handakuten).
// clang-format off
const u16 gUiText_MarkGlyphCodes[] = {
    TEXT_MARK_NONE,
    TEXT_MARK_VOICED,
    TEXT_MARK_SEMIVOICED,
    TEXT_MARK_NONE
};
#endif

/// The next-page arrow's bob (UI_SPRITE_PAGE_ARROW): a smooth rise and fall
/// of 8 pixels over 80 frames.
const u8 gUiSprite_CursorBobOffsets[80] = {
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
    7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
    7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

/// A swing along an arc of radius 4, as {x, y} pairs: x runs 0, 4, 0, -4, 0
/// while y rises at each side. UI_SPRITE_WOBBLE takes one of its 20 steps
/// every other frame.
const s8 gUiSprite_WobbleOffsets[40] = {
    0, 4, 1, 4, 2, 3, 3, 2,
    4, 1, 4, 0, 4, 1, 3, 2,
    2, 3, 1, 4, 0, 4, -1, 4,
    -2, 3, -3, 2, -4, 1, -4, 0,
    -4, 1, -3, 2, -2, 3, -1, 4,
};

/// Nothing in the game reads these 16 bytes.
const u8 gUiSprite_WobbleTail[16] = {
    4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/// A hop of up to 8 pixels over 16 frames, up for UI_SPRITE_HOP_UP and down
/// for the hops down.
const u8 gUiSprite_BounceOffsets[16] = {
    0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
// clang-format on

/// @file
/// Window text tables: each edition's articles and possessives, the kana
/// marks and the sprite motion curves.

#include "module/window/articles.h"
#include "types.h"
#include "edition.h"
#include "text_font.h"

// Articles and possessives, by edition. An item or name carries an article
// class (the {article_class:N} code of its message), and the message builder
// (Message_AppendArticleName in window/decode.c) copies the class's word in
// front of the name. The decoder copies each article table whole as a struct
// ArticleTable and each possessive table as a struct ItemPossessiveTable;
// French's one pronoun is a plain string. Each table's words come before it in
// .rodata.

#if (LANGUAGE == LANGUAGE_GERMAN)

const s8 gArticle_Empty[] = "";

static const s8 sArticleEin[] = "ein ";
static const s8 sArticleEine[] = "eine ";
static const s8 sArticleDie[] = "die ";
static const s8 sArticleDas[] = "das ";

/// The article of each article class.
const s8 *const gArticle_Table[8] = {
    gArticle_Empty, sArticleEin,    sArticleEine,   sArticleDie,
    sArticleDas,    gArticle_Empty, gArticle_Empty, gArticle_Empty,
};

/// Counted nouns, each plural before its singular; gLocalizedWord_CountedNouns
/// (window/counted_nouns.c) pairs them as {singular, plural}. "K\xe4pfe" is
/// the ROM's spelling of K\xe4mpfe.
const u8 gLocalizedWord_Battles[] = "K\xe4pfe";
const u8 gLocalizedWord_Battle[] = "Kampf";
const u8 gLocalizedWord_Tickets[] = "Tickets";
const u8 gLocalizedWord_Ticket[] = "Ticket";
const u8 gLocalizedWord_Rounds[] = "Runden";
const u8 gLocalizedWord_Round[] = "Runde";
const u8 gLocalizedWord_Coins[] = "M\xfcnzen";
const u8 gLocalizedWord_Coin[] = "M\xfcnze";
const u8 gLocalizedWord_Points[] = "Punkte";
const u8 gLocalizedWord_Point[] = "Punkt";

static const s8 sPossessiveDein[] = "dein ";
static const s8 sPossessiveDeine[] = "deine ";

/// The possessive ("your") for each item gender (gLocalizedWord_ItemGenders).
const s8 *const gLocalizedWord_ItemPossessives[4] = {
    sPossessiveDein,
    sPossessiveDeine,
    sPossessiveDein,
    gArticle_Empty,
};

#elif (LANGUAGE == LANGUAGE_SPANISH)

const s8 gArticle_Empty[] = "";

static const s8 sArticleEl[] = "el ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleLos[] = "los ";
static const s8 sArticleLas[] = "las ";

/// The definite article of each article class.
const s8 *const gArticle_Table[5] = {
    gArticle_Empty, sArticleEl, sArticleLa, sArticleLos, sArticleLas,
};

static const s8 sArticleUn[] = "un ";
static const s8 sArticleUna[] = "una ";
static const s8 sArticleUnos[] = "unos ";
static const s8 sArticleUnas[] = "unas ";

/// The indefinite article of each article class.
const s8 *const gArticle_Indefinite[5] = {
    gArticle_Empty, sArticleUn, sArticleUna, sArticleUnos, sArticleUnas,
};

static const s8 sPossessiveTu[] = "tu ";
static const s8 sPossessiveTus[] = "tus ";

/// The possessive ("your") for {singular, plural}.
const s8 *const gLocalizedWord_ItemPossessives[2] = {
    sPossessiveTu,
    sPossessiveTus,
};

#elif (LANGUAGE == LANGUAGE_FRENCH)

const s8 gArticle_Empty[] = "";

static const s8 sArticleLe[] = "le ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleL[] = "l' ";
static const s8 sArticleLes[] = "les ";
static const s8 sArticleUn[] = "un ";
static const s8 sArticleUne[] = "une ";
static const s8 sArticleDes[] = "des ";

/// The article of each article class.
const s8 *const gArticle_Table[8] = {
    gArticle_Empty, sArticleLe, sArticleLa,  sArticleL,
    sArticleLes,    sArticleUn, sArticleUne, sArticleDes,
};

const s8 gLocalizedWord_ItemPronoun[] = "vous ";

#elif (LANGUAGE == LANGUAGE_ITALIAN)

const s8 gArticle_Empty[] = "";

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
const s8 *const gArticle_Table[11] = {
    gArticle_Empty,   sArticleIl,  sArticleL,   sArticleLa,
    sArticleLo,       sArticleLe,  sArticleGli, sArticleUn,
    sArticleUnElided, sArticleUno, sArticleUna,
};

static const s8 sPossessiveIlTuo[] = "Il tuo ";
static const s8 sPossessiveLaTua[] = "la tua ";

/// The possessive ("your") for each item gender (gLocalizedWord_ItemGenders).
const s8 *const gLocalizedWord_ItemPossessives[4] = {
    sPossessiveIlTuo,
    sPossessiveLaTua,
    sPossessiveIlTuo,
    gArticle_Empty,
};

#elif (LANGUAGE != LANGUAGE_JAPANESE)

const s8 gArticle_Empty[] = "";

static const s8 sArticleA[] = "a ";
static const s8 sArticleAn[] = "an ";
static const s8 sArticleSome[] = "some ";
static const s8 sArticleThe[] = "the ";

/// The article of each article class.
const s8 *const gArticle_Table[8] = {
    gArticle_Empty, sArticleA,   sArticleAn,     sArticleSome,
    gArticle_Empty, sArticleThe, gArticle_Empty, gArticle_Empty,
};

#endif

#if (LANGUAGE == LANGUAGE_JAPANESE)
/// The glyph drawn over a kana for the mark in its code's top two bits: none,
/// the voiced mark (dakuten) or the semi-voiced mark (handakuten).
/// Not const, and placed in read-only data by its section: through a const
/// extern, its load in Text_DrawGlyphPair is scheduled ahead of the spill of
/// out (2 of 185 instructions differ in tbs-ja).
// clang-format off
u16 gFont_MarkGlyphCodes[] __attribute__((section(".rodata"))) = {
    TEXT_MARK_NONE,
    TEXT_MARK_VOICED,
    TEXT_MARK_SEMIVOICED,
    TEXT_MARK_NONE
};
#endif

/// The next-page arrow's bob (WINDOW_SPRITE_MOTION_PAGE_ARROW): a smooth rise and fall
/// of 8 pixels over 80 frames.
const u8 gWindowSprite_CursorBobOffsets[80] = {
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
    7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
    7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

/// A swing along an arc of radius 4, as {x, y} pairs: x runs 0, 4, 0, -4, 0
/// while y rises at each side. WINDOW_SPRITE_MOTION_WOBBLE takes one of its 20 steps
/// every other frame.
const s8 gWindowSprite_WobbleOffsets[40] = {
    0, 4, 1, 4, 2, 3, 3, 2,
    4, 1, 4, 0, 4, 1, 3, 2,
    2, 3, 1, 4, 0, 4, -1, 4,
    -2, 3, -3, 2, -4, 1, -4, 0,
    -4, 1, -3, 2, -2, 3, -1, 4,
};

/// Nothing in the game reads these 16 bytes.
const u8 gWindowSprite_WobbleTail[16] = {
    4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

/// A hop of up to 8 pixels over 16 frames, up for WINDOW_SPRITE_MOTION_HOP_UP and down
/// for the hops down.
const u8 gWindowSprite_BounceOffsets[16] = {
    0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
// clang-format on

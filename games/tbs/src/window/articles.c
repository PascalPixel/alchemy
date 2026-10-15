#include "unit/window/articles.h"
#include "types.h"
#include "edition.h"
#include "text_font.h"

//--------------------------------------------------------------------------
// 文字と絵の表: 冠詞, 濁点, カーソルと揺れ
// Window tables: each edition's articles, the kana marks and sprite motion.
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// 冠詞と所有の語 (言語ごと)
//--------------------------------------------------------------------------

// window.c は冠詞の表を struct ArticleTable (s8 *text[種類の数]) として
// まるごと写して読む。どの表も ROM の .rodata にある。語はそれぞれの表の
// 前に並ぶ。

#if defined(TBS_EDITION_DE)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleEin[] = "ein ";
static const s8 sArticleEine[] = "eine ";
static const s8 sArticleDie[] = "die ";
static const s8 sArticleDas[] = "das ";

// 種類ごとの冠詞
const s8 *const gUiText_ArticleTable[8] = {
    gUiText_ArticleEmpty,
    sArticleEin,
    sArticleEine,
    sArticleDie,
    sArticleDas,
    gUiText_ArticleEmpty,
    gUiText_ArticleEmpty,
    gUiText_ArticleEmpty,
};

// 数える語 (gUiText_NumberWords が {単数, 複数} に並べる)
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

// 道具の性 (gUiText_ItemGenders) ごとの所有の語
const s8 *const gUiText_ItemPossessives[4] = {
    sPossessiveDein,
    sPossessiveDeine,
    sPossessiveDein,
    gUiText_ArticleEmpty,
};

#elif defined(TBS_EDITION_ES)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleEl[] = "el ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleLos[] = "los ";
static const s8 sArticleLas[] = "las ";

// 種類ごとの定冠詞
const s8 *const gUiText_ArticleTable[5] = {
    gUiText_ArticleEmpty,
    sArticleEl,
    sArticleLa,
    sArticleLos,
    sArticleLas,
};

static const s8 sArticleUn[] = "un ";
static const s8 sArticleUna[] = "una ";
static const s8 sArticleUnos[] = "unos ";
static const s8 sArticleUnas[] = "unas ";

// 種類ごとの不定冠詞
const s8 *const gUiText_IndefiniteArticles[5] = {
    gUiText_ArticleEmpty,
    sArticleUn,
    sArticleUna,
    sArticleUnos,
    sArticleUnas,
};

static const s8 sPossessiveTu[] = "tu ";
static const s8 sPossessiveTus[] = "tus ";

// {単数, 複数} の所有の語
const s8 *const gUiText_ItemPossessives[2] = {
    sPossessiveTu,
    sPossessiveTus,
};

#elif defined(TBS_EDITION_FR)

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleLe[] = "le ";
static const s8 sArticleLa[] = "la ";
static const s8 sArticleL[] = "l' ";
static const s8 sArticleLes[] = "les ";
static const s8 sArticleUn[] = "un ";
static const s8 sArticleUne[] = "une ";
static const s8 sArticleDes[] = "des ";

// 種類ごとの冠詞
const s8 *const gUiText_ArticleTable[8] = {
    gUiText_ArticleEmpty,
    sArticleLe,
    sArticleLa,
    sArticleL,
    sArticleLes,
    sArticleUn,
    sArticleUne,
    sArticleDes,
};

const s8 gUiText_ItemPronoun[] = "vous ";

#elif defined(TBS_EDITION_IT)

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

// 種類ごとの冠詞
const s8 *const gUiText_ArticleTable[11] = {
    gUiText_ArticleEmpty,
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

static const s8 sPossessiveIlTuo[] = "Il tuo ";
static const s8 sPossessiveLaTua[] = "la tua ";

// 道具の性 (gUiText_ItemGenders) ごとの所有の語
const s8 *const gUiText_ItemPossessives[4] = {
    sPossessiveIlTuo,
    sPossessiveLaTua,
    sPossessiveIlTuo,
    gUiText_ArticleEmpty,
};

#elif EDITION_INTERNATIONAL

const s8 gUiText_ArticleEmpty[] = "";

static const s8 sArticleA[] = "a ";
static const s8 sArticleAn[] = "an ";
static const s8 sArticleSome[] = "some ";
static const s8 sArticleThe[] = "the ";

// 種類ごとの冠詞
const s8 *const gUiText_ArticleTable[8] = {
    gUiText_ArticleEmpty,
    sArticleA,
    sArticleAn,
    sArticleSome,
    gUiText_ArticleEmpty,
    sArticleThe,
    gUiText_ArticleEmpty,
    gUiText_ArticleEmpty,
};

#endif

//--------------------------------------------------------------------------
// 濁点とスプライトの動き
//--------------------------------------------------------------------------

#if !EDITION_INTERNATIONAL
// clang-format off
const u16 gUiText_MarkGlyphCodes[] = {
    TEXT_MARK_NONE,
    TEXT_MARK_VOICED,
    TEXT_MARK_SEMIVOICED,
    TEXT_MARK_NONE
};
#endif

// カーソルの上下
const u8 gUiSprite_CursorBobOffsets[80] = {
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
    5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7,
    7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 7, 7,
    7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5,
    5, 5, 4, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1,
};

// 半径 4 の円
const s8 gUiSprite_WobbleOffsets[40] = {
    0, 4, 1, 4, 2, 3, 3, 2,
    4, 1, 4, 0, 4, 1, 3, 2,
    2, 3, 1, 4, 0, 4, -1, 4,
    -2, 3, -3, 2, -4, 1, -4, 0,
    -4, 1, -3, 2, -2, 3, -1, 4,
};

const u8 gUiSprite_WobbleTail[16] = {
    4, 3, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

// 弾み
const u8 gUiSprite_BounceOffsets[16] = {
    0, 1, 1, 2, 3, 4, 6, 8, 6, 4, 3, 2, 1, 1, 0, 0,
};
// clang-format on
